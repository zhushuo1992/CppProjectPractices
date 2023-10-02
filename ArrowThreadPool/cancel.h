//
// Created by zhushuo on 2023/9/10.
//

#ifndef ARROWTHREADPOOL_CANCEL_H
#define ARROWTHREADPOOL_CANCEL_H

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "status.h"
#include "visibility.h"

namespace arrow{

class StopToken;

struct StopSourceImpl; //里面3个变量，代表是否停止

class ARROW_EXPORT StopSource{

public:
    StopSource();
    ~StopSource();

    void request_stop();
    void request_stop(Status error);
    void request_stop_from_signal(int signum);

    StopToken token();

    void reset();

protected:
    std::shared_ptr<StopSourceImpl> m_impl;
};

class ARROW_EXPORT StopToken{
protected:
    std::shared_ptr<StopSourceImpl> m_impl;
public:
    StopToken() {}

    explicit StopToken(std::shared_ptr<StopSourceImpl> impl): m_impl(std::move(impl)) {}

    static auto unstoppable() {return StopToken();}

    Status poll() const;
    bool is_stop_requested() const;
};

}



#endif //ARROWTHREADPOOL_CANCEL_H
