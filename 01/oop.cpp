/*
 * 本文件主要是表面类内设计的一些规范建议
 */

#include <iostream>
#include <string>
#include <vector>
#include <set>

#if 0

#endif

class Interface{};

class Implement final:  public Interface
{
public:
    using super_type = Interface;
    using this_type = Implement;
private:
};

struct KafkaConfig final
{
    int id;
    std::string ip_addr;
};

class DemoClass final
{
public:
    using this_type = DemoClass;
    using kafka_conf_type = KafkaConfig;
public:
    using string_type = std::string;
    using vector_type = std::vector<string_type>;
    using uint32_type = uint32_t;
    using set_type = std::set<int>;
public:
    DemoClass() = default; //无参数使用编译器默认构造函数
    ~DemoClass() = default;
    DemoClass(const DemoClass&) = delete; //禁止拷贝构造
    DemoClass& operator=(const DemoClass&) = delete; //禁止赋值

public:
    explicit DemoClass(const string_type& str) //禁止隐式类型转换
    {
        m_name = str;
    }

    explicit operator bool ()
    {
        return !m_name.empty();
    }

private:
    string_type  m_name = "zhushuo";
    uint32_type m_age = 30;
    set_type m_books;
    kafka_conf_type m_conf;
};

class DemoDelegating final
{
private:
    int a=0;
public:
    DemoDelegating(int x): a(x)
    {}

    DemoDelegating(): DemoDelegating(0)
    {}

    DemoDelegating(const std::string &s):DemoDelegating(std::stoi(s))
    {}
};

class DemoInit final
{
private:
    int a =0;
    std::string s = "hello";
    std::vector<int> v{1,2,3};
public:
    DemoInit() = default;
    ~DemoInit() = default;
    DemoInit(int x) : a(x){}

};
