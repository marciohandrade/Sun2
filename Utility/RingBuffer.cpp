// RingBuffer.cpp: implementation of the RingBuffer class.
//
//////////////////////////////////////////////////////////////////////
#include "UtilityCommon.h"
#include <Windows.h>
#include "RingBuffer.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace util
{


RingBuffer::RingBuffer()
{
	m_pBuffer = NULL;
}

RingBuffer::~RingBuffer()
{
	ReleaseBuffer();
}

VOID RingBuffer::ReleaseBuffer()
{
	if( m_pBuffer )
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}

VOID RingBuffer::InitBuffer( DWORD dwBufferSize )
{
	m_dwMaxBufferSize = dwBufferSize;
	m_dwExtraBufferSize = m_dwMaxBufferSize/4;
	m_pBuffer = new BYTE [ m_dwMaxBufferSize + m_dwExtraBufferSize ];

#ifdef __WAVERIX_PERFOMANCE_TUNEUP__
	memset( m_pBuffer, 0, m_dwMaxBufferSize + m_dwExtraBufferSize );
#endif

	ResetBuffer();
}

VOID RingBuffer::ResetBuffer()
{
	m_dwAvailableSize = m_dwMaxBufferSize;

	m_pFront = m_pRear = &m_pBuffer[0];
	m_pHead = &m_pBuffer[ 0 ];
	m_pTail = &m_pBuffer[ m_dwMaxBufferSize ];
}

BOOL RingBuffer::Put( BYTE * pData, DWORD dwSize )
{
	assert( dwSize > 0 );
	//<= -> <로 수정
	if( GetAvailableSize() < dwSize ) return FALSE;
	if( (m_pRear+dwSize) > (m_pTail+m_dwExtraBufferSize) )	
		return FALSE;

	if( pData ) memcpy( m_pRear, pData, dwSize );
	m_pRear += dwSize;

	// 정방향
	if( m_pFront <= m_pRear )
	{
		// m_dwMaxBufferSize 초과 부분을 사용했으므로 다음에는 m_pHead 부터 시작한다.
		if( m_pRear > m_pTail )
		{
			m_pRear = m_pHead;
		}
	}

	m_dwAvailableSize -= dwSize;

	return TRUE;
}

BOOL RingBuffer::Get( BYTE * pData, DWORD dwSize )
{
	if( GetOccupiedSize() < dwSize ) return FALSE;

	if( pData )	memcpy( pData, m_pFront, dwSize );

	m_pFront += dwSize;

	// m_dwMaxBufferSize 초과 부분을 사용했으므로 다음에는 m_pHead 부터 시작한다.
	if( m_pFront > m_pTail )
	{
		m_pFront = m_pHead;
	}

	m_dwAvailableSize += dwSize;

	return TRUE;
}

BOOL RingBuffer::Peek( BYTE * pData, DWORD dwSize )
{
 	if( GetOccupiedSize() < dwSize ) 
		return FALSE;  

	if( pData ) memcpy( pData, m_pFront, dwSize );

	return TRUE;
}

}














