/*
 * auto decltype
 */

#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <type_traits>
#include <signal.h>

void case1()
{
    int i=0;
    double x = 1.0;
    std::string str = "hello";
    std::map<int, std::string> m = {{1, "a"}, {2, "b"}};
    std::map<int, std::string>::const_iterator iter = m.begin();
}

void case2()
{
    auto i = 0;
    auto x = 1.0;
    auto str = "hello";
    std::map<int, std::string> m = {{1, "a"}, {2, "b"}};
    auto iter = m.begin();
    auto f = std::bind1st(std::less<int>(), 2); //二元算子转换为一元，<2
}

void case3()
{
    auto x = 0L;
    auto y=&x;
    auto z {&x};
}

class X final
{
    //auto a =10; 类内不支持
    int a  = 10;
};

void case4()
{
    auto x = 10L;
    auto& x1 = x; //推断为引用类型
    auto* x2 = &x;
    const auto& x3 = x;
    auto x4 = &x3;

    using namespace  std;

    cout<<*x2<<endl;
    cout<<*x4<<endl;

}

void case5()
{
    int x = 0;
    decltype(x) x1;
    decltype(x)& x2 =x;
    decltype(x)* x3; //auto做不到
    decltype(&x) x4;
    decltype(&x)* x5;
    decltype(x2) x6 = x2;

    using int_ptr = decltype(&x);
    using int_ref = decltype(x)&;

}

void case6()
{
    int x = 0;
    decltype(auto) x1 = (x);
    decltype(auto) x2 = &x;
    decltype(auto) x3 = x1;

    assert(std::is_lvalue_reference<decltype(x1)>::value);
    assert(std::is_pointer<decltype(x2)>::value);
    assert(std::is_lvalue_reference<decltype(x3)>::value);

}


auto get_a_set()
{
    std::set<int> s = {1,2,3};
    return  s;
}

void case7()
{
    using namespace std;
    vector<int> v = {2,3,2,5};

    for(auto& i:v)
    {
        i++;
        cout<<i<<",";
    }
    cout<<endl;

    for(const auto&i:v)
    {
        cout<<i<<",";
    }
    cout<<endl;
}

class Democlass final
{
public:
    using sig_func_ptr_t = decltype(&signal);
public:
    using set_type = std::set<int>;
private:
    set_type  m_set;
    using iter_type = decltype(m_set.begin());
    iter_type m_pos;

};


extern int main2();
extern int main3();
extern int main4();
extern int main5();

int main() {
    case1();
    case2();
    case3();
    case4();
    case5();
    case6();
    case7();


    std::cout << "Hello, World!" << std::endl;


    main2();
    main3();
    main4();
    main5();
    return 0;
}
