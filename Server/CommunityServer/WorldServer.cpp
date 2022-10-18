#include "WorldServerPch.hxx"
#include "./WorldServer.h"

#include <io.h>

#include <PacketStruct_MW.h>

#include "Functions/WorldScriptParserManager.h"
#include "PacketHandler/PacketHandler.h"

#include "Channel/WorldChannelManager.h"
#include "Objects/WorldUserManager.h"
#include "Services/Friends/FriendInvitation.h"
#include "Services/Lotto/Lotto.h"
#include "Services/Guild/WorldGuildManager.h"

//==================================================================================================

namespace ns_world {
;
}; //end of namespace

//==================================================================================================

ns_world::WorldServer* ns_world::WorldServer::instance_static_ = NULL;

ns_world::WorldServer* ns_world::WorldServer::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(WorldServer** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        WorldServer** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    WorldServer* provider = new WorldServer;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

ns_world::WorldServer::WorldServer()
{
    ZeroMemory(fields_start_, static_cast<size_t>(fields_end_ - fields_start_));
    if (ServerSessionPool* const pool = new ServerSessionPool)
    {
        pool->Initialize(5, 5);
        this->server_session_pool_ = pool;
    };
    if (TempServerSessionPool* const pool = new TempServerSessionPool)
    {
        pool->Initialize(5, 5);
        this->temp_session_pool_ = pool;
    };
    this->InitializeDataFields();
}

ns_world::WorldServer::~WorldServer()
{
    SAFE_DELETE(server_session_pool_);
    SAFE_DELETE(temp_session_pool_);
}

NetworkObject* ns_world::WorldServer::CreateServerSideAcceptedObject()
{
    ns_world::WorldServer* world_server = ns_world::WorldServer::Instance();
    return world_server->AllocServerSession(TEMP_SERVER);
}

void ns_world::WorldServer::DestroyServerSideAcceptedObject(NetworkObject* network_object)
{
    ns_world::WorldServer* world_server = ns_world::WorldServer::Instance();
    world_server->FreeServerSession(static_cast<ServerSession*>(network_object));
}

void ns_world::WorldServer::DestroyServerSideConnectedObject(NetworkObject* network_object)
{
    ns_world::WorldServer* world_server = ns_world::WorldServer::Instance();
    world_server->FreeServerSession(static_cast<ServerSession*>(network_object));
}

ServerSession* ns_world::WorldServer::AllocServerSession(eSERVER_TYPE server_type)
{
    if (server_type == TEMP_SERVER)
    {
        TempServerSession* temp_session = \
            reinterpret_cast<TempServerSession*>(this->temp_session_pool_->Alloc());
        static_cast<ServerSession*>(temp_session)->Init(this);
        return temp_session;
    }
    else if (kAcceptSessions & (1 << server_type))
    {
        ServerSessionInWorld* server_session = \
            reinterpret_cast<ServerSessionInWorld*>(this->server_session_pool_->Alloc());
        static_cast<ServerSession*>(server_session)->Init(this);
        server_session->ChangeServerType(server_type);
        return server_session;
    }
    else if (kConnectSessions & (1 << server_type))
    {
        ServerSessionInWorld* server_session = \
            reinterpret_cast<ServerSessionInWorld*>(this->server_session_pool_->Alloc());
        static_cast<ServerSession*>(server_session)->Init(this);
        server_session->ChangeServerType(server_type);
        if (server_type == GAME_DBPROXY) {
            this->connect_sessions_.dbproxy_server = server_session;
            this->connect_sessions_.dbproxy_enabled = false;
        }
        else if (server_type == MASTER_SERVER) {
            this->connect_sessions_.master_server = server_session;
            this->connect_sessions_.master_enabled = false;
        };

        return server_session;
    };
    //
    SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unexpected server type %d"), server_type);
    return NULL;
}

void ns_world::WorldServer::FreeServerSession(ServerSession* server_session)
{
    const eSERVER_TYPE server_type = server_session->GetServerType();
    if (server_type == TEMP_SERVER)
    {
        this->temp_session_pool_->Free(static_cast<TempServerSession*>(server_session));
        return;
    }
    else if (kAcceptSessions & (1 << server_type))
    {
        ServerSessionInWorld* server_session_in_world =
            static_cast<ServerSessionInWorld*>(server_session);
        this->server_session_pool_->Free(server_session_in_world);
        return;
    }
    else if (kConnectSessions & (1 << server_type))
    {
        ServerSessionInWorld* server_session_in_world =
            static_cast<ServerSessionInWorld*>(server_session);
        this->server_session_pool_->Free(server_session_in_world);

        if (server_type == GAME_DBPROXY) {
            this->connect_sessions_.dbproxy_server = NULL;
            this->connect_sessions_.dbproxy_enabled = false;
        }
        else if (server_type == MASTER_SERVER) {
            this->connect_sessions_.master_server = NULL;
            this->connect_sessions_.master_enabled = false;
        };

        return;
    };
}

void ns_world::WorldServer::EnableSessionService(
    ns_world::ServerSessionInWorld* server_session, bool enabled)
{
    eSERVER_TYPE server_type = server_session->GetServerType();
    if (kConnectSessions & (1 << server_type))
    {
        ServerSessionInWorld* server_session_in_world =
            static_cast<ServerSessionInWorld*>(server_session);

        if (server_type == GAME_DBPROXY)
        {
            if (enabled && !FlowControl::FCAssert(connect_sessions_.dbproxy_server != NULL)) {
                return;
            };
            connect_sessions_.dbproxy_enabled = enabled;
        }
        else if (server_type == MASTER_SERVER)
        {
            if (enabled && !FlowControl::FCAssert(connect_sessions_.master_server != NULL)) {
                return;
            };
            connect_sessions_.master_enabled = enabled;
        };

        return;
    };
}

void ns_world::WorldServer::InitializeDataFields()
{
    phase_shift_ = PHASE_UNKNOWN;
}

BOOL ns_world::WorldServer::Init()
{
    WorkLoadManager::CreateInstance();
    this->InitializeDataFields();
    if (this->InitializeLogger() == false) {
        CONSOLELOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't create logger|"), __FUNCTION__);
        return false;
    };
    sequential_shutdown_service_ = ns_world::SequentialShutdownService::Register();
    //;{
    //    int service_mode = 0;
    //    __READ_CONFIG_NUMBER(service_mode, "ATTRIBUTES", "SERVICE_MODE",
    //                         eSERVER_LIVE, WORLD_SERVER_INFO_FILE_NAME);
    //    if (service_mode < 0 || service_mode >= eSERVER_UPPERBOUND) {
    //        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=unexpected service mode = %d|"),
    //               __FUNCTION__, service_mode);
    //        return false;
    //    };
    //};
    if (this->IsServerInfoFromFile())
    {
        int world_id = 0;
        __READ_CONFIG_NUMBER(world_id, "ATTRIBUTES", "WORLD_ID",
                             -1, WORLD_SERVER_INFO_FILE_NAME);
        if (world_id < 0) {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=unexpected world id, check [ATTRIBUTES] WORLD_ID|"),
                   __FUNCTION__, world_id);
            return false;
        };
        server_key_.Set(BYTE(world_id), 0, 0, 1);
    };
    if (this->ReloadAdditionalConfig() == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=additional configuration load failed|"), __FUNCTION__);
    };
#ifdef _DEBUG
    if (const char* dll_file = "3DTerrain_sd.dll")
#else
    if (const char* dll_file = "3DTerrain_s.dll")
#endif
    {
        engine_.module_instance = ::Load3DTerrainLibrary(dll_file, &engine_.instance);
        if (engine_.module_instance == NULL) {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=%s file loading fails|"), dll_file);
            return false;
        };
        engine_.load_helper = (CLoadHelper*)engine_.instance(CT_NEW_MAPLOADER, 0);
    };
    //
    ns_world::PacketHandler::Instance();
    //
    parser_manager_ = new ns_world::WorldScriptParserManager;
    parser_manager_->RegisterParser();
    parser_manager_->InitParserPool(NULL);
#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    parser_manager_->Load();
    //
    this->StartServer();
#endif

    SUNLOG(eCRITICAL_LOG, _T("*** WorldServer Start ***"));
    SUNLOG(eCRITICAL_LOG, _T("*** WorldServer Server Version = %d ***"), SERVER_RELEASE_BUILD_NO);

    return true;
}

