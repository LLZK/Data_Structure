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
		void *_memory;//指向内存块的指针
		BlockNode* _next;//指向下一结点的指针
		size_t _objNum;//内存对象的个数

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
	size_t _CountIn;//当前结点使用到第几个内存块计数
	BlockNode *_first;
	BlockNode *_last;
	size_t _maxNum;
	static size_t _itemSize;//单个对象的大小
	T* _lastDelete;//释放指针

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
			//析构del时会调用BlockNode析构函数，在BlockNode析构函数内将_memory释放（delete）
		}
	}
	static size_t InitItemSize();//itemSize(单个对象大小)初始化函数

	T* New()
	{
		if (_lastDelete)
		{
			T* obj = _lastDelete;
			_lastDelete = *((T**)_lastDelete); //得到指针大小的空间的数据
			//也可	_lastDelete = *((int**)_lastDelete);
			//也可	_lastDelete = *((void**)_lastDelete);
			//...
			// 只要是指针类型就行，其他无所谓。
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
				cout << "开辟失败 _last->_next !!" << endl;
			}
			_last = _last->_next;
			_CountIn = 0;
		}

		//内存是已经在内存池中开辟好的;
		//直接把相应位置内存的地址放到obj中返回;
		//转化为char*是要让地址做单字节加法
		T* obj = (T*)((char*)_last->_memory + _CountIn*_itemSize);
		_CountIn++;

		return new(obj)T;//定位new表达式，对已有的一块空间进行初始化
	}

	void Delete(T* ptr)
	{
		ptr->~T();  //把ptr指向空间的对象析构
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
	//被释放的空间要存储上一个被释放空间的地址，所以对象大小至少能存下一个指针。
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
// 测试内存对象池的常规使用和内存对象的重复使用
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



// 针对当前的内存对象池进行简单的性能测试
void TestObjectPoolOP()
{
	size_t begin, end;
	vector<string*> v;
	const size_t N = 100000;
	v.reserve(N);

	cout << "pool new/delete===============================" << endl;
	// 反复申请释放5次
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
