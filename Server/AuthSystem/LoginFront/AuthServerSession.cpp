#include "stdafx.h"
#include ".\authserversession.h"
#include ".\ClientManager.h"
#include ".\ClientSession.h"
#include ".\LoginFrontFrame.h"
#include <ArchiveOfStream.h>
#include "ServerGlobal.h"
#include "ServerDataManager.h"
#include <SunAuth/AuthProtocol_CF.h>
#include <AuthProtocol_FS.h>

__IMPL_CUSTOMPOOL_PTR(AuthServerSession)

AuthServerSession::AuthServerSession(void)
{

}

AuthServerSession::~AuthServerSession(void)
{

}

void
AuthServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
    ServerSession::OnConnect(bSuccess, dwNetworkIndex);
    if(bSuccess)
    {
        LoginFrontFrame::Instance()->SetServiceControl(LoginFrontFrame::eService_Ok);
    }
}

VOID
AuthServerSession::OnDisconnect()
{
    LoginFrontFrame* pLoginFront = LoginFrontFrame::Instance();
    ReadyState& readyState = pLoginFront->GetReadyState();
    readyState.SetReadyState(FRONT_READY_GAMEVERSION, FALSE);
    readyState.SetReadyState(FRONT_READY_CONTROL_IPLIST, FALSE);

    pLoginFront->SetServiceControl(LoginFrontFrame::eService_Fail);

    ServerSession::OnDisconnect();
}

VOID
AuthServerSession::OnRecv(BYTE* pMsg, WORD wSize)
{
    MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL*)pMsg;
    SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pRecvMsg, wSize);

    switch(pRecvMsg->m_byProtocol)
    {
    case SF_AUTH_GAME_VERSION_NTF:      OnSF_AUTH_GAME_VERSION_NTF(pRecvMsg, wSize);    break;
    case SF_AUTH_CONTROL_IPLIST_NTF:    OnSF_AUTH_CONTROL_IPLIST_NTF(pRecvMsg, wSize);  break;
    case SF_AUTH_ID_PASSWORD_CHECK_ACK: OnSF_AUTH_ID_PASSWORD_CHECK_ACK(pRecvMsg, wSize);break;
    //__NA001334_090420_SERVER_SELECT_FACILITY__ : SF_AUTH_REAUTH_PASS_ACK
    case SF_AUTH_SERVER_SELECT_ACK:     OnSF_AUTH_SERVER_SELECT_ACK(pRecvMsg, wSize);   break;
    case SF_AUTH_CLIENT_DISCONNECT_CMD: OnSF_AUTH_CLIENT_DISCONNECT_CMD(pRecvMsg, wSize);break;
#ifdef _SECURE_AUTH
    case SF_AUTH_MATRIX_CARD_SYN:       OnSF_AUTH_MATRIX_CARD_SYN(pRecvMsg, wSize);     break;
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    case AU_AUTH_OTP_PASS_CMD: OnAU_AUTH_OTP_PASS_CMD(pRecvMsg, wSize); break;
    case AU_AUTH_OTP_PASS_ACK: OnAU_AUTH_OTP_PASS_ACK(pRecvMsg, wSize); break;
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__	
	case AU_AUTH_SECURITY_TOKEN_ACK: OnAU_AUTH_SECURITY_TOKEN_ACK(pRecvMsg, wSize); break;
#endif
//#ifdef _SHAPE_AUTH
//    case SF_AUTH_SHAPE_SYN:             OnSF_AUTH_SHAPE_SYN(pRecvMsg, wSize);           break;
//#endif
    case SF_AUTH_SERVER_LIST_NTF:       OnSF_AUTH_SERVER_LIST_NTF(pRecvMsg, wSize);     break;
    default:
        {
            SUNLOG(eCRITICAL_LOG,
                   "[AuthServerSession] Invalid Packet Category[%u] Protocol[%u] Size[%u]",
                   pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
        }
        break;
    }
}


void
AuthServerSession::OnSF_AUTH_GAME_VERSION_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_GAME_VERSION_NTF* pRecvMsg = (MSG_SF_AUTH_GAME_VERSION_NTF*)pMsg;

    LoginFrontFrame* pLoginFrontFrame = LoginFrontFrame::Instance();
    pLoginFrontFrame->SetVerProtocol(pRecvMsg->m_verProtocol);
    pLoginFrontFrame->SetVerLauncher(pRecvMsg->m_verLauncher);
    pLoginFrontFrame->SetVerMajor(pRecvMsg->m_verMajor);
    pLoginFrontFrame->SetVerMinor(pRecvMsg->m_verMinor);

    pLoginFrontFrame->DisplayVersionInfo();

    pLoginFrontFrame->GetReadyState().SetReadyState(FRONT_READY_GAMEVERSION, TRUE);
}

