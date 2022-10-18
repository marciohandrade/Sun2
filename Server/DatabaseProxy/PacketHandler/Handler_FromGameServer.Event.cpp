#include "stdafx.h"

#include <Macro.h>
#include <ResultCode.h>
#include <PacketStruct_DG.h>

#include <SCSlot.h>
#include <SCSlotContainer.h>

#include ".\Handler_FromGameServer.h"
#include "DBUser.h"
#include "DBUserManager.h"
#include "ProxyNodes/DBQueryProxyManager.h"
#include "Sessions/GameServerSession.h"
#include "DBProxyServerEx.h"
#include "QueryObjects/GameServerQuery.h"

#include "Services/SSQHandlerInDB.h"
#include "Services/GuildManagerInDBP.h"

#include "PacketHandler/Handler_FriendInvitation.h"
#include <PacketStruct_SmartNPCSystem.h> //_NA_000000_20140120_SMART_NPC_SYSTEM



Handler_FromGameServer_IMPL( DG_EVENT_SELECT_SYN )
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_SELECT_SYN* recv_msg = static_cast<MSG_DG_EVENT_SELECT_SYN*>(pMsg);
    const USERGUID user_guid = recv_msg->m_dwKey;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eFULL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, user_guid);
        return;
    };
    DBCharacter* db_character = db_user->GetConnectedChar();
    if (db_character == NULL) {
        SUNLOG(eFULL_LOG, _T("|[%s]|Can't find user (%u)'s character|"),
               __FUNCTION__, user_guid);
        return;
    };

    STRING_SAFER_QUERY128 query_buffer;
    query_buffer.MakeString("{?=call S_Event_Select(%d,'%s')}",
                            user_guid, db_character->GetCharName());

    SelectEventQuery* const query = SelectEventQuery::ALLOC();
    query->SetUserKey(user_guid);

    query->SetQuery(query_buffer);
    query->SetVoidObject(pServerSession);
    db_user->DBQuery(DG_EVENT, DG_EVENT_SELECT_DBR, query);
}

Handler_FromGameServer_IMPL( DG_EVENT_SELECT_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult    = (MSG_DBPROXY_RESULT*)pMsg;
    SelectEventQuery* query_result     = (SelectEventQuery*)pResult->m_pData;

    const USERGUID user_guid = query_result->GetUserKey();
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eFULL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, user_guid);
        SelectEventQuery::FREE(query_result);
        return;
    };
    if (query_result->ResultSuccess() == false)
    {
        MSG_DG_EVENT_SELECT_NAK nmsg;
        nmsg.m_dwKey = user_guid;
        nmsg.m_byErrorCode = 1;
        pServerSession->SendPacket(&nmsg, sizeof(nmsg));
        SelectEventQuery::FREE(query_result);
        return;
    }

    MSG_DG_EVENT_SELECT_ACK msg_ack;
    msg_ack.m_dwKey = user_guid;
    int number_of_records = query_result->GetResultRowNum();
    //                          EVENT_ITEM_TOTAL_INFO_EX::MAX_SLOT_NUM
    ASSERT(number_of_records <= _countof(msg_ack.m_TotalInfo.m_Slot));
    number_of_records = min(number_of_records, _countof(msg_ack.m_TotalInfo.m_Slot));
    //
    const SelectEventQuery::sQUERY* record_it = query_result->pResult;
    EVENT_ITEM_TOTAL_INFO_EX::SLOT_TYPE* dest_it = msg_ack.m_TotalInfo.m_Slot;
    msg_ack.m_TotalInfo.m_Count = static_cast<uint8_t>(number_of_records);
    for (int i = 0; i < number_of_records; ++i, ++dest_it, ++record_it)
    {
        dest_it->m_ItemCode = record_it->m_ItemCode;
        dest_it->m_ItemCount = record_it->m_ItemCount;
        dest_it->m_ItemSeq = record_it->m_ItemSeq;
        dest_it->m_DateTime = record_it->m_DateTime;
    }
    pServerSession->SendPacket(&msg_ack, msg_ack.GetSize());
    SelectEventQuery::FREE(query_result);
    return;
}

Handler_FromGameServer_IMPL( DG_EVENT_UPDATE_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_EVENT_UPDATE_SYN * pRecvMsg = (MSG_DG_EVENT_UPDATE_SYN *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;

    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* pUser = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(UserKey, &pUser);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eFULL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, UserKey);
        return;
    };
    DBCharacter* db_character = pUser->GetConnectedChar();
    if (db_character == NULL) {
        SUNLOG(eFULL_LOG, _T("|[%s]|Can't find user (%u)'s character|"),
               __FUNCTION__, UserKey);
        return;
    };

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString(
		"{?=call S_Event_Update(%d,'%s',%d)}", 
		pUser->GetUserGuid(), pUser->GetConnectedChar()->GetCharName(), pRecvMsg->m_ItemSeq );


	UdpateEventQuery * pQuery = UdpateEventQuery::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetItemPos( pRecvMsg->m_ItemPos );

	pQuery->SetQuery(szQueryBuff);
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_UPDATE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pUser->DBQuery(DG_EVENT, DG_EVENT_UPDATE_DBR, pQuery);
}

