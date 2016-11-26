#pragma once
#include<iostream>
using namespace std;

class UnionFindGather
{

public:

	void Init(int n)
	{
		a = new int[n];
		memset(a, -1, sizeof(int)*(n));
	}
	int FindRoot(int root)
	{
		if (a[root] == 0)
		{
			return 0;
		}
		if (a[root] < 0)
		{
			return root;
		}
		else
		{
			return FindRoot(a[root]);
		}
	}
	void MergeUnion(int left, int right)
	{
		int root1 = FindRoot(left);
		int root2 = FindRoot(right);
		a[root1] += a[root2];
		a[root2] = root1;
	}
	void Friend(int n,int m,int r[][2])
	{
		for (int i = 0; i < m; ++i)
		{
			MergeUnion(r[i][0], r[i][1]);
		}
	}
	int GetSize(int n)
	{
		int size = 0;
		for (int i = 0; i < n; i++)
		{
			if (a[i] < 0)
			{
				size++;
			}
		}
		return size;
	}
private:
	int *a;

};


void TestUnionGather()
{
	
	int a[7][2] = { { 0, 6 }, { 0, 7 }, { 0, 8 }, { 1, 4 }, { 1, 9 }, { 2, 3 }, { 2, 5 }};
	UnionFindGather u;
	u.Init(10);
	u.Friend(10, 7, a);
	cout << u.GetSize(10) << endl;
	u.MergeUnion(4, 8);
	cout << u.GetSize(10) << endl;
}
