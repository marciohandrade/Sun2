#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <Version.h>
#include <ResultCode.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AD.h>
#include <PacketStruct_AW.h>
#include <LogTimeFormat.h>
#include <CryptManager.h>
#include <SunAuth/AuthProtocol_GA.h>
//////////////////////////////////////////////////////////////////////////
#include <AgentServer.h>
#include <Zone/ZoneManager.h>
#include <Zone/CharSelect.h>

#include <TimeCheck/TimeKeeper.h>
#include <TimeCheck/ConnectionTimeout.h>

#include <Function/AgentServer.Timer.WorkLoad.h>

#include <PacketHandler/PacketHandler.h>

#include "UserManager.h"
#include "TempUser.h"
#include "User.h"

//==================================================================================================
// (WAVERIX) (090526) (CHANGES) 개발 디버깅을 위해서 라이브 디버깅 방지 목적 TempUser Timeout설정 변경
//
static DWORD interval_NoAction_s = 60 * 1000;
static DWORD interval_PostCheck_s = 16 * 1000;
static DWORD interval_ConnectionTimeout_s = 60 * 1000;

static BOOLEAN _ChangeTempUserTimeout()
{
    eSERVER_MODE server_mode = AgentServer::GetInstance()->GetMode();
    if(server_mode == eSERVER_DEV)
    {
        interval_NoAction_s = 60 * 60 * 1000;
        interval_PostCheck_s = 10 * 60 * 1000;
        interval_ConnectionTimeout_s = interval_NoAction_s;
    }
    return true;
}

DWORD
TempUser::GetConnectionTimeout()
{
    return interval_ConnectionTimeout_s;
}

//
//==================================================================================================
//

TempUser::TempUser()
    : m_dwChangeMod(IDLING)
    , m_pNewUser(NULL)
{
}

TempUser::~TempUser()
{
}

VOID
TempUser::OnAccept(DWORD dwNetworkIndex)
{
    UserSession::OnAccept(dwNetworkIndex);
}

VOID
TempUser::OnDisconnect()
{
    if(m_pNewUser)
    {
        UserManager::Instance()->FreeUser(m_pNewUser);
    }
    UserSession::OnDisconnectInfoToAuth();
    //리스트에 추가되지 않으므로 주석
    // Don't remove UserManager
    //UserSession::OnDisconnect();
}

VOID
TempUser::Init()
{
    UserSession::Init();
    m_dwChangeMod = IDLING;

#if defined(_KOREA)
    static BOOLEAN bChangeTempUserTimeout_s = _ChangeTempUserTimeout();
#endif

    // 해당 리스트 삭제는 timeout에서 수행한다.
    m_dwNoActionTimeout.SetNextTimeoutByCurTickBased(interval_NoAction_s);
    UserManager::Instance()->__AddTempUser(this);
}

VOID
TempUser::Release()
{
    UserSession::Release();
    m_pNewUser = NULL;

    m_dwNoActionTimeout.Disabled();
}

BOOL
TempUser::Authentication(MSG_CG_CONNECTION_ENTERSERVER_SYN* pMsg)
{
    ConnectionTimeout * pUserInfo = dynamic_cast<ConnectionTimeout*>(g_TimeKeeper.PeekTimer(pMsg->m_dwAuthID));
    if(pUserInfo)
    {
        // 접속 시리얼키 비교!!!
        if(0 != strncmp((char*)pUserInfo->GetSerialKey(), (char*)pMsg->m_szSerialKey, AUTH_SERIAL_KEY_LEN))
        {
            SUNLOG(eFULL_LOG, "[Authentication] Auth Serial Key:ClientSendKey is differnt!!");
            pUserInfo->ForceEnd();
            return FALSE;
        }

        // (WAVERIX) (090826) (CHANGES) 버퍼 처리 관련 로직 일원화
        //                    (WARNING) 해외 관련 빌드시 문제 생기면 알려주시와요.

#if defined(__KR000000_090128_CHANNELING__) || defined(__NA000000_GLOBAL_AUTH_PROCESS__)
        BOOST_STATIC_ASSERT(sizeof(m_szID) >= sizeof(pMsg->m_szID) &&
                            _countof(m_szID) >= _countof(pMsg->m_szID));
        _tcsncpy(m_szID, pMsg->m_szID, _countof(m_szID));
        m_szID[_countof(m_szID) - 1] = '\0';
#else
    #ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
        CHECK_ID_LENGTH_BUFFER(m_szID);
        CHECK_ID_LENGTH_BUFFER(pMsg->m_szID);
        _tcsncpy(m_szID, pMsg->m_szID, _countof(m_szID));
        m_szID[_countof(m_szID) - 1] = '\0';
    #else
        memcpy(m_szID, pMsg->m_szID, ID_MAX_LEN);
        m_szID[ID_MAX_LEN] = '\0';
    #endif
        #ifdef _US_ACCOUNT_LENTH_UNCHECKED_
        // 아이디 대소문자 구분안함
        //if(0 != strncmp(pUserInfo->GetUserID(), _strupr(m_szID), ID_MAX_LEN))
        //{
        //  SUNLOG(eFULL_LOG, "[Authentication]AccountID[%s] != ClientSendID[%s]", pUserInfo->GetUserID(), pMsg->m_szID);
        //     pUserInfo->ForceEnd();
        //     return FALSE;
        //}
        #else//_US_ACCOUNT_LENTH_UNCHECKED_
        if(0 != strncmp(pUserInfo->GetUserID(), _strupr(m_szID), ID_MAX_LEN))
        {
            SUNLOG(eFULL_LOG, "[Authentication]AccountID[%s] != ClientSendID[%s]", pUserInfo->GetUserID(), pMsg->m_szID);
            pUserInfo->ForceEnd();
            return FALSE;
        }
        #endif//_US_ACCOUNT_LENTH_UNCHECKED_
#endif

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

        return TRUE;
    }
    else
    {
        SUNLOG(eFULL_LOG, "[Authentication]AuthID[%d]Cookie Not Found", pMsg->m_dwAuthID);
    }

    return FALSE;
}
void
TempUser::SendEnterServerNak(BYTE byErrorCode)
{
    MSG_CG_CONNECTION_ENTERSERVER_NAK NakPacket;
    NakPacket.m_dwErrorCode = byErrorCode;
    this->SendPacket(&NakPacket, sizeof(NakPacket));
}

BOOL
TempUser::ProcessCHECKING(BYTE* pMsg, WORD wSize)
{
    MSG_CG_CONNECTION_ENTERSERVER_SYN* pRecvMsg = (MSG_CG_CONNECTION_ENTERSERVER_SYN *)pMsg;

    BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_CONNECTION_ENTERSERVER_SYN) == wSize);
    if(!bIsValidSize)
    {
        SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_CONNECTION_ENTERSERVER_SYN"));
        return FALSE;
    }
    MSG_CG_CONNECTION_ENTERSERVER_SYN pSafeBuffer[1];
    memcpy(pSafeBuffer, pRecvMsg, sizeof(MSG_CG_CONNECTION_ENTERSERVER_SYN));
    pRecvMsg = pSafeBuffer;
    pRecvMsg->Decode();

    if(!Authentication(pRecvMsg))
    {
        this->SetUserID(pRecvMsg->m_szID);
        this->SendEnterServerNak(RC::RC_LOGOUT_WRONG_AUTHKEY);
        this->SetDisconnectCode(RC::RC_DISCONNECT_INCORRECT_AUTH_KEY);
        this->DisconnectUser();

        return FALSE;
    }

    // 로그인 시간 기록
    WriteLoginTime();

#ifdef _NA_0_20110915_CLIENT_MAC_ADDRESS
    {
        const uint index = _countof(pRecvMsg->mac_address_) - 1;
        pRecvMsg->mac_address_[index] = '\0';
        GAMELOG->UserLogin(this, pRecvMsg->mac_address_);
    };
#else
    //로그인 로그
    GAMELOG->UserLogin(this);
#endif

    ;{  // STEP: version check section
        const BYTE version_no_high = (BYTE)pRecvMsg->m_byHighVerNo;
        const BYTE version_no_mid = (BYTE)pRecvMsg->m_byMiddleVerNo;
        const BYTE version_no_low = (BYTE)pRecvMsg->m_byLowVerNo;

        const DWORD mola_of_client = pRecvMsg->m_dwMOLAProtocolCRC;
        const DWORD mola_of_server = CryptManager::GetMolaProtocolCRC();
        if (mola_of_client != mola_of_server ||
            version_no_high != C2S_HIGH_VERSION_NO ||
            version_no_mid != C2S_MIDDLE_VERSION_NO ||
            version_no_low < C2S_LOW_VERSION_NO)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("PROTOCOL VERSION is not MATCHED. ")
                   _T("\"S:0x%08X != C:0x%08X\" or ")
                   _T("\"S:%d.%d.%d > C:%d.%d.%d\""),
                   mola_of_server, mola_of_client,
                   C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO,
                   version_no_high,     version_no_mid,        version_no_low);
            this->SendEnterServerNak(RC::RC_LOGOUT_WRONG_VERSION);
            this->SetDisconnectCode(RC::RC_DISCONNECT_INCORRECT_VERSION);
            this->DisconnectUser();
            return FALSE;
        }
    };
    
    // 국가 코드 검사
    BYTE byNationCode = (BYTE)pRecvMsg->m_byNationCode;
    if(byNationCode != NATION_CODE)
    {
        this->SendEnterServerNak(RC::RC_LOGOUT_WRONG_VERSION);
        this->SetDisconnectCode(RC::RC_DISCONNECT_INCORRECT_VERSION);
        this->DisconnectUser();
        return FALSE;
    }

    // 임시 동일키가 있는지 체크
    UserSession * pAbuseUser = UserManager::Instance()->GetUser(this->GetUserKey());
    if(NULL != pAbuseUser)
    {
        SUNLOG(eCRITICAL_LOG, "[UGuid:%d]인증통과-중복로그인발생", this->GetUserKey());

        // 접속해 있는 유저를 끊어버린다.
        pAbuseUser->SetDisconnectCode(RC::RC_DISCONNECT_DUPLICATE_LOGIN);
        pAbuseUser->DisconnectUser();

        // 자신도 끊어버린다.
        this->SetDisconnectCode(RC::RC_DISCONNECT_DUPLICATE_LOGIN);
        this->SendEnterServerNak(RC::RC_LOGOUT_DUPLICATED_USER);
        this->DisconnectUser();

        return FALSE;
    }

    User* pNewUser = (User*)UserManager::Instance()->AllocUser(UT_USER);
    pNewUser->CopyInfo(this);

    SecurePackNode4User& rSECURE = pNewUser->GetSecureNode();
    rSECURE.SendPacket_KEYINFO_KEY(this, pNewUser);

    m_pNewUser = pNewUser;

    NoActionTimeout().SetNextTimeoutByCurTickBased(interval_PostCheck_s);
    return TRUE;
}

