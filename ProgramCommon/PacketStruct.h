#ifndef __PACKETSTRUCT_H__
#define __PACKETSTRUCT_H__

#include <windows.h>
#include <StructInPacket.h>

#pragma once

#pragma pack(push,1)


/**
	패킷 구조체의 Naming Rule

=> MSG_CF_패킷구조체이름_REQ

1) 요청(Request)					_REQ
2) 요청에 대한 응답(Answer)			_ANS
3) Client -> Server 통보(Sync)		_SYN
4) Server -> Client 통보(Command)	-CMD
5) Server -> Client Broadcasting	_BRD

 => 수정 예정 : 2004-10.19
1) 요청(Request)					_SYN
2) 요청에 대한 허락					_ACK
3) 요청에 대한 거부					_NAK
4) Server -> Client 통보(Notify)	_NTF
5) Server -> Client Broadcasting	_BRD

*/



//-------------------------------------------------------------------------------------------------------
/// @name 기본 정보들

// - 가장 기초 패킷

//==================================================================================================
#define	__WAVERIX_CRYPT_ACK__			// 클라 CRYPTOGRAPHY_ACK 받기 위함, KEY<-CMD
#define	__WAVERIX_CRYPTOGRAPHY_TEST__	// 주기적 키교환 목적 테스트 <WARNING> __WAVERIX_CRYPT_ACK__ 과 섞였다. 풀려면 다 풀자.
#define	__WAVERIX_PACKET_HANDLING__		// 클라로 UserKey 전송 안하기 위한 테스트

//--------------------------------------------
// __WAVERIX_ABUSE_EXTENDED_FORMAT__
// 자유 포맷, 검색조건 강화를 포함해서 확장된 포맷
#define __WAVERIX_ABUSE_EXTENDED_FORMAT__		// 추가 변경, 기존 변경은 이 전처리기 외부로...
#define __WAVERIX_ABUSE_EXTENDED_SECURELOG__	// (07/02/08) 보안 로그 강화쪽 작업



struct MSG_BASE_INTERNAL
{
	BYTE						m_byCategory;	// 카테고리
	union
	{
		BYTE					m_byProtocol;		// 카테고리 아래의 타입
		BYTE					m_byEncryptedBuffer[1];		// 암호화된 버퍼
	};
};

struct MSG_PROTO
{
#	ifdef	_SERVER
	DWORD						m_dwKey;
#	endif
};

struct MSG_BASE : public MSG_PROTO
{
	BYTE						m_byCategory;	// 카테고리
	union
	{
		BYTE					m_byProtocol;		// 카테고리 아래의 타입
		BYTE					m_byEncryptedBuffer[1];		// 암호화된 버퍼
	};
};

#ifdef _YJ_IGC
class IGC_MSG_HEADER8		// 패킷 헤더
{
public:
	DWORD		dwID;			// 프로토콜 ID
};

#endif

struct MSG_SERVER_TYPE : public MSG_BASE
{
	BYTE						m_byServerType;
};

struct MSG_BASE_FORWARD : public MSG_BASE
{
};

//==================================================================================================



struct MSG_DBPROXY_RESULT : public MSG_BASE
{
	VOID *						m_pData;
};
struct MSG_BASE_GUILD : public MSG_BASE_FORWARD
{
	DWORD						m_dwSessionIndex;
};

#pragma pack(pop)


#endif // __PACKETSTRUCT_H__

