#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

//#define MAX_POOLEDOBJECT	32

#include <vector>

template<class T>
class ObjectPool
{
protected:
	std::vector<T*> stack;
	int m_nMaxPooled;
	
public:
	ObjectPool(int nMaxPooled = 64) : m_nMaxPooled(nMaxPooled)
	{
	}

	~ObjectPool()
	{
		std::vector<T*>::iterator iter = stack.begin();
		while(iter != stack.end() )
		{
			delete *iter;
		}

		stack.clear();
	}

	T* alloc()
	{
		if(stack.size() )
		{
			T* ret = stack.back();
			stack.pop_back();
			return ret;
		}
		else
		{
			return new T;
		}
	}

	void release(T* ptr)
	{
		if(stack.size() > m_nMaxPooled)
		{
			delete ptr;
		}
		else
		{
			stack.push_back(ptr);
		}
	}
};

#endif //__OBJECTPOOL_H__