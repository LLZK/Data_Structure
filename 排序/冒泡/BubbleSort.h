#pragma once
#include<iostream>
#include<assert.h>
using namespace std;


//ц╟ещеепР
void BubbleSort(int *a, int n)
{
	assert(a);
	for (int j = 0; j < n; j++)
	{
		bool exchange = false;
		for (int i = 1; i < n - j; i++)
		{
			if (a[i - 1]>a[i])
			{
				swap(a[i - 1], a[i]);
				exchange = true;
			}
		}
		if (exchange == false)
		{
			break;
		}
	}
}

void TestBubbleSort()
{
	int a[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
	BubbleSort(a, 10);
}