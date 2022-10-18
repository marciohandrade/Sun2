#include "stdafx.h"
#include "Query_Netcafe.h"

#include <AuthProtocol_FS.h>

#include "AuthFrame.h"
#include "LoginFrontServerSession.h"
#include "AccountManager.h"
#include "QueryManager.h"

extern TCHAR g_LoginErrorString[AUTHRESULT_MAX][MAX_AUTHRESULT_MESSAGE_LEN+1];


//#ifdef _NET_CAFE

void
QueryNetCafeLog::ClearField()
{
    m_NetCafeNum = 0;
    m_OtpRegistered = 0; //_JP000000_ONETIMEPASSWORD_AUTHFLOW_
    m_dwFrontUserId = 0;
    m_bLogin = false;
    m_ReAuthKey = 0;
    ZeroMemory(m_account, sizeof(m_account));
}

QueryNetCafeLogIn::QueryNetCafeLogIn()
    : QueryNetCafeLog(QUERYTYPE_NETCATFE_LOGIN)
{
    m_bLogin = true;
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    //m_recordSet.RegisterParameter(&m_NetCafeNum, "", adInteger, adParamReturnValue, sizeof(int), "");
    m_recordSet.RegisterVariable(&m_NetCafeNum, "NC_USER");
    m_recordSet.RegisterVariable(&m_OtpRegistered, "OTP_USER");

    //m_recordSet.RegisterParameter(&m_NetCafeNum, "NC_USER", adInteger, adParamOutput, sizeof(DWORD), "");
    //m_recordSet.RegisterParameter(&m_OtpRegistered, "OTP_USER", adInteger, adParamOutput, sizeof(DWORD), "");
#else
    m_recordSet.RegisterParameter(&m_NetCafeNum, "", adInteger, adParamReturnValue, sizeof(DWORD), "");
#endif
}