BOOL
TempUser::ProcessPASSING(BYTE* pMsg, WORD wSize)
{
    User* pNewUser = m_pNewUser;
    m_pNewUser = NULL;
    if(NULL == pNewUser)    
    {
        SUNLOG(eCRITICAL_LOG, "[ProcessPASSING] pNewUser is NULL!!");
        return FALSE;
    }

    //////////////////////////////////////////////////////////////////////////
    // pMsg --> CG_CRYPTOGRAPHY_ACK
    SecurePackNode4User& rSECURE = pNewUser->GetSecureNode();
    rSECURE.SendPacket_KEYINFO_CMD(this, pNewUser);

    //////////////////////////////////////////////////////////////////////////

    MSG_AD_CHARINFO_CHARLISTREQ_SYN msg;
    msg.m_dwKey    = pNewUser->GetUserKey();
    msg.m_UserGuid = pNewUser->GetUserGUID();
    msg.m_bIsPCRoom = !!pNewUser->GetReservedValue();    //< pc방 유저인지 체크
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    msg.m_LoginTime = pNewUser->GetLoginTime_t();
    // 18세 이상의 실명인증한 유저는 피로도를 적용시키지 않는다. 청소년만 피로도를 적용시킨다.
    if((pNewUser->GetAge() >= 18) && (pNewUser->GetRealNameAuth() == 1))    
        msg.m_bFatigueUser = FALSE;
    else
        msg.m_bFatigueUser = TRUE;
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msg.gamechu_usn_ = pNewUser->GetGameChuUSN();
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM    
    msg.betakey_ = pNewUser->GetBetaKeyInfo()->GetBetaKey();
#endif

    msg.m_bBackToSelect = false;
    msg.m_ChannelID = AgentServer::GetInstance()->GetKey().GetChannelID();

    if(FALSE == SendToDBPServer(&msg, sizeof(msg)))
    {
        SUNLOG(eFULL_LOG, "[TempUser::OnRecv] [ERROR]DBP에패킷을보낼수없습니다.");

        this->SetUserID(m_szID);
        this->SendEnterServerNak(RC::RC_LOGOUT_DB_ERROR);

        this->SetDisconnectCode(RC::RC_DISCONNECT_DBP_CONNETION_ERROR);
        this->DisconnectUser();

        UserManager::Instance()->FreeUser(pNewUser);

        return FALSE;
    }

    // 새로운 NetworkObject으로 교체
    Redirect(pNewUser);
    UserManager::Instance()->FreeUser(this);
    UserManager::Instance()->AddUser(pNewUser);
    // CHANGES: f110621.2L, approximately, 3500 users comsumed 450MB memory
    // CHANGES: f110905.5L, blocked temporary
    //pNewUser->UseSendPendingBuffer(128 * 1024);
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    MSG_AW_PREPARE_NEW_USER_SYN wmsg;
    wmsg.m_dwKey      = pNewUser->GetUserKey();
    wmsg.dwAuthUserID = pNewUser->GetAuthID();
    wmsg.dwUserGuid   = pNewUser->GetUserGUID();
    ;{
        util::StrArrayForUserID& dest = wmsg.ptszAccountID;
        _tcsncpy(dest, pNewUser->GetUserID(), _countof(dest));
        dest[_countof(dest) - 1] = _T('\0');
    };
    ;{
        strncpy(wmsg.szClientIP, pNewUser->GetClientIP(), _countof(wmsg.szClientIP));
        wmsg.szClientIP[_countof(wmsg.szClientIP) - 1] = '\0';
    };
    wmsg.dwPCRoomID = pNewUser->GetReservedValue();
    wmsg.byGMGrade  = pNewUser->GetSts();

    pNewUser->SendToWorldServer(&wmsg, sizeof(wmsg));
#endif
    CharSelect* pCharSelect = g_ZoneManager.FindCharSelect(nsKeyInfo::ZONE_KEY_START+1);
    ASSERT(pCharSelect);
    if(pCharSelect)
        pCharSelect->EnterUser(pNewUser);

    // AuthAgent로 로그인 했다고 알려준다.    
    MSG_GA_AUTH_PC_LOGIN_CMD loginMsg;
    loginMsg.m_dwAuthUserID    = pNewUser->GetAuthID();
    CHECK_ID_LENGTH_BUFFER(loginMsg.m_szAccountID);
    strncpy(loginMsg.m_szAccountID, pNewUser->GetUserID(), _countof(loginMsg.m_szAccountID));
    loginMsg.m_szAccountID[_countof(loginMsg.m_szAccountID) - 1] = '\0';
    AgentServer::GetInstance()->SendToAuthAgentServer((BYTE*)&loginMsg, (WORD)sizeof(loginMsg));

    // 혹시 어뷰징 블럭 유저인지 확인한다.
    ServerSession* pMasterServer = AgentServer::GetInstance()->SessionMasterServer();
    if(pMasterServer)
    {
        MSG_AM_CONNECTION_ENTERSERVER_CMD cmdMsg;
        cmdMsg.m_UserGuid = pNewUser->GetUserGUID();
        pMasterServer->Send((BYTE*)&cmdMsg, sizeof(cmdMsg));
    }

    pNewUser->BeginTransaction(TR_CHARACTER_LIST);

    return TRUE;
}




