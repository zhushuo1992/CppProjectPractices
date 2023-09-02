//
// Created by zhushuo on 2023/8/20.
// 智能指针相关
//

#include <cassert>
#include <memory>
#include <string>
#include <iostream>

using namespace std;

template<class T, class... Args>
unique_ptr<T> my_make_unique(Args&&... args)
{
   return  unique_ptr<T>(new T(forward<Args>(args)...));
}

void case41()
{
    unique_ptr<int> ptr1(new int(10));
    assert(*ptr1 == 10);
    assert(ptr1 != nullptr);
    unique_ptr<string> ptr2(new string("string"));
    assert(*ptr2 == "string");
    assert(ptr2->size() == 6);

    auto ptr3 = make_unique<int>(42);
    assert(ptr3 && *ptr3 == 42);

    auto ptr4 = make_unique<string>("dig");

    assert(!ptr4->empty());

    auto ptr5 = my_make_unique<long>(100L);
    assert(*ptr5 == 100);

}

void  case42()
{
    auto ptr1 = make_unique<int>(42);
    assert(ptr1 && *ptr1 == 42);

    auto ptr2 = move(ptr1);
    assert(!ptr1 && ptr2);
}

void case43()
{
    shared_ptr<int> ptr1(new int(10));
    assert(*ptr1 == 10 );

    shared_ptr<string> ptr2(new string("hello"));
    assert(*ptr2 == "hello");

    auto ptr3 = make_shared<int>(42);
    assert(ptr3 && *ptr3 == 42);

    auto ptr4 = make_shared<string>("aaaa");
    assert(!ptr4->empty());
}

void case44()
{
    auto ptr1 = make_shared<int>(42);
    assert(ptr1 && ptr1.unique());

    auto ptr2 = ptr1;
    assert(ptr1 && ptr2);
    assert(ptr1 == ptr2);
    assert(!ptr1.unique() && ptr1.use_count() == 2);
    assert(!ptr2.unique() && ptr2.use_count() == 2);
}

class DemoShared final
{
public:
    DemoShared() = default;
    ~DemoShared()
    {

    }
};

class Node final
{
public:
    using this_type = Node;
    using shared_type = weak_ptr<this_type>;
    shared_type next;
    Node() = default;
    ~Node()
    {
        cout<< "vvv"<<endl;
    }
};


void case45()
{
    auto n1 = make_shared<Node>();
    auto n2 = make_shared<Node>();

    assert(n1.use_count() == 1);
    assert(n2.use_count() == 1);

    n1->next = n2;
    n2->next = n1;

    if (!n1->next.expired()){
        auto ptr = n1->next.lock();
        assert(ptr == n2);
    }

}

int main5()
{
    case41();
    case42();
    case43();
    case44();
    case45();
}