void
QueryNetCafeLog::onResult()
{
    QueryObject::onResult();

#ifdef _NET_CAFE
    if(m_bLogin == 0) {
        SUNLOG(eFULL_LOG, "[ResultNetCafeLog] Account[%s] NetCafe Logout", m_account);
        return;
    }

    LoginFrontServerSession* front =
        static_cast<LoginFrontServerSession*>(AuthFrame::Instance()->FindSession(m_key));
    if(front == 0) {
        SUNLOG(eCRITICAL_LOG, "[ResultNetCafeLog] Cannot find LoginFront[%u]", m_key);
        return;
    }

    AccountInfo* account = AccountManager::Instance()->FindWithLoginFrontUserID(m_dwFrontUserId);
    if(account == 0) {
        SUNLOG(eCRITICAL_LOG, "[ResultNetCafeLog] Cannot find account[%s]", m_account);
        return;
    }

    // (NOTE) current status : (m_bLogin != false)
  #ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    if(netcafe_logintype_ == NETCAFE_ONTIME_LOGIN)    
    {
    #endif
        if (this->m_OtpRegistered == -2)
        {
            SUNLOG(eCRITICAL_LOG, "[ResultNetCafeLog] failed(OTP_USER=-2) SP_SUN_USER_LOGIN _ %s\n", \
                m_account);

            MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msg_ack;
            msg_ack.m_dwFrontUserID = m_dwFrontUserId;
            msg_ack.m_byFailCount = 0;
            msg_ack.m_byResult = AUTHRESULT_INVALIDACCOUNT;
            ::_tcsncpy(msg_ack.m_szInfo, g_LoginErrorString[msg_ack.m_byResult],
                _countof(msg_ack.m_szInfo));
            front->SendPacket(&msg_ack, sizeof(msg_ack));
            return;
        }
	#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    }  
	#endif
    AUTH_DEBUGLOG("OtpValue %d\n", this->m_OtpRegistered);
    AUTH_DEBUGLOG("ReAuthKey %d\n", this->m_ReAuthKey);
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|User=%s|OutputValues|NC_USER=%d|OTP_USER=%d|",
        m_account, this->m_NetCafeNum, this->m_OtpRegistered);
    // -1 specification
    //  OTP 자체를 완전히 DISABLE 시킴(차후 채널링 또는 포탈과의 연계를 위함)
    BOOLEAN otp_serviced = true;
    if (this->m_OtpRegistered == -1)
    {
        this->m_OtpRegistered = 0; // change to not otp user
        otp_serviced = false;
    }
    // remain 'this->m_OtpRegistered' status = { 0, 1 }
    
  #endif

    // -- OTP contents branch routine
    // #1. if otp user then query 2nd login
    // #2. else then change to authentication success

    // status table : assert(m_bLogin != false);
    // | re-auth | otp user | authentication result |
    //      0          0      Success (no otp user)
    //      0          1      need 2nd query (otp user)
    //      1          0      re-authentication by server select facility
    //      1          1      same as [1|0], re-authentication by server select facility
    // otp user = { -1 }인 경우, same as [0|0], [1|0] - pre-changed
    // otp user = { -2 }인 경우, authentication fail - pre-checked

    account->m_pcBangId = m_NetCafeNum;
    { // (NOTE) 하기 시간 갱신 루틴의 경우는 매번 하도록 유지 한다.
        account->m_tickConnectTime	= ::GetTickCount();
        ::GetLocalTime(&account->m_loginTime);
    }

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    if(netcafe_logintype_ == NETCAFE_ONTIME_LOGIN)    
    {
    #endif
        if (this->m_ReAuthKey == 0 && this->m_OtpRegistered != 0)
        {   // need 2nd query (otp user)
            // change configuration
            account->m_OtpStep.value = account->m_OtpStep.eOtp_WaitOtp;
            MSG_AU_AUTH_OTP_PASS_CMD msg_cmd;
            msg_cmd.command = msg_cmd.eCommand_Require_OneTimePassword;
            msg_cmd.front_user_id = account->m_LoginFrontUserId;
            front->SendPacket(&msg_cmd, sizeof(msg_cmd));
            AUTH_DEBUGLOG("Send 'MSG_AU_AUTH_OTP_PASS_CMD' %d\n", this->m_OtpRegistered);
        }
        else

        {
            // success = [0|0], [1|0], [1|1]
            account->m_status = ACCOUNTSTATUS_AUTH;

            if (this->m_ReAuthKey == 0) // not server select status
            {
                MSG_AU_AUTH_OTP_PASS_CMD msg_cmd;
                msg_cmd.command = otp_serviced ? msg_cmd.eCommand_AuthSuccess_HasNotOneTimePassword
                    :                msg_cmd.eCommand_AuthSuccess_NoOtpEvent;

                msg_cmd.front_user_id = account->m_LoginFrontUserId;
                front->SendPacket(&msg_cmd, sizeof(msg_cmd));
                AUTH_DEBUGLOG("Send 'MSG_AU_AUTH_OTP_PASS_CMD' [0|1] %d\n", 0);
            }

            MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msg_ack;
            msg_ack.m_dwFrontUserID = m_dwFrontUserId;
            msg_ack.m_byFailCount = 0;
            msg_ack.m_byResult = AUTHRESULT_OK;
            ::_tcsncpy(msg_ack.m_szInfo, g_LoginErrorString[msg_ack.m_byResult],
                _countof(msg_ack.m_szInfo));
            front->SendPacket(&msg_ack, sizeof(msg_ack));

            SUNLOG(eFULL_LOG, "[ResultNetCafeLog] Account[%s] NetCafe[%u] Login Success!",
                m_account, m_NetCafeNum);
        }
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    }    
    else
    {
        account->m_status = ACCOUNTSTATUS_AUTH;
        MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msg_ack;
        msg_ack.m_dwFrontUserID = m_dwFrontUserId;
        msg_ack.m_byFailCount = 0;
        msg_ack.m_byResult = AUTHRESULT_OK;
        ::_tcsncpy(msg_ack.m_szInfo, g_LoginErrorString[msg_ack.m_byResult],
            _countof(msg_ack.m_szInfo));
	#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        ::_tcsncpy(msg_ack.m_szID, m_account , _countof(msg_ack.m_szID));
        msg_ack.m_szID[_countof(msg_ack.m_szID) - 1] = '\0';
	#endif
        front->SendPacket(&msg_ack, sizeof(msg_ack));

        SUNLOG(eFULL_LOG, "[ResultNetCafeLog] Account[%s] NetCafe[%u] Login Success!",
            m_account, m_NetCafeNum);
    }
    #endif
#else
    account->m_status = ACCOUNTSTATUS_AUTH;
    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msg_ack;
    msg_ack.m_dwFrontUserID = m_dwFrontUserId;
    msg_ack.m_byFailCount = 0;
    msg_ack.m_byResult = AUTHRESULT_OK;
    ::_tcsncpy(msg_ack.m_szInfo, g_LoginErrorString[msg_ack.m_byResult],
        _countof(msg_ack.m_szInfo));
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	::_tcsncpy(msg_ack.m_szID, m_account , _countof(msg_ack.m_szID));
	msg_ack.m_szID[_countof(msg_ack.m_szID) - 1] = '\0';
#endif
    front->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, "[ResultNetCafeLog] Account[%s] NetCafe[%u] Login Success!",
        m_account, m_NetCafeNum);
