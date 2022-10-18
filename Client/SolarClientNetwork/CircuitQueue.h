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
    @history
            090416 - waverix : add-in method for double buffering
            090424 - waverix : select lock functionality
*/
//=============================================================================================================================

template<typename T>
class CircuitQueue;

typedef CircuitQueue<BYTE>  CircuitQueueByte;


#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__


template<typename T>
class CircuitQueue
{
public:
    CircuitQueue()
        : m_bNeedLock(0)
        , m_pData(NULL), m_nLength(0), m_nSize(0)
        , m_nHead(0), m_nTail(0), m_nExtraSize(0)
    {
        InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
    }

    virtual ~CircuitQueue()
    {
        if(m_pData)
            delete [] m_pData;
        DeleteCriticalSection(&m_cs);
    }

    void LockQueue()
    {
        if(m_bNeedLock)
            EnterCriticalSection(&m_cs);
    };

    void UnlockQueue()
    {
        if(m_bNeedLock)
            LeaveCriticalSection(&m_cs);
    }

    //=============================================================================================================================
    /**
        @remarks
       ť�� �����Ѵ�.
        @param	nSize
       ť ������
        @param	nExtraSize
       ��Ŷ�� ���� ���ʿ��� �������� ������ �� �� ��Ŷ�� �̾��ֱ� ���� Ȯ����
       ���� ���� ������ �޸� ũ��(���� ���� �޸� = sizeof(T) * nExtraSize)
    */
    //=============================================================================================================================
    void Create(int nSize, int nExtraSize = 0, BOOLEAN bDisableLock = 0)
    {
        if(bDisableLock == false)
            m_bNeedLock = true;

        //LockQueue();
        if(m_pData)
            delete [] m_pData;

        m_pData = new T[nSize + nExtraSize];
        m_nSize = nSize;
        m_nExtraSize = nExtraSize;
        //UnlockQueue()
    }

    //=============================================================================================================================
    /**
        @remarks
       ť�� û���Ѵ�.
    */
    //=============================================================================================================================
    inline void Clear()
    {
        LockQueue();
        m_nLength = m_nHead = m_nTail = 0;
        UnlockQueue();
    }

    // (WAVERIX) (BUG-FIX) ����ġ ���� �� ���� ���� �����ϱ� ���� �۾� + �ڵ� �ߺ� ����
    #define __GetSpace()            (m_nSize - m_nLength)
    #define __GetLength()           (m_nLength)
    #define __GetBackDataCount()    (m_nSize - m_nHead)
    #define __GetReadPtr()          (m_pData + m_nHead)
    #define __GetWritePtr()         (m_pData + m_nTail)

    /// ť�� ���� ������ �����Ѵ�.
    inline int GetSpace()
    {
        int iRet;

        LockQueue();
        iRet = __GetSpace();		
        UnlockQueue();

        return iRet;
    }

    /// ���� ������� ť�� ũ�⸦ �����Ѵ�.
    inline int GetLength()
    {
        int iRet;

        LockQueue();
        iRet = __GetLength();
        UnlockQueue();

        return iRet;
    }

    /// �����Ͱ� ť �������� �� ���� ������ �̾����� ���, ���� �������� ������ ������ �����Ѵ�.
    inline int GetBackDataCount()
    {
        int iRet;

        LockQueue();
        iRet = __GetBackDataCount();
        UnlockQueue();

        return iRet;
    }

    /// �����͸� ���� ������(Head)�� �����Ѵ�.
    inline T* GetReadPtr()
    {
        T *pRet;

        LockQueue();
        pRet = __GetReadPtr();
        UnlockQueue();

        return pRet;
    }

    /// �����͸� �� ������(Tail)�� �����Ѵ�.
    inline T* GetWritePtr()
    {
        T *pRet;

        LockQueue();
        pRet = __GetWritePtr();
        UnlockQueue();

        return pRet;
    }

    /// �߸��� �ʰ� �ѹ��� ���� �� �ִ� ���̸� �����Ѵ�.
    inline int GetReadableLen()
    {
        int iRet;

        LockQueue();
        if(m_nHead == m_nTail)      iRet = __GetLength() > 0 ? m_nSize - m_nHead: 0;
        else if(m_nHead < m_nTail)  iRet = m_nTail - m_nHead;
        else                        iRet = m_nSize - m_nHead;
        UnlockQueue();

        return iRet;
    }

