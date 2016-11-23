#pragma once
#include<iostream>
#include<assert.h>
#include<stack>
using namespace std;

//��������
void InsertSort(int *a, int n)
{
	int index = 1;
	for (; index < n; ++index)
	{
		int pos = index - 1;
		int tmp = a[index];
		while (pos >= 0 && a[pos] > tmp)
		{
			a[pos + 1] = a[pos];
			pos--;//pos=0��ʱ�� pos-- Ϊ-1
		}
		a[pos + 1] = tmp;
	}
}
//��������

//ǰ��ָ�뷨  prev  cur
int NewPortSort(int *a, int begin, int end)
{
	int prev = begin - 1;
	int cur = begin;
	int key = a[end];
	while (cur != end)
	{
		if (a[cur] < key && ++prev != cur)
		{
			swap(a[prev], a[cur]);
		}
		++cur;
	}
	swap(a[++prev], a[end]);
	return prev;
}

//�ڿӷ�
int PortSortPoth(int *a, int begin, int end)
{
	int left = begin;
	int right = end;
	int key = a[end];
	int Poth = end;
	while (left < right)
	{
		while (left < right&&a[left] <= key)
		{
			left++;
		}
		if (left < right)
		{
			a[Poth] = a[left];
			Poth = left;
		}
		while (left < right&&a[right] >= key)
		{
			right--;
		}
		if (left < right)
		{
			a[Poth] = a[right];
			Poth = right;
		}
	}
	a[Poth] = key;
	return Poth;
}

//����ָ�뷨
int PortSort(int *a, int begin, int end)
{
	int left = begin;
	int right = end;
	int key = a[end];
	while (left < right)
	{
		while (left < right && a[left] <= key)
		{
			left++;
		}
		while (left < right && a[right] >= key)
		{
			right--;
		}

		if (left < right)
		{
			swap(a[left], a[right]);
		}
	}
	swap(a[left], a[end]);
	return left;
}

//��λȡ��
int GetMidKey(int *a, int begin, int end)
{
	int mid = begin + ((end - begin) / 2);
	if (a[begin] > a[mid])
	{
		if (a[mid] > a[end])
		{
			return mid;
		}
		else if (a[begin] > a[end])
		{
			return end;
		}
		else
		{
			return begin;
		}
	}
	else
	{
		if (a[begin] > a[end])
		{
			return begin;
		}
		else if (a[mid] > a[end])
		{
			return end;
		}
		else
		{
			return mid;
		}
	}
}

void QuickSort(int *a, int begin, int end)
{
	assert(a);

	//����������ȡ�з��Ż�
	int mid = GetMidKey(a, begin, end);
	swap(a[mid], a[end]);

	if (begin < end)
	{

		//�ݹ����ö�ջ��������������£��������ջ�����
		//�����ڵײ��Բ�����������Ż�������һЩ����£�����Ż�Ч��������
		if (end - begin > 13)
		{
			int div = NewPortSort(a, begin, end);
			QuickSort(a, begin, div - 1);
			QuickSort(a, div + 1, end);
		}
		else
		{
			InsertSort(a + begin, (end - begin) + 1);
		}
	}
}