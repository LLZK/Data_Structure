#pragma once

#include<iostream>
#include<vector>
using namespace std;

template<typename K>
class BitMap
{
public:
	BitMap(const size_t& range)
		:_size(0)
	{
		_Map.resize((range >> 5) + 1);
	}

	void Set(const K& key)
	{
		size_t index = key / 32;
		int indexnum = key % 32;

		(_Map[index]) |= (1 << indexnum);
		_size++;
	}

	void ReSet(const K& key)
	{
		size_t index = key / 32;
		int indexnum = key % 32;
		
		int RindexNum = 1 << indexnum;
		(_Map[index]) &= (~RindexNum);
		_size--;
	}
	bool Test(const K& key)
	{
		size_t index = key / 32;
		int indexnum = key % 32;

		K src = _Map[index];

		int RindexNum = 1 << indexnum;
		(_Map[index]) &= (~RindexNum);

		if (_Map[index] != src)
		{
			return true;
		}
		else
		{
			return false;
		}
		_Map[index] = src;
	}
	size_t Size()
	{
		return _size;
	}
protected:
	vector<int> _Map;
	size_t _size;
};

void TestBitMap()
{
	BitMap<int> map(63);
	map.Set(4);
	map.Set(44);
	map.Set(32);
	map.Set(62);
	//map.ReSet(4);
	cout << map.Test(44) << endl;
}