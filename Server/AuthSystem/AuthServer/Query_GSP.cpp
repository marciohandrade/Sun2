#include "stdafx.h"
#include "./Query_GSP.h"

#include <AuthProtocol_AS.h>
#include "SunAuth/AuthProtocol_GA.h"

#include "./AuthFrame.h"
#include "./QueryManager.h"
#include "./ServerDataManager.h"
#include "./LoginFrontServerSession.h"
#include "./AuthAgentServerSession.h"
#include "./AccountManager.h"
#include "ReAuthInfo.h"
#include "ReAuthManager.h"

#if defined(_NA_20100307_BETAKEY_SYSTEM)    
#include <atlconv.h>
#endif

extern TCHAR g_LoginErrorString[AUTHRESULT_MAX][MAX_AUTHRESULT_MESSAGE_LEN + 1];

namespace nsGSP {
;
// check global game db rule
static void GlobalGameDBRuleCheck()
{
    BOOST_STATIC_ASSERT(sizeof(TCHAR) == sizeof(char));
#if defined(_AUTH_GLOBAL)
    Buffer_of_AccountID account_id;
    BOOST_STATIC_ASSERT(_countof(account_id) < ID_MAX_LEN + 1);
    Buffer_of_CharName char_name;
    BOOST_STATIC_ASSERT(_countof(char_name) >= MAX_CHARNAME_LENGTH + 1);

    Buffer_of_GameID game_id;
    AccountInfo account_info;
    BOOST_STATIC_ASSERT(_countof(account_info.m_GameID) == _countof(game_id) &&
                        sizeof(account_info.m_GameID) == sizeof(game_id));
#endif
}

}; //end of namespace 'nsGSP'

