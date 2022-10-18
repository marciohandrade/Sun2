#ifndef __AUTH_PROTOCOL_H__
#define __AUTH_PROTOCOL_H__

#include <SunAuth/AuthDefine.h>
#include <PacketStruct.h>

#pragma pack(push, 1)

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Agent
//		F : Login Front
//		A : Auth Agent
//	    B : Billing Server
//		R : RTT Agent
//		S : Auth Server
//
// * Postfix
//		SYN - Server�� ��û
//		ACK - ��û�� ���� ���
//		NAK - ��û�� ���� �ź�
//		CMD - Server�� ���
//		BRD - Server�� ����ȭ ���� ������ �˸�
//
// * �������� Naming Rule
//		Prefix_ī�װ���_�������ݸ�_Postfix
//		ex) CG_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------

#define PACKET_INTERNAL_OPEN( c, p )	\
    struct MSG_##p : public MSG_BASE_INTERNAL { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_INTERNAL_CLOSE	};

#define PACKET_INTERNAL_INHERITED_OPEN( c, p, base_ )  \
    struct MSG_##p : public base_ { MSG_##p() { m_byCategory = c; m_byProtocol = p; }
#define PACKET_INTERNAL_INHERITED_CLOSE	    };


enum eCATEGORY_AUTH
{
	CATEGORY_AUTH = 51,
};

// �α��� ����
enum ePROTOCOL_AUTH
{
	// ������ LoginFront���� ���ʷ� Ŭ���̾�Ʈ�� ����
	FC_AUTH_READY_CMD,

	// Ŭ���̾�Ʈ�� ���� ��û(����Ȯ��)
	CF_AUTH_VERIFY_SYN,
	FC_AUTH_VERIFY_ACK,

	// ID, PW üũ - START!
	CF_AUTH_ID_PASSWORD_CHECK_SYN,
	FS_AUTH_ID_PASSWORD_CHECK_SYN,
    // Channeling AuthToken : __KR000000_090128_CHANNELING__
    //< Peer-to-Peer�� ���������� �и����� ����.
    //�л�ó�� ���α׷����� �ؾ��� ��� �߿� �ϳ���.
    CF_AUTH_TOKEN_PASS_SYN      = CF_AUTH_ID_PASSWORD_CHECK_SYN,
    FS_AUTH_TOKEN_PASS_SYN      = FS_AUTH_ID_PASSWORD_CHECK_SYN,
    // 'NorthA' Channeling AuthToken : __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    CF_AUTH_TOKEN_NORTHA_PASS_SYN  = CF_AUTH_TOKEN_PASS_SYN,
    FS_AUTH_TOKEN_NORTHA_PASS_SYN  = FS_AUTH_TOKEN_PASS_SYN,
    // 'Global' Channeling AuthToken : __NA000000_GLOBAL_AUTH_PROCESS__
    CF_AUTH_TOKEN_GLOBAL_PASS_SYN  = CF_AUTH_TOKEN_NORTHA_PASS_SYN,
    FS_AUTH_TOKEN_GLOBAL_PASS_SYN  = FS_AUTH_TOKEN_NORTHA_PASS_SYN,
    //�Ϻ� ������ �α��� 
    CF_AUTH_TOKEN_JP_GAMECHU_SYN  = CF_AUTH_TOKEN_GLOBAL_PASS_SYN,
    FS_AUTH_TOKEN_JP_GAMECHU_SYN  = FS_AUTH_TOKEN_GLOBAL_PASS_SYN,
    //dev : CF, FS SYN �������� ���� ����
    // 'Russia' Ingamba auth process
    CF_AUTH_PASS_RU_INGAMBA_SYN  = CF_AUTH_TOKEN_JP_GAMECHU_SYN,
    FS_AUTH_PASS_RU_INGAMBA_SYN  = FS_AUTH_TOKEN_JP_GAMECHU_SYN,
    //!~ _CN_ZY_AUTH_PROCESS
    CF_AUTH_ZY_PASSWORD_CHECK_SYN   = CF_AUTH_PASS_RU_INGAMBA_SYN,
    FS_AUTH_ZY_PASSWORD_CHECK_SYN   = FS_AUTH_PASS_RU_INGAMBA_SYN,
    //~! _CN_ZY_AUTH_PROCESS
	// ID, PW üũ - ���� ����
	SF_AUTH_SHAPE_SYN,
	FC_AUTH_SHAPE_SYN,
	CF_AUTH_SHAPE_ACK,
	FS_AUTH_SHAPE_ACK,

