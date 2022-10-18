#include "stdafx.h"
#include "./DBQueryProxyManager.h"

#include <PacketStruct_AD.h>
#include <PacketStruct_DG.h>
#include <SCItemSlot.h>
//
#include "./DBUser.h"
#include "QueryObjects/AgentServerQuery.h"
#ifdef _USE_MAX_CHAR_LIST_PARSER
#include <MaxCharListParser.h>
#endif

//==================================================================================================

namespace nsQuery {
;

}; //end of namespace 'nsQuery'

nsQuery::DBQueryProxyManager* nsQuery::DBQueryProxyManager::db_query_proxy_manager_s = NULL;

//==================================================================================================

void nsQuery::DBQueryProxyManager::CreateInstance()
{
    if (db_query_proxy_manager_s != NULL) {
        return;
    }
    db_query_proxy_manager_s = new DBQueryProxyManager;
}

void nsQuery::DBQueryProxyManager::DestroyInstance()
{
    SAFE_DELETE(db_query_proxy_manager_s);
}

nsQuery::DBQueryProxyManager::DBQueryProxyManager()
{
}

nsQuery::DBQueryProxyManager::~DBQueryProxyManager()
{
}

// (NOTE) (f100423.3L) (WAVERIX) to support the synchronization fault tolerant mechanism
// the fast reconnect request should be pending process \
// whether the game server has the lazy request
// returns = { 0, 1, 2, 3, 4 };
// 0 : error,   reuse_db_user == NULL, send ~_CHARLISTREQ_NAK
// 1 : success, reuse_db_user == NULL, need create user and continuous process
// 2 : success, reuse_db_user != NULL, reuse the db user
// 3 : success, reuse_db_user != NULL, reuse the db user from extracted pending.
// 4 : success, reuse_db_user == NULL, the db user has pending state. the caller, packet handler,
//      should be non act because of a lazy event occurred after exit the pending state
nsQuery::DBQueryProxyManager::eConnectResult
nsQuery::DBQueryProxyManager::EnterConnectPendingProcess(DBUser** aquired_db_user,
    const MSG_AD_CHARINFO_CHARLISTREQ_SYN* recv_msg, ChildServerSession* server_session)
{
    *aquired_db_user = NULL;

    const USERGUID user_guid = recv_msg->m_UserGuid;
    LifetimeTable::iterator it = lifetime_table_.find(user_guid);
    DBUser* pending_user = (it == lifetime_table_.end()) ? NULL : it->second;
    if (pending_user != NULL && impl::IsValidUser(pending_user, __FUNCTION__, __LINE__) == false)
    {
        pending_user = NULL;
        lifetime_table_.erase(it);
        it = lifetime_table_.end();
    }
    DBUser* const db_user = (pending_user == NULL) ? g_DBUserManager.FindUser(user_guid) :
                                                     pending_user;
    if (db_user == NULL) {
        assert(pending_user == NULL);
        return this->eConnectResult_Create;
    }
    //
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    DBQueryProxyUserNode::Shared* user_shared = &user_node->shared_;
    if (user_shared->request & user_node->eReq_ConnectPending)
    {   // CHANGES: f110614.2L, to prevent connection request hangs
        if ((user_node->request_info_.registered_tick + (60 * 1000)) > ::GetTickCount()) {
            return this->eConnectResult_Error;
        };
        user_node->CancelRequest();
    };
    //
    if (pending_user == NULL)
    {   // the db_user, from the DBUserManager, has the lazy update state
        // case 1: reentered by the server select facility: hang in the balance user|char state
        // case 2: back to the char select scene: not disconnect user, disconnect char
        bool assert_rule = true;
        bool is_back_to_charselect = recv_msg->m_bBackToSelect != false;
        const int channel_id = db_user->GetChannelID();
        bool is_server_select = channel_id != recv_msg->m_ChannelID;
        __TOUCHED(is_server_select); // for debug
        // assert(is_back_to_charselect != is_server_select);
        // (NOTE) however, the non master mode hang in the balance a channel
        if (is_back_to_charselect) {
            assert_rule = FlowControl::FCAssert(db_user->GetConnectedChar() == NULL) != false;
        }
        if (assert_rule == false) {
            return this->eConnectResult_Error;
        }
        if (is_back_to_charselect) {
            *aquired_db_user = db_user;
            return this->eConnectResult_Reuse;
        }
        // other state, should be the db user using the server select facility.
        // ... if (HasQueryTransaction(db_user))

        // (NOTE) the server select state is occurred that a game server request \
        // to db proxy server to disconnect user \
        // after a game server is commanded by a agent server to disconnect.
        // so that the client is enabled fast speed re-connection another a agent server.
        // need to pending request and need to reply the lazy answer.
        //
        // (NOTE) this has a automatic event that the db user is moved to pending list \
        // after disconnect user by the game server request
        user_node->RequestConnect(recv_msg, server_session);
        return this->eConnectResult_Pending;
    }
    else
    {   // the db_user is acquired from the LifetimeTable
        assert(db_user == pending_user);
        // CHANGES: f101218.1L, reduce a dangling waiting possibility on the character select scene.
        // referred to f101217.2L
        UpdateQueryTransaction(pending_user);
        if (user_node->loaded() == false ||
            pending_user->GetChannelTransactionLevel() > 1)
        {
            user_node->RequestConnect(recv_msg, server_session);
            return this->eConnectResult_Pending;
        }
        else
        {
            lifetime_table_.erase(it);
            *aquired_db_user = pending_user;
            // CHANGES: f110825.2L
            if (pending_user->GetConnectedChar()) {
                pending_user->OnDisconnectChar();
            };
            return this->eConnectResult_PendingReuse;
        }
    }
    //return this->eConnectResult_Error;
}

nsQuery::DBQueryProxyManager::eEnterGameResult
nsQuery::DBQueryProxyManager::EnterGamePendingProcess(
    DBUser* db_user, DBCharacter* db_character, ChildServerSession* server_session)
{
    __UNUSED(server_session);
    assert(db_character != NULL && db_character == db_user->GetConnectedChar());
    DBQueryProxyCharNode* const char_node = db_character->db_query_node();
    if (char_node->loaded()) {
        return this->eEnterGameResult_Enterable;
    }
    eRequestResult request_result = Request_Char_Data_All(db_user, db_character);
    if (request_result == this->eReqResult_Failed) {
        return this->eEnterGameResult_Fail;
    }
    return this->eEnterGameResult_Pending;
}

// NOTE: f101020.5L, this method regard for pending users to support a find user function
// in other words, find in DBUserManager first, and find in PendingUser second
nsQuery::DBQueryProxyManager::eFindResult
nsQuery::DBQueryProxyManager::FindUserWithPendings(
    USERGUID user_guid,
    DBUser** find_db_user) const
{
    DBUser* db_user = g_DBUserManager.FindUser(user_guid);
    if (db_user != NULL)
    {
        *find_db_user = db_user;
        return this->eFindResult_Service;
    };
    LifetimeTable::const_iterator it = lifetime_table_.find(user_guid);
    if (it == lifetime_table_.end()) {
        *find_db_user = NULL;
        return this->eFindResult_NotFound;
    }
    db_user = it->second;
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        *find_db_user = NULL;
        return this->eFindResult_NotFound;
    }
    *find_db_user = db_user;
    return this->eFindResult_Pending;
}

// NOTE: f101020.5L, this method regard for pending users to support a find user function
// in other words, find in DBUserManager first, and find in PendingUser second
nsQuery::DBQueryProxyManager::eFindResult
nsQuery::DBQueryProxyManager::FindUserWithPendings(
    USERGUID user_guid,     CHARGUID char_guid,
    DBUser** find_db_user,  DBCharacter** find_db_character) const
{
    eFindResult result = FindUserWithPendings(user_guid, find_db_user);
    if (result == this->eFindResult_NotFound) {
        *find_db_character = NULL;
        // result set = { not found, not found user, not found character }
        return result;
    };
    DBUser* db_user = *find_db_user;
    DBCharacter* db_character = db_user->FindCharacter(char_guid);
    if (db_character == NULL)
    {
        *find_db_character = NULL;
        // result set = { not found, found user, not found character }
        return this->eFindResult_NotFound;
    };

    // result set = { X, found user, found character }
    *find_db_character = db_character;
    return result;
}

