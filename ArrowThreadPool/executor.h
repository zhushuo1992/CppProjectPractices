//
// Created by zhushuo on 2023/9/10.
//

#ifndef ARROWTHREADPOOL_EXECUTOR_H
#define ARROWTHREADPOOL_EXECUTOR_H

#pragma once

#include <future>
#include "cancel.h"
#include "status.h"
#include "functions.h"

namespace arrow{

struct TaskHints{
    int32_t priority = 0;
    int64_t io_size = -1;
    int64_t cpu_cost = -1;
    int64_t external_id = -1;
};

class  ARROW_EXPORT Executor{
public:
    //结束后回调函数
    using StopCallback = internal::FnOnce<void(const Status&)>;

    virtual ~Executor();

    virtual Status spawn_real(TaskHints hints, internal::FnOnce<void()> task, StopToken, StopCallback&&) = 0;

    template<typename Function>
    Status spawn(Function&& func)
    {
        return spawn_real(TaskHints{}, std::forward<Function>(func), StopToken::unstoppable(), StopCallback{});
    }


    template<typename Function>
    Status spawn(Function&& func, StopToken token)
    {
        return spawn_real(TaskHints{}, std::forward<Function>(func), std::move(token), StopCallback{});
    }

    template<typename Function>
    Status spawn(TaskHints hints, Function&& func)
    {
        return spawn_real(hints, std::forward<Function>(func), StopToken::unstoppable(), StopCallback{});
    }

    template<typename Function>
    Status spawn(TaskHints hints, Function&& func, StopToken stop_token)
    {
        return spawn_real(hints, std::forward<Function>(func), std::move(stop_token), StopCallback{});
    }

    template<typename Function>
    Status spawn(TaskHints hints, Function&& func, StopToken stop_token, StopCallback stop_callback)
    {
        return spawn_real(hints, std::forward<Function>(func), std::move(stop_token), std::move(stop_callback));
    }

    template<typename Function, typename... Args,
            typename  ReturnType = typename std::result_of<Function(Args...)>::type>
            std::future<ReturnType> submit(TaskHints hints, StopToken stop_token, StopCallback stop_callback, Function&& func,
                                          Args&&... args){
        std::promise<ReturnType> promise; //同步
        std::future<ReturnType> future = promise.get_future();
        auto task = [func = std::forward<Function>(func),
                //tuple（不同类型） array（相同类型） 不可变
                tup = std::make_tuple(std::forward<Args>(args)...),
                promise = std::move(promise)
                ]() mutable {
                    try {
                            if constexpr(!std::is_void_v<ReturnType>)
                            {
                                ReturnType result = std::apply(std::move(func), std::move(tup));
                                promise.set_value(result);
                            }

                            else
                            {
                                std::apply(std::move(func), std::move(tup));
                            }
                    }
                    catch(...)
                    {
                        promise.set_exception(std::current_exception());
                    }
                };

                Status status = spawn_real(hints, std::move(task), stop_token, std::move(stop_callback) );
                if(!status.ok())
                {
                    throw std::runtime_error("Failed to submit task");
                }

                return  future;

    }

    template <typename Function, typename... Args,
            typename ReturnType = typename std::result_of<Function(Args...)>::type>
    std::future<ReturnType> submit(StopToken stop_token, Function&& func, Args&&... args) {
        return submit(TaskHints{}, stop_token, StopCallback{}, std::forward<Function>(func),
                      std::forward<Args>(args)...);
    }

    template <typename Function, typename... Args,
            typename ReturnType = typename std::result_of<Function(Args...)>::type>
    std::future<ReturnType> submit(TaskHints hints, Function&& func, Args&&... args) {
        return submit(std::move(hints), StopToken::unstoppable(), StopCallback{},
                      std::forward<Function>(func), std::forward<Args>(args)...);
    }

    template <typename Function, typename... Args,
            typename ReturnType = typename std::result_of<Function(Args...)>::type>
    std::future<ReturnType> submit(StopCallback stop_callback, Function&& func,
                                   Args&&... args) {
        return submit(TaskHints{}, StopToken::unstoppable(), stop_callback,
                      std::forward<Function>(func), std::forward<Args>(args)...);
    }

    template <typename Function, typename... Args,
            typename ReturnType = typename std::result_of<Function(Args...)>::type>
    std::future<ReturnType> submit(Function&& func, Args&&... args) {
        return submit(TaskHints{}, StopToken::unstoppable(), StopCallback{},
                      std::forward<Function>(func), std::forward<Args>(args)...);
    }




};

}

#endif //ARROWTHREADPOOL_EXECUTOR_H
