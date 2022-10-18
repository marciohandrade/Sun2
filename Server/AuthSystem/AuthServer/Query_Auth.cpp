#include "stdafx.h"
#include "Query_Auth.h"
#include "AuthFrame.h"
#include "WZ_MD5.h"
#include "QueryManager.h"
#include "AlertRequestSender.h"
#include "AccountManager.h"
#include "ServerDataManager.h"
#include "AuthAgentServerSession.h"
#include "LoginFrontServerSession.h"
#include <AuthProtocol_FS.h>
#include <AuthProtocol_AS.h>
#include <AuthStepAuthToken.h>
#include "ReAuthInfo.h"

#ifdef _KR_0_20120207_CRYPT_SHA256
    #include "../ProgramCommon/SHA256.h"
#endif


#if defined(_JP_0_20100723_GAMECHU_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__ ) || defined(_NA_20100307_BETAKEY_SYSTEM)    
#include <atlconv.h>
#endif

extern TCHAR g_LoginErrorString[AUTHRESULT_MAX][MAX_AUTHRESULT_MESSAGE_LEN + 1];

#ifdef _CHANNELING_AUTH_FLOW

BOOST_STATIC_ASSERT(sizeof(MSG_CF_AUTH_TOKEN_PASS_SYN::Stream) > sizeof(DB_LOGIN_USER::AuthToken) &&
                    sizeof(DB_LOGIN_USER::AuthToken) == sizeof(wchar_t) * (36 + 1));

BOOST_STATIC_ASSERT(AuthStepAuthToken::DB_AuthToken_Length == DB_LOGIN_USER::DB_AuthToken_Length &&
                    sizeof(AuthStepAuthToken::AuthTokenStreamTimeout) == sizeof(DB_LOGIN_USER::AuthToken));

#endif //_CHANNELING_AUTH_FLOW

QueryAuth::QueryAuth()
    : QueryObject(QUERYTYPE_LOGINUSER)
    , m_TokenType(eAuthToken_None)
    , m_front_user_id(0)
{
    //Sts3는 중국에서만 실명 인증 식별용도로 쓰임(0 : 비실명 , 1 : 실명)
    m_recordSet.RegisterVariable(&m_Record.UserGuid, "UserGuid");
    m_recordSet.RegisterVariable(m_Record.PassWD, "PassWD");
    m_recordSet.RegisterVariable(&m_Record.Sts[0], "Sts");
    m_recordSet.RegisterVariable(&m_Record.Sts[1], "Sts2");
    m_recordSet.RegisterVariable(&m_Record.Sts[2], "Sts3");
    m_recordSet.RegisterVariable(m_Record.CharPassWD, "CharPassWD");
    m_recordSet.RegisterVariable(&m_Record.Age, "Age");
    m_recordSet.RegisterVariable(&m_Record.IpGuid, "IpGuid");
#ifdef _SECURE_AUTH
    m_recordSet.RegisterVariable(&m_Record.pData, "CardNo");
#endif
    m_recordSet.RegisterVariable(&m_Record.btAuthFailCount, "AuthFailCount");
#ifdef _AUTH_MATRIX_FAIL_COUNT
    m_recordSet.RegisterVariable(&m_Record.btShapeAuthFailCount, "ShapeAuthFailCount");
    m_recordSet.RegisterVariable(&m_Record.btMatrixAuthFailCount, "MatrixAuthFailCount");
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
#endif
#ifdef _CHANNELING_AUTH_FLOW
    m_recordSet.RegisterVariable(&m_Record.auth_token_result, "AuthKey");
#endif
#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
    m_recordSet.RegisterVariable(&m_Record.userType, "UserType");
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    m_recordSet.RegisterVariable(m_Record.user_account, "UserID");
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	m_recordSet.RegisterVariable(&m_Record.is_token_use_, "is_token_auth");	
	m_recordSet.RegisterVariable(m_Record.token_base64_key_, "base64string");
#endif
    m_recordSet.RegisterParameter(&m_DbResult, "", adInteger, adParamReturnValue, sizeof(int), "");
}

void
QueryAuth::Configuration(eAuthToken token_type)
{
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    gamechu_usn_ = 0;
#endif
    m_key = 0;
    m_TokenType = token_type;
    m_ReAuthKey = 0;
    m_front_user_id = 0;
    m_DbResult = eQueryResult_Undefined;
    ZeroMemory(&m_Record, sizeof(m_Record));
    ZeroMemory(&m_account, sizeof(m_account));
    ZeroMemory(&m_password, sizeof(m_password));
    ZeroMemory(&m_clientIp, sizeof(m_clientIp));
    ZeroMemory(&m_localIp, sizeof(m_localIp));
    ZeroMemory(&m_AuthToken, sizeof(m_AuthToken));
}


