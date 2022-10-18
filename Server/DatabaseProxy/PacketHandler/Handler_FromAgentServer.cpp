#include "stdafx.h"
#include "Handler_FromAgentServer.h"

#include <ResultCode.h>
#include <SCItemSlot.h>

#include <PacketStruct_AD.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_WZ.h>

#include <NewCharItemInfoParser.h>
#include <ItemInfoParser.h>
#include <SunEventInfoParser.h>
// (f100713.2L) import a drop information to support a changed new char information table
#include <ItemTypeList.h>
#ifdef _USE_MAX_CHAR_LIST_PARSER
#include <MaxCharListParser.h>
#endif

#include "DBProxyServerEx.h"
#include "DBUser.h"
#include "DBUserManager.h"
#include "ProxyNodes/DBQueryProxyManager.h"
#include "Services/NewCharacterItemProvider.h" // f101006.4L

#include "QueryObjects/DBHandler.h"
#include "QueryObjects/AgentServerQuery.h"
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
// (WAVERIX) (090816) (NOTE) 원래 해더는 디파인 처리를 하지 않지만, 땜빵용 포함파일이라 걸었음.
// 정리한 뒤 제거할 필요 있음.
    #include "QueryObjects/GameServerQuery.h" //Query_Char_Exist
#endif
#include "Sessions/ChildServerSession.h"
#include "PacketHandler/Handler_FriendInvitation.h"
#include "PacketHandler/Handler_Guild.DbpToGuild.h"
#include "CharInfoParser.h"
#include <PacketStruct_PointSystem.h>
#include "Services/Guild/Guild.h"
#include "Services/Guild/GuildManager.h"

#if 0
ChildServerSession* pServerSession; // argument definition argument, to support the intellisense
#endif


//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_FromAgentServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_FromAgentServer::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_FromAgentServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AD_CONNECTION, AD_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
        // NOTE: f110622.2L
        HANDLER_NODE_INFO(AD_CONNECTION, AD_CONNECTION_HE_IS_HERE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_SYN, PHASE_UNLIMITED),
        //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_CHAR_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_INVEN_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_PETLIST_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_EQUIP_ITEMSLOTINFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_CREATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_CREATE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_DESTROY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_DESTROY_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_RECOVER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_RECOVER_DBR, PHASE_UNLIMITED),

    #ifdef __NA_001044_ADD_CUSTOM_UI
        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_DBR, PHASE_UNLIMITED),
    #endif
        HANDLER_NODE_INFO(AD_CONNECTION, AD_CONNECTION_DISCONNECT_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_ADDIN_NTF, PHASE_UNLIMITED),

    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_QUERY_CHARINFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(SERVERCOMMON, SERVERCOMMON_QUERY_CHARINFO_DBR, PHASE_UNLIMITED),
    #endif
    #ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARINFO_PCBANG_BILLING_NTF, PHASE_UNLIMITED),
    #endif
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        HANDLER_NODE_INFO_DBR(AD_EVENT, AD_EVENT_INVITATIONRELATION_INSERT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_DBR, PHASE_UNLIMITED),
        //}
    #ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        HANDLER_NODE_INFO(AD_CHARINFO, AD_CHARNAME_CHANGE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_CHARINFO, AD_CHARNAME_CHANGE_DBR, PHASE_UNLIMITED),
    #endif
    #ifdef _NA_0_20110329_GAMEOPTION_SETTING
        HANDLER_NODE_INFO(AD_ETC, AD_ETC_GAMEOPTION_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_ETC, AD_ETC_GAMEOPTION_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(AD_ETC, AD_ETC_GAMEOPTION_UPDATE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_ETC, AD_ETC_GAMEOPTION_UPDATE_DBR, PHASE_UNLIMITED),
    #endif
    #ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO(AD_ETC, AD_ETC_GUILD_RANKINGINFO_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(AD_ETC, AD_ETC_GUILD_RANKINGINFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(AD_ETC, AD_ETC_GUILDINFO_SYN, PHASE_UNLIMITED),
    #endif
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================

Handler_FromAgentServer_IMPL(AD_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
};

//==================================================================================================

// NOTE: f110622.2L, when an user return to the character selection zone,
// the user may rarely miss the activated updating server in case the user that was leaving
// on a field of a game server. this protocol would notify the database proxy server
// in order to refresh the zombie user detection timeout.
Handler_FromAgentServer_IMPL(AD_CONNECTION_HE_IS_HERE_CMD)
{
    __UNUSED((pServerSession, wSize));
    const MSG_AD_CONNECTION_HE_IS_HERE_CMD* recv_msg = \
        static_cast<MSG_AD_CONNECTION_HE_IS_HERE_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_key, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|can' find DBUser|UserGuid=%d|"), user_key);
        return;
    };
    db_user->UpdateLatestUpdateTick();
};

//==================================================================================================

Handler_FromAgentServer_IMPL(AD_CONNECTION_DISCONNECT_CMD)
{
    __TOUCHED((pServerSession, wSize));
    MSG_AD_CONNECTION_DISCONNECT_CMD* recv_msg = (MSG_AD_CONNECTION_DISCONNECT_CMD *)pMsg;
    const DWORD user_key = recv_msg->m_dwKey;
    // CHANGES: f101215.2L, it's possible that occur hang
    // when an user is disconnected on no linked server.
    // condition = { NATION_BUILD_VERSION >= 1004 }
    // NOTE: f101216.1L, this flow is the one-way transaction by authentication server control.
    // however, it's possible that a game server precede the agent server on client lazy disconnect.
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_key, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|can' find DBUser|UserGuid=%d|"), user_key);
        return;
    }

    if (recv_msg->m_GuildGuid != 0)
    {
        MSG_DZ_GUILD_LOGOUT_CHAR_CMD msg_cmd;
        msg_cmd.m_GuildGuid = recv_msg->m_GuildGuid;
        msg_cmd.m_CharGuid = recv_msg->m_CharGuid;
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
        Handler_Guild_DbpToGuild::OnDZ_GUILD_LOGOUT_CHAR_CMD(
            g_DBHandler.GetSharedSession(), &msg_cmd, sizeof(msg_cmd));
    #else
        g_DBProxyServer.SendToSessionType(GUILD_SERVER, &msg_cmd, sizeof(msg_cmd));
    #endif
    }
    if (find_result == db_query_proxy_manager->eFindResult_Service)
    {
        if (db_user->GetChannelID() == recv_msg->m_ChannelID &&
            db_user->GetServerSession() == pServerSession)
        {
            if (db_user->GetChannelTransactionLevel() == 1) {
                db_user->LeaveChannelTransaction();
            };
            db_user->Disconnect(recv_msg->m_ChannelID);
        };
    }
    else {
    };
}

//==================================================================================================