#endif

    /*
  #ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    if (this->m_ReAuthKey == 0 && this->m_OtpRegistered != 0)
    {   // need 2nd query (otp user)
        // change configuration
        account->m_OtpStep.value = account->m_OtpStep.eOtp_WaitOtp;
        MSG_AU_AUTH_OTP_PASS_CMD msg_cmd;
        msg_cmd.command = msg_cmd.eCommand_Require_OneTimePassword;
        msg_cmd.front_user_id = account->m_LoginFrontUserId;
        front->SendPacket(&msg_cmd, sizeof(msg_cmd));
        AUTH_DEBUGLOG("Send 'MSG_AU_AUTH_OTP_PASS_CMD' %d\n", this->m_OtpRegistered);
    }
    else
  #endif
    {
        // success = [0|0], [1|0], [1|1]
        account->m_status = ACCOUNTSTATUS_AUTH;
  #ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
        if (this->m_ReAuthKey == 0) // not server select status
        {
            MSG_AU_AUTH_OTP_PASS_CMD msg_cmd;
            msg_cmd.command = otp_serviced ? msg_cmd.eCommand_AuthSuccess_HasNotOneTimePassword
                            :                msg_cmd.eCommand_AuthSuccess_NoOtpEvent;

            msg_cmd.front_user_id = account->m_LoginFrontUserId;
            front->SendPacket(&msg_cmd, sizeof(msg_cmd));
            AUTH_DEBUGLOG("Send 'MSG_AU_AUTH_OTP_PASS_CMD' [0|1] %d\n", 0);
        }
  #endif
        MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msg_ack;
        msg_ack.m_dwFrontUserID = m_dwFrontUserId;
        msg_ack.m_byFailCount = 0;
        msg_ack.m_byResult = AUTHRESULT_OK;
        ::_tcsncpy(msg_ack.m_szInfo, g_LoginErrorString[msg_ack.m_byResult],
                   _countof(msg_ack.m_szInfo));
        front->SendPacket(&msg_ack, sizeof(msg_ack));

        SUNLOG(eFULL_LOG, "[ResultNetCafeLog] Account[%s] NetCafe[%u] Login Success!",
               m_account, m_NetCafeNum);
    }*/
#endif //_NET_CAFE
}

QueryNetCafe2ndAuth::QueryNetCafe2ndAuth()
    : QueryObject(QUERYTYPE_NETCAFE_OTP_CHECK)
{
    ClearField();

    m_recordSet.RegisterParameter(&result_data_.auth_ret, "",
                                  adInteger, adParamReturnValue,
                                  sizeof(result_data_.auth_ret), "");
}

void
QueryNetCafe2ndAuth::onResult()
{
    QueryObject::onResult();
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    LoginFrontServerSession* front =
        static_cast<LoginFrontServerSession*>(AuthFrame::Instance()->FindSession(m_key));
    if(front == NULL) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Cannot find LoginFront[%u]", m_key);
        return;
    }

    AccountInfo* account = AccountManager::Instance()->FindWithLoginFrontUserID(front_user_id_);
    if(account == NULL) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Cannot find account[%s]", query_info_.account);
        return;
    }

    // specification:
    //{
    //    1 = success,
    //    0 = mismatch,
    //    -1 = can't find otp
    //};

    MSG_AU_AUTH_OTP_PASS_ACK pass_ack;
    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK check_ack;
    check_ack.m_dwFrontUserID = front_user_id_;
    check_ack.m_byFailCount = 0;

    BOOLEAN need_send_check_ack = false;

    AUTH_DEBUGLOG("|Result=%d|\n", result_data_.auth_ret);
    // need process
    if (false)
    {
    }
    else if (result_data_.auth_ret > 0)
    { // success
        pass_ack.result = pass_ack.eResult_AuthSuccess;
        pass_ack.front_user_id = account->m_LoginFrontUserId;
        check_ack.m_byResult = AUTHRESULT_OK;

        front->SendPacket(&pass_ack, sizeof(pass_ack));
        need_send_check_ack = true;

        // (NOTE) 하기 시간 갱신 루틴의 경우는 매번 하도록 유지 한다.
        account->m_tickConnectTime	= ::GetTickCount();
        ::GetLocalTime(&account->m_loginTime);
        account->m_status = ACCOUNTSTATUS_AUTH;
    }
    else if (result_data_.auth_ret == 0)
    { // mismatch
        pass_ack.result = pass_ack.eResult_Retry;
        pass_ack.front_user_id = account->m_LoginFrontUserId;
        front->SendPacket(&pass_ack, sizeof(pass_ack));
    }
    else if (result_data_.auth_ret == -1)
    { // expired
        pass_ack.result = pass_ack.eResult_OtpExpired;
        pass_ack.front_user_id = account->m_LoginFrontUserId;

        front->SendPacket(&pass_ack, sizeof(pass_ack));
    }
    else if (result_data_.auth_ret == -2)
    { // can't find otp
        check_ack.m_byResult = AUTHRESULT_INVALIDACCOUNT;
        pass_ack.result = pass_ack.eResult_Fail;
        pass_ack.front_user_id = account->m_LoginFrontUserId;

        front->SendPacket(&pass_ack, sizeof(pass_ack));
        need_send_check_ack = true;
    }

    if (need_send_check_ack)
    {
        ::_tcsncpy(check_ack.m_szInfo, g_LoginErrorString[check_ack.m_byResult],
                   _countof(check_ack.m_szInfo));
        front->SendPacket(&check_ack, sizeof(check_ack));
    }
#endif
}

//#endif _NET_CAFE