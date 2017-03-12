#pragma once
#include<iostream>
using namespace std;
struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag: public InputIteratorTag{};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};


template <class Category, class T, class distance = ptrdiff_t,
class Pointer = T*, class Reference = T&>
struct Iterator
{
	typedef Category IteratorCategory; // ����������
	typedef T ValueType; // ��������ָ��������
	typedef distance DifferenceType; // ����������֮��ľ���
	typedef Pointer Pointer; // ��������ָ�������͵�ָ��
	typedef Reference Reference; // ��������ָ�������͵�����
};

template <class Iterator>
struct IteratorTraits
{
	typedef typename Iterator::IteratorCategory IteratorCategory;
	typedef typename Iterator::ValueType ValueType;
	typedef typename Iterator::DifferenceType DifferenceType;
	typedef typename Iterator::Pointer Pointer;
	typedef typename Iterator::Reference Reference;
};

// ƫ�ػ�ԭ��ָ������
//
template <class T>
struct IteratorTraits<T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef T* Pointer;
	typedef T& Reference;
};
//
// ƫ�ػ�constԭ��ָ������
//
template <class T>
struct IteratorTraits<const T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef const T* Pointer;
	typedef const T& Reference;
};


template<class InputIterator,class distance>
inline void __Advancce(InputIterator &i, distance, InputIteratorTag)
{
	while (n--)
	{
		++i;
	}
}


template<class BidirectionIterator,class distance>
inline void __Advance(BidirectionIterator &i, distance n, BidirectionalIteratorTag)
{
	if (n >= 0)
	{
		while (n--)//�����ǰ��--�ͻ�����һ��
		{
			++i;
		}
	}
	else
	{
		while (n++)
		{
			--i;
		}
	}
}

template<class RandomAccessIterator,class distance>
inline void __Advance(RandomAccessIterator& i, distance n, RandomAccessIteratorTag)
{
	i += n;
}

template<class InputIterator,class distance>
inline void Advance(InputIterator& i, distance n)
{
	//__Advance( i, n, IteratorTraits<InputIterator>::IteratorCategory());
	__Advance(i, n, InputIterator::IteratorCategory());
}




//////////////////////////////////////////////////////////////////////////////
//Distance

template<class InputIterator>
inline typename IteratorTraits<InputIterator>::DifferenceType 
 Distance(InputIterator first, InputIterator last)
{
	//return __Distance(first, last, IteratorTraits<InputIterator>::IteratorCategory());
	return __Distance(first, last, InputIterator::IteratorCategory());
}


template<class RandomAccessIterator>
inline typename IteratorTraits<RandomAccessIterator>::DifferenceType
 __Distance(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIteratorTag)
{
	return last - first;
}

template<class InputIterator>
inline typename IteratorTraits<InputIterator>::DifferenceType
 __Distance(InputIterator first, InputIterator last, InputIteratorTag)
{
	int n = 0;
	while (first != last)
	{
		++first;
		++n;
	}
	return n;
}







