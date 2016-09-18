#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stack>
using namespace std;

/*
方法①
单栈------------------一次push俩，一次pop俩
*/
template<typename T>
class Stack
{
public:
	Stack()
		:min(T())
	{}
	void Push(const T& x)
	{
		if(s.empty()||min >= x)
		{
			 min = x;
		}
		s.push(x);
		s.push(min);
	}
	void Pop()
	{
		s.pop();
		s.pop();
	}
	T& Top()
	{
		return s.top();
	}
	bool Empty()
	{
		return s.empty();
	}
	size_t Size()
	{
		return s.size();
	}
	T& GetMin()
	{
		return s.top();
	}
private:
	stack<T> s;
    T min;
};

void Test()
{
	Stack<int> s1;
	s1.Push(9);
	s1.Push(8);
	s1.Push(10);
	s1.Push(2);
	s1.Pop();
	cout<<s1.GetMin()<<endl;
	

	cout<<s1.Top()<<endl;
}
//int main()
//{
//	Test();
//	system("pause");
//	return 0;
//}




