#ifndef __VAR_PACKET_H__
#define __VAR_PACKET_H__

#pragma once

#include <windows.h>
#include <assert.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	가변 패킷을 편리하기 사용하기 위한 클래스 그 두번째
//
/**
	@author
			Waverix < waverix@webzen.co.kr >
	@since
			2007. 09. 10
	@remarks
			- Packet Broadcaster 제작을 위해 고안된 VarMsg2 개량형
			- 1..n 용 Broadcast용 패킷 구현이 용이
			- n..1 용 Unicast용 패킷 구현이 용이 (VarMsg, VarPacket 대체용도)
	@example
			- typedef VARMSG_EX<VARMSG_T01, MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>
			MSG_ONEtoN_BROADCASTER;
*/
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
enum eVARMSGEX_T
{
	VARMSG_T00		,
	VARMSG_T01		,	//MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN
	VARMSG_T02		,
	VARMSG_T03		,
	VARMSG_T04		,	//VARMSG_ASSEMBLER0
	VARMSG_T05		,	//VARMSG_ASSEMBLER1
	VARMSG_T06		,	//VARMSG_ASSEMBLER2
	VARMSG_T07		,	//VARMSG_ASSEMBLER3
	VARMSG_T08		,	//VARMSG_ASSEMBLER4
	VARMSG_T09		,
	VARMSG_T10		,
	VARMSG_T11		,
	VARMSG_T12		,
	VARMSG_T13		,
	VARMSG_T14		,
	VARMSG_T15		,
	VARMSG_T16		,
	VARMSG_T17		,
	VARMSG_T18		,
	VARMSG_T19		,
	VARMSG_T20		,
};

template<typename eVARMSGEX_T _VMET, typename _BASETYPE>
struct VARMSG_EX
{
	static const INT		BUFFER_SIZE = (INT) ((WORD)-1);	// 65535

	VARMSG_EX() { Clear(); MSG_STREAM[0] = 0; }

	//	VARMSG_EX의 시작을 알리는 경종소리...
	inline static _BASETYPE&	InstanceOfBase()
	{
		MSG_STREAM_OFFSET = 0;
		//ZeroMemory( MSG_STREAM, sizeof(_BASETYPE) );	<- (WAVERIX)(080829) REMOVE
		return *(new (MSG_STREAM) _BASETYPE);
	}

	inline static VOID			Clear()							{ SetTotalSize(0); }
	inline static VOID			ClearStream( INT iClearSize )
	{
		INT iClearSizeCalced = (iClearSize && iClearSize < BUFFER_SIZE) ? iClearSize : (INT)sizeof(MSG_STREAM) ;
		ZeroMemory( MSG_STREAM, iClearSizeCalced );
	}

	inline static _BASETYPE*	GetFirstPacketPtr()				{ return (_BASETYPE*)MSG_STREAM; }
	inline static VOID			SetTotalSize( INT iTotalSize )	{ MSG_STREAM_OFFSET = iTotalSize; }
	inline static INT			GetTotalSize()					{ return MSG_STREAM_OFFSET; }
	inline static INT			AddOffset( INT iSize )			{ MSG_STREAM_OFFSET += iSize; return MSG_STREAM_OFFSET; }
	inline static INT			ControlOffset( BOOL bIsPlus, SIZE_T szSize )
	{
		INT iOffset = bIsPlus ? (INT)szSize : -((INT)szSize);
		MSG_STREAM_OFFSET += iOffset;
		return MSG_STREAM_OFFSET;
	}

	//	세부 타입에 관련된 처리 루틴
	//		- 가변 패킷으로서 뒤이어 들어올 타입을 선언해 주면, 그것의 기본 생성자를 호출하면서 객체 반환
	//		{ NEXT_TYPE& rNT = VARMSG_EX_T::InstanceOf<NEXT_TYPE>(); }
	template<typename _TYPE>
	inline static _TYPE&		InstanceOf()
	{
		BYTE* pPTR = &MSG_STREAM[MSG_STREAM_OFFSET];
		ZeroMemory( pPTR, sizeof(_TYPE) );
		return *(new (pPTR) _TYPE);
	}

	//	세부 타입에 관련된 처리 루틴
	//		- 가변 패킷으로서 뒤이어 들어올 타입을 선언해 주면, 그것의 객체 반환 (초기화 없음)
	//		{ NEXT_TYPE& rNT = VARMSG_EX_T::InstanceOf<NEXT_TYPE>(); }
	template<typename _TYPE>
	inline static _TYPE&		TypeOf()
	{
		BYTE* pPTR = &MSG_STREAM[MSG_STREAM_OFFSET];
		return *(_TYPE*)pPTR;
	}
	static DWORD	MSG_STREAM_OFFSET;
	static BYTE		MSG_STREAM[BUFFER_SIZE];
};

