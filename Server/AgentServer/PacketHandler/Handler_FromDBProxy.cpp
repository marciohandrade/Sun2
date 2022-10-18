#include "stdafx.h"
#include "Handler_FromDBProxy.h"
#include "../SolarShop/ShopDefine.h" // (WAVERIX) (090816) (NOTE) 적절히 수정할 것.

#include "PacketHandler.Shared.h"

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
    #include "Function/GameOptionManager.h"
#endif //_NA_0_20110329_GAMEOPTION_SETTING
#include <Function/GuildRanking/GuildRanking.h>

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_AD::On##p }


sPACKET_HANDLER_INFO*	Handler_AD::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
        ADD_HANDLER_INFO(AD_CONNECTION, AD_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AD_CONNECTION, AD_CONNECTION_DBCONNECT_UPDATE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_CREATE_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_CREATE_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_DESTROY_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_DESTROY_NAK, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_RECOVER_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_RECOVER_NAK, PHASE_UNLIMITED ),
    #ifdef __NA_001044_ADD_CUSTOM_UI
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARINFO_CHECK_CHARACTERID_NAK, PHASE_UNLIMITED ),
    #endif
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_QUERY_CHARINFO_ACK, PHASE_UNLIMITED ),
    #endif
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        ADD_HANDLER_INFO( AD_EVENT, AD_EVENT_INVITATIONR_INSERT_CMD, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( AD_EVENT, AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK, PHASE_UNLIMITED ),
        //}
    #ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARNAME_CHANGE_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( AD_CHARINFO, AD_CHARNAME_CHANGE_NAK, PHASE_UNLIMITED ),
    #endif

    #ifdef _NA_0_20110329_GAMEOPTION_SETTING
        ADD_HANDLER_INFO( AD_ETC, AD_ETC_GAMEOPTION_SELECT_ACK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO( AD_ETC, AD_ETC_GAMEOPTION_SELECT_NAK, PHASE_UNLIMITED ),
        ADD_HANDLER_INFO(AD_ETC, AD_ETC_GAMEOPTION_UPDATE_ACK, PHASE_UNLIMITED),
    #endif //_NA_0_20110329_GAMEOPTION_SETTING
    #ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        ADD_HANDLER_INFO(AD_ETC, AD_ETC_GUILD_RANKINGINFO_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AD_ETC, AD_ETC_GUILDINFO_ACK, PHASE_UNLIMITED),
    #endif //_NA004034_20120102_GUILD_POINT_SYSTEM

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_AD::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<GAME_DBPROXY>( GetHandlerInfo() );
	//pInstance->RegisterHandler_<GAME_DBPROXY>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//


#include <PacketStruct_CG.h>
#include <PacketStruct_AD.h>

#include <AgentServer.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>

#undef FINDnALIGN_USER
#define FINDnALIGN_USER(pUserInstance, UserKey)							\
	User* pUserInstance = UserManager::Instance()->GetUser( (UserKey) );	\
	if( !pUserInstance )												\
	{																	\
		SUNLOG( eFULL_LOG, _T("[D2A] Can't Find User %u"), (UserKey) );	\
		return;															\
	}

/*							SUNLOG( eFULL_LOG,  "없는 유저에게 접속해제 명렁 패킷을 보내려한다.[ToID:%d]\n", id );	\ */

HANDLER_AD_IMPL(AD_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_AD_IMPL(AD_CONNECTION_DBCONNECT_UPDATE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    // first received protocol ...
    const MSG_AD_CONNECTION_DBCONNECT_UPDATE_CMD* recv_msg = \
        static_cast<MSG_AD_CONNECTION_DBCONNECT_UPDATE_CMD*>(pMsg);
    //
    AgentServer* agent_server = AgentServer::GetInstance();
    agent_server->ChangeDBConnectorIndex(recv_msg->db_connector_index_);
    //
    MSG_AG_CONNECTION_DBCONNECT_UPDATE_CMD msg_cmd;
    msg_cmd.db_connector_index_ = recv_msg->db_connector_index_;
    agent_server->SendToGameServer(&msg_cmd, sizeof(msg_cmd));
}

// 굳이 모든 정보를 Agent의 User에 저장할 필요가 없지 않을까?
// 우선 모든 정보를 가지고 있는다! 혹시 나중에 필요한 정보가 있을지 모른다.
HANDLER_AD_IMPL( AD_CHARINFO_CHARLISTREQ_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_CHARLISTREQ_ACK* pRecvMsg = (MSG_AD_CHARINFO_CHARLISTREQ_ACK*)pMsg;

	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	ASSERT( pUser->IsBeginTransaction() );

	//-------------------------------------------------------------------------------------------
	// <WAVERIX>
	//   서버 열리고 첫 유저 접속시... MSG_CG_CONNECTION_ENTERSERVER_ACK을 정상적으로 받지 못하는
	//   현상 방지용.  패킷 전송 순서 맞추기 위함. 임시코드
	{
		static BOOL bInit = FALSE;
//		if( !bInit )
		{
			MSG_CG_CONNECTION_HEARTBEAT_SYN msgHB;
			pUser->SendPacket( &msgHB, sizeof(MSG_CG_CONNECTION_HEARTBEAT_SYN) );
			bInit = TRUE;
		}
	};
    ;{  // to support a client/server time synchronization
        MSG_CG_CONNECTION_TIME_SYNC_CMD msg_cmd;
        ZeroMemory(&msg_cmd.sync_node, sizeof(msg_cmd.sync_node));
        util::TimeSync::Command_ServerSend(&msg_cmd.sync_node);
        pUser->SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    ;{
        MSG_AW_PREPARE_NEW_USER_SYN msg_syn;
        msg_syn.m_dwKey      = pUser->GetUserKey();
        msg_syn.dwAuthUserID = pUser->GetAuthID();
        msg_syn.dwUserGuid   = pUser->GetUserGUID();
        ;{
            util::StrArrayForUserID& dest = msg_syn.ptszAccountID;
            _tcsncpy(dest, pUser->GetUserID(), _countof(dest));
            dest[_countof(dest) - 1] = _T('\0');
        };
        ;{
            ::strncpy(msg_syn.szClientIP, pUser->GetClientIP(), _countof(msg_syn.szClientIP));
            msg_syn.szClientIP[_countof(msg_syn.szClientIP) - 1] = '\0';
        };
        msg_syn.dwPCRoomID = pUser->GetReservedValue();
        msg_syn.byGMGrade  = pUser->GetSts();

        pUser->SendToWorldServer(&msg_syn, sizeof(msg_syn));
    };
#endif
	//-------------------------------------------------------------------------------------------

	static SERVER_CHARACTER_PART pCharPartOnRecv[MAX_CHARACTER_LIST_NUM];
	static CLIENT_CHARACTER_PART pCharacterListForSend[MAX_CHARACTER_LIST_NUM];

	pRecvMsg->ConvertForRecv( pCharPartOnRecv );


	BYTE byListNum = pRecvMsg->m_byCount;
	pUser->SetNumberOfCreatedChars( byListNum );

	MSG_CG_CONNECTION_ENTERSERVER_ACK msgACK;
	msgACK.m_bPVP = AgentServer::GetInstance()->IsPVPMode();
	msgACK.m_byCount = byListNum;
#ifdef _NA_002050_20110216_ADD_GENDER
    msgACK.char_slot_size = pRecvMsg->char_slot_size;
#endif
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    memset(msgACK.m_ReNameStatus, 0, sizeof(msgACK.m_ReNameStatus));
#endif
    
	//BYTE Count=0;
	for( BYTE i = 0  ; i < byListNum ; ++i )
	{
		SERVER_CHARACTER_PART* pSCharacterPart = &pCharPartOnRecv[i];
		BYTE bySlotIndex = pSCharacterPart->m_bySlot;
		pUser->SerializeCharacterPart( bySlotIndex, *pSCharacterPart, SERIALIZE_STORE );
        pUser->SetOccupiedSlot(bySlotIndex, true);
        // NOTE: f110517.2L, apply changed specifiaction
        pUser->LoadClientCharacterPart(bySlotIndex, &pCharacterListForSend[i]);
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        msgACK.m_ReNameStatus[i] = pRecvMsg->m_ReNameStatus[i];
#endif
	}
	//---------------------------------------------------------------------------------------------
	// 상태 설정
	//---------------------------------------------------------------------------------------------
	pUser->SetState( CHARACTER_LIST_STATE );
	// client로 정보 전달
	pUser->SendPacket( &msgACK, msgACK.ConvertForSend( pCharacterListForSend ) );

	pUser->EndTransaction();

	//__WAVERIX_HEARTBEAT_CONTROL__
    pUser->GetSecureNode().EnableHeartbeats(true);

	{	//	__NA000630_070817_MOLA_5th_UPDATE
		//	1) 이 시점엔 CRYPTED USER LIST에 소속되어 있지만, INIT상태이다.
		//	2) 필드로 입장하기 전까지는 TRANSACTION을 설정할 필요 있다.
		SecurePackNode4User& rSECURE = pUser->GetSecureNode();
		sSECURE_FLAG& rSECURE_FLAG = rSECURE.SECURE_FLAG();
		rSECURE_FLAG.STATUS.SetStatus( sSECURE_FLAG::STAT_CRYPTED );
		rSECURE_FLAG.STATUS.TransEnter();
	}

	MSG_AD_CHARINFO_ADDIN_NTF msgNTF;
	msgNTF.m_dwKey	= pUser->GetUserKey();
	msgNTF.m_EncodedSSN.CopyFrom((char*)pUser->GetLastSSNKey());
	AgentServer::GetInstance()->SendToDBPServer( &msgNTF, sizeof(msgNTF) );		
    pUser->ChangeHeIsHereNotificationTimer(true); // NOTE: f110622.2L
}

HANDLER_AD_IMPL( AD_CHARINFO_CHARLISTREQ_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_CHARLISTREQ_NAK* pRecvMsg = (MSG_AD_CHARINFO_CHARLISTREQ_NAK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	MSG_CG_CONNECTION_ENTERSERVER_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendPacket( &msgNAK, sizeof(msgNAK) );

	// 캐릭터 리스트를 받아 오지 못하면 끊어버린다.
	SUNLOG( eCRITICAL_LOG, _T("[AD_CHARINFO_CHARLISTREQ_NAK] [U:%d] Disconnect User (Can't Receive CharacterList)"), pRecvMsg->m_dwKey );

	pUser->EndTransaction();
	pUser->SetDisconnectCode( RC::RC_DISCONNECT_CANT_RECEIVE_CHAR_LIST_FROM_DBP );
	pUser->DisconnectUser();
}

HANDLER_AD_IMPL( AD_CHARINFO_CREATE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_CREATE_ACK* pRecvMsg = (MSG_AD_CHARINFO_CREATE_ACK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	static SERVER_CHARACTER_PART rServerCharPartForRecv;
	static CLIENT_CHARACTER_PART rClientCharInfoForSend;
	pRecvMsg->ConvertForRecv( rServerCharPartForRecv );

	ASSERT( pUser->GetNumberOfCreatedChars() < MAX_CHARACTER_SLOT_NUM );

	pUser->SetNumberOfCreatedChars( pUser->GetNumberOfCreatedChars() + 1 );
	pUser->SerializeCharacterPart( rServerCharPartForRecv.m_bySlot, rServerCharPartForRecv, SERIALIZE_STORE );
    pUser->SetOccupiedSlot(rServerCharPartForRecv.m_bySlot, true);
	pUser->EndTransaction();

	MSG_CG_CHARINFO_CREATE_ACK msgACK;
    // NOTE: f110517.2L, apply changed specifiaction
    pUser->LoadClientCharacterPart(rServerCharPartForRecv.m_bySlot, &rClientCharInfoForSend);

	//캐릭터 생성 로그파일
#ifdef _NA_002050_20110216_ADD_GENDER
    TCHAR log_add_info[MAX_ADDINFO_SIZE];
    const int num_of_written = ::_sntprintf(log_add_info, _countof(log_add_info),
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
        _T("Gender:%u,Class:%u"), rServerCharPartForRecv.gender, rServerCharPartForRecv.m_byClass);
#else  // _NA_007750_20141113_WZDAT_MODIFY_LOG
        _T("Gender:%u"), rServerCharPartForRecv.gender);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
    assert(num_of_written > 0 && num_of_written < _countof(log_add_info));
    log_add_info[num_of_written] = _T('\0');
    GAMELOG->CreateCharacter(true, pUser, rServerCharPartForRecv.m_tszCharName, log_add_info);
#else
    GAMELOG->CreateCharacter(true, pUser, rServerCharPartForRecv.m_tszCharName, NULL); 
#endif
	
	pUser->SendPacket( &msgACK, msgACK.ConvertForSend( rClientCharInfoForSend ) );
}

HANDLER_AD_IMPL( AD_CHARINFO_CREATE_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_CREATE_NAK * pRecvMsg = (MSG_AD_CHARINFO_CREATE_NAK *)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	pUser->EndTransaction();

	MSG_CG_CHARINFO_CREATE_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket( &msgNAK, sizeof(msgNAK) );
}

HANDLER_AD_IMPL( AD_CHARINFO_DESTROY_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_DESTROY_ACK* pRecvMsg = (MSG_AD_CHARINFO_DESTROY_ACK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	ASSERT( pUser->GetNumberOfCreatedChars() > 0 );

	// agent dbuser리팩토링 하기전까지 looping
    BYTE i = 0;
	for(; i < MAX_CHARACTER_SLOT_NUM ; ++i )
	{
		if( !pUser->IsValidSlot(i) ) continue;

		if( pRecvMsg->m_CharGuid == pUser->GetCharGUID(i) )
		{
			BYTE byDeletedSlotIndex = i;
			//캐릭터 삭제 로그파일
			GAMELOG->CreateCharacter(false, pUser, pUser->GetCharName(i), NULL);

			if( pUser->GetCharLV(byDeletedSlotIndex) >= CHAR_DESTROY_LEVEL )
				pUser->SetCharDelChk( byDeletedSlotIndex, 3 );
			else
			{
                pUser->SetOccupiedSlot(byDeletedSlotIndex, false);
				pUser->SetNumberOfCreatedChars( pUser->GetNumberOfCreatedChars() - 1 );
			}
			break;
		}
	}

	ASSERT( i != MAX_CHARACTER_SLOT_NUM );

	pUser->EndTransaction();

	MSG_CG_CHARINFO_DESTROY_ACK msgACK;
	pUser->SendPacket( &msgACK, sizeof(msgACK) );
}

HANDLER_AD_IMPL( AD_CHARINFO_DESTROY_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_DESTROY_NAK* pRecvMsg = (MSG_AD_CHARINFO_DESTROY_NAK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	pUser->EndTransaction();

	MSG_CG_CHARINFO_DESTROY_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket( &msgNAK, sizeof(msgNAK) );
}

HANDLER_AD_IMPL( AD_CHARINFO_RECOVER_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_RECOVER_ACK* pRecvMsg = (MSG_AD_CHARINFO_RECOVER_ACK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	// agent dbuser리팩토링 하기전까지 looping
	for( BYTE i = 0 ; i < MAX_CHARACTER_SLOT_NUM ; ++i )
	{
		if( !pUser->IsValidSlot(i) ) continue;

		if( pRecvMsg->m_CharGuid == pUser->GetCharGUID(i) )
		{
			BYTE byDeletedSlotIndex = i;
			//캐릭터 삭제 로그파일
			GAMELOG->CreateCharacter(true, pUser, pUser->GetCharName(i), NULL);
			pUser->SetCharDelChk( byDeletedSlotIndex, 0 );
		}
	}

	MSG_CG_CHARINFO_RECOVER_ACK msgACK;
	pUser->SendPacket( &msgACK, sizeof(msgACK) );

	pUser->EndTransaction();
}

HANDLER_AD_IMPL( AD_CHARINFO_RECOVER_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_RECOVER_NAK* pRecvMsg = (MSG_AD_CHARINFO_RECOVER_NAK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	MSG_CG_CHARINFO_RECOVER_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket( &msgNAK, sizeof(msgNAK) );

	pUser->EndTransaction();
}

#ifdef __NA_001044_ADD_CUSTOM_UI
HANDLER_AD_IMPL( AD_CHARINFO_CHECK_CHARACTERID_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_CHECK_CHARACTERID_ACK* pRecvMsg = (MSG_AD_CHARINFO_CHECK_CHARACTERID_ACK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	MSG_CG_CHARINFO_CHARACTERID_CHECK_ACK msgNAK;
	pUser->SendPacket( &msgNAK, sizeof(msgNAK) );
	pUser->EndTransaction();
}
HANDLER_AD_IMPL( AD_CHARINFO_CHECK_CHARACTERID_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AD_CHARINFO_CHECK_CHARACTERID_NAK* pRecvMsg = (MSG_AD_CHARINFO_CHECK_CHARACTERID_NAK*)pMsg;
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwKey );

	MSG_CG_CHARINFO_CHARACTERID_CHECK_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket( &msgNAK, sizeof(msgNAK) );
	pUser->EndTransaction();
}
#endif

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
HANDLER_AD_IMPL( SERVERCOMMON_QUERY_CHARINFO_ACK )
{
#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_SERVERCOMMON_QUERY_CHARINFO_ACK* pRecvMsg =
        static_cast<MSG_SERVERCOMMON_QUERY_CHARINFO_ACK*>(pMsg);
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    BOOLEAN success = true;
    BYTE error_code = RC::RC_SHOP_SUCCESS;

    ShopUserQuerySlot& query_slot = pUser->GetShopUserQuerySlot();

    BOOLEAN query_transaction = query_slot.IsTransaction();

    if(success && query_transaction == 0)
        success = false, error_code = RC::RC_SHOP_FAILED;

    if(success && pRecvMsg->result_ == 0)
    {
        success = false, error_code = RC::RC_SHOP_USERISNOTFOUND;
    }

#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK
    if(success && pRecvMsg->result_ == 2)
    {
        success = false, error_code = RC::RC_SHOP_NOT_FRIEND_ME;
    }

    if(success && pRecvMsg->result_ == 3)
    {
        success = false, error_code = RC::RC_SHOP_NOT_FRINED_YOUR;
    }
#endif

    if(success) {
        const MSG_CG_SHOP_SEND_GIFT_SYN* pReqMsg = query_slot.GetMessage();
        assert(pReqMsg);
        if(success && _strnicmp(pReqMsg->m_tszCharName,
                                pRecvMsg->user_info_.char_name_,
                                _countof(pReqMsg->m_tszCharName)) != 0)
        {
            success = false, error_code = RC::RC_SHOP_FAILED;
        }
    }

    if(success) {
        MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN msgSYN;
        msgSYN.Clear();
        msgSYN.action_type_ = eCHARNAME_ACTION_SHOP_SEND_GIFT;
        msgSYN.user_info_ = pRecvMsg->user_info_;

        AgentServer::GetInstance()->SendToAuthAgentServer((BYTE*)&msgSYN, msgSYN.GetSize());
    } else {
        MSG_CG_SHOP_SEND_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = error_code;
        pUser->SendPacket(&msgNAK, sizeof(msgNAK));
        query_slot.QueryEnd();
    }
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
}
#endif

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
HANDLER_AD_IMPL( AD_EVENT_INVITATIONR_INSERT_CMD )
{   
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AD_EVENT_INVITATIONR_INSERT_CMD* pRecvMsg = static_cast<MSG_AD_EVENT_INVITATIONR_INSERT_CMD*>(pMsg);
    
    User* pUser;

    for(int i = 0 ; i < pRecvMsg->m_CharGuidCount ; i++)
    {        
        pUser = UserManager::Instance()->GetUserByCharGuid( (pRecvMsg->InvitateInfo[i].dwGuid) );

        if( pUser )
        {
            MSG_AW_FRIEND_INVITATIONR_INSERT_CMD msg;
            memcpy(&msg.InvitateInfo, &pRecvMsg->InvitateInfo[i], sizeof(STRUCT_FRIEND_INVITATE_INFO));

            SUNLOG(eCRITICAL_LOG, "AD_EVENT_INVITATIONR_INSERT_CMD Send");
            pUser->SendToWorldServer((MSG_BASE*)&msg, sizeof(MSG_AW_FRIEND_INVITATIONR_INSERT_CMD));
        }
    }
}

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
HANDLER_AD_IMPL( AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK* pRecvMsg = (MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK*)pMsg;
    MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK msg;

    User* pUser = UserManager::Instance()->GetUserByCharGuid( pRecvMsg->CharGUID );

    if(pUser)
    {
        msg.byErrorCode = MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK::DB_FAIL;
        pUser->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "AD_EVENT_INVITATEFRIEND_REWARD_LIST_NAK Not Found User(%u)", pRecvMsg->CharGUID);
    }

}

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
HANDLER_AD_IMPL( AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK* pRecvMsg = (MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK*)pMsg;

    MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_ACK msg;
    

    User* pUser = UserManager::Instance()->GetUserByCharGuid( pRecvMsg->CharGUID );
    
    if(pUser)
    {
        msg.m_Count = pRecvMsg->Count;
        memcpy(msg.RewardInfo , pRecvMsg->RewardInfo, sizeof(InvitateFriendRewardInfo) * pRecvMsg->Count);
        pUser->SendPacket(&msg, msg.GetSize());
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "AD_EVENT_INVITATEFRIEND_REWARD_LIST_ACK Not Found User(%u)", pRecvMsg->CharGUID);
    }
}

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
HANDLER_AD_IMPL( AD_CHARNAME_CHANGE_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AD_CHARNAME_CHANGE_ACK* const recv_msg = 
        static_cast<MSG_AD_CHARNAME_CHANGE_ACK*>(pMsg);

    FINDnALIGN_USER( user, recv_msg->m_dwKey );

    //!~ 캐릭명 변경 로그 추가 - arycoat 2013.02.04
    GAMELOG->WriteCharRename(user, recv_msg->slot_index_, recv_msg->character_name_);
    //~!

    //!~ 캐릭명 변경 후 파티 초대 안되는 버그 수정 - arycoat 2013.01.02
    user->SetCharName(recv_msg->slot_index_, recv_msg->character_name_);
    user->SetOccupiedSlot(recv_msg->slot_index_, true);
    //~!

    MSG_CG_CHARNAME_CHANGE_ACK msg;
    
    strncpy(msg.character_name_, recv_msg->character_name_, MAX_CHARNAME_LENGTH+1);
    msg.character_name_[MAX_CHARNAME_LENGTH] = '\0';
    msg.slot_index_     = recv_msg->slot_index_;

    user->SendPacket(&msg, sizeof(msg));    
}

HANDLER_AD_IMPL( AD_CHARNAME_CHANGE_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AD_CHARNAME_CHANGE_NAK* const recv_msg = 
        static_cast<MSG_AD_CHARNAME_CHANGE_NAK*>(pMsg);

    FINDnALIGN_USER( user, recv_msg->m_dwKey );

    MSG_CG_CHARNAME_CHANGE_NAK msg;

    msg.result_     = (MSG_CG_CHARNAME_CHANGE_NAK::ERROR_CODE)recv_msg->result_;
    msg.slot_index_ = recv_msg->slot_index_;

    user->SendPacket(&msg, sizeof(msg));


}
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE


#ifdef _NA_0_20110329_GAMEOPTION_SETTING
HANDLER_AD_IMPL( AD_ETC_GAMEOPTION_SELECT_ACK )
{
    MSG_AD_ETC_GAMEOPTION_SELECT_ACK* const recv_message = 
        static_cast<MSG_AD_ETC_GAMEOPTION_SELECT_ACK*>(pMsg);
    
    FINDnALIGN_USER( user, recv_message->m_dwKey );
    GameOptionManager::Instance()->Select_Ack(user, recv_message);
}

HANDLER_AD_IMPL( AD_ETC_GAMEOPTION_SELECT_NAK )
{
    MSG_AD_ETC_GAMEOPTION_SELECT_NAK* const recv_message = 
        static_cast<MSG_AD_ETC_GAMEOPTION_SELECT_NAK*>(pMsg);

    FINDnALIGN_USER( user, recv_message->m_dwKey );
    GameOptionManager::Instance()->Select_Nak(user, recv_message);
}

HANDLER_AD_IMPL(AD_ETC_GAMEOPTION_UPDATE_ACK)
{
    const MSG_AD_ETC_GAMEOPTION_UPDATE_ACK* const update_ack = 
        static_cast<MSG_AD_ETC_GAMEOPTION_UPDATE_ACK*>(pMsg);
    GameOptionManager::Instance()->HandleUpdateAck(update_ack);
}
#endif //_NA_0_20110329_GAMEOPTION_SETTING

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
HANDLER_AD_IMPL(AD_ETC_GUILD_RANKINGINFO_ACK)
{
    MSG_AD_ETC_GUILD_RANKINGINFO_ACK* packet = \
        static_cast<MSG_AD_ETC_GUILD_RANKINGINFO_ACK*>(pMsg);

    g_GuildRankingSystem.SerializeGuildRanking( \
        packet->guild_ranking_info_, \
        packet->guild_ranking_info_count_, \
        SERIALIZE_LOAD);
}

HANDLER_AD_IMPL(AD_ETC_GUILDINFO_ACK)
{
    MSG_AD_ETC_GUILDINFO_ACK* packet = \
        static_cast<MSG_AD_ETC_GUILDINFO_ACK*>(pMsg);

    FINDnALIGN_USER( user, packet->m_dwKey );
    
    MSG_CG_ETC_GUILDINFO_ACK msg;
    msg.init();
    msg.guid_ = packet->guid_;
    memcpy(&msg.game_guild_info_, &packet->game_guild_info_, sizeof(packet->game_guild_info_));
    memcpy(&msg.guild_info_extra_, &packet->guild_info_extra_, sizeof(packet->guild_info_extra_));

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    msg.request_state_ = packet->request_state_;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    user->SendPacket(&msg, sizeof(msg));
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM