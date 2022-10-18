#include "stdafx.h"
#include ".\loginfrontframe.h"
#include <ServerSession.h>
#include ".\ClientManager.h"
#include "ServerDataManager.h"

#ifdef AUTH_TEST_CODE
#include "AuthProcessTestManager.h"
#endif


NetworkObject*
LoginFrontFrame::DoAllocServerSession()
{
    return (NetworkObject*)LoginFrontFrame::Instance()->AllocServerSession(TEMP_SERVER);
}
VOID
LoginFrontFrame::DoFreeServerSession(NetworkObject* pNetObject)
{
    LoginFrontFrame::Instance()->FreeServerSession((ServerSession*)pNetObject);
}
VOID
LoginFrontFrame::DoFreeConnectSession(NetworkObject* pNetObject)
{
    // nothing to do
    //LoginFrontFrame::Instance()->FreeServerSession((ServerSession*)pNetObject);
}

NetworkObject*
LoginFrontFrame::DoAllocClient()
{ 
    return (NetworkObject*)LoginFrontFrame::Instance()->AllocServerSession(CLIENT_SERVER);
}

VOID
LoginFrontFrame::DoFreeClient(NetworkObject* pNetObject)
{
    LoginFrontFrame::Instance()->FreeServerSession((ServerSession*)pNetObject);
}

VOID
LoginFrontFrame::DoFreeConnect_NotUsed(NetworkObject* pNetObject)
{
    // nothing to do
}

LoginFrontFrame::LoginFrontFrame()
    : m_pAuthServerSession(NULL)
    , m_ServiceControl(eService_Init)
{

}

LoginFrontFrame::~LoginFrontFrame()
{
}

BOOL
LoginFrontFrame::Init()
{
    if(!CreateSunLog())
    {
        DISPMSG(" Create SunLog Failed..");
        return FALSE;
    }

    const char* config_file = GetDescFileName();
    __READ_CONFIG_NUMBER(m_LoginTick, "ATTRIBUTES", "LOGIN_TICK", 60000, config_file);
    __READ_CONFIG_NUMBER(m_LoginFrontID, "ATTRIBUTES", "FRONT_ID", 0, config_file);

    InitServerData(false);

    m_ClientSessionPool.Initialize(20);
    m_AuthServerSessionPool.Initialize(2);

    ClientSession::SetPool(&m_ClientSessionPool);
    AuthServerSession::SetPool(&m_AuthServerSessionPool);

    GetReadyState().Init(FRONT_READY_MAX);

#ifdef _JP_0_20101123_PMS
    SERVER_KEY key = GetKey();
    key.Set(0,0,GetServerType(),0);
    SetKey(key);
#endif

    return TRUE;
}


void
LoginFrontFrame::InitServerData(BOOLEAN reload)
{
    ServerDataManager* const pServerDataManager = ServerDataManager::Instance();
    if(reload == false)
        pServerDataManager->Init();
#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    const char* config_file = GetDescFileName();

    int special_mode_world_id = 0;
    char special_mode_enterable_ip[256] = { 0, }; // (NOTE) to support '__READ_CONFIG_STRING'
    __READ_CONFIG_NUMBER(special_mode_world_id,
                         "ACCEPT_CONTROL", "APPLY_SPECIAL_ENTRY_WORLD", 0, config_file);
    if(special_mode_world_id != 0) {
        __READ_CONFIG_STRING(special_mode_enterable_ip,
                             "ACCEPT_CONTROL", "SPECIAL_ENTERABLE_IP", "", config_file);
        special_mode_enterable_ip[MAX_IP_NAME_LENGTH] = '\0';
    }
    pServerDataManager->ConfigureSpecialModeWorld(special_mode_world_id,
                                                  special_mode_enterable_ip);
    if(special_mode_world_id != 0) {
        SUNLOG(eCRITICAL_LOG, "|MSG=Setup special entry world|World=%d|EnterableIP=%s|",
               special_mode_world_id, special_mode_enterable_ip);
    }
    else {
        SUNLOG(eCRITICAL_LOG, "|MSG=Clear special entry world|",
                special_mode_world_id, special_mode_enterable_ip);
    }
#endif
}

//__CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__
void
LoginFrontFrame::SetServiceControl(eService service)
{
    m_ServiceControl = service;
}

BOOL
LoginFrontFrame::Update(DWORD tick)
{
    //__CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__
    if(m_ServiceControl == eService_Fail)
        return FALSE;

    ClientManager::Instance()->UpdateSession();

    g_pSunLog->Update();

    return TRUE;
}

void
LoginFrontFrame::Release()
{
    ClientManager::DestroyInstance();
    ServerDataManager::Instance()->Destroy();
    ServerDataManager::DestroyInstance();

    SAFE_DELETE(g_pSunLog);
}

BOOL
LoginFrontFrame::SendAuthServer(MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
    if(!m_pAuthServerSession)
        return FALSE;
    return m_pAuthServerSession->Send((BYTE*)pMsg, wSize);
}

BOOL
LoginFrontFrame::IsAuthServerConnected()
{
    if(m_pAuthServerSession && m_pAuthServerSession->IsConnected())
        return TRUE;
    return FALSE;
}

