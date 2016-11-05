#pragma once
#include<iostream>
#include<vector>
#include<cassert>
using namespace std;

//仿函数
template<typename T>
struct Max
{
public:
	bool operator()(const T& x1,const T& x2)
	{
		return x1 > x2;
	}
};

template<typename T>
struct Min
{
public:
	bool operator()(const T& x1, const T& x2)
	{
		return x1 < x2;
	}
};

template<typename T,typename Compare = Min<T>>
class Heap
{
public:
	//建堆
	Heap()
	{}
	Heap(const T* a,int sz)
	{
		//assert(a);
		_a.resize(sz);
		for (int i = 0; i < sz;i++)
		{
			_a[i] = a[i];
		}
		for (int i = (_a.size() - 2) / 2; i >= 0;i--)
		{
			AdjustDown(i);
		}
	}
	//插入
	void Push(const T& x)
	{
		_a.push_back(x);
		AdjustUp(_a.size() - 1);
	}
	//删除
	void Pop()
	{
		assert(!_a.empty());
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		AdjustDown(0);
	}
	T& Top()
	{
		return _a.front();
	}
	size_t Size()
	{
		return _a.size();
	}
	bool Empty()
	{
		return _a.empty();
	}
	void DisPlay()
	{
		for (size_t i = 0; i < _a.size(); i++)
		{
			cout << _a[i] << " ";
		}
		cout << endl;
	}


protected:
	//向下调整
	void AdjustDown(int root)
	{
		int parent = root;
		size_t child = parent * 2 + 1;
		Compare com;
		while (child < _a.size())
		{
			if (child + 1 < _a.size() && com(_a[child],_a[child+1]))
			{
				child++;
			}
			if (!com(_a[child] , _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//向上调整
	void AdjustUp(int root)
	{
		int child = root;
		int parent = (child - 1) / 2;
		Compare com;
		while (child > 0)
		{
			if (!com(_a[child] ,_a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}

protected:
	vector<T> _a;
};

//优先级队列
//每次访问优先级最高的
template<typename T,typename Compare>
class PriorityQueue
{
public:
	void Push(const T& x)
	{
		_q.Push(x);
	}
	void Pop()
	{
		_q.Pop();
	}
	size_t Size()
	{
		return _q._a.size();
	}
	T& Top()
	{
		return _q._a.front();
	}
	bool Empty()
	{
		return _q._a.empty();
	}
private:
	Heap<T, Max<T>> _q;
};

void Down(int* a, int pos, int sz)
{
	int parent = pos;
	int child = parent * 2 + 1;
	while (child < sz)
	{
		//右孩子大于左孩子
		if (child + 1 < sz && a[child + 1] > a[child])
		{
			child++;
		}
		//孩子结点大于父亲
		if (a[child] > a[parent])
		{
			swap(a[child], a[parent]);
			parent = child;
			child = parent * 2 + 1;
		}
		//孩子结点小于父亲
		else
		{
			break;
		}
	}
}


//堆排序
//时间复杂度O(N*lgN)

void HeapSort(int* a, int sz)
{
	assert(a);
	for (int i = (sz-2)/2; i >= 0; i--)
	{
		Down(a, i, sz);
	}
	int end = sz - 1;
	while (end > 0)
	{
		swap(a[0], a[end]);
		Down(a, 0, end);
		end--;
	}
}

//求N个数中最大或最小的前k个数

//void Test()
//{
//	int a[10] = { 10, 16,20 , 12, 11, 13, 15, 17, 14, 19 };
//	Heap<int> hp1(a, 10);
//	hp1.Push(30);
//	hp1.DisPlay();
//	hp1.Pop(); 
//	Heap<int, Max<int>> hp2(a, 10);
//	hp2.DisPlay();
//	HeapSort(a, 10);
//	for (int i = 0; i < 10; i++)
//	{
//		cout << a[i] << " ";
//	}
//	cout << endl;
//	//hp1.AdjustDown();
//}