DBUser* nsQuery::DBQueryProxyManager::MoveUserFromPendings(USERGUID user_guid)
{
    LifetimeTable::iterator it = lifetime_table_.find(user_guid);
    if (it == lifetime_table_.end()) {
        return NULL;
    }
    DBUser* db_user = it->second;
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        db_user = NULL;
    }
    // redirecting ...

    lifetime_table_.erase(it);
    return db_user;
}

bool nsQuery::DBQueryProxyManager::MoveUserToPendings(DBUser* db_user)
{
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return false;
    }
    USERGUID user_guid = db_user->GetUserGuid();
    if (lifetime_table_.find(user_guid) != lifetime_table_.end())
        return false;

    lifetime_table_[user_guid] = db_user;
    // pending control ...
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    user_node->lifetime_timeout_.SetNextTimeoutByCurTickBased(\
        user_node->kPendingLifeTimeAfterDelete);
    return true;
}

// call MoveUserToPendings after the db user extract from the table of the DBUserManager
bool nsQuery::DBQueryProxyManager::DisconnectUser(DBUser* db_user, bool remove_from_user_table)
{
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return false;
    }
    //assertion: db_user != NULL
    // CHANGES: f101216.1L, changes logic to cancel request process if an user request enterance.
    const DWORD user_key = db_user->GetUserKey();
    //
    if (remove_from_user_table == false) {
        // NOTE: f100424.1L, change concept. move to pending list.
        MoveUserToPendings(db_user);
    }
    else //if (remove_from_user_table)
    {
        DBUser* db_user_arg = NULL;
        eFindResult find_result = FindUserWithPendings(user_key, &db_user_arg);
        if (FlowControl::FCAssert(db_user && db_user == db_user_arg) == false) {
            return false;
        }
        if (find_result == this->eFindResult_Service)
        {
            DBUser* db_user_extracted_from_manager = g_DBUserManager.RemoveFromTable(user_key);
            ASSERT(db_user == db_user_extracted_from_manager);
            db_user = db_user_extracted_from_manager;
            MoveUserToPendings(db_user);
        }
    }

    return true;
}

bool nsQuery::DBQueryProxyManager::UpdateQueryTransaction(DBUser* db_user)
{
    const bool kHasTransaction = true;
    const bool kAllFlushed = false;
    //
    DBQueryProxyUserNode* const user_node = db_user->db_query_node();
    const DBQueryProxyUserNode::Shared* user_shared = &user_node->shared_;
    if (user_node->loaded() == false) {
        // NOTE: f101217.2L, prevent a corrupt querying regardless of transaction.
        return kHasTransaction;
    }
    //
    bool changed_events = false;
    //----------------------------------------------------------------------------------------------
    // NOTE: character level update block
    const DBUser::CHAR_HASH& char_list = db_user->GetCharHash();
    FOREACH_CONTAINER(const DBUser::CHAR_HASH::value_type& node, char_list, DBUser::CHAR_HASH)
    {
        DBCharacter* db_character = node.second;
        const DBQueryProxyCharNode* char_node = db_character->db_query_node();
        const DBQueryProxyCharNode::Shared* char_shared = &char_node->shared_;
        // (f100603.5L) add update routines by a latest data application mechanism
        if (char_shared->request & char_node->eReq_Update_CharInfo) {
            db_character->UpdateCharInfo();
        }
        if (char_shared->request & char_node->eReq_Update_InvenInfo) {
            db_character->UpdateInventory();
        }
        bool cleared = db_character->UpdateSubData();
        if (cleared == false) {
            changed_events = true;
        }
        if (char_shared->transaction != char_node->eTrans_None) {
            // CHANGES: f101217.2L, changed to lately conclude
            changed_events = true;
        }
    }
    // end of 'character level update block'
    //----------------------------------------------------------------------------------------------
    // NOTE: user level update block
    //
    // f100603.5L, add update routines by a latest data application mechanism
    ;{
        bool cleared = false;
        //
        if (user_shared->request & user_node->eReq_Update_Warehouse)
        {
            cleared = db_user->UpdateWarehouse();
            if (cleared == false) {
                changed_events = true;
            };
        }
        cleared = user_node->Update();
        if (cleared == false) {
            changed_events = true;
        };
    };
    // NOTE: enforce state check
    if (user_shared->transaction != user_node->eTrans_None) {
        changed_events = true;
    };
    // end of 'user level update block'
    //----------------------------------------------------------------------------------------------
    if (changed_events) {
        return kHasTransaction;
    }
    return kAllFlushed;
}

void nsQuery::DBQueryProxyManager::Update()
{
    LifetimeTable::iterator it = lifetime_table_.begin();
    while (it != lifetime_table_.end())
    {
        // timeout check & release control
        DBUser* const db_user = it->second;
        if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
        {
            lifetime_table_.erase(it++);
            continue;
        }
        //
        const DBQueryProxyUserNode* user_node = db_user->db_query_node();
        const DBQueryProxyUserNode::Shared* user_shared = &user_node->shared_;
        // CHANGES: f101217.2L, reduce a dangling waiting possibility
        // when a query transaction has high-load weight or
        // it has in the middle of doing transaction like a update.
        //
        // to synchronize db characters update transaction.
        const ulong has_transaction = UpdateQueryTransaction(db_user) << 8;
        // CHANGES: f110429.6L, moved to first condition check
        if (db_user->GetChannelTransactionLevel() != 0)
        {
            // waiting
            ++it;
            continue;
        };
        const ulong is_pending_connect_request = \
            (((user_shared->request & user_node->eReq_ConnectPending) != 0) << 16);
        const ulong is_loaded_user_level = user_node->loaded();
        // state table
        //     LOADED  | HAS_TRANS |  CONNECT | [ACTION]
        // [0]    X          X           X      releasing
        // [1]    X          X           O      redirecting + invoke transaction
        // [2]    X          O           X      waiting
        // [3]    X          O           O      waiting
        // [4]    O          X           X      releasing
        // [5]    O          X           O      redirecting
        // [6]    O          O           X      waiting
        // [7]    O          O           O      waiting
        //
        const bool is_expired = user_node->lifetime_timeout_.IsExpired();
        // [0][4]
        if (has_transaction == 0 && is_pending_connect_request == 0)
        {   // wait until timeout to flexible lifetime control because it's possible that
            // any unmanaged transaction by the manager exist.
            if (is_expired)
            {
                lifetime_table_.erase(it++);
                g_DBUserManager.DestroyUser(db_user);
                continue;
            }
            // waiting
            ++it;
            continue;
        };
        // [2][3][6][7] - all waiting
        if (has_transaction)
        {
        #if SUN_CODE_BACKUP
            if (is_loaded_user_level && is_pending_connect_request)
            {
                if (SendPendingConnectionResult(db_user))
                {   // if fails, send nak
                    lifetime_table_.erase(it++);
                    g_DBUserManager.InsertFromPendings(db_user);
                    continue;
                }
                else if (is_expired)
                {
                    lifetime_table_.erase(it++);
                    g_DBUserManager.DestroyUser(db_user);
                    continue;
                }
            }
        #endif
            // waiting
            ++it;
            continue;
        };
        // [1][5]
        assert(is_pending_connect_request);
        if (is_pending_connect_request)
        {
            if (SendPendingConnectionResult(db_user))
            {   // if fails, send nak
                lifetime_table_.erase(it++);
                g_DBUserManager.InsertFromPendings(db_user);
                continue;
            }
            else if (is_expired)
            {
                lifetime_table_.erase(it++);
                g_DBUserManager.DestroyUser(db_user);
                continue;
            }
            // waiting
            ++it;
            continue;
        }
        //
        //
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=unexpected status, state code = 0x%08X|"),
               __FUNCTION__, (is_pending_connect_request | has_transaction | is_loaded_user_level));

        // last... maybe this user become a zombie. (dangling, dangling...)
        ++it;
        continue;
    }
}

