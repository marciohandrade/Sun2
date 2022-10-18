#include "stdafx.h"
#include "Handler_MW.h"
#include "PacketHandler.Shared.h"


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_MW::On##p }


sPACKET_HANDLER_INFO*	Handler_MW::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
        ADD_HANDLER_INFO(MW_CONNECTION, MW_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MW_OPERATION, MW_USERCOUNT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MW_OPERATION, MW_BATTLEZONEINFO_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MW_OPERATION, MW_USERMONEY_SYN, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
	    //__NA001254_20090109_RTTG_FUNC_ADD 
		ADD_HANDLER_INFO(MW_OPERATION, MW_MULTIMEMO_SEND_ACK, PHASE_UNLIMITED),
	    //
		ADD_HANDLER_INFO(MW_ARMORKIT, MW_ARMORKIT_FINDHACK_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_MW::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<WORLD_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<WORLD_SERVER>( PacketHandlerShared::GetHandlerInfo() );
	pInstance->RegisterHandler_<WORLD_SERVER>( PacketHandlerShared::GetDataHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>

#include <PacketStruct_MO.h>
#include <Sessions/ServerSessionManager.h>

HANDLER_MW_IMPL(MW_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_MW_IMPL(MW_USERCOUNT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MW_USERCOUNT_SYN* pMsgSYN = (MSG_MW_USERCOUNT_SYN*)pMsg;
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
    // NOTE: the message from world server do not handle anymore.
    // each agent server report the statistics information.
}

HANDLER_MW_IMPL(MW_BATTLEZONEINFO_ANS)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MW_BATTLEZONEINFO_ANS* pRecvANS = (MSG_MW_BATTLEZONEINFO_ANS*)pMsg;
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pSession = pMasterServer->ConnectSessions.SessionOpServer();

	SERVER_KEY skWorld = pServerSession->GetServerKey();
	BYTE byWorldID = skWorld.GetWorldID();

	OpServerSession* pOpSession = pMasterServer->ConnectSessions.SessionOpServer();

	if( pOpSession && pOpSession->IsConnected() )
	{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_MO_RTTG_BATTLEZONEINFO_ANS* pSendANS = new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_BATTLEZONEINFO_ANS();
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_MO_RTTG_BATTLEZONEINFO_ANS pSendANS[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		DWORD dwCount = (DWORD)pRecvANS->byCount;

		pSendANS->byCount = (BYTE)dwCount;

		STRUCT_BATTLEZONE_INFO* pTarget = pSendANS->ChannelInfo;
		STRUCT_BATTLEZONE_INFO* pSource = pRecvANS->ChannelInfo;

		for( DWORD i=0 ; i<dwCount ; ++i, ++pTarget, ++pSource )
		{
			*pTarget = *pSource;
			pTarget->byWorldID = byWorldID;

			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
				_T("배틀존 상태 : W(%d)C(%d)V(%u)H(%u)M(%u)PVP(%u)"),
				byWorldID,
				pTarget->byChannelID,
				pTarget->dwZoneCount[eZONETYPE_VILLAGE], 
				pTarget->dwZoneCount[eZONETYPE_HUNTING] + pTarget->dwZoneCount[eZONETYPE_CHUNTING], 
				pTarget->dwZoneCount[eZONETYPE_MISSION], 
				pTarget->dwZoneCount[eZONETYPE_PVP] );
		}

		pOpSession->Send( (BYTE*)pSendANS, pSendANS->GetSize() );
	}
	else
	{
		MessageOut( eCRITICAL_LOG, _T("RTTA 배틀존 현황 보고 실패:No Data") );
	}
}

HANDLER_MW_IMPL(MW_USERMONEY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MW_USERMONEY_SYN *pRecvMsg = (MSG_MW_USERMONEY_SYN*)pMsg;
	/*
	MSG_MO_RTTG_USERMONEY_NTF sendMsg;

	sendMsg.dwTotalUserMoney	= pRecvMsg->dwTotalUserMoney;
	sendMsg.
	*/
}

HANDLER_MW_IMPL(MW_ARMORKIT_FINDHACK_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//??
}

//HANDLER_MW_IMPL(SERVERCOMMON_SERVERSHUTDOWN_ANS)
//{
//	//// 종료 대기중인 놈이 종료된 것이므로 체크
//	//if( MasterServer::Instance()->IsExitServer() && MasterServer::Instance()->GetExitType() == eEXIT_GAMEWAIT )
//	//{
//	//	MasterServer::Instance()->RemoveExitServer( pWorldServer->GetServerKey() );	// 종료되었다!
//	//	MessageOut( eCRITICAL_LOG, "World Exit Message Recv[%u]", pWorldServer->GetServerKey() );
//	//}
//}

//{__NA001254_20090109_RTTG_FUNC_ADD
HANDLER_MW_IMPL(MW_MULTIMEMO_SEND_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MW_MULTIMEMO_SEND_ACK *pRecvMsg = (MSG_MW_MULTIMEMO_SEND_ACK*)pMsg;
	MSG_MO_RTTG_SEND_MESSAGE_ANS AnsMsg;
	AnsMsg.m_dwWopsKey	= pRecvMsg->m_dwKey;
	AnsMsg.m_dwCount	= pRecvMsg->m_dwCount;

	memcpy( AnsMsg.m_arrResultInfoList, pRecvMsg->m_arrResultInfoList, sizeof(MMEMO_ERR_RESULT)*MMEMO_ERR_RESULT::MAX_RESULT_INFO );

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pSession = pMasterServer->ConnectSessions.SessionOpServer();
	if( pSession )
		pSession->SendPacket( &AnsMsg,sizeof(MSG_MO_RTTG_SEND_MESSAGE_ANS) );
	
 	for( DWORD i = 0; i < pRecvMsg->m_dwCount; ++i )
 	{
		SUNLOG(eCRITICAL_LOG, "[MSG_MO_RTTG_SEND_MESSAGE_ANS] WopsKey[%d] 멀티쪽지 결과[%s] [%s]", 
			AnsMsg.m_dwWopsKey,
			AnsMsg.m_arrResultInfoList[i].m_szRecvCharName,
			AnsMsg.m_arrResultInfoList[i].m_szResultDescription);
 	}
}
//}
