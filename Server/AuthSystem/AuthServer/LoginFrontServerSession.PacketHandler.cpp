#include "stdafx.h"
#include "LoginFrontServerSession.h"
#include "AccountManager.h"
#include "AuthAgentServerSession.h"
#include "ServerDataManager.h"
#include "QueryManager.h"
#include "AuthFrame.h"
#include <AuthProtocol_FS.h>
#include <AuthProtocol_AS.h>
#include <AuthStepAuthToken.h>
#include "ReAuthInfo.h"
#include <HanAuth/HanAuthQueryManager.h>

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
#include "GCCertificationHelper.h"
#include <atlconv.h>
#endif

#ifdef __CN_20100909_SECURITY_TOKEN__
#include "DCSH_SAE_1.h"
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
#include "IngambaAuthConnector.h"

using namespace Ingamba;
using namespace PaidServicesIntegration;
using namespace Wcf;
using namespace NativeClient::Dto;

#endif

extern TCHAR g_LoginErrorString[AUTHRESULT_MAX][MAX_AUTHRESULT_MESSAGE_LEN+1];

VOID
LoginFrontServerSession::OnRecv(BYTE *pMsg, WORD wSize)
{
    MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL *)pMsg;
    SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pRecvMsg, wSize);

    switch(pRecvMsg->m_byProtocol)
    {
//#ifdef _CHANNELING_AUTH_FLOW
    case FS_AUTH_ID_PASSWORD_CHECK_SYN:	OnFS_AUTH_AUTHENTICATION_SYN(pRecvMsg, wSize); break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    case FS_AUTH_REAUTH_PASS_SYN:       OnFS_AUTH_AUTHENTICATION_SYN(pRecvMsg, wSize); break;
#endif
//#else
//    case FS_AUTH_ID_PASSWORD_CHECK_SYN:	OnFS_AUTH_ID_PASSWORD_CHECK_SYN(pRecvMsg, wSize);	break;
//#endif
    case FS_AUTH_SERVER_SELECT_SYN:		OnFS_AUTH_SERVER_SELECT_SYN(pRecvMsg, wSize);		break;
    case FS_AUTH_CLIENT_DISCONNECT_CMD:	OnFS_AUTH_CLIENT_DISCONNECT_CMD(pRecvMsg, wSize);	break;
#ifdef _SECURE_AUTH
    case FS_AUTH_MATRIX_CARD_ACK:		OnFS_AUTH_MATRIX_CARD_ACK(pRecvMsg, wSize);		break;
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    case AU_AUTH_OTP_PASS_SYN: OnAU_AUTH_OTP_PASS_SYN(pRecvMsg, wSize); break;
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__		
	case AU_AUTH_SECURITY_TOKEN_SYN: OnAU_AUTH_SECURITY_TOKEN_SYN(pRecvMsg, wSize); break;
#endif
//#ifdef _SHAPE_AUTH
//    case FS_AUTH_SHAPE_ACK:				OnFS_AUTH_SHAPE_ACK(pRecvMsg, wSize);				break;
//#endif
    default:
        {
            SUNLOG(eCRITICAL_LOG, "[LoginFrontServerSession]Invalid Packet! Category[%u] Protocol[%u] Size[%u]", pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
        }
    }
}

//_CHANNELING_AUTH_FLOW
class AuthStepAuthTokenInAuthServer : public AuthStepAuthToken
{
public:
    AuthStepAuthTokenInAuthServer(DWORD encodeKey)
        : AuthStepAuthToken(encodeKey)
    {
    }

#ifdef _CHANNELING_AUTH_FLOW
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* stream_input,
                                 Token* id_info, Token* key_info)
    {
        //GetProtocolType에 의한 검증은 통과되어 있다 전제
        CopyMemory(m_AuthTokenStream, stream_input, sizeof(m_AuthTokenStream));
        MSG_CF_AUTH_TOKEN_PASS_SYN::Stream* stream =
            reinterpret_cast<MSG_CF_AUTH_TOKEN_PASS_SYN::Stream*>(m_AuthTokenStream);

        BOOLEAN success = CSAuthFlow::PostVerifier(stream, id_info, key_info);
        return success;
    }
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream_input,
                                 Token* id_info, Token* key_info)
    {
        CopyMemory(m_AuthTokenStream, stream_input, sizeof(m_AuthTokenStream));
        MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream =
            reinterpret_cast<MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream*>(m_AuthTokenStream);

        BOOLEAN success = CSAuthFlow::PostVerifier(stream, id_info, key_info);
        return success;
    }
#endif
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_REAUTH_PASS_SYN* stream_input,
                                 Token* id_info, Token* key_info)
    {
        CopyMemory(m_AuthTokenStream, stream_input, sizeof(m_AuthTokenStream));
        MSG_CF_AUTH_REAUTH_PASS_SYN* stream =
            reinterpret_cast<MSG_CF_AUTH_REAUTH_PASS_SYN*>(m_AuthTokenStream);

        BOOLEAN success = CSAuthFlow::PostVerifier(stream, id_info, key_info);
        return success;
    }
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* stream_input,
                                 Token* id_info, Token* key_info)
    {
        //ASSERT(!"Overrun!");
        CopyMemory(m_AuthTokenStream, stream_input, sizeof(m_AuthTokenStream));
        MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* stream =
            reinterpret_cast<MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN*>(m_AuthTokenStream);

        BOOLEAN success = CSAuthFlow::PostVerifier(stream, id_info, key_info);
        return success;
    }
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    virtual BOOLEAN PostVerifier(const MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN* recv_msg)
    {   
        return CSAuthFlow::PostVerifier(recv_msg);
    }
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg)
    {
        return CSAuthFlow::PostVerifier(auth_msg);
    }
#endif
};

void
LoginFrontServerSession::OnFS_AUTH_AUTHENTICATION_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    AuthStepAuthToken::eProtocol protocol = AuthStepAuthToken::GetProtocolType(pMsg, size);
    NORTHA_DEBUGLOG(__FUNCTION__" protocol check %d\n", protocol);
    if(AuthStepAuthToken::AcceptProtocol & (1 << protocol)) 
    {
        BOOLEAN success = false;
        if(protocol == AuthStepAuthToken::eProtocol_IdAndPassword) {
            success = Handling(static_cast<MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN*>(pMsg), size);
        }
    #ifdef _CHANNELING_AUTH_FLOW
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenKorea) {
            success = Handling(static_cast<MSG_FS_AUTH_TOKEN_PASS_SYN*>(pMsg), size);
        }
    #endif
    #ifdef __NA000000_GLOBAL_AUTH_PROCESS__
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenGlobal) {
            success = Handling(static_cast<MSG_FS_AUTH_TOKEN_GLOBAL_PASS_SYN*>(pMsg), size);
        }
    #endif
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        if(protocol == AuthStepAuthToken::eProtocol_ReAuthToken) {
            success = Handling(static_cast<MSG_FS_AUTH_REAUTH_PASS_SYN*>(pMsg), size);
        }
    #endif
    #ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenNorthA) {
            success = Handling(static_cast<MSG_FS_AUTH_TOKEN_NORTHA_PASS_SYN*>(pMsg), size);
            NORTHA_DEBUGLOG(__FUNCTION__" hangame authentication %d\n", success);
        }
    #endif
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenGamechu) {
            success = Handling(static_cast<MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN*>(pMsg), size);         
        }
    #endif
    #ifdef _RU_INGAMBA_AUTH_PROCESS
        if(protocol == AuthStepAuthToken::eProtocol_AuthIngamba) {
            success = Handling(static_cast<MSG_FS_AUTH_PASS_RU_INGAMBA_SYN*>(pMsg), size);         
        }
    #endif
    #ifdef _CN_ZY_AUTH_PROCESS
        if(protocol == AuthStepAuthToken::eProtocol_AuthZY) {
            success = Handling(static_cast<MSG_FS_AUTH_ZY_PASSWORD_CHECK_SYN*>(pMsg), size);
        }
    #endif //_CN_ZY_AUTH_PROCESS
        NORTHA_DEBUGLOG(__FUNCTION__" success %d\n", success);
    }
    // corrupt | unknown
}

