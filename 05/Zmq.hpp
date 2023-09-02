//
// Created by zhushuo on 2023/8/27.
//

#ifndef INC_05_ZMQ_HPP
#define INC_05_ZMQ_HPP

#include "cpplang.hpp"
#include <zmq.hpp>

namespace cpp_study{
    using zmq_context_t = zmq::context_t;
    using zmq_socket_t = zmq::socket_t;
    using zmq_message_t = zmq::message_t;

    template<int thread_num = 1>
    class ZmqContext final
    {
    public:
        ZmqContext() = default;
        ~ZmqContext() = default;

    public:
        static zmq_context_t& context()
        {
            static zmq_context_t ctx(thread_num);
            return  ctx;
        }

    public:
        static zmq_socket_t& send_sock(int hwm = 1000, int linger = 10)
        {
            zmq_socket_t sock(context(), ZMQ_PUSH);

            sock_setsockopt(ZMQ_SNDHWM, hwm);
            sock_setsockopt(ZMQ_LINGER, linger);

            return sock;
        }
    };
}

#endif //INC_05_ZMQ_HPP
