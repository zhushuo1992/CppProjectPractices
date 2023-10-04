//
// Created by zhushuo on 2023/10/2.
//

#ifndef ARROWMEMPOOL_STATUS_H
#define ARROWMEMPOOL_STATUS_H

#pragma once

#include <cstring>
#include <iostream>
#include "string_builder.h"

enum class StatusCode {
    INVALID = -1,
    OK = 0,
    CANCELLED = 1,
    KEY_ERROR = 2,
    CAPACITY_ERROR = 3,
    OUT_OF_MEMORY = 4
};

class Status
{
    StatusCode m_code;
    std::string m_msg;
public:
    using str_t = std::string;
    Status(): m_code(StatusCode::OK) {}
    explicit Status(StatusCode code): m_code(code){}

    Status(StatusCode code, const str_t& msg): m_code(code), m_msg(msg) {}

    auto code() const {return m_code;}

    const std::string& message() const {return m_msg;}

    bool ok() const {return m_code == StatusCode::OK;}

    static auto make_ok()
    {
        return Status();
    }

    template<typename... Args>
    static Status from_args(StatusCode code, Args&&... args)
    {
        return Status(code, arrow::util::string_builder(std::forward<Args>(args)...));
    }

    static auto make_invalid(const str_t& msg)
    {
        return Status(StatusCode::INVALID, msg);
    }

    template<typename... Args>
    static Status make_invalid(Args&&... args)
    {
        return Status::from_args(StatusCode::INVALID, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static Status make_capacity_error(Args&&... args)
    {
        return Status::from_args(StatusCode::CAPACITY_ERROR, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static Status make_out_of_memory(Args&&... args)
    {
        return Status::from_args(StatusCode::OUT_OF_MEMORY, std::forward<Args>(args)...);
    }

    static auto make_cancelled(const str_t& msg)
    {
        return Status(StatusCode::CANCELLED, msg);
    }

    static auto make_keyerr(const str_t& msg)
    {
        return Status(StatusCode::KEY_ERROR, msg);
    }

    auto to_string() const
    {
        str_t status_string;

        switch (m_code) {
            case StatusCode::OK:
                status_string = "OK";
                break;
            case StatusCode::CANCELLED:
                status_string = "Cancelled";
                break;
            default:
                status_string = "Unknown";
                break;

        }

        if(!m_msg.empty())
        {
            status_string += ": " + m_msg;
        }

        return status_string;
    }

    void abort(const str_t& message) const
    {
        std::cerr<<"----Arrow Fatal Error----"<<std::endl;
        if(!message.empty())
        {
            std::cerr<<message<<std::endl;
        }

        std::cerr<<to_string()<<std::endl;
        std::abort();
    }

};

#define ARROW_RETURN_IF_(condition, status, _) \
do {                                        \
    if(ARROW_PREDICT_FALSE(condition)) {    \
        return (status);                                        \
    }                                            \
} while (0)

#define ARROW_RETURN_NOT_OK(status) \
do{                                 \
    ARROW_RETURN_IF_(!status.ok(), status, ARROW_STRINGIFY(status)); \
} while(0)

#define  RETURN_NOT_OK(s) ARROW_RETURN_NOT_OK(s)
#endif //ARROWMEMPOOL_STATUS_H
