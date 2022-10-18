#include "stdafx.h"
#include "Handler_AU_AUTH.h"
#include <PacketStruct_CG.h>
#include <ResultCode.h>
#include <ServerConst.h>
#include "../SolarShop/ShopDefine.h" // (WAVERIX) (090816) (NOTE) 적절히 수정할 것.

#include <AgentServer.h>
#include <ServerSessions/ServerSessionEx.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>
#include <UserSessions/TempUser.h>
#include <TimeCheck/ConnectionTimeout.h>
#include <TimeCheck/ConnectionTimeoutFactory.h>
#include <TimeCheck/TimeKeeper.h>

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_AU_AUTH::On##p }


sPACKET_HANDLER_INFO*	Handler_AU_AUTH::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(CATEGORY_AUTH, AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CATEGORY_AUTH, AG_AUTH_ASK_PC_LOGOUT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CATEGORY_AUTH, AG_AUTH_BILLING_ALARM_SYN, PHASE_UNLIMITED),	// 빌링 만료 알림
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        ADD_HANDLER_INFO(CATEGORY_AUTH, AU_AUTH_SERVER_SELECT_ACK, PHASE_UNLIMITED),
    #endif
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        ADD_HANDLER_INFO(CATEGORY_AUTH, AU_AUTH_COMMON_QUERY_USER_ID_ACK, PHASE_UNLIMITED),
    #endif
    #ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
        ADD_HANDLER_INFO(CATEGORY_AUTH, AU_AUTH_BILLING_CHANGE_CONTROL_SYN, PHASE_UNLIMITED),
    #endif
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_AU_AUTH::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<AUTH_AGENT_SERVER>( GetHandlerInfo() );
}


