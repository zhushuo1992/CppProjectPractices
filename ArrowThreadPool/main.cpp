#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "cancel.h"
#include "io_util.h"
#include "marcos.h"
#include "thread_pool.h"

using namespace arrow;

std::mutex g_console_mutex;

void task(int id)
{
    std::cout<<"task start"<<std::endl;
    std::lock_guard<std::mutex> lock(g_console_mutex);
    std::thread::id thread_id = std::this_thread::get_id();
    std::cout<<"task "<<id<<" started by thread "<<thread_id<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"task "<<id<<" completed by thread "<<thread_id<<std::endl;
}

int main() {
    auto ret = SetEnvVar("OMP_NUM_THREADS", "4");
    std::cout<<ret.to_string()<<std::endl;
    auto thread_pool_1 = get_cpu_thread_pool();
    if(!thread_pool_1)
    {
        std::cerr<<"failed to create thread pool"<<std::endl;
        return 1;
    } else
    {
        std::cout<<"create thread pool succ"<<std::endl;
    }
    for(int i = 1; i<=2; ++i)
    {
        thread_pool_1->spawn([i](){task(i);});// 也可以submit
    }
    thread_pool_1->wait_for_idle();

    thread_pool_1->shut_down();


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