BOOLEAN
QueryAuth::ValidClientAuthInfo()
{
    BOOLEAN is_matched = FALSE;
    if (m_TokenType == eAuthToken_None)
    {
        is_matched = CheckPassword();
    }
    else if (m_TokenType == eAuthToken_AuthTokenKorea)
    {
        // DB_LOGIN_USER::DB_AuthToken_Length ==> hard-code value (DB AuthToken 32Byte)
        BOOST_STATIC_ASSERT(sizeof(m_AuthToken) == sizeof(m_Record.auth_token_result) &&
                            sizeof(m_AuthToken) == _countof(m_AuthToken) * sizeof(wchar_t));

        BOOLEAN is_null = memcmp(AuthStepAuthToken::AuthTokenStreamNull,
                                m_Record.auth_token_result,
                                sizeof(m_Record.auth_token_result)) == 0;

        BOOLEAN is_timeout = TRUE;
        if (is_null == FALSE)
        {   
            // "00000000-0000-0000-0000-000000000000" timeout stream
            is_timeout = memcmp(AuthStepAuthToken::AuthTokenStreamTimeout,
                               m_Record.auth_token_result,
                               sizeof(m_Record.auth_token_result)) == 0;
        }

        //////////////////////////////////////////////////////////////////////////
        // current step status table
        //  isNull  | is_timeout
        //     0          0     - O (compare client auth_token)
        //     0          1     - Timeout
        //     1          0     - X
        //     1          1     - Already Queried | Server Selecting Client
        //////////////////////////////////////////////////////////////////////////

        if (is_timeout == FALSE)
        {
            is_matched = memcmp(m_AuthToken, m_Record.auth_token_result,
                                sizeof(m_Record.auth_token_result)) == 0;
        }
        else
        {
            //////////////////////////////////////////////////////////////////////////
            // if server selecting client?
            // F: already queried client (irregular)
            // T: compare 're-authentication token stream' with 'client auth token'
            //////////////////////////////////////////////////////////////////////////
            is_matched = ReauthTokenAuthentication();

            if (is_matched == FALSE) {
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] reauth token auth failed [Account:%s]", this->m_account);
            }
        }

        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] [is_matched:%d][is_null:%d][is_timeout:%d] [Account:%s]", is_matched, is_null, is_timeout, this->m_account);
    }
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    else if (m_TokenType == eAuthToken_AuthTokenNorthA)
    {
        BOOST_STATIC_WARNING(0);
        is_matched = true;

    //#define __WAVERIX_DEBUG_CODE__
    #ifdef __WAVERIX_DEBUG_CODE__
        BYTE tokenStream[128];
        char outputMsg[1024];
        int iWritten = 0;
        //////////////////////////////////////////////////////////////////////////
        SUNLOG(eCRITICAL_LOG, "ID: {%s} AuthToken DBStream", m_account);
        BOOST_STATIC_ASSERT(sizeof(tokenStream) >= sizeof(m_Record.auth_token_result));
        CopyMemory(tokenStream, &m_Record.auth_token_result, sizeof(m_Record.auth_token_result));
        iWritten = 0;
        for(int i = 0; i < sizeof(m_Record.auth_token_result); ++i)
            iWritten += _snprintf(&outputMsg[iWritten], _countof(outputMsg) - iWritten,
            "[%02X]", tokenStream[i]);
        SUNLOG(eCRITICAL_LOG, outputMsg);
        //////////////////////////////////////////////////////////////////////////
        SUNLOG(eCRITICAL_LOG, "ID: {%s} AuthToken UserStream", m_account);
        BOOST_STATIC_ASSERT(sizeof(tokenStream) >= sizeof(m_AuthToken));
        CopyMemory(tokenStream, &m_AuthToken, sizeof(m_AuthToken));
        iWritten = 0;
        for(int i = 0; i < sizeof(m_AuthToken); ++i)
            iWritten += _snprintf(&outputMsg[iWritten], _countof(outputMsg) - iWritten,
            "[%02X]", tokenStream[i]);
        SUNLOG(eCRITICAL_LOG, outputMsg);
        //////////////////////////////////////////////////////////////////////////
    #endif
    }
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    else if(m_TokenType == eAuthToken_AuthTokenGameChu)
    {
        is_matched = true;
    }
