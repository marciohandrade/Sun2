#include "stdafx.h"
#include ".\AuthAgentServerSession.h"
#include <AuthProtocol_FS.h>
#include <AuthProtocol_AS.h>

#include ".\AuthFrame.h"
#include ".\LoginFrontServerSession.h"
#include ".\ServerDataManager.h"
#include ".\AccountManager.h"
#include ".\QueryManager.h"
#include ".\RTTAServerSession.h"
#ifdef __NA_20100201_INTERGRATEDBILLING_
#include "IntegratedBillingServerSession.h"
#else
#include ".\BillingServerSession.h"
#endif

#include "AlertRequestSender.h"
#include "ReAuthInfo.h"

#ifdef _NA_20120215_RUSSIA_PREMIUM_SERVICE
#include "IngambaAuthConnector.h"
#endif

AuthAgentServerSession::AuthAgentServerSession(void)
{

}

AuthAgentServerSession::~AuthAgentServerSession(void)
{

}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
void AuthAgentServerSession::OnRedirect(
    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(session_index, first_contact_msg);

	MSG_SA_AUTH_AGENT_IP_LIST_NTF msg;
	msg.m_Count = ServerDataManager::Instance()->GetServerIPListInfo(msg.m_AgentIPInfo);
	SendPacket(&msg, msg.GetSize());
}

class AuthAgentDisconnectedHandler
{
public:
	void operator() (ServerData* pServerData)
	{
		pServerData->SetLoginUserCnt(0);
		pServerData->SetAuthAgentSession(NULL);
		pServerData->SetConnectStatus(ServerData::SERVER_STATE_DISCONNECTED);

		AccountManager::Instance()->ClearLocation(pServerData->GetGameAgentID());
		// 지금 Auth랑 AuthAgent랑 연결이 끊어지면 복구 처리가 안되어있는거 같다;;
	}
};

void
AuthAgentServerSession::OnDisconnect()
{
	AuthAgentDisconnectedHandler op;
	ServerDataManager::Instance()->Foreach_ServerData(op);

	ServerSession::OnDisconnect();
}

void
AuthAgentServerSession::OnRecv(BYTE* pMsg, WORD wSize)
{
	MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL*)pMsg;
    SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pRecvMsg, wSize);

	switch(pRecvMsg->m_byProtocol)
	{
	case AS_AUTH_GAMEAGENT_LOGIN_CMD:   OnAS_AUTH_GAMEAGENT_LOGIN_CMD(pRecvMsg, wSize); break;
	case AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK: OnAS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK(pRecvMsg, wSize);break;
	case AS_AUTH_PC_LOGIN_CMD:	        OnAS_AUTH_PC_LOGIN_CMD(pRecvMsg, wSize);        break;
	case AS_AUTH_PC_LOGOUT_CMD:		    OnAS_AUTH_PC_LOGOUT_CMD(pRecvMsg, wSize);       break;
	case AS_AUTH_ASK_PC_LOGOUT_ACK:	    OnAS_AUTH_ASK_PC_LOGOUT_ACK(pRecvMsg, wSize);   break;
	case AS_AUTH_PC_NAME_NTF:		    OnAS_AUTH_PC_NAME_NTF(pRecvMsg, wSize);         break;
	case AS_AUTH_USER_COUNT_NTF:		OnAS_AUTH_USER_COUNT_NTF(pRecvMsg, wSize);      break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    case AU_AUTH_SERVER_SELECT_SYN:     OnAU_AUTH_SERVER_SELECT_SYN(pRecvMsg, wSize);   break;        
#endif
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    case AU_AUTH_COMMON_QUERY_USER_ID_SYN: OnAU_AUTH_COMMON_QUERY_USER_ID_SYN(pRecvMsg, wSize); break;
#endif
	default:
		{
			SUNLOG(eCRITICAL_LOG, "[AuthAgentServerSession]Invalid Packet! Category[%u] Protocol[%u] Size[%u]\n", pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
		}
	}
}

class SendServerList_broadcast : public std::unary_function<ServerSession*, void>
{
public:
	void operator() (ServerSession* pSession) const
	{
		if(pSession->GetServerType() == LOGIN_FRONT_SERVER)
		{
			LoginFrontServerSession* pLogin = (LoginFrontServerSession*)pSession;
			pLogin->SendServerList();
		}
	}
};

