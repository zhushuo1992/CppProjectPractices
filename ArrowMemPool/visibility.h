//
// Created by zhushuo on 2023/10/2.
//

#ifndef ARROWMEMPOOL_VISIBILITY_H
#define ARROWMEMPOOL_VISIBILITY_H

#pragma once
#ifndef ARROW_EXPORT
#define ARROW_EXPORT __attribute__((visibility("default")))
#endif

#endif //ARROWMEMPOOL_VISIBILITY_H
