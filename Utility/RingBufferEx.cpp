
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

// ���� ���ۿ��ٰ� pBuffer�� memcpy
int		
RingBufferEx::Write( const BYTE * pBuffer, int len )
{
	// Tail�ڿ��ٰ� pBuffer�� len��ŭ �߰��ؾ� �ϴµ�.. 
	// �ϴ�.. buffer�� �����ϸ� ����!
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

		// tail ������ ����..
		int rightFreeSize = m_BufferSize - m_Tail;		// 0��° �ڸ��� ��������Ƿ� ������ ä���� �ȴ�.(tail -1 �����൵ �ȴ�.)
		int writeLen = min( rightFreeSize, len );

		memcpy( m_pBuffer+m_Tail, pBuffer, writeLen );
		m_Tail = ( m_Tail + writeLen ) % m_BufferSize;

		// ���� �������� ���ʺ��� ����.
		int remainLen = len - writeLen;
		if (remainLen > 0)
		{
			assert( m_Tail == 0 );	// ���� ���� ���� tail�� 0�� �ȴ�.
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

		// FreeSize�� üũ�����Ƿ� tail���� ���� ���� �ȴ�.
		memcpy( m_pBuffer+m_Tail, pBuffer, len );
		m_Tail += len;
	}	

	return len;
}

// ���� ���ۿ��� pBuffer�� memcpy
int		
RingBufferEx::Read( BYTE * pBuffer, int len )
{
	if ( GetDataSize() < len )
	{
		// len�� ũ�� �ƿ� ���з� ó���Ѵ�.
		return -1;
	}

	if (m_Head < m_Tail)
	{
		// HooooooooooooT
		// ......HT......
		// ....HooooT....
		// HoooooooooT...

		// dataSize�� ������ üũ�����Ƿ� ����ϴ�.
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
		
		// head���� ������?
		int writeLen = min( len, m_BufferSize-m_Head );
		memcpy( pBuffer, m_pBuffer+m_Head, writeLen );
		m_Head = (m_Head + writeLen) % m_BufferSize;
		
		// ���� �� ��ߵǸ� ���ʺ��� ����.
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


// ���� ���ۿ��� pBuffer�� memcpy, �� ���� head, tail�� ��ȭ���� �ʴ´�.
int		
RingBufferEx::Peek( BYTE * pBuffer, int len )
{
	if ( GetDataSize() < len )
	{
		// len�� ũ�� �ƿ� ���з� ó���Ѵ�.
		return -1;
	}

	if (m_Head < m_Tail)
	{
		// HooooooooooooT
		// ......HT......
		// ....HooooT....
		// HoooooooooT...

		// dataSize�� ������ üũ�����Ƿ� ����ϴ�.
		memcpy( pBuffer, m_pBuffer+m_Head, len );		
	}
	else
	{
		// oooooooTHooooo	
		// T............H
		// T........Hoooo
		// ooooT........H
		// ooooT....Hoooo		

		// head���� ������?
		int writeLen = min( len, m_BufferSize-m_Head );
		memcpy( pBuffer, m_pBuffer+m_Head, writeLen );
		
		// ���� �� ��ߵǸ� ���ʺ��� ����.
		int remainLen = len - writeLen;
		if (remainLen > 0)
		{
			memcpy( pBuffer+writeLen, m_pBuffer, remainLen );			
		}
	}	

	return len;
}

// head, tail�� ��ġ�� �̵���Ų��.
int	
RingBufferEx::Skip( int len )
{
	if ( GetDataSize() < len )
	{
		// len�� ũ�� �ƿ� ���з� ó���Ѵ�.
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