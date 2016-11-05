#pragma once
#include<iostream>
#include"Heap.h"
using namespace std;

template<typename T>
struct HuffmanTreeNode
{
	T _weight;// 权值
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;

	HuffmanTreeNode(const T& weight)
		: _weight(weight)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}
};



template<typename T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree( T *a, size_t sz,const T& invalid)
	{

		//重写仿函数
		struct NodeMax
		{
			bool operator()(Node* l, Node* r) const
			{
				return (l->_weight) > (r->_weight);
			}
		};
	   //建小堆
		Heap<Node*,NodeMax> minHeap;

		for (size_t i = 0; i < sz; ++i)
		{

			//剔除哈希表中多余的元素
			if (a[i] != invalid)
			{
				Node *node = new Node(a[i]);
				minHeap.Push(node);
			}
		}

		while (minHeap.Size() > 1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();
			Node* parent = new Node(left->_weight + right->_weight);

			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;

			minHeap.Push(parent);
		}
		_root = minHeap.Top();
	}
	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _root;
};

//void Test()
//{
//	int a[4] = {4,3,2,1};
//	HuffmanTree<int> tree1(a, 4);
//}