Handler_FromAgentServer_IMPL(AD_CHARINFO_CHARLISTREQ_SYN)
{
    __TOUCHED((pServerSession, wSize));
    using nsQuery::DBQueryProxyManager;
    const MSG_AD_CHARINFO_CHARLISTREQ_SYN* recv_msg = \
        static_cast<MSG_AD_CHARINFO_CHARLISTREQ_SYN*>(pMsg);
    //
    const DWORD user_guid = recv_msg->m_dwKey;
    assert(static_cast<USERGUID>(user_guid) == recv_msg->m_UserGuid);

    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();

    RC::eCHAR_RESULT error_code = RC::RC_CHAR_SUCCESS;
    bool from_pendings = false; // if the db_user is not null that where is db_user from
    bool total_loaded = false;
    DBUser* db_user = NULL;
    const DBQueryProxyManager::eConnectResult connect_result = \
        db_query_proxy_manager->EnterConnectPendingProcess(&db_user, recv_msg, pServerSession);
    if (connect_result == db_query_proxy_manager->eConnectResult_Error)
    {
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = error_code;
        pServerSession->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        // CHANGES: f101202.2L, added a missing logic closer
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't receive character list, ")
               _T("UserGuid=%u, Result=%u, location=#%d|"),
               __FUNCTION__, user_guid, error_code, 1);
        return;
    }
    else if (connect_result == db_query_proxy_manager->eConnectResult_Create) {
    }
    else if (connect_result == db_query_proxy_manager->eConnectResult_Reuse) {
        assert(db_user != NULL); //FindUser(key) != NULL
    }
    else if (connect_result == db_query_proxy_manager->eConnectResult_PendingReuse) {
        from_pendings = true;
    }
    else if (connect_result == db_query_proxy_manager->eConnectResult_Pending) {
        return;
    };
    // CHANGES: f110613.2L
    int location_index = 0;
    //
    if (db_user != NULL)
    {
        nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node();
        total_loaded = user_node->loaded();
    }
    // remove connection state control, using db user cashing
    //
    if (db_user && total_loaded)
    {   // server select facility, etc...
        const nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node();
        __UNUSED(user_node); // for debugging
        //
        db_user->SetServerSession(pServerSession);
        const int number_of_characters = db_user->GetNumberOfChars();
        if (number_of_characters > MAX_CHARACTER_LIST_NUM)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|Msg=the number of characters out of range|")
                   _T("|UserGuid=%d|Number of characters=%d|"),
                   __FUNCTION__, user_guid, number_of_characters);
            error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
            location_index = 100 + 10;
            goto ERROR_HANDLING;
        }

        db_user->SetDBState(eDB_STATE_CREATED);
        db_user->SetIsPCRoom(recv_msg->m_bIsPCRoom);
        db_user->SetChannelID(recv_msg->m_ChannelID);
    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        db_user->SetLoginTime(recv_msg->m_LoginTime);
        db_user->SetFatigueUser(recv_msg->m_bFatigueUser);
    #endif
        if (from_pendings) {
            g_DBUserManager.InsertFromPendings(db_user);
        }

        if (db_query_proxy_manager->SendUserInfoToAgentOnAllCharacterLoaded(db_user) == false)
        {
            error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
            location_index = 100 + 20;
            goto ERROR_HANDLING;
        }
        db_user->EnterChannelTransaction(); // NOTE: f110426.2L
    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        g_DBProxyServer.ExecuteQuery_MatchFatigue_Select(user_guid, user_guid);
    #endif
    }
    else
    {   //create new db user
        if (db_user == NULL) {
            db_user = g_DBUserManager.CreateUser(user_guid);
        }
        else if(from_pendings) {
            assert(db_user != NULL);
            g_DBUserManager.InsertFromPendings(db_user);
        }
        const nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node();

        // 유저 초기화 : 상태 eDB_STATE_CREATING
        db_user->SetUserKey(user_guid);
        db_user->SetUserGuid(user_guid);
        db_user->SetDBState(eDB_STATE_CREATING);
        db_user->SetIsPCRoom(recv_msg->m_bIsPCRoom);
        // 서버 세션 초기화
        db_user->SetServerSession(pServerSession);
        db_user->SetChannelID(recv_msg->m_ChannelID);
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        db_user->SetGameChuUSN(recv_msg->gamechu_usn_);
    #endif

    #ifdef _NA_20100307_BETAKEY_SYSTEM
        db_user->SetBetakey(recv_msg->betakey_);
    #endif

    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        db_user->SetLoginTime(recv_msg->m_LoginTime);
        db_user->SetFatigueUser(recv_msg->m_bFatigueUser);
    #endif
        // ... 
        if (user_node->loaded() == false)
        {
            nsQuery::DBQueryProxyManager::eRequestResult req_result;
            // (f100729.3L)
            req_result = db_query_proxy_manager->Request_User_Select_All(db_user);
            if (req_result == db_query_proxy_manager->eReqResult_Failed)
            {
                error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
                location_index = 100 + 30;
                goto ERROR_HANDLING;
            }
        }
        // refresh loaded() state
        if (user_node->loaded())
        {
            if (db_query_proxy_manager->SendUserInfoToAgentOnAllCharacterLoaded(db_user) == false)
            {
                error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
                location_index = 100 + 40;
                goto ERROR_HANDLING;
            }
        }
    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        g_DBProxyServer.ExecuteQuery_MatchFatigue_Select(user_guid, user_guid);
    #endif
        db_user->EnterChannelTransaction(); // NOTE: f110426.2L
    };
    //
    // success
    assert(error_code == RC::RC_CHAR_SUCCESS);
    return;

    // failed process
ERROR_HANDLING:
    {
        assert(error_code != RC::RC_CHAR_SUCCESS);
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = error_code;
        pServerSession->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't receive character list, ")
               _T("UserGuid=%u, Result=%u, location=#%d|"),
               __FUNCTION__, user_guid, error_code, location_index);
    };
}

Handler_FromAgentServer_IMPL(AD_CHARINFO_CHARLISTREQ_INTER_CHAR_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_User_Select* query_result = reinterpret_cast<Query_User_Select*>(dbproxy_result->m_pData);
    //
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    const DWORD user_guid = query_result->GetUserKey();

    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL)
    {
        db_query_proxy_manager->DBResult_User_Select(user_guid, *query_result, \
            nsQuery::DBQueryProxyUserNode::eTrans_Select_CharInfo);
        SUNLOG(eFULL_LOG,
               _T("["__FUNCTION__"]|Msg=leave the user after query transaction|UserGuid=%d|"),
               user_guid);
        Query_User_Select::FREE(query_result);
        return;
    }
    //
    int location_index = 0;
    RC::eCHAR_RESULT error_code = RC::RC_CHAR_SUCCESS;
    const int number_of_records = query_result->GetResultRowNum();
    for (int index = 0; index < number_of_records; ++index)
    {
        Query_User_Select::sQUERY_RESULT* result_of_charinfo = \
            &query_result->pResult[index];
        result_of_charinfo->SetSafeBound(); // f101007.4L
        DBCharacter* db_character = db_user->OnAddChar(result_of_charinfo);

        if (!FlowControl::FCAssert(db_character))
        {
            db_query_proxy_manager->DBResult_User_Select(user_guid, *query_result, \
                nsQuery::DBQueryProxyUserNode::eTrans_Select_CharInfo);
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|Msg=Critical, can't alloc db character|")
                   _T("|UserGuid=%d|Record Index=%d|CharGuid=%d|"),
                   __FUNCTION__, user_guid, index, result_of_charinfo->m_CharGuid);
            error_code = RC::RC_CHAR_FAILED;
            location_index = 100 + 10;
            goto ERROR_HANDLING;
        }
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        db_character->SetReNameStatus(result_of_charinfo->m_RenameStatus);
#endif
    }

    const int number_of_characters = db_user->GetNumberOfChars();
    if (!FlowControl::FCAssert(number_of_characters <= MAX_CHARACTER_LIST_NUM))
    {
        db_query_proxy_manager->DBResult_User_Select(user_guid, *query_result, \
            nsQuery::DBQueryProxyUserNode::eTrans_Select_CharInfo);
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=the number of characters out of range|")
               _T("|UserGuid=%d|Number of characters=%d|"),
               __FUNCTION__, user_guid, number_of_characters);
        error_code = RC::RC_CHAR_FAILED;
        location_index = 100 + 20;
        goto ERROR_HANDLING;
    }

#ifdef _NA_002050_20110216_ADD_GENDER
    db_user->SetCharSlotSize(query_result->GetCharSlotSize());
#endif

    //TODO(WAVERIX) need pending SetDBState
    db_user->SetDBState(eDB_STATE_CREATED);

    db_query_proxy_manager->DBResult_User_Select(*query_result, db_user);
    //
    // (f100729.3L) change to a batch request routine
    db_query_proxy_manager->Request_User_Select_All(db_user);

    Query_User_Select::FREE(query_result);
    // success
    assert(error_code == RC::RC_CHAR_SUCCESS);
    return;

ERROR_HANDLING:
    {
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = error_code;
        if (pServerSession) { // (NOTE) ... unsafe ...
            pServerSession->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        }

        Query_User_Select::FREE(query_result);
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't receive character list, ")
               _T("UserGuid=%u, Result=%u, location=#%d|"),
               __FUNCTION__, user_guid, error_code, location_index);
    }
}

Handler_FromAgentServer_IMPL(AD_CHARINFO_CHARLISTREQ_INTER_INVEN_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_User_Select_Inventory* query_result = \
        reinterpret_cast<Query_User_Select_Inventory*>(dbproxy_result->m_pData);
    //
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    const DWORD user_guid = query_result->GetUserKey();

    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL)
    {
        db_query_proxy_manager->DBResult_User_Select(user_guid, *query_result, \
            nsQuery::DBQueryProxyUserNode::eTrans_Select_InvenInfo);
        SUNLOG(eFULL_LOG,
               _T("["__FUNCTION__"]|Msg=leave the user after query transaction|UserGuid=%d|"),
               user_guid);
        Query_User_Select_Inventory::FREE(query_result);
        return;
    }
    int location_index = 0;
    RC::eCHAR_RESULT error_code = RC::RC_CHAR_SUCCESS;
    const int number_of_characters = db_user->GetNumberOfChars();
    const int number_of_records = query_result->GetResultRowNum();
    if (number_of_characters != number_of_records)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=the number of characters mismatch the number of results|")
               _T("|UserGuid=%d|Number of characters=%d|Number of records=%d"),
               __FUNCTION__, user_guid, number_of_characters, number_of_records);
        error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
        location_index = 100 + 10;
        goto ERROR_HANDLING;
    }

    using nsQuery::DBQueryProxyNode;
    if (db_query_proxy_manager->DBResult_User_Select(*query_result, db_user) == false)
    {
        error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
        location_index = 100 + 20;
        goto ERROR_HANDLING;
    }

    Query_User_Select_Inventory::FREE(query_result); query_result = NULL;
    // success
    assert(error_code == RC::RC_CHAR_SUCCESS);
    return;

ERROR_HANDLING:
    {
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = error_code;
        if (pServerSession) { // (NOTE) ... unsafe ...
            pServerSession->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        }
        Query_User_Select_Inventory::FREE(query_result); query_result = NULL;
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't receive character list, ")
               _T("UserGuid=%u, Result=%u, location=#%d|"),
               __FUNCTION__, user_guid, error_code, location_index);
    }
}

