#include "stdafx.h"
#include ".\Handler_FromGameServer.h"

#include <ResultCode.h>
#include <Macro.h>
#include <PacketStruct_DG.h>

#include <SCSlot.h>
#include <SCSlotContainer.h>

#include "Sessions/ChildServerSession.h"
#include "DBUser.h"
#include "DBUserManager.h"
#include "Sessions/GameServerSession.h"
#include "DBProxyServerEx.h"
#include "QueryObjects/GameServerQuery.h"
#include "Services/GuildManagerInDBP.h"
#include "ProxyNodes/DBQueryProxyManager.h"

#if 0
ChildServerSession* pServerSession; // argument definition argument, to support the intellisense
#endif

Handler_FromGameServer_IMPL(DG_WAREHOUSE_START_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // @history
    //  10.04.21|waverix|fit in coding style
    const MSG_DG_WAREHOUSE_START_SYN* recv_msg = static_cast<MSG_DG_WAREHOUSE_START_SYN*>(pMsg);
    DWORD user_key = recv_msg->m_dwKey;

    DBUser* db_user = g_DBUserManager.FindUser(user_key);
    if (db_user == NULL)
    {
        SUNLOG(eFULL_LOG, _T("["__FUNCTION__"]|Msg=can't find user|UserGuid=%d|"), user_key);
        return;
    }

    if (!db_user->CanDBUpdate())
    {
        MSG_DG_WAREHOUSE_START_NAK msg_nak;
        msg_nak.m_dwKey = user_key;
        msg_nak.m_dwErrorCode = 1;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    }

    if (db_user->IsWarehouseLoaded())
    {
        MSG_DG_WAREHOUSE_START_ACK msg_ack;
        msg_ack.m_dwKey = user_key;
        BOOST_STATIC_ASSERT(_countof(msg_ack.m_WarehousePwd) == MAX_WAREHOUSE_INVENTORY_PWD_LENGTH);
        strncpy(msg_ack.m_WarehousePwd, db_user->GetWarehousePassword(),
                MAX_WAREHOUSE_INVENTORY_PWD_LENGTH );
        msg_ack.m_WarehouseMoney = db_user->GetWarehouseMoney();
        // (f100527.3L)
        _WAREHOUSE_TOTAL_INFO* const warehouse_item_block = &msg_ack.m_ItemInfo;
        warehouse_item_block->Clear();
        nsQuery::DBQueryProxyUserNode* db_query_node = db_user->db_query_node();
        bool success = db_query_node->LoadItemSlotInfo(SI_WAREHOUSE,
            warehouse_item_block->m_Slot, &warehouse_item_block->m_Count);
        if (success == false)
        {
            MSG_DG_WAREHOUSE_START_NAK msg_nak;
            msg_nak.m_dwKey = user_key;
            msg_nak.m_dwErrorCode = 1;
            pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
            return;
        }

        pServerSession->SendPacket(&msg_ack, msg_ack.GetSize());
        return;
    }
    else
    {
        nsQuery::DBQueryProxyManager* db_query_proxy_manager = \
            nsQuery::DBQueryProxyManager::Instance();
        db_query_proxy_manager->Request_User_Select(\
            db_user, nsQuery::DBQueryProxyUserNode::eTrans_Select_Warehouse);
        db_query_proxy_manager->Request_User_Select(\
            db_user, nsQuery::DBQueryProxyUserNode::eTrans_Select_WareItems);
    }
}