void
AuthAgentServerSession::OnAS_AUTH_GAMEAGENT_LOGIN_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_GAMEAGENT_LOGIN_CMD* pRecvMsg = (MSG_AS_AUTH_GAMEAGENT_LOGIN_CMD*)pMsg;

	ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
	if(!pServerData)
	{
		SUNLOG(eCRITICAL_LOG, "[OnS2SServerLogin] Not Info GameAgentID[%d]", pRecvMsg->m_GameAgentID);
		return;
	}

	switch(pRecvMsg->m_ConnectStatus)
	{
	case 0: //<  Disconencted
		{
			pServerData->SetAuthAgentSession(NULL);
			pServerData->SetLoginUserCnt(0);
			pServerData->SetConnectStatus(ServerData::SERVER_STATE_DISCONNECTED);

			SUNLOG(eCRITICAL_LOG, "[OnS2SServerLogin] GameAgentID[%d] is Disconnected", pRecvMsg->m_GameAgentID);

			// 에이젼트에 물려있는 계정 위치 정보 초기화
			AccountManager::Instance()->ClearLocation(pRecvMsg->m_GameAgentID);

		}
        break;
	case 1:	//< Connected
		{
			pServerData->SetAuthAgentSession(this);
			pServerData->SetLoginUserCnt(0);
			pServerData->SetConnectStatus(ServerData::SERVER_STATE_CONNECTED);

			SUNLOG(eCRITICAL_LOG, "[OnS2SServerLogin] GameAgentID[%d] is Connected", pRecvMsg->m_GameAgentID);

		}
        break;
	}

	// LoginFront로 게임Agent서버 변경 정보값 보냄~

	SendServerList_broadcast send;
	AuthFrame::Instance()->for_each(send);
}

void
AuthAgentServerSession::OnAS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK*	pRecvMsg = (MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK*)pMsg;

	AccountInfo* pAccountInfo = AccountManager::Instance()->Find(pRecvMsg->m_szID);
	if(pAccountInfo == NULL)
	{
		SUNLOG(eCRITICAL_LOG, "[OnS2SAnsNewClient] GameAgentID[%u] Cannot find Authkey(=0x%x, %s)",
               pRecvMsg->m_GameAgentID, pRecvMsg->m_dwAuthUserID, pRecvMsg->m_szID);
		return;
	}

    AuthFrame* const pAuthFrame = AuthFrame::Instance();
	LoginFrontServerSession* front =
        static_cast<LoginFrontServerSession*>(pAuthFrame->FindSession(pAccountInfo->m_LoginFrontId));
	if(front == NULL)
	{
		SUNLOG(eCRITICAL_LOG,
               "[OnS2SAnsNewClient] GameAgentID[%u] Cannot find LoginFrontServerSession(id=%d)",
               pRecvMsg->m_GameAgentID, pAccountInfo->m_LoginFrontId);
		return;
	}

	MSG_SF_AUTH_SERVER_SELECT_ACK sendPacket;
	sendPacket.m_dwFrontUserID = pAccountInfo->m_LoginFrontUserId;
	sendPacket.m_dwAuthUserID = pAccountInfo->m_authKey;
	::memcpy(sendPacket.m_serialKey, pAccountInfo->m_serialKey, sizeof(pAccountInfo->m_serialKey));

	// Client가 선택한 Agent에 대한 Ip/Port 정보를 찾는다.
	ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
	if(pServerData != NULL && pServerData->GetAuthAgentSession() != NULL)
	{
		::_tcsncpy(sendPacket.m_szSvrIP, pServerData->GetGameAgentPublicIP(), IP_MAX_LEN);
		sendPacket.m_dwSvrPort = pServerData->GetGameAgentPort();
	}
	else
	{
		::_tcsncpy(sendPacket.m_szSvrIP, "", IP_MAX_LEN);
		sendPacket.m_dwSvrPort = 0;
	}

	//  해당 서버는 현재 정상이 아니다 -_-
	if(pRecvMsg->m_byResult != 0)
	{
		ZeroMemory(sendPacket.m_szGLogKey, sizeof(sendPacket.m_szGLogKey));
		sendPacket.m_byResult = 1;

		SUNLOG(eCRITICAL_LOG,
               "[OnS2SAnsNewClient] GameAgentID[%u] Denied from Gameserver(or authagent)."
               "(id=%s, ret=%d, authSts=%d)",
			   pRecvMsg->m_GameAgentID, pAccountInfo->m_account,
               pRecvMsg->m_byResult, pAccountInfo->m_status);
	}
	else
	{
		//  GLog Key는 정상일 경우에만 보낸다.
		memcpy(sendPacket.m_szGLogKey, pAccountInfo->m_glogKey, GLOGKEY_LEN);
		sendPacket.m_byResult = 0;

		pAccountInfo->m_status = ACCOUNTSTATUS_TRYPLAY;
		if(pServerData)
			pAccountInfo->m_locationId = pServerData->GetGameAgentID();
	}

	front->SendPacket(&sendPacket, sizeof(sendPacket));

	SUNLOG(eFULL_LOG, "[OnAS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK] Success!");
}

