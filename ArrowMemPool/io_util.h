//
// Created by zhushuo on 2023/10/2.
//

#ifndef ARROWMEMPOOL_IO_UTIL_H
#define ARROWMEMPOOL_IO_UTIL_H

#pragma once
#include <iostream>
#include <optional>
#include "status.h"
#include "visibility.h"

ARROW_EXPORT
std::optional<std::string> GetEnvVar(const char* name); //C++17 ，用于表示可能不存在的值 has_value() value()
ARROW_EXPORT
Status SetEnvVar(const char* name, const char* value);
ARROW_EXPORT
Status SetEnvVar(const std::string& name, const std::string& value);


#endif //ARROWMEMPOOL_IO_UTIL_H
