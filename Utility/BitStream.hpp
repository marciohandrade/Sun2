#pragma once
#ifndef UTILITY_BITSTREAM_V1_H
#define UTILITY_BITSTREAM_V1_H

#define USING_BITSTREAM_VERSION (2)

#if (USING_BITSTREAM_VERSION == 2)

#include "./BitStream_v2.hpp"

#elif (USING_BITSTREAM_VERSION == 1)

#include "assert.h"
#include <fstream>

#define BITS_TO_BYTES(x) (((x)+7)>>3)
#define BYTES_TO_BITS(x) ((x)<<3)

#define DEBUG_BIT_STREAM
//#define DEBUG_BIT_STREAM_TEST

class BitStream
{
public:
	BitStream( BYTE * p, WORD wNumberOfByte, BOOL TF)
		:m_pBuffer(p), m_BufferMaxSizeByte(wNumberOfByte), m_Offset(0)
	{		
		if(TF)
		{
			memset(m_pBuffer,0, wNumberOfByte);
#ifdef DEBUG_BIT_STREAM_TEST
			FileStream.open( "Temp\\WriteBit.txt", ios::out );
#endif
		}
		else
		{
#ifdef DEBUG_BIT_STREAM_TEST
			FileStream.open( "Temp\\ReadBit.txt", ios::out );
#endif
		}
	}
	~BitStream()
	{
#ifdef DEBUG_BIT_STREAM_TEST
		FileStream.close();
#endif
	}

public:

	void SetBuf( BYTE * p, WORD wNumberOfByte, BOOL TF)
	{
		m_Offset=0;
		m_pBuffer=p;
		m_BufferMaxSizeByte=wNumberOfByte;
		if(TF)
			memset(m_pBuffer,0, wNumberOfByte);
	}

	template<class TYPE>
	__inline void Write( TYPE type, int NumberOfBits )
	{
		if( NumberOfBits <= 0)
			return;

		Write(&type, NumberOfBits);
	}

	template<class TYPE>
		__inline void Read( TYPE & type, int NumberOfBits )
	{
		if( NumberOfBits <= 0)
			return;

		memset(&type, 0, sizeof(TYPE));
		Read(&type, NumberOfBits);
	}

	template<class TYPE>
	__inline void Write( TYPE * pvBuf, int NumberOfBits)
	{
		if( NumberOfBits <= 0 )
			return;
		
		assert( m_Offset + NumberOfBits <= BYTES_TO_BITS(m_BufferMaxSizeByte) );
		if(m_Offset + NumberOfBits > BYTES_TO_BITS(m_BufferMaxSizeByte) )
			return;

#ifdef DEBUG_BIT_STREAM_TEST
		FileStream << endl;
		FileStream << NumberOfBits << endl;
#endif

		const BYTE* pBuffer = reinterpret_cast<const BYTE*>(pvBuf);

		//unit byte
        int i = 0;
		for(; i < (NumberOfBits >> 3); ++i)
		{
			WriteByte(*(pBuffer+i), 8);
#ifdef DEBUG_BIT_STREAM_TEST
			FileStream << (DWORD)(*(pBuffer+i)) << ' ';
#endif
		}
		
		//unit bit
		DWORD WriteBit = NumberOfBits % 8;
		if( WriteBit > 0)
		{
#ifdef DEBUG_BIT_STREAM
			unsigned __int64 i64_Test = 1;
			ASSERT(*(pBuffer+i) < (i64_Test << WriteBit));		
#endif
			WriteByte( (*(pBuffer+i)) << (8-WriteBit), WriteBit);
#ifdef DEBUG_BIT_STREAM_TEST
			FileStream << (DWORD)(*(pBuffer+i)) << ' ';
#endif
		}		
	}
	template<class TYPE>
	__inline void Read( TYPE * pvBuf, int NumberOfBits )
	{
		if( NumberOfBits <= 0)
			return;

//		assert( m_Offset + NumberOfBits <= BYTES_TO_BITS(m_BufferMaxSizeByte) );
		if(m_Offset + NumberOfBits > BYTES_TO_BITS(m_BufferMaxSizeByte) )
			return;

#ifdef DEBUG_BIT_STREAM_TEST
		FileStream << endl;
		FileStream << NumberOfBits << endl;
#endif
		BYTE* pBuffer = reinterpret_cast<BYTE*>(pvBuf);
		//unit byte
        int i = 0;
		for(; i < (NumberOfBits >> 3); ++i)
		{
			memset(pBuffer+i, 0, 1);
			ReadByte( pBuffer+i, 8, 0);
#ifdef DEBUG_BIT_STREAM_TEST
			FileStream << (DWORD)(*(pBuffer+i)) << ' ';
#endif
		}

		//unit bit
		DWORD ReadBit = NumberOfBits % 8;
		if( 0 != ReadBit )
		{
			memset(pBuffer+i, 0, 1);
			ReadByte( pBuffer+i, ReadBit, 0);
			*(pBuffer+i) = (*(pBuffer+i)) >> (8 - ReadBit);

#ifdef DEBUG_BIT_STREAM_TEST
			FileStream << (DWORD)(*(pBuffer+i)) << ' ';
#endif
		}
	}
		
