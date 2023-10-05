//
// Created by zhushuo on 2023/10/4.
//

#include <assert.h>
#include "my_simple_timer.h"

//用来存储1个任务
struct TimedTask
{
    TimedTask(const simple_timer::MySimpleTimer::task_t & task, int time_out);
    simple_timer::MySimpleTimer::task_t m_task;
    int m_time_out;
    long long m_deadline;

};

TimedTask::TimedTask(const simple_timer::MySimpleTimer::task_t & task, int time_out)
:m_task(task), m_time_out(time_out), m_deadline(0)
{

}

namespace simple_timer
{
MySimpleTimer::MySimpleTimer():m_stop(false), m_tick_count(0), m_thread_pool(2)
{
    m_start_time = std::chrono::steady_clock::now();
    m_thread = std::thread(&MySimpleTimer::thread_routine, this); //开个线程
}

MySimpleTimer::~MySimpleTimer()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
    }

    m_cond.notify_one(); //通知m_thread, m_stop变成false了
    m_thread.join(); //等待
}

void MySimpleTimer::add_task(const task_t &task, int msec) {
    if(msec <= 0)
    {
        m_thread_pool.commit(task); //直接放到线程池去执行
        return;
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_input_tasks.push_back(new TimedTask(task, msec));
    }

    m_cond.notify_one();
}

void MySimpleTimer::tick(long long tick_count)
{
    const int index = tick_count & INDEX_MASK;
    auto &task_list = m_timed_task[index];
    for (auto iter = task_list.begin(); iter != task_list.end();)
    {
        auto tm_task = *iter;
        if (tick_count >= tm_task->m_deadline) {
            m_thread_pool.commit(tm_task->m_task); //到时间了就提交
            iter = task_list.erase(iter);
            delete tm_task;
            continue;
        }

        break;
    }
}

void MySimpleTimer::thread_routine()
{
    while(!m_stop)
    {
        long long last_mill_sec = m_tick_count;
        int wait_mill_sec = get_next_wait_ms();
        assert(wait_mill_sec);
        std::vector<TimedTask*> task_list;
        std::cv_status status = std::cv_status::no_timeout;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (!m_stop && m_input_tasks.size() == 0 && status ==std::cv_status::no_timeout )
            {
                if(wait_mill_sec > 0)
                {
                    status = m_cond.wait_for(lock, std::chrono::milliseconds(wait_mill_sec));
                }
                else
                {
                    m_cond.wait(lock);
                }

            }

            if(m_stop)
            {
                return;
            }

            if(m_input_tasks.size() > 0)
            {
                task_list.swap(m_input_tasks);
            }
        }

        m_tick_count = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_start_time).count();
        long long dt = m_tick_count - last_mill_sec;
        if(dt >= wait_mill_sec && wait_mill_sec != -1)
        {
            for(int i = 0; i < dt - wait_mill_sec; ++i)
            {
                tick(last_mill_sec + wait_mill_sec + i);
            }
        }

        if(task_list.size() >0)
        {
            add_new_task(task_list);
        }
    }
}

int MySimpleTimer::get_next_wait_ms()
{
    int min_ms = -1;
    bool  find_next = false;
    const int start_index = m_tick_count & INDEX_MASK; //mask是
    for(int i = 1; i< WHEEL_LEN; ++i)
    {
        const int index = (start_index + i) & INDEX_MASK;
        auto& tasks = m_timed_task[index];
        if(tasks.size() > 0)
        {
            long long dt = tasks.front()->m_deadline - m_tick_count;
            if(find_next)
            {
                if(dt < min_ms)
                {
                    min_ms = dt;
                }

            }
            else
            {
                assert(dt > 0);
                min_ms = dt;
                find_next = true;
            }

            if(min_ms < WHEEL_LEN)
            {
                break;
            }
        }
    }
    assert(min_ms != 0);

    return min_ms;
}

void MySimpleTimer::add_new_task(const std::vector<TimedTask *> &task_list)
{
    for(auto& task:task_list)
    {
        assert(task->m_time_out > 0);
        task->m_deadline = m_tick_count + task->m_time_out;
        const int index = task->m_deadline & INDEX_MASK;

        auto& bucket = m_timed_task[index];

        if(bucket.empty() || task->m_deadline > bucket.back()->m_deadline)
        {
            bucket.push_back(task);
        }
        else
        {
            for(auto iter = bucket.begin(); iter!= bucket.end();++iter)
            {
                auto& t = *iter;
                if(task->m_deadline <= t->m_deadline)
                {
                    bucket.insert(iter, task);
                    break;
                }
            }
        }
    }
}

}