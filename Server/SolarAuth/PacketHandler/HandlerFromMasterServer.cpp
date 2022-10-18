
#include "stdafx.h"

#include <PacketStruct_SM.h>

#include <SolarAuthServer.h>

#include "PacketHandler.h"
#include "HandlerFromMasterServer.h"

#include "Sessions/MasterServerSession.h"

#include <Information/SolarAcceptableMasterServerListLoader.h>
#include <Information/ScriptLoadManager.h>
#include <Information/ScriptListLoader.h>
#include <Information/ScriptLoader.h>
#include <Information/ServerList/ServerInfo.h>

#include <WorkLoad/WorkLoadManager.h>


// WorkLoad Handlers
#include "HandlerFromMasterServer.WorkLoad.hxx"


#define SM_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define SM_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&HandlerFromMasterServer::On##p }



sPACKET_HANDLER_INFO*	HandlerFromMasterServer::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO		s_pHandlerInfo[] =
	{
		//
		SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_HEARTBEAT, (ePHASE_SHIFT)(PHASE_UNKNOWN|PHASE_SERVICE)),
		//SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_SYN, PHASE_UNKNOWN ),
		//SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_ACK, PHASE_UNKNOWN ),
		//SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_NAK, PHASE_UNKNOWN ),
		//SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_DISCONNECT_CMD, PHASE_UNLIMITED),
		//SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_PHASE_SHIFT_CMD, PHASE_UNLIMITED),
		//SM_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_BLOCK_CMD, PHASE_UNLIMITED),
		//
		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_SYN, PHASE_EXCHANGE ),
		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_ACK, PHASE_EXCHANGE ),
		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_NAK, PHASE_EXCHANGE ),
		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_SYN, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_ACK, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_CMD, PHASE_SERVICE),
		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_NAK, PHASE_SERVICE),

		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_SYN, PHASE_SERVICE ),
		//SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_ACK, PHASE_SERVICE ),
		//SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_NAK, PHASE_SERVICE ),
		SM_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_CACHEITEM_REPORT_NTF, PHASE_SERVICE ),
		//SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_KEY, PHASE_EXCHANGE ),
		//SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_ACK, PHASE_EXCHANGE ),
		//SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_NAK, PHASE_EXCHANGE ),
		//SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_CMD, PHASE_EXCHANGE ),
		SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_PACKET_ENCODED, PHASE_SERVICE),
		SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_PACKET_ENCODING, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_KEY, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_CMD, PHASE_SERVICE),
		SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_ACK, PHASE_SERVICE),
		SM_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_NAK, PHASE_SERVICE),
		//
		SM_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_SERVICE),
		//SM_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_CMD, PHASE_SERVICE),
		SM_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_ACK, PHASE_SERVICE),
		SM_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_NAK, PHASE_SERVICE),

		SM_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

VOID	HandlerFromMasterServer::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	//BOOL bResult =
	pInstance->RegisterHandler_<MASTER_SERVER>( GetHandlerInfo() );
	//
}

HandlerFromMasterServer_IMPL( SM_CONNECTION_HEARTBEAT )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

//
//HandlerFromMasterServer_IMPL( SM_CONNECTION_CONNECTION_SYN )

//HandlerFromMasterServer_IMPL( SM_CONNECTION_CONNECTION_ACK )

//HandlerFromMasterServer_IMPL( SM_CONNECTION_CONNECTION_NAK )

//HandlerFromMasterServer_IMPL( SM_CONNECTION_DISCONNECT_CMD )

//HandlerFromMasterServer_IMPL( SM_CONNECTION_PHASE_SHIFT_CMD )

//HandlerFromMasterServer_IMPL( SM_CONNECTION_BLOCK_CMD )

//

