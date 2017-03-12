/*
һ���ռ�������
allocate()ֱ�ӵ���malloc
dallocate()ֱ�ӵ���free
ģ��C++��set_new_handler�����ڴ治������
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
// ���ڵ���׷�ݵ�trace log
inline static void __trace_debug(const char* function,
	const char * filename, int line, char* format, ...)
{
	// ��ȡ�����ļ�
#ifdef __DEBUG__
	// ������ú�������Ϣ
	fprintf(stdout, "�� %s:%d��%s", GetFileName(filename).c_str(), line, function);
	fprintf(fOut, "�� %s:%d��%s", GetFileName(filename).c_str(), line, function);
	// ����û����trace��Ϣ
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


	//���ú���ָ�� __MallocAllocOomHandler��ֵ
	static ALLOCATEHANDER SetAllocHandler(void(*f)())
	{
		void(*old)() = __MallocAllocOomHandler;
		__MallocAllocOomHandler = f;
		return old;
	}

	//����ռ�
	static void* Allocate(size_t n)
	{

		__TRACE_DEBUG("����һ���ռ�������������%u���ֽ�",n);
		//���ٿռ�
		//������ʧ�ܣ������OOM_Malloc������ϵͳ���տռ�
		void *ret = malloc(n);
		if (ret == NULL)
		{
			ret = OOM_Malloc(n);
		}
		return ret;
	}


	//�ͷſռ�
	static void Deallocate(void *ptr, size_t/*n*/)
	{
		free(ptr);
	}


	//���ٿռ�ʧ�ܺ�Ĵ�����
	static void* OOM_Malloc(size_t n)
	{
		ALLOCATEHANDER MyAllocHandler;
		void *ret;

		//���ϳ����ͷţ����ã����ͷţ������á�����
		while (1)
		{
			//��ϵͳ���տռ�ķ�������MyAllocHandler
			MyAllocHandler = __MallocAllocOomHandler;
			if (MyAllocHandler == 0)
			{
				throw bad_alloc();
			}
			//���÷������տռ�
			(*MyAllocHandler)();
			//���¿���
			ret = malloc(n);
			// �ж��Ƿ񿪱ٳɹ������ɹ�������ret����ʧ�ܣ�����ѭ�����٣�
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