//
// Created by zhushuo on 2023/9/10.
//

#ifndef ARROWTHREADPOOL_STATUS_H
#define ARROWTHREADPOOL_STATUS_H

#pragma once

#include <cstring>
#include <iostream>
#include <string>

enum class StatusCode {
    INVALID = -1,
    OK = 0,
    CANCELLED = 1,
    KEY_ERROR = 2
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

    bool ok() const {return m_code == StatusCode::OK;}

    static auto make_ok()
    {
        return Status();
    }

    static auto make_invalid(const str_t& msg)
    {
        return Status(StatusCode::INVALID, msg);
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


#endif //ARROWTHREADPOOL_STATUS_H
