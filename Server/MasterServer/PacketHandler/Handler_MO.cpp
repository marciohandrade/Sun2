#include "stdafx.h"
#include "Handler_MO.h"
#include "PacketHandler.Shared.h"


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_MO::On##p }


sPACKET_HANDLER_INFO*	Handler_MO::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_CERTIFY_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_GAMENOTICE_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_BATTLEZONEINFO_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_USERMONEY_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_SERVERSHUTDOWN_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_USERDISCONNECT_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_FUNCRESTRICT_RELOAD_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_WAREHOUSEMONEY_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_GAMESTATISTICS_REQ, PHASE_UNLIMITED),
        //{__NA001254_20090109_RTTG_FUNC_ADD
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_CHANGE_GUILDNAME_REQ, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MO_OPERATION, MO_RTTG_SEND_MESSAGE_REQ, PHASE_UNLIMITED),
        //}
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_MO::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<OP_SERVER>( GetHandlerInfo() );
	//pInstance->RegisterHandler_<OP_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Sessions/ServerSessionManager.h>
#include <Function/Statistics.h>
#include <Function/ServerInfoManager.h>


/* 옛날엔 이런 코드가 있었던 모양이다. 일단 참고하자.
VOID Handler_MO::OnMO_RELOAD_DATA_SYN( OpServerSession *pOpServerSession, MSG_BASE *pMsg, WORD wSize )
{
	MSG_MO_RELOAD_DATA_SYN *pRecvMsg = (MSG_MO_RELOAD_DATA_SYN*)pMsg;

	// 리로드 하고자 하는 서버를 찾는다.
	SERVER_KEY findingServerKey;
	findingServerKey.Set( pRecvMsg->byWorldID, pRecvMsg->byChannelID, pRecvMsg->byServerType, pRecvMsg->byServerID );

	ServerSession *pGameServer = ServerSessionManager::Instance()->FindServer( findingServerKey );

	// 해당 게임 서버가 접속중이 아니면 리턴
	if( !pGameServer )
	{
	MessageOut( eCRITICAL_LOG, "게임서버(%d, %d, %d, %d)가 접속중이 아니므로 공지를 보낼 수 없습니다.\n",
	pRecvMsg->byWorldID,
	pRecvMsg->byChannelID,
	pRecvMsg->byServerType,
	pRecvMsg->byServerID );

	return;
	}

	// 게임 서버로 데이터 리로드 명령
	MSG_GM_RELOAD_DATA_CMD reloadMsg;
	reloadMsg.m_byCategory	= GM_OPERATION;
	reloadMsg.m_byProtocol	= GM_RELOAD_DATA_CMD;

	pGameServer->Send( (BYTE*)&reloadMsg, sizeof(reloadMsg) );
}
*/

HANDLER_MO_IMPL(MO_RTTG_CERTIFY_ANS)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	__noop;
}


