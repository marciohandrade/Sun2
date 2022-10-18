#include "StdAfx.h"
#include ".\handler_fromworldserver.h"
#include "QueryObjects/WorldServerQuery.h"
#include <PacketStruct_DG.h>
#include <PacketStruct_DW.h>
#include "Sessions/ChildServerSession.h"
#include <TimeConverter.h>
#include "DBUser.h"
#include "DBUserManager.h"
#include "PacketHandler/Handler_FriendInvitation.h" //친구추천 기능때문에 쓰인다.

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_FromWorldServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_FromWorldServer::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_FromWorldServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(DW_CONNECTION, DW_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
        //
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_GETLIST_SYN, PHASE_UNLIMITED),    //< 친구 목록
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_ADD_SYN, PHASE_UNLIMITED),        //< A->B add friend
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DEL_SYN, PHASE_UNLIMITED),        //< 친구 삭제
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_SYN, PHASE_UNLIMITED),      //< 유저 차단
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_FREE_SYN, PHASE_UNLIMITED), //< 차단 삭제
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_GETLIST_DBR, PHASE_UNLIMITED),//< 친구 목록

        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_ADD_DBR, PHASE_UNLIMITED),    //< A->B add friend
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_DEL_DBR, PHASE_UNLIMITED),    //< 친구 삭제
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_BLOCK_DBR, PHASE_UNLIMITED),  //< 유저 차단
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_BLOCK_FREE_DBR, PHASE_UNLIMITED), //< 차단 삭제

        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_SYN, PHASE_UNLIMITED),            // 채팅금지
        HANDLER_NODE_INFO_DBR(DW_GM, DW_GM_CHATBLOCK_DBR, PHASE_UNLIMITED),        // 채팅금지
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATALLOW_SYN, PHASE_UNLIMITED),            // 채팅허용
        HANDLER_NODE_INFO_DBR(DW_GM, DW_GM_CHATALLOW_DBR, PHASE_UNLIMITED),        // 채팅허용
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_UPDATE_SYN, PHASE_UNLIMITED),     // 채팅금지 시간 차감
        HANDLER_NODE_INFO_DBR(DW_GM, DW_GM_CHATBLOCK_UPDATE_DBR, PHASE_UNLIMITED), // 채팅금지 시간 차감
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCKTIME_REQ, PHASE_UNLIMITED),        // 채팅금지 시간 요청
        HANDLER_NODE_INFO_DBR(DW_GM, DW_GM_CHATBLOCKTIME_DBR, PHASE_UNLIMITED),    // 채팅금지 시간 요청
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_SYN, PHASE_UNLIMITED), //< Add Revenge Char
        HANDLER_NODE_INFO_DBR(DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_DBR, PHASE_UNLIMITED), //< Add Revenge Char
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_SYN, PHASE_UNLIMITED),  //< Remove Revenge Char
        HANDLER_NODE_INFO_DBR(DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_DBR, PHASE_UNLIMITED), //< Remove Revenge Char
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_SYN, PHASE_UNLIMITED),  //< PK Success - Remove Revenge Char
        HANDLER_NODE_INFO_DBR(DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_DBR, PHASE_UNLIMITED),  //< PK Success - Remove Revenge Char

        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_SEND_SYN, PHASE_UNLIMITED),     // 쪽지 전송
        HANDLER_NODE_INFO_DBR(DW_MEMO, DW_MEMO_SEND_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_LIST_REQ, PHASE_UNLIMITED),     // 목록 확인
        HANDLER_NODE_INFO_DBR(DW_MEMO, DW_MEMO_LIST_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_VIEW_REQ, PHASE_UNLIMITED),     // 쪽지 확인
        HANDLER_NODE_INFO_DBR(DW_MEMO, DW_MEMO_VIEW_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_DELETE_SYN, PHASE_UNLIMITED),   // 쪽지 삭제
        HANDLER_NODE_INFO_DBR(DW_MEMO, DW_MEMO_DELETE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_STATUS, DW_STATUS_INTERCEPTION_REQ, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_STATUS, DW_STATUS_INTERCEPTION_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_REQ, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_SELECT_NOTICE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_GUILD, DW_GUILD_SELECT_NOTICE_DBR, PHASE_UNLIMITED),
        //
        HANDLER_NODE_INFO(DW_GUILD, DW_DOMINATION_FINDMONEY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_GUILD, DW_DOMINATION_FINDMONEY_DBR, PHASE_UNLIMITED),
        //#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
        HANDLER_NODE_INFO(DW_COMMUNITY, DW_COMMUNITY_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_COMMUNITY, DW_COMMUNITY_MYLIST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_COMMUNITY, DW_COMMUNITY_INFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_COMMUNITY, DW_COMMUNITY_MYLIST_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_COMMUNITY, DW_COMMUNITY_CONTACT_ME_DBR, PHASE_UNLIMITED),
        //#endif 
        //{__NA001254_20090109_RTTG_FUNC_ADD
        HANDLER_NODE_INFO(DW_MEMO, DW_MULTIMEMO_SEND_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_MEMO, DW_MULTIMEMO_SEND_DBR, PHASE_UNLIMITED),
        //}
        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_ADD_SYN, PHASE_UNLIMITED),           //그룹 맴버 추가
        HANDLER_NODE_INFO_DBR(DW_GROUP, DW_GROUP_ADD_DBR, PHASE_UNLIMITED),       //그룹 맴버 추가 DB작업 결과

        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_DEL_SYN, PHASE_UNLIMITED),           //그룹 맴버 삭제
        HANDLER_NODE_INFO_DBR(DW_GROUP, DW_GROUP_DEL_DBR, PHASE_UNLIMITED),       //그룹 맴버 삭제 DB작업 결과

        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_LIST_REQ, PHASE_UNLIMITED),          //그룹 맴버 리스트 요청
        HANDLER_NODE_INFO_DBR(DW_GROUP, DW_GROUP_LIST_DBR, PHASE_UNLIMITED),      //그룹 맴버 리스트 DB작업 결과

        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_ACCUMEXP_CMD, PHASE_UNLIMITED),      //경험치 축적 요청
        HANDLER_NODE_INFO_DBR(DW_GROUP, DW_GROUP_ACCUMEXP_DBR, PHASE_UNLIMITED),  //경험치 축적 DB작업 결과

        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_INIT_ACCUMEXP_CMD, PHASE_UNLIMITED), //경험치 축적 요청
        HANDLER_NODE_INFO_DBR(DW_GROUP, DW_GROUP_INIT_ACCUMEXP_DBR, PHASE_UNLIMITED), //경험치 축적 DB작업 결과

        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_SET_MEMBERINFO_CMD, PHASE_UNLIMITED),
        //------------------------------------------------------------------------------------------------------------

        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_LOAD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_LOAD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_WINNER_LIST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_WINNER_LIST_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ROLLOVER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_ROLLOVER_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ACCUM_NTF, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_ACCUM_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ADD_TICKET_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_ADD_TICKET_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_REWARD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_REWARD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_NOTICE_LIST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_NOTICE_LIST_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_NOTICE_DEL_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_NOTICE_DEL_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_PAYMENT_UPDATE_NTF, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_LOTTO, DW_LOTTO_PAYMENT_UPDATE_DBR, PHASE_UNLIMITED),
        //}
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_INVITATIONRELATION_FREE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_INVITATIONRELATION_FREE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_INVITATIONRELATION_REWARDINSERT_DBR, PHASE_UNLIMITED),
        //}
        //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DETAIL_INFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_DATAIL_INFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_MEMO_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DW_FRIEND, DW_FRIEND_MEMO_UPDATE_DBR, PHASE_UNLIMITED),
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================

Handler_FromWorldServer_IMPL(DW_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

//기존 월드서버의 친구 목록 요청
Handler_FromWorldServer_IMPL( DW_FRIEND_GETLIST_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_FRIEND_GETLIST_SYN * pRecvMsg = (MSG_DW_FRIEND_GETLIST_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_CharRelation_Select(%d)}" , pRecvMsg->m_CharGuid );
	szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

	Query_Friend_SelectList * pQuery = Query_Friend_SelectList::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_GETLIST_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_FRIEND_GETLIST_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Friend_SelectList * pQResult	= (Query_Friend_SelectList *)pResult->m_pData;

	if( pQResult->GetResultRowNum() == 0 )
	{
		MSG_DW_FRIEND_GETLIST_NAK nmsg;
		nmsg.m_CharGuid	= pQResult->GetCharGuid();
		nmsg.m_byResult	= 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		Query_Friend_SelectList::FREE(pQResult); pQResult = NULL;
		return;
	}

	
	MSG_DW_FRIEND_GETLIST_ACK msg;
	msg.m_CharGuid		= pQResult->GetCharGuid();
    msg.number_of_friends = pQResult->GetResultRowNum();

#ifdef _DEBUG
	ASSERT( msg.number_of_friends <= MAX_FRIENDBLOCK_STAT_NUM );
#else
	if( msg.number_of_friends > MAX_FRIENDBLOCK_STAT_NUM )
	{
		msg.number_of_friends   = MAX_FRIENDBLOCK_STAT_NUM ;
		MessageOut( eCRITICAL_LOG, "Friend_GetList Error - Friend Number Overflow[%u]", msg.number_of_friends );
	}
#endif


	for( int i = 0 ; i < msg.number_of_friends ; ++i )
	{
		if( pQResult->pResult[i].m_Sts == STRUCT_FRIEND_DB_INFO::INFO_REVENGE_POINT )
		{
			msg.m_sFriendInfo[i].RevengePoint	= pQResult->pResult[i].m_FriendCharGuid;
			msg.m_sFriendInfo[i].Sts			= pQResult->pResult[i].m_Sts;
			continue;
		}
		else
		{
			msg.m_sFriendInfo[i].dwFriendGuid	= pQResult->pResult[i].m_FriendCharGuid;
		}
		memcpy( msg.m_sFriendInfo[i].ptszFriendName, pQResult->pResult[i].m_ptszFriendCharName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) );
		msg.m_sFriendInfo[i].ptszFriendName[MAX_CHARNAME_LENGTH] = '\0';
		msg.m_sFriendInfo[i].byClass		= pQResult->pResult[i].m_byClass;
		msg.m_sFriendInfo[i].Level			= pQResult->pResult[i].m_Level;
		msg.m_sFriendInfo[i].Sts			= pQResult->pResult[i].m_Sts;
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        msg.m_sFriendInfo[i].gender         = pQResult->pResult[i].gender;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
	}

	pServerSession->SendPacket( &msg, msg.GetSize() );
	Query_Friend_SelectList::FREE(pQResult); pQResult = NULL;
}

Handler_FromWorldServer_IMPL( DW_FRIEND_ADD_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_FRIEND_ADD_SYN * pRecvMsg = (MSG_DW_FRIEND_ADD_SYN *)pMsg;

	TCHAR tszTmpCharName[MAX_CHARNAME_LENGTH+1];
	memcpy( tszTmpCharName, pRecvMsg->m_ptszCharNameTo, MAX_CHARNAME_LENGTH*sizeof(TCHAR) );
	tszTmpCharName[MAX_CHARNAME_LENGTH]='\0';

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Insert(%d,'%s', 1)}" , pRecvMsg->m_CharGuidFrom, tszTmpCharName );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Friend_Request * pQuery = Query_Friend_Request::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_ADD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_FRIEND_ADD_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Friend_Request * pQResult	= (Query_Friend_Request *)pResult->m_pData;

	if( pQResult->ResultCode() != Query_Friend_Request::RETURNCODE_SUCCESS )
	{
		MSG_DW_FRIEND_ADD_NAK nmsg;
		nmsg.m_CharGuidFrom	= pQResult->GetCharGuid();
		nmsg.m_byResult		= pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		Query_Friend_Request::FREE(pQResult); pQResult = NULL;
		return;
	}


	MSG_DW_FRIEND_ADD_ACK msg;
	msg.m_CharGuidFrom				= pQResult->GetCharGuid();
	msg.m_ToFriendInfo.dwFriendGuid	= pQResult->pResult[0].m_FriendCharGuid;
	memcpy( msg.m_ToFriendInfo.sPacketInfo.ptszFriendName, pQResult->pResult[0].m_ptszFriendCharName, MAX_CHARNAME_LENGTH*sizeof(TCHAR) );
	msg.m_ToFriendInfo.sPacketInfo.byClass		= pQResult->pResult[0].m_byClass;
	msg.m_ToFriendInfo.sPacketInfo.Level		= pQResult->pResult[0].m_Level;
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    msg.m_ToFriendInfo.sPacketInfo.gender       = pQResult->pResult[0].m_Gender;
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

	pServerSession->SendPacket( &msg, sizeof(msg) );
	Query_Friend_Request::FREE(pQResult); pQResult = NULL;
}

//친구 삭제 요청
Handler_FromWorldServer_IMPL( DW_FRIEND_DEL_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_FRIEND_DEL_SYN * pRecvMsg = (MSG_DW_FRIEND_DEL_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //친구 삭제 요청시
	DFriendInvitationController::Instance()->GetFriendDelQuery(szQueryBuff, pRecvMsg);	
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Friend_Delete * pQuery = Query_Friend_Delete::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );
	pQuery->SetCharGuidTo( pRecvMsg->m_CharGuidTo );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_DEL_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_FRIEND_DEL_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Friend_Delete * pQResult		= (Query_Friend_Delete *)pResult->m_pData;

	if( pQResult->IsError() || !pQResult->ResultSuccess() )
	{
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), pQResult->pParam[0].m_Result);
        }

		MSG_DW_FRIEND_DEL_NAK nmsg;
		nmsg.m_CharGuidFrom	= pQResult->GetCharGuid();
		nmsg.m_byResult		= 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}


	MSG_DW_FRIEND_DEL_ACK msg;
	msg.m_CharGuidFrom				= pQResult->GetCharGuid();
	msg.m_CharGuidTo				= pQResult->GetCharGuidTo();
	pServerSession->SendPacket( &msg, sizeof(msg) );
	Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
}

