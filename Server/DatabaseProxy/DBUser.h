#pragma once
#ifndef DBPROXY_DBUSER_H
#define DBPROXY_DBUSER_H

//==================================================================================================
/// DBUser class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >	
    @since
        2004. 8. 11~
    @history
        - 2006/2/13 : 리팩토링 SCStream으로 공통화 작업
        DB_SI_로 시작하는 Enum추가
        SI_XXX -> DB_SI_로 바꿔주는 inline함수(SI2StreamPtr()) 추가
        Warehouse Stream데이터를 계정당 하나로 공유하도록 수정
        - 2010/4/16 : waverix|fit in the coding style
*/
//==================================================================================================

#include <Timer.h>
#include <SCStream.h>

#include "QueryObjects/StructInQuery.h"
#include "QueryObjects/GameServerQuery.h"
#include "QueryObjects/AgentServerQuery.h"
#include "QueryObjects/DatabaseProxyQuery.h"
#include "QueryObjects/WorldServerQuery.h"

#include "DBUserManager.h"
#include "DBCharacter.h"
#include "ProxyNodes/DBQueryProxyNode.h"

struct BASE_PLAYERINFO;
struct SERVER_CHARACTER_PART;
class ChildServerSession;

//==================================================================================================
//  (WAVERIX)(NOTE)
//  (MARK #01)
//      eDB_STATE_QUERYING 속성을 특수한 형태로 전환,
//      -> eDB_STATE_UPDATE_TRANS
//      상태전환시점 --> GAME_SERVER 에서 UPDATE_ITEM과 같은 정보 업데이트 중일 경우의 상태
//                      다른 상태로의 전환되는 로직이 임시 블록될 수 있도록 유지한다. 대기열이던 뭐든간에...
//      기대 효과 --> AGENT에서 등등에서 접속 종료 명령이 오더라도 게임서버가 DESTROYED까지 DB상태 저장을 안정되게
//                      수행될 수 있도록 하는 방어 메커니즘으로 사용한다.
//  (WAVERIX)(NOTE)
//      GameServer/Player::SerializeInfoToDBProxy와 같은 것이 호출 될 경우,
//      GameServer/Player::SerializeInfoToDBProxy호출 이후 Disconnect

//==================================================================================================

enum eDB_UPDATE_TYPE
{
    eDB_UPDATE_INNER,              // DBUser Update시 주기적으로 호출
    eDB_UPDATE_SYN,                // 존 이동 시(LeaveUser) 호출
    eDB_UPDATE_DISCONNECT_CMD,     // 완전히 접속을 종료할 때 호출
    eDB_UPDATE_GUILDWAREHOUSE_SYN, //길드 창고 변경에 의해 호출
};

enum eDB_STATE
{
    eDB_STATE_NONE,         // 사용하지 않고 Pool에 있는 상태
    eDB_STATE_CREATING,     // DB로부터 User 정보를 받는 상태
    eDB_STATE_CREATED,      // DB로부터 User 정보를 받은 상태(평소 상태)
    eDB_STATE_DESTROY_WAIT, // Destroy를 위해 다른 처리가 끝나기를 기다리는 상태
    eDB_STATE_MAX,
};

//==================================================================================================

class DBUser
{
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
public:
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    typedef STLX_HASH_MAP<CHARGUID, DBCharacter*> CHAR_HASH;
public:
    DBUser(void);
    ~DBUser(void);

    void Init();
    void Release();
    bool Update(BOOL is_by_warehouse = false);
    bool UpdateForTimeOut();

    bool UpdateChar(eDB_UPDATE_TYPE db_update_type, int channel_id, DBCharacter* activated_char);
    void Disconnect(BYTE channel_id);
    void RequestDisconnect(int channel_id);
    bool CanLogin() const;
    bool CanDBUpdate() const;
    // NOTE: f101126.2L, add query handler
    bool DBQuery(BYTE category, BYTE protocol, Query* query);

    void UpdateGroupAccumEXP();

    void  SetDBState(eDB_STATE db_state);
    DWORD GetDBState() const;

    bool  IsWarehouseLoaded() const;
    void  SetNeedWarehouseToSave(BOOL v);
    MONEY GetWarehouseMoney() const;
    void  SetWarehouseMoney(MONEY money);

    const char* GetWarehousePassword() const;
    void  SetWarehousePassword(const char password[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH]);

    void SetWorldType(ITEMPART& OUT rPart);

