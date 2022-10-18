#include "stdafx.h"
#include ".\AuthFrame.h"

#include "ServerGlobal.h"
#include <ServerSession.h>
#include <TempServerSession.h>

#include <AuthProtocol_FS.h>
#include <HanAuth/HanAuthQueryManager.h>

#include "RTTAServerSession.h"
#include "AuthAgentServerSession.h"
#include "LoginFrontServerSession.h"

#ifdef __NA_20100201_INTERGRATEDBILLING_
#include "IntegratedBillingServerSession.h"
#else
#include "BillingServerSession.h"
#endif

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
#include "GCCertificationHelper.h"
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
#include "IngambaAuthConnector.h"
#endif

#include "QueryManager.h"
#include "AccountManager.h"
#include "ServerDataManager.h"
#include "AlertRequestSender.h"
#include "QueryStruct.h"
#include "ReAuthManager.h"
#include "Query_Interfaces.h"

// 임시
// Global Member
TCHAR
g_LoginErrorString[AUTHRESULT_MAX][MAX_AUTHRESULT_MESSAGE_LEN + 1] = { 0, };

NetworkObject*
AuthFrame::DoAllocServerSession()
{
	return (NetworkObject*)AuthFrame::Instance()->AllocServerSession(TEMP_SERVER);
}

VOID
AuthFrame::DoFreeServerSession(NetworkObject* pNetObject)
{
	AuthFrame::Instance()->FreeServerSession((ServerSession*)pNetObject);
}

VOID
AuthFrame::DoFreeConnectSession(NetworkObject* pNetObject)
{
	//AuthFrame::Instance()->FreeServerSession((ServerSession*)pNetObject);
}

AuthFrame::AuthFrame()
    : m_pRTTAServerSession(NULL)
    , m_pBillingServerSession(NULL)
    , m_pPoolRouter(NULL)
#ifdef _RU_INGAMBA_AUTH_PROCESS
    , ingamba_auth_connector_(NULL)
#endif
{

	::ZeroMemory(&m_VerProtocol, sizeof(m_VerProtocol));
	::ZeroMemory(&m_VerLauncher, sizeof(m_VerLauncher));
	::ZeroMemory(&m_VerMajor, sizeof(m_VerMajor));
	::ZeroMemory(&m_VerMinor, sizeof(m_VerMinor));

	m_pPoolRouter = new SERVER_SESSION_ROUTER;
	m_pPoolRouter->Register<LoginFrontServerSession>(LOGIN_FRONT_SERVER);
	m_pPoolRouter->Register<AuthAgentServerSession>(AUTH_AGENT_SERVER);
	m_pPoolRouter->Register<RTTAServerSession>(OP_SERVER);    
#ifdef __NA_20100201_INTERGRATEDBILLING_
    m_pPoolRouter->Register<IntegratedBillingServerSession>(BILLING_SERVER);
#else
    m_pPoolRouter->Register<BillingServerSession>(BILLING_SERVER);
#endif    
	m_pPoolRouter->Register<TempServerSession>(TEMP_SERVER);

	m_pPoolRouter->Initialize(LOGIN_FRONT_SERVER, 10, 5);
	m_pPoolRouter->Initialize(AUTH_AGENT_SERVER, 10, 5);
	m_pPoolRouter->Initialize(OP_SERVER, 10, 5);	
    m_pPoolRouter->Initialize(BILLING_SERVER, 1, 1);
	m_pPoolRouter->Initialize(TEMP_SERVER, 10, 5);

#ifdef __NA_20100201_INTERGRATEDBILLING_
    billing_ipc_ = new util::WxMessageIpc(\
        1024 * 1024, 1024, sizeof(BillingMsg));
#endif
}

AuthFrame::~AuthFrame()
{
#ifdef __NA_20100201_INTERGRATEDBILLING_
    SAFE_DELETE(billing_ipc_);
#endif
	SAFE_DELETE(m_pPoolRouter);
}

#if WAVERIX_AUTH_TEST_CODE
static ITimerBase hanAuthQuery_Updater;
#endif