Handler_FromAgentServer_IMPL(AD_CHARINFO_CHARLISTREQ_INTER_PETLIST_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_User_Select_PetList* query_result = \
        reinterpret_cast<Query_User_Select_PetList*>(dbproxy_result->m_pData);
    //
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    const DWORD user_guid = query_result->GetUserKey();

    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user == NULL)
    {
        db_query_proxy_manager->DBResult_User_Select(user_guid, *query_result, \
            nsQuery::DBQueryProxyUserNode::eTrans_Select_PetList);
        SUNLOG(eFULL_LOG,
               _T("["__FUNCTION__"]|Msg=leave the user after query transaction|UserGuid=%d|"),
               user_guid);
        Query_User_Select_PetList::FREE(query_result);
        return;
    }

    if (db_query_proxy_manager->DBResult_User_Select(*query_result, db_user))
    {
        // success
        Query_User_Select_PetList::FREE(query_result);
        return;
    }
//ERROR_HANDLING:
    {
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = RC::RC_CHAR_FAILED;
        if (pServerSession) { // (NOTE) ... unsafe ...
            pServerSession->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        }
        Query_User_Select_PetList::FREE(query_result); query_result = NULL;
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=can't receive character list, UserGuid=%u, Result=%u|"),
               __FUNCTION__, user_guid, RC::RC_CHAR_FAILED);
    }
}

Handler_FromAgentServer_IMPL(AD_CHARINFO_CHARLISTREQ_INTER_EQUIP_ITEMSLOTINFO_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    RC::eCHAR_RESULT error_code = RC::RC_CHAR_SUCCESS;
    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    DBCharacter* db_character = NULL;
    bool success = true;
    if (db_user != NULL) {
        db_character = db_user->FindCharacter(char_guid);
    };
    int location_index = 0;
    if (db_user == NULL || db_character == NULL)
    {
        nsQuery::DBQueryProxyManager::eRequestResult req_result = \
            db_query_proxy_manager->DBResult_Char_Transaction(user_guid, char_guid, *query_result);
        const TCHAR* msg = _T("Msg=can't find user");
        if (req_result != db_query_proxy_manager->eReqResult_Failed) {
            msg = _T("Msg=leave the user after query transaction");
        }
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|%s|UserGuid=%d|"), msg, user_guid);
        location_index = 100 + 10;
        goto RELEASE_RESOURCE;
    }
    assert(error_code == RC::RC_CHAR_SUCCESS);

    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        location_index = 100 + 20;
        goto RELEASE_RESOURCE;
    };
    if (const nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node())
    {
        // may execute this routine if this query sequence is last.
        if (user_node->loaded()) {
            db_query_proxy_manager->SendUserInfoToAgentOnAllCharacterLoaded(db_user);
        }
    }
    // success
    assert(error_code == RC::RC_CHAR_SUCCESS);

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_ItemSlotInfo_Select: //S_ItemSlotInfo_Select
        Query_ItemSlotInfo_Select::FREE(static_cast<Query_ItemSlotInfo_Select*>(query_result));
        break;
    default:
        ASSERT(!"unexpected query");
        break;
    }
    //
    if (success == false)
    {
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = RC::RC_CHAR_FAILED;
        if (pServerSession) { // (NOTE) ... unsafe ...
            pServerSession->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        }
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't receive character list, ")
               _T("UserGuid=%u, Result=%u, location=#%d|"),
               __FUNCTION__, user_guid, RC::RC_CHAR_FAILED, location_index);
    }
}

Handler_FromAgentServer_IMPL(AD_CHARINFO_STATE_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    __TOUCHED((pMsg));
    ASSERT(!"don't support anymore");
    return;
}

//==================================================================================================

Handler_FromAgentServer_IMPL( AD_CHARINFO_STATE_UPDATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
    __TOUCHED((pMsg));
    ASSERT(!"don't support anymore");
    return;
}

// (CHANGES)
// @history
//  - 10/07/14 - code arrangement
Handler_FromAgentServer_IMPL(AD_CHARINFO_CREATE_SYN)
{
    __TOUCHED((pServerSession, wSize));
    using nsQuery::DBQueryProxyManager; // 1004
    //
    const MSG_AD_CHARINFO_CREATE_SYN* recv_msg = static_cast<MSG_AD_CHARINFO_CREATE_SYN*>(pMsg);
    //
    const DWORD user_key = recv_msg->m_dwKey;
    //
    MSG_AD_CHARINFO_CREATE_NAK msg_nak;
    ;{
        msg_nak.m_dwKey = user_key;
        msg_nak.m_dwErrorCode = RC::RC_CHAR_SUCCESS;
    };

    DBUser* const db_user = g_DBUserManager.FindUser(user_key);
    if (db_user == NULL)
    {
        msg_nak.m_dwErrorCode = RC::RC_CHAR_CREATE_DBUSER_NOT_EXIST;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    }
    // CHANGES: f101218.1L, fixed a missing filter that despite to maintain single transaction,
    // it will be caused a duplicated transaction. (version >= 1004)
    DBQueryProxyManager* const db_query_proxy_manager = DBQueryProxyManager::Instance();
    if (nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node())
    {
        ulong user_transaction = user_node->transaction();
        if (user_transaction & user_node->eTrans_Create_Character) {
            msg_nak.m_dwErrorCode = RC::RC_CHAR_CREATE_ALREADY_DOING_TRANSACTION;
            pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
            return;
        }
    };
    // (CHANGES) (f100714.1L) fix up a buffer overrun status \
    // which a charname data field has a MAX_CHARNAME_LENGTH value, not a ID_MAX_LEN
    //recv_msg->m_tszUserID[ID_MAX_LEN]='\0';
    //recv_msg->m_tszCharName[ID_MAX_LEN]='\0';
    NewCharItemInfoParser* const new_char_info_parser = NewCharItemInfoParser::Instance();
    // #1. first step. find the information related to new character information by a class
    const NEW_CHAR_ITEM_INFO* new_char_info = \
        new_char_info_parser->FindItemInfo(recv_msg->m_byClass);
    if (new_char_info == NULL)
    {
        msg_nak.m_dwErrorCode = RC::RC_CHAR_CREATE_DBUSER_NOT_EXIST;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    }
    // #2. make a query string
    TCHAR user_id[_countof(recv_msg->m_tszUserID)];
    TCHAR char_name[_countof(recv_msg->m_tszCharName)];
    _tcsncpy(user_id, recv_msg->m_tszUserID, _countof(user_id));
    user_id[_countof(user_id) - 1] = _T('\0');
    _tcsncpy(char_name, recv_msg->m_tszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = _T('\0');
    //
    STRING_SAFER_QUERY256 query_buffer;
  #ifdef _NA_002050_20110216_ADD_GENDER
    query_buffer.MakeString(
        _T("{?=call S_CharCreateByMaxLimit(%d,'%s','%s', %u, %u, %u, %u, %d, %u, %u, %u)}"),
        recv_msg->m_UserGuid, 
        user_id, 
        char_name,
        recv_msg->m_byClass, 
        recv_msg->m_byHeight,
        recv_msg->m_byFace, 
        recv_msg->m_byHair,
        new_char_info->m_wStyle, 
        recv_msg->gender, 
    #ifdef _USE_MAX_CHAR_LIST_PARSER
        MaxCharListParser::Instance()->GetDefaultSlotSize(), 
        MaxCharListParser::Instance()->GetMaxSlotSize());
    #else
        MaxCharListConfig::kDefaultSlotSize, 
        MaxCharListConfig::kMaxSlotSize);
    #endif
  #else
    // CHANGES: f101006.3L should be provide default items in the inventory for a creating character.
    // provide items are processed lately on 'ENTERVILLAGEREQ_SYN'
    query_buffer.MakeString(_T("{?=call S_Char_Create(")
                            _T("%d,'%s','%s', %u, %u, %u, %u, %d)}"),
                            recv_msg->m_UserGuid, user_id, char_name,
                            recv_msg->m_byClass, recv_msg->m_byHeight,
                            recv_msg->m_byFace, recv_msg->m_byHair,
                            new_char_info->m_wStyle);
  #endif // _NA_002050_20110216_ADD_GENDER

#if defined(_DEBUG) && defined(__CN_0_20090706_CHINA_VISTA_CHARNAME)
    for (int i = 0; i < MAX_CHARNAME_LENGTH; ++i) {
        SUNLOG(eCRITICAL_LOG, "DB Query value  : CharName[%d] = %X", i, recv_msg->m_tszCharName[i]);
    }
    SUNLOG(eCRITICAL_LOG, (TCHAR*)query_buffer);
#endif
    // #3. instance a query object for a create query
    Query_Char_Create* query = Query_Char_Create::ALLOC(); // (NOTE) this query object is cleared
    query->SetQuery(query_buffer);
    query->SetUserKey(user_key);
    //
    // CHANGES: f101006.4L, renewals. split code section about the creating character item provide
    // process to support multi-version control.
    const nsSlot::NewCharacterItemProvider* new_character_item_provider = \
        nsSlot::NewCharacterItemProvider::Instance();
    const bool valid_items = \
        new_character_item_provider->IsValidState(static_cast<eCHAR_TYPE>(recv_msg->m_byClass));
    if (valid_items == false)
    {
        msg_nak.m_dwErrorCode = RC::RC_CHAR_CREATE_DBUSER_NOT_EXIST;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));

        Query_Char_Create::FREE(query);
        return;
    }

    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    // CHANGES: f101218.1L, fixed the unexpected transaction state related to 'character create'
    DBQueryProxyManager::eRequestResult request_result = \
        db_query_proxy_manager->Request_Char_Create(*query, db_user);
    if (request_result != db_query_proxy_manager->eReqResult_Success)
    {
        msg_nak.m_dwErrorCode = RC::RC_CHAR_CREATE_TRANSACTION_ERROR;
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=occured unexpected status(%d)|UserGuid=%u|"),
               __FUNCTION__, request_result, user_key);
        return;
    }
    db_user->DBQuery(AD_CHARINFO, AD_CHARINFO_CREATE_DBR, query);

    return;
}

