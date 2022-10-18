/*-----------------------------------------------------------------------------
 * CStream
 *-----------------------------------------------------------------------------
 * 스트림 클래스
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2006, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2006, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CStream.h"

#include <windows.h>
#include <crtdbg.h>

/**	-----------------------------------------------------------------------
	\brief	

	\param	
	\return	
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
#define MAX_UNSIGNED_LONG	0xFFFFFFFF	
int CStream::ChangeCursor(const unsigned long offset, unsigned long from)
{
	_ASSERTE(0 <= offset);
	_ASSERTE(0 <= from);
	_ASSERTE(m_pos >= from);
	_ASSERTE(MAX_UNSIGNED_LONG >= from); 
	_ASSERTE(MAX_UNSIGNED_LONG >= offset); 
	_ASSERTE(MAX_UNSIGNED_LONG >= from + offset);

	if (	
			!(0 <= offset) ||
			!(0 <= from) ||
			!(m_pos >= from) ||
			!(MAX_UNSIGNED_LONG >= from) ||
			!(MAX_UNSIGNED_LONG >= offset) || 
			!(MAX_UNSIGNED_LONG >= from + offset)
		)
	{
		OutputDebugString("invalid parameter");
		return -1;
	}

	unsigned long newPosition = from + offset;		

	// position 이 스트림의 크기보다 크면 마지막으로 커서를 옮긴다. 
	//
	if (newPosition > m_size)
	{
		newPosition = m_size;		
	}

	setPos(newPosition);		
	return newPosition;
};


/**	-----------------------------------------------------------------------
	\brief	memory stream 의 크기를 변경한다.

	\param	
	\return	
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int CMemoryStream::SetSize(unsigned long newSize) 
{
	unsigned long oldPosition = GetCurrentCusor();

	char *ptr = (char *) realloc(m_pMemory, newSize);
	if (NULL == ptr)
	{
		if(0 == newSize)
		{
			// 스트림 clear 경우
			//
			m_pMemory = NULL;			
		}
		else
		{
			// 메모리가 부족함.
			// m_pMemory 는 변경되지 않음. 
			// 
			OutputDebugString(TEXT("not enough memory available"));
			return -1;
		}		
	}

	m_pMemory = ptr;
	m_size = newSize;
	if (oldPosition > newSize) ChangeCursor(0, newSize);
	
	return newSize;
}



 
/**	-----------------------------------------------------------------------
	\brief	스트림으로 부터 데이터를 읽어서 버퍼에 쓴다.

	\param	
	\return			
			성공시 읽은 바이트 수 리턴 
			(스트림이 Count 보다 작은 경우 포함)
			실패시 -1 리턴
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int CMemoryStream::ReadFromStream(void *Buffer, int Count)
{
	_ASSERTE( TRUE != IsBadWritePtr(Buffer, Count) );
	if ( TRUE == IsBadWritePtr(Buffer, Count) )
	{
		OutputDebugString(("%s >> Invalid Parameter", __FUNCTION__));
		return -1;
	}


	if ( (m_pos >= 0) && (Count >= 0) )	
	{
		int ret = m_size - m_pos;
		if (0 < ret)
		{
			if (ret > Count) ret = Count;

			memmove(Buffer, (char *)(DWORD_PTR(m_pMemory) + m_pos), ret);
			ChangeCursor(ret, m_pos);
			return ret;
		}
	}

	return 0;
}

/**	-----------------------------------------------------------------------
	\brief	버퍼로부터 데이터를 읽어 스트림의 현재 포지션에 쓴다.

	\param	
	\return			
			성공시 Write 한 바이트 수
			실패시 -1
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int CMemoryStream::WriteToStream(const void *Buffer, int Count)
{
	if ( (m_pos >= 0) && (Count >= 0) )
	{
		unsigned long pos = m_pos + Count;
		if (pos > 0)
		{
			if (pos > m_size)
			{
				if (-1 == SetSize(pos))
				{
					OutputDebugString("not enough memory");
					return -1;
				}
			}

			memmove(&m_pMemory[m_pos], Buffer, Count);
			this->m_pos = pos;
			return Count;
		}	  
	}

	return 0;	// write 한 바이트가 0 이므로
}





