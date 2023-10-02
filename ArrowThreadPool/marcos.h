//
// Created by zhushuo on 2023/9/10.
//

#ifndef ARROWTHREADPOOL_MARCOS_H
#define ARROWTHREADPOOL_MARCOS_H


#pragma once
#define ARROW_UNUSED(x) (void)(x)

#define ARROW_PREDICT_FALSE(x) (__builtin_expect(!!(x), 0))

//触发signal

#define DCHECK_GE(val1, val2) \
  do { \
    if (!((val1) >= (val2))) { \
      std::abort(); \
    } \
  } while (false)


#define DCHECK_EQ(val1, val2) \
  do { \
    if (!((val1) == (val2))) { \
      std::abort(); \
    } \
  } while (false)

#define DCHECK_OK(val) \
  do { \
    if (!((val).ok())) { \
      std::abort(); \
    } \
  } while (false)

#define DCHECK_NOT_OK(val) \
  do { \
    if (((val).ok())) { \
      std::abort(); \
    } \
  } while (false)


#endif //ARROWTHREADPOOL_MARCOS_H