HANDLER_AU_IMPL(AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN* pRecvMsg = (MSG_AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN*)pMsg;

	MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK msg;
	msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
	msg.m_byResult = 0;
    CHECK_ID_LENGTH_BUFFER(msg.m_szID);
    CHECK_ID_LENGTH_BUFFER(pRecvMsg->m_szID);
	strncpy(msg.m_szID, pRecvMsg->m_szID, _countof(msg.m_szID));
    msg.m_szID[_countof(msg.m_szID) - 1] = '\0';

	ConnectionTimeout * pUserInfo = dynamic_cast<ConnectionTimeout*>( g_TimeKeeper.PeekTimer( pRecvMsg->m_dwAuthUserID ) );
	if( pUserInfo )
	{
		pUserInfo->ForceEnd();
		msg.m_byResult = 1;
		pServerSession->Send( (BYTE*)&msg, sizeof(msg) );
		SUNLOG( eCRITICAL_LOG, "[AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN] ConnectionTimeout User Already exist! m_dwAuthUserID" );
		return;
	}
	else
	{
		ConnectionTimeout * pUserInfo = dynamic_cast<ConnectionTimeout*>( g_TimeKeeper.PeekTimerByUserGUID( pRecvMsg->m_dwUserGuid ) );
		if( pUserInfo )
		{
			pUserInfo->ForceEnd();
			msg.m_byResult = 1;
			pServerSession->Send( (BYTE*)&msg, sizeof(msg) );
			SUNLOG( eCRITICAL_LOG, "[AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN] ConnectionTimeout User Already exist! m_dwUserGuid" );
			return;
		}
		else
		{
			// 현재 UserKey를 UserGUID로 사용하기 때문에 g_UserManager에서 찾아도 가능!!!!
			User * pPreUser = UserManager::Instance()->GetUser( pRecvMsg->m_dwUserGuid );
			if( pPreUser )
			{
				SUNLOG( eCRITICAL_LOG,  "[OnAU_AUTH_S2S_ASK_NEWCLIENT] [U:%d]동일유저가 존재한다", pPreUser->GetUserKey() );
				// 접속해 있는 유저를 끊어버린다.
				pPreUser->SetDisconnectCode( RC::RC_DISCONNECT_REMAIN_USER_CONNECTION );
				pPreUser->DisconnectUser();

				msg.m_byResult = 1;
				pServerSession->Send( (BYTE*)&msg, sizeof(msg) );
				return;
			}
		}

        // (WAVERIX) (090526) (CHANGES)
        //const DWORD USER_AUTH_EXPIRE_DELAY = 60*1000; // 30초 안에 접속이 이루어져야 한다.

		// 계정 정보와
		// 클라이언트IP
		// 빌링 정보는 아직 기록하고 있지 않다!
		ConnectionTimeout* pTimeout = g_ConnTimeoutFactory.AllocConnection();
		pTimeout->SetUserGUID( pRecvMsg->m_dwUserGuid );			// DB에서 읽어온 GUID
		pTimeout->SetAuthUserID( pRecvMsg->m_dwAuthUserID );		// AuthServer에서 발급한 식별자
		pTimeout->SetUserID( pRecvMsg->m_szID );					// 계정아이디
		pTimeout->SetClientIP( pRecvMsg->m_szClientIP );			// 클라이언트 아이피
		pTimeout->SetBillingType(pRecvMsg->m_BillingType);		    // 빌링 타입
		pTimeout->SetBillingInfo(pRecvMsg->m_szBillingInfo);		// 빌링 정보
		pTimeout->SetReservedValue(pRecvMsg->m_dwReservedValue);	// 예약 (0:일반사용자, 나머지값은 PC방 접속자 UID)
        pTimeout->SetAuthUserType(pRecvMsg->m_AuthUserType);        // 0:일반유저, 1:한게임유저, 2:피망유저
		pTimeout->SetSerialKey(pRecvMsg->m_serialKey);				// 접속 시리얼키
		pTimeout->SetSts(pRecvMsg->m_userCode[0]);
		pTimeout->SetPCRoomSts(pRecvMsg->m_userCode[1]);
		pTimeout->SetLastSSNKey( pRecvMsg->m_MD5Key );
    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
		pTimeout->SetAge( pRecvMsg->m_byAge );
		pTimeout->SetRealNameAuth( pRecvMsg->m_byRealNameAuth );
		SUNLOG( eDEV_LOG, "[Handler_AU_AUTH::OnAU_AUTH_S2S_ASK_NEWCLIENT] UserGuid = %d, Age = %d, RealNameAuth = %d", pRecvMsg->m_dwUserGuid, pRecvMsg->m_byAge, pRecvMsg->m_byRealNameAuth );
    #endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
    #ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
        pTimeout->SetGameChuUSN(pRecvMsg->gamechu_usn_);
    #endif
    #ifdef _NA_20100307_BETAKEY_SYSTEM
        pTimeout->SetBetaKey(pRecvMsg->betakey_);
    #endif
		pTimeout->Start(TempUser::GetConnectionTimeout());
		g_TimeKeeper.PushTimer( pTimeout->GetAuthUserID(), pTimeout );

		// OK
		msg.m_byResult = 0;
		pServerSession->Send( (BYTE*)&msg, sizeof(msg) );
	}
}

