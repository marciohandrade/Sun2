#include "stdafx.h"
#include "OpServerSession.h"

#include <Sessions/ServerSessionManager.h>
#include <MasterServerEx.h>


// CHANGES: f110617.1L
void OpServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

VOID	OpServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
		pMasterServer->ConnectSessions.SessionOpServer() = this;

		// 서버 타입 전송
		MSG_MO_RTTG_CERTIFY_REQ msgCertREQ;
		msgCertREQ.dwType = SERVERTYPE_MASTER;
		Send( (BYTE*)&msgCertREQ, sizeof(msgCertREQ) );

		// GM 리스트 요청
		MSG_MO_RTTG_GAMEADMINLIST_REQ msgGM_REQ;
		Send( (BYTE*)&msgGM_REQ, sizeof(msgGM_REQ) );

		OpServerSession::SendFunc::AllServerInfo_OnLIVE( this );
	}
	else
	{
	}
}

VOID	OpServerSession::OnDisconnect()
{
	ServerSessionEx::OnDisconnect();

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	pMasterServer->ConnectSessions.SessionOpServer() = NULL;
}

VOID	OpServerSession::OnRecv( BYTE* pMsg, WORD wSize )
{
	eSERVER_TYPE serverType = this->GetServerType();
	MSG_BASE_INTERNAL* pMSG = (MSG_BASE_INTERNAL*)pMsg;

	//__WAVERIX_SOLAR_AUTH_DEBUG_CODE(	\
	//	if( !(pMSG->m_byCategory == 0 && pMSG->m_byProtocol == 1) )	\
	//		printf( "[%02u]::OnRecv [%03u:%03u]\n", serverType, pMSG->m_byCategory, pMSG->m_byProtocol ); );
	//printf( "[%02u]::OnRecv [%03u:%03u]\n", serverType, pMSG->m_byCategory, pMSG->m_byProtocol );

	Services().GetPacketHandler()->ParsePacket( serverType, this, (MSG_BASE_INTERNAL*)pMsg, wSize );
}


VOID	OpServerSession::SendFunc::AllServerInfo_OnLIVE( OpServerSession* pOpServer )
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_ALL_SERVER_STATUS_NTF* pMsgNTF =
		new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_ALL_SERVER_STATUS_NTF;
#else
	MSG_MO_RTTG_ALL_SERVER_STATUS_NTF pMsgNTF[1];
#endif __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	DWORD dwUpperBound;
	sSERVER_NODE* pNODEs = ServerSessionManager::GetServerTree()->GetServerArrayList( dwUpperBound );

	const DWORD cdwExcept = (DWORD)( (1<<SOLARAUTH_SERVER) | (1<<OP_SERVER) | (1<<UNKNOWN_SERVER) );
	DWORD dwCounts = 0;
	sSERVER_NODE* pNODE = pNODEs;

	for( DWORD dwInc=0 ; dwInc<dwUpperBound ; ++dwInc, ++pNODE )
	{
		DWORD dwSType = (DWORD)(1<<pNODE->m_ServerType);
		if( cdwExcept & dwSType)
			continue;

		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF::ServerConnectInfo& rConnectInfo =
			pMsgNTF->m_ServerConnectInfo[dwCounts];

		SERVER_KEY skServerKey = pNODE->m_ServerKey;
		SERVER_KEY skOldServerKey = SWAP_32BIT_DATATYPE((DWORD)skServerKey);
		rConnectInfo.m_ServerKey = skOldServerKey;
		rConnectInfo.m_bConnected = (BYTE)TRUE;
		++dwCounts;
	}

	pMsgNTF->m_Count = (BYTE)dwCounts;
	pOpServer->Send( (BYTE*)pMsgNTF, (WORD)pMsgNTF->GetSize() );
}

