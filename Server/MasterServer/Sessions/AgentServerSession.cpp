#include "stdafx.h"
#include "AgentServerSession.h"
#include <PacketHandler/Handler_AM.h>

#include <Function/ServerInfoManager.h>
#include "ServerSessionManager.h"

// CHANGES: f110617.1L
void AgentServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	Services().SetSyncKey( AGENT_SERVER );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__


	// 접속한 에이전트 서버에 필요한 접속 및 리슨 정보를 내려준다.
	SERVER_KEY key		= GetServerKey();
	BYTE byWorldID		= key.GetWorldID();
	BYTE byChannelID	= key.GetChannelID();

	SERVER_INFO	infoGAMEDBP;
	SERVER_INFO	infoAGENT;
	SERVER_INFO	infoWORLD;
	SERVER_INFO	infoGUILD;
	SERVER_INFO	infoAUTHAGENT;

	ServerInfoManager::GetGameDBProxyInfo( byWorldID, byChannelID, &infoGAMEDBP );
	ServerInfoManager::GetAgentServerInfo( byWorldID, byChannelID, &infoAGENT );
	ServerInfoManager::GetWorldServerInfo( byWorldID, byChannelID, &infoWORLD );
	ServerInfoManager::GetGuildServerInfo( byWorldID, byChannelID, &infoGUILD );
	ServerInfoManager::GetAuthAgentServerInfo( &infoAUTHAGENT );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_AM_CONNECTION_SERVER_INFO_CMD* pMsgCMD =
		new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_AM_CONNECTION_SERVER_INFO_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_AM_CONNECTION_SERVER_INFO_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	pMsgCMD->m_byCategory			= AM_CONNECTION;
	pMsgCMD->m_byProtocol			= AM_CONNECTION_SERVER_INFO_CMD;
	pMsgCMD->m_ServerKey			= key;
	pMsgCMD->m_dwServerGUID			= infoAGENT.dwServerGUID;

	memcpy( pMsgCMD->m_szDBProxyServerIP, infoGAMEDBP.szInnerIP, sizeof(pMsgCMD->m_szDBProxyServerIP) );
	pMsgCMD->m_wDBProxyServerPort	= infoGAMEDBP.wInnerPort;

	memcpy( pMsgCMD->m_szAgentServerIP, infoAGENT.szIP, sizeof(pMsgCMD->m_szAgentServerIP) );
	pMsgCMD->m_wAgentServerPort		= infoAGENT.wPort;

	memcpy( pMsgCMD->m_szAgentServerInnerIP, infoAGENT.szInnerIP, sizeof(pMsgCMD->m_szAgentServerInnerIP) );
	pMsgCMD->m_wAgentServerInnerPort	= infoAGENT.wInnerPort;

	memcpy( pMsgCMD->m_szWorldServerIP, infoWORLD.szInnerIP, sizeof(pMsgCMD->m_szWorldServerIP) );
	pMsgCMD->m_wWorldServerPort		= infoWORLD.wInnerPort;

	memcpy( pMsgCMD->m_szWorldServerClientIP, infoWORLD.szIP , sizeof(pMsgCMD->m_szWorldServerClientIP) );
	pMsgCMD->m_wWorldServerClientPort = infoWORLD.wPort;

	memcpy( pMsgCMD->m_szAuthAgentServerIP, infoAUTHAGENT.szInnerIP , sizeof(pMsgCMD->m_szAuthAgentServerIP) );
	pMsgCMD->m_wAuthAgentServerPort = infoAUTHAGENT.wInnerPort;

	memcpy( pMsgCMD->m_szGuildServerIP, infoGUILD.szInnerIP , sizeof(pMsgCMD->m_szGuildServerIP) );
	pMsgCMD->m_wGuildServerPort = infoGUILD.wInnerPort;

	// <WARNING!>
	//   TempServerSession을 통해 바로 Redirect가 일어나면 안된다. 꼭 변경해야 한다.
	//   현재는 테스트를 위한 것이므로 기존 코드를 유지한다.
	Send( (BYTE*)pMsgCMD, sizeof(MSG_AM_CONNECTION_SERVER_INFO_CMD) );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 지금 구조를 바꾸기에는 시간이 빠듯하다. 일단 이렇게 수행하기로 하자
	//if( !ScriptDataManager::GetNeedLoadDataCount() )
	//{
	//	MSG_AM_CONNECTION_PHASE_SHIFT_CMD msgCMD;
	//	pMsgCMD->m_dwPHASE_SHIFT = PHASE_EXCHANGE;
	//	Send( (BYTE*)&msgCMD, sizeof(msgCMD) );
	//}
#endif
    // CHANGES: f110617.1L
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}


VOID	AgentServerSession::Init()
{
	ServerSessionEx::Init();
	m_CCUofCHANNEL = 0;
}

VOID	AgentServerSession::CCUReport( MSG_AM_CONNECTION_CCU_REPORT_NTF* IN pINFO )
{
	DWORD dwCCU = pINFO->dwVillageCount;
	dwCCU += pINFO->dwVillageCountPC;
	dwCCU += pINFO->dwBattleZoneCount;
	dwCCU += pINFO->dwBattleZoneCountPC;

	m_CCUofCHANNEL = dwCCU;
}