	__inline int GetMaxBufferBytes() const { return m_BufferMaxSizeByte;	}
	__inline int GetReadOffset() const { return m_Offset; }
	__inline int GetNumberOfBitsUsed() const { return m_Offset; }
	__inline int GetNumberOfBytesUsed() const { return BITS_TO_BYTES(m_Offset); }
	__inline void SetReadOffSet( const int offset ) { m_Offset = offset; }
	__inline void SetWriteOffSet( const int offset ) { m_Offset = offset; }

private:
	//WriteByte는 왼쪽비트부터 복사하므로 오른쪽 기준으로 복사시에는 왼쪽으로 쉬프트 해서 호출.
	void WriteByte( BYTE Buffer, int NumberOfBits)
	{
		if(NumberOfBits <= 0)
			return;

		DWORD dwPortion	=	m_Offset >> 3;
		DWORD dwRemain	=	m_Offset % 8;

        m_pBuffer[dwPortion] |= (Buffer) >> dwRemain;
		
		int EmptySpace = (8-dwRemain);
		if(NumberOfBits > EmptySpace)
		{
			m_Offset += EmptySpace;
			WriteByte((Buffer) << EmptySpace, NumberOfBits - EmptySpace);
		}
		else
		{
			m_Offset += NumberOfBits;
		}
	}

	//ReadByte는 왼쪽비트부터 읽으므로 오른쪽 기준으로 복사를 원할시 호출후 오른쪽으로 쉬프트
	void ReadByte( BYTE* pBuffer, int NumberOfBits, DWORD ReadPos )
	{
		if( NumberOfBits <= 0 )
			return;

		DWORD dwPortion	=	m_Offset >> 3;
		DWORD dwRemain	=	m_Offset % 8;

		//memset(pBuffer, 0 , 1);		
				
		int ReadSpace = (8-dwRemain);
		if( NumberOfBits > ReadSpace )
		{
			*pBuffer |= m_pBuffer[dwPortion] << dwRemain;
			m_Offset += ReadSpace;

			ReadByte(pBuffer, NumberOfBits - ReadSpace, ReadPos+ReadSpace);
		}
		else
		{
			//쓰레기값 삭제
			BYTE Temp = m_pBuffer[dwPortion] >> (ReadSpace - NumberOfBits);
			Temp = Temp << (ReadSpace - NumberOfBits);			
			Temp = Temp << dwRemain;		

            *pBuffer |= Temp >> ReadPos;

			m_Offset += NumberOfBits;
		}
	}

private:
#ifdef DEBUG_BIT_STREAM_TEST
	std::ofstream  FileStream;
#endif

	BYTE *	m_pBuffer;
	int		m_BufferMaxSizeByte;
	int		m_Offset;
};

#endif //(USING_BITSTREAM_VERSION == 1)
#endif //UTILITY_BITSTREAM_V1_H