HANDLER_AU_IMPL(AG_AUTH_ASK_PC_LOGOUT_SYN)
{
    __TOUCHED((pServerSession, wSize));
#if USING_DUMMY_CLIENT
    return;
#endif
	// 게임서버로 부터 플레이어 접속 해제 요청 패킷
	MSG_AG_AUTH_ASK_PC_LOGOUT_SYN * pRecvMsg = (MSG_AG_AUTH_ASK_PC_LOGOUT_SYN*)pMsg;

	User * pUser = UserManager::Instance()->GetUserByAuthID( pRecvMsg->m_dwAuthUserID );

	if( pUser != NULL )
	{		
		SUNLOG( eFULL_LOG,  "[Handler_AU_AUTH::OnAU_AUTH_S2S_ASK_PC_LOGOUT] 인증 서버가 끊으라 해서 끊었음[ID:%d, AuthID:%d, szID:%s]", pUser->GetUserKey(), pUser->GetAuthID(), pRecvMsg->m_szID );

#ifdef _AUTH_USERDISCONN_CODE
		switch( pRecvMsg->m_dwLogoutCode )
		{
		case eDRC_OVERLAPPEDCONN:
			{
				pUser->SetDisconnectCode( RC::RC_DISCONNECT_OVERLAPPED_CONN_FROM_AUTH );
			}break;
		case eDRC_NOTAUTHUSER:
			{
				pUser->SetDisconnectCode( RC::RC_DISCONNECT_INVALID_AUTH_FROM_AUTH );
			}break;
		case eDRC_WOPS_GM:
			{
				pUser->SetDisconnectCode( RC::RC_DISCONNECT_FROM_GM );
			}break;
		case eDRC_BILLING_EXPIRE:
			{
				pUser->SetDisconnectCode( RC::RC_DISCONNECT_BILLING_EXPIRED );
			}break;
		default:
			{
				pUser->SetDisconnectCode( RC::RC_DISCONNECT_REQ_AUTH );
			}break;
		}
		pUser->DisconnectUser();
#else
		pUser->SetDisconnectCode( RC::RC_DISCONNECT_REQ_AUTH );
		pUser->DisconnectUser();
#endif

		MSG_GA_AUTH_ASK_PC_LOGOUT_ACK msg;
		msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
		msg.m_byResult = 1;
    #ifdef _NA000000_091005_EXTENDED_LOG_
        // 경우에 따라 (CharSelect scene)에서는 하기 내용이 존재하지 않을 것이다.
        const char* char_name = pUser->GetCharName();
        if(char_name) {
            strncpy(msg.m_CharName, char_name, _countof(msg.m_CharName));
            msg.m_CharName[_countof(msg.m_CharName) - 1] = '\0';
            msg.m_CharClass = pUser->GetSelectedCharClass();
            msg.m_CharLevel = pUser->GetSelectedCharLV();
        }
        else {
            msg.m_CharName[0] = '\0';
            msg.m_CharClass = 0;
            msg.m_CharLevel = 0;
        }
    #endif
		pServerSession->Send( (BYTE*)&msg, (WORD)sizeof(msg) );
	}
	else
	{
		ConnectionTimeout * pUserInfo = dynamic_cast<ConnectionTimeout*>( g_TimeKeeper.PeekTimer( pRecvMsg->m_dwAuthUserID ) );
		if( !pUserInfo )
		{
			pUserInfo = dynamic_cast<ConnectionTimeout*>( g_TimeKeeper.PeekTimerByUserGUID( pRecvMsg->m_dwUserGUID ) );
		}

		BYTE byLogOutType = AUTH_LOGOUT_AUTH_REQUEST;

		if( pUserInfo )
		{
			byLogOutType = AUTH_LOGOUT_AUTH_REQUEST;
			pUserInfo->ForceEnd();
		}
		else
		{
			SUNLOG( eFULL_LOG,  "[Handler_AU_AUTH::OnAU_AUTH_S2S_ASK_PC_LOGOUT] 인증서버가 없는 유저를 Logout시키려한다.[ToAuthID:%d, ID=%s]", pRecvMsg->m_dwAuthUserID, pRecvMsg->m_szID );
			byLogOutType = AUTH_LOGOUT_USER_NOT_FOUND;
		}

		MSG_GA_AUTH_ASK_PC_LOGOUT_ACK msg;
		msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
		msg.m_byResult = 0;
    #ifdef _NA000000_091005_EXTENDED_LOG_
        ZeroMemory(msg.m_CharName, sizeof(msg.m_CharName));
        msg.m_CharClass = 0;
        msg.m_CharLevel = 0;
    #endif
		pServerSession->Send( (BYTE*)&msg, (WORD)sizeof(msg) );
	}
}