void
AuthAgentServerSession::OnAS_AUTH_PC_LOGIN_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_PC_LOGIN_CMD* pRecvMsg = static_cast<MSG_AS_AUTH_PC_LOGIN_CMD*>(pMsg);

	ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
	if(pServerData == 0)
	{
		SUNLOG(eCRITICAL_LOG,
               "[OnAS_AUTH_PC_LOGIN_CMD] Cannot find GameAgentID[%u]",
               pRecvMsg->m_GameAgentID);
		return;
	}

	AccountInfo* pAccountInfo = AccountManager::Instance()->Find(pRecvMsg->m_szAccountID);
	if(pAccountInfo == 0)
	{
		SUNLOG(eCRITICAL_LOG,
               "[OnAS_AUTH_PC_LOGIN_CMD] GameAgentID[%u] pAccountInfo[%s] is NULL!",
               pRecvMsg->m_GameAgentID, pRecvMsg->m_szAccountID);
		goto ERROR_RESULT;
	}

	if(pAccountInfo->m_authKey != pRecvMsg->m_dwAuthUserID)
	{
		pAccountInfo->m_locationId = 0;
		SUNLOG(eCRITICAL_LOG,
               "[OnAS_AUTH_PC_LOGIN_CMD] GameAgentID[%u] Account[%s] m_dwAuthUserID[%u] is Invalid!", 
			   pRecvMsg->m_GameAgentID, pRecvMsg->m_szAccountID, pRecvMsg->m_dwAuthUserID);
		goto ERROR_RESULT;
	}

	pAccountInfo->m_status		= ACCOUNTSTATUS_PLAY;
	pAccountInfo->m_locationId	= pRecvMsg->m_GameAgentID;

	// 유저수 증가
	pServerData->IncLoginUserCount();

	SUNLOG(eFULL_LOG, "[OnAS_AUTH_PC_LOGIN_CMD] Account[%s] GameAgentID[%d] authStatus[%d]",
		   pAccountInfo->m_account, pAccountInfo->m_locationId, pAccountInfo->m_status);

