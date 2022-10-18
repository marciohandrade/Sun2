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

		// 서버 정보 테이블에서 IP와 서버 타입으로 서버를 찾아서 그 키를 세팅한다
		SERVER_KEY skServerKey = ServerInfoManager::GetServerKey( serverType, pTempSession->GetIP() );
		if( 0 == skServerKey )
		{
			_sntprintf( pErrMsg, sizeof(TCHAR)*1024,
				_T("DB에 IP(%s) ServerType(%d)에 해당하는 서버가 없으므로 연결을 끊습니다."),
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

		// 새로운 NetworkObject로 교체
		pTempSession->Redirect( pNewSession );
		TempServerSessionEx::ReleaseProcess( pTempSession );

		pNewSession->OnRedirect(pNewSession->GetSessionIndex(), pMsgTYPE);
		// 실제 사용할 세션을 매니져에 추가
		ServerSessionManager::AddServer( pNewSession );
		MessageOut( eCRITICAL_LOG,
			_T("Add Server (%u/%u/%u/%u)"),
			skServerKey.GetWorldID(), 
			skServerKey.GetChannelID(),
			skServerKey.GetServerType(),
			skServerKey.GetServerID() );

// 이 시점에 pNewSession이 역할을 대체하게 될 것이다.
// && 기존 코드에 의한다면, ScriptLoadManager를 통해 현재 마스터 서버가 PHASE_SERVICE 상태일 경우,
// 게임서버들의 정상적인 동작을 수행할 수 있도록 명령을 내리게 되어 있는데...
// 변경을 감안하자. 위의 것도 마찬가지다.
// 아래 __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__ 으로 구현된 스크립트 로딩 명령 부분을 옮기도록 하자.

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
