bool nsQuery::DBQueryProxyManager::SendPendingConnectionResult(DBUser* db_user)
{
    const USERGUID user_guid = db_user->GetUserGuid();
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    //DBQueryProxyUserNode::Shared* user_shared = &user_node->shared_;
    RC::eCHAR_RESULT error_code = RC::RC_CHAR_SUCCESS;

    const MSG_AD_CHARINFO_CHARLISTREQ_SYN& request_msg = user_node->request_info_.message;
    if (user_guid != request_msg.m_UserGuid) {
        ASSERT(!"|["__FUNCTION__"]|unexpected status|");
        return false;
    };
    struct AutoCancelRequest {
        AutoCancelRequest(DBQueryProxyUserNode* input_user_node)
            : user_node(input_user_node)
        {}
        ~AutoCancelRequest()
        {
            user_node->CancelRequest();
        }
        DBQueryProxyUserNode* const user_node;
    private:
        AutoCancelRequest() : user_node(NULL) {}
        void operator=(const AutoCancelRequest&) { new (this) AutoCancelRequest; };
    } auto_cancel_request(user_node);
    //
    if (user_node->request_info_.session == NULL ||
        user_node->request_info_.session->IsConnected() == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=replying session is disconnected|"), __FUNCTION__);
        return false;
    }

    //db_user->SetUserKey(user_guid);
    //db_user->SetUserGuid(user_guid);
    db_user->SetDBState(eDB_STATE_CREATING);
    db_user->SetIsPCRoom(request_msg.m_bIsPCRoom);
    // 서버 세션 초기화
    db_user->SetServerSession(user_node->request_info_.session);
    db_user->SetChannelID(request_msg.m_ChannelID);
    int location_index = 0;
    if (user_node->loaded() == false)
    {
        nsQuery::DBQueryProxyManager::eRequestResult req_result;
        // (f100729.3L) change to a batch request routine
        req_result = Request_User_Select_All(db_user);
        if (req_result == this->eReqResult_Failed)
        {
            error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
            location_index = 100 + 10;
            goto ERROR_HANDLING;
        }
    }
    // refresh loaded() state
    if (user_node->loaded())
    {
        db_user->SetDBState(eDB_STATE_CREATED);
        if (this->SendUserInfoToAgentOnAllCharacterLoaded(db_user) == false)
        {
            error_code = RC::RC_CHAR_DBUSER_ALREADY_CREATED;
            location_index = 100 + 20;
            goto ERROR_HANDLING;
        }
    }
    // CHANGES: f110825.2L
    if (FlowControl::FCAssert(db_user->GetConnectedChar() != NULL) == false) {
        db_user->OnDisconnectChar();
    };

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    g_DBProxyServer.ExecuteQuery_MatchFatigue_Select(user_guid, user_guid);
#endif
    // success
    return true;

ERROR_HANDLING:
    ;{
        MSG_AD_CHARINFO_CHARLISTREQ_NAK req_nak_msg;
        req_nak_msg.m_dwKey = user_guid;
        req_nak_msg.m_byErrorCode = error_code;
        user_node->request_info_.session->SendPacket(&req_nak_msg, sizeof(req_nak_msg));
        // CHANGES: f110613.2L, added the failed log when the user failed in connect
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't receive character list, ")
               _T("UserGuid=%u, Result=%u, location=#%d|"),
               __FUNCTION__, user_guid, error_code, location_index);
    };
    return false;
}

bool nsQuery::DBQueryProxyManager::SendUserInfoToAgentOnAllCharacterLoaded(DBUser* db_user)
{
    const USERGUID user_guid = db_user->GetUserGuid();
    ChildServerSession* server_session = db_user->GetServerSession();
    DBQueryProxyUserNode* const query_user_node = db_user->db_query_node();
    if (!FlowControl::FCAssert(query_user_node->loaded() != false)) {
        return false;
    }
    // CHANGES: f110622.2L, after a zombie user checker apply on agent server,
    // it needs refresh routine that is check routine whether the user is zombie.
    db_user->UpdateLatestUpdateTick();

    static SERVER_CHARACTER_PART server_char_part[MAX_CHARACTER_LIST_NUM];
    MSG_AD_CHARINFO_CHARLISTREQ_ACK req_ack_msg;
    req_ack_msg.m_dwKey = user_guid;
    req_ack_msg.m_byCount = static_cast<uint8_t>(db_user->GetNumberOfChars());
    assert(req_ack_msg.m_byCount <= MAX_CHARACTER_LIST_NUM);
#ifdef _NA_002050_20110216_ADD_GENDER
    req_ack_msg.char_slot_size = db_user->GetCharSlotSize();
#endif

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    ZeroMemory(req_ack_msg.m_ReNameStatus, sizeof(req_ack_msg.m_ReNameStatus));
#endif

    const DBUser::CHAR_HASH& char_list = db_user->GetCharHash();
    DBUser::CHAR_HASH::const_iterator it = char_list.begin(),
                                      end = char_list.end();
    for (int index = 0; index < _countof(server_char_part) && it != end; ++index, ++it)
    {
        DBCharacter* db_character = it->second;
        ZeroMemory(&server_char_part[index], sizeof(server_char_part[index]));
        db_character->Serialize(server_char_part[index], SERIALIZE_LOAD);
        const BASE_PLAYERINFO* char_info = db_character->GetInfo();
        if (!FlowControl::FCAssert(char_info->m_UserGuid == user_guid))
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[%s]|Msg=mismatch user guid in a table|")
                   _T("|Requested UserGuid=%d|Contained UserGuid=%d|"),
                   __FUNCTION__, user_guid, char_info->m_UserGuid);
            return false;
        }
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        req_ack_msg.m_ReNameStatus[char_info->m_bySlot] = 
                    static_cast<BYTE>(db_character->GetReNameStatus());
#endif
    }
    //                            must be m_byCount
    int sending_size = req_ack_msg.ConvertForSend(server_char_part);
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    // CHANGES: f110306.1L, fixed a invalid usage about '_countof'.
    // the '_countof' means number of elements.
    sending_size += sizeof(req_ack_msg.m_ReNameStatus);
#endif
    server_session->SendPacket(&req_ack_msg, static_cast<WORD>(sending_size));

    return true;
}

// (f100731.5L) replace the 'SendAllDataToGameServer' function
// to support a selected character's data control.
bool nsQuery::DBQueryProxyManager::SendCharInfoToGameOnAllCharDataLoaded(\
    DBUser* db_user, DBCharacter* db_character)
{
    __TOUCHED(db_user);
    // (NOTE) currently (100809), same as a custom logic
    assert(db_character != NULL && db_character == db_user->GetConnectedChar());
    db_character->SendAllDataToGameServer();
    return true;
}

//==================================================================================================
// (f100729.3L) added to support all loaded sequence process
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_User_Select_All(DBUser* db_user)
{
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    // r100928.3L
    eRequestResult req_result = this->eReqResult_Loaded;
    if (user_node->loaded() == false)
    {
        req_result = Request_User_Select(db_user, user_node->eTrans_Select_CharInfo);
        if (req_result == this->eReqResult_Failed) {
            return req_result;
        };
        req_result = Request_User_Select(db_user, user_node->eTrans_Select_InvenInfo);
        if (req_result == this->eReqResult_Failed) {
            return req_result;
        };
        req_result = Request_User_Select(db_user, user_node->eTrans_Select_PetList);
        if (req_result == this->eReqResult_Failed) {
            return req_result;
        };
    };

    return req_result;
}

// f101004.4L, added to support partial equipment queries after split T_Inventory query
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_Char_Data_Equipment(
    DBUser* db_user, DBCharacter* db_character)
{
    eRequestResult req_result = this->eReqResult_Loaded;
    // r100928.2L
    DBQueryProxyCharNode* char_node = db_character->db_query_node();
    DBQueryProxyCharNode::Shared* char_shared = &char_node->shared_;
    //
    const bool loading_transaction = \
        (char_shared->transaction & char_node->eTrans_Select_EquipInfo) != 0;
    const bool loaded = (char_shared->state & char_node->eState_Loaded_EquipInfo) != 0;
    if (loading_transaction) {
        req_result = this->eReqResult_Transaction;
    }
    else if (loading_transaction == false && loaded == false)
    {
        req_result = Request_Char_Transaction(\
            db_user, db_character, nsQuery::ProxyCommand::eCmd_Equip_Load);
    }
    return req_result;
}