Handler_FromGameServer_IMPL( DG_EVENT_UPDATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	UdpateEventQuery * pQResult		= (UdpateEventQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DG_EVENT_UPDATE_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetUserKey();
		nmsg.m_byErrorCode		= 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
		UdpateEventQuery::FREE(pQResult); pQResult = NULL;
		return;
	}
	MSG_DG_EVENT_UPDATE_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_ItemPos = pQResult->GetItemPos();
	pServerSession->SendPacket( &msg, sizeof(msg) );
	UdpateEventQuery::FREE(pQResult); pQResult = NULL;
}


//	오픈베타 에테르 이벤트
//	(WAVERIX)(080904) - 오래된 주석이라 첨삭
//	-	T_EventUser 테이블을 쿼리하는 처리가 포함된 핸들러
Handler_FromGameServer_IMPL( DG_EVENT_ITEM_STATUS_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_EVENT_ITEM_STATUS_SYN* pRecvMsg = (MSG_DG_EVENT_ITEM_STATUS_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
    const DWORD char_guid = pRecvMsg->m_CharGUID;

    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* pUser = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(UserKey, char_guid, &pUser, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, UserKey);
        return;
    };

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_Event_Status(%d)}", char_guid );

	EventStatusQuery* pQuery = EventStatusQuery::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );

	pQuery->SetCharGuid( char_guid );
	pQuery->SetQuery(szQueryBuff);
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_ITEM_STATUS_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pUser->DBQuery(DG_EVENT, DG_EVENT_ITEM_STATUS_DBR, pQuery);
}

Handler_FromGameServer_IMPL( DG_EVENT_ITEM_STATUS_DBR )
{
    __TOUCHED((pServerSession, wSize));
	const DWORD ERRCODE_QUERY_FAILED			= 10;
	const DWORD ERRCODE_CANT_FIND_USER_BY_KEY	= 20;

	const MSG_DBPROXY_RESULT* pResult	= (const MSG_DBPROXY_RESULT *)pMsg;
	EventStatusQuery* pQResult			= (EventStatusQuery *)pResult->m_pData;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
	    IfFailThenBreak(pQResult->ResultSuccess(), ERRCODE_QUERY_FAILED);

		static MSG_DG_EVENT_ITEM_STATUS_ACK msgACK;
		const DWORD resultRowNum = pQResult->GetResultRowNum();

		const DWORD user_key	= pQResult->GetUserKey();
		DBUser* const pUser		= g_DBUserManager.FindUser( user_key );
		IfFailThenBreak(pUser && pUser->GetConnectedChar(), ERRCODE_CANT_FIND_USER_BY_KEY);

		DBCharacter* const pCharacter = pUser->GetConnectedChar();
		ISSQHandlerInDB rSSQHandler;
		if( !rSSQHandler.IsValidState() )
			rSSQHandler.Init();

		BOOST_STATIC_ASSERT( _countof(pQResult->pResult) == _MAX_EVENT_ROW_NUM );

		DWORD inc_index = 0;
		EventStatusQuery::sQUERY* pItSrc = &pQResult->pResult[0];
		EventStatusQuery::sQUERY* pItSrcE= &pQResult->pResult[resultRowNum];
		for( ; (inc_index<_MAX_EVENT_ROW_NUM && pItSrc!=pItSrcE) ; ++pItSrc )
		{
			EventStatusQuery::sQUERY& rResult = *pItSrc;
			if( ISSQHandlerInDB::IsSSQEventID( (DWORD)rResult.m_EventID ) )	// 다중화를 위해서 여기의 함수는 좀더 일반화 되어야 할 것이다.
			{
				rSSQHandler->SetSerializedInfo( rResult );
			}
			else
			{
				EVENT_STATUS_EX& rEventStatus = msgACK.m_EventStatus[inc_index++];
				rEventStatus.m_dwEventID = rResult.m_EventID;
				rEventStatus.m_wRegisterCnt = ( SAFE_NUMERIC_TYPECAST(INT, rResult.m_RegisterCnt, WORD) ) ?
					(WORD)rResult.m_RegisterCnt : 0xFFFF;
				rEventStatus.m_DonateHeimRewardCnt = ( SAFE_NUMERIC_TYPECAST(INT, rResult.m_DonateHeimRewardCnt, BYTE) ) ?
					(BYTE)rResult.m_DonateHeimRewardCnt : 0xFF;	// 단위 구간 하임 기부로 보상받은 회수
				rEventStatus.m_CurDonatedHeims = rResult.m_DonatedHeims; //현재까지 누적된 하임 총액 (모든 라운드가 끝나면 자동 클리어)
				rEventStatus.m_Status = !!rResult.m_Status;	// 완료 여부
			}			
		}

		msgACK.m_wEventNum	= (WORD)inc_index;
		msgACK.m_dwKey		= user_key;

		pServerSession->SendPacket( &msgACK, msgACK.GetSize() );

		{	// 타락한 사원 관련 보상 정보가 있을 경우 처리
			MSG_DG_EVENT_SSQ_REWARD_EXIST_CMD msgCMD;
			msgCMD.m_dwKey	= user_key;
			if( rSSQHandler->GetRewardInfo( msgCMD.m_RewardInfo ) )
			{
				msgCMD.m_CharGuid = pCharacter->GetCharGuid();
				pServerSession->SendPacket( &msgCMD, msgCMD.GetSize() );
			}
		};

		EventStatusQuery::FREE(pQResult); pQResult = NULL;
		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	switch( flow.GetErrorCode() )
	{
	case ERRCODE_CANT_FIND_USER_BY_KEY:
		SUNLOG(eFULL_LOG, "["__FUNCTION__"] [오류]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", pQResult->GetUserKey() );
        EventStatusQuery::FREE(pQResult); pQResult = NULL;
		break;
	case ERRCODE_QUERY_FAILED:
	default:
		{
			MSG_DG_EVENT_ITEM_STATUS_NAK msgNAK;
			msgNAK.m_dwKey = pQResult->GetUserKey();
			pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
			EventStatusQuery::FREE(pQResult); pQResult = NULL;
		}
		break;
	}
	return /*RETURN FALSE VALUE*/;
}