Handler_FromWorldServer_IMPL( DW_FRIEND_BLOCK_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_FRIEND_BLOCK_SYN * pRecvMsg = (MSG_DW_FRIEND_BLOCK_SYN *)pMsg;

	
	TCHAR tszTmpCharName[MAX_CHARNAME_LENGTH+1];
	memcpy( tszTmpCharName, pRecvMsg->m_ptszCharNameTo, MAX_CHARNAME_LENGTH*sizeof(TCHAR) );
	tszTmpCharName[MAX_CHARNAME_LENGTH]='\0';

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Insert(%d,'%s',2)}" , pRecvMsg->m_CharGuidFrom, tszTmpCharName );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

//	Query_Friend_BlockChar * pQuery = Query_Friend_BlockChar::ALLOC();
	Query_Friend_Request* pQuery = Query_Friend_Request::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );
	pQuery->SetCharNameTo( pRecvMsg->m_ptszCharNameTo );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_BLOCK_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}


Handler_FromWorldServer_IMPL( DW_FRIEND_BLOCK_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
//	Query_Friend_BlockChar * pQResult	= (Query_Friend_BlockChar *)pResult->m_pData;
	Query_Friend_Request* pQResult		= (Query_Friend_Request*)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_FRIEND_BLOCK_NAK nmsg;
		nmsg.m_CharGuidFrom	= pQResult->GetCharGuid();
		nmsg.m_byResult		= 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

//		Query_Friend_BlockChar::FREE(pQResult); pQResult = NULL;
		Query_Friend_Request::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_FRIEND_BLOCK_ACK msg;
	msg.m_CharGuidFrom				= pQResult->GetCharGuid();
	msg.m_BlockInfo.dwBlockGuid		= pQResult->pResult[0].m_FriendCharGuid;	//.m_BlockedCharGuid;
	memcpy( msg.m_BlockInfo.ptszBlockName, pQResult->GetCharNameTo(), MAX_CHARNAME_LENGTH*sizeof(TCHAR) );
	msg.m_BlockInfo.ptszBlockName[MAX_CHARNAME_LENGTH] = '\0';
	pServerSession->SendPacket( &msg, sizeof(msg) );
//	Query_Friend_BlockChar::FREE(pQResult); pQResult = NULL;
	Query_Friend_Request::FREE(pQResult); pQResult = NULL;
}


Handler_FromWorldServer_IMPL( DW_FRIEND_BLOCK_FREE_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_FRIEND_BLOCK_FREE_SYN * pRecvMsg = (MSG_DW_FRIEND_BLOCK_FREE_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Cancel(%d,'%d',2)}" , pRecvMsg->m_CharGuidFrom, pRecvMsg->m_CharGuidTo );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

//	Query_Friend_FreeChar * pQuery = Query_Friend_FreeChar::ALLOC();
	Query_Friend_Delete * pQuery = Query_Friend_Delete::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );
	pQuery->SetCharGuidTo( pRecvMsg->m_CharGuidTo );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_FRIEND, (WORD)DW_FRIEND_BLOCK_FREE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );

}

Handler_FromWorldServer_IMPL( DW_FRIEND_BLOCK_FREE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
//	Query_Friend_FreeChar * pQResult	= (Query_Friend_FreeChar *)pResult->m_pData;
	Query_Friend_Delete * pQResult		= (Query_Friend_Delete *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_FRIEND_BLOCK_FREE_NAK nmsg;
		nmsg.m_CharGuidFrom	= pQResult->GetCharGuid();
		nmsg.m_byResult		= 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

//		Query_Friend_FreeChar::FREE(pQResult); pQResult = NULL;
		Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_FRIEND_BLOCK_FREE_ACK msg;
	msg.m_CharGuidFrom			= pQResult->GetCharGuid();
	msg.m_CharGuidTo			= pQResult->GetCharGuidTo();
	pServerSession->SendPacket( &msg, sizeof(msg) );
//	Query_Friend_FreeChar::FREE(pQResult); pQResult = NULL;
	Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
}

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
//  네트웍 이벤트 핸들 구현부 = { DW_FRIEND_INVITATION~ }
//W->D 이벤트 종료 레벨에 도달하였다.
Handler_FromWorldServer_IMPL( DW_FRIEND_INVITATIONRELATION_FREE_CMD )
{
    DFriendInvitationController::Instance()->OnRemoveInvitatedFriendCMD(pServerSession, pMsg, wSize);
}

//DB->D ㅇ;벤트 종료 레벨에 도달 하였다.
Handler_FromWorldServer_IMPL( DW_FRIEND_INVITATIONRELATION_FREE_DBR )
{
    DFriendInvitationController::Instance()->OnRemoveInvitatedFriendDBR(pServerSession, pMsg, wSize);
}

Handler_FromWorldServer_IMPL( DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD )
{
    DFriendInvitationController::Instance()->OnRewardInfoInsertCMD(pServerSession, pMsg, wSize);
}

//DB->D ㅇ;벤트 종료 레벨에 도달 하였다.
Handler_FromWorldServer_IMPL( DW_FRIEND_INVITATIONRELATION_REWARDINSERT_DBR )
{
    DFriendInvitationController::Instance()->OnRewardInfoInsertDBR(pServerSession, pMsg, wSize);
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
Handler_FromWorldServer_IMPL( DW_FRIEND_DETAIL_INFO_SYN )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_FRIEND_DETAIL_INFO_SYN* pRecvMsg = (MSG_DW_FRIEND_DETAIL_INFO_SYN*)pMsg;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];

    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, 
               "{?=call S_CharRelation_Detail_Info_Select(%d, %d)}", 
               pRecvMsg->m_dwKey, pRecvMsg->m_dwToCharGuid);

    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_Friend_Detail_Info_Select* pQuery = Query_Friend_Detail_Info_Select::ALLOC();	
    pQuery->SetCharGuid(pRecvMsg->m_dwKey);
    pQuery->SetToCharGuid(pRecvMsg->m_dwToCharGuid);

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_FRIEND, (WORD)DW_FRIEND_DATAIL_INFO_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL( DW_FRIEND_DATAIL_INFO_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Friend_Detail_Info_Select* pQResult = (Query_Friend_Detail_Info_Select*)pResult->m_pData;

    if (pQResult->ResultCode())
    {
        MSG_DW_FRIEND_DETAIL_INFO_NAK nmsg;
        nmsg.m_dwKey = pQResult->GetCharGuid();
        nmsg.m_byErrorCode = pQResult->ResultCode();
        pServerSession->SendPacket(&nmsg, sizeof(nmsg));

        MessageOut(eFULL_LOG, "DW_FRIEND_DATAIL_INFO_DBR Failed[%u] - ResultCode[%u]",
                   pQResult->GetCharGuid(), pQResult->ResultCode());
        
        Query_Friend_Detail_Info_Select::FREE(pQResult); 
        pQResult = NULL;

        return;
    }

    MSG_DW_FRIEND_DETAIL_INFO_ACK msg;
    Query_Friend_Detail_Info_Select::sQUERY& result = pQResult->pResult[0];

    msg.m_dwKey = pQResult->GetCharGuid();
    msg.m_dwToCharGuid = pQResult->GetToCharGuid();
    msg.m_byToCharGuildDuty = result.m_byToCharGuildDuty;

    ZeroMemory(msg.m_ptszMemo, (MAX_FRIEND_MEMO_LENGHT + 1) * sizeof(TCHAR));
    ZeroMemory(msg.m_ptszToCharName, (MAX_CHARNAME_LENGTH + 1) * sizeof(TCHAR));
    ZeroMemory(msg.m_ptszToCharGuildName, (MAX_GUILDNAME_LENGTH + 1) * sizeof(TCHAR));

    _tcsncpy(msg.m_ptszMemo, result.m_ptszMemo, MAX_FRIEND_MEMO_LENGHT);
    _tcsncpy(msg.m_ptszToCharName, result.m_ptszToCharName, MAX_CHARNAME_LENGTH);
    _tcsncpy(msg.m_ptszToCharGuildName, result.m_ptszToCharGuildName, MAX_GUILDNAME_LENGTH);

    msg.m_ptszMemo[MAX_FRIEND_MEMO_LENGHT]='\0';
    msg.m_ptszToCharName[MAX_CHARNAME_LENGTH]='\0';
    msg.m_ptszToCharGuildName[MAX_GUILDNAME_LENGTH]='\0';

    pServerSession->SendPacket(&msg, sizeof(msg));

    Query_Friend_Detail_Info_Select::FREE(pQResult);
    pQResult = NULL;
}

Handler_FromWorldServer_IMPL( DW_FRIEND_MEMO_UPDATE_SYN )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_FRIEND_MEMO_UPDATE_SYN* pRecvMsg = (MSG_DW_FRIEND_MEMO_UPDATE_SYN*)pMsg;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_512+1];

    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_512, "{?=call S_CharRelation_Update_Memo(%d, %d, '%s')}", 
               pRecvMsg->m_dwKey, pRecvMsg->m_dwToCharGuid, pRecvMsg->m_ptszMemo);

    szQueryBuff[MAX_QUERY_LENGTH_512]='\0';

    Query_Friend_Memo_Update *pQuery = Query_Friend_Memo_Update::ALLOC();	
    pQuery->SetCharGuid(pRecvMsg->m_dwKey);
    pQuery->SetToCharGuid(pRecvMsg->m_dwToCharGuid);

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_FRIEND, (WORD)DW_FRIEND_MEMO_UPDATE_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL( DW_FRIEND_MEMO_UPDATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_Friend_Memo_Update* pQResult = (Query_Friend_Memo_Update*)pResult->m_pData;

    if (pQResult->ResultCode())
    {
        MSG_DW_FRIEND_MEMO_UPDATE_NAK nmsg;
        nmsg.m_dwKey = pQResult->GetCharGuid();
        nmsg.m_byErrorCode = pQResult->ResultCode();
        pServerSession->SendPacket(&nmsg, sizeof(nmsg));

        MessageOut(eFULL_LOG, "DW_FRIEND_MEMO_UPDATE_DBR Failed[%u] - ResultCode[%u]",
                   pQResult->GetCharGuid(), pQResult->ResultCode());

        Query_Friend_Memo_Update::FREE(pQResult); 
        pQResult = NULL;

        return;
    }

    MSG_DW_FRIEND_MEMO_UPDATE_ACK msg;
    msg.m_dwKey = pQResult->GetCharGuid();
    msg.m_dwToCharGuid = pQResult->GetToCharGuid();

    pServerSession->SendPacket(&msg, sizeof(msg));

    Query_Friend_Memo_Update::FREE(pQResult);
    pQResult = NULL;
}


// 채팅금지
Handler_FromWorldServer_IMPL( DW_GM_CHATBLOCK_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_GM_CHATBLOCK_SYN* pRecvMsg = (MSG_DW_GM_CHATBLOCK_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_ChatBlock_Create(%d,%d,'','%s',0)}" , pRecvMsg->m_CharGuid, 
																								pRecvMsg->m_dwBlockTIme, 
																								pRecvMsg->m_ptszGMAccountID );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';
	
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	Query_Chat_BlockChar * pQuery = Query_Chat_BlockChar::ALLOC();
	pQuery->SetGMGuid( pRecvMsg->m_dwKey );
	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );
	pQuery->SetBlockTime( pRecvMsg->m_dwBlockTIme );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_GM, (WORD)DW_GM_CHATBLOCK_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );

	MessageOut( eFULL_LOG, "ChatBlock Req[GM:%u][%u]", pRecvMsg->m_dwKey, pRecvMsg->m_dwKey );
}

Handler_FromWorldServer_IMPL( DW_GM_CHATBLOCK_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	Query_Chat_BlockChar * pQResult	= (Query_Chat_BlockChar *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_GM_CHATBLOCK_NAK nmsg;
        nmsg.m_dwKey = pQResult->GetGMGuid(); // CHANGES: f110818.1L, char_guid
		nmsg.m_wErrorCode = 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "ChatBlock Failed[GM:%u][%u]", pQResult->GetGMGuid(), pQResult->GetCharGuid() );
		Query_Chat_BlockChar::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_GM_CHATBLOCK_ACK msg;
	msg.m_dwKey				= pQResult->GetGMGuid();
	msg.m_CharGuid			= pQResult->GetCharGuid();
	msg.m_dwBlockTime		= pQResult->GetBlockTime();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	MessageOut( eFULL_LOG, "ChatBlock Success[GM:%u][%u]", pQResult->GetGMGuid(), pQResult->GetCharGuid() );
	Query_Chat_BlockChar::FREE(pQResult); pQResult = NULL;
}

// 채팅허용
Handler_FromWorldServer_IMPL( DW_GM_CHATALLOW_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_GM_CHATALLOW_SYN* pRecvMsg = (MSG_DW_GM_CHATALLOW_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_ChatBlock_Update(%d,0,'','%s',0)}" , pRecvMsg->m_CharGuid, 
																								pRecvMsg->m_ptszGMAccountID );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';
	Query_Chat_BlockChar * pQuery = Query_Chat_BlockChar::ALLOC();
	pQuery->SetGMGuid( pRecvMsg->m_dwKey );
	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_GM, (WORD)DW_GM_CHATALLOW_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );

	MessageOut( eFULL_LOG, "ChatBlock Allow Req[GM:%u][%u]", pRecvMsg->m_dwKey, pRecvMsg->m_dwKey );
}

