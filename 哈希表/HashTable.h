#pragma once

#include<iostream>
#include<vector>
using namespace std;

enum STATE
{
	EMPTY,
	EXSIT,
	DELETE
};


template<typename K>
struct __Hashfunc
{
	size_t operator()(const K& key)
	{
		return key;
	}
};


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


// 命名空间 1  线性检测
namespace HASH1
{


	template<typename K, typename V>
	struct HashNode
	{
		K _key;
		V _value;

		STATE _sta;

		HashNode(const K& key = K(), const V& value = V())
			:_key(key)
			, _value(value)
			, _sta(EMPTY)
		{}
	};

	template<typename K, typename V, typename __HashFunc = __Hashfunc<K>>
	class HashTable
	{
		typedef HashNode<K, V> Node;
	public:
		HashTable()
			:_size(0)
		{
			_table.resize(GetPrimenum(0));
		}
		bool Insert(const K& key, const V& value)
		{
			CheckSize();

			size_t index = Getindex(key);
			while (_table[index]._sta == EXSIT)
			{
				if (_table[index]._key == key)
				{

					_table[index]._value++;
					return false;
				}

				if (index == _table.size() - 1)
				{
					index = 0;
				}
				++index;
			}
			_table[index]._key = key;
			_table[index]._value = value;
			_table[index]._sta = EXSIT;
			_size++;

			return true;


		}
		void Swap(HashTable<K, V>& hash)
		{
			this->_table.swap(hash._table);
			swap(_size, hash._size);
		}
		Node* Find(const K& key)
		{
			size_t index = Getindex(key);
			while (_table[index]._sta == EXSIT)
			{
				if (_table[index]._key == key)
				{
					return &_table[index];
				}
				if (index == _table.size() - 1)
				{
					index = 0;
				}
				++index;
			}
			return NULL;
		}

		bool Remove(const K& key)
		{
			size_t index = Getindex(key);
			while (_table[index]._sta == EXSIT)
			{
				if (_table[index]._key == key)
				{
					_table[index]._sta = DELETE;
					_size--;
					return true;
				}
				if (index == _table.size() - 1)
				{
					index = 0;
				}
				++index;
			}
			return false;
		}
	protected:
		size_t Getindex(const K& key)
		{
			//return key%_table.size();
			__HashFunc h;
			return h(key) % _table.size();
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
		void CheckSize()
		{
			if (_table.size() == 0 || _size * 10 / _table.size() >= 8)
			{
				HashTable<K, V> NewHash;
				NewHash._table.resize(GetPrimenum(_table.size()));

				for (size_t i = 0; i < _table.size(); ++i)
				{
					NewHash.Insert(_table[i]._key, _table[i]._value);
				}
				this->Swap(NewHash);
			}

		}
	protected:
		vector<Node> _table;
		size_t _size;
	};

}


//命名空间2  开链法
namespace HASH2
{
	template<typename K,typename V>
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

	template<typename K,typename V,typename __HashFunc = __Hashfunc<K>>
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

			size_t index = Getindex(key,_tables.size());

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
			size_t index = Getindex(key,_tables.size());

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
			size_t index = Getindex(key,_tables.size());

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
		size_t Getindex(const K& key,const size_t& size)
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
}


void Test3()
{
	HASH2::HashTable<int, int> hash;
	int a[] = { 89, 18, 49, 58, 9, 9 ,1,54};
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		hash.Insert(a[i], 1);
	}

	hash.Print();
	cout << endl;
	cout << hash.Find(89)->_key << endl;
	for (int i = 0; i < 53; i++)
	{
		hash.Insert(i, 1);
	}
	hash.Print();
	//hash.Delete(1);
}


//void Test()
//{
//	HASH1::HashTable<int, int> hash;
//	int a[] = { 89, 18, 49, 58, 9, 9 };
//	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
//	{
//		hash.Insert(a[i], 1);
//	}
//
//	hash.Remove(58);
//	hash.Insert(58, 1);
//	HASH1::HashNode<int, int>* ret = hash.Find(58);
//	cout << ret->_key << endl;
//}
//
//void Test2()
//{
//	HASH1::HashTable<string, int> hash;
//	char* a[] = { "sort", "sort", "hash", "sort" };
//	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
//	{
//		hash.Insert(a[i], 1);
//	}
//}