Handler_FromAgentServer_IMPL( AD_CHARINFO_CREATE_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult = (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_Create * pQResult = (Query_Char_Create *)pResult->m_pData;

    DWORD UserKey	= pQResult->GetUserKey();
	BYTE ResultNum	= (BYTE)pQResult->GetResultRowNum();
	static SERVER_CHARACTER_PART rServerCharPart;
	MSG_AD_CHARINFO_CREATE_ACK msgACK;

    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    // f101218.1L, NATION_BUILD_VERSION >= 1004
    DBUser* pUser = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(UserKey, &pUser);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, UserKey);
        goto lb_NakMsg; // must be 1st 'goto'
    };
    //
    RC::eCHAR_CREATE_RESULT ErrorCode = RC::RC_CHAR_CREATE_SUCCESS;

	if( pQResult->GetResult() != 0 )
	{
		// 캐릭터 생성 에러
		switch( pQResult->GetResult() )
		{
		case 1:
			ErrorCode = RC::RC_CHAR_CREATE_EXIST_SAME_NAME; break;
		case 2:
			ErrorCode = RC::RC_CHAR_CREATE_SLOT_FULL; break;
		case 3:
			ErrorCode = RC::RC_CHAR_CREATE_CANNOT_CREATE_TO_SERVICE; break;
		default:
			ErrorCode = RC::RC_CHAR_CREATE_TRANSACTION_ERROR; break;
		}
		goto lb_NakMsg;
	}

	if( ResultNum != 1 )
	{
		ErrorCode = RC::RC_CHAR_CREATE_QUERY_ERROR;
		goto lb_NakMsg;
	}

    Query_Char_Create::sQUERY_RESULT* const player_info = pQResult->pResult;
#if defined(_DEBUG) && defined(__CN_0_20090706_CHINA_VISTA_CHARNAME)
    ;{
        const char* char_it = player_info->result_info_char_name_;
        for (int i = 0; i < _countof(player_info->result_info_char_name_); ++i, ++char_it) {
            SUNLOG(eCRITICAL_LOG, _T("DB return value : CharName[%d] = %X"), i, *char_it);
        }

        SUNLOG(eCRITICAL_LOG, _T("[call S_Char_Create return] GUID[%d],CharName[%s]"), 
               player_info->m_UserGuid, player_info->result_info_char_name_);
    };
#endif

    // r101007.3L
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	// 매칭DB에 쿼리
	g_DBProxyServer.ExecuteQuery_MatchChar_Update( player_info->m_UserGuid, 
		player_info->result_info_char_name_, 1, player_info->m_byClass );
#endif
    // (NOTE) (WAVERIX) set initial values
    player_info->config_version = nsSlot::InventoryConfig::kConfigVersion;
	//printf( "Handler_FromAgentServer_IMPL( AD_CHARINFO_CREATE_DBR )\n");
	msgACK.m_dwKey = UserKey;
    // CHAGNES: f101207.3L, the rule for the 100 level change up change into a default value basically
    // reference: eCHAR_TYPE and eCHAREX_TYPE
    BOOST_STATIC_ASSERT(eCHAR_BERSERKER + eCHAREX_100LEVEL_CHANGEUP_CONSTANT ==
                        eCHAREX_BERSERKER_100LEVEL);
    player_info->m_ChangeOfClassStep = player_info->m_byClass + eCHAREX_100LEVEL_CHANGEUP_CONSTANT;
    
    // 시작 지점을 스크립트에서 읽은값으로 대체한다.
    const CharInfoParser* const char_info_parser = CharInfoParser::Instance();
    const INIT_CHARINFO* const init_char_info = char_info_parser->GetInitCharInfo(player_info->m_byClass);
    if (init_char_info != NULL)
    {
        player_info->m_iRegion = init_char_info->m_dwRegion;
        player_info->m_sX = init_char_info->m_sLocationX;
        player_info->m_sY = init_char_info->m_sLocationY;
        player_info->m_sZ = init_char_info->m_sLocationZ;
        player_info->m_iPreRegion = init_char_info->m_dwRegion;
    }

	DBCharacter* pCharacter = pUser->OnAddChar(player_info);
	if( !pCharacter )
	{
		ErrorCode = RC::RC_CHAR_CREATE_DBCHAR_ALREADY_CREATED;
		goto lb_NakMsg;
	}
    pCharacter->OnSetInfoInventory(player_info);
    db_query_proxy_manager->DBResult_Char_Create(*pQResult, pUser, RC::RC_CHAR_CREATE_SUCCESS);

	// 캐릭터 생성시에 날아오는 캐릭터 정보는 비번정보를 주지 않는다. DB Query Update요청시 미처 몰랐던 부분 -.-;
	// 일단 임시로 다음과 같이 비번 초기화 작업을 수행하자. 추후 개선이 필요할 경우 요청하자.
	CHAR pTempPwd[1] = { 0, };
	pCharacter->SetInventoryPassword( pTempPwd );

	pCharacter->Serialize( rServerCharPart, SERIALIZE_LOAD );
	pServerSession->SendPacket( &msgACK, msgACK.ConvertForSend( rServerCharPart ) );
	
    // NOTE: 여기서 친구초대 관계를 추가 한다.
	//  캐릭터 생성과 친구 초대 관계 설정을 트렌젝션을 묶을수 없으므로(DB 사정상)
	//  캐릭터 생성이 정상인데 친구 초대 관계가 성립이 안되면 로그 처리 해야 것다.
    DFriendInvitationController::Instance()->OnCharacterCreateDBR(pServerSession, pMsg, wSize);    
    // (CHANGES) (f100524.3L) add an update routine to maintain integrity
    pCharacter->UpdateInventory();

    Query_Char_Create::FREE( pQResult ); pQResult = NULL;

	return;

lb_NakMsg:
	switch( ErrorCode )
	{
	//case RC::RC_CHAR_CREATE_EXIST_SAME_NAME:
	//	break;
	case RC::RC_CHAR_CREATE_SLOT_FULL:
		SUNLOG( eCRITICAL_LOG, "[AD_CHARINFO_CREATE_DBR][U:%u][R:%u]RC_CHAR_CREATE_SLOT_FULL(%s)", 
						UserKey, pQResult->GetResult(), pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_CREATE_CANNOT_CREATE_TO_SERVICE:
		SUNLOG( eCRITICAL_LOG, "[AD_CHARINFO_CREATE_DBR][U:%u][R:%u]RC_CHAR_CREATE_CANNOT_CREATE_TO_SERVICE(%s)", 
						UserKey, pQResult->GetResult(), pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_CREATE_TRANSACTION_ERROR:
		SUNLOG( eCRITICAL_LOG, "[AD_CHARINFO_CREATE_DBR][U:%u][R:%u]RC_CHAR_CREATE_TRANSACTION_ERROR(%s)", 
						UserKey, pQResult->GetResult(), pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_CREATE_QUERY_ERROR:
		SUNLOG( eCRITICAL_LOG, "[AD_CHARINFO_CREATE_DBR][U:%u][R:%u]RC_CHAR_CREATE_QUERY_ERROR(%s)", 
						UserKey, pQResult->GetResult(), pQResult->GetQuery() );
		break;
    default:
        SUNLOG( eCRITICAL_LOG, "[AD_CHARINFO_CREATE_DBR][U:%u] RC_CHAR_CREATE_ERROR(%d)", 
            UserKey, ErrorCode);
        break;
	}
	MSG_AD_CHARINFO_CREATE_NAK nmsg;
	nmsg.m_dwKey		= UserKey;
	nmsg.m_dwErrorCode	= ErrorCode;
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
    // NOTE: version >= 1004
    assert(db_query_proxy_manager);
    if (pUser) {
        // release transaction lock
        db_query_proxy_manager->DBResult_Char_Create(*pQResult, pUser, ErrorCode);
    }

	Query_Char_Create::FREE( pQResult ); pQResult = NULL;
}

Handler_FromAgentServer_IMPL( AD_CHARINFO_DESTROY_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_AD_CHARINFO_DESTROY_SYN* pRcvMsg = (MSG_AD_CHARINFO_DESTROY_SYN *)pMsg;

	const DWORD dwUserKey = pRcvMsg->m_dwKey;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		DBUser* pUser = g_DBUserManager.FindUser( dwUserKey );
		IfFailThenBreak(pUser != 0, RC::RC_CHAR_DESTROY_DBUSER_DONOT_EXIST);
        // CHANGES: f110825.1L, prevent the unexpected packet flow
        // TODO: f110825.1L, potential critical issue,
        // it has not transaction between this request with the received result process.
        IfFailThenBreak(pUser->GetConnectedChar() == NULL,
            RC::RC_CHAR_DESTROY_ALREADY_DOING_TRANSACTION);
        //
		DBCharacter* pCharacter = pUser->FindCharacter( pRcvMsg->m_CharGuid );
		IfFailThenBreak(pCharacter && pUser->GetNumberOfChars(),
                        RC::RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST);

		STRING_SAFER_QUERY64 szQueryBuff;
		szQueryBuff.MakeString( "{?=call S_Char_Destroy(%d,%d)}", pUser->GetUserGuid(), pRcvMsg->m_CharGuid );

		Query_Char_Destroy* pQuery = Query_Char_Destroy::ALLOC();
		//pQuery->SetProtocol( AD_CHARINFO, AD_CHARINFO_DESTROY_DBR );
		//pQuery->SetVoidObject( pServerSession );
		pQuery->SetCharName( pCharacter->GetCharName() );
		pQuery->SetQuery( szQueryBuff );
		pQuery->SetUserKey( dwUserKey );
		pQuery->SetCharGuid( pRcvMsg->m_CharGuid );
		pUser->DBQuery( AD_CHARINFO, AD_CHARINFO_DESTROY_DBR, pQuery );

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	switch( flow.GetErrorCode() )
	{
	case RC::RC_CHAR_DESTROY_DBUSER_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_SYN][U:%u]RC_CHAR_DESTROY_DBUSER_DONOT_EXIST", dwUserKey );
		break;
	case RC::RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_SYN][U:%u]RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST", dwUserKey );
		break;
	}

	MSG_AD_CHARINFO_DESTROY_NAK msgNAK;
	msgNAK.m_dwKey			= dwUserKey;
	msgNAK.m_dwErrorCode	= flow.GetErrorCode();
	pServerSession->SendPacket( &msgNAK, sizeof(msgNAK) );
}