#ifdef _RU_INGAMBA_AUTH_PROCESS
BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_PASS_RU_INGAMBA_SYN* recv_msg, WORD msg_size)
{
    AuthStepAuthTokenInAuthServer auth_step(0);
    BOOLEAN is_success = auth_step.PostVerifier(recv_msg);

    if (is_success == TRUE)
    {
        MSG_FS_AUTH_PASS_RU_INGAMBA_SYN clone_msg = *(recv_msg);
        const IngambaAuthInfo& user_info = clone_msg.ingamba_auth_info_; 

        IngambaAuthConnector* auth_connector = AuthFrame::Instance()->GetIngambaAuthConnector();
        int user_guid = 0;
        ResultEnum result_value = auth_connector->AuthenticateUser(user_info,
                                                                   clone_msg.client_ip_,
                                                                   &user_guid);

        if (result_value == ::Ok)
        {            
            IngambaUserInfo response_user_info;
            if (auth_connector->GetAccountInfo(user_guid, &response_user_info) == false)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    __FUNCTION__" : Fail get user info : %d", 
                    user_guid
                    );
                return false;
            }

            QueryManager* query_manager = QueryManager::Instance();
            query_manager->QueryLoginUserByRussia(GetSessionIndex(),
                                                  clone_msg.front_user_id_, 
                                                  user_guid,
                                                  response_user_info.user_account,
                                                  0,
                                                  clone_msg.client_ip_,
                                                  clone_msg.local_ip_);
            return true;
        }
        else
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Auth fail : %u", 
                result_value
                );
            MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK nak_msg;
            nak_msg.m_dwFrontUserID = clone_msg.front_user_id_;
            nak_msg.m_byFailCount = 0;
            nak_msg.m_byResult = AUTHRESULT_INGAMBA_INVALID;
            SendPacket(&nak_msg, sizeof(nak_msg));
            return false;
        }
    }
    return false;
}
#endif

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN* recv_msg, WORD msg_size)
{
    AuthStepAuthTokenInAuthServer auth_step(0);

    BOOLEAN success = auth_step.PostVerifier(recv_msg);
    if(success) {
        MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN clone_msg = *recv_msg;

        gcBaseUserInfo gamechu_user_info;
        GCCertificaltionHelper::eGCCModuleResult gcc_module_result;

        gcc_module_result = (GCCertificaltionHelper::eGCCModuleResult)GCCertificaltionHelper::Instance()->CheckCertification(
            clone_msg.gamechu_info_.first_parameter,
            clone_msg.gamechu_info_.second_parameter,
            &gamechu_user_info);

        if(gcc_module_result == GCCertificaltionHelper::GCC_SUCCESS) //게임츄 인증 성공
        {
            USES_CONVERSION;

            QueryManager* query_manager = QueryManager::Instance();
            
            query_manager->QueryLoginUserByAuthTokeyGameChu(
                GetSessionIndex(),
                atoi(W2A(gamechu_user_info.szUserSerialNumber)), 
                clone_msg.m_dwFrontUserID, 0,
                clone_msg.m_szClientIP,
                clone_msg.m_szLocalIP);
          

            return true;
        }
        else
        {

            /*USES_CONVERSION;

            QueryManager* query_manager = QueryManager::Instance();

            query_manager->QueryLoginUserByAuthTokeyGameChu(
                GetSessionIndex(),
                W2A(L"1"), 
                clone_msg.m_dwFrontUserID, 0,
                clone_msg.m_szClientIP,
                clone_msg.m_szLocalIP);

            return true;*/
            SUNLOG(eCRITICAL_LOG, "GameChu Certificaltion fail (%u)", gcc_module_result);
            MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK sendPacket;
            sendPacket.m_dwFrontUserID = clone_msg.m_dwFrontUserID;
            sendPacket.m_byFailCount = 0;
            sendPacket.m_byResult = AUTHRESULT_GAMECHU_INVALIDUSN;
            
            SendPacket(&sendPacket, sizeof(sendPacket));
        }
    }
    return false;
}
#endif

#ifdef _CHANNELING_AUTH_FLOW
BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_TOKEN_PASS_SYN* recv_msg, WORD msg_size)
{
    AuthStepAuthTokenInAuthServer auth_step(0);
    AuthStepAuthToken::Token id_token, key_token;
    BOOLEAN success = auth_step.PostVerifier(&recv_msg->info_stream, &id_token, &key_token);
    if(success) {
        MSG_FS_AUTH_TOKEN_PASS_SYN clone_msg = *recv_msg;
        id_token.buffer[id_token.length - 1] = '\0';
        _strupr((CHAR*)id_token.buffer);
        clone_msg.m_szClientIP[_countof(clone_msg.m_szClientIP) - 1] = '\0';
        clone_msg.m_szLocalIP[_countof(clone_msg.m_szLocalIP) - 1] = '\0';

        //  계정인증 DB요청
        QueryManager* query_manager = QueryManager::Instance();
        query_manager->QueryLoginUserByAuthTokenKorea(GetSessionIndex(),
                                                      (CHAR*)id_token.buffer,
                                                      key_token.buffer, (WORD)key_token.length,
                                                      clone_msg.m_dwFrontUserID, 0,
                                                      clone_msg.m_szClientIP,
                                                      clone_msg.m_szLocalIP);
        return true; // result of handling
    }

    return false;
}
#endif

