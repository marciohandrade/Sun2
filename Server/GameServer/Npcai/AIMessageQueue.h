#ifndef _AIMESSAGEQUEUE_H_
#define _AIMESSAGEQUEUE_H_

#include <RingBuffer.h>


struct AI_MSG;

class AIMessageQueue
{
public:

	AIMessageQueue(DWORD dwSize)
	{
		m_pAIMessageBuffer	= new util::RingBuffer;
		m_pAIMessageBuffer->InitBuffer( dwSize );
	}

	AIMessageQueue()
	{
		m_pAIMessageBuffer	= new util::RingBuffer;
		m_pAIMessageBuffer->InitBuffer( RINGBUFFER_SIZE );
	}

	~AIMessageQueue()
	{
		if( m_pAIMessageBuffer )
		{
			m_pAIMessageBuffer->ReleaseBuffer();
		}

		SAFE_DELETE( m_pAIMessageBuffer );			
	}

	inline VOID Clear()
	{
		m_pAIMessageBuffer->ResetBuffer();
	}

	inline DWORD GetOccupiedSize()
	{
		return m_pAIMessageBuffer->GetOccupiedSize();
	}

	inline VOID SendAIMessage( AI_MSG *pMsg, WORD wSize, DWORD dwDelay = 0 )
	{
		pMsg->wSize = wSize;
		pMsg->dwDeliveryTick = GetTickCount() + dwDelay;
		m_pAIMessageBuffer->Put( (BYTE*)pMsg, wSize );
	}

	// dwSize에 사이즈를 얻어내고 데이터 시작 포인터를 리턴한다.
	inline BYTE* RecvAIMessage( WORD &wSize )
	{
		if( !m_pAIMessageBuffer->Peek( (BYTE*)&wSize, sizeof(WORD) ) )
		{
			return NULL;
		}

		return m_pAIMessageBuffer->GetFrontPtr();
	}

	inline VOID Completion( WORD wSize )
	{
		m_pAIMessageBuffer->Get( NULL, wSize ) ;
	}

private:
	enum { RINGBUFFER_SIZE = 500 };
	util::RingBuffer *			m_pAIMessageBuffer;
};





#endif