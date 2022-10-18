#include "stdafx.h"
#include ".\Handler_FromMasterServer.h"

#include <Macro.h>
#include <PacketStruct_MX.h>
#include <PacketStruct_DM.h>
#include <PacketStruct_DZ.h> //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
#include <PacketStruct_MZ.h>
#include <PacketStruct_ServerCommon.h>

#include "DBProxyServerEx.h"
#include "Sessions/ChildServerSession.h"
#include "QueryObjects/DBHandler.h"
#include "QueryObjects/MasterServerQuery.h"
#include "PacketHandler/Handler_Guild.GuildToDbp.h"

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_FromMasterServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_FromMasterServer::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_FromMasterServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        //
        HANDLER_NODE_INFO(DM_CONNECTION, DM_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
    #if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT)
        HANDLER_NODE_INFO(MX_CONNECTION, MX_CONNECTION_REQ_SERVER_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(MX_CONNECTION, MX_CONNECTION_REQ_SERVER_INFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(MX_CONNECTION, MX_FUNCRESTRICT_RELOAD_REQ, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(MX_CONNECTION, MX_FUNCRESTRICT_RELOAD_DBR, PHASE_UNLIMITED),
    #elif (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT)
        HANDLER_NODE_INFO(DM_CONNECTION, DM_CONNECTION_SERVER_INFO_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_UNLIMITED),
      #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
        HANDLER_NODE_INFO(MZ_GUILD, MZ_GUILD_NAME_CHANGE_SYN, PHASE_UNLIMITED),
      #endif
      #ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
        HANDLER_NODE_INFO(DM_CONNECTION, DM_CONNECTION_DISCONNECT_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DM_CONNECTION, DM_CONNECTION_PHASE_SHIFT_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DM_CONNECTION, DM_CONNECTION_BLOCK_CMD, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_UNLIMITED),
      #endif //
    #endif
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================

Handler_FromMasterServer_IMPL(DM_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

Handler_FromMasterServer_IMPL(DM_CONNECTION_SERVER_INFO_CMD)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DM_CONNECTION_SERVER_INFO_CMD * pRecvMsg = (MSG_DM_CONNECTION_SERVER_INFO_CMD *)pMsg;

	g_DBProxyServer.SetKey( pRecvMsg->m_ServerKey );
	g_DBProxyServer.SetGuid( pRecvMsg->m_dwServerGUID );
	g_DBProxyServer.UpdateAppTitle();

	g_DBProxyServer.SetListenIP_s(pRecvMsg->m_szDBProxyServerIP);
	g_DBProxyServer.SetListenPort_s(pRecvMsg->m_wDBProxyServerPort);

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	g_DBProxyServer.Listen( SERVER_IOHANDLER );
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	if( g_DBProxyServer.IsServerInfoFromFile() )
		g_DBProxyServer.Listen( SERVER_IOHANDLER );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

Handler_FromMasterServer_IMPL(MX_CONNECTION_REQ_SERVER_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	ExecuteServerInfoQuery( pServerSession, 1 );
}

VOID Handler_FromMasterServer::ExecuteServerInfoQuery( ChildServerSession * pServerSession, BYTE Page )
{
	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString(
		"S_ServerInfoList %u,%u",
		Page, ServerInfoQuery::SERVERINFO_ROW_NUM );

	ServerInfoQuery* pQuery = ServerInfoQuery::ALLOC();
	ZeroMemory( pQuery->pServerInfo, sizeof(ServerInfoQuery::tag_SERVERINFO)*ServerInfoQuery::SERVERINFO_ROW_NUM );
	pQuery->SetIndex( MAKEDWORD( (WORD)MX_CONNECTION, (WORD)MX_CONNECTION_REQ_SERVER_INFO_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetQuery( szQueryBuff );
	pQuery->SetCurPage(Page);
	pServerSession->DBQuery( pQuery );
}

Handler_FromMasterServer_IMPL(MX_CONNECTION_REQ_SERVER_INFO_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;

	ServerInfoQuery * pQResult		= (ServerInfoQuery *)pResult->m_pData;

	if( pQResult->GetResultRowNum() != 0 )
	{
		ASSERT( MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM != ServerInfoQuery::SERVERINFO_ROW_NUM );
		ASSERT( pQResult->GetResultRowNum() < MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM );

		MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK msg;
		msg.m_byCategory			= MX_CONNECTION;
		msg.m_byProtocol			= MX_CONNECTION_REQ_SERVER_INFO_ACK;
		msg.m_dwNumberOfServerInfo  = pQResult->GetResultRowNum();
		for( DWORD i = 0 ; i < msg.m_dwNumberOfServerInfo ; ++i )
		{
			msg.m_ServerInfo[i].ServerKey.Set( pQResult->pServerInfo[i].byWorldID,
				pQResult->pServerInfo[i].byChannelID,
				pQResult->pServerInfo[i].byServerType,
				pQResult->pServerInfo[i].byServerID );
			msg.m_ServerInfo[i].dwServerGUID			= pQResult->pServerInfo[i].dwServerGUID;
			msg.m_ServerInfo[i].wPort					= (WORD)pQResult->pServerInfo[i].iPort;
			msg.m_ServerInfo[i].wInnerPort				= (WORD)pQResult->pServerInfo[i].iInnerPort;
			strncpy( msg.m_ServerInfo[i].szIP,			pQResult->pServerInfo[i].pszIP, MAX_IPADDRESS_SIZE );
			msg.m_ServerInfo[i].szIP[MAX_IPADDRESS_SIZE] = '\0';
			strncpy( msg.m_ServerInfo[i].szInnerIP,		pQResult->pServerInfo[i].pszInnerIP, MAX_IPADDRESS_SIZE );
			msg.m_ServerInfo[i].szInnerIP[MAX_IPADDRESS_SIZE] = '\0';
		}
		pServerSession->Send( (BYTE *)&msg, msg.GetPacketSize() );
	}

	if( pQResult->GetResultRowNum() == 0 || pQResult->GetResultRowNum() < ServerInfoQuery::SERVERINFO_ROW_NUM )
	{
		/// 마지막 데이터 이다.
		MSG_MX_CONNECTION_REQ_SERVER_INFO_CMD msg;
		msg.m_byCategory	= MX_CONNECTION;
		msg.m_byProtocol	= MX_CONNECTION_REQ_SERVER_INFO_CMD;
		pServerSession->Send( (BYTE *)&msg, sizeof(msg) );
	}
	else
	{
		ExecuteServerInfoQuery( pServerSession, pQResult->GetCurPage()+1 );
	}

	ServerInfoQuery::FREE( pQResult ); pQResult = NULL;
}

Handler_FromMasterServer_IMPL(SERVERCOMMON_SERVERSHUTDOWN_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
//	MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS msg;
//	pServerSession->SendPacket( &msg, sizeof(msg) );

	// 서버 셧다운
	g_DBProxyServer.RequestShutdown();
}

Handler_FromMasterServer_IMPL(SERVERCOMMON_HEARTBEAT)
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

// 기능제한 리로드
Handler_FromMasterServer_IMPL(MX_FUNCRESTRICT_RELOAD_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	ExecuteRestrictInfoQuery( pServerSession );
}

VOID Handler_FromMasterServer::ExecuteRestrictInfoQuery( ChildServerSession * pServerSession )
{
	STRING_SAFER_QUERY32 szQueryBuff( "exec USP_GetFunction" );

	RestrictInfoQuery * pQuery = RestrictInfoQuery::ALLOC();
	pQuery->SetIndex( MAKEDWORD( (WORD)MX_CONNECTION, (WORD)MX_FUNCRESTRICT_RELOAD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetQuery( szQueryBuff );
	pServerSession->DBQuery( pQuery );
}

// 기능제한 리로드 결과
Handler_FromMasterServer_IMPL(MX_FUNCRESTRICT_RELOAD_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;

	RestrictInfoQuery * pQResult		= (RestrictInfoQuery *)pResult->m_pData;

	if( pQResult->GetResultRowNum() == 0 )
	{
		MSG_MX_FUNCRESTRICT_RELOAD_NAK	nakMsg;

		nakMsg.m_dwErrorCode = 0;

		pServerSession->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eCRITICAL_LOG, "FuncRestrict Reload DB Error - No Restrict Infomation" );
	}
	else
	{
		MSG_MX_FUNCRESTRICT_RELOAD_ANS msgANS;

		DWORD dwCount = pQResult->GetResultRowNum();
		msgANS.byRestrictNum = (BYTE)dwCount;
		memset( msgANS.sRestrict, 0, sizeof(STRUCT_RESTRICT) * MSG_MX_FUNCRESTRICT_RELOAD_ANS::_MAX_FUNCRESTRICT_LIST_NUM );

		STRUCT_RESTRICT_MASTER* pOFFSET = msgANS.sRestrict;
		RestrictInfoQuery::sQUERY* pQUERY = pQResult->pResult;
		for( DWORD i = 0 ; i < dwCount ; ++i, ++pOFFSET, ++pQUERY )
		{
			pOFFSET->bAllow = (BOOL)(pQUERY->byAllow != 0);
			pOFFSET->dwFunc = pQUERY->dwIndex;

			const DWORD cBUFLEN = sizeof(pOFFSET->ptszDesc)/sizeof(TCHAR);
			_tcsncpy( pOFFSET->ptszDesc, pQUERY->szDesc, cBUFLEN );
			pOFFSET->ptszDesc[cBUFLEN-1] = _T('\0');

			//msgANS.sRestrict[i].bAllow = ( pQResult->pResult[i].byAllow != 0) ? ( TRUE ) : ( FALSE );
			//msgANS.sRestrict[i].dwFunc = pQResult->pResult[i].dwIndex;
			//_tcsncpy( msgANS.sRestrict[i].ptszDesc, pQResult->pResult[i].szDesc, sizeof(msgANS.sRestrict[i].ptszDesc)/sizeof(TCHAR) );
		}
		pServerSession->Send( (BYTE *)&msgANS, msgANS.GetSize() );

		MessageOut( eCRITICAL_LOG, "FuncRestrict Reload DB Success[Cnt:%u]", dwCount );
	}

	RestrictInfoQuery::FREE( pQResult ); pQResult = NULL;
}

#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
//__NA001254_20090109_RTTG_FUNC_ADD
Handler_FromMasterServer_IMPL(MZ_GUILD_NAME_CHANGE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    FlowControl flow;
    FLOWCONTROL_START()
    {
        MSG_MZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_MZ_GUILD_NAME_CHANGE_SYN*)pMsg;

        MSG_DZ_GUILD_NAME_CHANGE_SYN msg;
        msg.m_dwKey          = pRecvMsg->m_dwKey; // WOPS key
        msg.m_TargetPos      = INVALID_POSTYPE_VALUE;
        msg.m_OperCharGuid   = 0;//INVALID_CHARGUID;
        msg.m_GuildGuid      = pRecvMsg->m_GuildGuid;
        msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
        _tcsncpy(msg.m_tszGuildName, pRecvMsg->m_tszGuildName, _countof(msg.m_tszGuildName));
        msg.m_tszGuildName[_countof(msg.m_tszGuildName) - 1] = '\0';
        Handler_Guild_GuildToDbp::OnDZ_GUILD_NAME_CHANGE_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));

        SUNLOG(eDEV_LOG, _T("[MZ_GUILD_NAME_CHANGE_SYN] 길드명 변경 신청 처리됨.[%d] [%s]"),
               pRecvMsg->m_GuildGuid, pRecvMsg->m_tszGuildName);

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>

    const DWORD errCode = flow.GetErrorCode();
    assert(SAFE_NUMERIC_TYPECAST(DWORD, errCode, BYTE));

    MSG_MZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_MZ_GUILD_NAME_CHANGE_SYN*)pMsg;
    MSG_MZ_GUILD_NAME_CHANGE_NAK  NakMsg;
    NakMsg.m_dwKey = pRecvMsg->m_dwKey;
    NakMsg.m_byErrorCode = (BYTE)errCode;

    pServerSession->SendPacket(&NakMsg, sizeof(NakMsg));
}
#endif

//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

Handler_FromMasterServer_IMPL( DM_CONNECTION_DISCONNECT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//printf( "RECEIVED [DM_CONNECTION_DISCONNECT_CMD]\n" );
}

// MASTER SERVER "SM_CONNECTION_PHASE_SHIFT_CMD" 변경시 참조해야 한다.
// 애초, SM_CONNECTION_PHASE_SHIFT_CMD를 수정할 때 일괄적으로 변경해야 한다.
Handler_FromMasterServer_IMPL( DM_CONNECTION_PHASE_SHIFT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//printf( "RECEIVED {DM_CONNECTION_PHASE_SHIFT_CMD} SERVICE MODE\n" );

	MSG_DM_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_DM_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
	ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD일 수 있다.
	ePHASE_SHIFT prevPhase = g_DBProxyServer.GetPhaseStatus();

	MasterServerSession* pMasterServer = (MasterServerSession*)pServerSession;

	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		// 인증 단계를 다시 거치라는 의미
		break;
	case PHASE_EXCHANGE:
		{

			if( PHASE_EXCHANGE == prevPhase )
				return;

			g_DBProxyServer.SetPhaseStatus( ePhase );

			if( PHASE_SERVICE == prevPhase )
			{
				// 다시 EXCHANGE상태로 전환되었다는 것은 서버 연결 상의 문제 때문에 제약을...
				// PHASE_SERVICE -> PHASE_EXCHANGE
				// 이것은 PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE가 존재한다는 의미

				// 데이터를 다시 받아야 하는지 등등을 NOTIFY_SIGNAL로 해결해 보자.
				eSCRIPT_DATA_CMD cmd =
					SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2EMPTYDATA;

				ScriptDataManager::DataStatusCommand( cmd, SCRIPT_NOTIFY_SIGNAL );
			}
			else
			{
				//
				// PHASE_UNKNOWN -> PHASE_EXCHANGE
				//
				// 서비스로의 전환이므로 파일 전송등등, 여러가지의 SolarAuth관련 서비스를
				// 받을 수 있다. 또는 주어진 CMD에 대한 처리를 수행할 수 있다.
				// 이곳에서 WorkLoad설정하면 된다.
				// 이것 이외의 상태일 경우는 관련 WorkLoad 제거하자. 아니면 제거되도록 구성한다.

				// <THINKING2>
				//   기존에 종료되지 않는 작업이 있을 수 있다. 이를 어떻게 해결할 것인가?
				//if( WorkLoadManager::c_NotExistKey ==
				//	pServerSession->GetWorkLoadContainer().FindWorkKey( scriptCode ) )

				PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE* pWorkNode =
					new PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE();
				pWorkNode->SetInformation( pMasterServer, pMasterServer->GetSyncKey(), (DWORD)PHASE_EXCHANGE );
				WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkNode );
				if( WorkLoadManager::c_NotExistKey != workKey )
				{
					pMasterServer->GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkNode );
					return;
				}
			}
		}
		break;
	case PHASE_SERVICE:
		{
			if( PHASE_UNKNOWN == prevPhase )
			{
				ASSERT( !"잘못된 제어 상태다 확인할 것" );
				return;
			}
			else if( PHASE_SERVICE == prevPhase )
			{
				ASSERT( !"잘못된 제어 상태다 확인할 것 - 뭔가 착각한 것 아닌가?" );
				return;
			}
			g_DBProxyServer.SetPhaseStatus( ePhase );

			g_DBProxyServer.Listen( SERVER_IOHANDLER );
		}
		break;
	};

	//if( pCMD->m_dwPHASE_SHIFT == PHASE_SERVICE /*0x00000001<<3*/ )
	//{
	//	MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN msgSYN;
	//	msgSYN.m_wDataType = 10;
	//	//pServerSession->SendPacket( &msgSYN, sizeof(msgSYN) );
	//	pServerSession->Send( (BYTE*)&msgSYN, sizeof(msgSYN) );

	//	for( DWORD i=11 ; i<20 ; ++i )
	//	{
	//		msgSYN.m_wDataType = i;
	//		pServerSession->Send( (BYTE*)&msgSYN, sizeof(msgSYN) );
	//	}
	//}
	//printf( "RECEIVED {AM_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n" );
}

Handler_FromMasterServer_IMPL( DM_CONNECTION_BLOCK_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//printf( "RECEIVED {DM_CONNECTION_BLOCK_CMD}\n" );
}


//
//Handler_FromMasterServer_IMPL( AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN )

Handler_FromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//printf( "RECEIVED {SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK}\n" );
}

Handler_FromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//printf( "RECEIVED {SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK}\n" );
}