#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_TOKEN_GLOBAL_PASS_SYN* recv_msg, WORD msg_size)
{
    AuthStepAuthTokenInAuthServer auth_step(0);
    AuthStepAuthToken::Token id_token, key_token;
    BOOLEAN success = auth_step.PostVerifier(&recv_msg->info_stream, &id_token, &key_token);
    GLOBAL_DEBUGLOG(__FUNCTION__" PostVerifier = %u\n", success);
    if(success) {
        MSG_FS_AUTH_TOKEN_GLOBAL_PASS_SYN clone_msg = *recv_msg;
        id_token.buffer[id_token.length - 1] = '\0';
        _strupr((CHAR*)id_token.buffer);
        clone_msg.m_szClientIP[_countof(clone_msg.m_szClientIP) - 1] = '\0';
        clone_msg.m_szLocalIP[_countof(clone_msg.m_szLocalIP) - 1] = '\0';

        AuthStepAuthToken::Token mac_token;
        success = auth_step.ExtractAddinInfo(&key_token, &mac_token);
        GLOBAL_DEBUGLOG("QueryArg|ID|%s|KEY|%s|MAX|%s|\n",
                        id_token.buffer, key_token.buffer, mac_token.buffer);
        if(success) {
            //  계정인증 DB요청
            QueryManager* query_manager = QueryManager::Instance();
            assert(mac_token.buffer[mac_token.length - 1] == '\0');
            query_manager->QueryLoginUserByAuthTokenGlobal(GetSessionIndex(),
                                                           (char*)id_token.buffer,
                                                           (char*)key_token.buffer,
                                                           (WORD)key_token.length,
                                                           clone_msg.m_dwFrontUserID, 0,
                                                           clone_msg.m_szClientIP,
                                                           clone_msg.m_szLocalIP,
                                                           (char*)mac_token.buffer);
            return true; // result of handling
        }
    }

    return false;
}
#endif

#ifdef _CN_ZY_AUTH_PROCESS
BOOLEAN LoginFrontServerSession::Handling(const MSG_FS_AUTH_ZY_PASSWORD_CHECK_SYN* recv_msg, WORD msg_size)
{
    MSG_FS_AUTH_ZY_PASSWORD_CHECK_SYN clone_msg = *recv_msg;

    CHECK_ID_LENGTH_BUFFER(clone_msg.m_szID);
    clone_msg.m_szID[_countof(clone_msg.m_szID) - 1] = '\0';
    _strupr(clone_msg.m_szID);
    clone_msg.m_szPasswd[_countof(clone_msg.m_szPasswd) - 1] = '\0';
    clone_msg.m_szClientIP[_countof(clone_msg.m_szClientIP) - 1] = '\0';
    clone_msg.m_szLocalIP[_countof(clone_msg.m_szLocalIP) - 1] = '\0';

#ifdef _CN_ZY_AUTH_PROCESS_DBAUTH
    //  계정인증 DB요청
    QueryManager* query_manager = QueryManager::Instance();
    query_manager->QueryCheckPasswordByChinaZY( \
        GetSessionIndex(), clone_msg.m_dwFrontUserID, clone_msg.m_szID, clone_msg.m_szPasswd, \
        0 /*reAuthKey*/, clone_msg.m_szClientIP, clone_msg.m_szLocalIP);
    
    /*query_manager->QueryLoginUserByChinaZY(GetSessionIndex(),
        clone_msg.m_szID, clone_msg.m_szPasswd, 
        clone_msg.m_dwFrontUserID, 0,
        clone_msg.m_szClientIP,
        clone_msg.m_szLocalIP);*/

#else
    //  계정인증 DB요청
    QueryManager* query_manager = QueryManager::Instance();
    query_manager->QueryLoginUser(GetSessionIndex(),
                                  clone_msg.m_szID, clone_msg.m_szPasswd, 
                                  clone_msg.m_dwFrontUserID, 0,
                                  clone_msg.m_szClientIP,
                                  clone_msg.m_szLocalIP);
#endif //_CN_ZY_AUTH_PROCESS_DBAUTH

    return true;
}
#endif //_CN_ZY_AUTH_PROCESS

BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN* recv_msg, WORD msg_size)
{
    MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN clone_msg = *recv_msg;
    // null-terminated 처리를 수행한다. ... 일반적으로 LoginFront에서 엄격하게 검증하지만,
    // 패킷이 꼬이는 경우를 감안하기로 한다. 또한 수신 패킷 버퍼를 건드리지 않도록 복사한다.
    // QueryLoginUser 참고상, 모두 -1을 허용하고 있다.
    // 그런데, 그 이전에 실패 처리하는 것이 더 올바른 방법이라 생각된다.
    CHECK_ID_LENGTH_BUFFER(clone_msg.m_szID);
    clone_msg.m_szID[_countof(clone_msg.m_szID) - 1] = '\0';
    _strupr(clone_msg.m_szID);
    clone_msg.m_szPasswd[_countof(clone_msg.m_szPasswd) - 1] = '\0';
    clone_msg.m_szClientIP[_countof(clone_msg.m_szClientIP) - 1] = '\0';
    clone_msg.m_szLocalIP[_countof(clone_msg.m_szLocalIP) - 1] = '\0';

    //  계정인증 DB요청
    QueryManager* query_manager = QueryManager::Instance();
    query_manager->QueryLoginUser(GetSessionIndex(),
                                  clone_msg.m_szID, clone_msg.m_szPasswd, 
                                  clone_msg.m_dwFrontUserID, 0,
                                  clone_msg.m_szClientIP,
                                  clone_msg.m_szLocalIP);

    return true;
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_REAUTH_PASS_SYN* recv_msg, WORD msg_size)
{
    // (NOTE) (091005) (WAVERIX) 판단 착오... 서버선택 기능을 Global버전에 바로 적용할 수 없다.
    // 기존은 키를 인증서버에서 검사했었는데, 글로벌 버전은 DB에서 검사한다. 이론~ 정보를 얻을 수
    // 없다. 다른 방법을 강구해야 할 듯. 추가 사항으로 GameID(no AccountID)로 검증해야 한다.
    struct LogHandling {
        enum {
            eLog_Success,
            eLog_InvalidPacketFormat,
            eLog_InvalidReAuthKey,
            eLog_CantFindReAuthInfo,
            eLog_NotMatchedIpOrAccount,
            eLog_NotMatchedReAuthKey,
            eLog_CantExtractFromReAuthKey,
            eLog_NotMatchedInputedKeyWithStoredKey,
            eLog_DelayedLogoutStatusButNotMatchedKey,
            eLog_Count,
        };
        BYTE m_eLog_Status;
        char m_account[ID_MAX_LEN + 1];
        char m_clientIp[IP_MAX_LEN];
        char m_report[32];

        LogHandling() : m_eLog_Status(eLog_Success)
        {
            ZeroMemory(m_account, sizeof(m_account));
            ZeroMemory(m_clientIp, sizeof(m_clientIp));
            ZeroMemory(m_report, sizeof(m_report));
        }
        ~LogHandling()
        {
            if(m_eLog_Status != eLog_Success)
                SUNLOG(eCRITICAL_LOG, "[ReAuth Process Fail Report][#%2u] %s, %s - %s",
                       m_eLog_Status, m_account, m_clientIp, m_report);
        };

        void SetReportInfoDefault(const char* account, const char* ip) {
            if(account)
                strncpy(m_account, account, _countof(m_account));
            if(ip)
                strncpy(m_clientIp, ip, _countof(m_clientIp));
        };
        void SetReportInfoNote(BYTE error_code, const char* report = 0) {
            m_eLog_Status = error_code;
            if(report) {
                strncpy(m_report, report, _countof(m_report));
                m_report[_countof(m_report) - 1] = '\0';
            }
        }
    } logHandling;

    AuthStepAuthTokenInAuthServer auth_step(0);
    AuthStepAuthToken::Token id_token, key_token;
    BOOLEAN success = auth_step.PostVerifier(recv_msg, &id_token, &key_token);
    if(success == 0)
    {
        logHandling.SetReportInfoDefault(0, recv_msg->m_szClientIP);
        logHandling.SetReportInfoNote(logHandling.eLog_InvalidPacketFormat);
        return false;
    }

    MSG_FS_AUTH_REAUTH_PASS_SYN clone_msg = *recv_msg;
    id_token.buffer[id_token.length - 1] = '\0';
    _strupr((CHAR*)id_token.buffer);
    clone_msg.m_szClientIP[_countof(clone_msg.m_szClientIP) - 1] = '\0';
    clone_msg.m_szLocalIP[_countof(clone_msg.m_szLocalIP) - 1] = '\0';

    logHandling.SetReportInfoDefault((char*)id_token.buffer, clone_msg.m_szClientIP);

    WORD tempReAuthKey = 0, reAuthKey = 0;
    if(AuthStepAuthToken::ExtractReAuthTokenRawIndex(&key_token, &tempReAuthKey) == 0)
    {
        logHandling.SetReportInfoNote(logHandling.eLog_InvalidReAuthKey);
        return false;
    }

    ReAuthManager* pReAuthManager = ReAuthManager::Instance();
    const ReAuthInfo* pReAuthInfo = pReAuthManager->FindReAuthInfo(tempReAuthKey);
    if(pReAuthInfo == 0)
    {
        logHandling.SetReportInfoNote(logHandling.eLog_CantFindReAuthInfo);
        return false;
    }

    DWORD inputed_ip_address = inet_addr(clone_msg.m_szClientIP);

    if(pReAuthInfo->CheckPositionInfo(inputed_ip_address, (char*)id_token.buffer) == 0)
    {
        logHandling.SetReportInfoNote(logHandling.eLog_NotMatchedIpOrAccount);
        return false;
    }

    DWORD userKey = 0;
    const ReAuthStepAuthTokenInfo* token_info = pReAuthInfo->GetReAuthStepAuthTokenInfo();
    if(memcmp(&token_info->format_stream, key_token.buffer, sizeof(token_info->format_stream)) != 0)
    {
        logHandling.SetReportInfoNote(logHandling.eLog_NotMatchedReAuthKey);
        return false;
    }

    SUNLOG(eCRITICAL_LOG, "[FromClient] %s -- [InAuthServer] %s",
           key_token.buffer, &token_info->format_stream);

    if(AuthStepAuthToken::ExtractReAuthToken(token_info, &key_token, &userKey, &reAuthKey) == 0)
    {
        logHandling.SetReportInfoNote(logHandling.eLog_CantExtractFromReAuthKey);
        return false;
    }
    if(tempReAuthKey != reAuthKey)
    {
        logHandling.SetReportInfoNote(logHandling.eLog_NotMatchedInputedKeyWithStoredKey);
        return false;
    }

    AccountInfo* pAccountInfo = AccountManager::Instance()->FindWithUserGuid(userKey);
    if(pAccountInfo != 0)
    {
        SUNLOG(eCRITICAL_LOG, "계정정보 남아있는 상황 post result %u",
               pAccountInfo->m_ReAuthKey != reAuthKey);
        if(pAccountInfo->m_ReAuthKey != reAuthKey)
        {
            logHandling.SetReportInfoNote(logHandling.eLog_DelayedLogoutStatusButNotMatchedKey);
            return false;
        }
        // wait process, problem < 5%, agentserver->authserver delayed notification
    }

    pReAuthManager->OnAcceptReAuthClient(pReAuthInfo);

	//  계정인증 DB요청
	QueryManager* query_manager = QueryManager::Instance();
	eSERVER_MODE serverMode = AuthFrame::Instance()->GetMode();
	DWORD query_selector = _AUTH_NATION_CODE_GENERIC;

#if defined(_JP_0_20100723_GAMECHU_AUTHFLOW_)
	query_selector = _AUTH_NATION_CODE_JAPAN;
#else
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	if(serverMode != eSERVER_DEV)
		query_selector = _AUTH_NATION_CODE;
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	query_selector = _AUTH_NATION_CODE;
#endif
#endif

#if defined(_RU_INGAMBA_AUTH_PROCESS)
    query_selector = _AUTH_NATION_CODE_RUSSIA;
#endif

	const DWORD session_index = GetSessionIndex();

	if(false)
	{
	}
#ifdef _CHANNELING_AUTH_FLOW
	else if(query_selector == _AUTH_NATION_CODE_KOREA)
	{
		query_manager->QueryLoginUserByAuthTokenKorea(session_index,
			(CHAR*)id_token.buffer,
			key_token.buffer, (WORD)key_token.length,
			clone_msg.m_dwFrontUserID, reAuthKey,
			clone_msg.m_szClientIP,
			clone_msg.m_szLocalIP);
	}
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
	else if(query_selector == _AUTH_NATION_CODE_USA)
	{
		query_manager->QueryLoginUserByAuthTokenNorthA(session_index,
			(CHAR*)id_token.buffer,
			clone_msg.m_dwFrontUserID, reAuthKey,
			clone_msg.m_szClientIP,
			clone_msg.m_szLocalIP);
	}
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
	else if(query_selector == _AUTH_NATION_CODE_GLOBAL)
	{
		query_manager->QueryLoginUserByAuthTokenGlobal(session_index,
			(char*)id_token.buffer,
			(char*)key_token.buffer, (WORD)key_token.length,
			clone_msg.m_dwFrontUserID, reAuthKey,
			clone_msg.m_szClientIP,
			clone_msg.m_szLocalIP,
			"");
	}
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	else if(query_selector == _AUTH_NATION_CODE_JAPAN)
	{		
		if(serverMode == eSERVER_DEV && pReAuthInfo->gamechu_usn_ == 0)
		{		
			char dummy[1] = { 0, };
			query_manager->QueryLoginUser(session_index,
				(LPTSTR)id_token.buffer, dummy, 
				clone_msg.m_dwFrontUserID, reAuthKey,
				clone_msg.m_szClientIP,
				clone_msg.m_szLocalIP);
		}
		else
		{	
			query_manager->QueryLoginUserByAuthTokeyGameChu(session_index,
				pReAuthInfo->gamechu_usn_, 
				clone_msg.m_dwFrontUserID, reAuthKey,
				clone_msg.m_szClientIP,
				clone_msg.m_szLocalIP);
		}		
	}
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
    else if(query_selector == _AUTH_NATION_CODE_RUSSIA)
    {
        if(serverMode == eSERVER_DEV)
        {		
            char dummy[1] = { 0, };
            query_manager->QueryLoginUser(session_index,
                                          (LPTSTR)id_token.buffer, dummy, 
                                          clone_msg.m_dwFrontUserID, reAuthKey,
                                          clone_msg.m_szClientIP,
                                          clone_msg.m_szLocalIP);
        }
        else
        {
            query_manager->QueryLoginUserByRussia(session_index,
                                                  clone_msg.m_dwFrontUserID, 
                                                  userKey,    
                                                  (char*)id_token.buffer,
                                                  reAuthKey,
                                                  clone_msg.m_szClientIP,
                                                  clone_msg.m_szLocalIP);
        }
    }
#endif
	else
	{
		char dummy[1] = { 0, };
		query_manager->QueryLoginUser(session_index,
			                          (LPTSTR)id_token.buffer, dummy, 
			                          clone_msg.m_dwFrontUserID, reAuthKey,
			                          clone_msg.m_szClientIP,
			                          clone_msg.m_szLocalIP);
	}
	return true; // result of handling
}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__


