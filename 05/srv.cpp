//
// Created by zhushuo on 2023/8/29.
//

#include "cpplang.hpp"
#include "Summary.hpp"
#include "Zmq.hpp"
#include "Config.hpp"
#include "json.hpp"

#include <cstdio>
#include <cpr/cpr.h>

using namespace std;
using namespace cpp_study;

static auto debug_print = [] (auto& b)
{
    using json_t = nlohmann::json;
    json_t j;

    j["id"] = b.id();
    j["sold"] = b.sold();
    j["revenue"] = b.revenue();

    cout<<j.dump(2)<<endl;
};

int main()
try
{
    cout<<"server start"<<endl;
    Config conf;
    conf.load("./config.lua");
    Summary sum;
    atomic_int count {0};

    auto recv_cycle = [&]()
    {
        using zmq_ctx = ZmqContext<1>;
        auto sock = zmq_ctx::recv_sock();
        sock.bind(conf.get<string>("config.zmq_ipc_addr")); //string指定返回值类型
        assert(sock.connected());

        for(;;)
        {
            auto msg_ptr = make_shared<zmq::zmq_message_t>();
            sock.recv(msg_ptr.get());
            ++count;
            cout<<count<<endl;

            std::thread([&sum, msg_ptr](){
                SalesData book;
                auto obj = msgpack::unpack(msg_ptr->data<char>(), msg_ptr->size()).get();
                obj.convert(book);
                sum.add_sales(book);
            }).detach();
        }
    };


    auto log_cycle = [&]()
    {
        auto http_addr = conf.get<string>("config.http_addr");
        auto time_interval = conf.get<int>("config.time_interval");
        for(;;)
        {
            std::this_thread::sleep_for(time_interval*1s);
            using json_t = nlohmann::json;
            json_t j;
            j["count"] = static_cast<int>(count);
            j["minmax"] = sum.minmax_sales();

            auto res = cpr::Post(
                    cpr::Url{http_addr},
                    cpr::Body(j.dump()),
                    cpr::Timeout{200ms}
                    );
            if(res.status_code != 200)
            {
                cerr<<"http post failed"<<endl;
            }
        }
    };

    auto fu1 = std::async(std::launch::async, log_cycle); //函数f必须异步执行，即在另一线程执行
    auto fu2 = std::async(std::launch::async, recv_cycle);

    fu2.wait();

}
catch (std::exception& e)
{
    std::cerr <<e.what()<<std::endl;
}
