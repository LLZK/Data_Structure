#pragma once
/*
内联 inline
以空间换时间：在编译时将函数体的代码替代到程序中，因此会
增加目标程序的代码量，进而增加空间开销，而减少时间上的开
销。
注意事项：
1、内联函数内结构不能太复杂，不能使用循环语句，开关语句。
2、内联函数的定义必须出现在内联函数第一次被调用之前
*/

#include<iostream>
using namespace std;



//构造
template<typename T1,typename T2>
inline void Construct(T1* p, const T2&  value)
{
	new(p)T1(value);//定位new表达式
}

//析构
template<typename T>
inline void Destroy(T* p)
{
	p->~T();
}