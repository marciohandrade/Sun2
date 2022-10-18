#include "stdafx.h"
#include ".\clientsession.h"
#include ".\LoginFrontFrame.h"
#include ".\ClientManager.h"
#include ".\AuthServerSession.h"
#include "PacketCrypt.h"
#include "ServerDataManager.h"
#include <SunAuth/AuthProtocol_CF.h>
#include <AuthProtocol_FS.h>
#include <AuthStepAuthToken.h>

//==================================================================================================

__IMPL_CUSTOMPOOL_PTR(ClientSession)

ClientSession::ClientSession(void)
    : m_CorrectVersion(false)
    , m_FrontKey(0)
    , m_AuthQueryTransaction(0)
{
    m_szAccountID[0] ='\0';
    m_szLocalIP[0] ='\0';
}

ClientSession::~ClientSession(void)
{
}

void
ClientSession::Init()
{
    m_DisconnectTimer.SetTimer(LoginFrontFrame::Instance()->GetLoginTick());
    // (WAVERIX) (090701) (CHANGES) ClientSession:: 초기화 안하는 필드 부분 수정
    m_CorrectVersion = false;
    m_FrontKey = 0;
    m_AuthQueryTransaction = 0;
    m_szAccountID[0] ='\0';
    m_szLocalIP[0] ='\0';
}

void
ClientSession::Release()
{
    m_DisconnectTimer.DisableCheckTime();
}

void
ClientSession::Update()
{
#ifndef _DEBUG
    if(m_DisconnectTimer.IsExpiredManual())
    {
        SUNLOG(eFULL_LOG, "[ClientSession::Update()] No more response Time[%u]! Session is killed!",
               LoginFrontFrame::Instance()->GetLoginTick());
        Disconnect(TRUE);
    }
#endif
}

VOID
ClientSession::OnDisconnect()
{
    MSG_FS_AUTH_CLIENT_DISCONNECT_CMD sendPacket;
    sendPacket.m_dwFrontUserID = m_FrontKey;
    LoginFrontFrame::Instance()->SendAuthServer(&sendPacket, sizeof(sendPacket));

    ClientManager::Instance()->RemoveSession(m_FrontKey);

    ServerSession::OnDisconnect();
}

DWORD
ClientSession::MakeFrontId(DWORD loginFrontID)
{
    static long offset = 0;
    const long mod = 0x0FFFFFFF;

    ::InterlockedIncrement(&offset);
    offset = offset & mod;

    return ((loginFrontID << 28) | offset);
}

VOID
ClientSession::OnAccept(DWORD dwNetworkIndex)
{
    ServerSession::OnAccept(dwNetworkIndex);

    LoginFrontFrame* pLoginFrontFrame = LoginFrontFrame::Instance();
    ClientManager* pClientManager = ClientManager::Instance();

    DWORD loginTick = pLoginFrontFrame->GetLoginTick();
    DWORD loginFrontId = pLoginFrontFrame->GetLoginFrontID();

    // (WAVERIX) (090701) (CHANGES) ClientSession:: 초기화 안하는 필드 부분 수정시 AddSession안에
    //서의 Init문제 해결 위해 초기화 순서 수정
    DWORD frontKey = MakeFrontId(loginFrontId);
    if(FALSE == pClientManager->AddSession(frontKey, this))
    {
        SUNLOG(eCRITICAL_LOG, "[ClientSession::OnAccept] Duplicated Session IP[%s]", GetIP());
        Disconnect(TRUE);
        return;
    }

    // Key를 발급하고 Active session을 등록한다.
    m_DisconnectTimer.SetTimer(loginTick);
    m_FrontKey = frontKey;

    // 허용 IP 검사
    const char* pIpString = GetIP();
    DWORD dwIP = (DWORD)inet_addr(pIpString);
    if(pClientManager->IsAllowIp(dwIP) == false)
    {
        SUNLOG(eCRITICAL_LOG, "[ClientSession::OnAccept] IP[%s][%u] Denied", pIpString, dwIP);
        Disconnect(TRUE);
        return;
    }

    // 인증 처리가 준비되었음을 통지한다.
    MSG_FC_AUTH_READY_CMD sendPacket;
    ZeroMemory(sendPacket.m_szServerInfo, sizeof(sendPacket.m_szServerInfo));
    sendPacket.m_dwEncKey = m_FrontKey;
    SendPacket(&sendPacket, sizeof(sendPacket));
}

