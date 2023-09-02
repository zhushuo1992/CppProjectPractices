//
// Created by zhushuo on 2023/8/26.
// 容器
//

#include <cassert>
#include <iostream>
#include <complex>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>

#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;


class Point final
{
public:
    int x = 0;
public:
    Point(int a) noexcept:x(a)
    {
        cout<<"ctor1"<<endl;

    }

    Point() noexcept
    {
        cout<<"ctor2"<<endl;
    }

    ~Point() noexcept
    {
        cout<<"dtor"<<endl;
    }

    Point(const Point& p) noexcept
    {
        x = p.x;
        cout<<"copy ctor"<<endl;

    }

    Point(Point&& p) noexcept
    {
        x = std::move(p.x);
        cout<<"move ctor"<<endl;

    }
};


void case11()
{
    cout<<"case11 start"<<endl;
    vector<Point> v;
    v.reserve(10);

    Point p;
    Point p2;

    v.push_back(p);
    v.emplace_back(p2);
    cout<<"------------"<<endl;
    v.push_back(Point());
    v.emplace_back(Point());
    cout<<"------------"<<endl;
    v.push_back(std::move(p));
    v.emplace_back(1);
    cout<<"case11 end"<<endl;
}

void case12()
{
    array<int, 2> arr;
    assert(arr.size() == 2);

    vector<int> v(2);

    for(int i = 0; i < 10; i++)
    {
        v.emplace_back(i);
    }

    assert(v.size() == 12);

    deque<int> d;
    d.emplace_back(9);
    d.emplace_front(1);
    assert(d.size() == 2);

}

bool operator<(const Point& a, const Point& b)
{
    return a.x < b.x;
}

void case13()
{
    set<Point> s;
    s.emplace(7);
    s.emplace(3);

    for(auto &p: s)
    {
        cout<<p.x<<endl;
    }
}


void case14()
{
    set<int> s = {7, 3, 9};
    for(auto& x: s)
    {
        cout<<x<<endl;
    }

    auto comp = [](auto a, auto b){
        return a > b;
    };

    set<int, decltype(comp)> gs(comp);

    std::copy(begin(s), end(s), inserter(gs, gs.end()));
    for(auto& x: gs)
    {
        cout<<x<<endl;
    }

}

void case15()
{
    using map_type = unordered_map<int, string>;

    map_type dict1;
    dict1[1] = "one";
    dict1.emplace(2, "two");

    for(auto&x: dict1)
    {
        cout<<x.first<<": "<< x.second <<endl;
    }
}

bool operator== (const Point& a, const Point& b)
{
    return a.x == b.x;
}

void case16()
{
    auto hasher = [](const auto & p)
    {
        return std::hash<int>()(p.x);
    };

    unordered_set<Point, decltype(hasher)> s(10, hasher);

    s.emplace(7);
    s.emplace(3);

    for(auto& p:s)
    {
        cout<<p.x<<endl;
    }

}

int main2()
{
    case11();
    case12();
    case13();
    case14();
    case15();
    case16();
}