#include <iostream>
#include <thread>
#include "ThreadPool.h"

using namespace std;

void fun1(int sleep_time)
{
    cout<<"hello, fun1 "<<std::this_thread::get_id()<<endl;
    if(sleep_time > 0)
    {
        cout<< std::this_thread::get_id() <<" fun1 sleep "<<sleep_time << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
}


struct gfun
{
    int operator()(int n)
    {
        cout<< n <<" hello gfun "<<std::this_thread::get_id()<<endl;
        return 42;
    }

};

class A{
public:
    static int Afun(int n = 0){
        cout<<n<<" hello Afun "<<std::this_thread::get_id()<<endl;
        return n;
    }

    static string Bfun(int n, string str, char c)
    {
        cout<<n<<" hello Bfun "<<str<< " "<<(int) c<< " " << std::this_thread::get_id() << endl;
        return str;
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;


    try{
        my_thread_pool::ThreadPool executor {50};
        A a;
        std::future<void> ff = executor.commit(fun1, 0);
        std::future<int> fg = executor.commit(gfun{}, 0);
        std::future<int> gg = executor.commit(a.Afun, 9999);
        std::future<string> gh = executor.commit(A::Bfun, 222, "multi args", 123);
        std::future<string> fh = executor.commit([](){cout<<" hello lambda " << std::this_thread::get_id()<<endl; return "hhh"s;});

        cout<<"===1== sleep =====" << std::this_thread::get_id()<<endl;
        std::this_thread::sleep_for(std::chrono::microseconds(900));

        for (int i = 0; i< 50; i++)
        {
            executor.commit(fun1, i*100);
        }

        cout<<"+++++++ commit all ++++++++"<< std::this_thread::get_id()<<endl;

        cout<<"===2== sleep =====" << std::this_thread::get_id()<<endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));


        ff.get(); //等待执行完成，获取返回值
        cout<<fg.get() <<" "<<fh.get()<<" "<<std::this_thread::get_id() <<endl;

        cout<<"===3== sleep =====" << std::this_thread::get_id()<<endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        cout<<"===== fun1, 55 =====" << std::this_thread::get_id()<<endl;
        executor.commit(fun1, 55).get();

        cout<<"end..  "<< std::this_thread::get_id()<<endl;


        my_thread_pool::ThreadPool pool(4);

        vector<std::future<int>> res;

        for(int i = 0; i < 8; ++i)
        {
            res.emplace_back(
                    pool.commit([i]{
                        cout<<"hello "<<i <<endl;
                        std::chrono::seconds(1);
                        cout<<"world "<<i <<endl;
                        return i*i;
                    })
                    );
        }

        cout<<"+++++++ commit all 2 ++++++++"<< std::this_thread::get_id()<<endl;

        for(auto&& result: res)
        {
            cout<<result.get()<<endl;
            cout<<endl;



        }




    }
    catch (std::exception& e)
    {
        cout<<e.what()<<endl;
    }
    return 0;
}
