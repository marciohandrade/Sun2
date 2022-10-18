#include "stdafx.h"
#include "Handler_FromGameServer.h"

#include <PacketStruct_AG.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_AZ.h>
#include <PacketControl/PacketControl.h>
#include <MapInfoParser.h>

#include "../SolarQuest/QuestPacket.h" // SolarQuest

//////////////////////////////////////////////////////////////////////////
#include "PacketHandler.Shared.h"
#include <AgentServer.h>
#include <ServerSessions/ServerSessionEx.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#include <Zone/HuntingRoom.h>
#include <Zone/MissionRoom.h>
#include <Zone/CharSelect.h>
#include <Zone/Village.h>
#include <Zone/LobbyRoom.h>
#include <Zone/ZoneManager.h>
#include <Zone/CHuntingRoom.h>
#include <Zone/InstanceDungeon.h>
#include <Zone/DungeonTopOfTrial.h>
#include <Zone/DungeonSSQ.h>
#include <Zone/PortalManager.h>

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>
#include <AgentParty/AgentPartyOperator.h>
//_NA_20110630_BATTLE_ZONE_RENEWAL
#include <Party/PartyRoom.h>
#include <AgentParty/PartyRoomList.h>


#include <Zone/WarControlManager.h>
#include <Zone/DominationField.h>
#include <DominationInfoParser.h>
#include <Function/GuildRanking/GuildRanking.h>

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include <ChaosZoneSystem/TeamBattleGroundManager.h>
#include <ChaosZoneSystem/TeamBattleGroundMatchingSystem.h>
#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "ChaosZoneSystem/SonnenScheinManager.h"
#include "PacketStruct_AG_ChaosZoneSystem.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include "ChaosZoneSystem/OneDaysGuildMissionManager.h"
#include "PacketStruct_AG_ChaosZoneSystem.h"
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO(c, p, accptablePHASE)		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_AG::On##p }

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_SYNC()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_SYNC, AG_SYNC_PLAYER_ENTER_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_SYNC, AG_SYNC_PLAYER_ENTER_NAK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_SYNC, AG_SYNC_PLAYER_WARP_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_SYNC, AG_SYNC_PLAYER_WARP_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_ZONE()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_ROOM_JOIN_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_ROOM_JOIN_NAK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_ROOM_LEAVE_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_ROOM_LEAVE_NAK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_MISSION_LEAVE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CHUNTING_LEAVE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CHUNTING_COMPETITION_END_CMD, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_PORTAL_FIELD_MOVE_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_PORTAL_FIELD_MOVE_NAK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_RETURN_VILLAGE_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_RETURN_VILLAGE_NAK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_RETURN_TO_ROOM_INFO_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK, PHASE_UNLIMITED),

        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_SSQ_JOIN_INFO_ACK,	PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_SSQ_JOIN_ACK,		PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_SSQ_ROOM_CTRL_CMD,	PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_SSQ_ROOM_INFO_BRD,	PHASE_UNLIMITED),

        ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_JOIN_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_JOIN_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_LEAVE_BY_GUILDRELATION_SYN, PHASE_UNLIMITED),
        //__NA001336_090514_PARTY_NAVI_SUPPORT__
        ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_NAVI_INFO_BRD, PHASE_UNLIMITED),
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_CAN_CHANGE_OPTION_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_CAN_CREATE_ROOM_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_READY_STATE_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_PARTY, AG_PARTY_CAN_JOIN_ROOM_ACK, PHASE_UNLIMITED),

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        ADD_HANDLER_INFO(AG_PARTY, AG_CHAOS_ZONE_PACKET, PHASE_UNLIMITED),
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        //{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_QUEST_PORTAL_REQUEST_SYN,    PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_QUEST_PORTAL_ENTER_ACK,    PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_QUEST_PORTAL_CONTROL_CMD,    PHASE_UNLIMITED),
        //}
        //{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_LOBBY_CREATE_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_LOBBY_JOIN_ACK, PHASE_UNLIMITED),
        //}
        //_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK, PHASE_UNLIMITED), 

        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_DOMINATION_FIELD_INFO_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_DOMINATION_FIELD_INFO_NAK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_DOMINATION_REQ_KICK_CMD, PHASE_UNLIMITED),
    //_NA_20110630_BATTLE_ZONE_RENEWAL
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK, PHASE_UNLIMITED),

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        ADD_HANDLER_INFO(AG_ZONE, AG_ZONE_DOMINATION_NOTIFIER_BRD, PHASE_UNLIMITED),
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        ADD_HANDLER_INFO( AG_ZONE, AG_ZONE_SPA_ENTER_ACK, PHASE_UNLIMITED),

        ADD_HANDLER_INFO( AG_ZONE, AG_ZONE_SPA_LEAVE_ACK, PHASE_UNLIMITED),
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_STATUS()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_LEVEL_UP_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_MAXHP_CHANGE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_HP_CHANGE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_DEAD_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_RESURRECTION_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_MP_CHANGE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_MAXMP_CHANGE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_CHANGE_STATE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_PLAYING_TUTORIAL_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_INTERCEPTION_SYN, PHASE_UNLIMITED),
        //__NA001336_090514_PARTY_NAVI_SUPPORT__
        ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_CHAR_STAT_CHANGE_CMD, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_GROUP_EXP_ACQUISITION_CMD, PHASE_UNLIMITED),
        //__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
        ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_HEIM_ACQUISITION_NTF, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_HEIM_CONSUMPTION_NTF, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_LOTTO_REWARD_ACK, PHASE_UNLIMITED),
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        ADD_HANDLER_INFO(AG_STATUS, AG_STATUS_LOTTO_TICKET_PURCHASE_ACK, PHASE_UNLIMITED),
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        //
        ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_GUILD()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_GUILD, AG_GUILD_CREATE_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GUILD, AG_GUILD_DESTROY_ACK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_GUILD, AG_GUILD_JOIN_SUCCESS_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GUILD, AG_GUILD_WITHDRAW_ACK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(AG_GUILD, AG_GUILD_KICKPLAYER_ACK, PHASE_UNLIMITED),

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_MISSION()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_MISSION, AG_MISSION_STREAM_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_MISSION, AG_MISSION_CLEAR_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_GM()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_GM, AG_GM_WHISPER_SET_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_USERCHAT_BLOCK_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_CHAR_INFO_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_CHAR_INFO_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_ROOM_INFO_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_CHANNEL_INFO_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_CHANGE_ROOMTITLE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_FORCE_DISCONNECT_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_SERVER_SHUTDOWN_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_GO_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_TRACKING_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_GM, AG_GM_NOTICE_CMD, PHASE_UNLIMITED),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        ADD_HANDLER_INFO(AG_GM, AG_GM_LOTTO_NUM_CMD, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_GM, AG_GM_LOTTO_SETTLE_CMD, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_GM, AG_GM_LOTTO_FUND_CMD, PHASE_UNLIMITED),
        //}
        ADD_HANDLER_INFO(AG_GM, AG_GM_DOMINATION_INIT_CMD, PHASE_UNLIMITED),
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        ADD_HANDLER_INFO(AG_GM, AG_GM_GUILD_RANKING_REFRESH, PHASE_UNLIMITED),
#endif // _NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        ADD_HANDLER_INFO(AG_GM, AG_GM_BATTLE_GROUND_CMD, PHASE_UNLIMITED),
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#ifdef _NA_008334_20150608_SONNENSCHEIN
        ADD_HANDLER_INFO(AG_GM, AG_GM_SONNENSCHEIN_CMD, PHASE_UNLIMITED),
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
		ADD_HANDLER_INFO(AG_GM, AG_GM_ONEDAYSGUILDMISSION_CMD, PHASE_UNLIMITED),
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

        //
        ADD_HANDLER_INFO_EOR(),
	};

    return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_CHUNTING()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_CHUNTING, AG_CHUNTING_WIN_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CHUNTING, AG_CHUNTING_START_COMPETITION_CMD, PHASE_UNLIMITED),

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_CHAO()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		// AG_CHAO
		ADD_HANDLER_INFO(AG_CHAO, AG_CHAO_REQUEST_REVENGEINFO_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CHAO, AG_CHAO_REMOVE_REVENGE_CHAR_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CHAO, AG_CHAO_ADD_REVENGE_CHAR_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CHAO, AG_CHAO_CHANGE_CHAO_STATE_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo_AG_ITEM()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_ITEM, AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ITEM, AG_ITEM_USE_DECREE_ITEM_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_ITEM, AG_ITEM_USE_DECREE_ITEM_NAK, PHASE_UNLIMITED),
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
		ADD_HANDLER_INFO(AG_ITEM, AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN, PHASE_UNLIMITED),
#endif

        ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

sPACKET_HANDLER_INFO*	Handler_AG::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(AG_CHARINFO, AG_CHARINFO_CHAR_CMD, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_UNREGISTER_ACK, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_UNREGISTER_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_UNREGISTER_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_DISCONNECT_SERVER_CMD, PHASE_UNLIMITED),
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        ADD_HANDLER_INFO(AG_CONNECTION, AG_CONNECTION_SERVER_SELECT_CONTROL_ACK, PHASE_UNLIMITED),
    #endif
		ADD_HANDLER_INFO(AG_EVENT, AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(AG_CHARINFO, AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD, PHASE_UNLIMITED),
        //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
        ADD_HANDLER_INFO(AG_EVENT, AG_EVENT_SYSTEMMEMO_SEND_CMD, PHASE_UNLIMITED),
    #ifdef _NA_003027_20111013_HONOR_SYSTEM
        ADD_HANDLER_INFO(AG_EVENT, AG_EVENT_TODAYQUEST_INFO_BRD, PHASE_UNLIMITED),
    #endif //_NA_003027_20111013_HONOR_SYSTEM
    #ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
        ADD_HANDLER_INFO(AG_CHARINFO, AG_CHARINFO_PREMIUMSERVICE_BILL_CMD, PHASE_UNLIMITED),
    #endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_AG::RegisterHandler_ALL(PACKET_HANDLER_DELEGATOR pInstance)
{
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo());
	//pInstance->RegisterHandler_<GAME_SERVER>(PacketHandlerShared::GetHandlerInfo());

	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_SYNC());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_ZONE());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_STATUS());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_GUILD());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_MISSION());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_GM());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_CHUNTING());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_CHAO());
	pInstance->RegisterHandler_<GAME_SERVER>(GetHandlerInfo_AG_ITEM());

}


//////////////////////////////////////////////////////////////////////////

#undef FINDnALIGN_USER
#undef FINDnALIGN_USER_BY_CHARGUID
#define FINDnALIGN_USER(pUserInstance, UserKey)							\
	User* pUserInstance = UserManager::Instance()->GetUser((UserKey));	\
	if(!pUserInstance)												\
	{																	\
		SUNLOG(eFULL_LOG, _T("[G2A] Can't Find User %u"), (UserKey));	\
		return;															\
	}

#define FINDnALIGN_USER_BY_CHARGUID(pUserInstance, CharGuid)					\
	User* pUserInstance = UserManager::Instance()->GetUserByCharGuid((CharGuid));	\
	if(!pUserInstance) return;

//==================================================================================================

