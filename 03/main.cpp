/*
 * 算法相关
 */

#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

void cas1()
{
    vector<int> v = {1,2,3,4,5,6};
    auto n1 = count(begin(v), end(v), 1);
    int n2 = 0;
    for(auto x:v)
    {
        if(1 == x)
        {
            n2++;
        }
    }

    assert(n1 == n2);

    auto n = count_if(begin(v), end(v), [](auto x){
        return x >2;
    });

    assert(n == 4);
}

void case2()
{
    vector<int> v = {1,2,3,4,5};

    auto iter1 = begin(v);
    auto iter2 = end(v);
    auto iter3 = v.begin();
    auto iter4 = v.end();

    assert(iter1 == iter3);
    assert(iter2 == iter4);
}

void case3()
{
    array<int, 5> arr = {0,1,2,3,4}; //不可变
    auto b = begin(arr);
    auto e = end(arr);
    assert(distance(b, e) == 5);

    auto p = next(b);
    assert(distance(b, p) == 1);
    assert(distance(p, b) == -1);

    advance(p, 2); //迭代器前进
    assert(*p == 3);
    assert(p == prev(e, 2)); //迭代器后退
}


void case4()
{
    vector<int> v = {0,1,2,3,4,5};
    for (const auto& x:v)
    {
        cout<<x<<endl;
    }

    auto print = [](const auto& x){
        cout <<x<<endl;
    };

    for_each(cbegin(v), cend(v), print);
    for_each(cbegin(v), cend(v), [](const auto& x){
        cout<<x<<endl;
    });
}

void case5()
{
    vector<int> v = {10,2,8,4,5,60,7};
    auto print = [](const auto& x){
        cout <<x<<endl;
    };

    sort(begin(v), end(v));
    for_each(cbegin(v), cend(v), print);

    //TopN 在原有的容器里，把TopN移动到最前面
    partial_sort(begin(v), next(begin(v), 3), end(v));
    for_each(cbegin(v), cend(v), print);

    //把第n大的放在下标为n的位置上
    nth_element(begin(v), next(begin(v), 3), end(v));
    for_each(cbegin(v), cend(v), print);

    auto mid_iter = next(begin(v), v.size()/2);
    nth_element(begin(v), mid_iter, end(v));
    cout<<"medium is " <<*mid_iter<<endl;

    //将区间[first,last)中的元素重新排列，满足判断条件pred的元素会被放在区间的前段，不满足pred的元素会被放在区间的后段
    auto pos = partition(begin(v), end(v), [](const auto& x){
       return  x> 9;
    });

    for_each(begin(v), pos, print);

    for_each(cbegin(v), cend(v), print);

    auto value = minmax_element(cbegin(v), cend(v));
    cout<<*value.first<<"   "<<*value.second<<endl;

}

void case6()
{
    vector<int> v = {99,3,4,123,66,7,0};

    auto print = [](const auto& x){
        cout <<x<<endl;
    };

    sort(begin(v), end(v));
    auto found = binary_search(cbegin(v), cend(v), 66);

    cout<<found <<endl;

    decltype(cend(v)) pos;

    pos = lower_bound(cbegin(v), cend(v), 66);

    found = (pos != cend(v)) && (*pos == 66);
    assert(found);

    pos = upper_bound(cbegin(v), cend(v), 66);
    cout<<*pos<<endl;

    //lower+upper
    auto range = equal_range(cbegin(v), cend(v), 66);

    for_each(range.first, next(range.second), print);


}

void case7()
{
    multiset<int> s = {1,3,5,6,2,3,4,5,7,8,7};

    auto print = [](const auto& x){
        cout <<x<<endl;
    };

    auto pos = s.find(7);
    assert(pos != s.end());

    auto lower_pos = s.lower_bound(7);
    auto upper_pos = s.upper_bound(7);

    for_each(lower_pos, upper_pos, print);
}

void case8()
{
    vector<int> v = {1,34,5,6,7,9,0,3,234,1,2};

    decltype(v.end()) pos;

    pos = find(begin(v), end(v), 3);
    assert(pos != end(v));

    pos = find_if(begin(v), end(v), [](auto x){
        return x % 2 == 0;
    });
    assert(pos != end(v));

    array<int, 2> arr = {3, 5};

    //如果在第一个范围内找到第二个范围内的任何元素，则返回一个迭代器
    pos = find_first_of(begin(v), end(v), begin(arr), end(arr));
    assert(pos != end(v));


}

extern int main2();
extern int main3 ();
extern int main4();
int main() {
    cas1();
    case2();
    case3();
    case4();
    case5();
    case6();
    case7();
    case8();
    std::cout << "Hello, World!" << std::endl;
    main2();
    main3();
    main4();
    return 0;
}