//==================================================================================================
//

VOID
ClientSession::OnRecv(BYTE* pMsg, WORD wSize)
{
    m_DisconnectTimer.SetTimer(LoginFrontFrame::Instance()->GetLoginTick());

    MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL*)pMsg;
    SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pRecvMsg, wSize);

    switch(pRecvMsg->m_byProtocol)
    {
    case CF_AUTH_REQ_PATCH_VER_SYN:     OnCF_AUTH_REQ_PATCH_VER_SYN(pRecvMsg, wSize);   break;
    case CF_AUTH_VERIFY_SYN:            OnCF_AUTH_VERIFY_SYN(pRecvMsg, wSize);          break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    case CF_AUTH_REAUTH_PASS_SYN:
#endif
//#ifdef _CHANNELING_AUTH_FLOW
    case CF_AUTH_ID_PASSWORD_CHECK_SYN:
    //case CF_AUTH_TOKEN_PASS_SYN:
    //case CF_AUTH_TOKEN_GLOBAL_PASS_SYN:
        OnCF_AUTH_AUTHENTICATION_SYN(pRecvMsg, wSize);
        break;
//#else
//    case CF_AUTH_ID_PASSWORD_CHECK_SYN:
//        OnCF_AUTH_ID_PASSWORD_CHECK_SYN(static_cast<MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN*>(pRecvMsg),
//                                        wSize);
//        break;
//#endif
    case CF_AUTH_SERVER_LIST_SYN:       OnCF_AUTH_SERVER_LIST_SYN(pRecvMsg, wSize);     break;
    case CF_AUTH_SERVER_SELECT_SYN:     OnCF_AUTH_SERVER_SELECT_SYN(pRecvMsg, wSize);   break;
#ifdef _SECURE_AUTH
    case CF_AUTH_MATRIX_CARD_ACK:       OnCF_AUTH_MATRIX_CARD_ACK(pRecvMsg, wSize);     break;
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    case CF_AUTH_OTP_PASS_SYN:			OnCF_AUTH_OTP_PASS_SYN(pRecvMsg, wSize); break;
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	case CF_AUTH_SECURITY_TOKEN_SYN:	OnCF_AUTH_SECURITY_TOKEN_SYN(pRecvMsg, wSize); break;
#endif
//#ifdef _SHAPE_AUTH
//    case CF_AUTH_SHAPE_ACK:             OnCF_AUTH_SHAPE_ACK(pRecvMsg, wSize);           break;
//#endif
    default:
        {
            SUNLOG(eCRITICAL_LOG,
                   "[ClientSession] Invalid Packet! Category[%u] Protocol[%u] Size[%u]",
                   pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
        }
        break;
    }
}

void
ClientSession::OnCF_AUTH_REQ_PATCH_VER_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_CF_AUTH_REQ_PATCH_VER_SYN* pRecvMsg = (MSG_CF_AUTH_REQ_PATCH_VER_SYN*)pMsg;
    LoginFrontFrame* pLoginFrontFrame = LoginFrontFrame::Instance();

    MSG_FC_AUTH_REQ_PATCH_VER_ACK sendPacket;
    sendPacket.m_verLauncher = pLoginFrontFrame->GetVerLauncher();
    sendPacket.m_verMajor = pLoginFrontFrame->GetVerMajor();
    sendPacket.m_verMinor = pLoginFrontFrame->GetVerMinor();

    SendPacket(&sendPacket, sizeof(sendPacket));
}