HANDLER_AG_IMPL(AG_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_AG_IMPL(AG_CONNECTION_UNREGISTER_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// 게임서버로 부터 플레이어 접속 해제 요청 패킷
	MSG_AG_CONNECTION_UNREGISTER_CMD* pRecvMsg = (MSG_AG_CONNECTION_UNREGISTER_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	SUNLOG(eFULL_LOG,
		_T("[Handler_FromGameServer::OnAG_CONNECTION_UNREGISTER_CMD] 게임 서버가 끊으라 해서 끊었음[ID:%d]"),
		pRecvMsg->m_dwKey);

	// 무조건 접속을 끊기전에 그 User에 대한 정보를 정리해 주었는지 체크 필요!!!
	pUser->SetDisconnectCode((RC::eCONNECTION_RESULT)pRecvMsg->m_byDisonnectCode);
	pUser->DisconnectUser();
}

HANDLER_AG_IMPL(AG_CONNECTION_DISCONNECT_SERVER_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// 게임서버로 부터 플레이어 접속 해제 요청 패킷
	MSG_AG_CONNECTION_DISCONNECT_SERVER_CMD* pRecvMsg = (MSG_AG_CONNECTION_DISCONNECT_SERVER_CMD *)pMsg;
	UserManager::Instance()->DisconnectLinkedServerUser(pRecvMsg->m_ServerType);
    // (CHANGES) (f100419.2L) (WAVERIX) fix up missing fields, \
    // field server is flushed all fields after DBP disconnect
    if (pServerSession->GetServerType() == FIELD_SERVER) {
        FieldServerSession* field_session = static_cast<FieldServerSession*>(pServerSession);
        field_session->CreateVillage();
    }
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
HANDLER_AG_IMPL(AG_CONNECTION_SERVER_SELECT_CONTROL_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_ACK* pRecvMsg =
        static_cast<const MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_ACK*>(pMsg);
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK msgACK;
    BOOLEAN bValidState = TRUE;

    // (WAVERIX) (NOTE) 중간 취소의 경우는 ACK을 수신 받지 않는다.
    if(bValidState && pUser->IsBeginTransaction() == 0)
        bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_INVALID_PLAYER_STATE;

    if(bValidState && pUser->GetTransactionState() != TR_QUERY_SERVER_SELECT_TRANSACTION)
        bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;

    if(bValidState && pRecvMsg->m_Result != RC::RC_ROOM_SUCCESS)
        bValidState = FALSE, msgACK.m_Result = pRecvMsg->m_Result;

    // 기존 정보 초기화 필요 가능성 타전...
    if(bValidState == 0)
    {
        pUser->SendPacket(&msgACK, msgACK.GetSize());
        return;
    }

    typedef nsPacket::ReAuthInfo Enum;
    pUser->OnServerSelectQueryResult(Enum::eReAuthSlot_GameServerSide, Enum::eReAuthStep_Accepted, 0);
}
#endif

HANDLER_AG_IMPL(AG_SYNC_PLAYER_ENTER_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_SYNC_PLAYER_ENTER_ACK* pRecvMsg = (MSG_AG_SYNC_PLAYER_ENTER_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// RESUME
	ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pZone)
	{
		MSG_CG_SYNC_PLAYER_ENTER_NAK msgNAK;
		msgNAK.m_dwErrorCode = 999999;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	{	//	__NA000630_070817_MOLA_5th_UPDATE
		SecurePackNode4User& rSECURE = pUser->GetSecureNode();
		rSECURE.MOLAController().SendQueryBuiltinCMD(pUser);
		sSECURE_FLAG& rSECURE_FLAG = rSECURE.SECURE_FLAG();
		rSECURE_FLAG.STATUS.TransLeave();
		//pUser->GetSecureNode().MOLAController().SendQueryBuiltinCMD(pUser);
	}

	// 들어와라
	MSG_CG_SYNC_PLAYER_ENTER_ACK msgACK;
	memcpy(msgACK.m_fPos, pRecvMsg->m_fPos, sizeof(float)*3);

	msgACK.m_TransSkillCode = pRecvMsg->m_TransSkillCode;
	//_NA001955_110210_WING_COSTUME
    // CHANGES: f110607.3L, added field to support whether his wings spreading status has continued.
    msgACK.continued_wings_spreading_ = pRecvMsg->continued_wings_spreading_;
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    msgACK.m_PcRoomStatus = pUser->GetBillingType();
#endif
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //대륙 영주 정보를 얻는다
    msgACK.m_ContinentLordInfo = pRecvMsg->m_ContinentLordInfo;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

	msgACK.Encode();

    //지역 점령전 존이동시 점령 길드를 넘긴다.
    memcpy( &(msgACK.m_GuildInfo), &(pRecvMsg->m_GuildInfo),  sizeof(DOMINATION_GUILD_INFO) );
	pUser->SendPacket(&msgACK, sizeof(msgACK));

	pZone->EnterUser(pUser);

    //__KR_001355_20090624_GROUPINGSYSTEM_ALL
    //월드서버에게 그룹관련 세팅을 시작하라는 신호를 보낸다.
    //최초 접속 / 케릭터 선택 변경시에만 발생 해야 한다.
    eZONETYPE eFromZoneType;	eZONETYPE eToZoneType;	DWORD ToLinkServerIndex;
    pUser->GetMoveZoneType( eFromZoneType, eToZoneType, ToLinkServerIndex );

    if (eFromZoneType == eZONETYPE_CHARSELECT)
    {
        MSG_AW_STATUS_GROUP_START_CMD groupMsg;
        groupMsg.m_CharGuid = pUser->GetSelectedCharGuid();

        pUser->SendToWorldServer(&groupMsg, sizeof(groupMsg));
    }
	/*
	// for debug
	switch(pUser->GetZoneState())
	{
	case ePRS_BEFORE_ENTER_VILLAGE:
	case ePRS_BEFORE_ENTER_LOBBY:
	case ePRS_BEFORE_ENTER_HUNTING:
	case ePRS_BEFORE_ENTER_MISSION:
	case ePRS_BEFORE_ENTER_PVP:
	{

	}
	break;
	case ePRS_AT_VILLAGE:
	case ePRS_AT_LOBBY:
	case ePRS_AT_HUNTING:
	case ePRS_AT_MISSION:
	case ePRS_AT_PVP:
	{
	// 단순 클라이언트로 전달
	}
	break;
	default:
	{
	MSG_CG_SYNC_PLAYER_ENTER_NAK msg;
	msg.m_dwErrorCode = 999999;
	pUser->SendPacket(&msg, sizeof(msg));
	}
	return;
	}
	*/
}

HANDLER_AG_IMPL(AG_SYNC_PLAYER_ENTER_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_SYNC_PLAYER_ENTER_NAK* pRecvMsg = (MSG_AG_SYNC_PLAYER_ENTER_NAK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// (WAVERIX) (WARNING)
	//   이 경우에 대한 핸들링이 필요하다.

	// 들어와라
	MSG_CG_SYNC_PLAYER_ENTER_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode; //! 임시
	pUser->SendPacket(&msgNAK, sizeof(msgNAK));
}

HANDLER_AG_IMPL(AG_SYNC_PLAYER_WARP_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_SYNC_PLAYER_WARP_ACK* pRecvMsg = (MSG_AG_SYNC_PLAYER_WARP_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 들어와라
	MSG_CG_SYNC_PLAYER_ENTER_ACK msgACK;
	memcpy(msgACK.m_fPos, pRecvMsg->m_fPos, sizeof(float)*3);

	// (WAVERIX) (WARNING)
	//   이 코드는 없었지만 추가한다. 나중에 이준석 대리님께 문의해봐야 한다.
	msgACK.m_TransSkillCode = 0;	//pRecvMsg->m_TransSkillCode;
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    msgACK.m_PcRoomStatus = pUser->GetBillingType();
#endif

	msgACK.Encode();

	pUser->SendPacket(&msgACK, sizeof(msgACK));
}

HANDLER_AG_IMPL(AG_SYNC_PLAYER_WARP_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_SYNC_PLAYER_WARP_NAK* pRecvMsg = (MSG_AG_SYNC_PLAYER_WARP_NAK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// (WAVERIX) (WARNING)
	//   이 경우에 대한 핸들링이 필요하다.

	// 들어와라 ... ?
	MSG_CG_SYNC_PLAYER_ENTER_NAK msgNAK;
	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode; //! 임시
	pUser->SendPacket(&msgNAK, sizeof(msgNAK));
}

HANDLER_AG_IMPL(AG_CHARINFO_CHAR_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHARINFO_CHAR_CMD* pRecvMsg = (MSG_AG_CHARINFO_CHAR_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	DWORD dwMaxHP = pRecvMsg->m_MaxHP;
	DWORD dwHP	= pRecvMsg->m_HP;
	DWORD dwMaxMP = pRecvMsg->m_MaxMP;
	DWORD dwMP	= pRecvMsg->m_MP;
    DWORD dwMaxSD = pRecvMsg->m_MaxSD;
    DWORD dwSD	  = pRecvMsg->m_SD;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    DWORD maxFP   = pRecvMsg->max_FP;
    DWORD curFP   = pRecvMsg->cur_FP;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    pUser->SetSelectedCharCurSD(dwSD);

	// Agent에서 필요한 HP 정보를 셋팅한 후 클라에 정보를 전달한다.
	pUser->SetSelectedCharCurHP(dwHP);
	pUser->SetSelectedCharCurMP(dwMP);

	if(dwMaxHP)
		pUser->SetSelectedCharMaxHP(dwMaxHP);
	if(dwMaxMP)
		pUser->SetSelectedCharMaxMP(dwMaxMP);

    if(dwMaxSD)
        pUser->SetSelectedCharMaxSD(dwMaxSD);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    pUser->SetSelectedCharCurFP(curFP);
    if (maxFP) {
        pUser->SetSelectedCharMaxFP(maxFP);
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    if(!dwMaxSD)
    {
        SUNLOG(eDEV_LOG,
            _T("[AG_CHARINFO_CHAR_CMD] UserName[%s] MaxSD Incorrect Status [%u]"),
            pUser->GetSelectedCharName(), dwMaxSD);
    }

    if(!(dwMaxHP && dwMaxMP))
	{
		SUNLOG(eCRITICAL_LOG,
			_T("[AG_CHARINFO_CHAR_CMD] UserName[%s] MaxHP/MP Incorrect Status [%u/%u]"),
			pUser->GetSelectedCharName(), dwMaxHP, dwMaxMP);
	}

	// 경쟁헌팅 전적
	pUser->SetSelectedCharCHuntingVCount(pRecvMsg->m_dwCHuntingVCount);
	pUser->SetSelectedCharCHuntingTotCount(pRecvMsg->m_dwCHuntingTotCount);

	// 카오상태
	pUser->SetSelectedCharChaoState(pRecvMsg->m_byChaoState);
}

HANDLER_AG_IMPL(AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD* pRecvMsg = (MSG_AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD*)pMsg;	
	User* pUser = UserManager::Instance()->GetUser(pRecvMsg->m_dwKey);
	if(pUser)
	{
		pUser->GetPartyReturnInfo().ReturnPos			= pRecvMsg->m_ReturnPos;
		pUser->GetPartyReturnInfo().ReturnFieldCode		= pRecvMsg->m_ReturnFieldCode;
		pUser->GetPartyReturnInfo().ReturnZoneType		= pUser->GetZoneType();
		pUser->GetPartyReturnInfo().ReturnZoneKey		= pUser->GetZoneKey();
	}
	else
	{
		ClosingUserInfo sUSER;
#ifdef _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION
        sUSER.char_guid_        = pRecvMsg->m_CharGUID;
#endif //_NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION
		sUSER.return_pos_		= pRecvMsg->m_ReturnPos;
		sUSER.return_field_code_ = pRecvMsg->m_ReturnFieldCode;
		sUSER.return_zone_type_	= pRecvMsg->m_ReturnZoneType;
		sUSER.return_zone_key_	= pRecvMsg->m_ReturnZoneKey;

		AgentPartyManager::Instance()->UpdateReturnUser(pRecvMsg->m_PartyKey, sUSER);
	}		
}

HANDLER_AG_IMPL(AG_ZONE_ROOM_JOIN_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_JOIN_NAK* pRecvMsg = (MSG_AG_ZONE_ROOM_JOIN_NAK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	ASSERT(pUser->IsBeginTransaction());

	SUNLOG(eCRITICAL_LOG, _T("AG_ZONE_ROOM_JOIN_NAK [E:%u]"), pRecvMsg->m_dwErrorCode);

	sSWITCH(pUser->GetTransactionState())
	CASE(TR_JOIN_VILLAGE)

		MSG_CG_CONNECTION_ENTERVILLAGEREQ_NAK msgNAK;
		msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));

		switch(pRecvMsg->m_dwErrorCode)
		{
		case RC::RC_DISCONNECT_NOT_FOUND_DB_USER:
		case RC::RC_DISCONNECT_NOT_CONNECTED_DB_CHARACTER:
		case RC::RC_DISCONNECTION_INVALID_CHAR_STATE:
		case RC::RC_DISCONNECT_DBP_CONNETION_ERROR:
			pUser->SetDisconnectCode((RC::eCONNECTION_RESULT)pRecvMsg->m_dwErrorCode);
			pUser->DisconnectUser();
			break;
		}

	//인스턴스 던전 종류에 따라 다른 패킷을 보내려구...
        CASE(TR_LEAVE_FIELD_FOR_INSTANCE)
		ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
		if(pZone && pZone->GetType() == eZONETYPE_INSTANCE) 
		{
			InstanceDungeon* pInstanceDungeon = (InstanceDungeon*)pZone;
			pInstanceDungeon->SendJoinNAKPacket(pUser);
		}

	DEFAULT

		MSG_CG_ZONE_MISSION_JOIN_NAK msgNAK;
		msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));

		switch(pRecvMsg->m_dwErrorCode)
		{
		case RC::RC_DISCONNECT_NOT_FOUND_DB_USER:
		case RC::RC_DISCONNECT_NOT_CONNECTED_DB_CHARACTER:
		case RC::RC_DISCONNECTION_INVALID_CHAR_STATE:
		case RC::RC_DISCONNECT_DBP_CONNETION_ERROR:
			pUser->SetDisconnectCode((RC::eCONNECTION_RESULT)pRecvMsg->m_dwErrorCode);
			pUser->DisconnectUser();
			break;
		}


	eSWITCH

		pUser->EndTransaction();

	// 정상적인 방법으로는 들어 올 수 없다!!!!
	// db가 죽었거나 버그,
	// 벌써 로비에서 나왔기때문에....
	// 끊어버리나?
	// Agent방에서는 빼줘야 하는건가?

	//TmpZoneInterface* pRoom = g_RoomController.GetRoom(pRecvMsg->m_RoomKey);
	//ASSERT(pRoom);
	//pRoom->Join(pUser);
	/*
	sSWITCH(pRoom->GetZoneType())
	CASE(eZONETYPE_HUNTING)
	pUser->EndTransaction();
	MSG_CG_ZONE_HUNTING_JOIN_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket(&msg, sizeof(msg));
	CASE(eZONETYPE_MISSION)
	pUser->EndTransaction();
	MSG_CG_ZONE_MISSION_JOIN_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket(&msg, sizeof(msg));	
	DEFAULT
	SASSERT(NULL, "AG_ZONE_ROOM_JOIN_NAK를 받았는데, 방타입이 잘못되었다.");
	eSWITCH
	*/
}

// 배틀의 헌팅, 미션 방에 들어갈 때
HANDLER_AG_IMPL(AG_ZONE_ROOM_JOIN_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_JOIN_ACK* pRecvMsg = (MSG_AG_ZONE_ROOM_JOIN_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	if(!pUser->IsBeginTransaction())
	{
		// Game서버에서 브레이크중에 유저가 끊으면, ACK가 오지만, TR_END이다.
		SUNLOG(eFULL_LOG, _T("AG_ZONE_ROOM_JOIN_ACK에서 트랜젝션이 끝났다."));
		return;
	}

	ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pZone)
		return;

	MAPCODE mcCODE = pZone->GetMapCode();

	sSWITCH(pUser->GetTransactionState())
	CASE(TR_LEAVE_LOBBY_FOR_HUNTING)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_HUNTING_JOIN_ACK msgACK;
		msgACK.m_MapCode = mcCODE;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_LOBBY_FOR_MISSION)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_MISSION_JOIN_ACK msgACK;
		msgACK.m_MapCode = mcCODE;
		pUser->SendPacket(&msgACK, sizeof(msgACK));	
	}
	CASE(TR_LEAVE_LOBBY_FOR_PVP)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_PVP_JOIN_ACK msgACK;
		msgACK.m_MapCode = mcCODE;
		pUser->SendPacket(&msgACK, sizeof(msgACK));	
		//DISPMSG("[U:%s]MSG_CG_ZONE_PVP_JOIN_ACK\n", pUser->GetUserID());
	}
	CASE(TR_LEAVE_LOBBY_FOR_CHUNTING)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_CHUNTING_JOIN_ACK msgACK;
		msgACK.m_dwMasterObjKey = 0;

		if(pZone && pZone->GetType() == eZONETYPE_CHUNTING)
		{
			CHuntingRoom* pCHunting = (CHuntingRoom*)pZone;
			if(pCHunting->GetMaster())
				msgACK.m_dwMasterObjKey = pCHunting->GetMaster()->GetPlayerKey();

			// (WAVERIX) (WARNING)
			//   그럼 CHUNTING이 아닌경우는 맵코드가 필요없는가?
			msgACK.m_MapCode = mcCODE;

		}
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_HUNTING_FOR_VILLAGE)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_HUNTING_LEAVE_ACK msgACK;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_MISSION_FOR_VILLAGE)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_MISSION_LEAVE_ACK msgACK;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_PVP_FOR_VILLAGE)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_PVP_LEAVE_ACK msgACK;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_CHUNTING_FOR_VILLAGE)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_CHUNTING_LEAVE_ACK msgACK;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_HUNTING_FOR_HUNTING)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_HUNTING_RELAY_JOIN_ACK msgACK;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_JOIN_VILLAGE)
	{
		pUser->EndTransaction();

		//	__NA000630_070817_MOLA_5th_UPDATE
		pUser->GetSecureNode().MOLAController().SendQueryBuiltinCMD(pUser);

		MSG_CG_CONNECTION_ENTERVILLAGEREQ_ACK msgACK;
		msgACK.m_dwPlayerKey = pUser->GetPlayerKey();
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}
	CASE(TR_LEAVE_MISSION_FOR_LOBBY)
	{
		pUser->EndTransaction();
		if(pZone && pZone->GetType() == eZONETYPE_LOBBY)
		{
			LobbyRoom* pLobby = (LobbyRoom*)pZone;

			MSG_CG_ZONE_MISSION_RELAY_JOIN_ACK msgACK;
			msgACK.m_LobbyKey = pUser->GetZoneKey();
			if(pLobby->GetMaster())	msgACK.m_dwMasterKey = pLobby->GetMaster()->GetPlayerKey();
			else						msgACK.m_dwMasterKey = 0;
			pUser->SendPacket(&msgACK, sizeof(msgACK));
		}
	}
	//인스턴스 던전 종류에 따라 다른 패킷을 보내려구...
    CASE(TR_LEAVE_FIELD_FOR_INSTANCE)
	{
		pUser->EndTransaction();
		if(pZone && pZone->GetType() == eZONETYPE_INSTANCE) 
		{
			InstanceDungeon* pInstanceDungeon = (InstanceDungeon*)pZone;
			pInstanceDungeon->SendJoinACKPacket(pUser);
		}
	}
	CASE(TR_LEAVE_INSTANCE_DUNGEON)
	{
		pUser->EndTransaction();
		MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_ACK msgACK;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
	}

	CASE(TR_LEAVE_FIELD_FOR_SSQFIELD)
	{
		pUser->EndTransaction();
		// Test packet
		//MSG_CG_ZONE_EX_ROOM_JOIN_ACK msgACK;
		//msgACK.m_MapCode = 60001;
		//msgACK.m_fcCODE = 60001;
		//msgACK.m_byZONETYPE = pZone->GetType();
		if(!FlowControl::FCAssert(pZone->GetType() == eZONETYPE_INSTANCE))
			break;

		InstanceDungeon* const pIndun = static_cast<InstanceDungeon*>(pZone);
        if(!FlowControl::FCAssert(pIndun->GetDungeonType() == INDUN_KIND_SS_QUEST))
			break;
		DungeonSSQ* const pDungeonSSQ = static_cast<DungeonSSQ*>(pIndun);

		WarControlManager::OnMsg(SSQCtrlCmd::UserEvent::AG_RoomJoinAck(pUser), pDungeonSSQ);
		//	MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgACK;	<- 내부적으로 이것을 보낸다.
	}
    CASE(TR_LEAVE_FIELD_FOR_ROOM_BY_PORTAL)
    {
        pUser->EndTransaction();
        MSG_CG_ZONE_EX_ROOM_JOIN_ACK msgACK;
        BOOLEAN result = PortalManager::JoinRoom(pUser, &msgACK);
        if(result == FALSE)
            break;
        pUser->SendPacket(&msgACK, sizeof(msgACK));
    }
    CASE(TR_LEAVE_FIELD_FOR_ROOM)
    {
        pUser->EndTransaction();
        MSG_CG_ZONE_EX_ROOM_JOIN_ACK msgACK;

        ClosingUserTransController& user_trans_controller = pUser->closing_user_trans_controller_;
        if (user_trans_controller.IsTransaction())
        {
            msgACK.m_MapCode = user_trans_controller.GetMapCode();
            msgACK.m_fcCODE = user_trans_controller.GetFieldCode();
            msgACK.m_byZONETYPE = pZone->GetType();
            user_trans_controller.EndTransaction();
        }
        else
        {
            msgACK.m_MapCode = pZone->GetMapCode();
        }
        pUser->SendPacket(&msgACK, sizeof(msgACK));

        user_trans_controller.Init();

        AgentPartyManager* const party_manager = AgentPartyManager::Instance();

        party_manager->RemoveClosingUser(
            pUser->GetPartyState().GetPartyKey(),
            pUser->GetSelectedCharGuid()
            );
    }
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    CASE(TR_LEAVE_FIELD_FOR_PARTY_MISSION)
    {
        pUser->EndTransaction();
        MSG_CG_ZONE_MISSION_JOIN_ACK msgACK;
        msgACK.m_MapCode = mcCODE;
        pUser->SendPacket(&msgACK, sizeof(msgACK));	        
    }
    CASE(TR_LEAVE_FIELD_FOR_PARTY_PVP)
    {
        pUser->EndTransaction();
        MSG_CG_ZONE_PVP_JOIN_ACK msgACK;
        msgACK.m_MapCode = mcCODE;
        pUser->SendPacket(&msgACK, sizeof(msgACK));	
    }
    CASE(TR_LEAVE_FIELD_FOR_PARTY_INSTANCEDUNGEON)
    {
        pUser->EndTransaction();
        if(pZone && pZone->GetType() == eZONETYPE_INSTANCE) 
        {
            InstanceDungeon* pInstanceDungeon = (InstanceDungeon*)pZone;
            pInstanceDungeon->SendJoinACKPacket(pUser);
        }     
    }
    CASE(TR_LEAVE_FIELD_FOR_PARTY_SSQ)
    {
        pUser->EndTransaction();
        MSG_CG_ZONE_MISSION_JOIN_ACK msgACK;
        msgACK.m_MapCode = mcCODE;
        pUser->SendPacket(&msgACK, sizeof(msgACK));	        
    }
	DEFAULT
	{
		SUNLOG(eFULL_LOG,  "AG_ZONE_ROOM_JOIN_ACK를 받았는데, 방타입이 잘못되었다.");
	}
	eSWITCH

	pUser->GetPartyReturnInfo().ReturnPos			= pRecvMsg->ReturnPos;
	pUser->GetPartyReturnInfo().ReturnFieldCode		= pRecvMsg->ReturnFieldCode;
	pUser->GetPartyReturnInfo().ReturnZoneType		= pRecvMsg->ReturnZoneType;
	pUser->GetPartyReturnInfo().ReturnZoneKey		= pRecvMsg->ReturnZoneKey;

    pUser->GetSecureNode().EnableHeartbeats(false);
}

// 헌팅,미션을 가기위해 필드의 로비를 나오거나, 
// 배틀의 헌팅, 미션 방에서 나와서 마을로 돌아갈 때
HANDLER_AG_IMPL(AG_ZONE_ROOM_LEAVE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_ROOM_LEAVE_ACK* pRecvMsg = (MSG_AG_ZONE_ROOM_LEAVE_ACK *)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    if(!pUser->IsBeginTransaction())
    {
        // Game서버에서 브레이크중에 유저가 끊으면, ACK가 오지만, TR_END이다.
        SUNLOG(eFULL_LOG,  "AG_ZONE_ROOM_LEAVE_ACK에서 트랜젝션이 끝났다.");
        return;
    }

    eZONETYPE roomType = eZONETYPE_MAX;
    sSWITCH(pUser->GetTransactionState())
    CASE(TR_LEAVE_LOBBY_FOR_HUNTING)            roomType = eZONETYPE_HUNTING;
    CASE(TR_LEAVE_LOBBY_FOR_MISSION)            roomType = eZONETYPE_MISSION;
    CASE(TR_LEAVE_LOBBY_FOR_PVP)                roomType = eZONETYPE_PVP;
        DISPMSG("[U:%s]MSG_AG_ZONE_ROOM_LEAVE_ACK+JOIN_SYN\n", pUser->GetUserID());
    CASE(TR_LEAVE_LOBBY_FOR_CHUNTING)           roomType = eZONETYPE_CHUNTING;
    CASE(TR_LEAVE_HUNTING_FOR_VILLAGE)          roomType = eZONETYPE_VILLAGE;
    CASE(TR_LEAVE_MISSION_FOR_VILLAGE)          roomType = eZONETYPE_VILLAGE;
    CASE(TR_LEAVE_PVP_FOR_VILLAGE)              roomType = eZONETYPE_VILLAGE;
    CASE(TR_LEAVE_CHUNTING_FOR_VILLAGE)         roomType = eZONETYPE_VILLAGE;
    CASE(TR_LEAVE_HUNTING_FOR_HUNTING)          roomType = eZONETYPE_HUNTING;
    CASE(TR_LEAVE_MISSION_FOR_LOBBY)            roomType = eZONETYPE_LOBBY;
    CASE(TR_LEAVE_FIELD_FOR_INSTANCE)           roomType = eZONETYPE_INSTANCE;
    CASE(TR_LEAVE_INSTANCE_DUNGEON)             roomType = eZONETYPE_VILLAGE;
    CASE(TR_LEAVE_FIELD_FOR_ROOM)               roomType = eZONETYPE_MISSION;	// 이것 수정할 것
    CASE(TR_LEAVE_FIELD_FOR_SSQFIELD)           roomType = eZONETYPE_INSTANCE;
    CASE(TR_LEAVE_FIELD_FOR_ROOM_BY_PORTAL)     roomType = eZONETYPE_MISSION;
    // _NA_20110630_BATTLE_ZONE_RENEWAL
    CASE(TR_LEAVE_FIELD_FOR_PARTY_MISSION)      roomType = eZONETYPE_MISSION;
    CASE(TR_LEAVE_FIELD_FOR_PARTY_PVP)          roomType = eZONETYPE_PVP;
    CASE(TR_LEAVE_FIELD_FOR_PARTY_INSTANCEDUNGEON)   roomType = eZONETYPE_INSTANCE;
    CASE(TR_LEAVE_FIELD_FOR_PARTY_SSQ)          roomType = eZONETYPE_INSTANCE;
    

    CASE(TR_BACKTOSELECT)
    {
        // 캐릭터선택창으로 되돌아 옴
        pUser->EndTransaction();
        UserManager::Instance()->RemoveUserForSelectedCharInfoHashes(pUser);

        MSG_CG_CONNECTION_BACKTOCHARSELECT_ACK msgACK;
        pUser->SendPacket(&msgACK, sizeof(msgACK));

        pUser->GetSecureNode().EnableHeartbeats(false);

        //캐릭터 종료 로그
        GAMELOG->DisconnectChar(pUser);
        pUser->EndCharLoginTime();

        CharSelect* pCharSelect = g_ZoneManager.FindCharSelect(1);
        ASSERT(pCharSelect);
        if(pCharSelect)
            pCharSelect->EnterUser(pUser);

        return;
    }
    DEFAULT
    {
        SUNLOG(eCRITICAL_LOG, "AG_ZONE_ROOM_LEAVE_ACK를 받았는데, GetTransactionState() 잘못되었다.");
        return;
    }
    eSWITCH;

    ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetTRRoomKey());
    if(pZone == 0) 
    {
        // 헌팅에 바로 들어 가는 경우에 생길 가능성 있음
        if(pUser->GetTransactionState() == TR_LEAVE_LOBBY_FOR_HUNTING)
        {
            // (WAVERIX) (WARNING)
            //   한번 더 살펴보자.
            Village* pVillage = g_ZoneManager.FindVillage(pUser->GetSelectedCharMapCode());
            if(pVillage)
                pVillage->ReadyUser(pUser, eSEND_SYN);
            SUNLOG(eCRITICAL_LOG,  "[s%u:k%u]MSG_AG_ZONE_ROOM_LEAVE_ACK시방이없어서마을로보냄!\n", pUser->GetTransactionState(), pUser->GetTRRoomKey());
        }
        else
            SUNLOG(eCRITICAL_LOG,  "[s%u:k%u]MSG_AG_ZONE_ROOM_LEAVE_ACK에 방이 없습니다!\n", pUser->GetTransactionState(), pUser->GetTRRoomKey());

        return;
    }

    pZone->ReadyUser(pUser, eSEND_SYN);
    const eZONETYPE toZoneType = pZone->GetType();
    // for debug report
    if(toZoneType != roomType)
    {
        BOOLEAN result = FALSE;

        switch(pUser->GetTransactionState())
        {
        case TR_LEAVE_INSTANCE_DUNGEON:	result = TRUE;	break;
        case TR_LEAVE_FIELD_FOR_ROOM:
        case TR_LEAVE_FIELD_FOR_ROOM_BY_PORTAL:
            switch(roomType)
            {
            case eZONETYPE_MISSION:	case eZONETYPE_INSTANCE:
                result = TRUE;
                break;
            };
            break;
        case TR_LEAVE_MISSION_FOR_VILLAGE:
            {
                result = (toZoneType == eZONETYPE_VILLAGE) ||
                         (toZoneType == eZONETYPE_DOMINATION_FIELD) ||
                         (toZoneType == eZONETYPE_FIELD) ||
                         (toZoneType == eZONETYPE_SPA_FIELD);
            }
            break;
        };

        if(result == 0)
        {
            SUNLOG(eCRITICAL_LOG,
                   "MSG_AG_ZONE_ROOM_LEAVE_ACK: Invalid Room [T:%u != %u][CODE:%u]\n",
                   pZone->GetType(), roomType, pZone->GetMapCode());
        }
    }

    pUser->GetSecureNode().EnableHeartbeats(false);
}

HANDLER_AG_IMPL(AG_ZONE_ROOM_LEAVE_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	//SASSERT(NULL, "AG_ZONE_ROOM_LEAVE_ACK를 받았는데, 방타입이 잘못되었다.");

	//User의 m_pLinkServer가 NULL 일때 Disconnect하면...
	SUNLOG(eCRITICAL_LOG, "[AG_ZONE_ROOM_LEAVE_NAK]받음. - shogen");
}

// 미션에서 강제추방
HANDLER_AG_IMPL(AG_ZONE_MISSION_LEAVE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_MISSION_LEAVE_CMD* pRecvMsg = (MSG_AG_ZONE_MISSION_LEAVE_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	if(pUser->IsBeginTransaction())
	{
		SUNLOG(eFULL_LOG,
			_T("[Handler_FromGameServer_IMPL(AG_ZONE_MISSION_LEAVE_CMD)] 이미 트랜젝션 중입니다."));
		return;
	}

    MAPCODE selectedMapCode = 0;
    if(pRecvMsg->m_LeaveType == pRecvMsg->eLeaveType_Default)
        selectedMapCode = pUser->GetSelectedCharMapCode();
    else
        selectedMapCode = pRecvMsg->m_ReturnMapCode;

	ZoneInterface* pPrevZone = g_ZoneManager.FindStaticZone(selectedMapCode);
	if(pPrevZone == NULL)
        return;

	eZONETYPE ztZONE = pPrevZone->GetType();
	// dwAcceptType 관련
	// 0 : 처리 여지가 없다. (버그 또는 해킹일 수 있다)
	// 1 : 마을이므로 처리 가능
	// 2 : 필드지만 튜토리얼 필드일 가능성 있다.
	// 3 : 튜토리얼 필드다.

	if(ztZONE != eZONETYPE_VILLAGE &&
        ztZONE != eZONETYPE_DOMINATION_FIELD &&
        ztZONE != eZONETYPE_FIELD &&
        ztZONE != eZONETYPE_SPA_FIELD)
		return;

    pUser->SetSelectedCharMapCode(selectedMapCode);

	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_MISSION,	ztZONE				//	eZONETYPE
			, pUser->GetZoneKey(),	pPrevZone->GetKey()	//	ZoneKey
			);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_MISSION_FOR_VILLAGE);
		pUser->SetTRRoomKey(pPrevZone->GetKey());
	}
	else
		SUNLOG(eFULL_LOG,  "[Handler_FromGameServer_IMPL(AG_ZONE_MISSION_LEAVE_CMD)] LeaveRoom Error(%d) ", rt);
}

