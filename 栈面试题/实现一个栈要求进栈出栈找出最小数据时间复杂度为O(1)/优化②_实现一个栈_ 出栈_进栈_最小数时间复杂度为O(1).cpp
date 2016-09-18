#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stack>
using namespace std;
/*
方法②:
双栈------------------一个主栈，一个辅助栈（保存每次的最小值）
*/
template<typename T>
class Stack
{
public:
	Stack()
	{}
	void Push(const T& x)
	{
		if(s.empty()||x < min.top())
		{
			s.push(x);
			min.push(x);
		}
		else
		{
			s.push(x);
		}
	}
	void Pop()
	{
		if(s.top() >= min.top())
		{
			s.pop();
		}
		else
		{
			s.pop();
			min.pop();
		}
	}
	T& Top()
	{
		return s.top();
	}
	T& GetMin()
	{
		return min.top();
	}
	size_t Size()
	{
		return s.size();
	}
	bool Empty()
	{
		return s.empty();
	}
private:
	stack<T> s;
	stack<T> min;
};
void Test1()
{
	Stack<int> s1;
	s1.Push(1);
	s1.Push(2);
	s1.Push(3);
	s1.Push(0);
	s1.Push(6);
	s1.Push(-1);
	cout<<s1.GetMin()<<endl;
 }
int main()
{
	Test1();
	system("pause");
	return 0;
}