BOOL
AuthFrame::Init()
{
    SUNLOG(eCRITICAL_LOG, "");

    if(!CreateSunLog())
    {
        DISPMSG(" Create SunLog Failed..");
        return FALSE;
    }

    const char* config_file_name = GetDescFileName();
    __READ_CONFIG_NUMBER(m_BlockTime, "ATTRIBUTES",
                         "ACCOUNT_BLOCK_TIME", 30, config_file_name);

    __READ_CONFIG_NUMBER(m_BlockLimit, "ATTRIBUTES",
                         "ACCOUNT_BLOCK_LIMIT", 3, config_file_name);

    __READ_CONFIG_NUMBER(m_AuthLimitTime, "ATTRIBUTES",
                         "AUTH_LIMIT_TICK", 180000, config_file_name);

    __READ_CONFIG_NUMBER(m_AuthKey, "ATTRIBUTES",
                         "AUTH_KEY", 1, config_file_name);

    __READ_CONFIG_NUMBER(m_RecoverTimeout, "ATTRIBUTES",
                         "RECOVER_TIMEOUT", 5000, config_file_name);

    // _PCBANG_POLICY_CHANGE_POST_EXPIRED 테스트 목적
    __READ_CONFIG_NUMBER(m_ChangePcRoomBillingStatus, "ATTRIBUTES",
                         "CHANGE_PC_BILLING_STATUS", 0, config_file_name);
    // (CHANGES) (f100531.3L) change an authentication information loader \
    // to support an authentication information enable loaded from a configuration file 
    ZeroMemory(m_DBInfo, sizeof(m_DBInfo));
    DB_INFO* db_Info = 0;
    db_Info = &m_DBInfo[DB_TYPE_AUTH];
    __READ_CONFIG_STRING(db_Info->m_szIpPort, "AuthDBInfo", "DB_IpPort", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szInitDB, "AuthDBInfo", "DB_InitDB", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szID, "AuthDBInfo", "DB_ID", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szPW, "AuthDBInfo", "DB_PWD", "", config_file_name);

    db_Info = &m_DBInfo[DB_TYPE_LOG];
    __READ_CONFIG_STRING(db_Info->m_szIpPort, "LogDBInfo",  "DB_IpPort", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szInitDB, "LogDBInfo",  "DB_InitDB", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szID, "LogDBInfo",  "DB_ID", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szPW, "LogDBInfo",  "DB_PWD", "", config_file_name);

    db_Info = &m_DBInfo[DB_TYPE_RTT];
    __READ_CONFIG_STRING(db_Info->m_szIpPort, "RTTDBInfo",  "DB_IpPort", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szInitDB, "RTTDBInfo",  "DB_InitDB", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szID, "RTTDBInfo",  "DB_ID", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szPW, "RTTDBInfo",  "DB_PWD", "", config_file_name);

    db_Info = &m_DBInfo[DB_TYPE_NETCAFE];
    __READ_CONFIG_STRING(db_Info->m_szIpPort, "NetCafeDBInfo", "DB_IpPort", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szInitDB, "NetCafeDBInfo", "DB_InitDB", "", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szID, "NetCafeDBInfo","DB_ID", "sun_ncf_login", config_file_name);
    __READ_CONFIG_STRING(db_Info->m_szPW, "NetCafeDBInfo","DB_PWD", "logsun40!!net",config_file_name);

    // 에러메세지 로딩 STRING_FILENAME
    char keyName[16] = { 0, };
    for(int i = 0; i < AUTHRESULT_MAX; ++i)
    {
        itoa(i, keyName, 10); // 10이라는 수치 재검증할 것
        GetPrivateProfileString("LOGINERROR", keyName, "Unknown Error",
            g_LoginErrorString[i], MAX_AUTHRESULT_MESSAGE_LEN, STRING_FILENAME);
        g_LoginErrorString[i][MAX_AUTHRESULT_MESSAGE_LEN] = '\0';
    }

    GetReadyState().Init(AUTH_READY_MAX);

//#ifdef _PAAS_ALERT
//    CAlertRequestSender::Instance()->Create(config_file_name);
//#endif

    ServerDataManager::Instance()->Init();

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    NORTHA_DEBUGLOG("CreateInstance()\n");
    // (WAVERIX) (090722) (NOTE) 유저 관련된 정보를 'HanAuthQueryManager::Init'에서 수행하지 말 것.
    nsHanAuth::HanAuthQueryManager::CreateInstance();
    BOOLEAN hanAuthInitResult =                     //service code를 넣지 않으면 u_sun.xml로딩
        nsHanAuth::HanAuthQueryManager::Init("u_sun", 0/*SERVICE_KOR|SERVICE_ALPHA*/);
    if(hanAuthInitResult == 0)
    {
        SUNLOG(eCRITICAL_LOG, "HanGame Module Loading Failed!");
        if(GetMode() != eSERVER_DEV)
            return FALSE;
    }
    NORTHA_DEBUGLOG("nsHanAuth::HanAuthQueryManager::Init %u\n", hanAuthInitResult);

  #if WAVERIX_AUTH_TEST_CODE
    hanAuthQuery_Updater.Configure(ITimerBase::LOOP, 1 * 1000);
  #endif
#endif
    //  DB를 활성화시킨다.
    int dbc_num = 0;
    __READ_CONFIG_NUMBER(dbc_num, "ATTRIBUTES", "DBC_NUM", 12, config_file_name);

    {
        QueryManager* pQueryManager = QueryManager::Instance();
        pQueryManager->Create(dbc_num);

        pQueryManager->QueryGameVersion();
        pQueryManager->QueryControlIpList();
    };

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    // 1st Instance & Randomize
    ReAuthManager::Instance()->Randomize();
#endif
    AccountManager::Instance()->Initialize();

    ReloadAllowedSetting();
    m_AllowedStsTimer.SetTimer(MAX_ALLOWEDSTS_RELOAD_DELAY_TIME);

//#ifdef _SHAPE_AUTH
//    DWORD dwReloadSATime = 0;
//    __READ_CONFIG_NUMBER(dwReloadSATime, "ATTRIBUTES", "SHAPE_AUTH_RELOAD_TIME",
//                         3600000, config_file_name);
//    m_ReloadSATimer.SetTimer(dwReloadSATime);
//#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    if(!GCCertificaltionHelper::Instance()->Load_GamechuDLL())
    {
        SUNLOG(eCRITICAL_LOG, "Japan GameChu Module Loading fail");
        __asm { int 3 };
    }
#endif

#ifdef _JP_0_20101123_PMS
    SERVER_KEY key = GetKey();
    key.Set(0,0,GetServerType(),0);
    SetKey(key);
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
    if (GetMode() == eSERVER_LIVE)
    {
        ingamba_auth_connector_ = new IngambaAuthConnector;
    }
#endif

    return TRUE;
}

