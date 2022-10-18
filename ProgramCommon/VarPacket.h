#ifndef __VAR_PACKET_H__
#define __VAR_PACKET_H__

#pragma once

#include <windows.h>
#include <assert.h>

//������������������������������������������������������������������������������������������������
//	���� ��Ŷ�� ���ϱ� ����ϱ� ���� Ŭ���� �� �ι�°
//
/**
	@author
			Waverix < waverix@webzen.co.kr >
	@since
			2007. 09. 10
	@remarks
			- Packet Broadcaster ������ ���� ��ȵ� VarMsg2 ������
			- 1..n �� Broadcast�� ��Ŷ ������ ����
			- n..1 �� Unicast�� ��Ŷ ������ ���� (VarMsg, VarPacket ��ü�뵵)
	@example
			- typedef VARMSG_EX<VARMSG_T01, MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN>
			MSG_ONEtoN_BROADCASTER;
*/
//������������������������������������������������������������������������������������������������
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

	//	VARMSG_EX�� ������ �˸��� �����Ҹ�...
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

	//	���� Ÿ�Կ� ���õ� ó�� ��ƾ
	//		- ���� ��Ŷ���μ� ���̾� ���� Ÿ���� ������ �ָ�, �װ��� �⺻ �����ڸ� ȣ���ϸ鼭 ��ü ��ȯ
	//		{ NEXT_TYPE& rNT = VARMSG_EX_T::InstanceOf<NEXT_TYPE>(); }
	template<typename _TYPE>
	inline static _TYPE&		InstanceOf()
	{
		BYTE* pPTR = &MSG_STREAM[MSG_STREAM_OFFSET];
		ZeroMemory( pPTR, sizeof(_TYPE) );
		return *(new (pPTR) _TYPE);
	}

	//	���� Ÿ�Կ� ���õ� ó�� ��ƾ
	//		- ���� ��Ŷ���μ� ���̾� ���� Ÿ���� ������ �ָ�, �װ��� ��ü ��ȯ (�ʱ�ȭ ����)
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
/// ���� ��Ŷ�� ���ϰ� ����ϱ� ���� Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 30
	@remarks
			- ���ø����� ���� �������� ���� ����� �� �� �ְ� �ߴ�.
			- buffer overrun erroró���� �� �� �ִ�.
	@note
			- ������ ��Ŷ�� �޴� ��Ŷ�� ó������� �ٸ��Ƿ� ������ �����Ѵ�.
	@Usage
			- ������
				VarPacket<100> varpack;
				varpack.SetShort(2344);
				varpack.SetStream("��Ʈ���̰ų����۴�", strlen("��Ʈ���̰ų����۴�"));
			- ������
				short data1 = 0;
				char data2[128];	// ����� �Ҵ�
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
	// Ÿ�Ժ��� ����� ��� �Ѵ�! 
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