#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
BOOLEAN
LoginFrontServerSession::Handling(const MSG_FS_AUTH_TOKEN_NORTHA_PASS_SYN* recv_msg, WORD msg_size)
{
    using namespace nsHanAuth;

    AuthStepAuthTokenInAuthServer auth_step(0);
    AuthStepAuthToken::Token id_token, key_token;
    BOOLEAN success = auth_step.PostVerifier(recv_msg, &id_token, &key_token);
    NORTHA_DEBUGLOG(__FUNCTION__" post verifier %d\n", success);
    if(success) {
        MSG_FS_AUTH_TOKEN_NORTHA_PASS_SYN clone_msg = *recv_msg;
        id_token.buffer[id_token.length - 1] = '\0';
        // (WAVERIX) (090813) (CHANGES) 대/소문자 구별 하는 테스트 계정이 존재하므로 가공하지 않는다.
        //_strlwr((char*)id_token.buffer);

        clone_msg.m_szClientIP[_countof(clone_msg.m_szClientIP) - 1] = '\0';
        clone_msg.m_szLocalIP[_countof(clone_msg.m_szLocalIP) - 1] = '\0';

        QuerySlot slot;
        slot.SetInfo(INVALID_INDEX_ID, GetSessionIndex(), clone_msg.m_dwFrontUserID,
                     clone_msg.m_szClientIP, clone_msg.m_szLocalIP);
        INDEX_ID_T slot_index = HanAuthQueryManager::AllocSlot(slot);
        success = HanAuthQueryManager::QueryAuthentication(slot_index,
            (char*)id_token.buffer, (char*)key_token.buffer);
        NORTHA_DEBUGLOG(__FUNCTION__" HanAuthQueryManager query auth %u\n", success);
        return success;

        ////  계정인증 DB요청
        //QueryManager* query_manager = QueryManager::Instance();
        //query_manager->QueryLoginUserByAuthTokenNorthA(GetSessionIndex(),
        //                                               (CHAR*)id_token.buffer,
        //                                               clone_msg.m_dwFrontUserID, 0,
        //                                               clone_msg.m_szClientIP,
        //                                               clone_msg.m_szLocalIP);
        return true; // result of handling
    }

    return false;

}
#endif

//#else //!_CHANNELING_AUTH_FLOW
//
//void
//LoginFrontServerSession::OnFS_AUTH_ID_PASSWORD_CHECK_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
//{
//    MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN* pRecvMsg = (MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN*)pMsg;
//    MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN clone_msg = *pRecvMsg;
//
//    CHECK_ID_LENGTH_BUFFER(clone_msg.m_szID);
//    clone_msg.m_szID[_countof(clone_msg.m_szID) - 1] = '\0';
//    ::_tcsupr(clone_msg.m_szID);
//
//    //  계정인증 DB요청
//    QueryManager::Instance()->QueryLoginUser(GetSessionIndex(),
//                                             clone_msg.m_szID, clone_msg.m_szPasswd, 
//                                             clone_msg.m_dwFrontUserID, 0,
//                                             clone_msg.m_szClientIP, clone_msg.m_szLocalIP);
//}
//
//#endif //!_CHANNELING_AUTH_FLOW


void
LoginFrontServerSession::OnFS_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_FS_AUTH_SERVER_SELECT_SYN* pRecvMsg = (MSG_FS_AUTH_SERVER_SELECT_SYN*)pMsg;
    MSG_SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN sendPacket;
    BYTE resultCode = 0;

    AccountManager* const pAccountManager = AccountManager::Instance();
    ServerDataManager* const pServerDataManager = ServerDataManager::Instance();

    //  인증이 완료되지 않은 계정인 경우 에러를 보낸다.
    AccountInfo* pAccount = pAccountManager->FindWithLoginFrontUserID(pRecvMsg->m_dwFrontUserID);

    if(pAccount == NULL ||
       pAccount->m_status != ACCOUNTSTATUS_AUTH ||
       pAccount->m_LoginFrontUserId != pRecvMsg->m_dwFrontUserID)
    {
        if(pAccount == NULL)
            SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] Not found AccountInfo. no more information\n");
        else if(pAccount->m_status != ACCOUNTSTATUS_AUTH)
            SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] Not authenticate.(=%s, authStatus=%d)\n",
                   pAccount->m_account, pAccount->m_status);
        else if(pAccount->m_LoginFrontUserId != pRecvMsg->m_dwFrontUserID)
            SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] Invalid front key(=%s, authStatus=%d)\n",
                   pAccount->m_account, pAccount->m_status);

        resultCode = AUTHRESULT_INVALIDACCOUNT;
        goto ERROR_RESULT;
    }

    //  나이제한 서버인지를 검사한다.
    ServerData* pServerData =
        pServerDataManager->FindServerData(pRecvMsg->m_GroupID, pRecvMsg->m_ChannelID);
    if(!pServerData)
    {
        SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] id=%s age=%d failed.\n",
               pAccount->m_account, pAccount->m_age);

        resultCode = AUTHRESULT_SYSTEMERROR;
        goto ERROR_RESULT;
    }

    GROUP_DATA* pGroupData =
        pServerDataManager->FindGroupData(pServerData->GetBaseServerData().m_GroupID);
    if(!pGroupData)
    {
        SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] pGroupData[%u] is NULL!",
               pServerData->GetBaseServerData().m_GroupID);
        resultCode = AUTHRESULT_SYSTEMERROR;
        goto ERROR_RESULT;
    }

    if(pAccount->m_age < pGroupData->m_LimitedAge)
    {
        resultCode = AUTHRESULT_RESTRICTADULT;
        goto ERROR_RESULT;
    }

    // 혼잡도가 풀인지 검사한다.
    if(!pServerData->CheckServerCrowd())
    {
        // GM은 통과할 수 있고, 일반 유저만 걸린다
        if(pAccount->m_userCode[0] <= 99 || FALSE == pRecvMsg->m_IsOfficeIP)
        {
            SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] Selected Server[%dW %dC] is busy!! UserCount[%u]", pRecvMsg->m_GroupID, pRecvMsg->m_ChannelID, pServerData->GetLoginUserCnt());

            resultCode = AUTHRESULT_RESTRICTCROWD;
            goto ERROR_RESULT;
        }
    }

    pAccount->m_LoginFrontId = GetSessionIndex();

    pAccountManager->GenerateAuthKey(pAccount);
    pAccountManager->GenerateSerialKey(pAccount);