void
AuthServerSession::OnSF_AUTH_CONTROL_IPLIST_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_CONTROL_IPLIST_NTF*	pRecvMsg = (MSG_SF_AUTH_CONTROL_IPLIST_NTF*)pMsg;

    ClientManager* pClientManager = ClientManager::Instance();

    switch(pRecvMsg->m_Flag)
    {
    case 0:	// m_Flag 0은 IP 초기화 명령이다.
        {
            pClientManager->ClearControlIp();
        }
        break;
    case 1:	// m_Flag 1은 IP 추가이다.
        {
            for(int i = 0 ; i < pRecvMsg->m_Count ; i++)
            {
                CONTROL_IP_INFO& ipInfo = pRecvMsg->m_IPDataList[i];
                pClientManager->AddControlIp(ipInfo.m_IPType, ipInfo.m_StartIP, ipInfo.m_Count);

                SUNLOG(eCRITICAL_LOG, "[OnS2SControlIpList] type[%d] %s + count %d", 
                    ipInfo.m_IPType, IPOperation::ConvertDWORD2StrIP(ipInfo.m_StartIP), ipInfo.m_Count);
            }
        }
        break;
    case 2:	// m_Flag 2는 작업 완료 명령이다.
        {
            ReadyState& readyState = LoginFrontFrame::Instance()->GetReadyState();
            readyState.SetReadyState(FRONT_READY_CONTROL_IPLIST, TRUE);
            SUNLOG(eCRITICAL_LOG, "[OnS2SControlIpList] Control IP List[%d]",
                   pClientManager->GetControlIpSize());
        }
        break;
    }
}

void
AuthServerSession::OnSF_AUTH_ID_PASSWORD_CHECK_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK* pRecvMsg = (MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK*)pMsg;
    MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK sendPacket;
    sendPacket.m_byResult = pRecvMsg->m_byResult;
    sendPacket.m_byFailCount = pRecvMsg->m_byFailCount;
    BOOST_STATIC_ASSERT(_countof(sendPacket.m_szInfo) == _countof(pRecvMsg->m_szInfo));
    ::_tcsncpy(sendPacket.m_szInfo, pRecvMsg->m_szInfo, _countof(sendPacket.m_szInfo));

    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->m_dwFrontUserID);
    if(client == NULL) 
        return;

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    ::_tcsncpy(sendPacket.m_szID, pRecvMsg->m_szID, _countof(sendPacket.m_szID));
    client->SetAccountID(pRecvMsg->m_szID);
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	sendPacket.m_Is_Token_Use = pRecvMsg->m_Is_Token_Use;
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    sendPacket.m_betakey_ = pRecvMsg->m_betakey_;
#endif
    client->SendPacket(&sendPacket, sizeof(sendPacket));
    //SUNLOG(eCRITICAL_LOG,
    //       "[AuthServerSession::OnS2SAnsAuth] Send Auth Result to Client(Result:%d)\n",
    //       sendPacket.byResult);
    client->UnlockAuthQueryTransaction(); //__NA000000_090701_AUTH_QUERY_TRANSACTION__
}

void
AuthServerSession::OnSF_AUTH_CLIENT_DISCONNECT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_CLIENT_DISCONNECT_CMD* pRecvMsg = (MSG_SF_AUTH_CLIENT_DISCONNECT_CMD*)pMsg;

    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->m_dwFrontUserID);
    if(client != 0)
    {
        client->Disconnect(TRUE);
        SUNLOG(eCRITICAL_LOG, "[OnSF_AUTH_CLIENT_DISCONNECT_CMD] User[%s] is Disconnected!", 
               client->GetAccountID());
    }
}