#endif
#ifdef _RU_INGAMBA_AUTH_PROCESS
    else if(m_TokenType == eAuthToken_AuthIngamba)
    {
        is_matched = true;
    }
#endif
#ifdef _CN_ZY_AUTH_PROCESS
    else if (m_TokenType == eAuthToken_AuthChinaZY) {
#ifdef _CN_ZY_AUTH_PROCESS_DBAUTH
        is_matched = true;
#else
        is_matched = CheckPassword();
#endif //_CN_ZY_AUTH_PROCESS_DBAUTH
    }
#endif //_CN_ZY_AUTH_PROCESS
    return is_matched;
}

//#endif //_CHANNELING_AUTH_FLOW

BOOLEAN
QueryAuth::ReauthTokenAuthentication()
{
    BOOLEAN is_matched = false;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    if(m_ReAuthKey != 0)
    {
        ReAuthManager* pReAuthManager = ReAuthManager::Instance();
        const ReAuthInfo* pReAuthInfo = pReAuthManager->FindReAuthInfo(m_ReAuthKey);
        if(pReAuthInfo != 0)
        {   
            // 이미 m_ReAuthKey가 할당되는 시점 검증된 값임.
            pReAuthManager->OnAcceptQueryLoginUser(pReAuthInfo);
            is_matched = true;
        }
    }
#endif
    return is_matched;
}

BOOLEAN
QueryAuth::CheckPassword()
{
    wchar_t password_widen[32] = { 0, };
    wchar_t key_widen[32] = { 0, };

    char* password      = m_password;
    int   password_len  = int(strlen(m_password));

    MultiByteToWideChar(CP_ACP, 0,
                        password,       password_len + 1,
                        password_widen, _countof(password_widen));

    char* key           = m_account;
    int   key_len       = int(strlen(m_account));

    MultiByteToWideChar(CP_ACP, 0,
                        key,        key_len + 1,
                        key_widen,  _countof(key_widen));

    bool is_matched = false;
#ifdef _KR_0_20120207_CRYPT_SHA256
    int mode = AuthFrame::Instance()->GetMode();
    switch (mode)
    {
    case eSERVER_DEV:
        {
            is_matched = !!MD5::Compare((BYTE*)password_widen, int(wcslen(password_widen) * sizeof(wchar_t)),
                (BYTE*)m_Record.PassWD, // digest[16]
                (BYTE*)key_widen, int(wcslen(key_widen) * sizeof(wchar_t)));
        }
        break;
    case eSERVER_INTRAOFFICE:
    case eSERVER_LIVE:
    case eSERVER_TESTLIVE:
        {
            Crypt::SHA256 sha256;
            is_matched = sha256.Compare(password, (const char*)m_account, m_Record.PassWD);
            if (is_matched == false && strlen(sha256.GetLastError()) != 0)
                SUNLOG(eCRITICAL_LOG, "[SHA256]Account FAIL : %s", sha256.GetLastError());
        }
        break;
    default:
        is_matched = false;
        break;
    }
#else
    is_matched =
        !!MD5::Compare((BYTE*)password_widen, int(wcslen(password_widen) * sizeof(wchar_t)),
                       (BYTE*)m_Record.PassWD, // digest[16]
                       (BYTE*)key_widen, int(wcslen(key_widen) * sizeof(wchar_t)));
#endif

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    if(is_matched == 0 && m_ReAuthKey != 0)
    {
        ReAuthManager* pReAuthManager = ReAuthManager::Instance();
        const ReAuthInfo* pReAuthInfo = pReAuthManager->FindReAuthInfo(m_ReAuthKey);
        if(pReAuthInfo != 0)
        {   // 이미 m_ReAuthKey가 할당되는 시점 검증된 값임.
            pReAuthManager->OnAcceptQueryLoginUser(pReAuthInfo);
            is_matched = true;
        }
    }
#endif
    return is_matched;
}

