#include "stdafx.h"
#include ".\Handler_FromGameServer.h"

#include <Macro.h>
#include <ResultCode.h>
#include <SCSlot.h>
#include <SCSlotContainer.h>
#include <PacketStruct_DG.h>

#include "Sessions/ChildServerSession.h"
#include "DBUser.h"
#include "DBUserManager.h"
#include "Sessions/GameServerSession.h"
#include "DBProxyServerEx.h"
#include "QueryObjects/GameServerQuery.h"
#include "ProxyNodes/DBQueryProxyManager.h"

#if 0
ChildServerSession* pServerSession; // argument definition argument, to support the intellisense
#endif

Handler_FromGameServer_IMPL(DG_CHARINFO_CONNECT_CHAR_CMD)
{
    __TOUCHED((wSize));
    // CHANGES: f110426.2L,
    const MSG_DG_CHARINFO_CONNECT_CHAR_CMD* recv_msg =
        static_cast<MSG_DG_CHARINFO_CONNECT_CHAR_CMD*>(pMsg);
    const USERGUID user_guid = recv_msg->m_dwKey;
    const CHARGUID char_guid = recv_msg->char_guid;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    //
    DBCharacter* const prev_db_character = db_user->GetConnectedChar();
    if (prev_db_character != NULL)
    {
        ChildServerSession* user_session = db_user->GetServerSession();
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=mismatch sync. DBChar already exist|")
               _T("Addins: UserGuid=%d, DBState=%d, ")
               _T("CharGuid(prev)=%d -> CharGuid(new)=%d, ")
               _T("ServerInfo(prev)={%d,%d} -> ServerInfo(new)={%d,%d}|"),
               __FUNCTION__,
               user_guid, db_user->GetDBState(),
               prev_db_character->GetCharGuid(), char_guid, 
               (user_session ? user_session->GetSessionIndex() : 0),
               (user_session ? user_session->GetServerType() : 0),
               pServerSession->GetSessionIndex(),
               pServerSession->GetServerType());
        //
        db_user->UpdateChar(eDB_UPDATE_INNER, db_user->GetChannelID(), prev_db_character);
        if (prev_db_character != db_character) {
            db_user->OnDisconnectChar();
        };
    };
    // NOTE: if this case occurred, we have to regard intended abusing action
    if (find_result == db_query_proxy_manager->eFindResult_Pending)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=unexpected states. (pending state)|")
               _T("UserGuid=%d, State=%d, Transaction=%d, Update=%d|"),
               __FUNCTION__,
               user_guid, db_user->GetDBState(),
               db_user->IsTransactionSerialize(), db_user->IsTransactionDBUpdate());
        return;
    };
    //
    ASSERT(db_user->CanLogin());
    if (prev_db_character != db_character) {
        db_user->OnConnectChar(char_guid); // assertion
    };
    db_user->SetServerSession(pServerSession);
    //
    ;{
        // NOTE: f110429.4L
        db_user->UpdateLatestUpdateTick();
        db_user->IncreaseChannelTransactionPhase();
        // NOTE: f110622.2L, call twice, this process support to reduce irregula hangs
        // like an unexpected fault down server
        db_user->IncreaseChannelTransactionPhase();
        // CHANGES: f110426.2L, changes to forwarding message to make successful replication process.
        MSG_DG_CHARINFO_CONNECT_CHAR_CMD msg_ack = *recv_msg;
        pServerSession->SendPacket(&msg_ack, msg_ack.GetSize());
    };
    //
    return;
    //----------------------------------------------------------------------------------------------
