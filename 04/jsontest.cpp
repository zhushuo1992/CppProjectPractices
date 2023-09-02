//
// Created by zhushuo on 2023/8/27.
//
#include <cassert>
#include <iostream>
#include "json.hpp" //nlohmann header-only

using namespace std;

using json_t = nlohmann::json;

void case11()
{
    json_t j;
    j["age"] = 23;
    j["geaer"]["suits"] = "2099";
    j["jobs"] = {"a", "b"};

    vector<int> v = {1,2,3};
    j["numbers"] = v;

    map<string, int> m = {{"name":1}, {"tt": 2}};
    j["kv"] = m;

    json_t j2 = {
            {"nn", 23},
            {"jobs", {"111", "1112"}}, //数组
            {"gear", {"suit", "2222"}} //数组
    };

}

void case12()
{
    string str = R"({
        "name": "peter",
        "age" : 23,
        "married" : true
    })";

    auto j = json_t::parse(str);

    assert(j["age"] == 23);
}

void case13()
{
    string str = R"({
        "name": "peter",
        "age" :
        "married" : true
    })";

    try
    {
        auto j = json_t::parse(str);
    }
    catch (std::exception& e)
    {
        cout<<e.what()<<endl;
    }



}