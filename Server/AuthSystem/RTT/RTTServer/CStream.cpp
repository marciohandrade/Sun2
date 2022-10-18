/*-----------------------------------------------------------------------------
 * CStream
 *-----------------------------------------------------------------------------
 * ��Ʈ�� Ŭ����
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

	// position �� ��Ʈ���� ũ�⺸�� ũ�� ���������� Ŀ���� �ű��. 
	//
	if (newPosition > m_size)
	{
		newPosition = m_size;		
	}

	setPos(newPosition);		
	return newPosition;
};


/**	-----------------------------------------------------------------------
	\brief	memory stream �� ũ�⸦ �����Ѵ�.

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
			// ��Ʈ�� clear ���
			//
			m_pMemory = NULL;			
		}
		else
		{
			// �޸𸮰� ������.
			// m_pMemory �� ������� ����. 
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
	\brief	��Ʈ������ ���� �����͸� �о ���ۿ� ����.

	\param	
	\return			
			������ ���� ����Ʈ �� ���� 
			(��Ʈ���� Count ���� ���� ��� ����)
			���н� -1 ����
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
	\brief	���۷κ��� �����͸� �о� ��Ʈ���� ���� �����ǿ� ����.

	\param	
	\return			
			������ Write �� ����Ʈ ��
			���н� -1
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

	return 0;	// write �� ����Ʈ�� 0 �̹Ƿ�
}





