#ifndef __RINGARRAY_H__
#define __RINGARRAY_H__

//=============================================================================================================================
// m_byMaxSize ������ Data History�� �����. �� ���� ������ ����Ÿ�� ����� ���ο� ����Ÿ�� �����Ѵ�.
//=============================================================================================================================


#include <vector>

namespace util
{


template <typename T>
class RingArray
{
public:
	RingArray()
	{
	}

	~RingArray()
	{
	}

	void Init(DWORD iMaxSize)
	{
		m_dwCurSize = m_dwCurPos = 0;
		T TempData;
		m_Array.insert(m_Array.begin(), iMaxSize, TempData);
	}

	void InsertData(T Data)
	{
		m_Array[m_dwCurPos] = Data;

		if(++m_dwCurPos >= m_Array.capacity())
		{
			m_dwCurPos = 0;
		}
		if(++m_dwCurSize > m_Array.capacity())
		{
			--m_dwCurSize;
		}
	}

	T AverageData()
	{
		T SumData;
		DWORD dwPos;
		memset(&SumData, 0, sizeof(T));

		for(DWORD i=0; i< m_dwCurSize; i++)
		{
			dwPos = m_dwCurPos+i;
			if(dwPos >= m_dwCurSize)	dwPos -= m_dwCurSize;

			SumData += m_Array[dwPos];
		}

		return SumData * (float)(1/m_dwCurSize);
	}

	DWORD GetSize()
	{
		return m_dwCurSize;
	}


private:
	DWORD				m_dwCurSize;
	DWORD				m_dwCurPos;		// ���� �����Ͱ� �� �ڸ�
	std::vector<T>		m_Array;
};


}

#endif