// (f100731.2L) added to support all loaded sequence process for character
// (WARNING) no rule check routine
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_Char_Data_All(DBUser* db_user, DBCharacter* db_character)
{
    eRequestResult req_result = this->eReqResult_Loaded;
    // r100928.2L
    DBQueryProxyCharNode* char_node = db_character->db_query_node();
    DBQueryProxyCharNode::Shared* char_shared = &char_node->shared_;
    //
    if (char_node->loaded()) {
        req_result = this->eReqResult_Loaded;
        return req_result;
    }
    //
    nsQuery::DBQueryProxyManager::eRequestResult req_char_result = this->eReqResult_Loaded;
    //
    if (false) // NOTE: this request is called before the 'ENTERVILLAGEREQ_x'
    {
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_EquipInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_EquipInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Equip_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_InvenInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_InvenInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Inven_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_PerkInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_PerkInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Perk_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_SkillInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_SkillInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Skill_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_QuickInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_QuickInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Quick_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_QuestInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_QuestInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Quest_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_BattleRecords) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_BattleRecords) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_BattleRecords_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
    //
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_CharStateInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_CharStateInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_Char_State_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
  #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	;{
		const bool loading_transaction = \
			(char_shared->transaction & char_node->eTrans_Select_Achievement) != 0;
		const bool loaded = (char_shared->state & char_node->eState_Loaded_Achievement) != 0;
		if (loading_transaction) {
			req_result = this->eReqResult_Transaction;
		}
		else if (loading_transaction == false && loaded == false)
		{
			req_char_result = Request_Char_Transaction(\
				db_user, db_character, nsQuery::ProxyCommand::eCmd_Achievement_Load);
			if (req_char_result == this->eReqResult_Failed) {
				req_result = this->eReqResult_Failed;
			}
		}
	};
  #endif //C_NA_0_20100520_ACSYSTEM_CHANGED
    ;{
        const bool loading_transaction = \
            (char_shared->transaction & char_node->eTrans_Select_MissionInfo) != 0;
        const bool loaded = (char_shared->state & char_node->eState_Loaded_MissionInfo) != 0;
        if (loading_transaction) {
            req_result = this->eReqResult_Transaction;
        }
        else if (loading_transaction == false && loaded == false)
        {
            req_char_result = Request_Char_Transaction(\
                db_user, db_character, nsQuery::ProxyCommand::eCmd_MissionInfo_Load);
            if (req_char_result == this->eReqResult_Failed) {
                req_result = this->eReqResult_Failed;
            }
        }
    };
#ifdef _NA_006372_20130108_BADGE_ITEM
    ;{
        //
        req_char_result = Request_Char_Transaction(\
            db_user, db_character, nsQuery::ProxyCommand::eCmd_CharacterExtraInfo_Load);
        if (req_char_result == this->eReqResult_Failed) {
            req_result = this->eReqResult_Failed;
        }
    };
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    ;{
        //
        req_char_result = Request_Char_Transaction(\
            db_user, db_character, nsQuery::ProxyCommand::eCmd_UserExtraInfo_Load);
        if (req_char_result == this->eReqResult_Failed) {
            req_result = this->eReqResult_Failed;
        }
    };
#endif // _NA_007932_20150106_USER_EXTRA_INFO
    return req_result;
}