#ifdef _SECURE_AUTH
void
AuthServerSession::OnSF_AUTH_MATRIX_CARD_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_MATRIX_CARD_SYN* pRecvMsg = (MSG_SF_AUTH_MATRIX_CARD_SYN*)pMsg;

    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->m_dwFrontUserID);
    if(client != 0)
    {
        MSG_FC_AUTH_MATRIX_CARD_SYN	sendPacket;
        sendPacket.m_byResult = pRecvMsg->m_byResult;
        BOOST_STATIC_ASSERT(sizeof(sendPacket.m_SecureInfo) == sizeof(pRecvMsg->m_SecureInfo));
        CopyMemory(&sendPacket.m_SecureInfo, &pRecvMsg->m_SecureInfo, sizeof(sendPacket.m_SecureInfo));
        //memcpy(&(sendPacket.m_SecureInfo), &(pRecvMsg->m_SecureInfo), sizeof(SECURE_INFO));
        client->SendPacket(&sendPacket, sizeof(sendPacket));
        SC_INFO* pInfo = (SC_INFO*)sendPacket.m_SecureInfo.byData;

        SUNLOG(eFULL_LOG, "[OnSF_AUTH_MATRIX_CARD_SYN] m_wMatrix[%u,%u,%u]",
               pInfo->wMatrix[0], pInfo->wMatrix[1], pInfo->wMatrix[2]);
    }
}
#endif

//#ifdef _SHAPE_AUTH
//void
//AuthServerSession::OnSF_AUTH_SHAPE_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
//{
//    MSG_SF_AUTH_SHAPE_SYN* pRecvMsg = (MSG_SF_AUTH_SHAPE_SYN*)pMsg;
//
//    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->m_dwFrontUserID);
//    if(client != 0)
//    {
//        MSG_FC_AUTH_SHAPE_SYN sendPacket;
//
//        sendPacket.m_btResult        = pRecvMsg->m_btResult;
//        sendPacket.m_btAuthFailCount = pRecvMsg->m_btAuthFailCount;
//        sendPacket.m_btDataSts       = pRecvMsg->m_btDataSts;
//        sendPacket.m_dwShapeDataSize = pRecvMsg->m_dwShapeDataSize;
//
//        if(1 == sendPacket.m_btDataSts)
//        {
//            memcpy(sendPacket.m_szShapeData, pRecvMsg->m_szShapeData, sendPacket.m_dwShapeDataSize);
//        }
//
//        client->SendPacket(&sendPacket,
//                           sizeof(sendPacket) - (SHAPE_DATA_MAX_LEN - (sendPacket.m_dwShapeDataSize * sizeof(BYTE))));
//
//        if(2 == sendPacket.m_btDataSts)
//        {
//            SUNLOG(eFULL_LOG, "[OnSF_AUTH_SHAPE_SYN] Ask Shape Auth\n");
//        }
//    }
//    else
//    {
//        SUNLOG(eCRITICAL_LOG, "[OnSF_AUTH_SHAPE_SYN] Cannot find session for Ask Shape Auth\n");
//    }
//}
//#endif

void
AuthServerSession::OnSF_AUTH_SERVER_LIST_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_SERVER_LIST_NTF* pRecvMsg = (MSG_SF_AUTH_SERVER_LIST_NTF*)pMsg;

    ServerDataManager::Instance()->SetDynamicServerListInfo(pRecvMsg->m_ServerDataList,
                                                            pRecvMsg->m_Count);
}

void
AuthServerSession::OnSF_AUTH_SERVER_SELECT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SF_AUTH_SERVER_SELECT_ACK* pRecvMsg = (MSG_SF_AUTH_SERVER_SELECT_ACK*)pMsg;

    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->m_dwFrontUserID);
    if(client != 0)
    {
        MSG_FC_AUTH_SERVER_SELECT_ACK sendPacket;
        sendPacket.m_byResult     = pRecvMsg->m_byResult;
        sendPacket.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
        sendPacket.m_dwSvrPort    = pRecvMsg->m_dwSvrPort;
        sendPacket.m_byResult     = pRecvMsg->m_byResult;

        ::_tcsncpy(sendPacket.m_szSvrIP, pRecvMsg->m_szSvrIP, IP_MAX_LEN);
        ::memcpy(sendPacket.m_serialKey, pRecvMsg->m_serialKey, 32);
        ::memcpy(sendPacket.m_szGLogKey, pRecvMsg->m_szGLogKey, GLOGKEY_LEN);

        client->SendPacket(&sendPacket, sizeof(sendPacket));

        SUNLOG(eFULL_LOG,
               "[OnSF_AUTH_SERVER_SELECT_ACK] Success!"
               " Result[%d] AuthUserID[%d] SvrPort[%d] SvrIP[%s]",
               sendPacket.m_byResult, sendPacket.m_dwAuthUserID,
               sendPacket.m_dwSvrPort, sendPacket.m_szSvrIP);
    }
}

