#include "stdafx.h"

#include "Handler_FromWorldServer.h"
#include "PacketHandler.Shared.h"

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
#include "AgentParty/AgentPartyManager.h"
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_AW::On##p }


sPACKET_HANDLER_INFO*	Handler_AW::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
        ADD_HANDLER_INFO(AW_CONNECTION, AW_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( AW_CONNECTION, AW_PREPARE_NEW_USER_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_CONNECTION, AW_PREPARE_NEW_USER_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_USER_STATUS, AW_USER_RESTORE_SYN, PHASE_UNLIMITED ),
		//AW_FRIENDINVITATE 친구 초대 이벤트
		ADD_HANDLER_INFO( AW_FRIENDINVITATE, AW_FRIENDINVITATE_INVITATELIST_CMD, PHASE_UNLIMITED ),
		// AW_CHAO
		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_CMD, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_SYN, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_ITEM, AW_ITEM_USE_DECREE_ITEM_SYN, PHASE_UNLIMITED),
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
        ADD_HANDLER_INFO( AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK, PHASE_UNLIMITED ),
#endif

        ADD_HANDLER_INFO( AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_ITEM, AW_OPERATION_ROOM_CTRL_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AW_CONNECTION, AW_ABUSING_USER_VERIFY_QUERY_CMD, PHASE_UNLIMITED ),
        // __KR_001355_20090608_GROUPINGSYSTEM_GW = {
        ADD_HANDLER_INFO( AW_USER_STATUS, AW_STATUS_GROUP_SET_MEMBERINFO_CMD, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( AW_USER_STATUS, AW_STATUS_GROUP_ADDEXP_CMD, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( AW_USER_STATUS, AW_STATUS_GROUP_ADDMONEY_CMD, PHASE_UNLIMITED ),
        // }
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        ADD_HANDLER_INFO( AW_USER_STATUS, AW_USER_LOTTO_REWARD_SYN, PHASE_UNLIMITED ),
      #ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        ADD_HANDLER_INFO( AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_SYN, PHASE_UNLIMITED ),
      #endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        //}
        ADD_HANDLER_INFO( AW_USER_STATUS, AW_USER_DOMINATION_RETURNHIEM_CMD, PHASE_UNLIMITED ),
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
        ADD_HANDLER_INFO(AW_PARTY, AW_PARTY_FINDING_JOIN_SYN, PHASE_UNLIMITED), 
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};


VOID	Handler_AW::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<WORLD_SERVER>( GetHandlerInfo() );
	//pInstance->RegisterHandler_<WORLD_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//


#include <ServerSessions/ServerSessionEx.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AG.h>
#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>
#include <AgentServer.h>
#include <Zone/WarControlManager.h>

#undef FINDnALIGN_USER
#undef FINDnALIGN_USER_BY_CHARGUID
#define FINDnALIGN_USER(pUserInstance, UserKey)							\
	User* pUserInstance = UserManager::Instance()->GetUser( (UserKey) );	\
	if( !pUserInstance )												\
	{																	\
		SUNLOG( eFULL_LOG, _T("[W2A] Can't Find User %u"), (UserKey) );	\
		return;															\
	}

#define FINDnALIGN_USER_BY_CHARGUID(pUserInstance, CharGuid)					\
	User* pUserInstance = UserManager::Instance()->GetUserByCharGuid( (CharGuid) );	\
	if( !pUserInstance ) return;


HANDLER_AW_IMPL(AW_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_AW_IMPL( AW_PREPARE_NEW_USER_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AW_PREPARE_NEW_USER_ACK* pRecvMsg = (MSG_AW_PREPARE_NEW_USER_ACK *)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    pUser->ChangeWorldServiceEnabledState(pRecvMsg->enabled);
    MSG_CG_PREPARE_WORLD_CONNECT_ACK msg_ack;
    msg_ack.enabled = pRecvMsg->enabled;
    pUser->SendPacket(&msg_ack, sizeof(msg_ack));
#else
    AgentServer* pAgentServer = AgentServer::GetInstance();

    MSG_CG_PREPARE_WORLD_CONNECT_ACK msgACK;

    strncpy(msgACK.szWorldServerIP, pAgentServer->GetWorldServerClientIP(), IP_MAX_LEN);
    msgACK.wWorldServerPort = pAgentServer->GetWorldServerClientPort();

    pUser->SendPacket(&msgACK, sizeof(msgACK));
#endif
}

HANDLER_AW_IMPL(AW_PREPARE_NEW_USER_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    ASSERT(!"does not support anymore");
    return;
#else
    MSG_AW_PREPARE_NEW_USER_NAK* pRecvMsg = (MSG_AW_PREPARE_NEW_USER_NAK *)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    MSG_CG_PREPARE_WORLD_CONNECT_NAK msgNAK;
    pUser->SendPacket(&msgNAK, sizeof(msgNAK));
#endif
}

HANDLER_AW_IMPL( AW_USER_RESTORE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	/*
	MSG_AW_USER_RESTORE_SYN* pRecvMsg = (MSG_AW_USER_RESTORE_SYN*)pMsg;

	// 나한테 물린 놈이 몇명이야?
	DWORD dwUserNum = UserManager::Instance()->GetNumberOfUsers();

	MSG_AW_USER_RESTORE_CMD sendMsg;

	sendMsg.m_byCategory = AW_USER_STATUS;
	sendMsg.m_byProtocol = AW_USER_RESTORE_CMD;
	sendMsg.m_wUserNum = dwUserNum;

	UserManager::Instance()->SetFirst();

	User*			pUser;

	//	SERVER_KEY	serverKey;
	//	serverKey.Set( g_AgentServer.GetWorldID(), g_AgentServer.GetChannelID(), g_AgentServer.GetServerType(), g_AgentServer.GetServerGUID() );
	//	sendMsg.m_dwKey = serverKey;
	sendMsg.m_dwKey = g_AgentServer.GetServerKey();

	DWORD dwCount = 0;

	while( pUser = reinterpret_cast<User*>(UserManager::Instance()->GetNext()) )
	{
	sendMsg.m_UserInfo[dwCount].dwAuthUserID = pUser->GetAuthID();
	sendMsg.m_UserInfo[dwCount].dwUserGUID = pUser->GetUserGUID();
	sendMsg.m_UserInfo[dwCount].dwZoneKey = pUser->GetZoneKey();
	sendMsg.m_UserInfo[dwCount].byZoneType = pUser->GetZoneType();
	if( sendMsg.m_UserInfo[dwCount].byZoneType != eZONETYPE_CHARSELECT )
	{
	sendMsg.m_UserInfo[dwCount].CharGuid = pUser->GetSelectedCharGuid();
	memset( sendMsg.m_UserInfo[dwCount].tszCharName, 0, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	memcpy( sendMsg.m_UserInfo[dwCount].tszCharName, pUser->GetSelectedCharName(), sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	}
	else
	{
	sendMsg.m_UserInfo[dwCount].CharGuid = 0;
	memset( sendMsg.m_UserInfo[dwCount].tszCharName, 0, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	}
	memcpy( sendMsg.m_UserInfo[dwCount].tszClientIP, pUser->GetClientIP(), sizeof(TCHAR)*IP_MAX_LEN );
	dwCount++;
	if( dwCount >= 50 )
	{
	pServerSession->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );
	dwCount = 0;
	}
	}

	// 혹시나 50에 딱 맞으면 끝이고 아니면 남은 놈들 보낸다
	if( dwCount != 0 )
	{
	pServerSession->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );
	}

	MessageOut( eFULL_LOG, "World Server - Restore %u User(s)", dwUserNum );
	*/
}

//AW_FRIENDINVITATE 친구 초대 이벤트, implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
HANDLER_AW_IMPL( AW_FRIENDINVITATE_INVITATELIST_CMD )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_FRIENDINVITATE_INVITATELIST_CMD* pRecvMsg = (MSG_AW_FRIENDINVITATE_INVITATELIST_CMD*)pMsg;
	if(pRecvMsg->m_InvitateFriendListInfo.m_byListCount > 0 ) 
	{
		FINDnALIGN_USER_BY_CHARGUID( pUser, pRecvMsg->m_InvitateFriendListInfo.m_dwCharGuid );

		MSG_AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD cmd;
        // (CHANGES) (f100624.2L)
        cmd.m_InvitateFriendListInfo = pRecvMsg->m_InvitateFriendListInfo;
        INVITATEFRIENDLIST_INFO& friend_list_info = cmd.m_InvitateFriendListInfo;
        assert(friend_list_info.m_byListCount <= _countof(friend_list_info.m_GuidList));
        friend_list_info.m_byListCount = \
            min(friend_list_info.m_byListCount, _countof(friend_list_info.m_GuidList));
		//memcpy(&cmd.m_InvitateFriendListInfo, &pRecvMsg->m_InvitateFriendListInfo, sizeof(INVITATEFRIENDLIST_INFO));

		pUser->SendToLinkedServer(&cmd, cmd.GetSize());

        MessageOut( eDEV_LOG, "Send SendInvitateFriendList Info - GameServer Count[%u]", pRecvMsg->m_InvitateFriendListInfo.m_byListCount );
	}
}

// 리벤지 정보 받음 
HANDLER_AW_IMPL( AW_CHAO_REQUEST_REVENGEINFO_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK* pRecvMsg = (MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK*)pMsg;
	FINDnALIGN_USER_BY_CHARGUID( pUser, pRecvMsg->m_dwCharGuid );

	// 게임서버에 리벤지 정보를 전달한다.
	MSG_AG_CHAO_REQUEST_REVENGEINFO_ACK msgACK;
	msgACK.m_byRevengePoint = pRecvMsg->m_byRevengePoint;
	msgACK.m_byRevengeStatNum = pRecvMsg->m_byRevengeStatNum;

	const INT iStartOff = (INT)offsetof(MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK, m_sRevengeUserInfo);
	const INT iArrSize = (INT)(sizeof(STRUCT_REVENGE_INFO)*msgACK.m_byRevengeStatNum);
	const INT iRecvedSize = ((INT)wSize - iStartOff);
	if( iRecvedSize != iArrSize )
	{
		SUNLOG( eCRITICAL_LOG, _T("수신된 REVENGE_INFO 값의 개수와 실제 수신값과 일치하지 않습니다 [%u : %u]"), iRecvedSize, iArrSize );
		return;
	}

	memcpy( msgACK.m_sRevengeUserInfo, pRecvMsg->m_sRevengeUserInfo, iArrSize );
	//	0이면 보낼 필요가 있는지는 모르겠지만.
	//	(WAVERIX)(080605)(BUG-FIX) 3KB 상당의 패킷을 GameServer로 전송하고 있었음.
	//pUser->SendToLinkedServer( &msgACK, sizeof(msgACK) );
	pUser->SendToLinkedServer( &msgACK, msgACK.GetSize() );
}

HANDLER_AW_IMPL( AW_CHAO_REQUEST_REVENGEINFO_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_REQUEST_REVENGEINFO_NAK* pRecvMsg = (MSG_AW_CHAO_REQUEST_REVENGEINFO_NAK*)pMsg;
	DWORD dwCharGUID = pRecvMsg->m_dwCharGuid;
	FINDnALIGN_USER_BY_CHARGUID( pUser, dwCharGUID );

	//	SUNLOG( eCRITICAL_LOG, "[AW_CHAO_REQUEST_REVENGEINFO_NAK] CharGuid[%d] ErrorCode[%d]", pRecvMsg->m_dwCharGuid, pRecvMsg->m_byErrorCode );

	// 게임서버에 전달
	MSG_AG_CHAO_REQUEST_REVENGEINFO_NAK msgNAK;
	msgNAK.m_dwCharGuid = dwCharGUID;
	msgNAK.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendToLinkedServer( &msgNAK, sizeof(msgNAK) );
}

// 리벤지 대상 삭제(클라이언트가 삭제했을 때)
HANDLER_AW_IMPL( AW_CHAO_REMOVE_REVENGE_CHAR_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_REMOVE_REVENGE_CHAR_CMD* pRecvMsg = (MSG_AW_CHAO_REMOVE_REVENGE_CHAR_CMD*)pMsg;
	FINDnALIGN_USER_BY_CHARGUID( pUser, pRecvMsg->m_dwCharGuid );

	// 게임서버에 전달
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_CMD msgCMD;
	msgCMD.m_dwRevengeCharGuid = pRecvMsg->m_dwRevengeCharGuid;
	pUser->SendToLinkedServer( &msgCMD, sizeof(msgCMD) );
}

// 리벤지 대상 한명 제거
HANDLER_AW_IMPL( AW_CHAO_REMOVE_REVENGE_CHAR_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_REMOVE_REVENGE_CHAR_ACK* pRecvMsg = (MSG_AW_CHAO_REMOVE_REVENGE_CHAR_ACK*)pMsg;
	FINDnALIGN_USER_BY_CHARGUID( pUser, pRecvMsg->m_dwCharGuid );

	// 게임서버에 전달
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_ACK msgACK;
	msgACK.m_dwRevengeCharGuid = pRecvMsg->m_dwRevengeCharGuid;
	pUser->SendToLinkedServer( &msgACK, sizeof(msgACK) );
}

HANDLER_AW_IMPL( AW_CHAO_REMOVE_REVENGE_CHAR_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK* pRecvMsg = (MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK*)pMsg;
	DWORD dwCharGUID = pRecvMsg->m_dwCharGuid;
	FINDnALIGN_USER_BY_CHARGUID( pUser, dwCharGUID );

	SUNLOG( eCRITICAL_LOG,
		_T("[AW_CHAO_REMOVE_REVENGE_CHAR_NAK] CharGuid[%d] ErrorCode[%d]"),
		dwCharGUID, pRecvMsg->m_byErrorCode );

	// 게임서버에 전달
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_NAK msgNAK;
	msgNAK.m_dwCharGuid = dwCharGUID;
	msgNAK.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendToLinkedServer( &msgNAK, sizeof(msgNAK) );
}

// 리벤지 대상 추가 성공
HANDLER_AW_IMPL( AW_CHAO_ADD_REVENGE_CHAR_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_ADD_REVENGE_CHAR_ACK* pRecvMsg = (MSG_AW_CHAO_ADD_REVENGE_CHAR_ACK*)pMsg;
	FINDnALIGN_USER_BY_CHARGUID( pUser, pRecvMsg->m_dwCharGuid );

	BYTE pBuffer[sizeof(MSG_AG_CHAO_ADD_REVENGE_CHAR_ACK)+sizeof(MSG_CG_CHAO_ADD_REVENGE_CHAR_ACK)];

	// 게임서버에 전달
	MSG_AG_CHAO_ADD_REVENGE_CHAR_ACK* pMsgACK_AG = new (pBuffer) MSG_AG_CHAO_ADD_REVENGE_CHAR_ACK;
	pMsgACK_AG->m_dwRevengeCharGuid = pRecvMsg->m_dwRevengeCharGuid;
	_tcsncpy( pMsgACK_AG->m_ptszRevengeName, pRecvMsg->m_ptszRevengeName, MAX_CHARNAME_LENGTH );
	pMsgACK_AG->m_ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
	pUser->SendToLinkedServer( pMsgACK_AG, sizeof(*pMsgACK_AG) );

	// 유저에게 전달
	MSG_CG_CHAO_ADD_REVENGE_CHAR_ACK* pMsgACK_CG = new (pBuffer) MSG_CG_CHAO_ADD_REVENGE_CHAR_ACK;
	_tcsncpy( pMsgACK_CG->m_ptszRevengeName, pRecvMsg->m_ptszRevengeName, MAX_CHARNAME_LENGTH );
	pMsgACK_CG->m_ptszRevengeName[MAX_CHARNAME_LENGTH] = '\0';
	pUser->SendPacket( pMsgACK_CG, sizeof(*pMsgACK_CG) );
}

HANDLER_AW_IMPL( AW_CHAO_ADD_REVENGE_CHAR_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK* pRecvMsg = (MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK*)pMsg;
	DWORD dwCharGUID = pRecvMsg->m_dwCharGuid;
	FINDnALIGN_USER_BY_CHARGUID( pUser, dwCharGUID );

	BYTE pBuffer[sizeof(MSG_AG_CHAO_ADD_REVENGE_CHAR_NAK)+sizeof(MSG_CG_CHAO_ADD_REVENGE_CHAR_NAK)];

	// 게임서버에 전달
	MSG_AG_CHAO_ADD_REVENGE_CHAR_NAK* pMsgNAK_AG = new (pBuffer) MSG_AG_CHAO_ADD_REVENGE_CHAR_NAK;
	pMsgNAK_AG->m_dwCharGuid = dwCharGUID;
	pMsgNAK_AG->m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendToLinkedServer( pMsgNAK_AG, sizeof(*pMsgNAK_AG) );

	// 유저에게 전달
	MSG_CG_CHAO_ADD_REVENGE_CHAR_NAK* pMsgNAK_CG = new (pBuffer) MSG_CG_CHAO_ADD_REVENGE_CHAR_NAK;
	pMsgNAK_CG->m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendPacket( pMsgNAK_CG, sizeof(*pMsgNAK_CG) );
}

HANDLER_AW_IMPL( AW_ITEM_CHANGE_SPECIAL_ITEM_SYN )
{
    __TOUCHED((pMsg, wSize));
	MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_SYN* pRecvMsg = (MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_SYN*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	// 월드서버에서 필요한 아이템정보에 대해 알려준다.
	MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD msgCMD;
	msgCMD.m_dwCharGuid = pUser->GetSelectedCharGuid();
	msgCMD.m_Count = pUser->GetItemOwnFlag().GetAllSpecialItemInfoToWorld( msgCMD.m_ItemInfo, MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD::MAX_SPECIAL_ITEM_COUNT );
	if( msgCMD.m_Count )
		pUser->SendToWorldServer( &msgCMD, sizeof(msgCMD) );
}

HANDLER_AW_IMPL( AW_ITEM_USE_DECREE_ITEM_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_ITEM_USE_DECREE_ITEM_SYN* pRecvMsg = (MSG_AW_ITEM_USE_DECREE_ITEM_SYN *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	//게임 서버에 포고문 아이템을 사용했음을 전송
	MSG_AG_ITEM_USE_DECREE_ITEM_SYN msgSYN;
	msgSYN.m_CharGuid = pRecvMsg->m_dwCharGuid;

	pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// 하임행운 캐시 응모권 사용 응답
HANDLER_AW_IMPL( AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK* msg = \
        static_cast<const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK*>(pMsg);

    User* user = UserManager::Instance()->GetUserByCharGuid(msg->char_guid);
    if (user == NULL)
    { 
        SUNLOG(eFULL_LOG, _T("[W2A] Can't Find Character %u"), msg->char_guid);
        return;
    }

    // 게임 서버로 중계
    MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
    ack.result_code = msg->result_code;
    ack.char_guid   = msg->char_guid;
    ack.index       = msg->index;
    ack.pos         = msg->pos;
    user->SendToLinkedServer(&ack, (WORD)sizeof(ack));
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

HANDLER_AW_IMPL( AW_USER_STATUS_INTERCEPTION_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_USER_STATUS_INTERCEPTION_ACK* pRecvMsg = (MSG_AW_USER_STATUS_INTERCEPTION_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AG_STATUS_INTERCEPTION_ACK ackMSG;
	ackMSG.m_dwKey				= pRecvMsg->m_dwKey;
	ackMSG.m_byInterceptionCode	= pRecvMsg->m_byInterceptionCode;
	ackMSG.m_bRequestFlag		= pRecvMsg->m_bFlag;

	pUser->SendToLinkedServer(&ackMSG, sizeof(ackMSG));
}

HANDLER_AW_IMPL( AW_USER_STATUS_INTERCEPTION_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_USER_STATUS_INTERCEPTION_NAK* pRecvMsg = (MSG_AW_USER_STATUS_INTERCEPTION_NAK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AG_STATUS_INTERCEPTION_NAK nakMSG;
	nakMSG.m_dwKey = pRecvMsg->m_dwKey;
	pUser->SendToLinkedServer(&nakMSG, sizeof(nakMSG));
}

HANDLER_AW_IMPL( AW_OPERATION_ROOM_CTRL_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv( pMsg, wSize, SSQSessionCmd::FromWorld( pServerSession ) );
}

HANDLER_AW_IMPL( AW_ABUSING_USER_VERIFY_QUERY_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AW_ABUSING_USER_VERIFY_QUERY_CMD* pRecvMsg = (MSG_AW_ABUSING_USER_VERIFY_QUERY_CMD*)pMsg;
	User* pUser = UserManager::Instance()->GetUser((pRecvMsg->m_dwKey));	
	if(pUser)
	{
		MSG_AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD msg;
		msg.m_dwKey = pRecvMsg->m_dwKey;
		msg.m_CharGuid = pRecvMsg->m_CharGuid;
		pUser->SendToLinkedServer(&msg, sizeof(msg));
		return;
	}

	SUNLOG(eCRITICAL_LOG, _T("MSG_AW_ABUSING_USER_VERIFY_QUERY_CMD(%u)(%u)"), (pRecvMsg->m_dwKey), (pRecvMsg->m_CharGuid));
}

HANDLER_AW_IMPL( AW_STATUS_GROUP_SET_MEMBERINFO_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AW_STATUS_GROUP_SET_MEMBERINFO_CMD* recv_msg = 
        static_cast<MSG_AW_STATUS_GROUP_SET_MEMBERINFO_CMD*>(pMsg);

    User* user = UserManager::Instance()->GetUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL)
    {
        return;
    }

    MSG_AG_STATUS_GROUP_SET_MEMBERINFO_CMD cmd_msg;
    {
        cmd_msg.m_CharGuid = recv_msg->m_CharGuid;
        cmd_msg.m_HighGroupInfo = recv_msg->m_HighGroupinfo;
        cmd_msg.m_LowGroupInfo = recv_msg->m_LowGroupInfo;
    }
    user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
}
HANDLER_AW_IMPL(AW_STATUS_GROUP_ADDEXP_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AW_STATUS_GROUP_ADDEXP_CMD* recv_msg = 
        static_cast<MSG_AW_STATUS_GROUP_ADDEXP_CMD*>(pMsg);

    User* user = UserManager::Instance()->GetUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL)
    {
        return;
    }

    MSG_AG_STATUS_GROUP_ADDEXP_CMD send_msg;
    {
        send_msg.m_CharGuid = recv_msg->m_CharGuid;
        send_msg.m_Amount = recv_msg->m_Amount;
        _tcsncpy(send_msg.m_szCharName, recv_msg->m_szCharName, _countof(send_msg.m_szCharName));
        send_msg.m_szCharName[_countof(send_msg.m_szCharName) - 1] = '\0';
    }
    user->SendToLinkedServer(&send_msg, sizeof(send_msg));
}
HANDLER_AW_IMPL(AW_STATUS_GROUP_ADDMONEY_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AW_STATUS_GROUP_ADDMONEY_CMD* recv_msg = 
        static_cast<MSG_AW_STATUS_GROUP_ADDMONEY_CMD*>(pMsg);

    User* user = UserManager::Instance()->GetUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL)
    {
        return;
    }
    
    MSG_AG_STATUS_GROUP_ADDMONEY_CMD send_msg;
    {
        send_msg.m_CharGuid = recv_msg->m_CharGuid;
        send_msg.m_Amount = recv_msg->m_Amount;
        _tcsncpy(send_msg.m_szCharName, recv_msg->m_szCharName, _countof(send_msg.m_szCharName));
        send_msg.m_szCharName[_countof(send_msg.m_szCharName) - 1] = '\0';
    }
    user->SendToLinkedServer(&send_msg, sizeof(send_msg));
}

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM
HANDLER_AW_IMPL( AW_USER_LOTTO_REWARD_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AW_USER_LOTTO_REWARD_SYN* pRecvMsg = (MSG_AW_USER_LOTTO_REWARD_SYN*)pMsg;

    User* pUser = UserManager::Instance()->GetUserByCharGuid(pRecvMsg->m_CharGuid);
    if (NULL == pUser)
    { 
        MSG_AW_USER_LOTTO_REWARD_ACK ack;
        ack.m_iResult  = MSG_AW_USER_LOTTO_REWARD_ACK::ERROR_CHAR_NOT_ONLINE;
        ack.m_CharGuid = pRecvMsg->m_CharGuid;
        ack.m_Prize    = pRecvMsg->m_Prize;
        pServerSession->Send((BYTE*)&ack, (WORD)sizeof(ack));
        return;
    }

    MSG_AG_STATUS_LOTTO_REWARD_SYN syn;
    syn.m_CharGuid = pRecvMsg->m_CharGuid;
    syn.m_Prize    = pRecvMsg->m_Prize;
    pUser->SendToLinkedServer(&syn, (WORD)sizeof(syn));
}
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
HANDLER_AW_IMPL( AW_USER_LOTTO_TICKET_PURCHASE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AW_USER_LOTTO_TICKET_PURCHASE_SYN* pRecvMsg = \
        static_cast<MSG_AW_USER_LOTTO_TICKET_PURCHASE_SYN*>(pMsg);

    User* pUser = UserManager::Instance()->GetUserByCharGuid(pRecvMsg->iCharGuid);
    if (NULL == pUser)
    { 
        MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK ack;
        ack.iResult = MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK::ERROR_CHAR_NOT_ONLINE;
        ack.iCharGuid = pRecvMsg->iCharGuid;
        ack.wDrawNum = pRecvMsg->wDrawNum;
        ack.bAuto = pRecvMsg->bAuto;
        for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
            ack.bPickedNum[i] = pRecvMsg->bPickedNum[i];
        }
        pServerSession->Send((BYTE*)&ack, (WORD)sizeof(ack));
        return;
    }

    MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_SYN syn;
    syn.iCharGuid = pRecvMsg->iCharGuid;
    syn.wDrawNum = pRecvMsg->wDrawNum;
    syn.bAuto = pRecvMsg->bAuto;
    for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
        syn.bPickedNum[i] = pRecvMsg->bPickedNum[i];
    }
    syn.i64TicketPrice = pRecvMsg->i64TicketPrice;
    pUser->SendToLinkedServer(&syn, (WORD)sizeof(syn));
}
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

HANDLER_AW_IMPL( AW_USER_DOMINATION_RETURNHIEM_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AW_USER_DOMINATION_RETURNHIEM_CMD* pRecvMsg = (MSG_AW_USER_DOMINATION_RETURNHIEM_CMD*)pMsg;

    User* pUser = UserManager::Instance()->GetUserByCharGuid(pRecvMsg->m_CharGuid);
    if (NULL == pUser)
    { 
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]User Not found" );
        return;
    }

    MSG_AG_USER_DOMINATION_RETURNHIEM_CMD sendMsg;
    sendMsg.m_CharGuid = pRecvMsg->m_CharGuid;
    sendMsg.m_TenderCost = pRecvMsg->m_TenderCost;
    sendMsg.m_Type = pRecvMsg->m_Type;


    pUser->SendToLinkedServer(&sendMsg, (WORD)sizeof(sendMsg));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// 파티 모집에서 파티 참가 신청이 오면 호출된다.
HANDLER_AW_IMPL(AW_PARTY_FINDING_JOIN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    __UNUSED(wSize);

    WorldServerSession* const world_server = pServerSession;

    const MSG_AW_PARTY_FINDING_JOIN_SYN* const join_syn = 
        static_cast<const MSG_AW_PARTY_FINDING_JOIN_SYN* const>(pMsg);

    // 응답 메시지를 만든다.
    MSG_AW_PARTY_FINDING_JOIN_RESULT_ACK join_result_ack;
    join_result_ack.join_wanted_user_id_ = join_syn->join_wanted_user_id_;
    join_result_ack.join_result_ = RC::PartyFindingResultUtil::GetDefaultValue();
    join_result_ack.join_wanted_party_finding_key_ = join_syn->join_wanted_party_finding_key_;
    join_result_ack.join_wanted_party_key_ = PartyFindingInfo::NULL_PARTY;
    join_result_ack.num_of_joined_people_ = 0;

    do // == try
    {
        // 파티장을 검사한다.
        User* const party_master = 
            UserManager::Instance()->GetUserByCharGuid(join_syn->join_wanted_party_master_id_);
        if (party_master == NULL)
        {
            // == throw
            join_result_ack.join_result_ = RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER;
            break;
        }

        // 파티 참가 신청자를 검사한다.
        User* const join_wanted_user = 
            UserManager::Instance()->GetUserByCharGuid(join_syn->join_wanted_user_id_);
        if (join_wanted_user == NULL)
        {
            // == throw
            join_result_ack.join_result_ = RC::PARTY_FINDING_FAIL_CANT_FIND_JOIN_WANTED_USER;
            break;
        }

        // 기존 파티 초대 로직을 사용하여 파티에 참가 시킨다.
        AgentPartyManager* const agent_party_manager = AgentPartyManager::Instance();
        if (agent_party_manager->InviteParty(party_master, join_wanted_user) != RC::RC_PARTY_SUCCESS)
        {
            // == throw
            join_result_ack.join_result_ = RC::PARTY_FINDING_FAIL_CANT_JOIN_PARTY;
            break;
        }

        // 참가한 파티를 검사한다.
        BaseParty* join_wanted_party = 
            const_cast<BaseParty*>(
                agent_party_manager->FindParty(party_master->GetPartyState().GetPartyKey()));
        if (join_wanted_party == NULL)
        {
            // == throw
            join_result_ack.join_result_ = RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY;
            break;
        }
        
        join_result_ack.join_wanted_party_key_ = join_wanted_party->GetPartyKey();
        join_result_ack.num_of_joined_people_ = 
            PartyFindingNumOfMemberConst::ConvertValue(join_wanted_party->GetMemberNum());

    } while (false); // == catch

    // 월드 서버로 전송한다.
    world_server->Send(reinterpret_cast<BYTE*>(&join_result_ack), join_result_ack.GetSize());

    // 로그를 기록한다.
    const BYTE log_level = (join_result_ack.join_result_ == RC::PARTY_FINDING_SUCCESS) ? eFULL_LOG : eCRITICAL_LOG;
    MessageOut(log_level, 
               _T("|PartyFinding|%s|Result:%d|JoinWantedPartyFindingKey:%u|JoinWantedPartyKey:%u|NumOfJoinedPeople:%u|JoinWantedUserID:%u|"), 
               __FUNCTION__, 
               join_result_ack.join_result_, 
               join_result_ack.join_wanted_party_finding_key_,                    
               join_result_ack.join_wanted_party_key_, 
               join_result_ack.num_of_joined_people_, 
               join_result_ack.join_wanted_user_id_);
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