    void SendAllDataToGameServer();
    void MakeQuery_Warehouse_Update(Query_Warehouse_Update* query);
    // 캐릭터 접속/해제시 콜백
    DBCharacter* FindCharacter(const CHARGUID char_guid) const;
    DWORD GetUserKey() const;
    void  SetUserKey(DWORD user_key);
    // 연결된 Agent의 ServerSessionIndex
    ChildServerSession* GetServerSession() const;
    void SetServerSession(ChildServerSession* server_session);
    USERGUID GetUserGuid() const;
    void SetUserGuid(USERGUID char_guid);
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN      GetGameChuUSN() const;
    VOID            SetGameChuUSN(GAMECHUUSN gamechu_usn);
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY         GetBetaKey() const;
    VOID            SetBetakey(BETAKEY betakey);
#endif
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE GetCharSlotSize() const;
    void SetCharSlotSize(const BYTE char_slot_size);
#endif

    BYTE GetChannelID() const;
    void SetChannelID(BYTE channel_id);

    DBCharacter* GetConnectedChar() const
    {
        return FindCharacter(conn_char_guid_);
    }
    void SetConnectedChar(const DBCharacter* const db_char);

    bool SendPacketToGameServer(MSG_BASE_FORWARD* pMsg, WORD wSize);
    DBCharacter* OnAddChar(const sQUERY_CHARACTER* queried_char_info);
    bool OnDelChar(CHARGUID char_guid);
    DBCharacter* OnRecoverChar(CHARGUID char_guid);

    bool OnConnectChar(const CHARGUID char_guid);
    bool OnDisconnectChar();

    void Clear();

    bool OnSetWarehouseBaseInfo(const Query_Warehouse_Select& query);

    int GetNumberOfChars() const;
    const CHAR_HASH& GetCharHash() const;

    void SetIsPCRoom(BOOL v);
    bool IsPCRoom() const;

    DWORD& WarehouseFlagStream();
    enum ePWD_TRANSACTION { ePWD_NO_TRANS, ePWD_TRANS_WAREHOUSE, ePWD_TRANS_INVENTORY };
    ePWD_TRANSACTION& PasswordTransaction();
    sPASSWORD_BLOCK_IN_DBPROXY* GetTempWarehousePassword() const;
    void SetTempWarehousePassword(sPASSWORD_BLOCK_IN_DBPROXY* temp_password_block);
    // NOTE: f110426.2L
    uint8_t GetChannelTransactionLevel() const;
    void EnterChannelTransaction();
    void IncreaseChannelTransactionPhase();
    void LeaveChannelTransaction();
    // NOTE: f110429.4L, a dangling user occur possibility
    // when he moves a field server to a battle server
    void UpdateLatestUpdateTick();
    void MakeToExpiredLatestUpdateTick();
    bool IsExpiredDanglingTimeout() const;
    //
    void EnterTransactionSerialize();
    void LeaveTransactionSerialize();
    bool IsTransactionSerialize() const;

    void EnterTransactionDBUpdate();
    void LeaveTransactionDBUpdate();
    bool IsTransactionDBUpdate() const;
    bool IsReleased() const
    {
        return is_released_;
    }

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    void   SetPlayTime(WORD wPlayTime);
    WORD   GetPlayTime() const;
    void   SetOfflineTime(WORD wOfflineTime);
    WORD   GetOfflineTime() const;
    void   SetLoginTime(time_t LoginTime);
    time_t GetLoginTime() const;
    void   SetFatigueUser(BOOL bFatigueUser);
    bool   IsFatigueUser() const;
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
    nsQuery::DBQueryProxyUserNode* db_query_node();
    const nsQuery::DBQueryProxyUserNode* db_query_node() const;
    DWORD db_unit_id() const;
//--------------------------------------------------------------------------------------------------
private:
    enum 
    { 
        MAX_CACHE_TIMEOUT_DELAY_TIME = 1*1000*60,
        MAX_DESTROY_WAIT_TIME = 1*1000, 
        MAX_TRANSACTION_WAIT_TIME = 1*1000*60,  
    };
    //
    bool UpdateWarehouse();
    bool NeedWarehouseToSave() const;
    void SetWarehouseLoaded(BOOL v);
    int  GetWarehouseVersion() const;
    void ReleaseChar(DBCharacter*& db_character) const;
    
    bool is_released_;

