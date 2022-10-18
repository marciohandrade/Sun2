
#include "UtilityCommon.h"
#include "RingBufferEx.h"

namespace util
{


RingBufferEx::RingBufferEx(int bufferSize)
: m_pBuffer(NULL), m_BufferSize(bufferSize), m_Head(0), m_Tail(0)
{
	assert( bufferSize >= 0 );

	if (bufferSize > 0)
	{
		Init( bufferSize );
	}
}

RingBufferEx::~RingBufferEx()
{
	Release();
}

void
RingBufferEx::Init( int bufferSize )
{
	assert( bufferSize > 0 );
	assert( m_pBuffer==NULL );

	m_pBuffer		= new BYTE [bufferSize];
	m_BufferSize	= bufferSize;
	m_Head			= 0;
	m_Tail			= 0;
}

void
RingBufferEx::Release()
{
	SAFE_DELETE_ARRAY( m_pBuffer );
	m_BufferSize	= 0;
	m_Head			= 0;
	m_Tail			= 0;
}

void	
RingBufferEx::Recycle()
{
	m_Head		= 0;
	m_Tail		= 0;
}

// 내부 버퍼에다가 pBuffer를 memcpy
int		
RingBufferEx::Write( const BYTE * pBuffer, int len )
{
	// Tail뒤에다가 pBuffer를 len만큼 추가해야 하는데.. 
	// 일단.. buffer가 부족하면 낭패!
	if ( GetFreeSize() < len )
	{
		// HooooooooooooT
		// oooooooTHooooo
		return -1;
	}

	if (m_Head <= m_Tail)
	{
		// ......HT......
		// ....HooooT....		

		// tail 뒤쪽을 쓰고..
		int rightFreeSize = m_BufferSize - m_Tail;		// 0번째 자리가 비어있으므로 끝까지 채워도 된다.(tail -1 안해줘도 된다.)
		int writeLen = min( rightFreeSize, len );

		memcpy( m_pBuffer+m_Tail, pBuffer, writeLen );
		m_Tail = ( m_Tail + writeLen ) % m_BufferSize;

		// 쓸게 남았으면 앞쪽부터 쓴다.
		int remainLen = len - writeLen;
		if (remainLen > 0)
		{
			assert( m_Tail == 0 );	// 쓸게 남은 경우는 tail은 0이 된다.
			memcpy( m_pBuffer, pBuffer+writeLen, remainLen );
			m_Tail = remainLen;
		}
	}
	else
	{	
		// T............H
		// T........Hoooo
		// ooooT........H
		// ooooT....Hoooo		
		// HoooooooooT...

		// FreeSize는 체크했으므로 tail부터 맘껏 쓰면 된다.
		memcpy( m_pBuffer+m_Tail, pBuffer, len );
		m_Tail += len;
	}	

	return len;
}

// 내부 버퍼에서 pBuffer로 memcpy
int		
RingBufferEx::Read( BYTE * pBuffer, int len )
{
	if ( GetDataSize() < len )
	{
		// len이 크면 아예 실패로 처리한다.
		return -1;
	}

	if (m_Head < m_Tail)
	{
		// HooooooooooooT
		// ......HT......
		// ....HooooT....
		// HoooooooooT...

		// dataSize는 위에서 체크했으므로 충분하다.
		memcpy( pBuffer, m_pBuffer+m_Head, len );
		m_Head += len;
	}
	else
	{
		// oooooooTHooooo	
		// T............H
		// T........Hoooo
		// ooooT........H
		// ooooT....Hoooo
		
		// head부터 끝까지?
		int writeLen = min( len, m_BufferSize-m_Head );
		memcpy( pBuffer, m_pBuffer+m_Head, writeLen );
		m_Head = (m_Head + writeLen) % m_BufferSize;
		
		// 아직 더 써야되면 앞쪽부터 쓴다.
		int remainLen = len - writeLen;
		if (remainLen > 0)
		{
			assert( m_Head == 0 );
			memcpy( pBuffer+writeLen, m_pBuffer, remainLen );
			m_Head += remainLen;
		}
	}	

	return len;
}


// 내부 버퍼에서 pBuffer로 memcpy, 단 내부 head, tail은 변화하지 않는다.
int		
RingBufferEx::Peek( BYTE * pBuffer, int len )
{
	if ( GetDataSize() < len )
	{
		// len이 크면 아예 실패로 처리한다.
		return -1;
	}

	if (m_Head < m_Tail)
	{
		// HooooooooooooT
		// ......HT......
		// ....HooooT....
		// HoooooooooT...

		// dataSize는 위에서 체크했으므로 충분하다.
		memcpy( pBuffer, m_pBuffer+m_Head, len );		
	}
	else
	{
		// oooooooTHooooo	
		// T............H
		// T........Hoooo
		// ooooT........H
		// ooooT....Hoooo		

		// head부터 끝까지?
		int writeLen = min( len, m_BufferSize-m_Head );
		memcpy( pBuffer, m_pBuffer+m_Head, writeLen );
		
		// 아직 더 써야되면 앞쪽부터 쓴다.
		int remainLen = len - writeLen;
		if (remainLen > 0)
		{
			memcpy( pBuffer+writeLen, m_pBuffer, remainLen );			
		}
	}	

	return len;
}

// head, tail의 위치만 이동시킨다.
int	
RingBufferEx::Skip( int len )
{
	if ( GetDataSize() < len )
	{
		// len이 크면 아예 실패로 처리한다.
		return -1;
	}

	m_Head = (m_Head + len) % m_BufferSize;	

	return len;
}

BYTE *	RingBufferEx::GetContigousPtr(OUT int & size) const
{
	if (m_Head <= m_Tail)
	{
		size = m_Tail - m_Head;
		return (m_pBuffer+m_Head);
	}
	else
	{
		size = 	m_BufferSize-m_Head;
		return (m_pBuffer+m_Head);
	}
}

}