	// ID, PW üũ - ��Ʈ���� ī�� ����
	SF_AUTH_MATRIX_CARD_SYN,
	FC_AUTH_MATRIX_CARD_SYN,
	CF_AUTH_MATRIX_CARD_ACK,
	FS_AUTH_MATRIX_CARD_ACK,

    //_JP000000_ONETIMEPASSWORD_AUTHFLOW_
    CF_AUTH_OTP_PASS_CMD = SF_AUTH_MATRIX_CARD_SYN, // S -> C (show OTP windows, ref: packet)
    CF_AUTH_OTP_PASS_SYN = FC_AUTH_MATRIX_CARD_SYN, // C -> S (retry with OTP)
    CF_AUTH_OTP_PASS_ACK = CF_AUTH_MATRIX_CARD_ACK, // S -> C (if success then next, if fail then retry 'show OTP window')

    AU_AUTH_OTP_PASS_CMD = CF_AUTH_OTP_PASS_CMD, // S -> F (ref: CF_AUTH_OTP_PASS_CMD)
    AU_AUTH_OTP_PASS_SYN = CF_AUTH_OTP_PASS_SYN, // F -> S (ref: CF_AUTH_OTP_PASS_SYN)
    AU_AUTH_OTP_PASS_ACK = CF_AUTH_OTP_PASS_ACK, // S -> F (ref: CF_AUTH_OTP_PASS_ACK)

	//__CN_20100909_SECURITY_TOKEN__
	CF_AUTH_SECURITY_TOKEN_SYN = FC_AUTH_MATRIX_CARD_SYN, // C -> S (retry with OTP)
	CF_AUTH_SECURITY_TOKEN_ACK = CF_AUTH_MATRIX_CARD_ACK, // S -> C (if success then next, if fail then retry 'show OTP window')
	
	AU_AUTH_SECURITY_TOKEN_SYN = CF_AUTH_SECURITY_TOKEN_SYN, // F -> S
	AU_AUTH_SECURITY_TOKEN_ACK = CF_AUTH_SECURITY_TOKEN_ACK, // S -> F
    //
    // NOTE: security protocol last boundary (4/4)
    AU_AUTH_OTP_PASS_RESERVED_SLOT = FS_AUTH_MATRIX_CARD_ACK, // no specification, reserved slot
    //
	// ID, PW üũ - END!
	SF_AUTH_ID_PASSWORD_CHECK_ACK,
	FC_AUTH_ID_PASSWORD_CHECK_ACK,
    // Channeling AuthToken : __KR000000_090128_CHANNELING__
    SF_AUTH_TOKEN_PASS_ACK      = SF_AUTH_ID_PASSWORD_CHECK_ACK,
    FC_AUTH_TOKEN_PASS_ACK      = FC_AUTH_ID_PASSWORD_CHECK_ACK,
    // Server Select Facility : __NA001334_090420_SERVER_SELECT_FACILITY__
    SF_AUTH_REAUTH_PASS_ACK     = SF_AUTH_TOKEN_PASS_ACK,
    FC_AUTH_REAUTH_PASS_ACK     = FC_AUTH_TOKEN_PASS_ACK,
    // NorthA Channeling AuthToken : __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    SF_AUTH_TOKEN_NORTHA_PASS_ACK  = SF_AUTH_REAUTH_PASS_ACK,
    FC_AUTH_TOKEN_NORTHA_PASS_ACK  = FC_AUTH_REAUTH_PASS_ACK,
    // 'Global' Channeling AuthToken : __NA000000_GLOBAL_AUTH_PROCESS__
    SF_AUTH_TOKEN_GLOBAL_PASS_ACK  = SF_AUTH_TOKEN_NORTHA_PASS_ACK,
    FC_AUTH_TOKEN_GLOBAL_PASS_ACK  = FC_AUTH_TOKEN_NORTHA_PASS_ACK,
    //dev : SF, FC ACK �������� ����
    // 'Russia' Ingamba Auth process : _RU_INGAMBA_AUTH_PROCESS
    SF_AUTH_PASS_RU_INGAMBA_ACK = SF_AUTH_TOKEN_GLOBAL_PASS_ACK,
    FC_AUTH_PASS_RU_INGAMBA_ACK  = FC_AUTH_TOKEN_GLOBAL_PASS_ACK,

	// ���� ��� ��û
	CF_AUTH_SERVER_LIST_SYN,
	SF_AUTH_SERVER_LIST_NTF,
	FC_AUTH_SERVER_LIST_WORLD_ACK,
	FC_AUTH_SERVER_LIST_CHANNEL_ACK,