Handler_FromGameServer_IMPL(DG_WAREHOUSE_START_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // @history
    //  10.04.21|waverix|fit in coding style
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Warehouse_Select* query_result = const_cast<Query_Warehouse_Select*>(\
        reinterpret_cast<const Query_Warehouse_Select*>(db_result->m_pData));
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();

    bool success = true;
    // (CHANGES) (080711) (WAVERIX) ERROR CODE 잘못 설정하는 문제 수정
    const DWORD user_key = query_result->GetUserKey();

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // start LOGIC_BLOCK
        IfFailThenBreak(query_result->GetResultRowNum() == 1, 20);

        // 캐쉬 처리 코드 -------------------------------------------------------------------------
        DBUser* db_user = g_DBUserManager.FindUser(user_key);
        // (NOTE) (ref100705.1) when a database result are received,
        // prevent the occurrence of an object transaction hang
        // whether the user exist at this point.
        IfFailThenBreak(db_user != NULL, 30);
        success = db_query_proxy_manager->DBResult_User_Select(*query_result, db_user);
        IfFailThenBreak(success, 40);
        //
        Query_Warehouse_Select::FREE(query_result); query_result = NULL;
        //
        assert(success);
        //
        return /*RETURN true VALUE*/;
    }
    FLOWCONTROL_END;

    // (ref100705.1)
    db_query_proxy_manager->DBResult_User_Select(user_key, *query_result, \
        nsQuery::DBQueryProxyUserNode::eTrans_Select_Warehouse);
    Query_Warehouse_Select::FREE(query_result); query_result = NULL;
    //
    MSG_DG_WAREHOUSE_START_NAK msg_nak;
    msg_nak.m_dwKey = user_key;
    msg_nak.m_dwErrorCode = 1;
    pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
    SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|UserGuid=%d|ErrorCode=%u|"),
           user_key, flow.GetErrorCode());
}

//
Handler_FromGameServer_IMPL(DG_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_ItemSlotInfo_Select* query_result = const_cast<Query_ItemSlotInfo_Select*>(\
        reinterpret_cast<const Query_ItemSlotInfo_Select*>(db_result->m_pData));
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();

    bool success = true;
    // (CHANGES) (080711) (WAVERIX) ERROR CODE 잘못 설정하는 문제 수정
    const DWORD user_key = query_result->GetUserKey();

    FlowControl flow;
    FLOWCONTROL_START()
    {
        // 캐쉬 처리 코드 -------------------------------------------------------------------------
        DBUser* db_user = g_DBUserManager.FindUser(user_key);
        // (NOTE) (ref100705.1) when a database result are received,
        // prevent the occurrence of an object transaction hang
        // whether the user exist at this point.
        IfFailThenBreak(db_user != NULL, 30);
        success = db_query_proxy_manager->DBResult_User_Select(*query_result, db_user);
        IfFailThenBreak(success, 40);

        if (FlowControl::FCAssert(db_user->IsWarehouseLoaded()) == false) {
            SetFailFc("desired db_user->IsWarehouseLoaded() is success, but failed", 45);
            break;
        }

        // 개인창고정보?
        MSG_DG_WAREHOUSE_START_ACK msg_ack;
        msg_ack.m_dwKey = user_key;
        strncpy(msg_ack.m_WarehousePwd, db_user->GetWarehousePassword(),\
                MAX_WAREHOUSE_INVENTORY_PWD_LENGTH);
        msg_ack.m_WarehouseMoney = db_user->GetWarehouseMoney();
        //
        _WAREHOUSE_TOTAL_INFO* const warehouse_item_block = &msg_ack.m_ItemInfo;
        warehouse_item_block->Clear();
        //
        nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node();
        user_node->LoadItemSlotInfo(SI_WAREHOUSE,
            warehouse_item_block->m_Slot, &warehouse_item_block->m_Count);
        //
        pServerSession->SendPacket(&msg_ack, msg_ack.GetSize());
        Query_ItemSlotInfo_Select::FREE(query_result);

        return /*RETURN true VALUE*/;
    }
    FLOWCONTROL_END;

    // (ref100705.1)
    db_query_proxy_manager->DBResult_User_Select(user_key, *query_result, \
        nsQuery::DBQueryProxyUserNode::eTrans_Select_WareItems);
    Query_ItemSlotInfo_Select::FREE(query_result); query_result = NULL;
    //
    MSG_DG_WAREHOUSE_START_NAK msg_nak;
    msg_nak.m_dwKey = user_key;
    msg_nak.m_dwErrorCode = 1;
    pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
    SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|UserGuid=%d|ErrorCode=%u|"),
           user_key, flow.GetErrorCode());
}

//
Handler_FromGameServer_IMPL(DG_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_ItemSlotInfo_Update* query_result = const_cast<Query_ItemSlotInfo_Update*>(\
        reinterpret_cast<const Query_ItemSlotInfo_Update*>(db_result->m_pData));
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();

    const USERGUID user_guid = query_result->parameters_[0].guid_;
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, user_guid);
        return;
    };
    //
    db_query_proxy_manager->DBResult_User_Update(*query_result, db_user);
    //
    // release resource
    Query_ItemSlotInfo_Update::FREE(query_result);
};