Handler_FromAgentServer_IMPL( AD_CHARINFO_DESTROY_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_Destroy * pQResult	= (Query_Char_Destroy *)pResult->m_pData;
	static MSG_AD_CHARINFO_DESTROY_ACK msg;

	DWORD UserKey		= pQResult->GetUserKey();
	DWORD ErrorCode		= 0;
	CHARGUID CharGuid	= pQResult->GetCharGuid();

	if( pQResult->GetResult() != 0 )
	{
		switch(pQResult->GetResult())
		{
		case 1:
			ErrorCode = RC::RC_CHAR_DESTROY_TRANSACTION_ERROR; break;
		case 2:
			ErrorCode = RC::RC_CHAR_DESTROY_GUILD_MASTER; break;
        case 3:
            ErrorCode = RC::RC_CHAR_DESTROY_GROUP_MEMBER; break; //[20090811][4540]그룹 맴버 존재시 삭제 불가 메세지 추가
		default:
			ErrorCode = RC::RC_CHAR_DESTROY_FAILED; break;
		}
		goto lb_NakMsg;
	}

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		ErrorCode = RC::RC_CHAR_DESTROY_DBUSER_DONOT_EXIST;
		goto lb_NakMsg;
	}


// 길드서버에 전송해서 해당 캐릭터를 삭제한다.
	DBCharacter* pCharacter = pUser->FindCharacter( pQResult->GetCharGuid() );
	// 여기에 DBR온다는 자체가, 길마가 아닌경우이고 40렙 이상인 경우에는 sts 값이 3으로 변경되어서 캐릭터 삭제 대기 상태에 들어간다.
	// 따라서 삭제 대기 상태인 놈은 게임에서 계속? 길드원으로 처리 해야 하는가?
	if( pCharacter && pCharacter->GetInfo()->m_GuildGuid != 0 && pCharacter->GetInfo()->m_LV < CHAR_DESTROY_LEVEL )
	{
		MSG_DZ_GUILD_WITHDRAW_ACK msgACK;
		msgACK.m_dwSessionIndex	= 0; //모든 게임 서버에 보내니까 Index필요 없다.
		msgACK.m_GuildGuid		= pCharacter->GetInfo()->m_GuildGuid;
		msgACK.m_CharGuid		= pCharacter->GetInfo()->m_CharGuid;
		msgACK.m_MemberPenaltyTime = 0;
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
        Handler_Guild_DbpToGuild::OnDZ_GUILD_WITHDRAW_ACK(
            g_DBHandler.GetSharedSession(), &msgACK, sizeof(msgACK));
    #else
		g_DBProxyServer.SendToSessionType( GUILD_SERVER, &msgACK, sizeof(msgACK) );
    #endif
	}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 길드 가입 요청 리스트에서 모두 삭제한다.
    g_GuildManager.InitailizeGuildJoinRequest(CharGuid);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	BOOL rt = pUser->OnDelChar( CharGuid );
	if( !rt )
	{
		ErrorCode = RC::RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST;
		goto lb_NakMsg;
	}

	msg.m_dwKey		= UserKey;
	msg.m_CharGuid	= CharGuid;
	pServerSession->SendPacket( &msg, sizeof(msg) );
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	// 매칭DB에 쿼리
	g_DBProxyServer.ExecuteQuery_MatchChar_Delete( pUser->GetUserGuid(), pQResult->GetCharName() );
#endif
	Query_Char_Destroy::FREE( pQResult ); pQResult = NULL;

	return;

lb_NakMsg:
	switch( ErrorCode )
	{
	case RC::RC_CHAR_DESTROY_DBUSER_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]RC_CHAR_DESTROY_DBUSER_DONOT_EXIST(%s)", UserKey, pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_DESTROY_TRANSACTION_ERROR:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]RC_CHAR_DESTROY_TRANSACTION_ERROR(%s)", UserKey, pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_DESTROY_GUILD_MASTER:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]RC_CHAR_DESTROY_GUILD_MASTER(%s)", UserKey, pQResult->GetQuery() );
		break;
    case RC::RC_CHAR_DESTROY_GROUP_MEMBER://[20090811][4540]그룹 맴버 존재시 삭제 불가 메세지 추가
        SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]RC_CHAR_DESTROY_GROUP_MEMBER(%s)", UserKey, pQResult->GetQuery() );
        break;
	case RC::RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST(%s)", UserKey, pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_DESTROY_FAILED:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]RC_CHAR_DESTROY_FAILED(%s)", UserKey, pQResult->GetQuery() );
		break;
	default:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_DESTROY_DBR][U:%u]Code:%u(%s)", UserKey, pQResult->GetResult(), pQResult->GetQuery() );
	}
	MSG_AD_CHARINFO_DESTROY_NAK nmsg;
	nmsg.m_dwKey		= UserKey;
	nmsg.m_dwErrorCode	= ErrorCode;
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

	Query_Char_Destroy::FREE( pQResult ); pQResult = NULL;
}



Handler_FromAgentServer_IMPL( AD_CHARINFO_RECOVER_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_AD_CHARINFO_RECOVER_SYN* pRcvMsg = (MSG_AD_CHARINFO_RECOVER_SYN *)pMsg;

	BOOL bProcess = TRUE;
	const DWORD dwUserKey = pRcvMsg->m_dwKey;
	DWORD dwErrorCode	= 0;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		DBUser* pUser = g_DBUserManager.FindUser( dwUserKey );
		IfFailThenBreak(pUser != 0, RC::RC_CHAR_RECOVER_DBUSER_DONOT_EXIST);

		DBCharacter* pCharacter = pUser->FindCharacter( pRcvMsg->m_CharGuid );
		// 삭제된 개수도 같이 체크
		IfFailThenBreak(pCharacter && pCharacter->IsDeleted() && pUser->GetNumberOfChars(),
                        RC::RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST);

		STRING_SAFER_QUERY64 szQueryBuff;
		szQueryBuff.MakeString( "{?=call S_Char_Recover(%d,%d)}", pUser->GetUserGuid(), pRcvMsg->m_CharGuid );

		Query_Char_Recover* pQuery = Query_Char_Recover::ALLOC();
		//pQuery->SetProtocol( AD_CHARINFO, AD_CHARINFO_DESTROY_DBR );
		//pQuery->SetVoidObject( pServerSession );
		pQuery->SetQuery( szQueryBuff );
		pQuery->SetUserKey( dwUserKey );
		pQuery->SetCharGuid( pRcvMsg->m_CharGuid );
		pUser->DBQuery( AD_CHARINFO, AD_CHARINFO_RECOVER_DBR, pQuery );

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	switch( flow.GetErrorCode() )
	{
	case RC::RC_CHAR_RECOVER_DBUSER_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_RECOVER_SYN][U:%u]RC_CHAR_RECOVER_DBUSER_DONOT_EXIST", dwUserKey );
		break;
	case RC::RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_RECOVER_SYN][U:%u]RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST", dwUserKey );
		break;
	}

	MSG_AD_CHARINFO_RECOVER_NAK nmsg;
	nmsg.m_dwKey		= dwUserKey;
	nmsg.m_dwErrorCode	= flow.GetErrorCode();
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
}

