#include "stdafx.h"
#include "./DBProxyServerEx.h"

#include <io.h> // for '_access'
//
#include <PacketStruct_ServerCommon.h>
#include <ServerBuildVersion.h>

#include "ProxyNodes/DBQueryProxyManager.h"

#include "PacketHandler/PacketHandler.h"
#include "QueryObjects/DBHandler.h"
#include "Services/DBSerialManager.h"

#include "DBUserManager.h"
#include "DBUSer.h"
//
#include "Services/GuildManagerInDBP.h"
#include "Services/PortalRankingManager.h"

#include "DBProxyScriptParserManager.h"
#include "QueryObjects/DatabaseProxyQuery.h"
#include "QueryObjects/QueryPoolFactory.h"

// DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
#include "SpecialPurposeUnits/BatchProcessor.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "Services/AccumulatePointMatchDate.h"
#endif
//_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
#include "Services/Guild/GuildManager.h"
#include "Services/DominationManager.h"

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
#include "Services/SkillCoolTimeStorage.h"
#endif
#include <Services/GuildPointSystem/GuildPointSystem.h>


#ifdef _DEBUG
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/tinyxml_d.lib")    
#else
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/tinyxml.lib")
#endif //

//=================================================================================================
//=================================================================================================
//=================================================================================================

extern void CallBackMessage1(char* szMessage);
extern void CallBackMessageEx(WORD wDBIndex, QueryResult* pData);
extern void CallBackMessage1_Match(char* szMessage);
extern void CallBackMessageEx_Match(WORD wDBIndex, QueryResult* pData);
extern void CallBackQueryResult1(DWORD dwIndex, QueryResult* pData);

#define CHECK_PARAM_NUMBER(variable_type, variable, szSectionName, keyName, defValue) \
    variable_type variable = 0; \
    __READ_CONFIG_NUMBER(variable, szSectionName, keyName, defValue, GetDescFileName()); \
    if (variable == defValue) return false
#define CHECK_PARAM_STRING(variable, szSectionName, keyName, defValue) \
    char variable [128] = {0,}; \
    __READ_CONFIG_STRING(variable, szSectionName, keyName, defValue, GetDescFileName()); \
    if (variable == defValue) return false

//=================================================================================================
// operator
//=================================================================================================
// 서버 종료시 연결 강제종료
class DisconnServer : public std::unary_function<ServerSession*, void>
{
public: 
    DisconnServer(){}
    ~DisconnServer(){}
    void operator()(ServerSession* pSession) const
    {
        if (pSession->IsConnected())
        {
            // 마스터 서버는 마지막 종료 확인 패킷을 줘야 하므로 끊으면 안된다
            if (pSession->GetServerType() != MASTER_SERVER)
            {
                pSession->Disconnect();
            }
        }
    }
};

//==================================================================================================

DBProxyServerEx g_DBProxyServer;

//==================================================================================================

NetworkObject* DBProxyServerEx::DoAllocServerSession()
{
    return g_DBProxyServer.AllocServerSession(TEMP_SERVER);
}

void DBProxyServerEx::DoFreeServerSession(NetworkObject* network_object)
{
    g_DBProxyServer.FreeServerSession(static_cast<ServerSession*>(network_object));
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

DBProxyServerEx::DBProxyServerEx()
    : phase_shift_(PHASE_UNKNOWN)
    , master_server_session_(NULL)
    , world_server_session_(NULL)
    , item_world_id_(0xFF)
    , disconnected_match_db_(true)
    , disconnected_main_db_(true)
    , shutdown_requested_(false)
    , shutdown_state_(RUNNING)
    , latest_main_db_connected_ticks_(0)
    , latest_match_db_connected_ticks_(0)
{
    ASSERT(g_pDBSerialManager == NULL);
}

DBProxyServerEx::~DBProxyServerEx()
{
}

//==================================================================================================

bool DBProxyServerEx::SendMaster(MSG_BASE* msg, uint16_t msg_size)
{
    return (master_server_session_->Send(reinterpret_cast<BYTE*>(msg), msg_size) != false);
}

bool DBProxyServerEx::IsMasterConnected() const
{
    if (master_server_session_ && master_server_session_->IsConnected()) {
        return true;
    };
    return false;
}

bool DBProxyServerEx::SendToWorldServer(MSG_BASE* msg, uint16_t msg_size)
{
    if (world_server_session_ == NULL) {
        return false;
    };
    return (world_server_session_->Send(reinterpret_cast<BYTE*>(msg), msg_size) != false);
}

bool DBProxyServerEx::IsWorldConnected() const
{
    if (world_server_session_ && world_server_session_->IsConnected()) {
        return true;
    };
    return false;
}

#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
// GuildServer,DBProxyServer 통합 이후
// Z<->D 패킷은 dbproxy server의 parse로 직접 연결한다. - 2012.03.14 arycoat
void DBProxyServerEx::SendToDBPServer(MSG_BASE* pMsg, WORD wSize)
{
    PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_GAME, \
        g_DBHandler.GetSharedSession(), (MSG_BASE *)pMsg, wSize );    
}

void DBProxyServerEx::SendToGuildServer(MSG_BASE* pMsg, WORD wSize)
{
    PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_GAME, \
        g_DBHandler.GetSharedSession(), (MSG_BASE *)pMsg, wSize );    
}
#endif //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION

//==================================================================================================

ServerSession* DBProxyServerEx::AllocServerSession(eSERVER_TYPE server_type)
{
    ServerSession* session = NULL;
    switch (server_type)
    {
    case FIELD_SERVER:
    case BATTLE_SERVER:
    case GAME_SERVER:   session = GameServerSession::ALLOC(); break;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    case GUILD_SERVER:  session = GuildServerSession::ALLOC(); break;
#endif
    case WORLD_SERVER:
        // NOTE: f110522.3L
        if (world_server_session_ != NULL)
        {
            SUNLOG(eCRITICAL_LOG, "world connection is duplicated");
            return NULL;
        };
        session = WorldServerSession::ALLOC();
        world_server_session_ = static_cast<WorldServerSession*>(session);
        break;
    case TEMP_SERVER:   session = TempServerSession::ALLOC(); break;
    case AGENT_SERVER:  session = AgentServerSession::ALLOC(); break;
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    case PORTAL_SERVER: session = PortalServerSession::ALLOC(); break;
#endif
    case SOLARAUTH_SERVER: session = SolarAuthServerSession::ALLOC(); break;
    case MASTER_SERVER: 
        if (master_server_session_ != NULL)
        {
            SUNLOG(eCRITICAL_LOG, "Master가 Connect할 수 있는 개수가 넘었습니다.");
            return NULL;
        }
        session = MasterServerSession::ALLOC();
        master_server_session_ = static_cast<MasterServerSession*>(session);
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=trying to unexpected server_type(%u)|"),
               server_type);
        assert(!"존재하지 않는 서버타입입니다.");
        return NULL;
    }; //end 'switch'
    //
    session->Init(this);
    //
    return session;
}

void DBProxyServerEx::FreeServerSession(ServerSession* session)
{
    session->Release();
    const eSERVER_TYPE server_type = session->GetServerType();
    switch (server_type)
    {
    case FIELD_SERVER:
    case BATTLE_SERVER:
    case GAME_SERVER:   GameServerSession::FREE((GameServerSession *)session); break;
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    case GUILD_SERVER:  GuildServerSession::FREE((GuildServerSession *)session); break;
#endif
    case WORLD_SERVER:
        // NOTE: f110522.3L
        WorldServerSession::FREE((WorldServerSession*)session);
        world_server_session_ = NULL;
        break;
    case TEMP_SERVER:   TempServerSession::FREE((TempServerSession *)session); break;
    case AGENT_SERVER:  AgentServerSession::FREE((AgentServerSession *)session); break;
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    case PORTAL_SERVER: PortalServerSession::FREE((PortalServerSession *)session); break;
#endif
    case SOLARAUTH_SERVER: SolarAuthServerSession::FREE((SolarAuthServerSession *)session); break;
    case MASTER_SERVER:
        MasterServerSession::FREE((MasterServerSession *)session);
        master_server_session_ = NULL;
        break;
    default:
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=trying to unexpected server_type(%u)|"),
               server_type);
        assert(!"존재하지 않는 서버타입입니다.");
        break;
    }; //end 'switch'
}

//==================================================================================================

#if SUN_CODE_BACKUP
ServerSession* DBProxyServerEx::GetSession(eSERVER_TYPE server_type)
{
    switch (server_type)
    {
    case MASTER_SERVER:
        return master_server_session_;
    }
    return NULL;
}
#endif //SUN_CODE_BACKUP

bool DBProxyServerEx::ConnectTo(eSERVER_TYPE server_type)
{
    ServerSession* session = NULL;
    switch (server_type)
    {
    case MASTER_SERVER:
        session = master_server_session_;
        break;
    }

    if (session == NULL) {
        return false;
    };

    if (session->IsConnecting() || session->IsConnected())
    {
        ASSERT(!"try to repeated connect to master request");
        return false;
    };
    // process
    Connect(session);
    //
    return true;
}

//==================================================================================================

// 모든 데이터 로딩이 완료되었을 경우 콜백으로 호출될 것이다.
void DBProxyServerEx::StartServer()
{
    // <해야 할 일>
    //
    // <NOTICE>
    //   이것이 호출되었다는 의미는 필요한 모든 데이터를 받았다는 의미다.
    // <THINKING>
    static bool instanced_static = false;
    if (instanced_static == false)
    {
    #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        g_RankingMgr.OnScriptsLoaded();
#endif // _NA_008012_20150130_RANKING_SYSTEM
    #endif
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
        DominationManager::Instance()->OnScriptsLoaded();
    #endif
#ifdef _NA_008334_20150608_SONNENSCHEIN
        g_GuildManager.OnScriptLoaded();
#endif //_NA_008334_20150608_SONNENSCHEIN
    };
    //
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    // 현 시점 PHASE_EXCHANGE일 것이다.
    MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msgNTF;
    g_DBProxyServer.SendMaster(&msgNTF, sizeof(msgNTF));
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

//==================================================================================================

bool DBProxyServerEx::InitializeScriptData(BOOL read_from_file)
{
    __UNUSED(read_from_file);
#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT
    script_parser_manager_ = new DBProxyScriptParserManager;
    script_parser_manager_->InitParserPool(NULL);
    script_parser_manager_->RegisterParser();
    return (script_parser_manager_->Load() != false);
#else
    script_parser_manager_ = 0;
    return true;
#endif
}

bool DBProxyServerEx::CreateSunLog(BYTE sun_log_option, BYTE sun_log_level,
                                   const TCHAR* log_path, const TCHAR* file_name)
{
    g_pSunLog = new CSunLog;
    if (g_pSunLog->Init(sun_log_option, sun_log_level, log_path, file_name) == false)
    {
        DISPMSG("Create SunLog Fail\n");
        SAFE_DELETE(g_pSunLog);
        return false;
    }

    //2. 보안로그
    g_pSecureLog = new SecureLog();
    if (g_pSecureLog->Init(sun_log_option, sun_log_level, log_path, "Secure", false) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("Create SecureLog Failed"));
        SAFE_DELETE(g_pSecureLog);
        P_FAILED;
    }
    else
    {
        g_pSecureLog->RegisterHandler(SecureLog::WriteMessage_Network);
    }

#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT
    g_pSunConnLog = new CSunLog;
    if (g_pSunConnLog->Init(sun_log_option, sun_log_level, log_path, "DBConn") == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("Create SunDBConnLog Fail\n"));
        SAFE_DELETE(g_pSunConnLog);
        return false;
    }
