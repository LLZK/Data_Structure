#include<iostream>
#include"TreeIterator.h"
using namespace std;
//enum Type
//{
//	THREAD,
//	LINK
//};
//template<typename T>
//struct BinaryTreeNode
//{
//	T _data;
//	BinaryTreeNode<T> *_left;
//	BinaryTreeNode<T> *_right;
//	Type _leftTag;
//	Type _rightTag;
//	BinaryTreeNode(const T& x)
//		:_data(x)
//		,_left(NULL)
//		,_right(NULL)
//		,_leftTag(LINK)
//		,_rightTag(LINK)
//	{}
//
//	BinaryTreeNode()
//	{}
//};

template<typename T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
	typedef __TreeIterator<T, T&, T*> Iterator;
	typedef __TreeIterator<T,const T&,const T*> ConstIterator;
public:

	BinaryTree()
	{}
	BinaryTree(const T* a, int size, int index, const T& invalid)
	{
		_root = _MakeTree(a, size, index, invalid);
	}
	BinaryTree(const BinaryTree<T>& b)
	{
		_root = _CopyTree(b._root);
	}
	//~BinaryTree()
	//{
	//	Destroy(_root);
	//	cout << "~" << endl;
	//}
	//前序线索化
	void PrevOrder_Thd()
	{
		_PrevOrder_Thd(_root);
	}
	//中序线索化
	void InOrder_Thd()
	{
		_InOrder_Thd(_root);
	}
	//后序线索化
	void PostOrder_Thd()
	{
		_PostOrder_Thd(_root);
	}
	//前序遍历
	void PrevOrder()
	{
		Node* cur = _root;
		while (cur)
		{
			while (cur->_leftTag == LINK && cur)
			{
				cout << cur->_data << " ";
				cur = cur->_left;
			}
			cout << cur->_data << " ";
			cur = cur->_right;
		}
		cout << endl;
	}
	//中序遍历
	void InOrder()
	{
		Node* cur = _root;
		Node* prev = NULL;
		while (cur)
		{
			while (cur->_leftTag == LINK)
			{
				cur = cur->_left;
			}
			cout << cur->_data << " ";
			while (cur->_rightTag == THREAD && cur->_right)
			{
				cur = cur->_right;
				cout << cur->_data << " ";
			}
			cur = cur->_right;
		}
	}
	//后序遍历 
	//方法①：三叉树
	//方法②：写一个Find函数，找到父节点
	void PostOrder()
	{}

	//利用中序线索化思想将搜索二叉树转换成有序的双向链表
	Node* TreeToList()
	{
		//Node* prev = NULL;
		_TreeToList(_root);
		Node* cur = _root;
		while (cur->_left)
		{
			cur = cur->_left;
		}
		return cur;
	}
	Iterator Begin()
	{
		while (_root->_leftTag == LINK)
		{
			_root = _root->_left;
		}
		return Iterator(_root);
	}
	Iterator End()
	{
		return Iterator(NULL);
	}
	ConstIterator Begin() const 
	{
		while (_root->_leftTag == LINK)
		{
			_root = _root->_left;
		}
		return ConstIterator(_root);
	}
	ConstIterator End() const
	{
		return ConstIterator(_root);
	}
