#ifndef _CIRCUITQUEUE_H_
#define _CIRCUITQUEUE_H_

#include <windows.h>
#include <stdio.h>
#include <assert.h>

//=============================================================================================================================
/// ���� ť Ŭ���� ���ø�
/**
	@author
			Baek Sang Hyun
	@since
			2004. 5. 30
*/
//=============================================================================================================================
template<typename T>
class CircuitQueue
{
public:
	CircuitQueue() : m_pData( NULL ), m_nLength( 0 ), m_nSize( 0 ), m_nHead( 0 ), m_nTail( 0 )
	{
		InitializeCriticalSection( &m_cs );		
	}
	
	virtual ~CircuitQueue()
	{
		if( m_pData ) delete [] m_pData;
		DeleteCriticalSection( &m_cs );
	}
	
//=============================================================================================================================
/**
	@remarks
			ť�� �����Ѵ�.
	@param	nSize
			ť ������
	@param	nExtraSize
			��Ŷ�� ���� ���ʿ��� �������� ������ �� �� ��Ŷ�� �̾��ֱ� ���� Ȯ����
			���� ���� ������ �޸� ũ��(���� ���� �޸� = sizeof(T) * nExtraSize )
*/
//=============================================================================================================================
	void Create( int nSize, int nExtraSize = 0 )
	{
		EnterCriticalSection( &m_cs );
		if( m_pData ) delete [] m_pData;

		m_pData			= new T[nSize + nExtraSize];
		m_nSize			= nSize;
		m_nExtraSize	= nExtraSize;
		LeaveCriticalSection( &m_cs );
	}

//=============================================================================================================================
/**
	@remarks
			ť�� û���Ѵ�.
*/
//=============================================================================================================================
	inline void Clear()
	{
		EnterCriticalSection( &m_cs );

		m_nLength       = 0;
		m_nHead         = 0;
		m_nTail         = 0;

		LeaveCriticalSection( &m_cs );
	}
	
	/// ť�� ���� ������ �����Ѵ�.
	inline int      GetSpace()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		iRet = m_nSize - m_nLength;		
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

	/// ���� ������� ť�� ũ�⸦ �����Ѵ�.
	inline int      GetLength()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		iRet = m_nLength;
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

	/// �����Ͱ� ť �������� �� ���� ������ �̾����� ���, ���� �������� ������ ������ �����Ѵ�.
	inline int IsCutData()
	{
	}

	inline int      GetBackDataCount()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		iRet = m_nSize - m_nHead;
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

	/// �����͸� ���� ������(Head)�� �����Ѵ�.
	inline T*       GetReadPtr()
	{
		T *pRet;

		EnterCriticalSection( &m_cs );
		pRet = m_pData + m_nHead;
		LeaveCriticalSection( &m_cs );

		return pRet;
	}

	/// �����͸� �� ������(Tail)�� �����Ѵ�.
	inline T*       GetWritePtr()
	{
		T *pRet;

		EnterCriticalSection( &m_cs );
		pRet = m_pData + m_nTail;
		LeaveCriticalSection( &m_cs );

		return pRet;
	}
	
	/// �߸��� �ʰ� �ѹ��� ���� �� �ִ� ���̸� �����Ѵ�.
	inline int GetReadableLen()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		if( m_nHead == m_nTail )		iRet = GetLength() > 0 ? m_nSize - m_nHead: 0;
		else if( m_nHead < m_nTail )	iRet = m_nTail - m_nHead;
		else							 iRet = m_nSize - m_nHead;
		LeaveCriticalSection( &m_cs );

		return iRet;
	}
	
	/// �߸��� �ʰ� �ѹ��� ���� ������ ���̸� �����Ѵ�.
	inline int GetWritableLen()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		if( m_nHead == m_nTail )		iRet = GetLength() > 0 ? 0 : m_nSize - m_nTail;
		else if( m_nHead < m_nTail )	iRet = m_nSize - m_nTail;
		else							iRet = m_nHead - m_nTail;
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

