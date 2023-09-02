//
// Created by zhushuo on 2023/8/29.
// 从lua读配置
//

#ifndef INC_05_CONFIG_HPP
#define INC_05_CONFIG_HPP

#include "cpplang.hpp"

extern "C" {
#include <luajit.h>
#include <lualib.h>
#include <luaxlib.h>
};

#include <LuaBridge/LuaBridge.h>

namespace cpp_study{
    class Config final
    {
    public:
        using vm_t = std::shared_ptr<Lua_Statte>;
        using value_t = luabridge::LuaRef;

        using string_t = std::string;
        using string_view_t = const std::string&;
        using regex_t = std::regex;

        using match_t = std::smatch;
    public:
        Config() noexcept
        {
            assert(m_vm);

            luaL_openlibs(m_vm.get()); //载入基础库
        }
        ~Config() = default;

    public:
        void load(string_view_t filename) const
        {
            auto status = luaL_dofile(vm.get(), filename.c_str()); //运行脚本
            if(status != 0)
            {
                throw std::runtime_error("config parse fail");
            }
        }

        template<typename T>
        T get(string_view_t key) const
        {
            if(!std::regex_match(key, m_what, m_reg))
            {
                throw std::runtime_error("config key error");
            }

            auto w1 = m_what[1].str();
            auto w2 = m_what[2].str();
            using namespace luabridge;
            auto v = getGlobal(m_vm.get(), w1.c_str());

            return LuaRef_cast<T>(v[w2]);
        }

    private:
        vm_type m_vm{luaL_newstate(), lua_close}; //创建全局状态机，通过智能指针释放

        const regex_t m_reg  {R"(^(\w+)\.(\w+)$)"};

        mutable match_t m_what;
    };
}

#endif //INC_05_CONFIG_HPP