    // data fields
    //----------------------------------------------------------------------------------------------
    // NOTE: f101008.2L, the below block is processed by operation of fill zero memory.
    // WARNING: should be don't declare non-pod fields in this ranges.
    char pod_field_start_[1];
    //
    // 유저 정보
    eDB_STATE db_state_;
    USERGUID  user_guid_;
    DWORD     user_key_;
    BYTE      channel_id_;
    bool      is_pc_room_; // 0: 일반 유저 1: PC방 유저
    // CHANGES: f110426.2L, the db user anymore disable activating migration like a disconnect process
    // to the pending list. so that the db user in DBUserManager::Update manipulate using lifetime cycle
    uint8_t channel_lock_;
    DWORD     db_unit_id_;
    ChildServerSession* server_session_;
    CHARGUID conn_char_guid_; // 선택된 캐릭터 Guid
    //
    // 중복 업데이트, 연결 종료 동기화 보정용
    ulong latest_update_timeout_; // f110429.4L
    bool serialize_transaction_; // Player::SerializeInfoToDBProxy()
    bool db_update_transaction_; // DG_CHARINFO_UPDATE_DBR

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN gamechu_usn_; 
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BETAKEY betakey_;
#endif
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE char_slot_size_;
#endif
    // 창고 관련
    int       warehouse_version_;
    SCStream* warehouse_stream_;
    bool      warehouse_loaded_;
    bool  need_to_save_;
    MONEY warehouse_money_;
    DWORD warehouse_flag_stream_;
    ePWD_TRANSACTION password_transaction_;
    sPASSWORD_BLOCK_IN_DBPROXY* temp_password_;
    char warehouse_password_[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH];
public:
    sSSN_ENCODED_STREAM SSN; // property
private:
    // 중국 피로도 저장 단위. __CN_OO0510__FATIGUE_MATCHING_UPDATE
    // 피로도 관련
    WORD play_time_;
    WORD offline_time_;
    time_t login_time_; //현 계정의 로그인 시간.
    bool is_fatigue_user_;
    //
    char pod_field_end_[1];
    //----------------------------------------------------------------------------------------------
#pragma pack(push) // NOTE: default value is 8, but it support compiler hint to fit alignment
#pragma pack()
    CHAR_HASH character_list_hash_;
    nsQuery::DBQueryProxyUserNode db_query_node_;
#pragma pack(pop)
    util::Timer destroy_wait_timer_;
    util::Timer transaction_wait_timer_; // Transaction이 계속 안 풀리면 문제가 있는 것으로 간주한다.
    //
    friend class nsQuery::DBQueryProxyManager;
};

//==================================================================================================

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
inline GAMECHUUSN DBUser::GetGameChuUSN() const {
    return gamechu_usn_;
};

inline VOID DBUser::SetGameChuUSN(GAMECHUUSN gamechu_usn) {
    gamechu_usn_ = gamechu_usn;
};
#endif

#ifdef _NA_20100307_BETAKEY_SYSTEM
inline BETAKEY DBUser::GetBetaKey() const {
    return betakey_;
};

inline VOID DBUser::SetBetakey(BETAKEY betakey) {
    betakey_ = betakey;
};
#endif

#ifdef _NA_002050_20110216_ADD_GENDER
inline BYTE DBUser::GetCharSlotSize() const
{
    return char_slot_size_;
}

inline void DBUser::SetCharSlotSize(const BYTE char_slot_size)
{
    char_slot_size_ = char_slot_size;
}
#endif

inline void  DBUser::SetDBState(eDB_STATE db_state) {
    db_state_ = db_state;
}

inline DWORD DBUser::GetDBState() const {
    return db_state_;
}

inline bool DBUser::IsWarehouseLoaded() const
{   // r100928.4L
    int state = db_query_node_.state();
    return (state & db_query_node_.eState_Loaded_Warehouse) != 0 &&
           (state & db_query_node_.eState_Loaded_WareItems) != 0;
}

inline bool DBUser::NeedWarehouseToSave() const {
    return need_to_save_;
}

inline void DBUser::SetWarehouseLoaded(BOOL v) {
    warehouse_loaded_ = v != false;
}

inline int  DBUser::GetWarehouseVersion() const {
    return warehouse_version_;
}

inline void  DBUser::SetNeedWarehouseToSave(BOOL v) {
    need_to_save_ = v != false;
}

inline MONEY DBUser::GetWarehouseMoney() const {
    return warehouse_money_;
}

inline void  DBUser::SetWarehouseMoney(MONEY money) {
    warehouse_money_ = money;
}

inline const char* DBUser::GetWarehousePassword() const {
    return warehouse_password_;
}

inline void  DBUser::SetWarehousePassword(const char password[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH])
{   // not null string
    strncpy(warehouse_password_, password, _countof(warehouse_password_));
}

inline DWORD DBUser::GetUserKey() const {
    return user_key_;
}

inline void  DBUser::SetUserKey(DWORD user_key) {
    user_key_ = user_key;
}

