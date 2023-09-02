//
// Created by zhushuo on 2023/8/27.
//
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <regex>

#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>


using namespace std;

auto make_cpu_profiler = [](const tring& filename)
{
    ProfilerStar(filename.c_str());
    ProfileRegistThread();
    return std::shared_ptr<void>(nullptr, [](void*){
        ProfilterStop();
    });
};

static auto make_regex = [](const auto& txt)
{
    return std::regex(txt);
};

static auto make_match = [](){
    return std::smatch();
};


void case1()
{
    auto cp = make_cpu_profiler("case1.perf");
    auto str = "neir:automata"s;
    for(int i=0; i< 100; i++)
    {
        auto reg = make_regex(R"((^(\w+)\:(\w+)$)");
        auto what = make_match();
        assert(regex_match(str, what, reg));
    }
}