// 경쟁헌팅에서 강제추방
HANDLER_AG_IMPL(AG_ZONE_CHUNTING_LEAVE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CHUNTING_LEAVE_CMD* pRecvMsg = (MSG_AG_ZONE_CHUNTING_LEAVE_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	if(pUser->IsBeginTransaction())
	{
		SUNLOG(eFULL_LOG,
			_T("[Handler_FromGameServer_IMPL(AG_ZONE_CHUNTING_LEAVE_CMD)] 이미 트랜젝션 중입니다."));
		return;
	}

	Village* pVillage = g_ZoneManager.FindVillage((MAPCODE)pUser->GetSelectedCharMapCode());
	if(!pVillage) return;

	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_CHUNTING_TO_VILLAGE>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_CHUNTING,	eZONETYPE_VILLAGE	//	eZONETYPE
			, pUser->GetZoneKey(),	pVillage->GetKey()	//	ZoneKey
			);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_CHUNTING_FOR_VILLAGE);
		pUser->SetTRRoomKey(pVillage->GetKey());
	}
	else
	{
		SUNLOG(eFULL_LOG,  "[Handler_FromGameServer_IMPL(AG_ZONE_CHUNTING_LEAVE_CMD)] LeaveRoom Error(%d) ", rt);
	}
}

// 경쟁헌팅에서 경쟁종료
HANDLER_AG_IMPL(AG_ZONE_CHUNTING_COMPETITION_END_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CHUNTING_COMPETITION_END_CMD* pRecvMsg = (MSG_AG_ZONE_CHUNTING_COMPETITION_END_CMD*)pMsg;

	// 경쟁이 종료되었으므로 새로이 경쟁상대를 받을 수 있도록 플래그를 셋팅한다.
	CHuntingRoom* pCHunting = g_ZoneManager.FindCHunting(pRecvMsg->m_RoomKey);
	if(pCHunting)
	{
		pCHunting->CompleteCompetition();
	}
}

