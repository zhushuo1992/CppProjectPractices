#include <iostream>
#include "my_simple_timer.h"

int main() {
    simple_timer::MySimpleTimer timer {};
    auto start = std::chrono::high_resolution_clock::now();

    timer.add_task([start](){
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start; // 计算时间差
        std::cout << "Code executed in " << diff.count() << " seconds" << std::endl;

        std::cout<<"hhhhh"<<std::endl;
        return;
    }, 10000);
    timer.add_task([start](){
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start; // 计算时间差
        std::cout << "Code executed in " << diff.count() << " seconds" << std::endl;
        std::cout<<"jjjjj"<<std::endl;
        return;
    }, 30000);
    std::cout << "Hello, World!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50000));
    return 0;
}