Handler_FromAgentServer_IMPL( AD_CHARINFO_RECOVER_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_Recover * pQResult	= (Query_Char_Recover *)pResult->m_pData;
	static MSG_AD_CHARINFO_RECOVER_ACK msg;

	DWORD UserKey		= pQResult->GetUserKey();
	DWORD ErrorCode		= 0;
	CHARGUID CharGuid	= pQResult->GetCharGuid();

	if( pQResult->GetResult() != 0 )
	{
		switch(pQResult->GetResult())
		{
		case 1:
			ErrorCode = RC::RC_CHAR_RECOVER_TRANSACTION_ERROR; break;
		}
		goto lb_NakMsg;
	}

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		ErrorCode = RC::RC_CHAR_RECOVER_DBUSER_DONOT_EXIST;
		goto lb_NakMsg;
	}

	DBCharacter * pRecoveredChar = pUser->OnRecoverChar( CharGuid );
	if( !pRecoveredChar )
	{
		ErrorCode = RC::RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST;
		goto lb_NakMsg;
	}

	msg.m_dwKey		= UserKey;
	msg.m_CharGuid	= CharGuid;
	pServerSession->SendPacket( &msg, sizeof(msg) );
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	// 매칭DB에 쿼리
	g_DBProxyServer.ExecuteQuery_MatchChar_Update( 
		pUser->GetUserGuid(), pRecoveredChar->GetCharName(), pRecoveredChar->GetLV(), pRecoveredChar->GetClassCode() );
#endif
	Query_Char_Recover::FREE( pQResult ); pQResult = NULL;

	return;

lb_NakMsg:
	switch( ErrorCode )
	{
	case RC::RC_CHAR_RECOVER_DBUSER_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_RECOVER_DBR][U:%u]RC_CHAR_RECOVER_DBUSER_DONOT_EXIST(%s)", UserKey, pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_RECOVER_TRANSACTION_ERROR:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_RECOVER_DBR][U:%u]RC_CHAR_RECOVER_TRANSACTION_ERROR(%s)", UserKey, pQResult->GetQuery() );
		break;
	case RC::RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_RECOVER_DBR][U:%u]RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST(%s)", UserKey, pQResult->GetQuery() );
		break;
	default:
		SUNLOG( eFULL_LOG, "[AD_CHARINFO_RECOVER_DBR][U:%u]Code:%u(%s)", UserKey, pQResult->GetResult(), pQResult->GetQuery() );
	}
	MSG_AD_CHARINFO_DESTROY_NAK nmsg;
	nmsg.m_dwKey		= UserKey;
	nmsg.m_dwErrorCode	= ErrorCode;
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );

	Query_Char_Recover::FREE( pQResult ); pQResult = NULL;
}

#ifdef __NA_001044_ADD_CUSTOM_UI
Handler_FromAgentServer_IMPL( AD_CHARINFO_CHECK_CHARACTERID_SYN )
{
    __TOUCHED((pServerSession, wSize));
	MSG_AD_CHARINFO_CHECK_CHARACTERID_SYN* pRcvMsg = (MSG_AD_CHARINFO_CHECK_CHARACTERID_SYN *)pMsg;

	DWORD UserKey = pRcvMsg->m_dwKey;
	DWORD ErrorCode	= 0;

	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		ErrorCode = RC::RC_CHAR_IDCHECK_DBUSER_DONOT_EXIST;
		goto lb_NakMsg;
	}

	TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_128, "{?=call S_Char_NameCheck(\'%s\')}", pRcvMsg->m_szCharName );
	szQueryBuff[MAX_QUERY_LENGTH_128]='\0';

	Query_Char_NameCheck * pQuery = Query_Char_NameCheck::ALLOC();
	pQuery->SetQuery( szQueryBuff );
	pQuery->SetUserKey( UserKey );
	pQuery->SetSearchID( pRcvMsg->m_szCharName );
	pUser->DBQuery( AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_DBR, pQuery );
	return;

lb_NakMsg:
	SUNLOG( eFULL_LOG, 
			"[AD_CHARINFO_CHECK_CHARACTERID_SYN][U:%u]RC_CHAR_IDCHECK_DBUSER_DONOT_EXIST", 
			UserKey );

	MSG_AD_CHARINFO_CHECK_CHARACTERID_NAK nmsg;
	nmsg.m_dwKey		= UserKey;
	nmsg.m_dwErrorCode	= ErrorCode;
	pServerSession->SendPacket( &nmsg, sizeof(nmsg) );
	return;
}

Handler_FromAgentServer_IMPL( AD_CHARINFO_CHECK_CHARACTERID_DBR )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_NameCheck* pQResult	= (Query_Char_NameCheck *)pResult->m_pData;

	DWORD UserKey		= pQResult->GetUserKey();
	DWORD ErrorCode		= 0;

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		ErrorCode = RC::RC_CHAR_IDCHECK_DBUSER_DONOT_EXIST;
		goto lb_NakMsg;
	}

	switch( pQResult->GetResult() )
	{
	case 0:
		{
			MSG_AD_CHARINFO_CHECK_CHARACTERID_ACK msg;
			msg.m_dwKey = UserKey;
			pServerSession->SendPacket( &msg, sizeof( msg));
			Query_Char_NameCheck::FREE( pQResult );
			pQResult = NULL;
			return;
		} break;
	case 1:
		//{
		//	ErrorCode = RC::RC_CHAR_IDCHECK_DBUSER_DONOT_EXIST;
		//	goto lb_NakMsg;
		//}break;
	default:
		{
			ErrorCode = RC::RC_CHAR_IDCHECK_FAILED;
			goto lb_NakMsg;
		} break;
	}

lb_NakMsg:
	SUNLOG( eFULL_LOG, 
			"[AD_CHARINFO_CHECK_CHARACTERID_DBR][U:%u]Code:%u(%s)", 
			UserKey, 
			pQResult->GetResult(), 
			pQResult->GetQuery() );

	MSG_AD_CHARINFO_CHECK_CHARACTERID_NAK nmsg;
	nmsg.m_dwKey		= UserKey;
	nmsg.m_dwErrorCode	= ErrorCode;
	pServerSession->SendPacket( &nmsg, sizeof( nmsg));

	Query_Char_NameCheck::FREE( pQResult );
	pQResult = NULL;
	return;
}
#endif

Handler_FromAgentServer_IMPL( AD_CHARINFO_ADDIN_NTF )
{
    __TOUCHED((pServerSession, wSize));
	MSG_AD_CHARINFO_ADDIN_NTF* pRcvMsg = (MSG_AD_CHARINFO_ADDIN_NTF *)pMsg;

	DWORD UserKey = pRcvMsg->m_dwKey;
	DBUser* pUser = g_DBUserManager.FindUser( UserKey );

	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[MSG_AD_CHARINFO_ADDIN_NTF]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}

	pUser->SSN.CopyFrom( pRcvMsg->m_EncodedSSN.GetBUFFER() );
}


#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
Handler_FromAgentServer_IMPL( SERVERCOMMON_QUERY_CHARINFO_SYN )
{
    __TOUCHED((pServerSession, wSize));
    MSG_SERVERCOMMON_QUERY_CHARINFO_SYN* pRecvMsg =
        static_cast<MSG_SERVERCOMMON_QUERY_CHARINFO_SYN*>(pMsg);

    // normal query,    
    STRING_SAFER_QUERY128 szQueryBuff;

#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK    
    if(pRecvMsg->action_type_ == eCHARNAME_ACTION_SHOP_SEND_GIFT)
    {
        szQueryBuff.MakeString("{?=call S_Char_Exist_Friend('%s',%d,?,?,?,?)}", \
            pRecvMsg->user_info_.char_name_, pRecvMsg->user_info_.from_char_guid_);
    }
    else
    {
#endif
        szQueryBuff.MakeString("{call S_Char_Exist('%s',?,?,?,?)}", pRecvMsg->user_info_.char_name_);
#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK
    }
#endif


    Query_Char_Exist* pQuery = Query_Char_Exist::ALLOC();
    pQuery->Clear(); //Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetUserKey(0);
    pQuery->SetIndex(MAKEDWORD((WORD)SERVERCOMMON, (WORD)SERVERCOMMON_QUERY_CHARINFO_DBR));
    pQuery->SetVoidObject(pServerSession);

    pQuery->SetQueryActionType(eCHARNAME_QUERY_ACTION_TYPE(pRecvMsg->action_type_));
    pQuery->m_PassingBlock = pRecvMsg->user_info_;

    pServerSession->DBQuery(pQuery);
}

Handler_FromAgentServer_IMPL( SERVERCOMMON_QUERY_CHARINFO_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;
    Query_Char_Exist* pQResult  = (Query_Char_Exist *)pResult->m_pData;
    BOOLEAN success = !!pQResult->ResultSuccess();

    MSG_SERVERCOMMON_QUERY_CHARINFO_ACK msgACK;
    msgACK.Clear();
    msgACK.action_type_ = pQResult->GetQueryActionType();
    
#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK
    if(pQResult->GetResult() == 0)
    {
        msgACK.result_ = success;
    }
    else
    {
        msgACK.result_ = static_cast<BYTE>(pQResult->GetResult());
    }
#else
    msgACK.result_ = success;
#endif
    
    msgACK.user_info_ = pQResult->m_PassingBlock;

    if(success) {
        QueryUserIdInfo& user_info = msgACK.user_info_;
        user_info.user_guid_ = pQResult->GetResultUserGuid();
        user_info.char_guid_ = pQResult->GetResultCharGuid();
        user_info.char_class_ = pQResult->GetResultCharClass();
        user_info.char_level_ = pQResult->GetResultCharLevel();
    }

    msgACK.m_dwKey = msgACK.user_info_.requester_user_key_;
    pServerSession->SendPacket(&msgACK, msgACK.GetSize());

    Query_Char_Exist::FREE(pQResult);
}
#endif //__NA000000_SHOP_USERID_50_PASSING_PROC__

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
Handler_FromAgentServer_IMPL( AD_CHARINFO_PCBANG_BILLING_NTF )
{
    __TOUCHED((pServerSession, wSize));
    MSG_AD_CHARINFO_PCBANG_BILLING_NTF* pRecvMsg = static_cast<MSG_AD_CHARINFO_PCBANG_BILLING_NTF*>(pMsg);
    DWORD user_key = pRecvMsg->m_dwKey;
    DBUser* user = g_DBUserManager.FindUser(user_key);
    if(user == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]:[UserGuid:%u]:Can't find user.", user_key);
        return;
    }

    user->SetIsPCRoom(!!pRecvMsg->m_PcRoomId);
}
#endif

