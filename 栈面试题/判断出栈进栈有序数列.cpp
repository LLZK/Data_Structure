#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stack>
#include<string>
using namespace std;

//�ж�Ԫ�س�ջ����ջ˳��ĺϷ��ԡ��磺��ջ�����У�1,2,3,4,5������ջ����Ϊ��4,5,3,2,1���ǺϷ����У� ��ջ������
//��1,2,3,4,5������ջ����Ϊ��1,5,3,2,4���ǲ��Ϸ�����
bool LegalJudgment(const string& str1,const string& str2)
{
	int i = 0;
	int j = 0;
	stack<char> s1;
	while(j < str2.size())
	{
		while( s1.empty()||s1.top() != str2[j] )
		{
			if(i < str1.size())
			{
				s1.push(str1[i]);
				//i++;
			}
			else
				return false;
		}
		j++;
		s1.pop();
	}
	return true;
}
int main()
{
	string str1("12345");
	string str2("45321");
	cout<<LegalJudgment(str1,str2)<<endl;
	system("pause");
	return 0;
}