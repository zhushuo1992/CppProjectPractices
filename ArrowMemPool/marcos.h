//
// Created by zhushuo on 2023/10/2.
//

#ifndef ARROWMEMPOOL_MARCOS_H
#define ARROWMEMPOOL_MARCOS_H

#pragma once

//禁止拷贝构造与赋值运算符
#define ARROW_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;            \
  void operator=(const TypeName&) = delete

#define ARROW_UNUSED(x) (void)(x)

//可能性较大
#define ARROW_PREDICT_FALSE(x) (__builtin_expect(!!(x), 0))

#define ARROW_STRINGIFY(x) #x

#define DCHECK_GE(val1, val2)  \
  do {                         \
    if (!((val1) >= (val2))) { \
      std::abort();            \
    }                          \
  } while (false)

#define DCHECK(val) \
  do {              \
    if (!(val)) {   \
      std::abort(); \
    }               \
  } while (false)

#define DCHECK_EQ(val1, val2)  \
  do {                         \
    if (!((val1) == (val2))) { \
      std::abort();            \
    }                          \
  } while (false)

#define DCHECK_OK(val)   \
  do {                   \
    if (!((val).ok())) { \
      std::abort();      \
    }                    \
  } while (false)

#define DCHECK_NOT_OK(val) \
  do {                     \
    if (((val).ok())) {    \
      std::abort();        \
    }                      \
  } while (false)

#endif //ARROWMEMPOOL_MARCOS_H
