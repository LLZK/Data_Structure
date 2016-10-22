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
	//递归
	SearchBinarryTree(const SearchBinarryTree<T>& x)
	{
		_root = _CopyTree(x._root);
	}
	//现代写法
	SearchBinarryTree<T>& operator=(SearchBinarryTree<T> x)
	{
		if (*this != x)
		{
			std::swap(_root, x._root);
		}
		return *this;
	}
	//递归
	~SearchBinarryTree()
	{
		_Destroy(_root);
	}

	//非递归
	bool Insert(const T& x)
	{
		//树空
		//x已经存在
		//x不存在
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

		//树为空
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

		//树中不存在该元素
		if (cur == NULL)
		{
			return false;
		}

		//找到删除结点
		//删除结点 左为空
		Node* del = cur;
		if (cur->_left == NULL)
		{
			if (parent == NULL)
			{
				_root = cur->_right;
			}
			else if (parent->_left = cur)
			{
				parent->_left = cur->_right;
			}
			else
			{
				parent->_right = cur->_right;
			}
		}
		//删除结点 右为空
		else if (cur->_right == NULL)
		{
			if (parent == NULL)
			{
				_root = cur->_right;
			}
			else if (parent->_left == cur)
			{
				parent->_left = cur->_left;
			}
			else
			{
				parent->_right = cur->_left;
			}
		}
		//删除结点 左右都不为空
		//找左子树最右结点或者右子树最左结点进行替换，然后再删除
		else
		{
			//找左子树最右结点
			Node* Rnode = cur->_left;
			Node* parent = cur;
			while (Rnode->_right)
			{
				parent = Rnode;
				Rnode = Rnode->_right;
			}
			//cur->key = Rnode->key
			swap(cur->_key, Rnode->_key);
			if (parent->_left = Rnode)
			{
				parent->_left = Rnode->_left;
			}
			else
			{
				parent->_right = Rnode->_left;
			}
			del = Rnode;
		}
		delete del;
		del = NULL;
		return true;
	}

	//递归
	bool InsertR(const T& x)
	{
		return _InsertR(x,_root);
	}
	bool _FindR(const T& x,Node* root)
	{
		if (root == NULL)
		{
			return false;
		}
		else
		{
			if (x > root->_key)
			{
				return _FindR(x, root->_right);
			}
			else if (x < root->_key)
			{
				return _FindR(x, root->_left);
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	bool RemoveR(const T& x)
	{
		return _RemoveR(x, _root);
	}
	//得到根结点
	Node* GetRoot()
	{
		return _root;
	}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
protected:
	//拷贝构造递归函数
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
	//析构递归函数
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
	bool _InsertR(const T& x, Node*& root)
	{
		if (root == NULL)
		{
			root = new Node(x);
			return true;
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
				_InsertR(x, root->_right);
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
				_InsertR(x, root->_left);
			}
		}
		else
		{
			return false;
		}
	}
	bool _RemoveR(const T& x, Node*& root)
	{
		static Node* parent = root;
		if (root == NULL)
		{
			return false;
		}
		//parent = root;
		if (x > root->_key)
		{
			parent = root;
			_RemoveR(x, root->_right);
		}
		else if (x < root->_key)
		{
			parent = root;
			_RemoveR(x, root->_left);
		}
		else
		{
			Node* del = root;
			if (root->_left == NULL)
			{
				if (root == _root)
				{
					_root = _root->_right;
				}
				else if (parent->_left = root)
				{
					parent->_left = root->_right;
				}
				else
				{
					parent->_right = root->_right;
				}
			}
			//删除结点 右为空
			else if (root->_right == NULL)
			{
				if (root == _root)
				{
					_root = _root->_left;
				}
				else if (parent->_left = root)
				{
					parent->_left = root->_left;
				}
				else
				{
					parent->_right = root->_left;
				}
			}
			//删除结点 左右都不为空
			//找左子树最右结点或者右子树最左结点进行替换，然后再删除
			else
			{
				//找左子树最右结点
				Node* Rnode = root->_left;
				Node* parent = root;
				while (Rnode->_right)
				{
					parent = Rnode;
					Rnode = Rnode->_right;
				}
				swap(root->_key, Rnode->_key);
				if (parent->_left = Rnode)
				{
					parent->_left = Rnode->_left;
				}
				else
				{
					parent->_right = Rnode->_left;
				}
				del = Rnode;
			}
			delete del;
			del = NULL;
			return true;
		}
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
		tree.InsertR(a[i]);
	}
	tree.InOrder();
	tree.RemoveR(0);
	tree.RemoveR(1);
	tree.RemoveR(2);
	tree.InOrder();
	tree.RemoveR(3);
	tree.InOrder();
	tree.RemoveR(4);
	tree.InOrder();
	tree.RemoveR(5);
	tree.InOrder();
	tree.RemoveR(6);
	tree.RemoveR(7);
	tree.RemoveR(8);
	tree.RemoveR(9);
	tree.InOrder();
//	cout << tree._FindR(3,tree.GetRoot()) << endl;
	//tree.InOrder();
	//tree._RemoveR(5,tree.GetRoot());
	SearchBinarryTree<int> tree2(tree);
}