Handler_FromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD )
{
    __TOUCHED((pServerSession, wSize));
	//printf( "RECEIVED {SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD}\n" );

	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD* pMsgCMD = (MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD*)pMsg;
	switch( (DWORD)pMsgCMD->m_wControlCmd )
	{
	case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER:
		{
			DWORD dwDataType = (DWORD)pMsgCMD->m_FILE_INFO.m_wDataType;
			DWORD dwBufferSize = (DWORD)pMsgCMD->m_FILE_INFO.m_dwBufferSize;

			// <Work><Sequence>
			//   해당 처리는 현재 데이터 로딩을 요청한 이후에 대한 첫번째 응답에 대한 반응이다.
			// <1> 해당 스크립트 코드가 올바른 지 검사 -> F : NAK
			// <2> T : 해당 스크립트 코드에 버퍼를 할당한다.
			//     DataStreamCommand (SET blah~
			// <3> 기타 타임아웃등등의 보안정책을 강구할 것

			//printf( "\n\tHEADER {Data Type=%u, Buffer Size = %u\n", dwDataType, dwBufferSize );

			BYTE* pSTREAM;

			eSCRIPT_DATA_CMD cmdSTREAM =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;

			ScriptDataManager::DataStreamCommand( cmdSTREAM, (eSCRIPT_CODE)dwDataType, pSTREAM, dwBufferSize );

			eSCRIPT_DATA_CMD cmdSTATUS =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADING;

			ScriptDataManager::DataStatusCommand( cmdSTATUS, (eSCRIPT_CODE)dwDataType );
		}
		break;

	case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA:
		{
			DWORD dwDataType = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wDataType;
			DWORD dwTokenNo = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wTokenNo;
			DWORD dwBufferSize = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wBufferSize;

			// <Work><Sequence>
			//   해당 처리는 현재 데이터 로딩을 요청에 대한 응답을 처리하며, 실재 데이터가 입력되어 온다.
			//   종결자가 올때까지 해당 토큰에 해당하는 위치에
			//   (해당 스크립에 할당된 버퍼를 얻어서 그 위치에) 데이터를 복사해 간다.
			//   DataStreamCommand (SET blah~

			ScriptDataManager::SaveDataStreamToken(
				(eSCRIPT_CODE)dwDataType,
				dwTokenNo,
				dwBufferSize,
				pMsgCMD->m_TOKEN_INFO.m_pSTREAM
				);

			//if( SCRIPT_CODE_KEY(FIELD) == dwDataType )
			//{
			//	//printf( "\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize );
			//	printf( (CHAR*)pMsgCMD->m_TOKEN_INFO.m_pSTREAM );
			//	printf( "\n\nDELIM %u\n\n", pMsgCMD->m_TOKEN_INFO.m_pSTREAM[dwBufferSize-1] );
			//}
			//printf( "\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize );

		}
		break;

	case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE:
		// 이것으로 종료하도록 수행하자.
		// 완료되면, 해당 스크립트 데이터 완료 콜백(내부 연산 목적)을 수행한다.
		//AsyncReloadData
		{
			DWORD dwDataType = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_wDataType;
			DWORD dwCRC = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_dwCRC;

			printf( "SCRIPT %u\t RECEIVED\n", dwDataType );

			eSCRIPT_DATA_CMD cmdSTATUS =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADED;

			ScriptDataManager::DataStatusCommand( cmdSTATUS, (eSCRIPT_CODE)dwDataType/*, dwCRC*/ );
			ScriptDataManager::ProcessLoadedData( (eSCRIPT_CODE)dwDataType );
		}
		break;
	};
}

//Handler_FromMasterServer_IMPL( AM_DATA_TRANSFER_SENDDATA_ACK )
//Handler_FromMasterServer_IMPL( AM_DATA_TRANSFER_SENDDATA_NAK )

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//-------------------------------------------------------------------------------------------------

