#pragma once
#ifndef DBPROXY_DBQUERY_PROXY_MANAGER_H
#define DBPROXY_DBQUERY_PROXY_MANAGER_H

#include "./DBQueryProxyNode.h"

class QueryForUser;
class QueryForCharTransaction;
class Query_User_Select;
class Query_User_Select_Inventory;
class Query_User_Select_PetList;
//
class Query_Warehouse_Select;

class Query_Char_Update;
class Query_Char_Update_Inventory;

namespace nsQuery {

class DBQueryProxyManager;

}; //end of namespace

//==================================================================================================
// @history
//  |10.04.15|waverix|created, 
//                   |to support reliable user data db synchronization with life-time management

namespace nsQuery {
;

//==================================================================================================

class DBQueryProxyManager
{
public:
    enum eRequestResult {
        eReqResult_Success      = 0,
        eReqResult_Failed       , //unknown
        eReqResult_Transaction  , //transaction locked
        eReqResult_Loaded       , //already loaded
    };
    enum eConnectResult {
        eConnectResult_Error = 0,
        eConnectResult_Create,
        eConnectResult_Reuse,
        eConnectResult_PendingReuse,
        eConnectResult_Pending,
    };
    enum eEnterGameResult {
        eEnterGameResult_Fail = 0,
        eEnterGameResult_Enterable,
        eEnterGameResult_Pending,
    };
    // NOTE: f101020.5L
    enum eFindResult {
        eFindResult_NotFound = 0,
        eFindResult_Service,
        eFindResult_Pending,
    };

    static void CreateInstance();
    static void DestroyInstance();
    static DBQueryProxyManager* Instance();
    int GetPendingUsers() const;
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
    eConnectResult EnterConnectPendingProcess(DBUser** reuse_db_user,
                                              const MSG_AD_CHARINFO_CHARLISTREQ_SYN* recv_msg,
                                              ChildServerSession* server_session);
    eEnterGameResult EnterGamePendingProcess(DBUser* db_user, DBCharacter* db_character,
                                             ChildServerSession* server_session);
    // NOTE: f101020.5L, this method regard for pending users to support a find user function
    // in other words, find in DBUserManager first, and find in PendingUser second
    eFindResult FindUserWithPendings(USERGUID user_guid, DBUser** find_db_user) const;
    // NOTE: f101020.5L, this method regard for pending users to support a find user function
    // in other words, find in DBUserManager first, and find in PendingUser second
    eFindResult FindUserWithPendings(USERGUID user_guid, CHARGUID char_guid,
                                     DBUser** find_db_user, DBCharacter** find_db_character) const;
    // if the result is null that can't find user in the pending list, \
    // do create DBUser related to the user guid.
    // if the result is not null that find user in the pending list \
    // which is synchronized latest data, delete in the pending list implicit and can use the object
    DBUser* MoveUserFromPendings(USERGUID user_guid);
    bool MoveUserToPendings(DBUser* db_user);
    // call MoveUserToPendings after the db user extract from the table of the DBUserManager
    bool DisconnectUser(DBUser* db_user, bool extract_from_user_table);
    //bool ChangePendingControl(DBUser* db_user, bool enabled,
    //                          DBQueryProxyUserNode::eRequest transaction);
    void Update();
    //
    bool SendUserInfoToAgentOnAllCharacterLoaded(DBUser* db_user);
    // (f100731.5L) replace the 'SendAllDataToGameServer' function
    // to support a selected character's data control.
    bool SendCharInfoToGameOnAllCharDataLoaded(DBUser* db_user, DBCharacter* db_character);
    //
    //----------------------------------------------------------------------------------------------
    // S_User_Select + S_User_Select_Inventory + S_CharacterState_Select
    // S_Warehouse_Select
    // (f100729.3L) added to support all loaded sequence process
    // (WARNING) no rule check routine
    eRequestResult Request_User_Select_All(DBUser* db_user);
    // f101004.4L, added to support partial equipment queries after split T_Inventory query
    eRequestResult Request_Char_Data_Equipment(DBUser* db_user, DBCharacter* db_character);
    // (f100731.2L) added to support all loaded sequence process for character
    // (WARNING) no rule check routine
    eRequestResult Request_Char_Data_All(DBUser* db_user, DBCharacter* db_character);
    eRequestResult Request_User_Select(DBUser* db_user,
                                       DBQueryProxyUserNode::eTransaction transaction);
    // call it if can't find in a DBUserManager, should be updated the db user of the pending list
    eRequestResult DBResult_User_Select(USERGUID user_guid, const QueryForUser& result_base,
                                        DBQueryProxyUserNode::eTransaction transaction);
    // NOTE: r100928.3L
    // CHANGES: f101220.5L, change to specific template function to easy get funtion debugging point
    // WARNING: will be occurred delayed link-time error if this function is not implement
    template<class QueryObject_>
    bool DBResult_User_Select(const QueryObject_& query_result, DBUser* db_user);
    // CHANGES: f101220.5L, change to specific template function to easy get funtion debugging point
    // WARNING: will be occurred delayed link-time error if this function is not implement
    template<class QueryObject_>
    bool DBResult_User_Update(const QueryObject_& query_result, DBUser* db_user);

