#ifndef __PACKETSTRUCT_SM_H__
#define __PACKETSTRUCT_SM_H__

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_SM.h>

#define __WAVERIX_DONT_HAVE_KEY_DATA__

#pragma pack(push,1)

#define PACKET_SM_OPEN( c, p )			\
struct MSG_##p : public MSG_BASE	\
	{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_SM_CLOSE		};


//================================================================================
// <SM_CONNECTION>
//
// <NOTICE>
//   상호 인증을 위한 교환 알고리즘이 현재 (060817) 미구현된 상태이므로 임시적으로
//   패킷을 처리하기로 한다.
//

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_HEARTBEAT )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_CONNECTION_SYN )
#ifdef __WAVERIX_DONT_HAVE_KEY_DATA__
#else
#endif
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_CONNECTION_ACK )
#ifdef __WAVERIX_DONT_HAVE_KEY_DATA__
#else
#endif
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_CONNECTION_NAK )
#ifdef __WAVERIX_DONT_HAVE_KEY_DATA__
#else
#endif
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_DISCONNECT_CMD )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_PHASE_SHIFT_CMD )
	DWORD	m_dwPHASE_SHIFT;
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CONNECTION, SM_CONNECTION_BLOCK_CMD )
PACKET_SM_CLOSE

//
//================================================================================




//================================================================================
// <SM_SERVER_INFO>
//

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_SYN )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_ACK )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_NAK )
PACKET_SM_CLOSE

// 28B
typedef struct sSERVER_INFO_GENERIC
{
	DWORD			m_dwMasterIP;
	SERVER_KEY		m_ServerKey;
	eSERVER_TYPE	m_ServerType;
	DWORD			m_dwPublicIP;
	DWORD			m_dwNAT_IP;
	DWORD			m_dwInnerIP;
	WORD			m_wPublicPort;
	//WORD			m_wNAT_Port;
	WORD			m_wInnerPort;
} *PSERVER_INFO_GENERIC;

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_QUERY_SYN )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_QUERY_ACK )
	enum { _MAX_SERVERINFO_ARRAY_COUNT = 100, };
	WORD					m_wRndKey;
	WORD					m_wCounts;
	sSERVER_INFO_GENERIC	m_ServerInfos[_MAX_SERVERINFO_ARRAY_COUNT];
	WORD					GetSize()
	{
		return (WORD)( sizeof(*this) - (sizeof(sSERVER_INFO_GENERIC)*m_wCounts) );
	}
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_QUERY_CMD )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_QUERY_NAK )
PACKET_SM_CLOSE

// 서버 인증
// GameServers (MSG_SERVER_TYPE) -> <MasterServer> -> SolarAuthServer
PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_SYN )
	DWORD					m_dwInnerIP;
	SERVER_KEY				m_ServerKey;
PACKET_SM_CLOSE

// (1) <MasterServer> -> SolarAuthServer
// (2) MasterServer <- <SolarAuthServer>
PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_ACK )
	DWORD					m_dwInnerIP;
	sSERVER_INFO_GENERIC	m_ServerInfo;	//요청한 서버 정보
PACKET_SM_CLOSE

// (1) <MasterServer> -> SolarAuthServer
// (2) MasterServer <- <SolarAuthServer>
PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_NAK )
	DWORD					m_ErrorCode;
PACKET_SM_CLOSE

//__NA000000_070523_REPORT_TO_SOLARAUTH
PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_CACHEITEM_REPORT_NTF )
	sCACHEITEM_REPORT_INFO_MINORVER		m_INFO;
PACKET_SM_CLOSE

struct sCCU_MASTER2SOLARAUTH_INFO
{
	BYTE					byIDofWORLD;
	DWORD					dwCCU;
};

PACKET_SM_OPEN( SM_SERVER_INFO, SM_SERVER_INFO_CCU_REPORT_NTF )
	enum { MAX_WORLD_CCU_LIST_SIZE = 10 };
	BYTE					byCount;
	DWORD					m_INFO[MAX_WORLD_CCU_LIST_SIZE];

	inline VOID				Clear()
	{ byCount=0; ZeroMemory( m_INFO, sizeof(m_INFO) ); }

	inline WORD				GetSize()
	{
		return (WORD) ( (sizeof(*this) - sizeof(m_INFO)) + (sizeof(m_INFO)*byCount) );
	}
PACKET_SM_CLOSE
//
//================================================================================