Handler_FromWorldServer_IMPL( DW_GM_CHATALLOW_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Chat_BlockChar * pQResult	= (Query_Chat_BlockChar *)pResult->m_pData;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
    if( !pQResult->ResultSuccess() )
	{
		MSG_DW_GM_CHATALLOW_NAK nmsg;
        nmsg.m_dwKey = pQResult->GetGMGuid();
		nmsg.m_wErrorCode = 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "ChatBlock Allow Failed[GM:%u][%u]", pQResult->GetGMGuid(), pQResult->GetCharGuid() );
		Query_Chat_BlockChar::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_GM_CHATALLOW_ACK msg;
	msg.m_dwKey				= pQResult->GetGMGuid();
	msg.m_CharGuid			= pQResult->GetCharGuid();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	MessageOut( eFULL_LOG, "ChatBlock Allow Success[GM:%u][%u]", pQResult->GetGMGuid(), pQResult->GetCharGuid() );
	Query_Chat_BlockChar::FREE(pQResult); pQResult = NULL;
}

// 채팅금지 시간 차감
Handler_FromWorldServer_IMPL( DW_GM_CHATBLOCK_UPDATE_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_GM_CHATBLOCK_UPDATE_SYN* pRecvMsg = (MSG_DW_GM_CHATBLOCK_UPDATE_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	_sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_ChatBlock_Tick(%d,%d)}" , pRecvMsg->m_dwKey, pRecvMsg->m_dwRemainTime );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Chat_BlockChar * pQuery = Query_Chat_BlockChar::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_GM, (WORD)DW_GM_CHATBLOCK_UPDATE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );

	MessageOut(eFULL_LOG, "ChatBlock Cancle Update Req_[CharGuid:%u]", pRecvMsg->m_dwKey);
}

Handler_FromWorldServer_IMPL( DW_GM_CHATBLOCK_UPDATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;

	Query_Chat_BlockChar * pQResult	= (Query_Chat_BlockChar *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_GM_CHATBLOCK_UPDATE_NAK nmsg;
		nmsg.m_wErrorCode = 1;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "ChatBlock Time Update Failed[%u]", pQResult->GetCharGuid() );
		Query_Chat_BlockChar::FREE(pQResult); pQResult = NULL;
		return;
	}

	MessageOut( eDEV_LOG, "ChatBlock Time Update Success[%u]", pQResult->GetCharGuid() );
	
	Query_Chat_BlockChar::FREE(pQResult); pQResult = NULL;
}

// 채팅금지 시간 요청
Handler_FromWorldServer_IMPL( DW_GM_CHATBLOCKTIME_REQ )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_GM_CHATBLOCKTIME_REQ * pRecvMsg = (MSG_DW_GM_CHATBLOCKTIME_REQ *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_ChatBlock_Select(%d)}" , pRecvMsg->m_dwKey );	
	szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

	Query_ChatBlock_Info * pQuery = Query_ChatBlock_Info::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_dwKey );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_GM, (WORD)DW_GM_CHATBLOCKTIME_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_GM_CHATBLOCKTIME_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_ChatBlock_Info * pQResult	= (Query_ChatBlock_Info *)pResult->m_pData;

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	if(!pQResult->ResultSuccess())
	{
		MSG_DW_GM_CHATBLOCKTIME_NAK nmsg;
		nmsg.m_dwKey = pQResult->GetCharGuid();
		nmsg.m_wErrorCode = 0;
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eCRITICAL_LOG, "ChatBlock Update Failed[%u]", pQResult->GetCharGuid() );
		Query_ChatBlock_Info::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_GM_CHATBLOCKTIME_ANS msg;
	msg.m_dwKey		= pQResult->GetCharGuid();

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	msg.m_dwRemainTime = pQResult->pResult[0].m_dwRemainTime;
	
	pServerSession->SendPacket(&msg, sizeof(msg));

    //__NA_000313_20070223_REALTIME_CHAT_BLOCK
	MessageOut(eDEV_LOG, "ChatBlock Update Success[CharGuid:%u][%u]", pQResult->GetCharGuid(), pQResult->pResult[0].m_dwRemainTime );

	Query_ChatBlock_Info::FREE(pQResult); pQResult = NULL;
}

/////////////////////////////////////
// Revenge List Protocol Handler

Handler_FromWorldServer_IMPL( DW_CHAO_ADD_REVENGE_CHAR_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_CHAO_ADD_REVENGE_CHAR_SYN* pRecvMsg = (MSG_DW_CHAO_ADD_REVENGE_CHAR_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Insert(%d,'%s',3)}" , pRecvMsg->m_CharGuidFrom, pRecvMsg->m_ptszCharNameTo );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

//	Query_Revenge_AddChar * pQuery = Query_Revenge_AddChar::ALLOC();
	Query_Friend_Request * pQuery = Query_Friend_Request::ALLOC();

	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );
	pQuery->SetCharNameTo( pRecvMsg->m_ptszCharNameTo );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_CHAO, (WORD)DW_CHAO_ADD_REVENGE_CHAR_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_CHAO_ADD_REVENGE_CHAR_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
//	Query_Revenge_AddChar * pQResult	= (Query_Revenge_AddChar *)pResult->m_pData;
	Query_Friend_Request * pQResult		= (Query_Friend_Request *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_CHAO_ADD_REVENGE_CHAR_NAK nmsg;
		nmsg.m_dwKey = pQResult->GetKey();
		nmsg.m_CharGuidFrom = pQResult->GetCharGuid();
		nmsg.m_dwKey = pQResult->GetCharGuid();
		nmsg.m_byErrorCode = pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "Revenge Character Add Failed[%u, %s]", pQResult->GetCharGuid(), pQResult->GetCharNameTo() );
//		Query_Revenge_AddChar::FREE(pQResult); pQResult = NULL;
		Query_Friend_Request::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_CHAO_ADD_REVENGE_CHAR_ACK msg;

	msg.m_dwKey = pQResult->GetKey();
	msg.m_CharGuidFrom = pQResult->GetCharGuid();

	msg.m_sRevengeUserInfo.dwRevengeCharGuid = pQResult->pResult[0].m_FriendCharGuid;	//.m_RevengeCharGuid;
	memset( msg.m_sRevengeUserInfo.sPacketInfo.ptszRevengeName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
	_tcsncpy( msg.m_sRevengeUserInfo.sPacketInfo.ptszRevengeName, pQResult->GetCharNameTo(), MAX_CHARNAME_LENGTH );
	msg.m_sRevengeUserInfo.sPacketInfo.byClass = pQResult->pResult[0].m_byClass;
	msg.m_sRevengeUserInfo.sPacketInfo.Level = pQResult->pResult[0].m_Level;

	pServerSession->SendPacket( &msg, sizeof(msg) );
//	Query_Revenge_AddChar::FREE(pQResult); pQResult = NULL;
	Query_Friend_Request::FREE(pQResult); pQResult = NULL;
}

Handler_FromWorldServer_IMPL( DW_CHAO_REMOVE_REVENGE_CHAR_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN* pRecvMsg = (MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Cancel(%d,%d,3)}" , pRecvMsg->m_CharGuidFrom, pRecvMsg->m_RevengeGuidTo );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

//	Query_Revenge_RemoveChar * pQuery = Query_Revenge_RemoveChar::ALLOC();
	Query_Friend_Delete * pQuery = Query_Friend_Delete::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );
	pQuery->SetCharGuidTo( pRecvMsg->m_RevengeGuidTo );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_CHAO, (WORD)DW_CHAO_REMOVE_REVENGE_CHAR_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_CHAO_REMOVE_REVENGE_CHAR_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
//	Query_Revenge_RemoveChar * pQResult	= (Query_Revenge_RemoveChar *)pResult->m_pData;
	Query_Friend_Delete * pQResult = (Query_Friend_Delete *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK nmsg;
		nmsg.m_dwKey		= pQResult->GetKey();
		nmsg.m_CharGuidFrom	= pQResult->GetCharGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "DW_CHAO_REMOVE_REVENGE_CHAR_SYN Failed[%u,%u] - ResultCode[%u]", pQResult->GetCharGuid(), 
																								pQResult->GetCharGuidTo(), 
																								pQResult->ResultCode() );
//		Query_Revenge_RemoveChar::FREE(pQResult); pQResult = NULL;
		Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK msg;
	msg.m_dwKey						= pQResult->GetKey();
	msg.m_CharGuidFrom				= pQResult->GetCharGuid();
	msg.m_RevengeGuidTo				= pQResult->GetCharGuidTo();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	MessageOut( eFULL_LOG, "DW_CHAO_REMOVE_REVENGE_CHAR_SYN[%u,%u]", pQResult->GetCharGuid(), pQResult->GetCharGuidTo() );
//	Query_Revenge_RemoveChar::FREE(pQResult); pQResult = NULL;
	Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
}

Handler_FromWorldServer_IMPL( DW_CHAO_PK_REVENGE_CHAR_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_CHAO_PK_REVENGE_CHAR_SYN* pRecvMsg = (MSG_DW_CHAO_PK_REVENGE_CHAR_SYN*)pMsg;
	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_CharRelation_Cancel(%d,%d,3)}" , pRecvMsg->m_CharGuidFrom, pRecvMsg->m_RevengeGuidTo );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

//	Query_Revenge_RemoveChar * pQuery = Query_Revenge_RemoveChar::ALLOC();
	Query_Friend_Delete * pQuery = Query_Friend_Delete::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuidFrom );
	pQuery->SetCharGuidTo( pRecvMsg->m_RevengeGuidTo );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_CHAO, (WORD)DW_CHAO_REMOVE_REVENGE_CHAR_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );

}

Handler_FromWorldServer_IMPL( DW_CHAO_PK_REVENGE_CHAR_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
//	Query_Revenge_RemoveChar * pQResult	= (Query_Revenge_RemoveChar *)pResult->m_pData;
	Query_Friend_Delete * pQResult = (Query_Friend_Delete *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK nmsg;
		nmsg.m_dwKey = pQResult->GetKey();
		nmsg.m_CharGuidFrom	= pQResult->GetCharGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

//		Query_Revenge_RemoveChar::FREE(pQResult); pQResult = NULL;
		Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
		MessageOut( eFULL_LOG, "DW_CHAO_PK_REVENGE_CHAR_SYN Failed[%u,%u] - ResultCode[%u]", pQResult->GetCharGuid(), 
																							pQResult->GetCharGuidTo(), 
																							pQResult->ResultCode() );
		return;
	}

	MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK msg;
	msg.m_dwKey						= pQResult->GetKey();
	msg.m_CharGuidFrom				= pQResult->GetCharGuid();
	msg.m_RevengeGuidTo				= pQResult->GetCharGuidTo();
	pServerSession->SendPacket( &msg, sizeof(msg) );

//	Query_Revenge_RemoveChar::FREE(pQResult); pQResult = NULL;
	Query_Friend_Delete::FREE(pQResult); pQResult = NULL;
}


Handler_FromWorldServer_IMPL( DW_MEMO_SEND_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_MEMO_SEND_SYN* pRecvMsg = (MSG_DW_MEMO_SEND_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_512+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_512, 
               "{?=call S_Memo_Send(%d,%d,'%s','%s',%d,%d,%I64d)}" , 
               pRecvMsg->m_SendGuid, 
               pRecvMsg->m_recvGuid,
               pRecvMsg->m_ptszRecvName, 
               pRecvMsg->m_ptszMemo,
               pRecvMsg->m_typeSystemMemo,
               pRecvMsg->m_recvItemCode,
               pRecvMsg->m_amountCost);
	szQueryBuff[MAX_QUERY_LENGTH_512]='\0';

	Query_Memo_Send * pQuery = Query_Memo_Send::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_SendGuid );
	pQuery->SetRecvName( pRecvMsg->m_ptszRecvName );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_MEMO, (WORD)DW_MEMO_SEND_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_MEMO_SEND_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Memo_Send * pQResult = (Query_Memo_Send *)pResult->m_pData;

	if (pQResult->IsError() || !pQResult->ResultSuccess())
	{
		MSG_DW_MEMO_SEND_NAK nmsg;
		nmsg.m_dwKey = pQResult->GetKey();
		nmsg.m_SendGuid	= pQResult->GetCharGuid();
		nmsg.m_byErrorCode = pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "DW_MEMO_SEND_SYN Failed[%u,'%s'] - ResultCode[%u]", pQResult->GetCharGuid(), 
																					pQResult->GetRecvName(), 
																	pQResult->ResultCode() );
        // added rule by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
        if (pQResult->GetCharGuid() == 0)
        {
            // 시스템 메모 저장 실패 로그
            SUNLOG(eCRITICAL_LOG, "[Memo] Failed to store system memo for character '%s'", pQResult->GetRecvName());
        }

        Query_Memo_Send::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_MEMO_SEND_ACK msg;
	msg.m_dwKey						= pQResult->GetKey();
	msg.m_SendGuid					= pQResult->GetCharGuid();
	msg.m_RecvGuid					= pQResult->pResult[0].m_RecvGuid;
	msg.m_Subject.m_byRead			= 0;
	msg.m_Subject.m_MemoGuid		= pQResult->pResult[0].m_MemoGuid;
    msg.m_Subject.m_MemoType        = ( msg.m_SendGuid != 0 ? \
        STRUCT_SUBJECT_PACKETINFO::NORMAL_MEMO : STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO );

    ZeroMemory(msg.m_Subject.m_SendName, sizeof(msg.m_Subject.m_SendName));
    _tcsncpy(msg.m_Subject.m_tszCreateDate, pQResult->pResult[0].m_ptszCreateDate,
             _countof(msg.m_Subject.m_tszCreateDate));
    msg.m_Subject.m_tszCreateDate[_countof(msg.m_Subject.m_tszCreateDate) - 1] = _T('\0');
    // CHANGES: f110504.4L, fixed 'msg.m_Subject.m_ptszSubject' buffer overrun
    _tcsncpy(msg.m_Subject.m_ptszSubject, pQResult->pResult[0].m_ptszSubject,
             _countof(msg.m_Subject.m_ptszSubject));
    msg.m_Subject.m_ptszSubject[_countof(msg.m_Subject.m_ptszSubject) - 1] = _T('\0');
    _tcsncpy(msg.m_ptszRecvName, pQResult->GetRecvName(), _countof(msg.m_ptszRecvName));
    msg.m_ptszRecvName[_countof(msg.m_ptszRecvName) - 1] = '\0';
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //이 패킷은 DW패킷으로 실제 용도 뿐만아니라 사용되고 있는 부분도 DBP -> World로 사용되므로 아래와 같이 수정한다.
    g_DBProxyServer.SendToWorldServer(&msg, sizeof(msg));
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    pServerSession->SendPacket( &msg, sizeof(msg) );
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	Query_Memo_Send::FREE(pQResult); pQResult = NULL;
    //_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    if (msg.m_SendGuid == 0)
    {
        // 시스템 메모 저장 성공 로그
        SUNLOG(eMIDDLE_LOG, "[Memo] Stored system memo %I64u for character '%s' : %s", 
            msg.m_Subject.m_MemoGuid, msg.m_ptszRecvName, msg.m_Subject.m_ptszSubject);
    }
}

