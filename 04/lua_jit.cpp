//
// Created by zhushuo on 2023/8/27.
//

#include <cassert>
#include <iostream>
#include <memory>

extern "C" {
#include <luajit.h>
#include <lualib.h>
#include <luaxlib.h>
}

#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

using namespace std;

auto make_luavm = []()
{
    std::shared_ptr<lua_State> vm(
            luaL_newstate(), lua_close
            );
    assert(vm);

    lua_openlibs(vm.get());

    return vm;

};

#define L vm.get()

void case51()
{
    auto vm = make_luavm();

    auto package = getGlobal(L, "package");

    string path = package["path"];

    string cpath = package["cpath"];

    cout<<path<<endl;
    cout<<cpath<<endl;

}

void case52()
{
    auto vm = make_luavm();
    int status;
    status = luaL_dostring(L, "print('hello lua')");
    status = luaL_dofile(L, "./embedded.lua");
    status = luaL_dofile(L, "./shared.lua");

}

void case53()
{
    auto vm = make_luavm();

    int status;

    string chunk = R"(
function say(s)
    print(s)
end
function add(a, b)
    return a + b
end
)";
    status = luaL_dostring(L, chunk.c_str());
    assert(status == 0);

    auto f1 = getGlobal(L, "say");
    f1("say something");

    auto f2 = getGlobal(L, "add");
    auto v = f2(10, 20);
    assert(v == 30);
}