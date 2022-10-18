#include "stdafx.h"

#include <Version.h>
#include <ServerBuildVersion.h>

#include <IOCPServer.h>
#include <NetworkObject.h>
#include <PacketControl/PacketControl.h>


//=========================================================================================================
// MasterServer Include Files
//
#include "MasterServerEx.h"

#include "Sessions/ServerSessionEx.h"
#include "Sessions/ServerSessionManager.h"
#include "PacketHandler/PacketHandler.h"
#include "Function/ServerInfoManager.h"
#include "Function/AbuseUserManager.h"
#include <WorkLoad/WorkLoadManager.h>

//
#include <WorkLoad/MasterServer.WorkLoad.cxx>
#include "MasterScriptParserManager.h"

//
//=========================================================================================================

MasterServerEx::MasterServerEx()
{    
    SetPhaseStatus(PHASE_UNKNOWN);

    is_service_mode_ = FALSE;

    ConnectSessions.Init();

    // (CHANGES)(20100311)(hukim) RestrictManager(), GetMasterServerInfo() 자신의 멤버 변수에 
    // 접근하기 위해 접근자를 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체

    //RestrictManager().Clear();
    //GetMasterServerInfo().Init( this );
    restrict_manager_.Clear();
    master_server_info_.Init(this);

    auth_process_work_load_key_ = 0;
    master_script_parser_manager_ = NULL;
}

MasterServerEx::~MasterServerEx()
{
    // ServerFrame에서 알아서 연결된 Session들을 제거할 것이다... 그런가?

    SetPhaseStatus(PHASE_UNKNOWN);

    is_service_mode_ = FALSE;

    ConnectSessions.Init();

    // (CHANGES)(20100311)(hukim) RestrictManager() 자신의 멤버 변수에 접근하기 위해 접근자를 
    // 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체

    //RestrictManager().Clear();
    restrict_manager_.Clear();
    auth_process_work_load_key_ = 0;
}


BOOL MasterServerEx::Init()
{
    nsPacket::Environment::Init();

    InitializeLogger();

    MessageOut(eCRITICAL_LOG, _T("*** MasterServer Start *** "));
    MessageOut(eCRITICAL_LOG, _T("*** MasterServer Server Version = %d ***"), SERVER_RELEASE_BUILD_NO);

    // (서비스/콘솔)모드 제어 확인할 것.
    __READ_CONFIG_NUMBER(is_service_mode_, _T("COMMON"), _T("SERVICE_MODE"), 0, MASTER_SERVER_INFO_FILE_NAME);

    WorkLoadManager::CreateInstance();

    PacketHandler::RegisterHandler();

    ServerInfoManager::CreateInstance();
    ServerSessionManager::CreateInstance();

    InitializeDataLoad();

    ReloadFromFile_RestrictFunction();
    // 통계 수집 대기 시간 저장(기본값은 10초)
    ReloadFromFile_StatisticsInterval();
    ReloadFromFile_LogicViolationControl();

    AbuseUserManager::Instance()->Init();

#ifdef _JP_0_20101123_PMS
    SERVER_KEY key = GetKey();
    key.Set(0,0,GetServerType(),0);
    SetKey(key);
#endif

    return TRUE;
}

VOID MasterServerEx::Release()
{
    WorkLoadManager::DestroyInstance();
    AbuseUserManager::Instance()->Release();    
    AbuseUserManager::DestroyInstance();
    PacketHandler::ReleaseHandler();

    if (master_script_parser_manager_ != NULL)
    {
        master_script_parser_manager_->Release();
        master_script_parser_manager_->DestroyInstanceEx();
        SAFE_DELETE( master_script_parser_manager_);
    }

    MessageOut(eCRITICAL_LOG, _T("*** MasterServer Close *** "));

    SAFE_DELETE(g_pSunLog);
}

VOID MasterServerEx::Finalize()
{
    ServerSessionManager::DestroyInstance();
    ServerInfoManager::DestroyInstance();
}

BOOL MasterServerEx::Update( DWORD /*tick*/ )
{
    // <2> Server Session 관리
    WorkLoadManager::Update();
    AbuseUserManager::Instance()->Update();

    // SunLog Update 위치나 종료시점의 로그 쓰기 등을 고려해서 재설정할 것.
    g_pSunLog->Update();
    g_pSecureCollectLog->Update();

    // (CHANGES)(20100311)(hukim) GetMasterServerInfo() 자신의 멤버 변수에 접근하기 위해 접근자를 
    // 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체
    
    //MasterServerInfo& master_server_info = GetMasterServerInfo();
    //master_server_info.Update();
    master_server_info_.Update();

    return TRUE;
}