namespace nsGSP {
;
//==================================================================================================
//
QueryGspCheckAuthKey::QueryGspCheckAuthKey()
    : QueryObject(QUERYTYPE_GSP_CHECK_AUTH)
    , account_(0)
{
    Clear(eAuthToken_None);
    // gb.USP_WS_CheckAuthKey
}

QueryGspCheckAuthKey::~QueryGspCheckAuthKey()
{
    if(account_) {
        TAllocDelete(AccountInfo, account_);
        account_ = 0;
    }
}


//// Input Parameters
////  @GameID,
////  @GameCode,
////  @AuthKey,
////  @IP,
////  @MacAddress
//// Output Parameters
////  @AccountID : int,
////  @AccountType : tinyint, (eAccountType)
////  @BlockState : tinyint,  (eBlockState)
////  @Result : tinyint       (eResult)
//
//// @AccountType - 계정 종류
//enum eAccountType {
//    eAccountType_Normal = 1,
//    eAccountType_GM     = 2,
//    eAccountType_Guest  = 3,
//};
////  @BlockState - 블럭 상태
//enum eBlockState {
//    eBlockState_Normal  = 0,
//    eBlockState_Block   = 1,
//};
////  @Result - 인증키 체크 성공 여부
//enum eResult {
//    eResult_Fail    = 0,
//    eResult_Success = 1,
//};

void
QueryGspCheckAuthKey::Clear(eAuthToken token_type)
{
    AccountInfo* account = account_;
    ZeroMemory(&input_args_, sizeof(input_args_));
    ZeroMemory(&result_data_, sizeof(result_data_));
    // init
    token_type_ = token_type;
    if (account == NULL) {
        account = TAllocNew(AccountInfo);
    };
    ZeroMemory(account, sizeof(*account));
    account_ = account;
    reauth_key_ = 0;
}

void
QueryGspCheckAuthKey::ReauthByServerSelect(nsPacket::ReAuthInfoBase::ReAuthKeyType reauth_key)
{
    reauth_key_ = reauth_key;
}

void
QueryGspCheckAuthKey::onResult()
{
#ifdef _NA_20100307_BETAKEY_SYSTEM
    USES_CONVERSION;
#endif
    QueryObject::onResult();
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|AccountGuid|%d|\n", result_data_.account_guid);
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|AccountType|%d|\n", result_data_.account_type);
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|BlockState|%d|\n", result_data_.block_state);
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|Result|%d|\n", result_data_.result);
#ifdef _NA_20100307_BETAKEY_SYSTEM
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|BetaKey|%s|\n", W2A(result_data_.betakey));
#endif
#ifdef _AUTH_GLOBAL
    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    QueryManager* const pQueryManager = QueryManager::Instance();

    LoginFrontServerSession* front =                                // front_key_
        static_cast<LoginFrontServerSession*>(pAuthFrame->FindSession(account_->m_LoginFrontId));
    if(front == 0) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Cannot find LoginFront[%u]",
               account_->m_LoginFrontId);
        return;
    }

    BOOLEAN success = true;

    if(success && reauth_key_ != 0) {
        success = onResult_ByServerSelect(front);
        return;
    }

    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msgACK;
    msgACK.m_dwFrontUserID = account_->m_LoginFrontUserId;
    msgACK.m_byFailCount = 0;
    msgACK.m_byResult = AUTHRESULT_OK;

    if(success && result_data_.result == this->eResult_Fail)
    { success = false, msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT; }

    if(success && result_data_.block_state == this->eBlockState_Block)
    { success = false, msgACK.m_byResult = AUTHRESULT_NOTALLOWEDACCOUNT; } // block user?

    if(success) {
        {// proc 'account_->m_userCode[0]' { GM, not GM }
            BYTE account_type
                = result_data_.account_type == this->eAccountType_GM     ?  USER_STS_GM + 1 // GM code >= 100
                : result_data_.account_type == this->eAccountType_Normal ?  USER_STS_NORMAL
                : /* Guest */                                               USER_STS_1ST_BETA_TESTER;
            account_->m_userCode[0] = account_type;
        }

        account_->m_userCode[1] =    // pc room status <- 돈 안내고 튄 경우란다.(요 주의 유저)
        account_->m_userCode[2] = 0; //m_Record.Sts[2];   // 실명 인증자 인지 식별

        //account_->m_netFrontId < already alloced
        account_->m_userGuid     = result_data_.account_guid;
        account_->m_age          = 20;   //연령 제한 문제는 ...음...
        account_->m_pcBangId     = 0;    //GSP는 PC방이 없도록 처리한다.

        strncpy(account_->m_clientIp, input_args_.client_ip, _countof(account_->m_clientIp));
        account_->m_clientIp[_countof(account_->m_clientIp) - 1] = '\0';
        //ZeroMemory(account_->m_MD5, sizeof(account_->m_MD5)); // already inited
        account_->m_AuthUserType = AUTH_USER_DEFAULT;
        account_->m_tickConnectTime = ::GetTickCount();

        //account_->m_status = ACCOUNTSTATUS_AUTH; // set on 2nd auth (GetAccountID)
        //::GetLocalTime(&account_->m_loginTime);

        strncpy(account_->m_GameID, input_args_.game_id, _countof(account_->m_GameID));

        //{ // process success result
        //    msgACK.m_byResult = AUTHRESULT_OK;
        //    strncpy(msgACK.m_szInfo, g_LoginErrorString[msgACK.m_byResult], _countof(msgACK.m_szInfo));
        //    front->SendPacket(&msgACK, sizeof(msgACK));
        //}
        
        // no null-set

        // query->GSP GetAccountID(~~)
#ifdef _NA_20100307_BETAKEY_SYSTEM        
        USES_CONVERSION;
        account_->betakey_ = _atoi64(W2A(result_data_.betakey));
#endif        
        pQueryManager->Query_GspGetAccountID_ForAuth(account_);
        account_ = 0; // passing pointer

        //SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Auth Success!! Account[%s] IP[%s]",
        //       account_->m_account, account_->m_clientIp);

        return;
    }

    if(success == false) {
        strncpy(msgACK.m_szInfo, g_LoginErrorString[msgACK.m_byResult], _countof(msgACK.m_szInfo));
        front->SendPacket(&msgACK, sizeof(msgACK));
        return;
    }

    // can't reachable section
    SUNLOG(eCRITICAL_LOG, "Unexpected Step, Re-check logic ["__FUNCTION__"]");
#endif //_AUTH_GLOBAL
}

