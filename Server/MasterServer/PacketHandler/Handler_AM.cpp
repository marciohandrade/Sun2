#include "stdafx.h"
#include "Handler_AM.h"
#include "PacketHandler.Shared.h"
#include "Function/AbuseUserManager.h"

#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_AM::On##p }


sPACKET_HANDLER_INFO*	Handler_AM::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
        ADD_HANDLER_INFO(AM_CONNECTION, AM_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(AM_OPERATION, AM_OPERATION_USERCOUNT_SYN, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AM_ARMORKIT, AM_ARMORKIT_FINDHACK_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AM_OPERATION, AM_OPERATION_CHAR_DISCONNECT_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AM_CONNECTION, AM_CONNECTION_CCU_REPORT_NTF, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AM_CONNECTION, AM_CONNECTION_ENTERSERVER_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_AM::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<AGENT_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<AGENT_SERVER>( PacketHandlerShared::GetHandlerInfo() );
	pInstance->RegisterHandler_<AGENT_SERVER>( PacketHandlerShared::GetDataHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>

#include <Sessions/AgentServerSession.h>
#include <Sessions/OpServerSession.h>

#ifdef _DEBUG
#	include <Armorkit.h>
#endif

HANDLER_AM_IMPL(AM_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_AM_IMPL(AM_ARMORKIT_FINDHACK_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HANDLER_AM_IMPL(AM_OPERATION_CHAR_DISCONNECT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_OPERATION_CHAR_DISCONNECT_ACK* pRecvMsg = (MSG_AM_OPERATION_CHAR_DISCONNECT_ACK*)pMsg;

	MessageOut( eCRITICAL_LOG, _T("Agent User Disconnect [GUID:%u] Complete"), pRecvMsg->m_dwUserGuid );
}

HANDLER_AM_IMPL(AM_CONNECTION_CCU_REPORT_NTF)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_CONNECTION_CCU_REPORT_NTF* pRecvMsg = (MSG_AM_CONNECTION_CCU_REPORT_NTF*)pMsg;

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pSession = pMasterServer->ConnectSessions.SessionOpServer();

	BOOL bSended = FALSE;
	SERVER_KEY skKEY = pServerSession->GetServerKey();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_USERCOUNT_NTF* pMsgNTF = new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_USERCOUNT_NTF();
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_USERCOUNT_NTF pMsgNTF[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	pMsgNTF->dwWorldID      = skKEY.GetWorldID();
	pMsgNTF->dwChannelID    = skKEY.GetChannelID();
    //////////////////////////////////////////////////////////////////////////
	pMsgNTF->dwVillageCount             = pRecvMsg->dwVillageCount;
	pMsgNTF->dwVillagePCRoomCount       = pRecvMsg->dwVillageCountPC;
	pMsgNTF->dwBattleZoneCount          = pRecvMsg->dwBattleZoneCount;
	pMsgNTF->dwBattleZonePCRoomCount    = pRecvMsg->dwBattleZoneCountPC;
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    pMsgNTF->numberOfInVillageHangame       = pRecvMsg->numberOfInVillageHangame;
    pMsgNTF->numberOfInVillageHangamePC     = pRecvMsg->numberOfInVillageHangamePC;
    pMsgNTF->numberOfInBattleZoneHangame    = pRecvMsg->numberOfInBattleZoneHangame;
    pMsgNTF->numberOfInBattleZoneHangamePC  = pRecvMsg->numberOfInBattleZoneHangamePC;
#endif
#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
    pMsgNTF->numberOfInVillageNeowiz       = pRecvMsg->numberOfInVillageNeowiz;
    pMsgNTF->numberOfInVillageNeowizPC     = pRecvMsg->numberOfInVillageNeowizPC;
    pMsgNTF->numberOfInBattleZoneNeowiz    = pRecvMsg->numberOfInBattleZoneNeowiz;
    pMsgNTF->numberOfInBattleZoneNeowizPC  = pRecvMsg->numberOfInBattleZoneNeowizPC;
#endif

	if(pSession && pSession->IsConnected())
	{
		pSession->Send((BYTE*)pMsgNTF, sizeof(MSG_MO_RTTG_USERCOUNT_NTF));
		bSended = TRUE;
	}

	pServerSession->CCUReport(pRecvMsg);

#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
    #if !defined(_CHANNELING_AUTH_FLOW_HANGAME_USER_)
        #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
                 "Report CCU to RTTA : W(%u)C(%u)V(%u/%u/%u/%u/%u/%u)B(%u/%u/%u/%u/%u/%u) - SEND RET {%s}",
                 pMsgNTF->dwWorldID, pMsgNTF->dwChannelID, 
                 pMsgNTF->dwVillageCount,               pMsgNTF->dwVillagePCRoomCount,
                 pMsgNTF->numberOfInVillageHangame,     pMsgNTF->numberOfInVillageHangamePC,
                 pMsgNTF->numberOfInVillageNeowiz,      pMsgNTF->numberOfInVillageNeowizPC,
                 pMsgNTF->dwBattleZoneCount,            pMsgNTF->dwBattleZonePCRoomCount,
                 pMsgNTF->numberOfInBattleZoneHangame,  pMsgNTF->numberOfInBattleZoneHangamePC,
                 pMsgNTF->numberOfInBattleZoneNeowiz,   pMsgNTF->numberOfInBattleZoneNeowizPC,
                 bSended ? _T("TRUE") : _T("FALSE"));
#else

  #ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
                 "Report CCU to RTTA : W(%u)C(%u)V(%u/%u/%u/%u)B(%u/%u/%u/%u) - SEND RET {%s}",
                 pMsgNTF->dwWorldID, pMsgNTF->dwChannelID, 
                 pMsgNTF->dwVillageCount,               pMsgNTF->dwVillagePCRoomCount,
                 pMsgNTF->numberOfInVillageHangame,     pMsgNTF->numberOfInVillageHangamePC,
                 pMsgNTF->dwBattleZoneCount,            pMsgNTF->dwBattleZonePCRoomCount,
                 pMsgNTF->numberOfInBattleZoneHangame,  pMsgNTF->numberOfInBattleZoneHangamePC,
                 bSended ? _T("TRUE") : _T("FALSE"));
  #else //!_CHANNELING_AUTH_FLOW_HANGAME_USER_
	MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
                 _T("Report CCU to RTTA : W(%u)C(%u)V(%u/%u)B(%u/%u) - SEND RET {%s}"),
                 pMsgNTF->dwWorldID, pMsgNTF->dwChannelID, 
                 pMsgNTF->dwVillageCount,    pMsgNTF->dwVillagePCRoomCount,
                 pMsgNTF->dwBattleZoneCount, pMsgNTF->dwBattleZonePCRoomCount,
                 bSended ? _T("TRUE") : _T("FALSE"));
  #endif //!_CHANNELING_AUTH_FLOW_HANGAME_USER_

#endif
}

HANDLER_AM_IMPL(AM_CONNECTION_ENTERSERVER_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_CONNECTION_ENTERSERVER_CMD* pRecvMsg = (MSG_AM_CONNECTION_ENTERSERVER_CMD*)pMsg;

	AbuseUserManager::Instance()->EnterServer( pRecvMsg->m_UserGuid, pRecvMsg->m_pUserID, pRecvMsg->m_IP );
}