// S_User_Select + S_User_Select_Inventory
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_User_Select(
    DBUser* db_user, DBQueryProxyUserNode::eTransaction transaction)
{
    const USERGUID user_guid = db_user->GetUserGuid();
    // assert(server_session)

    DBQueryProxyUserNode* const user_node = db_user->db_query_node();
    DBQueryProxyUserNode::Shared* const user_shared = &user_node->shared_;

    if (transaction & user_node->eTrans_Select_CharInfo)
    {   // r100928.3L, r100928.4L
        if ((user_shared->transaction & user_node->eTrans_Select_CharInfo) ||
            (user_shared->transaction & user_node->eTrans_Select_InvenInfo) ||
            (user_shared->transaction & user_node->eTrans_Select_PetList))
        {
            return eReqResult_Transaction;
        };
        if (user_shared->state & user_node->eState_Loaded_CharInfo) {
            return eReqResult_Loaded;
        }

        STRING_SAFER_QUERY64 query_buffer;
#ifdef _NA_002050_20110216_ADD_GENDER
        query_buffer.MakeString(
            _T("{?=call S_CharListByMaxLimit(%d,%u)}"), 
            user_guid, 
  #ifdef _USE_MAX_CHAR_LIST_PARSER
            MaxCharListParser::Instance()->GetDefaultSlotSize()
  #else
            MaxCharListConfig::kDefaultSlotSize
  #endif
        );
#else
        query_buffer.MakeString(_T("S_User_Select %d"), user_guid);
#endif

        Query_User_Select* query = Query_User_Select::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);

        user_node->BeginTransaction(user_node->eTrans_Select_CharInfo);
        db_user->DBQuery(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_CHAR_DBR, query);

        return eReqResult_Success;
    };
    //
    if (transaction & user_node->eTrans_Select_InvenInfo)
    {
        if ((user_shared->transaction & user_node->eTrans_Select_InvenInfo)) {
            return eReqResult_Transaction;
        };
        if (user_shared->state & user_node->eState_Loaded_InvenInfo) {
            return eReqResult_Loaded;
        }

        STRING_SAFER_QUERY64 query_buffer;
#ifdef _NA_002050_20110216_ADD_GENDER
        query_buffer.MakeString(
            _T("S_CharInvenListByMaxLimit %d, %u"), 
            user_guid, 
  #ifdef _USE_MAX_CHAR_LIST_PARSER
            MaxCharListParser::Instance()->GetDefaultSlotSize()
  #else
            MaxCharListConfig::kDefaultSlotSize
  #endif
        );
#else
        query_buffer.MakeString(_T("S_User_Select_Inventory %d"), user_guid);
#endif

        Query_User_Select_Inventory* query = Query_User_Select_Inventory::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);

        user_node->BeginTransaction(user_node->eTrans_Select_InvenInfo);
        db_user->DBQuery(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_INVEN_DBR, query);

        return eReqResult_Success;
    };
    //
    if (transaction & user_node->eTrans_Select_PetList)
    {
        if ((user_shared->transaction & user_node->eTrans_Select_PetList)) {
            return eReqResult_Transaction;
        };
        if (user_shared->state & user_node->eState_Loaded_PetList) {
            return eReqResult_Loaded;
        }

        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString(_T("{?=call S_User_Select_PetList (?)}"));

        Query_User_Select_PetList* query = Query_User_Select_PetList::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);
        //
        query->parameters_[0].user_guid_ = user_guid;
        //
        user_node->BeginTransaction(user_node->eTrans_Select_PetList);
        db_user->DBQuery(AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_PETLIST_DBR, query);

        return eReqResult_Success;
    }
    //
    if (transaction & user_node->eTrans_Select_Warehouse)
    {
        if (user_shared->transaction & user_node->eTrans_Select_Warehouse) {
            return eReqResult_Transaction;
        }
        if (user_shared->state & user_node->eState_Loaded_Warehouse) {
            return eReqResult_Loaded;
        }

        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString(_T("S_Warehouse_Select ?"));

        Query_Warehouse_Select* query = Query_Warehouse_Select::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);
        query->parameter_[0].user_guid_ = user_guid;
        user_node->BeginTransaction(user_node->eTrans_Select_Warehouse);
        db_user->DBQuery(DG_WAREHOUSE, DG_WAREHOUSE_START_DBR, query);

        return eReqResult_Success;
    }
    //
    if (transaction & user_node->eTrans_Select_WareItems)
    {
        if (user_shared->transaction & user_node->eTrans_Select_WareItems) {
            return eReqResult_Transaction;
        }
        if (user_shared->state & user_node->eState_Loaded_WareItems) {
            return eReqResult_Loaded;
        }

        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString(_T("{?=call S_WarehouseItemSlotInfo_Select (?,?)}"));

        Query_ItemSlotInfo_Select* query = Query_ItemSlotInfo_Select::ALLOC();
        query->SetQuery(query_buffer);
        query->SetUserKey(user_guid);
        // set query parameters
        query->parameters_[0].guid_ = user_guid;
        query->parameters_[0].slot_type_ = WarehouseSlotUpdateTable::kSlotIndex;
        user_node->BeginTransaction(user_node->eTrans_Select_WareItems);
        db_user->DBQuery(DG_WAREHOUSE, DG_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR, query);

        return eReqResult_Success;
    }

    return eReqResult_Failed;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Select<Query_User_Select>(
    const Query_User_Select&, DBUser* db_user)
{
    DBQueryProxyUserNode* db_query_user_node = db_user->db_query_node();
    db_query_user_node->EndTransaction(db_query_user_node->eTrans_Select_CharInfo, true);
    return true;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Select<Query_User_Select_Inventory>(
    const Query_User_Select_Inventory& query_result, DBUser* db_user)
{
    typedef Query_User_Select_Inventory::sQUERY_RESULT sQUERY_RESULT;
    const DBUser::CHAR_HASH& char_list = db_user->GetCharHash();
    //DBUser::CHAR_HASH::const_iterator it = char_list.begin(),
    //                                  end = char_list.end();
    // TODO(WAVERIX) change interface to support const object
    const int number_of_records = \
        const_cast<Query_User_Select_Inventory&>(query_result).GetResultRowNum();

    FOREACH_CONTAINER(const DBUser::CHAR_HASH::value_type& node, char_list, DBUser::CHAR_HASH)
    {
        DBCharacter* db_character = node.second;
        const CHARGUID char_guid = db_character->GetCharGuid();
        int find_index = 0;
        const sQUERY_RESULT* inven_it = query_result.pResult;
        for ( ; find_index < _countof(query_result.pResult); ++find_index, ++inven_it)
        {
            if (char_guid == inven_it->m_CharGuid) {
                break;
            }
        }
        if (find_index == _countof(query_result.pResult))
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("["__FUNCTION__"]|Msg=can't find inven for db character|")
                   _T("|UserGuid=%d|CharGuid=%d|"),
                   db_user->GetUserGuid(), char_guid);
            return false; // end transaction... hummm...
        }
        db_character->OnSetInfoInventory(const_cast<sQUERY_RESULT*>(inven_it));
        Request_Char_Data_Equipment(db_user, db_character);
    }

    DBQueryProxyUserNode* db_query_user_node = db_user->db_query_node();
    db_query_user_node->EndTransaction(db_query_user_node->eTrans_Select_InvenInfo, true);
    db_query_user_node->BeginTransaction(db_query_user_node->eTrans_Select_EquipInfo);
    // CHANGES: f101215.1L, fixed a missing event when the user hasn't any character.
    if (number_of_records == 0) {
        db_query_user_node->EndTransaction(db_query_user_node->eTrans_Select_EquipInfo, true);
    }
    if (db_query_user_node->loaded()) {
        SendUserInfoToAgentOnAllCharacterLoaded(db_user);
    }
    return true;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Select<Query_User_Select_PetList>(
    const Query_User_Select_PetList& query_result, DBUser* db_user)
{
    // 1st load
    static SCItemSlot pet_item_s;
    typedef Query_User_Select_PetList::sQueryResult sQueryResult;
    const DBUser::CHAR_HASH& char_list = db_user->GetCharHash();
    DBUser::CHAR_HASH::const_iterator it = char_list.begin(),
                                      end = char_list.end();
    const int number_of_records = query_result.GetResultRowNum();
    FOREACH_CONTAINER(const DBUser::CHAR_HASH::value_type& node, char_list, DBUser::CHAR_HASH)
    {
        DBCharacter* db_character = node.second;
        const CHARGUID char_guid = db_character->GetCharGuid();
        BASE_PLAYERINFO* player_info = db_character->GetInfo();
        player_info->m_bSummonedPet = false;
        //
        const sQueryResult* item_record_it = query_result.result_records_;
        for (int find_index = 0; find_index < number_of_records; ++find_index, ++item_record_it)
        {
            if (char_guid != item_record_it->char_guid_) {
                continue;
            };
            DBQueryProxyCharNode* char_node = db_character->db_query_node();
            ITEMSTREAMEX* pet_stream = &char_node->pet_item_stream_;
            ZeroMemory(pet_stream, sizeof(*pet_stream));
            ITEMPART* item_part = &pet_stream->Part;
            item_part->wCode = static_cast<SLOTCODE>(item_record_it->code_);
            item_part->byDura = static_cast<DURATYPE>(item_record_it->dura_or_num_);
            item_part->m_WorldSerial = item_record_it->serial_;
            CopyMemory(&pet_stream->OptionPart, item_record_it->option_stream_,
                       sizeof(pet_stream->OptionPart));
            pet_item_s.Copy(*pet_stream);
            bool sucess_summon = (pet_item_s.IsPetItem() && pet_item_s.IsUserBlocked());
            player_info->m_bSummonedPet = sucess_summon;
            if (sucess_summon == false)
            {
                player_info->m_bMAXIntimacy = 0;
                player_info->m_PetItemPos = INVALID_POSTYPE_VALUE;
                ZeroMemory(pet_stream, sizeof(*pet_stream));
            }
        }
    }

    DBQueryProxyUserNode* db_query_user_node = db_user->db_query_node();
    db_query_user_node->EndTransaction(db_query_user_node->eTrans_Select_PetList, true);

    if (db_query_user_node->loaded()) {
        SendUserInfoToAgentOnAllCharacterLoaded(db_user);
    }
    return true;
}
// call it if can't find in a DBUserManager, should be updated the db user of the pending list
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::DBResult_User_Select(
    USERGUID user_guid, const QueryForUser& result_base,
    DBQueryProxyUserNode::eTransaction transaction)
{
    __UNUSED(result_base);
    LifetimeTable::iterator it = lifetime_table_.find(user_guid);
    if (it == lifetime_table_.end()) {
        return this->eReqResult_Failed;
    }
    DBUser* db_user = it->second;
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return this->eReqResult_Failed;
    }
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    user_node->EndTransaction(transaction, false);

    // regard shared_.request ... how to do it?
    //db_query_shared->
    return this->eReqResult_Success;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Select<Query_Warehouse_Select>(
    const Query_Warehouse_Select& query_result, DBUser* db_user)
{
    bool succeed = db_user->OnSetWarehouseBaseInfo(query_result);
    //
    if (!FlowControl::FCAssert(succeed)) {
        return false;
    }
    DBQueryProxyUserNode* db_query_user_node = db_user->db_query_node();
    db_query_user_node->EndTransaction(db_query_user_node->eTrans_Select_Warehouse, true);
    return true;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Select<Query_ItemSlotInfo_Select>(
    const Query_ItemSlotInfo_Select& query_result, DBUser* db_user)
{
    // NOTE: list = { warehouse ... } per a user object
    //const SLOTIDX slot_type = query_result.
    nsQuery::DBQueryProxyUserNode* user_node = db_user->db_query_node();
    bool succeed = user_node->DBResult(query_result);
    if (!FlowControl::FCAssert(succeed)) {
        return false;
    }
    user_node->EndTransaction(user_node->eTrans_Select_WareItems, true);
    return true;
}

//==================================================================================================
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_Char_Transaction(
    DBUser* db_user, DBCharacter* db_character,
    ProxyCommand::eCommand command)
{

    const USERGUID user_guid = db_user->GetUserGuid();
    // assert(server_session)

    const CHARGUID char_guid = db_character->GetCharGuid();
    DBQueryProxyCharNode* const char_node = db_character->db_query_node();
    DBQueryProxyCharNode::Shared* const char_shared = &char_node->shared_;

    const nsQuery::ProxyCommand kProxyCommand = { kProxyCommand.eCmd_None };
    __UNUSED(kProxyCommand); //use only object reference
    // r100928.2L
    switch (command)
    {
    case kProxyCommand.eCmd_Equip_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_EquipInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_EquipInfo) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_ItemSlotInfo_Select (?,?)}"));

            Query_ItemSlotInfo_Select* query = Query_ItemSlotInfo_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].guid_ = char_guid;
            query->parameters_[0].slot_type_ = EquipSlotUpdateTable::kSlotIndex;
            //
            db_user->DBQuery(\
                AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_INTER_EQUIP_ITEMSLOTINFO_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_EquipInfo);

            return eReqResult_Success;
        };
        break;
        //
    case kProxyCommand.eCmd_Equip_ChangedUpdate:
        {
            if (char_node->UpdateItemSlotInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_Inven_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_InvenInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_InvenInfo) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_ItemSlotInfo_Select (?,?)}"));

            Query_ItemSlotInfo_Select* query = Query_ItemSlotInfo_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].guid_ = char_guid;
            query->parameters_[0].slot_type_ = InvenSlotUpdateTable::kSlotIndex;
            //
            db_user->DBQuery(\
                DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_ITEMSLOTINFO_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_InvenInfo);

            return eReqResult_Success;
        };
        break;
        //
    case kProxyCommand.eCmd_Inven_ChangedUpdate:
        {
            if (char_node->UpdateItemSlotInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_Perk_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_PerkInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_PerkInfo) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_Perk_Select (?)}"));

            Query_Perk_Select* query = Query_Perk_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_PERK_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_PerkInfo);

            return eReqResult_Success;
        };
        break;
    //
    case kProxyCommand.eCmd_Perk_ChangedUpdate:
        {
            if (char_node->UpdatePerk()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_Skill_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_SkillInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_SkillInfo) {
                return eReqResult_Loaded;
            }
            //
            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_SlotInfo_Select (?,?)}"));

            Query_SlotInfo_Select* query = Query_SlotInfo_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            query->parameters_[0].slot_index_ = SkillSlotUpdateTable::kSlotIndex;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_SkillInfo);

            return eReqResult_Success;
        };
        break;
    //
    case kProxyCommand.eCmd_Skill_ChangedUpdate:
        {
            if (char_node->UpdateSlotInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_Quick_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_QuickInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_QuickInfo) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_SlotInfo_Select (?,?)}"));

            Query_SlotInfo_Select* query = Query_SlotInfo_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            query->parameters_[0].slot_index_ = QuickSlotUpdateTable::kSlotIndex;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_QuickInfo);

            return eReqResult_Success;
        };
        break;
    //
    case kProxyCommand.eCmd_Quick_ChangedUpdate:
        {
            if (char_node->UpdateSlotInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_Quest_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_QuestInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_QuestInfo) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_Quest_Select (?)}"));

            Query_Quest_Select* query = Query_Quest_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_INTER_QUEST_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_QuestInfo);

            return eReqResult_Success;
        };
        break;
    //
    case kProxyCommand.eCmd_Quest_ChangedUpdate:
        {
            if (char_node->UpdateQuest()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_BattleRecords_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_BattleRecords) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_BattleRecords) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_BattleRecords_Select (?)}"));

            Query_BattleRecords_Select* query = Query_BattleRecords_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_INTER_BATTLERECORDS_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_BattleRecords);

            return eReqResult_Success;
        };
        break;
    //
    case kProxyCommand.eCmd_BattleRecords_ChangedUpdate:
        {
            if (char_node->UpdateBattleRecords()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    case kProxyCommand.eCmd_Char_State_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_CharStateInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_CharStateInfo) {
                return eReqResult_Loaded;
            }

            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_Char_State_Select (?)}"));

            Query_Char_State_Select* query = Query_Char_State_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_CHAR_STATE_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_CharStateInfo);

            return eReqResult_Success;
        };
        break;
        //
    case kProxyCommand.eCmd_Char_State_ChangedUpdate:
        {
            if (char_node->UpdateCharStates()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
  #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	case kProxyCommand.eCmd_Achievement_Load:
		{			
			if (char_shared->transaction & char_node->eTrans_Select_Achievement) {
				return eReqResult_Transaction;
			}
			if (char_shared->state & char_node->eState_Loaded_Achievement) {
				return eReqResult_Loaded;
			}

			STRING_SAFER_QUERY64 query_buffer;
			query_buffer.MakeString(_T("{?=call S_ACSystem_Select (?)}"));

			Query_Char_Achievement_Select* query = Query_Char_Achievement_Select::ALLOC();
			query->SetQuery(query_buffer);
			query->SetUserKey(user_guid);
			query->set_char_guid(char_guid);			
			//
			query->parameters_[0].char_guid_ = char_guid;

			db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_SELECT_ACHIEVEMENT_DBR, query);
			char_node->BeginTransaction(char_node->eTrans_Select_Achievement);

			return eReqResult_Success;
		}
		break;
	case kProxyCommand.eCmd_Achievement_ChangedUpdate:
		{
			if (char_node->UpdateAchievement()) {
				return eReqResult_Success;
			}
			return eReqResult_Transaction;
		}
		break;
  #endif //C_NA_0_20100520_ACSYSTEM_CHANGED
    case kProxyCommand.eCmd_MissionInfo_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_MissionInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_MissionInfo) {
                return eReqResult_Loaded;
            }
            //
            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_MissionInfo_Select (?)}"));

            Query_MissionInfo_Select* query = Query_MissionInfo_Select::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_[0].char_guid_ = char_guid;
            //
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_MISSION_DBR, query);
            char_node->BeginTransaction(char_node->eTrans_Select_MissionInfo);

            return eReqResult_Success;
        };
        break;
        //
    case kProxyCommand.eCmd_MissionInfo_ChangedUpdate:
        {
            if (char_node->UpdateMissionInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        };
        break;
    //!~ _NA_006372_20130108_BADGE_ITEM
    case kProxyCommand.eCmd_CharacterExtraInfo_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_CharacterExtraInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_CharacterExtraInfo) {
                return eReqResult_Loaded;
            }
            //
            char_node->SelectCharacterExtraInfo();

            return eReqResult_Success;
        } break;
    case kProxyCommand.eCmd_CharacterExtraInfo_ChangedUpdate:
        {
            if (char_node->UpdateCharacterExtraInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        } break;
    //~! _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    case kProxyCommand.eCmd_UserExtraInfo_Load:
        {
            if (char_shared->transaction & char_node->eTrans_Select_UserExtraInfo) {
                return eReqResult_Transaction;
            }
            if (char_shared->state & char_node->eState_Loaded_UserExtraInfo) {
                return eReqResult_Loaded;
            }
            //
            char_node->SelectUserExtraInfo();

            return eReqResult_Success;
        } break;
    case kProxyCommand.eCmd_UserExtraInfo_ChangedUpdate:
        {
            if (char_node->UpdateUserExtraInfo()) {
                return eReqResult_Success;
            }
            return eReqResult_Transaction;
        } break;
#endif // _NA_007932_20150106_USER_EXTRA_INFO
    default:
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=unknown command %d|"), __FUNCTION__, command);
        assert(!"unknown command");
        break;
    }; //end switch
    //
    return this->eReqResult_Failed;
}

nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::DBResult_Char_Transaction(
    USERGUID user_guid, CHARGUID char_guid, const QueryForCharTransaction& query_result)
{
    LifetimeTable::iterator it = lifetime_table_.find(user_guid);
    if (it == lifetime_table_.end()) {
        return this->eReqResult_Failed;
    }
    DBUser* const db_user = it->second;
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return this->eReqResult_Failed;
    }
    DBCharacter* db_character = db_user->FindCharacter(char_guid);
    if (db_character == NULL) {
        return this->eReqResult_Failed;
    }
    if (DBResult_Char_Transaction(query_result, db_user, db_character) == false) {
        return this->eReqResult_Failed;
    }

    return this->eReqResult_Success;
}

bool nsQuery::DBQueryProxyManager::DBResult_Char_Transaction(
    const QueryForCharTransaction& query_result,
    DBUser* db_user, DBCharacter* db_character)
{
    const nsQuery::ProxyTransaction transaction = { query_result.transaction_type() };
    nsQuery::DBQueryProxyCharNode* const char_node = db_character->db_query_node();
    bool success = false;
    // r100928.2L
    switch (transaction.value_)
    {
    case transaction.eTrans_ItemSlotInfo_Select: //S_ItemSlotInfo_Select
        {
            const Query_ItemSlotInfo_Select& query_slotinfo = \
                static_cast<const Query_ItemSlotInfo_Select&>(query_result);
            success = char_node->DBResult(query_slotinfo);
            const int slot_type = query_slotinfo.parameters_[0].slot_type_; //origin type : SLOTIDX
            const DBQueryProxyCharNode::eTransaction char_trans \
                = slot_type == SI_INVENTORY    ? char_node->eTrans_Select_InvenInfo
                : slot_type == SI_EQUIPMENT    ? char_node->eTrans_Select_EquipInfo
                :                                char_node->eTrans_None;
            char_node->EndTransaction(char_trans, success);
            char_node->UpdateItemSlotInfo();
            if (char_trans != char_node->eTrans_Select_EquipInfo) {
                char_node->ProcessRequestEvent(transaction, success);
                break;
            }

            // NOTE: loading of equipments is occurred by S_User_Select_~.
            // should be update loading complete condition
            bool all_loaded = true;
            const DBUser::CHAR_HASH& char_list = db_user->GetCharHash();
            FOREACH_CONTAINER(DBUser::CHAR_HASH::value_type& node, char_list, DBUser::CHAR_HASH)
            {
                DBCharacter* partial_char = node.second;
                DBQueryProxyCharNode* partial_char_node = partial_char->db_query_node();
                DBQueryProxyCharNode::Shared* const partial_shared = &partial_char_node->shared_;
                if ((partial_shared->state & partial_char_node->eState_Loaded_EquipInfo) == 0) {
                    all_loaded = false;
                    break;
                }
            };
            if (all_loaded) {
                DBQueryProxyUserNode* user_node = db_user->db_query_node();
                user_node->EndTransaction(user_node->eTrans_Select_EquipInfo, success);
            }

            char_node->ProcessRequestEvent(transaction, success);
        };
        break;
    case transaction.eTrans_ItemSlotInfo_Update: //S_ItemSlotInfo_Update
        char_node->DBResult(static_cast<const Query_ItemSlotInfo_Update&>(query_result));
        break;
    case transaction.eTrans_Perk_Select: //S_Perk_Select
        success = char_node->DBResult(static_cast<const Query_Perk_Select&>(query_result));
        char_node->EndTransaction(char_node->eTrans_Select_PerkInfo, success);
        char_node->UpdatePerk();
        char_node->ProcessRequestEvent(transaction, success);
        break;
    case transaction.eTrans_Perk_Insert: //S_Perk_Insert
        char_node->DBResult(static_cast<const Query_Perk_Insert&>(query_result));
        break;
    case transaction.eTrans_Perk_Delete: //S_Perk_Delete
        char_node->DBResult(static_cast<const Query_Perk_Delete&>(query_result));
        break;
    case transaction.eTrans_Perk_Update: //S_Perk_Update
        char_node->DBResult(static_cast<const Query_Perk_Update&>(query_result));
        break;
    case transaction.eTrans_SlotInfo_Select: //S_SlotInfo_Select
        {
            const Query_SlotInfo_Select& query_slotinfo = \
                static_cast<const Query_SlotInfo_Select&>(query_result);
            success = char_node->DBResult(query_slotinfo);
            const int slot_index = query_slotinfo.parameters_[0].slot_index_; //orgin type : SLOTIDX
            const DBQueryProxyCharNode::eTransaction char_trans \
                = slot_index == SkillSlotUpdateTable::kSlotIndex ? char_node->eTrans_Select_SkillInfo
                : slot_index == QuickSlotUpdateTable::kSlotIndex ? char_node->eTrans_Select_QuickInfo
                :                                                  char_node->eTrans_None;
            char_node->EndTransaction(char_trans, success);
            char_node->UpdateSlotInfo();
            char_node->ProcessRequestEvent(transaction, success);
        };
        break;
    case transaction.eTrans_SlotInfo_Insert: //S_SlotInfo_Insert
        char_node->DBResult(static_cast<const Query_SlotInfo_Insert&>(query_result));
        break;
    case transaction.eTrans_SlotInfo_Update: //S_SlotInfo_Update
        char_node->DBResult(static_cast<const Query_SlotInfo_Update&>(query_result));
        break;
    case transaction.eTrans_Quest_Select: //S_Quest_Select
        success = char_node->DBResult(static_cast<const Query_Quest_Select&>(query_result));
        char_node->EndTransaction(char_node->eTrans_Select_QuestInfo, success);
        char_node->UpdateQuest();
        char_node->ProcessRequestEvent(transaction, success);
        break;
    case transaction.eTrans_Quest_Update: //S_Perk_Update
        char_node->DBResult(static_cast<const Query_Quest_Update&>(query_result));
        break;
    case transaction.eTrans_BattleRecords_Select: //S_BattleRecords_Select
        success = char_node->DBResult(static_cast<const Query_BattleRecords_Select&>(query_result));
        char_node->EndTransaction(char_node->eTrans_Select_BattleRecords, success);
        char_node->UpdateBattleRecords();
        char_node->ProcessRequestEvent(transaction, success);
        break;
    case transaction.eTrans_BattleRecords_Update: //S_BattleRecords_Update
        char_node->DBResult(static_cast<const Query_BattleRecords_Update&>(query_result));
        break;
    case transaction.eTrans_Char_State_Select: //S_Char_State_Select
        success = char_node->DBResult(static_cast<const Query_Char_State_Select&>(query_result));
        char_node->EndTransaction(char_node->eTrans_Select_CharStateInfo, success);
        char_node->UpdateCharStates();
        char_node->ProcessRequestEvent(transaction, success);
        break;
    case transaction.eTrans_Char_State_Insert: //S_Char_State_Insert
        char_node->DBResult(static_cast<const Query_Char_State_Insert&>(query_result));
        break;
    case transaction.eTrans_Char_State_Delete: //S_Char_State_Delete
        char_node->DBResult(static_cast<const Query_Char_State_Delete&>(query_result));
        break;
    case transaction.eTrans_Char_State_Update: //S_Char_State_Update
        char_node->DBResult(static_cast<const Query_Char_State_Update&>(query_result));
        break;
  #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	case transaction.eTrans_Achievement_Select:
		success = char_node->DBResult(static_cast<const Query_Char_Achievement_Select&>(query_result));
		char_node->EndTransaction(char_node->eTrans_Select_Achievement, success);
		char_node->ProcessRequestEvent(transaction, success);
		break;
	case transaction.eTrans_Achievement_Update:
		char_node->DBResult(static_cast<const Query_Char_Achievement_Update&>(query_result));		
		break;
  #endif
    case transaction.eTrans_MissionInfo_Select:
        success = char_node->DBResult(static_cast<const Query_MissionInfo_Select&>(query_result));
        char_node->EndTransaction(char_node->eTrans_Select_MissionInfo, success);
        char_node->UpdateMissionInfo();
        char_node->ProcessRequestEvent(transaction, success);
        break;
    case transaction.eTrans_MissionInfo_Insert:
        char_node->DBResult(static_cast<const Query_MissionInfo_Insert&>(query_result));
        break;
    case transaction.eTrans_MissionInfo_Update:
        char_node->DBResult(static_cast<const Query_MissionInfo_Update&>(query_result));
        break;
    case transaction.eTrans_CharacterExtraInfo_Select:
        {
            success = char_node->DBResult( \
                static_cast<const Query_CharacterExtraInfo_Select&>(query_result));

            // 여기서 ExtraInfo를 EndTransaction하지 않고 MonsterBook을 로딩한다.
            // MonsterBook DBResult에서 EndTransaction을 호출한다.
            char_node->QueryNext(ProxyTransaction::eTrans_CharacterExtraInfo_Select);
        } break;
    case transaction.eTrans_CharacterExtraInfo_Update:
        {
            char_node->DBResult(static_cast<const Query_CharacterExtraInfo_Update&>(query_result));
        } break;
#ifdef _NA_007086_20140318_MONSTERBOOK
    case transaction.eTrans_CharacterMonsterBook_Select:
        char_node->DBResult(static_cast<const Query_CharacterMonsterBook_Select&>(query_result));
        break;
    case transaction.eTrans_CharacterMonsterBook_Update:
        char_node->DBResult(static_cast<const Query_CharacterMonsterBook_Update&>(query_result));
        break;
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    case transaction.eTrans_UserExtraInfo_Select:
        {
            success = char_node->DBResult(static_cast<const Query_UserExtraInfo_Select&>(query_result));
            char_node->EndTransaction(char_node->eTrans_Select_UserExtraInfo, success);
            char_node->UpdateUserExtraInfo();
            char_node->ProcessRequestEvent(transaction, success);
        } break;
    case transaction.eTrans_UserExtraInfo_Update:
        {
            success = char_node->DBResult(static_cast<const Query_UserExtraInfo_Update&>(query_result));
        } break;
#endif // _NA_007932_20150106_USER_EXTRA_INFO
    default:
        success = false;
        break;
    };
    return true;
}

