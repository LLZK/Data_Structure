#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
using namespace std;
/*
栈面试题：
用一个数组实现两个栈
*/
const int N = 100;
template<typename T>
class StackFromNum
{
public:
	StackFromNum()
		:_sta1(0)
		,_sta2(0)
		,_capacity(2)
	{
		_data = new T[2];
	}
	void Push1(const T& x)
	{
		CheckCapacity();
		_data[_sta1] = x;
		_sta1++;
	}
	void Push2(const T& x)
	{
		CheckCapacity();
		_data[_capacity-_sta2-1] = x;
		_sta2++;
	}
	void Print()
	{
		for(int i = 0;i <_capacity;i++)
		{
			cout<<_data[i]<<" ";
		}
	}
	void Pop1()
	{
		_sta1--;
	}
	void Pop2()
	{
		_sta2--;
	}
	T& Top1()
	{
		return _data[_sta1-1];
	}
	T& Top2()
	{
		return _data[_capacity-_sta2];
	}
	size_t Size1()
	{
		return _sta1;
	}
	size_t Size2()
	{
		return _sta2;
	}
	bool Empty1()
	{
		return _sta1 == 0;
	}
	
	bool Empty2()
	{
		return _sta2 == 0;
	}
protected:
	void CheckCapacity()
	{
		if((_data+_sta1) >= (_data+_capacity-_sta2))
		{
			size_t NewCapacity = _capacity*2+3;
			T *tmp = new T[NewCapacity];
			for(size_t i = 0;i<_sta1;i++)
			{
				tmp[i] = _data[i];
			}
			for(size_t i = 0;i<_sta2;i++)
			{
				tmp[NewCapacity-i-1] = _data[_capacity-i-1];
			}
			delete _data;
			_data = tmp;
			_capacity = NewCapacity;
		}
	}

private:
	T *_data;
	size_t _sta1;
	size_t _sta2;
	size_t _capacity;
};
void TestStackFromNum()
{
	StackFromNum<int> s;
	s.Push2(2);
	s.Push2(4);
	s.Push2(1);
	s.Push1(1);
	s.Push1(2);
	s.Push1(3);
	s.Pop1();
	s.Pop2();
	s.Pop2();
	cout<<s.Top1()<<endl;
	cout<<s.Top2()<<endl;
	cout<<s.Size1()<<endl;
	cout<<s.Size2()<<endl;
	cout<<s.Empty1()<<endl;
	cout<<s.Empty2()<<endl;
	s.Print();
	
}
int main()
{
	TestStackFromNum();
	system("pause");
	return 0;
}