#include <AuthStepAuthToken.h>

BOOL
AuthFrame::Update(DWORD tick)
{
//#ifdef	_SHAPE_AUTH
//    if(m_ReloadSATimer.IsExpired())
//        ReloadShapeImage();
//#endif

    QueryManager::Instance()->Update();

    AccountManager::Instance()->Update();

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    ReAuthManager::Instance()->Update();
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    nsHanAuth::HanAuthQueryManager::Update(&HanAuthQueryCallback_Global);

//#if WAVERIX_AUTH_TEST_CODE
//    if(hanAuthQuery_Updater.IsExpired())
//    {
//        using namespace nsHanAuth;
//        QuerySlot slot;
//        slot.SetInfo(INVALID_INDEX_ID, 1, 1, "10.101.12.110", "10.101.12.110");
//        INDEX_ID_T queryIndex = HanAuthQueryManager::AllocSlot(slot);
//        HanAuthQueryManager::QueryAuthentication(queryIndex, "MemberId", "AuthStringBlock");
//    }
//#endif
#endif
#ifdef __NA_20100201_INTERGRATEDBILLING_
    if (ServerSession* server_session = GetSession(BILLING_SERVER))
    {
        util::IWxMainMessageHandler* handler = \
            static_cast<IntegratedBillingServerSession*>(server_session);
        billing_ipc_->GetMainLogicHandler().Update(handler);
    };
#endif
    g_pSunLog->Update();

    return TRUE;
}

