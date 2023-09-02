//
// Created by zhushuo on 2023/8/27.
//

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;

namespace py = pybind11;

class Point final
{
private:
    int x = 0;
public:
    Point() = default;
    ~Point() = default;
    Point(int a):x(a){};
public:
    int get() const
    {
        return x;
    }

    void set(int a)
    {
        x = a;
    }
};

int add2 (int a, int b)
{
    return a+b;
}

PYBIND11_MODULE(pydemo, m)
{
    m.doc = "pybind11 demo doc";

    m.def("info", [](){
        py::print("c++ version = ", __cplusplus);

    });

    m.def("add", [](int a, int b){
        return a+ b;

    });
    m.def("add2", &add2, "cpp add2");

    m.def("use_str", [](const string& str){
        py::print(str);
        return str + "!!";


    });

    m.def("use_tuple", [](tuple<int, int, string> x){
        get<0>(x)++;
        get<1>(x)++;
        get<2>(x)+="?";
        return x;


    });

    m.def("use_list", [](const vector<int>&v){
        auto vv = v;
        py::print("input: ", vv);
        vv.push_back(100);
        return vv;


    });

    py::class_<Point>(m, "Ponit")
            .def(py::init())
            .def(py::init<int>())
            .def("get", &Point::get)
            .def("set", &Point::set);
}