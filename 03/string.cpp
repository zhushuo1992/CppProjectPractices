//
// Created by zhushuo on 2023/8/26.
//

#include <cassert>
#include <iostream>
#include <string>
#include <regex>

using namespace std;

void case21()
{
    string str = "abc";
    cout<<str.length()<<" "<< (str<"xyz") << (str.substr(0, 1) == "a") <<endl;
    assert(str[1] == 'b');
    assert(str.find("1") == string::npos);
    assert(str+"d" == "abcd");
}

void case22()
{
    //enable s
    using namespace std::literals::string_literals;

    auto str = "std string"s; //不然是const char*

    assert("time"s.size() == 4);

}

void case23()
{

    auto str = R"(nier:automata)"; //原始字符串
    auto str1 = R"(char""'')";
    auto str2 = R"(\r\b\t\")";
    auto str3 = R"(\\\$)";
    auto str4 = "\\\\\\$";
    auto str5 = R"==(R"(xxx)")==";

    cout<< str <<endl;
    cout<< str1 <<endl;
    cout<< str2 <<endl;
    cout<< str3 <<endl;
    cout<< str4 <<endl;
    cout<< str5 <<endl;


}

void case24()
{
    assert(stoi("42") == 42);
    assert(stoi("421") == 421L);
    assert(stod("4.2") == 4.2);
    assert(to_string(1984) == "1984");
}

class my_string_view final
{
public:
    using this_type = my_string_view;
    using string_type = std::string;
    using string_ref_type = const std::string&;
    using char_ptr_type = const char*;
    using size_type = size_t;
private:
    char_ptr_type ptr = nullptr;
    size_type len = 0;
public:
    my_string_view() = default;
    ~my_string_view() = default;

    my_string_view(string_ref_type str) noexcept : ptr(str.data()), len(str.length())
    {
    }

public:
    char_ptr_type data() const
    {
        return ptr;
    }

    size_type size() const
    {
        return len;
    }
};

static auto make_regex = [](const auto& x) {
    return std::regex(x);
};

static auto make_match = []() {
    return std::smatch();
};

void case25()
{
    auto str = "neir:automata"s;
    auto reg = make_regex(R"(^(\w+)\:(\w+)$)");
    auto what = make_match();
    assert(regex_match(str, what, reg));

    for (const auto& x : what)
    {
        cout<<x<<endl;
    }
}

void case26()
{
    auto str = "god of war"s;
    auto reg = make_regex(R"((\w+)\s(\w+))");
    auto what = make_match();

    auto found = regex_search(str, what, reg);

    assert(found);
    assert(!what.empty());
    assert(what[1] == "god");
    assert(what[2] == "of");

    auto new_str = regex_replace(str, make_regex(R"(\w+$)"), "peace");

    cout<<new_str<<endl;

}

int main3 ()
{
    case21();
    case22();
    case23();
    case24();
    case25();
    case26();
}