// 이벤트 아이템 등록 신청 처리
Handler_FromGameServer_IMPL( DG_EVENT_ITEM_REG_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_EVENT_ITEM_REG_SYN* pRecvMsg = (MSG_DG_EVENT_ITEM_REG_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[Handler_FromGameServer_IMPL(DG_EVENT_ITEM_REG_SYN)] [오류]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}

	STRING_SAFER_QUERY256 szQueryBuff;
	EVENT_STATUS_EX& rEVENT_STATUS = pRecvMsg->m_EventStatus;
	szQueryBuff.MakeString(
		"{?=call S_Event_Status_Reg(%d,%d,%d,%d,%d,%d)}"
		, pRecvMsg->m_dwCharGUID //순서는 갑자기 왜 바꿨댜~ -.- (waverix)
		, rEVENT_STATUS.m_dwEventID
		, rEVENT_STATUS.m_wRegisterCnt
		, rEVENT_STATUS.m_DonateHeimRewardCnt
		, rEVENT_STATUS.m_CurDonatedHeims
		, rEVENT_STATUS.m_Status
		);

	EventItemRegQuery* pQuery = EventItemRegQuery::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetEventID( pRecvMsg->m_EventStatus.m_dwEventID );
	pQuery->SetCharGuid( pRecvMsg->m_dwCharGUID );

	pQuery->SetQuery(szQueryBuff);
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_ITEM_REG_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pUser->DBQuery(DG_EVENT, DG_EVENT_ITEM_REG_DBR, pQuery);
}

// 이벤트 아이템 등록 결과 처리
Handler_FromGameServer_IMPL( DG_EVENT_ITEM_REG_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	EventItemRegQuery * pQResult	= (EventItemRegQuery *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DG_EVENT_ITEM_REG_NAK msg;

		msg.m_dwKey = pQResult->GetUserKey();
		msg.m_dwEventID = pQResult->GetEventID();
		pServerSession->SendPacket( &msg, sizeof(msg) );
		SUNLOG( eCRITICAL_LOG, "[DG_EVENT_ITEM_REG_DBR][오류]:쿼리에 실패(%s)", pQResult->GetQuery() );
		EventItemRegQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DG_EVENT_ITEM_REG_ACK msg;

	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_dwEventID = pQResult->GetEventID();
	pServerSession->SendPacket( &msg, sizeof(msg) );
	EventItemRegQuery::FREE(pQResult); pQResult = NULL;
}

Handler_FromGameServer_IMPL( DG_EVENT_QUERY_EXIST_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_EVENT_QUERY_EXIST_SYN* pRecvMsg = (MSG_DG_EVENT_QUERY_EXIST_SYN*)pMsg;
	const DWORD userKey = pRecvMsg->m_dwKey;
	DBUser* pUser = g_DBUserManager.FindUser( userKey );

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(pUser != 0, 10);

		IfFailThenBreak(pUser->GetConnectedChar(), 20);

		STRING_SAFER_QUERY128 szQueryBuff;
		szQueryBuff.MakeString(
			"{?=call S_Event_Select(%d,'%s')}",
			pUser->GetUserGuid(), pUser->GetConnectedChar()->GetCharName() );


		SelectEventQuery* pQuery = SelectEventQuery::ALLOC();
		pQuery->SetUserKey( pRecvMsg->m_dwKey );

		pQuery->SetQuery(szQueryBuff);
		//pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_QUERY_EXIST_DBR ) );
		pQuery->SetVoidObject( pServerSession );
		pUser->DBQuery(DG_EVENT, DG_EVENT_QUERY_EXIST_DBR, pQuery);

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	switch( flow.GetErrorCode() )
	{
	case 10: case 20:
		SUNLOG( eFULL_LOG, "[DG_EVENT_QUERY_EXIST_SYN] [오류]:[ID:%u]:%s", userKey, flow.GetErrorString() );
		break;
	default:
		SUNLOG( eCRITICAL_LOG, _T("Undefined Error %u"), flow.GetErrorCode() );
		break;
	}
}

Handler_FromGameServer_IMPL( DG_EVENT_QUERY_EXIST_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT *)pMsg;
	SelectEventQuery* pQResult = (SelectEventQuery *)pResult->m_pData;

	MSG_DG_EVENT_QUERY_EXIST_ACK msgACK;
	msgACK.m_dwKey = pQResult->GetUserKey();
	if( pQResult->ResultSuccess() )
	{
		msgACK.m_bExist = pQResult->GetResultRowNum()
			?	MSG_DG_EVENT_QUERY_EXIST_ACK::EXIST
			:	MSG_DG_EVENT_QUERY_EXIST_ACK::EMPTY
			;
	}
	else
	{
		msgACK.m_bExist = MSG_DG_EVENT_QUERY_EXIST_ACK::DISABLED;
	}

	pServerSession->SendPacket( &msgACK, sizeof(msgACK) );
	SelectEventQuery::FREE(pQResult);
	pQResult = NULL;

	return;
}

