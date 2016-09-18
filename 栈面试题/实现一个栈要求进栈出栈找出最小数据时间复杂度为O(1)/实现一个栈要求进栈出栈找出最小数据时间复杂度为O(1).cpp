#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
using namespace std;

//ʵ��һ��ջ��Ҫ��ʵ��Push����ջ����Pop����ջ����Min��������Сֵ�Ĳ�������ʱ�临�Ӷ�ΪO(1)
#pragma once
#include<iostream>
using namespace std;

template<typename T>
class Stack
{
public:
	Stack()
		:_data(NULL)
		,_sz(0)
		,_capacity(0)
		,min(T())
	{}
	~Stack()
	{
		if(_data != NULL)
		{
			//delete[] _data;
			//_data = NULL;
			_sz = 0;
			_capacity = 0;
		}
	}
	void Push(const T& t)
	{
		if(Empty())
		{
			min = t;
		}
		if(min > t)
		{
			min = t;
		}
		CheckCapacity();
		_data[_sz] = t;
		_sz++;
	}
	void Pop()
	{  
		if(_sz!=0)
		{
			_sz--;
		}
	}
	bool Empty()
	{
		return _sz==0;
	}
	T& Top()
	{
		return _data[_sz-1]; 
	}
	size_t Getsize()
	{
		return _sz;
	}
	void Display()
	{
		while(_sz != 0)
		{
			cout<<Top()<<" ";
			Pop();
		}
		cout<<endl;

	}
	T& GetMin()
	{
		return min;
	}
protected:
	void CheckCapacity()
	{
		if(_sz == _capacity)
		{
			size_t Newcapacity = _capacity*2+3;
			T* tmp = new T[Newcapacity];
			for(size_t i = 0;i<_sz;i++)
			{
				tmp[i] = _data[i];
			}
			T* del = _data;
			_data = tmp;
			delete del;
			del = NULL;
			_capacity = Newcapacity;
		}
	}

private:
	T *_data;
	T min;
	size_t _sz;
	size_t _capacity;
};