/*
BOOLEAN
QueryAuth::CheckPassword(LPTSTR key, LPTSTR password, LPTSTR recvPasswd)
{
	WCHAR wszPasswd[32] = {0, };
	WCHAR wszKey[32] = {0, };

	::ZeroMemory(wszPasswd, 32);
	::MultiByteToWideChar(CP_ACP, 0,
                        recvPasswd, (int)strlen(recvPasswd) + 1,
                        wszPasswd, (int)sizeof(wszPasswd) / 2);

	::ZeroMemory(wszKey, 32);
	::MultiByteToWideChar(CP_ACP, 0, key, (int)strlen(key) + 1, wszKey, (int)sizeof(wszKey) / 2);

	if(MD5::Compare((BYTE*)wszPasswd, (int)strlen(recvPasswd) * 2,
		            (BYTE*)password, (BYTE*)wszKey, (int)strlen(key) * 2))
	{
		return TRUE;
	}

	return FALSE;
}
*/

AccountInfo*
QueryAuth::CheckOverlapLogin(DWORD user_guid, 
                             LPTSTR account,
                             DWORD login_front_id, 
                             DWORD front_user_id, 
                             BYTE& result_value)
{
    AuthFrame* const auth_frame = AuthFrame::Instance();
    AccountManager* const account_manager = AccountManager::Instance();

	LoginFrontServerSession* login_front = (LoginFrontServerSession*)auth_frame->FindSession(login_front_id);

	// 이전 접속자가 있다면, 그 정보를 얻어온다.
	AccountInfo* prev_account_info = NULL;
	AccountInfo* current_account_info = 
        account_manager->CreateAccountInfo(account, &prev_account_info);

	// 이전 접속자가 없으면 성공!
	if (prev_account_info == NULL)
	{
		current_account_info->m_LoginFrontId = login_front_id;
		current_account_info->m_LoginFrontUserId	= front_user_id;
		current_account_info->m_locationId = 0;
		current_account_info->m_userGuid = user_guid;
		return current_account_info;
	}

	if(prev_account_info->m_locationId == NULL)
	{
        // 현재 인증중인 계정이 존재(서버를 선택하지 않은 상황)
		result_value = AUTHRESULT_PROCESSLOGIN;
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : account : %s, prev auth status : %u",
            prev_account_info->m_account, 
            prev_account_info->m_status
            );
	}
	else
	{
        // 현재 접속중인 계정이 존재(서버를 선택한 상황)
		result_value = AUTHRESULT_ALREADYLOGIN;
		SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : account : %s, prev auth status : %u, game agent id : %u",
            prev_account_info->m_account, 
            prev_account_info->m_status, 
            prev_account_info->m_locationId
            );

		AuthAgentServerSession* auth_agent_server =
            ServerDataManager::Instance()->FindAuthAgentServerSession(prev_account_info->m_locationId);
		if (auth_agent_server)
		{
			// 이전 접속자 강제종료 요청
			MSG_SA_AUTH_ASK_PC_LOGOUT_SYN logout_msg;
			logout_msg.m_dwAuthUserID = prev_account_info->m_authKey;
			logout_msg.m_GameAgentID = prev_account_info->m_locationId;
			logout_msg.m_dwUserGUID = prev_account_info->m_userGuid;
            CHECK_ID_LENGTH_BUFFER(logout_msg.m_szID);
            CHECK_ID_LENGTH_BUFFER(prev_account_info->m_account);
			::_tcsncpy(logout_msg.m_szID, prev_account_info->m_account, _countof(logout_msg.m_szID));
            logout_msg.m_szID[_countof(logout_msg.m_szID) - 1] = '\0';
			logout_msg.m_dwLogoutCode = eDRC_OVERLAPPEDCONN;

			auth_agent_server->SendPacket(&logout_msg, sizeof(logout_msg));
		}
		else
		{
			// 다운된 서버의 경우, 위치정보를 삭제한다.
			account_manager->ClearInfo(prev_account_info->m_account);
		}
	}

	return NULL;
}

VOID
QueryAuth::onResult()
{
    QueryObject::onResult();

    AuthFrame* const auth_frame = AuthFrame::Instance();
    QueryManager* const query_manager = QueryManager::Instance();

    LoginFrontServerSession* login_front = 
        static_cast<LoginFrontServerSession*>(auth_frame->FindSession(m_key));
    if (login_front == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found LoginFront : %u", m_key
            );
        return;
    }

    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK ack_msg;
    ack_msg.m_dwFrontUserID = m_front_user_id;
    ack_msg.m_byFailCount = 0;
    ack_msg.m_byResult = AUTHRESULT_OK;
#ifdef __CN_20100909_SECURITY_TOKEN__
    ack_msg.m_Is_Token_Use = m_Record.is_token_use_;	
#endif
    // - eQueryResult_Undefined  = -1,  Impossible
    // - eQueryResult_Success    = 0,   Query Successed
    // - eQueryResult_Fail       = 1,   can't find user, 기존 일반 에러.

    if (m_DbResult != eQueryResult_Success &&
        m_DbResult != eQueryResult_Fail)
    {
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        if(m_TokenType == eAuthToken_AuthTokenGameChu)    
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Db Query Result : %d, about : %u",
                m_DbResult, 
                gamechu_usn_
                );
        }
        else
        {
#endif
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__ " : Db Query Result : %d - about : %s", 
                m_DbResult, 
                m_account
                );
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        }
#endif
    }

    // 해당 계정이 DB에 있는지 체크
    if (m_recordSet.GetRecordCount() == 0)
    {
        ack_msg.m_byResult = AUTHRESULT_INVALIDACCOUNT;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        if(m_TokenType == eAuthToken_AuthTokenGameChu)    
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : doesn't exist in DB : GameChu USN : %u", 
                gamechu_usn_
                );
        }
        else
        {
#endif
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : doesn't exist in DB : Account : %s", 
                m_account
                );
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        }
#endif  
        goto ERROR_RESULT;
    }

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_ 
    //게임츄 인증에서는 m_account 에 게임츄 USN 이 있으므로 여기서 교체
    if (m_TokenType == eAuthToken_AuthTokenGameChu)    
    {
        USES_CONVERSION;
        ::_tcsncpy(m_account, W2A(m_Record.user_account), _countof(m_account));
        m_account[_countof(m_account) - 1] = '\0';

        _strupr(m_account);

        ::_tcsncpy(ack_msg.m_szID, m_account , _countof(m_account));
        ack_msg.m_szID[_countof(ack_msg.m_szID) - 1] = '\0';
    }
#endif

    //계정상태(Sts) 검사
    if (auth_frame->IsAllowSts((USER_STS)m_Record.Sts[0]) == false)
    {
        ack_msg.m_byResult = AUTHRESULT_NOTALLOWEDACCOUNT;
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Is not allowed : account : %s, status : %u",
            m_account, 
            m_Record.Sts[0]
            );
            goto ERROR_RESULT;
    }

#ifdef _RESTRICT_ADULT
    //18세 이하는 접속 불가 판정
    if (m_Record.Age < 18)
    {
        ack_msg.m_byResult = AUTHRESULT_RESTRICTADULT;
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Is not allowed : account : %s, age : %u",
            m_account,
            m_Record.Age
            );
        goto ERROR_RESULT;
    }
#endif

    const DWORD limit_block_count = auth_frame->GetBlockLimit();

    //계정 블록 검사
    if (m_Record.btAuthFailCount >= limit_block_count)
    {
        ack_msg.m_byResult = AUTHRESULT_ACCOUNT_BLOCK;
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Block account : account : %s, fail count : %u",
            m_account, 
            m_Record.btAuthFailCount
            );
        goto ERROR_RESULT;
    }

    //암호 검사
    BOOLEAN is_valid_client = ValidClientAuthInfo();
    if (is_valid_client == FALSE)
    {
#ifdef _AUTH_CHINA
        ack_msg.m_byResult = AUTHRESULT_INVALIDACCOUNT;
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__ " : Invalid password : Account : %s", 
            m_account
            );
        goto ERROR_RESULT;
#else
        //DB에 연속 오류 횟수를 증가시킨 후 ACK를 보낸다.
        query_manager->QueryUpdateAuthFailCount(m_account, m_clientIp, 0, 
            m_front_user_id, m_key, 
            ACCOUNT_AUTH);
        return;
#endif
    }

#if !defined(_SECURE_AUTH) && !defined(_AUTH_CHINA)
    // 인증 성공 후엔 연속오류회수를 초기화
    query_manager->QueryUpdateAuthFailCount(m_account, 
        m_clientIp, 
        1,
        m_front_user_id, 
        m_key, 
        ACCOUNT_AUTH);