HANDLER_MO_IMPL(MO_RTTG_GAMENOTICE_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MO_RTTG_GAMENOTICE_REQ* pMsgREQ = (MSG_MO_RTTG_GAMENOTICE_REQ*)pMsg;

	// SERVER_KEY 내용 바뀌면서 바이트 뒤집어져서 임시로 수정 - 장기적으로는 구조체로 보내지 않는 편이 좋겠다.

	SUNLOG( eDEV_LOG, " %d, %d, %d, %s, %d, %d, %d ", 
		pMsgREQ->dwServerUID, 
		pMsgREQ->dwTickPerLine,
		pMsgREQ->dwNoticeLength,
		pMsgREQ->szNotice,
		pMsgREQ->m_byRed,
		pMsgREQ->m_byGreen,
		pMsgREQ->m_byBlue );


	// DEBUG, RELEASE 동일코드 유지 목적
	DWORD dwSK = pMsgREQ->dwServerUID;
	SERVER_KEY skServerKey = dwSK;// SWAP_32BIT_DATATYPE(dwSK);
	SERVER_KEY skServerKey2 = SWAP_32BIT_DATATYPE(dwSK);
	SUNLOG( eDEV_LOG, _T("받은 서버 UID = %d"), dwSK );
	SUNLOG( eDEV_LOG, _T("받은 서버 UID 변환 = %d"), SWAP_32BIT_DATATYPE(dwSK) );

	DWORD DDD = (DWORD)skServerKey2;
	SUNLOG( eDEV_LOG, _T("DDD = %d"), DDD );

	BYTE byWorldID = skServerKey.GetWorldID();
	BYTE byChannelID = skServerKey.GetChannelID();

	DWORD dwNoticeLen = pMsgREQ->dwNoticeLength;

	SUNLOG( eDEV_LOG, _T("월드 ID = %d"), byWorldID );
	SUNLOG( eDEV_LOG, _T("채널 ID = %d"), byChannelID );

	if( MAX_NOTICE_LEN < dwNoticeLen )
	{
		SUNLOG( eCRITICAL_LOG, _T("공지 길이 이상!(%u/%u)"), dwNoticeLen, MAX_NOTICE_LEN );
		return;
	}

	BOOL bIsOneNode = (BOOL)(byWorldID != 0);
	sSERVER_NODE			worldNode;
	sPTR_SERVER_NODE_ARRAY	ptrARRAY;

	SUNLOG( eDEV_LOG, _T("bIsOneNode = %d"), bIsOneNode );

	if( !bIsOneNode )
	{
		// 전체 월드에 공지
		ServerSessionManager::GetWorldServerList( &ptrARRAY );
	}
	else
	{
		// 단일 월드에 공지
		worldNode.m_pServerSession = ServerSessionManager::FindServer( skServerKey );
	}

	BOOL bProc = bIsOneNode ?
					(worldNode.m_pServerSession ? TRUE : FALSE)
					: (ptrARRAY.m_dwCount ? TRUE : FALSE);

	if( !bProc )
	{
		SUNLOG( eCRITICAL_LOG, _T("공지 Error - No World Server[%u]"), skServerKey.GetWorldID() );
		return;
	}

	if( bIsOneNode )
	{
		ptrARRAY.m_dwCount = 1;
		ptrARRAY.m_pPTRs[0] = &worldNode;
	}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MW_NOTICE_CMD* pNoticeCMD =
		new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MW_NOTICE_CMD;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MW_NOTICE_CMD pNoticeCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	pNoticeCMD->m_byCategory		= MW_OPERATION;
	pNoticeCMD->m_byProtocol		= MW_NOTICE_CMD;
	pNoticeCMD->dwServerUID			= skServerKey;
	pNoticeCMD->dwTickPerLine		= pMsgREQ->dwTickPerLine;
	pNoticeCMD->dwNoticeLength		= pMsgREQ->dwNoticeLength;
	memcpy( pNoticeCMD->szNotice, pMsgREQ->szNotice, sizeof(pNoticeCMD->szNotice) );
	pNoticeCMD->m_byRed = pMsgREQ->m_byRed;
	pNoticeCMD->m_byGreen = pMsgREQ->m_byGreen;
	pNoticeCMD->m_byBlue = pMsgREQ->m_byBlue;

	DWORD dwCount = ptrARRAY.m_dwCount;
	sSERVER_NODE** ppNODE = ptrARRAY.m_pPTRs;

	for( DWORD i=0 ; i<dwCount ; ++i, ++ppNODE )
	{
		ServerSessionEx* pSession = (*ppNODE)->m_pServerSession;
		pSession->SendPacket( pNoticeCMD, pNoticeCMD->GetSize() );
	}
}

HANDLER_MO_IMPL(MO_RTTG_BATTLEZONEINFO_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MO_RTTG_BATTLEZONEINFO_REQ* pRecvREQ = (MSG_MO_RTTG_BATTLEZONEINFO_REQ*)pMsg;

	sPTR_SERVER_NODE_ARRAY ptrARRAY;

	ServerSessionManager::GetWorldServerList( &ptrARRAY );

	DWORD dwMaxCount = ptrARRAY.m_dwCount;

	if( !dwMaxCount )
		return;

	// 현재 접속중인 모든 World 서버에게 이 프로토콜을 전달한다
	MSG_MW_BATTLEZONEINFO_REQ msgREQ;

	sSERVER_NODE** pSESSION = ptrARRAY.m_pPTRs;
	for( DWORD i=0 ; i<dwMaxCount ; ++i, ++pSESSION )
	{
		(*pSESSION)->m_pServerSession->SendPacket( &msgREQ, msgREQ.GetSize() );
	}
}