//==================================================================================================
// S_Warehouse_Update
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_User_Update(
    DBUser* db_user, DBQueryProxyUserNode::eTransaction transaction)
{
    const USERGUID user_guid = db_user->GetUserGuid();
    // assert(server_session)

    DBQueryProxyUserNode* const user_node = db_user->db_query_node();
    DBQueryProxyUserNode::Shared* const user_shared = &user_node->shared_;
    assert(user_shared->loaded != false);
    //
    if (transaction & user_node->eTrans_Update_Warehouse)
    {
        if ((user_shared->state & user_node->eState_Loaded_Warehouse) == 0) {
            return eReqResult_Failed;
        }

        if (user_shared->transaction & user_node->eTrans_Update_Warehouse) {
            // (f100603.5L) changes an update routine by a latest data application mechanism
            user_shared->request |= user_node->eReq_Update_Warehouse;
            return eReqResult_Transaction;
        }

        Query_Warehouse_Update* query = Query_Warehouse_Update::ALLOC();
        query->SetUserKey(user_guid);
        db_user->MakeQuery_Warehouse_Update(query);
        //
        db_user->DBQuery(DG_WAREHOUSE, DG_WAREHOUSE_UPDATE_INNER_DBR, query);
        user_node->BeginTransaction(user_node->eTrans_Update_Warehouse);

        return eReqResult_Success;
    }

    return eReqResult_Failed;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Update<Query_Warehouse_Update>(
    const Query_Warehouse_Update& query_result, DBUser* db_user)
{
    __UNUSED(query_result);
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    //DBQueryProxyUserNode::Shared* user_shared = &user_node->shared_;
    user_node->EndTransaction(user_node->eTrans_Update_Warehouse, true);
    return true;
}

// NOTE: this query result is a part of partial update tokens
template<>
bool nsQuery::DBQueryProxyManager::DBResult_User_Update<Query_ItemSlotInfo_Update>(
    const Query_ItemSlotInfo_Update& query_result, DBUser* db_user)
{
    DBQueryProxyUserNode* user_node = db_user->db_query_node();
    //DBQueryProxyUserNode::Shared* user_shared = &user_node->shared_;
    bool success = user_node->DBResult(query_result);
    assert(success);
    return success;
}

//==================================================================================================

nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_Char_Update(
    DBUser* db_user, DBCharacter* db_character,
    DBQueryProxyCharNode::eTransaction transaction)
{
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character->GetCharGuid();
    if (!FlowControl::FCAssert(user_guid == db_character->GetInfo()->m_UserGuid)) {
        return eReqResult_Failed;
    }

    DBQueryProxyUserNode* const user_node = db_user->db_query_node();
    DBQueryProxyUserNode::Shared* const user_shared = &user_node->shared_;
    DBQueryProxyCharNode* const char_node = db_character->db_query_node();
    DBQueryProxyCharNode::Shared* const char_shared = &char_node->shared_;
    // NOTE: f101006.3L, removed useless check anymore
    //assert(char_shared->loaded != false);

    if (transaction & char_node->eTrans_Update_CharInfo)
    {
        if ((user_shared->state & user_node->eState_Loaded_CharInfo) == 0) {
            return eReqResult_Failed;
        }
        if (char_shared->transaction & char_node->eTrans_Update_CharInfo)
        {
            // (f100603.5L) changes an update routine by a latest data application mechanism
            char_shared->request |= char_node->eReq_Update_CharInfo;
            return eReqResult_Transaction;
        };

        Query_Char_Update* query = Query_Char_Update::ALLOC();
        db_character->MakeQuery_Char_Update(query);
        query->SetUserKey(user_guid);
        query->set_char_guid(char_guid);
        char_node->BeginTransaction(char_node->eTrans_Update_CharInfo);
        db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_DBR, query);
        //

        return eReqResult_Success;
    };
    //
    if (transaction & char_node->eTrans_Update_InvenInfo)
    {
        if ((user_shared->state & user_node->eState_Loaded_InvenInfo) == 0) {
            return eReqResult_Failed;
        }
        if (char_shared->transaction & char_node->eTrans_Update_InvenInfo)
        {
            // (f100603.5L) changes an update routine by a latest data application mechanism
            char_shared->request |= char_node->eReq_Update_InvenInfo;
            return eReqResult_Transaction;
        };

        Query_Char_Update_Inventory* query = Query_Char_Update_Inventory::ALLOC();
        db_character->MakeQuery_Char_Update_Inventory(query);
        query->SetUserKey(user_guid);
        query->set_char_guid(char_guid);
        char_node->BeginTransaction(char_node->eTrans_Update_InvenInfo);
        db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_INVEN_DBR, query);
        //

        return eReqResult_Success;
    };
    return eReqResult_Failed;
}

