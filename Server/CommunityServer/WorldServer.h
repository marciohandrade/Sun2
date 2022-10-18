#pragma once
#ifndef WORLDSERVER_H
#define WORLDSERVER_H

#pragma warning(push)
#pragma warning(disable : 4100)
#include <client/3DEngine/3DTerrain/3DTerrain.h>
#pragma warning(pop)

#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

#include "Sessions/ServerSessionInWorld.h"
#include "Services/SequentialShutdownService.h"
//#include "Services/pms/PMSManagerInWorld.h"

#define WORLD_SERVER_INFO_FILE_NAME ".\\WorldServer.ini"
struct MSG_MW_SERVER_INFO_CMD;

//==================================================================================================

namespace ns_world {
;

class WorldScriptParserManager;

//==================================================================================================
//
class WorldServer : public ServerFrame
{
public:
    struct Options {
        ulong statistics_interval; // collect statistics information
        ulong shout_interval; // the shout facility has continual usage interval
        bool use_file_notice_function; // notificaiton to all users using notice.txt
    };
    struct Engine {
        HINSTANCE module_instance;
        CLoadHelper* load_helper;
        FUNC_ContactTerrain instance; // ¸Ê Á¤º¸ ¿¬°á function
    };
    //
    static WorldServer* Instance();
    // callback handlers
    static NetworkObject* CreateServerSideAcceptedObject();
    static void DestroyServerSideAcceptedObject(NetworkObject* network_object);
    static void DestroyServerSideConnectedObject(NetworkObject* network_object);
    // ServerFrame interfaces
    virtual eSERVER_TYPE GetServerType();
    virtual ServerSession* AllocServerSession(eSERVER_TYPE server_type);
    virtual void FreeServerSession(ServerSession* server_session);
    virtual BOOL Init();
    virtual BOOL Update(DWORD delta_tick);
    virtual void Release();
    //
    ePHASE_SHIFT GetPhaseStatus() const;
    void SetPhaseStatus(ePHASE_SHIFT phase);
    const SERVER_KEY& GetServerKey() const;
    const Options& GetOptions() const;
    const Engine& GetEngine() const;
    //ns_world::services::PMSManagerInWorld* GetPMSManager();
    ns_world::ServerSessionInWorld* GetGameDBProxy() const;
    ns_world::ServerSessionInWorld* GetMasterServer() const;
    void EnableSessionService(ns_world::ServerSessionInWorld* server_session, bool enabled);

    void UpdateConnectSessionInfo(const MSG_MW_SERVER_INFO_CMD& msg_cmd);
private:
    typedef util::CMemoryPoolFactory<ServerSessionInWorld> ServerSessionPool;
    typedef util::CMemoryPoolFactory<TempServerSession> TempServerSessionPool;
    static const ulong kConnectSessions = (1 << MASTER_SERVER) | (1 << GAME_DBPROXY);
    static const ulong kAcceptSessions = (1 << AGENT_SERVER);
    //
    WorldServer();
    ~WorldServer();
    static WorldServer* CreateInstance();

    static void StartServer();
    bool ReloadAdditionalConfig();

    void InitializeDataFields();
    bool InitializeLogger();
    // ServerFrame interface
    virtual BOOL KeyEvent(char ch);
    //
    bool UpdateServices(const util::ServerTimeInfoPerFrame& time_info);
    void BroadcastNoticeFromFile();
    //----------------------------------------------------------------------------------------------
    // fields...
    uint8_t fields_start_[1];
    //
    eSERVER_MODE server_mode_;
    ePHASE_SHIFT phase_shift_;
    SERVER_KEY server_key_;
    Engine engine_;
    struct ServerSessions {
        ns_world::ServerSessionInWorld* dbproxy_server;
        ns_world::ServerSessionInWorld* master_server;
        bool dbproxy_enabled;
        bool master_enabled;
    } connect_sessions_;
    ServerSessionPool* server_session_pool_;
    TempServerSessionPool* temp_session_pool_;
    ns_world::WorldScriptParserManager* parser_manager_;
    //
    Options options_;
    // extra stored data
    struct {
        uint16_t dbproxy_port;
        char dbproxy_ip_address[MAX_IP_STRING_LEN + 1];
    } connect_data_;
    //
    ns_world::SequentialShutdownService* sequential_shutdown_service_;
    //
    uint8_t fields_end_[1]; // end of data fields.
    //
    //ns_world::services::PMSManagerInWorld pms_manager_; // non-pod object
    // last order...
    static WorldServer* instance_static_;
    //
    __DISABLE_COPY(WorldServer);
};

}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================

inline ns_world::WorldServer* ns_world::WorldServer::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

inline eSERVER_TYPE ns_world::WorldServer::GetServerType() {
    return WORLD_SERVER;
}

inline ePHASE_SHIFT ns_world::WorldServer::GetPhaseStatus() const {
    return phase_shift_;
}

inline void ns_world::WorldServer::SetPhaseStatus(ePHASE_SHIFT phase) {
    phase_shift_ = phase;
}

inline const SERVER_KEY& ns_world::WorldServer::GetServerKey() const {
    return server_key_;
};

inline const ns_world::WorldServer::Options& ns_world::WorldServer::GetOptions() const {
    return options_;
};

inline const ns_world::WorldServer::Engine& ns_world::WorldServer::GetEngine() const {
    return engine_;
};

//inline ns_world::services::PMSManagerInWorld* ns_world::WorldServer::GetPMSManager()
//{
//    return &pms_manager_;
//}

inline ns_world::ServerSessionInWorld* ns_world::WorldServer::GetGameDBProxy() const
{
    if (connect_sessions_.dbproxy_enabled) {
        return connect_sessions_.dbproxy_server;
    };
    return NULL;
}

inline ns_world::ServerSessionInWorld* ns_world::WorldServer::GetMasterServer() const
{
    if (connect_sessions_.master_enabled) {
        return connect_sessions_.master_server;
    };
    return NULL;
}

#endif //WORLDSERVER_H