BOOL ns_world::WorldServer::Update(DWORD delta_tick)
{
    __UNUSED(delta_tick);
    const util::ServerTimeInfoPerFrame& time_info = this->GetTimeInfoPerFrame();
    //
    WorkLoadManager::Update();
    //
    this->UpdateServices(time_info);
    //
    if (g_pSunLog) {
        g_pSunLog->Update();
    };
    if (ns_world::count_log_global) {
        ns_world::count_log_global->Update();
    };
    //
    return true;
}

bool ns_world::WorldServer::UpdateServices(const util::ServerTimeInfoPerFrame& time_info)
{
    if (ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance()) {
        user_manager->UpdateServices(time_info, this->GetGameDBProxy());
    };
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM
    if (connect_sessions_.dbproxy_enabled) {
        Lotto::GetInstance().Update();
    };
    return true;
}

void ns_world::WorldServer::Release()
{
    SUNLOG(eCRITICAL_LOG, _T("World Server Closing..."));
    WorkLoadManager::DestroyInstance();
    if (sequential_shutdown_service_)
    {
        sequential_shutdown_service_->Shutdown(sequential_shutdown_service_);
        sequential_shutdown_service_ = NULL;
    };
    //
    if (engine_.instance) {
        engine_.instance(CT_DELETE_MAPLOADER, engine_.load_helper);
        engine_.load_helper = NULL;
        engine_.instance = NULL;
    };
    if (engine_.module_instance) {
        ::FreeLibrary(engine_.module_instance);
        engine_.module_instance = NULL;
    };
    Lotto::GetInstance().Shutdown();
    ns_world::WorldGuildManager::DestroyInstance();
    //
    if (parser_manager_)
    {
        parser_manager_->Release();
        parser_manager_->DestroyInstanceEx();
        delete parser_manager_;
    };
    //
    SAFE_DELETE(ns_world::world_log);
    SAFE_DELETE(g_pSunLog);
    SAFE_DELETE(ns_world::count_log_global);
}