void
AuthFrame::Release()
{
#ifdef __NA_20100201_INTERGRATEDBILLING_
    billing_ipc_->StopWorking();
    SAFE_DELETE(billing_ipc_);
#endif
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    ReAuthManager::Instance()->DestroyInstance();
#endif
    QueryManager::Instance()->Destroy();        QueryManager::DestroyInstance();
    AccountManager::Instance()->Destroy();      AccountManager::DestroyInstance();
    ServerDataManager::Instance()->Destroy();   ServerDataManager::DestroyInstance();

#ifdef _RU_INGAMBA_AUTH_PROCESS
    if (GetMode() == eSERVER_LIVE)
    {
        SAFE_DELETE(ingamba_auth_connector_);
    }
#endif

//#ifdef _PAAS_ALERT
//    CAlertRequestSender::DestroyInstance();
//#endif

    SAFE_DELETE(g_pSunLog);
}

BOOL
AuthFrame::IsRTTAServerConnected()
{
    if(m_pRTTAServerSession && m_pRTTAServerSession->IsConnected())
        return TRUE;
    return FALSE;
}

BOOL
AuthFrame::IsBillingConnected()
{
    if(m_pBillingServerSession && m_pBillingServerSession->IsConnected())
        return TRUE;
    return FALSE;
}

BOOL
AuthFrame::SendRTTAServer(MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
    if(!m_pRTTAServerSession)
        return FALSE;
    return m_pRTTAServerSession->SendPacket(pMsg, wSize);
}

BOOL
AuthFrame::SendToSession(DWORD idx, MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
    return ServerFrame::SendToSession(idx, (MSG_BASE*)pMsg, wSize);
}

VOID
AuthFrame::SendToSessionType(eSERVER_TYPE type, MSG_BASE_INTERNAL* pMsg, WORD wSize, DWORD dwExceptIndex)
{
    ServerFrame::SendToSessionType(type, pMsg, wSize, dwExceptIndex);
}

ServerSession*
AuthFrame::AllocServerSession(eSERVER_TYPE type)
{
    ServerSession* pServerSession = (ServerSession*)m_pPoolRouter->Alloc(type);
    switch(type)
    {
    case OP_SERVER:
        m_pRTTAServerSession = (RTTAServerSession*)pServerSession; break;
    case BILLING_SERVER:
#ifdef __NA_20100201_INTERGRATEDBILLING_
        m_pBillingServerSession = (IntegratedBillingServerSession*)pServerSession;
        billing_ipc_->StartWorking(billing_ipc_->eReacting_Medium, NULL);
        break;
#else
        m_pBillingServerSession = (BillingServerSession*)pServerSession; break;
#endif
    }
    if(pServerSession) 
        pServerSession->Init(this);
    return pServerSession;
}

void
AuthFrame::FreeServerSession(ServerSession* pSession)
{
    eSERVER_TYPE type = pSession->GetServerType();
    switch(type)
    {
    case OP_SERVER:
        m_pRTTAServerSession = NULL; break;
    case BILLING_SERVER:
    #ifdef __NA_20100201_INTERGRATEDBILLING_
        billing_ipc_->StopWorking();
    #endif
        m_pBillingServerSession = NULL;
        break;
    }
    pSession->Release();
    m_pPoolRouter->Free(type, pSession);
}

BOOL
AuthFrame::KeyEvent(char ch)
{
    switch(ch)
    {
    case ' ': 
        DisplayServerInfo();
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        SUNLOG(eCRITICAL_LOG, "Current ReAuthSlot Information : Remained = %u",
               ReAuthManager::Instance()->NumberOfFreeReAuthKey());
    #endif
        break;
    case 'P':
        DisplayControlIPInfo();
        break;
    case 'S':
        ServerDataManager::Instance()->Display();
        break;
    case 'A':
        AccountManager::Instance()->DisplayDetailInfo();
        break;
    case 'T':
        {
            QueryManager* query_manager = QueryManager::Instance();
            query_manager->QueryPCRoomIPList(182);
        }
        break;
    }

    return TRUE;
}

