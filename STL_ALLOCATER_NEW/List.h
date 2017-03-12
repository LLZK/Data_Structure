#pragma once
#include<iostream>
#include"DefaultAllocTemplate.h"
#include"Construct.h"
#include"Iterator_Trais.h"
using namespace std;


template<typename T>
struct ListNode
{
	ListNode<T> * _next;
	ListNode<T>* _prev;
	T _data;

	ListNode(T data = T())
		:_data(data)
		, _next(NULL)
		, _prev(NULL)
	{
	}
};


/*
List的迭代器 ListIterator
*/

template<typename T,typename Ref,typename Ptr>
struct ListIterator
{
	typedef ListNode<T> Node;
	typedef ListIterator<T, Ref, Ptr>Self;

	typedef BidirectionalIteratorTag IteratorCategory;
	typedef size_t DifferenceType;
	typedef T ValueType;
	typedef T* Pointer;
	typedef T& Reference;
	
	Node* _node;

	ListIterator(Node* node)
		:_node(node)
	{}

	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		return &(operator*());
	}

	Self operator++()
	{
		_node = _node->_next;
		return *this;
	}

	Self operator++(int)
	{
		Node *tmp = _node;
		_node = _node->_next;
		return tmp;
	}

	Self operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	Self operator--(int)
	{
		Node *tmp = _node;
		_node = _node->_prev;
		return tmp;
	}

	bool operator != (const Self& s) const
	{
		return _node != s._node;
	}

	bool operator==(const Self& s) const
	{
		return !(*this != s);
	}
};




/*
List
*/
template<typename T,typename _Alloc = Alloc>
class List
{
	typedef ListNode<T> Node;
	typedef SimpleAlloc<Node, Alloc> ListNodeAllocater;
public:
	typedef ListIterator<T, T&, T*> Iterator;
	typedef ListIterator<T, const T&, const T*> ConstIterator;



	List()
		:_head (BuyNode(T()))
	{
			_head->_next = _head;
			_head->_prev = _head;
	}
	~List()
	{
		Node* cur = _head;
		while (cur != _head)
		{
			DestroyNode(cur);
			cur = cur->_next;
		}
		DestroyNode(_head);
	}

	Node* BuyNode(const T& x = T())
	{
		Node* node = ListNodeAllocater::Allocate();
		Construct(node, x);
		return node;
	}
	void DestroyNode( Node* p)
	{
		Destroy(p);
		ListNodeAllocater::Deallocate(p);
	}

	void PushBack(const T& x)
	{
		Insert(Iterator(_head), x);
	}

	void PushFront()
	{
		Insert(Iterator(_head->_next), x);
	}

	void PopBack()
	{
		Erase(Iterator(_head->_prev));
	}

	void PopFront()
	{
		Erase(Iterator(_head->_next));
	}

	void Insert(Iterator postion, const T& x)
	{
		Node* node = BuyNode(x);
		Node* cur = postion._node;
		Node* prev = cur->_prev;

		prev->_next = node;
		node->_prev = prev;

		cur->_prev = node;
		node->_next = cur;
	}

	void Clear()
	{
		Node* cur = _head;
		while (cur != _head)
		{
			Node * del = cur;
			cur = cur->_next;
			DestroyNode(del);
		}
		_head->_next = _head;
		_head->_prev = _head;
	}

	void Erase(Iterator pos)
	{
		Node* node = pos->_node;
		Node* prev = node->_prev;
		Node* next = node->_next;

		prev->_next = next;
		next->_prev = prev;

		DestroyNode(node);
		return Iterator(next);
	}


	Iterator Begin()
	{
		return _head->_next;

		//return Iterator(_head->_next)   调用迭代器构造函数
		//因为是值返回，返回的是临时变量的值，在返回时会再调用一次拷贝构造，将拷贝体返回。
		//直接返回指针，编译器会优化，将构造，拷贝构造过程优化成一步操作。构造完后直接返回。
	}

	ConstIterator Begin() const
	{
		return _head->_next;
	}

	Iterator End()
	{
		return _head;
	}

	ConstIterator End() const
	{
		return _head;
	}

private:
	Node* _head;
};


void PrintList( const List<int>& l)
{
	List<int> ::ConstIterator it = l.Begin();
	while (it != l.End())
	{
		cout << *it << endl;
		++it;
	}
}
void TestList()
{
	List<int> L;
	L.PushBack(1);
	L.PushBack(2);
	L.PushBack(3);
	L.PushBack(5);
	List<int>::Iterator it1 = L.Begin();
	List<int>::Iterator it2 = L.End();
	//Advance(it, -2);
	cout << "Distance?" << Distance(it1, it2) << endl;
	//cout << *it << endl;
	//PrintList(L);
}