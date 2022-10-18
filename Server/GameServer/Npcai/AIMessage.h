#ifndef _AIMESSAGE_H_
#define _AIMESSAGE_H_

#include <RingBuffer.h>



class AIMessage
{
public:
	AIMessage()
	{
		m_pAIMessageBuffer	= new util::RingBuffer;
		m_pAIMessageBuffer->InitBuffer( RINGBUFFER_SIZE );
	}
	~AIMessage()
	{
		if( m_pAIMessageBuffer )
		{
			m_pAIMessageBuffer->ReleaseBuffer();
			delete m_pAIMessageBuffer;
			m_pAIMessageBuffer = NULL;
		}
	}

	inline VOID SendAIMessage( BYTE *pMsg, DWORD dwSize )
	{
		m_pAIMessageBuffer->Put( pMsg, dwSize );
	}

	inline BOOL RecvAIMessage( BYTE *pMsg, DWORD &dwSize )
	{
		if( !m_pAIMessageBuffer->Get( (BYTE*)&dwSize, sizeof(DWORD) ) )
		{
			return FALSE;
		}

		pMsg = m_pAIMessageBuffer->GetFrontPtr();

		return TRUE;
	}

	inline VOID Completion( DWORD dwSize )
	{
		m_pAIMessageBuffer->Get( NULL, dwSize ) ;
	}

private:
	enum { RINGBUFFER_SIZE = 5000 };
	util::RingBuffer *			m_pAIMessageBuffer;
};





#endif