Handler_FromGameServer_IMPL(DG_WAREHOUSE_END_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_DG_WAREHOUSE_END_SYN* pRecvMsg = (MSG_DG_WAREHOUSE_END_SYN *)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;

	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG,  "[Handler_FromGameServer_IMPL(DG_WAREHOUSE_END_SYN)] [오류]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}

	//SUNLOG( eFULL_LOG, "[%s]창고종료", pUser->GetSelectedCharName() );

	MSG_DG_WAREHOUSE_END_ACK msgACK;
	msgACK.m_dwKey	= UserKey;
	pServerSession->SendPacket( &msgACK, sizeof(msgACK) );
}

Handler_FromGameServer_IMPL(DG_WAREHOUSE_UPDATE_INNER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Warehouse_Update* query_result = \
        reinterpret_cast<Query_Warehouse_Update*>(db_result->m_pData);
    const DWORD user_key = query_result->GetUserKey();

    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_key, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find an user (%u)|"),
               __FUNCTION__, user_key);
        goto RELEASE_RESOURCE;
    };
    // success
    db_query_proxy_manager->DBResult_User_Update(*query_result, db_user);
    //
RELEASE_RESOURCE:
    Query_Warehouse_Update::FREE(const_cast<Query_Warehouse_Update*>(query_result));
    return;
}

