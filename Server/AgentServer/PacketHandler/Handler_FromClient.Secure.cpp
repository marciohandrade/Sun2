#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_CG.h>

//////////////////////////////////////////////////////////////////////////
#include <PacketHandler/Handler_FromClient.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#include <TimeCheck/HeartbeatChecker.h>


HANDLER_CG_IMPL( CG_CONNECTION_HEARTBEAT_SYN )
{
    __TOUCHED((pMsg, wSize));
	DWORD dwCurTick = GetTickCount();
	HeartbeatChecker& rChecker = pUser->GetHeartbeatChecker();

	BOOL bIsValidSize = (BOOL)( sizeof(MSG_CG_CONNECTION_HEARTBEAT_SYN) == wSize );

	rChecker.Signal( pUser, dwCurTick );

	// 섣부르게 하지 말자. 일단 크기 검사만 수행하는 걸로 하자. 나중에 기능 개선용으로 사용하자.
	if( !bIsValidSize )
	{
		SUNLOG( eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] HEARTBEAT") );
		//SecurePackNode4User& rCRYPT = pUser->GetSecureNode();
		//DWORD dwMaxDelay = RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE;
		//DWORD dwRelative = RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY;
		//rCRYPT.m_dwTimeout = RandomTimeUtil::GetRandomTimeDelay_ByMinMaxBoundary( dwCurTick, dwMaxDelay, dwRelative );
		//RemoveCryptedUser( pUser );
		//AddCryptingUser( pUser, sSECURE_CONTROL_STATUS::CRYPTING );
	}

	//SUNLOG( eCRITICAL_LOG, _T("HBChecker Is NULL") );
}

#ifdef _NA_20100802_SPEED_HACK_DETECTION
HANDLER_CG_IMPL( CG_CONNECTION_TICK_QUERY_ACK )
{
    __TOUCHED((pMsg, wSize));
    DWORD current_tick = GetTickCount();

    if (TickSpeedChecker::IsGloballyEnabled() == false) { // 전역 활성화 여부 먼저 검사
        return;
    }

	TickSpeedChecker& checker = pUser->GetTickSpeedChecker();

	MSG_CG_CONNECTION_TICK_QUERY_ACK* packet = \
        static_cast<MSG_CG_CONNECTION_TICK_QUERY_ACK*>(pMsg);

    checker.HandleClientResponse(packet->round, packet->index, packet->tick_count, current_tick);
}
#endif

HANDLER_CG_IMPL( CG_CRYPTOGRAPHY_ACK )
{
    __TOUCHED((pMsg, wSize));
	SecurePackNode4User& rSECURE = pUser->GetSecureNode();
	rSECURE.OnRecvCG_CRYPTOGRAPHY_ACK( pUser );

	return;
}

HANDLER_CG_IMPL( CG_CRYPTOGRAPHY_NAK )
{
    __TOUCHED((pMsg, wSize));
	SecurePackNode4User& rSECURE = pUser->GetSecureNode();
	rSECURE.OnRecvCG_CRYPTOGRAPHY_NAK( pUser );

	return;
}




HANDLER_CG_IMPL(CG_SECURE_MOLACONTROL)
{
    __TOUCHED((pMsg, wSize));
	SecurePackNode4User& rSECURE = pUser->GetSecureNode();
	MOLAControl& rMOLA = rSECURE.MOLAController();
	rMOLA.RecvQuery( pUser, pMsg, wSize );
	
}



HANDLER_CG_IMPL( CG_HACKSHIELD_SERVER_EX_QUERY_ACK )
{
    __TOUCHED((pMsg, wSize));
#if USING_HACKSHIELD
	SecurePackNode4User& rSECURE = pUser->GetSecureNode();
	rSECURE.OnRecvCG_HACKSHIELD_SERVER_EX_QUERY_ACK( pUser, (MSG_CG_HACKSHIELD_SERVER_EX_QUERY_ACK*)pMsg, wSize );
#endif
}


HANDLER_CG_IMPL(CG_XIGNCODE_SERVER_QUERY_ACK)
{
    __TOUCHED((pMsg, wSize));
#if USING_XIGNCODE
    SecurePackNode4User& secure_node = pUser->GetSecureNode();
	secure_node.OnRecvXIGNCODE_SERVER_QUERY_ACK(pUser,
        static_cast<MSG_CG_XIGNCODE_SERVER_QUERY_ACK*>(pMsg), wSize);
#endif
}

HANDLER_CG_IMPL(CG_GAMEGUARD_SERVER_QUERY_ACK)
{
    __TOUCHED((pMsg, wSize));
#if USING_GAMEGUARD
    SecurePackNode4User& secure_node = pUser->GetSecureNode();
    secure_node.OnRecv_GAMEGUARD_SERVER_QUERY_ACK(pUser, pMsg, wSize);
#endif // USING_GAMEGUARD
}

HANDLER_CG_IMPL(CG_GAMEGUARD_HACKDATA_CMD)
{
    __TOUCHED((pMsg, wSize));
#if USING_GAMEGUARD
    SecurePackNode4User& secure_node = pUser->GetSecureNode();
    secure_node.OnRecv_GAMEGUARD_HACKDATA_CMD(pUser, pMsg, wSize);
#endif // USING_GAMEGUARD
}

