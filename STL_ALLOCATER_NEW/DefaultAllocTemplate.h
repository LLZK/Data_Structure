#define _CRT_SECURE_NO_WARNINGS 1

#pragma once

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
using namespace std;

#include"MallocAllocTemplate.h"

// Trace 跟踪


template<bool threads,int inst>
class DefaultAllocTemplate
{
protected:
	enum{_ALIGN = 8};
	enum{_MAX_BYTES = 128};
	enum{_NFREELISTS = _MAX_BYTES/_ALIGN};

	//自由链表下挂的结点的类型
	union obj
	{
		obj* FreeListLink;//指向下一个结点内存块的指针
		char ClientData[1];
	};

	static obj* FreeList[_NFREELISTS];


	//将n上升到8的倍数
	static size_t ROUND_UP(size_t n)
	{
		return((n) + _ALIGN - 1)& ~(_ALIGN - 1);
	}

	//计算Freelist中的下标
	static size_t _FREELIST_INDEX(size_t n)
	{
		return((n + _ALIGN - 1) / _ALIGN - 1);
	}
	//指向内存池可用空间的头
	static char* Start_Free;
	//指向内存池可用空间的尾o
	static char* End_Free;
	//向堆申请空间时所用 Chunk_Malloc
	static size_t Heap_Size;
public:
	//分配空间
	static void* Allocate(size_t n)
	{

		//若申请的空间大于128字节，调用一级空间配置器
		if (n > (size_t)_MAX_BYTES)
		{
			__TRACE_DEBUG("大于128字节，调用一级空间配置器，分配%d个字节",n);
			return(MallocAllocTemplate<inst>::Allocate(n));
		}

		__TRACE_DEBUG("小于128字节，调用二级空间配置器，先去自由链表中查找:")
		obj** MyFreeList;//自由链表
		obj* result;//返回值
		
		//_FREELIST_INDEX利用n计算下标
		//MyFreeList找到相应大小内存块在自由链表中位置
		MyFreeList = FreeList + _FREELIST_INDEX(n);
		result = *MyFreeList;//将第一个内存块给result

		//若分配失败，即自由链表中没有相应内存块
		//调用Refill函数向内存池申请。
		if (result == NULL)
		{
			
			void* r = Refill(ROUND_UP(n));//ROUND_UP将n上升到8的倍数
			return r;
		}

		__TRACE_DEBUG("自由链表中存在%u大小的区块，分配，返回，结束", n);
		//令自由链表中指针指向下一个内存块
		*MyFreeList = result->FreeListLink;
		return result;

	}
	//释放空间
	static void Deallocate(void* p, size_t n)
	{
		obj** MyFreeList;
		obj* rb = (obj*)p;

		if (n > _MAX_BYTES)
		{
			MallocAllocTemplate<inst>::Deallocate(p, n);
		}

		MyFreeList = FreeList + _FREELIST_INDEX(n);
		rb->FreeListLink = *MyFreeList;
		*MyFreeList = rb;
	}

	//向内存池申请空间
	static void* Refill(size_t n)
	{
		__TRACE_DEBUG("自由链表没有所需要区块，调用Refill，向内存池申请");
		int nobjs = 20;// 标准值：开辟20个内存块

		char* chunk = NULL;


		//如果内存池不足以开辟nobjs个内存块就会修改nobjs的值。
		//内存池实际能开辟多少个大小为size的内存块就将nobjs的值设为多少
		chunk = ChunkAlloc(n, nobjs);//向内存池申请内存

		obj**MyFreeList;//自由链表
		obj* cur_obj = NULL;//指向当前内存块的指针
		obj* next_obj = NULL;//指向下一个内存块的指针
		obj* result = NULL;//返回值


		//若nbjs = 1，即内存池只剩下一个size大小的内存
		//将其返回
		if (nobjs == 1)
		{
			__TRACE_DEBUG("内存池只足以分配1个%u字节大小的区块，将其返回，程序结束", n);
			return (obj*)chunk;
		}



		__TRACE_DEBUG("内存池分配了%d个的区块，将其中一个返回，剩下的%d块挂到自由链表上", nobjs, nobjs - 1);
		//若nobjs>1，即内存池还有多个size大小的内存。
		//将第一个返回，将剩下的链到自由链表上
		
		//准备自由链表，设置自由链表位置（下标）
		MyFreeList = FreeList + _FREELIST_INDEX(n);

		// 将第一个置成返回值
		result = (obj*)chunk;

		//  读取“下一块”空间
		next_obj = (obj*)(chunk + n);

		//  将自由链表指向“下一块”空间
		*MyFreeList = next_obj;
		
		//再次读取下一块空间
		//next_obj = (obj*)((char*)next_obj + n);

		
	    //将剩下的链上
		for (int i = 1; i < nobjs; i++)
		{
			cur_obj = next_obj;
			if (i == nobjs - 1)
			{
				cur_obj->FreeListLink = NULL;
				break;
			}	
			next_obj = (obj*)((char*)next_obj + n);
			cur_obj->FreeListLink = next_obj;
		}
		
		return result;

	}

