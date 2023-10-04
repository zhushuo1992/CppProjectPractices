//
// Created by zhushuo on 2023/10/3.
//

#ifndef ARROWMEMPOOL_ALLOCATOR_H
#define ARROWMEMPOOL_ALLOCATOR_H

#define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)
//posix_memalign成功时会返回(size)字节的动态内存，即p所指向的内存的地址，并且这块内存的地址是(alignment)的倍数


#pragma once
#include "status.h"

namespace arrow {
class SystemAllocator {
public:
    static Status allocate_aligned(int64_t size, uint8_t** out);
    static Status reallocate_aligned(int64_t old_size, int64_t new_size, uint8_t** ptr);
    static void deallocate_aligned(uint8_t* ptr, int64_t size);

    static void release_unused();

};

}
#endif //ARROWMEMPOOL_ALLOCATOR_H