HANDLER_AG_IMPL(AG_ZONE_PORTAL_FIELD_MOVE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_PORTAL_FIELD_MOVE_ACK* pRecvMsg = (MSG_AG_ZONE_PORTAL_FIELD_MOVE_ACK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	//1. 현재 포탈의 FromField코드를 얻어낸다.
	sMAP_ENTERENCE_PORTAL* pFromInfo = MapInfoParser::Instance()->GetMapPortal(pRecvMsg->m_wIndex);
	if(!pFromInfo)
	{
		SUNLOG(eCRITICAL_LOG,
			_T("HANDLER_FROMCLIENT_IMPL(CG_ZONE_PORTAL_FIELD_MOVE_SYN) 잘못된 포탈 index, %d"),
			pRecvMsg->m_wIndex);

		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_ROOM_INVALID_MAPCODE;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	WORD wFieldCode = 0;
	if(pFromInfo->bMoveType == 0) //< 필드간 이동이면 특정 필드 지정
		wFieldCode = pFromInfo->wFieldTo;

	// 이동하려는 곳의 필드 정보를 불러온다.
	ZoneInterface* pToZone = g_ZoneManager.FindStaticZone(pFromInfo->wWorldTo);
	if(!pToZone)
	{
		SUNLOG(eCRITICAL_LOG,
			_T("HANDLER_FROMCLIENT_IMPL(CG_ZONE_PORTAL_FIELD_MOVE_SYN) 맵이 존재하지 않습니다. To = %u"),
			pFromInfo->wWorldFrom);

		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	// 반드시 현재 맵과 이동하려는 맵은 같은 서버에 존재해야 한다!
	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_STATIC_TO_STATIC>::AlignInfo(
			pUser
			//{		From		|			To		}
			, pUser->GetZoneType(),	pToZone->GetType()	//	eZONETYPE
			, pUser->GetZoneKey(),	pToZone->GetKey()	//	ZoneKey
			);
	RoomArgument args(eZONEPUBLIC_PUBLIC, NULL, wFieldCode, pFromInfo->m_dwToAreaID);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		ASSERT(pUser->GetLinkedServer() == pToZone->GetLinkedServer());

		MSG_CG_ZONE_PORTAL_FIELD_MOVE_ACK msgACK;
		msgACK.m_wIndex = pRecvMsg->m_wIndex;
		pUser->SendPacket(&msgACK, sizeof(msgACK));

        pUser->GetSecureNode().EnableHeartbeats(false);

		//필드이동 로그

		GAMELOG->MoveVillage(pUser, pFromInfo->wWorldFrom, pFromInfo->wWorldTo, FALSE);

	}
	else
	{
		// 마을 이동 실패
		SUNLOG(eCRITICAL_LOG,
			_T("HANDLER_FROMCLIENT_IMPL(CG_ZONE_PORTAL_FIELD_MOVE_SYN) 마을이동 실패"));

		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msgNAK;
		msgNAK.m_byErrorCode = rt;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
	}
}

HANDLER_AG_IMPL(AG_ZONE_PORTAL_FIELD_MOVE_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK* pRecvMsg = (MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 마을 이동 실패
	MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msgNAK;
	msgNAK.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendPacket(&msgNAK, sizeof(msgNAK));
}

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
HANDLER_AG_IMPL(AG_ZONE_QUEST_PORTAL_REQUEST_SYN)
{// [1279,1281] need implementation
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_QUEST_PORTAL_REQUEST_SYN* pRecvMsg =
        static_cast<MSG_AG_ZONE_QUEST_PORTAL_REQUEST_SYN*>(pMsg);
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    if(pUser->IsBeginTransaction())
    {   // irregular로 밖에 볼 수 없다.
        return;
    }

    const eZONE_STATE zoneState = pUser->GetZoneState();
    const BOOLEAN acceptableState =
        (ePRS_AT_ZONE & zoneState) && (ePRS_AT_CHARSELECT != zoneState);
    if(!acceptableState)
    {   // irregular
        return;
    }
    // (Work to do)
    // - requester에 대한 Zone관련 처리 상태를 파악해야 한다.(+추가 관리 구조)
    // - 임시로 포탈 생성 명령 (중복 포탈 생성???)
    // - 파티에 따른 처리 | 해당 유저(들)에 포탈생성 정보 + Key
    // - 해당 방 조건 (파티장 기준)
    // - 개별입장 가능한 관리 구조
    // - 아직 방이 생성되지 않은 상태일 것이므로,
    //  한명이 최초 요청시 다른 유저에 대한 트랜잭션 관리가 필요하다.

    const QuestPortalInfo& rInfo = pRecvMsg->questPortalInfo;
    RC::eROOM_RESULT result = g_ZoneManager.CreatePortal(rInfo, pUser, true);
    if(result != RC::RC_ROOM_SUCCESS)
    {
        //skip
        return;
    }
    // <성공시에는 ZoneManager::CreatePortal->PortalManager::CreatePortal에서 전송한다.
}

HANDLER_AG_IMPL(AG_ZONE_QUEST_PORTAL_ENTER_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK* pRecvMsg =
        static_cast<MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK*>(pMsg);
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    PortalManager::OnPortalEnterResult(pRecvMsg, pUser);
}

HANDLER_AG_IMPL(AG_ZONE_QUEST_PORTAL_CONTROL_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_QUEST_PORTAL_CONTROL_CMD* pRecvMsg =
        static_cast<MSG_AG_ZONE_QUEST_PORTAL_CONTROL_CMD*>(pMsg);
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    PortalManager::RequestClosePortal(pUser);
}

//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__


HANDLER_AG_IMPL(AG_ZONE_RETURN_VILLAGE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_RETURN_VILLAGE_ACK* pRecvMsg = (MSG_AG_ZONE_RETURN_VILLAGE_ACK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_RETURN_VILLAGE_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	// 이동하려는 곳의 필드 정보를 불러온다.
	ZoneInterface* pToZone = NULL;
	pToZone = g_ZoneManager.FindStaticZone(pRecvMsg->m_wMapCode);

	//마을이 존재하지 않거나, 타입이 마을이 아닌경우 NAK를 보낸다.
	if(!pToZone || pToZone->GetType() != eZONETYPE_VILLAGE)
	{
		MSG_CG_ZONE_RETURN_VILLAGE_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	ZoneInterface*	pFromZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	WORD			wFromMapCode = 0;
	if(pFromZone)
		wFromMapCode = pFromZone->GetMapCode();

	//	(WAVERIX) (081012) (BUG-FIX) 마을 복귀 안되는 현상
	//	- 이 패킷이 미션같은데서 날리는 줄 알았음. 정책을 MZTF_ROOM_TO_STATIC으로 설정해서 생긴 문제
	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_STATIC_TO_STATIC>::AlignInfo(
			pUser
			//{		From		|			To		}
			, pUser->GetZoneType(),	pToZone->GetType()	//	eZONETYPE
			, pUser->GetZoneKey(),	pToZone->GetKey()	//	ZoneKey
			);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_RETURN_VILLAGE_ACK msgACK;
		msgACK.m_wMapCode = pRecvMsg->m_wMapCode;
		pUser->SendPacket(&msgACK, sizeof(msgACK));
		//필드이동 로그..
		GAMELOG->MoveVillage(pUser, wFromMapCode, pRecvMsg->m_wMapCode, FALSE);
	}
	else
	{
		// 마을 이동 실패
		MSG_CG_ZONE_RETURN_VILLAGE_NAK msgNAK;
		msgNAK.m_byErrorCode = rt;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
	}
}

HANDLER_AG_IMPL(AG_ZONE_RETURN_VILLAGE_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_RETURN_VILLAGE_NAK* pRecvMsg = (MSG_AG_ZONE_RETURN_VILLAGE_NAK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 마을 이동 실패
	MSG_CG_ZONE_RETURN_VILLAGE_NAK msgNAK;
	msgNAK.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendPacket(&msgNAK, sizeof(msgNAK));
}

//캐릭터 레벨업
HANDLER_AG_IMPL(AG_STATUS_LEVEL_UP_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_LEVEL_UP_CMD* pRecvMsg = (MSG_AG_STATUS_LEVEL_UP_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	DWORD dwMaxHP = (DWORD)pRecvMsg->m_dwCurHP;
	DWORD dwHP	= dwMaxHP;
	DWORD dwMaxMP = (DWORD)pRecvMsg->m_dwMaxMP;
	DWORD dwMP	= (DWORD)pRecvMsg->m_dwCurMP;
    DWORD dwMaxSD = (DWORD)pRecvMsg->m_dwMaxAndCurSD;
    DWORD dwSD	= (DWORD)pRecvMsg->m_dwMaxAndCurSD;

	pUser->SetCharLevel(pUser->GetSelectedCharIndex(), pRecvMsg->m_Level);
	pUser->SetSelectedCharCurHP(dwHP);
	pUser->SetSelectedCharCurMP(dwMP);

	if(dwMaxHP)
		pUser->SetSelectedCharMaxHP(dwMaxHP);
	if(dwMaxMP)
		pUser->SetSelectedCharMaxMP(dwMaxMP);

    pUser->SetSelectedCharCurSD(dwSD);
    if(dwMaxSD)
        pUser->SetSelectedCharMaxSD(dwMaxSD);

    if(!dwMaxSD)
    {
        SUNLOG(eCRITICAL_LOG,
            _T("[AG_STATUS_LEVEL_UP_CMD] UserName[%s] MaxSD Incorrect Status [%u]"),
            pUser->GetSelectedCharName(), dwMaxSD);
    }

	if(!(dwMaxHP && dwMaxMP))
	{
		SUNLOG(eCRITICAL_LOG,
			_T("[AG_STATUS_LEVEL_UP_CMD] UserName[%s] MaxHP/MP Incorrect Status [%u/%u]"),
			pUser->GetSelectedCharName(), dwHP, dwMaxMP);
	}

	// HPRatio, MPRatio를 구한다.
	float fHPRatio = .0f, fMPRatio = .0f;

	dwMaxHP = pUser->GetSelectedCharMaxHP();
	if(dwMaxHP > 0)
		fHPRatio = (float)dwHP/dwMaxHP;

	dwMaxMP = pUser->GetSelectedCharMaxMP();
	if(dwMaxMP > 0)
		fMPRatio = (float)dwMP/dwMaxMP;

	// 파티원들에게 레벨업을 알린다.
	AgentPartyManager::Instance()->MemberLevelUp(pUser, fHPRatio, fMPRatio);

	{ //
		MSG_CG_STATUS_LEVEL_UP_CMD msgCMD;
		msgCMD.m_Level			= pRecvMsg->m_Level;
		msgCMD.m_wRemainStat	= pRecvMsg->m_wRemainStat;
		msgCMD.m_wRemainSkill	= pRecvMsg->m_wRemainSkill;	
		msgCMD.m_dwCurHP		= dwHP;	
		msgCMD.m_dwCurMP		= dwMP;	
        msgCMD.m_dwCurSD		= dwSD;	
		pUser->SendPacket(&msgCMD, sizeof(msgCMD));
	};
	{ // 월드서버에도 통보한다(길드/친구 레벨업 알림 위해서)
		MSG_AW_USER_LEVELUP_CMD	msgCMD;
		msgCMD.m_dwKey		= pUser->GetUserKey();
		msgCMD.m_CharGuid	= pUser->GetCharGUID(pUser->GetSelectedCharIndex());
		msgCMD.m_Level		= pRecvMsg->m_Level;
		pUser->SendToWorldServer(&msgCMD, sizeof(msgCMD));
	};
}

HANDLER_AG_IMPL(AG_STATUS_MAXHP_CHANGE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_MAXHP_CHANGE_CMD* pRecvMsg = (MSG_AG_STATUS_MAXHP_CHANGE_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	DWORD dwMaxHP = pRecvMsg->m_dwMaxHP;
	if(!dwMaxHP)
	{
		SUNLOG(eCRITICAL_LOG,
			_T("[AG_STATUS_MAXHP_CHANGE_CMD] MaxHP is 0!! UserName[%s]"),
			pUser->GetSelectedCharName());
		return;
	}

	// MaxHP 값을 셋팅한다.
	pUser->SetSelectedCharMaxHP(dwMaxHP);
}

HANDLER_AG_IMPL(AG_STATUS_MAXMP_CHANGE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_MAXMP_CHANGE_CMD* pRecvMsg = (MSG_AG_STATUS_MAXMP_CHANGE_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	DWORD dwMaxMP = pRecvMsg->m_dwMaxMP;
	if(!dwMaxMP)
	{
		SUNLOG(eCRITICAL_LOG,
			_T("[AG_STATUS_MAXMP_CHANGE_CMD] MaxMP is 0!! UserName[%s]"),
			pUser->GetSelectedCharName());
		return;
	}

	// MaxMP 값을 셋팅한다.
	pUser->SetSelectedCharMaxMP(dwMaxMP);
}

HANDLER_AG_IMPL(AG_STATUS_MAXSD_CHANGE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_STATUS_MAXSD_CHANGE_CMD* pRecvMsg = (MSG_AG_STATUS_MAXSD_CHANGE_CMD*)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    DWORD dwMaxSD = pRecvMsg->m_dwMaxSD;
    if(!dwMaxSD)
    {
        SUNLOG(eCRITICAL_LOG,
            _T("[AG_STATUS_MAXSD_CHANGE_CMD] MaxSD is 0!! UserName[%s]"),
            pUser->GetSelectedCharName());
        return;
    }

    // MaxSD 값을 셋팅한다.
    pUser->SetSelectedCharMaxSD(dwMaxSD);
}

HANDLER_AG_IMPL(AG_STATUS_HP_CHANGE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    //__NA001336_090514_PARTY_NAVI_SUPPORT__
    ASSERT(!"Function Not Support Anymore!");
}

HANDLER_AG_IMPL(AG_STATUS_MP_CHANGE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    //__NA001336_090514_PARTY_NAVI_SUPPORT__
    ASSERT(!"Function Not Support Anymore!");
}

HANDLER_AG_IMPL(AG_STATUS_DEAD_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_DEAD_CMD* pRecvMsg = (MSG_AG_STATUS_DEAD_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// HP 값을 셋팅한다.
	pUser->SetSelectedCharCurHP(0);

	// 파티원들에게 HP변화를 알린다.
	AgentPartyManager::Instance()->MemberDead(pUser);
}

HANDLER_AG_IMPL(AG_STATUS_RESURRECTION_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_RESURRECTION_CMD* pRecvMsg = (MSG_AG_STATUS_RESURRECTION_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	DWORD dwHP	= (DWORD)pRecvMsg->m_dwCurHP;
	DWORD dwMP	= (DWORD)pRecvMsg->m_dwCurMP;
    DWORD dwSD	= (DWORD)pRecvMsg->m_dwCurSD;
    pUser->SetSelectedCharCurSD(dwSD);

	// HP, MP 값을 셋팅한다.
	pUser->SetSelectedCharCurHP(pRecvMsg->m_dwCurHP);
	pUser->SetSelectedCharCurMP(pRecvMsg->m_dwCurMP);

	// HPRatio, MPRatio를 구한다.
	float fHPRatio = .0f, fMPRatio = .0f;

	DWORD dwMaxHP = pUser->GetSelectedCharMaxHP();
	if(dwMaxHP > 0)
		fHPRatio = (float)dwHP/dwMaxHP;

	DWORD dwMaxMP = pUser->GetSelectedCharMaxMP();
	if(dwMaxMP > 0)
		fMPRatio = (float)dwMP/dwMaxMP;

	// 파티원들에게 HP변화를 알린다.
	AgentPartyManager::Instance()->MemberResurrection(pUser, fHPRatio, fMPRatio);
}

HANDLER_AG_IMPL(AG_STATUS_CHANGE_STATE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_CHANGE_STATE_CMD* pRecvMsg = (MSG_AG_STATUS_CHANGE_STATE_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// BehaveState를 셋팅한다
	pUser->SetBehaveState(pRecvMsg->m_eBehaveState);
}


HANDLER_AG_IMPL(AG_STATUS_PLAYING_TUTORIAL_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
    ASSERT(!"can't entered this handler because the renewaled tutorial has other attachment.");
    return;
#if SUN_CODE_BACKUP
	MSG_AG_STATUS_PLAYING_TUTORIAL_CMD* pRecvMsg = (MSG_AG_STATUS_PLAYING_TUTORIAL_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	pUser->SetSelectedCharPlayingTutorial(pRecvMsg->m_bIsPlayingTutorial);
#endif
}


/*
HANDLER_AG_IMPL(AG_CONNECTION_UNREGISTER_ACK)
{
	MSG_AG_CONNECTION_UNREGISTER_ACK* pRecvMsg = (MSG_AG_CONNECTION_UNREGISTER_ACK *)pMsg;
	User* pUser = GETUSER(pRecvMsg->m_dwKey);

	SUNLOG(eFULL_LOG,  "[HANDLER_AG_IMPL(AG_CONNECTION_UNREGISTER_ACK)] 삭제:%s#############", pUser->GetUserID());
	SUNLOG(eFULL_LOG,  "[HANDLER_AG_IMPL(AG_CONNECTION_UNREGISTER_ACK)] %s", pUser->GetSelectedCharName());
	SUNLOG(eFULL_LOG,  "[HANDLER_AG_IMPL(AG_CONNECTION_UNREGISTER_ACK)] ###########");

	//Send to Guild Server
	if(0 != pUser->GetSelectedCharGuildGuid())
	{
		MSG_AZ_GUILD_LOGOUT_CHAR_CMD cmsg;
		cmsg.m_GuildGuid = pUser->GetSelectedCharGuildGuid();
		pUser->SendToGuildServer(&cmsg, sizeof(cmsg));
	}


	UserManager::Instance()->RemoveUserForCharName(pUser);

	pUser->UnlinkServer();

	pUser->EndTransaction();

	MSG_CG_CONNECTION_BACKTOCHARSELECT_ACK msg;
	pUser->SendPacket(&msg, sizeof(msg));

}

HANDLER_AG_IMPL(AG_CONNECTION_UNREGISTER_NAK)
{
	MSG_AG_CONNECTION_UNREGISTER_NAK* pRecvMsg = (MSG_AG_CONNECTION_UNREGISTER_NAK *)pMsg;
	User* pUser = GETUSER(pRecvMsg->m_dwKey);

	pUser->EndTransaction();

	MSG_CG_CONNECTION_BACKTOCHARSELECT_NAK msg;
	msg.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket(&msg, sizeof(msg));
}
*/

// 길드서버에서 방어적인 처리를 함
// 생성패킷을 보낸후 바로 로그아웃을 했을 때 : 방장은 로그인 상태로 그대로 있다. (무시?)
HANDLER_AG_IMPL(AG_GUILD_CREATE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GUILD_CREATE_ACK* pRecvMsg = (MSG_AG_GUILD_CREATE_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	eZONE_STATE zsZONE = pUser->GetZoneState();
	if(ePRS_AT_CHARSELECT == zsZONE || ePRS_BEFORE_ENTER_CHARSELECT == zsZONE)
	{
		SUNLOG(eCRITICAL_LOG, _T("길드생성하자마자 캐릭터 선택창으로 돌아감!!"));
		return;
	}

	pUser->SetSelectedCharGuildGuid(pRecvMsg->m_GuildGuid);
}

HANDLER_AG_IMPL(AG_GUILD_DESTROY_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GUILD_DESTROY_ACK* pRecvMsg = (MSG_AG_GUILD_DESTROY_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	eZONE_STATE zsZONE = pUser->GetZoneState();
	if(ePRS_AT_CHARSELECT == zsZONE || ePRS_BEFORE_ENTER_CHARSELECT == zsZONE)
	{
		SUNLOG(eCRITICAL_LOG, _T("길드파괴하자마자 캐릭터 선택창으로 돌아감!!"));
		return;
	}

	pUser->SetSelectedCharGuildGuid(0);

	MSG_CG_GUILD_DESTROY_ACK msgACK;
	pUser->SendPacket(&msgACK, sizeof(msgACK));
}


HANDLER_AG_IMPL(AG_GUILD_JOIN_SUCCESS_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GUILD_JOIN_SUCCESS_CMD* pRecvMsg = (MSG_AG_GUILD_JOIN_SUCCESS_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	eZONE_STATE zsZONE = pUser->GetZoneState();
	if(ePRS_AT_CHARSELECT == zsZONE || ePRS_BEFORE_ENTER_CHARSELECT == zsZONE)
	{
		SUNLOG(eCRITICAL_LOG, _T("길드생성하자마자 캐릭터 선택창으로 돌아감!!"));
		return;
	}

	pUser->SetSelectedCharGuildGuid(pRecvMsg->m_GuildGuid);
}

HANDLER_AG_IMPL(AG_GUILD_WITHDRAW_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GUILD_WITHDRAW_ACK* pRecvMsg = (MSG_AG_GUILD_WITHDRAW_ACK *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	eZONE_STATE zsZONE = pUser->GetZoneState();
	if(ePRS_AT_CHARSELECT == zsZONE || ePRS_BEFORE_ENTER_CHARSELECT == zsZONE)
	{
		SUNLOG(eCRITICAL_LOG, _T("길드파괴하자마자 캐릭터 선택창으로 돌아감!!"));
		return;
	}

	pUser->SetSelectedCharGuildGuid(0);
}


// 길드원 강제탈퇴 처리
HANDLER_AG_IMPL(AG_GUILD_KICKPLAYER_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GUILD_KICKPLAYER_ACK* pRecvMsg = (MSG_AG_GUILD_KICKPLAYER_ACK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	User* pKickedUser = UserManager::Instance()->FindUserForCharName(pRecvMsg->m_ptszCharName);
	if(!pKickedUser)
		return;

	pKickedUser->SetSelectedCharGuildGuid(0);
}

HANDLER_AG_IMPL(AG_MISSION_STREAM_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_MISSION_STREAM_CMD* pRecvMsg = (MSG_AG_MISSION_STREAM_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MissionManager* pManager = pUser->GetMissionManager();
	MSG_CG_MISSION_STREAM_CMD msgCMD;

	// 미션 스트림 저장
	pManager->SerializeStream(pRecvMsg->m_TotalInfo, SERIALIZE_STORE);
	pManager->SerializeStream(msgCMD.m_TotalInfo, SERIALIZE_LOAD);
	pUser->SendPacket(&msgCMD, msgCMD.GetSize());
}

HANDLER_AG_IMPL(AG_MISSION_CLEAR_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_MISSION_CLEAR_CMD* pRecvMsg = (MSG_AG_MISSION_CLEAR_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MissionManager* pManager = pUser->GetMissionManager();
	// 미션 스트림 저장
	WORD wMissionPoint = pManager->GetMissionPoint(pRecvMsg->m_MissionNo) + 1;
	pManager->SetMissionPoint(pRecvMsg->m_MissionNo, wMissionPoint);
}


HANDLER_AG_IMPL(AG_CHUNTING_WIN_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHUNTING_WIN_CMD* pRecvMsg = (MSG_AG_CHUNTING_WIN_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 경쟁헌팅 전적
	pUser->SetSelectedCharCHuntingVCount(pRecvMsg->m_WinCount);
	pUser->SetSelectedCharCHuntingTotCount(pRecvMsg->m_TotCount);
}

HANDLER_AG_IMPL(AG_CHUNTING_START_COMPETITION_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHUNTING_START_COMPETITION_CMD* pRecvMsg = (MSG_AG_CHUNTING_START_COMPETITION_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 경쟁헌팅 전적
	pUser->SetSelectedCharCHuntingTotCount(pRecvMsg->m_TotCount);

	// 경쟁헌팅 시작을 알린다.
	MSG_CG_CHUNTING_START_COMPETITION_BRD msgBRD;
	pUser->SendPacket(&msgBRD, sizeof(msgBRD));
}

// GM 귓말 켬/끔
HANDLER_AG_IMPL(AG_GM_WHISPER_SET_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_WHISPER_SET_CMD* pRecvMsg = (MSG_AG_GM_WHISPER_SET_CMD *)pMsg;
	MSG_AW_GM_WHISPER_SET_CMD msgCMD;

	msgCMD.m_bOn = pRecvMsg->m_bOn;
	msgCMD.m_dwKey = pRecvMsg->m_dwKey;

	WorldServerSession* pSession = AgentServer::GetInstance()->SessionWorldServer();
	if(pSession) pSession->SendPacket(&msgCMD, sizeof(msgCMD));

	SUNLOG(eFULL_LOG,
		_T("GM 귓말 켬/끔 프로토콜 전달(G->W)[%u][bOn:%u]"),
		pRecvMsg->m_dwKey, pRecvMsg->m_bOn);
}

// 채팅금지/허용
HANDLER_AG_IMPL(AG_GM_USERCHAT_BLOCK_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_USERCHAT_BLOCK_CMD* pRecvMsg = (MSG_AG_GM_USERCHAT_BLOCK_CMD *)pMsg;
	MSG_AW_GM_USERCHAT_BLOCK_CMD msgCMD;

	msgCMD.m_dwKey = pRecvMsg->m_dwKey;
	msgCMD.m_bBlock = pRecvMsg->m_bBlock;
	msgCMD.m_CharGuid = pRecvMsg->m_CharGuid;
	msgCMD.m_dwBlockTime = pRecvMsg->m_dwBlockTime;
    //__NA_000313_20070223_REALTIME_CHAT_BLOCK

	WorldServerSession* pSession = AgentServer::GetInstance()->SessionWorldServer();
	if(pSession) pSession->SendPacket(&msgCMD, sizeof(msgCMD));

	SUNLOG(eFULL_LOG,
		_T("채팅금지 프로토콜 전달(G->W)[GM:%u][CharGuid:%u][bBlock:%u][BlockTime:%u]"),
		pRecvMsg->m_dwKey, pRecvMsg->m_CharGuid, pRecvMsg->m_bBlock, pRecvMsg->m_dwBlockTime);
}

// 캐릭터 정보 요청
HANDLER_AG_IMPL(AG_GM_CHAR_INFO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_CHAR_INFO_CMD* pRecvMsg = (MSG_AG_GM_CHAR_INFO_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	TCHAR pszCharName[MAX_CHARNAME_LENGTH+1];
    _tcsncpy(pszCharName, pRecvMsg->m_szCharName, _countof(pszCharName));
    pszCharName[_countof(pszCharName) - 1] = _T('\0');

	User* pFindUser = UserManager::Instance()->FindUserForCharName(pszCharName);
	if(!pFindUser)
	{
		MSG_CG_GM_CHARINFO_NAK msgNAK;
		msgNAK.m_byCategory = CG_GM;
		msgNAK.m_byProtocol = CG_GM_CHARINFO_NAK;
		msgNAK.m_dwErrorCode = RC::RC_GM_PLAYER_NOTEXIST;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	MSG_AG_GM_CHAR_INFO_SYN	msgSYN;
	msgSYN.m_dwGMUserKey = pUser->GetUserKey();
	pFindUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
}

HANDLER_AG_IMPL(AG_GM_CHAR_INFO_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_CHAR_INFO_ACK* pRecvMsg = (MSG_AG_GM_CHAR_INFO_ACK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwGMUserKey);	//GM캐릭터

	MSG_CG_GM_CHARINFO_ACK msgACK;
	msgACK.m_byCategory = CG_GM;
	msgACK.m_byProtocol = CG_GM_CHARINFO_ACK;

	_tcsncpy(msgACK.m_pszAccountID, pRecvMsg->m_pszAccountID, ID_MAX_LEN); // 계정 아이디.
	msgACK.m_pszAccountID[ID_MAX_LEN] = '\0';

	msgACK.m_eCharType = pRecvMsg->m_eCharType;		// 캐릭터 클래스
	msgACK.m_wLevel = pRecvMsg->m_wLevel;			// 레벨
	msgACK.m_Money = pRecvMsg->m_Money;				// 가진 돈
	msgACK.m_RoomKey = pRecvMsg->m_RoomKey;			// 방키
	msgACK.m_MapCode = pRecvMsg->m_MapCode;			// 맵코드
	memcpy(&(msgACK.m_vCurPos), &(pRecvMsg->m_vCurPos), sizeof(WzVector));	// 현재 위치

	_tcsncpy(msgACK.m_szGuildName, pRecvMsg->m_szGuildName, MAX_GUILDNAME_LENGTH);		// 길드이름
	msgACK.m_szGuildName[MAX_GUILDNAME_LENGTH] = '\0';

	GAMELOG->ReqCharInfo(pUser, msgACK);
	pUser->SendPacket(&msgACK, sizeof(msgACK));
}

class GetPlayerInfoOpr
{
	BYTE			m_byPlayerNum;
	GM_CHARINFO*	m_pCharInfo;
public:
	GetPlayerInfoOpr(GM_CHARINFO* pCharInfo) : m_pCharInfo(pCharInfo), m_byPlayerNum(0) {}
	~GetPlayerInfoOpr() {}

	inline BYTE		GetPlayerNum()	{ return m_byPlayerNum; }

	VOID operator() (User* pUser)
	{
		if((! (MSG_CG_GM_ROOMINFO_ACK::_MAX_PLAYER_INFO_SIZE > m_byPlayerNum))
			|| !m_pCharInfo)
			return;

		GM_CHARINFO& rGM_CHARINFO = m_pCharInfo[m_byPlayerNum];

		strncpy(rGM_CHARINFO.m_szCharName, pUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH);
		rGM_CHARINFO.m_byClass = pUser->GetSelectedCharClass();
		rGM_CHARINFO.m_LV = pUser->GetSelectedCharLV();
		m_byPlayerNum++;
	}
};


HANDLER_AG_IMPL(AG_GM_ROOM_INFO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_ROOM_INFO_CMD* pRecvMsg = (MSG_AG_GM_ROOM_INFO_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	ZoneInterface* pZone = g_ZoneManager.FindZone((KEYTYPE)pRecvMsg->m_dwRoomKey);
	if(!pZone)
	{
		MSG_CG_GM_ROOMINFO_NAK msgNAK;
		msgNAK.m_dwErrorCode = RC::RC_GM_ROOM_NOT_EXIST;
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	//룸일때만..체크..
	switch(pZone->GetType())
	{
	case eZONETYPE_MISSION:
	case eZONETYPE_HUNTING:
	case eZONETYPE_PVP:
	case eZONETYPE_CHUNTING:
		{
			RoomInterface* pRoom = (RoomInterface*)pZone;

			User* pMaster = pRoom->GetMaster();
			if(!pMaster) return;

			MSG_CG_GM_ROOMINFO_ACK msgACK;

			_tcsncpy(msgACK.m_szMasterName, pMaster->GetSelectedCharName(), MAX_CHARNAME_LENGTH);
			msgACK.m_szMasterName[MAX_CHARNAME_LENGTH] = '\0';
			msgACK.m_byCount = (BYTE)pRoom->GetUserNum();
			GetPlayerInfoOpr opGetInfo(msgACK.m_pPlayerInfo);
			pZone->Foreach(opGetInfo);
			pUser->SendPacket(&msgACK, msgACK.GetSize());
		}
		break;
	}
}

//채널정보
HANDLER_AG_IMPL(AG_GM_CHANNEL_INFO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_CHANNEL_INFO_CMD*	pRecvMsg = (MSG_AG_GM_CHANNEL_INFO_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	MSG_CG_GM_CHANNELINFO_ACK msgACK;
	msgACK.m_dwChannelUserNum = UserManager::Instance()->GetNumberOfUsers();
	msgACK.m_wHuntingRoomCount = (WORD)g_ZoneManager.GetNumberOfHuntingRooms();
	msgACK.m_wMissionRoomCount = (WORD)g_ZoneManager.GetNumberOfMissionRooms();

	pUser->SendPacket(&msgACK, sizeof(msgACK));
}

//방제목 변경
HANDLER_AG_IMPL(AG_GM_CHANGE_ROOMTITLE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_CHANGE_ROOMTITLE_CMD*	pRecvMsg = (MSG_AG_GM_CHANGE_ROOMTITLE_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	ZoneInterface* pZone = g_ZoneManager.FindZone(pRecvMsg->m_dwRoomKey);
	if(!pZone) return;

	//방일때만.. 의미가 있다.
	switch(pZone->GetType())
	{
	case eZONETYPE_MISSION:
	case eZONETYPE_HUNTING:
	case eZONETYPE_PVP:
	case eZONETYPE_CHUNTING:
		{
			RoomInterface* pRoom = (RoomInterface*)pZone;

			pRoom->SetRoomTitle(pRecvMsg->m_szRoomTitle);

			MSG_CG_GM_CHANGE_ROOMTITLE_ACK msgACK;
			memset(msgACK.m_szRoomTitle, 0, MAX_ROOMTITLE_LENGTH);
			strncpy(msgACK.m_szRoomTitle, pRecvMsg->m_szRoomTitle, strlen(pRecvMsg->m_szRoomTitle) + 1);
			msgACK.m_RoomKey = pRecvMsg->m_dwRoomKey;

				AgentPartyManager::Instance()->SendPacketAllMember(pUser->GetPartyState().GetPartyKey(), &msgACK, sizeof(msgACK));
		}
		break;
	default:
		{
			MSG_CG_GM_CHANGE_ROOMTITLE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_GM_ROOM_NOT_EXIST; //해당 방이 존재하지 않습니다.
			pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		}
		break;
	}
}

//강제 유저 접속종료
HANDLER_AG_IMPL(AG_GM_FORCE_DISCONNECT_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_FORCE_DISCONNECT_CMD*	pRecvMsg = (MSG_AG_GM_FORCE_DISCONNECT_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	User* pFindUser = UserManager::Instance()->FindUserForCharName(pRecvMsg->m_szCharName);
	if(!pFindUser)
	{
		MSG_CG_GM_STRING_CMD_NAK msgNAK;
		msgNAK.m_byErrorCode = RC::RC_GM_NOTEXIST_USER; // 유저가 존재하지 않습니다.
		pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		return;
	}

	GAMELOG->DisconnectCmd(pUser, pFindUser);	

	pFindUser->SetDisconnectCode(RC::RC_DISCONNECT_FROM_GM);
	pFindUser->DisconnectUser();
}

HANDLER_AG_IMPL(AG_GM_SERVER_SHUTDOWN_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_SERVER_SHUTDOWN_CMD *	pRecvMsg = (MSG_AG_GM_SERVER_SHUTDOWN_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	//MSG_CG_CONNECTION_SERVER_SHUTDOWN_BRD msgBRD;
	//msgBRD.m_dwStringCode	= pRecvMsg->m_dwStringCode;
	//msgBRD.m_byShutdownTime	= pRecvMsg->m_byShutdownTime;

	//UserPacketSender<MSG_CG_CONNECTION_SERVER_SHUTDOWN_BRD> op(msgBRD);
	//UserManager::Instance()->Foreach(op);

	AgentServer* pAgentServer = AgentServer::GetInstance();
	pAgentServer->SERVER_EXIT.TotalDelayTime() = (pRecvMsg->m_byShutdownTime*1000);
	pAgentServer->SERVER_EXIT.StartServerExitProcess();

	SUNLOG(eCRITICAL_LOG,
		_T("[%s] 서버종료시작[Start:%u, Delay:%u]"),
		pUser->GetUserID(), pRecvMsg->m_dwStringCode, pRecvMsg->m_byShutdownTime);
}

static VOID SendTrackAckMsg(User* pMoveUser, User* pTrackUser, MSG_AG_GM_GO_CMD* pRecvMsg)
{
	MSG_CG_ITEM_DELETE_CMD msgCMD;
	msgCMD.m_atIndex = SI_INVENTORY;
	msgCMD.m_atPos = pRecvMsg->m_atItemPos;

	WzVector vCurPos = pRecvMsg->m_vCurPos;
	WzVector vDestPos = pRecvMsg->m_vDestPos;

	if(pRecvMsg->IsRevenge())
	{
		msgCMD.m_byReason = RC::RC_ITEM_DELETE_REVENGE_STONE;
		pMoveUser->SendPacket(&msgCMD, sizeof(msgCMD));

		MSG_AG_CHAO_TRACK_REVENGE_CHAR_ACK msgACK;
		msgACK.m_atItemPos = pRecvMsg->m_atItemPos;
		pMoveUser->SendToLinkedServer(&msgACK, sizeof(msgACK));

		// 리벤지 추적 로그
		GAMELOG->TrackRevenge(pMoveUser, pTrackUser, vCurPos, vDestPos);
	}
	else if(pRecvMsg->IsItem())
	{
	    //__NA001164_20081024_CHAR_WAY_POINT_ITEM
		if(pRecvMsg->m_PortalType != ePORTAL_TYPE_ITEM_EX)
		{
			msgCMD.m_byReason = RC::RC_ITEM_DELETE_PORTAL_SUCCESS;
			pMoveUser->SendPacket(&msgCMD, sizeof(msgCMD));
		}
		MSG_AG_ITEM_PORTAL_ACK msgACK;
		msgACK.m_PortalPos = pRecvMsg->m_atItemPos;
        msgACK.m_ItemCode = pRecvMsg->m_atItemCode;
		pMoveUser->SendToLinkedServer(&msgACK, sizeof(msgACK));

		// Portal 로그
		GAMELOG->Portal(pMoveUser, pRecvMsg->m_FieldCode, vCurPos, vDestPos);
	}
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    else if(pRecvMsg->IsFriendInvitatedEvent())
    {
        MSG_AG_EVENT_RECALL_ACK Msg;
        Msg.CalleeGUID = pMoveUser->GetSelectedCharGuid();
        pMoveUser->SendToLinkedServer(&Msg, sizeof(Msg));
    }
}

VOID SendTrackNakMsg(User *pUser, ePORTAL_TYPE ePortalType, BYTE ErrorCode)
{
	switch(ePortalType)
	{
	case ePORTAL_TYPE_GM:
		{
			MSG_CG_GM_GO_NAK msgNAK;
			msgNAK.m_byErrorCode = ErrorCode;
			pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		}
		break;
	case ePORTAL_TYPE_REVENGE:
		{
			MSG_CG_CHAO_TRACK_REVENGE_CHAR_NAK NakMsg;
			NakMsg.m_byErrorCode = ErrorCode;
			pUser->SendPacket(&NakMsg, sizeof(NakMsg));
		}
		break;
	case ePORTAL_TYPE_ITEM:
		{
			MSG_CG_ITEM_USE_NAK NakMsg;
			NakMsg.m_ErrorCode = ErrorCode;
			pUser->SendPacket(&NakMsg, sizeof(NakMsg));
		}
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    case ePORTAL_TYPE_FIRENDINVITATED_EVENT:
        {
            MSG_AG_EVENT_RECALL_NAK Msg;
            Msg.ErrorCode = ErrorCode;
            pUser->SendToLinkedServer(&Msg, sizeof(Msg));
        }
		break;
	}
}

HANDLER_AG_IMPL(AG_GM_GO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_GO_CMD* pRecvMsg = (MSG_AG_GM_GO_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

    const BOOLEAN isRevenge = !!pRecvMsg->IsRevenge();
    const BOOLEAN isItem = !!pRecvMsg->IsItem();
    const BOOLEAN isFriendInvitatedEvent = !!pRecvMsg->IsFriendInvitatedEvent();

	// 이동시키려는 유저를 찾는다.
	User* pFindUser = UserManager::Instance()->FindUserForCharName(pRecvMsg->m_szCharName);
	if(!pFindUser)
	{
		if(isRevenge)   SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_NO_CHAR);
		else if(isItem) SendTrackNakMsg(pUser, ePORTAL_TYPE_ITEM, RC::RC_ITEM_CANNOT_FIND_PLAYER_TO_MOVE);
        // added rule to support _KR_0_20091021_FIRENDINVITATION_EXTEND
        else if(isFriendInvitatedEvent) SendTrackNakMsg(pUser, ePORTAL_TYPE_FIRENDINVITATED_EVENT, 
                                                        RC::RC_ITEM_CANNOT_FIND_PLAYER_TO_MOVE);
		else            SendTrackNakMsg(pUser, ePORTAL_TYPE_GM, RC::RC_GM_PLAYER_NOTEXIST);
		return;
	}
    // added by __CN_000001_20090513_ADD_FRIEND_CHECK_GM
    else
    {
        if( isRevenge &&
            TRUE  == pFindUser->IsGM() &&
            FALSE == pUser->IsGM() )   
        {
            SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_NO_CHAR);
            return;
        }
    }
    // (WAVERIX) (090410) (BUG-FIX) WayPoint등등에서 이뤄지는 이동 트랜젝션 상태 검사 추가
    if(pFindUser->IsBeginTransaction())
    {
        if(isRevenge)   SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_MOVEZONE_FAIL);
        else if(isItem) SendTrackNakMsg(pUser, ePORTAL_TYPE_ITEM, RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
        // added rule to support _KR_0_20091021_FIRENDINVITATION_EXTEND
        else if(isFriendInvitatedEvent) SendTrackNakMsg(pUser, ePORTAL_TYPE_FIRENDINVITATED_EVENT, 
                                                        RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
        else            SendTrackNakMsg(pUser, ePORTAL_TYPE_GM, RC::RC_GM_MOVEZONE_FAIL);
        return;
    }

	// 현재 존을 찾는다.
	ZoneInterface *pFromZone = g_ZoneManager.FindZone(pFindUser->GetZoneKey());
	if(!pFromZone)
	{
        if(isRevenge)   SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_CHAR_INVALID_STATE);
        else if(isItem) SendTrackNakMsg(pUser, ePORTAL_TYPE_ITEM, RC::RC_ITEM_INVALIDSTATEOFPLAYER);
        // added rule to support _KR_0_20091021_FIRENDINVITATION_EXTEND
        else if(isFriendInvitatedEvent) SendTrackNakMsg(pUser, ePORTAL_TYPE_FIRENDINVITATED_EVENT, 
                                                        RC::RC_ITEM_INVALIDSTATEOFPLAYER);
        else            SendTrackNakMsg(pUser, ePORTAL_TYPE_GM, RC::RC_GM_USER_STATE_INVALID);
		return;
	}

	// 이동하려는 존을 찾는다.
	ZoneInterface* pToZone = NULL;
	if(pRecvMsg->m_RoomKey)			// 배틀룸
	{
		pToZone = g_ZoneManager.FindZone(pRecvMsg->m_RoomKey);
	}
	else if(pRecvMsg->m_FieldCode)	// 마을, 필드
	{
		pToZone = g_ZoneManager.FindStaticZone(pRecvMsg->m_FieldCode);
	}
	else	// RoomKey, FieldCode 모두 0이면 현재 필드에서의 Teleport를 의미한다.
	{
		SendTrackAckMsg(pFindUser, pUser, pRecvMsg);

		MSG_AG_GM_TELEPORT_CMD msgCMD;
		msgCMD.m_vDestPos = pRecvMsg->m_vDestPos;
		pFindUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));
		return;
	}

	if(pToZone)
	{
		// 같은 존으로의 이동이면 텔레포트한다.
		if(pFromZone->GetKey() == pToZone->GetKey())
		{
			SendTrackAckMsg(pFindUser, pUser, pRecvMsg);

			MSG_AG_GM_TELEPORT_CMD msgCMD;
			msgCMD.m_vDestPos = pRecvMsg->m_vDestPos;
			pFindUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));
			return;
		}
		// 방으로의 이동은 아직 지원하지 않는다.
		else
		{
			eZONETYPE ztFROM = pFromZone->GetType();
			eZONETYPE ztTO = pToZone->GetType();

 			if( ((eZONETYPE_FIELD != ztFROM) && (eZONETYPE_DOMINATION_FIELD != ztFROM) && (eZONETYPE_VILLAGE != ztFROM) && (eZONETYPE_SPA_FIELD != ztFROM)) ||
				((eZONETYPE_FIELD != ztTO)   && (eZONETYPE_DOMINATION_FIELD != ztTO)   && (eZONETYPE_VILLAGE != ztTO) && (eZONETYPE_SPA_FIELD != ztTO)) )
			{
                if(isRevenge)   SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_INVALID_ROOM);
                else if(isItem) SendTrackNakMsg(pUser, ePORTAL_TYPE_ITEM, RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
                // added rule to support _KR_0_20091021_FIRENDINVITATION_EXTEND
                else if(isFriendInvitatedEvent) SendTrackNakMsg(pUser, ePORTAL_TYPE_FIRENDINVITATED_EVENT, 
                                                                RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
                else            SendTrackNakMsg(pUser, ePORTAL_TYPE_GM, RC::RC_GM_CANNOT_MOVE_TO_BATTLEROOM);
				return;
			}
			// 리벤지 워프일 경우 마을로는 이동하지 못한다.
			else if(isRevenge && (eZONETYPE_VILLAGE == ztTO))
			{
				SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_INVALID_ROOM);
				return;
			}
		}
	}
	else
	{
        if(isRevenge)   SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_INVALID_ROOM);
        else if(isItem) SendTrackNakMsg(pUser, ePORTAL_TYPE_ITEM, RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
        // added rule to support _KR_0_20091021_FIRENDINVITATION_EXTEND
        else if(isFriendInvitatedEvent) SendTrackNakMsg(pUser, ePORTAL_TYPE_FIRENDINVITATED_EVENT, 
                                                        RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
        else            SendTrackNakMsg(pUser, ePORTAL_TYPE_GM, RC::RC_GM_ROOM_NOT_EXIST);
		return;
	}

	const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(pToZone->GetMapCode());
	if(isRevenge && pMapInfo)
	{
		if(pMapInfo->minLV > pFindUser->GetCharLV() ||
			(0 != pMapInfo->maxLV &&
			pMapInfo->maxLV < pFindUser->GetCharLV()))
		{
			SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_INVALID_ZONE_LEVEL);
			return;
		}
	}

	WORD wFromMapCode = 0;
	if(pFromZone)
		wFromMapCode = pFromZone->GetMapCode();

	WzVector rDestPos = pRecvMsg->m_vDestPos;
	// 해당 존으로 이동

	// 해당 존으로 이동
	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_STATIC_TO_STATIC>::AlignInfo(
			pFindUser,
			//{         From		  |          To         }
			pFindUser->GetZoneType(),   pToZone->GetType(),	//	eZONETYPE
			pFindUser->GetZoneKey(),    pToZone->GetKey()	//	ZoneKey
			);
	RoomArgument args(eZONEPUBLIC_PUBLIC, NULL, pRecvMsg->m_FieldCode, 0, &rDestPos);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		SendTrackAckMsg(pFindUser, pUser, pRecvMsg);

		MSG_CG_ZONE_GMPORTAL_FIELD_MOVE_CMD msgCMD;
		msgCMD.m_MapCode = pRecvMsg->m_FieldCode;
		pFindUser->SendPacket(&msgCMD, sizeof(msgCMD));

		GAMELOG->MoveVillage(pUser, wFromMapCode, pRecvMsg->m_FieldCode, FALSE);
	}
	else
	{
		if(pRecvMsg->IsRevenge())		SendTrackNakMsg(pFindUser, ePORTAL_TYPE_REVENGE, RC::RC_REVENGE_MOVEZONE_FAIL);
		else if(pRecvMsg->IsItem())	SendTrackNakMsg(pUser, ePORTAL_TYPE_ITEM, RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
        // added rule to support _KR_0_20091021_FIRENDINVITATION_EXTEND
        else if(isFriendInvitatedEvent) SendTrackNakMsg(pUser, ePORTAL_TYPE_FIRENDINVITATED_EVENT, 
                                                        RC::RC_ITEM_CANNOT_MOVE_TO_THE_AREA);
		else							SendTrackNakMsg(pUser, ePORTAL_TYPE_GM, RC::RC_GM_MOVEZONE_FAIL);
	}
}

HANDLER_AG_IMPL(AG_GM_TRACKING_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_TRACKING_CMD*	pRecvMsg = (MSG_AG_GM_TRACKING_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);		//GM캐릭터

	User* pFindUser = UserManager::Instance()->FindUserForCharName(pRecvMsg->m_szCharName);
	if(!pFindUser)
	{
		if(ePORTAL_TYPE_REVENGE == pRecvMsg->m_PortalType)
		{
			MSG_CG_CHAO_TRACK_REVENGE_CHAR_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;
			pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		}
		else
		{
			MSG_CG_GM_TRACKING_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_GM_PLAYER_NOTEXIST;
			pUser->SendPacket(&msgNAK, sizeof(msgNAK));
		}
		return;
	}

	// 추적할 대상의 위치정보를 요청한다.
	MSG_AG_GM_TRACKING_REQUEST_INFO_SYN msgSYN;
	_tcsncpy(msgSYN.m_szGMCharName, pUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH);
	msgSYN.m_szGMCharName[MAX_CHARNAME_LENGTH] = _T('\0');
	msgSYN.m_vCurPos = pRecvMsg->m_vCurPos;
	msgSYN.m_PortalType = pRecvMsg->m_PortalType;
	msgSYN.m_atItemPos = pRecvMsg->m_atItemPos;
	pFindUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// AG_CHAO
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 리벤지 정보 요청
HANDLER_AG_IMPL(AG_CHAO_REQUEST_REVENGEINFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REQUEST_REVENGEINFO_SYN* pRecvMsg = (MSG_AG_CHAO_REQUEST_REVENGEINFO_SYN*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 게임서버의 요청을 World에 전달한다.
	MSG_AW_CHAO_REQUEST_REVENGEINFO_SYN msgSYN;
	msgSYN.m_dwKey = pUser->GetUserGUID();
	msgSYN.m_dwCharGuid = pUser->GetSelectedCharGuid();
	pUser->SendToWorldServer(&msgSYN, sizeof(msgSYN));
}

// 리벤지 대상 한명 제거(PK를 통해 제거했을 때)
HANDLER_AG_IMPL(AG_CHAO_REMOVE_REVENGE_CHAR_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_REMOVE_REVENGE_CHAR_SYN* pRecvMsg = (MSG_AG_CHAO_REMOVE_REVENGE_CHAR_SYN*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 게임서버의 요청을 World에 전달한다.
	MSG_AW_CHAO_REMOVE_REVENGE_CHAR_SYN msgSYN;
	msgSYN.m_dwCharGuid = pUser->GetSelectedCharGuid();
	msgSYN.m_dwRevengeCharGuid = pRecvMsg->m_dwRevengeCharGuid;
	pUser->SendToWorldServer(&msgSYN, sizeof(msgSYN));
}

// 리벤지 대상 추가 요청
HANDLER_AG_IMPL(AG_CHAO_ADD_REVENGE_CHAR_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_ADD_REVENGE_CHAR_SYN* pRecvMsg = (MSG_AG_CHAO_ADD_REVENGE_CHAR_SYN*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// 게임서버의 요청을 World에 전달한다.
	MSG_AW_CHAO_ADD_REVENGE_CHAR_SYN msgSYN;
	msgSYN.m_dwCharGuid = pUser->GetSelectedCharGuid();
	_tcsncpy(msgSYN.m_ptszRevengeName, pRecvMsg->m_ptszRevengeName, MAX_CHARNAME_LENGTH);
	msgSYN.m_ptszRevengeName[MAX_CHARNAME_LENGTH] = _T('\0');
	pUser->SendToWorldServer(&msgSYN, sizeof(msgSYN));
}

// 카오상태 변경
HANDLER_AG_IMPL(AG_CHAO_CHANGE_CHAO_STATE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_CHAO_CHANGE_CHAO_STATE_CMD* pRecvMsg = (MSG_AG_CHAO_CHANGE_CHAO_STATE_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	// Agent 카오상태 갱신
	pUser->SetSelectedCharChaoState(pRecvMsg->m_byChaoState);

	// 파티원들에게 카오상태 변화를 알린다.
	AgentPartyManager::Instance()->MemberChangeChaoState(pUser);

	// 클라에 전달
	MSG_CG_CHAO_CHANGE_CHAO_STATE_BRD msgBRD;
	msgBRD.m_dwObjKey = pRecvMsg->m_dwObjKey;
	msgBRD.m_byChaoState = pRecvMsg->m_byChaoState;
	pUser->SendPacket(&msgBRD, sizeof(msgBRD));
}

// 스페셜 아이템 소유 변경
HANDLER_AG_IMPL(AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD* pRecvMsg = (MSG_AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	ItemOwnFlag& rOwnFlag = pUser->GetItemOwnFlag();
	if(pRecvMsg->m_Apply)
		rOwnFlag.IncItemInfo((eITEMTYPE)pRecvMsg->m_ItemType, pRecvMsg->m_Count);
	else
		rOwnFlag.DecItemInfo((eITEMTYPE)pRecvMsg->m_ItemType, pRecvMsg->m_Count);

	// 월드서버에서 필요한 아이템정보에 대해 알려준다.
	if(rOwnFlag.IsSendToWorldServer((eITEMTYPE)pRecvMsg->m_ItemType))
	{
		MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD msgCMD;
		msgCMD.m_dwCharGuid = pUser->GetSelectedCharGuid();
		msgCMD.m_Count = 1;
		msgCMD.m_ItemInfo[0].m_Apply = pRecvMsg->m_Apply;
		msgCMD.m_ItemInfo[0].m_ItemType = pRecvMsg->m_ItemType;
		msgCMD.m_ItemInfo[0].m_Count = pRecvMsg->m_Count;
		pUser->SendToWorldServer(&msgCMD, msgCMD.GetSize());
	}
}


static VOID
StatusRestoration_By_Create_InstanceDungeon_Reply(
	const MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK::eRet error_group, const DWORD error_code,
    User* const pUser)
{
	switch(error_group)
	{
	case MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK::eErr_RoomRC:
		{
			switch(error_code)
			{
			case RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER:
			case RC::RC_ROOM_NEED_UNEQUIP_WEAPON_FOR_ENTER:
            case RC::RC_ROOM_INVALID_MAPCODE: // (BUGFIX) (091211) (WAVERIX) 기획 스크립트 설정상 (유지 보수 cost) 아이템을 막지는 않는다.
				{
					//ItemUnLock
					MSG_AG_ITEM_UNLOCK_CMD msgCMD;
					msgCMD.m_ItemIndex  = SI_INVENTORY;
					msgCMD.m_ItemPos    = pUser->GetLockItemPosForInstanceDungeon();
                    // (f100528.2L) prevent a locked state
                    // after an instance dungeon moving transaction failed
                    msgCMD.post_action_ = msgCMD.eAction_MoveFailed;
					pUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));

					pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
				}
				break;
			default:
				{
                    // (f100528.2L) enforced a rule
                    if (pUser->IsBeginTransaction()) {
                        eBEHAVE_TRANSACTION trans_state = pUser->GetTransactionState();
                        if (trans_state == TR_CREATE_INSTANCE_DUNGEON) {
                            pUser->EndTransaction();
                        }
                    }

					const POSTYPE item_pos = pUser->GetLockItemPosForInstanceDungeon();
					if(INVALID_POSTYPE_VALUE != item_pos)
					{	//ItemUnLock
						MSG_AG_ITEM_UNLOCK_CMD msgCMD;
						msgCMD.m_ItemIndex  = SI_INVENTORY;
						msgCMD.m_ItemPos    = item_pos;
                        // (f100528.2L) prevent a locked state
                        // after an instance dungeon moving transaction failed
                        msgCMD.post_action_ = msgCMD.eAction_MoveFailed;
						pUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));
					}

					pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
				}
			break;
			}
		}
		break;
	case MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK::eErr_ItemRC:
		{
			switch(error_code)
			{
			case RC::RC_ITEM_UNUSABLE_FUNCTION:
			case RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE:
				{
					//ItemUnLock
					MSG_AG_ITEM_UNLOCK_CMD msgCMD;
					msgCMD.m_ItemIndex		= SI_INVENTORY;
					msgCMD.m_ItemPos		= pUser->GetLockItemPosForInstanceDungeon();
                    // (f100528.2L) prevent a locked state
                    // after an instance dungeon moving transaction failed
                    msgCMD.post_action_ = msgCMD.eAction_MoveFailed;
					pUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));

					pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
				}
			break;
			}
		break;
		}
	}
}

HANDLER_AG_IMPL(AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK* const pRecvMsg =
		static_cast<MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK*>(pMsg);
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	const MAPCODE map_code = pRecvMsg->m_MapCode;
	const sMAPINFO* const pMapInfo = MapInfoParser::Instance()->FindMapInfo(map_code);
    //	(WAVERIX) (NOTE) Assertion: (pMapInfo != 0)
    // 위 단정이 실패하는 경우는 맵 관련 스크립트가 잘못되었을 경우이며,
    //반드시 해결하고 넘어갈 수 있도록 뻗도록 방치하기로 한다.
    //라이브에서 잘못될 가능성은 없어야 한다.
	const eZONE_RULESET& rRuleset = pMapInfo->Ruleset;

	static MemberOperator Opr;
	AgentPartyManager* const pPartyManager = AgentPartyManager::Instance();

	FlowControl flow;
	FLOWCONTROL_START()
    {
		Opr.Init(ePARTY_OPERATOR_MIGRATE_CHECK, 0, 0);
		pPartyManager->For_eachMembers(pUser->GetPartyState().GetPartyKey(), Opr);
        // (WAVERIX) (090402) (BUG-FIX) 파티원 로딩중 저탑 입장시 클라 종료 현상
        IfFailThenBreak(Opr.IsSuccess(), RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER);

		if(rRuleset.rules & rRuleset.ZONE_RULE_INDUN_SHADOW_NOT_PLAYING_MODE)
		{
			IfFailThenBreak(eCHAR_SHADOW != pUser->GetCharClass(),
                            RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER);

			Opr.Init(ePARTY_OPERATOR_SHADOW_CHECK, 0, 0);
			pPartyManager->For_eachMembers(pUser->GetPartyState().GetPartyKey(), Opr);
			IfFailThenBreak(Opr.IsSuccess(), RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER);
		}

		if(rRuleset.rules & rRuleset.ZONE_RULE_INDUN_SINGLE_PLAYING_MODE)
			pPartyManager->LeaveParty(pUser, eREASON_LEAVE_PARTY_INTEND);

		pUser->BeginTransaction(TR_CREATE_INSTANCE_DUNGEON);

		DungeonTopOfTrial* pDungeonTopOfTrial = NULL;
		const RC::eROOM_RESULT rt =
            g_ZoneManager.CreateInstanceDungeon(pUser->GetZoneKey(), pUser,
                                                map_code, (InstanceDungeon *&)pDungeonTopOfTrial,
                                                INDUN_KIND_TOP_OF_TRIAL, // 기본 sub_type
                                                pRecvMsg->m_UseCharge);

		IfFailThenBreak(rt == RC::RC_ROOM_SUCCESS, rt);

		{
			//Game서버에 알리고
			MSG_AG_ITEM_DECREASE_UNLOCK_CMD msgCMD;
			msgCMD.m_ItemIndex	= SI_INVENTORY;
			msgCMD.m_ItemPos	= pUser->GetLockItemPosForInstanceDungeon();
			pUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));
		};
		{
			//클라이언트에게 알린다.
			MSG_CG_ITEM_DELETE_CMD msgCMD;
			msgCMD.m_byReason = RC::RC_ITEM_DELETE_USE_KEY_SUCCESS;
			msgCMD.m_atIndex = SI_INVENTORY;
			msgCMD.m_atPos = pUser->GetLockItemPosForInstanceDungeon();
			pUser->SendPacket(&msgCMD, sizeof(msgCMD));
		};

		pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
		pUser->EndTransaction();

		ZoneInterface* const pZoneInterface = g_ZoneManager.FindZone(pUser->GetZoneKey());		
		if(!pZoneInterface)
			return; // hummm...

        pPartyManager->MigrateToInstanceDungeon(pUser, pDungeonTopOfTrial);

		return;
    }
    FLOWCONTROL_END;
    
    StatusRestoration_By_Create_InstanceDungeon_Reply(
        MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK::eErr_RoomRC, flow.GetErrorCode(), pUser);


	//	인던 생성 실패
	MSG_CG_ZONE_INSTANCE_DUNGEON_REQUEST_RESULT_NAK msgNAK;
	msgNAK.m_byKind			= 1;   //RC::eROOM_RESULT
	msgNAK.m_dwErrorCode	= flow.GetErrorCode();
	pUser->SendPacket(&msgNAK, sizeof(msgNAK));
}

HANDLER_AG_IMPL(AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK* const pRecvMsg =
		static_cast<MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK*>(pMsg);

	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    StatusRestoration_By_Create_InstanceDungeon_Reply(
        MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK::eRet(pRecvMsg->m_ErrorKind),
        pRecvMsg->m_dwErrorCode, pUser);

	MSG_CG_ZONE_INSTANCE_DUNGEON_REQUEST_RESULT_NAK msgNAK;
	msgNAK.m_byKind = pRecvMsg->m_ErrorKind != pRecvMsg->eErr_ItemRC;

	msgNAK.m_dwErrorCode = pRecvMsg->m_dwErrorCode;
	pUser->SendPacket(&msgNAK, sizeof(msgNAK));

	//////////////////////////////////////////////////////////////////////////
	//	인던 세부 룰에 따라서 
}


HANDLER_AG_IMPL(AG_ITEM_USE_DECREE_ITEM_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_USE_DECREE_ITEM_ACK* pRecvMsg = (MSG_AG_ITEM_USE_DECREE_ITEM_ACK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AW_ITEM_USE_DECREE_ITEM_ACK msgACK;
	msgACK.m_dwKey = pRecvMsg->m_dwKey;

	BOOL bSuccess = pUser->SendToWorldServer(&msgACK, sizeof(msgACK));
	if(!bSuccess)
	{
		//만일 전송 실패면은 위에서 아이템은 소모 되었으므로, 복구 로그를 남긴다.
		//복그 로그 
	}
}

HANDLER_AG_IMPL(AG_ITEM_USE_DECREE_ITEM_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ITEM_USE_DECREE_ITEM_NAK* pRecvMsg = (MSG_AG_ITEM_USE_DECREE_ITEM_NAK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AW_ITEM_USE_DECREE_ITEM_NAK msgNAK;
	msgNAK.m_dwKey = pRecvMsg->m_dwKey;
	msgNAK.m_byErrorCode = pRecvMsg->m_byErrorCode;
	pUser->SendToWorldServer(&msgNAK, sizeof(msgNAK));
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// 하임행운 캐시 응모권 사용 요청
HANDLER_AG_IMPL(AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN* msg = \
        static_cast<const MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN*>(pMsg);

    User* user = UserManager::Instance()->GetUserByCharGuid(msg->char_guid);
    if (user == NULL)
    {
        MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
        ack.result_code = RC::RC_ITEM_INVALIDSTATEOFPLAYER;
        ack.char_guid   = msg->char_guid;
        ack.index       = msg->index;
        ack.pos         = msg->pos;
        pServerSession->SendPacket(&ack, (WORD)sizeof(ack));
        return;
    }

    // 월드 서버로 중계
    MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN syn;
    syn.char_guid = msg->char_guid;
    syn.index     = msg->index;
    syn.pos       = msg->pos;
    user->SendToWorldServer(&syn, sizeof(syn));
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

HANDLER_AG_IMPL(AG_STATUS_INTERCEPTION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_STATUS_INTERCEPTION_SYN* pRecvMsg = (MSG_AG_STATUS_INTERCEPTION_SYN*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AW_USER_STATUS_INTERCEPTION_SYN synMSG;
	synMSG.m_dwKey				= pRecvMsg->m_dwKey;
	synMSG.m_byInterceptionCode	= pRecvMsg->m_byInterceptionCode;
	synMSG.m_bFlag				= pRecvMsg->m_bRequestFlag;

	if(!pUser->SendToWorldServer(&synMSG, sizeof(synMSG)))
	{
		MSG_AG_STATUS_INTERCEPTION_NAK nakMSG;

		nakMSG.m_dwKey = pRecvMsg->m_dwKey;
		nakMSG.m_byInterceptionCode = pRecvMsg->m_byInterceptionCode;
		nakMSG.m_byErrorCode = RC::RC_STATUS_WHISPER_INTERCEPTION_FAILED;

		pUser->SendToLinkedServer(&nakMSG, sizeof(nakMSG));
	}
}

HANDLER_AG_IMPL(AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // CHANGES: f110531.4L, remove the broadcast upper boundary limitation using 1toN broadcaster
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN* recv_msg =
        static_cast<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN*>(pMsg);

    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::MsgBodyInfo msg_info;
    uint16_t calculated_size = recv_msg->GetPacketInfo(&msg_info);
    if (FlowControl::FCAssert(msg_info.msg_send_size != 0 && calculated_size == wSize) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("SENDING/RECVED Broadcast Msg is not Matched %u %u [%3u:%3u]"),
               calculated_size, wSize,
               msg_info.msg_send->m_byCategory, msg_info.msg_send->m_byProtocol);
        return;
    };

    const int upperbound = msg_info.number_of_users;
    const uint16_t* object_keys = msg_info.user_object_keys;

    if (upperbound == 0) {
        assert(!"check sender 'game server'");
        return;
    };

    nsPacket::PacketInfo fixed_packet_info =
        nsPacket::PacketControl::Pack(msg_info.msg_send, msg_info.msg_send_size);
    msg_info.msg_send = static_cast<MSG_BASE_FORWARD*>(fixed_packet_info.pMsg);
    msg_info.msg_send_size = fixed_packet_info.Size;
    //
    UserManager* const user_manager = UserManager::Instance();
    for (int index = 0; index < upperbound; ++index, ++object_keys)
    {
        DWORD object_key = *object_keys;
        User* const user = user_manager->GetUserByObjKey(object_key);
        if (user)
        {
            user->SendPacket(msg_info.msg_send, msg_info.msg_send_size, false);
        };
    };
#if SUN_CODE_BACKUP

	MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN* pRecvBrdMsg = (MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN*)pMsg;
	// 이것은 이 안의 패킷의 값에 그 의미가 존재한다. 하지만 전송자가 없어도 되는지는 확인할 필요 있음
	//FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN::MSG_INFO rINFO;
	WORD wCalcSize = pRecvBrdMsg->GetPacketInfo(rINFO);

#	ifdef _DEBUG
	ASSERT(rINFO.wSIZE && (wCalcSize == wSize));
#	endif

	if(wCalcSize != wSize)
	{
		MSG_BASE_FORWARD*& rpMSG = rINFO.pMSG_BODY;
		SUNLOG(eCRITICAL_LOG, _T("SENDING/RECVED Broadcast Msg is not Matched %u %u [%3u:%3u]"),
			wCalcSize, wSize, rpMSG->m_byCategory, rpMSG->m_byProtocol);

		return;
	}

	const DWORD upperbound = rINFO.wUSER_COUNT;
	DWORD* pKeys = rINFO.pUSERs;

	using namespace nsPacket;

	if(!upperbound)
		return;

	PacketInfo packetInfo = PacketControl::Pack(rINFO.pMSG_BODY, rINFO.wSIZE);
	//if(rINFO.wSIZE > 80)
	//	printf("PackingInfo src:pack = { %4u:%4u } - %.2f\n", rINFO.wSIZE, packetInfo.Size, ((FLOAT)packetInfo.Size/rINFO.wSIZE)*100.f);
	rINFO.pMSG_BODY = (MSG_BASE_FORWARD*)packetInfo.pMsg;
	rINFO.wSIZE = packetInfo.Size;

	UserManager& rMGR = *UserManager::Instance();
	for(DWORD dwLoop=0 ; dwLoop<upperbound ; ++dwLoop, ++pKeys)
	{
		User* const pFindUser = rMGR.GetUserByObjKey(*pKeys);
		if(pFindUser)
		{
			pFindUser->SendPacket(rINFO.pMSG_BODY, rINFO.wSIZE, FALSE);
		}
	}
#endif //SUN_CODE_BACKUP
}

HANDLER_AG_IMPL(AG_GM_NOTICE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_GM_NOTICE_CMD *pRecvMsg = (MSG_AG_GM_NOTICE_CMD *)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	MSG_AW_GM_NOTICE_CMD msgCMD;

	msgCMD.m_dwKey	= pRecvMsg->m_dwKey;
    msgCMD.m_eType	= pRecvMsg->m_eType;
	msgCMD.m_wLen	= pRecvMsg->m_wLen;
	_tcsncpy(msgCMD.m_tszNoticeMsg, pRecvMsg->m_tszNoticeMsg, (sizeof(TCHAR)*MAX_NOTICE_LEN+1));
	msgCMD.m_tszNoticeMsg[MAX_NOTICE_LEN] = '\0';

	//pUser->SendToWorldServer(&msgCMD, sizeof(msgCMD));
	pUser->SendToWorldServer(&msgCMD, msgCMD.GetSize());
}

// impelmented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = { AG_GM_LOTTO~ }
HANDLER_AG_IMPL(AG_GM_LOTTO_NUM_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_GM_LOTTO_NUM_CMD* pRecvMsg = (MSG_AG_GM_LOTTO_NUM_CMD*)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->dwUserKey);

    MSG_AW_GM_LOTTO_NUM_CMD cmd;
    cmd.dwUserKey = pRecvMsg->dwUserKey;
    pUser->SendToWorldServer(&cmd, sizeof(cmd));
}

HANDLER_AG_IMPL(AG_GM_LOTTO_SETTLE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_GM_LOTTO_SETTLE_CMD* pRecvMsg = (MSG_AG_GM_LOTTO_SETTLE_CMD*)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->dwUserKey);

    MSG_AW_GM_LOTTO_SETTLE_CMD cmd;
    cmd.dwUserKey = pRecvMsg->dwUserKey;
    pUser->SendToWorldServer(&cmd, sizeof(cmd));
}

HANDLER_AG_IMPL(AG_GM_LOTTO_FUND_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_GM_LOTTO_FUND_CMD* pRecvMsg = (MSG_AG_GM_LOTTO_FUND_CMD*)pMsg;
    FINDnALIGN_USER(pUser, pRecvMsg->user_key);

    MSG_AW_GM_LOTTO_FUND_CMD cmd;
    cmd.user_key = pRecvMsg->user_key;
    cmd.amount_delta = pRecvMsg->amount_delta;
    pUser->SendToWorldServer(&cmd, sizeof(cmd));
}

HANDLER_AG_IMPL(AG_GM_DOMINATION_INIT_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_GM_DOMINATION_INIT_CMD* pRecvMsg = (MSG_AG_GM_DOMINATION_INIT_CMD*)pMsg;


    if (pRecvMsg->m_flag == MSG_AG_GM_DOMINATION_INIT_CMD::time)
    {
        if (FALSE != pRecvMsg->m_bMode)
            DominationInfoParser::Instance()->GMClearEventInfo();
        else
            DominationInfoParser::Instance()->GMInsertEventInfo(pRecvMsg->m_Info);
    }
    else if (pRecvMsg->m_flag == MSG_AG_GM_DOMINATION_INIT_CMD::observer)
    {
        const DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP& script_infos = 
            DominationInfoParser::Instance()->GetDominationInfoMap();
        const MAPCODE map_code = script_infos.begin()->first;

        DominationField* const field = g_ZoneManager.FindDominationField(map_code);
        if (field == NULL)
        {
            return;
        }
        field->is_gm_observer = pRecvMsg->m_bMode;
    }
    else
    {
        return;
    }
}

HANDLER_AG_IMPL(AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// FlowControl 사용 시, 정의되지 않고 로직단에서 처리하는 일반적인 반환값에 대한 선언
	const DWORD 	RC_ROOM_FLOW_CONTROL_DEFAULT_RETURN_VALUE = RC::RC_ROOM_FAILED + 100000;
	const DWORD 	RC_ROOM_SEND_FAIL_MESSAGE_TO_USER = RC::RC_ROOM_FAILED + 100001;

	MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK* pRecvMsg = (MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK*)pMsg;
	FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK

		IfFailThenBreak(
            MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::SUCCESS == pRecvMsg->m_Result,
            RC_ROOM_SEND_FAIL_MESSAGE_TO_USER);

        AgentPartyManager* pPartyManager = AgentPartyManager::Instance();

		static MemberOperator Opr;
		Opr.Init(ePARTY_OPERATOR_MIGRATE_CHECK, 0, 0);
		pPartyManager->For_eachMembers(pUser->GetPartyState().GetPartyKey(), Opr);

		IfFailThenBreak(Opr.IsSuccess(), RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER);

		pUser->BeginTransaction(TR_CREATE_INSTANCE_DUNGEON);

		//// 파티관련해서 인던을 만들 수 있는 상태인가?
		//RC::eROOM_RESULT rt = pUser->GetPartyState().CanCreateTowerOfTrial();
		//IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, (DWORD)rt);

		Milestone100LevelQuest* p100LevelQuest = NULL;
		RC::eROOM_RESULT rt = g_ZoneManager.CreateInstanceDungeon(pUser->GetZoneKey(), pUser, pRecvMsg->RETINFO_SUCCESS.m_MapCode,
				(InstanceDungeon*&)p100LevelQuest, INDUN_KIND_100LEVEL_QUEST);

		IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, (DWORD)rt); // resource 해제 필요

		{
			//Game서버에 알리고
			MSG_AG_ITEM_DECREASE_UNLOCK_CMD msgCMD;
			msgCMD.m_ItemIndex	= SI_INVENTORY;
			msgCMD.m_ItemPos	= pUser->GetLockItemPosForInstanceDungeon();
			pUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));
		};
		{
			//클라이언트에게 알린다.
			MSG_CG_ITEM_DELETE_CMD msgCMD;
			msgCMD.m_byReason = RC::RC_ITEM_DELETE_USE_KEY_SUCCESS;
			msgCMD.m_atIndex = SI_INVENTORY;
			msgCMD.m_atPos = pUser->GetLockItemPosForInstanceDungeon();
			pUser->SendPacket(&msgCMD, sizeof(msgCMD));
		};

        // QuestItem은... 계속 유지해야 할 것인가?
		pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);

		//	<WARNING!!>
		//		방장 뿐만 아니라 파티의 모든 사람의 QuestItem Lock이 요구됨.
		//		파티원들 중에서 조건에 만족되는 유저들 모두 퀘스트를 만족시키도록 하므로...

		// 방장은 CreateInstance안에서 Move 안 했음!!!
		//pUser->SetTRRoomKey(pRoom->GetKey());
		pUser->EndTransaction();
		//LobbyRoom *pLobby = g_ZoneManager.FindMissionLobby(LobbyKey);
		//if(!pLobby)	return;

		Village* pVillage = (Village *)g_ZoneManager.FindZone(pUser->GetZoneKey());	
		IfFailThenBreak(pVillage, (DWORD)RC_ROOM_FLOW_CONTROL_DEFAULT_RETURN_VALUE); // 이 경우는 뭐다냐?

        pPartyManager->MigrateToInstanceDungeon(pUser, p100LevelQuest);

		return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;
	
	switch(flow.GetErrorCode())
	{
	case RC_ROOM_SEND_FAIL_MESSAGE_TO_USER:
		{
			pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
			MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK msgACK;	// 실패 처리
			msgACK.SetFailResult(pRecvMsg->RETINFO_FAIL.m_byKind, pRecvMsg->RETINFO_FAIL.m_dwErrorCode);
		}
		return;
	case RC_ROOM_FLOW_CONTROL_DEFAULT_RETURN_VALUE:
		{
			// 별도의 루틴에 의한 것이 아닌 예외 사항이 발견되었음. 2가지 사례.
			SUNLOG(eCRITICAL_LOG, _T("Detect Invalid State : %u:%s"), flow.GetErrorCode(), flow.GetErrorString());
		}
		return;
	default:	// [ENTERED!] Transaction
		pUser->EndTransaction();
	case RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER:
	case RC::RC_ROOM_ALREADY_DOING_TRANSACTION:
		{
			//if(flow.GetErrorCode() != (DWORD)RC::RC_ROOM_ALREADY_DOING_TRANSACTION)
			//	pUser->EndTransaction();

			//ItemUnLock
			MSG_AG_ITEM_UNLOCK_CMD msgCMD;
			msgCMD.m_ItemIndex		= SI_INVENTORY;
			msgCMD.m_ItemPos		= pUser->GetLockItemPosForInstanceDungeon();
            // (f100528.2L) prevent a locked state
            // after an instance dungeon moving transaction failed
            msgCMD.post_action_ = msgCMD.eAction_MoveFailed;
			pUser->SendToLinkedServer(&msgCMD, sizeof(msgCMD));

			pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);

			//인던 생성 실패
			MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK msgACK;
			msgACK.SetFailResult(MSG_CG_ZONE_EX_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::RET_ROOM, (DWORD)flow.GetErrorCode());
			pUser->SendPacket(&msgACK, msgACK.GetSize());
		}
		return;
	}
	return /*RETURN FALSE VALUE*/;
}

HANDLER_AG_IMPL(AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	typedef MSG_AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN::eTIME_CONTROL		eTIME_CONTROL;

	const MSG_AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN* pRecvMsg = (const MSG_AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN*)pMsg;
	const CODETYPE eventID = pRecvMsg->m_EventID;
	const eTIME_CONTROL tmCtrl = (eTIME_CONTROL)pRecvMsg->m_TimeControl;

	MSG_CG_ETC_BUFF_EVENT_TIME_CONTROL_BRD msgBRD;
	msgBRD.SetInfo(eventID, tmCtrl);
    UserManager::Instance()->SendMessageToAllUsers(&msgBRD, sizeof(msgBRD));
}


HANDLER_AG_IMPL(AG_ZONE_SSQ_JOIN_INFO_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromGame(pServerSession));
}

HANDLER_AG_IMPL(AG_ZONE_SSQ_JOIN_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromGame(pServerSession));
}

HANDLER_AG_IMPL(AG_ZONE_SSQ_ROOM_CTRL_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromGame(pServerSession));
}

HANDLER_AG_IMPL(AG_ZONE_SSQ_ROOM_INFO_BRD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromGame(pServerSession));
}

//_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
HANDLER_AG_IMPL(AG_PARTY_JOIN_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    PACKET_PTR_CONVERT(MSG_AG_PARTY_JOIN_ACK, pRecvMsg, pMsg);

    User* pRequestUser = UserManager::Instance()->GetUserByObjKey(pRecvMsg->m_dwReqMemberKey);
    User* pResponseUser = UserManager::Instance()->GetUserByObjKey(pRecvMsg->m_dwRecvMemberKey);

    if(pRequestUser == NULL || pResponseUser == NULL)
        return;

    RC::ePARTY_RESULT rcResult = RC::RC_PARTY_UNKNOWN_REASON;
    switch(pRecvMsg->m_dwFlag)
    {
    case MSG_AG_PARTY_JOIN_SYN::ENABLE_REQUEST:
        {
            rcResult = AgentPartyManager::Instance()->CanRequestJoin(pRequestUser, pResponseUser);
            if(rcResult == RC::RC_PARTY_SUCCESS)
            {
                // 상대방 UserKey를 서로 등록해 놓는다.
                AgentPartyManager::Instance()->SetOtherMemberKey(pRequestUser, pResponseUser);
                // 파티요청 패킷을 상대방에 전달한다.
                MSG_CG_PARTY_JOIN_RESPONSE_SYN msgSYN;
                strncpy(msgSYN.m_szMasterName, pRequestUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH);
                pResponseUser->SendPacket(&msgSYN, sizeof(msgSYN));
            }
            else
            {
                MSG_CG_PARTY_JOIN_NAK msgNAK;
                msgNAK.m_byErrorCode = rcResult;
                if(rcResult == RC::RC_PARTY_INVALID_PLAYER_STATE)
                    msgNAK.m_ePlayerState = pResponseUser->GetBehaveState();
                pRequestUser->SendPacket(&msgNAK, sizeof(msgNAK));
            }
        }
        break;

    case MSG_AG_PARTY_JOIN_SYN::ENABLE_RESPONSE:
        {
            AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
            // 상대방 UserKey를 초기화한다.
            pPartyManager->ResetOtherMemberKey( pRequestUser, pResponseUser );
            // 파티가입 조건이 되는지 체크하고 가입시킨다.
            rcResult = pPartyManager->InviteParty(pRequestUser, pResponseUser);
            if(rcResult != RC::RC_PARTY_SUCCESS)
            {
                MSG_CG_PARTY_JOIN_NAK msgNAK;
                msgNAK.m_byErrorCode = rcResult;
                if(rcResult == RC::RC_PARTY_INVALID_PLAYER_STATE)
                    msgNAK.m_ePlayerState = pResponseUser->GetBehaveState();
                pRequestUser->SendPacket(&msgNAK, sizeof(msgNAK));
                pResponseUser->SendPacket(&msgNAK, sizeof(msgNAK));
            }
        }
        break;
    }
}
HANDLER_AG_IMPL(AG_PARTY_JOIN_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    PACKET_PTR_CONVERT(MSG_AG_PARTY_JOIN_NAK, pRecvMsg, pMsg);
    User* pRequestUser = UserManager::Instance()->GetUserByObjKey(pRecvMsg->m_dwReqMemberKey);
    User* pResponseUser = UserManager::Instance()->GetUserByObjKey(pRecvMsg->m_dwRecvMemberKey);

    if(pRequestUser == NULL || pResponseUser == NULL)
        return;

    MSG_CG_PARTY_JOIN_NAK msgNAK;
    msgNAK.m_byErrorCode = (BYTE)pRecvMsg->m_dwErrorCode;
    switch(pRecvMsg->m_dwFlag)
    {
    case MSG_AG_PARTY_JOIN_SYN::ENABLE_REQUEST:
        {
            pRequestUser->SendPacket(&msgNAK, sizeof(msgNAK));
        }
        break;

    case MSG_AG_PARTY_JOIN_SYN::ENABLE_RESPONSE:
        {
            pRequestUser->SendPacket(&msgNAK, sizeof(msgNAK));
            pResponseUser->SendPacket(&msgNAK, sizeof(msgNAK));
        }
        break;
    }
}

HANDLER_AG_IMPL(AG_PARTY_LEAVE_BY_GUILDRELATION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_AG_PARTY_LEAVE_BY_GUILDRELATION_SYN, pRecvMsg, pMsg);

	User* pLeaveUser = UserManager::Instance()->GetUserByObjKey(pRecvMsg->m_dwMemberKey);	
	RC::ePARTY_RESULT rcResult = AgentPartyManager::Instance()->LeaveParty(pLeaveUser, eREASON_LEAVE_PARTY_BY_GUILDRELATION);
	if(rcResult != RC::RC_PARTY_SUCCESS)
	{
		MSG_CG_PARTY_LEAVE_NAK msgNAK;
		msgNAK.m_byErrorCode = rcResult;
		pLeaveUser->SendPacket(&msgNAK, sizeof(msgNAK));
	}
}

//{__NA001336_090514_PARTY_NAVI_SUPPORT__
HANDLER_AG_IMPL(AG_PARTY_NAVI_INFO_BRD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_PARTY_NAVI_INFO_BRD* pRecvMsg = static_cast<MSG_AG_PARTY_NAVI_INFO_BRD*>(pMsg);
    UserManager* pUserManager = UserManager::Instance();
    User* pMember = pUserManager->GetUser(pRecvMsg->m_dwKey);
    if(pMember == 0)
        return;

    WORD partyKey = pMember->GetPartyState().GetPartyKey();
    if(partyKey != 0)
    {
        AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
        pPartyManager->ChangedMembersInfo(partyKey, pRecvMsg, 0);
    }
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    else
    {
        MSG_AW_USER_STADING_POSITION_CMD msg_pos_cmd;
        assert(pRecvMsg->m_NumberOfNodes == 1);
        msg_pos_cmd.number_of_nodes = 1;
        for (uint32_t index = 0; index < msg_pos_cmd.number_of_nodes; ++index)
        {
            MSG_AW_USER_STADING_POSITION_CMD::Node* dest = &msg_pos_cmd.nodes[index];
            const MSG_AG_PARTY_NAVI_INFO_BRD::Node& src = pRecvMsg->m_Nodes[index];
            dest->user_guid = pMember->GetUserKey();
            dest->pos_x = src.posX;
            dest->pos_y = src.posY;
        };
        pMember->SendToWorldServer(&msg_pos_cmd, msg_pos_cmd.GetSize());
    };
#endif
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
HANDLER_AG_IMPL(AG_PARTY_CAN_JOIN_ROOM_ACK)
{
    const MSG_AG_PARTY_CAN_JOIN_ROOM_ACK* recv_msg = 
        static_cast<MSG_AG_PARTY_CAN_JOIN_ROOM_ACK*>(pMsg);

    UserManager* user_manager = UserManager::Instance();
    User* user = user_manager->GetUserByObjKey(recv_msg->request_user_key_);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : %d", recv_msg->m_dwKey);
        return;
    }

    if (recv_msg->result_code_ != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = recv_msg->result_code_;
        user->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    WORD party_key = recv_msg->join_party_key_;
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }

    RC::ePARTY_RESULT result_code = RC::RC_PARTY_SUCCESS;

    if (user->GetPartyState().IsPartyMember() == true)
    {
        result_code = party_manager->LeaveParty(user, eREASON_LEAVE_PARTY_INTEND);
        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            SUNLOG(eCRITICAL_LOG, 
                __FUNCTION__" : Fail LeaveParty() : Name : %s, Code : %d",
                user->GetSelectedCharName(), result_code);
            return;
        }
    }

    result_code = party_manager->JoinParty(party_key, user);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
        nak_msg.result_code_ = result_code;
        user->SendPacket(&nak_msg, nak_msg.GetSize());
        return;
    }

    MSG_CG_PARTY_JOIN_ROOM_ACK ack_msg;
    ack_msg.result_code_ = result_code;
    ack_msg.party_room_info_ = *(party_room->GetPartyRoomInfo());
    user->SendPacket(&ack_msg, ack_msg.GetSize());
}

HANDLER_AG_IMPL(AG_PARTY_READY_STATE_ACK)
{
    const MSG_AG_PARTY_READY_STATE_ACK* recv_msg = 
        static_cast<MSG_AG_PARTY_READY_STATE_ACK*>(pMsg);

    UserManager* user_manager = UserManager::Instance();
    User* user = user_manager->GetUserByObjKey(recv_msg->member_key_);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return;
    }

    if (recv_msg->result_code_ != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_READY_STATE_ACK nak_msg;
        nak_msg.result_code_ = recv_msg->result_code_;
        user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail change state to ready : %d", recv_msg->result_code_);
        return;
    }
    
    MSG_CG_PARTY_READY_STATE_ACK ack_msg;
    ack_msg.result_code_ = recv_msg->result_code_;
    user->SendPacket(&ack_msg, sizeof(ack_msg));

    AgentPartyManager* party_manager = AgentPartyManager::Instance();
    WORD party_key = user->GetPartyState().GetPartyKey();

    MSG_CG_PARTY_READY_STATE_BRD brd_msg;
    brd_msg.member_key_ = user->GetPlayerKey();
    brd_msg.ready_state_ = recv_msg->ready_state_;
    party_manager->SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}

HANDLER_AG_IMPL(AG_PARTY_CAN_CHANGE_OPTION_ACK)
{
    const MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* recv_msg = 
        static_cast<MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK*>(pMsg);

    UserManager* user_manager = UserManager::Instance();
    User* master_user = user_manager->GetUserByObjKey(recv_msg->master_key_);
    if (master_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return;
    }

    if (recv_msg->result_code_ != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK nak_msg;
        nak_msg.result_code_ = recv_msg->result_code_;
        master_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eFULL_LOG, 
               __FUNCTION__" : Don't change party room option : %s", 
               master_user->GetSelectedCharName());
        return;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const WORD party_key = master_user->GetPartyState().GetPartyKey();

    RC::ePARTY_RESULT result_code = party_manager->ChangePartyRoomOption(master_user, 
                                                                         party_key, 
                                                                         recv_msg);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK nak_msg;
        nak_msg.result_code_ = result_code;
        master_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Fail change option : %d", 
               result_code);
        return;
    }

    party_manager->KickPartyMemberForChangeOption(recv_msg, master_user);

    MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK ack_msg;
    ack_msg.result_code_ = result_code;
    master_user->SendPacket(&ack_msg, sizeof(ack_msg));

    MSG_CG_PARTY_CHANGE_ROOM_OPTION_BRD brd_msg;
    brd_msg.change_party_room_info_ = recv_msg->party_room_info_;
    party_manager->SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}

HANDLER_AG_IMPL(AG_PARTY_CAN_CREATE_ROOM_ACK)
{
    const MSG_AG_PARTY_CAN_CREATE_ROOM_ACK* recv_msg = 
        static_cast<MSG_AG_PARTY_CAN_CREATE_ROOM_ACK*>(pMsg);

    AgentPartyManager* party_manager = AgentPartyManager::Instance();
    UserManager* user_manager = UserManager::Instance();
    User* master_user = user_manager->GetUserByObjKey(recv_msg->master_key_);
    if (master_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return;
    }

    if (recv_msg->result_code_ != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_CREATE_ROOM_ACK nak_msg;
        nak_msg.result_code_ = recv_msg->result_code_;
        master_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eFULL_LOG, __FUNCTION__" : Have not mission item");
        return;
    }

    RC::ePARTY_RESULT result_code = 
    party_manager->CanCreatePartyRoom(master_user, 
                                      &(recv_msg->party_room_info_));
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_CREATE_ROOM_ACK nak_msg;
        nak_msg.result_code_ = result_code;
        master_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eFULL_LOG, __FUNCTION__" : CanCreate() Process Fail : %d", result_code);
        return;
    }

    BasePartyRoomInfo copied_party_room_info = recv_msg->party_room_info_;
    AgentPartyManager::FixStringOfPartyRoomInfo(&copied_party_room_info);
    result_code = party_manager->CreatePartyRoom(copied_party_room_info, master_user);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_CREATE_ROOM_ACK nak_msg;
        nak_msg.result_code_ = result_code;
        master_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eFULL_LOG, __FUNCTION__" : CreatePartyRoom() Process Fail : %d", result_code);
        return;
    }

    const PartyRoomInfo* party_room_info = 
        party_manager->GetPartyRoomInfoByMemberKey(recv_msg->master_key_);
    if (party_room_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found room info");
        return;
    }

    MSG_CG_PARTY_CREATE_ROOM_ACK ack_msg;
    ack_msg.result_code_ = result_code;
    ack_msg.party_room_info_ = *(party_room_info);
    master_user->SendPacket(&ack_msg, sizeof(ack_msg));
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosZoneSystem/AgentChaosZoneManager.h"
HANDLER_AG_IMPL(AG_CHAOS_ZONE_PACKET)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    AgentChaosZoneManager::Instance()->ParseAGPacket(pServerSession, pMsg, wSize);
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

HANDLER_AG_IMPL(AG_STATUS_CHAR_STAT_CHANGE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD* pRecvMsg =
        static_cast<MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD*>(pMsg);
    UserManager* pUserManager = UserManager::Instance();
    User* pUser = pUserManager->GetUser(pRecvMsg->m_dwKey);
    if(pUser == 0)
        return;

    WORD partyKey = pUser->GetPartyState().GetPartyKey();
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
    // 파티키의 존재여부와 노드의 개수를 모두 체크하여,
    // '파티정보 처리'인지 '개인정보 처리'인지 판단하도록 한다
    BYTE user_count = pRecvMsg->user_count_;
    if(partyKey != 0 && user_count > 1)
    {
        AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
        pPartyManager->ChangedMembersInfo(partyKey, 0, pRecvMsg);
    }
#else
    if(partyKey != 0)
    {
        AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
        pPartyManager->ChangedMembersInfo(partyKey, 0, pRecvMsg);
    }
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
    else
    {
        const BYTE upperbound = pRecvMsg->m_NumberOfNodes < _countof(pRecvMsg->m_Nodes)
                              ? pRecvMsg->m_NumberOfNodes
                              : _countof(pRecvMsg->m_Nodes);

        typedef MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD::uNode StatChangeNode;
        const StatChangeNode* pSrcIt = &pRecvMsg->m_Nodes[0],
                            * pSrcEnd = &pRecvMsg->m_Nodes[upperbound];

        const DWORD playerKey = pUser->GetPlayerKey();
        //for(DWORD loop = 2 ; loop && pSrcIt != pSrcEnd; ++pSrcIt, --loop)
        for( ; pSrcIt != pSrcEnd; ++pSrcIt)
        {
            if(pSrcIt->m_HP.objectKey != playerKey)
                continue;
            if(pSrcIt->m_HP.event == pRecvMsg->eEvent_ChangedHP)
            {
                const StatChangeNode::HP& rHP = pSrcIt->m_HP;
                //assert(rHP.maxHP != 0)
                // 유저에게 HP변화를 알린다.
                pUser->SetSelectedCharCurHP(rHP.currentHP);
                pUser->SetSelectedCharMaxHP(rHP.maxHP);
                pUser->SetReservedHP(rHP.reservedHP);

                MSG_CG_STATUS_CHANGE_HP_BRD msgBRD;
                msgBRD.m_dwObjectKey = playerKey;
                msgBRD.m_dwTargetHP = rHP.currentHP;
                msgBRD.m_dwReserveHP = rHP.reservedHP;
                pUser->SendPacket(&msgBRD, sizeof(msgBRD));
            }
            if(pSrcIt->m_MP.event == pRecvMsg->eEvent_ChangedMP)
            {
                const StatChangeNode::MP& rMP = pSrcIt->m_MP;
                //assert(rMP.maxMP != 0)
                pUser->SetSelectedCharCurMP(rMP.currentMP);
                pUser->SetSelectedCharMaxMP(rMP.maxMP);

                MSG_CG_STATUS_CHANGE_MP_BRD msgBRD;
                msgBRD.m_dwObjectKey = playerKey;
                msgBRD.m_dwTargetMP = rMP.currentMP;
                pUser->SendPacket(&msgBRD, sizeof(msgBRD));
            }
            if(pSrcIt->m_SD.event == pRecvMsg->eEvent_ChangedSD)
            {
                const StatChangeNode::SD& rSD = pSrcIt->m_SD;
                //assert(rSP.maxSD != 0)
                pUser->SetSelectedCharCurSD(rSD.currentSD);
                pUser->SetSelectedCharMaxSD(rSD.maxSD);

                MSG_CG_STATUS_CHANGE_SD_CMD msgCMD;
                msgCMD.m_dwSD = rSD.currentSD;
                pUser->SendPacket(&msgCMD, sizeof(msgCMD));
            }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            if (pSrcIt->m_FP.event == pRecvMsg->eEvent_ChangedFP)
            {
                const StatChangeNode::FP& rFP = pSrcIt->m_FP;

                pUser->SetSelectedCharCurFP(rFP.currentFP);
                pUser->SetSelectedCharMaxFP(rFP.maxFP);

                MSG_CG_STATUS_CHANGE_FP_CMD msgCMD;
                msgCMD.object_key = playerKey;
                msgCMD.m_dwTargetFP = rFP.currentFP;
                pUser->SendPacket(&msgCMD, sizeof(msgCMD));
                
            }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        }
    }
}
//}__NA001336_090514_PARTY_NAVI_SUPPORT__

// implemented by __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
HANDLER_AG_IMPL(AG_STATUS_GROUP_EXP_ACQUISITION_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_STATUS_GROUP_EXP_ACQUISITION_CMD* pRecvMsg = (MSG_AG_STATUS_GROUP_EXP_ACQUISITION_CMD*)pMsg;

    // 월드 서버로 중계
    MSG_AW_STATUS_GROUP_EXP_ACQUISITION_CMD sendMsg;

    sendMsg.m_CharGuid          = pRecvMsg->m_CharGuid;
    sendMsg.m_byAcquisitionType = pRecvMsg->m_byAcquisitionType;
    sendMsg.m_Amount            = pRecvMsg->m_Amount;

    WorldServerSession* pSession = AgentServer::GetInstance()->SessionWorldServer();
    if(pSession)
        pSession->SendPacket( &sendMsg, sizeof(sendMsg) );
}

// implemented by __NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
HANDLER_AG_IMPL(AG_STATUS_HEIM_ACQUISITION_NTF)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_HEIM_ACQUISITION_NTF* pRecvMsg = \
        static_cast<MSG_AG_STATUS_HEIM_ACQUISITION_NTF*>(pMsg);

    // 월드 서버로 중계
    MSG_AW_USER_STATUS_HEIM_ACQUISITION_NTF ntf;
    ntf.m_CharGuid          = pRecvMsg->m_CharGuid;
    ntf.m_byAcquisitionType = pRecvMsg->m_byAcquisitionType;
    ntf.m_Amount            = pRecvMsg->m_Amount;
    WorldServerSession* pSession = AgentServer::GetInstance()->SessionWorldServer();
    if (pSession) { pSession->SendPacket(&ntf, sizeof(ntf)); }
}

// implemented by __NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
HANDLER_AG_IMPL(AG_STATUS_HEIM_CONSUMPTION_NTF)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_HEIM_CONSUMPTION_NTF* pRecvMsg = \
        static_cast<MSG_AG_STATUS_HEIM_CONSUMPTION_NTF*>(pMsg);

    // 월드 서버로 중계
    MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF ntf;
    ntf.m_CharGuid          = pRecvMsg->m_CharGuid;
    ntf.m_byConsumptionType = pRecvMsg->m_byConsumptionType;
    ntf.m_Amount            = pRecvMsg->m_Amount;
    ntf.m_LottoAccumRatio = pRecvMsg->m_LottoAccumRatio;
    WorldServerSession* pSession = AgentServer::GetInstance()->SessionWorldServer();
    if (pSession) { pSession->SendPacket(&ntf, sizeof(ntf)); }
}

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM
HANDLER_AG_IMPL(AG_STATUS_LOTTO_REWARD_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_LOTTO_REWARD_ACK* pRecvMsg = \
        static_cast<MSG_AG_STATUS_LOTTO_REWARD_ACK*>(pMsg);

    MSG_AW_USER_LOTTO_REWARD_ACK ack;
    ack.m_iResult  = pRecvMsg->m_iResult;
    ack.m_CharGuid = pRecvMsg->m_CharGuid;
    ack.m_Prize    = pRecvMsg->m_Prize;
    WorldServerSession* pWorldServerSession = AgentServer::GetInstance()->SessionWorldServer();
    if(pWorldServerSession) {
        pWorldServerSession->SendPacket(&ack, (WORD)sizeof(ack)); 
    }
}
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
HANDLER_AG_IMPL(AG_STATUS_LOTTO_TICKET_PURCHASE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_ACK* pRecvMsg = \
        static_cast<MSG_AG_STATUS_LOTTO_TICKET_PURCHASE_ACK*>(pMsg);

    MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK ack;
    ack.iResult = pRecvMsg->iResult;
    ack.iCharGuid = pRecvMsg->iCharGuid;
    ack.wDrawNum = pRecvMsg->wDrawNum;
    ack.bAuto = pRecvMsg->bAuto;
    for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
        ack.bPickedNum[i] = pRecvMsg->bPickedNum[i];
    }
    WorldServerSession* pWorldServerSession = AgentServer::GetInstance()->SessionWorldServer();
    if(pWorldServerSession) {
        pWorldServerSession->SendPacket(&ack, (WORD)sizeof(ack)); 
    }
}
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

//{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
HANDLER_AG_IMPL(AG_ZONE_CAN_CREATE_ROOM_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    typedef MSG_AG_ZONE_CAN_CREATE_ROOM_ACK Packet;
    const MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* recv_msg = static_cast<MSG_AG_ZONE_CAN_CREATE_ROOM_ACK*>(pMsg);

    if(!FlowControl::FCAssert(sizeof(*recv_msg) == wSize && recv_msg->GetSize() == wSize))
    {
        return;
    }

    UserManager* user_manager = UserManager::Instance();
    // recv_msg->m_dwKey로 유저를 찾으면 Null일 수 있다. 가능성의 문제이긴 하지만,
    // Agent->Game->Agent의 시점이므로 가능성은 충분하다.
    User* request_user = user_manager->GetUser(recv_msg->m_dwKey);
    // 이 문제때문에 맴버들 Transaction은 불가, 요청자 기준으로 결과 반환이므로 문제 없음.
    if (request_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return;
    }

    MSG_CG_ZONE_MISSION_CREATE_NAK nak_msg;
    if (request_user->IsBeginTransaction())
    {
        nak_msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    // copy clone for byte alignment
    if (recv_msg->m_Result == recv_msg->eResult_InvalidMap)
    {
        nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALID_ROOM_TYPE;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    const Packet::CheckArgMembers check_member = recv_msg->m_CheckArgMembers;

    if (recv_msg->m_CheckMethod == recv_msg->eCheck_Members)
    {
        g_ZoneManager.CreateMissionByLobby(request_user, recv_msg);
    }
    else if (recv_msg->m_CheckMethod == recv_msg->eCheck_Party)
    {   
        ASSERT(!"Not Implementation");
        return;
    }
    else
    {
        ASSERT(!"Unexpected status");
        return;
    }
}
//_NA_20110630_BATTLE_ZONE_RENEWAL
HANDLER_AG_IMPL(AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    typedef MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK Packet;
    const MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK* recv_msg = 
        static_cast<MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK*>(pMsg);

    UserManager* user_manager = UserManager::Instance();
    User* request_user = user_manager->GetUser(recv_msg->m_dwKey);
    if (request_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return;
    }

    MSG_CG_PARTY_ROOM_START_ACK nak_msg;
    if (request_user->IsBeginTransaction())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : User is already doing transaction");
        nak_msg.result_code_ = RC::RC_PARTY_ALREADY_DOING_TRANSACTION;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    if (recv_msg->result_value_ == recv_msg->eResult_InvalidMap)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Map code is invalid");
        nak_msg.result_code_ = RC::RC_PARTY_INVALID_ROOMTYPE;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    if (recv_msg->result_value_ == recv_msg->eResult_FailCheckTerms)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail check terms to start room");
        nak_msg.result_code_ = RC::RC_PARTY_FAIL_CREATE_PARTY_ROOM;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();

    static MemberOperator member_operator;
    member_operator.Init(ePARTY_OPERATOR_MIGRATE_CHECK, 0, 0);
    party_manager->For_eachMembers(request_user->GetPartyState().GetPartyKey(), member_operator);
    if (member_operator.IsSuccess() == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail check member zone state");
        nak_msg.result_code_ = RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    MAPCODE map_code = recv_msg->map_code_;
    const sMAPINFO* const map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found map info : %d", map_code);
        return;
    }

    const eZONE_RULESET& rule_set = map_info->Ruleset;
    if (rule_set.rules & rule_set.ZONE_RULE_INDUN_SHADOW_NOT_PLAYING_MODE)
    {
        if (request_user->GetCharClass() == eCHAR_SHADOW)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail check member zone state");
            nak_msg.result_code_ = RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
            request_user->SendPacket(&nak_msg, sizeof(nak_msg));
            return;
        }

        member_operator.Init(ePARTY_OPERATOR_SHADOW_CHECK, 0, 0);
        party_manager->For_eachMembers(request_user->GetPartyState().GetPartyKey(), member_operator);
        if (member_operator.IsSuccess() == false)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail check member zone state");
            nak_msg.result_code_ = RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
            request_user->SendPacket(&nak_msg, sizeof(nak_msg));
            return;
        }
    }

    RC::ePARTY_RESULT result_code = party_manager->CanStartRoom(request_user);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        nak_msg.result_code_ = result_code;
        request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : CanStartParty process result fail: %d", result_code);
        return;
    }

    WORD party_key = recv_msg->party_key_;
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room");
        return;
    }

    if (party_manager->ExceptionProcessByPartyMember(recv_msg, request_user) == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Exception process result fail");
        return;
    }

    //파티룸 추가 정보를 입력 한다.
    //gamelulu's note : 추가 정보가 많아질경우에는 구조체로 정리하여 입력한다.
    PartyRoomList* party_room_list = party_manager->GetPartyRoomList();
    party_room_list->SetPartyRoomAddInfOfRequireItem(party_key, recv_msg->require_item_);

    //파티룸 시작 대기방에 등록한다.
    party_manager->AddPartyRoomToWaitList(party_key);

    MSG_CG_PARTY_START_ROOM_COUNT_BRD brd_msg;
    party_manager->SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}