VOID
AuthFrame::DisplayServerInfo()
{
    SUNLOG(eCRITICAL_LOG, "Alloc Query Counts : %d", QueryManager::Instance()->GetAllocQueryCount());

    AccountManager::Instance()->DisplayInfo();
    DisplayVersionInfo();
}

VOID
AuthFrame::DisplayVersionInfo()
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

VOID
AuthFrame::DisplayControlIPInfo()
{
    //SUNLOG(eCRITICAL_LOG, "\n");

    ListControlIp::iterator it = m_listControlIp.begin(),
                            end = m_listControlIp.end();
    for( ; it != end; ++it)
    {
        CONTROL_IP_INFO* pcIPInfo = *it;

        SUNLOG(eCRITICAL_LOG, "IPType[%d] %s + Count %d",
               pcIPInfo->m_IPType,
               IPOperation::ConvertDWORD2StrIP(pcIPInfo->m_StartIP),
               pcIPInfo->m_Count);
	}
}

ServerSession*
AuthFrame::GetSession(eSERVER_TYPE type)
{
    switch(type)
    {
    case OP_SERVER:
        return m_pRTTAServerSession;
    case BILLING_SERVER:
        return m_pBillingServerSession;
    }
    return NULL;
}

BOOL
AuthFrame::ConnectTo(eSERVER_TYPE type)
{
    ServerSession* pSession = NULL;
    switch(type)
    {
    case OP_SERVER:
        pSession = m_pRTTAServerSession; break;
    case BILLING_SERVER:
        pSession = m_pBillingServerSession; break;
    }

    if(!pSession) return FALSE;


    if(pSession->IsConnecting() || pSession->IsConnected())
    {
        ASSERT(!"중복 Connect 시도");
        return FALSE;
    }

    Connect(pSession);

    return TRUE;
}

#ifdef _SECURE_AUTH
void
AuthFrame::SendMatrixCardAuth(LoginFrontServerSession* pSession,
                              AccountInfo* pAccount,
                              DWORD dwFrontUserID,
                              BYTE btMatrixAuthFailCount,
                              BYTE bFirst)
{
    if(NULL == pSession)
    {
        SUNLOG(eFULL_LOG, "[AuthFrame::SendMatrixCardAuth] LoginFrontServerSession is NULL\n");
        return;
    }

    if(NULL == pAccount)
    {
        SUNLOG(eFULL_LOG, "[AuthFrame::SendMatrixCardAuth] AccountInfo is NULL\n");
        return;
    }

    pAccount->m_btMatrixAuthFailCount = btMatrixAuthFailCount;

    MSG_SF_AUTH_MATRIX_CARD_SYN	msg;
    msg.m_dwFrontUserID = dwFrontUserID;
    msg.m_byResult = bFirst;		// 1이면 처음, 0이면 실패후 재인증
    msg.m_SecureInfo.dwType = eSC_SCARD;

    // 보안 인증 체크할 좌표 번호들 세팅
    SC_INFO* pData = (SC_INFO*)(msg.m_SecureInfo.byData);

    srand(::GetTickCount());

    SC_INFO& matrixInfo = pAccount->m_secureAuthInfo;
    matrixInfo.wMatrix[0] = pData->wMatrix[0] = rand() % 81;
    matrixInfo.wMatrix[1] = pData->wMatrix[1] = rand() % 81;
    matrixInfo.wMatrix[2] = pData->wMatrix[2] = rand() % 81;

    pSession->SendPacket(&msg, sizeof(msg));

    if(bFirst)
    {
        SUNLOG(eFULL_LOG,
               "[AuthFrame::SendMatrixCardAuth] Send SCAuth Msg[%u,%u,%u]\n",
               pData->wMatrix[0], pData->wMatrix[1], pData->wMatrix[2]);
    }
    else
    {
        SUNLOG(eFULL_LOG,
               "[AuthFrame::SendMatrixCardAuth] Matrix Auth Failed. Send SCAuth Msg[%u,%u,%u]\n",
               pData->wMatrix[0], pData->wMatrix[1], pData->wMatrix[2]);
    }
}
#endif

