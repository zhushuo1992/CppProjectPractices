//
// Created by zhushuo on 2023/9/3.
//
#pragma once //在编译时告诉编译器只包含一次该头文件

#ifndef THREADPOOL_THREADPOOL_H
#define THREADPOOL_THREADPOOL_H

#include <vector>
#include <queue>
#include <functional>
#include <atomic>
#include <future>
#include <stdexcept>

namespace my_thread_pool {
    //写一个线程池

#define THREADPOOL_MAX_NUM 16

//线程池是否可以自动增长(如果需要,且不超过 THREADPOOL_MAX_NUM)
//#define  THREADPOOL_AUTO_GROW

//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
//不直接支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等

class ThreadPool{
private:
    unsigned short m_init_size = 3; //线程初始化数量
    //std::function<return_type(arguments)> func;
    //template<typename FRet>
    using Task = std::function<void()>; //只能void类型?
    std::vector<std::thread> m_pool; //线程池
    std::queue<Task> m_task_que; //任务队列
    std::mutex m_lock;           //任务队列同步锁
#ifdef THREADPOOL_AUTO_GROW
    std::mutex m_lock_grow;     //线程池争执同步锁
#endif
    std::condition_variable m_task_cv; //条件变量
    std::atomic<bool> m_run {true}; //线程池是否执行
    std::atomic<int> m_idl_thread_num {0}; //空闲线程数量

public:
    ThreadPool(unsigned short size)
    {
        m_init_size = size;
        add_thread(size);
    };

    ~ThreadPool()
    {
        m_run = false;
        m_task_cv.notify_all(); //唤醒所有线程
        for (auto & one_thread: m_pool )
        {
            if(one_thread.joinable())
            {
                one_thread.join();
            }
        }
    };

public:

    //提交1个任务
    //调用.get()获取返回值，等待任务执行完成后，获取返回值
    // bind: .commit(std::bind(&Dog::say_hello, &dog));
    // mem_fn: commit(std::mem_fn(&Dog::say_hello), this);

    template<typename F, typename... Args>
    auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
    {
        if(!m_run)
        {
            throw std::runtime_error("commit on threadpool is stopped");
        }

        using ret_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ret_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                );

        std::future<ret_type> tm_future = task->get_future();

        {
            std::lock_guard<std::mutex> tm_lock {m_lock};
            m_task_que.emplace([task](){(*task)();});
        }
#ifdef THREADPOOL_AUTO_GROW
        if(m_idl_thread_num<1 && m_pool.size() < THREADPOOL_MAX_NUM)
        {
            add_thread(1);
        }
#endif

        m_task_cv.notify_one(); // 唤醒1个线程去执行

        return tm_future;
    }

    //提交1个无参数任务，且无返回值
    template<typename F>
    void commit_without_para_ret(F&& task)
    {
        if(!m_run)
        {
            return;
        }

        {
            std::lock_guard<std::mutex> lock{m_lock};
            m_task_que.emplace(std::forward<F>(task));
        }

#ifdef THREADPOOL_AUTO_GROW
        if(m_idl_thread_num<1 && m_pool.size() < THREADPOOL_MAX_NUM)
        {
            add_thread(1);
        }
#endif
        m_task_cv.notify_one();
    }

    int get_idl_count()
    {
        return m_idl_thread_num;
    }

    int get_thread_count()
    {
        return m_pool.size();
    }

#ifdef THREADPOOL_AUTO_GROW
private:
#endif

    void add_thread(unsigned short size)
    {
#ifdef THREADPOOL_AUTO_GROW
        if (!m_run)
        {
            throw std::runtime_error("grow on threadpool is stopped");
        }

        std::unique_lock<std::mutex> lock_grow{m_lock_grow};
#endif

        for(;m_pool.size() < THREADPOOL_MAX_NUM && size > 0; --size)
        {
            m_pool.emplace_back([this]{
                while(true)
                {
                    Task task;


                    {
                        std::unique_lock<std::mutex> tm_lock {m_lock};
                        m_task_cv.wait(tm_lock, [this]{
                            return !m_run || !m_task_que.empty();
                        }); //wait，直到有task或者需要停止， 后面是个外层while
                        //为啥是或? 如果是且，这个while的条件，开始的时候被!m_run恒不成立，这个线程就一直被唤醒
                        //等待表达式为true，并且需要别人notify

                        //析构时所有线程唤醒，然后发现m_run为false，线程结束
                        //如果是或，线程池刚建立的时候，m_task_que为空，直接退出了，所以是且
                        if(!m_run && m_task_que.empty()) //为啥是且？
                        {
                            return;
                        }

                        m_idl_thread_num-- ;
                        task = std::move(m_task_que.front());
                        m_task_que.pop();
                    }

                    task(); //执行任务
#ifdef THREADPOOL_AUTO_GROW
                    if(m_idl_thread_num >0 && m_pool.size() > m_init_size)
                    {
                        return;
                    }
#endif
                    //任务执行完成了，空闲线程数量+1
                    {
                        std::unique_lock<std::mutex> tm_lock {m_lock};
                        m_idl_thread_num++;
                    }
                }
            });

            //add_thread执行完成了，新加了一个执行线程，空闲线程数量+1
            {
                std::unique_lock<std::mutex> tm_lock {m_lock};
                m_idl_thread_num++;
            }
        }
    };

};

}





#endif //THREADPOOL_THREADPOOL_H