// 쪽지 목록 확인
Handler_FromWorldServer_IMPL( DW_MEMO_LIST_REQ )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_MEMO_LIST_REQ* pRecvMsg = (MSG_DW_MEMO_LIST_REQ*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_MemoListSelect(%d)}" , pRecvMsg->m_RecvGuid );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Memo_ListSelect * pQuery = Query_Memo_ListSelect::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_RecvGuid );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_MEMO, (WORD)DW_MEMO_LIST_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}


Handler_FromWorldServer_IMPL( DW_MEMO_LIST_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Memo_ListSelect * pQResult = (Query_Memo_ListSelect *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_MEMO_LIST_NAK nmsg;
		nmsg.m_dwKey = pQResult->GetKey();
		nmsg.m_RecvGuid = pQResult->GetCharGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "DW_MEMO_LIST_REQ Failed[%u] - ResultCode[%u]", pQResult->GetCharGuid(), 
																				pQResult->ResultCode() );
		Query_Memo_ListSelect::FREE(pQResult); pQResult = NULL;
		return;
	}

	// 목록 전송은 페이지별로 한다
	ULONG uResultNum = pQResult->GetResultRowNum();

	MSG_DW_MEMO_LIST_ANS msg;
	msg.m_dwKey						= pQResult->GetKey();
	msg.m_RecvGuid					= pQResult->GetCharGuid();
	// 페이지 개수 구한다
	if( 0 == uResultNum )
	{
		msg.m_byMemoCnt = 0;
		msg.m_byMemoPage = 0;
		msg.m_byMemoPageCnt = 0;

		pServerSession->SendPacket( &msg, msg.GetSize() );

#ifdef _DEBUG
		MessageOut( eFULL_LOG, "DW_MEMO_LIST_REQ[%u] - No Memo",  msg.m_RecvGuid );
#endif
		Query_Memo_ListSelect::FREE(pQResult); pQResult = NULL;
		return;
	}
	msg.m_byMemoPageCnt = BYTE(	(uResultNum / MAX_MEMO_PAGENUM) + ( (uResultNum % MAX_MEMO_PAGENUM) ? (1) : (0) )	);

	BYTE i, k, byOffset;
	for( i = 0; i < msg.m_byMemoPageCnt; i++ )
	{
		msg.m_byMemoPage = i+1;
		// 마지막 페이지 아니면(또는 정확히 MAX_MEMO_PAGENUM으로 맞아떨어지는 마지막 페이지면)
		if( (uResultNum - (i * MAX_MEMO_PAGENUM)) >= MAX_MEMO_PAGENUM )
		{
			// 풀로 채운다
			msg.m_byMemoCnt = MAX_MEMO_PAGENUM;
		}
		else
		{
			msg.m_byMemoCnt = BYTE( uResultNum % MAX_MEMO_PAGENUM );	// 마지막 페이지면 나머지 계산
		}
        // CHANGES: f110504.4L, logic improvements, and fixed buffer overrun problem
        // 실제 메모 내용 복사
        for (k = 0; k < msg.m_byMemoCnt; ++k)
        {
            byOffset = (i * MAX_MEMO_PAGENUM) + k; // 해당 페이지에 맞는 내용을 넣는다
            STRUCT_SUBJECT_PACKETINFO* const target_memo = &msg.m_Memo[k];
            const Query_Memo_ListSelect::sQUERY& query_result = pQResult->pResult[byOffset];

            target_memo->m_byRead = query_result.m_bRead;

            target_memo->m_byRead = query_result.m_bRead;
            target_memo->m_MemoGuid = query_result.m_MemoGuid;
            target_memo->m_MemoType = query_result.m_SenderGuid != 0
                                    ?   STRUCT_SUBJECT_PACKETINFO::NORMAL_MEMO
                                    :   STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO;
            //
            // CHANGES: f110504.4L, fixed 'msg.m_Subject.m_ptszSubject' buffer overrun
            _tcsncpy(target_memo->m_ptszSubject, query_result.m_ptszSubject,
                     _countof(target_memo->m_ptszSubject));
            target_memo->m_ptszSubject[_countof(target_memo->m_ptszSubject) - 1] = _T('\0');
            // implemented by __NA_0_20090818_SYSTEM_MEMO_SUPPORT = {
            // 일반 쪽지
            if (query_result.m_SenderGuid != 0) {
                _tcsncpy(target_memo->m_SendName, query_result.m_ptszSendName,
                         _countof(target_memo->m_SendName));
                target_memo->m_SendName[_countof(target_memo->m_SendName) - 1] = _T('\0');
            }
            // 시스템 쪽지
            else {
                _tcsncpy(target_memo->m_SendName, "System", _countof(target_memo->m_SendName));
                target_memo->m_SendName[_countof(target_memo->m_SendName) - 1] = _T('\0');
                // 시스템 쪽지이면 타입에 따라 송신자 이름을 다르게 설정할 수 있다
                std::string s;
                // assign... note about MAX_SUBJECT_LENGTH = 20, MAX_SUBJECT_STRLENGTH = 21,
                // I know the std::string::assign function process copying with garbage stream
                // and changes related size until the input size.
                // example:
                //    const char* str = "hello\0dum|my";
                //    std::string s;
                //    s.assign(str, 20); // hello\0dum|my\0???????
                //    assert(s.size() == 20 && s.length() == 20);
                //    assert(strlen(s.c_str()) == 5);
                //    assert(s.find('|') == 9); // not 'std::string::npos'
                //s.assign(target_memo->m_ptszSubject, MAX_SUBJECT_LENGTH);
                s.assign(target_memo->m_ptszSubject);
                std::string::size_type pos = s.find('|');
                if (pos != std::string::npos) {
                    std::string t = s.substr(0, pos);
                    int type = atoi(t.c_str());
                    switch (type)
                    {
                    // handled by _KR_0_20091021_FIRENDINVITATION_EXTEND = {
                    case SYSMEMO_FRIENDINVITATION_INVATES_TART:
                    case SYSMEMO_FRIENDINVITATION_INVATE_END:
                    case SYSMEMO_FRIENDINVITATION_INVATE_PRESENT:
                    case SYSMEMO_FRIENDINVITATION_BEINVATED_START:
                    case SYSMEMO_FRIENDINVITATION_BEINVATED_END:
                    case SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT:
                        _tcsncpy(target_memo->m_SendName, "Event Manager", MAX_CHARNAME_LENGTH);
                        break;
                    //}
                    // handled by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
                    case SYSMEMO_LOTTO_WIN:
                    case SYSMEMO_LOTTO_WIN_NEW:
                        _tcsncpy(target_memo->m_SendName, "Lotto", MAX_CHARNAME_LENGTH);
                        break;
                    //}
                        //__KR_001355_20090805_CHECK_GROUPSYSTEM_MSG
                    case SYSMEMO_GROUP_MGR:
                        _tcsncpy(target_memo->m_SendName, "Group", MAX_CHARNAME_LENGTH);
                        break;
                    case SYSMEMO_DOMINATION_RETURN_MONEY:
                    case SYSMEMO_DOMINATION_REWARD_MONEY:
                    case SYSMEMO_DOMINATION_RETURN_MONEY_NEW:
                    case SYSMEMO_DOMINATION_REWARD_MONEY_NEW:
                        _tcsncpy(target_memo->m_SendName, "Domination", MAX_CHARNAME_LENGTH);
                        break;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
                    case SYSMEMO_WORLD_RANKING_GRADE:
                        _tcsncpy(target_memo->m_SendName, "WorldRanking", MAX_CHARNAME_LENGTH);
                        break;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                    case SYSMEMO_GUILD_JOIN_REQUEST:
                        _tcsncpy(target_memo->m_SendName, "GuildJoinReq", MAX_CHARNAME_LENGTH);
                        break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
                    case SYSMEMO_SUN_RANKING_SETTLE_RESULT:
                        _tcsncpy(target_memo->m_SendName, "SunRanking", MAX_CHARNAME_LENGTH);
                        break;
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
                    default:
                        SUNLOG(eCRITICAL_LOG, "Unknown system memo type : %d", type);
                        break;
                    }
                }
                target_memo->m_SendName[_countof(target_memo->m_SendName) - 1] = _T('\0');
            }
            // }; end of '__NA_0_20090818_SYSTEM_MEMO_SUPPORT'
            // CHANGES: f110504.4L, 21 or 16(MAX_SMALLDATETIME_SIZE) ?????
            _tcsncpy(target_memo->m_tszCreateDate, query_result.m_ptszSendDate,
                     _countof(target_memo->m_tszCreateDate));
            target_memo->m_tszCreateDate[_countof(target_memo->m_tszCreateDate) - 1] = _T('\0');
        }
		// 페이지 내용 다 채웠으면 전송
		pServerSession->SendPacket( &msg, sizeof(msg) );
#ifdef _DEBUG
		MessageOut( eFULL_LOG, "DW_MEMO_LIST_ANS - CharGuid[%u] Page[%u/%u] MemoCnt[%u]", msg.m_RecvGuid, 
																							msg.m_byMemoPage,
																							msg.m_byMemoPageCnt,
																							msg.m_byMemoCnt );
#endif
	}

	Query_Memo_ListSelect::FREE(pQResult); pQResult = NULL;
}


// 쪽지 확인
Handler_FromWorldServer_IMPL( DW_MEMO_VIEW_REQ )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_MEMO_VIEW_REQ* pRecvMsg = (MSG_DW_MEMO_VIEW_REQ*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_MemoSelect(%d, %d)}" , pRecvMsg->m_RecvGuid, pRecvMsg->m_MemoGuid );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Memo_Select * pQuery = Query_Memo_Select::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_RecvGuid );
	pQuery->SetMemoGuid( pRecvMsg->m_MemoGuid );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_MEMO, (WORD)DW_MEMO_VIEW_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_MEMO_VIEW_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Memo_Select * pQResult = (Query_Memo_Select *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_MEMO_VIEW_NAK nmsg;
		nmsg.m_dwKey = pQResult->GetKey();
		nmsg.m_RecvGuid = pQResult->GetCharGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "DW_MEMO_VIEW_DBR Failed[%u/Memo:%u] - ResultCode[%u]", pQResult->GetCharGuid(), 
																				pQResult->GetMemoGuid(),
																				pQResult->ResultCode() );
		Query_Memo_Select::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_MEMO_VIEW_ANS msg;
	msg.m_dwKey						= pQResult->GetKey();
	msg.m_MemoGuid					= pQResult->GetMemoGuid();
	msg.m_RecvGuid					= pQResult->GetCharGuid();
	_tcsncpy( msg.m_ptszMemo, pQResult->pResult[0].m_Memo, MAX_MEMO_LENGTH );
	msg.m_ptszMemo[MAX_MEMO_LENGTH] = '\0';
    msg.m_typeSystemMemo            = pQResult->pResult->system_memo_type_;
    msg.m_recvItemCode              = pQResult->pResult->recv_item_code_;
    msg.m_amountCost                = pQResult->pResult->amount_cost_;
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Memo_Select::FREE(pQResult); pQResult = NULL;
}