HandlerFromMasterServer_IMPL( SM_SERVER_INFO_DESTINATION_KEY_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HandlerFromMasterServer_IMPL( SM_SERVER_INFO_DESTINATION_KEY_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HandlerFromMasterServer_IMPL( SM_SERVER_INFO_DESTINATION_KEY_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HandlerFromMasterServer_IMPL( SM_SERVER_INFO_QUERY_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// 마스터별 서버정보 목록은 이미 로딩한 상태가 전제된다.
	// 서버 인증관련 매크로가 적용되지 않은 상태에서는 기존 루틴에 추가된 flow가 추가된 것으로 처리하라.
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	SolarAcceptableMasterServerListLoaderDelegator& rMasterInfoManager =
		ScriptLoadManager::Instance()->SolarAcceptableMasterServerListLoaderHandle;
	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rINFO = rMasterInfoManager->GetMasterServerByIP( pServerSession->GetIPAddress() );
	ServerTreeINFO* pTREE = rINFO->GetServerTree();

	MSG_SM_SERVER_INFO_QUERY_ACK* pMsgACK = NULL;

	if( !pTREE )
	{
		MessageOut( eCRITICAL_LOG, _T("해당 마스터 서버 {%s}에 서버 목록이 존재하지 않습니다."), pServerSession->GetIP() );
		return;
	}

	if( pTREE )
	{
		DWORD rdwUpper;
		PSERVER_INFO_GENERIC pINFOs = pTREE->GetServerArrayList( rdwUpper );
		PSERVER_INFO_GENERIC pINFO = pINFOs;

		DWORD dwIdx = 0;
		while( dwIdx < rdwUpper )
		{
			DWORD dwCount = 0;
			pMsgACK = new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_SM_SERVER_INFO_QUERY_ACK;

			sSERVER_INFO_GENERIC* pServerINFO = pMsgACK->m_ServerInfos;
			for( ; dwIdx<rdwUpper ; ++dwIdx, ++pINFO )
			{
				if( pINFO->m_ServerType == UNKNOWN_SERVER )
					continue;

				*pServerINFO = *pINFO;
				++dwCount;
				++pServerINFO;

				if( MSG_SM_SERVER_INFO_QUERY_ACK::_MAX_SERVERINFO_ARRAY_COUNT == dwCount )
				{
					++dwIdx;
					++pINFO;
					break;
				}
			}

			if( dwCount )
			{
				pMsgACK->m_wCounts = (WORD)dwCount;
				pServerSession->SendPacket( pMsgACK, pMsgACK->GetSize() );
				dwCount = 0;
			}
		}

		MSG_SM_SERVER_INFO_QUERY_CMD pMsgCMD[1];
		pServerSession->SendPacket( pMsgCMD, sizeof(MSG_SM_SERVER_INFO_QUERY_CMD) );

		MSG_SM_CONNECTION_PHASE_SHIFT_CMD pMsgCMD_SHIFT[1];
		pMsgCMD_SHIFT->m_dwPHASE_SHIFT = (DWORD)PHASE_SERVICE;

		pServerSession->SendPacket( pMsgCMD_SHIFT, sizeof(MSG_SM_CONNECTION_PHASE_SHIFT_CMD) );
		printf( "SEND {MSG_SM_CONNECTION_PHASE_SHIFT_CMD} EXCHANGE->SERVICE\n" );
	}
#else //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

//HandlerFromMasterServer_IMPL( SM_SERVER_INFO_QUERY_ACK )
//HandlerFromMasterServer_IMPL( SM_SERVER_INFO_QUERY_CMD )

HandlerFromMasterServer_IMPL( SM_SERVER_INFO_QUERY_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// 서버 인증관련 매크로가 적용되지 않은 상태에서는 기존 루틴에 추가된 flow가 추가된 것으로 처리하라.
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
#else //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

HandlerFromMasterServer_IMPL(SM_SERVER_INFO_AUTHENTICATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_SM_SERVER_INFO_AUTHENTICATION_SYN* pMsgSYN = (MSG_SM_SERVER_INFO_AUTHENTICATION_SYN*)pMsg;

#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	SolarAcceptableMasterServerListLoaderDelegator& rMasterInfoManager =
		ScriptLoadManager::Instance()->SolarAcceptableMasterServerListLoaderHandle;
	ACCEPTABLE_MASTER_SERVER_INFO_DELEGATOR& rINFO = rMasterInfoManager->GetMasterServerByIP( pServerSession->GetIPAddress() );
	ServerTreeINFO* pTREE = rINFO->GetServerTree();

	if( !pTREE )
	{
		MessageOut( eCRITICAL_LOG, _T("해당 마스터 서버 {%s}에 서버 목록이 존재하지 않습니다."), pServerSession->GetIP() );
		return;
	}

	SERVER_KEY skKey = pMsgSYN->m_ServerKey;
	if( sSERVER_INFO_GENERIC* pSERVER_INFO = pTREE->FindServer( skKey ) )
	{
		if( pSERVER_INFO )
		{
			MSG_SM_SERVER_INFO_AUTHENTICATION_ACK pMsgACK[1];
			pMsgACK->m_dwInnerIP = pMsgSYN->m_dwInnerIP;
			pMsgACK->m_ServerInfo = *pSERVER_INFO;
			pServerSession->SendPacket( pMsgACK, sizeof(MSG_SM_SERVER_INFO_AUTHENTICATION_ACK) );

			CHAR strIP[2][MAX_IP_STRING_LEN+1];
			CHAR* pIP = IPOperation::ConvertDWORD2StrIP(pSERVER_INFO->m_dwMasterIP);
			memcpy( strIP[0], pIP, MAX_IP_STRING_LEN ); strIP[0][MAX_IP_STRING_LEN] = '\0';
			pIP = IPOperation::ConvertDWORD2StrIP(pSERVER_INFO->m_dwInnerIP);
			memcpy( strIP[1], pIP, MAX_IP_STRING_LEN ); strIP[1][MAX_IP_STRING_LEN] = '\0';
			SERVER_KEY& rKey = pSERVER_INFO->m_ServerKey;

			MessageOut( eCRITICAL_LOG,
				_T("서버 인증 MasterIP {%s}\t인증된 서버 {%s}-[%02u][%02u][%s][%02u]"),
				strIP[0], strIP[1], rKey.GetWorldID(), rKey.GetChannelID(),
				GetServerType2String( pSERVER_INFO->m_ServerType ), rKey.GetServerID() );

			return;
		}
	}

	MessageOut( eCRITICAL_LOG,
		_T("해당 마스터 서버 {%s}에 인증 요청한 서버 정보[%02u:%02u:%02u:%02u]가 존재하지 않습니다."),
		pServerSession->GetIP(), skKey.GetWorldID(), skKey.GetChannelID(), skKey.GetServerType(), skKey.GetServerID() );

	return;

#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

//HandlerFromMasterServer_IMPL(SM_SERVER_INFO_AUTHENTICATION_ACK)
//HandlerFromMasterServer_IMPL(SM_SERVER_INFO_AUTHENTICATION_NAK)

//
//HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_KEY )

//HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_ACK )

//HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_NAK )

//HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_CMD )

HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_PACKET_ENCODED )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_PACKET_ENCODING )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

//HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_SNK_KEY )

//HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_SNK_CMD )

HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_SNK_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HandlerFromMasterServer_IMPL( SM_CRYPTOGRAPHY_SNK_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

//
HandlerFromMasterServer_IMPL( SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN* pMsgSYN = (MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN*)pMsg;
	ScriptLoadManager* pLoadManager = ScriptLoadManager::Instance();

	eSCRIPT_CODE scriptCode = (eSCRIPT_CODE)pMsgSYN->m_wDataType;

	if( !(SCRIPT_CONTROL_FLAG_UPPERBOUNDARY > scriptCode) )
	{
		SCRIPT_RECORD_INFO_DELEGATOR& rRecord =
			pLoadManager->ScriptListLoaderHandle->GetScriptInfo( scriptCode );

		if( rRecord != NULL )
		{
			MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK msgACK;
			pServerSession->SendPacket( &msgACK, sizeof(msgACK) );

			PROCESS_SM_DATA_TRANSFER_SENDDATA_CMD* pWorkCMD = new PROCESS_SM_DATA_TRANSFER_SENDDATA_CMD();

			SCRIPT_DATA_STREAM_DELEGATOR rSTREAM_INFO =
				ScriptLoadManager::GetScriptDataByIP( pServerSession->GetIPAddress(), scriptCode );

            // (WAVERIX) (0807XX) (BUG-FIX) WorkKey의 중복 가능성 낮추기 위한 버그 수정
			const DWORD ipaddress = pServerSession->GetIPAddress();
			const WORK_KEY regWorkKey = (WORD)(
				( (LOBYTE(LOWORD(ipaddress))+HIBYTE(LOWORD(ipaddress))) << 8 ) +
				( (LOBYTE(HIWORD(ipaddress))+HIBYTE(HIWORD(ipaddress))) << 4 ) +
				scriptCode);

			Services4Session& rServices = pServerSession->Services();
			if(rSTREAM_INFO != NULL &&
			   WorkLoadManager::c_NotExistKey == rServices.GetWorkLoadContainer().FindWorkKey( regWorkKey ) )
			{
				pWorkCMD->SetServerSession( pServerSession );

				pWorkCMD->SetInformation(
					rSTREAM_INFO->m_pDataStream, rSTREAM_INFO->m_CompressSize,rSTREAM_INFO->m_dwFileSize, 
					0, (WORD)scriptCode
					);

                WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkCMD, TRUE, regWorkKey );
				if( WorkLoadManager::c_NotExistKey != workKey )
				{
					rServices.GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkCMD );
					MessageOut( eCRITICAL_LOG, _T("[%-16s] DATA_SYN(%u) -> ACCEPT -> SEND PROC"), pServerSession->GetIP(), scriptCode );
					return;
				}
			}

			SAFE_DELETE( pWorkCMD );
		}

		MessageOut( eCRITICAL_LOG, _T("[%-16s] DATA_SYN(%u) -> REJECT -> Already SendWorking or Can't Find Script"),
			pServerSession->GetIP(), scriptCode );
	}
	else if( SCRIPT_NOTIFY_SIGNAL == scriptCode )
	{
		// NOTIFY_SIGNAL
		MSG_SM_DATA_TRANSFER_SENDDATA_CMD* pMsgCMD =
			new ( PACKET_CONTROL_SM::GetBasePacketPtr4Send() ) MSG_SM_DATA_TRANSFER_SENDDATA_CMD();

		pMsgCMD->m_wControlCmd = MSG_SM_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER;
		pMsgCMD->m_FILE_INFO.m_wDataType = SCRIPT_NOTIFY_SIGNAL;
		pMsgCMD->m_FILE_INFO.m_dwBufferSize = 4;
		pMsgCMD->m_FILE_INFO.m_dwCRC = 0;
		pServerSession->SendPacket( pMsgCMD, pMsgCMD->GetSize() );

		new ( PACKET_CONTROL_SM::GetBasePacketPtr4Send() ) MSG_SM_DATA_TRANSFER_SENDDATA_CMD();
		pMsgCMD->m_wControlCmd = MSG_SM_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA;
		pMsgCMD->m_TOKEN_INFO.m_Header.m_wDataType = SCRIPT_NOTIFY_SIGNAL;
		pMsgCMD->m_TOKEN_INFO.m_Header.m_wBufferSize = 4;
		pMsgCMD->m_TOKEN_INFO.m_Header.m_wTokenNo = 0;
		pMsgCMD->m_TOKEN_INFO.m_pSTREAM[0] = '0';
		pMsgCMD->m_TOKEN_INFO.m_pSTREAM[1] = '0';
		pMsgCMD->m_TOKEN_INFO.m_pSTREAM[2] = '0';
		pMsgCMD->m_TOKEN_INFO.m_pSTREAM[3] = '\0';
		pServerSession->SendPacket( pMsgCMD, pMsgCMD->GetSize() );

		new ( PACKET_CONTROL_SM::GetBasePacketPtr4Send() ) MSG_SM_DATA_TRANSFER_SENDDATA_CMD();
		pMsgCMD->m_wControlCmd = MSG_SM_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE;
		pMsgCMD->m_FILE_CLOSURE.m_wDataType = SCRIPT_NOTIFY_SIGNAL;
		pMsgCMD->m_FILE_CLOSURE.m_dwCRC = 0;
		pServerSession->SendPacket( pMsgCMD, pMsgCMD->GetSize() );

		MessageOut( eCRITICAL_LOG, _T("[%-16s] NTF_SYN(%u) -> ACCEPT -> RAISE NEXT PROCESS"), pServerSession->GetIP(), scriptCode );
		return;
	}

	MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK msgNAK;
	pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
	return;




	//MSG_SM_DATA_TRANSFER_SENDDATA_CMD msgCMD;
	//pServerSession->SendPacket( &msgCMD, sizeof(msgCMD) );
	//printf( "SEND {SM_DATA_TRANSFER_SENDDATA_CMD}\n" );
}

//HandlerFromMasterServer_IMPL( SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )

//HandlerFromMasterServer_IMPL( SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )

//HandlerFromMasterServer_IMPL( SM_DATA_TRANSFER_SENDDATA_CMD )

HandlerFromMasterServer_IMPL( SM_DATA_TRANSFER_SENDDATA_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	printf( "RECEIVED {SM_DATA_TRANSFER_SENDDATA_ACK}\n" );
}

HandlerFromMasterServer_IMPL( SM_DATA_TRANSFER_SENDDATA_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	printf( "RECEIVED {SM_DATA_TRANSFER_SENDDATA_ACK}\n" );
}


HandlerFromMasterServer_IMPL( SM_SERVER_INFO_CACHEITEM_REPORT_NTF )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_SM_SERVER_INFO_CACHEITEM_REPORT_NTF* pNTF = (MSG_SM_SERVER_INFO_CACHEITEM_REPORT_NTF*)pMsg;
	if( sizeof(*pNTF) != (SIZE_T)wSize )
	{
		MessageOut( eCRITICAL_LOG, _T("INVALID REPORT INFO") );
		return;
	}
//INFO.dwPriceGuid,
//rINFO.wItemCode,
//rINFO.wCategoryID,
//rINFO.dwPrice
	sCACHEITEM_REPORT_INFO_MINORVER& rINFO = pNTF->m_INFO;

	sSIG_REPORTFROMMASTERS SIG;
	//SIG.MasterIP() = this->GetMaster
	SIG.SessionRef() = pServerSession;
	memset( &SIG.INFO(), 0, sizeof(SIG.INFO()) );
	SIG.INFO().dwPriceGuid = rINFO.dwPriceGuid;
	SIG.INFO().wItemCode = rINFO.wItemCode;
	SIG.INFO().byCategoryID = rINFO.byCategoryID;
	SIG.INFO().dwPrice = rINFO.dwPrice;

	WORK_KEY wkKEY = SolarAuthServer::GetMainFrame()->GetReportWorkKey();
	WorkLoadManager::OnMsg( &SIG, wkKEY );

	//MessageOut( eCRITICAL_LOG, _T( "Received CacheItem Purchase Info" ) );
}