#if SUN_CODE_BACKUP
	MSG_DG_CHARINFO_CONNECT_CHAR_CMD* pRecvMsg = (MSG_DG_CHARINFO_CONNECT_CHAR_CMD*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	CHARGUID CharGuid = pRecvMsg->m_CharGuid;

	DBUser * pUser = g_DBUserManager.FindUser(UserKey);
	if( !pUser )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CONNECT_CHAR_CMD][UGuid:%u,CGuid:%u]DBUserIsNotFound", UserKey, CharGuid );
		return;
	}
	if( pUser->GetConnectedChar() )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CONNECT_CHAR_CMD][UGuid:%u,CGuid:%u]DBCharacterAlreadyExist(%u),Index(%u),t(%u)", 
			UserKey, CharGuid, pUser->GetDBState(), pServerSession->GetSessionIndex(), pServerSession->GetServerType() );
		if( pUser->GetServerSession() )
		{
			SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CONNECT_CHAR_CMD][UGuid:%u,CGuid:%u]DBCharacterAlreadyExist(%u),Index(%u),t(%u)", 
				UserKey, CharGuid, pUser->GetDBState(), pUser->GetServerSession()->GetSessionIndex(), pUser->GetServerSession()->GetServerType() );
		}
		pUser->OnDisconnectChar();
	}

	if( !pUser->CanLogin() )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CONNECT_CHAR_CMD][UGuid:%u,CGuid:%u] State(%u) Transaction(%u/%u) IsStrange!", 
			UserKey, CharGuid, pUser->GetDBState(), pUser->IsTransactionSerialize(), pUser->IsTransactionDBUpdate() );
		pUser->SetDBState(eDB_STATE_CREATED);
	}

	DBCharacter* pCharacter = pUser->FindCharacter( CharGuid );
	if( !pCharacter )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CONNECT_CHAR_CMD][UGuid:%u,CGuid:%u]DBCharacterIsNotFound", UserKey, CharGuid );
		return;	
	}

	// 캐릭터 접속
	if( !pUser->OnConnectChar( CharGuid ) )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CONNECT_CHAR_CMD][UGuid:%u,CGuid:%u]OnConnectCharFailed", UserKey, CharGuid );
	}
	pUser->SetServerSession( pServerSession );
#endif
}

Handler_FromGameServer_IMPL(DG_CHARINFO_DISCONNECT_CHAR_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_DISCONNECT_CHAR_CMD* recv_msg = \
        static_cast<MSG_DG_CHARINFO_DISCONNECT_CHAR_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;

    DBUser* db_user = g_DBUserManager.FindUser(user_key);
    if (db_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("["__FUNCTION__"]|Msg=can't find DBUser|UserGuid=%d|"), user_key);
        return;
    }
}

// 유저 접속 해제
Handler_FromGameServer_IMPL(DG_CHARINFO_DISCONNECT_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_DISCONNECT_CMD* recv_msg = \
        static_cast<MSG_DG_CHARINFO_DISCONNECT_CMD*>(pMsg);
    const USERGUID user_guid = recv_msg->m_dwKey;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, user_guid);
        return;
    };

    // NOTE: f110426.2L, check duplicated connection states
    BYTE current_channel_id = db_user->GetChannelID();
    if (current_channel_id == recv_msg->m_ChannelID)
    {
        db_user->LeaveChannelTransaction();
        if (find_result == db_query_proxy_manager->eFindResult_Service) {
            db_user->Disconnect(recv_msg->m_ChannelID);
        }
        db_user->OnDisconnectChar();
    }
    else
    {
        // NOTE: f110426.2L, I may think that this state can a reversed sequence
        // like a duplicated connection status.
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=unexpected status, tic-toc mechanism is crashed ")
               _T("about connection and disconnection flow is crashed|UserGuid=%d|"),
               user_guid);
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN)
{
    // CHANGES: f110412.2L, code arrangement and added 'char_guid' control
    // to prevent problem that can't find handle like user
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN* recv_msg =
        static_cast<MSG_DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN*>(pMsg);
    const USERGUID user_guid = recv_msg->m_dwKey;
    const CHARGUID char_guid = recv_msg->char_guid_;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    //
    switch (recv_msg->m_byTransControl)
    {
    case recv_msg->SERIALIZE_LOCKING:
        db_user->EnterTransactionSerialize();
        // NOTE: f110429.4L, a dangling user occur possibility
        // when he moves a field server to a battle server
        db_user->UpdateLatestUpdateTick();
        break;
    case recv_msg->SERIALIZE_UNLOCKING:
        db_user->LeaveTransactionSerialize();
        break;
    case recv_msg->DBUPDATE_LOCKING:
        db_user->EnterTransactionDBUpdate();
        break;
    case recv_msg->DBUPDATE_UNLOCKING:
        db_user->LeaveTransactionDBUpdate();
        break;
    case recv_msg->SERIALIZE_UNLOCKING_WITH_GUILDWAREHOUSE:
        db_user->LeaveTransactionSerialize();
        db_user->EnterTransactionDBUpdate();
        if (db_user->UpdateChar(eDB_UPDATE_GUILDWAREHOUSE_SYN,
                recv_msg->m_ChannelID, db_character) == false)
        {
            //(lst1024)(090619) NULL 참조 버그 수정. (pUser->GetConnectedChar()->Get...)
            SUNLOG(eCRITICAL_LOG, _T("|User update fails with a guild warehouse|UserGuid=%d|"),
                   user_guid);
        }
        break;
    };
}

