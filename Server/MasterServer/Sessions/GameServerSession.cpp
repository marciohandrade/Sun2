#include "stdafx.h"
#include "GameServerSession.h"
#include <PacketHandler/Handler_GM.h>

#include <Function/ServerInfoManager.h>
#include "ServerSessionManager.h"

#include <MasterServerEx.h>

#include <Function/Statistics.h>

VOID	GameServerSession::Init()
{
	ServerSessionEx::Init();
	m_pStatisticsData = new GameStatisticsData;
	m_pStatisticsData->Initialize();
}

VOID	GameServerSession::Release()
{
	ServerSessionEx::Release();
	SAFE_DELETE( m_pStatisticsData );
}

// CHANGES: f110617.1L
void GameServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
	// SetSyncKey??

	// ������ ���� ������ �ʿ��� ���� �� ���� ������ �����ش�.
	SERVER_KEY key = GetServerKey();
	BYTE byWorldID = key.GetWorldID();
	BYTE byChannelID = key.GetChannelID();

    SERVER_INFO infoGAMEDBP;ZeroMemory(&infoGAMEDBP, sizeof(infoGAMEDBP));
    SERVER_INFO infoAGENT;  ZeroMemory(&infoAGENT, sizeof(infoAGENT));
    SERVER_INFO infoGUILD;  ZeroMemory(&infoGUILD, sizeof(infoGUILD));
    SERVER_INFO infoPORTAL; ZeroMemory(&infoPORTAL, sizeof(infoPORTAL));
    SERVER_INFO infoSHOP;   ZeroMemory(&infoSHOP, sizeof(infoSHOP));

	ServerInfoManager::GetGameDBProxyInfo( byWorldID, byChannelID, &infoGAMEDBP );
	ServerInfoManager::GetAgentServerInfo( byWorldID, byChannelID, &infoAGENT );
	ServerInfoManager::GetGuildServerInfo( byWorldID, byChannelID, &infoGUILD );
	ServerInfoManager::GetPortalServerInfo( byWorldID, byChannelID, &infoPORTAL );
	ServerInfoManager::GetShopServerInfo( &infoSHOP );

	SERVER_INFO* pOwnServerInfo = ServerInfoManager::GetServerInfo( key );
	ASSERT( pOwnServerInfo );

	//---------------------------------------------------------------------------------------------------------
	{	// SERVERINFO Packet
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_GM_CONNECTION_SERVER_INFO_CMD* pMsgCMD =
			new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_GM_CONNECTION_SERVER_INFO_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_GM_CONNECTION_SERVER_INFO_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		pMsgCMD->m_byCategory	= GM_CONNECTION;
		pMsgCMD->m_byProtocol	= GM_CONNECTION_SERVER_INFO_CMD;
		pMsgCMD->m_ServerKey	= key;
		pMsgCMD->m_dwServerGUID	= pOwnServerInfo->dwServerGUID;

		memcpy( pMsgCMD->m_szDBProxyServerIP, infoGAMEDBP.szInnerIP, sizeof(pMsgCMD->m_szDBProxyServerIP) );
		pMsgCMD->m_wDBProxyServerPort	= infoGAMEDBP.wInnerPort;

		memcpy( pMsgCMD->m_szAgentServerIP, infoAGENT.szInnerIP, sizeof(pMsgCMD->m_szAgentServerIP) );
		pMsgCMD->m_wAgentServerPort		= infoAGENT.wInnerPort;

		memcpy( pMsgCMD->m_szGuildServerIP, infoGUILD.szInnerIP, sizeof(pMsgCMD->m_szGuildServerIP) );
		pMsgCMD->m_wGuildServerPort		= infoGUILD.wInnerPort;

		memcpy( pMsgCMD->m_szPortalServerIP, infoPORTAL.szInnerIP, sizeof(pMsgCMD->m_szPortalServerIP) );
		pMsgCMD->m_wPortalServerPort	= infoPORTAL.wInnerPort;

		memcpy( pMsgCMD->m_szShopServerIP, infoSHOP.szInnerIP, sizeof(pMsgCMD->m_szShopServerIP) );
		pMsgCMD->m_wShopServerPort	= infoSHOP.wInnerPort;

		Send( (BYTE*)pMsgCMD, sizeof(MSG_GM_CONNECTION_SERVER_INFO_CMD) );
	}

	//---------------------------------------------------------------------------------------------------------
	{	// ���� ��ɵ� ���� ������ �����ش�
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_GM_OPERATION_FUNCRESTRICT_CMD* pMsgCMD =
			new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_GM_OPERATION_FUNCRESTRICT_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_GM_OPERATION_FUNCRESTRICT_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
		sRESTRICT_MGR::MakePacket::Make_GM_OPERATION_FUNCRESTRICT_CMD( &pMasterServer->RestrictManager(), pMsgCMD );

		// ���� �־�� �����ش�
		if( pMsgCMD->byRestrictNum )
		{
			Send( (BYTE*)pMsgCMD, pMsgCMD->GetSize() );
		}
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// ���� ������ �ٲٱ⿡�� �ð��� �����ϴ�. �ϴ� �̷��� �����ϱ�� ����
	//if( !ScriptDataManager::GetNeedLoadDataCount() )
	//{
	//	MSG_GM_CONNECTION_PHASE_SHIFT_CMD msgCMD;
	//	msgCMD.m_dwPHASE_SHIFT = PHASE_EXCHANGE;
	//	Send( (BYTE*)&msgCMD, sizeof(msgCMD) );
	//}
#endif

    // CHANGES: f110617.1L
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}