    //----------------------------------------------------------------------------------------------
    // S_Warehouse_Update
    eRequestResult Request_User_Update(DBUser* db_user,
                                       DBQueryProxyUserNode::eTransaction transaction);
    //----------------------------------------------------------------------------------------------
    eRequestResult Request_Char_Create(const Query_Char_Create& query_result, DBUser* db_user);
    bool DBResult_Char_Create(const Query_Char_Create& query_result, DBUser* db_user, RC::eCHAR_CREATE_RESULT rc);
    //----------------------------------------------------------------------------------------------
    eRequestResult Request_Char_Transaction(DBUser* db_user, DBCharacter* db_character,
                                            ProxyCommand::eCommand transaction);
    eRequestResult DBResult_Char_Transaction(USERGUID user_guid, CHARGUID char_guid,
                                             const QueryForCharTransaction& query_result);
    bool DBResult_Char_Transaction(const QueryForCharTransaction& query_result,
                                   DBUser* db_user, DBCharacter* db_character);
    //----------------------------------------------------------------------------------------------
    // S_Char_Update + S_Char_Update_Inventory + S_CharacterState_Update
    eRequestResult Request_Char_Update(DBUser* db_user, DBCharacter* db_character,
                                       DBQueryProxyCharNode::eTransaction transaction);
    // call it if can't find in a DBUserManager, should be updated the db user of the pending list
    eRequestResult DBResult_Char_Update(USERGUID user_guid, CHARGUID char_guid,
                                        DBQueryProxyCharNode::eTransaction transaction);
    // CHANGES: f101220.5L, change to specific template function to easy get funtion debugging point
    // WARNING: will be occurred delayed link-time error if this function is not implement
    template<class QueryObject_>
    bool DBResult_Char_Update(const QueryObject_& query_result, DBUser* db_user);

private:
    typedef STLX_MAP<USERGUID, DBUser*> LifetimeTable;

    DBQueryProxyManager();
    ~DBQueryProxyManager();

    bool SendPendingConnectionResult(DBUser* db_user);
    bool UpdateQueryTransaction(DBUser* db_user);

    // fields ...
    LifetimeTable lifetime_table_;
    //
    static DBQueryProxyManager* db_query_proxy_manager_s;
};

}; //end of namespace 'nsQuery'

//==================================================================================================
// implementation

namespace nsQuery {
;

inline DBQueryProxyManager* DBQueryProxyManager::Instance()
{
    return db_query_proxy_manager_s;
}

inline int DBQueryProxyManager::GetPendingUsers() const
{
    return static_cast<int>(lifetime_table_.size());
}

}; //end of namespace 'nsQuery'

//==================================================================================================

#endif //DBPROXY_DBQUERY_PROXY_MANAGER_H