// 정보 조회 요청
Handler_FromGameServer_IMPL(DG_CHARINFO_QUERY_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_CHARINFO_QUERY_SYN* pRecvMsg = (MSG_DG_CHARINFO_QUERY_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DWORD ErrorCode = 0;
	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		ErrorCode = 1;
		goto lb_NakMsg;
	}

	if( pUser->GetServerSession() != pServerSession )
	{
		CHARGUID char_guid = 0;
		if( pUser->GetConnectedChar() )
			char_guid = pUser->GetConnectedChar()->GetCharGuid();
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_QUERY_SYN][UserGuid:%u,C:%d]SessionPtrIsDifferent(%u),index(%u),t(%u)", 
			UserKey, char_guid, pUser->GetDBState(), pServerSession->GetSessionIndex(), pServerSession->GetServerType() );
		if( pUser->GetServerSession() )
		{
			SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_QUERY_SYN][UGuid:%u,C:%d]ServerSessionIndex index(%u),t(%u)", 
				UserKey, char_guid, pUser->GetServerSession()->GetSessionIndex(), pUser->GetServerSession()->GetServerType() );
		}
		ErrorCode = 2;
		goto lb_NakMsg;
	}

	if( !pUser->CanLogin() )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_QUERY_SYN][UserGuid:%u] State(%u) Transaction(%u/%u) IsStrange!", UserKey, pUser->GetDBState(), 
			pUser->IsTransactionSerialize(), pUser->IsTransactionDBUpdate() );
		ErrorCode = 2;
		goto lb_NakMsg;
	}

	DBCharacter * pCharacter = pUser->GetConnectedChar();
	if( pCharacter )
	{
        nsQuery::DBQueryProxyManager* const query_proxy_manager = \
            nsQuery::DBQueryProxyManager::Instance();
        nsQuery::DBQueryProxyManager::eEnterGameResult enter_game_result = \
            query_proxy_manager->EnterGamePendingProcess(pUser, pCharacter, pServerSession);
        if (enter_game_result == query_proxy_manager->eEnterGameResult_Fail) {
            ErrorCode = 3;
            goto lb_NakMsg;
        }
        if (enter_game_result == query_proxy_manager->eEnterGameResult_Enterable) {
            query_proxy_manager->SendCharInfoToGameOnAllCharDataLoaded(pUser, pCharacter);
            MSG_DG_CHARINFO_QUERY_ACK msg_ack;
            pUser->SendPacketToGameServer(&msg_ack, sizeof(msg_ack));
        }
        if (enter_game_result == query_proxy_manager->eEnterGameResult_Pending) {
            // not handled about query_proxy_manager->eEnterGameResult_Pending,
            // that state is controlled delayed process.
            nsQuery::DBQueryProxyCharNode* char_node = pCharacter->db_query_node();
            char_node->RequestEnterGame(pRecvMsg, pServerSession); /// check multi request
        }
        //
	}
	else
	{
		ErrorCode = 3;
		goto lb_NakMsg;
	}

	return;