Handler_FromGameServer_IMPL( DG_EVENT_STATUS_UPDATE_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_EVENT_STATUS_UPDATE_SYN* pRecvMsg = (MSG_DG_EVENT_STATUS_UPDATE_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	EVENT_STATUS_EX& rEventStatus = pRecvMsg->m_EventStatus;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
	    IfFailThenBreak(pUser != 0, 10);

		DBCharacter* pCharacter = pUser->GetConnectedChar();
		IfFailThenBreak(pCharacter != 0, 20);

		STRING_SAFER_QUERY128 szQueryBuff;
		szQueryBuff.MakeString(
			"{?=call S_Event_Status_Reg(%d,%d,%d,%d,%d,%d)}",
			pCharacter->GetCharGuid(),
			rEventStatus.m_dwEventID,
			rEventStatus.m_wRegisterCnt,
			rEventStatus.m_DonateHeimRewardCnt,
			rEventStatus.m_CurDonatedHeims,
			rEventStatus.m_Status
			);

		EventItemRegQuery* pQuery = EventItemRegQuery::ALLOC();
		pQuery->SetUserKey( UserKey );
		pQuery->SetEventID( rEventStatus.m_dwEventID );
		pQuery->SetCharGuid( pCharacter->GetCharGuid() );

		pQuery->SetQuery( szQueryBuff );
		//pQuery->SetIndex( MAKEDWORD( DG_EVENT, DG_EVENT_STATUS_UPDATE_DBR ) );
		pQuery->SetVoidObject( pServerSession );
		pUser->DBQuery(DG_EVENT, DG_EVENT_STATUS_UPDATE_DBR, pQuery);

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	switch( flow.GetErrorCode() )
	{
	case 10: case 20:
		SUNLOG( eCRITICAL_LOG, "[%s][ERROR:%u][ID:%u] Can't Find User-or-Character", __FUNCTION__, flow.GetErrorCode(), UserKey );
		break;
	default:
		SUNLOG( eCRITICAL_LOG, "Unexpected Status %u", flow.GetErrorCode() );
		break;
	}
	return /*RETURN FALSE VALUE*/;
}

Handler_FromGameServer_IMPL( DG_EVENT_STATUS_UPDATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	EventItemRegQuery* pQResult	= (EventItemRegQuery *)pResult->m_pData;

	BOOL bFailed = !pQResult->ResultSuccess();

	MSG_DG_EVENT_STATUS_UPDATE_ACK msgACK;
	msgACK.m_dwKey = pQResult->GetUserKey();
	msgACK.m_EventID = pQResult->GetEventID();
	pServerSession->SendPacket( &msgACK, sizeof(msgACK) );

	if( bFailed )
		SUNLOG( eCRITICAL_LOG, "[%s][ERROR] Query Failed (%s)", __FUNCTION__, pQResult->GetQuery() );

	EventItemRegQuery::FREE(pQResult); pQResult = NULL;
}


Handler_FromGameServer_IMPL( DG_EVENT_SSQ_REWARD_UPDATE_SYN )
{
    __TOUCHED((pServerSession, wSize));
	const DWORD ERRCODE_CANT_FIND_USER	= 100;
	const DWORD ERRCODE_INVALID_INFO	= 101;
	const DWORD ERRCODE_CANT_MAKE_QUERY	= 102;

	MSG_DG_EVENT_SSQ_REWARD_UPDATE_SYN* pRecvMsg = static_cast<MSG_DG_EVENT_SSQ_REWARD_UPDATE_SYN*>(pMsg);
	const DWORD user_key = pRecvMsg->m_dwKey;
	const DWORD char_guid = pRecvMsg->m_CharGuid;

	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		DBUser* const pUser = g_DBUserManager.FindUser( user_key );
		IfFailThenBreak(pUser != 0, ERRCODE_CANT_FIND_USER);

		STRING_SAFER_QUERY256 szQueryBuff;
		SSQHandlerInDB handler;

		SSQ_RESERVED_REWARD_INFO& rInfo = pRecvMsg->m_RewardInfo;
		IfFailThenBreak(handler.SetRewardInfo(rInfo), ERRCODE_INVALID_INFO);
		IfFailThenBreak(handler.MakeQuery(szQueryBuff, char_guid), ERRCODE_CANT_MAKE_QUERY);

		EventItemRegQuery* const pQuery = EventItemRegQuery::ALLOC();
		pQuery->SetUserKey( user_key );
		pQuery->SetEventID( rInfo.m_SharedInfo.EventID );
		pQuery->SetCharGuid( char_guid );

		pQuery->SetQuery( szQueryBuff );
		//pQuery->SetIndex( MAKEDWORD( (WORD)DG_EVENT, (WORD)DG_EVENT_SSQ_REWARD_UPDATE_DBR ) );
		pQuery->SetVoidObject( pServerSession );
		pUser->DBQuery(DG_EVENT, DG_EVENT_SSQ_REWARD_UPDATE_DBR, pQuery);

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	switch( flow.GetErrorCode() )
	{
	case ERRCODE_CANT_FIND_USER:
		SUNLOG( eFULL_LOG, "["__FUNCTION__"] [오류]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", user_key );
		break;
	default:
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"][%u] %s", flow.GetErrorCode(), flow.GetErrorString() );
		break;
	}
	
	return /*RETURN FALSE VALUE*/;
}


