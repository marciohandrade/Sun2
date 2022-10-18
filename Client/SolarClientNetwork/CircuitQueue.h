#ifndef _CIRCUITQUEUE_H_
#define _CIRCUITQUEUE_H_

#include <windows.h>
#include <stdio.h>
#include <assert.h>

//=============================================================================================================================
/// 원형 큐 클래스 템플릿
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
       큐를 생성한다.
        @param	nSize
       큐 사이즈
        @param	nExtraSize
       패킷이 버퍼 뒤쪽에서 앞쪽으로 나누어 들어갈 때 패킷을 이어주기 위해 확보할
       버퍼 뒤쪽 여분의 메모리 크기(실제 여분 메모리 = sizeof(T) * nExtraSize)
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
       큐를 청소한다.
    */
    //=============================================================================================================================
    inline void Clear()
    {
        LockQueue();
        m_nLength = m_nHead = m_nTail = 0;
        UnlockQueue();
    }

    // (WAVERIX) (BUG-FIX) 예기치 않은 락 해제 상태 방지하기 위한 작업 + 코드 중복 감소
    #define __GetSpace()            (m_nSize - m_nLength)
    #define __GetLength()           (m_nLength)
    #define __GetBackDataCount()    (m_nSize - m_nHead)
    #define __GetReadPtr()          (m_pData + m_nHead)
    #define __GetWritePtr()         (m_pData + m_nTail)

    /// 큐의 남은 공간을 리턴한다.
    inline int GetSpace()
    {
        int iRet;

        LockQueue();
        iRet = __GetSpace();		
        UnlockQueue();

        return iRet;
    }

    /// 현재 사용중인 큐의 크기를 리턴한다.
    inline int GetLength()
    {
        int iRet;

        LockQueue();
        iRet = __GetLength();
        UnlockQueue();

        return iRet;
    }

    /// 데이터가 큐 뒤쪽으로 꽉 차서 앞으로 이어지는 경우, 뒤쪽 끝까지의 데이터 개수를 리턴한다.
    inline int GetBackDataCount()
    {
        int iRet;

        LockQueue();
        iRet = __GetBackDataCount();
        UnlockQueue();

        return iRet;
    }

    /// 데이터를 읽을 포인터(Head)를 리턴한다.
    inline T* GetReadPtr()
    {
        T *pRet;

        LockQueue();
        pRet = __GetReadPtr();
        UnlockQueue();

        return pRet;
    }

    /// 데이터를 쓸 포인터(Tail)를 리턴한다.
    inline T* GetWritePtr()
    {
        T *pRet;

        LockQueue();
        pRet = __GetWritePtr();
        UnlockQueue();

        return pRet;
    }

    /// 잘리지 않고 한번에 읽을 수 있는 길이를 리턴한다.
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

    /// 잘리지 않고 한번에 쓰기 가능한 길이를 리턴한다.
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
       큐에 데이터를 넣는다.
        @param	pSrc
       큐에 삽입할 데이터 포인터
        @param	nSize
       큐에 삽입할 데이터의 길이
        @retval	BOOL
       정상적으로 데이터 삽입이 끝나면 TRUE를 리턴한다.
       큐가 꽉차서 삽입할 수 없는 경우 FALSE를 리턴한다.
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

        // pData가 NULL인 경우에는 데이터 큐잉 없이 tail과 length만 갱신한다.
        if(pSrc != NULL)
        {
            if(m_nHead <= m_nTail)
            {
                // 1. head가 tail보다 앞에 있는 경우
                int nBackSpaceCount = m_nSize - m_nTail;

                if(nBackSpaceCount >= nSize)  
                {
                    // 뒤쪽에 남은 공간이 복사할 양보다 같거나 큰 경우 한번에 복사한다.
                    memcpy(m_pData + m_nTail, pSrc, sizeof(T) * nSize);
                }
                else
                {
                    // 뒤쪽에 남은 공간이 복사할 양보다 작은 경우 두번에 나누어 복사한다.
                    memcpy(m_pData + m_nTail, pSrc, sizeof(T) * nBackSpaceCount);
                    memcpy(m_pData, pSrc + nBackSpaceCount, sizeof(T) * (nSize - nBackSpaceCount));
                }
            }
            else
            {
                // 2. head가 tail보다 뒤에 있는 경우
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
       큐에서 데이터를 꺼낸다.
        @param	pTar
       큐에서 꺼낸 데이터를 복사할 버퍼의 포인터
        @param	nSize
       큐에서 꺼낼 데이터 길이
        @retval	BOOL
       정상적으로 데이터를 꺼내면 TRUE를 리턴한다.
       꺼낼 데이터가 nSize보다 적은 경우 FALSE를 리턴한다.
    */
    //=============================================================================================================================
    BOOL Dequeue(T* pTar, int nSize)
    {    
        LockQueue();

        // Peek과 로직 동일
        if(m_nLength < nSize)
        {
            UnlockQueue();
            return FALSE;
        }

        // 내용을 받을 포인터가 NULL이 아닌 경우에만 메모리를 복사한다.
        if(pTar != NULL)
        {
            if(m_nHead < m_nTail)
            {
                // 1. head가 tail보다 앞에 있는 경우
                memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);
            }
            else
            {
                // 2. head가 tail보다 뒤에 있는 경우
                int backDataCount = __GetBackDataCount();
                if(backDataCount >= nSize)
                {
                    // 뒤쪽에 남은 데이터가 꺼내갈 양보다 같거나 큰 경우 nSize만큼 한번에 복사한다.
                    memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);                           
                }
                else
                {
                    // 뒤쪽에 남은 데이터가 꺼내갈 양보다 작은 경우 두번에 나누어 복사한다.
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
       큐에서 데이터를 꺼내지 않고 복사만 해온다.
        @param	pTar
       큐의 데이터를 복사할 버퍼의 포인터
        @param	nSize
       큐에서 복사할 데이터 길이
        @retval	BOOL
       정상적으로 데이터를 복사하면 TRUE를 리턴한다.
       꺼낼 데이터가 nSize보다 적은 경우 FALSE를 리턴한다.
    */
    //=============================================================================================================================
    BOOL Peek(T* pTar, int nSize)
    {
        LockQueue();

        // Dequeue와 로직 동일
        if(m_nLength < nSize)
        {
            UnlockQueue();
            return FALSE;
        }

        // 내용을 받을 포인터가 NULL이 아닌 경우에만 메모리를 복사한다.
        if(pTar != NULL)
        {
            if(m_nHead < m_nTail)
            {
                // 1. head가 tail보다 앞에 있는 경우
                memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);
            }
            else
            {
                // 2. head가 tail보다 뒤에 있는 경우
                int backDataCount = __GetBackDataCount();
                if(backDataCount >= nSize)
                {
                    // 뒤쪽에 남은 데이터가 꺼내갈 양보다 같거나 큰 경우 nSize만큼 한번에 복사한다.
                    memcpy(pTar, m_pData + m_nHead, sizeof(T) * nSize);                           
                }
                else
                {
                    // 뒤쪽에 남은 데이터가 꺼내갈 양보다 작은 경우 두번에 나누어 복사한다.
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
       패킷이 버퍼 뒤쪽에서 앞쪽으로 나누어질 때 패킷이 이어지도록 여분의 공간에 카피한다.
        @param	nSize
       뒤쪽에서 잘려서 앞쪽으로 이어진 데이터의 길이
    */
    //=============================================================================================================================
    inline void CopyHeadDataToExtraBuffer(int nSize)
    {
        assert(nSize <= m_nExtraSize);

        LockQueue();

        // 버퍼 맨 뒷쪽 여분 버퍼에다가 버퍼 맨 앞쪽에 있는 데이터를 쓴다.
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
                // 1. head가 tail보다 앞에 있는 경우
                CopyMemory(pThreadUnsafeDoubleBuffer->m_pData, m_pData + m_nHead, readableLength);
                pThreadUnsafeDoubleBuffer->m_nLength = readableLength;
            }
            else
            {
                // 2. head가 tail보다 뒤에 있는 경우
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

    // (WAVERIX) (BUG-FIX) 예기치 않은 락 해제 상태 방지하기 위한 작업 + 코드 중복 감소
    #undef __GetSpace
    #undef __GetLength
    #undef __GetBackDataCount
    #undef __GetReadPtr
    #undef __GetWritePtr

protected:
    CRITICAL_SECTION m_cs;
    BOOLEAN m_bNeedLock;    // 해당 CircuitQueue에 Lock을 걸 필요가 있는가?
    T*  m_pData;			/// 큐 버퍼 포인터
    int m_nLength;			/// 현재 데이터 길이
    int m_nSize;			/// 큐 버퍼의 전체 크기
    int m_nHead;			/// 현재 데이터가 시작되는 곳의 위치
    int m_nTail;			/// 현재 데이터가 써질 곳의 위치
    int m_nExtraSize;		/// 큐 뒤쪽에 여분으로 잡을 버퍼 크기 
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
			큐를 생성한다.
	@param	nSize
			큐 사이즈
	@param	nExtraSize
			패킷이 버퍼 뒤쪽에서 앞쪽으로 나누어 들어갈 때 패킷을 이어주기 위해 확보할
			버퍼 뒤쪽 여분의 메모리 크기(실제 여분 메모리 = sizeof(T) * nExtraSize )
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
			큐를 청소한다.
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
	
	/// 큐의 남은 공간을 리턴한다.
	inline int      GetSpace()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		iRet = m_nSize - m_nLength;		
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

	/// 현재 사용중인 큐의 크기를 리턴한다.
	inline int      GetLength()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		iRet = m_nLength;
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

	/// 데이터가 큐 뒤쪽으로 꽉 차서 앞으로 이어지는 경우, 뒤쪽 끝까지의 데이터 개수를 리턴한다.
	inline int      GetBackDataCount()
	{
		int iRet;

		EnterCriticalSection( &m_cs );
		iRet = m_nSize - m_nHead;
		LeaveCriticalSection( &m_cs );

		return iRet;
	}

	/// 데이터를 읽을 포인터(Head)를 리턴한다.
	inline T*       GetReadPtr()
	{
		T *pRet;

		EnterCriticalSection( &m_cs );
		pRet = m_pData + m_nHead;
		LeaveCriticalSection( &m_cs );

		return pRet;
	}

	/// 데이터를 쓸 포인터(Tail)를 리턴한다.
	inline T*       GetWritePtr()
	{
		T *pRet;

		EnterCriticalSection( &m_cs );
		pRet = m_pData + m_nTail;
		LeaveCriticalSection( &m_cs );

		return pRet;
	}
	
	/// 잘리지 않고 한번에 읽을 수 있는 길이를 리턴한다.
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
	
	/// 잘리지 않고 한번에 쓰기 가능한 길이를 리턴한다.
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
			큐에 데이터를 넣는다.
	@param	pSrc
			큐에 삽입할 데이터 포인터
	@param	nSize
			큐에 삽입할 데이터의 길이
	@retval	BOOL
			정상적으로 데이터 삽입이 끝나면 TRUE를 리턴한다.
			큐가 꽉차서 삽입할 수 없는 경우 FALSE를 리턴한다.
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

		// pData가 NULL인 경우에는 데이터 큐잉 없이 tail과 length만 갱신한다.
		if( pSrc != NULL )
		{
			if( m_nHead <= m_nTail )
			{
				// 1. head가 tail보다 앞에 있는 경우
				int nBackSpaceCount = m_nSize - m_nTail;

				if( nBackSpaceCount >= nSize )  
				{
					// 뒤쪽에 남은 공간이 복사할 양보다 같거나 큰 경우 한번에 복사한다.
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nSize );
				}
				else
				{
					// 뒤쪽에 남은 공간이 복사할 양보다 작은 경우 두번에 나누어 복사한다.
					memcpy( m_pData + m_nTail, pSrc, sizeof(T) * nBackSpaceCount );
					memcpy( m_pData, pSrc + nBackSpaceCount, sizeof(T) * ( nSize - nBackSpaceCount ) );
				}
			}
			else
			{
				// 2. head가 tail보다 뒤에 있는 경우
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
			큐에서 데이터를 꺼낸다.
	@param	pTar
			큐에서 꺼낸 데이터를 복사할 버퍼의 포인터
	@param	nSize
			큐에서 꺼낼 데이터 길이
	@retval	BOOL
			정상적으로 데이터를 꺼내면 TRUE를 리턴한다.
			꺼낼 데이터가 nSize보다 적은 경우 FALSE를 리턴한다.
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
			큐에서 데이터를 꺼내지 않고 복사만 해온다.
	@param	pTar
			큐의 데이터를 복사할 버퍼의 포인터
	@param	nSize
			큐에서 복사할 데이터 길이
	@retval	BOOL
			정상적으로 데이터를 복사하면 TRUE를 리턴한다.
			꺼낼 데이터가 nSize보다 적은 경우 FALSE를 리턴한다.
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

		// 내용을 받을 포인터가 NULL이 아닌 경우에만 메모리를 복사한다.
		if( pTar != NULL )
		{
			if( m_nHead < m_nTail )
			{
				// 1. head가 tail보다 앞에 있는 경우
				memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );
			}
			else
			{
				// 2. head가 tail보다 뒤에 있는 경우
				if( GetBackDataCount() >= nSize )
				{
					// 뒤쪽에 남은 데이터가 꺼내갈 양보다 같거나 큰 경우 nSize만큼 한번에 복사한다.
					memcpy( pTar, m_pData + m_nHead, sizeof(T) * nSize );                           
				}
				else
				{
					// 뒤쪽에 남은 데이터가 꺼내갈 양보다 작은 경우 두번에 나누어 복사한다.
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
			패킷이 버퍼 뒤쪽에서 앞쪽으로 나누어질 때 패킷이 이어지도록 여분의 공간에 카피한다.
	@param	nSize
			뒤쪽에서 잘려서 앞쪽으로 이어진 데이터의 길이
*/
//=============================================================================================================================
	inline void CopyHeadDataToExtraBuffer( int nSize )
	{
		assert( nSize <= m_nExtraSize );

		EnterCriticalSection( &m_cs );

		// 버퍼 맨 뒷쪽 여분 버퍼에다가 버퍼 맨 앞쪽에 있는 데이터를 쓴다.
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
                // 1. head가 tail보다 앞에 있는 경우
                CopyMemory(pThreadUnsafeDoubleBuffer->m_pData, m_pData + m_nHead, readableLength);
                pThreadUnsafeDoubleBuffer->m_nLength = readableLength;
            }
            else
            {
                // 2. head가 tail보다 뒤에 있는 경우
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
	T					*m_pData;			/// 큐 버퍼 포인터
	int					m_nLength;			/// 현재 데이터 길이
	int					m_nSize;			/// 큐 버퍼의 전체 크기
	int					m_nHead;			/// 현재 데이터가 시작되는 곳의 위치
	int					m_nTail;			/// 현재 데이터가 써질 곳의 위치
	int					m_nExtraSize;		/// 큐 뒤쪽에 여분으로 잡을 버퍼 크기 
};

#endif //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

#endif //_CIRCUITQUEUE_H_

