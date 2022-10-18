#pragma once
#ifndef DBPROXY_SERVER_EX_H
#define DBPROXY_SERVER_EX_H

//==================================================================================================
/// GameServerEx class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 10. 23
    @remark
    - 프레임워크를 상속받아 구현하는 것으로 코드 변경
    @note
    - 
    @history 
    - 
*/
//==================================================================================================

#include <ServerFrame.h>

#include "TempServerSession.h" // ServerCommon
#include "Sessions/GameServerSession.h"
#include "Sessions/MasterServerSession.h"
#include "Sessions/AgentServerSession.h"
#include "Sessions/WorldServerSession.h"
#include "Sessions/GuildServerSession.h"
#include "Sessions/PortalServerSession.h"
#include "Sessions/SolarAuthServerSession.h"

class DBUser;
class ScriptParserManager;

//==================================================================================================

class DBProxyServerEx : public ServerFrame
{
public:
    // db result handler index containing in DBProxyServerEx
    enum QUERY_INDEX 
    { 
        QUERY_SELECTDBSERIAL = 0,
        //QUERY_UPDATECHARACTER,
        //QUERY_UPDATEWAREHOUSE,
        QUERY_MATCHCHAR_UPDATE,
        QUERY_MATCHCHAR_DELETE,
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        QUERY_MATCHFATIGUE_UPDATE,
        QUERY_MATCHFATIGUE_SELECT,
#endif
        QUERY_TEST_INSERT,
        QUERY_FATE_ITEM_IDENTIFY_DBR,
    };

    //----------------------------------------------------------------------------------------------
    // Network Session Alloc/DeAlloc Handlers
    static NetworkObject* DoAllocServerSession();
    static void DoFreeServerSession(NetworkObject* network_object);
    static void DoFreeConnectSession(NetworkObject* network_object) { __UNUSED(network_object); }

    static NetworkObject* DoAllocClient_NotUsed() { return NULL; }
    static void DoFreeClient_NotUsed(NetworkObject* network_object) { __UNUSED(network_object); }
    static void DoFreeConnect_NotUsed(NetworkObject* network_object) { __UNUSED(network_object); }

    //----------------------------------------------------------------------------------------------
    DBProxyServerEx();
    virtual ~DBProxyServerEx();
    // ServerFrame interface
#if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT)
    virtual eSERVER_TYPE GetServerType() { return ACCOUNT_DBPROXY; }
#else
    virtual eSERVER_TYPE GetServerType() { return GAME_DBPROXY; }
#endif
    //virtual void SendServerType(ServerSession* session);
    //----------------------------------------------------------------------------------------------
    static void StartServer();
    ePHASE_SHIFT GetPhaseStatus() const;
    void SetPhaseStatus(ePHASE_SHIFT phase);
    // requested shutdown from master server
    void RequestShutdown();
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    SolarDatabaseInDBP* Get1stDatabase(); // { SUNOnline, }
    SolarDatabaseInDBP* Get2ndDatabase(); // { MatchingDB, }
    SolarDatabaseInDBP* Get3rdDatabase(); // { (spared for batch units...) }
#endif

    bool SendMaster(MSG_BASE* msg, uint16_t msg_size);
    bool IsMasterConnected() const;
    // NOTE: f110522.3L
    bool SendToWorldServer(MSG_BASE* msg, uint16_t msg_size);
    bool IsWorldConnected() const;
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    void SendToDBPServer(MSG_BASE* pMsg, WORD wSize);
    void SendToGuildServer(MSG_BASE* pMsg, WORD wSize);
#endif //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION

    bool ConnectTo(eSERVER_TYPE server_type);
#if SUN_CODE_BACKUP
    ServerSession* GetSession(eSERVER_TYPE server_type);
#endif
    //----------------------------------------------------------------------------------------------
    // DB 관련
    void DBResult(DWORD dwIndex, Query* pData);  // <- CHECK110519
    BYTE GetItemWorldID() const;
    //----------------------------------------------------------------------------------------------
    // DB Connections
    bool IsDBDisconnected() const;
    bool IsMatchDBDisconnected() const;

    DWORD& DBLastedConnectedTick() { return latest_main_db_connected_ticks_; }  // <- CHECK110519
    DWORD& MatchDBLastedConnectedTick() { return latest_match_db_connected_ticks_; }  // <- CHECK110519
    bool SetDBDisconnected(bool disconnected, DWORD query_start_tick = 0);
    bool SetMatchDBDisconnected(bool disconnected, DWORD query_start_tick = 0);
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    // 매칭 DB관련 쿼리 함수
    void ExecuteQuery_MatchChar_Update(USERGUID user_guid, const TCHAR* char_name,
                                       LEVELTYPE Level, BYTE class_type);
    void ExecuteQuery_MatchChar_Delete(USERGUID user_guid, const TCHAR* char_name);
    //피로도 관련 쿼리 함수
    void ExecuteQuery_MatchFatigue_Update(USERGUID user_guid, DWORD user_key, int play_time);
    void ExecuteQuery_MatchFatigue_Select(USERGUID user_guid, DWORD user_key);
#endif  //__CN_OO0510__FATIGUE_MATCHING_UPDATE

