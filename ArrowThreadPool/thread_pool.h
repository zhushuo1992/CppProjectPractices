//
// Created by zhushuo on 2023/9/16.
//

#ifndef ARROWTHREADPOOL_THREAD_POOL_H
#define ARROWTHREADPOOL_THREAD_POOL_H

#pragma once

#include <unistd.h>
#include <cstdint>
#include <functional>
#include <future>
#include <optional>
#include <queue>
#include <type_traits>
#include <utility>

#include "cancel.h"
#include "functions.h"
#include "status.h"
#include "status.h"
#include "visibility.h"
#include "executor.h"

namespace arrow{

ARROW_EXPORT int get_cpu_thread_pool_capacity();

ARROW_EXPORT Status set_cpu_thread_pool_capacity();

class ARROW_EXPORT ThreadPool : public Executor{
public:
    static std::optional<std::shared_ptr<ThreadPool>> make(int threads);

    static std::optional<std::shared_ptr<ThreadPool>> make_external(int threads);

    ~ThreadPool();

    int  get_capacity();

    bool owns_this_thread();

    int get_num_tasks();

    Status set_capacity(int threads);

    static int default_capacity();

    Status shut_down(bool wait = true);

    void wait_for_idle();

    struct State;

protected:
    friend ARROW_EXPORT ThreadPool* get_cpu_thread_pool();

    ThreadPool();

    Status spawn_real(TaskHints hints, internal::FnOnce<void()> task, StopToken, StopCallback&&);

    void collect_finish_workers_unlocked();

    void launch_workers_unlocked(int threads);

    int get_actual_capacity();

    void protect_against_fork();

    static std::shared_ptr<ThreadPool> make_cpu_thread_pool();

    std::shared_ptr<State> m_sp_state;
    State* m_state;
    bool m_shutdown_on_destroy;
    pid_t m_pid;


};
    ARROW_EXPORT ThreadPool* get_cpu_thread_pool();
}

#endif //ARROWTHREADPOOL_THREAD_POOL_H