// 쪽지 삭제
Handler_FromWorldServer_IMPL( DW_MEMO_DELETE_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_MEMO_DELETE_SYN* pRecvMsg = (MSG_DW_MEMO_DELETE_SYN*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_Memo_Delete(%d, %d)}" , pRecvMsg->m_RecvGuid, pRecvMsg->m_MemoGuid );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Memo_Delete * pQuery = Query_Memo_Delete::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_RecvGuid );
	pQuery->SetMemoGuid( pRecvMsg->m_MemoGuid );
    pQuery->SetMemoType( pRecvMsg->m_MemoType );

	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_MEMO, (WORD)DW_MEMO_DELETE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_MEMO_DELETE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Memo_Delete * pQResult		= (Query_Memo_Delete *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_MEMO_DELETE_NAK nmsg;
		nmsg.m_dwKey			= pQResult->GetKey();
		nmsg.m_RecvGuid			= pQResult->GetCharGuid();
		nmsg.m_byErrorCode		= pQResult->ResultCode();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

		MessageOut( eFULL_LOG, "DW_MEMO_DELETE_SYN Failed[%u/Memo:%u] - ResultCode[%u]", pQResult->GetCharGuid(), 
																				pQResult->GetMemoGuid(),
																				pQResult->ResultCode() );
		Query_Memo_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_MEMO_DELETE_ACK msg;
	msg.m_dwKey						= pQResult->GetKey();
	msg.m_MemoGuid					= pQResult->GetMemoGuid();
	msg.m_RecvGuid					= pQResult->GetCharGuid();
    msg.m_MemoType = (STRUCT_SUBJECT_PACKETINFO::eMEMOTYPE)pQResult->GetMemoType();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Memo_Delete::FREE(pQResult); pQResult = NULL;
}

Handler_FromWorldServer_IMPL( DW_STATUS_INTERCEPTION_REQ )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_STATUS_INTERCEPTION_REQ* pRecvMsg = (MSG_DW_STATUS_INTERCEPTION_REQ*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_Char_InterceptFlagGet(%d)}", pRecvMsg->m_CharGuid);
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Interception_Request* pQuery = Query_Interception_Request::ALLOC();

	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );
	pQuery->SetKey( pRecvMsg->m_dwKey );
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_STATUS, (WORD)DW_STATUS_INTERCEPTION_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL(DW_STATUS_INTERCEPTION_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult			= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Interception_Request *pQResult	= (Query_Interception_Request *)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		MSG_DW_STATUS_INTERCEPTION_NAK nakMSG;
		nakMSG.m_dwKey			= pQResult->GetKey();
		nakMSG.m_CharGuid		= pQResult->GetCharGuid();
		nakMSG.m_byErrorCode	= pQResult->ResultCode();
		pServerSession->SendPacket( &nakMSG, sizeof(nakMSG) );

		MessageOut( eFULL_LOG, "DW_STATUS_INTERCEPTION_NAK_[CHARGUID:%u]", pQResult->GetCharGuid() );

		Query_Interception_Request::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_STATUS_INTERCEPTION_ANS ansMSG;
	ansMSG.m_dwKey		= pQResult->GetKey();
	ansMSG.m_CharGuid	= pQResult->GetCharGuid();
	ansMSG.m_bWhisperFlag = pQResult->pResult[0].m_bWhisperFlag;
	pServerSession->SendPacket( &ansMSG, sizeof(ansMSG) );

	Query_Interception_Request::FREE(pQResult); pQResult = NULL;
}

Handler_FromWorldServer_IMPL(DW_GUILD_WAREHOUSE_LOGLIST_REQ)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_GUILD_WAREHOUSE_LOGLIST_REQ* pRecvMsg = (MSG_DW_GUILD_WAREHOUSE_LOGLIST_REQ*)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_GuildWarehouseLog_Select(%d)}" , pRecvMsg->m_GuildGuid);
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	GuildWarehouseLogSelectQuery* pQuery = GuildWarehouseLogSelectQuery::ALLOC();

	for( int i = 0 ; i < GuildWarehouseLogSelectQuery::RESULT_ROW_NUM ; ++i )
	{
		ZeroMemory(&pQuery->pResult[i], sizeof(pQuery->pResult[i]));
	}

	pQuery->SetKey(pRecvMsg->m_dwKey);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetCharGuid(pRecvMsg->m_ReqCharGuid);

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DW_GUILD, (WORD)DW_GUILD_WAREHOUSE_LOGLIST_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_GUILD_WAREHOUSE_LOGLIST_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	GuildWarehouseLogSelectQuery*  pQResult = (GuildWarehouseLogSelectQuery*)pResult->m_pData;

	if(!pQResult->ResultSuccess()) //사용내역이 없어도 에러코드를 보낸다.
	{
		MSG_DW_GUILD_WAREHOUSE_LOGLIST_NAK msg;
		msg.m_dwKey = pQResult->GetKey();
		msg.m_ReqCharGuid = pQResult->GetCharGuid();
		msg.m_byErrorCode = pQResult->ResultCode();
		pServerSession->SendPacket(&msg, sizeof(msg));
		GuildWarehouseLogSelectQuery::FREE(pQResult);
		return;
	}

	MSG_DW_GUILD_WAREHOUSE_LOGLIST_ANS msg;
	msg.m_dwKey = pQResult->GetKey();
	msg.m_GuildGuid = pQResult->GetGuildGuid();
	msg.m_ReqCharGuid = pQResult->GetCharGuid();

	ULONG  uResultNum = pQResult->GetResultRowNum();
	if( 0 == uResultNum )
	{
		msg.m_byLogCount = 0;
		msg.m_byCurrentPage = 0;
		msg.m_byLogPageCount = 0;
		pServerSession->SendPacket( &msg, msg.GetSize() );
		GuildWarehouseLogSelectQuery::FREE(pQResult); pQResult = NULL;
		return;
	}

	ASSERT(uResultNum);
	msg.m_byLogPageCount = BYTE(  (uResultNum / MAX_GUILDWAREHOUSE_LOGPAGE_NUM) 
								+ ((uResultNum % MAX_GUILDWAREHOUSE_LOGPAGE_NUM) ? (1) : (0))	);

	//페이지 개수 구하기
	int i, k, byOffset;
	for(i=0; i < msg.m_byLogPageCount; ++i)
	{
		msg.m_byCurrentPage = i + 1; //현재 전송되는 페이지
		
		if( (uResultNum - (i * MAX_GUILDWAREHOUSE_LOGPAGE_NUM)) >= MAX_GUILDWAREHOUSE_LOGPAGE_NUM )
		{
			// 풀로 채운다
			msg.m_byLogCount = MAX_GUILDWAREHOUSE_LOGPAGE_NUM;
		}
		else
		{
			msg.m_byLogCount = BYTE( uResultNum % MAX_GUILDWAREHOUSE_LOGPAGE_NUM );	// 마지막 페이지면 나머지 계산
		}

		for(k=0; k < msg.m_byLogCount; ++k)
		{
			byOffset = (i*MAX_GUILDWAREHOUSE_LOGPAGE_NUM) + k;	// 해당 페이지에 맞는 내용을 넣는다
			msg.m_LogList[k].m_Money = pQResult->pResult[byOffset].m_Money;
			msg.m_LogList[k].m_LogType = pQResult->pResult[byOffset].m_LogType;
			msg.m_LogList[k].m_ItemCode = pQResult->pResult[byOffset].m_ItemCode;
			msg.m_LogList[k].m_LimiteCode = pQResult->pResult[byOffset].m_LimiteCode;
			msg.m_LogList[k].m_ItemNum = pQResult->pResult[byOffset].m_ItemNum;
			msg.m_LogList[k].m_UsedTime	= util::ConvertTCharToDWORD64(pQResult->pResult[byOffset].m_UsedTime);
			_tcsncpy(msg.m_LogList[k].m_UsedCharName, pQResult->pResult[byOffset].m_UsedCharName, MAX_CHARNAME_LENGTH);
			msg.m_LogList[k].m_UsedCharName[MAX_SMALLDATETIME_SIZE] = '\0';
		}

		pServerSession->SendPacket(&msg, sizeof(msg));
	}

	GuildWarehouseLogSelectQuery::FREE(pQResult);
	pQResult = NULL;
}	

Handler_FromWorldServer_IMPL(DW_GUILD_SELECT_NOTICE_SYN)
{
    __TOUCHED((pServerSession, wSize));
	PACKET_PTR_CONVERT(MSG_DW_GUILD_SELECT_NOTICE_SYN, pRecvMsg, pMsg);

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
	_sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_GuildNotice_Select(%d)}" , pRecvMsg->m_GuildGuid);
	szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

	GuildNoticeSelectQuery* pQuery = GuildNoticeSelectQuery::ALLOC();
	pQuery->SetGuildNotice();
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetCharGuid(pRecvMsg->m_OperCharGuid);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DW_GUILD, (WORD)DW_GUILD_SELECT_NOTICE_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_GUILD_SELECT_NOTICE_DBR)
{
    __TOUCHED((pServerSession, wSize));
	PACKET_PTR_CONVERT(MSG_DBPROXY_RESULT, pResult, pMsg);
	PACKET_PTR_CONVERT(GuildNoticeSelectQuery, pQResult, pResult->m_pData);

	//if(pQResult->ResultSuccess()) //사용내역이 없어도 에러코드를 보낸다.
	//{
	//	MSG_DW_GUILD_SELECT_NOTICE_NAK msg;
	//	msg.m_dwErrorCode= pQResult->ResultCode();
	//	pServerSession->SendPacket(&msg, sizeof(msg));
	//	GuildNoticeSelectQuery::FREE(pQResult);
	//	return;
	//}

	MSG_DW_GUILD_SELECT_NOTICE_ACK msg;
	msg.m_GuildGuid = pQResult->GetGuildGuid();
	msg.m_OperCharGuid = pQResult->GetCharGuid();
	msg.m_GuildNoticeType = pQResult->pResult[0].m_NoticeType;
	memcpy(msg.m_szGuildNotice, pQResult->pResult[0].m_tszGuildNotice, MAX_GUILDNOTICE_LENGTH*sizeof(TCHAR));
	msg.m_szGuildNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
	pServerSession->SendPacket(&msg, sizeof(msg));

	GuildNoticeSelectQuery::FREE(pQResult);
	pQResult = NULL;

}

//{__NA001254_20090109_RTTG_FUNC_ADD
Handler_FromWorldServer_IMPL( DW_MULTIMEMO_SEND_SYN )
{
    __TOUCHED((pServerSession, wSize));
 	MSG_DW_MULTIMEMO_SEND_SYN* pRecvMsg = (MSG_DW_MULTIMEMO_SEND_SYN*)pMsg;

	// 파싱 스트링 : '케릭명1,케릭명2,케릭명3'
	const int max_len = Query_MMemo_Send::MAX_RECVNAMES_LENGHT;	
	int LastIdx = pRecvMsg->m_MMemoInfo.m_nCount-1;
	TCHAR szRecvNames[max_len+1];
	ZeroMemory(szRecvNames,sizeof(szRecvNames));

	for( int i = 0; i < LastIdx ; ++i )
	{
		strncat(szRecvNames,pRecvMsg->m_MMemoInfo.GetRecvName(i),max_len);
		strncat(szRecvNames,",",max_len);
	}
	strncat(szRecvNames,pRecvMsg->m_MMemoInfo.GetRecvName(LastIdx),max_len);

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_1024+1];
 	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_1024, "{?=call USP_InsertMultiMemo('%s','%s','%s')}" ,
				pRecvMsg->m_MMemoInfo.m_OperCharName, szRecvNames, pRecvMsg->m_MMemoInfo.m_szMessage );

 	szQueryBuff[MAX_QUERY_LENGTH_1024]='\0';

	SUNLOG(eFULL_LOG, "[DW_MULTIMEMO_SEND_SYN] %s", szQueryBuff);

 	Query_MMemo_Send * pQuery = Query_MMemo_Send::ALLOC();
 	pQuery->SetInfo( pRecvMsg->m_MMemoInfo ); 
	pQuery->SetKey( pRecvMsg->m_dwKey );
 	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DW_MEMO, (WORD)DW_MULTIMEMO_SEND_DBR ) );
 	pQuery->SetVoidObject( pServerSession );
 	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL( DW_MULTIMEMO_SEND_DBR )
{
    __TOUCHED((pServerSession, wSize));
 	MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;
 	Query_MMemo_Send* pQResult	= (Query_MMemo_Send *)pResult->m_pData;
 

	MSG_DW_MULTIMEMO_SEND_ACK msg;
	msg.m_dwKey		= pQResult->GetKey();
	msg.m_dwCount	= pQResult->GetResultRowNum();

	_tcsncpy( msg.m_SendName, pQResult->GetInfo().m_OperCharName, MAX_CHARNAME_LENGTH );
	msg.m_SendName[MAX_CHARNAME_LENGTH] = '\0';

    // (WAVERIX) (090513) (CHANGES) int->dword, use aliasing
	// 에러가 있든 없든 셋팅해서 웁스로 보내야한다.
	for(DWORD i = 0; i < msg.m_dwCount; ++i)
	{
        Query_MMemo_Send::sQUERY& rResult = pQResult->pResult[i];
        msg.m_arrResultInfoList[i].Set(rResult.m_RecvCharName,
                                       rResult.m_nError,
                                       rResult.m_RecvGuid,
                                       rResult.m_MemoGuid,
                                       rResult.m_ptszCreateDate,
                                       rResult.m_ptszSubject);

        SUNLOG(eFULL_LOG, "[DW_MULTIMEMO_SEND_DBR] Query Info "
               "RecvCharName[%s] "
               "nError[%d] "
               "RecvGuid[%d] "
               "MemoGuid[%d] "
               "ptszCreateDate[%s] "
               "ptszSubject[%s] ",
               rResult.m_RecvCharName, 
               rResult.m_nError,
               rResult.m_RecvGuid,
               rResult.m_MemoGuid,
               rResult.m_ptszCreateDate,
               rResult.m_ptszSubject);
	}

	if( 0 != pQResult->ResultCode() || 0 == msg.m_dwCount)
 		SUNLOG(eCRITICAL_LOG, "[DW_MULTIMEMO_SEND_DBR] Query fail ResultCode[%d] dwCount[%d]", pQResult->ResultCode(), msg.m_dwCount );

	pServerSession->SendPacket( &msg, sizeof(msg) );
 
 	Query_MMemo_Send::FREE(pQResult); pQResult = NULL;
}
//}__NA001254_20090109_RTTG_FUNC_ADD

