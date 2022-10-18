// RingBuffer.h: interface for the RingBuffer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#pragma once

//=======================================================================================================================
/// 링버퍼 구현 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 8. 2
	@remarks
			- 링버퍼에 데이터를 put, get하는 utility library
			- Put( BYTE * pData, DWORD dwSize )	 : 현재 위치의 버퍼(m_pRear)에 데이터를 dwSize만큼 삽입
			- Get( BYTE * pData, DWORD dwSize )	 : 현재 위치의 버퍼(m_pRear)에서 데이터를 dwSize만큼 추출
			- Peek( BYTE * pData, DWORD dwSize ) : 현재 위치의 버퍼(m_pRear)에서 데이터를 dwSize만큼 추출( m_pRear에 +dwSize하지 않음 )
	@todo
			- template으로 유연하게 할 필요 있음(^O^)/
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
	DWORD			m_dwExtraBufferSize;	// Tail 뒤에 붙는 여분의 버퍼사이즈
};

}

#endif // __RINGBUFFER_H__