HANDLER_AG_IMPL(AG_ZONE_LOBBY_CREATE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_LOBBY_CREATE_ACK* pRecvMsg = static_cast<MSG_AG_ZONE_LOBBY_CREATE_ACK*>(pMsg);

    if(!FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize && pRecvMsg->GetSize() == wSize))
        return;

    UserManager* pRequesterManager = UserManager::Instance();
    // pRecvMsg->m_dwKey로 유저를 찾으면 Null일 수 있다. 가능성의 문제이긴 하지만,
    // Agent->Game->Agent의 시점이므로 가능성은 충분하다.
    User* pRequester = pRequesterManager->GetUser(pRecvMsg->m_dwKey);
    // 이 문제때문에 맴버들 Transaction은 불가, 요청자 기준으로 결과 반환이므로 문제 없음.
    if(pRequester == NULL)
        return;

    if(pRequester->IsBeginTransaction())
    {
        MSG_CG_ZONE_LOBBY_CREATE_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        pRequester->SendPacket(&msg, sizeof(msg));
        return;
    }

    if(pRecvMsg->m_1stCheckResultInGameServer != RC::RC_ROOM_SUCCESS)
    {
        MSG_CG_ZONE_LOBBY_CREATE_NAK msg;
        msg.m_dwErrorCode = pRecvMsg->m_1stCheckResultInGameServer;
        pRequester->SendPacket(&msg, sizeof(msg));
        return;
    }

    pRequester->BeginTransaction(TR_CREATELOBBY);

    LobbyRoom* pLobbyRoom = NULL;
    BYTE byMaxLimitUserNum = pRecvMsg->m_byMaxLimitUserNum;

    RC::eROOM_RESULT rt =
        g_ZoneManager.CreateLobbyRoom(pRequester->GetZoneKey(), pRequester,
                                      pRecvMsg->m_MapCode, 
                                      (eZONETYPE)pRecvMsg->m_byRoomType,
                                      (eZONEPUBLIC)pRecvMsg->m_byRoomPublic, 
                                      pRecvMsg->m_pszRoomTitle, // -.-
                                      pRecvMsg->m_pszRoomPWD,   // -.-
                                      pRecvMsg->m_RoomInfo,
                                      byMaxLimitUserNum, 0, pLobbyRoom);

    if(RC::RC_ROOM_SUCCESS == rt)
    {
        MSG_CG_ZONE_LOBBY_CREATE_ACK msg;
        msg.m_LobbyKey = pLobbyRoom->GetKey();
        pRequester->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        // 로비생성실패
        MSG_CG_ZONE_LOBBY_CREATE_NAK msg;
        msg.m_dwErrorCode = rt;
        pRequester->SendPacket(&msg, sizeof(msg));
    }

    pRequester->EndTransaction();
}


