#pragma once
#include<iostream>
#include<assert.h>
#include<stack>
using namespace std;

//插入排序
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
			pos--;//pos=0的时候 pos-- 为-1
		}
		a[pos + 1] = tmp;
	}
}
//快速排序

//前后指针法  prev  cur
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

//挖坑法
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

//左右指针法
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

//三位取中
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

	//考虑用三数取中法优化
	int mid = GetMidKey(a, begin, end);
	swap(a[mid], a[end]);

	if (begin < end)
	{

		//递归会调用堆栈，海量数据情况下，可能造成栈溢出，
		//所以在底层以插入排序进行优化。但在一些情况下，这个优化效果不明显
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