// NOTE: //DB->D 캐릭터 생성후 친구추천관계 맺기 구현부
//  implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND 
Handler_FromAgentServer_IMPL( AD_EVENT_INVITATIONRELATION_INSERT_DBR )
{
    DFriendInvitationController::Instance()->OnAddInvitatedFriendDBR(pServerSession, pMsg, wSize);
}

Handler_FromAgentServer_IMPL( AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN )
{
    DFriendInvitationController::Instance()->OnRewardItemListSelectSYN(pServerSession, pMsg, wSize);
}

Handler_FromAgentServer_IMPL( AD_EVENT_INVITATEFRIEND_REWARD_LIST_DBR )
{
    DFriendInvitationController::Instance()->OnRewardItemListSelectDBR(pServerSession, pMsg, wSize);
}

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
Handler_FromAgentServer_IMPL( AD_CHARNAME_CHANGE_SYN )
{
    const MSG_AD_CHARNAME_CHANGE_SYN* pRecvMsg = 
        static_cast<MSG_AD_CHARNAME_CHANGE_SYN*>(pMsg);

    DBUser* const db_user = g_DBUserManager.FindUser(pRecvMsg->m_dwKey);        
    DBCharacter* db_character = NULL;
    if (db_user)
    {
        db_character = db_user->FindCharacter(pRecvMsg->character_guid_);        
    }

    if(db_user == NULL || db_character == NULL)
    {
        MSG_AD_CHARNAME_CHANGE_NAK nmsg;        
        nmsg.m_dwKey = pRecvMsg->m_dwKey;
        nmsg.slot_index_ = pRecvMsg->slot_index_;
        nmsg.result_ = MSG_AD_CHARNAME_CHANGE_NAK::NOT_FIND_USER;
        pServerSession->SendPacket(&nmsg, sizeof(nmsg));
        return;
    }

    if(db_character->GetReNameStatus() != 1)
    {
        MSG_AD_CHARNAME_CHANGE_NAK nmsg;
        nmsg.m_dwKey = pRecvMsg->m_dwKey;
        nmsg.slot_index_ = pRecvMsg->slot_index_;
        nmsg.result_ = MSG_AD_CHARNAME_CHANGE_NAK::DONOT_RENAME;
        pServerSession->SendPacket(&nmsg, sizeof(nmsg));
        return;
    }
        
    STRING_SAFER_QUERY128 szQueryBuff;
    szQueryBuff.MakeString("{?=call S_Char_ReName(%d,'%s','%s')}", 
        pRecvMsg->character_guid_,
        db_character->GetCharName(),
        pRecvMsg->character_name_);

    Query_CharName_Change* pQuery = Query_CharName_Change::ALLOC();

    pQuery->SetQuery(szQueryBuff);
    pQuery->SetSlotIndex(pRecvMsg->slot_index_);
    pQuery->SetUserKey(db_user->GetUserGuid());
    pQuery->SetCharGuid(pRecvMsg->character_guid_);
    pQuery->SetCharName(pRecvMsg->character_name_);
    pQuery->SetIndex(MAKEDWORD((WORD)AD_CHARINFO, (WORD)AD_CHARNAME_CHANGE_DBR));
    pQuery->SetVoidObject(pServerSession);

    pServerSession->DBQuery(pQuery);
    
}

Handler_FromAgentServer_IMPL( AD_CHARNAME_CHANGE_DBR )
{
    MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;	
    Query_CharName_Change * pQResult = 
        static_cast<Query_CharName_Change*>(pResult->m_pData);

    if( pQResult->IsError() || !pQResult->ResultSuccess() )
    {
        if(pQResult->IsError())
        {
            SUNLOG(eCRITICAL_LOG, "DBERROR: %s (%s)", pQResult->GetQuery(), pQResult->GetErrorMsg());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "QUERY FAIL: %s (%d)", pQResult->GetQuery(), pQResult->GetReturn());
        }	

        MSG_AD_CHARNAME_CHANGE_NAK msg;
        msg.m_dwKey = pQResult->GetUserKey();
        msg.result_     = (MSG_AD_CHARNAME_CHANGE_NAK::ERROR_CODE)pQResult->GetReturn();
        msg.slot_index_ = pQResult->GetSlotIndex();
        pServerSession->SendPacket(&msg, sizeof(msg));
    }	
    else
    {
        DBUser* const db_user = g_DBUserManager.FindUser(pQResult->GetUserKey());        
        DBCharacter* db_character = NULL;
        if (db_user)
        {
            db_character = db_user->FindCharacter(pQResult->GetCharGuid());        
        }

        if(db_user == NULL || db_character == NULL)
        {
            MSG_AD_CHARNAME_CHANGE_NAK msg;
            msg.m_dwKey = pQResult->GetUserKey();
            msg.result_     = MSG_AD_CHARNAME_CHANGE_NAK::NOT_FIND_USER;
            msg.slot_index_ = pQResult->GetSlotIndex();
            pServerSession->SendPacket(&msg, sizeof(msg));
        }
        else
        {
            db_character->SetCharName(pQResult->GetCharName());
            db_character->SetReNameStatus(0);

            MSG_AD_CHARNAME_CHANGE_ACK msg;

            msg.m_dwKey = pQResult->GetUserKey();
            strncpy(msg.character_name_, pQResult->GetCharName(), MAX_CHARNAME_LENGTH+1);
            msg.character_name_[MAX_CHARNAME_LENGTH] = '\0';

            msg.slot_index_ = pQResult->GetSlotIndex();

            pServerSession->SendPacket(&msg, sizeof(msg));

            if (GUILDGUID guild_guid = db_character->GetInfo()->m_GuildGuid)
            {
                CHARGUID char_guid = db_character->GetCharGuid();

                GuildMember* guild_member = g_GuildManager.FindGuildMember(guild_guid, char_guid);
                if (guild_member) {
                    guild_member->SetCharName(pQResult->GetCharName());
                }

                MSG_GZ_GUILD_RENAME_CHAR_CMD gz_guild_msg;
                gz_guild_msg.guildguid = guild_guid;
                gz_guild_msg.charguid  = char_guid;
                strncpy(gz_guild_msg.character_name_, pQResult->GetCharName(), MAX_CHARNAME_LENGTH+1);
                gz_guild_msg.character_name_[MAX_CHARNAME_LENGTH] = '\0';
                g_DBProxyServer.SendToSessionType(GAME_SERVER, &gz_guild_msg, sizeof(gz_guild_msg));

                MSG_WZ_GUILD_RENAME_CHAR_CMD wz_guild_msg;
                wz_guild_msg.guildguid = guild_guid;
                wz_guild_msg.charguid  = char_guid;
                strncpy(wz_guild_msg.character_name_, pQResult->GetCharName(), MAX_CHARNAME_LENGTH+1);
                wz_guild_msg.character_name_[MAX_CHARNAME_LENGTH] = '\0';
                g_DBProxyServer.SendToWorldServer(&wz_guild_msg, sizeof(wz_guild_msg));
            }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            g_GuildManager.UpdateGuildJoinRequestCharacter(db_character->GetCharGuid(), db_character->GetLV(), db_character->GetCharName());
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
    }

    Query_CharName_Change::FREE(pQResult); pQResult = NULL;
}
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
Handler_FromAgentServer_IMPL( AD_ETC_GAMEOPTION_SELECT_SYN )
{
    const MSG_AD_ETC_GAMEOPTION_SELECT_SYN* recv_message = 
        static_cast<MSG_AD_ETC_GAMEOPTION_SELECT_SYN*>(pMsg);

    DBUser* const db_user = g_DBUserManager.FindUser(recv_message->m_dwKey);
    if (db_user == NULL)
    {
        SUNLOG(eFULL_LOG, _T("["__FUNCTION__"]|Msg=can't find user|UserGuid=%d|"), recv_message->m_dwKey);
        MSG_AD_ETC_GAMEOPTION_SELECT_NAK nak_message;
        nak_message.option_type_ = recv_message->option_type_;
        nak_message.error_code_ = RC::RC_GAME_OPTION_DB_USER_NOT_FOUND;
        pServerSession->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    STRING_SAFER_QUERY1024 query_buffer;
    query_buffer.MakeString(_T("{?=call S_GameOption_Select(?,?,?)}"));

    Query_GameOption_Select* query = Query_GameOption_Select::ALLOC();
    Query_GameOption_Select::sQueryParam& parameter = query->parameter_[0];

    parameter.user_guid_ = recv_message->user_guid_;
    parameter.character_guid_ = recv_message->character_guid_;
    parameter.gameoption_type_ = recv_message->option_type_;
    query->SetQuery(query_buffer);
    query->SetUserKey(recv_message->user_guid_);
    pServerSession->DBQuery(AD_ETC, AD_ETC_GAMEOPTION_SELECT_DBR, query);
}

Handler_FromAgentServer_IMPL( AD_ETC_GAMEOPTION_SELECT_DBR )
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GameOption_Select* gameoption_select =
        reinterpret_cast<Query_GameOption_Select*>(db_result->m_pData);

    Query_GameOption_Select::sQueryParam& parameter = gameoption_select->parameter_[0];
    Query_GameOption_Select::sQueryResult& result = gameoption_select->result_[0];

    if (parameter.result_ == 0 || parameter.result_ == 1)
    {
        MSG_AD_ETC_GAMEOPTION_SELECT_ACK ack_message;
        ack_message.m_dwKey = gameoption_select->GetUserKey();
        ack_message.user_guid_ = parameter.user_guid_;
        ack_message.character_guid_ = parameter.character_guid_;
        ack_message.game_option_.option_type_ = (GameOption::Type)parameter.gameoption_type_;

        if (parameter.result_ == 0)
        {
            BOOST_STATIC_ASSERT(sizeof(ack_message.game_option_.binary_data_) ==
                sizeof(result.gameoption_data_));
            // CHANGES: f110609.9L, the modification of the query result interation
            ASSERT(sizeof(ack_message.game_option_.binary_data_) >= result.gameoption_size_);
            ack_message.game_option_.binary_data_size_ =
                min(sizeof(ack_message.game_option_.binary_data_), result.gameoption_size_);
            CopyMemory(ack_message.game_option_.binary_data_,
                    result.gameoption_data_, ack_message.game_option_.binary_data_size_);
        }
        else
        {
            ack_message.game_option_.binary_data_size_ = 0;
        }
        pServerSession->SendPacket(&ack_message, ack_message.GetSize());
    }
    else
    {
        MSG_AD_ETC_GAMEOPTION_SELECT_NAK nak_message;
        nak_message.m_dwKey = gameoption_select->GetUserKey();
        nak_message.option_type_ = parameter.gameoption_type_;
        nak_message.error_code_ = RC::RC_GAME_OPTION_LOAD_FAIL_FROM_DB;
        pServerSession->SendPacket(&nak_message, sizeof(nak_message));
    }

    Query_GameOption_Select::FREE(gameoption_select);
}

