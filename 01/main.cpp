/*
 * 本文件主要用来进行编码规范
 */
#include <iostream>
#include <string>

using std::string; //代表下面的string是std::string

#define MAX_PATH_LEN 256 //宏定义采用全大写
int g_sys_flag; //全局变量全小写 g_开头 类似Python风格, 全小写_

namespace  linux_sys{       //命名空间全小写
    void get_rlimit_core(); //函数全小写
}


class FilePath final //类名首字母大写，final表示无法继承, 修饰函数则表示无法重写
{
public:
    void set_path(const string& str); //类成员函数全小写

private:
    string m_path;   //类成员变量全小写 m_开头
    int m_level;
};

/*
 * author: zhushuo
 * date: 2023.08.13
 * purpose: xxxx
 * notice: xxxxx
 */

template<typename T>
int get_value(const T& v);

extern int main2();

int main() {
    std::cout << "Hello, World!" << std::endl;
    main2();
    return 0;
}