#ifdef __NA_20100201_INTERGRATEDBILLING_
	IntegratedBillingServerSession* pBillingSession = (IntegratedBillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#else
    BillingServerSession* pBillingSession = (BillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#endif
	// 빌링서버에 알린다
#ifdef _BILLING_ONLYPCBANG
	if(pAccountInfo->m_pcBangId)
#endif
	{
		pAccountInfo->m_tickBillingTime = ::GetTickCount();	// 이때부터 빌링 체크 시작
		
#if defined (__20100916_BILLING_GODLCHANNEL__)
        if(pBillingSession) pBillingSession->InquirePersonDeduct(*pAccountInfo);
#elif defined (_NA_20120215_RUSSIA_PREMIUM_SERVICE)
        IngambaAuthConnector* ingamba_connector = AuthFrame::Instance()->GetIngambaAuthConnector();
        RC::PrimiumServiceResult result_code = RC::RC_PRIMIUM_FAIL;
        if (ingamba_connector)
        {
            result_code = ingamba_connector->StartPrimiumService(*pAccountInfo);
        }

        if (result_code == RC::RC_PRIMIUM_SUCCESS)
        {
            AuthAgentServerSession* auth_agent_session =
                ServerDataManager::Instance()->FindAuthAgentServerSession(pAccountInfo->m_locationId);

            if (auth_agent_session) 
            {
                MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN send_msg;
                send_msg.game_agent_id_ = pAccountInfo->m_locationId;
                send_msg.status_.status_ = send_msg.status_.eStatus_PcRoom;
                send_msg.pc_room_id_ = 1;
                send_msg.user_key_ = pAccountInfo->m_userGuid;
                send_msg.auth_user_key_ = pAccountInfo->m_authKey;
                auth_agent_session->SendPacket(&send_msg, sizeof(send_msg));

                pAccountInfo->m_pcBangStatus.status_ = pAccountInfo->m_pcBangStatus.eStatus_PcRoom;
            }
        }
#else

#ifdef _NA_005011_20120621_MULTICLIENTS
        if (pBillingSession) {
            pBillingSession->CheckIn(*pAccountInfo);
            if (pBillingSession->CheckMultiUserTypeAndLogin(*pAccountInfo) == true) {
                pBillingSession->SendLogin(*pAccountInfo);
            }
        }
#else
		if(pBillingSession) pBillingSession->SendLogin(*pAccountInfo);
#endif //_NA_005011_20120621_MULTICLIENTS
#endif
		
			
	}

    WORD location = pServerData->GetGameAgentID();
    QueryManager* const pQueryManager = QueryManager::Instance();
	//  UserSts를 변경한다.
    // (NOTE) current state : AgentServer::TempUser->CharSelect
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	pQueryManager->QueryChangeUserSts(pAccountInfo->m_account,
                                      pAccountInfo->m_userGuid,
                                      pAccountInfo->m_glogKey,
                                      pAccountInfo->m_locationId,
                                      pAccountInfo->m_clientIp,
                                      pAccountInfo->m_localIp,
                                      USERSTATUS_PLAY,
                                      pAccountInfo->m_loginTime,
                                      pAccountInfo->m_authKey);
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    pQueryManager->Query_GspUpdateUserState(pAccountInfo->m_userGuid,
                                            location,
                                            pAccountInfo->m_clientIp,
                                            true);
#endif
	//  로그기록
	{
    #if !defined(_RU_INGAMBA_AUTH_PROCESS)
		pQueryManager->QueryConnectLog(pAccountInfo->m_userGuid,
                                       pAccountInfo->m_account,
                                       location,
                                       "",
                                       0, 0,
                                       pAccountInfo->m_clientIp,
                                       pAccountInfo->m_pcBangId,
                                       0, pAccountInfo->m_loginTime,
                                       0);
    #endif
    // in global - connection log 남기지 않는다.
	}

	return;

ERROR_RESULT:
    {   //ERROR_RESULT
	    //  인증되지 않은 사용자이므로 종료를 요청해야한다.
	    MSG_SA_AUTH_ASK_PC_LOGOUT_SYN sendLogout;
	    sendLogout.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
	    sendLogout.m_GameAgentID = pRecvMsg->m_GameAgentID;
	    sendLogout.m_dwUserGUID = pAccountInfo != NULL ? pAccountInfo->m_userGuid : 0;
        CHECK_ID_LENGTH_BUFFER(pRecvMsg->m_szAccountID);
        CHECK_ID_LENGTH_BUFFER(sendLogout.m_szID);
	    ::_tcsncpy(sendLogout.m_szID, pRecvMsg->m_szAccountID, _countof(sendLogout.m_szID));
        sendLogout.m_szID[_countof(sendLogout.m_szID) - 1] = '\0';

	    sendLogout.m_dwLogoutCode = eDRC_NOTAUTHUSER;
	    SendPacket(&sendLogout, sizeof(sendLogout));
    };
}

#ifdef _NA_20120215_RUSSIA_PREMIUM_SERVICE 
void
AuthAgentServerSession::OnAS_AUTH_PC_LOGOUT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_AS_AUTH_PC_LOGOUT_CMD* recv_msg = (MSG_AS_AUTH_PC_LOGOUT_CMD*)pMsg;

    ServerData* server_data = ServerDataManager::Instance()->FindServerData(recv_msg->m_GameAgentID);
    if (server_data == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Can not find game agent : id : %u",
            recv_msg->m_GameAgentID
            );
        return;
    }

    AccountInfo* account_info = AccountManager::Instance()->Find(recv_msg->m_szAccountID);
    if (account_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found account info : agent id : %u, account : %s",
            recv_msg->m_GameAgentID, 
            recv_msg->m_szAccountID
            );
        return;
    }

    if (account_info->m_authKey != recv_msg->m_dwAuthUserID)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : auth key is defferent : agent id : %u, account : %s , key : %u != %u",
            recv_msg->m_GameAgentID, 
            account_info->m_account,
            account_info->m_authKey, 
            recv_msg->m_dwAuthUserID
            );
        return;
    }

    QueryManager* const pQueryManager = QueryManager::Instance();
    //  UserSts를 변경한다.
    //  (NOTE) current state : AgentServer::(Temp)User::OnDisconnect
    pQueryManager->QueryChangeUserSts(account_info->m_account,
        account_info->m_userGuid,
        account_info->m_glogKey,
        account_info->m_locationId,
        account_info->m_clientIp, 
        account_info->m_localIp,
        USERSTATUS_LOGOUT, 
        account_info->m_loginTime);

    if (account_info->m_pcBangStatus.status_ == CHANGE_PCROOM_STATUS::eStatus_PcRoom)
    {
        IngambaAuthConnector* ingamba_auth_connector = 
            AuthFrame::Instance()->GetIngambaAuthConnector();

        if (ingamba_auth_connector)
        {
            ingamba_auth_connector->StopPrimiumService(*account_info);
        }
    }

    if (AccountManager::Instance()->ClearInfo(recv_msg->m_szAccountID))
    {
        server_data->DecLoginUserCount();
    }
}
#else//_NA_20120215_RUSSIA_PREMIUM_SERVICE
void
AuthAgentServerSession::OnAS_AUTH_PC_LOGOUT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_PC_LOGOUT_CMD* pRecvMsg = (MSG_AS_AUTH_PC_LOGOUT_CMD*)pMsg;

	ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
	if(!pServerData)
	{
		SUNLOG(eCRITICAL_LOG, "[OnAS_AUTH_PC_LOGOUT_CMD] Cannot find GameAgentID[%u]",
               pRecvMsg->m_GameAgentID);
		return;
	}

	AccountInfo* pAccountInfo = AccountManager::Instance()->Find(pRecvMsg->m_szAccountID);
	if(!pAccountInfo)
	{
		SUNLOG(eCRITICAL_LOG, "[OnAS_AUTH_PC_LOGOUT_CMD] GameAgentID[%u] pAccountInfo[%s] is NULL!",
               pRecvMsg->m_GameAgentID, pRecvMsg->m_szAccountID);
		return;
	}

	if(pAccountInfo->m_authKey != pRecvMsg->m_dwAuthUserID)
	{
		SUNLOG(eCRITICAL_LOG,
               "[OnAS_AUTH_PC_LOGOUT_CMD] GameAgentID[%u] Account[%s] AuthKey[%u != %u] is different!", 
			   pRecvMsg->m_GameAgentID, pAccountInfo->m_account,
               pAccountInfo->m_authKey, pRecvMsg->m_dwAuthUserID);
		return;
	}

    QueryManager* const pQueryManager = QueryManager::Instance();
	//  UserSts를 변경한다.
    //  (NOTE) current state : AgentServer::(Temp)User::OnDisconnect
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	pQueryManager->QueryChangeUserSts(pAccountInfo->m_account, pAccountInfo->m_userGuid,
                                      pAccountInfo->m_glogKey,
		                              pAccountInfo->m_locationId,
                                      pAccountInfo->m_clientIp, pAccountInfo->m_localIp,
                                      USERSTATUS_LOGOUT, pAccountInfo->m_loginTime);
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    pQueryManager->Query_GspUpdateUserState(pAccountInfo->m_userGuid, pAccountInfo->m_locationId,
                                            pAccountInfo->m_clientIp, false);
