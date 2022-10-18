#pragma once

#include <PacketStruct.h>
#include <SunAuth/AuthProtocol.h>

#pragma pack(push,1)


enum eCATEGORY_RTT
{
	CATEGORY_RTT = 0,
};

// RTT ���
enum ePROTOCOL_RTT
{
	// RTT���� ���� ����
	SR_RTT_CERTIFY_SYN						= 1,
	RS_RTT_CERTIFY_ACK						= 2,

	// RTT�κ��� PC �α׾ƿ��� ��û����
	RS_RTT_ASK_PC_LOGOUT_SYN				= 9,
	SR_RTT_ASK_PC_LOGOUT_ACK				= 10,

	// RTT�κ��� ��� IPList Reload�� ��û����
	RS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN	= 32,
	SR_RTT_ASK_CONTROL_IPLIST_RELOAD_ACK	= 33,

	// RTT�κ��� GameVersion Reload�� ��û����
	RS_RTT_ASK_GAME_VERSION_RELOAD_SYN		= 34,
	SR_RTT_ASK_GAME_VERSION_RELOAD_ACK		= 35,
};

// RTT���� ���� ����
PACKET_INTERNAL_OPEN( CATEGORY_RTT, SR_RTT_CERTIFY_SYN )
	RTT_SERVER_TYPE	m_dwServerType;
	DWORD			m_dwVersion;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_RTT, RS_RTT_CERTIFY_ACK )
	DWORD			m_dwResult;									// 0 : ���� ����
	DWORD			m_dwServerType;
	DWORD			m_dwServerGUID;								// ������ ������ȣ
PACKET_INTERNAL_CLOSE

// RTT���� PC �α׾ƿ��� ��û��
//  RTT Names:
//      MSG_RTTG_DROPUSER_REQ
//      MSG_RTTG_DROPUSER_ANS
PACKET_INTERNAL_OPEN( CATEGORY_RTT, RS_RTT_ASK_PC_LOGOUT_SYN )
#ifdef _ACCOUNT_ID_LENGTH_EXTENTION_
    char			m_szUserID[ID_MAX_LEN + 1];
#else
	char			m_szUserID[ID_MAX_LEN];
#endif
	RTT_DROP_REASON	m_dwDropReason;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_RTT, SR_RTT_ASK_PC_LOGOUT_ACK )
#ifdef __NA000000_090430_ADDIN_DROP_CODE__
    DWORD           m_dwDropReason; //
#endif
	DWORD			m_dwError;
#ifdef _ACCOUNT_ID_LENGTH_EXTENTION_
    char			m_szUserID[ID_MAX_LEN + 1];
#else
	char			m_szUserID[ID_MAX_LEN];
#endif
PACKET_INTERNAL_CLOSE

// RTT�κ��� ��� IPList Reload�� ��û����
PACKET_INTERNAL_OPEN( CATEGORY_RTT, RS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN )
	DWORD			m_dwWopsKey;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_RTT, SR_RTT_ASK_CONTROL_IPLIST_RELOAD_ACK )
	DWORD			m_dwWopsKey;
	DWORD			m_dwResult;
PACKET_INTERNAL_CLOSE

// RTT�κ��� GameVersion Reload�� ��û����
PACKET_INTERNAL_OPEN( CATEGORY_RTT, RS_RTT_ASK_GAME_VERSION_RELOAD_SYN )
	DWORD			m_dwWopsKey;
PACKET_INTERNAL_CLOSE

PACKET_INTERNAL_OPEN( CATEGORY_RTT, SR_RTT_ASK_GAME_VERSION_RELOAD_ACK )
	DWORD			m_dwResult;
	DWORD			m_dwWopsKey;
PACKET_INTERNAL_CLOSE


#pragma pack(pop)

