ServerSession*
LoginFrontFrame::AllocServerSession(eSERVER_TYPE type)
{
    ServerSession* pServerSession = NULL;
    switch(type)
    {
    case AUTH_SERVER:
        {
            pServerSession = AuthServerSession::ALLOC();
            m_pAuthServerSession = (AuthServerSession*)pServerSession;
        }
        break;
    case CLIENT_SERVER:
        pServerSession = ClientSession::ALLOC();
        break;
    default: { ASSERT(!"존재하지 않는 서버타입입니다."); return NULL; }
    }

    pServerSession->Init(this);
    return pServerSession;
}

void
LoginFrontFrame::FreeServerSession(ServerSession* pSession)
{
    pSession->Release();
    switch(pSession->GetServerType())
    {
    case AUTH_SERVER:
        {
            AuthServerSession::FREE((AuthServerSession*)pSession); 
            m_pAuthServerSession = NULL;
        }
        break;
    case CLIENT_SERVER:
        ClientSession::FREE((ClientSession*)pSession);
        break;
    default:
        {
            ASSERT(!"서버타입이 존재하지 않습니다.");
        }
        break;
    }
}

#if WAVERIX_AUTH_TEST_CODE
static ClientSession*
__WaverixTest_GetClientSession()
{
    static ClientSession client_session;
    static bool sb_inited = false;
    if(sb_inited == false)
    {
        client_session.Init();
        sb_inited = true;
    }
    return &client_session;
}
#endif

BOOL
LoginFrontFrame::KeyEvent(char ch)
{
    switch(ch)
    {
    case ' ': 
        DisplayServerInfo();
        break;
    case 'C':
    #ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
        InitServerData(true);
    #endif
        break;
    case 'P':
        ClientManager::Instance()->DisplayControlIPInfo();
        break;
    case 'S':
        ServerDataManager::Instance()->Display();
        break;
    case 'T':
        {
    #if WAVERIX_AUTH_TEST_CODE
            NORTHA_DEBUGLOG(__FUNCTION__" Start Test\n");
            static ClientSession* client_session_s = __WaverixTest_GetClientSession();
            client_session_s->OnCF_AUTH_AUTHENTICATION_SYN(0, 0);

    #endif

    #if AUTH_TEST_CODE
            SUNLOG(eCRITICAL_LOG, "====Auth Process Test Start====");
            AuthProcessTestManager::Instance()->Start(AuthProcessTestManager::russia_ingamba);
    #endif
        }
        break;
    }

    return TRUE;
}

VOID
LoginFrontFrame::DisplayServerInfo()
{
    DisplayVersionInfo();
}

VOID
LoginFrontFrame::DisplayVersionInfo()
{
    static char buff[128];
    static std::string versionString;
    versionString.clear();

    versionString = "Protocol";
    {
        GAME_VERSION& version = GetVerProtocol();
        sprintf(buff, "[%d.%d.%d] ", version.m_ClassA, version.m_ClassB, version.m_ClassC);
        versionString += buff;
    };

    {
        GAME_VERSION& version = GetVerLauncher();
        versionString += "Launcher"; 
        sprintf(buff, "[%d.%d.%d] ", version.m_ClassA, version.m_ClassB, version.m_ClassC);
        versionString += buff;
    };

    {
        GAME_VERSION& version = GetVerMajor();
        versionString += "Major"; 
        sprintf(buff, "[%d.%d.%d] ", version.m_ClassA, version.m_ClassB, version.m_ClassC);
        versionString += buff;
    };

    {
        GAME_VERSION& version = GetVerMinor();
        versionString += "Minor"; 
        sprintf(buff, "[%d.%d.%d]", version.m_ClassA, version.m_ClassB, version.m_ClassC);
        versionString += buff;
    };

    SUNLOG(eCRITICAL_LOG, "GameVersion : %s", versionString.c_str());
}

ServerSession*
LoginFrontFrame::GetSession(eSERVER_TYPE type)
{
    switch(type)
    {
    case AUTH_SERVER:
        return m_pAuthServerSession;
    }
    return NULL;
}

BOOL
LoginFrontFrame::ConnectTo(eSERVER_TYPE type)
{
    ServerSession* pSession = NULL;
    switch(type)
    {
    case AUTH_SERVER:
        pSession = m_pAuthServerSession;
        break;
    }

    if(pSession == 0)
        return FALSE;

    if(pSession->IsConnecting() || pSession->IsConnected())
    {
        ASSERT(!"중복 Connect 시도");
        return FALSE;
    }

    Connect(pSession);

    return TRUE;
}

BOOL
LoginFrontFrame::CreateSunLog()
{
    BYTE bySunLogOption = 0,
         bySunLogFileLevel = 0;
    TCHAR szSunLogFilePath[256];

    const char* config_file = GetDescFileName();
    __READ_CONFIG_NUMBER(bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, config_file);
    __READ_CONFIG_NUMBER(bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, config_file);
    __READ_CONFIG_STRING(szSunLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", "", config_file);

    g_pSunLog = new CSunLog;
    if(!g_pSunLog->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "LoginFront"))
        return FALSE;

    g_pSunLog->SetFileOutDelayOption(FALSE);

    return TRUE;
}












