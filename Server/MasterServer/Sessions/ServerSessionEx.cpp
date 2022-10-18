#include "stdafx.h"

#include <MasterServerEx.h>
#include "ServerSessionEx.h"
#include <Sessions/ServerSessionManager.h>

//-------------------------------------------------------------------------------------------------
// Init & Release
VOID	ServerSessionEx::_Initialize()
{
	ServerSession::Init();
	Services().RegisterPacketHandler();
	Services().Init();
}

VOID	ServerSessionEx::_Release()
{
	ServerSession::Release();
	Services().Init();
}

// CHANGES: f110617.1L
void ServerSessionEx::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    Services().SetSyncKey(GetServerType());
#endif //
    // �ڵ尡 �ټ� �ߺ��Ǵ� �κ��� �����. ������ �ذ�����.
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID	ServerSessionEx::OnDisconnect()
{
	// �̰��� ��� Work���� �����ϴ� ���̹Ƿ� �ùٸ� ��ġ�� ������ ���� ���̴�.
	//WorkLoadManager::ReleaseTaskAll();
	Services().GetWorkLoadPacketStream().Initialize(
											WorkLoadPacketStream::c_DefaultUpdateTick,
											WorkLoadPacketStream::c_DefaultLimitTransferSize
											);
	Services().GetWorkLoadContainer().Release( TRUE );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	Services().SetSyncKey( TEMP_SERVER );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	MasterServerInfo& rINFO = pMasterServer->GetMasterServerInfo();

	ServerSessionManager::RemoveServer( GetServerKey() );

	if( rINFO.IsTransactionExitProcess() )
	{
		WORK_KEY wkKey = rINFO.GetWorkKey4ExitProcess();
		sSIG_EXITED_SESSION msgSIGNAL;
		msgSIGNAL.Session() = this;
		WorkLoadManager::OnMsg( &msgSIGNAL, wkKey );
	}

	ServerSession::OnDisconnect();
}

VOID	ServerSessionEx::OnRecv( BYTE* pMsg, WORD wSize )
{
	eSERVER_TYPE serverType = this->GetServerType();
	MSG_BASE* pMSG = (MSG_BASE*)pMsg;

	__WAVERIX_SOLAR_AUTH_DEBUG_CODE(	\
		if( !(pMSG->m_byCategory == 0 && pMSG->m_byProtocol == 1) )	\
			printf( "[%02u]::OnRecv [%03u:%03u]\n", serverType, pMSG->m_byCategory, pMSG->m_byProtocol ); );

	Services().GetPacketHandler()->ParsePacket( serverType, this, (MSG_BASE*)pMsg, wSize );
}

