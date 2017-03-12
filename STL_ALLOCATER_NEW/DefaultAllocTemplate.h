#define _CRT_SECURE_NO_WARNINGS 1

#pragma once

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
using namespace std;

#include"MallocAllocTemplate.h"

// Trace ����


template<bool threads,int inst>
class DefaultAllocTemplate
{
protected:
	enum{_ALIGN = 8};
	enum{_MAX_BYTES = 128};
	enum{_NFREELISTS = _MAX_BYTES/_ALIGN};

	//���������¹ҵĽ�������
	union obj
	{
		obj* FreeListLink;//ָ����һ������ڴ���ָ��
		char ClientData[1];
	};

	static obj* FreeList[_NFREELISTS];


	//��n������8�ı���
	static size_t ROUND_UP(size_t n)
	{
		return((n) + _ALIGN - 1)& ~(_ALIGN - 1);
	}

	//����Freelist�е��±�
	static size_t _FREELIST_INDEX(size_t n)
	{
		return((n + _ALIGN - 1) / _ALIGN - 1);
	}
	//ָ���ڴ�ؿ��ÿռ��ͷ
	static char* Start_Free;
	//ָ���ڴ�ؿ��ÿռ��βo
	static char* End_Free;
	//�������ռ�ʱ���� Chunk_Malloc
	static size_t Heap_Size;
public:
	//����ռ�
	static void* Allocate(size_t n)
	{

		//������Ŀռ����128�ֽڣ�����һ���ռ�������
		if (n > (size_t)_MAX_BYTES)
		{
			__TRACE_DEBUG("����128�ֽڣ�����һ���ռ�������������%d���ֽ�",n);
			return(MallocAllocTemplate<inst>::Allocate(n));
		}

		__TRACE_DEBUG("С��128�ֽڣ����ö����ռ�����������ȥ���������в���:")
		obj** MyFreeList;//��������
		obj* result;//����ֵ
		
		//_FREELIST_INDEX����n�����±�
		//MyFreeList�ҵ���Ӧ��С�ڴ��������������λ��
		MyFreeList = FreeList + _FREELIST_INDEX(n);
		result = *MyFreeList;//����һ���ڴ���result

		//������ʧ�ܣ�������������û����Ӧ�ڴ��
		//����Refill�������ڴ�����롣
		if (result == NULL)
		{
			
			void* r = Refill(ROUND_UP(n));//ROUND_UP��n������8�ı���
			return r;
		}

		__TRACE_DEBUG("���������д���%u��С�����飬���䣬���أ�����", n);
		//������������ָ��ָ����һ���ڴ��
		*MyFreeList = result->FreeListLink;
		return result;

	}
	//�ͷſռ�
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

