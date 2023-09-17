//
// Created by zhushuo on 2023/9/16.
//

#include "thread_pool.h"

#include <condition_variable>
#include <deque>
#include <algorithm>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "cancel.h"
#include "io_util.h"
#include "marcos.h"

namespace arrow
{
Executor::~Executor() = default;

namespace {
    struct Task{
        internal::FnOnce<void()> callable;
        StopToken stop_token;
        Executor::StopCallback stop_callback;
    };

}

struct ThreadPool::State
{
    State() = default;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::condition_variable m_cv_shutdown;
    std::condition_variable m_cv_idle;

    std::list<std::thread> m_workers;
    std::vector<std::thread> m_finished_workers;
    std::deque<Task> m_pending_tasks;

    int m_desired_capacity = 0;
    int m_tasks_queued_or_running = 0;
    bool m_please_shutdown = false;
    bool m_quick_shutdown = false;
};

static void worker_loop(std::shared_ptr<ThreadPool::State> state,
                        std::list<std::thread>::iterator it)
{
    std::unique_lock<std::mutex> lock(state->m_mutex);
    DCHECK_EQ(std::this_thread::get_id(), it->get_id());

    const auto should_secede = [&]()->bool
    {
        return state->m_workers.size() > static_cast<size_t> (state->m_desired_capacity);

    };

    while(true)
    {
        while (!state->m_pending_tasks.empty() && !state->m_quick_shutdown)
        {
            if(should_secede())
            {
                break;
            }


            DCHECK_GE(state->m_tasks_queued_or_running, 0);

            {
                Task task = std::move(state->m_pending_tasks.front());
                state->m_pending_tasks.pop_front();
                StopToken* pt_stop_token = &task.stop_token;
                lock.unlock();
                if(!pt_stop_token->is_stop_requested())
                {
                    std::move(task.callable)();
                }
                else
                {
                    if(task.stop_callback)
                    {
                        std::move(task.stop_callback)(pt_stop_token->poll());
                    }
                }

                ARROW_UNUSED(std::move(task));
                lock.lock();
            }
            if(ARROW_PREDICT_FALSE(--state->m_tasks_queued_or_running == 0))
            {
                state->m_cv_idle.notify_all();
            }
        }

        if (state->m_please_shutdown || should_secede())
        {
            break;
        }

        state->m_cv.wait(lock);
    }

    DCHECK_GE(state->m_tasks_queued_or_running, 0);

    DCHECK_EQ(std::this_thread::get_id(), it->get_id());

    state->m_finished_workers.push_back(std::move(*it));
    state->m_workers.erase(it);
    if(state->m_please_shutdown)
    {
        state->m_cv_shutdown.notify_one();
    }
}

void ThreadPool::wait_for_idle()
{
    std::unique_lock<std::mutex> lk(m_state->m_mutex);
    m_state->m_cv_idle.wait(lk, [this] {return m_state->m_tasks_queued_or_running == 0;});
}

ThreadPool::ThreadPool() : m_sp_state(std::make_shared<ThreadPool::State>()),
                            m_state(m_sp_state.get()),
                            m_shutdown_on_destroy(true)
{
    m_pid = getpid();
}

ThreadPool::~ThreadPool() {
    if(m_shutdown_on_destroy)
    {
        ARROW_UNUSED(shut_down(false));
    }
}

void ThreadPool::protect_against_fork() {
    pid_t current_pid = getpid();
    if(m_pid != current_pid)
    {
        int capacity = m_state->m_desired_capacity;
        auto new_state = std::make_shared<ThreadPool::State>();
        new_state->m_please_shutdown = m_state->m_please_shutdown;
        new_state->m_quick_shutdown = m_state->m_quick_shutdown;
        m_pid = current_pid;
        m_sp_state = new_state;
        m_state = m_sp_state.get();
        if(!m_state->m_please_shutdown)
        {
            ARROW_UNUSED(set_capacity(capacity));
        }
    }
}

Status ThreadPool::set_capacity(int threads) {
    protect_against_fork();
    std::unique_lock<std::mutex> lock(m_state->m_mutex);
    if(m_state->m_please_shutdown)
    {
        return  Status::make_invalid("operation forbidden during or after shutdown");
    }

    collect_finish_workers_unlocked();

    m_state->m_desired_capacity = threads;

    const int required = std::min(static_cast<int>(m_state->m_pending_tasks.size()),threads - static_cast<int>(m_state->m_workers.size()));

    if(required>0)
    {
        launch_workers_unlocked(required);
    }
    else if(required < 0)
    {
        m_state->m_cv.notify_all();
    }

    return Status::make_ok();

}

int ThreadPool::get_capacity() {
    protect_against_fork();
    std::unique_lock<std::mutex> lock(m_state->m_mutex);
    return m_state->m_desired_capacity;

}

int ThreadPool::get_num_tasks() {
    protect_against_fork();
    std::unique_lock<std::mutex> lock(m_state->m_mutex);
    return m_state->m_tasks_queued_or_running;

}

int ThreadPool::get_actual_capacity() {
    protect_against_fork();
    std::unique_lock<std::mutex> lock(m_state->m_mutex);
    return static_cast<int>(m_state->m_workers.size());

}

Status ThreadPool::shut_down(bool wait) {
    protect_against_fork();
    std::unique_lock<std::mutex> lock(m_state->m_mutex);
    if(m_state->m_please_shutdown)
    {
        return Status::make_invalid("ShutDown already called");
    }

    m_state->m_please_shutdown = true;
    m_state->m_quick_shutdown = !wait;
    m_state->m_cv.notify_all();
    m_state->m_cv_shutdown.wait(lock, [this]{return m_state->m_workers.empty();});
    if(!m_state->m_quick_shutdown)
    {
        DCHECK_EQ(m_state->m_pending_tasks.size(), 0);
    }
    else
    {
        m_state->m_pending_tasks.clear();
    }

    collect_finish_workers_unlocked();
     return Status::make_ok();


}

void ThreadPool::collect_finish_workers_unlocked() {
    for(auto& thread: m_state->m_finished_workers)
    {
        thread.join();
    }

    m_state->m_finished_workers.clear();
}

thread_local ThreadPool* current_thread_pool_ = nullptr;

bool ThreadPool::owns_this_thread() {
    return current_thread_pool_ == this;
}

void ThreadPool::launch_workers_unlocked(int threads) {
    std::shared_ptr<State> state = m_sp_state;

    for(int i = 0; i < threads; i++)
    {
        m_state->m_workers.emplace_back();
        auto it = --(m_state->m_workers.end());
        *it = std::thread([this, state, it]
                          {
                            current_thread_pool_ = this;
                              worker_loop(state, it);
                          });
    }
}

Status ThreadPool::spawn_real(TaskHints hints, internal::FnOnce<void()> task, StopToken stop_token, StopCallback && stop_callback)
{
    {
        protect_against_fork();
        std::lock_guard<std::mutex> lock(m_state->m_mutex);
        if (m_state->m_please_shutdown) {
            return Status::make_invalid("operation forbidden during or after shutdown");
        }

        collect_finish_workers_unlocked();

        m_state->m_tasks_queued_or_running++;
        if (static_cast<int>(m_state->m_workers.size()) < m_state->m_tasks_queued_or_running &&
            m_state->m_desired_capacity > static_cast<int>(m_state->m_workers.size())) {
            // We can still spin up more workers so spin up a new worker
            launch_workers_unlocked(/*threads=*/1);
        }
        m_state->m_pending_tasks.push_back(
                {std::move(task), std::move(stop_token), std::move(stop_callback)});
    }

    m_state->m_cv.notify_one();
    return Status::make_ok();
}

std::optional<std::shared_ptr<ThreadPool>> ThreadPool::make(int threads) {
    auto pool = std::shared_ptr<ThreadPool>(new ThreadPool());
    DCHECK_OK(pool->set_capacity(threads));
    return pool;
}

std::optional<std::shared_ptr<ThreadPool>> ThreadPool::make_external(int threads) {
    auto pool = make(threads);
    return pool;
}


static int ParseOMPEnvVar(const char* name) {
    // OMP_NUM_THREADS is a comma-separated list of positive integers.
    // We are only interested in the first (top-level) number.
    auto result = GetEnvVar(name);
    if (!result.has_value()) {
        return 0;
    }
    auto str = *std::move(result);
    auto first_comma = str.find_first_of(',');
    if (first_comma != std::string::npos) {
        str = str.substr(0, first_comma);
    }
    try {
        return std::max(0, std::stoi(str));
    } catch (...) {
        return 0;
    }
}

int ThreadPool::default_capacity()  {
    int capacity, limit;
    capacity = ParseOMPEnvVar("OMP_NUM_THREADS");
    if (capacity == 0) {
        capacity = std::thread::hardware_concurrency();
    }
    //std::cout<<capacity<<std::endl;
    limit = ParseOMPEnvVar("OMP_THREAD_LIMIT");
    if (limit > 0) {
        capacity = std::min(limit, capacity);
    }
    if (capacity == 0) {
        std::cout << "Failed to determine the number of available threads, "
                     "using a hardcoded arbitrary value";
        capacity = 4;
    }
    return capacity;
}

// Helper for the singleton pattern
std::shared_ptr<ThreadPool> ThreadPool::make_cpu_thread_pool() {
    auto maybe_pool = ThreadPool::make_external(ThreadPool::default_capacity());
    if (!maybe_pool.has_value()) {
        Status().abort("Failed to create global CPU thread pool");
    }
    return *std::move(maybe_pool);
}

ThreadPool* get_cpu_thread_pool() {
    static std::shared_ptr<ThreadPool> singleton = ThreadPool::make_cpu_thread_pool();
    return singleton.get();
}

int get_cpu_thread_pool_capacity() { return get_cpu_thread_pool()->get_capacity(); }

Status set_cpu_thread_pool_capacity(int threads) {
    return get_cpu_thread_pool()->set_capacity(threads);
}

}