void
ClientSession::OnCF_AUTH_VERIFY_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_CF_AUTH_VERIFY_SYN *pRecvMsg = (MSG_CF_AUTH_VERIFY_SYN*)pMsg;
    MSG_FC_AUTH_VERIFY_ACK sendPacket;

    ::_tcsncpy(m_szLocalIP, pRecvMsg->m_szLocalIP, IP_MAX_LEN);

    // Version Check
    // Low version은 지정된 버젼 이상의 경우도 허용한다.
    const GAME_VERSION& protocolVersion = LoginFrontFrame::Instance()->GetVerProtocol();
    if(protocolVersion.m_ClassA == pRecvMsg->m_byHighVersion &&
       protocolVersion.m_ClassB == pRecvMsg->m_byMiddleVersion &&
       protocolVersion.m_ClassC <= pRecvMsg->m_byLowVersion)
    {
        sendPacket.m_byResult = PACKET_RESULT_SUCCESS;
        m_CorrectVersion = true;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Protocol Version is Incorrect! DB[%d.%d.%d] : C[%d.%d.%d]",
               protocolVersion.m_ClassA, protocolVersion.m_ClassB, protocolVersion.m_ClassC, 
               pRecvMsg->m_byHighVersion, pRecvMsg->m_byMiddleVersion, pRecvMsg->m_byLowVersion);
        sendPacket.m_byResult = PACKET_RESULT_FAIL;
        m_CorrectVersion = false;
    }

    //_DEV_007390_20140703_AUTH_EXCEPTION_VERSIONCHECK
    // 사내서버, 개발서버에서는 프로토콜체크 생략한다
    if (LoginFrontFrame::Instance()->GetMode() == eSERVER_DEV ||
        LoginFrontFrame::Instance()->GetMode() == eSERVER_INTRAOFFICE)
    {
        sendPacket.m_byResult = PACKET_RESULT_SUCCESS;
        m_CorrectVersion = true;
    }

    // Send result
    SendPacket(&sendPacket, sizeof(sendPacket));
}

//==================================================================================================
//
class AuthStepAuthTokenInLoginFront : public AuthStepAuthToken
{
public:
    AuthStepAuthTokenInLoginFront(DWORD encodeKey)
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

        Crypt::PacketDecode((BYTE*)&stream_input, sizeof(*stream_input),
                            (BYTE*)stream,
                            m_EncodeKey);

        BOOLEAN success = CSAuthFlow::PostVerifier(stream, id_info, key_info);
        return success;
    }
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream_input,
                                 Token* id_info, Token* key_info)
    {
        //GetProtocolType에 의한 검증은 통과되어 있다 전제
        CopyMemory(m_AuthTokenStream, stream_input, sizeof(m_AuthTokenStream));
        MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream* stream =
            reinterpret_cast<MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN::Stream*>(m_AuthTokenStream);

        Crypt::PacketDecode((BYTE*)&stream_input, sizeof(*stream_input),
                            (BYTE*)stream,
                            m_EncodeKey);

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

        //Crypt::PacketDecode((BYTE*)&stream_input->info_stream, sizeof(stream->info_stream),
        //                    (BYTE*)&stream->info_stream,
        //                    m_EncodeKey);
        BOOLEAN success = CSAuthFlow::PostVerifier(stream, id_info, key_info);
        NORTHA_DEBUGLOG(__FUNCTION__ " post verifier %d\n", success);
        NORTHA_DEBUGLOG(__FUNCTION__ " id %s\n", (char*)&stream_input->info_stream);
        return success;
    }
#endif

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* msg)
    {   
        return CSAuthFlow::PostVerifier(msg);        
    }
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
    virtual BOOLEAN PostVerifier(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* auth_msg)
    {
        return CSAuthFlow::PostVerifier(auth_msg);
    }
#endif
};

//--------------------------------------------------------------------------------------------------

