#include "stdafx.h"
#include "GuildServerSession.h"
#include <PacketHandler/Handler_MZ.h>

#include <Function/ServerInfoManager.h>
#include "ServerSessionManager.h"

// CHANGES: f110617.1L
void GuildServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
	// 길드 서버에 필요한 서버 정보를 보내준다.
	SERVER_KEY key		= GetServerKey();
	BYTE byWorldID		= key.GetWorldID();
	BYTE byChannelID	= key.GetChannelID();

	SERVER_INFO* pOwnServerInfo = ServerInfoManager::GetServerInfo( key );
	ASSERT(pOwnServerInfo);

	SERVER_INFO infoGUILD;
	SERVER_INFO infoGAMEDBP;
	SERVER_INFO infoWORLD;

	ServerInfoManager::GetGuildServerInfo( byWorldID, byChannelID, &infoGUILD );
	ServerInfoManager::GetGameDBProxyInfo( byWorldID, byChannelID, &infoGAMEDBP );
	ServerInfoManager::GetWorldServerInfo( byWorldID, byChannelID, &infoWORLD );


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MZ_SERVER_INFO_CMD* pMsgCMD =
		new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MZ_SERVER_INFO_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MZ_SERVER_INFO_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	pMsgCMD->ServerKey				= key;
	pMsgCMD->m_dwServerGUID			= pOwnServerInfo->dwServerGUID;

	memcpy( pMsgCMD->m_szGuildServerInnerIP, infoGUILD.szInnerIP, sizeof(pMsgCMD->m_szGuildServerInnerIP) );
	pMsgCMD->m_wGuildServerServerInnerPort	= infoGUILD.wInnerPort;

	memcpy( pMsgCMD->m_szDBProxyServerIP, infoGAMEDBP.szInnerIP, sizeof(pMsgCMD->m_szDBProxyServerIP) );
	pMsgCMD->m_wDBProxyServerPort			= infoGAMEDBP.wInnerPort;

	memcpy( pMsgCMD->m_szWorldServerIP, infoWORLD.szInnerIP, sizeof(pMsgCMD->m_szWorldServerIP) );
	pMsgCMD->m_wWorldServerPort				= infoWORLD.wInnerPort;

	Send( (BYTE*)pMsgCMD, sizeof(MSG_MZ_SERVER_INFO_CMD) );

    // CHANGES: f110617.1L
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}