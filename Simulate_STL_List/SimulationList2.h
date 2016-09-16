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
	void operator=(const Self& s)
	{
		_node = s._node;
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
	Self operator+(int n)
	{
		Self tmp(_node);
		for(int i = 0;i<n;i++)
		{
			tmp._node = tmp._node->_next;
		}
		return tmp;
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
	
public:
	typedef __ListNode<T> Node;
	typedef __ListIterator<T,T&,T*> Iterator;
	typedef __ListIterator<T,const T&,const T*> const_Iterator;
	Node* BuyNode(const T& x)
	{
		Node* node = new Node(x);
		return node;
	}
	List()
	{
		_head = BuyNode(T());//T���͵���������
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
	T& Front()
	{
		return *Begin();
	}
	T& Back()
	{
		return *(--End());
	}
	void Insert(Iterator& pos,const T& x)
	{
		Node* tmp = BuyNode(x);
		Node* cur = pos._node;
		Node* PrevNode = cur->_prev;//�пգ�

		tmp->_prev = cur->_prev;
		PrevNode->_next = tmp;

		cur->_prev = tmp;
		tmp->_next = cur;
	}
	void insert1(Iterator pos,Iterator first,Iterator last)
	{
		for(;first!=last;++first)
		{
			Insert(pos,first._node->_data);
		}
	}
	void insert1(Iterator pos,const_Iterator first,const_Iterator last)
	{
		for(;first!=last;++first)
		{
			Insert(pos,first._node->_data);
		}
	}
	//void Insert2(Iterator& pos,const_Iterator& first,const_Iterator& last)//��posǰ���루first��last������
	//{
	//	for(;first!=last;++first)
	//	{
	//		insert(pos,first._node->_data);
	//	}
	//}
	//void Insert3(Iterator& pos,const T* first,const T* last)//ͬ�ϣ����������������Ͳ�ͬ
	//{
	//	for(;first!=last;++first)
	//	{
	//		insert(pos,*first);
	//	}
	//}
	void Insert4(Iterator& pos,size_t n,const T& x = T())
	{
		while(n)
		{
			Insert(pos,x);
				n--;
		}
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
	void Erase1(Iterator first,Iterator last)
	{
		while(first!=last)
		{
			Erase(first++);//��ȷд����first++����++�ٷ��أ����ص�����һ��ֵ������first����
		}                  //���忴operator++


		//����д��//first��ɾ����delete�ͷ�_nodeָ�룬
		//_node�����������ֵ����++ʱ���ͻ���һ������ֵ
		//for(;first!=last;first++)
		//{
		//	Erase(first);
		//}
	}
	void resize(size_t newsize,const T& x = T())
	{
		Iterator i = Begin();
		size_t len = 0;
		for(;len < newsize&& i!= End();i++,len++)
		{
			 if(len == newsize )
				 Erase1(i,End());//��newsizeС��sizeʱ��ɾ����������
		}
		Insert4(End(),newsize-len,x);//��newsize����sizeʱ��������Ĳ��ֳ�ʼ��Ϊx
	}
	Iterator Begin()
	{
		return Iterator(_head->_next);
	}
	//�д�
	//����	1	error C2664: ��__ListIterator<T,Ref,Ptr>::__ListIterator(__ListNode<T> *)��: ���ܽ����� 1 �ӡ�__ListIterator<T,Ref,Ptr>��ת��Ϊ��__ListNode<T> *��	e:\bit��ҵ\stl\simulatinlist\simulatinlist\simulationlist.h	224	1	SimulatinList

	//const_Iterator Begin() const
	//{
	//	Iterator tmp(_head->_next);
	//	//return Iterator(_head->_next);
 //       return tmp;	
	//}
	Iterator End()
	{
		return Iterator(_head);
	}
	const_Iterator End() const
	{
		return Iterator(_head);
	}
	void PrintList()
	{
		Iterator it = Begin();
		while(it != End())
		{
			cout<<*it<<" ";
			it++;
		}
		cout<<endl;
	}
	void clear()
	{
		Node* cur = _head->_next;
		while(cur != _head)
		{
			Node* del = cur;
			cur = cur->_next;
			delete del;
		}
		_head->_next = _head;
		_head->_prev = _head;
	}
	//void operator=(const List<T>& x)
	//{
	//	Iterator first1 = Begin();
	//	Iterator last1 = End();
 //   	const_Iterator first2 = x.Begin();
	//	const_Iterator last2 = x.End();
	//	while(first1!=last1 && first2!=last2)
	//	{
	//		*(first1++) = *(first2++);
	//	}
	//	if(first2 == last2)
	//	{
	//		Erase1(first1,last1);
	//	}
	//	if(first1 == last1)
	//	{
	//		insert1(last1,first2,last2);
	//	}
	//}
	void operator=( List<T>& x)
	{
		Iterator first1 = Begin();
		Iterator last1 = End();
		Iterator first2 = x.Begin();
		Iterator last2 = x.End();
		while(first1!=last1 && first2!=last2)
		{
			*(first1++) = *(first2++);
		}
		if(first2 == last2)
		{
			Erase1(first1,last1);
		}
		if(first1 == last1)
		{
			insert1(last1,first2,last2);
		}
	}
	void Swap(List<T>& x)
	{
		std::swap(_head,x._head);
	}
	size_t max_size()
	{
		return size_t(-1)/sizeof(T);
	}
	size_t Distance(Iterator first,Iterator last,size_t ret)
	{
		ret = 0;
		while(first!=last)
		{
			ret++;
			++first;
		}
		return ret;
	}
	size_t size()
	{
		size_t ret = 0;
		return Distance(Begin(),End(),ret);
	}
	void Remove(const T& x)
	{
		Iterator first = Begin();
		Iterator last = End();
		while(first!=last)
		{
			Iterator tmp = first;
			tmp++;
			if(*first == x)
			{
				Erase(first);
			}
			first = tmp;
		}
	}
private:
	Node* _head;
};
void TestList()
{
	List<int> l;
	l.Pushback(1);
	l.Pushback(2);
	l.Pushback(2);
	l.Pushback(2);
	l.Pushback(5);
	List<int> l2;
	l2.PrintList();
	cout<<l2.size()<<endl;
	l.Remove(2);
	l.PrintList();
	cout<<l.size()<<endl;
}