HANDLER_AG_IMPL(AG_ZONE_LOBBY_JOIN_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_LOBBY_JOIN_ACK* pRecvMsg = static_cast<MSG_AG_ZONE_LOBBY_JOIN_ACK*>(pMsg);

    if(!FlowControl::FCAssert(sizeof(*pRecvMsg) == wSize && pRecvMsg->GetSize() == wSize))
        return;

    UserManager* pUserManager = UserManager::Instance();
    // pRecvMsg->m_dwKey로 유저를 찾으면 Null일 수 있다. 가능성의 문제이긴 하지만,
    // Agent->Game->Agent의 시점이므로 가능성은 충분하다.
    User* pRequester = pUserManager->GetUser(pRecvMsg->m_dwKey);
    // 이 문제때문에 맴버들 Transaction은 불가, 요청자 기준으로 결과 반환이므로 문제 없음.
    if(pRequester == NULL)
        return;

    if(pRequester->IsBeginTransaction())
    {
        MSG_CG_ZONE_LOBBY_JOIN_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        pRequester->SendPacket(&msg, sizeof(msg));
        return;
    }

    if(pRecvMsg->m_1stCheckResultInGameServer != RC::RC_ROOM_SUCCESS)
    {
        MSG_CG_ZONE_LOBBY_CREATE_NAK msg;
        msg.m_dwErrorCode = pRecvMsg->m_1stCheckResultInGameServer;
        pRequester->SendPacket(&msg, sizeof(msg));
        return;
    }

    pRequester->BeginTransaction(TR_JOINLOBBY);

    CheckMoveZonePolicy* const pCheckMoveZonePolicy =
        MoveZonePolicy<MZTF_VILLAGE_TO_LOBBY>::AlignInfo(
            pRequester
            //{     From           |         To         }
            , eZONETYPE_VILLAGE,        eZONETYPE_LOBBY         //	eZONETYPE
            , pRequester->GetZoneKey(), pRecvMsg->m_LobbyKey    //	ZoneKey
  );
    RoomArgument args((eZONEPUBLIC)pRecvMsg->m_byRoomPublic,
        ((pRecvMsg->m_byRoomPublic==eZONEPUBLIC_PRIVATE) ? pRecvMsg->m_pszRoomPWD : NULL));
    const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);

    if(RC::RC_ROOM_SUCCESS == rt)
    {
        LobbyRoom* pLobby = g_ZoneManager.FindLobby(pRecvMsg->m_LobbyKey);
        if(!pLobby)
            return;

        MSG_CG_ZONE_LOBBY_JOIN_ACK msg;
        msg.m_dwMasterKey = pLobby->GetMaster()->GetPlayerKey();
        pRequester->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        // 로비생성실패
        MSG_CG_ZONE_LOBBY_JOIN_NAK msg;
        msg.m_dwErrorCode = rt;
        pRequester->SendPacket(&msg, sizeof(msg));
    }

    pRequester->EndTransaction();
}