// @brief: SubRoutine in 'OnCF_AUTH_ID_PASSWORD_CHECK_SYN'
void
ClientSession::OnCF_AUTH_AUTHENTICATION_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{   

#ifdef __NA000000_090701_AUTH_QUERY_TRANSACTION__
    if(m_AuthQueryTransaction != 0)
        return;
#endif
    NORTHA_DEBUGLOG(__FUNCTION__"\n", 0);
#if WAVERIX_AUTH_TEST_CODE
    MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN msgSYN;
    pMsg = &msgSYN;
    size = sizeof(msgSYN);

    char member_id[50] = { 0, };
    char auth_string[4096] = { 0, };

    GetPrivateProfileString("USER_INFO", "USER_ID", "waverix123",
                            member_id, _countof(member_id) - 1,
                            "./HanAuthUserTestInfo.ini");
    GetPrivateProfileString("USER_INFO", "AUTH_STRING", "waverix auth string",
                            auth_string, _countof(auth_string) - 1,
                            "./HanAuthUserTestInfo.ini");

    AuthStepAuthToken::MakeStream(member_id, auth_string, &msgSYN);
    Crypt::PacketEncode((BYTE*)&msgSYN.info_stream, sizeof(msgSYN.info_stream),
                        (BYTE*)&msgSYN.info_stream, m_FrontKey);
#endif
    m_AuthQueryTransaction = true;
    BOOLEAN success = false;
    AuthStepAuthToken::eProtocol protocol = AuthStepAuthToken::GetProtocolType(pMsg, size);
    if(AuthStepAuthToken::AcceptProtocol & (1 << protocol)) {
        if(protocol == AuthStepAuthToken::eProtocol_IdAndPassword) {
            success = Handling(static_cast<MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN*>(pMsg), size);
        }
    #ifdef _CHANNELING_AUTH_FLOW
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenKorea) {
            success = Handling(static_cast<MSG_CF_AUTH_TOKEN_PASS_SYN*>(pMsg), size);
        }
    #endif
    #ifdef __NA000000_GLOBAL_AUTH_PROCESS__
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenGlobal) {
            success = Handling(static_cast<MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN*>(pMsg), size);
        }
    #endif
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        if(protocol == AuthStepAuthToken::eProtocol_ReAuthToken) {
            success = Handling(static_cast<MSG_CF_AUTH_REAUTH_PASS_SYN*>(pMsg), size);
        }
    #endif
    #ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenNorthA) {
            success = Handling(static_cast<MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN*>(pMsg), size);
            NORTHA_DEBUGLOG(__FUNCTION__" check protocol = %d\n", success);
        }
    #endif
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        if(protocol == AuthStepAuthToken::eProtocol_AuthTokenGamechu) {     
            success = Handling(static_cast<MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN*>(pMsg), size);
            NORTHA_DEBUGLOG(__FUNCTION__" check protocol = %d\n", success);
        }
    #endif
    #ifdef _RU_INGAMBA_AUTH_PROCESS
        if (protocol == AuthStepAuthToken::eProtocol_AuthIngamba){
            success = Handling(static_cast<MSG_CF_AUTH_PASS_RU_INGAMBA_SYN*>(pMsg), size);
        }
    #endif
    #ifdef _CN_ZY_AUTH_PROCESS
        if (protocol == AuthStepAuthToken::eProtocol_AuthZY){
            success = Handling(static_cast<MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN*>(pMsg), size);
        }
    #endif //_CN_ZY_AUTH_PROCESS
    }

    // corrupt | unknown
    //if(success == 0)
    //    m_AuthQueryTransaction = false;
}

//--------------------------------------------------------------------------------------------------
#ifdef _RU_INGAMBA_AUTH_PROCESS
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* msg, WORD msg_size)
{
    AuthStepAuthTokenInLoginFront auth_step(m_FrontKey);
    bool success = auth_step.PostVerifier(msg);

    if (success == true) 
    {
        MSG_FS_AUTH_PASS_RU_INGAMBA_SYN syn_msg;
        syn_msg.front_user_id_ = m_FrontKey;
        syn_msg.ingamba_auth_info_ = msg->ingamba_auth_info_;

        ::_tcsncpy(syn_msg.client_ip_, GetIP(), _countof(syn_msg.client_ip_));
        ::_tcsncpy(syn_msg.local_ip_, m_szLocalIP, _countof(syn_msg.local_ip_));

        LoginFrontFrame::Instance()->SendAuthServer(&syn_msg, sizeof(syn_msg));
        SUNLOG(eCRITICAL_LOG, "Send auth request %s, %s, %s, %s", syn_msg.ingamba_auth_info_.user_id_,
                                                                  syn_msg.ingamba_auth_info_.user_password_,
                                                                  syn_msg.client_ip_, 
                                                                  syn_msg.local_ip_);
        return true;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail");
        return false;
    }
}
#endif

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* msg, WORD msg_size)
{
    AuthStepAuthTokenInLoginFront auth_step(m_FrontKey);
    BOOLEAN success = auth_step.PostVerifier(msg);

    if(success) {        
        MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN syn_msg;
        syn_msg.m_dwFrontUserID = m_FrontKey;
        ::_tcsncpy(syn_msg.m_szClientIP, GetIP(), _countof(syn_msg.m_szClientIP));
        ::_tcsncpy(syn_msg.m_szLocalIP, m_szLocalIP, _countof(syn_msg.m_szLocalIP));
        CopyMemory(&syn_msg.gamechu_info_, &msg->gamechu_info_, sizeof(syn_msg.gamechu_info_));
        LoginFrontFrame::Instance()->SendAuthServer(&syn_msg, sizeof(syn_msg));
    }

    return false;
}
#endif

