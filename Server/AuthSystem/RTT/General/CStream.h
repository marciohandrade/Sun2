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

#ifndef _INCLUDE_C_STREAM_H_
#define _INCLUDE_C_STREAM_H_

#include <crtdbg.h>



//
// �ֻ��� ��Ʈ�� �������̽� 
// 
class CStream
{
private:	
protected:
	unsigned long m_size;
	unsigned long m_pos;

	// ChangeCursor() method ���� setPos() �� ȣ���ϸ� ��� ��ȿ�� �˻�� 
	// ChangeCursor() method ������ �����Ѵ�. 
	//
	unsigned long setPos(const unsigned long newPosition)
	{
		m_pos = newPosition;
		return m_pos;
	};


	virtual int SetSize(unsigned long newSize) = 0;
public:
	CStream()
		:	m_pos(0),
			m_size(0)
	{
	};
	virtual ~CStream(){};


	int GetSize() { return m_size; };
	int GetCurrentCusor() { return m_pos; };
	int ChangeCursor(const unsigned long offset, unsigned long from);


	// ��Ʈ���� ����� �ڿ��� �Ҹ��Ѵ�. 
	//	(�����̵�, �޸𸮵�...)
	// 
	virtual void ClearStream(void) = 0;

	// ��Ʈ������ ���� �����͸� �о ���ۿ� ����.
	// 
	virtual int ReadFromStream(void *Buffer, int Count) = 0;

	// ���۷κ��� �����͸� �о� ��Ʈ���� ���� �����ǿ� ����.
	//
	virtual int WriteToStream(const void *Buffer, int Count) = 0;	
};


//
// �޸� ��Ʈ�� Ŭ���� 
// 
// WARNING !
//		�� Ŭ�������� �ٸ� Ŭ������ �Ļ����� ����.
//		����/����/�Ҹ��� ���.. ���� �Ļ� ��ü�� ������� �ʾ���. ������ :-)
// 
class CMemoryStream : public CStream
{
private:
	char *m_pMemory;
	virtual int SetSize(unsigned long newSize);	
protected:
public:
	CMemoryStream()
		:	m_pMemory(NULL)
	{};
	~CMemoryStream()			
	{
		if (NULL != m_pMemory)
		{
			// free and change cursor			
			//
			if (0 != SetSize(0))
			{
				_ASSERTE(!"SetSize() error");				
			}
		}
	};


	// ��Ʈ���� ����� �ڿ��� �Ҹ��Ѵ�. 
	//	(�����̵�, �޸𸮵�...)
	//	-- inline method �� - cpp ������ �ѱ��� ����
	//
	virtual void ClearStream(void)
	{
		// free and change cursor			
		//
		if (0 != SetSize(0))
		{
			_ASSERTE(!"SetSize() error");			
		}		
	}

	// ByteToRead ��ŭ ���� �� �ִ��� �˻�
	//
	//	-- inline method �� - cpp ������ �ѱ��� ����
	//
	bool CanReadFromStream(int ByteToRead)
	{
		if (m_pos + ByteToRead > m_size)
			return false;
		else
			return true;
	}

	// ��Ʈ������ ���� �����͸� �о ���ۿ� ����.
	// 
	virtual int ReadFromStream(void *Buffer, int Count);

	// ���۷κ��� �����͸� �о� ��Ʈ���� ���� �����ǿ� ����.
	//
	virtual int WriteToStream(const void *Buffer, int Count);

	// �޸� ����� ���� ������ �ϱ� ���� �޼ҵ�
	//
	const void *GetMemory() { return m_pMemory; };

};



#endif