template<typename eVARMSGEX_T _VMET, typename _BASETYPE>
DWORD	VARMSG_EX<_VMET, _BASETYPE>::MSG_STREAM_OFFSET = 0;

template<typename eVARMSGEX_T _VMET, typename _BASETYPE>
BYTE	VARMSG_EX<_VMET, _BASETYPE>::MSG_STREAM[BUFFER_SIZE];

//=======================================================================================================================
/// 가변 패킷을 편리하게 사용하기 위한 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 30
	@remarks
			- 템플릿으로 만들어서 가변적인 버퍼 사이즈를 줄 수 있게 했다.
			- buffer overrun error처리를 할 수 있다.
	@note
			- 보내는 패킷과 받는 패킷의 처리방법이 다르므로 사용법에 주의한다.
	@Usage
			- 보낼때
				VarPacket<100> varpack;
				varpack.SetShort(2344);
				varpack.SetStream("스트링이거나버퍼다", strlen("스트링이거나버퍼다"));
			- 받을때
				short data1 = 0;
				char data2[128];	// 충분히 할당
				pPacket = VarPacket<>::GetShort( pPacket, data1 );
				pPacket = VarPacket<>::GetStream( pPacket, data2 );
*/
//=======================================================================================================================



template <WORD BUFFER_SIZE = 1>
class VarPacket
{
public:
	
	VarPacket() : m_wSize( 0 ) {}
	~VarPacket() {}
	
	static BYTE * GetChar( BYTE * pPacket, char & cChar )
	{
		cChar = *(char*)pPacket;
		return ( pPacket + 1 );
	}
	
	static BYTE * GetShort( BYTE * pPacket, short & sShort )
	{
		sShort = *(short *)pPacket;
		return ( pPacket + 2 );
	}
	
	static BYTE * GetInteger( BYTE * pPacket, int & iInteger )
	{
		iInteger = *(int *)pPacket;
		return ( pPacket + 4 );
	}
	
	static BYTE * GetBigInteger( BYTE * pPacket, __int64 & i64BigInteger )
	{
		i64BigInteger = *(__int64 *)pPacket;
		return ( pPacket + 8 );
	}

	static BYTE * GetStream( BYTE * pPacket, BYTE * pBuffer )
	{
		short size = 0;
		pPacket = GetShort( pPacket, size );
		memcpy( pBuffer, pPacket, size );

		return pPacket + size;
	}
	
	VOID SetChar( char cChar )
	{
		m_pPacketBuffer[m_wSize] = cChar;
		m_wSize += 1;
		assert( m_wSize < BUFFER_SIZE );
	}
	
	VOID SetShort( short sShort )
	{
		*(short*)&m_pPacketBuffer[m_wSize] = sShort;
		m_wSize += 2;
		assert( m_wSize < BUFFER_SIZE );
	}
	
	VOID SetInteger( int iInteger )
	{
		*(int*)&m_pPacketBuffer[m_wSize] = iInteger;
		m_wSize += 4;
		assert( m_wSize < BUFFER_SIZE );
	}
	
	VOID SetBigInteger( __int64 i64BigInteger )
	{
		*(__int64*)&m_pPacketBuffer[m_wSize] = i64BigInteger;
		m_wSize += 8;
		assert( m_wSize < BUFFER_SIZE );
	}

	VOID SetStream( BYTE * pBuffer, short sSize )
	{
		SetShort( sSize );
		memcpy( &m_pPacketBuffer[m_wSize], pBuffer, sSize );
		m_wSize += sSize;
		assert( m_wSize < BUFFER_SIZE );
	}

	/*
	// 타입별로 만들어 줘야 한다! 
	VOID				operator << ( int data ){}
	VOID				operator >> ( int data ){}
	*/

	BYTE *				GetBufferPointer() { return &m_pPacketBuffer[m_wSize];	}
	WORD				GetSize() { return m_wSize; }
	BYTE *				GetPtr() { return &m_pPacketBuffer[0]; }
	VOID				Reset()	{ m_wSize = 0;	}
private:
	BYTE				m_pPacketBuffer[BUFFER_SIZE];
	WORD				m_wSize;
};

#endif // __VAR_PACKET_H__