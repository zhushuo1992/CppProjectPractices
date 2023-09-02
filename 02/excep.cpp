//
// Created by zhushuo on 2023/8/20.
// 异常相关
//

#include <iostream>
#include <stdexcept>

using namespace std;

class my_exception:public std::runtime_error
{
public:
    using this_type = my_exception;
    using super_type = std::runtime_error;
public:
    my_exception(const char* msg):super_type(msg)
    {
    }
    my_exception() = default;
    ~my_exception() = default;

private:
    int code = 0;
};

[[noreturn]]
void raise(const char* msg)
{
    throw my_exception(msg);
}

void case21()
try
{
    raise("test error");
}
catch (const exception& e)
{
    cout<< e.what()<<endl;
}

void case22() noexcept //提升性能
{
    cout << " noexcept " <<endl;
}

void case23() noexcept
{
    throw "aaaa";
}

int main3()
{
    case21();
    case22();
    //case23();
    return 0;
}