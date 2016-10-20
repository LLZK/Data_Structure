#pragma once
#include<iostream>
using namespace std;

enum Type
{
	THREAD,
	LINK
};
template<typename T>
struct BinaryTreeNode
{
	T _data;
	BinaryTreeNode<T> *_left;
	BinaryTreeNode<T> *_right;
	Type _leftTag;
	Type _rightTag;
	BinaryTreeNode(const T& x)
		:_data(x)
		, _left(NULL)
		, _right(NULL)
		, _leftTag(LINK)
		, _rightTag(LINK)
	{}

	BinaryTreeNode()
	{}
};

template<typename T,typename Ref,typename Ptr>
struct __TreeIterator
{
	typedef BinaryTreeNode<T> Node;
	typedef __TreeIterator<T,Ref,Ptr> Self;
public:
	__TreeIterator()
	{}
	__TreeIterator(Node* node)
		:_node(node)
	{}
	bool Empty()
	{
		return _node == NULL;
	}
	Self& operator++()
	{
		if (_node->_rightTag == THREAD)
		{
			_node = _node->_right;
		}
		else
		{
			_node = _node->_right;
			if (_node == NULL)
			{
				return *this;
			}
			while (_node->_leftTag == LINK)
			{
				_node = _node->_left;
			}
		}
		return *this;
	}
	Self operator++(int)
	{
		Self tmp = *this;
		++*this;
		return tmp;
	}
	Self& operator--()
	{
		_node = _node->_left;
		return *this;
	}
	Self operator--(int)
	{
		Self tmp = *this;
		--*this;
		return tmp;
	}
	T& operator*()
	{
		return _node->_data;
	}
	Node* _node;
};