BOOLEAN
QueryGspCheckAuthKey::onResult_ByServerSelect(LoginFrontServerSession* reply_session)
{
#if !defined(_AUTH_GLOBAL)
    return false;
#endif

#if defined(_AUTH_GLOBAL)
    BOOLEAN success = true;

    GLOBAL_DEBUGLOG(__FUNCTION__" Enter\n", 0);

    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msgACK;
    msgACK.m_dwFrontUserID = account_->m_LoginFrontUserId;
    msgACK.m_byFailCount = 0;
    msgACK.m_byResult = AUTHRESULT_OK;

    QueryManager* pQueryManager = QueryManager::Instance();
    ReAuthManager* const pReAuthManager = ReAuthManager::Instance();
    const ReAuthInfo* reauth_info = pReAuthManager->FindReAuthInfo(reauth_key_);

    if(success && reauth_info == 0)
    { success = false, msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT; }

    const AccountInfo* prev_account = 0;
    if(success && !(prev_account = reauth_info->GetBackupedAccountInfo()))
    { assert(!"Impossible"); success = false, msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT; }

    GLOBAL_DEBUGLOG(__FUNCTION__" GetReauthInfo|%p|\n", prev_account);

    if(success && (prev_account->m_userGuid == 0 || prev_account->m_account[0] == '\0'))
    { success = false, msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT; }

    if(success) {
        GLOBAL_DEBUGLOG(__FUNCTION__" process for success|UserGuid|%d|\n", prev_account->m_userGuid);
        CopyMemory(account_->m_userCode, prev_account->m_userCode, sizeof(account_->m_userCode));
        account_->m_userGuid    = prev_account->m_userGuid;
        account_->m_age          = 20;
        account_->m_pcBangId     = 0;
        success = strncmp(input_args_.client_ip, prev_account->m_clientIp,
                          _countof(input_args_.client_ip)) == 0;
        if(success == 0)
        { msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT; }

        if(success) {
            strncpy(account_->m_clientIp, input_args_.client_ip, _countof(account_->m_clientIp));
            account_->m_clientIp[_countof(account_->m_clientIp) - 1] = '\0';

            account_->m_AuthUserType = AUTH_USER_DEFAULT;
            account_->m_tickConnectTime = ::GetTickCount();

            strncpy(account_->m_GameID, input_args_.game_id, _countof(account_->m_GameID));
            GLOBAL_DEBUGLOG(__FUNCTION__" GameID|%s|\n", input_args_.game_id);

            pQueryManager->Query_GspGetAccountID_ForAuth(account_);
            account_ = 0; // passing pointer

            // free resource 're-authentication key'
            pReAuthManager->OnAcceptQueryLoginUser(reauth_info);

            return true;
        }
    }

    if(success == false) {
        GLOBAL_DEBUGLOG(__FUNCTION__" Reauth Fail Result|%s|\n", g_LoginErrorString[msgACK.m_byResult]);
        strncpy(msgACK.m_szInfo, g_LoginErrorString[msgACK.m_byResult], _countof(msgACK.m_szInfo));
        reply_session->SendPacket(&msgACK, sizeof(msgACK));
        return false;
    }

    // can't reachable section
    SUNLOG(eCRITICAL_LOG, "Unexpected Step, Re-check logic ["__FUNCTION__"]");
    return false;
#endif //_AUTH_GLOBAL
}


//==================================================================================================
//
QueryGspChangeUserState::QueryGspChangeUserState()
    : QueryObject(QUERYTYPE_GSP_ACCOUNT_STATE)
{
    // gb.USP_INSERT_ACCOUNTSTATE

}

QueryGspChangeUserState::~QueryGspChangeUserState()
{
}


//// Input Parameters
////  @AccountGuid,
////  @GameCode,
////  @ServerGuid,
////  @ConnectIP
////  @ConnectState       (eConnectState)
//// Output Parameters
////  @Result : tinyint
//
////  @ConnectState
//enum eConnectState {
//    eConnectState_Login     = 0,
//    eConnectState_Logout    = 1,
//};
//
////  @Result
//enum eResult {
//    eResult_Fail    = 0,
//    eResult_Success = 1,
//};

void
QueryGspChangeUserState::Clear()
{
    ZeroMemory(&input_args_, sizeof(input_args_));
    ZeroMemory(&result_data_, sizeof(result_data_));
}

void
QueryGspChangeUserState::onResult()
{
    QueryObject::onResult();
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|Result|%d|\n", result_data_.result);
#ifdef _AUTH_GLOBAL
#endif
}


//==================================================================================================
//
QueryGspLoggingUserState::QueryGspLoggingUserState()
    : QueryObject(QUERYTYPE_GSP_CONNECT_LOG)
{
    // gb.USP_INSERT_CONNECTLOG
}

QueryGspLoggingUserState::~QueryGspLoggingUserState()
{
}

