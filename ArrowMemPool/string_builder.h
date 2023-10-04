//
// Created by zhushuo on 2023/10/2.
//

#ifndef ARROWMEMPOOL_STRING_BUILDER_H
#define ARROWMEMPOOL_STRING_BUILDER_H


#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include "visibility.h"

namespace arrow{

namespace util {

namespace detail {

    class ARROW_EXPORT StringStreamWrapper {
    protected:
        std::unique_ptr<std::ostringstream> m_sstream;
        std::ostream &m_ostream;
        //std::ostream是C++标准库中的一个类，它用于将数据写入到各种输出流，例如控制台，文件，或者字符串。
        //它通常与std::ofstream（用于写入文件）和std::ostringstream（用于构建字符串）一起使用

    public:
        StringStreamWrapper();

        ~StringStreamWrapper();

        std::ostream &stream() { return m_ostream; };

        std::string str();
    };

}

template <typename Head>
void string_builder_recursive(std::ostream& stream, Head&& head)
{
    stream << head;
}

//根据参数个数调用重载函数
template <typename  Head, typename... Tail>
void string_builder_recursive(std::ostream& stream, Head&& head, Tail&&... tail)
{
    string_builder_recursive(stream, std::forward<Head>(head));
    string_builder_recursive(stream, std::forward<Tail>(tail)...);
}

//提供一个可变参数输入的流字符串拼接功能
template <typename... Args>
std::string string_builder(Args&&... args)
{
    detail::StringStreamWrapper ss;
    string_builder_recursive(ss.stream(), std::forward<Args>(args)...);
    return ss.str();
}

}

}



#endif //ARROWMEMPOOL_STRING_BUILDER_H
