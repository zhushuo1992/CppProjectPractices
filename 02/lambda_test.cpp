//
// Created by zhushuo on 2023/8/20.
// lambda函数相关
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

void my_square(int x)
{
    cout<<x*x<<endl;
}

void case31()
{
    auto pfunc = &my_square;
    (*pfunc)(3);
    pfunc(3);

    auto func = [](int x){
        cout<<"lambda "<<x*x<<endl;
    };

    func(3);
}

void case32()
{
    int n = 10;
    auto func = [=](int x)
    {
        cout << x*n <<endl;
    };
    func(2);
}

void  case33()
{
    auto f1 = [](){};

    auto f2 = []()
    {
        cout<<"lambda f2"<<endl;

        auto f3 = [](int x)
        {
            return  x*x;
        };

        cout<<f3(10)<<endl;
    };

    f1();
    f2();

    vector<int> v = {3,1,2,3,4,5};

    //找到大于5的，begin end由 std提供
    cout << *find_if(begin(v), end(v), [](int x)
                {
                    return x >= 5;
                }
    )<<endl;
}

void case34()
{
    int x = 33;
    auto f1 = [=]()
    {
        cout<<x<<endl;
    };

    auto f2 = [&]()
    {
        x+= 10;
    };

    auto f3 = [=, &x]()
    {
        x += 20;
    };

    f1();
    f2();
    cout<<x<<endl;
    f3();
    cout<<x<<endl;
}

class DemoLambda final
{
public:
    DemoLambda() = default;
    ~DemoLambda() = default;

private:
    int x = 0;
public:
    auto print()
    {
        return [this]()
        {
            cout<<"member " << x <<endl;
        };
    }
};

void case35()
{
    DemoLambda obj;
    auto f = obj.print();
    f();
}

void case36()
{
    auto f = [](const auto& x)
    {
        return x+x;
    };

    cout<<f(3)<<endl;
    cout<<f(0.618)<<endl;

    string str = "m2";
    cout<<f(str)<<endl;

    cout<<f(str)<<endl;
}

class Demo final
{
public:
    using func_type = std::function<void()>;
public:
    func_type print = [this]()
    {
        cout << "value" << m_value<<endl;
        cout<<"hello"<<endl;
    };
private:
    int m_value = 10;
};


int main4()
{
    case31();
    case32();
    case33();
    case34();
    case35();
    case36();

    Demo d;
    d.print();

}