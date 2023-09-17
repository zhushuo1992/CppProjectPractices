//
// Created by zhushuo on 2023/9/10.
//

#ifndef ARROWTHREADPOOL_FUNCTIONS_H
#define ARROWTHREADPOOL_FUNCTIONS_H

#pragma once

#include <type_traits>
#include <utility>
#include <memory>

namespace arrow{

namespace internal{


    //FnOnce是一个模板类，它有一个模板参数Signature，这个参数是一个函数签名
    template<typename  Signature>
    class FnOnce;

    //然后我们对FnOnce进行了部分特化，当模板参数Signature是一个函数签名R(A...)时，我们就可以使用这个特化版本的FnOnce。在这个特化版本中，R是函数的返回类型，A...是函数的参数类型
    template<typename R, typename... A>
    class FnOnce<R(A...)> {
    private:

        struct Impl{
            virtual  ~Impl() = default;
            virtual R invoke(A&&... a) = 0;
        };

        template<typename Fn>
        struct FnImpl:Impl{
            explicit FnImpl(Fn fn): m_fn(std::move(fn)){}
            //R代表
            R invoke(A&&... a) override {return std::move(m_fn)(std::forward<A&&>(a)...);} //override代表重写父类同名函数
            //转发执行
            Fn m_fn;
        };

        std::unique_ptr<Impl> m_impl;
    public:
        FnOnce() = default;

        //std::enable_if 编译期，根据入参类型选择模板函数
        //std::is_convertible 在编译时检查一个类型是否可以被转换为另一个类型
        template<typename Fn, typename=typename std::enable_if<std::is_convertible<
                decltype(std::declval<Fn&&>()(std::declval<A>()...)), R>::value>::type>
        FnOnce(Fn fn): m_impl(new FnImpl<Fn>(std::move(fn))){

        }

        explicit operator bool() const {return m_impl != nullptr;}

        R operator()(A... a) &&{
            auto bye = std::move(m_impl);
            return bye->invoke(std::forward<A&&>(a)...);
        }
    };

}
}

#endif //ARROWTHREADPOOL_FUNCTIONS_H
