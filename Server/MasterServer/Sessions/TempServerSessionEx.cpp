#include "stdafx.h"
#include "TempServerSessionEx.h"
#include <ServerFrame.h>

#include <MasterServerEx.h>
#include "ServerSessionManager.h"

VOID	TempServerSessionEx::_Initialize()
{
	TempServerSession::Init();

	Services().RegisterPacketHandler();
	Services().Init();
	m_dwPacketStrictSequence = 0;
}

VOID	TempServerSessionEx::_Release()
{
	TempServerSession::Release();
	Services().Init();
	m_dwPacketStrictSequence = 0;
}

VOID	TempServerSessionEx::ReleaseProcess( TempServerSessionEx* pTempSession )
{
	// TempServerSession 제거
	ServerFrame* pFRAME = pTempSession->GetFrame();
	pFRAME->RemoveSession( pTempSession->GetSessionIndex() );
	pTempSession->Release();
	ServerSessionManager::FreeSession( pTempSession );
}

VOID	TempServerSessionEx::Redirect2NewSession( DWORD dwIP, SERVER_KEY skKey )
{
    __TOUCHED((dwIP, skKey));
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	sSIG_SERVER_AUTHENTICATION sAUTH;
	sAUTH.m_dwSubType = sSIG_SERVER_AUTHENTICATION::eSUBTYPE::AUTHED_RELEASE;
	sAUTH.m_dwInnerIP = dwIP;
	sAUTH.m_dwServerKey = (DWORD)skKey;

	WorkLoadManager::OnMsg( &sAUTH, MasterServerEx::GetMainFrame()->GetAuthProcessWorkLoadKey() );

#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

#include <PacketStruct_ServerCommon.h>
VOID	TempServerSessionEx::OnRecv( BYTE* pMsg, WORD wSize )
{
	// 패킷 제어가 필요하다.
MSG_BASE* pMSG = (MSG_BASE*)pMsg;
//if( m_dwPacketStrictSequence == 0 )
//{	// 기존 SERVERTYPE 패킷이 정상 처리되도록 강제한다. 패킷 플로는 동일하므로,
//	// 하지만 앞으로는 TempServer레벨에서 게임서버 인증을 수행할 예정이므로
//	pMSG->m_byCategory = SERVERCOMMON;
//	pMSG->m_byProtocol = SERVERCOMMON_SERVERTYPE;
//	m_dwPacketStrictSequence++;
//}

__WAVERIX_SOLAR_AUTH_DEBUG_CODE( \
	if( !(pMSG->m_byCategory == 0 && pMSG->m_byProtocol == 1) )	\
		printf( "TempServerSessionEx::OnRecv [%03u:%03u]\n", pMSG->m_byCategory, pMSG->m_byProtocol ); );

	Services().GetPacketHandler()->ParsePacket( TEMP_SERVER, this, (MSG_BASE*)pMsg, wSize );
}


BOOL	TempServerSessionEx::SendPacket( MSG_BASE* pMsg, WORD wSize )
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "TempServerSessionEx::SendPacket\n" ); );
	return Send( (BYTE*)pMsg, wSize );
}