Handler_FromGameServer_IMPL( DG_EVENT_SSQ_REWARD_UPDATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	const DWORD ERRCODE_QUERY_FAILED			= 10;
	const DWORD ERRCODE_CANT_FIND_USER_BY_KEY	= 20;

	const MSG_DBPROXY_RESULT* pResult	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
	EventItemRegQuery* pQResult			= (EventItemRegQuery *)pResult->m_pData;

	MSG_DG_EVENT_SSQ_REWARD_UPDATE_ACK msgACK;
	const DWORD user_key = pQResult->GetUserKey();
	const DWORD char_key = pQResult->GetCharGuid();
	msgACK.m_dwKey		= user_key;
	msgACK.m_CharGuid	= char_key;

	const BOOLEAN isSuccess = !!pQResult->ResultSuccess();
	if( !isSuccess )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"][오류]:쿼리에 실패(%s)", pQResult->GetQuery() );
	}

	msgACK.m_UpdateResult = isSuccess;

	pServerSession->SendPacket( &msgACK, msgACK.GetSize() );
	EventItemRegQuery::FREE(pQResult); pQResult = NULL;
}

// handled by _KR_0_20091021_FIRENDINVITATION_EXTEND = { DG_EVENT_INVITATEFRIEND~ }

Handler_FromGameServer_IMPL( DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN )
{
    DFriendInvitationController::Instance()->OnRewardItemUpdateSYN(pServerSession, pMsg, wSize);
}

Handler_FromGameServer_IMPL( DG_EVENT_INVITATEFRIEND_REWARDUPDATE_DBR )
{
    DFriendInvitationController::Instance()->OnRewardItemUpdateDBR(pServerSession, pMsg, wSize);
}

Handler_FromGameServer_IMPL( DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN )
{
    DFriendInvitationController::Instance()->OnRecallInfoSelectSYN(pServerSession, pMsg, wSize);
}

Handler_FromGameServer_IMPL( DG_EVENT_INVITATEFRIEND_RECALLSELECT_DBR )
{
    DFriendInvitationController::Instance()->OnRecallInfoSelectDBR(pServerSession, pMsg, wSize);
}

Handler_FromGameServer_IMPL( DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD )
{
    DFriendInvitationController::Instance()->OnRecallInfoUpdateCMD(pServerSession, pMsg, wSize);
}