void ns_world::WorldServer::BroadcastNoticeFromFile()
{
    // NOTE: f110823.1L, migrated from the original WorldServer.cpp
    const char* const kNoticeFile = "Notice.txt";
    // 파일에서 공지 읽기
    FILE* file_handle = ::fopen(kNoticeFile, "rt");
    if (file_handle == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("Can't Open File %s"), kNoticeFile);
        return;
    };
    //
    MSG_CW_NOTICE_BRD msg_brd;
    ZeroMemory(msg_brd.szMsg, sizeof(msg_brd.szMsg));

    ::fread(msg_brd.szMsg, sizeof(msg_brd.szMsg[0]), _countof(msg_brd.szMsg), file_handle);
    ::fclose(file_handle);

    ;{  // setup bradcasting message
        msg_brd.m_byRed = 0xFF;
        msg_brd.m_byGreen = 0xFF;
        msg_brd.m_byBlue = 0xFF;
        msg_brd.szMsg[_countof(msg_brd.szMsg) - 1] = '\0';
        msg_brd.wLen = static_cast<WORD>(::strlen(msg_brd.szMsg) + 1);
    };
    // 전체 공지
    if (ns_world::WorldUserManager* user_manager = ns_world::WorldUserManager::Instance()) {
        user_manager->SendToAll(&msg_brd, msg_brd.GetSize());
    };
    SUNLOG(eCRITICAL_LOG, ">> WorldNotice(File): %s", msg_brd.szMsg);
}

