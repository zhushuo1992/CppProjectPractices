//
// Created by zhushuo on 2023/8/29.
//

#ifndef INC_05_SPINLOCK_HPP
#define INC_05_SPINLOCK_HPP

#include "cpplang.hpp"

namespace cpp_study{
    class SplinLock final
    {
    public:
        using this_t = SpinLock;
        using atomic_t = std:: atomic_flag;

    public:
        SplinLock() = default;
        ~SplinLock() = default;

        SplinLock(const this_t&) = delete;
        SplinLock& operator=(const this_t&) = delete;

    public:
        void lock() noexcept
        {
            for(;;)
            {
                if(! m_lock.test_and_set()){
                    return;
                }

                std::this_thread::yield(); //是让当前线程让渡出自己的CPU时间片(给其他线程使用)
            }
        }

        bool try_lock() noexcept
        {
            return !m_lock.test_and_set();

        }

        void unlock() noexcept
        {
            m_lock.clear(); //设置为false
        }

    private:
        atomic_t m_lock {false};
    };


    class SplinLockGuard final
    {
    public:
        using this_t = SplinLockGuard;
        using sp_lock_t = SplinLock;

    public:
        SplinLockGuard(sp_lock_t& lock) noexcept
                : m_lock(lock)
        {
            m_lock.lock();
        }

        ~SplinLockGuard() noexcept
        {
            m_lock.unlock();
        }

    public:
        SplinLockGuard(const this_t& ) = delete;
        SplinLockGuard& operator=(const this_t&) = delete;

    private:
        sp_lock_t& m_lock;
    };
}

#endif //INC_05_SPINLOCK_HPP