#endif

	//  빌링서버에 통보한다.
#ifdef __NA_20100201_INTERGRATEDBILLING_
    IntegratedBillingServerSession* pBillingSession = (IntegratedBillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#else
    BillingServerSession* pBillingSession = (BillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#endif
	
	if(pBillingSession)
	{
#ifdef _BILLING_ONLYPCBANG
		if(pAccountInfo->m_pcBangId)
#endif
		{
#ifdef _NA_005011_20120621_MULTICLIENTS
            pBillingSession->CheckOut(*pAccountInfo);
#else
            pBillingSession->SendLogout(*pAccountInfo);
#endif //_NA_005011_20120621_MULTICLIENTS
		}
	}

#ifdef _NET_CAFE	// 일본 넷카페 DB에 유저가 로그아웃함을 통보한다
	if(pAccountInfo->m_pcBangId)
	{
		pQueryManager->QueryNetCafeLogInsert(pAccountInfo->m_LoginFrontId,
                                             pAccountInfo->m_LoginFrontUserId,
                                             pAccountInfo->m_account,
                                             pAccountInfo->m_clientIp,
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                             NETCAFE_LOGOUT,
#else
                                             false,
#endif
                                             0);
	}
#endif

	//  로그기록
    // (CHANGES) AgentServer::TempUser일 경우 selected char가 없다.
    //           (null setting) 이름으로 구별하도록 전환.
	//if(pRecvMsg->m_wPCLevel >= 0 && pRecvMsg->m_wPCLevel < 1000)
    //2010-04-20 로그인 로그를 남겼으면 로그아웃로그를 남겨서 쌍을 맞춘다.
    //if(pRecvMsg->m_szPCName[0] != '\0')
    if (pAccountInfo->m_IsLoginLogWrite)    
	{
		WORD	connectTime			= AccountManager::CalcConnectTime(pAccountInfo);
		TCHAR	charNameBuffer[32 + 1] = {0,};
		WORD location				= pServerData->GetGameAgentID();
        BOOST_STATIC_ASSERT(_countof(charNameBuffer) > _countof(pRecvMsg->m_szPCName));
		::_tcsncpy(charNameBuffer, pRecvMsg->m_szPCName, _countof(pRecvMsg->m_szPCName));
        charNameBuffer[_countof(charNameBuffer) - 1] = _T('\0');

    // (NOTE) current state : AgentServer::User::OnDisconnect (no TempUser)
    #if !defined(__NA000000_GLOBAL_AUTH_PROCESS__) && !defined(_RU_INGAMBA_AUTH_PROCESS)
		pQueryManager->QueryConnectLog(pAccountInfo->m_userGuid, pAccountInfo->m_account,
                                       location, charNameBuffer,
			                           pRecvMsg->m_byPCClass, pRecvMsg->m_wPCLevel,
                                       pAccountInfo->m_clientIp,
                                       pAccountInfo->m_pcBangId,
                                       connectTime, pAccountInfo->m_loginTime, 1);
    #elif defined(__NA000000_GLOBAL_AUTH_PROCESS__) && defined(_NA000000_091005_EXTENDED_LOG_)
        DWORD play_tick = GetTickCount() - pAccountInfo->m_tickConnectTime;
        play_tick = play_tick / 1000;
        pQueryManager->Query_GspUpdateConnectLog(pAccountInfo->m_userGuid,
                                                 pAccountInfo->m_GameID,
                                                 location,
                                                 play_tick,
                                                 false,
                                                 charNameBuffer,
                                                 pRecvMsg->m_byPCClass,
                                                 pRecvMsg->m_wPCLevel,
                                                 pAccountInfo->m_clientIp);
    #elif defined (_RU_INGAMBA_AUTH_PROCESS)
        //Not use connectlog
    #else
        #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
	}

	if(AccountManager::Instance()->ClearInfo(pRecvMsg->m_szAccountID))
		pServerData->DecLoginUserCount();

	SUNLOG(eFULL_LOG, "[OnAS_AUTH_PC_LOGOUT_CMD] Account[%s] LogoutType[%u]",
           pRecvMsg->m_szAccountID, pRecvMsg->m_byLogoutType);
}

#endif//_NA_20120215_RUSSIA_PREMIUM_SERVICE

void
AuthAgentServerSession::OnAS_AUTH_ASK_PC_LOGOUT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_ASK_PC_LOGOUT_ACK* pRecvMsg = (MSG_AS_AUTH_ASK_PC_LOGOUT_ACK*)pMsg;

	ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
	if(!pServerData)
	{
		SUNLOG(eCRITICAL_LOG, "[OnAS_AUTH_ASK_PC_LOGOUT_ACK] Cannot find GameAgentID[%u]", pRecvMsg->m_GameAgentID);
		return;
	}
	AccountInfo* pAccountInfo = AccountManager::Instance()->FindWithAuthKey(pRecvMsg->m_dwAuthUserID);
	if(!pAccountInfo)
	{
		SUNLOG(eCRITICAL_LOG, "[OnAS_AUTH_ASK_PC_LOGOUT_ACK] GameAgentID[%u] pAccountInfo[%u] is NULL!", pRecvMsg->m_GameAgentID, pRecvMsg->m_dwAuthUserID);
		return;
	}

	SUNLOG(eFULL_LOG, "[OnAS_AUTH_ASK_PC_LOGOUT_ACK] Account[%s] Result[%u]", pAccountInfo->m_account, pRecvMsg->m_byResult);

    QueryManager* const pQueryManager = QueryManager::Instance();
    // (NOTE) current state - Agent로 diconnect request에 대한 reply result
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	pQueryManager->QueryChangeUserSts(pAccountInfo->m_account, pAccountInfo->m_userGuid,
                                      pAccountInfo->m_glogKey,
		                              pAccountInfo->m_locationId,
                                      pAccountInfo->m_clientIp, pAccountInfo->m_localIp,
                                      USERSTATUS_LOGOUT, pAccountInfo->m_loginTime);
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    pQueryManager->Query_GspUpdateUserState(pAccountInfo->m_userGuid, pAccountInfo->m_locationId,
                                            pAccountInfo->m_clientIp, false);
#endif

	//  빌링서버에 통보한다.
#ifdef __NA_20100201_INTERGRATEDBILLING_
    IntegratedBillingServerSession* pBillingSession = (IntegratedBillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#else
    BillingServerSession* pBillingSession = (BillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#endif
	if(pBillingSession)
	{
#ifdef _BILLING_ONLYPCBANG
		if(pAccountInfo->m_pcBangId)
#endif
		{
#ifdef _NA_005011_20120621_MULTICLIENTS
            pBillingSession->CheckOut(*pAccountInfo);
#else
            pBillingSession->SendLogout(*pAccountInfo);
#endif //_NA_005011_20120621_MULTICLIENTS
		}
	}

#ifdef _NET_CAFE	// 일본 넷카페 DB에 유저가 로그아웃함을 통보한다
	if(pAccountInfo->m_pcBangId)
	{
		pQueryManager->QueryNetCafeLogInsert(pAccountInfo->m_LoginFrontId,
                                             pAccountInfo->m_LoginFrontUserId,
                                             pAccountInfo->m_account,
                                             pAccountInfo->m_clientIp,
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                             NETCAFE_LOGOUT,
#else
                                             false,
#endif
                                             0);
	}
#endif

	//  로그기록 - 20060825 추가
	WORD connectTime = AccountManager::CalcConnectTime(pAccountInfo);
	WORD location = pServerData->GetGameAgentID();

    // (CHANGES) AgentServer::(Temp)User의 상태 구별을 수행한다.
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__) && !defined(_RU_INGAMBA_AUTH_PROCESS)
	pQueryManager->QueryConnectLog(pAccountInfo->m_userGuid, pAccountInfo->m_account,
                                   location, "",
                                   0, 0, pAccountInfo->m_clientIp,
                                   pAccountInfo->m_pcBangId, connectTime,
                                   pAccountInfo->m_loginTime, 1);
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__) && defined(_NA000000_091005_EXTENDED_LOG_)
    if(pRecvMsg->m_CharName[0] != '\0') {
        char char_name[MAX_CHARNAME_LENGTH + 1];
        strncpy(char_name, pRecvMsg->m_CharName, _countof(char_name));
        char_name[_countof(char_name) - 1] = '\0';
        DWORD play_tick = GetTickCount() - pAccountInfo->m_tickConnectTime;
        play_tick = play_tick / 1000;
        pQueryManager->Query_GspUpdateConnectLog(pAccountInfo->m_userGuid,
                                                 pAccountInfo->m_GameID,
                                                 location,
                                                 play_tick, false,
                                                 char_name, //char_name
                                                 pRecvMsg->m_CharClass,
                                                 pRecvMsg->m_CharLevel,
                                                 pAccountInfo->m_clientIp);
    }
#elif defined (_RU_INGAMBA_AUTH_PROCESS)
    //Not use connectlog
#else
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

	// 유저 감소
	pServerData->DecLoginUserCount();

	// 계정 처리
	AccountManager::Instance()->ClearInfo(pAccountInfo->m_account);
}

