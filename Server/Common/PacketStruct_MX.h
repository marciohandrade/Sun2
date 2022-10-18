#ifndef __PACKETSTRUCT_MX_H__
#define __PACKETSTRUCT_MX_H__

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_MX.h>

#pragma pack(push,1)

// 이 메세지 뒤에 SERVER_INFO 구조체 정보를 붙여서 보냄
struct MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN : public MSG_BASE
{
};

struct MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK : public MSG_BASE
{
	enum { _MAX_SERVERINFO_ARRAY_NUM = 100, };
	DWORD						m_dwNumberOfServerInfo;
	SERVER_INFO					m_ServerInfo[_MAX_SERVERINFO_ARRAY_NUM];
	int							GetPacketSize()
	{
		return ( sizeof(MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK) - sizeof(SERVER_INFO) * ( _MAX_SERVERINFO_ARRAY_NUM - m_dwNumberOfServerInfo ) );
	}
};
struct MSG_MX_CONNECTION_REQ_SERVER_INFO_CMD : public MSG_BASE
{
};

struct MSG_MX_FUNCRESTRICT_RELOAD_REQ : public MSG_BASE
{
	MSG_MX_FUNCRESTRICT_RELOAD_REQ() { m_byCategory = MX_CONNECTION; m_byProtocol = MX_FUNCRESTRICT_RELOAD_REQ; }
};

struct MSG_MX_FUNCRESTRICT_RELOAD_ANS : public MSG_BASE
{
	enum { _MAX_FUNCRESTRICT_LIST_NUM = 100, };

	MSG_MX_FUNCRESTRICT_RELOAD_ANS() { m_byCategory = MX_CONNECTION; m_byProtocol = MX_FUNCRESTRICT_RELOAD_ANS; }

	BYTE						byRestrictNum;
	STRUCT_RESTRICT_MASTER		sRestrict[_MAX_FUNCRESTRICT_LIST_NUM];

	int GetSize()
	{
		return ( sizeof(MSG_MX_FUNCRESTRICT_RELOAD_ANS) - ( sizeof(STRUCT_RESTRICT_MASTER) * (_MAX_FUNCRESTRICT_LIST_NUM - byRestrictNum) ) );
	}
};

struct MSG_MX_FUNCRESTRICT_RELOAD_NAK : public MSG_BASE
{
	MSG_MX_FUNCRESTRICT_RELOAD_NAK() { m_byCategory = MX_CONNECTION; m_byProtocol = MX_FUNCRESTRICT_RELOAD_NAK; }

	DWORD		m_dwErrorCode;
};

#define PACKET_MX_OPEN( c, p )			\
struct MSG_##p : public MSG_BASE	\
{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_MX_CLOSE		};

//
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

PACKET_MX_OPEN( MX_CONNECTION, MX_CONNECTION_DISCONNECT_CMD )
PACKET_MX_CLOSE

PACKET_MX_OPEN( MX_CONNECTION, MX_CONNECTION_PHASE_SHIFT_CMD )
	DWORD	m_dwPHASE_SHIFT;
PACKET_MX_CLOSE

PACKET_MX_OPEN( MX_CONNECTION, MX_CONNECTION_BLOCK_CMD )
PACKET_MX_CLOSE

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

//
PACKET_MX_OPEN( MX_LOG, MX_LOG_SECURE_LOG_NTF )
	enum eSIZE { MAXIPSTRINGLEN = 15, };
	//
	BYTE		m_byWorldID;
	BYTE		m_byChannelID;
	BYTE		m_byServerType;
	BYTE		m_byCategory;
	__int64		m_LoggedTime;
	TCHAR		m_pUserID[ID_MAX_LEN+1];
	TCHAR		m_pCharName[MAX_CHARNAME_LENGTH];
	TCHAR		m_pIP[MAXIPSTRINGLEN+1];
	//
	WORD		m_wLength;		// with NULL?
	CHAR		m_pStream[1];	// 가변 패킷용 포인터
PACKET_MX_CLOSE

#pragma pack(pop)

#endif	// __PACKETSTRUCT_MX_H__