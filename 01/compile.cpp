/*
 * 本文件主要用来用来练习编译期指令
 */

#include <cassert>
#include <string>
#include <iostream>
#include <stdexcept>
#include <type_traits>

//编译期计算斐波那契数列
template<int N>
struct fib
{
    static_assert(N>=0, "N must >= 0");
    static const int value = fib<N-1>::value + fib<N-2>::value;
};

//特化特定值
template<>
struct fib <0>
{
    static const int value = 1;
};

template<>
struct fib<1>
{
    static const int value = 1;
};

#if __cplusplus >= 201402
//编译期即可获得函数返回值，但是可以退化为普通函数
constexpr int const_fib(int n)
{
    if (n<=1)
    {
        return 1;
    }

    return const_fib(n-1) + const_fib(n-2);
}
#else
int const_fib(int n)
{
    return 42;
}
#endif

[[deprecated(("deadline 2023.09.01"))]]
//[[gnu::deprecated()]] //编译时候会提示这个函数将在后续删除
int old_func()
{
    int value = 0;
    return value;
}

[[gnu::constructor]]
void first_func()
{
    //在main函数之前执行，不能使用cout
    printf("before main!");
}

[[gnu::destructor]]
void last_func()
{
    //在main函数之后执行，不能使用cout
    printf("after main!");
}

[[gnu::always_inline]]
inline int get_num()
{
    return 42;
}

[[noreturn]]
int case1(bool flag)
{
    throw std::runtime_error("xxxxx");
}

void case2()
{
    using namespace  std;
    [[gnu::unused]] //下面虽然有未使用变量，但是编译时不要告警 __attribute__((unused))

    int nouse;
    cout<<"case2"<<endl;
}

//热点函数，方便预测
[[gnu::hot]]
void case3()
{
    using namespace std;
    cout<<"case3"<<endl;
}

void case4()
{
    //静态断言，影响编译，不能出现变量
    static_assert(sizeof(int) == 4, "int must be 32bit");
    //static_assert(sizeof(long) >= 8, "must run on x64");
}

template<typename T>
void check_type(T v)
{
    using namespace std;

    //is_integral 判读是否整数，如果是value为true type_traits
    static_assert(is_integral<T>::value, "must be int");
    cout<<"static_assert: "<<typeid(v).name() << endl;
    //is_void 判读是void，如果是value为true type_traits
    cout<< is_void<void>::value<<endl;
}

void case5()
{
    int i=10;
    int *p = &i;

    assert(i>0 && "i must > 0");
    assert(p!= nullptr);
    std::string str = "hello";
    assert(!str.empty());
}

int main2()
{
    using namespace std;

    cout<<fib<2>::value <<endl;
    cout<<fib<3>::value<<endl;
    cout<<fib<3>::value<<endl;
    cout<<fib<5>::value<<endl;
    cout<<const_fib(10)<<endl;

    old_func();
    get_num();
    case2();
    case3();
    case4();
    case5();
    check_type(10);

    cout<<"compile stage learn"<<endl;
}

