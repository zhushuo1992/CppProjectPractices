//
// Created by zhushuo on 2023/10/4.
//

#ifndef MYSIMPLETIMER_MY_SIMPLE_TIMER_H
#define MYSIMPLETIMER_MY_SIMPLE_TIMER_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <list>
#include <functional>
#include "ThreadPool.h"

struct TimedTask;

namespace simple_timer
{
class MySimpleTimer
{
public:
    static const int WHEEL_LEN = 1024;
    static const int INDEX_MASK = WHEEL_LEN - 1;
    using task_t = std::function<void()>;

public:
    MySimpleTimer();
    ~MySimpleTimer();

public:
    void add_task(const task_t& task, int msec);

private:
    void thread_routine();
    int get_next_wait_ms();
    void tick(long long tick_count);
    void add_new_task(const std::vector<TimedTask*>& task_list);

private:
    bool m_stop; //是否要停止
    long long m_tick_count; //距离起始时间过去了多少周期
    std::mutex m_mutex;
    std::thread m_thread;
    std::condition_variable m_cond;
    my_thread_pool::ThreadPool m_thread_pool;
    std::vector<TimedTask*> m_input_tasks;
    std::list<TimedTask*> m_timed_task[WHEEL_LEN];
    std::chrono::time_point<std::chrono::steady_clock> m_start_time; //定时器起始时间
};
}



#endif //MYSIMPLETIMER_MY_SIMPLE_TIMER_H
