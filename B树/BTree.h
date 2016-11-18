#pragma once 

#include<iostream>
using namespace std;

//  M代表M阶（每个结点有M个孩子）
template<typename K, int M>
struct BTreeNode
{
	//对关键码数组和孩子数组都多开辟一个空间
	//方便后面的分裂  
	K _key[M];
	
	BTreeNode<K, M>* _subs[M + 1];
	BTreeNode<K, M>* _parent;

	size_t _size;//记录关键码的数量

	BTreeNode()
		:_parent(NULL)
		, _size(0)
	{
		
		int i = 0;
		for (i = 0; i < M; ++i)
		{
			_key[i] = K();
			_subs[i] = NULL;
		}
		_subs[i++] = NULL;
	/*	for (int i = 0; i < M + 1; ++i)
		{
			_subs[i] = NULL;
		}*/
	}
};

template<typename K, int M>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	BTree()
		:_root(NULL)
	{
	}
	bool Insert(const K& key)
	{
		if (_root == NULL)
		{
			_root = new Node();
			_root->_key[0] = key;
			_root->_size++;
			return true;
		}
		pair<Node*, int> ret = Find(key);
		if (ret.second != -1)
		{
			return false;
		}

		Node* cur = ret.first;

		K newKey = key;
		Node* sub = NULL;
		while (1)
		{
			InsertKey(newKey, cur,sub);

			//分裂
			//如果cur的关键码不满，则不用分裂。
			if (cur->_size < M)
			{
				return true;
			}

			size_t mid = cur->_size / 2  ;
			Node* tmp = new Node();
			size_t j = 0;
			for (size_t i = mid+1; i < cur->_size; i++)
			{
				tmp->_key[j] = cur->_key[i];
				cur->_key[i] = K();
				cur->_size--;
				tmp->_size++;
			}

			j = 0;
			for (j = 0; j <= tmp->_size; j++)
			{
				tmp->_subs[j] = cur->_subs[mid + 1 + j];
				if (tmp->_subs[j])
				{
					tmp->_subs[j]->_parent = tmp;
				}
				cur->_subs[mid + 1 + j] = NULL;
			}
			

			if (cur->_parent == NULL)
			{
				_root = new Node();

				_root->_key[0] = cur->_key[mid];
				cur->_key[mid] = K();
				cur->_size--;
				_root->_size++;

				_root->_subs[0] = cur;
				cur->_parent = _root;
				_root->_subs[1] = tmp;
				tmp->_parent = _root;

				return true;
			}
			else
			{
				newKey = cur->_key[mid];
				cur->_key[mid] = K();
				cur->_size--;
				sub = tmp;
				cur = cur->_parent;
			}
		}
	}	
	bool Delete(const K& key)
	{
		if (_root == NULL)
		{
			return false;
		}

		pair<Node*, int> ret = Find(key);
		if (ret.second == -1)
		{
			return false;
		}
		else
		{
			Node* cur = ret.first;
			int index = ret.second;
			if (cur->_subs[index] == NULL && cur->_subs[index+1] == NULL)
			{
				for (size_t i = 0; i < cur->_size - index; ++i)
				{
					cur->_key[i] = cur->_key[i + 1];
					//cur->_subs[i] = cur->_subs[i + 1];
				}
				cur->_size--;

				if (cur->_size == 0)
				{
					Node* parent = cur->_parent;

					delete cur;
					cur = NULL;
				}
			}
			else
			{
				Node* tmp = cur->_subs[index];
				while (tmp)
				{
					tmp = tmp->_subs[tmp->_size];
				}
				Node* parent = tmp->_parent;
				std::swap(cur->_key[index], tmp->_key[tmp->_size - 1]);
				tmp->_key[tmp->_size - 1] = K();
				tmp->_size--;
			}
			return true;
		}


	}
	void InOrder()
	{
		__InOrder(_root);
		cout << endl;
	}
protected:
	void InsertKey(const K& key,Node* cur,Node* sub)
	{
		//只有当当前结点不满，或孩子指针为空时才执行
		//所以从后面开始遍历
		//这里并不分裂。仅仅挪动孩子，维护关键码与孩子的对应关系
		int i = cur->_size - 1;;
		while (i >= 0)
		{
			if (key > cur->_key[i])
			{
				break;
			}
			else
			{
				cur->_key[i + 1] = cur->_key[i];
				cur->_subs[i + 2] = cur->_subs[i + 1];
			}
			--i;
		}

		cur->_key[i + 1] = key;
		cur->_subs[i + 2] = sub;
		if (sub)
		{
			cur->_subs[i + 2]->_parent = cur;
		}
		if (cur->_size < M)
		{
			cur->_size++;
		}
	}
	void __InOrder(Node* root)
	{
		if (root == NULL)
		{
			return ;
		}
		size_t i = 0;//key下标
		//size_t j = 0;//sub下标
		Node* prev = NULL;
		for (i = 0; i < root->_size; ++i)
		{
			if (prev != root->_subs[i])
				__InOrder(root->_subs[i]);

			cout << root->_key[i] << " ";
			__InOrder(root->_subs[i + 1]);
			prev = root->_subs[i + 1];
		}
	}


	pair<Node*,int> Find(const K& key)
	{
		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			size_t i = 0;
			while (i < cur->_size)
			{
				if (cur->_key[i] < key)
				{
					++i;
				}
				else if (cur->_key[i] > key)
				{
					parent = cur;
					cur = cur->_subs[i];
					i = 0;
					break;
				}
				else
				{
					return pair<Node*, int>(cur,i);
				}
			}

			//可能出现访问空指针的问题
			if (cur != NULL&&i == cur->_size)
			{
				parent = cur;
				cur = cur->_subs[i];
			}
		}

		return pair<Node*, int>(parent, -1);
	}



protected:
	Node* _root;
};


void Test()
{
	BTree<int, 3> b;
	//int a[] = { 53, 75, 139, 49, 145, 36, 101,12,32,44 };
	int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
	{
      b.Insert(a[i]);
	}
	b.Delete(1);
	b.Delete(2);
	b.Delete(3);
	b.InOrder();
}