#ifndef __PACKETSTRUCT_AM_H__
#define __PACKETSTRUCT_AM_H__

#include <ServerStruct.h>
#include <PacketStruct.h>
#include <Protocol_AM.h>

#pragma pack(push,1)

#define PACKET_AM_OPEN( c, p )			\
	struct MSG_##p : public MSG_BASE	\
	{	MSG_##p() { m_byCategory = c; m_byProtocol = p; }

#define PACKET_AM_CLOSE		};


// ī�װ� CONNECTION
struct MSG_AM_CONNECTION_SERVER_INFO_CMD : public MSG_BASE
{
	SERVER_KEY		m_ServerKey;
	DWORD			m_dwServerGUID;
	char			m_szDBProxyServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wDBProxyServerPort;
	char			m_szAgentServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wAgentServerPort;
	char			m_szAgentServerInnerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wAgentServerInnerPort;
	char			m_szWorldServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wWorldServerPort;
	char			m_szWorldServerClientIP[MAX_IP_STRING_LEN+1];
	WORD			m_wWorldServerClientPort;
	char			m_szAuthAgentServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wAuthAgentServerPort;
	char			m_szGuildServerIP[MAX_IP_STRING_LEN+1];
	WORD			m_wGuildServerPort;
};

// A->M (World ������ ��� Agent �������� �����ϱ� �����̴�.
PACKET_AM_OPEN( AM_CONNECTION, AM_CONNECTION_CCU_REPORT_NTF )
	DWORD   dwVillageCount;
	DWORD   dwVillageCountPC;
	DWORD   dwBattleZoneCount;
	DWORD   dwBattleZoneCountPC;
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    DWORD   numberOfInVillageHangame;       // Village    | Default | Hangame
    DWORD   numberOfInVillageHangamePC;     // Village    | PC      | Hangame
    DWORD   numberOfInBattleZoneHangame;    // BattleZone | Default | Hangame
    DWORD   numberOfInBattleZoneHangamePC;  // BattleZone | PC      | Hangame
#endif
#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
  #if !defined(_CHANNELING_AUTH_FLOW_HANGAME_USER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
    DWORD   numberOfInVillageNeowiz;       // Village    | Default | Neowiz
    DWORD   numberOfInVillageNeowizPC;     // Village    | PC      | Neowiz
    DWORD   numberOfInBattleZoneNeowiz;    // BattleZone | Default | Neowiz
    DWORD   numberOfInBattleZoneNeowizPC;  // BattleZone | PC      | Neowiz
#endif
PACKET_AM_CLOSE

PACKET_AM_OPEN( AM_CONNECTION, AM_CONNECTION_ENTERSERVER_CMD )
	DWORD			m_UserGuid;
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    TCHAR			m_pUserID[ID_MAX_LEN + 1];
#else
	TCHAR			m_pUserID[(ID_MAX_LEN>>1) + 1];
#endif
	DWORD			m_IP;
PACKET_AM_CLOSE

// <Waverix><ArmorKit>
//////////////////////////////////
// Armorkit ���� ��Ŷ
struct MSG_AM_ARMORKIT_FINDHACK_SYN : public MSG_BASE
{
	MSG_AM_ARMORKIT_FINDHACK_SYN() { m_byCategory = AM_ARMORKIT; m_byProtocol = AM_ARMORKIT_FINDHACK_SYN; }

	DWORD	dwGuid;			// ��ŷ�� ������ Ŭ���̾�Ʈ guid
	TCHAR	szIP[16];		// ��ŷ�� ������ Ŭ���̾�Ʈ IP�ּ�
	int		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
	int		Size;			// Buffer �� ����
	TCHAR	Buf[1024];		// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)
};



/*
// ī�װ� OPERATION
struct MSG_AM_OPERATION_USERCOUNT_SYN : public MSG_BASE
{
	MSG_AM_OPERATION_USERCOUNT_SYN() { m_byCategory = AM_OPERATION; m_byProtocol = AM_OPERATION_USERCOUNT_SYN; }
	DWORD			dwVillageCount;
	DWORD			dwBattleZoneCount;
};
*/
/*
// ���� ���� ��û
struct MSG_AM_OPERATION_SERVERSHUTDOWN_REQ : public MSG_BASE
{
	MSG_AM_OPERATION_SERVERSHUTDOWN_REQ() { m_byCategory = AM_OPERATION; m_byProtocol = AM_OPERATION_SERVERSHUTDOWN_REQ; }
};

// ���� ���� ����
struct MSG_AM_OPERATION_SERVERSHUTDOWN_ANS : public MSG_BASE
{
	MSG_AM_OPERATION_SERVERSHUTDOWN_ANS() { m_byCategory = AM_OPERATION; m_byProtocol = AM_OPERATION_SERVERSHUTDOWN_ANS; }
};
*/

struct MSG_AM_OPERATION_CHAR_DISCONNECT_SYN : public MSG_BASE
{
	MSG_AM_OPERATION_CHAR_DISCONNECT_SYN() { m_byCategory = AM_OPERATION; m_byProtocol = AM_OPERATION_CHAR_DISCONNECT_SYN; }
	DWORD		m_dwUserGuid;
};

struct MSG_AM_OPERATION_CHAR_DISCONNECT_ACK : public MSG_BASE
{
	MSG_AM_OPERATION_CHAR_DISCONNECT_ACK() { m_byCategory = AM_OPERATION; m_byProtocol = AM_OPERATION_CHAR_DISCONNECT_ACK; }
	DWORD		m_dwUserGuid;
};


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

PACKET_AM_OPEN( AM_CONNECTION, AM_CONNECTION_PHASE_SHIFT_CMD )
	DWORD	m_dwPHASE_SHIFT;
PACKET_AM_CLOSE

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

// SecureLog�� ���� ������ ����� ���� MasterServer�� �ش� Agent�� ������ User�� ���� ���� ���� ��� ������ ��Ŷ
PACKET_AM_OPEN( AM_OPERATION, AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD )
	inline DWORD&	UserGUID() { return m_dwKey; }
PACKET_AM_CLOSE


//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif // __PACKETSTRUCT_AM_H__