#ifdef _NA_20100307_BETAKEY_SYSTEM
    sendPacket.betakey_ = pAccount->betakey_;
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    sendPacket.gamechu_usn_ = pAccount->gamechu_usn_;
#endif
    sendPacket.m_dwAuthUserID = pAccount->m_authKey;
    sendPacket.m_dwUserGuid = pAccount->m_userGuid;
    sendPacket.m_GameAgentID = pServerData->GetGameAgentID();

    CHECK_ID_LENGTH_BUFFER(pAccount->m_account);
    CHECK_ID_LENGTH_BUFFER(sendPacket.m_szID);
    ::_tcsncpy(sendPacket.m_szID, pAccount->m_account, _countof(sendPacket.m_szID));
    sendPacket.m_szID[_countof(sendPacket.m_szID) - 1] = '\0';

    ::_tcsncpy(sendPacket.m_szClientIP, pAccount->m_clientIp, IP_MAX_LEN);
#ifdef _NA_005011_20120621_MULTICLIENTS
    // 나중에 빌링에 로그인이 성공하면 피시방상태로 바꿔줌
    sendPacket.m_BillingType.status_ = pAccount->m_pcBangStatus.eStatus_None; 
#else
    sendPacket.m_BillingType.status_ = pAccount->m_pcBangId == 0
                                     ? pAccount->m_pcBangStatus.eStatus_None
                                     : pAccount->m_pcBangStatus.eStatus_PcRoom; //만료 여부는 나중에 보낸다.
#endif //_NA_005011_20120621_MULTICLIENTS
    sendPacket.m_szBillingInfo[0] = 0;
    sendPacket.m_dwReservedValue = pAccount->m_pcBangId;
    sendPacket.m_AuthUserType = pAccount->m_AuthUserType;

    BOOST_STATIC_ASSERT(sizeof(sendPacket.m_serialKey) == sizeof(pAccount->m_serialKey));
    BOOST_STATIC_ASSERT(sizeof(sendPacket.m_MD5Key) == sizeof(pAccount->m_MD5));
    CopyMemory(sendPacket.m_serialKey, pAccount->m_serialKey, sizeof(pAccount->m_serialKey));
    CopyMemory(sendPacket.m_MD5Key, pAccount->m_MD5, sizeof(sendPacket.m_MD5Key));
    CopyMemory(sendPacket.m_userCode, pAccount->m_userCode, sizeof(sendPacket.m_userCode));

#ifdef _TIRED_CHECK
    sendPacket.m_byAge = pAccount->m_age;
    sendPacket.m_byRealNameAuth = pAccount->m_ucRealNameAuth;
#endif

    // GM인데 사내 아닌 경우에 한하여 일반 유저 등급으로 바꾼다
    if(pAccount->m_userCode[0] > 99 && FALSE == pRecvMsg->m_IsOfficeIP)
    {
        sendPacket.m_userCode[0] = 0;
        SUNLOG(eCRITICAL_LOG, "[OnS2SAskSvrSelect] GMUser[%s] but is not OfficeIP[%s]",
               pAccount->m_account, pAccount->m_clientIp);

    #ifdef _GM_IPCHECK_DISCONN
        MSG_SF_AUTH_CLIENT_DISCONNECT_CMD disMsg;
        disMsg.m_dwFrontUserID = pAccount->m_LoginFrontUserId;
        SendPacket(&disMsg, sizeof(disMsg));
        SUNLOG(eCRITICAL_LOG,
               "[OnFS_AUTH_SERVER_SELECT_SYN] GMUser[%s] is Disconnected in LiveServer",
               pAccount->m_account);
    #endif
    }

    //  GameAgent로 패킷을 보낸다.
    AuthAgentServerSession* pAuthAgentServerSession = pServerData->GetAuthAgentSession();
    if(pAuthAgentServerSession)
    {
        //  관련된 AuthAgent가 있다면 그 곳으로 메시지를 전달한다.
        pAuthAgentServerSession->SendPacket(&sendPacket, sizeof(sendPacket));

        SUNLOG(eFULL_LOG, "[OnS2SAskSvrSelect] Selected Server[%dW %dC] Success!! ID[%s]",
               pRecvMsg->m_GroupID, pRecvMsg->m_ChannelID, pAccount->m_account);
    }
    else
    {
        SUNLOG(eCRITICAL_LOG,
               "[OnS2SAskSvrSelect] Selected Server[%dW %dC] is Disconnected!! ID[%s]",
               pRecvMsg->m_GroupID, pRecvMsg->m_ChannelID, pAccount->m_account);

        resultCode = AUTHRESULT_SYSTEMERROR;
        goto ERROR_RESULT;
    }

    //  UserSts를 변경한다.
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    QueryManager::Instance()->QueryChangeUserSts(pAccount->m_account, pAccount->m_userGuid,
                                                 pAccount->m_glogKey,
                                                 pServerData->GetGameAgentID(),
                                                 pAccount->m_clientIp, pAccount->m_localIp,
                                                 USERSTATUS_TRYLOGIN,
                                                 pAccount->m_loginTime, pAccount->m_authKey);
#endif
    // in Global - 'server select'시점에는 남기지 않는다. (게임 플레이 가능 시점에 남긴다.)

    return;

ERROR_RESULT:
    MSG_SF_AUTH_SERVER_SELECT_ACK sendResult;
    sendResult.m_dwFrontUserID	= pRecvMsg->m_dwFrontUserID;
    sendResult.m_dwAuthUserID	= 0;							//  AuthKey를 0으로 지정하여 '실패'를 알린다.
    sendResult.m_byResult		= AUTHRESULT_INVALIDACCOUNT;
    sendResult.m_serialKey[0]	= 0;
    sendResult.m_dwSvrPort		= 0;
    sendResult.m_szSvrIP[0]		= 0;
    sendResult.m_szGLogKey[0]	= 0;
    SendPacket(&sendResult, sizeof(sendResult));
}

void
LoginFrontServerSession::OnFS_AUTH_CLIENT_DISCONNECT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_FS_AUTH_CLIENT_DISCONNECT_CMD* pRecvMsg = (MSG_FS_AUTH_CLIENT_DISCONNECT_CMD*)pMsg;

    QueryManager* const pQueryManager = QueryManager::Instance();
    AccountManager* const pAccountManager = AccountManager::Instance();
    AccountInfo* pAccount = pAccountManager->FindWithLoginFrontUserID(pRecvMsg->m_dwFrontUserID);
    if(pAccount == NULL) 
    {
        // 인증(ID PW)를 시도하지 않은 상태에서 접속을 끊으면 리스트에 없다.
        return;
    }

    if(pAccount->m_status != ACCOUNTSTATUS_TRYPLAY && pAccount->m_status != ACCOUNTSTATUS_PLAY)
    {
        //  UserSts를 변경한다.
        if(pAccount->m_userGuid != 0)
        {
        #if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
            pQueryManager->QueryChangeUserSts(pAccount->m_account, pAccount->m_userGuid,
                                              pAccount->m_glogKey, pAccount->m_locationId,
                                              pAccount->m_clientIp, pAccount->m_localIp,
                                              USERSTATUS_LOGOUT, pAccount->m_loginTime);
        #endif

            // 이 시점은 로그인 시점이 아니므로 빌링서버에 통보해서는 안된다!
            // (NOTE) in global - 정책상 게임 플레이 중이 아니므로 로깅하지 않는다.
        }

    #ifdef _NET_CAFE	// 일본 넷카페 DB에 유저가 로그아웃함을 통보한다
        if(pAccount->m_pcBangId)
        {
            pQueryManager->QueryNetCafeLogInsert(pAccount->m_LoginFrontId,
                                                 pAccount->m_LoginFrontUserId,
                                                 pAccount->m_account, pAccount->m_clientIp,
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                                 NETCAFE_LOGOUT,
#else
                                                 false,
#endif
                                                 0);
        }
    #endif

        pAccountManager->ClearInfo(pRecvMsg->m_dwFrontUserID);
    }
}