//#ifdef _SHAPE_AUTH
//void
//AuthFrame::SendShapeAuthImage(LoginFrontServerSession* pSession,
//                              AccountInfo* pAccount,
//                              DWORD dwFrontUserID,
//                              BYTE btShapeAuthFailCount,
//                              BYTE bFirst)
//{
//    if(NULL == pSession)
//    {
//        SUNLOG(eFULL_LOG, "[AuthFrame::SendShapeAuthImage] LoginFrontServerSession is NULL\n");
//        return;
//    }
//
//    if(NULL == pAccount)
//    {
//        SUNLOG(eFULL_LOG, "[AuthFrame::SendShapeAuthImage] AccountInfo is NULL\n");
//        return;
//    }
//
//    IShapeAuth* pSA = AuthFrame::Instance()->_GetShapeMgrPtr()->get();
//
//    if(NULL == pSA)
//    {
//        SUNLOG(eFULL_LOG, "[AuthFrame::SendShapeAuthImage] Create Shape Auth Instance Failed\n");
//        return;
//    }
//
//    SA_AUTH_DATA	SA_Data;
//    memset(&SA_Data, 0, sizeof(SA_AUTH_DATA));
//
//    if(0 != pSA->GetSAData(&SA_Data))
//    {
//        SUNLOG(eFULL_LOG, "[AuthFrame::SendShapeAuthImage] GetSAData() Failed\n");
//        return;
//    }
//
//    if(bFirst)
//    {
//        pAccount->m_bSucceedAuth = true; // 계정인증 통과
//    }
//
//    pAccount->m_btShapeAuthFailCount = btShapeAuthFailCount;
//
//    memset(pAccount->m_SA_Value, 0, SA_VALUE_SIZE);
//    ::_tcsncpy(pAccount->m_SA_Value, SA_Data.Value, SA_VALUE_SIZE);		// 라이브러리로부터 넘겨받은 형상 이미지의 결과값 저장
//
//    MSG_SF_AUTH_SHAPE_SYN sendPacket;		// 형상 이미지 데이터 전송 패킷
//
//    sendPacket.m_dwFrontUserID	= dwFrontUserID;
//    sendPacket.m_btResult		= bFirst;							// 형상 인증이 처음인지 재시도인지 여부
//    sendPacket.m_btAuthFailCount= btShapeAuthFailCount;				// 인증 연속 오류 횟수
//    sendPacket.m_btDataSts		= 0;								// 형상 이미지 전송을 시작함(데이터는 없다)
//    sendPacket.m_dwShapeDataSize= SA_Data.ImageSize;				// 형상 이미지의 총 크기
//
//    // Sts 0, 형상 이미지 데이터 전송 시작함
//    pSession->SendPacket(&sendPacket, sizeof(sendPacket) - SHAPE_DATA_MAX_LEN);
//
//    // Sts 1, 실제 이미지 데이터 전송
//    sendPacket.m_btDataSts		= 1;		// 이제부터 형상 이미지 데이터 전송
//
//    DWORD dwOffset = 0;
//
//    // 최대 크기보다 크면 잘라서 보낸다
//    while(SA_Data.ImageSize - dwOffset > SHAPE_DATA_MAX_LEN)
//    {
//        memcpy(sendPacket.m_szShapeData, SA_Data.ImageData + dwOffset, SHAPE_DATA_MAX_LEN* sizeof(BYTE));
//        sendPacket.m_dwShapeDataSize	= SHAPE_DATA_MAX_LEN;	// 현재 보내는 형상 이미지의 크기
//
//        pSession->SendPacket(&sendPacket, sizeof(sendPacket));
//
//        // 보냈으니 초기화
//        memset(sendPacket.m_szShapeData, 0, SHAPE_DATA_MAX_LEN* sizeof(BYTE));
//        dwOffset += SHAPE_DATA_MAX_LEN;
//    }
//
//    // 남은 데이터 전송
//    if(SA_Data.ImageSize > dwOffset)
//    {
//        memset(sendPacket.m_szShapeData, 0, SHAPE_DATA_MAX_LEN* sizeof(BYTE));
//        sendPacket.m_dwShapeDataSize = SA_Data.ImageSize - dwOffset;		// 맨 마지막 보내는 형상 이미지의 크기
//        memcpy(sendPacket.m_szShapeData, SA_Data.ImageData + dwOffset, SA_Data.ImageSize - dwOffset);
//
//        pSession->SendPacket(&sendPacket, sizeof(sendPacket) - (SHAPE_DATA_MAX_LEN - (sendPacket.m_dwShapeDataSize* sizeof(BYTE))));
//    }
//
//    // Sts 2 형상 이미지 데이터 전송 끝
//
//    sendPacket.m_btDataSts		= 2;	// 형상 이미지 전송을 끝냄(데이터는 없다)
//    sendPacket.m_dwShapeDataSize= 0;
//
//    pSession->SendPacket(&sendPacket, sizeof(sendPacket) - SHAPE_DATA_MAX_LEN);
//
//    if (0 != pSA->FreeSAData(&SA_Data))		// GetSAData()로 부터 얻은 구조체 소멸
//    {
//        SUNLOG(eCRITICAL_LOG, "[SendShapeAuthImage] FreeSAData() Failed\n");
//    }
//}
//#endif //_SHAPE_AUTH

