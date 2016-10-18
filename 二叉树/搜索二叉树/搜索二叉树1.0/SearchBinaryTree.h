#pragma once
#include<iostream>
using namespace std;

template<typename T>
struct SearchBinarryTreeNode
{
	SearchBinarryTreeNode<T>* _left;
	SearchBinarryTreeNode<T>* _right;
	T _key;
	SearchBinarryTreeNode(const T& key)
		:_key(key)
		,_left(NULL)
		,_right(NULL)
	{}
};

template<typename T>
class SearchBinarryTree
{
	typedef SearchBinarryTreeNode<T> Node;
public:
	SearchBinarryTree()
		:_root(NULL)
	{}
	//�ݹ�
	SearchBinarryTree(const SearchBinarryTree<T>& x)
	{
		_root = _CopyTree(x._root);
	}
	//�ִ�д��
	SearchBinarryTree<T>& operator=(SearchBinarryTree<T> x)
	{
		if (*this != x)
		{
			std::swap(_root, x._root);
		}
		return *this;
	}
	//�ݹ�
	~SearchBinarryTree()
	{
		_Destroy(_root);
	}

	//�ǵݹ�
	bool Insert(const T& x)
	{
		//����
		//x�Ѿ�����
		//x������
		if (_root == NULL)
		{
			_root = new Node(x);
			return true;
		}
		else
		{
			Node* newnode = new Node(x);
			Node *cur = _root;
			while (cur)
			{
				if (x > cur->_key)
				{
					if (cur->_right == NULL)
					{
						cur->_right = newnode;
						return true;
					}
					else
					{
						cur = cur->_right;
					}
				}
				else if (x < cur->_key)
				{
					if (cur->_left == NULL)
					{
						cur->_left = newnode;
						return true;
					}
					else
					{
						cur = cur->_left;
					}
				}
				else
				{
					return false;
				}
			}
		}
	}
	bool Find(const T& x)
	{
		if (_root == NULL)
		{
			return false;
		}
		else
		{
			Node* cur = _root;
			while (cur)
			{
				if (x > cur->_key)
				{
					cur = cur->_right;
				}
				else if (x < cur->_key)
				{
					cur = cur->_left;
				}
				else
				{
					return true;
				}
			}
			return false;
		}
	}
	bool Remove(const T& x)
	{

		//��Ϊ��
		if (_root == NULL)
		{
			return false;
		}


		Node* cur = _root;
		Node* parent = NULL;
		while (cur)
		{
			if (x > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (x < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				break;
			}
		}

		//���в����ڸ�Ԫ��
		if (cur == NULL)
		{
			return false;
		}

		//�ҵ�ɾ�����
		//ɾ����� ��Ϊ��
		if (cur->_left == NULL)
		{
			parent = cur->_right;
			delete cur;
			cur = NULL;
		}
		//ɾ����� ��Ϊ��
		else if (cur->_right == NULL)
		{
			parent = cur->_left;
			delete cur;
			cur = NULL;
		}
		//ɾ����� ���Ҷ���Ϊ��
		//�����������ҽ���������������������滻��Ȼ����ɾ��
		else
		{
			//�����������ҽ��
			Node* Rnode = cur->_left;
			Node* parent = cur;
			while (Rnode->_right)
			{
				parent = Rnode;
				Rnode = Rnode->_right;
			}
			swap(cur->_key, Rnode->_key);
			parent->_right = Rnode->_left;
			delete Rnode;
			Rnode = NULL;
		}
		return true;
	}

	//�ݹ�
	bool InsertR(const T& x,Node* root)
	{
		if (root == NULL)
		{
			return false;
		}
		if (x > root->_key)
		{
			if (root->_right == NULL)
			{
				root->_right = new Node(x);
				return true;
			}
			else
			{
				Insert(x, root->_right);
			}
		}
		else if (x < root->_key)
		{
			if (root->_left == NULL)
			{
				root->_left = new Node(x);
				return true;
			}
			else
			{
				Insert(x, root->_left);
			}
		}
		else
		{
			return false;
		}
	}
	bool FindR(const T& x,Node* root)
	{
		if (root == NULL)
		{
			return false;
		}
		else
		{
			if (x > root->_key)
			{
				return FindR(x, root->_right);
			}
			else if (x < root->_key)
			{
				return FindR(x, root->_left);
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	bool RemoveR(const T& x,Node* root)
	{
		static Node* parent = NULL;
		if (root == NULL)
		{
			return false;
		}
		if (x > root->_key)
		{
			parent = root;
			RemoveR(x, root->_right);
		}
		else if (x < root->_key)
		{
			parent = root;
			RemoveR(x, root->_left);
		}
		else
		{

			if (root->_left == NULL)
			{
				parent = root->_right;
				delete root;
				root = NULL;
			}
			//ɾ����� ��Ϊ��
			else if (root->_right == NULL)
			{
				parent = root->_left;
				delete root;
				root = NULL;
			}
			//ɾ����� ���Ҷ���Ϊ��
			//�����������ҽ���������������������滻��Ȼ����ɾ��
			else
			{
				//�����������ҽ��
				Node* Rnode = root->_left;
				Node* parent = root;
				while (Rnode->_right)
				{
					parent = Rnode;
					Rnode = Rnode->_right;
				}
				swap(root->_key, Rnode->_key);
				parent->_right = Rnode->_left;
				delete Rnode;
				Rnode = NULL;
			}
			return true;
		}
	}
	//�õ������
	Node* GetRoot()
	{
		return _root;
	}

	void InOrder()
	{
		_InOrder(_root);
	}
protected:
	//��������ݹ麯��
	Node* _CopyTree(Node* root)
	{
		if (root == NULL)
		{
			return NULL;
		}
		Node* cur = new Node(root->_key);
		cur->_left = _CopyTree(root->_left);
		cur->_right = _CopyTree(root->_right);
		return cur;
	}
	//�����ݹ麯��
	void _Destroy(Node* root)
	{
		if (root == NULL)
		{
			return;
		}
		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
		root = NULL;
	}
	void _InOrder(Node* _root)
	{
		if (_root == NULL)
		{
			return;
		}
		_InOrder(_root->_left);
		cout << _root->_key << " ";
		_InOrder(_root->_right);
	}
protected:
	Node* _root;
};

void Test()
{
	SearchBinarryTree<int> tree;
	int a[10] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	for (int i = 0; i < 10; i++)
	{
		tree.Insert(a[i]);
	}
	cout << tree.FindR(3,tree.GetRoot()) << endl;
	tree.InOrder();
	tree.RemoveR(5,tree.GetRoot());
	SearchBinarryTree<int> tree2(tree);
}