protected:
	//构造函数
	Node* _MakeTree(const T *a, int size, int& index, const T& invalid)
	{
		Node* root = NULL;
		if (index < size && a[index] != invalid)
		{
			root = new Node(a[index]);
			root->_left = _MakeTree(a, size, ++index, invalid);
			root->_right = _MakeTree(a, size, ++index, invalid);
		}
		return root;
	}
	//拷贝构造
	Node* _CopyTree(Node* _root)
	{
		Node* root = NULL;
		if (_root == NULL )
		{
			return root;
		}
		if (_root->_leftTag == THREAD)
		{
			root = new Node(_root->_data);
			root->_left = _root->_left;
			root->_right = _root->_right;
			root->_leftTag = _root->_leftTag;
			root->_rightTag = _root->_rightTag;
			return root;
		}
		root = new Node(_root->_data);
		root->_leftTag = _root->_leftTag;
		root->_rightTag = _root->_rightTag;
		root->_left = _CopyTree(_root->_left);
     	root->_right = _CopyTree(_root->_right);
		return root;
	}
	//析构
	void Destroy(Node* _root)
	{
		Node* tmp = _root;
		if (tmp == NULL || tmp->_leftTag == THREAD)
		{
			if (tmp == NULL)
			{
				return;
			}
			delete tmp;
			tmp = NULL;
			return;
		}
		Destroy(_root->_left);
		Destroy(_root->_right);
		delete tmp;
		tmp = NULL;
	}
	//前序线索化
	void _PrevOrder_Thd(Node* _root)
	{
		 static Node* prev = NULL;
		if (_root)
		{
			if (!_root->_left)
			{
				_root->_leftTag = THREAD;
				_root->_left = prev;
			}
			if (prev && !prev->_right)
			{
				prev->_rightTag = THREAD;
				prev->_right = _root;
			}
			prev = _root;
			if (_root->_leftTag == LINK)
				_PrevOrder_Thd(_root->_left);
			if (_root->_rightTag == LINK)
				_PrevOrder_Thd(_root->_right);
		}
	}
	//中序线索化
	void _InOrder_Thd(Node* _root)
	{
		static Node* prev = NULL;
		if (_root)
		{
			if (_root->_leftTag == LINK)
			{
				_InOrder_Thd(_root->_left);
			}
			if (!_root->_left)
			{
				_root->_leftTag = THREAD;
				_root->_left = prev;
			}
			if (prev && !prev->_right)
			{
				prev->_rightTag = THREAD;
				prev->_right = _root;
			}
			prev = _root;
			if (_root->_rightTag == LINK)
			{
				_InOrder_Thd(_root->_right);
			}
		}
	}
	//后序线索化
	void _PostOrder_Thd(Node* _root)
	{
		if (_root == NULL)
		{
			return;
		}
		static Node* prev = NULL;
		_PostOrder_Thd(_root->_left);
		_PostOrder_Thd(_root->_right);

		if (!_root->_left)
		{
			_root->_leftTag = THREAD;
			_root->_left = prev;
		}
		if (prev && !prev->_right)
		{
			prev->_rightTag = THREAD;
			prev->_right = _root;
		}
		prev = _root;
	}
	//利用中序线索化思想将搜索二叉树转换成有序的双向链表
	void _TreeToList(Node* _root)
	{
		static Node* prev = NULL;
		if (_root == NULL)
		{
			return;
		}
		_TreeToList(_root->_left);

		_root->_left = prev;
		if (prev)
		{
			prev->_right = _root;
		}
		prev = _root;

		_TreeToList(_root->_right);
	}
private:
	Node* _root;
};
void TestBinaryTreeThd()
{
	int array[10] = { 1, 2, 3, '#', '#', 4, '#', '#', 5, 6 }; 
	int array2[15] = { 1, 2, '#', 3, '#', '#', 4, 5, '#', 6, '#', 7, '#', '#', 8 };
		//BinaryTree<int> c;
	BinaryTree<int> b(array, 10, 0, '#');
	//BinaryTreeNode<int>* ret = b.TreeToList();
	/*b.PostOrder_Thd();*/
	//b.PrevOrder_Thd();
	//b.PrevOrder();
	BinaryTree<int> c(array2,15,0,'#');
	//c.PrevOrder_Thd();
	//c.PrevOrder();
	c.InOrder_Thd();
	__TreeIterator<int,int&,int*> it = c.Begin();
	while (!it.Empty())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
	//b.InOrder();

	//b.PrevOrder_Thd();
	//b.PrevOrder();
}
int main()
{
	TestBinaryTreeThd();
	system("pause");
	return 0;
}