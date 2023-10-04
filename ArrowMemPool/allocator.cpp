//
// Created by zhushuo on 2023/10/3.
//

#include "allocator.h"
#include <cstdint>
#include "marcos.h"

namespace arrow{
constexpr size_t kAlignment = 64;

//alignas对齐
alignas(kAlignment) static uint8_t zero_size_area[1]; //unsigned char

Status SystemAllocator::allocate_aligned(int64_t size, uint8_t **out) {
    if(size == 0)
    {
        *out = zero_size_area;
        return Status::make_ok();
    }

    const int result = posix_memalign(reinterpret_cast<void**>(out), kAlignment, static_cast<size_t>(size));

    if(result == ENOMEM)
    {
        return Status::make_out_of_memory("malloc of size ", size, " failed");
    }

    if(result == EINVAL)
    {
        return Status::make_invalid("invalid alignment param: ", kAlignment);
    }

    return Status::make_ok();
}

Status SystemAllocator::reallocate_aligned(int64_t old_size, int64_t new_size, uint8_t **ptr) {
    uint8_t* previous_ptr = *ptr;
    if(previous_ptr == zero_size_area)
    {
        DCHECK_EQ(old_size, 0);
        return allocate_aligned(new_size, ptr);
    }

    if(new_size == 0)
    {
        deallocate_aligned(previous_ptr, old_size);
        *ptr = zero_size_area;
        return Status::make_ok();
    }

    uint8_t* out = nullptr;
    auto tmp = allocate_aligned(new_size, &out);
    RETURN_NOT_OK(tmp);

    DCHECK(out);

    memcpy(out, *ptr, static_cast<size_t>(std::min(new_size, old_size)));

    free(*ptr);

    *ptr = out;
    return Status::make_ok();
}

void SystemAllocator::deallocate_aligned(uint8_t *ptr, int64_t size) {
    if(ptr == zero_size_area)
    {
        DCHECK_EQ(size, 0);

    }
    else
    {
        free(ptr);
    }
}

void SystemAllocator::release_unused() {
#ifdef __GLIBC__:
    ARROW_UNUSED(malloc_trim(0));
#endif
}

}