lb_NakMsg:
	switch(ErrorCode)
	{
	case 1:
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_QUERY_SYN][UserGuid:%u] Not Exist DBUser", UserKey );
		break;
	case 2:
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_QUERY_SYN][UserGuid:%u]SessionPtrIsDifferent", UserKey );
		break;
	case 3:
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_QUERY_SYN][UserGuid:%u]DBCharacterIsNotFound", UserKey );
		break;
	}

	MSG_DG_CHARINFO_QUERY_NAK nmsg;
	nmsg.m_dwKey = UserKey;

	switch(ErrorCode)
	{
	case 1:
		nmsg.m_dwErrorCode = RC::RC_DISCONNECT_NOT_FOUND_DB_USER;
		break;
	case 2:
		nmsg.m_dwErrorCode = RC::RC_DISCONNECTION_INVALID_CHAR_STATE;
		break;
	case 3:
		nmsg.m_dwErrorCode = RC::RC_DISCONNECT_NOT_CONNECTED_DB_CHARACTER;
		break;
	}

	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
}

// 정보 저장 요청
Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_SYN)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_UPDATE_SYN* recv_msg = static_cast<MSG_DG_CHARINFO_UPDATE_SYN*>(pMsg);
    // CHANGES: f110412.2L, changes logic to prevent disabled update
    // when the this entry point has connectless character
    const USERGUID user_guid = recv_msg->m_dwKey;
    const CHARGUID char_guid = recv_msg->char_guid_;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound)
    {
        MSG_DG_CHARINFO_UPDATE_NAK msg_nak;
        msg_nak.m_dwKey = user_guid;
        msg_nak.m_dwErrorCode = RC::RC_DISCONNECT_NOT_FOUND_DB_USER;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    //
    if (db_user->UpdateChar(eDB_UPDATE_SYN, recv_msg->m_ChannelID, db_character) == false)
    {
        MSG_DG_CHARINFO_UPDATE_NAK msg_nak;
        msg_nak.m_dwKey = user_guid;
        msg_nak.m_dwErrorCode = RC::RC_DISCONNECT_NOT_CONNECTED_DB_CHARACTER;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
               "["__FUNCTION__"]|Msg=irregular status|UserGuid=%d|State=%d|Transaction=(%u/%u)|",
               user_guid, db_user->GetDBState(),
               db_user->IsTransactionSerialize(), db_user->IsTransactionDBUpdate());
        return;
    }
    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    ;{  // character leave field update event
        MSG_DG_CHARINFO_UPDATE_ACK msg_ack;
        db_user->SendPacketToGameServer(&msg_ack, sizeof(msg_ack));
        db_user->OnDisconnectChar();
        db_user->LeaveChannelTransaction(); // NOTE: f110622.2L, reduce user connection hangs.
        db_user->SetServerSession(pServerSession); // (NOTE) custom logic is settled null
    };
}

Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Char_Update* const query_result = \
        reinterpret_cast<Query_Char_Update*>(db_result->m_pData);
    const DWORD user_guid = query_result->GetUserKey();

    using nsQuery::DBQueryProxyManager;
    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();
    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL)
    {
        db_query_proxy_manager->DBResult_Char_Update(user_guid, query_result->char_guid(), \
            nsQuery::DBQueryProxyCharNode::eTrans_Update_CharInfo);
        Query_Char_Update::FREE(query_result);
        return;
    }

    db_query_proxy_manager->DBResult_Char_Update(*query_result, db_user);
    Query_Char_Update::FREE(query_result);
    return;
}

//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_INVEN_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Char_Update_Inventory* const query_result = \
        reinterpret_cast<Query_Char_Update_Inventory*>(db_result->m_pData);
    const DWORD user_guid = query_result->GetUserKey();

    using nsQuery::DBQueryProxyManager;
    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();
    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL)
    {
        db_query_proxy_manager->DBResult_Char_Update(user_guid, query_result->char_guid(),\
            nsQuery::DBQueryProxyCharNode::eTrans_Update_InvenInfo);
        Query_Char_Update_Inventory::FREE(query_result);
        return;
    }

    db_query_proxy_manager->DBResult_Char_Update(*query_result, db_user);
    Query_Char_Update_Inventory::FREE(query_result);
    return;
}