#endif //

    return true;
}

//==================================================================================================

bool DBProxyServerEx::SetDBDisconnected(bool disconnected, DWORD query_start_tick)
{
    if (disconnected)
    {
        if (latest_main_db_connected_ticks_ < query_start_tick)
        {
            InterlockedCompareExchange(&disconnected_main_db_, 1, 0);
            return true;
        }
    }
    else
    {
        DWORD current_tick = GetTickCount();
        InterlockedExchange((LONG volatile*)&latest_main_db_connected_ticks_, current_tick);
        InterlockedExchange(&disconnected_main_db_, 0);
        return true;
    }

    return false;
}

bool DBProxyServerEx::SetMatchDBDisconnected(bool disconnected, DWORD query_start_tick)
{
    if (disconnected)
    {
        if (latest_match_db_connected_ticks_ < query_start_tick)
        {
            InterlockedCompareExchange(&disconnected_match_db_, true, false);
            return true;
        }
    }
    else
    {
        DWORD current_tick = GetTickCount();
        InterlockedExchange((LONG volatile*)&latest_match_db_connected_ticks_, current_tick);
        InterlockedExchange(&disconnected_match_db_, false);
        return true;
    }
    return false;
}

//==================================================================================================

void DBProxyServerEx::ReloadAttributes()
{
    // 기능 제한 설정
    int nSerialIntervalSize = -1;
    __READ_CONFIG_NUMBER(nSerialIntervalSize,
        "RELOAD_ATTRIBUTES", "SERIAL_INTERVAL_SIZE", -1, GetDescFileName());
    if (nSerialIntervalSize > 0)
    {
        SUNLOG(eCRITICAL_LOG, "SERIAL_INTERVAL_SIZE=%u", nSerialIntervalSize);
        g_pDBSerialManager->SetInterval(nSerialIntervalSize);
    }

    // 로그 레벨 설정
    uint sun_log_level;
    __READ_CONFIG_NUMBER(sun_log_level, "RELOAD_ATTRIBUTES", "SUN_LOG_LEVEL", 0xFF, GetDescFileName());
    if (0xFF != sun_log_level)
    {
        if (g_pSunLog)
        {
            g_pSunLog->SetLogLevel(static_cast<BYTE>(sun_log_level));
            SUNLOG(eCRITICAL_LOG, "SUN_LOG_LEVEL=%u", sun_log_level);
        };
    };
}