    // TODO: f110519.7L, need changes
    sPASSWORD_BLOCK_IN_DBPROXY* AllocTempPasswordBlock() { return (sPASSWORD_BLOCK_IN_DBPROXY*)password_pool_.Alloc(); }
    BOOL FreeTempPasswordBlock(sPASSWORD_BLOCK_IN_DBPROXY* pTempPwdBlock) { return password_pool_.Free(pTempPwdBlock); }
protected:
    //----------------------------------------------------------------------------------------------
    // ServerFrame interfaces
    virtual BOOL Init();
    virtual BOOL Update(DWORD delta_tick);
    virtual void Release();
    virtual ServerSession* AllocServerSession(eSERVER_TYPE);
    virtual void FreeServerSession(ServerSession*);
    virtual BOOL KeyEvent(char inputed_char);
    //----------------------------------------------------------------------------------------------
private:
    enum eSHUTDOWNSTATE { RUNNING, READYTOSHUTDOWN, };
    //
    void SetShutdownState(eSHUTDOWNSTATE state);
    //
    bool InitializeScriptData(BOOL read_from_file);
    bool InitializeLogger(const char* server_name);
    bool CreateSunLog(BYTE sun_log_option, BYTE sun_log_level,
                      const TCHAR* log_path, const TCHAR* file_name);
    bool InitializeSessionPool();
    bool InitializeDatabase();
    //
    bool UpdateDB();
    //
    void ReloadAttributes();
    void DisplayServerInfo();
    //----------------------------------------------------------------------------------------------
    // data fields...
    ePHASE_SHIFT phase_shift_;
    MasterServerSession* master_server_session_;
    WorldServerSession* world_server_session_; // NOTE: f110522.3L
    BYTE item_world_id_;
    struct {
        long  disconnected_main_db_; // mutual exclusion
        long  disconnected_match_db_; // mutual exclusion
        DWORD latest_main_db_connected_ticks_;  // 가장 최신 연결 완료 시점.
        DWORD latest_match_db_connected_ticks_; // 가장 최신 연결 완료 시점.
    };
    //----------------------------------------------------------------------------------------------
    SolarDatabaseInDBP main_database_;
#if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT) || \
    defined(__CN_OO0510__FATIGUE_MATCHING_UPDATE)
    SolarDatabaseInDBP match_database_;
#endif
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    SolarDatabaseInDBP extra_database_;
#endif
    //----------------------------------------------------------------------------------------------
    struct {
        bool shutdown_requested_;    // 마스터 서버로부터 종료 요청 받은 경우 true
        eSHUTDOWNSTATE shutdown_state_;
    };

    ScriptParserManager* script_parser_manager_;
    TEMP_PWD_POOL password_pool_; // TODO: waverix, need renewal

    //----------------------------------------------------------------------------------------------
    util::CMemoryPoolFactory<AgentServerSession>  m_AgentServerSessionPool;
    util::CMemoryPoolFactory<GameServerSession>   m_GameServerSessionPool;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    util::CMemoryPoolFactory<GuildServerSession>  m_GuildServerSessionPool;
#endif
    util::CMemoryPoolFactory<TempServerSession>   m_TempServerSessionPool;
    util::CMemoryPoolFactory<MasterServerSession> m_MasterServerSessionPool;
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    util::CMemoryPoolFactory<PortalServerSession> m_PortalServerSessionPool;
#endif
    util::CMemoryPoolFactory<WorldServerSession>  m_WorldServerSessionPool;
    util::CMemoryPoolFactory<SolarAuthServerSession> m_SolarAuthServerSessionPool;
    //----------------------------------------------------------------------------------------------
};

//==================================================================================================

extern DBProxyServerEx g_DBProxyServer;

//==================================================================================================

inline ePHASE_SHIFT DBProxyServerEx::GetPhaseStatus() const {
    return phase_shift_;
}

inline void DBProxyServerEx::SetPhaseStatus(ePHASE_SHIFT phase) {
    phase_shift_ = phase;
}

inline BYTE DBProxyServerEx::GetItemWorldID() const {
    return item_world_id_;
}

inline bool DBProxyServerEx::IsDBDisconnected() const {
    return disconnected_main_db_ != 0;
}

inline bool DBProxyServerEx::IsMatchDBDisconnected() const {
    return disconnected_match_db_ != 0;
}

// requested shutdown from master server
inline void DBProxyServerEx::RequestShutdown()
{
    SetShutdownState(READYTOSHUTDOWN);
#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_UNIT
    shutdown_requested_ = true;
#endif
}

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
inline SolarDatabaseInDBP*
DBProxyServerEx::Get1stDatabase() {
    return &main_database_;
}

inline SolarDatabaseInDBP*
DBProxyServerEx::Get2ndDatabase() {
    return &match_database_;
};

inline SolarDatabaseInDBP*
DBProxyServerEx::Get3rdDatabase() {
    return &extra_database_;
};
#endif

inline void DBProxyServerEx::SetShutdownState(eSHUTDOWNSTATE state) {
    shutdown_state_ = state;
}

#endif //DBPROXY_SERVER_EX_H