//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
Handler_FromWorldServer_IMPL(DW_COMMUNITY_MYLIST_SYN)
{
    __TOUCHED((pServerSession, wSize));
	PACKET_PTR_CONVERT(MSG_DW_COMMUNITY_MYLIST_SYN, pRecvMsg, pMsg);
	TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_CharRelation_Select_Temp(%d)}" , pRecvMsg->m_CharGuid );
	szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

	SelectQueryCommunityList * pQuery = SelectQueryCommunityList::ALLOC();
	pQuery->SetCharGuid( pRecvMsg->m_CharGuid );
	ZeroMemory(pQuery->pResult, 
		SelectQueryCommunityList ::RESULT_ROW_NUM * sizeof(SelectQueryCommunityList ::sQUERY));

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DW_COMMUNITY, (WORD)DW_COMMUNITY_MYLIST_DBR));
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromWorldServer_IMPL(DW_COMMUNITY_MYLIST_DBR)
{
    __TOUCHED((pServerSession, wSize));
	PACKET_PTR_CONVERT(MSG_DBPROXY_RESULT , pResult, pMsg);
	PACKET_PTR_CONVERT(SelectQueryCommunityList , pQResult, pResult->m_pData);
	CHARGUID requestCharGuid = pQResult->GetCharGuid();
	ULONG	rowCount = pQResult->GetResultRowNum();

	if(0 == rowCount || MAX_FRIENDBLOCK_STAT_NUM < rowCount)
	{
		MSG_DW_COMMUNITY_MYLIST_NAK nmsg;
		nmsg.m_byResult	= 1;
		nmsg.m_CharGuid = pQResult->GetCharGuid();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
		SelectQueryCommunityList ::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_COMMUNITY_MYLIST_ACK packetMsg;
	packetMsg.charGuid = pQResult->GetCharGuid();
	packetMsg.dataNum = (WORD)rowCount;

	for(ULONG i = 0 ; i < rowCount; ++i)
	{
		packetMsg.communityInfo[i].friendRelationType = pQResult->pResult[i].m_Sts;
		packetMsg.communityInfo[i].friendGuid = pQResult->pResult[i].m_FriendCharGuid;
		packetMsg.communityInfo[i].friendClass = pQResult->pResult[i].m_byClass;
		packetMsg.communityInfo[i].friendLevel = pQResult->pResult[i].m_Level;
		_tcsncpy(packetMsg.communityInfo[i].friendName, pQResult->pResult[i].m_ptszFriendCharName,
			MAX_CHARNAME_LENGTH);
	}

	pServerSession->SendPacket(&packetMsg, packetMsg.GetSize());
	SelectQueryCommunityList ::FREE(pQResult); pQResult = NULL;

	///////////...///////////...///////////...///////////...///////////...///////////...///////////...///////////...
	//나를 등록한 유저들의 리스트 Query
	TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_CharRelation_ContactMe(%d)}" , requestCharGuid);
	szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

	SelectQueryContactMeList* pQuery = SelectQueryContactMeList::ALLOC();
	pQuery->SetCharGuid(requestCharGuid);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DW_COMMUNITY, (WORD)DW_COMMUNITY_CONTACT_ME_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_COMMUNITY_CONTACT_ME_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	SelectQueryContactMeList * pQResult = (SelectQueryContactMeList *)pResult->m_pData;
	ULONG	rowCount = pQResult->GetResultRowNum();

	if(0 == rowCount || MAX_FRIENDBLOCK_STAT_NUM < rowCount)
	{
		MSG_DW_COMMUNITY_MYLIST_NAK nmsg;
		nmsg.m_byResult = 2;
		nmsg.m_CharGuid = pQResult->GetCharGuid();
		pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
		SelectQueryContactMeList::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DW_COMMUNITY_CONTACT_ME_ACK packetMsg;
	packetMsg.m_CharGuid = pQResult->GetCharGuid();
	packetMsg.m_DataNum = (WORD)rowCount;

	for(ULONG i = 0 ; i < rowCount; ++i)
	{
		packetMsg.m_CotactMEList[i] = pQResult->pResult[i].m_ContactMeCharGuid;
	}

	pServerSession->SendPacket(&packetMsg, packetMsg.GetSize());
	SelectQueryContactMeList::FREE(pQResult); pQResult = NULL;

}

///////////////////////////////////////////////////////////////
// 월드서버에서 관리되는 캐릭터 정보
///////////////////////////////////////////////////////////////
Handler_FromWorldServer_IMPL(DW_COMMUNITY_INFO_SYN)
{
    __TOUCHED((pServerSession, wSize));
	PACKET_PTR_CONVERT(MSG_DW_COMMUNITY_INFO_SYN, pRecvMsg, pMsg);
	CHARGUID requestCharGuid = pRecvMsg->m_RequestCharGuid;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_Char_CommunityData_Select(%d)}", requestCharGuid);
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	CommunityDataQuery* pQuery = CommunityDataQuery::ALLOC();

	pQuery->SetCharGuid(requestCharGuid);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD((WORD)DW_COMMUNITY, (WORD)DW_COMMUNITY_INFO_DBR));
	pQuery->SetVoidObject(pServerSession);
	pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_COMMUNITY_INFO_DBR)
{
    __TOUCHED((pServerSession, wSize));
	PACKET_PTR_CONVERT(MSG_DBPROXY_RESULT , pResult, pMsg);
	PACKET_PTR_CONVERT(CommunityDataQuery , pQResult, pResult->m_pData);

	if(pQResult->ResultSuccess())
	{
		MSG_DW_COMMUNITY_INFO_ANS packetMsg;
		packetMsg.m_RequestCharGuid = pQResult->GetCharGuid();
		packetMsg.m_RevengePoint = pQResult->pResult[0].m_RevengePoint;
		packetMsg.m_bWhisperFlag = pQResult->pResult[0].m_bWhisperFlag;
		pServerSession->SendPacket(&packetMsg, sizeof(packetMsg));
		return;
	}

	MSG_DW_COMMUNITY_INFO_NAK packetMsg;
	packetMsg.m_RequestCharGuid = pQResult->GetCharGuid();
	pServerSession->SendPacket(&packetMsg, sizeof(packetMsg));
	SUNLOG(eCRITICAL_LOG, "community_data error(%u)", pQResult->GetCharGuid());
}

//////////////////////////////////////////////////////////////////////////
//그룹 맺기 시스템
//////////////////////////////////////////////////////////////////////////
//그룹 맴버 추가 요청
Handler_FromWorldServer_IMPL( DW_GROUP_ADD_SYN )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_GROUP_ADD_SYN * pRecvMsg = (MSG_DW_GROUP_ADD_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128,"{?=call S_Group_Insert(%d, %d, %d, %d, %d)}", 
        pRecvMsg->m_fromCharGUID, pRecvMsg->m_toCharGUID, 0, pRecvMsg->m_GroupSTATFirst, pRecvMsg->m_GroupSTATSecond );
	szQueryBuff[MAX_QUERY_LENGTH_128] = '\0';

	Query_Group_Insert * pQuery = Query_Group_Insert::ALLOC();

	pQuery->SetCharGuid( pRecvMsg->m_fromCharGUID, pRecvMsg->m_toCharGUID );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD( (WORD)DW_GROUP, (WORD)DW_GROUP_ADD_DBR ));
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}
//그룹 맴버 추가 DB 결과 값 작업
Handler_FromWorldServer_IMPL( DW_GROUP_ADD_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
	Query_Group_Insert * pQResult = (Query_Group_Insert *)pResult->m_pData;

	if( pQResult->ResultCode() != Query_Group_Insert::RETURNCODE_SUCCESS )
	{
		MSG_DW_GROUP_ADD_NAK msg;
		msg.m_fromCharGUID = pQResult->GetfromCharGuid();
        msg.m_toCharGUID = pQResult->GettoCharGuid();
		msg.m_byResult = pQResult->ResultCode();

		pServerSession->SendPacket( &msg, sizeof(msg) );

		Query_Group_Insert::FREE(pQResult);
		pQResult = NULL;

		return;
	}

	MSG_DW_GROUP_ADD_ACK msg;
	msg.m_fromCharGUID = pQResult->GetfromCharGuid();
	msg.m_toCharGUID = pQResult->GettoCharGuid();

	pServerSession->SendPacket( &msg, sizeof(msg) );
	Query_Group_Insert::FREE(pQResult);
	pQResult = NULL;
}
//그룹 맴버 삭제 작업 요청
Handler_FromWorldServer_IMPL( DW_GROUP_DEL_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DW_GROUP_DEL_SYN * pRecvMsg = (MSG_DW_GROUP_DEL_SYN *)pMsg;

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128,"{?=call S_Group_Delete(%d, %d)}", pRecvMsg->m_fromCharGUID, pRecvMsg->m_toCharGUID);
	szQueryBuff[MAX_QUERY_LENGTH_128] = '\0';

	Query_Group_Delete * pQuery = Query_Group_Delete::ALLOC();

	pQuery->SetCharGuid( pRecvMsg->m_fromCharGUID, pRecvMsg->m_toCharGUID );

	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex(MAKEDWORD( (WORD)DW_GROUP, (WORD)DW_GROUP_DEL_DBR ));
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}
//그룹 맴버 삭제 DB결과 값 작업
Handler_FromWorldServer_IMPL( DW_GROUP_DEL_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
	Query_Group_Delete * pQResult = (Query_Group_Delete *)pResult->m_pData;

	if( pQResult->ResultCode() != Query_Group_Delete::RETURNCODE_SUCCESS )
	{
		MSG_DW_GROUP_DEL_NAK msg;
		msg.m_fromCharGUID = pQResult->GetfromCharGuid();
        msg.m_toCharGUID = pQResult->GettoCharGuid();
		msg.m_byResult = pQResult->ResultCode();

		pServerSession->SendPacket( &msg, sizeof(MSG_DW_GROUP_DEL_NAK) );

		Query_Group_Delete::FREE(pQResult);
		pQResult = NULL;

		return;
	}

	MSG_DW_GROUP_DEL_ACK msg;
	msg.m_fromCharGUID = pQResult->GetfromCharGuid();
	msg.m_toCharGUID = pQResult->GettoCharGuid();

	pServerSession->SendPacket( &msg, sizeof(MSG_DW_GROUP_DEL_ACK) );
	Query_Group_Delete::FREE(pQResult);
	pQResult = NULL;
}

//그룹 리스트 요청 작업
Handler_FromWorldServer_IMPL( DW_GROUP_LIST_REQ )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_GROUP_LIST_REQ * pRecvMsg = (MSG_DW_GROUP_LIST_REQ *)pMsg;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_Group_Select(%d)}",
        pRecvMsg->m_CharGUID );
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_Group_Select * pQuery = Query_Group_Select::ALLOC();

    pQuery->SetCharGuid( pRecvMsg-> m_CharGUID );
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex( MAKEDWORD( (WORD)DW_GROUP, (WORD)DW_GROUP_LIST_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery ); 
}
//그룹 리스트 쿼리 결과 값 작업
Handler_FromWorldServer_IMPL( DW_GROUP_LIST_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Group_Select* pQResult = static_cast<Query_Group_Select*>(pResult->m_pData);

    //그룹 맴버가 존재 하지 않는 경우도 존재
    if (pQResult->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]:%s", pQResult->GetErrorMsg());

        Query_Group_Select::FREE(pQResult);
        pQResult = NULL;

        return;
    }

    int datanum = pQResult->GetResultRowNum();

    assert (datanum <= MAX_GROUP_MEMBER && datanum >= 0);
    if (datanum > MAX_GROUP_MEMBER || datanum < 0)
    {
        //DB내용에서 이미 MAX값을 넘었다면 해당 내용에 대한 에러 처리는 서버에서 해줘야 한다.
        //현재 기획적으로 반복 메세지를 뿌려주고 그룹 보상을 받지 못하게 처리.
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Group Memeber Count incorrect[%d]", datanum);
    } 
    // (CHANGES) (f100425.3L) (WAVERIX) prevent buffer overrun process
    MSG_DW_GROUP_LIST_ACK msg;
    msg.m_CharGUID = pQResult->GetCharGuid();
    const int number_of_node = datanum > MAX_GROUP_MEMBER ? _countof(msg.m_GroupInfo) : datanum;
    msg.m_DataNum = static_cast<BYTE>(number_of_node);

    for (int i = 0; i < number_of_node; ++i)
    {
        STRUCT_GROUP_MEMBER_INFO& Info = msg.m_GroupInfo[i];
        STRUCT_GROUP_MEMBER_PACKET_INFO& memberInfo = Info.GroupMemberInfo;

        Query_Group_Select::sQUERY& QRecords = pQResult->pResult[i];

        Info.charGUID = QRecords.m_DestCharGUID;
        Info.accumEXP = QRecords.m_AccumEXP;
        Info.groupSTAT = QRecords.m_GroupSTAT;
        Info.lastDate = util::ConvertTCharToDWORD64(QRecords.m_UserLastDate);
        strncpy(memberInfo.ptszMemberName, QRecords.m_ptszGroupCharName, 
                _countof(memberInfo.ptszMemberName));
        memberInfo.ptszMemberName[_countof(memberInfo.ptszMemberName)-1] = '\0';
        memberInfo.byClass = QRecords.m_MemberClass;
        memberInfo.Level = QRecords.m_MemberLevel;
        memberInfo.lastDate = util::ConvertTCharToDWORD64( QRecords.m_MemberLastDate );
    }
    // (CHANGES) (f100425.3L) change to the variable size message
    pServerSession->SendPacket(&msg, msg.GetSize());

    Query_Group_Select::FREE(pQResult); 
    pQResult = NULL;
}