	// ���� ���� - START!
	CF_AUTH_SERVER_SELECT_SYN,
	FS_AUTH_SERVER_SELECT_SYN,

	// ���� ���� - GameAgent ���� �� �̸� �˸�
	SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN,
	AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN,
	GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK,
	AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK,

	// ���� ���� - END!
	SF_AUTH_SERVER_SELECT_ACK,
	FC_AUTH_SERVER_SELECT_ACK,

	// PC LOGIN, LOGOUT CMD From GameAgent
	GA_AUTH_PC_LOGIN_CMD,
	AS_AUTH_PC_LOGIN_CMD,
	GA_AUTH_PC_LOGOUT_CMD,
	AS_AUTH_PC_LOGOUT_CMD,

	// PC LOGOUT ASK To GameAgent
	SA_AUTH_ASK_PC_LOGOUT_SYN,
	AG_AUTH_ASK_PC_LOGOUT_SYN,
	GA_AUTH_ASK_PC_LOGOUT_ACK,
	AS_AUTH_ASK_PC_LOGOUT_ACK,

	// GameAgent LOGIN
	AS_AUTH_GAMEAGENT_LOGIN_CMD,

	// Billing ������ GameAgnet�� �˸�
	SA_AUTH_BILLING_ALARM_CMD,
	AG_AUTH_BILLING_ALARM_SYN,
	GA_AUTH_BILLING_ALARM_ACK,

	// Client Disconnect ���� ���� From LoginFront
	FS_AUTH_CLIENT_DISCONNECT_CMD,

	// Client Disconnect ��� ���� To LoginFront
	SF_AUTH_CLIENT_DISCONNECT_CMD,

	// PC Name�� UserCount NTF From GameAgent
	GA_AUTH_PC_NAME_NTF,
	AS_AUTH_PC_NAME_NTF,
	GA_AUTH_USER_COUNT_NTF,
	AS_AUTH_USER_COUNT_NTF,

	// NTF To LoginFront
	SF_AUTH_GAME_VERSION_NTF,
	SF_AUTH_CONTROL_IPLIST_NTF,

	// NTF To AuthAgent
	SA_AUTH_AGENT_IP_LIST_NTF,

    // ���� ���� ��� ������ __NA001334_090420_SERVER_SELECT_FACILITY__
    CF_AUTH_REAUTH_PASS_SYN,
    FS_AUTH_REAUTH_PASS_SYN,

    //----------------------------------------------------------------------------------------------
    // ���� ���� ��� ���� (AuthServer <-> AgentServer) __NA001334_090420_SERVER_SELECT_FACILITY__
    // (WAVERIX) (NOTE) ������ �������� ȥ�������� �ȵǰڴ�.
    // ���� �迭(AuthAgent�迭 ��� ������ ����, ���� ���� AuthAgent�� ���ſ���)
    AU_AUTH_SERVER_SELECT_SYN,  // AgentServer -> (AuthAgent) -> AuthServer
    AU_AUTH_SERVER_SELECT_ACK,  // AuthServer -> (AuthAgent) -> AgentServer

    AU_AUTH_COMMON_QUERY_USER_ID_SYN,   //__NA000000_SHOP_USERID_50_PASSING_PROC__
    AU_AUTH_COMMON_QUERY_USER_ID_ACK,

    AU_AUTH_BILLING_CHANGE_CONTROL_SYN, // AuthServer -> AgentServer (_PCBANG_POLICY_CHANGE_POST_EXPIRED)
    AU_AUTH_BILLING_CHANGE_CONTROL_ACK,
    // NOTE: f101030.1L, change the specification to accept multi-agent on same ip address
    AU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN, // AgentServer -> AuthAgent

    // _NA_002455_20110328_AUTH_HEART_BEAT
    AU_AUTH_HEART_BEAT_SYN, // AgentServer -> AuthAgent Heartbeat ����
    AU_AUTH_HEART_BEAT_ACK, // AuthAgent -> AgentServer Heartbeat ����

    //----------------------------------------------------------------------------------------------
    AU_AUTH_UPPERBOUND_CHECK_PROTOCOL, // CF_AUTH_REQ_PATCH_VER_XXX�迭 ���� �˻��.
	// ���İ� ��ġ���� ��û
	CF_AUTH_REQ_PATCH_VER_SYN		= 254,
	FC_AUTH_REQ_PATCH_VER_ACK		= 255,
};

// (NOTE) (100118) (WAVERIX) �ſ� ȥ���������� �������� ���� �������� �и��Ѵ�.
enum ePROTOCOL_AUTH_S2S
{
};

#pragma pack(pop)

#endif //__AUTH_PROTOCOL_H__