VOID MasterServerEx::StartListen()
{
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
    do
    {
        PROCESS_SERVER_AUTHENTICATION* server_auth_work = new PROCESS_SERVER_AUTHENTICATION();
        const WORK_KEY server_auth_work_key = WorkLoadManager::RegisterTask(server_auth_work);
        if (server_auth_work_key == WorkLoadManager::c_NotExistKey)
        {
            MessageOut(eCRITICAL_LOG, _T("Can't Register Server Authentication Routine"));
            return;
        }

        auth_process_work_load_key_ = server_auth_work_key;
    } while(0);

#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
    // DB Proxy에 연결된 후에 Listen 시작?
    Listen(SERVER_IOHANDLER);

    //  // Account DB pxory 세션은 DB 정보를 받은 지금 시점에 서버 세션 매니져에 추가
    //  SERVER_KEY serverKey = ServerInfoManager::Instance()->GetServerKey( ACCOUNT_DBPROXY, (char*)m_pAccountDBProxySession->GetConnectIP().c_str() );
    //  if( !ServerSessionManager::Instance()->FindServer( serverKey ) )
    //  {
    //      // 서버 정보를 업데이트 하는 경우 또 추가하지 않게끔 서버키로 찾아본 후에 추가
    //      m_pAccountDBProxySession->SetServerKey( serverKey );
    //      ServerSessionManager::Instance()->AddServer( m_pAccountDBProxySession );
    //  }
    //
    //  // Op Server 세션은 DB 정보를 받은 지금 시점에 서버 세션 매니져에 추가
    //  SERVER_KEY opServerKey = ServerInfoManager::Instance()->GetServerKey( OP_SERVER, (char*)m_pOpServerSession->GetConnectIP().c_str() );
    //  if( !ServerSessionManager::Instance()->FindServer( opServerKey ) )
    //  {
    //      // Op Server가 연결이 되어있는 경우 추가!
    //      if( m_pOpServerSession->IsConnected() )
    //      {
    //          m_pOpServerSession->SetServerKey( opServerKey );
    //          ServerSessionManager::Instance()->AddServer( m_pOpServerSession );
    //      }
    //  }
    //
    //#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    //  // Solar Auth Server 세션은 DB 정보를 받은 지금 시점에 서버 세션 매니져에 추가
    //  SERVER_KEY solarAuthServerKey = ServerInfoManager::Instance()->GetServerKey( SOLARAUTH_SERVER, (char*)m_pSolarAuthServerSession->GetConnectIP().c_str() );
    //  if( !ServerSessionManager::Instance()->FindServer( solarAuthServerKey ) )
    //  {
    //      // 서버 정보를 업데이트 하는 경우 또 추가하지 않게끔 서버키로 찾아본 후에 추가
    //      m_pSolarAuthServerSession->SetServerKey( solarAuthServerKey );
    //      ServerSessionManager::Instance()->AddServer( m_pSolarAuthServerSession );
    //  }
    //#endif
}

ServerSession* MasterServerEx::AllocServerSession(eSERVER_TYPE server_type)
{
    ServerSessionEx* server_session = 
        static_cast<ServerSessionEx*>(MasterServerEx::Handler_AllocServerSession_NoInit(server_type));

    if (server_session != NULL)
    {
        (static_cast<ServerSession*>(server_session))->Init(this);
    }

    return static_cast<ServerSession*>(server_session);
}

VOID MasterServerEx::FreeServerSession(ServerSession* server_session)
{
    // SolarAuth와 달라진 것은 여기서는 Release를 호출한다는 것임. 문제 없는지 확인해 볼 것.
    MasterServerEx::Handler_FreeServerSession(server_session);
}

#include "PacketHandler\Handler_MO.h"