Handler_FromGameServer_IMPL( DG_EVENT_INVITATEFRIEND_RECALLUPDATE_DBR )
{
    DFriendInvitationController::Instance()->OnRecallInfoUpdateDBR(pServerSession, pMsg, wSize);
}

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
Handler_FromGameServer_IMPL(DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN* recv_msg = 
        static_cast<MSG_DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN*>(pMsg);

    // NOTE: f110405.5L, currently no problems about data usage case study.
    // however, we can't ensure to stable now and for ever.
    DominationAuctionInfo temp_db_info;
    DOMINATION_ACTION_INFO& temp_info = temp_db_info.m_ActionInfo;
   
    temp_db_info.m_MasterGuid = recv_msg->char_guid_;
    temp_info.m_FieldCode = recv_msg->map_code_;
    temp_info.m_TenderCost = recv_msg->tender_cost_;

    g_GuildManagerInDBP.DominationReturnMoneyUpdate(pServerSession, temp_db_info, 
                                                 recv_msg->reward_type_);
}

Handler_FromGameServer_IMPL(DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN* recv_msg = 
        static_cast<MSG_DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN*>(pMsg);

    TCHAR query_string[MAX_QUERY_LENGTH_64+1];
    _sntprintf(query_string, MAX_QUERY_LENGTH_64, 
        "{?=call S_DominationInfo_ActionMoney_Delete(%d,%d)}", 
        recv_msg->reward_area_mapcode_,
        recv_msg->recv_char_guid_);
    query_string[MAX_QUERY_LENGTH_64]='\0';

    Query_Domination_Reward_And_Delete* query = Query_Domination_Reward_And_Delete::ALLOC();

    query->SetMemoGuid(recv_msg->memo_guid_);
    query->SetCharGuid(recv_msg->recv_char_guid_);
    query->SetRewardType(recv_msg->reward_type_);
    query->SetMapCode(recv_msg->reward_area_mapcode_);
    query->SetQuery(query_string);
    query->SetIndex(MAKEDWORD((WORD)DG_EVENT, (WORD)DG_EVENT_DOMINATION_REWARD_AND_DEL_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_EVENT_DOMINATION_REWARD_AND_DEL_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_Domination_Reward_And_Delete* query_result = 
        static_cast<Query_Domination_Reward_And_Delete*>(db_result->m_pData);

    if (query_result->IsError()) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
            query_result->GetQuery(), query_result->GetErrorMsg());

        Query_Domination_Reward_And_Delete::FREE(query_result);
        return;
    }

    const Query_Domination_Reward_And_Delete::sQUERY* result_record  = query_result->pResult;

    MSG_DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK ack_msg;
    ack_msg.result_value_ = query_result->pParam->result_value_;
    ack_msg.memo_guid_ = query_result->GetMemoGuid();
    ack_msg.db_result_map_code_ = result_record->result_map_code_;
    ack_msg.request_map_code_ = query_result->GetMapCode();
    ack_msg.db_result_char_guid_ = result_record->result_char_guid_;
    ack_msg.request_char_guid_ = query_result->GetCharGuid();
    ack_msg.db_result_reward_type_ = 
        static_cast<eSYSTEM_MEMO_TYPE>(result_record->result_reward_type_);
    ack_msg.request_reward_type_ = query_result->GetRewardType();
    ack_msg.tender_cost_ = result_record->result_amount_cost_;
    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));

    Query_Domination_Reward_And_Delete::FREE(query_result);

    return;
}
Handler_FromGameServer_IMPL(DG_EVENT_LOTTO_REWARD_AND_DEL_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_LOTTO_REWARD_AND_DEL_SYN* recv_msg = 
        static_cast<MSG_DG_EVENT_LOTTO_REWARD_AND_DEL_SYN*>(pMsg);

    TCHAR query_string[MAX_QUERY_LENGTH_64+1];
    _sntprintf( query_string, MAX_QUERY_LENGTH_64, "{?=call S_LottoNotice_Delete(%d,?,?)}", 
        recv_msg->lotto_entry_index_);
    query_string[MAX_QUERY_LENGTH_64]='\0';

    Query_Lotto_Reward_And_Delete* query = Query_Lotto_Reward_And_Delete::ALLOC();

    query->SetQuery(query_string);
    query->SetCharGuid(recv_msg->request_char_guid_);
    query->SetMemoGuid(recv_msg->memo_guid_);
    query->SetIndex(MAKEDWORD((WORD)DG_EVENT, (WORD)DG_EVENT_LOTTO_REWARD_AND_DEL_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}
Handler_FromGameServer_IMPL(DG_EVENT_LOTTO_REWARD_AND_DEL_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Lotto_Reward_And_Delete* query_result = 
        static_cast<Query_Lotto_Reward_And_Delete*>(db_result->m_pData);

    if (query_result->IsError()) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
            query_result->GetQuery(), query_result->GetErrorMsg());

        Query_Lotto_Reward_And_Delete::FREE(query_result);
        return;
    }

    MSG_DG_EVENT_LOTTO_REWARD_AND_DEL_ACK ack_msg;
    ack_msg.result_value_ = query_result->pParam->result_value_;
    ack_msg.request_char_guid_ = query_result->GetCharGuid();
    ack_msg.db_result_char_guid_ = 
        (ack_msg.result_value_ == 0 ? query_result->pParam->result_char_guid_ : 0);
    ack_msg.db_result_prize_money_ = 
        (ack_msg.result_value_ == 0 ? query_result->pParam->result_prize_ : 0);
    ack_msg.memo_guid_ = query_result->GetMemoGuid();

    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));

    Query_Lotto_Reward_And_Delete::FREE(query_result);
}
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

