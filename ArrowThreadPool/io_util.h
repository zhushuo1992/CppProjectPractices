//
// Created by zhushuo on 2023/9/16.
//

#ifndef ARROWTHREADPOOL_IO_UTIL_H
#define ARROWTHREADPOOL_IO_UTIL_H

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

#endif //ARROWTHREADPOOL_IO_UTIL_H