void DBProxyServerEx::DisplayServerInfo()
{
    g_DBUserManager.Display();
    SUNLOG(eCRITICAL_LOG, "Number Of Buffered Queries : %u",
           g_DBHandler.GetQueuingQueryNum());

    g_DBHandler.Display();
    SUNLOG(eCRITICAL_LOG, "[Serial] Interval: %u, Cur: %u, Max: %u", 
           g_pDBSerialManager->GetInterval(), 
           g_pDBSerialManager->GetCurSerial(),
           g_pDBSerialManager->GetMaxSerial());
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

bool DBProxyServerEx::UpdateDB()
{
    //----------------------------------------------------------------------------------------------
    // NOTE: f110427.5L, do shutdown process if a handler disconnected with odbc.
    bool disconnected_or_conditions = false;
    main_database_.Update();
    if (IsDBDisconnected()) {
        disconnected_or_conditions = true;
    }
    if (g_DBHandler.Update() == false) {
        disconnected_or_conditions = true;
    };
#if (DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT)
  #if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT) || \
      defined(__CN_OO0510__FATIGUE_MATCHING_UPDATE)
    match_database_.Update();
    if (IsMatchDBDisconnected()) {
        disconnected_or_conditions = true;
    };
  #endif
#endif
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    extra_database_.Update();
#endif
    if (disconnected_or_conditions) {
        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|Msg=ODBC handler is disconnected, forced shutdown|"));
        return false;
    };
    //----------------------------------------------------------------------------------------------
    // (WAVERIX) (090529) (MEMORIAL) 일전에는 재연결 시도를 했었다. 하지만... 일반적으로 재연결
    // 시도가 실패했기 때문에 변경된 바 있다.
    if (m_ReconnectTimer.IsExpired())
    {
        // DB연결시도에 렉이 걸리고 어차피 재연결이 되지도 않으므로 시도하지 않는다.
        if (IsDBDisconnected()) {
            SUNLOG(eCRITICAL_LOG, "GameDB is Disconnected!");
        };
#if defined(__CN_OO0510__FATIGUE_MATCHING_UPDATE)
        if (IsMatchDBDisconnected()) {
            SUNLOG(eCRITICAL_LOG, "MatchingDB is Disconnected!");
        };
#endif
    }

    return true;
}
//

BOOL DBProxyServerEx::Update(DWORD delta_tick)
{
    __UNUSED(delta_tick);
    bool update_total_result = true;
    //
    // NOTE: f110427.5L, UpdateDB(), do shutdown process if a handler disconnected with odbc.
    if ((shutdown_state_ != READYTOSHUTDOWN) && (UpdateDB() == false))
    {
        update_total_result = false;
    }
    else if (shutdown_state_ != READYTOSHUTDOWN)
    {
        WorkLoadManager::Update();
        g_DBUserManager.Update();
        if (nsQuery::DBQueryProxyManager* db_query_proxy_manager =
                nsQuery::DBQueryProxyManager::Instance())
        {
            db_query_proxy_manager->Update();
        };
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
        ;{
            g_GuildManagerInDBP.Update();
        #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
#ifdef _NA_008012_20150130_RANKING_SYSTEM
            g_RankingMgr.Update();
#endif // _NA_008012_20150130_RANKING_SYSTEM
        #endif
        #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
            g_GuildManager.Update();
            DominationManager::Instance()->Update();
        #endif
        };
#endif
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        AccumulatePointMatchDate::Instance()->Update();
#endif
        nsQuery::QueryPoolFactory::CheckPoolAlarm();

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
        batch::BatchProcessor::Update();
#endif
        // 콘솔처리
        if (ProcessConsole() == false)
        {
            SetShutdownState(READYTOSHUTDOWN);
        }
    };
    // SunLog Update 위치나 종료시점의 로그 쓰기 등을 고려해서 재설정할 것.
    g_pSunLog->Update();
    //
    if (update_total_result == false) {
        return false;
    };
    // <NOTEs>
    //  이 코드는 뭔가 깔끔하지 못한 문제가 있다. 재 업데이트 되기 전까지 붙었다 떨어졌다...
    //  AgentServer Code형태로 재정리를 가하자.
    if (shutdown_state_ == READYTOSHUTDOWN)
    {
        DisconnServer op_disconn;
        //CalcQueryCount op_calcQuery;
        static int nState = 0;

        // 종료 과정 동안 서버들 접속 못 하도록 블럭
        for_each(op_disconn);

        switch (nState)
        {
        case 0:
            {
                // 매번 서버들이 다시 접속하므로 매번 끊어준다-_-;
                for_each(op_disconn);
                // 남은 쿼리 다 끝내고
                //for_each(op_calcQuery);

                if (!g_DBHandler.GetQueuingQueryNum())
                {
                    nState++;
                }
                break;
            }
        case 1:
            {
                //나간다
                if (shutdown_requested_ && IsMasterConnected())
                {
                    MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS msg;
                    master_server_session_->SendPacket(&msg, sizeof(msg));

                    // 한바퀴 더 돌아야 SHUTDOWN_ANSWER를 보낼 수 있으므로 돈다
                    nState++;
                }
                else
                {
                    Shutdown();
                }
            }
            break;
        case 2:
            {
                Shutdown();
            }
            break;
        }
    }

    return true;
}

BOOL    DBProxyServerEx::KeyEvent(CHAR ch)
{
    switch (ch)
    {
    case 0x1b:
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
        {
            BOOLEAN safable_stop = batch::BatchProcessor::Started() == false;
            if (safable_stop == false) {
                SUNLOG(eCRITICAL_LOG, "Can't exit process while batch processing");
            }
            return safable_stop;
        }
        break;
#else
        return true;
#endif
    case ' ':
        DisplayServerInfo();
        break;
    case 'G':
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
        {
            BOOLEAN started = batch::BatchProcessor::Started();
            batch::BatchProcessor::ChangeRunState(!started); //toggle
        }
#endif
        break;
    case 'R':
        ReloadAttributes(); 
        break;
    case 'P': 
        {
            g_DBUserManager.DisplayPoolInfo();
        } break;
    case 'K':
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    #ifdef _DEBUG
        g_RankingMgr.DisplayRanking();
    #endif
        g_RankingMgr.Display();
#endif // _NA_008012_20150130_RANKING_SYSTEM
#endif
        break;
    case 'U':
        RunUnitTest();
        break;
    case 'T':
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
        WaverixTestUnit();
#endif
        break;
    }

    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

BOOL DBProxyServerEx::Init()
{
    //----------------------------------------------------------------------------------------------
    // CHANGES: f100529.3L, fix up an unexpected error
    // to solve the irregular point of a compile generated assembly code
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
    const char* server_name = "DBProxy";
#elif DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT
    const char* server_name = "AccountDBProxy";
#elif DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    const char* server_name = "DBProxyUnit";
#endif
    //----------------------------------------------------------------------------------------------
    const char* const config_file = GetDescFileName();
    CONSOLELOG(eCRITICAL_LOG, _T("|[%s]|Msg=Initializing %s server|"),
               __FUNCTION__, server_name, config_file);
    //----------------------------------------------------------------------------------------------
    // #1. create (sunlog + guildlog) logger
    if (this->InitializeLogger(server_name) == false)
    {
        CONSOLELOG(eCRITICAL_LOG, _T("|Msg=fails the logger initialization"));
        return false;
    };
    //----------------------------------------------------------------------------------------------
    // #2. instance base workload manager
    WorkLoadManager::CreateInstance();
    //----------------------------------------------------------------------------------------------
    // #3. initialize scripts. if this server is managed by the solar auth server,
    // it loads scripts lately.
    if (InitializeScriptData(IsServerInfoFromFile()) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|Msg=fails the script data loading process"));
        return false;
    };
    //----------------------------------------------------------------------------------------------
    // #4. load configurations
    DBSERIAL ___serial_issue_reserve_size = 0;
    const DBSERIAL& serial_issue_reserve_size = ___serial_issue_reserve_size;
    ;{
        __READ_CONFIG_NUMBER(___serial_issue_reserve_size,
            "ATTRIBUTES", "SERIAL_INTERVAL_SIZE", 0, config_file);
        SUNLOG(eCRITICAL_LOG, "SERIAL_INTERVAL_SIZE='%u'\n", ___serial_issue_reserve_size);
        if (FlowControl::FCAssert(___serial_issue_reserve_size > 0) == false) {
            return false;
        };
    };
#if SUN_CODE_BACKUP
    ;{
        DWORD dwPoolSize = 0;
        __READ_CONFIG_NUMBER(dwPoolSize, "ATTRIBUTES", "QUERY_POOL_SIZE", 0, config_file);
        CONSOLELOG(eCRITICAL_LOG, "QUERY_POOL_SIZE:%u개\n", dwPoolSize);
        // (CHANGES) (WAVERIX) dwPoolSize 100->20, __ZeroClearPool에서는 큰 수량의 풀은 오히려
        // 불필요한 런타임 성능 저하를 야기할 수 있으며, 불필요한 메모리 낭비도 대단히 많아진다.
        // 고정값으로 대체한다.
        //ASSERT(dwPoolSize > 40);
        dwPoolSize = 20;
    };
#endif
    ;{
        uint item_world_id = 0;
        __READ_CONFIG_NUMBER(item_world_id, "ATTRIBUTES", "ITEM_WORLD_ID", 0xFF, config_file);
        SUNLOG(eCRITICAL_LOG, "ITEM_WORLD_ID : %u\n", item_world_id);
        if (FlowControl::FCAssert(item_world_id != 0xFF) == false) {
            return false;
        };
        item_world_id_ = static_cast<BYTE>(item_world_id);
    };
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
  #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    //----------------------------------------------------------------------------------------------
    // #4-2. load configurations (ranking information)
    ;{
        int settled_year, settled_month, settled_day, period_day, suspend_time;
        __READ_CONFIG_NUMBER(suspend_time, "ATTRIBUTES", "RANKING_SUSPEND_TIME", 0, config_file);
        __READ_CONFIG_NUMBER(period_day, "ATTRIBUTES", "RANKING_PERIOD_DAY", 0, config_file);
        __READ_CONFIG_NUMBER(settled_year, "ATTRIBUTES", "RANKING_SETTLED_YEAR", 0, config_file);
        __READ_CONFIG_NUMBER(settled_month, "ATTRIBUTES", "RANKING_SETTLED_MONTH", 0, config_file);
        __READ_CONFIG_NUMBER(settled_day, "ATTRIBUTES", "RANKING_SETTLED_DAY", 0, config_file);
        if (FlowControl::FCAssert(suspend_time > 0 && period_day > 0 &&
                settled_year > 0 && settled_month > 0 && settled_day > 0) == false)
        {
            SUNLOG(eCRITICAL_LOG, _T("must be checked Ranking settlement date information in '%s'"),
                    config_file);
            return false;
        }

    #ifdef _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT
        // 정산 시점을 suspend time만큼 뒤로 밀어 앞뒤로 주어지는 중단 시간이 정시에 시작하도록 변경
        int minute = suspend_time;
    #else
        int minute = 0;
    #endif
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        //4시에 시작한다
        g_RankingMgr.InitComputation(settled_year, settled_month, settled_day,
                                     4, minute, 0, period_day, suspend_time);
#else
        g_RankingMgr.InitComputation(settled_year, settled_month, settled_day,
                                     0, minute, 0, period_day, suspend_time);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        g_RankingMgr.Initialize();
    };
    //----------------------------------------------------------------------------------------------
  #endif //_NA002635_GP_DP_PROTOCOL_UINIFICATION
#endif
#endif // _NA_008012_20150130_RANKING_SYSTEM
    //----------------------------------------------------------------------------------------------
    // #5. set up session pool
    if (FlowControl::FCAssert(InitializeSessionPool()) == false) {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    // #6. initialize services
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT
    g_DBUserManager.Init(1);
    g_GuildManagerInDBP.Init(1);
#else //DBPROXY_TYPE_DEFAULT|DBPROXY_TYPE_UNIT
    g_DBUserManager.Init(20);
    g_GuildManagerInDBP.Init(10 /*_MAX_GUILD_POOL_NUM*/); 
    password_pool_.Initialize(10, 10);
#endif //DBPROXY_TYPE_DEFAULT|DBPROXY_TYPE_UNIT
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    g_GuildPointSystem.Initialize();
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
    nsQuery::DBQueryProxyManager::CreateInstance();
    //----------------------------------------------------------------------------------------------
    CONSOLELOG(eCRITICAL_LOG, "============== DataBaseProxy Start ================= \n");
    //----------------------------------------------------------------------------------------------
    // #7. set up main db connection
    if (this->InitializeDatabase() == false) {
        return false;
    };
    //----------------------------------------------------------------------------------------------
    SUNLOG(eCRITICAL_LOG, "*** %s Start ***", server_name);
    SUNLOG(eCRITICAL_LOG, "*** %s Server Version = %d ***", server_name, SERVER_RELEASE_BUILD_NO);
    //----------------------------------------------------------------------------------------------
    // #8. first db query event (request issue serial)
    ;{
        g_pDBSerialManager = new DBSerialManager(main_database_, *this);
        g_pDBSerialManager->SetInterval(serial_issue_reserve_size);
    #if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
        // 디비 시리얼 얻어 오는 쿼리
        g_pDBSerialManager->AllocSerial();
    #endif
    };
    shutdown_requested_ = false;

    //----------------------------------------------------------------------------------------------
    // #9. set up extra controller
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    batch::BatchProcessor::CreateInstance();
    if (batch::BatchProcessor::IsAutoMode()) {
        batch::BatchProcessor::ChangeRunState(true);
    };
#endif

    //----------------------------------------------------------------------------------------------
    // #10. Last order of the initialization
#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    StartServer();
#endif
    //----------------------------------------------------------------------------------------------

    return true;
}

bool DBProxyServerEx::InitializeLogger(const char* server_name)
{
    //----------------------------------------------------------------------------------------------
    // #1. create (sunlog+guildlog) logger
    ;{
        //1. 로그 정보를 불러온다.
        CHECK_PARAM_NUMBER(uint, log_option, "LOGFILE", "SUN_LOGFILE_OPTION", 0xFF);
        CHECK_PARAM_NUMBER(uint, log_level, "LOGFILE", "SUN_LOGFILE_LEVEL", 0xFF);
        CHECK_PARAM_STRING(log_file_path, "LOGFILE", "SUN_LOGFILE_PATH", NULL);

        if (CreateSunLog(BYTE(log_option), BYTE(log_level),
                log_file_path, server_name) == false)
        {
            return false;
        };
    };
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    //----------------------------------------------------------------------------------------------
    // #1-2. create guildlog logger
    ;{
        CONSOLELOG(eCRITICAL_LOG,  _T("|[%s]|Msg=Initializing guild logger|"), __FUNCTION__);
        g_guild_log = new GuildLog();
        CHECK_PARAM_STRING(guild_log_file_path, "LOGFILE", "WOPS_LOGFILE_PATH", NULL);
        if (_access(guild_log_file_path, 0) == -1) // 폴더가 존재하지 않으면..
        {
            if (::CreateDirectory(guild_log_file_path, NULL) == false)
            {
                SUNLOG(eCRITICAL_LOG, _T("|Msg=can't create guild log directory = '%s'|"),
                       guild_log_file_path);
                return false;
            };
        };
        TCHAR binary_log_path[MAX_PATH], text_log_path[MAX_PATH];
        _sntprintf(binary_log_path, _countof(binary_log_path),
                   _T("%s\\\\GUILD"), guild_log_file_path);
        _sntprintf(text_log_path, _countof(text_log_path),
                   _T("%s\\\\GUILD_TEXT"), guild_log_file_path);

        CHECK_PARAM_NUMBER(uint, wops_log_option, "LOGFILE", "WOPS_LOGFILE_OPTION", 0xFF);
      #ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
        TCHAR log_file_name[MAX_PATH];
        _sntprintf(log_file_name, _countof(log_file_name), _T("%d_SUN_GUILD"), NATION_BUILD_VERSION);
      #else
        TCHAR* log_file_name = _T("SUN_GUILD");
      #endif
        if ((wops_log_option > 2) ||
            (g_guild_log->Init(static_cast<BYTE>(wops_log_option), \
                log_file_name, binary_log_path, text_log_path) == false))
        {
            SUNLOG(eCRITICAL_LOG, _T("|Msg=can't create guild logger|")
                   _T("bin_path='%s', text_path='%s', file_name='%s'|"),
                   guild_log_file_path);
        };
    };
#endif //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    return true;
}

bool DBProxyServerEx::InitializeSessionPool()
{
    const char* const config_file = GetDescFileName();
    __TOUCHED(config_file);
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT
    m_TempServerSessionPool.Initialize(10);
    m_GameServerSessionPool.Initialize(1);
    m_AgentServerSessionPool.Initialize(1);
  #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    m_GuildServerSessionPool.Initialize(1);
  #endif
    m_MasterServerSessionPool.Initialize(2);
  #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    m_PortalServerSessionPool.Initialize(1);
  #endif
    m_WorldServerSessionPool.Initialize(1);

    m_SolarAuthServerSessionPool.Initialize(2);

    GameServerSession::SetPool(&m_GameServerSessionPool);
  #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    GuildServerSession::SetPool(&m_GuildServerSessionPool);
  #endif
    WorldServerSession::SetPool(&m_WorldServerSessionPool);
    TempServerSession::SetPool(&m_TempServerSessionPool);
    AgentServerSession::SetPool(&m_AgentServerSessionPool);
  #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    PortalServerSession::SetPool(&m_PortalServerSessionPool);
  #endif
    MasterServerSession::SetPool(&m_MasterServerSessionPool);
    SolarAuthServerSession::SetPool(&m_SolarAuthServerSessionPool);
#else //DBPROXY_TYPE_DEFAULT|DBPROXY_TYPE_UNIT
    m_GameServerSessionPool.Initialize(15);
    m_TempServerSessionPool.Initialize(15);

    m_AgentServerSessionPool.Initialize(5);
  #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    m_GuildServerSessionPool.Initialize(2);
  #endif
    m_MasterServerSessionPool.Initialize(2);
  #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    m_PortalServerSessionPool.Initialize(2);
  #endif
    m_WorldServerSessionPool.Initialize(2);

    GameServerSession::SetPool(&m_GameServerSessionPool);
  #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    GuildServerSession::SetPool(&m_GuildServerSessionPool);
  #endif
    WorldServerSession::SetPool(&m_WorldServerSessionPool);
    TempServerSession::SetPool(&m_TempServerSessionPool);
    AgentServerSession::SetPool(&m_AgentServerSessionPool);
  #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    PortalServerSession::SetPool(&m_PortalServerSessionPool);
  #endif
    MasterServerSession::SetPool(&m_MasterServerSessionPool);
#endif //DBPROXY_TYPE_DEFAULT|DBPROXY_TYPE_UNIT
    return true;
};

bool DBProxyServerEx::InitializeDatabase()
{
    const char* const config_file = GetDescFileName();
    __TOUCHED(config_file);
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT
    char* nameOfGameDB = "SUNOnline_Account";
  #ifdef _JP_0_20101123_PMS
    SERVER_KEY key = GetKey();
    key.Set(0,0,GetServerType(),0);
    SetKey(key);
  #endif
#else
    char* nameOfGameDB = "SUNOnline";
#endif
    char connectOdbcName[256];
    __READ_CONFIG_STRING(connectOdbcName, "ATTRIBUTES", "CONNECT_ODBC_NAME", "", config_file);
    if (connectOdbcName[0] != '\0') {
        nameOfGameDB = connectOdbcName;
    };
    //----------------------------------------------------------------------------------------------
    // 디비 초기화
    SolarDatabaseDesc db_desc = { DBCInterfaceType_ODBC, };
    db_desc.database_module_type = DBCInterfaceType_ODBC;
    db_desc.query_pool_size      = 2000;
    db_desc.query_processor_num  = 1;
    db_desc.database_ip          = "";
    db_desc.database_name        = nameOfGameDB;

    const eSERVER_MODE server_mode = GetMode();
    __TOUCHED(server_mode);
#if (DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_UNIT)
    // DBPROXY_TYPE_DEFAULT|DBPROXY_TYPE_ACCOUNT
    if ((server_mode && server_mode < eSERVER_UPPERBOUND) == false)
    {
        ASSERT(!"서버 타입 선택 실패");
        return false;
    };

    DBDecryptFileReader* const decoder = new DBDecryptFileReader();
    if (decoder->ReadStartFromBuffer(CryptInfoStream::GetCryptStream(), "DBP") == false)
    {
        ASSERT(!"Can't open encrypted file");
        decoder->ReadEnd();
        delete decoder;
        return false;
    }
    ;{
        char connect_id[256] = { 0, };
        __READ_CONFIG_STRING(connect_id, "ATTRIBUTES", "CONNECT_ODBC_ID", "", config_file);
        connect_id[_countof(connect_id) - 1] = '\0';
        if (connect_id[0] != '\0')
        {
            db_desc.user_name = connect_id;
        }
        else
        {
            db_desc.user_name = (char*)decoder->GetIdentification(server_mode);
        }
        char connect_pwd[256] = { 0, };
        __READ_CONFIG_STRING(connect_pwd, "ATTRIBUTES", "CONNECT_ODBC_PASSWORD", "", config_file);
        connect_pwd[_countof(connect_pwd) - 1] = '\0';
        if (connect_pwd[0] != '\0')
        {
            db_desc.user_password  = connect_pwd;
        }
        else
        {
            db_desc.user_password  = (char*)decoder->GetPassword(server_mode);
        }
        db_desc.fnptr_error_message    = CallBackMessage1;
        db_desc.fnptr_error_message_ex = CallBackMessageEx;
        db_desc.fnptr_query_result     = CallBackQueryResult1;
    };

    g_DBHandler.Init(nameOfGameDB, db_desc.user_name, db_desc.user_password);

    if (FlowControl::FCAssert(main_database_.Initialize(db_desc, false)) == false)
    {
        decoder->ReadEnd();
        delete decoder;
        return false;
    }
    SetDBDisconnected(false);
#elif (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT)
    ;{
        char connect_id[256] = { 0, };
        char connect_pwd[256] = { 0, };

        __READ_CONFIG_STRING(connect_id, "ATTRIBUTES", "CONNECT_ODBC_ID", "", config_file);
        __READ_CONFIG_STRING(connect_pwd, "ATTRIBUTES", "CONNECT_ODBC_PASSWORD", "", config_file);

        connect_id[_countof(connect_pwd) - 1] = '\0';
        connect_pwd[_countof(connect_pwd) - 1] = '\0';
        db_desc.user_name      = connect_id;
        db_desc.user_password  = connect_pwd;
        db_desc.fnptr_error_message   = CallBackMessage1;
        db_desc.fnptr_error_message_ex = CallBackMessageEx;
        db_desc.fnptr_query_result    = batch::BatchProcessor::QueryResultCallBack;
        g_DBHandler.Init(nameOfGameDB, connect_id, connect_pwd);
        if (FlowControl::FCAssert(main_database_.Initialize(db_desc, true)) == false) {
            return false;
        }
        SetDBDisconnected(false);
    };
#endif

    //----------------------------------------------------------------------------------------------
    // set up secondary db connection
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
    // DBPROXY_TYPE_DEFAULT|DBPROXY_TYPE_ACCOUNT
  #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    char* nameOfMatchDB = "MatchingDB";
    __READ_CONFIG_STRING(connectOdbcName, "ATTRIBUTES", "CONNECT_ODBC_NAME_MATCH", "", config_file);
    if (connectOdbcName[0] != '\0') {
        nameOfMatchDB = connectOdbcName;
    };
    ;{
        db_desc.database_name = nameOfMatchDB;
        db_desc.fnptr_error_message    = CallBackMessage1_Match;
        db_desc.fnptr_error_message_ex = CallBackMessageEx_Match;
    };
    if (FlowControl::FCAssert(match_database_.Initialize(db_desc, false)) == false)
    {
        decoder->ReadEnd();
        delete decoder;
        return false;
    }
    SetMatchDBDisconnected(false);
  #endif
#elif (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT)
    char* nameOfMatchDB = "MatchingDB";
    __READ_CONFIG_STRING(connectOdbcName, "ATTRIBUTES", "CONNECT_ODBC_NAME_MATCH", "", config_file);
    if (connectOdbcName[0] != '\0') {
        nameOfMatchDB = connectOdbcName;
    };
    ;{
        char connect_id[256] = { 0, };
        char connect_pwd[256] = { 0, };

        __READ_CONFIG_STRING(connect_id, "ATTRIBUTES", "CONNECT_ODBC_MATCH_ID", "", config_file);
        __READ_CONFIG_STRING(connect_pwd, "ATTRIBUTES", "CONNECT_ODBC_MATCH_PASSWORD", "", config_file);

        connect_id[_countof(connect_pwd) - 1] = '\0';
        connect_pwd[_countof(connect_pwd) - 1] = '\0';
        db_desc.user_name      = connect_id;
        db_desc.user_password  = connect_pwd;
        db_desc.database_name  = nameOfMatchDB;
        db_desc.fnptr_error_message   = CallBackMessage1;
        db_desc.fnptr_error_message_ex = CallBackMessageEx;
        db_desc.fnptr_query_result    = batch::BatchProcessor::QueryResultCallBack;
    };
    if (FlowControl::FCAssert(match_database_.Initialize(db_desc, true)) == false) {
        return false;
    }
    SetMatchDBDisconnected(false);
    //
    if (FlowControl::FCAssert(extra_database_.Initialize(db_desc, true)) == false) {
        return false;
    }
#endif

#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_UNIT
    decoder->ReadEnd();
    delete decoder;
#endif
    return true;
}

//==================================================================================================

void DBProxyServerEx::Release()
{
    SUNLOG(eCRITICAL_LOG, "*** DBProxyServer Close ***");

#ifdef _NA_008012_20150130_RANKING_SYSTEM
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
  #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    g_RankingMgr.Release();
  #endif
#endif
#endif // _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    AccumulatePointMatchDate::DestroyInstance();
#endif

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    batch::BatchProcessor::DestroyInstance();
#endif

    if (script_parser_manager_)
    {
        script_parser_manager_->Release();
        script_parser_manager_->DestroyInstanceEx();
        SAFE_DELETE(script_parser_manager_);
    }

    SetPhaseStatus(PHASE_UNKNOWN);
    WorkLoadManager::DestroyInstance();

    SAFE_DELETE(g_pDBSerialManager);

    main_database_.Release();
#if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT) || \
    defined(__CN_OO0510__FATIGUE_MATCHING_UPDATE)
    match_database_.Release();
#endif
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    extra_database_.Release();
#endif
    nsQuery::DBQueryProxyManager::DestroyInstance();
    g_GuildManager.Release();
    g_DBUserManager.Release();

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    SkillCoolTimeStorage::DestroyInstance();
#endif

    PacketHandler::DestroyInstance();

    // 게임로그
    SAFE_DELETE(g_pSunLog);
    SAFE_DELETE(g_pSecureLog);
    SAFE_DELETE(g_guild_log); //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_ACCOUNT
    if (g_pSunConnLog)
    {
        CSunLog* pTmpLog = g_pSunConnLog;
        InterlockedExchange((LONG volatile*)&g_pSunConnLog, NULL);
        delete pTmpLog;
    }
#endif //
}