Handler_FromAgentServer_IMPL( AD_ETC_GAMEOPTION_UPDATE_CMD )
{
    const MSG_AD_ETC_GAMEOPTION_UPDATE_CMD* cmd_message = 
        static_cast<MSG_AD_ETC_GAMEOPTION_UPDATE_CMD*>(pMsg);

    DBUser* const db_user = g_DBUserManager.FindUser(cmd_message->m_dwKey);
    if (db_user == NULL)
    {
        SUNLOG(eFULL_LOG, _T("["__FUNCTION__"]|Msg=can't find user|UserGuid=%d|CharGuid=%d|OptionType:%d"), 
                    cmd_message->m_dwKey, cmd_message->character_guid_, cmd_message->game_option_.option_type_);
        return;
    }

    Query_GameOption_Update* query = Query_GameOption_Update::ALLOC();
    STRING_SAFER_QUERY1024 query_buffer;
    query_buffer.MakeString(_T("{?=call S_GameOption_Update(?,?,?,?,?)}"));

    Query_GameOption_Update::sQueryParam& parameter = query->parameter_[0];
    parameter.user_guid_ = cmd_message->user_guid_;
    parameter.character_guid_ = cmd_message->character_guid_;
    parameter.gameoption_type_ = cmd_message->game_option_.option_type_;
    parameter.stream_size_ = cmd_message->game_option_.binary_data_size_;
    if (cmd_message->game_option_.binary_data_size_)
    {
        CopyMemory(parameter.gameoption_stream_, 
                   cmd_message->game_option_.binary_data_, 
                   cmd_message->game_option_.binary_data_size_);
    }
    else
    {
        ZeroMemory(parameter.gameoption_stream_, sizeof(parameter.gameoption_stream_));
    }
    query->SetQuery(query_buffer);
    query->SetUserKey(cmd_message->user_guid_);
    pServerSession->DBQuery(AD_ETC, AD_ETC_GAMEOPTION_UPDATE_DBR, query);
}

Handler_FromAgentServer_IMPL( AD_ETC_GAMEOPTION_UPDATE_DBR )
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GameOption_Update* gameoption_update =
        reinterpret_cast<Query_GameOption_Update*>(db_result->m_pData);

    if (!gameoption_update->ResultSuccess())
    {
        const DWORD user_key = gameoption_update->GetUserKey();
        SUNLOG(eCRITICAL_LOG,
            _T("[AD_ETC_GAMEOPTION_SAVE_DBR][U:%u][%s]: Game Option Update Error!!"),
            user_key, gameoption_update->GetQuery());
    }

#ifdef _NA_004517_20120312_GAME_OPTION_TRANSACTION
    MSG_AD_ETC_GAMEOPTION_UPDATE_ACK update_ack;
    {
        update_ack.Clear();
        update_ack.m_dwKey = gameoption_update->GetUserKey();
        const Query_GameOption_Update::sQueryParam& query_param = gameoption_update->parameter_[0];
        update_ack.user_guid = query_param.user_guid_;
        update_ack.char_guid = query_param.character_guid_;
        update_ack.option_type = query_param.gameoption_type_;
        update_ack.is_success = gameoption_update->ResultSuccess();
    }
    pServerSession->SendPacket(&update_ack, sizeof(update_ack));
#endif // _NA_004517_20120312_GAME_OPTION_TRANSACTION

    Query_GameOption_Update::FREE(gameoption_update);
}

#endif //_NA_0_20110329_GAMEOPTION_SETTING

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
Handler_FromAgentServer_IMPL(AD_ETC_GUILD_RANKINGINFO_SYN)
{
    const MSG_AD_ETC_GUILD_RANKINGINFO_SYN* packet = \
        static_cast<MSG_AD_ETC_GUILD_RANKINGINFO_SYN*>(pMsg);

    STRING_SAFER_QUERY128 query_string;
    query_string.MakeString(_T("{? = call S_GuildRanking_Select}"));

    Query_GuildRanking_Select* query = Query_GuildRanking_Select::ALLOC();
    query->SetIndex(MAKEDWORD((WORD)AD_ETC, (WORD)AD_ETC_GUILD_RANKINGINFO_DBR));
    query->SetQuery(query_string);
    query->SetVoidObject(pServerSession);

    pServerSession->DBQuery(query);
}

Handler_FromAgentServer_IMPL(AD_ETC_GUILD_RANKINGINFO_DBR)
{
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildRanking_Select* query_result = \
        static_cast<Query_GuildRanking_Select*>(db_result->m_pData);

    if (query_result->IsError()) {
        SUNLOG(eCRITICAL_LOG, "%s : %s", 
            query_result->GetQuery(), query_result->GetErrorMsg());
    }
    else 
    {
        int row_count = query_result->GetResultRowNum();
        GUILD_RANKING_INFO* guild_rank_info = &query_result->result_records_->guild_rank_info;

        MSG_AD_ETC_GUILD_RANKINGINFO_ACK msg;
        msg.guild_ranking_info_count_ = row_count;
        memcpy(&msg.guild_ranking_info_, guild_rank_info,
            sizeof(GUILD_RANKING_INFO)*row_count);
        pServerSession->SendPacket(&msg, sizeof(msg));

        g_GuildManager.ProcessGuildRankingUpdate(guild_rank_info);
    }

    Query_GuildRanking_Select::FREE(query_result);
}

Handler_FromAgentServer_IMPL(AD_ETC_GUILDINFO_SYN)
{
    const MSG_AD_ETC_GUILDINFO_SYN* packet = \
        static_cast<MSG_AD_ETC_GUILDINFO_SYN*>(pMsg);

    MSG_AD_ETC_GUILDINFO_ACK msg;
    msg.init();
    msg.m_dwKey = packet->m_dwKey;

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    msg.request_state_ = eGUILD_JOIN_REQUEST_OK;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    Guild* guild = g_GuildManager.FindGuild(packet->guid_);
    if(guild) 
    {
        GUILDGUID guid = guild->GetGuildGuid();
        msg.guid_ = guid;
        guild->SerializeGameGuildInfo(msg.game_guild_info_, SERIALIZE_LOAD);
        g_GuildManager.GetGuildInfoExtra(guid, msg.guild_info_extra_);
        DBUser* db_user = g_DBUserManager.FindUser(packet->m_dwKey);
        if (db_user)
        {
            DBCharacter* db_character = db_user->GetConnectedChar();
            if (db_character)
            {
                if (g_GuildManager.CanGuildJoinRequest(db_character->GetCharGuid(), guid) == RC::RC_GUILD_JOIN_REQUEST_ALREADY_EXIST)
                {
                    msg.request_state_ = eGUILD_JOIN_REQUEST_CANCEL;
                }
            }
        }
    }

    pServerSession->SendPacket(&msg, sizeof(msg));
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM