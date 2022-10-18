#include "stdafx.h"
#include "./DummyUser.h"

#if USING_DUMMY_CLIENT

#include <SunAuth/AuthProtocol_GA.h>

#include <PacketHandler/PacketHandler.h>

#include <TimeCheck/ConnectionTimeoutFactory.h>
#include <TimeCheck/ConnectionTimeout.h>
#include <TimeCheck/TimeKeeper.h>

#include <UserSessions/UserManager.h>
#include <Zone/ZoneManager.h>
#include <Zone/CharSelect.h>

#include <DummyClient/DummyClientSession.h>
#include <DummyClient/DummyProtocolHandler.h>

namespace test_unit {
;
//==================================================================================================

struct DummyAgentServer : public AgentServer
{
    IOCPServer*	DummyGetIOCPServer() { return GetIOCPServer(); }
};

//==================================================================================================
// default configuration data

// NOTE: do input your test user guid
const DWORD kuserinfo_user_key = ;
// NOTE: do input your test user id of the user key
const char* kuserinfo_user_id = ;
// NOTE: do input your ip address
const char* kuserinfo_ip_address = ;

// constants
const DWORD kuserinfo_auth_id = kuserinfo_user_key;
const DWORD kuserinfo_object_key = kuserinfo_auth_id;
const char* kuserinfo_birth_date = "111111";
static DummyUser* dummy_user_static_ = NULL;

//==================================================================================================

void StartDummyClient()
{
    if (dummy_user_static_) {
        return;
    }

    dummy_user_static_ = new DummyUser;
    dummy_user_static_->Init();
    if (dummy_user_static_->Start() == false) {
        SAFE_DELETE(dummy_user_static_);
    }
}

void UpdateDummyClient()
{
    if (dummy_user_static_ == NULL) {
        return;
    }
    const int step = dummy_user_static_->configuration_step();
    switch (step)
    {
    case 0:
        return;
    case 1:
        dummy_user_static_->SimulSend_CG_CONNECTION_ENTERSERVER_SYN();
        break;
    case 3:
        dummy_user_static_->SimulSend_CG_CONNECTION_ENTERVILLAGEREQ_SYN();
        break;
    case 5:
        dummy_user_static_->SimulSend_CG_SYNC_PLAYER_ENTER_SYN();
    default:
        break;
    }

    if (step > 100)
    {
        DummyUserProtocolHandler::ActionStep(step, dummy_user_static_);
    }
}

void SendToDummyClient(MSG_BASE* msg_base, WORD msg_size)
{
    const WORD category_and_protocol = MAKEWORD(msg_base->m_byCategory, msg_base->m_byProtocol);
    PACKET_CONTROL::ShowPacketStream(msg_base, msg_size, "A2C");
    switch (category_and_protocol)
    {
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_HEARTBEAT_SYN):
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_TIME_SYNC_CMD):
        break;
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_ENTERSERVER_ACK):
        // 2
        dummy_user_static_->change_configuration_step(3);
        break;
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_ENTERVILLAGEREQ_ACK):
        // 4
        dummy_user_static_->change_configuration_step(5);
        break;
    case MAKEWORD(CG_SYNC, CG_SYNC_PLAYER_ENTER_ACK):
        // 6
        //dummy_user_static_->change_configuration_step(7);
        dummy_user_static_->change_configuration_step(1000);
        break;
    }
    if (dummy_user_static_->configuration_step() > 100) {
        dummy_user_static_->OnRecv(reinterpret_cast<uint8_t*>(msg_base), msg_size);
    };
}

//==================================================================================================

DummyUser::DummyUser()
    : user_session_(NULL)
    , dummy_session_(NULL)
    , dummy_protocol_handler_(NULL)
    , configuration_step_(0)
{
}

DummyUser::~DummyUser()
{
}

//==================================================================================================
// <OBJECT CONTROL>
void DummyUser::Init()
{
    User::Init();
}

void DummyUser::Release()
{
    User::Release();
}

void DummyUser::CopyInfo(ns_object::UserAccountInfo* user_info)
{
    SetAuthID(user_info->KEY_INFO.dwAuthID);
    SetUserGUID(user_info->KEY_INFO.dwUserGUID);
    SetUserKey(user_info->KEY_INFO.dwUserGUID);
    SetPlayerKey(user_info->KEY_INFO.dwObjectKey);
    SetUserID(user_info->ID_INFO.pszUserID);
    SetClientIP(kuserinfo_ip_address);
    SetLastSSNKey((uint8_t*)(kuserinfo_birth_date));
    SetReservedValue(1);
    SetSts(1);
    SetPCRoomSts(1);
    //SetLoginTime(pTempUser->GetLoginTime());
    //SetLoginTime_t(pTempUser->GetLoginTime_t());
}

//==================================================================================================
// <NETWORK OBJECT><INHERITANCE>
void DummyUser::OnRecv(BYTE* msg_ptr, WORD msg_size)
{
    MSG_BASE* msg_base = reinterpret_cast<MSG_BASE*>(msg_ptr);
    this->dummy_protocol_handler_->OnRecv(this, msg_base, msg_size);
}

void DummyUser::OnAccept(DWORD dwNetworkIndex)
{
    printf("|["__FUNCTION__"]|OnAccept|\n");
}

void DummyUser::OnDisconnect()
{
    printf("|["__FUNCTION__"]|OnDisconnect|\n");
}

//==================================================================================================
// <NETWORK OBJECT CONTROL>
BOOL DummyUser::SendPacket(MSG_BASE* msg_base, WORD msg_size)
{
    PacketHandler::GetHandler()->ParsePacketCA(UPT_USER, this, msg_base, msg_size);
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

bool DummyUser::Start()
{
    DummyAgentServer* main_frame = static_cast<DummyAgentServer*>(AgentServer::GetInstance());
    ServerSession* auth_agent_server = main_frame->SessionAuthAgentServer();
    ServerSession* dbproxy_server = main_frame->SessionDBProxyServer();
    if (auth_agent_server == NULL || dbproxy_server == NULL) {
        printf("|Connection Failed|you should be check below contents|\n"
               "|#1. is connected 'auth agent'|\n"
               "|#2. is connected 'dbproxy and field server'?|\n"
               "|#3. is correct input values about a test user information|\n");
        return false;
    }

    dummy_session_ = new DummyClientSession();
    dummy_session_->dummy_user_ = this;

    MSG_AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN msg_syn;
    msg_syn.m_dwAuthUserID = kuserinfo_auth_id;
    msg_syn.m_dwUserGuid = kuserinfo_user_key;
    strncpy(msg_syn.m_szID, kuserinfo_user_id, _countof(msg_syn.m_szID));
    msg_syn.m_szID[_countof(msg_syn.m_szID) - 1] = '\0';
    strncpy(msg_syn.m_szClientIP, kuserinfo_ip_address, _countof(msg_syn.m_szClientIP));
    msg_syn.m_szClientIP[_countof(msg_syn.m_szClientIP) - 1] = '\0';
    msg_syn.m_BillingType.status_ = msg_syn.m_BillingType.eStatus_None;
    ZeroMemory(msg_syn.m_szBillingInfo, sizeof(msg_syn.m_szBillingInfo));
    msg_syn.m_dwReservedValue = 0;
    msg_syn.m_AuthUserType = 0;
    ZeroMemory(msg_syn.m_serialKey, sizeof(msg_syn.m_serialKey));
    ZeroMemory(msg_syn.m_MD5Key, sizeof(msg_syn.m_MD5Key));
    ZeroMemory(msg_syn.m_userCode, sizeof(msg_syn.m_userCode));
    msg_syn.m_userCode[0] = 101;
    //
    PacketHandler::GetHandler()->ParsePacket(AUTH_AGENT_SERVER, auth_agent_server,
                                             &msg_syn, sizeof(msg_syn));
    main_frame->DummyGetIOCPServer()->Connect(SERVER_IOHANDLER, dummy_session_,
        const_cast<char*>(kuserinfo_ip_address), 20002);

    return true;
}

void DummyUser::SimulSend_CG_CONNECTION_ENTERSERVER_SYN()
{
    MSG_CG_CONNECTION_ENTERSERVER_SYN msg_syn;
    msg_syn.m_byNationCode = NATION_CODE;
    strncpy(msg_syn.m_szID, kuserinfo_user_id, _countof(msg_syn.m_szID)); // no-control
    msg_syn.m_dwAuthID = kuserinfo_auth_id;
    ZeroMemory(msg_syn.m_szSerialKey, sizeof(msg_syn.m_szSerialKey));

    msg_syn.m_byHighVerNo = C2S_HIGH_VERSION_NO;
    msg_syn.m_byMiddleVerNo	= C2S_MIDDLE_VERSION_NO;
    msg_syn.m_byLowVerNo = C2S_LOW_VERSION_NO;
    msg_syn.m_dwMOLAProtocolCRC = CryptManager::GetMolaProtocolCRC();
    //msg_syn.Encode();

    Authentication(&msg_syn);
}


bool DummyUser::Authentication(MSG_CG_CONNECTION_ENTERSERVER_SYN* pMsg)
{
    ConnectionTimeout* pUserInfo = \
        dynamic_cast<ConnectionTimeout*>(g_TimeKeeper.PeekTimer(pMsg->m_dwAuthID));
    if(pUserInfo)
    {
        // 접속 시리얼키 비교!!!
        if(0 != strncmp((char*)pUserInfo->GetSerialKey(), (char*)pMsg->m_szSerialKey, AUTH_SERIAL_KEY_LEN))
        {
            SUNLOG(eFULL_LOG, "[Authentication] Auth Serial Key:ClientSendKey is differnt!!");
            pUserInfo->ForceEnd();
            return false;
        }

        this->SetAuthID(pUserInfo->GetAuthUserID());
        this->SetUserGUID(pUserInfo->GetUserGUID());
        this->SetUserKey(pUserInfo->GetUserGUID());            //< USER GUID를 키로 셋팅!
        this->SetUserID(pUserInfo->GetUserID());
// 사정을 따져보자.
//        this->SetClientIP(pUserInfo->GetClientIP());
        this->SetLastSSNKey(pUserInfo->GetLastSSN_Key());
        this->SetBillingType(pUserInfo->GetBillingType()); //_PCBANG_POLICY_CHANGE_POST_EXPIRED
        this->SetReservedValue(pUserInfo->GetReservedValue());
        this->SetAuthUserType(pUserInfo->GetAuthUserType());
        this->SetSts(pUserInfo->GetSts());
        this->SetPCRoomSts(pUserInfo->GetPCRoomSts());
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        this->SetGameChuUSN(pUserInfo->GetGameChuUSN());
#endif  
#ifdef _NA_20100307_BETAKEY_SYSTEM
        this->SetBetaKey(pUserInfo->GetBetaKey());
#endif

        if(eGM_GRADE_BASE < pUserInfo->GetSts() && pUserInfo->GetSts() < eGM_GRADE_MAX)
        {
            SUNLOG(eFULL_LOG, "[Authentication]GMUserLogon[Grade:%u][Guid:%u][ID:%s]", pUserInfo->GetSts() - eGM_GRADE_BASE, pUserInfo->GetUserGUID(), pUserInfo->GetUserID());
        }
        else if(eGM_GRADE_VIRTUAL ==  pUserInfo->GetSts())
        {
            SUNLOG(eFULL_LOG, "[Authentication]VirtualUserLogon[stat:%u][Guid:%u][ID:%s]", pUserInfo->GetSts(), pUserInfo->GetUserGUID(), pUserInfo->GetUserID());
        }

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        this->SetAge(pUserInfo->GetAge());
        this->SetRealNameAuth(pUserInfo->GetRealNameAuth());
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

        pUserInfo->ForceEnd();
    }
    else
    {
        SUNLOG(eFULL_LOG, "[Authentication]AuthID[%d]Cookie Not Found", pMsg->m_dwAuthID);
        return false;
    }

    // 로그인 시간 기록
    WriteLoginTime();

    BYTE byHighVerNo = (BYTE)pMsg->m_byHighVerNo;
    BYTE byMiddleVerNo = (BYTE)pMsg->m_byMiddleVerNo;
    BYTE byLowVerNo = (BYTE)pMsg->m_byLowVerNo;

    {    //    __NA000630_070817_MOLA_5th_UPDATE
        DWORD dwProtocolVER = pMsg->m_dwMOLAProtocolCRC;
        DWORD dwLIBProtocolVER = CryptManager::GetMolaProtocolCRC();
        if((dwLIBProtocolVER != dwProtocolVER)
            || (false == (C2S_HIGH_VERSION_NO == byHighVerNo && C2S_MIDDLE_VERSION_NO == byMiddleVerNo && C2S_LOW_VERSION_NO <=  byLowVerNo))
            )
        {
            SUNLOG(eCRITICAL_LOG, "PROTOCOL VERSION is not MATCHED %08X != %08X", dwLIBProtocolVER, dwProtocolVER);
            return false;
        }
    };
    
    // 국가 코드 검사
    BYTE byNationCode = (BYTE)pMsg->m_byNationCode;
    if(byNationCode != NATION_CODE)
    {
        return false;
    }

    // 임시 동일키가 있는지 체크
    UserSession * pAbuseUser = UserManager::Instance()->GetUser(this->GetUserKey());
    if(NULL != pAbuseUser)
    {
        SUNLOG(eCRITICAL_LOG, "[UGuid:%d]인증통과-중복로그인발생", this->GetUserKey());
        return false;
    }

    MSG_AD_CHARINFO_CHARLISTREQ_SYN msg;
    msg.m_dwKey    = this->GetUserKey();
    msg.m_UserGuid = this->GetUserGUID();
    msg.m_bIsPCRoom = !!this->GetReservedValue();    //< pc방 유저인지 체크
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    msg.m_LoginTime = this->GetLoginTime_t();
    // 18세 이상의 실명인증한 유저는 피로도를 적용시키지 않는다. 청소년만 피로도를 적용시킨다.
    if((this->GetAge() >= 18) && (this->GetRealNameAuth() == 1))    
        msg.m_bFatigueUser = FALSE;
    else
        msg.m_bFatigueUser = TRUE;
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msg.gamechu_usn_ = this->GetGameChuUSN();
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    msg.betakey_ = this->GetBetaKey();
#endif

    msg.m_bBackToSelect = false;
    msg.m_ChannelID = AgentServer::GetInstance()->GetKey().GetChannelID();

    if(FALSE == SendToDBPServer(&msg, sizeof(msg)))
    {
        SUNLOG(eFULL_LOG, "[TempUser::OnRecv] [ERROR]DBP에패킷을보낼수없습니다.");
        UserManager::Instance()->FreeUser(this);

        return FALSE;
    }

    // for Dummy
    this->SetPlayerKey(kuserinfo_object_key);
    UserManager::Instance()->AddUser(this);
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    MSG_AW_PREPARE_NEW_USER_SYN wmsg;
    wmsg.m_dwKey      = this->GetUserKey();
    wmsg.dwAuthUserID = this->GetAuthID();
    wmsg.dwUserGuid   = this->GetUserGUID();
    ;{
        util::StrArrayForUserID& dest = wmsg.ptszAccountID;
        _tcsncpy(dest, this->GetUserID(), _countof(dest));
        dest[_countof(dest) - 1] = _T('\0');
    };
    ;{
        strncpy(wmsg.szClientIP, this->GetClientIP(), _countof(wmsg.szClientIP));
        wmsg.szClientIP[_countof(wmsg.szClientIP) - 1] = '\0';
    };
    wmsg.dwPCRoomID = this->GetReservedValue();
    wmsg.byGMGrade  = this->GetSts();

    this->SendToWorldServer(&wmsg, sizeof(wmsg));
#endif
    CharSelect* pCharSelect = g_ZoneManager.FindCharSelect(nsKeyInfo::ZONE_KEY_START+1);
    ASSERT(pCharSelect);
    if(pCharSelect)
        pCharSelect->EnterUser(this);

    // AuthAgent로 로그인 했다고 알려준다.    
    MSG_GA_AUTH_PC_LOGIN_CMD loginMsg;
    loginMsg.m_dwAuthUserID    = this->GetAuthID();
    CHECK_ID_LENGTH_BUFFER(loginMsg.m_szAccountID);
    strncpy(loginMsg.m_szAccountID, this->GetUserID(), _countof(loginMsg.m_szAccountID));
    loginMsg.m_szAccountID[_countof(loginMsg.m_szAccountID) - 1] = '\0';
    AgentServer::GetInstance()->SendToAuthAgentServer((BYTE*)&loginMsg, (WORD)sizeof(loginMsg));

    // 혹시 어뷰징 블럭 유저인지 확인한다.
    ServerSession* pMasterServer = AgentServer::GetInstance()->SessionMasterServer();
    if(pMasterServer)
    {
        MSG_AM_CONNECTION_ENTERSERVER_CMD cmdMsg;
        cmdMsg.m_UserGuid = this->GetUserGUID();
        pMasterServer->Send((BYTE*)&cmdMsg, sizeof(cmdMsg));
    }

    this->BeginTransaction(TR_CHARACTER_LIST);
    this->configuration_step_ = 2;
    return TRUE;
}

void DummyUser::SimulRecv_AD_CHARINFO_CHARLISTREQ_ACK()
{
}

void DummyUser::SimulSend_CG_CONNECTION_ENTERVILLAGEREQ_SYN()
{
    MSG_CG_CONNECTION_ENTERVILLAGEREQ_SYN msg_syn;
    msg_syn.m_bySelectedCharIndex = 1;
    this->configuration_step_ = 4;
    this->SendPacket(&msg_syn, sizeof(msg_syn));
}

void DummyUser::SimulSend_CG_SYNC_PLAYER_ENTER_SYN()
{
    MSG_CG_SYNC_PLAYER_ENTER_SYN msg_syn;
    msg_syn.m_dwCheckSum = 0;
    msg_syn.checksum_wmo_ = 0;
    this->configuration_step_ = 6;
    this->SendPacket(&msg_syn, sizeof(msg_syn));
}

}; //end of namespace

#endif //USING_DUMMY_CLIENT