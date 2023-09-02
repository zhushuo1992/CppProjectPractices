/*
 * 预处理相关
 */

#include <iostream>

using namespace std;

void case11()
{
#if __linux__
    #define HAS_LINUX 1
#endif
    //cout<<HAS_LINUX<<endl;
}

void  case12()
{
#define  CUBE(a) (a)*(a)*(a)
    cout<<CUBE(10)<<endl;
    cout<<CUBE(15)<<endl;
#undef CUBE

#ifdef AUTH_PWD
    #undef AUTH_PWD
#endif
#define AUTH_PWD "XXXX"
#define MAX_BUF_LEN 65535
#define  VERSION "1.1.122
}

#define BEGIN_NAMESPACE(x) namespace x {

#define END_NAMESPACE(x) }

BEGIN_NAMESPACE(my_own)

class MyClass final
{

};

void case13()
{
    cout<<"working in own namespace"<<endl;
}


END_NAMESPACE(my_own)


#ifdef __cplusplus
    extern "C" {
#endif
    void  a_c_func(int a);
#ifdef __cplusplus
    }
#endif


void case14()
{
#if __cplusplus >= 201402
    cout<<"c++14 or later"<<endl;
#elif __cplusplus >= 201103
    cout<<"c++11"<<endl;
#else
#error "c++ is too old"
#endif
#if __GNUC_ <= 4
    cout<<"gcc is too old"<<endl;
#else
    cout<<"gcc good"<<endl;
#endif

#ifndef NDEBUG
    cout<<"debug mode"<<endl;
#endif
}