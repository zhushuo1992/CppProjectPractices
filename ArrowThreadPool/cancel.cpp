//
// Created by zhushuo on 2023/9/10.
//

#include <atomic>
#include <mutex>
#include <sstream>

#include <utility>
#include "marcos.h"
#include "cancel.h"

namespace arrow{
#if ATOMIC_INT_LOCK_FREE != 2
#error Lock_free atomic int required for signal safety
#endif

struct StopSourceImpl{
   std::atomic<int> m_requested {0}; //是否停止 非0代表停止
   std::mutex m_mutex; //不同的线程可能都会操作停止，所有有个锁
   Status m_cancel_error;
}; //接口实现

    StopSource::StopSource() : m_impl(new StopSourceImpl){}

    StopSource::~StopSource() = default;

    void StopSource::request_stop() {
        request_stop(Status::make_cancelled("Operation Cancelled"));
    }

    void StopSource::request_stop(Status error) {
        std::lock_guard<std::mutex> tm_lock(m_impl->m_mutex);
        DCHECK_NOT_OK(error);
        if(!m_impl->m_requested)
        {
            m_impl->m_requested = -1;
            m_impl->m_cancel_error = std::move(error);
        }
    }

    void StopSource::request_stop_from_signal(int signum) {
        m_impl->m_requested.store(signum);
    }

    void StopSource::reset() {
        std::lock_guard<std::mutex> tm_lock(m_impl->m_mutex);
        m_impl->m_cancel_error = Status::make_ok();
        m_impl->m_requested.store(0);
    }

    StopToken StopSource::token() {
        return  StopToken(m_impl);
    }


    bool  StopToken::is_stop_requested() const {
        if(!m_impl)
        {
            return false;
        }

        return m_impl->m_requested.load() != 0; //加载值
    }


    Status StopToken::poll() const { //获取状态
        if (!m_impl)
        {
            return  Status::make_ok();
        }
        if(!m_impl->m_requested.load())
        {
            return  Status::make_ok();
        }
        std::lock_guard<std::mutex> tm_lock(m_impl->m_mutex);
        if (m_impl->m_cancel_error.ok())
        {
            auto signum = m_impl->m_requested.load();
            DCHECK_GE(signum, 0);
            m_impl->m_cancel_error = Status::make_cancelled("Operation Cancelled");
        }

        return  m_impl->m_cancel_error;

    }



}