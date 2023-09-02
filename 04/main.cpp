#include <iostream>
#include <cassert>
#include <cpr/cpr.h>

using namespace std;

void case1()
{
    const auto url = "http://openresty.org"s;
    auto res = cpr::Get(cpr::Url{url});

    cout<< res.elapsed<<endl;
    cout<< res.url<<endl;
    cout<< res.status_code<<endl;

}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