#endif

    AccountInfo* account_info = CheckOverlapLogin(m_Record.UserGuid, 
                                                  m_account, 
                                                  m_key, 
                                                  m_front_user_id,
                                                  ack_msg.m_byResult);
    if (account_info == NULL)
    {
        goto ERROR_RESULT;
    }
    else
    {
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        account_info->gamechu_usn_ = gamechu_usn_;
#endif//_JP_0_20100723_GAMECHU_AUTHFLOW_

#ifdef __CN_20100909_SECURITY_TOKEN__
        account_info->is_token_use_ = m_Record.is_token_use_;
        if (m_Record.is_token_use_)
        {            
            USES_CONVERSION;
            ::_tcsncpy(account_info->token_base64_key_, 
                W2A(m_Record.token_base64_key_),
                _countof(account_info->token_base64_key_));
            account_info->token_base64_key_[_countof(account_info->token_base64_key_) - 1] = '\0';
        }
#endif//__CN_20100909_SECURITY_TOKEN__

#ifdef _NA_20100307_BETAKEY_SYSTEM
        account_info->betakey_ = ack_msg.m_betakey_ = 0;
#endif//_NA_20100307_BETAKEY_SYSTEM

        BOOST_STATIC_ASSERT(_countof(account_info->m_userCode) == _countof(m_Record.Sts));
        for (int i = 0; i < _countof(account_info->m_userCode); ++i)
        {
            account_info->m_userCode[i] = 
                static_cast<BYTE>(m_Record.Sts[i]);
        }
        account_info->m_LoginFrontId = m_key;
        account_info->m_userGuid = m_Record.UserGuid;
        account_info->m_age = static_cast<BYTE>(m_Record.Age);
        account_info->m_pcBangId = m_Record.IpGuid;
        ::_tcsncpy(account_info->m_clientIp, m_clientIp, IP_MAX_LEN);
        ::_tcsncpy(account_info->m_localIp, m_localIp, IP_MAX_LEN);

#ifdef __PCBANG_IP_UPDATE
        account_info->pc_room_ip_value_ = 
            ::inet_addr(account_info->m_clientIp);
#endif//__PCBANG_IP_UPDATE

        BOOST_STATIC_ASSERT(
            sizeof(account_info->m_MD5) == sizeof(m_Record.CharPassWD)
            );

        ::memcpy(account_info->m_MD5, 
            m_Record.CharPassWD, 
            sizeof(account_info->m_MD5));
        account_info->m_AuthUserType = static_cast<BYTE>(m_Record.userType);

#if defined(_SECURE_AUTH)
        //매트릭스 인증
        if (m_Record.pData[0] != '\0')
        {
            //매트릭스 카드 사용자일 경우
            auth_frame->SendMatrixCardAuth(login_front, 
                account_info, 
                m_front_user_id, 
                m_Record.btMatrixAuthFailCount, 
                1);
        }
        else
        {
            //아니면 그냥 성공
            ack_msg.m_byResult = AUTHRESULT_OK;
            ::_tcsncpy(ack_msg.m_szInfo, 
                g_LoginErrorString[ack_msg.m_byResult], 
                _countof(ack_msg.m_szInfo));
            login_front->SendPacket(&ack_msg, sizeof(ack_msg));
            account_info->m_status = ACCOUNTSTATUS_AUTH;
        }
#else//_SECURE_AUTH
        //계정 인증만 사용
#ifdef _NET_CAFE
        account_info->m_pcBangId = 0;	// 일본 넷카페의 경우 DB에서 고유번호를 넘겨준다
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        eNetCafeLoginType token_type;
        token_type = (m_TokenType == eAuthToken_AuthTokenGameChu) ? NETCAFE_GAMECHU_LOGIN : 
        NETCAFE_ONTIME_LOGIN;

        query_manager->QueryNetCafeLogInsert(m_key, m_front_user_id, m_account, m_clientIp,
            token_type, m_ReAuthKey);
#else
        query_manager->QueryNetCafeLogInsert(m_key, m_front_user_id, m_account, m_clientIp,
            true, m_ReAuthKey);
#endif
#else//_NET_CAFE
        //중국,일본,글로벌을 제외한 다른 버전들은 이곳에서 최종 인증 성공
        ack_msg.m_byResult = AUTHRESULT_OK;
        ::_tcsncpy(ack_msg.m_szInfo, g_LoginErrorString[ack_msg.m_byResult],
            _countof(ack_msg.m_szInfo));
        login_front->SendPacket(&ack_msg, sizeof(ack_msg));
        account_info->m_status = ACCOUNTSTATUS_AUTH;
#endif//_NET_CAFE
#endif//_SECURE_AUTH

#ifdef _TIRED_CHECK
        account_info->m_ucRealNameAuth = m_Record.Sts[2];   // 실명 인증자 인지 식별
#endif

#ifdef _SECURE_AUTH
        if (m_Record.pData[0] != '\0')
        {
            // 매트릭스 카드 사용자일 경우
            char szSeedValue[(SEED_SIZE * 2) + 1] = { 0, };
            wchar_t* wszSCValue = (wchar_t*)m_Record.pData;
            ::WideCharToMultiByte(CP_ACP, 0, wszSCValue, -1, szSeedValue, 
                SEED_SIZE * 2, 0, 0);

            // 카드번호를 가지고 시드값을 만든다
            genseed(szSeedValue, account_info->m_seedValue); 
            account_info->m_bSucceedShape = true;
            SUNLOG(
                eFULL_LOG, 
                __FUNCTION__" : DB Seed Value : %s", 
                szSeedValue
                );
        }
#endif

        account_info->m_tickConnectTime	= ::GetTickCount();
        ::GetLocalTime(&account_info->m_loginTime);
    }

    SUNLOG(
        eCRITICAL_LOG, 
        __FUNCTION__" : Auth Success : account : %s, ip : %s",
        account_info->m_account, 
        account_info->m_clientIp
        );

    return;