// 연결된 Agent의 ServerSessionIndex
inline ChildServerSession* DBUser::GetServerSession() const {
    return server_session_;
}

inline USERGUID DBUser::GetUserGuid() const {
    return user_guid_;
}

inline void DBUser::SetUserGuid(USERGUID char_guid) {
    user_guid_ = char_guid;
}

inline BYTE DBUser::GetChannelID() const {
    return channel_id_;
}

inline void DBUser::SetChannelID(BYTE channel_id) {
    channel_id_ = channel_id;
}

inline int DBUser::GetNumberOfChars() const {
    return (int)character_list_hash_.size();
}

inline const DBUser::CHAR_HASH& DBUser::GetCharHash() const {
    return character_list_hash_;
}

inline void DBUser::SetIsPCRoom(BOOL v) {
    is_pc_room_ = v != false;
}

inline bool DBUser::IsPCRoom() const {
    return is_pc_room_;
}

inline DWORD& DBUser::WarehouseFlagStream() {
    return warehouse_flag_stream_;
}

inline DBUser::ePWD_TRANSACTION& DBUser::PasswordTransaction() {
    return password_transaction_;
}

inline sPASSWORD_BLOCK_IN_DBPROXY* DBUser::GetTempWarehousePassword() const {
    return temp_password_;
}

inline void DBUser::SetTempWarehousePassword(sPASSWORD_BLOCK_IN_DBPROXY* pTempPwdBlock) {
    temp_password_ = pTempPwdBlock;
}

inline uint8_t DBUser::GetChannelTransactionLevel() const {
    return channel_lock_;
}

inline void DBUser::EnterChannelTransaction() {
    channel_lock_ = 1;
}

inline void DBUser::LeaveChannelTransaction() {
    channel_lock_ = 0;
}

inline void DBUser::IncreaseChannelTransactionPhase() {
    ++channel_lock_;
};

// NOTE: f110429.4L, a dangling user occur possibility
// when he moves a field server to a battle server
inline void DBUser::UpdateLatestUpdateTick() {
    latest_update_timeout_ = ::GetTickCount() + (8 * 60 * 1000); // 8 minutes
};

inline void DBUser::MakeToExpiredLatestUpdateTick() {
    latest_update_timeout_ = 0;
};

inline bool DBUser::IsExpiredDanglingTimeout() const
{
    // CHANGES: f110622.2L, removed the depth lock check.
    // details of this reason, see the reference 'f110622.2L'
#if !defined(_DEBUG)
    ulong current_tick = ::GetTickCount();
    if (current_tick > latest_update_timeout_) {
        return true;
    };
#endif
    return false;
};

inline void DBUser::EnterTransactionSerialize() {
    serialize_transaction_ = true;
}

inline void DBUser::LeaveTransactionSerialize() {
    serialize_transaction_ = false;
}
inline bool DBUser::IsTransactionSerialize() const {
    return serialize_transaction_;
}

inline void DBUser::EnterTransactionDBUpdate()  {
    db_update_transaction_ = true;
}

inline void DBUser::LeaveTransactionDBUpdate()  {
    db_update_transaction_ = false;
}

inline bool DBUser::IsTransactionDBUpdate() const {
    return db_update_transaction_;
}

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
inline void   DBUser::SetPlayTime(WORD wPlayTime) {
    play_time_ = wPlayTime;
}

inline WORD   DBUser::GetPlayTime() const {
    return play_time_;
}

inline void   DBUser::SetOfflineTime(WORD wOfflineTime) {
    offline_time_ = wOfflineTime;
}

inline WORD   DBUser::GetOfflineTime() const {
    return offline_time_;
}

inline void   DBUser::SetLoginTime(time_t LoginTime) {
    login_time_ = LoginTime;
}

inline time_t DBUser::GetLoginTime() const {
    return login_time_;
}

inline void   DBUser::SetFatigueUser(BOOL bFatigueUser) {
    is_fatigue_user_ = bFatigueUser;
}

inline bool   DBUser::IsFatigueUser() const {
    return is_fatigue_user_;
} 
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE\

inline nsQuery::DBQueryProxyUserNode*
DBUser::db_query_node()
{
    return &db_query_node_;
}

inline const nsQuery::DBQueryProxyUserNode*
DBUser::db_query_node() const
{
    return &db_query_node_;
}

inline DWORD DBUser::db_unit_id() const {
    return db_unit_id_;
};

namespace impl {

bool IsValidCharacter(const DBCharacter* const db_character, const char* func_str, const int line_number);

} // impl

#endif //DBPROXY_DBUSER_H
