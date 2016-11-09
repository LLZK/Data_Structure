#pragma once  

#include<iostream>
#include<vector>
using namespace std;


struct __HashFunc1
{
	size_t operator()(const string& str)
	{
		register size_t hash = 0;
		size_t i = 0;
		while (i< str.size())
		{
			hash = hash * 131 + str[i];// 也可以乘以31、131、1313、13131、131313..          
			i++;
		}
		return hash;
	}
};



struct __HashFunc2
{
	size_t operator()(const string& str)
	{
		register size_t hash = 0;
		size_t i = 0;
		while (i < str.size())
		{
			hash = 65599 * hash + str[i];
			i++;
			//hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;  
		}
		return hash;
	}
};


struct __HashFunc3
{
	size_t operator()(const string& str)
	{
		register size_t hash = 0;
		size_t magic = 63689;
		size_t i = 0;
		while (i < str.size())
		{
			hash = hash * magic + str[i];
			magic *= 378551;
			i++;
		}
		return hash;
	}
};


struct __HashFunc4
{
	size_t operator()(const string& str)
	{
		register size_t hash = 0;

		for (size_t i = 0; i < str.size();i++)
		{
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ str[i] ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ str[i] ^ (hash >> 5)));
			}
		}
		return hash;
	}
};



struct __HashFunc5
{
	size_t operator()(const string&  str)
	{
		if (str.empty())        // 这是由本人添加，以保证空字符串返回哈希值0  
			return 0;
		register size_t hash = 1315423911;
		size_t i = 0;
		while (i < str.size())
		{
			hash ^= ((hash << 5) + str[i] + (hash >> 2));
			i++;
		}
		return hash;
	}
};

template<typename K = string,typename HashFunc1 = __HashFunc1,
	typename HashFunc2 = __HashFunc2,
	typename HashFunc3 = __HashFunc3,
	typename HashFunc4 = __HashFunc4,
	typename HashFunc5 = __HashFunc5>
class BloomFilters
{
public:
	BloomFilters(const size_t& range)
	{
		_BitMap.resize(range * 5);
	}
	void Set(const K& key)
	{
		size_t hash1 = HashFunc1()(key) % _BitMap.size();
		size_t hash2 = HashFunc2()(key) % _BitMap.size();
		size_t hash3 = HashFunc3()(key) % _BitMap.size();
		size_t hash4 = HashFunc4()(key) % _BitMap.size();
		size_t hash5 = HashFunc5()(key) % _BitMap.size();

		_BitMap[hash1]++;
		_BitMap[hash2]++;
		_BitMap[hash3]++;
		_BitMap[hash4]++;
		_BitMap[hash5]++;
	}

	bool Reset(const K& key)
	{
		size_t hash1 = HashFunc1(key) % _BitMap.size();
		size_t hash2 = HashFunc2(key) % _BitMap.size();
		size_t hash3 = HashFunc3(key) % _BitMap.size();
		size_t hash4 = HashFunc4(key) % _BitMap.size();
		size_t hash5 = HashFunc5(key) % _BitMap.size();

		if (_BitMap[hash1] == 0 ||
			_BitMap[hash2] == 0 ||
			_BitMap[hash3] == 0 ||
			_BitMap[hash4] == 0 ||
			_BitMap[hash5] == 0)
		{
			return false;
		}
		else
		{
			_BitMap[hash1]--;
			_BitMap[hash2]--;
			_BitMap[hash3]--;
			_BitMap[hash4]--;
			_BitMap[hash5]--;
		}
	}

	bool Test(const K& key)
	{
		size_t hash1 = HashFunc1(key) % _BitMap.size();
		size_t hash2 = HashFunc2(key) % _BitMap.size();
		size_t hash3 = HashFunc3(key) % _BitMap.size();
		size_t hash4 = HashFunc4(key) % _BitMap.size();
		size_t hash5 = HashFunc5(key) % _BitMap.size();

		if (_BitMap[hash1] == 0 ||
			_BitMap[hash2] == 0 ||
			_BitMap[hash3] == 0 ||
			_BitMap[hash4] == 0 ||
			_BitMap[hash5] == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

protected:
	vector<size_t> _BitMap;
};


void TestBloomFilters()
{
	BloomFilters<> b(15);
	string s("wadasdqg");
	b.Set(s);

}