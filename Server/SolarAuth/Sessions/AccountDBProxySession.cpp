#include "stdafx.h"
#include "AccountDBProxySession.h"

#include "PacketStruct_SX.h"

#include <SolarAuthServer.h>

// CHANGES: f110617.1L
void AccountDBProxySession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID	AccountDBProxySession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		SolarAuthServer::GetMainFrame()->AccountDBP() = this;

		SendFunc::RequestServerInfo( this );
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

	SolarAuthServer::GetMainFrame()->AccountDBP() = NULL;
}


// SendFunc

VOID	AccountDBProxySession::SendFunc::RequestServerInfo( AccountDBProxySession* pServerSession )
{
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MSG_SX_CONNECTION_REQ_SERVER_INFO_SYN msgSYN;
	msgSYN.m_byCategory	= SX_CONNECTION;
	msgSYN.m_byProtocol	= SX_CONNECTION_REQ_SERVER_INFO_SYN;

	pServerSession->Send( (BYTE*)&msgSYN, sizeof(MSG_SX_CONNECTION_REQ_SERVER_INFO_SYN) );

	MessageOut( eCRITICAL_LOG, _T("Request Server Information...") );
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