#ifdef _CHANNELING_AUTH_FLOW
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_TOKEN_PASS_SYN* msg, WORD msg_size)
{
    AuthStepAuthTokenInLoginFront auth_step(m_FrontKey);
    AuthStepAuthToken::Token id_token;
    BOOLEAN success = auth_step.PostVerifier(&msg->info_stream, &id_token, 0);
    if(success) {
        //<- AUTH_TOKEN_PASS Front->Auth
        ::_tcsncpy(m_szAccountID, (CHAR*)id_token.buffer, _countof(m_szAccountID));
        m_szAccountID[_countof(m_szAccountID)-1] = '\0';

        MSG_FS_AUTH_TOKEN_PASS_SYN syn_msg;
        syn_msg.m_dwFrontUserID = m_FrontKey;
        ::_tcsncpy(syn_msg.m_szClientIP, GetIP(), _countof(syn_msg.m_szClientIP));
        ::_tcsncpy(syn_msg.m_szLocalIP, m_szLocalIP, _countof(syn_msg.m_szLocalIP));
        CopyMemory(&syn_msg.info_stream, auth_step.GetTokenStream(), sizeof(syn_msg.info_stream));
        LoginFrontFrame::Instance()->SendAuthServer(&syn_msg, sizeof(syn_msg));
    }

    return false;
}
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN* msg, WORD msg_size)
{
    AuthStepAuthTokenInLoginFront auth_step(m_FrontKey);
    AuthStepAuthToken::Token id_token;
    BOOLEAN success = auth_step.PostVerifier(&msg->info_stream, &id_token, 0);
    GLOBAL_DEBUGLOG(__FUNCTION__" success = %u\n", success);
    if(success) {
        //<- AUTH_TOKEN_GLOBAL_PASS Front->Auth
        ::_tcsncpy(m_szAccountID, (CHAR*)id_token.buffer, _countof(m_szAccountID));
        m_szAccountID[_countof(m_szAccountID)-1] = '\0';
        GLOBAL_DEBUGLOG(__FUNCTION__" account {%s}\n", m_szAccountID);
        MSG_FS_AUTH_TOKEN_GLOBAL_PASS_SYN syn_msg;
        syn_msg.m_dwFrontUserID = m_FrontKey;
        ::_tcsncpy(syn_msg.m_szClientIP, GetIP(), _countof(syn_msg.m_szClientIP));
        ::_tcsncpy(syn_msg.m_szLocalIP, m_szLocalIP, _countof(syn_msg.m_szLocalIP));
        CopyMemory(&syn_msg.info_stream, auth_step.GetTokenStream(), sizeof(syn_msg.info_stream));
        LoginFrontFrame::Instance()->SendAuthServer(&syn_msg, sizeof(syn_msg));
    }

    return false;
}
#endif

//--------------------------------------------------------------------------------------------------

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_REAUTH_PASS_SYN* recv_msg, WORD msg_size)
{
    AuthStepAuthTokenInLoginFront auth_step(m_FrontKey);
    AuthStepAuthToken::Token id_token, key_info;
    BOOLEAN success = auth_step.PostVerifier(recv_msg, &id_token, &key_info);
    if(success) {
        MSG_FS_AUTH_REAUTH_PASS_SYN msgSYN;
        msgSYN.info_stream = recv_msg->info_stream;
        msgSYN.m_dwFrontUserID = m_FrontKey;
        ::_tcsncpy(msgSYN.m_szClientIP, GetIP(), _countof(msgSYN.m_szClientIP));
        ::_tcsncpy(msgSYN.m_szLocalIP, m_szLocalIP, _countof(msgSYN.m_szLocalIP));
        LoginFrontFrame::Instance()->SendAuthServer(&msgSYN, sizeof(msgSYN));
    }
    return TRUE;
}
#endif