//#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
//void
//AuthServerSession::OnSF_AUTH_REAUTH_PASS_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
//{
//    const MSG_SF_AUTH_REAUTH_PASS_ACK* pRecvMsg = static_cast<MSG_SF_AUTH_REAUTH_PASS_ACK*>(pMsg);
//    MSG_FC_AUTH_REAUTH_PASS_ACK msg;
//
//    msg.m_byResult = pRecvMsg->m_byResult;
//    msg.m_byFailCount = pRecvMsg->m_byFailCount;
//    BOOST_STATIC_ASSERT(_countof(msg.m_szInfo) == _countof(pRecvMsg->m_szInfo));
//    ::_tcsncpy(msg.m_szInfo, pRecvMsg->m_szInfo, _countof(msg.m_szInfo));
//
//    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->m_dwFrontUserID);
//    if(client == NULL) 
//        return;
//
//    client->SendPacket(&msg, sizeof(msg));
//    //SUNLOG(eCRITICAL_LOG,
//    //       "[AuthServerSession::OnS2SAnsAuth] Send Auth Result to Client(Result:%d)\n",
//    //       sendPacket.byResult);
//}
//#endif

#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
void
AuthServerSession::OnAU_AUTH_OTP_PASS_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_OTP_PASS_CMD* pRecvMsg = static_cast<const MSG_AU_AUTH_OTP_PASS_CMD*>(pMsg);
    AUTH_DEBUGLOG("check point == %d", 0);
    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->front_user_id);
    if (client == NULL) {
        AUTH_DEBUGLOG("Can't find user %d\n", pRecvMsg->front_user_id);
        return;
    }

    MSG_CF_AUTH_OTP_PASS_CMD msg_cmd;
    BOOST_STATIC_ASSERT(CF_AUTH_OTP_PASS_CMD == AU_AUTH_OTP_PASS_CMD);
    ZeroMemory(&msg_cmd, sizeof(msg_cmd));
    msg_cmd = *pRecvMsg; //up-cast
    client->SendPacket(&msg_cmd, sizeof(msg_cmd));
}

void
AuthServerSession::OnAU_AUTH_OTP_PASS_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_OTP_PASS_ACK* pRecvMsg = static_cast<const MSG_AU_AUTH_OTP_PASS_ACK*>(pMsg);
    AUTH_DEBUGLOG("check point == %d", 0);
    ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->front_user_id);
    if (client == NULL) {
        AUTH_DEBUGLOG("Can't find user %d\n", pRecvMsg->front_user_id);
        return;
    }

    MSG_CF_AUTH_OTP_PASS_ACK msg_cmd;
    BOOST_STATIC_ASSERT(CF_AUTH_OTP_PASS_ACK == AU_AUTH_OTP_PASS_ACK);
    ZeroMemory(&msg_cmd, sizeof(msg_cmd));
    msg_cmd = *pRecvMsg; //up-cast
    client->SendPacket(&msg_cmd, sizeof(msg_cmd));
}
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_

#ifdef __CN_20100909_SECURITY_TOKEN__	
void
AuthServerSession::OnAU_AUTH_SECURITY_TOKEN_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	const MSG_AU_AUTH_SECURITY_TOKEN_ACK* pRecvMsg = static_cast<const MSG_AU_AUTH_SECURITY_TOKEN_ACK*>(pMsg);
	AUTH_DEBUGLOG("check point == %d", 0);
	ClientSession* client = ClientManager::Instance()->FindSession(pRecvMsg->front_user_id);
	if (client == NULL) {
		AUTH_DEBUGLOG("Can't find user %d\n", pRecvMsg->front_user_id);
		return;
	}

	MSG_CF_AUTH_SECURITY_TOKEN_ACK msg;
	BOOST_STATIC_ASSERT(CF_AUTH_SECURITY_TOKEN_ACK == AU_AUTH_SECURITY_TOKEN_ACK);
	ZeroMemory(&msg, sizeof(msg));
	msg = *pRecvMsg; //up-cast
	client->SendPacket(&msg, sizeof(msg));
}
#endif //__CN_20100909_SECURITY_TOKEN__	



