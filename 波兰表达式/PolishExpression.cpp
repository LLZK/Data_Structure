#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stack>
using namespace std;
const int N = 100;
bool Isoperator(char c)
{
	char ope[] = "+-*/";
	for(int i = 0;i<4;i++)
	{
		if(c == ope[i])
		{
			return true;
		}
	}
	return false;
}
int Priority(char op1,char op2)
{
	if((op1 == '+')||(op1 == '-'))
	{
		if((op2 = '*')||(op2 == '/'))
		{
			return -1;
		}
		return 0;
	}
	else if(op1 == '*'||op1 =='/')
	{
		if(op2 == '+'||op2 == '-')
		{
			return 1;
		}
		return 0;
	}
	return -1;
}
stack<char>&  MidToTail(char *s,stack<char>& ret)
{
	stack<char> s1;
	int j = 0;
	while(s[j])
	{
		int i = 0;
		if(Isoperator(s[j]))
		{
			if(s1.empty())
			{
				s1.push(s[j]);
			}
			else
			{
				if(Priority(s1.top(),s[j])>=0)
				{
					while(Priority(s1.top(),s[j])>=0)
					{
						ret.push(s1.top());
						s1.pop();
						if(s1.empty())
						{
							break;
						}
					}
					s1.push(s[j]);
				}
				else
				{
					s1.push(s[j]);
				}
			}
		}
		else
		{
			ret.push(s[j]);
		}
		j++;
	}
	while(!(s1.empty()))
	{
		ret.push(s1.top());
		s1.pop();
	}
	return ret;
}
int PolishExpression(stack<char>& exp)
{
	stack<int> cal;
	exp.pop();
	while(!(exp.empty()))
	{
		if(!(Isoperator(exp.top())))
		{
			cal.push(exp.top()-'0');
			exp.pop();
		}
		else
		{
			int a = cal.top();
			cal.pop();
			int b = cal.top();
			cal.pop();
			int c = 0;
			switch(exp.top())
			{
			case'+':
				 c = a+b;
				break;
			case'-':
				 c = b-a;
				break;
			case'*':
				 c = a*b;
				break;
			case'/':
				 c = b/a;
				break;
			default:
				break;	
			}
			cal.push(c);
			exp.pop();
		}
	}
	return cal.top();
}
void Reverse(stack<char>& s)
{
	char arr[N] = {0};
	int i = 0;
	for(i = 0;!(s.empty());i++)
	{
		arr[i] = s.top();
		s.pop();
	}
	int n = i;
    for(int i = 0;i<=n;i++)
	{
		s.push(arr[i]);
	}
}
int main()
{
	stack<char> ret;
	char *str =	"4/2+1*9";
	char output[N] = {0};
	stack<char> s = MidToTail(str,ret);
	Reverse(s);
    cout<<PolishExpression(s)<<endl;
	system("pause");
	return 0;
}