//--------------------------------------------------------------------------------------------------

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* recv_msg, WORD msg_size)
{
    NORTHA_DEBUGLOG(__FUNCTION__"\n");
    AuthStepAuthTokenInLoginFront auth_step(m_FrontKey);
    AuthStepAuthToken::Token id_token, key_info;
    BOOLEAN success = auth_step.PostVerifier(recv_msg, &id_token, &key_info);
    if(success) {
        MSG_FS_AUTH_TOKEN_NORTHA_PASS_SYN msgSYN;
        msgSYN.info_stream = recv_msg->info_stream;
        msgSYN.m_dwFrontUserID = m_FrontKey;
    #if _AUTH_NORTHA_TEST_MODE == _AUTH_NORTHA_TEST_MODE__DEVTEST__
        GetPrivateProfileString("CONFIG_INFO", "ClientIP", "",
                                msgSYN.m_szClientIP, _countof(msgSYN.m_szClientIP),
                                "./HanAuthUserTestInfo.ini");
        GetPrivateProfileString("CONFIG_INFO", "LocalIP", "",
                                msgSYN.m_szLocalIP, _countof(msgSYN.m_szLocalIP),
                                "./HanAuthUserTestInfo.ini");
    #else
        ::_tcsncpy(msgSYN.m_szClientIP, GetIP(), _countof(msgSYN.m_szClientIP));
        ::_tcsncpy(msgSYN.m_szLocalIP, m_szLocalIP, _countof(msgSYN.m_szLocalIP));
    #endif
        NORTHA_DEBUGLOG(__FUNCTION__ " post verifier success mode %d\n", _AUTH_NORTHA_TEST_MODE);
        LoginFrontFrame::Instance()->SendAuthServer(&msgSYN, sizeof(msgSYN));
    }
    else {
        NORTHA_DEBUGLOG(__FUNCTION__" fail\n");
    }
    return TRUE;
}
#endif

#ifdef _CN_ZY_AUTH_PROCESS
BOOLEAN ClientSession::Handling(const MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN* pRecvMsg, WORD size)
{
    MSG_FS_AUTH_ZY_PASSWORD_CHECK_SYN syn_msg;
    if(m_CorrectVersion == false || pRecvMsg->m_szID[0] == 0)
        return false;

    // (waverix) (note) (090129) Protocol정의상 'char'로 설정되어 있으므로
    // _tcsncpy는 맞지 않는다. 하지만 바뀔 리는 없으므로 그냥 두기로 한다.
    CHECK_ID_LENGTH_BUFFER(m_szAccountID);
    CHECK_ID_LENGTH_BUFFER(pRecvMsg->m_szID);
    BOOST_STATIC_ASSERT(sizeof(m_szAccountID) == sizeof(syn_msg.m_szID) ||
        sizeof(m_szAccountID) == sizeof(syn_msg.m_szID) + 1);

    ::_tcsncpy(m_szAccountID, pRecvMsg->m_szID, _countof(m_szAccountID));
    m_szAccountID[_countof(m_szAccountID) - 1] = '\0';

    syn_msg.m_dwFrontUserID = m_FrontKey;
    syn_msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
    // 문자열은 Null을 포함하기 때문에 ID_MAX_LEN보다 1byte는 확보되어야 한다.
    // (waverix) (note) (090129) 그럼 23자?

    ::_tcsncpy(syn_msg.m_szID, m_szAccountID, _countof(syn_msg.m_szID));
    ::_tcsncpy(syn_msg.m_szClientIP, GetIP(), _countof(syn_msg.m_szClientIP));
    ::_tcsncpy(syn_msg.m_szLocalIP, m_szLocalIP, _countof(syn_msg.m_szLocalIP));
    ZeroMemory(syn_msg.m_szPasswd, sizeof(syn_msg.m_szPasswd));
    Crypt::PacketDecode((BYTE*)pRecvMsg->m_szPasswd, sizeof(pRecvMsg->m_szPasswd),
        (BYTE*)syn_msg.m_szPasswd, m_FrontKey);

    LoginFrontFrame::Instance()->SendAuthServer(&syn_msg, sizeof(syn_msg));
    //SUNLOG(eCRITICAL_LOG,
    //       "[ClientSession::OnC2SAskAuth] Send Auth Ask to AUTH_SERVER, %s, %s\n",
    //       sendPacket.szID, sendPacket.szClientIP);
    return true;
}
#endif //_CN_ZY_AUTH_PROCESS

//--------------------------------------------------------------------------------------------------

