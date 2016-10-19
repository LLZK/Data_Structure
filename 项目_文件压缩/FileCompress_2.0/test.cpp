#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include"FileCompress.h"
using namespace std;


int main()
{
	Test1();
	Test2();
	FILE* fout = fopen("123.txt.config", "r");
	char Data[128];
	
	while (fgets(Data, 128, fout))
	{
		cout << Data;
	}
	system("pause");
	return 0;
}