// (WAVERIX) DB Result
Handler_FromGameServer_IMPL(DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Password_Update* pQResult	= (Query_Password_Update*)pResult->m_pData;
	DWORD UserKey					= pQResult->GetUserKey();

	MSG_DG_WAREHOUSE_PWD_SETTING_ACK msgACK;

	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG(eFULL_LOG, "[DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR][UserGuid:%u]DBUserIsNotFound(%s)", UserKey, pQResult->GetQuery() );
		goto RELEASE_ROUTINE;
	}

	DBCharacter* pCharacter = pUser->GetConnectedChar();
	if( !pCharacter )
	{
		// 이 경우가 발생한다면 정말 미묘...
		//msgACK.m_byRetCode = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_START_FAIL;
		//pUser->SendPacketToGameServer( &msgACK, sizeof(msgACK) );

		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR][UGuid:%u]DBCharacterIsNotConnected", UserKey );
		goto RELEASE_ROUTINE;
	}

	DBUser::ePWD_TRANSACTION eTrans = pUser->PasswordTransaction();
	sPASSWORD_BLOCK_IN_DBPROXY* pPwdBlock
		= DBUser::ePWD_TRANS_WAREHOUSE == eTrans ? pUser->GetTempWarehousePassword()
		: DBUser::ePWD_TRANS_INVENTORY == eTrans ? pCharacter->GetTempInventoryPassword()
		: NULL;

	if( !(pPwdBlock && pQResult->ResultSuccess())  )
	{
		msgACK.m_byRetCode = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_FAIL;
		switch( eTrans )
		{
		case DBUser::ePWD_TRANS_WAREHOUSE:
			strncpy( msgACK.m_Password, pUser->GetWarehousePassword(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
			msgACK.TypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::PWD_WAREHOUSE;
			break;
		case DBUser::ePWD_TRANS_INVENTORY:
			strncpy( msgACK.m_Password, pCharacter->GetInventoryPassword(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
			msgACK.TypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::PWD_INVENTORY;
			break;
		default:
			{
				msgACK.m_byRetCode = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_START_FAIL;
			}
			break;
		}

		pUser->SendPacketToGameServer( &msgACK, sizeof(msgACK) );
		SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR][UGuid:%u]Invalid Password -or- Failed", UserKey );
		goto RELEASE_ROUTINE;
	}

	// 여기까지 오는 것은 성공이라는 소리.
	{
		msgACK.ReturnCode() = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_SUCCESS;
		switch( pPwdBlock->Type() )
		{
		case sPASSWORD_BLOCK_IN_DBPROXY::WAREHOUSE:
			pUser->SetWarehousePassword( pPwdBlock->Get() );
			strncpy( msgACK.m_Password, pUser->GetWarehousePassword(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
			msgACK.TypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::PWD_WAREHOUSE;
			break;
		case sPASSWORD_BLOCK_IN_DBPROXY::INVENTORY:
			pCharacter->SetInventoryPassword( pPwdBlock->Get() );
			strncpy( msgACK.m_Password, pCharacter->GetInventoryPassword(), MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
			msgACK.TypeCode() = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::PWD_INVENTORY;
			break;
		}

		pUser->PasswordTransaction() = DBUser::ePWD_NO_TRANS;
		pUser->SendPacketToGameServer( &msgACK, sizeof(msgACK) );

		g_DBProxyServer.FreeTempPasswordBlock( pPwdBlock );
	}

RELEASE_ROUTINE:
	Query_Password_Update::FREE( pQResult );
	pQResult = NULL;
}


//피로도 저장
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE

//GameServer에서 Update
Handler_FromGameServer_IMPL(DG_OTHER_FATIGUE_CMD)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_OTHER_FATIGUE_CMD* pRecvMsg = (MSG_DG_OTHER_FATIGUE_CMD *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;

	DBUser* pUser = g_DBUserManager.FindUser(UserKey);
	if( !pUser )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_OTHER_FATIGUE_CMD][UGuid:%u,CGuid:%u]DBUserIsNotFound", UserKey );
		return;
	}

	pUser->SetPlayTime( pRecvMsg->m_wPlayTime );
	pUser->SetOfflineTime( pRecvMsg->m_wOfflineTime );
}

#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE