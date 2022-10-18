#include "stdafx.h"
#include "AccountDBProxySession.h"

#include "PacketStruct_MX.h"

#include <MasterServerEx.h>


// CHANGES: f110617.1L
void AccountDBProxySession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

VOID	AccountDBProxySession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
		pMasterServer->ConnectSessions.SessionAccountDB() = this;

#ifndef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
		SendFunc::RequestServerInfo( this );
#else //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
		if( !pMasterServer->IsRestrictFuncFromFile() )
		{
			// AccountDB로 기능제한 정보 요청
			pMasterServer->GetMasterServerInfo().SendMX_FUNCRESTRICT_RELOAD_REQ( pMasterServer );
		}
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	}
	else
	{
		static BOOL bMessageDisplayed = FALSE;
		if( !bMessageDisplayed )
		{
			MessageOut( eCRITICAL_LOG, "Account DB proxy와 연결 실패! 연결 후에 리슨합니다." );
			bMessageDisplayed = TRUE;
		}
	}
}

VOID	AccountDBProxySession::OnDisconnect()
{
	ServerSessionEx::OnDisconnect();

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	pMasterServer->ConnectSessions.SessionAccountDB() = NULL;
}


// SendFunc

VOID	AccountDBProxySession::SendFunc::RequestServerInfo( AccountDBProxySession* pServerSession )
{
	MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN msgSYN;
	msgSYN.m_byCategory	= MX_CONNECTION;
	msgSYN.m_byProtocol	= MX_CONNECTION_REQ_SERVER_INFO_SYN;

	pServerSession->Send( (BYTE*)&msgSYN, sizeof(MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN) );

	MessageOut( eCRITICAL_LOG, _T("Request Server Information...") );
}