//=============================================================================================================================
/**
	@remarks
			ť�� �����͸� �ִ´�.
	@param	pSrc
			ť�� ������ ������ ������
	@param	nSize
			ť�� ������ �������� ����
	@retval	BOOL
			���������� ������ ������ ������ TRUE�� �����Ѵ�.
			ť�� ������ ������ �� ���� ��� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
	inline BOOL Enqueue( T *pSrc, int nSize )
	{
		EnterCriticalSection( &m_cs );

		if( GetSpace() < nSize )
		{
			LeaveCriticalSection( &m_cs );
			return FALSE;
		}

		// pData�� NULL�� ��쿡�� ������ ť�� ���� tail�� length�� �����Ѵ�.
		if( pSrc != NULL )
		{
			if( m_nHead <= m_nTail )
			{
				// 1. head�� tail���� �տ� �ִ� ���
				int nBackSpaceCount = m_nSize - m_nTail;

				if( nBackSpaceCount >= nSize )  
				{
					// ���ʿ� ���� ������ ������ �纸�� ���ų� ū ��� �ѹ��� �����Ѵ�.
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
				}
				else
				{
					// ���ʿ� ���� ������ ������ �纸�� ���� ��� �ι��� ������ �����Ѵ�.
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nBackSpaceCount );
					memcpy( m_pData, pSrc + nBackSpaceCount, sizeof(T) * ( nSize - nBackSpaceCount ) );
				}
			}
			else
			{
				// 2. head�� tail���� �ڿ� �ִ� ���
				memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
			}
		}

		m_nTail		+= nSize;
		m_nTail		%= m_nSize;
		m_nLength	+= nSize;

		LeaveCriticalSection( &m_cs );

		return TRUE;
	}

//=============================================================================================================================
/**
	@remarks
			ť���� �����͸� ������.
	@param	pTar
			ť���� ���� �����͸� ������ ������ ������
	@param	nSize
			ť���� ���� ������ ����
	@retval	BOOL
			���������� �����͸� ������ TRUE�� �����Ѵ�.
			���� �����Ͱ� nSize���� ���� ��� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
	inline BOOL Dequeue( T *pTar, int nSize )
	{    
		EnterCriticalSection( &m_cs );

		if( !Peek( pTar, nSize ) )
		{
			LeaveCriticalSection( &m_cs );
			return FALSE;
		}

		m_nHead		+= nSize;
		m_nHead		%= m_nSize;
		m_nLength	-= nSize;

		LeaveCriticalSection( &m_cs );

		return TRUE;
	}

//=============================================================================================================================
/**
	@remarks
			ť���� �����͸� ������ �ʰ� ���縸 �ؿ´�.
	@param	pTar
			ť�� �����͸� ������ ������ ������
	@param	nSize
			ť���� ������ ������ ����
	@retval	BOOL
			���������� �����͸� �����ϸ� TRUE�� �����Ѵ�.
			���� �����Ͱ� nSize���� ���� ��� FALSE�� �����Ѵ�.
*/
//=============================================================================================================================
	inline BOOL Peek( T *pTar, int nSize )
	{
		EnterCriticalSection( &m_cs );

		if( m_nLength < nSize )
		{
			LeaveCriticalSection( &m_cs );
			return FALSE;
		}

		// ������ ���� �����Ͱ� NULL�� �ƴ� ��쿡�� �޸𸮸� �����Ѵ�.
		if( pTar != NULL )
		{
			if( m_nHead < m_nTail )
			{
				// 1. head�� tail���� �տ� �ִ� ���
				memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );
			}
			else
			{
				// 2. head�� tail���� �ڿ� �ִ� ���
				if( GetBackDataCount() >= nSize )
				{
					// ���ʿ� ���� �����Ͱ� ������ �纸�� ���ų� ū ��� nSize��ŭ �ѹ��� �����Ѵ�.
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );                           
				}
				else
				{
					// ���ʿ� ���� �����Ͱ� ������ �纸�� ���� ��� �ι��� ������ �����Ѵ�.
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * GetBackDataCount() );
					memcpy( pTar + GetBackDataCount(), m_pData, sizeof(T) * ( nSize - GetBackDataCount() ) );
				}
			}
		}

		LeaveCriticalSection( &m_cs );

		return TRUE;
	}

//=============================================================================================================================
/**
	@remarks
			��Ŷ�� ���� ���ʿ��� �������� �������� �� ��Ŷ�� �̾������� ������ ������ ī���Ѵ�.
	@param	nSize
			���ʿ��� �߷��� �������� �̾��� �������� ����
*/
//=============================================================================================================================
	inline void CopyHeadDataToExtraBuffer( int nSize )
	{
		assert( nSize <= m_nExtraSize );

		EnterCriticalSection( &m_cs );

		// ���� �� ���� ���� ���ۿ��ٰ� ���� �� ���ʿ� �ִ� �����͸� ����.
		memcpy( m_pData + m_nSize, m_pData, nSize );

		LeaveCriticalSection( &m_cs );
	}

protected:
	CRITICAL_SECTION	m_cs;
	T					*m_pData;			/// ť ���� ������
	int					m_nLength;			/// ���� ������ ����
	int					m_nSize;			/// ť ������ ��ü ũ��
	int					m_nHead;			/// ���� �����Ͱ� ���۵Ǵ� ���� ��ġ
	int					m_nTail;			/// ���� �����Ͱ� ���� ���� ��ġ
	int					m_nExtraSize;		/// ť ���ʿ� �������� ���� ���� ũ�� 
};

#endif