BOOL MasterServerEx::KeyEvent(CHAR pressed_key)
{
    switch(pressed_key)
    {
    case 'A':
        {
            AbuseUserManager::Instance()->DisplayAbuseInfo();
        }
        break;
    case 'U':
        {
            AccountDBProxySession* adbp_server = ConnectSessions.SessionAccountDB();
            if ((adbp_server != NULL) && adbp_server->IsConnected())
            {
                MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN msg;
                msg.m_byCategory = MX_CONNECTION;
                msg.m_byProtocol = MX_CONNECTION_REQ_SERVER_INFO_SYN;
                adbp_server->Send(reinterpret_cast<BYTE*>(&msg), sizeof(MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN));

                MessageOut(eCRITICAL_LOG, _T("Request Server Information..."));
            }
            else
            {
                MessageOut(eCRITICAL_LOG, _T("Not Connected AccountDBProxy Server."));
            }
        }
        break;
    case 'S':
        break;
    case 'R':
        {
            ReloadFromFile_RestrictFunction();
            if (!IsRestrictFuncFromFile())
            {
                // (CHANGES)(20100311)(hukim) GetMasterServerInfo() 자신의 멤버 변수에 접근하기 위해 접근자를 
                // 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체

                //GetMasterServerInfo().SendMX_FUNCRESTRICT_RELOAD_REQ(this);
                master_server_info_.SendMX_FUNCRESTRICT_RELOAD_REQ(this);
            }
            ReloadFromFile_LogicViolationControl();
        }
        break;
    case 'X':
        {
#ifdef _DEBUG
            BYTE world_id = 0;
            BYTE channel_id = 0xFF;
            BYTE server_type = 0;
            BYTE server_id = 0;
            SERVER_KEY server_exit_key(world_id, channel_id, server_type, server_id);

            EXIT_SERVER_MAP* agent_server_list = NULL;
            EXIT_SERVER_MAP* dbp_server_list = NULL;
            EXIT_SERVER_MAP* server_list = NULL;
            
            // (CHANGES)(20100311)(hukim) GetMasterServerInfo() 자신의 멤버 변수에 접근하기 위해 접근자를 
            // 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체

            //MasterServerInfo& master_server_info = this->GetMasterServerInfo();
            //BOOL is_success = master_server_info.MakeServerExitList(server_exit_key, 
            //                                                        agent_server_list, 
            //                                                        dbp_server_list, 
            //                                                        server_list);
            BOOL is_success = master_server_info_.MakeServerExitList(server_exit_key, 
                                                                     agent_server_list, 
                                                                     dbp_server_list, 
                                                                     server_list);
            if (is_success)
            {
                is_success = master_server_info_.RegisterServerExitProcess(agent_server_list, 
                                                                          dbp_server_list, 
                                                                          server_list);
                if (is_success)
                {
                    //서버 종료는 순차적으로(필드/배틀/에이전트->DB프락시) 이루어져야 하므로, 서버 종료 모드로 들어가서 처리해야 한다
                    MessageOut(
                        eCRITICAL_LOG,
                        _T("정상 종료 프로토콜 수신[W:%u/C:%u/T:%u/I:%u] - 서버군 종료 프로세스에 맞춰 종료 명령을 내리고 있습니다."),
                        server_exit_key.GetWorldID(), 
                        server_exit_key.GetChannelID(),
                        server_exit_key.GetServerType(), 
                        server_exit_key.GetServerID());

                    return TRUE;
                }
            }
#else
            DISPMSG(_T("This Function is enabled in DEBUG Mode\n"));
#endif
        }
        break;
    //__NA001254_20090109_RTTG_FUNC_ADD 
    case 'T':
        {
            Handler_MO::SendTest();
        }
    }

    return TRUE;
}

//============================================================================================================
// Logger Initializer
//

BOOL MasterServerEx::InitializeLogger()
{
    // (CHANGES)(20100311)(hukim) sun_log_file_path 경로 길이가 너무 짧아 버퍼 오버 플로우가 일어날
    // 가능성이 많기 때문에 버퍼 크기 수정

    BYTE sun_log_option = 0;
    BYTE sun_log_file_level = 0;
    CHAR sun_log_file_path[MAX_PATH];

    __READ_CONFIG_NUMBER(sun_log_option, _T("LOGFILE"), _T("SUN_LOGFILE_OPTION"), 0, 
                         MASTER_SERVER_INFO_FILE_NAME); 
    __READ_CONFIG_NUMBER(sun_log_file_level, _T("LOGFILE"), _T("SUN_LOGFILE_LEVEL"), 0, 
                         MASTER_SERVER_INFO_FILE_NAME);
    __READ_CONFIG_STRING(sun_log_file_path, _T("LOGFILE"), _T("SUN_LOGFILE_PATH"), NULL, 
                         MASTER_SERVER_INFO_FILE_NAME);

    DISPMSG("SUNLOG.Master Initializing...\t");

    g_pSunLog = new CSunLog;
    BOOL is_success = g_pSunLog->Init(sun_log_option, sun_log_file_level, sun_log_file_path, _T("Master"));
    if (is_success)
    { 
        DISP_OK; 
    }
    else        
    { 
        DISP_FAIL; 
    }

    DISPMSG("SUNLOG.Secure Initializing...\t");

    g_pSecureCollectLog = new CSunLog;
    is_success = g_pSecureCollectLog->Init(sun_log_option, sun_log_file_level, sun_log_file_path, _T("Secure"));
    if (is_success) 
    { 
        DISP_OK; 
    }
    else        
    { 
        DISP_FAIL; 
    }

    return is_success;
}

VOID MasterServerEx::InitializeDataLoad()
{
    master_script_parser_manager_ = new MasterScriptParserManager;
    master_script_parser_manager_->InitParserPool(NULL);
    master_script_parser_manager_->RegisterParser();

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    master_script_parser_manager_->Load();
#endif
}

