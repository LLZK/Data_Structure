#include<iostream>
using namespace std;

#include<Windows.h>
#include<string>
#include<vector>

template<typename T>
class ObjectPool
{
	struct BlockNode
	{
		void *_memory;//ָ���ڴ���ָ��
		BlockNode* _next;//ָ����һ����ָ��
		size_t _objNum;//�ڴ����ĸ���

		BlockNode(size_t objNum)
			:_objNum(objNum)
			, _next(NULL)
		{
			_memory = malloc(_itemSize*_objNum);
		}
		~BlockNode()
		{
			free(_memory);
			_memory = NULL;
			_next = NULL;
		}
	};
protected:
	size_t _CountIn;//��ǰ���ʹ�õ��ڼ����ڴ�����
	BlockNode *_first;
	BlockNode *_last;
	size_t _maxNum;
	static size_t _itemSize;//��������Ĵ�С
	T* _lastDelete;//�ͷ�ָ��

public:
	ObjectPool(size_t initNum = 32,size_t maxNum = 100000)
		:_CountIn(0)
		, _maxNum(maxNum)
	{
		_first = _last = new BlockNode(initNum);
		_lastDelete = NULL;
	}
	~ObjectPool()
	{
		Destroy();
	}
	void Destroy()
	{
		BlockNode* cur = _first;
		BlockNode* del = NULL;
		while (cur)
		{
			del = cur;
			cur = cur->_next;
			delete del;
			//����delʱ�����BlockNode������������BlockNode���������ڽ�_memory�ͷţ�delete��
		}
	}
	static size_t InitItemSize();//itemSize(���������С)��ʼ������

	T* New()
	{
		if (_lastDelete)
		{
			T* obj = _lastDelete;
			_lastDelete = *((T**)_lastDelete); //�õ�ָ���С�Ŀռ������
			//Ҳ��	_lastDelete = *((int**)_lastDelete);
			//Ҳ��	_lastDelete = *((void**)_lastDelete);
			//...
			// ֻҪ��ָ�����;��У���������ν��
			return new(obj)T;
		}
		if (_last->_objNum == _CountIn)
		{
			size_t newObjNum = _last->_objNum * 2;
			if (newObjNum >= _maxNum)
			{
				newObjNum = _maxNum;
			}
			_last->_next = new BlockNode(newObjNum);
			if (_last->_next == NULL)
			{
				cout << "����ʧ�� _last->_next !!" << endl;
			}
			_last = _last->_next;
			_CountIn = 0;
		}

		//�ڴ����Ѿ����ڴ���п��ٺõ�;
		//ֱ�Ӱ���Ӧλ���ڴ�ĵ�ַ�ŵ�obj�з���;
		//ת��Ϊchar*��Ҫ�õ�ַ�����ֽڼӷ�
		T* obj = (T*)((char*)_last->_memory + _CountIn*_itemSize);
		_CountIn++;

		return new(obj)T;//��λnew���ʽ�������е�һ��ռ���г�ʼ��
	}

	void Delete(T* ptr)
	{
		ptr->~T();  //��ptrָ��ռ�Ķ�������
		if (ptr)
		{
			*(T**)ptr = _lastDelete;
			_lastDelete = ptr;
		}
	}
};

template<typename T>
 size_t ObjectPool<T>::InitItemSize()
{
	//���ͷŵĿռ�Ҫ�洢��һ�����ͷſռ�ĵ�ַ�����Զ����С�����ܴ���һ��ָ�롣
	if (sizeof(T) <= sizeof(void*))
	{
		return sizeof(void*);
	}
	else
	{
		return sizeof(T);
	}
}

template<typename T>
 size_t ObjectPool<T>::_itemSize = ObjectPool<T>::InitItemSize();

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// �����ڴ����صĳ���ʹ�ú��ڴ������ظ�ʹ��
void TestObjectPool()
{
	vector<string*> v;

	ObjectPool<string> pool;
	for (size_t i = 0; i < 32; ++i)
	{
		v.push_back(pool.New());
		printf("Pool New [%d]: %p\n", i, v.back());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < 32; ++i)
	{
		v.push_back(pool.New());
		printf("Pool New [%d]: %p\n", i, v.back());
	}

	v.push_back(pool.New());
}



// ��Ե�ǰ���ڴ����ؽ��м򵥵����ܲ���
void TestObjectPoolOP()
{
	size_t begin, end;
	vector<string*> v;
	const size_t N = 100000;
	v.reserve(N);

	cout << "pool new/delete===============================" << endl;
	// ���������ͷ�5��
	begin = GetTickCount();
	ObjectPool<string> pool;
	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}


	end = GetTickCount();
	cout << "Pool:" << end - begin << endl;

	cout << "new/delete===============================" << endl;
	begin = GetTickCount();

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	end = GetTickCount();
	cout << "new/delete:" << end - begin << endl;
}
