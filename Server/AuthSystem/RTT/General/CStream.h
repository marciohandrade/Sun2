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

#ifndef _INCLUDE_C_STREAM_H_
#define _INCLUDE_C_STREAM_H_

#include <crtdbg.h>



//
// 최상위 스트림 인터페이스 
// 
class CStream
{
private:	
protected:
	unsigned long m_size;
	unsigned long m_pos;

	// ChangeCursor() method 만이 setPos() 를 호출하며 모든 유효성 검사는 
	// ChangeCursor() method 내에서 수행한다. 
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


	// 스트림이 사용한 자원을 소멸한다. 
	//	(파일이든, 메모리든...)
	// 
	virtual void ClearStream(void) = 0;

	// 스트림으로 부터 데이터를 읽어서 버퍼에 쓴다.
	// 
	virtual int ReadFromStream(void *Buffer, int Count) = 0;

	// 버퍼로부터 데이터를 읽어 스트림의 현재 포지션에 쓴다.
	//
	virtual int WriteToStream(const void *Buffer, int Count) = 0;	
};


//
// 메모리 스트림 클래스 
// 
// WARNING !
//		이 클래스에서 다른 클래스를 파생하지 말것.
//		복사/대입/소멸자 등등.. 전혀 파생 객체를 고려하지 않았음. 쿠쿠쿠 :-)
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


	// 스트림이 사용한 자원을 소멸한다. 
	//	(파일이든, 메모리든...)
	//	-- inline method 임 - cpp 쪽으로 넘기지 말것
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

	// ByteToRead 만큼 읽을 수 있는지 검사
	//
	//	-- inline method 임 - cpp 쪽으로 넘기지 말것
	//
	bool CanReadFromStream(int ByteToRead)
	{
		if (m_pos + ByteToRead > m_size)
			return false;
		else
			return true;
	}

	// 스트림으로 부터 데이터를 읽어서 버퍼에 쓴다.
	// 
	virtual int ReadFromStream(void *Buffer, int Count);

	// 버퍼로부터 데이터를 읽어 스트림의 현재 포지션에 쓴다.
	//
	virtual int WriteToStream(const void *Buffer, int Count);

	// 메모리 멤버에 직접 엑세스 하기 위한 메소드
	//
	const void *GetMemory() { return m_pMemory; };

};



#endif