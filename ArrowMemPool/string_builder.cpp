//
// Created by zhushuo on 2023/10/2.
//

#include "string_builder.h"

#include <sstream>

namespace arrow {

namespace util {
namespace detail {

    StringStreamWrapper::StringStreamWrapper()
            : m_sstream(std::make_unique<std::ostringstream>()), m_ostream(*m_sstream) {}

    StringStreamWrapper::~StringStreamWrapper() {}

    std::string StringStreamWrapper::str() { return m_sstream->str(); }

}  // namespace detail
}  // namespace util
}  // namespace arrow