//================================================================================
// <SM_CRYPTOGRAPHY>
//

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_KEY )
	enum { eSM_CRYPTOGRAPHY_KEY_BLOCK = 200 };
	BYTE				m_bySN;			// Sequence Number
	BYTE				m_byCryptInfo;	// 인코딩된 패킷 암호화키 블록 풀기 위한 키
	union
	{
		DWORD			m_dwUserKey;
		BYTE			m_byCryptData[eSM_CRYPTOGRAPHY_KEY_BLOCK];	// 알고리즘 정보 (키, 등등...)
	};
	inline BYTE&		SNKey()			{ return m_bySN; }
	inline BYTE&		CryptInfo()		{ return m_byCryptInfo; }
	inline BYTE*		GetCryptStream(){ return m_byCryptData; }
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_ACK )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_NAK )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_CMD )
PACKET_SM_CLOSE

// 암호화된 패킷 풀기위한...
struct MSG_SM_CRYPTOGRAPHY_PACKET_ENCODED;

// 패킷 암호화 하기 위한...
struct MSG_SM_CRYPTOGRAPHY_PACKET_ENCODING;
// <END>

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_KEY )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_CMD )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_ACK )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_NAK )
PACKET_SM_CLOSE


//
//================================================================================




//================================================================================
// <SM_DATA_TRANSFER>
//

PACKET_SM_OPEN( SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN )
	WORD	m_wDataType;	// 스크립트 번호
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
	// Error Code
PACKET_SM_CLOSE

//-------------------------------------------------------------------------------------------------------
//
//
PACKET_SM_OPEN( SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_CMD )
// <TYPE><DEFINITIONs>
enum
{
	TRANS_CMD_NOTIFY = 0,
	TRANS_CMD_SEND_HEADER = 1,
	TRANS_CMD_SEND_DATA = 2,
	TRANS_CMD_SEND_CLOSURE = 3,
	TRANS_CMD_UPPERBOUND
};

struct SEND_INFO_FILE_INFO
{
	WORD	m_wDataType;	// 스크립트 번호, 파서 종류 선택해서...
	DWORD	m_dwBufferSize;	// 앞으로 보내질 패킷의 총 크기
	DWORD	m_PlainSize;	// 앞으로 보내질 총 크기의 Uncompress된 크기
	DWORD	m_dwCRC;		// 보내진 데이터의 CRC값
};

struct SEND_INFO_FILE_CLOSURE
{
	WORD	m_wDataType;
	DWORD	m_dwCRC;
};

struct SEND_INFO_TOKEN_INFO_HEADER
{
	WORD	m_wDataType;	// 스크립트 번호, 파서 종류 선택해서...
	WORD	m_wTokenNo;		// 이 블록에 포함된 토큰 번호
	WORD	m_wBufferSize;	// 토큰에 포함된 실제 데이터 길이
};

struct SEND_INFO_TOKEN_INFO
{
	enum { MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE = 4000 };
	SEND_INFO_TOKEN_INFO_HEADER		m_Header;
	BYTE	m_pSTREAM[MAX_SENDDATA_CMD_SEND_INFO_TOKEN_SIZE];

	WORD	GetSize()
	{
		return (WORD)( (sizeof(*this)-sizeof(m_pSTREAM)) + (sizeof(BYTE)*m_Header.m_wBufferSize) );
	}
};

// <DATA>
	WORD	m_wControlCmd;
	union
	{
		SEND_INFO_FILE_INFO		m_FILE_INFO;
		SEND_INFO_FILE_CLOSURE	m_FILE_CLOSURE;
		SEND_INFO_TOKEN_INFO	m_TOKEN_INFO;	//이 값이 제일 크다
	};

	WORD	GetSize()
	{
		switch( m_wControlCmd )
		{
		case TRANS_CMD_NOTIFY:
			return (WORD)( sizeof(*this)-sizeof(m_TOKEN_INFO) );
		case TRANS_CMD_SEND_HEADER:
			return (WORD)( ( sizeof(*this)-sizeof(m_TOKEN_INFO) ) + sizeof(m_FILE_INFO) );
		case TRANS_CMD_SEND_CLOSURE:
			return (WORD)( ( sizeof(*this)-sizeof(m_TOKEN_INFO) ) + sizeof(m_FILE_CLOSURE) );
		case TRANS_CMD_SEND_DATA:
			return (WORD)( ( sizeof(*this)-sizeof(m_TOKEN_INFO) ) + m_TOKEN_INFO.GetSize() );
		}
		return 0;
	}
PACKET_SM_CLOSE
//
//-------------------------------------------------------------------------------------------------------

PACKET_SM_OPEN( SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_ACK )
	WORD	m_wDataType;	// 스크립트 번호
PACKET_SM_CLOSE

PACKET_SM_OPEN( SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_NAK )
	WORD	m_wDataType;	// 스크립트 번호
PACKET_SM_CLOSE


//
//================================================================================




#pragma pack(pop)

#endif //__PACKETSTRUCT_SM_H__