//상위 멤버에게 줄 경험치 축적
Handler_FromWorldServer_IMPL( DW_GROUP_ACCUMEXP_CMD )
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DW_GROUP_ACCUMEXP_CMD* pRecvMsg = static_cast<MSG_DW_GROUP_ACCUMEXP_CMD *>(pMsg);

	DBUser* pUser = g_DBUserManager.FindUser(pRecvMsg->m_UserKey);
    if (pUser == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No DBUser found [%u]", pRecvMsg->m_UserKey);
        return;
    }

    DBCharacter* dbChar = pUser->GetConnectedChar();
    if (dbChar == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No connected char found [%u]", pRecvMsg->m_UserKey);
        return;
    }
    
    //상위 멤버가 바뀌었다면 Guid를 바꾸고 저장경험치를 0으로 초기화 한다.
    if (dbChar->GetHighMemberGuid() != pRecvMsg->m_HighCharGUID)
    {
        dbChar->SetHighMemberGuid(pRecvMsg->m_HighCharGUID);
        dbChar->SetAccumEXP(0);
    }

    dbChar->AddAccumEXP(pRecvMsg->m_AccumEXP);
}


//그룹 맴버 추가 DB 결과 값 작업 요청
Handler_FromWorldServer_IMPL( DW_GROUP_ACCUMEXP_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
    Query_Group_AccumExp_Insert * pQResult = (Query_Group_AccumExp_Insert *)pResult->m_pData;

    DBUser * pUser = g_DBUserManager.FindUser( pQResult->GetUserKey() );
    DBCharacter* pCharacter = NULL;
    if (pUser != NULL)
    {
        pCharacter = pUser->GetConnectedChar();
    }

    if( pQResult->ResultCode() != Query_Group_AccumExp_Insert::RETURNCODE_SUCCESS )
    {
        //실패 했을 경우 예외 처리
        Query_Group_AccumExp_Insert::FREE(pQResult);
        pQResult = NULL;

        // 쿼리 실패 관해 guid 출력 추가
        if (pCharacter != NULL) {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GroupAccumExp_Insert Error! [Char:%d][DestChar:%d]",
                pCharacter->GetCharGuid(), pCharacter->GetHighMemberGuid());
        }
        else {
            MessageOut( eCRITICAL_LOG, "Group_AccumExp_Error" );
        }
        return;
    }
    
    //pUser가 존재 한다는 것은 재접속이나 종료가 아닌 주기적 업데이트 이다.
    if(pCharacter)
    {
        pUser->GetConnectedChar()->SetAccumEXP( 0 );//접속 종료나 재시작은 초기화가 되므로 주기적 업데이트 시에 0으로 초기화한다.
    }

    Query_Group_AccumExp_Insert::FREE(pQResult);
    pQResult = NULL;
}

//상위 멤버에게 줄 경험치 초기화
Handler_FromWorldServer_IMPL( DW_GROUP_INIT_ACCUMEXP_CMD )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_GROUP_INIT_ACCUMEXP_CMD* pRecvMsg = (MSG_DW_GROUP_INIT_ACCUMEXP_CMD*)pMsg;

    CHARGUID charGuid = pRecvMsg->m_CharGUID;

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_Group_AccumExp_Update(%d)}" , pRecvMsg->m_CharGUID );
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_Group_AccumExp_Update * pQuery = Query_Group_AccumExp_Update::ALLOC();

    pQuery->SetCharGuid( pRecvMsg-> m_CharGUID );
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex( MAKEDWORD( (WORD)DW_GROUP, (WORD)DW_GROUP_INIT_ACCUMEXP_DBR ) );
    pQuery->SetVoidObject( pServerSession );
    pServerSession->DBQuery( pQuery ); 


}

//축적 경험치 결과값 처리
Handler_FromWorldServer_IMPL( DW_GROUP_INIT_ACCUMEXP_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
    Query_Group_AccumExp_Update * pQResult = (Query_Group_AccumExp_Update *)pResult->m_pData;

    if( pQResult->ResultCode() != Query_Group_AccumExp_Update::RETURNCODE_SUCCESS )
    {
        //실패 했을 경우 예외 처리
        Query_Group_AccumExp_Update::FREE(pQResult);
        pQResult = NULL;
        MessageOut( eCRITICAL_LOG, "Query_Group_AccumExp_Update Error" );
        return;
    }

    Query_Group_AccumExp_Update::FREE(pQResult);
    pQResult = NULL;
}

//그룹 상하위 정보를 세팅한다.
Handler_FromWorldServer_IMPL( DW_GROUP_SET_MEMBERINFO_CMD )
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DW_GROUP_SET_MEMBERINFO_CMD* pRecvMsg = 
        static_cast<MSG_DW_GROUP_SET_MEMBERINFO_CMD *>(pMsg);

    DBUser* pUser = g_DBUserManager.FindUser(pRecvMsg->m_UserKey);
    if(pUser == NULL)
    {
        MessageOut(eCRITICAL_LOG, "["__FUNCTION__"]Error Not Found User");
        return;
    }

    DBCharacter* pCharacter = pUser->GetConnectedChar();
    if(pCharacter == NULL)
    {
        MessageOut(eCRITICAL_LOG, "["__FUNCTION__"]Error Not Found DBCharacter");
        return;
    }

    pCharacter->SetStatHighMember(pRecvMsg->m_HighGroupinfo);
    pCharacter->SetStatLowMember(pRecvMsg->m_LowGroupInfo);
}

//#endif //_NA_0_20080612_WORLDSERVER_REFACTORING

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { DW_LOTTO~ handlers }

