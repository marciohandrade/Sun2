#include "stdafx.h"
#include "WorldServerSession.h"
#include <PacketHandler/Handler_MW.h>

#include <Function/ServerInfoManager.h>
#include "ServerSessionManager.h"


// CHANGES: f110617.1L
void WorldServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
	// 월드 서버에 필요한 서버 정보를 보내준다.
	SERVER_KEY key		= GetServerKey();
	BYTE byWorldID		= key.GetWorldID();
	BYTE byChannelID	= key.GetChannelID();

	SERVER_INFO infoWORLD;
	SERVER_INFO infoGAMEDBP;

	ServerInfoManager::GetWorldServerInfo( byWorldID, byChannelID, &infoWORLD );
	ServerInfoManager::GetGameDBProxyInfo( byWorldID, byChannelID, &infoGAMEDBP );

	{	// 서버 정보 패킷
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_MW_SERVER_INFO_CMD* pMsgCMD =
			new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MW_SERVER_INFO_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_MW_SERVER_INFO_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		pMsgCMD->m_byCategory			= MW_CONNECTION;
		pMsgCMD->m_byProtocol			= MW_SERVER_INFO_CMD;
		pMsgCMD->ServerKey				= key;

		memcpy( pMsgCMD->m_szWorldServerIP, infoWORLD.szIP, sizeof(pMsgCMD->m_szWorldServerIP) );
		pMsgCMD->m_wWorldServerPort			= infoWORLD.wPort;

		memcpy( pMsgCMD->m_szWorldServerInnerIP, infoWORLD.szInnerIP, sizeof(pMsgCMD->m_szWorldServerInnerIP) );
		pMsgCMD->m_wWorldServerInnerPort	= infoWORLD.wInnerPort;

		memcpy( pMsgCMD->m_szGameDBProxyServerIP, infoGAMEDBP.szInnerIP, sizeof(pMsgCMD->m_szGameDBProxyServerIP) );
		pMsgCMD->m_wGameDBProxyServerPort	= infoGAMEDBP.wInnerPort;

		Send( (BYTE*)pMsgCMD, sizeof(MSG_MW_SERVER_INFO_CMD) );
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 지금 구조를 바꾸기에는 시간이 빠듯하다. 일단 이렇게 수행하기로 하자
	//if( !ScriptDataManager::GetNeedLoadDataCount() )
	//{
	//	MSG_MW_CONNECTION_PHASE_SHIFT_CMD msgCMD;
	//	msgCMD.m_dwPHASE_SHIFT = PHASE_EXCHANGE;
	//	Send( (BYTE*)&msgCMD, sizeof(msgCMD) );
	//}
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

    // CHANGES: f110617.1L
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}
