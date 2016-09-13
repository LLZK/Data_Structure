#define _CRT_SECURE_NO_WARNINGS 1
#include"SimulationList.h"
#include <list>

struct A
{
	A(int i = 10,char c = 'a')
		:_i(i)
		,_c(c)
	{}
	int _i;
	char _c;
};

int main()
{
	//TestList();
	List<A> li;
	li.Pushback(A());
	li.Pushback(A());
	li.Pushback(A());
	li.Pushback(A());
	li.Pushback(A());
	List<A>::Iterator it = li.Begin();
	for(;it != li.End();it++)
	{
		cout<<it->_c<<endl;
	}
	system("pause");
	return 0;
}