Handler_FromWorldServer_IMPL(DW_LOTTO_LOAD_ENTRY_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_LOAD_ENTRY_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_LOAD_ENTRY_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_LottoEntries_Select(%d,%d,?,?,?)}", 
        pRecvMsg->iCharGuid, pRecvMsg->bNumRecent);
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_LottoEntries_Select* pQuery = Query_LottoEntries_Select::ALLOC();

    pQuery->flag         = pRecvMsg->bFlag;
    pQuery->charGuid     = pRecvMsg->iCharGuid;
    pQuery->inPreLoading = pRecvMsg->bInPreLoading;

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_LOAD_ENTRY_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_LOAD_ENTRY_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoEntries_Select* pQResult = static_cast<Query_LottoEntries_Select*>(pResult->m_pData);

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        if (pQResult->IsError()) {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoEntries_Select : %s", pQResult->GetErrorMsg());
        }
        else {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoEntries_Select returned %d", pQResult->pParam[0].m_Result);
        }
        Query_LottoEntries_Select::FREE(pQResult);
        return;
    }

    Query_LottoEntries_Select::sQUERY* pRecords = pQResult->pResult;
    MSG_DW_LOTTO_LOAD_ENTRY_ACK respMsg;

    respMsg.bFlag                = pQResult->flag;
    respMsg.iCharGuid            = pQResult->charGuid;
    respMsg.bNumRemainingTickets = pQResult->pParam->m_NumTickets;
    respMsg.bNumUsedTickets      = pQResult->pParam->m_NumUsedTickets;
    respMsg.dwEffectivePayment   = (DWORD)pQResult->pParam->m_EffectivePayment;
    respMsg.bInPreLoading        = pQResult->inPreLoading;

    size_t numRecords = pQResult->GetResultRowNum();
    ASSERT(numRecords <= MSG_DW_LOTTO_LOAD_ENTRY_ACK::MAX_ENTRIES);
    numRecords = min(numRecords, MSG_DW_LOTTO_LOAD_ENTRY_ACK::MAX_ENTRIES);
    for (int i = 0; i < (int)numRecords; ++i)
    {
        respMsg.entries[i].wDrawNum      = (WORD)pRecords[i].m_DrawNum;
        respMsg.entries[i].bStatus       = pRecords[i].m_Status;
        respMsg.entries[i].bPickedNum[0] = pRecords[i].m_PickedNum1;
        respMsg.entries[i].bPickedNum[1] = pRecords[i].m_PickedNum2;
        respMsg.entries[i].bPickedNum[2] = pRecords[i].m_PickedNum3;
        respMsg.entries[i].bPickedNum[3] = pRecords[i].m_PickedNum4;
    }
    respMsg.wNumEntries = (WORD)numRecords;

    pServerSession->SendPacket(&respMsg, (WORD)respMsg.GetSize());

    Query_LottoEntries_Select::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_LOAD_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_LOAD_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_LOAD_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoResults_Select(%d)}" , pRecvMsg->bNumRecent );
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoResults_Select* pQuery = Query_LottoResults_Select::ALLOC();

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_LOAD_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_LOAD_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoResults_Select* pQResult = static_cast<Query_LottoResults_Select*>(pResult->m_pData);

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        // 실패시 응답을 보내지 않고, 월드 서버에서 주기적으로 재시도한다
        if (pQResult->IsError()) {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoResults_Select : %s", pQResult->GetErrorMsg());
        }
        else {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoResults_Select returned %d", pQResult->pParam[0].m_Result);
        }
        Query_LottoResults_Select::FREE(pQResult);
        return;
    }

    Query_LottoResults_Select::sQUERY* pRecords = pQResult->pResult;
    MSG_DW_LOTTO_LOAD_ACK respMsg;

    respMsg.wCurrentDrawNum   = 0;
    respMsg.i64TotalAccum     = 0;
    respMsg.bNumResultRecords = 0;

    size_t numRecords = pQResult->GetResultRowNum();
    ASSERT(numRecords <= MSG_DW_LOTTO_LOAD_ACK::MAX_RESULTS);
    numRecords = min(numRecords, MSG_DW_LOTTO_LOAD_ACK::MAX_RESULTS);
    if (numRecords > 0)
    {
        // 쿼리에서 높은 회차부터 정렬되어 선택되도록 지정되어 있어야 한다
        
        // 최고 회차가 진행중이면
        if (pRecords[0].m_NumWinners < 0)
        {
            // 현재 회차 적립액 정보는 분리해서 보낸다
            respMsg.wCurrentDrawNum = pRecords[0].m_DrawNum;
            respMsg.i64TotalAccum   = pRecords[0].m_TotalAccum;
        }
        for (int i = 0; i < (int)numRecords; ++i)
        {
            respMsg.results[i].wDrawNum       = pRecords[i].m_DrawNum;
            respMsg.results[i].i64Jackpot     = pRecords[i].m_Jackpot;
            respMsg.results[i].iNumWinners    = (INT8)pRecords[i].m_NumWinners;
            respMsg.results[i].bWinningNum[0] = pRecords[i].m_WinningNum1;
            respMsg.results[i].bWinningNum[1] = pRecords[i].m_WinningNum2;
            respMsg.results[i].bWinningNum[2] = pRecords[i].m_WinningNum3;
            respMsg.results[i].bWinningNum[3] = pRecords[i].m_WinningNum4;
        }
        respMsg.bNumResultRecords = (BYTE)numRecords;
    }
    
    pServerSession->SendPacket(&respMsg, sizeof(respMsg));

    Query_LottoResults_Select::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_WINNER_LIST_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_WINNER_LIST_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_WINNER_LIST_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoWinners_Select(%d,%d,?)}", 
        pRecvMsg->wDrawNum, pRecvMsg->iPage);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoWinners_Select* pQuery = Query_LottoWinners_Select::ALLOC();

    pQuery->wDrawNum = pRecvMsg->wDrawNum;
    pQuery->iPage    = pRecvMsg->iPage;

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_WINNER_LIST_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_WINNER_LIST_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoWinners_Select* pQResult = static_cast<Query_LottoWinners_Select*>(pResult->m_pData);

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        if (pQResult->IsError()) {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoWinners_Select : %s", pQResult->GetErrorMsg());
        }
        else {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoWinners_Select returned %d", pQResult->pParam[0].m_Result);
        }
        Query_LottoWinners_Select::FREE(pQResult);
        return;
    }

    Query_LottoWinners_Select::sQUERY* pRecords = pQResult->pResult;
    MSG_DW_LOTTO_WINNER_LIST_ACK respMsg;

    respMsg.wDrawNum = pQResult->wDrawNum;
    respMsg.iPage    = pQResult->iPage;
    respMsg.iMaxPage = pQResult->pParam[0].m_EndPage;

    size_t numRecords = pQResult->GetResultRowNum();
    ASSERT(numRecords <= MSG_DW_LOTTO_WINNER_LIST_ACK::MAX_RECORDS);
    numRecords = min(numRecords, MSG_DW_LOTTO_WINNER_LIST_ACK::MAX_RECORDS);
    for (int i = 0; i < (int)numRecords; ++i)
    {
        respMsg.winners[i].i64Idx    = pRecords[i].m_Idx;
        respMsg.winners[i].iCharGuid = pRecords[i].m_CharGuid;
        respMsg.winners[i].bStatus   = pRecords[i].m_Status;
    }
    respMsg.iNumRecords = (INT32)numRecords;

    pServerSession->SendPacket(&respMsg, (WORD)respMsg.GetSize());

    Query_LottoWinners_Select::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_SAVE_ENTRY_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_SAVE_ENTRY_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_SAVE_ENTRY_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_LottoEntry_Insert(%d,%d,%d,%d,%d,%d,%d)}", 
        pRecvMsg->wDrawNum,
        pRecvMsg->iCharGuid,
        pRecvMsg->bStatus,
        pRecvMsg->bPickedNum[0],
        pRecvMsg->bPickedNum[1],
        pRecvMsg->bPickedNum[2],
        pRecvMsg->bPickedNum[3]
        );
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_LottoEntry_Insert* pQuery = Query_LottoEntry_Insert::ALLOC();

    pQuery->userKey  = pRecvMsg->m_dwKey;
    pQuery->charGuid = pRecvMsg->iCharGuid;
    pQuery->drawNum  = pRecvMsg->wDrawNum;
    pQuery->isAuto   = pRecvMsg->bIsAuto;
    pQuery->status   = pRecvMsg->bStatus;
    for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
        pQuery->pickedNum[i] = pRecvMsg->bPickedNum[i];
    }

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_SAVE_ENTRY_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_SAVE_ENTRY_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoEntry_Insert* pQResult = static_cast<Query_LottoEntry_Insert*>(pResult->m_pData);

    if (pQResult->IsError()) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoEntry_Insert : %s", pQResult->GetErrorMsg());
        Query_LottoEntry_Insert::FREE(pQResult);
        return;
    }

    MSG_DW_LOTTO_SAVE_ENTRY_ACK respMsg;

    respMsg.iResult   = pQResult->pParam->m_Result;
    respMsg.iCharGuid = pQResult->charGuid;
    respMsg.wDrawNum  = pQResult->drawNum;
    respMsg.bIsAuto   = pQResult->isAuto;
    respMsg.bStatus   = pQResult->status;
    for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
        respMsg.bPickedNum[i] = pQResult->pickedNum[i];
    }

    pServerSession->SendPacket(&respMsg, sizeof(respMsg));

    // 하임 행운 응모 액션 로깅을 위해 게임 서버로 통지
    DWORD userKey = pQResult->userKey;
    if (userKey != 0) {
        DBUser* user = g_DBUserManager.FindUser(userKey);
        if (user != NULL) {
            DBCharacter* character = user->GetConnectedChar();
            if (character != NULL) {
                if (character->GetCharGuid() == pQResult->charGuid) {
                    MSG_DG_OTHER_HEIM_LOTTO_PICK_NTF ntf;
                    ntf.m_dwKey = userKey;
                    ntf.iCharGuid = pQResult->charGuid;
                    ntf.wDrawNum = pQResult->drawNum;
                    ntf.bIsAuto = pQResult->isAuto;
                    for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
                        ntf.bPickedNum[i] = pQResult->pickedNum[i];
                    }
                    user->SendPacketToGameServer(&ntf, (WORD)sizeof(ntf));
                }
            }
        }
    }

    Query_LottoEntry_Insert::FREE(pQResult);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_ROLLOVER_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_ROLLOVER_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_ROLLOVER_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128, 
        "{?=call S_LottoResults_Insert(%d, %d, %I64d, %I64d, %I64d, %d, %d, %d, %d)}", 
        pRecvMsg->wCurrentDrawNum,
        pRecvMsg->iNumWinners,
        pRecvMsg->i64Jackpot,
        pRecvMsg->i64Prize2,
        pRecvMsg->i64Prize3,
        pRecvMsg->bWinningNum[0],
        pRecvMsg->bWinningNum[1],
        pRecvMsg->bWinningNum[2],
        pRecvMsg->bWinningNum[3]
        );
    szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

    Query_LottoResults_Insert* pQuery = Query_LottoResults_Insert::ALLOC();

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_ROLLOVER_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_ROLLOVER_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoResults_Insert* pQResult = static_cast<Query_LottoResults_Insert*>(pResult->m_pData);

    MSG_DW_LOTTO_ROLLOVER_ACK respMsg;

    if (pQResult->IsError()) {
        respMsg.iResult = -1;
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoResults_Insert : %s", pQResult->GetErrorMsg());
    }
    else {
        respMsg.iResult = pQResult->pParam->m_Result;
    }
    pServerSession->SendPacket(&respMsg, sizeof(respMsg));

    Query_LottoResults_Insert::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_ACCUM_NTF)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_ACCUM_NTF* pRecvMsg = static_cast<MSG_DW_LOTTO_ACCUM_NTF*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoTotal_Update(%d, %I64d)}", 
        pRecvMsg->wCurrentDrawNum, pRecvMsg->i64TotalAccum);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoTotal_Update* pQuery = Query_LottoTotal_Update::ALLOC();

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_ACCUM_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_ACCUM_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoTotal_Update* pQResult = static_cast<Query_LottoTotal_Update*>(pResult->m_pData);

    if (pQResult->IsError()) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoTotal_Update : %s", pQResult->GetErrorMsg());
    }
    Query_LottoTotal_Update::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_ADD_TICKET_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_ADD_TICKET_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_ADD_TICKET_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoTicket_Insert(%d,%d,%d)}", 
        pRecvMsg->iCharGuid, pRecvMsg->bNumRemainingTickets, pRecvMsg->bMaxTickets);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoTicket_Insert* pQuery = Query_LottoTicket_Insert::ALLOC();

    pQuery->charGuid   = pRecvMsg->iCharGuid;
    pQuery->numTickets = pRecvMsg->bNumRemainingTickets;

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_ADD_TICKET_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_ADD_TICKET_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoTicket_Insert* pQResult = static_cast<Query_LottoTicket_Insert*>(pResult->m_pData);

    if (pQResult->IsError()) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoTicket_Insert : %s", pQResult->GetErrorMsg());
        Query_LottoTicket_Insert::FREE(pQResult);
        return;
    }

    MSG_DW_LOTTO_ADD_TICKET_ACK respMsg;

    respMsg.iResult              = pQResult->pParam->m_Result;
    respMsg.iCharGuid            = pQResult->charGuid;
    respMsg.bNumRemainingTickets = pQResult->numTickets;

    pServerSession->SendPacket(&respMsg, sizeof(respMsg));

    Query_LottoTicket_Insert::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_REWARD_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_REWARD_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_REWARD_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoMoney_Insert(%I64d)}", 
        pRecvMsg->i64Idx);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoMoney_Insert* pQuery = Query_LottoMoney_Insert::ALLOC();

    pQuery->dbUserKey = pRecvMsg->dwUserKey;
    pQuery->entryIdx  = pRecvMsg->i64Idx;
    pQuery->drawNum   = pRecvMsg->wDrawNum;
    pQuery->charGuid  = pRecvMsg->iCharGuid;
    pQuery->status    = pRecvMsg->bStatus;
    pQuery->prize     = pRecvMsg->i64Prize;

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_REWARD_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_REWARD_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoMoney_Insert* pQResult = static_cast<Query_LottoMoney_Insert*>(pResult->m_pData);

    DWORD userKey = pQResult->dbUserKey;
    CHARGUID charGuid = pQResult->charGuid;

    if (pQResult->IsError()) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoMoney_Insert : %s", pQResult->GetErrorMsg());
        Query_LottoMoney_Insert::FREE(pQResult);
        return;
    }

    MSG_DW_LOTTO_REWARD_ACK respMsg;

    respMsg.iResult   = pQResult->pParam->m_Result;
    respMsg.dwUserKey = pQResult->dbUserKey;
    respMsg.i64Idx    = pQResult->entryIdx;
    respMsg.wDrawNum  = pQResult->drawNum;
    respMsg.iCharGuid = pQResult->charGuid;
    respMsg.bStatus   = pQResult->status;
    respMsg.i64Prize  = pQResult->prize;
    memcpy(respMsg.szCharName, pQResult->pResult->m_RecvName, MAX_CHARNAME_LENGTH * sizeof(TCHAR));
    respMsg.szCharName[MAX_CHARNAME_LENGTH] = '\0';
    
    pServerSession->SendPacket(&respMsg, sizeof(respMsg));

    Query_LottoMoney_Insert::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_NOTICE_LIST_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_NOTICE_LIST_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_NOTICE_LIST_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoNotice_Select(%d,%d)}", 
        pRecvMsg->iCharGuid, pRecvMsg->bMaxCount);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoNotice_Select* pQuery = Query_LottoNotice_Select::ALLOC();

    pQuery->iCharGuid = pRecvMsg->iCharGuid;

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_NOTICE_LIST_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_NOTICE_LIST_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoNotice_Select* pQResult = static_cast<Query_LottoNotice_Select*>(pResult->m_pData);

    if (pQResult->IsError() || !pQResult->ResultSuccess())
    {
        if (pQResult->IsError()) {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoNotice_Select : %s", pQResult->GetErrorMsg());
        }
        else {
            SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoNotice_Select returned %d", pQResult->pParam[0].m_Result);
        }
        Query_LottoNotice_Select::FREE(pQResult);
        return;
    }

    Query_LottoNotice_Select::sQUERY* pRecords = pQResult->pResult;
    MSG_DW_LOTTO_NOTICE_LIST_ACK respMsg;

    respMsg.iCharGuid = pQResult->iCharGuid;

    size_t numRecords = (int)pQResult->GetResultRowNum();
    ASSERT(numRecords <= MSG_DW_LOTTO_NOTICE_LIST_ACK::MAX_RECORDS);
    numRecords = min(numRecords, MSG_DW_LOTTO_NOTICE_LIST_ACK::MAX_RECORDS);
    for (int i = 0; i < (int)numRecords; ++i)
    {
        respMsg.notices[i].i64Index = pRecords[i].m_EntryIdx;
        respMsg.notices[i].wDrawNum = pRecords[i].m_DrawNum;
        respMsg.notices[i].bRank    = pRecords[i].m_Rank;
        respMsg.notices[i].i64Prize = pRecords[i].m_Prize;
    }
    respMsg.iNumRecords = (INT)numRecords;

    pServerSession->SendPacket(&respMsg, (WORD)respMsg.GetSize());

    Query_LottoNotice_Select::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_NOTICE_DEL_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_NOTICE_DEL_SYN* pRecvMsg = static_cast<MSG_DW_LOTTO_NOTICE_DEL_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoNotice_Delete(%d,?,?)}", 
        pRecvMsg->i64EntryIdx);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoNotice_Delete* pQuery = Query_LottoNotice_Delete::ALLOC();

    pQuery->charGuid = pRecvMsg->iCharGuid;

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_NOTICE_DEL_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_NOTICE_DEL_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoNotice_Delete* pQResult = static_cast<Query_LottoNotice_Delete*>(pResult->m_pData);

    if (pQResult->IsError()) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoNotice_Delete : %s", pQResult->GetErrorMsg());
        Query_LottoNotice_Delete::FREE(pQResult);
        return;
    }

    MSG_DW_LOTTO_NOTICE_DEL_ACK respMsg;
    respMsg.iResult   = pQResult->pParam->m_Result;
    respMsg.iCharGuid1 = pQResult->charGuid;
    respMsg.iCharGuid2 = ( respMsg.iResult == 0 ? pQResult->pParam->m_CharGuid : 0 );
    respMsg.i64Prize  = ( respMsg.iResult == 0 ? pQResult->pParam->m_Prize : 0 );
    pServerSession->SendPacket(&respMsg, (WORD)sizeof(respMsg));

    Query_LottoNotice_Delete::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_LOTTO_PAYMENT_UPDATE_NTF)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_LOTTO_PAYMENT_UPDATE_NTF* pRecvMsg = static_cast<MSG_DW_LOTTO_PAYMENT_UPDATE_NTF*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_LottoPayment_Update(%d, %d)}", 
        pRecvMsg->iCharGuid, (int)pRecvMsg->dwEffectivePayment);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_LottoPayment_Update* pQuery = Query_LottoPayment_Update::ALLOC();

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_LOTTO, (WORD)DW_LOTTO_PAYMENT_UPDATE_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_LOTTO_PAYMENT_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;
    Query_LottoPayment_Update* pQResult = static_cast<Query_LottoPayment_Update*>(pResult->m_pData);

    if (pQResult->IsError()) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] S_LottoPayment_Update : %s", pQResult->GetErrorMsg());
    }
    Query_LottoPayment_Update::FREE(pQResult);
    return;
}

Handler_FromWorldServer_IMPL(DW_DOMINATION_FINDMONEY_SYN)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DW_DOMINATION_FINDMONEY_SYN* pRecvMsg = static_cast<MSG_DW_DOMINATION_FINDMONEY_SYN*>(pMsg);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, 
               "{?=call S_DominationInfo_ActionMoney_Delete(%d,%d)}", 
               pRecvMsg->m_MapCode,
               pRecvMsg->m_RecvCharGuid);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_Domination_AuctionMoney_Delete* pQuery = Query_Domination_AuctionMoney_Delete::ALLOC();

    pQuery->SetCharGuid(pRecvMsg->m_RecvCharGuid);
    pQuery->SetMoneyType(pRecvMsg->m_CostType);
    pQuery->SetMapCode(pRecvMsg->m_MapCode);

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DW_GUILD, (WORD)DW_DOMINATION_FINDMONEY_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

Handler_FromWorldServer_IMPL(DW_DOMINATION_FINDMONEY_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult = (MSG_DBPROXY_RESULT*)pMsg;

    Query_Domination_AuctionMoney_Delete* pQResult = 
        static_cast<Query_Domination_AuctionMoney_Delete*>(pResult->m_pData);

    if (pQResult->IsError()) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s", 
            pQResult->GetQuery(), pQResult->GetErrorMsg());

        Query_Domination_AuctionMoney_Delete::FREE(pQResult);
        return;
    }

    const Query_Domination_AuctionMoney_Delete::sQUERY* pRecords = pQResult->pResult;

    MSG_DW_DOMINATION_FINDMONEY_ACK respMsg;
    respMsg.m_Result = pQResult->pParam->m_Result;
    respMsg.m_DBMapCode = pRecords->m_MapCode;
    respMsg.m_RequestMapCode = pQResult->GetMapCode();
    respMsg.m_RecvCharGuid = pQResult->GetCharGuid();
    respMsg.m_DBCharGuid = pRecords->m_CharGuid;
    respMsg.m_CostType =  pQResult->GetMoneyType();
    respMsg.m_DBCostType = (eSYSTEM_MEMO_TYPE)pRecords->RewardFlag;
    respMsg.m_TenderCost  = pRecords->m_AmountMoney;
    pServerSession->SendPacket(&respMsg, (WORD)sizeof(respMsg));

    Query_Domination_AuctionMoney_Delete::FREE(pQResult);
    return;
}