// 빌링 만료 알림
HANDLER_AU_IMPL(AG_AUTH_BILLING_ALARM_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_AG_AUTH_BILLING_ALARM_SYN* pRecvMsg = (MSG_AG_AUTH_BILLING_ALARM_SYN*)pMsg;
	User * pUser = UserManager::Instance()->GetUserByAuthID( pRecvMsg->m_dwAuthUserID );

	//AgentServer* pAgentServer = AgentServer::GetInstance();
	if( !pUser )
	{
		SUNLOG( eFULL_LOG, "[OnAU_AUTH_S2S_BILLING_ALARM] No User To Send Alarm.[ToAuthID:%d]", pRecvMsg->m_dwAuthUserID );

		MSG_GA_AUTH_BILLING_ALARM_ACK nakMsg;
		nakMsg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
		nakMsg.m_byResult = 0;
		pServerSession->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

	MSG_CG_CONNECTION_BILLING_ALARM_BRD	sendMsg;
	sendMsg.dwRestTerm = pRecvMsg->m_dwRestTerm;
	pUser->SendPacket( &sendMsg, sizeof(sendMsg) );

	MSG_GA_AUTH_BILLING_ALARM_ACK ansMsg;
	ansMsg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
	ansMsg.m_byResult = 1;
	pServerSession->Send( (BYTE*)&ansMsg, sizeof(ansMsg) );

	SUNLOG( eFULL_LOG, "[OnAU_AUTH_S2S_BILLING_ALARM] Send Alarm Msg[ToUser:%s]", pUser->GetCharName() );
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
HANDLER_AU_IMPL(AU_AUTH_SERVER_SELECT_ACK)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_AU_AUTH_SERVER_SELECT_ACK* pRecvMsg =
        static_cast<const MSG_AU_AUTH_SERVER_SELECT_ACK*>(pMsg);

    User* pUser = UserManager::Instance()->GetUserByAuthID(pRecvMsg->m_AuthUserID);
    if(pUser == 0)
        return;

    MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK msgACK;
    BOOLEAN bValidState = TRUE;

    if(bValidState && pUser->IsBeginTransaction() == 0)
        bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_INVALID_PLAYER_STATE;

    if(bValidState && pUser->GetTransactionState() != TR_QUERY_SERVER_SELECT_TRANSACTION)
        bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;

    // 기존 정보 초기화 필요 가능성 타전...
    if(bValidState == 0)
    {
        pUser->SendPacket(&msgACK, msgACK.GetSize());
        return;
    }

    typedef nsPacket::ReAuthInfo Enum;
    pUser->OnServerSelectQueryResult(Enum::eReAuthSlot_AuthServerSide, Enum::eReAuthStep_Accepted,
                                     &pRecvMsg->m_ReAuthToken);
}
#endif

HANDLER_AU_IMPL(AU_AUTH_COMMON_QUERY_USER_ID_ACK)
{
#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

    __TOUCHED((pServerSession, wSize));
#if !defined(__NA000000_SHOP_USERID_50_PASSING_PROC__)
    ASSERT(!"Function "__FUNCTION__" not supported in this version");
#endif

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    MSG_AU_AUTH_COMMON_QUERY_USER_ID_ACK* pRecvMsg =
        static_cast<MSG_AU_AUTH_COMMON_QUERY_USER_ID_ACK*>(pMsg);

    User* pUser = UserManager::Instance()->GetUser(pRecvMsg->user_info_.requester_user_key_);
    if(pUser == 0)
        return;

    BOOLEAN success = true;
    BYTE error_code = RC::RC_SHOP_SUCCESS;
    ShopUserQuerySlot& query_slot = pUser->GetShopUserQuerySlot();

    if(success && pRecvMsg->result_ == 0)
    { success = false, error_code = RC::RC_SHOP_USERISNOTFOUND; }

    if(success && query_slot.IsTransaction() == 0)
    { success = false, error_code = RC::RC_SHOP_FAILED; }

    if(success && pUser->IsBeginTransaction() != 0) // by long-term transaction...
    { success = false, error_code = RC::RC_SHOP_FAILED; }

    if(success) {
        const MSG_CG_SHOP_SEND_GIFT_SYN* requested_msg = query_slot.GetMessage();
        assert(requested_msg);
        if(!FlowControl::FCAssert(_strnicmp(requested_msg->m_tszCharName,
                                            pRecvMsg->user_info_.char_name_,
                                            _countof(requested_msg->m_tszCharName)) == 0))
        {
            success = false, error_code = RC::RC_SHOP_FAILED;
            // (WAVERIX) (WARNING) ShopUserQuerySlot 데이터 설정 부분 검증할 것.
            //SUNLOG(eCRITICAL_LOG, "Check ShopUserQuerySlot argument input point '%s' != '%s'",
            //       requested_msg->m_tszCharName, pRecvMsg->user_info_.char_name_);
        }
    }
    // latest result process
    if(success) {
        const MSG_CG_SHOP_SEND_GIFT_SYN* requested_msg = query_slot.GetMessage();

        MSG_CG_SHOP_SEND_GIFT_RELAY_SYN msgSYN;
        msgSYN.item_type_ = requested_msg->m_ItemType;
        msgSYN.price_guid_ = requested_msg->m_PriceGuid;
        msgSYN.purchase_type_ = requested_msg->m_PurchaseType;
        strncpy(msgSYN.gift_message_, requested_msg->m_tszGiftMessage,
                _countof(msgSYN.gift_message_));
        msgSYN.gift_message_[_countof(msgSYN.gift_message_) - 1] = '\0';
        msgSYN.user_info_ = pRecvMsg->user_info_;

        pUser->SendToLinkedServer(&msgSYN, msgSYN.GetSize());
    } else {
        MSG_CG_SHOP_SEND_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = error_code;
        pUser->SendPacket(&msgNAK, sizeof(msgNAK));
    }

    // release resource
    query_slot.QueryEnd();
#endif //__NA000000_SHOP_USERID_50_PASSING_PROC__

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
}

HANDLER_AU_IMPL(AU_AUTH_BILLING_CHANGE_CONTROL_SYN)
{
    __TOUCHED((pServerSession, wSize));
#if !defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED)
    ASSERT(!"Function "__FUNCTION__" not supported in this version");
#endif

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN* pRecvMsg =
        static_cast<MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN*>(pMsg);

    BOOLEAN success = true;
    //MSG_AU_AUTH_BILLING_CHANGE_CONTROL_ACK msgACK;
    //MSG_AU_AUTH_BILLING_CHANGE_CONTROL_ACK::eResult result = msgACK.eResult_Success;

    UserManager* pUserManager = UserManager::Instance();
    User* pUser = pUserManager->GetUser(pRecvMsg->user_key_);
    if(pUser != 0)
    {
        pUser->SetBillingType(pRecvMsg->status_);
        pUser->SetReservedValue(pRecvMsg->pc_room_id_);
        pUser->NotifyPcBangBillingStatus(true, true);
    }
    else
    {
        ConnectionTimeout* pUserInfo =
            dynamic_cast<ConnectionTimeout*>(g_TimeKeeper.PeekTimer(pRecvMsg->auth_user_key_));
        if(pUserInfo == 0) {
            pUserInfo = dynamic_cast<ConnectionTimeout*>(
                g_TimeKeeper.PeekTimerByUserGUID(pRecvMsg->user_key_));
        }
        if(pUserInfo == 0) {
            SUNLOG(eCRITICAL_LOG, "PC방 상태를 변경시킬 유저를 찾을 수 없습니다. %u\n",
                   pRecvMsg->user_key_);
            success = false;
        }

        if(success) {
            pUserInfo->SetBillingType(pRecvMsg->status_);
            pUserInfo->SetReservedValue(pRecvMsg->pc_room_id_);
        }
    }

    //msgACK. 처리를...음...
#endif
}

