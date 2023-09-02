//
// Created by zhushuo on 2023/8/27.
//

#include <cassert>
#include <iostream>
#include <vector>

#include "sample,pd.cc" //编译.proto后得到 protoc --cpp_out=. sample_v3.

using namespace std;

void case71()
{
    using vendor_t = sample::Vendor;
    vendor_t v;
    assert(!v.IsInitialized());
    v.set_id(1);
    v.set_name("sony");
    v.set_valid(true);

    assert(v.IsInitialized());
    assert(v.hs_id()&&v.id() == 1);
    cout<<v.DebugString()<<endl;

    string enc;
    v.SerializeToString(&enc);

    ventor_t v2;
    assert(!v2.IsInitialized());
    v2.ParseFromString(enc);

    assert(v.id() == v2.id());
}

void case72()
{
    auto alloc_vendor = [](){
        return new sample::Vendor;
    };

    auto v = alloc_vendor();

    v->set_id(1);
    v.set_name("sony");
    v.set_valid(true);

    using product_t = sample::Product;

    product_t p;

    p.set_id(1);

}