//// Input Parameters
////  @AccountGuid,
////  @GameID,
////  @GameCode,
////  @ServerGuid,
////  @PlayTime,  (초 단위, 입력 불가시 0)
////  @LogType,   (eLogType)
////  @CharacterName,
////  @Class,
////  @Lv,
////  @LogIP
//// Output Parameters
////  @Result : tinyint (eResult)
//
////  @LogType,
//enum eLogType {
//    eLogType_Login  = 0,
//    eLogType_Logout = 1,
//};
//
////  @Result
//enum eResult {
//    eResult_Fail    = 0,
//    eResult_Success = 1,
//};

void
QueryGspLoggingUserState::Clear()
{
    ZeroMemory(&input_args_, sizeof(input_args_));
    ZeroMemory(&result_data_, sizeof(result_data_));
}

void
QueryGspLoggingUserState::onResult()
{
    QueryObject::onResult();
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|Result|%d|\n", result_data_.result);

#ifdef _AUTH_GLOBAL
#endif
}


//==================================================================================================
//
QueryGspGetAccount::QueryGspGetAccount()
    : QueryObject(QUERYTYPE_GSP_ACCOUNT_ID)
    , account_(0)
{
    // gb.USP_INSERT_CONNECTLOG
    ZeroMemory(&passing_data_, sizeof(passing_data_));
}

QueryGspGetAccount::~QueryGspGetAccount()
{
    if (account_ != NULL) {
        TAllocDelete(AccountInfo, account_);
        account_ = NULL;
    }

    if (passing_data_.query_info_ != NULL) {
        TAllocDelete(QueryUserIdInfo, passing_data_.query_info_);
        passing_data_.query_info_ = NULL;
    }
}

// Input Parameters
//  @GameID,
//  @GameCode,
// Output Parameters
//  @AccountID

void
QueryGspGetAccount::PrepareForAuthStep(AccountInfo* account)
{
    auth_process_ = true;
    if (account_ != NULL) {
        TAllocDelete(AccountInfo, account_);
    };
    account_ = account;
    ZeroMemory(&input_args_, sizeof(input_args_));
    ZeroMemory(&result_data_, sizeof(result_data_));
    result_data_.overrun_detector = 0xDEADBEEF;
}

//void
//QueryGspGetAccount::PrepareQueryUserIdInfo(AGENTID agent_id, const QueryUserIdInfo* query_info)
//{
//    auth_process_ = false;
//     if(passing_data_.query_info_ == 0)
//        passing_data_.query_info_ = TAllocNew(QueryUserIdInfo);
//
//    *passing_data_.query_info_ = *query_info;
//    passing_data_.agent_id_ = agent_id;
//
//    ZeroMemory(&input_args_, sizeof(input_args_));
//    ZeroMemory(&result_data_, sizeof(result_data_));
//    result_data_.overrun_detector = 0xDEADBEEF;
//}

void
QueryGspGetAccount::onResult()
{
    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    if(!FlowControl::FCAssert(result_data_.overrun_detector == 0xDEADBEEF))
        return;

    QueryObject::onResult();
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|AccountID(db result)|%s|\n", result_data_.account_id);

#ifdef _AUTH_GLOBAL
    if(!FlowControl::FCAssert(auth_process_ != 0))
        return;

    BOOLEAN success = true;

    // authentication process 'get account'
    QueryManager* const pQueryManager = QueryManager::Instance();

    LoginFrontServerSession* front =                                // front_key_
        static_cast<LoginFrontServerSession*>(pAuthFrame->FindSession(account_->m_LoginFrontId));

    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msgACK;
    msgACK.m_dwFrontUserID = account_->m_LoginFrontUserId;
    msgACK.m_byFailCount = 0;
    msgACK.m_byResult = AUTHRESULT_OK;

    if(success && result_data_.account_id[0] == L'\0')
    { success = false, msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT; }

    AccountInfo* new_account = 0;
    if(success) {
        WideCharToMultiByte(CP_ACP, 0,
                            result_data_.account_id, -1,
                            account_->m_account, sizeof(account_->m_account),
                            NULL, NULL);
        //strncpy(account_->m_account, result_data_.account_id, _countof(account_->m_account));
        account_->m_account[_countof(account_->m_account) - 1] = '\0';
        GLOBAL_DEBUGLOG("["__FUNCTION__"]|AccountID(ansi)|%s|\n", account_->m_account);

        new_account = CheckOverlapLogin(msgACK.m_byResult);
    }

    if(success && new_account == 0)
    { 
        success = false;
        msgACK.m_byResult = AUTHRESULT_ALREADYLOGIN; 
    }

    const char* result_msg = success ? "Success" : "Fail";
    if(success) {
        new_account->m_status = ACCOUNTSTATUS_AUTH;
        new_account->m_tickConnectTime = GetTickCount();
        GetLocalTime(&new_account->m_loginTime);
#ifdef _NA_20100307_BETAKEY_SYSTEM        
        msgACK.m_betakey_ = new_account->betakey_;        
#endif
    }

    // current status note
    //  if(success == 0) then 'new_account == 0'
    //  aligned in auth user manager
    strncpy(msgACK.m_szInfo, g_LoginErrorString[msgACK.m_byResult], _countof(msgACK.m_szInfo));
    front->SendPacket(&msgACK, sizeof(msgACK));

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Auth %s!! Account[%s] IP[%s]",
            result_msg, account_->m_account, account_->m_clientIp);

    TAllocDelete(AccountInfo, account_);
    account_ = 0;
