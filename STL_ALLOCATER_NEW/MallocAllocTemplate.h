/*
一级空间配置器
allocate()直接调用malloc
dallocate()直接调用free
模拟C++的set_new_handler处理内存不足的情况
*/
#pragma once  
#include<iostream>
using namespace std;



#include<stdarg.h>
//#define __DEBUG__
FILE* fOut = fopen("trace.log", "w");
static string GetFileName(const string& path)
{
	char ch = '/';
#ifdef _WIN32
	ch = '\\';
#endif
	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
// 用于调试追溯的trace log
inline static void __trace_debug(const char* function,
	const char * filename, int line, char* format, ...)
{
	// 读取配置文件
#ifdef __DEBUG__
	// 输出调用函数的信息
	fprintf(stdout, "【 %s:%d】%s", GetFileName(filename).c_str(), line, function);
	fprintf(fOut, "【 %s:%d】%s", GetFileName(filename).c_str(), line, function);
	// 输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	cout << endl;
	vfprintf(fOut, format, args);
	vfprintf(fOut, "\n", args);
	va_end(args);
#endif
}
#define __TRACE_DEBUG(...)  \
	__trace_debug(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);




template<int inst>
class MallocAllocTemplate
{
protected:
	typedef void (*ALLOCATEHANDER)();
	 static ALLOCATEHANDER __MallocAllocOomHandler;
public:


	//设置函数指针 __MallocAllocOomHandler的值
	static ALLOCATEHANDER SetAllocHandler(void(*f)())
	{
		void(*old)() = __MallocAllocOomHandler;
		__MallocAllocOomHandler = f;
		return old;
	}

	//分配空间
	static void* Allocate(size_t n)
	{

		__TRACE_DEBUG("调用一级空间配置器，分配%u个字节",n);
		//开辟空间
		//若开辟失败，则调用OOM_Malloc函数令系统回收空间
		void *ret = malloc(n);
		if (ret == NULL)
		{
			ret = OOM_Malloc(n);
		}
		return ret;
	}


	//释放空间
	static void Deallocate(void *ptr, size_t/*n*/)
	{
		free(ptr);
	}


	//开辟空间失败后的处理函数
	static void* OOM_Malloc(size_t n)
	{
		ALLOCATEHANDER MyAllocHandler;
		void *ret;

		//不断尝试释放，配置，再释放，再配置。。。
		while (1)
		{
			//将系统回收空间的方法赋给MyAllocHandler
			MyAllocHandler = __MallocAllocOomHandler;
			if (MyAllocHandler == 0)
			{
				throw bad_alloc();
			}
			//调用方法回收空间
			(*MyAllocHandler)();
			//重新开辟
			ret = malloc(n);
			// 判断是否开辟成功，若成功：返回ret，若失败：继续循环开辟；
			if (ret)
			{
				return ret;
			}
		}
	}

};


template<int inst>
typename MallocAllocTemplate<inst>::ALLOCATEHANDER
MallocAllocTemplate<inst>::__MallocAllocOomHandler = 0;