    /// �߸��� �ʰ� �ѹ��� ���� ������ ���̸� �����Ѵ�.
    inline int GetWritableLen()
    {
        int iRet;

        LockQueue();
        if(m_nHead == m_nTail)      iRet = __GetLength() > 0 ? 0 : m_nSize - m_nTail;
        else if(m_nHead < m_nTail)  iRet = m_nSize - m_nTail;
        else                        iRet = m_nHead - m_nTail;
        UnlockQueue();

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
    BOOL Enqueue(T* pSrc, int nSize)
    {
        LockQueue();

        if(__GetSpace() < nSize)
        {
            UnlockQueue();
            return FALSE;
        }

        // pData�� NULL�� ��쿡�� ������ ť�� ���� tail�� length�� �����Ѵ�.
        if(pSrc != NULL)
        {
            if(m_nHead <= m_nTail)
            {
                // 1. head�� tail���� �տ� �ִ� ���
                int nBackSpaceCount = m_nSize - m_nTail;

                if(nBackSpaceCount >= nSize)  
                {
                    // ���ʿ� ���� ������ ������ �纸�� ���ų� ū ��� �ѹ��� �����Ѵ�.
                    memcpy(m_pData + m_nTail, pSrc, sizeof(T) * nSize);
                }
                else
                {
                    // ���ʿ� ���� ������ ������ �纸�� ���� ��� �ι��� ������ �����Ѵ�.
                    memcpy(m_pData + m_nTail, pSrc, sizeof(T) * nBackSpaceCount);
                    memcpy(m_pData, pSrc + nBackSpaceCount, sizeof(T) * (nSize - nBackSpaceCount));
                }
            }
            else
            {
                // 2. head�� tail���� �ڿ� �ִ� ���
                memcpy(m_pData + m_nTail, pSrc, sizeof(T) * nSize);
            }
        }

        m_nTail		+= nSize;
        m_nTail		%= m_nSize;
        m_nLength	+= nSize;

        UnlockQueue();

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
    BOOL Dequeue(T* pTar, int nSize)
    {    
        LockQueue();

        // Peek�� ���� ����
        if(m_nLength < nSize)
        {
            UnlockQueue();
            return FALSE;
        }

        // ������ ���� �����Ͱ� NULL�� �ƴ� ��쿡�� �޸𸮸� �����Ѵ�.
        if(pTar != NULL)
        {
            if(m_nHead < m_nTail)
            {
                // 1. head�� tail���� �տ� �ִ� ���
                memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);
            }
            else
            {
                // 2. head�� tail���� �ڿ� �ִ� ���
                int backDataCount = __GetBackDataCount();
                if(backDataCount >= nSize)
                {
                    // ���ʿ� ���� �����Ͱ� ������ �纸�� ���ų� ū ��� nSize��ŭ �ѹ��� �����Ѵ�.
                    memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);                           
                }
                else
                {
                    // ���ʿ� ���� �����Ͱ� ������ �纸�� ���� ��� �ι��� ������ �����Ѵ�.
                    memcpy(pTar, m_pData + m_nHead, sizeof(T) * backDataCount);
                    memcpy(pTar + backDataCount, m_pData, sizeof(T) * (nSize - backDataCount));
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        m_nHead		+= nSize;
        m_nHead		%= m_nSize;
        m_nLength	-= nSize;

        UnlockQueue();

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
    BOOL Peek(T* pTar, int nSize)
    {
        LockQueue();

        // Dequeue�� ���� ����
        if(m_nLength < nSize)
        {
            UnlockQueue();
            return FALSE;
        }

        // ������ ���� �����Ͱ� NULL�� �ƴ� ��쿡�� �޸𸮸� �����Ѵ�.
        if(pTar != NULL)
        {
            if(m_nHead < m_nTail)
            {
                // 1. head�� tail���� �տ� �ִ� ���
                memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);
            }
            else
            {
                // 2. head�� tail���� �ڿ� �ִ� ���
                int backDataCount = __GetBackDataCount();
                if(backDataCount >= nSize)
                {
                    // ���ʿ� ���� �����Ͱ� ������ �纸�� ���ų� ū ��� nSize��ŭ �ѹ��� �����Ѵ�.
                    memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);                           
                }
                else
                {
                    // ���ʿ� ���� �����Ͱ� ������ �纸�� ���� ��� �ι��� ������ �����Ѵ�.
                    memcpy(pTar, m_pData + m_nHead, sizeof(T) * backDataCount);
                    memcpy(pTar + backDataCount, m_pData, sizeof(T) * (nSize - backDataCount));
                }
            }
        }

        UnlockQueue();

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
    inline void CopyHeadDataToExtraBuffer(int nSize)
    {
        assert(nSize <= m_nExtraSize);

        LockQueue();

        // ���� �� ���� ���� ���ۿ��ٰ� ���� �� ���ʿ� �ִ� �����͸� ����.
        memcpy(m_pData + m_nSize, m_pData, nSize);

        UnlockQueue();
    }

    BOOL CopyLocalToSecondBuffer(CircuitQueue<T>* pThreadUnsafeDoubleBuffer, int* pRemainedRatio)
    {
        if(this == pThreadUnsafeDoubleBuffer)
            return FALSE;
        // assert(pThreadUnsafeDoubleBuffer->(m_nSize - m_nLength) >= m_nLength)

        pThreadUnsafeDoubleBuffer->Clear();

        LockQueue();
        *pRemainedRatio = int((m_nSize - m_nLength) * 100 / m_nSize);
        int readableLength = int(sizeof(T) * m_nLength);
        if(readableLength)
        {
            if(m_nHead < m_nTail)
            {
                // 1. head�� tail���� �տ� �ִ� ���
                CopyMemory(pThreadUnsafeDoubleBuffer->m_pData, m_pData + m_nHead, readableLength);
                pThreadUnsafeDoubleBuffer->m_nLength = readableLength;
            }
            else
            {
                // 2. head�� tail���� �ڿ� �ִ� ���
                int backDataCount = __GetBackDataCount();
                CopyMemory(pThreadUnsafeDoubleBuffer->m_pData, m_pData + m_nHead, backDataCount);
                if(readableLength > backDataCount)
                {
                    CopyMemory(pThreadUnsafeDoubleBuffer->m_pData + backDataCount,
                               m_pData,
                               readableLength - backDataCount);
                }
                pThreadUnsafeDoubleBuffer->m_nLength = readableLength;
            }
        }
        UnlockQueue();
        return TRUE;
    }

    // (WAVERIX) (BUG-FIX) ����ġ ���� �� ���� ���� �����ϱ� ���� �۾� + �ڵ� �ߺ� ����
    #undef __GetSpace
    #undef __GetLength
    #undef __GetBackDataCount
    #undef __GetReadPtr
    #undef __GetWritePtr

protected:
    CRITICAL_SECTION m_cs;
    BOOLEAN m_bNeedLock;    // �ش� CircuitQueue�� Lock�� �� �ʿ䰡 �ִ°�?
    T*  m_pData;			/// ť ���� ������
    int m_nLength;			/// ���� ������ ����
    int m_nSize;			/// ť ������ ��ü ũ��
    int m_nHead;			/// ���� �����Ͱ� ���۵Ǵ� ���� ��ġ
    int m_nTail;			/// ���� �����Ͱ� ���� ���� ��ġ
    int m_nExtraSize;		/// ť ���ʿ� �������� ���� ���� ũ�� 
};


#else //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

template<typename T>
class CircuitQueue
{
public:
	CircuitQueue()
        : m_pData( NULL ), m_nLength( 0 ), m_nSize( 0 ), m_nHead( 0 ), m_nTail( 0 )
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

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    BOOL CopyLocalToSecondBuffer(CircuitQueue<T>* pThreadUnsafeDoubleBuffer, int* pRemainedRatio)
    {
        if(this == pThreadUnsafeDoubleBuffer)
            return FALSE;
        // assert(pThreadUnsafeDoubleBuffer->(m_nSize - m_nLength) >= m_nLength)

        pThreadUnsafeDoubleBuffer->Clear();

        EnterCriticalSection(&m_cs);
        *pRemainedRatio = int((m_nSize - m_nLength) * 100 / m_nSize);
        int readableLength = int(sizeof(T) * m_nLength);
        if(readableLength)
        {
            if(m_nHead < m_nTail)
            {
                // 1. head�� tail���� �տ� �ִ� ���
                CopyMemory(pThreadUnsafeDoubleBuffer->m_pData, m_pData + m_nHead, readableLength);
                pThreadUnsafeDoubleBuffer->m_nLength = readableLength;
            }
            else
            {
                // 2. head�� tail���� �ڿ� �ִ� ���
                int backDataCount = m_nSize - m_nHead;
                CopyMemory(pThreadUnsafeDoubleBuffer->m_pData, m_pData + m_nHead, backDataCount);
                if(readableLength > backDataCount)
                {
                    CopyMemory(pThreadUnsafeDoubleBuffer->m_pData + backDataCount,
                               m_pData,
                               readableLength - backDataCount);
                }
                pThreadUnsafeDoubleBuffer->m_nLength = readableLength;
            }
        }
        LeaveCriticalSection(&m_cs);
        return TRUE;
    }
#endif //__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

protected:
	CRITICAL_SECTION	m_cs;
	T					*m_pData;			/// ť ���� ������
	int					m_nLength;			/// ���� ������ ����
	int					m_nSize;			/// ť ������ ��ü ũ��
	int					m_nHead;			/// ���� �����Ͱ� ���۵Ǵ� ���� ��ġ
	int					m_nTail;			/// ���� �����Ͱ� ���� ���� ��ġ
	int					m_nExtraSize;		/// ť ���ʿ� �������� ���� ���� ũ�� 
};

#endif //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

#endif //_CIRCUITQUEUE_H_