// implmented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
Handler_FromGameServer_IMPL(DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD* recv_msg = 
        static_cast<MSG_DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD*>(pMsg);

    TCHAR query_buffer[MAX_QUERY_LENGTH_128];
    _sntprintf(query_buffer, MAX_QUERY_LENGTH_128, 
               "{?=call S_Memo_RewardInfo_Update(%I64d,%d,%d,%d,%I64d)}",
               recv_msg->memo_guid_,
               recv_msg->char_guid_,
               recv_msg->read_stat_,
               recv_msg->reward_item_code_,
               recv_msg->reward_item_num_);

    Query_System_Memo_Reward_Item_Update_Select* query = 
        Query_System_Memo_Reward_Item_Update_Select::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_EVENT, (WORD)DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_System_Memo_Reward_Item_Update_Select* query_result = 
        static_cast<Query_System_Memo_Reward_Item_Update_Select*>(db_result->m_pData);

    //-- 에러 처리
    if (query_result->IsError() || !query_result->ResultSuccess()) 
    {
        if(query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
                query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]System Memo Reward Info Update Fail");
        }
        Query_System_Memo_Reward_Item_Update_Select::FREE(query_result);
        return;
    }

    Query_System_Memo_Reward_Item_Update_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN* recv_msg = 
        static_cast<MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buffer;

    query_buffer.MakeString("{?=call S_Memo_Reward_Item_Select(%I64d,%d)}",
                            recv_msg->memo_guid_,recv_msg->char_guid_);

    Query_System_Memo_Reward_Item_Check_Select* query = 
        Query_System_Memo_Reward_Item_Check_Select::ALLOC();

    query->SetCharGuid(recv_msg->char_guid_);
    query->SetMemoGuid(recv_msg->memo_guid_);
    query->SetMemoContents(recv_msg->memo_contents_); //_NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_EVENT, (WORD)DG_EVENT_SYSTEMMEMO_ITEM_CHECK_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_EVENT_SYSTEMMEMO_ITEM_CHECK_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_System_Memo_Reward_Item_Check_Select* query_result = 
        static_cast<Query_System_Memo_Reward_Item_Check_Select*>(db_result->m_pData);

    //-- 에러 처리
    if (query_result->IsError() || !query_result->ResultSuccess()) 
    {
        if(query_result->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
                query_result->GetQuery(), query_result->GetErrorMsg());
        }
        else
        {
            MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK nak_msg;
            {
                nak_msg.db_result_value_ = query_result->ResultSuccess();
                nak_msg.memo_guid_ = 0;
                nak_msg.char_guid_ = 0;
                nak_msg.memo_type_ = NORMAL_MEMO;
                nak_msg.reward_item_code_ = 0;
                nak_msg.reward_item_num_ = 0;
                nak_msg.memo_contents_[0] = '\0';
            }
            pServerSession->SendPacket(&nak_msg, sizeof(nak_msg));
        }
        Query_System_Memo_Reward_Item_Check_Select::FREE(query_result);
        return;
    }

    //-- 응답
    MSG_DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK ack_msg;
    {
        ack_msg.db_result_value_ = query_result->ResultSuccess();
        ack_msg.memo_guid_ = query_result->GetMemoGuid();
        ack_msg.char_guid_ = query_result->GetCharGuid();
        ack_msg.memo_type_ = query_result->result_records_->system_memo_type_;
        ack_msg.reward_item_code_ = query_result->result_records_->recv_item_code_;
        ack_msg.reward_item_num_ = query_result->result_records_->amount_cost_;
        _tcsncpy(ack_msg.memo_contents_, query_result->GetMemoContents(),
                 _countof(ack_msg.memo_contents_));
        ack_msg.memo_contents_[_countof(ack_msg.memo_contents_) - 1] = '\0';
    }
    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));

    Query_System_Memo_Reward_Item_Check_Select::FREE(query_result);
}

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
Handler_FromGameServer_IMPL(DG_EVENT_SMARTNPC_SYSTEM)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_SMARTNPC_SYSTEM* request = 
        static_cast<MSG_DG_EVENT_SMARTNPC_SYSTEM*>(pMsg);

    const DWORD user_key = request->m_dwKey;
    DBUser* const dbuser = g_DBUserManager.FindUser(user_key);
    if (dbuser == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
            __FUNCTION__, user_key);
        return;
    }
    else if(dbuser->GetConnectedChar() == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
            __FUNCTION__, user_key);
        return;
    }

    switch(request->packet_type)
    {
        case DG_SMARTNPC_SET_ATTENDANCE_SYN: 
        {
            const MSG_SMARTNPC_SET_ATTENDANCE_SYN* packet = \
                static_cast<MSG_SMARTNPC_SET_ATTENDANCE_SYN*>(pMsg);

            STRING_SAFER_QUERY64 query_string;
            query_string.MakeString(_T("{? = call S_Attendance_Update (%d, %d, %d, %d)}"), \
                dbuser->GetUserGuid(), dbuser->GetConnectedChar()->GetCharGuid(),
                packet->attendance_type, packet->attendance_data);

            Query_Attendance_Update* query = Query_Attendance_Update::ALLOC();
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);
            query->CID = packet->CID;
            query->NID = packet->NID;
            query->EID = packet->EID;
            query->STEP = packet->STEP;

            dbuser->DBQuery(DG_EVENT, DG_EVENT_SET_ATTENDANCE_DBR, query);
        } break;
        case DG_SMARTNPC_GET_ATTENDANCE_SYN: 
            {
                const MSG_SMARTNPC_GET_ATTENDANCE_SYN* packet = \
                    static_cast<MSG_SMARTNPC_GET_ATTENDANCE_SYN*>(pMsg);

                STRING_SAFER_QUERY64 query_string;
                query_string.MakeString(_T("{? = call S_Attendance_Select (%d, %d, %d, %d)}"), \
                    dbuser->GetUserGuid(), dbuser->GetConnectedChar()->GetCharGuid(),
                    packet->attendance_type, packet->attendance_time);

                Query_Attendance_Update* query = Query_Attendance_Update::ALLOC();
                query->SetUserKey(user_key);
                query->SetQuery(query_string);
                query->SetVoidObject(pServerSession);
                query->CID = packet->CID;
                query->NID = packet->NID;
                query->EID = packet->EID;
                query->STEP = packet->STEP;

                dbuser->DBQuery(DG_EVENT, DG_EVENT_GET_ATTENDANCE_DBR, query);
            } break;
    }
}

