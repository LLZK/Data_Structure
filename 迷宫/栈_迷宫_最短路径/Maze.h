#pragma once
#include<iostream>
#include<cassert>
#include"Stack.h"
const int N = 10; 
using namespace std;

struct Pos
{
	Pos(int row = 0,int cols = 0)
		:_row(row)
		,_cols(cols)
	{}
	void operator=(const Pos& p)
	{
		_row = p._row;
		_cols = p._cols;
	}
	bool operator==(const Pos& p)
	{
		return (p._row==_row && p._cols==_cols);
	}
	int _row;
	int _cols;
};
class Maze
{
public:
	Maze()
	{}
	void IninMap()
	{
		FILE *fout = fopen("MazeMap.txt","r");
		assert(fout);
		int ret = 0;
		for(int i = 0;i<N;i++)
		{
			for(int j = 0;j<N;j++)
			{
				char ret = fgetc(fout);
				if(ret == ' '||ret == '\n')
				{
					ret = fgetc(fout);
				}
				map[i][j] = ret- '0';
			}
		}

	}
	void PrintMaze()
	{
		for(int i = 0;i<N;i++)
		{
			for(int j = 0;j<N;j++)
			{
				cout<<map[i][j] <<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	void operator=(const Maze& m)
	{
		for(int i = 0;i<N;i++)
		{
			for(int j = 0;j<N;j++)
			{
				map[i][j] = m.map[i][j];
			}
		}
	}
	bool GetPath(Pos entry)
	{
		p.Push(entry);
		Pos d;
		map[p.Top()._row][p.Top()._cols] = 2;
		while(d._row != N-1 && d._cols != N-1&& !(d == entry))
		{
			//PrintMaze();
			d = p.Top();
			//上
			if(map[d._row - 1][d._cols]== 0)
			{
				d._row--;
				map[d._row][d._cols] = 2;
				p.Push(d);
				//PrintMaze();
				continue;

			}
			//下
			if(map[d._row + 1][d._cols]== 0)
			{
				d._row++;
				map[d._row][d._cols] = 2;
				p.Push(d);
				//PrintMaze();

				continue;

			}
			//左
			if(map[d._row][d._cols-1]== 0)
			{
				d._cols--;
				map[d._row][d._cols] = 2;
				p.Push(d);
				//PrintMaze();

				continue;

			}
			//右
			if(map[d._row][d._cols+1]== 0)
			{
				d._cols++;
				map[d._row][d._cols] = 2;
				p.Push(d);
				//PrintMaze();

				continue;
			}
			p.Pop();
		}
		return !(d == entry);
		//return ((p.Top()._row == N-1)||(p.Top()._cols == N-1));
	}

	Stack<Pos> p;
	int map[N][N];
};
void TestMaze()
{
	Maze m;
	m.IninMap();
	m.PrintMaze();
	//m.GetPath(Pos(2,0));
	cout<<m.p.Getsize()<<endl;
	m.PrintMaze();
	int MinLength = 0;
	Maze tmp = m;
	while(tmp.GetPath(Pos(2,0)))
	{
		m.map[tmp.p.Top()._row][tmp.p.Top()._cols] = 1;
		if(MinLength == 0||MinLength>tmp.p.Getsize())
		{
			MinLength = tmp.p.Getsize();
		}
		tmp = m;
	}
	m.PrintMaze();
	cout<<"最短路径:"<<MinLength<<"步"<<endl;

    
}