//==================================================================================================
//  Guild Warehouse
//==================================================================================================

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_CMD, recv_msg, pMsg);
    // DONE: f110627.1L, f101021.2L, change logic to enhanced synchronize about guild management
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
    GUILDGUID guild_guid = recv_msg->m_GuildGuid;
    GuildWarehouseInfo* const guild_warehouse_info =
        g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    if (guild_warehouse_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find guild warehouse, user(%u)|"),
               __FUNCTION__, user_guid);
        return;
    };

    guild_warehouse_info->SetWarehouseMoney(recv_msg->m_WarehouseMoney);
    _GUILDWAREHOUSE_TOTAL_INFO* const guild_warehouse_item_block = &recv_msg->m_ItemInfo;
    guild_warehouse_info->StoreItemSlotInfo(SI_GUILDWAREHOUSE,
        guild_warehouse_item_block->m_Slot, guild_warehouse_item_block->m_Count);
    const CHARGUID char_guid = recv_msg->char_guid_;
    CHARGUID locker_char;
    USERGUID locker = guild_warehouse_info->GetOpenLocker(&locker_char);
    DBCharacter* const db_character = db_user->GetConnectedChar();
    bool binded = (db_character && db_character->GetCharGuid() == char_guid);
    // NOTE: f110627.1L, support the custom version processing.
    if (binded) {
        db_character->BindGuildWarehouse(guild_warehouse_info);
    };

    if ((binded == false) ||
        (locker == 0) || (locker_char == 0) ||
        (locker != user_guid) || (locker_char != char_guid))
    {
        //언제 발생 되는지 확인필요
        CHARGUID current_char = (db_character != NULL) ? db_character->GetCharGuid() : 0;
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|invalid open locker, ")
               _T("user:locker=%u:%u, char:locker=%u:%u, current_char=%u|")
               __FUNCTION__, user_guid, locker, char_guid, locker_char, current_char);
    };
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSE_UPDATE_INNER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    GuildWarehouseUpdateQuery* query_result = \
        reinterpret_cast<GuildWarehouseUpdateQuery*>(db_result->m_pData);
    const GUILDGUID guild_guid = query_result->GetGuildGuid();
    GuildWarehouseInfo* guild_warehouse_info = g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    //
    bool success = false;
    if (guild_warehouse_info) {
        success = guild_warehouse_info->DBResult(*query_result);
    };
    const USERGUID user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();
    if (success == false)
    {
    #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
        MSG_DG_GUILD_WAREHOUSE_CLOSE_NAK msg_nak;
        msg_nak.m_byErrorCode = 1;
        msg_nak.m_GuildGuid = guild_guid;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
    #endif
        ;{
            GuildWarehouseUpdateQuery::FREE(query_result);
            query_result = NULL;
        };
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|ERROR|UserGuid=%u|CharGuid=%u|GuildGuid=%u|"),
               user_guid, char_guid, guild_guid);
        return;
    }

    // NOTE: f101011.2L, I think that this logic has many problems. need change more enhanced logic.
    // CHANGES: f101112.1L, fixed synchronization problem, if user exit a game on opening warehouse.
    // CHANGES: f101117.1L, the below code block is removed because of useless.
    // this routine is result by 'Update' of GuildWarehouseInfo.
    // that means that the owner, to invoke query, would be differ with that to receive reply.
    // furthermore, it is the only reply process about 'Update' not post events.
    //
    // pseudo code for history
    //  #1. find an user and a connected character (possible corrupt data)
    //  #2. unlink a guild warehouse and a character
    //      (...to support one-time update? how to you next update? ...mythery...)
    //  #2-2. if character is not found, lock a guild warehouse. (...terrible)
    //
    ;{  // release resources
        GuildWarehouseUpdateQuery::FREE(query_result);
        query_result = NULL;
    };
    return;
    //
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_ItemSlotInfo_Update* query_result = \
        reinterpret_cast<Query_ItemSlotInfo_Update*>(db_result->m_pData);
    const Query_ItemSlotInfo_Update::sQueryParam& parameter = query_result->parameters_[0];
    const GUILDGUID guild_guid = parameter.guid_;
    GuildWarehouseInfo* guild_warehouse_info = g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    //
    bool success = false;
    if (guild_warehouse_info) {
        success = guild_warehouse_info->DBResult(*query_result);
    };
    //
    // release resources
    Query_ItemSlotInfo_Update::FREE(query_result);
    //
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSE_CLOSE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_CLOSE_SYN, recv_msg, pMsg);

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
    GUILDGUID guild_guid = recv_msg->m_GuildGuid;
    GuildWarehouseInfo* const guild_warehouse_info =
        g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    if (guild_warehouse_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find guild warehouse, user(%u)|"),
               __FUNCTION__, user_guid);
        return;
    };
    // NOTE: the forced closing command by gm will skip who is a locker of the warehouse.
    CHARGUID open_locker_char;
    USERGUID open_locker = guild_warehouse_info->GetOpenLocker(&open_locker_char);
    if (user_guid != open_locker)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|closed the guild warehouse with a different locker|")
               _T("user:user=%d:%d, char=%d|"),
               __FUNCTION__, open_locker, user_guid, open_locker_char);
    };
    guild_warehouse_info->ReleaseOpenLock();
    //
    MSG_DG_GUILD_WAREHOUSE_CLOSE_ACK msg_ack;
    msg_ack.m_dwKey = user_guid;
    msg_ack.m_GuildGuid = guild_guid;
    pServerSession->SendPacket(&msg_ack, sizeof(msg_ack));
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSELOG_UPDATE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSELOG_UPDATE_SYN, pRecvMsg, pMsg);
    const USERGUID user_guid = pRecvMsg->m_dwKey; // updater
    GUILDGUID guild_guid = pRecvMsg->m_GuildGuid;
    MONEY in_out_heim = pRecvMsg->m_Money;
	STRING_SAFER_CHAR_NAME szCharName(pRecvMsg->m_CharName);
	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( 
		"{?=call S_GuildWarehouseLog_Insert(%d, %d, %d, %d, '%s', %I64u, %d)}",
		pRecvMsg->m_bInOutState,
        guild_guid,
		pRecvMsg->m_ItemCode,
		pRecvMsg->m_ItemNum,
		(TCHAR*)szCharName,
		in_out_heim,
        pRecvMsg->m_LimitedIndex
		);
	
	GuildWarehouseLogInsertQuery* pQuery = GuildWarehouseLogInsertQuery::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DG_WAREHOUSE, (WORD)DG_GUILD_WAREHOUSELOG_UPDATE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );

    //----------------------------------------------------------------------------------------------
    // CHANGES: f110627.1L, it directly added about the real heim(money) in the guild warehouse
    GuildWarehouseInfo* const guild_warehouse_info =
        g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    if (guild_warehouse_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find guild warehouse, guild(%u)|"),
               __FUNCTION__, guild_guid);
        return;
    };

    CHARGUID open_locker_char;
    const USERGUID open_locker = guild_warehouse_info->GetOpenLocker(&open_locker_char);
    if (open_locker != user_guid)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|the updater(%d) differ the current opener(user:%d,char:%d)|"),
               __FUNCTION__, pRecvMsg->m_dwKey, open_locker, open_locker_char);
    };
    if (pRecvMsg->m_bInOutState == MONEY_IN ||
        pRecvMsg->m_bInOutState == MONEY_OUT)
    {
        const bool deposit = (pRecvMsg->m_bInOutState == MONEY_IN);
        MONEY current_heim = guild_warehouse_info->GetWarehouseMoney();
        if (deposit) {
            current_heim += in_out_heim;
        } else {
            if (FlowControl::FCAssert(current_heim >= in_out_heim) == false) {
                current_heim = 0;
            } else {
                current_heim -= in_out_heim;
            };
        };
        guild_warehouse_info->SetWarehouseMoney(current_heim); // for fast update
    };
    //----------------------------------------------------------------------------------------------
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSELOG_UPDATE_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DBPROXY_RESULT, pResult, pMsg);
	PACKET_PTR_CONVERT(GuildWarehouseLogInsertQuery, pQResult, pResult->m_pData);
	
	if(!pQResult->ResultSuccess())	
	{
		SUNLOG(eDEV_LOG, "GUILDWAREHOUSE_LOG DBSAVE_ERROR!!!");
		GuildWarehouseLogInsertQuery::FREE(pQResult); 
		return;
	}

	GuildWarehouseLogInsertQuery::FREE(pQResult); 
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSE_TAPEX_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DG_GUILD_WAREHOUSE_TAPEX_SYN, pRecvMsg, pMsg);

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( 
		"{?=call S_GuildWarehouse_SlotTapEx(%d, %d, %d)}",
		pRecvMsg->m_GuildGuid, 
		pRecvMsg->m_TabCount, 
		pRecvMsg->m_CharGuid);

	GuildWarehouseTabExQuery* pQuery = GuildWarehouseTabExQuery::ALLOC();
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetSlotTabNum(pRecvMsg->m_TabCount);
	pQuery->SetGuildGuid(pRecvMsg->m_GuildGuid);
	pQuery->SetSlotIndex(pRecvMsg->m_SlotIndex);
	pQuery->SetSlotPos(pRecvMsg->m_SlotPos);
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetIndex( MAKEDWORD( (WORD)DG_WAREHOUSE, (WORD)DG_GUILD_WAREHOUSE_TAPEX_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pServerSession->DBQuery( pQuery );
}

Handler_FromGameServer_IMPL(DG_GUILD_WAREHOUSE_TAPEX_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_DBPROXY_RESULT, pResult, pMsg);
	PACKET_PTR_CONVERT(GuildWarehouseTabExQuery, pQResult, pResult->m_pData);

	if(!pQResult->ResultSuccess())	
	{
		MSG_DG_GUILD_WAREHOUSE_TAPEX_NAK msg;
		msg.m_dwKey = pQResult->GetUserKey();
		msg.m_bErrorCode = pQResult->GetGuildResult(pQResult->pParam[0].m_Result);
		pServerSession->SendPacket(&msg, sizeof(msg));
		GuildWarehouseTabExQuery::FREE(pQResult); 
		pQResult = NULL;
		return;
	}


	GuildWarehouseInfo* pGuildWarehouseInfo =  g_GuildManagerInDBP.FindGuildWarehouse(pQResult->GetGuildGuid());
	if(pGuildWarehouseInfo)
	{
		pGuildWarehouseInfo->SetSlotTabNum(pQResult->GetSlotTabNum());
		
		MSG_DG_GUILD_WAREHOUSE_TAPEX_ACK msg;
		msg.m_dwKey = pQResult->GetUserKey();
		msg.m_GuildGuid = 	pQResult->GetGuildGuid();
		msg.m_TabCount = pQResult->GetSlotTabNum();
		msg.m_SlotIndex = pQResult->GetSlotIndex();
		msg.m_SlotPos = pQResult->GetSlotPos();
		pServerSession->SendPacket(&msg, sizeof(msg));
	}
	else
	{
		//여긴 들어오는 경우가 생겨서는 안됨
		SUNLOG(eCRITICAL_LOG, "GuildWarehouseTabExQuery Exist None[%u][%u]", pQResult->GetGuildGuid(), pQResult->GetUserKey());
	}

	GuildWarehouseTabExQuery::FREE(pQResult);
	pQResult = NULL;
}