//==================================================================================================

namespace ns_world { namespace internal {
;
static ulong LoadStatisticInterval();
static ulong LoadShoutInterval();
static bool LoadUseFileNoticeFunction();

}}; //end of namespace

static ulong ns_world::internal::LoadStatisticInterval()
{
    int statistics_interval = 0;
    __READ_CONFIG_NUMBER(statistics_interval, "INTERVAL", "STATISTICS",
                         10000, WORLD_SERVER_INFO_FILE_NAME);
    return statistics_interval;
}

static ulong ns_world::internal::LoadShoutInterval()
{
    int shout_interval = 0;
    __READ_CONFIG_NUMBER(shout_interval, "INTERVAL", "SHOUT",
                         30000, WORLD_SERVER_INFO_FILE_NAME);
    return shout_interval;
}

static bool ns_world::internal::LoadUseFileNoticeFunction()
{
    int use_file_notice_function = 0;
    __READ_CONFIG_NUMBER(use_file_notice_function, "COMMON", "NOTICEFROMFILE",
                         0, WORLD_SERVER_INFO_FILE_NAME);
    return (use_file_notice_function != 0);
}

//==================================================================================================

bool ns_world::WorldServer::ReloadAdditionalConfig()
{
    options_.statistics_interval = ns_world::internal::LoadStatisticInterval();
    options_.shout_interval = ns_world::internal::LoadShoutInterval();
    options_.use_file_notice_function = ns_world::internal::LoadUseFileNoticeFunction();

    return true;
};

bool ns_world::WorldServer::InitializeLogger()
{
    int log_option = 0;
    int log_level = 0;
    char log_file_path[256];
    ;{
        __READ_CONFIG_NUMBER(log_option, "LOGFILE", "SUN_LOGFILE_OPTION",
                             OUTPUT_BOTH, WORLD_SERVER_INFO_FILE_NAME);
        __READ_CONFIG_NUMBER(log_level, "LOGFILE", "SUN_LOGFILE_LEVEL",
                             eCRITICAL_LOG, WORLD_SERVER_INFO_FILE_NAME);
        __READ_CONFIG_STRING(log_file_path, "LOGFILE", "SUN_LOGFILE_PATH",
                             NULL, WORLD_SERVER_INFO_FILE_NAME);
    };
    //
    CONSOLELOG(eCRITICAL_LOG, _T("create server log"));
    ASSERT(g_pSunLog == NULL);
    g_pSunLog = new CSunLog;
    if (g_pSunLog->Init(BYTE(log_option), BYTE(log_level),
                log_file_path, "World") == false)
    {
        CONSOLELOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't create server log|"));
        SAFE_DELETE(g_pSunLog);
        return false;
    };
    //
    CONSOLELOG(eCRITICAL_LOG, _T("create count log"));
    ASSERT(count_log_global == NULL);
    count_log_global = new CSunLog;
    if (count_log_global->Init(BYTE(log_option), BYTE(log_level),
                log_file_path, "WorldCount") == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't create count log|"));
        SAFE_DELETE(count_log_global);
        return false;
    };
    //
    CONSOLELOG(eCRITICAL_LOG, _T("create game log"));
    ;{
        __READ_CONFIG_NUMBER(log_option, "LOGFILE", "WOPS_LOGFILE_OPTION",
                             0, WORLD_SERVER_INFO_FILE_NAME);
        __READ_CONFIG_STRING(log_file_path, "LOGFILE", "WOPS_LOGFILE_PATH",
                             "..\\Log", WORLD_SERVER_INFO_FILE_NAME);
    };
    if (log_option == 0)
    {
        CONSOLELOG(eCRITICAL_LOG, _T("    log option = 'don't write game log'"));
        return true;
    };

    if (_access(log_file_path, 0) == -1)
    {
        if (::CreateDirectory(log_file_path, NULL) == false)
        {
            SUNLOG(eCRITICAL_LOG,
                _T("|["__FUNCTION__"]|Msg=can't create gamelog directory = '%s'|"), log_file_path);
            return false;
        };
    };

    char full_bin_path[1024];
    char full_text_path[1024];
    _snprintf(full_bin_path, _countof(full_bin_path), "%s\\\\WORLD", log_file_path);
    _snprintf(full_text_path, _countof(full_text_path), "%s\\\\WORLD_TEXT", log_file_path);

    ns_world::world_log = new ns_world::WorldLog;
#ifdef __NA_0_ADD_VERSION_TO_LOG_FILE_NAME
    char file_name[MAX_PATH];
    _sntprintf(file_name, _countof(file_name), "%d_SUN_WORLD", NATION_BUILD_VERSION);
#else
    char* file_name = "SUN_WORLD";
#endif
    if (ns_world::world_log->Init(log_option, file_name, full_bin_path, full_text_path) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't create game_log, ")
            _T("path='%s', file_name='%s'|"), full_bin_path, file_name);
        SAFE_DELETE(ns_world::world_log);
        return false;
    };
    return true;
}