VOID
AuthFrame::SendGameVersion_ToLoginFront(DWORD dwServerIndex)
{
	if(!m_VerProtocol.IsValid())	return;
	if(!m_VerLauncher.IsValid())	return;
	if(!m_VerMajor.IsValid())		return;
	if(!m_VerMinor.IsValid())		return;

	MSG_SF_AUTH_GAME_VERSION_NTF sendMsg;

	::CopyMemory(&sendMsg.m_verProtocol, GetVerProtocolPtr(), sizeof(GAME_VERSION));
	::CopyMemory(&sendMsg.m_verLauncher, GetVerLauncherPtr(), sizeof(GAME_VERSION));
	::CopyMemory(&sendMsg.m_verMajor, GetVerMajorPtr(), sizeof(GAME_VERSION));
	::CopyMemory(&sendMsg.m_verMinor, GetVerMinorPtr(), sizeof(GAME_VERSION));

	if(dwServerIndex)	SendToSession(dwServerIndex, &sendMsg, sizeof(sendMsg));
	else				SendToSessionType(LOGIN_FRONT_SERVER, &sendMsg, sizeof(sendMsg));
}

VOID
AuthFrame::AddControlIp(DB_CONTROL_IP_INFO* pDBIPInfo)
{
	CONTROL_IP_INFO* pcIPInfo = new CONTROL_IP_INFO;

	pcIPInfo->m_IPType = (eIPTYPE)pDBIPInfo->ti_Type;
	pcIPInfo->m_StartIP	= (DWORD)pDBIPInfo->i_IPAddress;
	pcIPInfo->m_Count = (WORD)pDBIPInfo->i_IPAddressCount;

	SUNLOG(eCRITICAL_LOG, "[AddControlIp] IPType[%d] %s + Count %d",
           pcIPInfo->m_IPType,
           IPOperation::ConvertDWORD2StrIP(pcIPInfo->m_StartIP),
           pcIPInfo->m_Count);

	m_listControlIp.push_back(pcIPInfo);
}

VOID
AuthFrame::ReleaseControlIp()
{
	while(m_listControlIp.empty() == false)
	{
		CONTROL_IP_INFO* pcIPInfo = m_listControlIp.front();

		delete pcIPInfo;
		m_listControlIp.pop_front();
	}
}