#ifdef _SECURE_AUTH

// 보안 인증 요청
void
LoginFrontServerSession::OnFS_AUTH_MATRIX_CARD_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_FS_AUTH_MATRIX_CARD_ACK* pRecvMsg = (MSG_FS_AUTH_MATRIX_CARD_ACK*)pMsg;

    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK sendPacket;
    sendPacket.m_dwFrontUserID = pRecvMsg->m_dwFrontUserID;
    sendPacket.m_byFailCount = 0;
    sendPacket.m_byResult = AUTHRESULT_OK;

    AccountManager* const pAccountManager = AccountManager::Instance();
    AccountInfo* pAccount = pAccountManager->FindWithLoginFrontUserID(pRecvMsg->m_dwFrontUserID);
    if(!pAccount)
    {
        sendPacket.m_byResult = AUTHRESULT_INVALIDACCOUNT;
        SUNLOG(eCRITICAL_LOG, "[OnFS_AUTH_MATRIX_CARD_ACK] dwFrontUserID[%u] pAccount is NULL!", pRecvMsg->m_dwFrontUserID);
        goto ERROR_RESULT;
    }

#ifdef __CN_20100909_SECURITY_TOKEN__
    sendPacket.m_Is_Token_Use = pAccount->is_token_use_;
#endif

    // 보안 인증 대상자가 맞는지 검증한다
    //#ifdef _SHAPE_AUTH
    //    if(!pAccount->m_bSucceedShape || pAccount->m_status || !pAccount->m_bSucceedAuth)
    //#else
    //    if(!pAccount->m_bSucceedShape || pAccount->m_status)
    //#endif
    if(!pAccount->m_bSucceedShape || pAccount->m_status)
    {
        sendPacket.m_byResult = AUTHRESULT_INVALIDACCOUNT;
        SUNLOG(eCRITICAL_LOG, "[OnFS_AUTH_MATRIX_CARD_ACK] dwFrontUserID[%u]m_bSucceedShape[%u]m_status[%u] is not Secure User", 
            pRecvMsg->m_dwFrontUserID, pAccount->m_bSucceedShape, pAccount->m_status);
        goto ERROR_RESULT;
    }

    AuthFrame* pAuthFrame = AuthFrame::Instance();
#ifndef _AUTH_CHINA
    //  Block 유무 체크
    if(pAccount->m_btMatrixAuthFailCount >= pAuthFrame->GetBlockLimit())
    {
        sendPacket.byResult = AUTHRESULT_ACCOUNT_BLOCK;
        SUNLOG(eCRITICAL_LOG, "[OnFS_AUTH_MATRIX_CARD_ACK] Account[%s] MatrixFailCount[%u] is Blocked!", pAccount->m_account, pAccount->m_btMatrixAuthFailCount);
        goto ERROR_RESULT;
    }
#endif

    // 매트릭스 데이타 체크
    int matrix[MAX_MATRIX_KEY] = {0, };
    matrix_gen(pAccount->m_seedValue, MAX_MATRIX_KEY, matrix, MAX_MATRIX_BASE, MAX_MATRIX_SEED);

    char szMatrix[32] = {0, };
    sprintf(szMatrix, "%d%d%d", 
            matrix[pAccount->m_secureAuthInfo.wMatrix[0]], 
            matrix[pAccount->m_secureAuthInfo.wMatrix[1]], 
            matrix[pAccount->m_secureAuthInfo.wMatrix[2]]);

    if (*((int*)pRecvMsg->m_SecureInfo.byData) != atoi(szMatrix))
    {
    #ifndef	_AUTH_CHINA
        // 오류 카운트를 증가시킨다.
        QueryManager::Instance()->QueryUpdateAuthFailCount(pAccount->m_account,
            pAccount->m_clientIp, 0, pAccount->m_LoginFrontId, GetSessionIndex(), MATRIX_CARD_AUTH);
    #else
        // 검증 다시 요청
        pAuthFrame->SendMatrixCardAuth(this, pAccount, pRecvMsg->m_dwFrontUserID, 0, 0);
    #endif
        return;
    }

    // 정상 인증 처리
    sendPacket.m_byResult = AUTHRESULT_OK;
    ::_tcsncpy(sendPacket.m_szInfo, g_LoginErrorString[sendPacket.m_byResult],
               _countof(sendPacket.m_szInfo));
    SendPacket(&sendPacket, sizeof(sendPacket));

    pAccount->m_status = ACCOUNTSTATUS_AUTH;
    pAccount->m_tickConnectTime	= ::GetTickCount();
    ::GetLocalTime(&pAccount->m_loginTime);

#ifndef	_AUTH_CHINA
    // 오류 카운트 초기화
    QueryManager::Instance()->QueryUpdateAuthFailCount(pAccount->m_account,
        pAccount->m_clientIp, 1, pAccount->m_LoginFrontId, GetSessionIndex(), MATRIX_CARD_AUTH);
#endif

    SUNLOG(eFULL_LOG, "[OnFS_AUTH_MATRIX_CARD_ACK] Account[%s] Login Success!", pAccount->m_account);

    return;

ERROR_RESULT:
    ::_tcsncpy(sendPacket.m_szInfo, g_LoginErrorString[sendPacket.m_byResult],
               _countof(sendPacket.m_szInfo));
    SendPacket(&sendPacket, sizeof(sendPacket));
}
#endif

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
void
LoginFrontServerSession::OnAU_AUTH_OTP_PASS_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_OTP_PASS_SYN* pRecvMsg = static_cast<const MSG_AU_AUTH_OTP_PASS_SYN*>(pMsg);
    AUTH_DEBUGLOG("check point == %d", 0);

    AccountManager* const account_manager = AccountManager::Instance();

    AccountInfo* account = account_manager->FindWithLoginFrontUserID(pRecvMsg->front_user_id);
    if (account == NULL) {
        AUTH_DEBUGLOG("Can't find user %d\n", pRecvMsg->front_user_id);
        return;
    }
    OtpStep& otp_step = account->m_OtpStep;
    if (otp_step.value != otp_step.eOtp_WaitOtp) {
        AUTH_DEBUGLOG("violated packet flow transaction:%d\n", otp_step.value);
        return;
    }

    QueryManager* query_manager = QueryManager::Instance();

    nsPacket::OneTimePassword password;
    // (NOTE) assert (true)
    if (nsPacket::CSAuthFlow::PostVerifier(pRecvMsg, &password) == false) {
        return;
    }
    query_manager->QueryNetCafe2ndAuthResult(account->m_LoginFrontId,
                                             account->m_LoginFrontUserId,
                                             account->m_account,
                                             password);
}
#endif