void
AuthAgentServerSession::OnAS_AUTH_PC_NAME_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_PC_NAME_NTF* pRecvMsg = (MSG_AS_AUTH_PC_NAME_NTF*)pMsg;

	AccountInfo* pAccountInfo = AccountManager::Instance()->Find(pRecvMsg->m_szAccountID);
	if(!pAccountInfo)
	{
		SUNLOG(eCRITICAL_LOG, "[OnAS_AUTH_PC_NAME_NTF] GameAgentID[%u] pAccountInfo[%s] is NULL!",
               pRecvMsg->m_GameAgentID, pRecvMsg->m_szAccountID);
		return;
	}

	if(pAccountInfo->m_authKey != pRecvMsg->m_dwAuthUserID)
	{
		SUNLOG(eCRITICAL_LOG,
               "[OnAS_AUTH_PC_NAME_NTF] GameAgentID[%u] Account[%s] AuthKey[%u != %u] is different!",
               pRecvMsg->m_GameAgentID, pAccountInfo->m_account,
               pAccountInfo->m_authKey, pRecvMsg->m_dwAuthUserID);
		return;
	}

	char szCharNameBuffer[PC_MAX_LEN + 1] = {0, };
	::strncpy(szCharNameBuffer, pRecvMsg->m_szPCName, PC_MAX_LEN);

    // (NOTE) current state : AgentServer::'Enter Village state'

