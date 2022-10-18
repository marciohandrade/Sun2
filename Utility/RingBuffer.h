// RingBuffer.h: interface for the RingBuffer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#pragma once

//=======================================================================================================================
/// ������ ���� Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 8. 2
	@remarks
			- �����ۿ� �����͸� put, get�ϴ� utility library
			- Put( BYTE * pData, DWORD dwSize )	 : ���� ��ġ�� ����(m_pRear)�� �����͸� dwSize��ŭ ����
			- Get( BYTE * pData, DWORD dwSize )	 : ���� ��ġ�� ����(m_pRear)���� �����͸� dwSize��ŭ ����
			- Peek( BYTE * pData, DWORD dwSize ) : ���� ��ġ�� ����(m_pRear)���� �����͸� dwSize��ŭ ����( m_pRear�� +dwSize���� ���� )
	@todo
			- template���� �����ϰ� �� �ʿ� ����(^O^)/
*/
//=======================================================================================================================

namespace util
{


class RingBuffer  
{
public:
	RingBuffer();
	virtual ~RingBuffer();

	VOID			InitBuffer( DWORD dwBufferSize );
	VOID			ReleaseBuffer();
	VOID			ResetBuffer();
 
	BOOL			Put( BYTE * pData, DWORD dwSize );
	BOOL			Get( BYTE * pData, DWORD dwSize );
	BOOL			Peek( BYTE * pData, DWORD dwSize );

	inline DWORD	GetAvailableSize() { return m_dwAvailableSize;	}
	inline DWORD	GetOccupiedSize() { return m_dwMaxBufferSize - m_dwAvailableSize;	}

	inline BYTE *	GetFrontPtr()  { return m_pFront;	}

private:
	BYTE			* m_pBuffer;

	BYTE			* m_pHead;
	BYTE			* m_pTail;

	BYTE			* m_pFront;
	BYTE			* m_pRear;

	DWORD			m_dwMaxBufferSize;
	DWORD			m_dwAvailableSize;
	DWORD			m_dwExtraBufferSize;	// Tail �ڿ� �ٴ� ������ ���ۻ�����
};

}

#endif // __RINGBUFFER_H__
