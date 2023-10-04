#include <iostream>

#include "memory_pool.h"

int main() {

    arrow::MemoryPool* pool = arrow::default_memory_pool();
    char* val;
    Status status = pool->allocate(14, reinterpret_cast<uint8_t**>(&val));

    if(status.ok())
    {
        std::cout<<"memory allocate success" <<std::endl;
        std::strcpy(val, "Hello!!!");
        std::cout<<"filled content: "<< val<<std::endl;
        pool->free(reinterpret_cast<uint8_t*>(val), 4);
    }
    else
    {
        std::cout<<"memory allocate failed"<<std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
