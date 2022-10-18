#include "stdafx.h"
#include "PortalServerSession.h"
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#include <PacketHandler/Handler_MP.h>

#include <Function/ServerInfoManager.h>
#include "ServerSessionManager.h"

VOID PortalServerSession::OnRedirect()
{
	// 길드 서버에 필요한 서버 정보를 보내준다.
	SERVER_KEY key		= GetServerKey();
	BYTE byWorldID		= key.GetWorldID();
	BYTE byChannelID	= key.GetChannelID();

	SERVER_INFO* pOwnServerInfo = ServerInfoManager::GetServerInfo( key );
	ASSERT(pOwnServerInfo);

	SERVER_INFO infoPORTAL;
	SERVER_INFO infoGAMEDBP;

	ServerInfoManager::GetPortalServerInfo( byWorldID, byChannelID, &infoPORTAL );
	ServerInfoManager::GetGameDBProxyInfo( byWorldID, byChannelID, &infoGAMEDBP );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MP_CONNECTION_SERVER_INFO_CMD* pMsgCMD =
		new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MP_CONNECTION_SERVER_INFO_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MP_CONNECTION_SERVER_INFO_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	pMsgCMD->m_ServerKey				= key;
	pMsgCMD->m_dwServerGUID				= pOwnServerInfo->dwServerGUID;

	memcpy( pMsgCMD->m_szPortalServerInnerIP, infoPORTAL.szInnerIP, sizeof(pMsgCMD->m_szPortalServerInnerIP) );
	pMsgCMD->m_wPortalServerInnerPort	= infoPORTAL.wInnerPort;
	memcpy( pMsgCMD->m_szDBProxyServerIP, infoGAMEDBP.szInnerIP, sizeof(pMsgCMD->m_szDBProxyServerIP) );
	pMsgCMD->m_wDBProxyServerPort		= infoGAMEDBP.wInnerPort;

	Send( (BYTE*)pMsgCMD, sizeof(MSG_MP_CONNECTION_SERVER_INFO_CMD) );

	ServerSessionEx::OnRedirect();
}

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)