//#ifdef _CHANNELING_AUTH_FLOW
BOOLEAN
ClientSession::Handling(const MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN* pRecvMsg, WORD size)
//#else
//BOOLEAN
//ClientSession::OnCF_AUTH_ID_PASSWORD_CHECK_SYN(MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN* pRecvMsg, WORD size)
//#endif
{
    MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN syn_msg;
    if(m_CorrectVersion == false || pRecvMsg->m_szID[0] == 0)
        return false;

    // (waverix) (note) (090129) Protocol정의상 'char'로 설정되어 있으므로
    // _tcsncpy는 맞지 않는다. 하지만 바뀔 리는 없으므로 그냥 두기로 한다.
    CHECK_ID_LENGTH_BUFFER(m_szAccountID);
    CHECK_ID_LENGTH_BUFFER(pRecvMsg->m_szID);
    BOOST_STATIC_ASSERT(sizeof(m_szAccountID) == sizeof(syn_msg.m_szID) ||
                        sizeof(m_szAccountID) == sizeof(syn_msg.m_szID) + 1);

    ::_tcsncpy(m_szAccountID, pRecvMsg->m_szID, _countof(m_szAccountID));
    m_szAccountID[_countof(m_szAccountID) - 1] = '\0';

    syn_msg.m_dwFrontUserID = m_FrontKey;
    syn_msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
    // 문자열은 Null을 포함하기 때문에 ID_MAX_LEN보다 1byte는 확보되어야 한다.
    // (waverix) (note) (090129) 그럼 23자?

    ::_tcsncpy(syn_msg.m_szID, m_szAccountID, _countof(syn_msg.m_szID));
    ::_tcsncpy(syn_msg.m_szClientIP, GetIP(), _countof(syn_msg.m_szClientIP));
    ::_tcsncpy(syn_msg.m_szLocalIP, m_szLocalIP, _countof(syn_msg.m_szLocalIP));
    ZeroMemory(syn_msg.m_szPasswd, sizeof(syn_msg.m_szPasswd));
    Crypt::PacketDecode((BYTE*)pRecvMsg->m_szPasswd, sizeof(pRecvMsg->m_szPasswd),
                        (BYTE*)syn_msg.m_szPasswd, m_FrontKey);

    LoginFrontFrame::Instance()->SendAuthServer(&syn_msg, sizeof(syn_msg));
    //SUNLOG(eCRITICAL_LOG,
    //       "[ClientSession::OnC2SAskAuth] Send Auth Ask to AUTH_SERVER, %s, %s\n",
    //       sendPacket.szID, sendPacket.szClientIP);
    return true;
}

//--------------------------------------------------------------------------------------------------

void
ClientSession::OnCF_AUTH_SERVER_LIST_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_CF_AUTH_SERVER_LIST_SYN *pRecvMsg = (MSG_CF_AUTH_SERVER_LIST_SYN*)pMsg;

    if(m_CorrectVersion == false)
    {
        Disconnect(TRUE);
        return;
    }

    ServerDataManager* const pServerDataManager = ServerDataManager::Instance();
    const char* ip_address = GetIP();

    // Group 및 Server 리스트를 보내준다.
    MSG_FC_AUTH_SERVER_LIST_WORLD_ACK groupMsg;
    groupMsg.m_Count = pServerDataManager->GetLiveGroupListInfo(groupMsg.m_GroupDataList, ip_address);
    SendPacket(&groupMsg, groupMsg.GetSize());

    MSG_FC_AUTH_SERVER_LIST_CHANNEL_ACK serverMsg;
    serverMsg.m_Count = pServerDataManager->GetLiveServerListInfo(serverMsg.m_ServerDataList, ip_address);
    SendPacket(&serverMsg, serverMsg.GetSize());
}

void
ClientSession::OnCF_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_CF_AUTH_SERVER_SELECT_SYN* pRecvMsg = (MSG_CF_AUTH_SERVER_SELECT_SYN*)pMsg;
    MSG_FS_AUTH_SERVER_SELECT_SYN sendPacket;

    if(m_CorrectVersion == false)
    {
        Disconnect(TRUE);
        return;
    }

#ifdef _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_
    ServerDataManager* const pServerDataManager = ServerDataManager::Instance();
    const char* ip_address = GetIP();
    if(pServerDataManager->IsEnterableWorld(pRecvMsg->m_GroupID, ip_address) == false) {
        Disconnect(TRUE);
        return; // (NOTE) 그냥 반환시... 상태 검증할 것.
    }
#endif

    sendPacket.m_dwFrontUserID = m_FrontKey;
    sendPacket.m_GroupID = pRecvMsg->m_GroupID;
    sendPacket.m_ChannelID = pRecvMsg->m_ChannelID;

    //  사내 IP 검사
    sendPacket.m_IsOfficeIP = ClientManager::Instance()->IsOfficeIp(GetIPAddress());

    LoginFrontFrame::Instance()->SendAuthServer(&sendPacket, sizeof(sendPacket));
}