ERROR_RESULT:
    ::_tcsncpy(ack_msg.m_szInfo, 
        g_LoginErrorString[ack_msg.m_byResult],
        _countof(ack_msg.m_szInfo));
    login_front->SendPacket(&ack_msg, sizeof(ack_msg));
}


VOID
QueryRecoverLocation::onResult()
{
	QueryObject::onResult();

	TCHAR temp_account_id[ID_MAX_LEN + 1];
	int	count = 0;

    AccountManager* const account_manager = AccountManager::Instance();

    ServerDataManager* const server_data_manager = ServerDataManager::Instance();

	AccountInfo* account_info = NULL;
	AccountInfo* temp_account_info = NULL;

	//  복구작업을 시작한다.
	//  SET NOCOUNT 를 사용하는 procedure이므로 GetRecordCount를 사용할 수 없다.
	while (m_recordSet.IsEndRecord() == false)
	{
        CHECK_ID_LENGTH_BUFFER(temp_account_id);
        ZeroMemory(temp_account_id, sizeof(temp_account_id));
        // (WAVERIX) (090331) UserID 하드코딩 값 수정
		::WideCharToMultiByte(CP_ACP, 0, m_Record.UserID, -1, temp_account_id, ID_MAX_LEN, 0, 0);

		account_info = account_manager->CreateAccountInfo(temp_account_id, &temp_account_info);
		if (account_info != NULL)
		{
			ServerData* server_data = 
                server_data_manager->FindServerData((AGENTID)m_Record.ServerGuid);

			if (server_data == NULL)
			{
				SUNLOG(
                    eCRITICAL_LOG, 
                    __FUNCTION__" : Not found server : ID : %s, Server : %u",
                    temp_account_id, 
                    m_Record.ServerGuid
                    );
				m_recordSet.Next();
				continue;
			}

            short status_type = (m_Record.ConnectSts == USERSTATUS_PLAY ? ACCOUNTSTATUS_PLAY : 0);
            account_info->m_status = static_cast<ACCOUNTSTATUS_TYPE>(status_type);
			account_info->m_userGuid = m_Record.UserGuid;
			account_info->m_locationId = server_data->GetGameAgentID();
			account_info->m_tickBillingTime	= ::GetTickCount();
			account_info->m_pcBangId = m_Record.CompGuid;
			::WideCharToMultiByte(CP_ACP, 0, m_Record.IpPublicAddr, -1, 
                                  account_info->m_clientIp, 15, 0, 0);
			::WideCharToMultiByte(CP_ACP, 0, m_Record.IpLocalAddr, -1, 
                                  account_info->m_localIp, 15, 0, 0);
			::WideCharToMultiByte(CP_ACP, 0, m_Record.AuthUserGuid, -1, 
                                  account_info->m_glogKey, 8, 0, 0);
			account_info->m_authKey = m_Record.AuthKey;
			account_info->m_tickConnectTime = account_info->m_tickBillingTime;

			SYSTEMTIME login_time;
			VariantTimeToSystemTime(static_cast<double>(m_Record.ConnectTime), &login_time);
			account_info->m_loginTime = login_time;
			++count;

			//  location Id는 1부터 시작한다.
			server_data->IncLoginUserCount();
		}

		m_recordSet.Next();
	}

#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    // (WAVERIX) (NOTE) 글로벌 인증서버는 해당 쿼리를 사용하지 않는다. 코드를 막는 이유는
    // ReadyState에 의해 listen정책이 정해지기 때문
	AuthFrame::Instance()->GetReadyState().SetReadyState(AUTH_READY_RECOVER_STS, TRUE);
#endif

	SUNLOG(eCRITICAL_LOG, "[QueryRecoverLocation::onResult] Recover Count[%d]", count);
}

