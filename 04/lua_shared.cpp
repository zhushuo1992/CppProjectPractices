//
// Created by zhushuo on 2023/8/27.
//

extern "C"{
    int num = 10;
    int my_add(int a, int b);
}

int my_add(int a, int b)
{
    return a+b;
}