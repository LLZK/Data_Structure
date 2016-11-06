#include"HashTable.h"

int main()
{
	Test3();
	int a = 1;
	int b = a;
	system("pause");
	return 0;
}

#include<iostream>
#include<vector>
using namespace std;
//特化
template<>
struct __Hashfunc<string>
{
public:
	size_t operator()(const string& k)
	{
		return BKDRHash(k.c_str());
	}
private:
	//字符串哈希处理算法
	static size_t BKDRHash(const char* str)
	{
		unsigned int seed = 131;// 31 131 1313 13131 131313
		unsigned int hash = 0;
		while (*str)
		{
			hash = hash*seed + (*str++);
		}
		return(hash & 0x7FFFFFFF);
	}
};


template<typename K, typename V>
struct HashNode
{
	K _key;
	V _value;

	HashNode<K, V>* _next;

	HashNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _next(NULL)
	{
	}
};

template<typename K, typename V, typename __HashFunc = __Hashfunc<K>>
class HashTable
{
	typedef HashNode<K, V> Node;
public:
	HashTable()
		:_size(0)
	{
		_tables.resize(GetPrimenum(0));
	}

	bool Insert(const K& key, const V& value)
	{
		CheckSize();

		size_t index = Getindex(key, _tables.size());

		Node* cur = _tables[index];
		Node* parent = NULL;

		if (_tables[index] == NULL)
		{
			_tables[index] = new Node(key, value);
			_size++;
			return true;
		}

		while (cur)
		{
			if (cur->_key == key)
			{
				cur->_value++;
				_size++;
				return true;
			}
			cur = cur->_next;
		}

		Node* newNode = new Node(key, value);
		newNode->_next = _tables[index];
		_tables[index] = newNode;
		_size++;
		return true;
	}
	Node* Find(const K& key)
	{
		size_t index = Getindex(key, _tables.size());

		if (_tables[index] == NULL)
			return NULL;
		Node* cur = _tables[index];

		while (cur)
		{
			if (cur->_key == key)
				return cur;
			cur = cur->_next;
		}
		return NULL;
	}

	bool Delete(const K& key)
	{
		size_t index = Getindex(key, _tables.size());

		if (_tables[index] == NULL)
		{
			return false;
		}

		Node* cur = _tables[index];
		Node* parent = NULL;

		if (cur->_key == key)
		{
			cur = cur->_next;
		}


		while (cur)
		{
			if (cur->_key == key)
			{
				if (cur->_next == NULL)
				{
					parent->_next = NULL;
				}
				else
				{
					parent->_next = cur->_next;
				}
				break;
			}
			parent = cur;
			cur = cur->_next;
		}

		if (cur)
		{
			delete cur;
			cur = NULL;
			return true;
		}

		return false;
	}
	void Print()
	{
		for (size_t i = 0; i < _tables.size(); ++i)
		{
			printf("tables[%d]->", i);
			Node* cur = _tables[i];
			while (cur)
			{
				cout << cur->_key << "->";
				cur = cur->_next;
			}
			cout << "NULL" << endl;
		}
	}
protected:
	void CheckSize()
	{
		if (_tables.size() == 0 || _size / _tables.size() >= 1)
		{
			vector<Node*> Newtable;
			Newtable.resize(GetPrimenum(_tables.size()));

			for (size_t i = 0; i < _tables.size(); ++i)
			{
				Node* cur = _tables[i];
				while (cur)
				{
					Node* next = cur->_next;
					_tables[i] = next;
					size_t index = Getindex(cur->_key, Newtable.size());

					Node* tmp = Newtable[index];
					Newtable[index] = cur;
					cur->_next = tmp;

					cur = next;

				}
			}

			//this->_size = Newtable.size();
			this->_tables.swap(Newtable);

		}
	}
	size_t Getindex(const K& key, const size_t& size)
	{
		//return key%_table.size();
		__HashFunc h;
		return h(key) % size;
	}
	size_t GetPrimenum(const size_t& sz)
	{
		const int Primesize = 28;
		static const unsigned long Primenum[Primesize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul,
			786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul,
			25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul,
			805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};
		for (int i = 0; i < Primesize; ++i)
		{
			if (Primenum[i] > sz)
			{
				return Primenum[i];
			}
		}
		return sz;
	}
protected:
	vector<Node*> _tables;
	size_t _size;
};