	//内存池分配
	static char* ChunkAlloc(size_t size, int &nobjs)
	{
		obj**MyFreeList;
		char* ret = NULL;
		size_t totalBytes = nobjs * size;
		__TRACE_DEBUG("调用ChunkAlloc函数向内存池申请区块：%u个%d，共%u字节", nobjs, size, totalBytes);

		size_t Bytes_left = End_Free - Start_Free;

		if (Bytes_left >= totalBytes)
		{
			__TRACE_DEBUG("内存池内存足够，分配了：%u个%d，共%u字节", nobjs, size, totalBytes);
			ret = Start_Free;
			Start_Free += totalBytes;
			return ret;
		}
		else if (Bytes_left >= size)
		{
			nobjs = Bytes_left / size;
			//注意修改totalBytes，剩下的字节可能不足以开辟整数个size大小的区块
			//可能有剩余
			totalBytes = nobjs * size;
			__TRACE_DEBUG("内存池内存不足，但至少可以分配1个，实际可分配:%d个%u，共%u字节", nobjs, size, totalBytes);
			ret = Start_Free;
			Start_Free += totalBytes;
			return ret;
		}
		else
		{
			__TRACE_DEBUG("内存池干涸，连1个%u个字节的区块都无法分配",size);
			//先查看内存池是否还有剩余内存，将剩余内存挂到自由链表上
			if (Bytes_left > 0)
			{
				__TRACE_DEBUG("内存池剩余%u个字节，将其挂到自由链表上，防止浪费", Bytes_left);
				MyFreeList = FreeList + _FREELIST_INDEX(Bytes_left);

				((obj*)Start_Free)->FreeListLink = *MyFreeList;
				*MyFreeList = (obj*)Start_Free;
				Start_Free += Bytes_left;
			}

			//用malloc向系统申请内存
			//将要申请的内存的大小上升至8的倍数
			//多申请，一部分分配出去，一部分存入内存池

			size_t BytesToGet = totalBytes * 2 + ROUND_UP(Heap_Size >> 4);

			__TRACE_DEBUG("调用malloc，向系统申请%u个字节内存", BytesToGet);
			Start_Free = (char*)malloc(BytesToGet);//malloc返回值为void*

			//若malloc分配失败
			if (Start_Free == NULL)
		    {
				__TRACE_DEBUG("向系统申请失败，去自由链表查找是否有比&u大的可用区块", size);
				//先遍历自由链表查找是否有比size大的区块
				for (size_t i = size; i < _MAX_BYTES; i += _ALIGN)
				{
					MyFreeList = FreeList + _FREELIST_INDEX(i);
					obj* p = *MyFreeList;
					if (p != NULL)
					{
						__TRACE_DEBUG("发现自由链表中有%u字节的可用区块，重置Start,End,递归调用ChunkAlloc", i);
						Start_Free = (char*)p;
						End_Free = Start_Free + i;
						//将大于size的区块编入内存池，递归再次调用ChunkAlloc，满足至少分配1个的条件
						//一定分配成功
						return ChunkAlloc(size, nobjs);
					}

				}

				__TRACE_DEBUG("自由链表中没有可用区块，将End置空，调用一级空间配置器进行最后挣扎");
				End_Free = 0;//
		        //调用一级空间配置器进行最后挣扎
				//一级空间配置器中有提醒操作系统释放内存的机制“内存不足处理机制”
				//若一级空间配置器开辟失败（“内存不足处理机制”没有被设置），则直接会抛出bad_alloc异常
				//这也是为什么在上面将End_Free置成空，
				//一般来说当捕获到异常我们不会直接终止程序，而会再次尝试执行重复操作
				//所以当在bad_alloc异常被捕获后，我们可能会再次调用ChunkAlloc进行空间分配
				//若不将End_Free置成空
				//当再次计算Bytes_left时，Start_Free因为空间分配失败而是NULL,End_Free是一个极大的值
				//这样算出来的Bytes_left就特别大，造成不可预知的错误。
				Start_Free = (char*)MallocAllocTemplate<inst>::Allocate(BytesToGet);
			}


			__TRACE_DEBUG("malloc分配成功，重置Start，End，递归调用ChunkAlloc");
			End_Free = Start_Free + BytesToGet;
			Heap_Size += BytesToGet;
			return ChunkAlloc(size, nobjs);
		    
		}

		//内存池足以分配size*nobjs个字节的空间
	    //内存池不足size*nobjs个字节，但可以分配1个以上size个字节的空间
		//内存池不足以分配1个size
	}

};

//将自由链表置成空
template<bool threads,int inst>
typename DefaultAllocTemplate<threads, inst>::obj*
DefaultAllocTemplate<threads, inst>::FreeList[_NFREELISTS] = { 0 };

template<bool threads,int inst>
char* DefaultAllocTemplate<threads, inst>::Start_Free = 0;
template<bool threads, int inst>
char* DefaultAllocTemplate<threads, inst>::End_Free = 0;
template<bool threads, int inst>
size_t DefaultAllocTemplate<threads, inst>::Heap_Size = 0;


////////////////////////////////////////////////////////////


#ifdef __USR_MALLOC
typedef MallocAllocTemplate<0> Alloc;
#else
typedef DefaultAllocTemplate<0, 0> Alloc;
#endif  //__USR_MALLOC



template<typename T,typename __Alloc>
class SimpleAlloc
{
public:

	static T* Allocate(size_t n)
	{
		return n == 0 ? 0 : (T*)__Alloc::Allocate(n * sizeof(T));
	}
	static T* Allocate(void)
	{
		return (T*)__Alloc::Allocate(sizeof(T));
	}
	static void Deallocate(T*p, size_t n)
	{
		if (n != 0)
		{
			__Alloc::Deallocate(p, sizeof(T)*n);
		}
	}
	static void Deallocate(T*p)
	{
		if (p != NULL)
		{
			__Alloc::Deallocate(p, sizeof(T));
		}
	}
};