	//���ڴ������ռ�
	static void* Refill(size_t n)
	{
		__TRACE_DEBUG("��������û������Ҫ���飬����Refill�����ڴ������");
		int nobjs = 20;// ��׼ֵ������20���ڴ��

		char* chunk = NULL;


		//����ڴ�ز����Կ���nobjs���ڴ��ͻ��޸�nobjs��ֵ��
		//�ڴ��ʵ���ܿ��ٶ��ٸ���СΪsize���ڴ��ͽ�nobjs��ֵ��Ϊ����
		chunk = ChunkAlloc(n, nobjs);//���ڴ�������ڴ�

		obj**MyFreeList;//��������
		obj* cur_obj = NULL;//ָ��ǰ�ڴ���ָ��
		obj* next_obj = NULL;//ָ����һ���ڴ���ָ��
		obj* result = NULL;//����ֵ


		//��nbjs = 1�����ڴ��ֻʣ��һ��size��С���ڴ�
		//���䷵��
		if (nobjs == 1)
		{
			__TRACE_DEBUG("�ڴ��ֻ���Է���1��%u�ֽڴ�С�����飬���䷵�أ��������", n);
			return (obj*)chunk;
		}



		__TRACE_DEBUG("�ڴ�ط�����%d�������飬������һ�����أ�ʣ�µ�%d��ҵ�����������", nobjs, nobjs - 1);
		//��nobjs>1�����ڴ�ػ��ж��size��С���ڴ档
		//����һ�����أ���ʣ�µ���������������
		
		//׼����������������������λ�ã��±꣩
		MyFreeList = FreeList + _FREELIST_INDEX(n);

		// ����һ���óɷ���ֵ
		result = (obj*)chunk;

		//  ��ȡ����һ�顱�ռ�
		next_obj = (obj*)(chunk + n);

		//  ����������ָ����һ�顱�ռ�
		*MyFreeList = next_obj;
		
		//�ٴζ�ȡ��һ��ռ�
		//next_obj = (obj*)((char*)next_obj + n);

		
	    //��ʣ�µ�����
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

	//�ڴ�ط���
	static char* ChunkAlloc(size_t size, int &nobjs)
	{
		obj**MyFreeList;
		char* ret = NULL;
		size_t totalBytes = nobjs * size;
		__TRACE_DEBUG("����ChunkAlloc�������ڴ���������飺%u��%d����%u�ֽ�", nobjs, size, totalBytes);

		size_t Bytes_left = End_Free - Start_Free;

		if (Bytes_left >= totalBytes)
		{
			__TRACE_DEBUG("�ڴ���ڴ��㹻�������ˣ�%u��%d����%u�ֽ�", nobjs, size, totalBytes);
			ret = Start_Free;
			Start_Free += totalBytes;
			return ret;
		}
		else if (Bytes_left >= size)
		{
			nobjs = Bytes_left / size;
			//ע���޸�totalBytes��ʣ�µ��ֽڿ��ܲ����Կ���������size��С������
			//������ʣ��
			totalBytes = nobjs * size;
			__TRACE_DEBUG("�ڴ���ڴ治�㣬�����ٿ��Է���1����ʵ�ʿɷ���:%d��%u����%u�ֽ�", nobjs, size, totalBytes);
			ret = Start_Free;
			Start_Free += totalBytes;
			return ret;
		}
		else
		{
			__TRACE_DEBUG("�ڴ�ظɺԣ���1��%u���ֽڵ����鶼�޷�����",size);
			//�Ȳ鿴�ڴ���Ƿ���ʣ���ڴ棬��ʣ���ڴ�ҵ�����������
			if (Bytes_left > 0)
			{
				__TRACE_DEBUG("�ڴ��ʣ��%u���ֽڣ�����ҵ����������ϣ���ֹ�˷�", Bytes_left);
				MyFreeList = FreeList + _FREELIST_INDEX(Bytes_left);

				((obj*)Start_Free)->FreeListLink = *MyFreeList;
				*MyFreeList = (obj*)Start_Free;
				Start_Free += Bytes_left;
			}

			//��malloc��ϵͳ�����ڴ�
			//��Ҫ������ڴ�Ĵ�С������8�ı���
			//�����룬һ���ַ����ȥ��һ���ִ����ڴ��

			size_t BytesToGet = totalBytes * 2 + ROUND_UP(Heap_Size >> 4);

			__TRACE_DEBUG("����malloc����ϵͳ����%u���ֽ��ڴ�", BytesToGet);
			Start_Free = (char*)malloc(BytesToGet);//malloc����ֵΪvoid*

			//��malloc����ʧ��
			if (Start_Free == NULL)
		    {
				__TRACE_DEBUG("��ϵͳ����ʧ�ܣ�ȥ������������Ƿ��б�&u��Ŀ�������", size);
				//�ȱ���������������Ƿ��б�size�������
				for (size_t i = size; i < _MAX_BYTES; i += _ALIGN)
				{
					MyFreeList = FreeList + _FREELIST_INDEX(i);
					obj* p = *MyFreeList;
					if (p != NULL)
					{
						__TRACE_DEBUG("����������������%u�ֽڵĿ������飬����Start,End,�ݹ����ChunkAlloc", i);
						Start_Free = (char*)p;
						End_Free = Start_Free + i;
						//������size����������ڴ�أ��ݹ��ٴε���ChunkAlloc���������ٷ���1��������
						//һ������ɹ�
						return ChunkAlloc(size, nobjs);
					}

				}

				__TRACE_DEBUG("����������û�п������飬��End�ÿգ�����һ���ռ������������������");
				End_Free = 0;//
		        //����һ���ռ������������������
				//һ���ռ��������������Ѳ���ϵͳ�ͷ��ڴ�Ļ��ơ��ڴ治�㴦����ơ�
				//��һ���ռ�����������ʧ�ܣ����ڴ治�㴦����ơ�û�б����ã�����ֱ�ӻ��׳�bad_alloc�쳣
				//��Ҳ��Ϊʲô�����潫End_Free�óɿգ�
				//һ����˵�������쳣���ǲ���ֱ����ֹ���򣬶����ٴγ���ִ���ظ�����
				//���Ե���bad_alloc�쳣����������ǿ��ܻ��ٴε���ChunkAlloc���пռ����
				//������End_Free�óɿ�
				//���ٴμ���Bytes_leftʱ��Start_Free��Ϊ�ռ����ʧ�ܶ���NULL,End_Free��һ�������ֵ
				//�����������Bytes_left���ر����ɲ���Ԥ֪�Ĵ���
				Start_Free = (char*)MallocAllocTemplate<inst>::Allocate(BytesToGet);
			}


			__TRACE_DEBUG("malloc����ɹ�������Start��End���ݹ����ChunkAlloc");
			End_Free = Start_Free + BytesToGet;
			Heap_Size += BytesToGet;
			return ChunkAlloc(size, nobjs);
		    
		}

		//�ڴ�����Է���size*nobjs���ֽڵĿռ�
	    //�ڴ�ز���size*nobjs���ֽڣ������Է���1������size���ֽڵĿռ�
		//�ڴ�ز����Է���1��size
	}

};

//�����������óɿ�
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