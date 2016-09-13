#pragma once
#include<iostream>
#include<cassert>
using namespace std;

template<typename T>
struct __ListNode
{
	__ListNode<T>* _prev;
	__ListNode<T>* _next;
	T _data;

	__ListNode(const T& x)
		:_prev(NULL)
		,_next(NULL)
		,_data(x)
	{}
};

template<typename T,typename Ref,typename Ptr>
struct __ListIterator
{
	typedef __ListNode<T> Node;
	typedef __ListIterator<T,Ref,Ptr> Self;
public:
	__ListIterator()
	{}
	__ListIterator(Node* node)
		:_node(node)
	{}
	bool operator==(const Self& s)
	{
		return _node == s._node;
	}
	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}
	Self& operator++()
	{
		_node = _node->_next;

		return *this;
	}
	Self operator++(int)
	{
		Self tmp(_node);
		//Node* tmp = _node;
		_node = _node->_next;

		return tmp;
	}
	Self& operator--()
	{
		_node = _node->_prev;

		return *this;
	}
	Self operator--(int)
	{
		Self tmp(_node);
		//Node tmp = _node;
		_node = _node ->_prev;

		return tmp;
	}
	Ref operator*()
	{
		return _node->_data;
	}
	Ptr operator->()
	{
		return &(_node->_data);
	}
	Node* _node;

};
template<typename T>
class List
{
	typedef __ListNode<T> Node;
public:
	typedef __ListIterator<T,T&,T*> Iterator;
	Node* BuyNode(const T& x)
	{
		Node* node = new Node(x);
		return node;
	}
	List()
	{
		_head = BuyNode(T());//T类型的匿名对象
		_head->_next = _head;
		_head->_prev = _head;
	}
	void Pushback(const T& x)
	{
		Insert(End(),x);
	}
	void PushFront(const T& x)
	{
		Insert(Begin(),x);
	}
	void PopBack()
	{
		Erase(--End());
	}
	void PopFront()
	{
		Erase(Begin());
	}
	void Insert(Iterator& pos,const T& x)
	{
		Node* tmp = BuyNode(x);
		Node* cur = pos._node;
		Node* PrevNode = cur->_prev;//判空？

		tmp->_prev = cur->_prev;
		PrevNode->_next = tmp;

		cur->_prev = tmp;
		tmp->_next = cur;
	}
	void Erase(Iterator& pos)
	{
		assert(pos != End());
		if(_head->_next != _head)
		{
			Node* del = pos._node;
			Node *PrevNode = del->_prev;
			Node *NextNode = del->_next;
			
			NextNode->_prev = PrevNode;
			PrevNode->_next = NextNode;
			
			delete del;
			del = NULL;
		}
	}
	Iterator Begin()
	{
		return Iterator(_head->_next);
	}
	Iterator End()
	{
		return Iterator(_head);
	}
	void PrintList()
	{
		Iterator it = Begin();
		while(it != End())
		{
			cout<<*it<<endl;
			it++;
		}
	}
private:
	Node *_head;
};
void TestList()
{
	List<int> l;
	l.Pushback(1);
	l.Pushback(2);
	l.Pushback(3);
	l.Pushback(4);
	l.PushFront(0);
	l.PopBack();
	l.PopFront();
	l.PrintList();
	List<int>::Iterator it = l.Begin();
	
	while(it != l.End())
	{
		cout<<*it<<endl;
		++it;
	}
}
