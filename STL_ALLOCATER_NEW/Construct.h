#pragma once
/*
���� inline
�Կռ任ʱ�䣺�ڱ���ʱ��������Ĵ�������������У���˻�
����Ŀ�����Ĵ��������������ӿռ俪����������ʱ���ϵĿ�
����
ע�����
1�����������ڽṹ����̫���ӣ�����ʹ��ѭ����䣬������䡣
2�����������Ķ���������������������һ�α�����֮ǰ
*/

#include<iostream>
using namespace std;



//����
template<typename T1,typename T2>
inline void Construct(T1* p, const T2&  value)
{
	new(p)T1(value);//��λnew���ʽ
}

//����
template<typename T>
inline void Destroy(T* p)
{
	p->~T();
}