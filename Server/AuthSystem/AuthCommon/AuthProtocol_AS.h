#pragma once

#include <PacketStruct.h>
#include <SunAuth/AuthProtocol_GA.h>

#pragma pack(push,1)


// 서버 선택 - GameAgent 검증 및 미리 알림
struct MSG_SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN : MSG_AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN
{
	MSG_SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN;
	}
	AGENTID			m_GameAgentID;
};

struct MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK : MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK
{
	MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK;
	}
	AGENTID			m_GameAgentID;
};

// PC LOGIN, LOGOUT CMD From GameAgent
struct MSG_AS_AUTH_PC_LOGIN_CMD : MSG_GA_AUTH_PC_LOGIN_CMD
{
	MSG_AS_AUTH_PC_LOGIN_CMD()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = AS_AUTH_PC_LOGIN_CMD;
	}
	AGENTID			m_GameAgentID;
};

struct MSG_AS_AUTH_PC_LOGOUT_CMD : MSG_GA_AUTH_PC_LOGOUT_CMD
{
	MSG_AS_AUTH_PC_LOGOUT_CMD()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = AS_AUTH_PC_LOGOUT_CMD;
	}
	AGENTID			m_GameAgentID;
};

// PC LOGOUT ASK To GameAgent
struct MSG_SA_AUTH_ASK_PC_LOGOUT_SYN : MSG_AG_AUTH_ASK_PC_LOGOUT_SYN
{
	MSG_SA_AUTH_ASK_PC_LOGOUT_SYN()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = SA_AUTH_ASK_PC_LOGOUT_SYN;
	}
	AGENTID			m_GameAgentID;
};

struct MSG_AS_AUTH_ASK_PC_LOGOUT_ACK : MSG_GA_AUTH_ASK_PC_LOGOUT_ACK
{
	MSG_AS_AUTH_ASK_PC_LOGOUT_ACK()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = AS_AUTH_ASK_PC_LOGOUT_ACK;
	}
	AGENTID			m_GameAgentID;
};

// GameAgent LOGIN
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, AS_AUTH_GAMEAGENT_LOGIN_CMD )
	AGENTID			m_GameAgentID;
	BYTE			m_ConnectStatus;				// 0 : Disconencted, 1 : Connected
PACKET_INTERNAL_CLOSE

// 빌링 마감을 GmaeAgent에 알림
struct MSG_SA_AUTH_BILLING_ALARM_CMD : MSG_AG_AUTH_BILLING_ALARM_SYN
{
	MSG_SA_AUTH_BILLING_ALARM_CMD()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = SA_AUTH_BILLING_ALARM_CMD;
	}
	AGENTID			m_GameAgentID;
};

// PC Name과 UserCount NTF From GameAgent
struct MSG_AS_AUTH_PC_NAME_NTF : MSG_GA_AUTH_PC_NAME_NTF
{
	MSG_AS_AUTH_PC_NAME_NTF()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = AS_AUTH_PC_NAME_NTF;
	}
	AGENTID			m_GameAgentID;
};

struct MSG_AS_AUTH_USER_COUNT_NTF : MSG_GA_AUTH_USER_COUNT_NTF
{
	MSG_AS_AUTH_USER_COUNT_NTF()
	{
		m_byCategory = CATEGORY_AUTH;
		m_byProtocol = AS_AUTH_USER_COUNT_NTF;
	}
	AGENTID			m_GameAgentID;
};

// NTF To AuthAgent
PACKET_INTERNAL_OPEN( CATEGORY_AUTH, SA_AUTH_AGENT_IP_LIST_NTF )
	BYTE				m_Count;
	sGAME_AGENT_INFO	m_AgentIPInfo[MAX_SVR];
	int GetSize()
	{
		return sizeof(*this) - (MAX_SVR-m_Count)*sizeof(sGAME_AGENT_INFO);
	}
PACKET_INTERNAL_CLOSE




#pragma pack(pop)



















