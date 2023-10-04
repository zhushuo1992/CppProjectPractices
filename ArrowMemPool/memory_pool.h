//
// Created by zhushuo on 2023/10/3.
//

#ifndef ARROWMEMPOOL_MEMORY_POOL_H
#define ARROWMEMPOOL_MEMORY_POOL_H

#include <iostream>
#include <string>
#include <memory>
#include <atomic>

#include "visibility.h"
#include "marcos.h"
#include "status.h"

namespace arrow
{
namespace internal {

    class MemoryPoolStats {
    protected:
        std::atomic<int64_t> m_bytes_allocated;
        std::atomic<int64_t> m_max_memory;
    public:
        MemoryPoolStats() : m_bytes_allocated(0), m_max_memory(0) {}

        int64_t max_memory() const { return m_max_memory.load(); }

        int64_t bytes_allocated() const { return m_bytes_allocated.load(); }

        inline void update_allocated_bytes(int64_t diff) {
            auto allcated = m_bytes_allocated.fetch_add(diff) + diff; //+=diff
            if (diff > 0 && allcated > m_max_memory) {
                m_max_memory = allcated;
            }
        }
    };
}
class ARROW_EXPORT MemoryPool{
public:
    virtual ~MemoryPool() = default;

    static std::unique_ptr<MemoryPool> create_default();

    virtual Status allocate(int64_t size, uint8_t** out) = 0;

    virtual Status reallocate(int64_t old_size, int64_t new_size, uint8_t** ptr) = 0;

    virtual void free(uint8_t* buffer, int64_t size) = 0;

    virtual void release_unused() {}

    virtual int64_t bytes_allocated() const = 0;

    virtual int64_t max_memory() const;

    virtual  std::string backend_name() const = 0;

protected:
    MemoryPool() = default;
};

extern MemoryPool* default_memory_pool();



}



#endif //ARROWMEMPOOL_MEMORY_POOL_H