VOID MasterServerEx::StartDataService()
{
    MessageOut(eCRITICAL_LOG, _T("All Data Loaded!\n"));

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    MasterServerEx::GetMainFrame()->GetParserManager()->Load();
    // 연결된 모든 서버들에게 PHASE_SHIFT_CMD :: PHASE_EXCHANGE 명령을 보낸다.
    // 연결된 서버들은 데이터 요청할 것이다.

    // 이전 코드 <060802>
    ServerSessionManager::SEND_PHASE_SHIFT_CMD_BROADCAST( PHASE_EXCHANGE, PHASE_SERVICE );


    // 위와 같이 하면, PHASE_SERVICE 상태에 있는 경우라도 일시적으로 PHASE_EXCHANGE로
    // 전환되어 데이터 교환을 할 것이다. 하지만, 모두 읽은 경우라도 지정된 Control Flag만을
    // 읽어들이고 다시 SERVICE 로 전환되게 하자.
    // 마스터가 데이터를 읽어들이기 전에 요청할 경우는 대기 상태가 될 텐데, 이 경우도 적용받게
    // 될 것이다.
    // 이후 별도로 접속하는 경우라면, 개별 PHASE_EXCHANGE 명령을 통해 데이터를 받게 될 것이다.

    // 완료 명령이 올 경우, Field, Battle, DB 등등은 바로 PHASE_SERVICE 로 전환한다.
    // Agent, World 등의 Front 계열 또는 다른 서버들과의 연관도가 있는 경우
    // 늦게 PHASE_SERVICE로 전환시킨다. <- 어떻게 찾아야 하는 지, 감시해야 하는지를 판단할 것.
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

BOOL MasterServerEx::ReloadFromFile_RestrictFunction()
{
    __READ_CONFIG_NUMBER(is_restrict_from_file_, _T("FUNCTIONALRESTRICT"), 
                         _T("GETRESTRICTINFO_FROMFILE"), 0, MASTER_SERVER_INFO_FILE_NAME);
    if (is_restrict_from_file_)
    {
        // (CHANGES)(20100311)(hukim) RestrictManager(), GetMasterServerInfo() 자신의 멤버 변수에 
        // 접근하기 위해 접근자를 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체

        //sRESTRICT_MGR::MakePacket::Read_FUNCRESTRICT_FROM_FILE(&RestrictManager(), TRUE);
        //GetMasterServerInfo().SendGM_OPERATION_FUNCRESTRICT_CMD(this);
        sRESTRICT_MGR::MakePacket::Read_FUNCRESTRICT_FROM_FILE(&restrict_manager_, TRUE);
        master_server_info_.SendGM_OPERATION_FUNCRESTRICT_CMD(this);

        MessageOut(eCRITICAL_LOG, _T("Reload FuncRestrict from DB & Broadcast GameServers"));
    }
    return TRUE;
}

BOOL MasterServerEx::ReloadFromFile_StatisticsInterval()
{
    __READ_CONFIG_NUMBER(statistics_interval_, _T("INTERVAL"), _T("STATISTICS"), 10000, 
                         MASTER_SERVER_INFO_FILE_NAME );
    //GetMasterServerInfo().GetMoneyTimer()->SetTimer( statistics_interval_ );
    
    // (CHANGES)(20100311)(hukim) GetMasterServerInfo() 자신의 멤버 변수에 접근하기 위해 접근자를 
    // 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체

    //GetMasterServerInfo().StartTimerProcess(statistics_interval_);
    master_server_info_.StartTimerProcess(statistics_interval_);

    return TRUE;
}

BOOL MasterServerEx::ReloadFromFile_LogicViolationControl()
{
    // (CHANGES)(20100311)(hukim) GetLogicViolationControl() 자신의 멤버 변수에 접근하기 위해 접근자를
    // 만들어 사용한다. 불필요한 함수 호출이므로 직접 접근으로 대체
    
    //LOGIC_VIOLATION_CONTOL_FLAGS& controller = GetLogicViolationControl();
    //controller.ClearAll();

    logic_violation_control_.ClearAll();

    BOOL is_forced_disconnect_control;
    __READ_CONFIG_NUMBER(is_forced_disconnect_control, _T("LOGIC_VIOLATION_CONTROL"), 
                         _T("FORCED_DISCONNECT"), 0, MASTER_SERVER_INFO_FILE_NAME);
    
    MessageOut(eCRITICAL_LOG, _T("Reload Logic Violation Control"));

    if (is_forced_disconnect_control)
    {
        logic_violation_control_.Set(eLOGIC_VIOLATION_FIELD_INFO::FORCED_DISCONNECT_CONTROL);
    }
        
    return TRUE;
}


//==================================================================================================
//==================================================================================================
//==================================================================================================

