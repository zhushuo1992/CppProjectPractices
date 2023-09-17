//
// Created by zhushuo on 2023/9/10.
//

#ifndef ARROWTHREADPOOL_VISIBILITY_H
#define ARROWTHREADPOOL_VISIBILITY_H

#pragma once

#ifndef ARROW_EXPORT //其他链接单元可见
#define ARROW_EXPORT __attribute__((visibility("default")))
#endif

#endif //ARROWTHREADPOOL_VISIBILITY_H