//==================================================================================================

void ns_world::WorldServer::StartServer()
{
    static bool initialized_s = false;
    ns_world::WorldServer* const this_ = ns_world::WorldServer::Instance();
    __TOUCHED(this_);
    if (initialized_s == false)
    {
        ns_world::WorldChannelManager* channel_manager = ns_world::WorldChannelManager::Instance();
        if (FlowControl::FCAssert(channel_manager->LoadFieldInfo() != false) == false) {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't load field info|"));
        };
#if WORLD_CONTENTS_BLOCK
    #ifdef _APPLY_NHN_FILTERFING
        int result = IAF::LoadAbuseDB();
        // error handling?
        if (result != 0)
        {
            SUNLOG(eCRITICAL_LOG, "Fail to load IAF LoadAbuseDB %d", result);
        };
    #endif
#endif
        ns_world::FriendInvitationInWorld::Instance()->StartService();
        initialized_s = true;
    };

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    ;{
        MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF msg_ntf;
        if (ns_world::ServerSessionInWorld* session = this_->GetMasterServer())
        {
            session->SendPacket(&msg_ntf, sizeof(msg_ntf));
        };
    };
#endif
}


void ns_world::WorldServer::UpdateConnectSessionInfo(const MSG_MW_SERVER_INFO_CMD& msg_cmd)
{
    server_key_ = msg_cmd.ServerKey;
    this->UpdateAppTitle();
    //
    strncpy(connect_data_.dbproxy_ip_address, msg_cmd.m_szGameDBProxyServerIP,
            _countof(connect_data_.dbproxy_ip_address));
    connect_data_.dbproxy_ip_address[_countof(connect_data_.dbproxy_ip_address) - 1] = '\0';
    connect_data_.dbproxy_port = msg_cmd.m_wGameDBProxyServerPort;

    this->SetListenIP_s(msg_cmd.m_szWorldServerInnerIP);
    this->SetListenPort_s(msg_cmd.m_wWorldServerInnerPort);
    //
    if (connect_sessions_.dbproxy_server == NULL)
    {   // base interface
        this->InitConnectSession(GAME_DBPROXY,
            connect_data_.dbproxy_ip_address, connect_data_.dbproxy_port);
    };
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
#else
    if (this->IsServerInfoFromFile() == false) {
        this->Listen(SERVER_IOHANDLER);
    };
#endif
}

//==================================================================================================
// ServerFrame interface
BOOL ns_world::WorldServer::KeyEvent(char ch)
{
    switch (ch)
    {
    case 'N':
        options_.use_file_notice_function = ns_world::internal::LoadUseFileNoticeFunction();
        break;
    case 0x1B:
        if (sequential_shutdown_service_)
        {
            sequential_shutdown_service_->Shutdown(sequential_shutdown_service_);
            sequential_shutdown_service_ = NULL;
        };
        return true;
    };

    return true;
}
