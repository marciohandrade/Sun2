#include "stdafx.h"
#include "GameDBProxySession.h"
#include <PacketHandler/Handler_DM.h>

#include <Function/ServerInfoManager.h>
#include "ServerSessionManager.h"

// CHANGES: f110617.1L
void GameDBProxySession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
	// 접속한 게임 디비 프락시의 리슨 정보를 보내준다.
	SERVER_KEY key = GetServerKey();
	BYTE byWorldId = key.GetWorldID();
	BYTE byChannelID = key.GetChannelID();

    SERVER_INFO infoGAMEDBP;

	ServerInfoManager::GetGameDBProxyInfo( byWorldId, byChannelID, &infoGAMEDBP );

	MSG_DM_CONNECTION_SERVER_INFO_CMD pMsgCMD[1];

	pMsgCMD->m_byCategory			= DM_CONNECTION;
	pMsgCMD->m_byProtocol			= DM_CONNECTION_SERVER_INFO_CMD;
	pMsgCMD->m_ServerKey			= key;
	pMsgCMD->m_dwServerGUID			= infoGAMEDBP.dwServerGUID;

	memcpy( pMsgCMD->m_szDBProxyServerIP, infoGAMEDBP.szInnerIP, sizeof(pMsgCMD->m_szDBProxyServerIP) );
	pMsgCMD->m_wDBProxyServerPort	= infoGAMEDBP.wInnerPort;

	Send( (BYTE*)pMsgCMD, sizeof(MSG_DM_CONNECTION_SERVER_INFO_CMD) );

    // CHANGES: f110617.1L
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