// 통화량 집계
HANDLER_MO_IMPL(MO_RTTG_USERMONEY_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_USERMONEY_ANS* pMsgANS = new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_USERMONEY_ANS;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_USERMONEY_ANS pMsgANS[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	GameStatistics::SendPacket_SumMoneyCirculation( pMsgANS );

	//MasterServer::Instance()->CheckServerMoney();
}


// 서버 정상종료 요청
HANDLER_MO_IMPL(MO_RTTG_SERVERSHUTDOWN_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MO_RTTG_SERVERSHUTDOWN_REQ* pMsgREQ = (MSG_MO_RTTG_SERVERSHUTDOWN_REQ*)pMsg;

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	MasterServerInfo& rMasterInfo = pMasterServer->GetMasterServerInfo();

	SERVER_KEY	skExitKey( pMsgREQ->m_byWorldID, pMsgREQ->m_byChannelID, pMsgREQ->m_byServerType, pMsgREQ->m_byServerID );

	EXIT_SERVER_MAP* pRefAgentServerList;
	EXIT_SERVER_MAP* pRefDBPServerList;
	EXIT_SERVER_MAP* pRefServerList;
	BOOL bSuccess = rMasterInfo.MakeServerExitList( skExitKey, pRefAgentServerList, pRefDBPServerList, pRefServerList );

	if( bSuccess )
	{
		bSuccess = rMasterInfo.RegisterServerExitProcess( pRefAgentServerList, pRefDBPServerList, pRefServerList );

		if( bSuccess )
		{
			//서버 종료는 순차적으로(필드/배틀/에이전트->DB프락시) 이루어져야 하므로, 서버 종료 모드로 들어가서 처리해야 한다
			MessageOut(
				eCRITICAL_LOG,
				_T("정상 종료 프로토콜 수신[W:%u/C:%u/T:%u/I:%u] - 서버군 종료 프로세스에 맞춰 종료 명령을 내리고 있습니다."),
				skExitKey.GetWorldID(), 
				skExitKey.GetChannelID(),
				skExitKey.GetServerType(), 
				skExitKey.GetServerID() );

			return;
		}
	}

	// 실패 처리

	// 해당하는 서버가 없다
	MSG_MO_RTTG_SERVERSHUTDOWN_ANS	msgANS;

	msgANS.dwError = 1;
	msgANS.dwKey = pMsgREQ->dwKey;
	msgANS.m_ServerKey = skExitKey;
	msgANS.i64WopsKey = pMsgREQ->i64WopsKey;
	pServerSession->Send( (BYTE*)&msgANS, sizeof(msgANS) );

	MessageOut( eCRITICAL_LOG,
		_T("SERVERSHUTDOWN_REQ Error - 해당 서버 없음![W:%u/C:%u/T:%u/I:%u]"),
		skExitKey.GetWorldID(), 
		skExitKey.GetChannelID(),
		skExitKey.GetServerType(), 
		skExitKey.GetServerID() );
}


// 접속 중인 유저 강제 종료 요청
HANDLER_MO_IMPL(MO_RTTG_USERDISCONNECT_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_MO_RTTG_USERDISCONNECT_REQ* pRecvREQ = (MSG_MO_RTTG_USERDISCONNECT_REQ*)pMsg;

	// 현재 접속중인 모든 Agent 서버에게 이 프로토콜을 전달한다
	sPTR_SERVER_NODE_ARRAY ptrARRAY;

	ServerSessionManager::GetAgentServerList( &ptrARRAY );

	DWORD dwMaxCount = ptrARRAY.m_dwCount;

	if( !dwMaxCount )
	{
		MessageOut( eCRITICAL_LOG, _T("MO_RTTG_USERDISCONNECT Error - No Agent Server[GUID:%u]"), pRecvREQ->dwUserGUID );
		return;
	}

	MSG_AM_OPERATION_CHAR_DISCONNECT_SYN msgSYN;
	msgSYN.m_dwUserGuid = pRecvREQ->dwUserGUID;

	sSERVER_NODE** pSESSION = ptrARRAY.m_pPTRs;
	for( DWORD i=0 ; i<dwMaxCount ; ++i, ++pSESSION )
	{
		(*pSESSION)->m_pServerSession->SendPacket( &msgSYN, sizeof(msgSYN) );
	}

	MessageOut( eFULL_LOG, _T("USERDISCONNECT Send[GUID:%u]"), pRecvREQ->dwUserGUID );
}

// 기능제한 테이블 다시 읽어서 Game서버 들에 Broadcast
HANDLER_MO_IMPL(MO_RTTG_FUNCRESTRICT_RELOAD_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	if( pMasterServer->GetMasterServerInfo().SendMX_FUNCRESTRICT_RELOAD_REQ( pMasterServer ) )
	{
		MessageOut( eFULL_LOG, "FUNCRESTRICT Reload Request to AccountDBProxy" );
	}
	else
	{
		MessageOut( eCRITICAL_LOG, "FUNCRESTRICT Reload Error - AccountDBProxy Not Connected" );
	}
}

// 창고 통화량
HANDLER_MO_IMPL(MO_RTTG_WAREHOUSEMONEY_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// <임시주석처리>
	//MasterServer::Instance()->CheckServerWarehouseMoney();
	FASSERT( !"MO_RTTG_WAREHOUSEMONEY_REQ handler ...이것은 들어와선 안된당");
}

// 통계 계산하여 보내준다
HANDLER_MO_IMPL(MO_RTTG_GAMESTATISTICS_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// <임시주석처리>
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_GAMESTATISTICS_ANS* pMsgANS = new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_GAMESTATISTICS_ANS;
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MO_RTTG_GAMESTATISTICS_ANS pMsgANS[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	GameStatistics::SendPacket_StatisticsEtcInfo( pMsgANS );
}

//HANDLER_MO_IMPL(MO_RELOAD_DATA_SYN);

//{__NA001254_20090109_RTTG_FUNC_ADD
HANDLER_MO_IMPL(MO_RTTG_CHANGE_GUILDNAME_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		MSG_MO_RTTG_CHANGE_GUILDNAME_REQ* pMsgCMD = (MSG_MO_RTTG_CHANGE_GUILDNAME_REQ*)pMsg;
		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

		SUNLOG(eCRITICAL_LOG, "MSG_MO_RTTG_CHANGE_GUILDNAME_REQ 패킷 데이터 "
							"WopsKey[%d] WorldID[%d] ChannelID[%d] "
							"ServerType[%d] ServerID[%d] GuildGuid[%d] "
							"GuildName[%s] Description[%s]", 
							pMsgCMD->m_dwWopsKey,								
							pMsgCMD->m_byWorldID,								
							pMsgCMD->m_byChannelID,								
							pMsgCMD->m_byServerType,
							pMsgCMD->m_byServerID,
							pMsgCMD->m_dwGuildGuid,
							pMsgCMD->m_szGuildName,
							pMsgCMD->m_szDescription );

		sSERVER_NODE worldNode;
		worldNode.m_pServerSession = ServerSessionManager::FindServer( pMsgCMD->GetServerKey() );

		IfFailThenBreak(NULL != worldNode.m_pServerSession, RC::RC_GUILD_DISCONNECTED_GUILD_SERVER);

		MSG_MZ_GUILD_NAME_CHANGE_SYN msg;
		msg.m_dwKey = pMsgCMD->m_dwWopsKey;
		msg.m_GuildGuid	= pMsgCMD->m_dwGuildGuid;
		memcpy( msg.m_tszGuildName, pMsgCMD->m_szGuildName, MAX_GUILDNAME_LENGTH*sizeof(TCHAR) );
		msg.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

		IfFailThenBreak(worldNode.m_pServerSession->SendPacket(&msg, sizeof(msg)),
                        RC::RC_GUILD_DISCONNECTED_GUILD_SERVER);

		pMsgCMD->m_szDescription[MSG_MO_RTTG_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH] = '\0';
		SUNLOG(eCRITICAL_LOG, "[MO_RTTG_CHANGE_GUILDNAME_REQ] [%d] [%s] [%s]", pMsgCMD->m_dwGuildGuid, pMsgCMD->m_szGuildName, pMsgCMD->m_szDescription);
	
		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	const int MaxLen = MSG_MO_RTTG_CHANGE_GUILDNAME_ANS::MAX_RESULT_DESCRIPTION_LENGTH;

	MSG_MO_RTTG_CHANGE_GUILDNAME_REQ* pMsgCMD = (MSG_MO_RTTG_CHANGE_GUILDNAME_REQ*)pMsg;
	MSG_MO_RTTG_CHANGE_GUILDNAME_ANS  AnsMsg;
	AnsMsg.m_dwWopsKey = pMsgCMD->m_dwWopsKey;
	AnsMsg.m_dwResult  = MSG_MO_RTTG_CHANGE_GUILDNAME_ANS::CHANGE_FAIL;

	const DWORD errCode = flow.GetErrorCode();
	switch( errCode )
	{
	case RC::RC_GUILD_DISCONNECTED_GUILD_SERVER:
		strncpy( AnsMsg.m_szResultDescription, "GUILD SERVER NOT CONNECT", MaxLen );
		SUNLOG(eDEV_LOG, "invalid ServerSession WorldID[%d], ChannelID[0], ServerType[GUILD_SERVER], ServerID[1]", pMsgCMD->m_byWorldID );
		break;

	default:
		strncpy( AnsMsg.m_szResultDescription, "ERROR GUILD SERVER", MaxLen );
		SUNLOG(eDEV_LOG, "GUILD_NAME_CHANGE SendPacket Error[%d]", pMsgCMD->m_byWorldID);
		break;
	}

	AnsMsg.m_szResultDescription[MaxLen] = '\0';
	pServerSession->SendPacket( &AnsMsg,sizeof(MSG_MO_RTTG_CHANGE_GUILDNAME_ANS) );

	assert( SAFE_NUMERIC_TYPECAST( DWORD, errCode, BYTE ) );
}


HANDLER_MO_IMPL(MO_RTTG_SEND_MESSAGE_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		MSG_MO_RTTG_SEND_MESSAGE_REQ* pMsgREQ = (MSG_MO_RTTG_SEND_MESSAGE_REQ*)pMsg;

		SUNLOG(eCRITICAL_LOG, "MSG_MO_RTTG_SEND_MESSAGE_REQ 패킷데이터 "
								"WopsKey[%d] WorldID[%d] ChannelID[%d] "
								"ServerType[%d] ServerID[%d] Count[%d] "
								"OperCharName[%s] Message[%s]", 
								pMsgREQ->m_dwWopsKey,								
								pMsgREQ->m_byWorldID,								
								pMsgREQ->m_byChannelID,								
								pMsgREQ->m_byServerType,
								pMsgREQ->m_byServerID,
								pMsgREQ->m_MMemoInfo.m_nCount,
								pMsgREQ->m_MMemoInfo.m_OperCharName,
								pMsgREQ->m_MMemoInfo.m_szMessage );

		for( int i = 0; i < pMsgREQ->m_MMemoInfo.m_nCount; ++i )
		{
			SUNLOG(eCRITICAL_LOG, "UserInfo[%s]",pMsgREQ->m_MMemoInfo.m_arrUserInfoList[i].m_szRecvCharName);
		}

		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

		sSERVER_NODE worldNode;
		worldNode.m_pServerSession = ServerSessionManager::FindServer( pMsgREQ->GetServerKey() );

		IfFailThenBreak(NULL != worldNode.m_pServerSession, RC::RC_MEMO_NOSERVER);
		IfFailThenBreak(pMsgREQ->m_MMemoInfo.ChkRecvCount(), RC::RC_MEMO_WRONG_RECVNAMES);

		MSG_MW_MULTIMEMO_SEND_SYN SendMsg;
		SendMsg.m_dwKey			= pMsgREQ->m_dwWopsKey;
		SendMsg.m_MMemoInfo		= pMsgREQ->m_MMemoInfo;

		IfFailThenBreak(worldNode.m_pServerSession->SendPacket(&SendMsg,sizeof(SendMsg)),
                        RC::RC_MEMO_NOSERVER);


 		SUNLOG(eCRITICAL_LOG, "[MSG_MO_RTTG_SEND_MESSAGE_REQ] WorldID[%d] CharGuid[%d] Message[%s]", 
			pMsgREQ->m_byWorldID, pMsgREQ->m_MMemoInfo.m_OperCharName, pMsgREQ->m_MMemoInfo.m_szMessage);

 		for( int i = 0; i < pMsgREQ->m_MMemoInfo.m_nCount; ++i )
 			SUNLOG(eCRITICAL_LOG, "[MSG_MO_RTTG_SEND_MESSAGE_REQ] [%s]", pMsgREQ->m_MMemoInfo.m_arrUserInfoList[i].m_szRecvCharName);
	
		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	const DWORD errCode = flow.GetErrorCode();

	MSG_MO_RTTG_SEND_MESSAGE_REQ* pMsgREQ = (MSG_MO_RTTG_SEND_MESSAGE_REQ*)pMsg;
	MSG_MO_RTTG_SEND_MESSAGE_ANS  AnsMsg;
	AnsMsg.m_dwWopsKey = pMsgREQ->m_dwWopsKey;
	AnsMsg.m_dwCount = pMsgREQ->m_MMemoInfo.m_nCount;

	for( int i = 0; i < pMsgREQ->m_MMemoInfo.m_nCount; ++i )
	{
		AnsMsg.m_arrResultInfoList->SetCharName(pMsgREQ->m_MMemoInfo.m_arrUserInfoList[i].m_szRecvCharName);
		AnsMsg.m_arrResultInfoList->SetResult(errCode);
	}

	pServerSession->SendPacket( &AnsMsg,sizeof(MSG_MO_RTTG_SEND_MESSAGE_ANS) );

	SUNLOG(eCRITICAL_LOG, "[MSG_MO_RTTG_SEND_MESSAGE_REQ] WorldID[%d] CharGuid[%d] Message[%s] errCode[%d]", 
		pMsgREQ->m_byWorldID, pMsgREQ->m_MMemoInfo.m_OperCharName, pMsgREQ->m_MMemoInfo.m_szMessage, errCode);

	assert( SAFE_NUMERIC_TYPECAST( DWORD, errCode, BYTE ) );
}

void Handler_MO::SendTest()
{
	if( 0 )	
		SendMMemo();
	if( 0 )	
		SendGuildNameChange();
}

void Handler_MO::SendMMemo()
{
	SUNLOG(eCRITICAL_LOG, "TEST SendMMemo");

	sSERVER_NODE worldNode;
	SERVER_KEY	 skServerKey( 10, 0, WORLD_SERVER, 1 );

	worldNode.m_pServerSession = ServerSessionManager::FindServer( skServerKey );
	if( !worldNode.m_pServerSession )
		return;

	MSG_MW_MULTIMEMO_SEND_SYN SendMsg;
	SendMsg.m_dwKey			= 10;
	SendMsg.m_MMemoInfo.m_nCount = 3;
	_tcsncpy( SendMsg.m_MMemoInfo.m_OperCharName, "소환테슷", MAX_CHARNAME_LENGTH );
	SendMsg.m_MMemoInfo.m_OperCharName[MAX_CHARNAME_LENGTH] = '\0';

	_tcsncpy( SendMsg.m_MMemoInfo.m_szMessage, "메모테스트", MMEMO_INFOS::MAX_MESSAGE_LENGTH );
	SendMsg.m_MMemoInfo.m_OperCharName[MMEMO_INFOS::MAX_MESSAGE_LENGTH] = '\0';

	_tcsncpy( SendMsg.m_MMemoInfo.m_arrUserInfoList[0].m_szRecvCharName, "이수택", MAX_CHARNAME_LENGTH );
	SendMsg.m_MMemoInfo.m_arrUserInfoList[0].m_szRecvCharName[MAX_CHARNAME_LENGTH] = '\0';
	_tcsncpy( SendMsg.m_MMemoInfo.m_arrUserInfoList[1].m_szRecvCharName, "121234", MAX_CHARNAME_LENGTH );
	SendMsg.m_MMemoInfo.m_arrUserInfoList[1].m_szRecvCharName[MAX_CHARNAME_LENGTH] = '\0';
	_tcsncpy( SendMsg.m_MMemoInfo.m_arrUserInfoList[2].m_szRecvCharName, "2312313", MAX_CHARNAME_LENGTH );
	SendMsg.m_MMemoInfo.m_arrUserInfoList[2].m_szRecvCharName[MAX_CHARNAME_LENGTH] = '\0';

	worldNode.m_pServerSession->SendPacket(&SendMsg,sizeof(MSG_MW_MULTIMEMO_SEND_SYN));
}

void Handler_MO::SendGuildNameChange()
{
	SUNLOG(eCRITICAL_LOG, "TEST SendGuildNameChange");

	sSERVER_NODE worldNode;
	SERVER_KEY	 skServerKey( 10, 0, GUILD_SERVER, 1 );

	worldNode.m_pServerSession = ServerSessionManager::FindServer( skServerKey );
	if( !worldNode.m_pServerSession )
		return;

	MSG_MZ_GUILD_NAME_CHANGE_SYN msg;
	msg.m_dwKey = 10;
	msg.m_GuildGuid	= 612;

	_tcsncpy( msg.m_tszGuildName, "길드테슷웁스", MAX_GUILDNAME_LENGTH*sizeof(TCHAR)  );
	msg.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

	worldNode.m_pServerSession->SendPacket(&msg,sizeof(MSG_MZ_GUILD_NAME_CHANGE_SYN));
}
//}__NA001254_20090109_RTTG_FUNC_ADD