#ifdef _SECURE_AUTH
void
ClientSession::OnCF_AUTH_MATRIX_CARD_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_CF_AUTH_MATRIX_CARD_ACK* pRecvMsg = (MSG_CF_AUTH_MATRIX_CARD_ACK*)pMsg;
    MSG_FS_AUTH_MATRIX_CARD_ACK	sendPacket;

    if(m_CorrectVersion == false)
    {
        Disconnect(TRUE);
        return;
    }

    sendPacket.m_dwFrontUserID = m_FrontKey;
    memcpy(&(sendPacket.m_SecureInfo), &(pRecvMsg->m_SecureInfo), sizeof(SECURE_INFO)); 

    LoginFrontFrame::Instance()->SendAuthServer(&sendPacket, sizeof(sendPacket));
    SUNLOG(eFULL_LOG, "AnsSCAuth:%d", *((int*)sendPacket.m_SecureInfo.byData));
}
#endif

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
void
ClientSession::OnCF_AUTH_OTP_PASS_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_CF_AUTH_OTP_PASS_SYN* pRecvMsg = static_cast<const MSG_CF_AUTH_OTP_PASS_SYN*>(pMsg);
    if (nsPacket::CSAuthFlow::PreVerifier(pRecvMsg, size) == false) {
        AUTH_DEBUGLOG("PreVerifier == %d", 0);
        return;
    }

    MSG_AU_AUTH_OTP_PASS_SYN msg_syn;
    // compile time checker
    static_cast<MSG_AU_AUTH_OTP_PASS_SYN*>(static_cast<MSG_CF_AUTH_OTP_PASS_SYN*>(0));
    BOOST_STATIC_ASSERT(CF_AUTH_OTP_PASS_SYN == AU_AUTH_OTP_PASS_SYN &&
                        sizeof(*pRecvMsg) <= sizeof(msg_syn));

    ZeroMemory(&msg_syn, sizeof(msg_syn));
    static_cast<MSG_CF_AUTH_OTP_PASS_SYN&>(msg_syn) = *pRecvMsg;
    msg_syn.front_user_id = m_FrontKey;

    LoginFrontFrame::Instance()->SendAuthServer(&msg_syn, sizeof(msg_syn));
}
#endif

#ifdef __CN_20100909_SECURITY_TOKEN__
void
ClientSession::OnCF_AUTH_SECURITY_TOKEN_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	const MSG_CF_AUTH_SECURITY_TOKEN_SYN* pRecvMsg = static_cast<const MSG_CF_AUTH_SECURITY_TOKEN_SYN*>(pMsg);
	if (nsPacket::CSAuthFlow::PreVerifier(pRecvMsg, size) == false) {
		AUTH_DEBUGLOG("PreVerifier == %d", 0);
		return;
	}

	MSG_AU_AUTH_SECURITY_TOKEN_SYN msg_syn;	
	static_cast<MSG_AU_AUTH_SECURITY_TOKEN_SYN*>(static_cast<MSG_AU_AUTH_SECURITY_TOKEN_SYN*>(0));
	BOOST_STATIC_ASSERT(CF_AUTH_SECURITY_TOKEN_SYN == AU_AUTH_SECURITY_TOKEN_SYN &&
		sizeof(*pRecvMsg) <= sizeof(msg_syn));	

	ZeroMemory(&msg_syn, sizeof(msg_syn));
	dynamic_cast<MSG_CF_AUTH_SECURITY_TOKEN_SYN&>(msg_syn) = *pRecvMsg;
	msg_syn.front_user_id = m_FrontKey;

	LoginFrontFrame::Instance()->SendAuthServer(&msg_syn, sizeof(msg_syn));
}
#endif

//#ifdef _SHAPE_AUTH
//void
//ClientSession::OnCF_AUTH_SHAPE_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
//{
//    MSG_CF_AUTH_SHAPE_ACK* pRecvMsg = (MSG_CF_AUTH_SHAPE_ACK*)pMsg;
//    MSG_FS_AUTH_SHAPE_ACK sendPacket;
//
//    if(m_CorrectVersion == false)
//    {
//        Disconnect(TRUE);
//        return;
//    }
//
//    sendPacket.m_dwFrontUserID = m_FrontKey;
//    ::_tcsncpy(sendPacket.m_SA_Value, pRecvMsg->m_SA_Value, SA_VALUE_SIZE);
//
//    LoginFrontFrame::Instance()->SendAuthServer(&sendPacket, sizeof(sendPacket));
//
//    SUNLOG(eFULL_LOG, "[OnCF_AUTH_SHAPE_ACK] m_SA_Value[%s]", sendPacket.m_SA_Value);
//}
//#endif