// if can't find in a DBUserManager, should be updated the db user of the pending list
nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::DBResult_Char_Update(
    USERGUID user_guid, CHARGUID char_guid,
    DBQueryProxyCharNode::eTransaction transaction)
{
    LifetimeTable::iterator it = lifetime_table_.find(user_guid);
    if (it == lifetime_table_.end()) {
        return this->eReqResult_Failed;
    }
    DBUser* const db_user = it->second;
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return this->eReqResult_Failed;
    }
    DBCharacter* db_character = db_user->FindCharacter(char_guid);
    if (db_character == NULL)
    {
        return this->eReqResult_Failed;
    }
    DBQueryProxyCharNode* db_query_char_node = db_character->db_query_node();
    DBQueryProxyCharNode::Shared* db_query_shared = &db_query_char_node->shared_;
    db_query_char_node->EndTransaction(transaction, false);
    if (db_query_shared->transaction != db_query_char_node->eTrans_None) {
        return this->eReqResult_Success;
    }
    // regard shared_.request ... how to do it?
    //db_query_shared->
    return this->eReqResult_Success;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_Char_Update<Query_Char_Update>(
    const Query_Char_Update& query_result, DBUser* db_user)
{
    DBCharacter* db_character = db_user->FindCharacter(query_result.char_guid());
    if (!FlowControl::FCAssert(db_character != NULL)) {
        return false;
    }
    DBQueryProxyCharNode* db_query_char_node = db_character->db_query_node();
    db_query_char_node->EndTransaction(db_query_char_node->eTrans_Update_CharInfo, true);
    return true;
}

template<>
bool nsQuery::DBQueryProxyManager::DBResult_Char_Update<Query_Char_Update_Inventory>(
    const Query_Char_Update_Inventory& query_result, DBUser* db_user)
{
    DBCharacter* db_character = db_user->FindCharacter(query_result.char_guid());
    if (!FlowControl::FCAssert(db_character != NULL)) {
        return false;
    }
    DBQueryProxyCharNode* db_query_char_node = db_character->db_query_node();
    db_query_char_node->EndTransaction(db_query_char_node->eTrans_Update_InvenInfo, true);
    return true;
}

nsQuery::DBQueryProxyManager::eRequestResult
nsQuery::DBQueryProxyManager::Request_Char_Create(
    const Query_Char_Create& query_result, DBUser* db_user)
{
    __UNUSED(query_result);
    DBQueryProxyUserNode* const query_node = db_user->db_query_node();
    DBQueryProxyUserNode::Shared* const query_shared = &query_node->shared_;
    assert(query_shared->loaded != false);

    if (query_shared->transaction & query_node->eTrans_Create_Character) {
        return eReqResult_Transaction;
    };
    query_node->BeginTransaction(query_node->eTrans_Create_Character);
    return eReqResult_Success;
}

bool
nsQuery::DBQueryProxyManager::DBResult_Char_Create(
    const Query_Char_Create& query_result, DBUser* db_user, RC::eCHAR_CREATE_RESULT rc)
{
    // CHANGES: f101222.1L, fixed a missing transaction when a character create query is failed.
    const bool success = query_result.IsError() == false &&
                         const_cast<Query_Char_Create&>(query_result).GetResult() == 0;
    //
    DBQueryProxyUserNode* const user_node = db_user->db_query_node();
    user_node->EndTransaction(user_node->eTrans_Create_Character, success);
    //
    if (success && (rc == RC::RC_CHAR_CREATE_SUCCESS))
    {
        const Query_Char_Create::sQUERY_RESULT* player_info = query_result.pResult;
        DBCharacter* db_character = db_user->FindCharacter(player_info->m_CharGuid);
        if (!FlowControl::FCAssert(db_character != NULL)) {
            return false;
        }

        DBQueryProxyCharNode* const char_node = db_character->db_query_node();
        char_node->OnCreateCharacter();
    }
    return success;
}

//==================================================================================================