Handler_FromGameServer_IMPL(DG_EVENT_SET_ATTENDANCE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_Attendance_Update* query_result = \
        static_cast<Query_Attendance_Update*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_Attendance_Update::FREE(query_result);
        return;
    }

    const DWORD user_guid = query_result->GetUserKey();
    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL) {
        return;
    }

    MSG_SMARTNPC_SET_ATTENDANCE_ACK msg(\
        query_result->CID, query_result->NID, query_result->EID, query_result->STEP);
    msg.result = query_result->parameters.result_;
    db_user->SendPacketToGameServer(&msg, sizeof(msg));

    Query_Attendance_Update::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_EVENT_GET_ATTENDANCE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_Attendance_Update* query_result = \
        static_cast<Query_Attendance_Update*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_Attendance_Update::FREE(query_result);
        return;
    }

    const DWORD user_guid = query_result->GetUserKey();
    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL) {
        return;
    }

    MSG_SMARTNPC_GET_ATTENDANCE_ACK msg(\
        query_result->CID, query_result->NID, query_result->EID, query_result->STEP);
    msg.result = query_result->parameters.result_;
    db_user->SendPacketToGameServer(&msg, sizeof(msg));

    Query_Attendance_Update::FREE(query_result);
}
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
Handler_FromGameServer_IMPL(DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN)
{ 
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN* request = 
        static_cast<MSG_DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_buffer;

    query_buffer.MakeString("{?=call S_Attendance_Update_Mondschien(%d)}", request->guild_guid);

    Query_Attendance_Update_Mondschien* query = 
        Query_Attendance_Update_Mondschien::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_EVENT, (WORD)DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_DBR)
{ 
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_Attendance_Update_Mondschien* query_result = \
        static_cast<Query_Attendance_Update_Mondschien*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_Attendance_Update_Mondschien::FREE(query_result);
        return;
    }

    Query_Attendance_Update_Mondschien::FREE(query_result);
}
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
Handler_FromGameServer_IMPL(DG_GMEVENT_PEAKTIME_REGISTER_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_GMEVENT_PEAKTIME_REGISTER_SYN* syn_msg = 
          static_cast<MSG_DG_GMEVENT_PEAKTIME_REGISTER_SYN*>(pMsg);

    MSG_DG_GMEVENT_PEAKTIME_REGISTER_ACK ack_msg;
    ack_msg.register_event = syn_msg->register_event;

    g_DBProxyServer.SendToSessionType(GAME_SERVER, &ack_msg, sizeof(ack_msg));
}

Handler_FromGameServer_IMPL(DG_GMEVENT_PEAKTIME_TERMINATE_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_GMEVENT_PEAKTIME_TERMINATE_SYN* syn_msg = 
        static_cast<MSG_DG_GMEVENT_PEAKTIME_TERMINATE_SYN*>(pMsg);

    MSG_DG_GMEVENT_PEAKTIME_TERMINATE_ACK ack_msg;

    g_DBProxyServer.SendToSessionType(GAME_SERVER, &ack_msg, sizeof(ack_msg));
}
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT