//
// Created by zhushuo on 2023/10/3.
//

#include "memory_pool.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

#include "allocator.h"
#include "io_util.h"

namespace arrow
{
constexpr char  k_default_backend_env_var[] = "ARROW_DEFAULT_MEMORY_POOL";
enum class  MemoryPoolBackend : uint8_t {System};

struct SupportedBackend
{
    const char* name;
    MemoryPoolBackend backend;
};

const std::vector<SupportedBackend>& supported_backends()
{
    static std::vector<SupportedBackend> backends = {{"system",MemoryPoolBackend::System}};
    return backends;
}

std::optional<MemoryPoolBackend> user_selected_backend()
{
    static auto user_selected_backend = []()->std::optional<MemoryPoolBackend>{
        auto unsupported_backend = [] (const std::string& name)
        {
            std::vector<std::string> supported;
            for(const auto backend: supported_backends())
            {
                supported.push_back(std::string("'") + backend.name + "'");
            }

            std::cout<<"Unsupported backend '" <<name<<"' specified in "
            << k_default_backend_env_var <<std::endl;
        };

        auto maybe_name = GetEnvVar(k_default_backend_env_var);

        if(!maybe_name.has_value())
        {
            return {};
        }

        const auto name = *std::move(maybe_name);

        if (name.empty())
        {
            return {};
        }

        const auto found = std::find_if(supported_backends().begin(), supported_backends().end(),
                                        [&](const SupportedBackend& backend) {return name == backend.name;});

        if(found != supported_backends().end())
        {
            return found->backend;
        }

        unsupported_backend(name);

        return {};
    }();

    return user_selected_backend;
}

int64_t MemoryPool::max_memory() const {return -1;}

template <typename Allocator>
class BaseMemoryPoolImpl:public  MemoryPool
{
protected:
    internal::MemoryPoolStats m_stats;
public:
    ~BaseMemoryPoolImpl() override {}

    Status allocate(int64_t size, uint8_t** out) override
    {
        if(size<0)
        {
            return  Status::make_invalid("negative malloc size");
        }

        if(static_cast<uint64_t>(size) >= std::numeric_limits<size_t>::max())
        {
            return Status::make_capacity_error("malloc size overflows size_t");
        }
        RETURN_NOT_OK(Allocator::allocate_aligned(size, out));

        m_stats.update_allocated_bytes(size);
        return Status::make_ok();
    }

    Status reallocate(int64_t old_size, int64_t new_size, uint8_t** ptr) override
    {
        if(new_size < 0)
        {
            return Status::make_invalid("negative reallocate size");
        }

        if(static_cast<uint64_t>(new_size) >= std::numeric_limits<size_t>::max())
        {
            return Status::make_capacity_error("reallocate overflows size_t");
        }

        RETURN_NOT_OK(Allocator::reallocate_aligned(old_size, new_size, ptr));

        m_stats.update_allocated_bytes(new_size - old_size);
        return Status::make_ok();
    }

    void free(uint8_t* buffer, int64_t size) override
    {
        Allocator::deallocate_aligned(buffer, size);
        m_stats.update_allocated_bytes(-size);
    }

    void  release_unused() override
    {
        Allocator::release_unused();
    }

    int64_t bytes_allocated() const override
    {
        return m_stats.bytes_allocated();
    }

    int64_t  max_memory() const override
    {
        return m_stats.max_memory();
    }


};


class SystemMemoryPool:public BaseMemoryPoolImpl<SystemAllocator> {
public:
    std::string backend_name() const override {
        return "system";
    }//没用到，按理说应该从这里获取name，找到自己的内存池
};


MemoryPoolBackend default_backend()
{
    auto backend = user_selected_backend(); //本来是用环境变量（设置量）取内存池，但是
    if(backend.has_value())
    {
        return backend.value();
    }

    struct SupportedBackend default_b = supported_backends().front();

    return default_b.backend;
}

std::unique_ptr<MemoryPool> MemoryPool::create_default()
{
    auto backend = default_backend();
    switch(backend)
    {
        case MemoryPoolBackend::System:
            return std::unique_ptr<MemoryPool>(new SystemMemoryPool);
        default:
            std::cout<<"internal error: cannot create default memory pool";
            return nullptr;
    }
}

static struct GlobalState{
    std::atomic<bool> finalizing{false};
    ~GlobalState() {finalizing.store(true, std::memory_order_relaxed);}
    bool  is_finalizing() const
    {
        return finalizing.load(std::memory_order_relaxed);
    }

    SystemMemoryPool system_pool;
} global_state;

MemoryPool* default_memory_pool()
{
    auto backend = default_backend();
    switch (backend) {
        case MemoryPoolBackend::System:
            return  &global_state.system_pool;
        default:
            std::cout<<"internal error: cannot create default memory pool"<<std::endl;
            return nullptr;
    }
}
}


