//
// Created by zhushuo on 2023/8/27.
//

#include "cpplang.hpp"
#include "SalesData.hpp"
#include "Zmq.hpp"

#include "json.hpp"


using namespace std;

using namespace cpp_study;

static auto debug_print = [](auto & b)
{
    using json_t = nlohmann::json;
    json_t j;
    j["id"] = b.id();
    j["sold"] = b.sold();
    j["revenue"] = b.revenue();

    cout<<j.dump(2)<<endl;
};

static auto make_sales = [](const auto& id, auto s, auto r) {
    return SalesData(id, s, r).pack();
};

static auto send_sales = [](const auto& addr, const auto& buf){
    using zmq_ctx = ZmqContext<1>;
    auto sock = zmq_ctx::send_sock();
    sock.connect(addr);
    assert(sock.connected());

    auto len = sock.send(buf.data(), buf.size());

    assert(len == buf.size());
    cout<<"send len: " <<len<<endl;
};

int main()
try
{
    cout<<"hello client"<<endl;

    send_sales("tcp://127.0.01:5555", make_sales("001", 10, 100));
    this_thread::sleep_for(100ms);
    send_sales("tcp://127.0.01:5555", make_sales("002", 20, 200));
}
catch(exception& e)
{
    cerr<<e.what()<<endl;
}