//}__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__

//_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
HANDLER_AG_IMPL(AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK)
{
    __UNUSED(pServerSession);
    
    const MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK* const receive_msg = 
        static_cast<MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK*>(pMsg);

    if (!FlowControl::FCAssert(receive_msg->GetSize() == wSize))
    {
        return;
    }
    
    const UserManager* const user_manager = UserManager::Instance();
    User* const user = user_manager->GetUser(receive_msg->m_dwKey);
    if (user == NULL)
    {
        return;
    }        

    MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK msg;
    msg.m_Count = receive_msg->current_room_count_;
    for (int room_index = 0; room_index < msg.m_Count; ++room_index)
    {
        msg.m_RoomInfo[room_index] = receive_msg->room_info_list_[room_index];
    }

    user->SendPacket(&msg, msg.GetSize());
}

HANDLER_AG_IMPL(AG_ZONE_DOMINATION_FIELD_INFO_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_DOMINATION_FIELD_INFO_ACK* pRecvMsg = (MSG_AG_ZONE_DOMINATION_FIELD_INFO_ACK *)pMsg;
    
    ZoneInterface* const pZoneInterface = g_ZoneManager.FindZone(pRecvMsg->m_RoomKey);
    if (NULL == pZoneInterface)
        return;

    if (eZONETYPE_DOMINATION_FIELD != pZoneInterface->GetType())
        return;

    DominationField* pZone = (DominationField*)pZoneInterface;
    pZone->SetGuildGuid(pRecvMsg->m_offenceGuild, pRecvMsg->m_defenceGuild, pRecvMsg->m_eSetType);
}

