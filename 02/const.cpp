//
// Created by zhushuo on 2023/8/19.
// const
//

#include <iostream>

using namespace std;

void case11()
{
    const volatile int MAX_LEN = 1024; //告诉编译器不要做优化
    const string NAME = "mmm";

    auto ptr = (int*) (&MAX_LEN);
    *ptr = 2018;
    cout<<MAX_LEN<<endl;
}

void  case12()
{
    int x = 100;
    const int& rx = x;
    const int* px  = &x;

    cout<<rx<<*px<<endl;

    string name= "uncharted";
    const string* ps1 = &name;
    //ps1值不可变

    cout<< *ps1 <<endl;

    string* const ps2 = &name;
    *ps2 = "sss"; //指针不变，值可变

    cout<<*ps2<<endl;
    const string* const ps3 = &name;

}

class DemoClass final
{
private:
    using mutex_type = int;
private:
    mutable mutex_type  m_mutex;
    const long MAX_SIZE = 256;
    int m_value;
public:
    int get_value() const
    {
        return  m_value;
    }

    void save_data() const
    {
        m_mutex++; //const函数操作了非const的变量，用mutable修饰
    }
};

constexpr int fib(int n)
{
    if(0 == n || 1 == n)
    {
        return  1;
    }

    return fib(n-1) + fib(n-2);

}

int main2()
{

    case11();
    case12();

    constexpr int fib5 = fib(5); //编译期求值

    cout<<fib5<<endl;

    return 0;
}