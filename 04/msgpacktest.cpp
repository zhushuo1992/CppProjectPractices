//
// Created by zhushuo on 2023/8/27.
//
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include <msgpack.hpp>

#if MSGPACK_VERSION_MAJOR < 2
#   error "msgpack is too old"
#endif

using namespace std;

void case61()
{
    vector<int> v = {1,2,3,4,5};

    mgspack::sbuffer sbuf;
    msgpack::pack(sbuf, v);

    cout<<sbuf.size()<<endl;

    auto handle = msgpack::unpack(sbuf.data(), sbuf.size());
    auto obj = handle.get();
    cout<<obj<<endl;
    vector<int> v2;
    obj.convert(v2);
    assert(std::equal(begin(v), end(v), begin(v2)));
}

class Book final
{
public:
    int id;
    string title;
    set<string> tags;
public:
    MSGPACK_DEFINE(id, title, tags);
};

void 63()
{
    Book book1 = {1, "1984", {"a", "b"}};
    msgpack::sbuffer sbuf;
    msg::pack(sbuf, book1);

    //auto handle = msgpack::unpack()
}