VOID
QueryAuthFailCount::onResult()
{
    QueryObject::onResult();

    if(m_btFlag)
    {
        return;
    }

    // 인증 실패에 대한 처리를 한다.
    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK ack_msg;
    ack_msg.m_dwFrontUserID = m_front_user_id;
    ack_msg.m_byFailCount = 0;
    ack_msg.m_byResult = AUTHRESULT_OK;

    AuthFrame* const auth_frame = AuthFrame::Instance();
    LoginFrontServerSession* login_front = 
        static_cast<LoginFrontServerSession*>(auth_frame->FindSession(m_key));
    if (login_front == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found Login front: %u", 
            m_key
            );
        return;
    }

    AccountInfo* account_info = 
        AccountManager::Instance()->FindWithLoginFrontUserID(m_front_user_id);

    // 누적 오류 횟수 제한에 걸려 계정 블록
    if (m_btAuthFailCount >= auth_frame->GetBlockLimit())
    {
        if(account_info)
        {
        #ifdef _SECURE_AUTH
            account_info->m_bSucceedShape = false;
        #endif
        }

        ack_msg.m_byResult = AUTHRESULT_ACCOUNT_BLOCK;
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Block account : account : %s, fail count : %u",
            m_account, 
            m_btAuthFailCount
            );
        goto ERROR_RESULT;
    }

    switch(m_btAuthType)
    {
    case ACCOUNT_AUTH:
        {
            ack_msg.m_byResult = AUTHRESULT_INVALIDACCOUNT;
            ack_msg.m_byFailCount = static_cast<BYTE>(m_btAuthFailCount);
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Invalid password : account : %s",
                m_account
                );
            goto ERROR_RESULT;
        }
        break;

#ifdef _SECURE_AUTH
    case MATRIX_CARD_AUTH:
        {
            auth_frame->SendMatrixCardAuth(login_front, account_info, m_front_user_id, 0, 0);
            return;
        }
        break;
#endif
    default:
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Invalid auth type : %u",
                m_btAuthType
                );
        }
        break;
    }

    return;

ERROR_RESULT:
    ::_tcsncpy(ack_msg.m_szInfo, g_LoginErrorString[ack_msg.m_byResult],
               _countof(ack_msg.m_szInfo));
    login_front->SendPacket(&ack_msg, sizeof(ack_msg));
}

QueryCheckPassword::QueryCheckPassword()
    : QueryObject(QUERYTYPE_CHECK_PASSWORD)
{
    memset(m_account, 0, sizeof(m_account));
    memset(m_password, 0, sizeof(m_password));
    m_front_user_id = 0;

    m_recordSet.RegisterParameter(&m_DbResult, "", adInteger, adParamReturnValue, sizeof(int), "");
}

VOID QueryCheckPassword::onResult()
{
    QueryObject::onResult();

    AuthFrame* const auth_frame = AuthFrame::Instance();
    QueryManager* const query_manager = QueryManager::Instance();

    LoginFrontServerSession* login_front = 
        static_cast<LoginFrontServerSession*>(auth_frame->FindSession(m_key));
    if (login_front == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found LoginFront : %u", m_key
            );
        return;
    }

    if (m_DbResult == 0) // success
    {
#ifdef _CN_ZY_AUTH_PROCESS_DBAUTH
        query_manager->QueryLoginUserByChinaZY( \
            m_key/*login_front_id*/, m_account, NULL/*password*/, \
            m_front_user_id, m_ReAuthKey, m_clientIp, m_localIp);
#endif //_CN_ZY_AUTH_PROCESS_DBAUTH
    }
    else
    {
        MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK ack_msg;
        ack_msg.m_dwFrontUserID = m_front_user_id;
        ack_msg.m_byFailCount = 0;
        ack_msg.m_byResult = AUTHRESULT_INVALIDACCOUNT;
        login_front->SendPacket(&ack_msg, sizeof(ack_msg));
    }
}