#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__) && !defined(_RU_INGAMBA_AUTH_PROCESS)
	//  Log DB에 유저가 선택한 캐릭터명을 업데이트한다
	QueryManager::Instance()->QueryConnectLogUpdate(pAccountInfo->m_dwLogSeq,
        pAccountInfo->m_userGuid, szCharNameBuffer, pAccountInfo->m_loginTime);
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__) && defined(_NA000000_091005_EXTENDED_LOG_)
    QueryManager::Instance()->Query_GspUpdateConnectLog(pAccountInfo->m_userGuid,
                                                        pAccountInfo->m_GameID,
                                                        pRecvMsg->m_GameAgentID,
                                                        0,
                                                        true,
                                                        szCharNameBuffer,
                                                        pRecvMsg->m_CharClass,
                                                        pRecvMsg->m_CharLevel,
                                                        pAccountInfo->m_clientIp);
#elif defined (_RU_INGAMBA_AUTH_PROCESS)
    //Not use connectlog
#else
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
}

void
AuthAgentServerSession::OnAS_AUTH_USER_COUNT_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
	MSG_AS_AUTH_USER_COUNT_NTF*	pRecvMsg = (MSG_AS_AUTH_USER_COUNT_NTF*)pMsg;

	ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
	if(!pServerData)
	{
		SUNLOG(eCRITICAL_LOG, "[OnAS_AUTH_USER_COUNT_NTF] Cannot find GameAgentID[%u] UserCount[%u]", pRecvMsg->m_GameAgentID, pRecvMsg->m_wUserCount);
		return;
	}

	pServerData->SetLoginUserCnt(pRecvMsg->m_wUserCount);
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
void
AuthAgentServerSession::OnAU_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_SERVER_SELECT_SYN* pRecvMsg = static_cast<MSG_AU_AUTH_SERVER_SELECT_SYN*>(pMsg);
    ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->m_GameAgentID);
    if(pServerData == 0)
        return;

    AccountInfo* pAccountInfo = AccountManager::Instance()->FindWithAuthKey(pRecvMsg->m_AuthUserID);
    if(pAccountInfo == 0)
        return;

    MSG_AU_AUTH_SERVER_SELECT_ACK msgACK;
    msgACK.m_GameAgentID = pRecvMsg->m_GameAgentID;
    msgACK.m_AuthUserID = pRecvMsg->m_AuthUserID;
    msgACK.m_Result = msgACK.eResult_DefaultFail;

    BOOLEAN bValidState = TRUE;
    ReAuthManager* const pReAuthManager = ReAuthManager::Instance();
	if(pRecvMsg->m_Request == pRecvMsg->eRequest_QueryStart)
	{
		if(bValidState && pAccountInfo->m_ReAuthKey != 0)
			bValidState = FALSE, msgACK.m_Result = msgACK.eResult_DefaultFail;
		if(bValidState)
		{
			DWORD ipAddress = inet_addr(pAccountInfo->m_clientIp);
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
			/*const ReAuthInfo* pReAuthInfo = pReAuthManager->AllocReAuthInfo(
			pAccountInfo->m_userGuid, ipAddress, pAccountInfo->m_account);*/
			AccountInfo* account_info = 0;
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
			/*const ReAuthInfo* pReAuthInfo = pReAuthManager->AllocReAuthInfo(
			pAccountInfo->m_userGuid, ipAddress, pAccountInfo->m_account, pAccountInfo);*/
			AccountInfo* account_info = pAccountInfo;
#else
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
			const ReAuthInfo* pReAuthInfo = pReAuthManager->AllocReAuthInfo(
				pAccountInfo->m_userGuid, ipAddress, pAccountInfo->m_account,
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
				pAccountInfo->gamechu_usn_,
#endif
				pAccountInfo);	

			if(pReAuthInfo == 0)
				bValidState = FALSE, msgACK.m_Result = msgACK.eResult_DefaultFail;

			if(bValidState)
			{
				pAccountInfo->m_ReAuthKey = pReAuthInfo->GetReAuthKey();
				const ReAuthStepAuthTokenInfo* pInfo = pReAuthInfo->GetReAuthStepAuthTokenInfo();
				AuthStepAuthToken::MakeStream(0, pInfo->stored_stream, &msgACK);
				msgACK.m_Result = msgACK.eResult_Success;
			}
		}

		SendPacket(&msgACK, msgACK.GetSize());
		return;
	}
    else
    {   // no result packet
        if(bValidState && pAccountInfo->m_ReAuthKey == 0)
            bValidState = FALSE;

        const ReAuthInfo* pReAuthInfo = 0;
        if(bValidState && !(pReAuthInfo = pReAuthManager->FindReAuthInfo(pAccountInfo->m_ReAuthKey)))
            bValidState = FALSE;

        if(bValidState)
        {
            pReAuthManager->OnCancelByGameServer(pReAuthInfo);
            pAccountInfo->m_ReAuthKey = 0;
        }
    }
}
#endif

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
void
AuthAgentServerSession::OnAU_AUTH_COMMON_QUERY_USER_ID_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN* pRecvMsg =
        static_cast<MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN*>(pMsg);

    ServerData* pServerData = ServerDataManager::Instance()->FindServerData(pRecvMsg->game_agent_id_);
    if(pServerData == 0)
        return;

    QueryManager* pQueryManager = QueryManager::Instance();
#ifdef _AUTH_GLOBAL
    pQueryManager->Query_GspGetAccountID(pRecvMsg->game_agent_id_, pRecvMsg->user_info_);
#else
    pQueryManager->Query_UserId(pRecvMsg->game_agent_id_, pRecvMsg->user_info_);
#endif
}
#endif