#endif //_AUTH_GLOBAL
}

AccountInfo*
QueryGspGetAccount::CheckOverlapLogin(BYTE& byResult)
{
    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    AccountManager* const pAccountManager = AccountManager::Instance();

    LoginFrontServerSession* front =
        static_cast<LoginFrontServerSession*>(pAuthFrame->FindSession(account_->m_LoginFrontId));
    //assert(front != 0);

    // 이전 접속자가 있다면, 그 정보를 얻어온다.
    AccountInfo* prev_account = NULL;
    AccountInfo* curr_account =
        pAccountManager->CreateAccountInfo(account_->m_account, &prev_account);

    // 이전 접속자가 없으면 성공!
    if(prev_account == 0)
    {
        // pre-aligned (4) by pAccountManager->CreateAccountInfo
        // { m_account, m_tickAuth, m_dwWaitLogoutCnt, m_glogKey }
        account_->m_tickAuth = curr_account->m_tickAuth;
    #ifdef _BILLING_WAITLOGOUT
        account_->m_dwWaitLogoutCnt = curr_account->m_dwWaitLogoutCnt;
    #endif
        CopyMemory(account_->m_glogKey, curr_account->m_glogKey, sizeof(account_->m_glogKey));

        *curr_account = *account_;
        //curr_account->m_locationId = 0;
        return curr_account;
    }

    // 현재 인증중인 계정이 존재(서버를 선택하지 않은 상황)
    if(prev_account->m_locationId == 0)
    {
        byResult = AUTHRESULT_PROCESSLOGIN;
        SUNLOG(eCRITICAL_LOG, "[CheckOverlapLogin] Account[%s] preAuthStatus[%u]",
               prev_account->m_account, prev_account->m_status);
    }
    // 현재 접속중인 계정이 존재(서버를 선택한 상황)
    else
    {
        byResult = AUTHRESULT_ALREADYLOGIN;
        SUNLOG(eCRITICAL_LOG, "[CheckOverlapLogin] Account[%s] preAuthStatus[%u] GameAgentID[%u]", 
               prev_account->m_account, prev_account->m_status, prev_account->m_locationId);

        AuthAgentServerSession* pAuthAgentSession =
            ServerDataManager::Instance()->FindAuthAgentServerSession(prev_account->m_locationId);
        if(pAuthAgentSession)
        {
            // 이전 접속자 강제종료 요청
            MSG_SA_AUTH_ASK_PC_LOGOUT_SYN logoutSYN;
            logoutSYN.m_dwAuthUserID = prev_account->m_authKey;
            logoutSYN.m_GameAgentID = prev_account->m_locationId;
            logoutSYN.m_dwUserGUID = prev_account->m_userGuid;
            CHECK_ID_LENGTH_BUFFER(logoutSYN.m_szID);
            CHECK_ID_LENGTH_BUFFER(prev_account->m_account);
            ::_tcsncpy(logoutSYN.m_szID, prev_account->m_account, _countof(logoutSYN.m_szID));
            logoutSYN.m_szID[_countof(logoutSYN.m_szID) - 1] = '\0';
            logoutSYN.m_dwLogoutCode = eDRC_OVERLAPPEDCONN;

            pAuthAgentSession->SendPacket(&logoutSYN, sizeof(logoutSYN));
        }
        else
        {
            // 다운된 서버의 경우, 위치정보를 삭제한다.
            pAccountManager->ClearInfo(prev_account->m_account);
        }
    }

    return NULL;
}


//==================================================================================================
//
QueryGspGetAccountByGuid::QueryGspGetAccountByGuid()
    : QueryObject(QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID)
    , account_(0)
{
    // gb.USP_INSERT_CONNECTLOG
    ZeroMemory(&passing_data_, sizeof(passing_data_));
}

QueryGspGetAccountByGuid::~QueryGspGetAccountByGuid()
{
    if (account_ != NULL) {
        TAllocDelete(AccountInfo, account_);
        account_ = NULL;
    }

    if (passing_data_.query_info_ != NULL) {
        TAllocDelete(QueryUserIdInfo, passing_data_.query_info_);
        passing_data_.query_info_ = NULL;
    }
}

// Input Parameters
//  @AccountGuid,
//  @GameCode,
// Output Parameters
//  @AccountID

void
QueryGspGetAccountByGuid::PrepareQueryUserIdInfo(AGENTID agent_id, const QueryUserIdInfo* query_info)
{
    auth_process_ = false;
    if (passing_data_.query_info_ == NULL) {
        passing_data_.query_info_ = TAllocNew(QueryUserIdInfo);
    };
    *passing_data_.query_info_ = *query_info;
    passing_data_.agent_id_ = agent_id;

    ZeroMemory(&input_args_, sizeof(input_args_));
    ZeroMemory(&result_data_, sizeof(result_data_));
    result_data_.overrun_detector = 0xDEADBEEF;
}

void
QueryGspGetAccountByGuid::onResult()
{
    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    if(!FlowControl::FCAssert(result_data_.overrun_detector == 0xDEADBEEF))
        return;

    QueryObject::onResult();
    GLOBAL_DEBUGLOG("["__FUNCTION__"]|AccountID(db result)|%s|\n", result_data_.account_id);

#ifdef _AUTH_GLOBAL
    if(!FlowControl::FCAssert(auth_process_ == 0))
        return;

    BOOLEAN success = true;

    // normal get account
    if(success && result_data_.account_id[0] == L'\0')
        success = false;

    result_data_.account_id[_countof(result_data_.account_id) - 1] = L'\0';

    char account_id[ID_MAX_LEN + 1] = { 0, };
    if(success) {
        int ret = WideCharToMultiByte(CP_ACP, 0,
                                      result_data_.account_id, -1,
                                      account_id, sizeof(account_id),
                                      NULL, NULL);
        if(ret != 0)
            success = account_id[_countof(account_id) - 1] == 0;
    }

    MSG_AU_AUTH_COMMON_QUERY_USER_ID_ACK msgACK;
    msgACK.Clear();
    msgACK.game_agent_id_ = passing_data_.agent_id_;
    //msgACK.action_type_ <- unhandled
    msgACK.result_ = success;

    if(success) {
        strncpy(passing_data_.query_info_->user_id_, account_id,
                _countof(passing_data_.query_info_->user_id_));
        passing_data_.query_info_->user_id_[_countof(passing_data_.query_info_->user_id_) - 1] = '\0';
    } else {
        ZeroMemory(passing_data_.query_info_->user_id_,
            sizeof(passing_data_.query_info_->user_id_));
    }
    msgACK.user_info_ = *passing_data_.query_info_;

    ServerData* pServerData =
        ServerDataManager::Instance()->FindServerData(passing_data_.agent_id_);
    if(pServerData == 0)
        return;

    AuthAgentServerSession* pAuthAgentServerSession = pServerData->GetAuthAgentSession();
    if(pAuthAgentServerSession == 0)
        return;

    pAuthAgentServerSession->SendPacket(&msgACK, msgACK.GetSize());

    //////////////////////////////////////////////////////////////////////////////
    #if 0 // Test Code
    {
        if(success && result_data_.account_id[0] == L'\0')
        { success = false/*, msgACK.m_byResult = AUTHRESULT_INVALIDACCOUNT*/; }

        GLOBAL_DEBUGLOG("["__FUNCTION__"] success %d\n", success);
        if(success) {
            Buffer_of_AccountID account_id;
            WideCharToMultiByte(CP_ACP, 0,
                result_data_.account_id, -1,
                account_id, sizeof(account_id),
                NULL, NULL);
            account_id[_countof(account_id) - 1] = '\0';
            GLOBAL_DEBUGLOG("["__FUNCTION__"]|AccountID(ansi)|%s|\n", account_id);
        }
    };
    #endif

#endif //_AUTH_GLOBAL
}


}; //end of namespace 'nsGSP'
