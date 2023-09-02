//
// Created by zhushuo on 2023/8/26.
//

#include <cassert>
#include <iostream>
#include <atomic>
#include <mutex>
#include <future>
#include <thread>

using namespace std;

void case31()
{
    static std::once_flag flag;
    auto f = []()
    {
        cout<<"tid="<<this_thread::get_id<<endl;
        std::call_once(flag, [](){
            cout<<"only once"<<endl;
        });
    };

    thread t1(f);
    thread t2(f);

    t1.join();
    t2.join();
}

void case32()
{
    thread_local int n = 0;
    auto f = [&] (int x)
    {
        n += x;
        cout<<n;
        cout<<", tid="<<this_thread::get_id()<<endl;

    };

    thread t1(f, 10);
    thread t2(f, 20);

    t1.join();
    t2.join();
}

void case33()
{
    atomic_int x{0};
    atomic_long y(1000L);
    assert(++x == 1);
    y+= 200;
    assert(y<2000);

    static atomic_flag flag{false};
    static atomic_int n;

    auto f = [&]()
    {
        auto value = flag.test_and_set(); //设置为新值（非0）返回旧值
        if(value)
        {
            cout<<"flag already set"<<endl;
        }
        else
        {
            cout<<"set flag by"<<this_thread::get_id()<<endl;
        }
        n+= 100;
        this_thread::sleep_for(n.load()*10ms);
        cout<<n<<endl;

    };

    thread t1(f);
    thread t2(f);

    t1.join();
    t2.join();
}

void case34()
{
    auto task = [](auto x)
    {
        this_thread::sleep_for(x*1ms);
        cout<<"sleep for "<<x<<endl;
        return  x;
    };

    auto f = std::async(task, 10);
    f.wait();
    assert(f.valid());
    cout<<f.get()<<endl;
    assert(!f.valid());
}

int main4()
{
    case31();
    case32();
    case33();
    case34();
}