#ifdef __CN_20100909_SECURITY_TOKEN__
void
LoginFrontServerSession::OnAU_AUTH_SECURITY_TOKEN_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AU_AUTH_SECURITY_TOKEN_SYN* pRecvMsg = static_cast<MSG_AU_AUTH_SECURITY_TOKEN_SYN*>(pMsg);
	AUTH_DEBUGLOG("check point == %d", 0);

	AccountManager* const account_manager = AccountManager::Instance();

	AccountInfo* account = account_manager->FindWithLoginFrontUserID(pRecvMsg->front_user_id);
	if (account == NULL) {
		AUTH_DEBUGLOG("Can't find user %d\n", pRecvMsg->front_user_id);
		return;
	}

	MSG_AU_AUTH_SECURITY_TOKEN_ACK	msg;

	msg.front_user_id = pRecvMsg->front_user_id;	

	if (nsPacket::CSAuthFlow::PostVerifier(pRecvMsg) == false)
	{
		return;
	}		

	int return_value = 0;
	int adjust_step = 0;
	int advise_update = 0;

    return_value = DCSHSACheckPasscode_for_City9 (	account->token_base64_key_, 
													pRecvMsg->security_token_info_.security_token_code_,
													NULL, &adjust_step, &advise_update);

    if(advise_update == 1)//base64key 값을 업데이트 해야 한다.
    {
        QueryManager* query_manager = QueryManager::Instance();
        query_manager->Query_TokenKeyUpdate(account->m_userGuid, account->token_base64_key_);
    }

	if(return_value == DCSHEMFIRST)
	{
		msg.result_ = MSG_AU_AUTH_SECURITY_TOKEN_ACK::eResult_Success;
	}
	else if(return_value == DCSHEMSUCCESS)
	{		
		msg.result_ = MSG_AU_AUTH_SECURITY_TOKEN_ACK::eResult_Success;
	}
	else
	{
        SUNLOG(eCRITICAL_LOG, "DCSHSACheckPasscode_for_City9 fail (%s, %s, %d)",
                            account->token_base64_key_,
                            pRecvMsg->security_token_info_.security_token_code_,
                            return_value);
		msg.result_ = MSG_AU_AUTH_SECURITY_TOKEN_ACK::eResult_Fail;
	}
	//토큰 인증

	SendPacket(&msg, sizeof(msg));	
}
#endif

//#ifdef _SHAPE_AUTH
//void
//LoginFrontServerSession::OnFS_AUTH_SHAPE_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
//{
//	MSG_FS_AUTH_SHAPE_ACK* pRecvMsg = (MSG_FS_AUTH_SHAPE_ACK*)pMsg;
//
//	MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK sendPacket;
//	sendPacket.m_dwFrontUserID = pRecvMsg->m_dwFrontUserID;
//	sendPacket.m_byFailCount = 0;
//	sendPacket.m_byResult = AUTHRESULT_OK;
//
//	AccountInfo* pAccount = AccountManager::Instance()->Find(pRecvMsg->m_dwFrontUserID);
//	if(!pAccount)
//	{
//		sendPacket.m_byResult = AUTHRESULT_INVALIDACCOUNT;
//		SUNLOG(eCRITICAL_LOG, "[OnFS_AUTH_SHAPE_ACK] dwFrontUserID[%u] pAccount is NULL!", pRecvMsg->m_dwFrontUserID);
//		goto ERROR_RESULT;
//	}
//
//	// 형상 인증 대상자가 맞는지 검증한다
//#ifdef _SECURE_AUTH
//	if(!pAccount->m_bSucceedAuth || pAccount->m_status || pAccount->m_bSucceedShape)
//#else
//	if(!pAccount->m_bSucceedAuth || pAccount->m_status)
//#endif
//	{
//		sendPacket.m_byResult = AUTHRESULT_INVALIDACCOUNT;
//		SUNLOG(eCRITICAL_LOG, "[OnFS_AUTH_SHAPE_ACK] dwFrontUserID[%u]m_bSucceedAuth[%u]m_status[%u] is not Secure User", 
//			pRecvMsg->m_dwFrontUserID, pAccount->m_bSucceedAuth, pAccount->m_status);
//		goto ERROR_RESULT;
//	}
//
//#ifndef	_AUTH_CHINA
//	if(pAccount->m_btShapeAuthFailCount >= AuthFrame::Instance()->GetBlockLimit())
//	{
//		sendPacket.m_byResult = AUTHRESULT_ACCOUNT_BLOCK;
//		SUNLOG(eCRITICAL_LOG, "[OnFS_AUTH_SHAPE_ACK] Account[%s] MatrixFailCount[%u] is Blocked!", pAccount->m_account, pAccount->m_btShapeAuthFailCount);
//		goto ERROR_RESULT;
//	}
//#endif
//
//	// 형상인증 데이타 체크
//	if (0 != _tcscmp(pAccount->m_SA_Value, pRecvMsg->m_SA_Value))
//	{
//    #ifndef	_AUTH_CHINA
//		// 오류 카운트를 증가시킨다.
//		QueryManager::Instance()->QueryUpdateAuthFailCount(pAccount->m_account, pAccount->m_clientIp, 0, pAccount->m_LoginFrontId, GetSessionIndex(), SHAPE_AUTH);
//    #else
//		AuthFrame::Instance()->SendShapeAuthImage(this, pAccount, pRecvMsg->m_dwFrontUserID, pAccount->m_btShapeAuthFailCount + 1, 0);
//    #endif
//		return;
//	}
//
//#ifdef _SECURE_AUTH
//	// 매트릭스 카드를 사용하는 유저
//	if('\0' != &pAccount->m_seedValue[0])	
//	{
//		pAccount->m_bSucceedShape = true;
//		AuthFrame::Instance()->SendMatrixCardAuth(this, pAccount, pRecvMsg->m_dwFrontUserID, 0, 1);
//		return;
//	}
//#endif
//
//	// 정상 인증 처리
//	sendPacket.m_byResult = AUTHRESULT_OK;
//	::_tcsncpy(sendPacket.m_szInfo, g_LoginErrorString[sendPacket.m_byResult],
//               _countof(sendPacket.m_szInfo));
//	SendPacket(&sendPacket, sizeof(sendPacket));
//
//	pAccount->m_status = ACCOUNTSTATUS_AUTH;
//	pAccount->m_tickConnectTime	= ::GetTickCount();
//	::GetLocalTime(&pAccount->m_loginTime);
//
//	SUNLOG(eFULL_LOG, "[OnFS_AUTH_SHAPE_ACK] Account[%s] Login Success!", pAccount->m_account);
//
//#ifndef	_AUTH_CHINA
//	// 오류 카운트 초기화
//	QueryManager::Instance()->QueryUpdateAuthFailCount(pAccount->m_account, pAccount->m_clientIp, 1, pAccount->m_LoginFrontId, GetSessionIndex(), SHAPE_AUTH);
//#endif
//
//	return;
//
//ERROR_RESULT:
//	::_tcsncpy(sendPacket.m_szInfo, g_LoginErrorString[sendPacket.m_byResult],
//               _countof(sendPacket.m_szInfo));
//	SendPacket(&sendPacket, sizeof(sendPacket));
//}
//#endif