VOID
AuthFrame::SendControlIp_ToLoginFront(DWORD dwServerIndex)
{
    MSG_SF_AUTH_CONTROL_IPLIST_NTF sendMsg;

    // m_Flag를 0으로 보내 LoginFront에서 IP갱신 준비를 시작하도록 한다.
    sendMsg.m_Flag = 0;
    sendMsg.m_Count = 0;
    if(dwServerIndex)	SendToSession(dwServerIndex, &sendMsg, sendMsg.GetSize());
    else				SendToSessionType(LOGIN_FRONT_SERVER, &sendMsg, sendMsg.GetSize());

    // IP 목록전송을 전송한다.
    sendMsg.m_Flag = 1;
    int count = 0;
    ListControlIp::iterator it = m_listControlIp.begin(),
                            end = m_listControlIp.end();
    for( ; it != end; ++it)
    {
        CONTROL_IP_INFO* pcIPInfo = *it;
        sendMsg.m_IPDataList[count] = *pcIPInfo;

        ++count;

        if(count == MSG_SF_AUTH_CONTROL_IPLIST_NTF::_MAX_IPDATA_NUM)
        {
            sendMsg.m_Count = count;
            if(dwServerIndex)   SendToSession(dwServerIndex, &sendMsg, sendMsg.GetSize());
            else                SendToSessionType(LOGIN_FRONT_SERVER, &sendMsg, sendMsg.GetSize());
            count = 0;
        }
    }

    if(count > 0)
    {
        sendMsg.m_Count = count;
        if(dwServerIndex)   SendToSession(dwServerIndex, &sendMsg, sendMsg.GetSize());
        else                SendToSessionType(LOGIN_FRONT_SERVER, &sendMsg, sendMsg.GetSize());
    }

    //  종료 패킷을 마지막으로 보낸다.
    sendMsg.m_Flag = 2;
    sendMsg.m_Count = 0;
    if(dwServerIndex)   SendToSession(dwServerIndex, &sendMsg, sendMsg.GetSize());
    else                SendToSessionType(LOGIN_FRONT_SERVER, &sendMsg, sendMsg.GetSize());
}

BOOL
AuthFrame::CreateSunLog()
{
	BYTE bySunLogOption = 0, bySunLogFileLevel = 0;
	TCHAR szSunLogFilePath[256];
	__READ_CONFIG_NUMBER(bySunLogOption, "LOGFILE", "SUN_LOGFILE_OPTION", 0, GetDescFileName());
	__READ_CONFIG_NUMBER(bySunLogFileLevel, "LOGFILE", "SUN_LOGFILE_LEVEL", 0, GetDescFileName());
	__READ_CONFIG_STRING(szSunLogFilePath, "LOGFILE", "SUN_LOGFILE_PATH", "", GetDescFileName());

	g_pSunLog = new CSunLog;
	if(!g_pSunLog->Init(bySunLogOption, bySunLogFileLevel, szSunLogFilePath, "AuthServer"))
		return FALSE;

	g_pSunLog->SetFileOutDelayOption(FALSE);

	return TRUE;
}


bool
AuthFrame::IsAllowSts(USER_STS sts)
{
	//  100이상은 관리자이다.
	if(sts >= USER_STS_GM && sts < USER_STS_MAX)
		return true;

	if(sts < USER_STS_GM)
		return m_allowSts[sts];

	return false;
}

void
AuthFrame::ReloadAllowedSetting()
{
	//  허용  sts값을 설정한다.
    LPTSTR	token;
    TCHAR	buffer[256], dbgStr[1024] = "";
    int		offset;

    ::memset(m_allowSts, 0, 256);
    ::GetPrivateProfileString("AllowSts", "Allowed", "0, 100", buffer, 256, GetDescFileName());

    ::_tcscat(dbgStr, "Allowed Sts = ");

    token = ::_tcstok(buffer, ", ");
    while(token != 0)
    {
        offset = ::_ttoi(token);
        if(offset >= 0 && offset < 256)
            m_allowSts[offset] = true;

        ::_tcscat(dbgStr, token);
        ::_tcscat(dbgStr, ", ");

        token = ::_tcstok(0, ", ");
    }

    SUNLOG(eCRITICAL_LOG, "%s\n", dbgStr);
}

//#ifdef _SHAPE_AUTH
//void
//AuthFrame::ReloadShapeImage()
//{
//    IShapeAuth* sa = m_ShapeMgr.get();
//
//    if (0 != sa->ReloadSA(".\\ShapeAuth.cfg"))
//    {
//        SUNLOG(eCRITICAL_LOG, "[AuthFrame::ReloadShapeImage] ReloadSA() failed\n");
//    }
//    else
//    {
//        SUNLOG(eCRITICAL_LOG, "[AuthFrame::ReloadShapeImage] ReloadSA()\n");
//    }
//}
//#endif








