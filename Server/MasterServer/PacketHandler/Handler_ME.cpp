#include "stdafx.h"
#include "Handler_ME.h"
#include "PacketHandler.Shared.h"


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_ME::On##p }


sPACKET_HANDLER_INFO*	Handler_ME::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERTYPE, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_ME::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<TEMP_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<TEMP_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>
#include <Sessions/ServerSessionManager.h>
#include <Sessions/ServerSessionEx.h>



HANDLER_ME_IMPL(SERVERCOMMON_SERVERTYPE)
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "[RECEIVED] SERVERCOMMON_SERVERTYPE\n" ); );

	TCHAR pErrMsg[1024];
	pErrMsg[0] = _T('\0');

	MSG_SERVER_TYPE* pMsgTYPE = (MSG_SERVER_TYPE *)pMsg;
	TempServerSessionEx* pTempSession = (TempServerSessionEx*)pServerSession;

	eSERVER_TYPE serverType = (eSERVER_TYPE)pMsgTYPE->m_byServerType;

	do
	{
		if( (DWORD)serverType > (DWORD)MAX_SERVER )
		{
			_sntprintf( pErrMsg, sizeof(TCHAR)*1024, _T("Invalid ServerT") );
			break;
		}

		// ���� ���� ���̺��� IP�� ���� Ÿ������ ������ ã�Ƽ� �� Ű�� �����Ѵ�
		SERVER_KEY skServerKey = ServerInfoManager::GetServerKey( serverType, pTempSession->GetIP() );
		if( 0 == skServerKey )
		{
			_sntprintf( pErrMsg, sizeof(TCHAR)*1024,
				_T("DB�� IP(%s) ServerType(%d)�� �ش��ϴ� ������ �����Ƿ� ������ �����ϴ�."),
				pTempSession->GetIP(), serverType );

			break;
		}

		pTempSession->TempServerKey() = skServerKey;

		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

#ifndef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__

		ServerSessionEx* pNewSession = (ServerSessionEx*)ServerSessionManager::AllocNewSession( serverType );
		((ServerSession*)pNewSession)->Init( pMasterServer );

		// Copy Session Information
		pNewSession->SetSessionIndex( pTempSession->GetSessionIndex() );
		pNewSession->SetServerKey( skServerKey );

		// ���ο� NetworkObject�� ��ü
		pTempSession->Redirect( pNewSession );
		TempServerSessionEx::ReleaseProcess( pTempSession );

		pNewSession->OnRedirect(pNewSession->GetSessionIndex(), pMsgTYPE);
		// ���� ����� ������ �Ŵ����� �߰�
		ServerSessionManager::AddServer( pNewSession );
		MessageOut( eCRITICAL_LOG,
			_T("Add Server (%u/%u/%u/%u)"),
			skServerKey.GetWorldID(), 
			skServerKey.GetChannelID(),
			skServerKey.GetServerType(),
			skServerKey.GetServerID() );

// �� ������ pNewSession�� ������ ��ü�ϰ� �� ���̴�.
// && ���� �ڵ忡 ���Ѵٸ�, ScriptLoadManager�� ���� ���� ������ ������ PHASE_SERVICE ������ ���,
// ���Ӽ������� �������� ������ ������ �� �ֵ��� ����� ������ �Ǿ� �ִµ�...
// ������ ��������. ���� �͵� ����������.
// �Ʒ� __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__ ���� ������ ��ũ��Ʈ �ε� ��� �κ��� �ű⵵�� ����.

#	ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		if( ScriptDataManager::IsAllDataLoaded() )
		{
			if( pMasterServer->GetPhaseStatus() == PHASE_SERVICE )
			{
				Services4Session& rSERVICE = pNewSession->Services();
				rSERVICE.SetPhaseStatus( PHASE_EXCHANGE );
				ServerSessionManager::SEND_PHASE_SHIFT_CMD( pNewSession, PHASE_EXCHANGE, PHASE_NOACTION );
			}
		}
#	endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#else // __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__

		MessageOut( eCRITICAL_LOG,
			_T("Request Server Authentication (%u/%u/%u/%u)"),
			skServerKey.GetWorldID(), 
			skServerKey.GetChannelID(),
			skServerKey.GetServerType(),
			skServerKey.GetServerID() );

		sSIG_SERVER_AUTHENTICATION sAUTH;
		sAUTH.m_dwSubType = sSIG_SERVER_AUTHENTICATION::eSUBTYPE::REGISTERATION;
		sAUTH.m_pServerSession = pTempSession;

		WorkLoadManager::OnMsg( &sAUTH, pMasterServer->GetAuthProcessWorkLoadKey() );

		SolarAuthServerSession* pSolarAuth = pMasterServer->ConnectSessions.SessionSolarAuth();
		if( pSolarAuth && pSolarAuth->IsConnected() )
		{
			MSG_SM_SERVER_INFO_AUTHENTICATION_SYN msgSYN;
			msgSYN.m_dwInnerIP = pServerSession->GetIPAddress();
			msgSYN.m_ServerKey = pTempSession->TempServerKey();
			pSolarAuth->SendPacket( &msgSYN, sizeof(MSG_SM_SERVER_INFO_AUTHENTICATION_SYN) );
		}
		
		
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__

		return;

	} while( 1 );

	MessageOut( eCRITICAL_LOG, pErrMsg );
	pTempSession->Disconnect();
}
