HANDLER_AG_IMPL(AG_ZONE_DOMINATION_FIELD_INFO_NAK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_DOMINATION_FIELD_INFO_NAK* pRecvMsg = (MSG_AG_ZONE_DOMINATION_FIELD_INFO_NAK *)pMsg;

    // 존이 존재 하지 않거나, 타입이 다른 경우...
    SUNLOG( eCRITICAL_LOG, "[MSG_AG_ZONE_DOMINATION_FIELD_INFO_NAK]Room[%d]IsNotFound[%d]",
        pRecvMsg->m_RoomKey, pRecvMsg->m_dwErrorCode );
}

HANDLER_AG_IMPL(AG_ZONE_DOMINATION_REQ_KICK_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_DOMINATION_REQ_KICK_CMD* pRecvMsg = (MSG_AG_ZONE_DOMINATION_REQ_KICK_CMD *)pMsg;

    UserManager* pUserManager = UserManager::Instance();
    User* pUser = pUserManager->GetUser(pRecvMsg->m_userGuid);  // _NA001385_20090924_DOMINATION_BUG
    if(pUser == NULL)
        return;

    ZoneInterface* const pZoneInterface = g_ZoneManager.FindDominationField(pRecvMsg->m_mapCode);
    if (NULL == pZoneInterface)
        return;

    DominationField* pZone = (DominationField*)pZoneInterface;
    const DWORD playerKey  = pUser->GetPlayerKey();

    pZone->KickUser(playerKey);
}

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
//시스템 메모 GameServer로부터 WorldServer로 포워딩
HANDLER_AG_IMPL(AG_EVENT_SYSTEMMEMO_SEND_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_EVENT_SYSTEMMEMO_SEND_CMD* recv_msg = 
        static_cast<MSG_AG_EVENT_SYSTEMMEMO_SEND_CMD*>(pMsg);

    AgentServer* agent_server = AgentServer::GetInstance();

    MSG_AW_USER_SYSTEMMEMO_SEND_CMD cmd_msg;
    {
        cmd_msg.system_memo_list_num_ = recv_msg->system_memo_list_num_;
        memcpy(cmd_msg.system_memo_info_, recv_msg->system_memo_info_, 
               sizeof(cmd_msg.system_memo_info_));
    }
    agent_server->SendToWorldServer(&cmd_msg, cmd_msg.GetSize());
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
HANDLER_AG_IMPL(AG_EVENT_TODAYQUEST_INFO_BRD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_EVENT_TODAYQUEST_INFO_BRD* recv_msg = 
        static_cast<MSG_AG_EVENT_TODAYQUEST_INFO_BRD*>(pMsg);

    MSG_CG_QUEST_TODAYQUEST_INFO_CMD msg;
    msg.count_ = recv_msg->count_;
    memcpy(&msg.today_quest_info, &recv_msg->today_quest_info, \
        sizeof(QUEST_TODAY_QUEST_INFO)*recv_msg->count_);

    BOOST_STATIC_ASSERT(_countof(msg.today_quest_info) == _countof(recv_msg->today_quest_info));
    util::AssertCompileTimeVariableTypeEqual(\
        msg.today_quest_info[0], recv_msg->today_quest_info[0]);

    UserManager::Instance()->SendMessageToAllUsers(&msg, sizeof(msg));
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

HANDLER_AG_IMPL(AG_ZONE_RETURN_TO_ROOM_INFO_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_RETURN_TO_ROOM_INFO_ACK* recv_msg = (MSG_AG_ZONE_RETURN_TO_ROOM_INFO_ACK*)pMsg;
    FINDnALIGN_USER(pUser, recv_msg->m_dwKey);

    User* pole_user = pUser;

    User* return_user = UserManager::Instance()->GetUser(recv_msg->REQUEST_USER_KEY());
    if (return_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return;
    }

    if (recv_msg->RESULT() == MSG_AG_ZONE_RETURN_TO_ROOM_INFO_ACK::FAIL)
    {
        return_user->EndTransaction();
        return;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    if (party_manager->CheckReturnPartyInfo(pole_user, return_user) == FALSE)
    {
        return_user->EndTransaction();
        return_user->closing_user_trans_controller_.Init();
        return;
    }

    ZoneInterface* from_zone = g_ZoneManager.FindZone(return_user->GetZoneKey());
    ZoneInterface* to_zone = g_ZoneManager.FindZone(pole_user->GetZoneKey());
    if (from_zone == NULL || to_zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : zone data is null");
        return_user->EndTransaction();
        return_user->closing_user_trans_controller_.Init();
        return;
    }

    RoomInterface* to_room = static_cast<RoomInterface*>(to_zone);

    CheckMoveZonePolicy* const check_policy = 
        MoveZonePolicy<MZTF_STATIC_TO_ROOM_FOR_UCUSER>::AlignInfo(
        return_user, 
        from_zone->GetType(), 
        to_zone->GetType(), 
        return_user->GetZoneKey(),	
        pole_user->GetZoneKey()
        );

    RoomArgument args(
        to_room->GetZonePublic(), 
        to_room->GetRoomPWD(),
        return_user->closing_user_trans_controller_.GetFieldCode(), 
        0,
        &return_user->closing_user_trans_controller_.GetRandomPos()
        );

    if (g_ZoneManager.MoveZone(check_policy, args) != RC::RC_ROOM_SUCCESS)
    {
        return_user->EndTransaction();
        return_user->closing_user_trans_controller_.Init();
        return;
    }

    WORD party_key = pole_user->GetPartyState().GetPartyKey();
    party_manager->RemoveClosingUser(party_key, return_user->GetSelectedCharGuid());

    return_user->SetTRRoomKey(pole_user->GetZoneKey());
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
HANDLER_AG_IMPL(AG_GM_GUILD_RANKING_REFRESH)
{
    const MSG_AG_GM_GUILD_RANKING_REFRESH* packet = \
        static_cast<MSG_AG_GM_GUILD_RANKING_REFRESH*>(pMsg);

    g_GuildRankingSystem.RequestGuildRankingLoad();
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
HANDLER_AG_IMPL(AG_CHARINFO_PREMIUMSERVICE_BILL_CMD)
{
    const MSG_AG_CHARINFO_PREMIUMSERVICE_BILL_CMD* packet = \
        static_cast<MSG_AG_CHARINFO_PREMIUMSERVICE_BILL_CMD*>(pMsg);
    FINDnALIGN_USER(pUser, packet->m_dwKey);

    pUser->SetPremiumService(packet->premiumservice_);
    pUser->NotifyPcBangBillingStatus(false, true);
}
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
HANDLER_AG_IMPL(AG_GM_BATTLE_GROUND_CMD)
{
    const MSG_AG_GM_BATTLE_GROUND_CMD* packet = \
        static_cast<MSG_AG_GM_BATTLE_GROUND_CMD*>(pMsg);
    FINDnALIGN_USER(pUser, packet->m_dwKey);

    TeamBattleGroundManager* const team_battle_ground_manager = TeamBattleGroundManager::Instance();

    const int cmd_mode = packet->cmd_mode;
    const CHARGUID char_guid = packet->char_guid;
    const ChaosZoneTeam team_division = packet->team_key;
#ifdef _NA_007191_20140428_MODIFY_GM_COMMAND
    const MAPCODE map_code = packet->map_code;
#else
    const MAPCODE map_code = 40050;
#endif //_NA_007191_20140428_MODIFY_GM_COMMAND

    enum cmd_mode {
        reservation,
        cancel_reservation,
        team_reservation,
        cancel_team_reservation,
        start,
        enter,
        clear,
        max_cmd_mode,
    };

    bool is_cmd_success = 0;
    MSG_CG_GM_STRING_CMD_NAK nak_msg;
    switch(cmd_mode)
    {
    case reservation: // 개인예약
        {
            if (team_battle_ground_manager->InsertReservation(char_guid, team_division, map_code)) {
                is_cmd_success = 1;
            }
            else {
                nak_msg.m_byErrorCode = RC::RC_GM_CMD_FAILED;
            }
            
        }break;

    case cancel_reservation:
        {
            if (team_battle_ground_manager->RemoveReservation(char_guid)) {
                is_cmd_success = 1;
            }
            else {
                nak_msg.m_byErrorCode = RC::RC_GM_CMD_FAILED;
            }
        }break;

    case team_reservation: // 팀예약
        {
            if (team_battle_ground_manager->InsertReservationTeam(char_guid, team_division, map_code)) {
                is_cmd_success = 1;
            }
            else {
                nak_msg.m_byErrorCode = RC::RC_GM_CMD_FAILED;
            }

        }break;
    case cancel_team_reservation:
        {
            if (team_battle_ground_manager->RemoveReservationTeam(char_guid)) {
                is_cmd_success = 1;
            }
            else {
                nak_msg.m_byErrorCode = RC::RC_GM_CMD_FAILED;
            }
        }break;

    case start: // 시작
        {
            //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
            if (team_battle_ground_manager->CheckMatching(map_code))  {
                is_cmd_success = 1;
            }
            else {
                nak_msg.m_byErrorCode = RC::RC_GM_CMD_FAILED;
            }
        }break;

    case enter:
        {
            KEYTYPE zone_key = packet->zone_key;
            //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
            if (team_battle_ground_manager->IsValidZoneKey(zone_key) == false)
            {
                zone_key = team_battle_ground_manager->GetZoneKey();
                if (zone_key == 0)
                    break;
            }

            InstanceDungeon* const target_room = \
                static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(zone_key));
            if (target_room == NULL) {
                break;
            }

            ZoneInterface* current_zone = g_ZoneManager.FindZone(pUser->GetZoneKey());
            if (current_zone == NULL)
            {
                SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : Not found zone : %d", pUser->GetUserGUID());
                break;
            }
            pUser->BeginTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);
            pUser->SetTRRoomKey(zone_key);

            CheckMoveZonePolicy* const check_move_zone_policy = 
                MoveZonePolicy<MZTF_STATIC_TO_ROOM_FOR_UCUSER>::AlignInfo(pUser,
                pUser->GetZoneType(), target_room->GetType(),
                pUser->GetZoneKey(),  target_room->GetKey());

            RoomArgument argument(eZONEPUBLIC_PUBLIC, NULL);

            RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(check_move_zone_policy, RoomArgument::GetEmpty());

        }break;

    case clear:
        {
            if(team_battle_ground_manager->RemoveReservationTeamAll()) {
                is_cmd_success = 1;
            }
            else {
                nak_msg.m_byErrorCode = RC::RC_GM_CMD_FAILED;
            }

        }break;
    }

    if (is_cmd_success)
    {
        MSG_CG_GM_STRING_CMD_ACK ackMsg; // pad out with zeroes    
        strncpy(ackMsg.m_szCommand, packet->gm_cmd_str, MSG_CG_GM_STRING_CMD_ACK::MAX_STRING_CMD_LENGTH); 
        pUser->SendPacket(&ackMsg, sizeof(ackMsg));
    }
    else
    {
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
HANDLER_AG_IMPL(AG_ZONE_DOMINATION_NOTIFIER_BRD)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_AG_ZONE_DOMINATION_NOTIFIER_BRD* packet = \
        static_cast<MSG_AG_ZONE_DOMINATION_NOTIFIER_BRD*>(pMsg);

    MSG_CG_DOMINATION_RESULT_BRD brd_msg;
    memset(brd_msg.win_guild_name, 0, sizeof(brd_msg.win_guild_name));
    brd_msg.m_winGuid  = packet->win_guild_guid;
    brd_msg.m_loseGuid = packet->lose_guild_guid;
    brd_msg.m_mapCode  = packet->map_code;
    _tcsncpy(brd_msg.win_guild_name, packet->win_guild_name, MAX_GUILDNAME_LENGTH);

    if (packet->lose_guild_guid != 0) {
        // 공성 성공, 전지역에 알림
        UserManager::Instance()->SendMessageToAllUsers(&brd_msg, sizeof(brd_msg));
    }
    else
    {
        // 해당 지역에 알림
        ZoneInterface* const current_zone = g_ZoneManager.FindDominationField(packet->map_code);
        if (current_zone) {
            current_zone->SendToAll(&brd_msg, sizeof(brd_msg));
        }
    }
}
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
HANDLER_AG_IMPL(AG_ZONE_SPA_ENTER_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_SPA_ENTER_ACK* recv_msg = static_cast<MSG_AG_ZONE_SPA_ENTER_ACK*>(pMsg);
    FINDnALIGN_USER(pUser, recv_msg->m_dwKey);

    if(pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_USER_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    MAPCODE spa_mapcode = recv_msg->spa_mapcode;
    if (spa_mapcode == 0)
    {
        // 이동 실패
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_INVALID_MAPCODE;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
    }

    ZoneInterface* spa_zone = g_ZoneManager.FindStaticZone(spa_mapcode);
    if (spa_zone == NULL || spa_zone->GetType() != eZONETYPE_SPA_FIELD)
    {
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_INVALID_ZONE;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    //현재 캐릭터의 위치
    ZoneInterface* cur_zone = g_ZoneManager.FindZone(pUser->GetZoneKey());
    MAPCODE cur_mapcode = 0;
    if (cur_zone != NULL) {
        cur_mapcode = cur_zone->GetMapCode();
    }

    CheckMoveZonePolicy* const check_movezone_policy = \
        MoveZonePolicy<MZTF_STATIC_TO_STATIC>::AlignInfo(
        pUser
        //{		From		|			To		}
        , pUser->GetZoneType(),	spa_zone->GetType()	//	eZONETYPE
        , pUser->GetZoneKey(),	spa_zone->GetKey()	//	ZoneKey
        );
    const RC::eROOM_RESULT result = g_ZoneManager.MoveZone(check_movezone_policy, RoomArgument::GetEmpty());

    if(RC::RC_ROOM_SUCCESS == result)
    {
        MSG_CG_ZONE_EX_SPA_ENTER_ACK ack_packet;
        ack_packet.spa_mapcode = spa_mapcode;
        pUser->SendPacket(&ack_packet, sizeof(ack_packet));
        
        //필드이동 로그..
        GAMELOG->MoveVillage(pUser, cur_mapcode, spa_mapcode, FALSE);
    }
    else
    {
        // 이동 실패
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_packet;
        nak_packet.error_code = result;
        pUser->SendPacket(&nak_packet, sizeof(nak_packet));
    }
}

HANDLER_AG_IMPL(AG_ZONE_SPA_LEAVE_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_SPA_LEAVE_ACK* recv_msg = static_cast<MSG_AG_ZONE_SPA_LEAVE_ACK*>(pMsg);
    FINDnALIGN_USER(pUser, recv_msg->m_dwKey);

    if(pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_EX_SPA_LEAVE_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_USER_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    //맵코드 검사
    MAPCODE village_map_code = recv_msg->village_map_code;
    if (village_map_code == 0)
    {
        // 이동 실패
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_INVALID_MAPCODE;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
    }

    //가려는 Zone 검사
    ZoneInterface* village_zone = g_ZoneManager.FindStaticZone(village_map_code);
    if (village_zone == NULL)
    {
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_INVALID_ZONE;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    //현재 캐릭터의 위치
    ZoneInterface* cur_zone = g_ZoneManager.FindZone(pUser->GetZoneKey());
    MAPCODE cur_mapcode = 0;
    if (cur_zone != NULL) {
        cur_mapcode = cur_zone->GetMapCode();
    }

    CheckMoveZonePolicy* const check_movezone_policy = \
        MoveZonePolicy<MZTF_STATIC_TO_STATIC>::AlignInfo(
        pUser
        //{		From		|			To		}
        , pUser->GetZoneType(),	village_zone->GetType()	//	eZONETYPE
        , pUser->GetZoneKey(),	village_zone->GetKey()	//	ZoneKey
        );
    const RC::eROOM_RESULT result = g_ZoneManager.MoveZone(check_movezone_policy, RoomArgument::GetEmpty());

    if(RC::RC_ROOM_SUCCESS == result)
    {
        MSG_CG_ZONE_EX_SPA_LEAVE_ACK ack_packet;
        ack_packet.return_map_code = village_map_code;
        pUser->SendPacket(&ack_packet, sizeof(ack_packet));

        //필드이동 로그
        GAMELOG->MoveVillage(pUser, cur_mapcode, village_map_code, FALSE);
    }
    else
    {
        //이동 실패
        MSG_CG_ZONE_EX_SPA_LEAVE_NAK nak_packet;
        nak_packet.error_code = result;
        pUser->SendPacket(&nak_packet, sizeof(nak_packet));
    }
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
HANDLER_AG_IMPL(AG_GM_SONNENSCHEIN_CMD)
{
    const MSG_AG_GM_SONNENSCHEIN_CMD* packet = \
        static_cast<MSG_AG_GM_SONNENSCHEIN_CMD*>(pMsg);
    FINDnALIGN_USER(pUser, packet->m_dwKey);

    SonnenScheinManager* sonnenschein_mgr = SonnenScheinManager::Instance();

    if (strcmp(packet->gm_cmd, "생성") == 0 || strcmp(packet->gm_cmd, "create") == 0)
    {
        RC::eSONNENSCHEIN_RESULT create_result = sonnenschein_mgr->CreateSonnenScheinRoom(pUser, pUser->IsGM());
    }
    else if (strcmp(packet->gm_cmd, "입장") == 0 || strcmp(packet->gm_cmd, "enter") == 0)
    {
        sonnenschein_mgr->MoveToSonnenScheinRoom(pUser, pUser->IsGM());
    }
    else if (strcmp(packet->gm_cmd, "파괴") == 0 || strcmp(packet->gm_cmd, "destroy") == 0)
    {
        GUILDGUID user_guild_guid = pUser->GetSelectedCharGuildGuid();
        if (user_guild_guid == 0) {
            return;
        }

        sonnenschein_mgr->DestroyRoom(user_guild_guid);
    }
}

#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
HANDLER_AG_IMPL(AG_GM_ONEDAYSGUILDMISSION_CMD)
{
	const MSG_AG_GM_ONEDAYSGUILDMISSION_CMD* packet = \
		static_cast<MSG_AG_GM_ONEDAYSGUILDMISSION_CMD*>(pMsg);
	FINDnALIGN_USER(pUser, packet->m_dwKey);

	OneDaysGuildMissionManager* onedaysguildmission_mgr = OneDaysGuildMissionManager::Instance();

	if (strcmp(packet->gm_cmd, "생성") == 0 || strcmp(packet->gm_cmd, "create") == 0)
	{
		RC::eONEDAYSGUILDMISSION_RESULT create_result = onedaysguildmission_mgr->CreateOneDaysGuildMissionRoom(pUser, pUser->IsGM());
	}
	else if (strcmp(packet->gm_cmd, "입장") == 0 || strcmp(packet->gm_cmd, "enter") == 0)
	{
		onedaysguildmission_mgr->MoveToOneDaysGuildMissionRoom(pUser, pUser->IsGM());
	}
	else if (strcmp(packet->gm_cmd, "파괴") == 0 || strcmp(packet->gm_cmd, "destroy") == 0)
	{
		GUILDGUID user_guild_guid = pUser->GetSelectedCharGuildGuid();
		if (user_guild_guid == 0) {
			return;
		}

		onedaysguildmission_mgr->DestroyRoom(user_guild_guid);
	}
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)