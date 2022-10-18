#include "stdafx.h"
#include "./Handler_AG_ZONE.h"
//==================================================================================================

#include <PacketStruct_AG.h>
#include <PacketStruct_DG.h>
#include <PacketStruct_GZ.h>
#include <MapInfoParser.h>
//==================================================================================================

#include <ServerSession.h>
#include <WayPoint.h>
//==================================================================================================

#include "FunctionalRestrictManager.h"
#include "ObjectFactory.h"
#include "ChangeMonsterAbility.h"

#include "Player.h"
#include "ItemManager.h"
#include "GameServerEx.h"
#include "GamePartyManager.h"
#include "MissionManager.h"
#include "QuestManager.h"
#include "PVPManager.h"

#include <World/GameWarControlManager.h>
#include "GameZoneManager.h"
#include "GameLobbyRoom.h"
#include "GameHuntingRoom.h"
#include "GameMissionRoom.h"
#include "GameVillage.h"
#include "GameInstanceDungeon.h"
#include "GamePVPRoom.h"

#include "GameCHuntingRoom.h"
#include "GameHuntingField.h"
#include "QuestManager_Concrete.h"
#include "GameDominationField.h"
#include "GameDominationManager.h"
#include "TriggerManagerEx.h"

#include "GameGuildManager.h"
#include "ChaoState.h"

#include <GameParty/NewGamePartyManager.h>
#include <GameParty/NewGameParty.h>

#include "WarehouseSlotContainer.h"
#include "GuildWarehouseSlotContainer.h"
#include "StatusManager.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM
#include <BetaKeyImpl.h>
#endif

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "BattlezoneMissionTreeParser.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#include "GameDominationField.h"
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "GameSpaField.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

Handler_AG_ZONE::Handler_AG_ZONE(void)
{
}

Handler_AG_ZONE::~Handler_AG_ZONE(void)
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_ZONE::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_ZONE::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_LOBBY_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_HUNTING_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_MISSION_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_VILLAGE_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_PVP_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_CHUNTION_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_FIELD_CREATE_CMD, PHASE_SERVICE),
        //_NA001385_20090924_DOMINATION_BASE
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_DOMINATION_FIELD_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_DOMINATION_FIELD_INFO_SYN, PHASE_SERVICE),
        //
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_ROOM_DESTROY_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_ROOM_JOIN_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_ROOM_JOIN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_ROOM_LEAVE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_ROOM_LEAVE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_MASTER_CHANGED_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_PVP_INFO_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_PORTAL_FIELD_MOVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_RETURN_VILLAGE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_CHANGE_SPECIALTYPE_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_INSTANCE_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_TUTORIAL_SKIP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_RETURN_TO_ROOM_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE,  AG_ZONE_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_SSQ_JOIN_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_SSQ_JOIN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_SSQ_ROOM_CTRL_CMD, PHASE_SERVICE),
        //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_QUEST_PORTAL_ENTER_SYN, PHASE_SERVICE),
        //{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_LOBBY_CREATE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_LOBBY_JOIN_SYN, PHASE_SERVICE),
        //}
        //_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_CHANGE_MONSTER_ABILITY_CMD, PHASE_SERVICE),
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_SYN, PHASE_SERVICE),
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_SPAFIELD_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_SPA_ENTER_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(AG_ZONE, AG_ZONE_SPA_LEAVE_SYN, PHASE_SERVICE),
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


//(lst1024)(090410) �߱� ������ ���� ���� ����
// ��Ƽ������ �̵� �غ� �Ǿ����� üũ �Ѵ�.
class CheckReadyPartyMember
{
public :
	CheckReadyPartyMember() : m_AllReady(TRUE) {}
	~CheckReadyPartyMember(){}

	VOID	operator () ( Player * pPlayer )
	{
		if( pPlayer->GetField() && 
			pPlayer->GetBehave() == PLAYER_BEHAVE_IDLE_STATE )
			return;

		m_AllReady = FALSE;
	}

	BOOL m_AllReady;
};


Handler_AG_ZONE_IMPL( AG_ZONE_LOBBY_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_LOBBY_CREATE_CMD * pRecvMsg = (MSG_AG_ZONE_LOBBY_CREATE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_LOBBY_CREATE_CMD )" );
#endif


	GameLobbyRoom * pZone = static_cast<GameLobbyRoom *>(g_GameZoneManager.CreateZone( 
		eZONETYPE_LOBBY, pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode ));
	if( pZone )
	{
		pZone->SetRoomTitle(pRecvMsg->m_pszRoomTitle);
		pZone->SetRoomPWD(pRecvMsg->m_pszRoomPWD);
		pZone->SetBaseRoomInfo(&pRecvMsg->m_RoomInfo);
		pZone->SetMasterUserKey( pRecvMsg->m_dwKey );

		pZone->OnCreate();
	}
}
Handler_AG_ZONE_IMPL( AG_ZONE_HUNTING_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    ASSERT("Function Not Suported!!");
    return;
//	MSG_AG_ZONE_HUNTING_CREATE_CMD * pRecvMsg = (MSG_AG_ZONE_HUNTING_CREATE_CMD *)pMsg;
//
//#ifdef __NA_PACKETFLOW_TEST
//	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_HUNTING_CREATE_CMD )" );
//#endif
//
//	GameHuntingRoom * pZone = static_cast<GameHuntingRoom *>(g_GameZoneManager.CreateZone( 
//		eZONETYPE_HUNTING, pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode ));
//	if( pZone )
//	{
//		pZone->SetRoomTitle(pRecvMsg->m_pszRoomTitle);
//		pZone->SetRoomPWD(pRecvMsg->m_pszRoomPWD);
//		pZone->SetBaseRoomInfo(&pRecvMsg->m_RoomInfo);
//		pZone->SetMasterUserKey(pRecvMsg->m_dwKey);
//
//		pZone->SetAdditionalRoomInfo(&pRecvMsg->m_AdditionalInfo);
//
//		pZone->OnCreate();
//	}
}
Handler_AG_ZONE_IMPL( AG_ZONE_MISSION_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_MISSION_CREATE_CMD* pRecvMsg = (MSG_AG_ZONE_MISSION_CREATE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_MISSION_CREATE_CMD )" );
#endif

    GameZone* pBaseZone = g_GameZoneManager.CreateZone(eZONETYPE_MISSION,
                                                       pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode,
                                                       pRecvMsg->m_Rules);
    GameMissionRoom* pZone = static_cast<GameMissionRoom*>(pBaseZone);
	if( pZone )
	{
		pZone->SetRoomTitle(pRecvMsg->m_pszRoomTitle);
		pZone->SetRoomPWD(pRecvMsg->m_pszRoomPWD);
		pZone->SetBaseRoomInfo(&pRecvMsg->m_RoomInfo);
		pZone->SetMasterUserKey(pRecvMsg->m_dwKey);
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        pZone->SetPartyZoneType(pRecvMsg->party_zone_type);
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

		pZone->OnCreate();
	}
}

Handler_AG_ZONE_IMPL( AG_ZONE_PVP_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_PVP_CREATE_CMD * pRecvMsg = (MSG_AG_ZONE_PVP_CREATE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_PVP_CREATE_CMD )" );
#endif

	GamePVPRoom * pZone = static_cast<GamePVPRoom *>(g_GameZoneManager.CreateZone( 
		eZONETYPE_PVP, pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode ));
	if( pZone )
	{
		pZone->SetRoomTitle(pRecvMsg->m_pszRoomTitle);
		pZone->SetRoomPWD(pRecvMsg->m_pszRoomPWD);
		pZone->SetBaseRoomInfo(&pRecvMsg->m_RoomInfo);
		pZone->SetMasterUserKey(pRecvMsg->m_dwKey);

		pZone->SetAdditionalPVPInfo(&pRecvMsg->m_AdditionalPVPInfo);

		pZone->OnCreate();
	}
}

Handler_AG_ZONE_IMPL( AG_ZONE_CHUNTION_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CHUNTION_CREATE_CMD* pRecvMsg = (MSG_AG_ZONE_CHUNTION_CREATE_CMD*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_CHUNTION_CREATE_CMD )" );
#endif

	GameCHuntingRoom * pZone = static_cast<GameCHuntingRoom *>(g_GameZoneManager.CreateZone( 
		eZONETYPE_CHUNTING, pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode ));
	if( pZone )
	{
		pZone->SetRoomTitle(pRecvMsg->m_pszRoomTitle);
		pZone->SetRoomPWD(pRecvMsg->m_pszRoomPWD);
		pZone->SetBaseRoomInfo(&pRecvMsg->m_RoomInfo);
		pZone->SetMasterUserKey(pRecvMsg->m_dwKey);

		pZone->SetAdditionalCHuntingInfo(&pRecvMsg->m_AdditionalInfo);

		pZone->OnCreate();
	}
}

Handler_AG_ZONE_IMPL( AG_ZONE_VILLAGE_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_VILLAGE_CREATE_CMD * pRecvMsg = (MSG_AG_ZONE_VILLAGE_CREATE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_VILLAGE_CREATE_CMD )" );
#endif

	GameVillage * pZone = static_cast<GameVillage *>(g_GameZoneManager.CreateZone( 
		eZONETYPE_VILLAGE, pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode ));
	if( pZone )
	{
		SUNLOG( eFULL_LOG,  "[%u,%u]:���� ����", pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode );
#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
		pZone->OnCreate();
#endif
	}
}

Handler_AG_ZONE_IMPL( AG_ZONE_FIELD_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_FIELD_CREATE_CMD * pRecvMsg = (MSG_AG_ZONE_FIELD_CREATE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_FIELD_CREATE_CMD )" );
#endif

	GameHuntingField * pZone = static_cast<GameHuntingField *>(g_GameZoneManager.CreateZone( 
		eZONETYPE_FIELD, pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode ));
	if( pZone )
	{
		SUNLOG( eFULL_LOG,  "[%u,%u]:�ʵ� ����", pRecvMsg->m_RoomKey, pRecvMsg->m_MapCode );
#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
		pZone->OnCreate();
#endif
	}
}

Handler_AG_ZONE_IMPL(AG_ZONE_DOMINATION_FIELD_CREATE_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_DOMINATION_FIELD_CREATE_CMD * recv_msg = 
        static_cast<MSG_AG_ZONE_DOMINATION_FIELD_CREATE_CMD *>(pMsg);

#ifdef __NA_PACKETFLOW_TEST
    SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( MSG_AG_ZONE_DOMINATION_FIELD_CREATE_CMD )" );
#endif

    GameDominationField* zone = 
        static_cast<GameDominationField *>(g_GameZoneManager.CreateZone(eZONETYPE_DOMINATION_FIELD, 
                                                                        recv_msg->m_RoomKey, 
                                                                        recv_msg->m_MapCode ));
    if (zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not create zone[map:%d]", recv_msg->m_MapCode);
        return;
    }
    
    SUNLOG(eCRITICAL_LOG,"[%u,%u]:DOMINATION FIELD CREATE", recv_msg->m_RoomKey,
                                                            recv_msg->m_MapCode);

#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
    zone->OnCreate();
#endif
}

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
Handler_AG_ZONE_IMPL(AG_ZONE_SPAFIELD_CREATE_CMD)
{
    MSG_AG_ZONE_SPAFIELD_CREATE_CMD* recv_msg = static_cast<MSG_AG_ZONE_SPAFIELD_CREATE_CMD*>(pMsg);

    GameSpaField* zone = static_cast<GameSpaField*>(g_GameZoneManager.CreateZone(eZONETYPE_SPA_FIELD, 
                                                                                 recv_msg->room_key, 
                                                                                 recv_msg->map_code));
    if (zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Cannot create zone[map:%d]", recv_msg->map_code);
        return;
    }
    SUNLOG(eCRITICAL_LOG,"[%u,%u]:SpaField Created!", recv_msg->room_key,recv_msg->map_code);

    zone->OnCreate();
}

//_NA_008069_20150210_ADD_SPA_SYSTEM
Handler_AG_ZONE_IMPL(AG_ZONE_SPA_ENTER_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    MSG_AG_ZONE_SPA_ENTER_SYN* recv_msg = static_cast<MSG_AG_ZONE_SPA_ENTER_SYN*>(pMsg);
    CODETYPE ticket_item_code = recv_msg->item_code;

    Player * player = PlayerManager::Instance()->FindPlayerByUserKey( recv_msg->m_dwKey );
    if ( player == NULL )
    {
        // �÷��̾ ����
        MSG_AG_ZONE_SPA_ENTER_ACK ack_msg;
        ack_msg.m_dwKey = 0;
        ack_msg.spa_mapcode = 0;
        pServerSession->Send((BYTE*)&ack_msg, sizeof(ack_msg));
        return;
    }

    BASE_ITEMINFO* ticket_info = ItemInfoParser::Instance()->GetItemInfo(ticket_item_code);
    if (ticket_info == NULL)
    {
        //�̵� ����
        MSG_AG_ZONE_SPA_ENTER_ACK ack_msg;
        ack_msg.m_dwKey = player->GetUserKey();
        ack_msg.spa_mapcode = 0;
        pServerSession->Send((BYTE*)&ack_msg, sizeof(ack_msg));
        return;
    }
    MAPCODE spa_field_code = ticket_info->m_wSkillCode;

    // Ƽ���� �����ϴ��� üũ�Ѵ�-----------------------------------------------------
    ItemManager* item_manager = player->GetItemManager();

    int ticket_count = item_manager->ExistItem(SI_INVENTORY, ticket_item_code, 1);
    if (ticket_count == 0)
    {
        // Ƽ�Ͼ���
        MSG_AG_ZONE_SPA_ENTER_ACK ack_msg;
        ack_msg.m_dwKey = player->GetUserKey();
        ack_msg.spa_mapcode = 0;
        pServerSession->Send((BYTE*)&ack_msg, sizeof(ack_msg));
        return;
    }

    // ��õ�� �����ϱ� ���� �뺯�Ż��¸� �����Ѵ�--------------------------------------
    player->RemoveDragonTransformStatus();

    {
        MSG_AG_ZONE_SPA_ENTER_ACK ack_msg;
        ack_msg.m_dwKey = player->GetUserKey();
        ack_msg.spa_mapcode = spa_field_code;
        pServerSession->Send((BYTE*)&ack_msg, sizeof(ack_msg));
    }
}

Handler_AG_ZONE_IMPL(AG_ZONE_SPA_LEAVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_SPA_LEAVE_SYN* recv_msg = static_cast<MSG_AG_ZONE_SPA_LEAVE_SYN*>(pMsg);

    //GameServer���� ���� ���ǰ˻�
    DWORD UserKey = recv_msg->m_dwKey;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
    ASSERT( player );
    if( player == NULL ) return;

    // � ������ ������ ���⼭ üũ�Ѵ�.

    {
        MSG_AG_ZONE_SPA_LEAVE_ACK ack_msg;
        ack_msg.m_dwKey = player->GetUserKey();
        ack_msg.village_map_code = player->GetPreVillageMapCode();
        pServerSession->Send((BYTE*)&ack_msg, sizeof(ack_msg));
    }
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

Handler_AG_ZONE_IMPL(AG_ZONE_DOMINATION_FIELD_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_AG_ZONE_DOMINATION_FIELD_INFO_SYN * recv_msg = 
        static_cast<MSG_AG_ZONE_DOMINATION_FIELD_INFO_SYN *>(pMsg);

#ifdef __NA_PACKETFLOW_TEST
    SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( MSG_AG_ZONE_DOMINATION_FIELD_INFO_SYN )" );
#endif

    GameZone* zone = g_GameZoneManager.GetZone(recv_msg->m_RoomKey);
    if (zone == NULL) 
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Room[%d]is not found", recv_msg->m_RoomKey );

        MSG_AG_ZONE_DOMINATION_FIELD_INFO_NAK nak_msg;
        nak_msg.m_RoomKey = recv_msg->m_RoomKey;
        nak_msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTROOM;

        pServerSession->Send((BYTE*)&nak_msg, sizeof(nak_msg));
        return;
    }

    if (zone->GetZoneType() != eZONETYPE_DOMINATION_FIELD)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]This is not domination field[%d]", 
               recv_msg->m_RoomKey);

        MSG_AG_ZONE_DOMINATION_FIELD_INFO_NAK nak_msg;
        nak_msg.m_RoomKey = recv_msg->m_RoomKey;
        nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;

        pServerSession->Send((BYTE*)&nak_msg, sizeof(nak_msg));
        return;
    }

    GameDominationField* domination_field = static_cast<GameDominationField*>(zone);
    domination_field->SetEventState(recv_msg->m_byEventState);
  
    MSG_AG_ZONE_DOMINATION_FIELD_INFO_ACK ack_msg;
    ack_msg.m_RoomKey = recv_msg->m_RoomKey;
    ack_msg.m_offenceGuild = domination_field->GetOffenseGuild();
    ack_msg.m_defenceGuild = domination_field->GetDefenseGuild();
    ack_msg.m_eSetType = DOMINATION_INIT;

    pServerSession->Send((BYTE*)&ack_msg, sizeof(ack_msg));
}

Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_DESTROY_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_DESTROY_CMD * pRecvMsg = (MSG_AG_ZONE_ROOM_DESTROY_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_DESTROY_CMD )" );
#endif

	g_GameZoneManager.DestroyZone( pRecvMsg->m_RoomKey );
}

Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_JOIN_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_JOIN_CMD * pRecvMsg = (MSG_AG_ZONE_ROOM_JOIN_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_JOIN_CMD )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	GameZone * pZone = g_GameZoneManager.GetZone( pRecvMsg->m_RoomKey );
	if(!pZone) 
	{
		SUNLOG( eCRITICAL_LOG, "[AG_ZONE_ROOM_JOIN_CMD]Room[%d]IsNotFound", pRecvMsg->m_RoomKey );
		return;
	}

	//�ʵ��ڵ带 ��´�.
	WORD	wFieldCode = 0;
	if( pRecvMsg->m_wFieldCode == 0 )	wFieldCode = (WORD)pZone->GetEntryFieldCode();
	else								wFieldCode = pRecvMsg->m_wFieldCode;

	//�����ڵ带 ��´�.
	DWORD dwAreaID = 0;
	if( pRecvMsg->m_dwAreaID == 0 )		dwAreaID = pZone->GetEntryAreaID();
	else								dwAreaID = pRecvMsg->m_dwAreaID;

	g_GameZoneManager.ReadyPlayer( pRecvMsg->m_RoomKey, pPlayer, wFieldCode, dwAreaID, &pRecvMsg->m_vStartPos );

	//SUNLOG( eFULL_LOG,  "[U:%d,P:%d,ID:%s][%d]:�÷��̾� ���Ӽ��� ��������CMD", UserKey, pPlayer->GetObjectKey(), pPlayer->GetUserID(), pRecvMsg->m_RoomKey );
}

Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_JOIN_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_JOIN_SYN * pRecvMsg = (MSG_AG_ZONE_ROOM_JOIN_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_JOIN_SYN )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( pPlayer )
	{
		SUNLOG( eCRITICAL_LOG, "[AG_ZONE_ROOM_JOIN_SYN]PlayerIsExist:%d, %s", UserKey, pPlayer->GetCharName() );
		return;
	}

	//////////////////////////////////////////////////////////////
	// �ߺ��α����� ������ ���⿡ �ɸ� �� �ִ�!!!!!!!!
	//////////////////////////////////////////////////////////////
	GameZone * pZone = g_GameZoneManager.GetZone( pRecvMsg->m_RoomKey );
	if(!pZone) 
	{
		SUNLOG( eCRITICAL_LOG, "[AG_ZONE_ROOM_JOIN_SYN]Room[%d]IsNotFound", pRecvMsg->m_RoomKey );
		return;
	}

    //----------------------------------------------------------------------------------------------
    // CHANGES: f110426.2L, changes to forwarding message to make successful replication process.
    MSG_DG_CHARINFO_CONNECT_CHAR_CMD msg_syn;
    msg_syn.m_dwKey = UserKey;
    msg_syn.char_guid = pRecvMsg->m_dwCharGUID;
    msg_syn.session_index = pServerSession->GetSessionIndex();
    msg_syn.forward_msg = *pRecvMsg;
    if (g_pGameServer->SendToServer(GAME_DBPROXY, &msg_syn, msg_syn.GetSize()) == false)
    {
        SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|Msg=fails, network problem|");
        MSG_AG_ZONE_ROOM_JOIN_NAK msg_nak;
        msg_nak.m_dwKey = UserKey;
        msg_nak.m_dwErrorCode = RC::RC_ZONE_DISCONNECTED_DBPROXY;
        // CHANGES: f110712.4L, fixed the null object handling problem
        // because this handler is based on the first instance.
        pServerSession->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    // next process: on receive MSG_DG_CHARINFO_CONNECT_CHAR_CMD from the database proxy server
    return;
}


Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_LEAVE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_LEAVE_CMD * pRecvMsg = (MSG_AG_ZONE_ROOM_LEAVE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_LEAVE_CMD )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	GameZone * pEnterZone	= NULL;
	MAPCODE EnterFieldCode = 0;
	eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
	if( eRoomState & ePRS_AT_ZONE )
	{
		g_GameZoneManager.LeavePlayer( pRecvMsg->m_RoomKey, pPlayer );
		//SUNLOG( eFULL_LOG,  "[Room:%d][U:%d]:AG_ZONE_ROOM_LEAVE_CMD��Ŷ��", pRecvMsg->m_RoomKey, UserKey );
	}

}

Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_LEAVE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_ROOM_LEAVE_SYN * pRecvMsg = (MSG_AG_ZONE_ROOM_LEAVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_LEAVE_SYN )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	if( !pPlayer ) 
	{
		// �濡�� ������� ��û�ε� �̹� �濡 ���ٸ� ���̻� ó���� �ʿ䵵 �˸� �ʿ䵵 ���°� ��������...
/*		MSG_AG_ZONE_ROOM_LEAVE_NAK NakMsg;
		NakMsg.m_dwErrorCode = RC_ROOM_NOT_EXIST_MEMBER;
		pServerSession->Send( &NakMsg, sizeof(NakMsg) );*/
		// ���� �ٷΰ��� ��Ÿ�� ���� ��� �����.
		ASSERT( ! "The Player doesn't exist at The GameServer" );
		return;
	}

	//DISPMSG( "Leave\n" );

	GameZone * pEnterZone	= NULL;
	MAPCODE EnterFieldCode = 0;
	eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
	if( eRoomState & ePRS_AT_ZONE )
	{
		g_GameZoneManager.LeavePlayer( pRecvMsg->m_RoomKey, pPlayer );
	}
    //__NA_0_20090311_ITEMCOPY_BUG_FOR_LEAVESYN_ERROR
	else if( eRoomState & ePRS_BEFORE_ENTER_ZONE )
	{
		// �ٸ� ���� ���� ���� Player_Enter_Syn�� ��ٸ��� ������ �� �ִ�. ����->�ʵ� �̵��� �ε������� �� ��ž���� �ٷ� �̵��� ��� �߻��� �� �ִ�.
		// �̹� LeavePlayer ó���� �س��� �����̹Ƿ� ������ �ɾ���� Ready ���¸� �����ϸ� �ȴ�.
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
	}
	else
	{	
		ASSERT( ! "[AG_ZONE_ROOM_LEAVE_SYN] The ZoneState of Player is not ePRS_AT_ZONE" );

		// ���� ���κ��� MSG_DG_CHARINFO_UDPATE_ACK �޽����� �������� ������ �� �����Ƿ�
		// �ߺ� �α����� ���ϱ� ���� NAK�� ������.
//		MSG_AG_ZONE_ROOM_LEAVE_NAK NakMsg;
//		pServerSession->Send( (BYTE*)&NakMsg, sizeof(NakMsg) );
		return;
	}
	//SUNLOG( eFULL_LOG, "MSG_AG_ZONE_ROOM_LEAVE_SYN[%s][%d]", pPlayer->GetCharName(), pPlayer->GetBehave() );

    MSG_DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN msg_transaction;
    msg_transaction.m_byTransControl = msg_transaction.DBUPDATE_LOCKING;
    msg_transaction.char_guid_ = pPlayer->GetCharGuid(); // CHANGES: f110412.2L
    pPlayer->SendToGameDBPServer(&msg_transaction, sizeof(msg_transaction));

	//��尡 �����ϸ�.
	if( pPlayer->GetCharInfo()->m_GuildGuid != 0 )
	{
		// ĳ���� ����â���� �̵��ϸ� ĳ���� �α׾ƿ� ó�� �ؾ� �Ѵ�.
		if( pRecvMsg->m_ToRoomType == eZONETYPE_CHARSELECT )
		{
			MSG_GZ_GUILD_LOGOUT_CHAR_SYN	synMsg;
			synMsg.m_GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
			synMsg.m_CharGuid = pPlayer->GetCharGuid();
			if( !pPlayer->SendToGuildServer( &synMsg, sizeof( synMsg ) ) )
			{
				// ���� ��� ������ ������ ���ٸ� ���� ó���� �Ұ��� �ϴ�, ���⼭ ��ü�� �����Ѵ�.
				g_GameGuildManager.DestroyMember( pPlayer->GetCharInfo()->m_GuildGuid, pPlayer->GetCharGuid() );
				SUNLOG( eCRITICAL_LOG, "[Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_LEAVE_SYN )] Fail Send GuildServer And DestroyMember, GuildGuid = %u, CharGuid = %d",
					pPlayer->GetCharInfo()->m_GuildGuid, pPlayer->GetCharGuid() );
			}
		}
		else
		{
			//ĳ���� ������ �ƴϸ�, ���Ӽ������� �̵��̹Ƿ� ���Ӽ������� ���� ������ �����Ѵ�.
			g_GameGuildManager.LeaveServer( pPlayer->GetGuildGuid(), pPlayer->GetCharGuid() );
		}
	}

    //���׼���:5898 ������ 2010-02-19 (��籹������) �κ�� �̵��Ұ�� �籸�� ����� �ʱ�ȭ �Ѵ�.
    if( pRecvMsg->m_ToRoomType == eZONETYPE_CHARSELECT ) 
    {
        pPlayer->GetItemManager()->GetTradingImpl()->ClearRepurchasItemList();
    }

	if( FALSE == pPlayer->SerializeInfoToDBProxy(UPDATE_FACTOR_LOGOUT))
	{
		SUNLOG( eFULL_LOG,  "[Handler_AG_ZONE_IMPL( AG_ZONE_ROOM_LEAVE_SYN )] [ERROR]DBP����Ŷ(AG_ZONE_ROOM_LEAVE_SYN:SerializeInfoToDBProxy)�������Ҽ������ϴ�." );
		MSG_AG_ZONE_ROOM_LEAVE_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_DISCONNECT_DBP_CONNETION_ERROR;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		PlayerManager::Instance()->RemovePlayer( pPlayer );
		return;
	}

	MSG_DG_CHARINFO_UPDATE_SYN msg;
	msg.m_ChannelID = g_pGameServer->GetKey().GetChannelID();
    msg.char_guid_ = pPlayer->GetCharGuid(); // CHANGES: f110412.2L
	pPlayer->SendToGameDBPServer( &msg, sizeof(msg) );

	// �߿�!! : DB�� ���� ACK���� ������Ȳ���� Trade�� �Ͼ�� ���� �߻� 
	pPlayer->Release();
}

Handler_AG_ZONE_IMPL( AG_ZONE_MASTER_CHANGED_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_MASTER_CHANGED_CMD * pRecvMsg = (MSG_AG_ZONE_MASTER_CHANGED_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_MASTER_CHANGED_CMD )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	GameZone *pZone = g_GameZoneManager.GetZone( pRecvMsg->m_RoomKey );
	if( !pZone || pZone->GetZoneType() == eZONETYPE_VILLAGE )
	{
		return;
	}

	// ���� �� ��Ƽ���� �����Ѵ�.
	GameRoom *pRoom = (GameRoom*)pZone;
	pRoom->ChangeMaster( UserKey );
}

Handler_AG_ZONE_IMPL( AG_ZONE_PVP_INFO_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_PVP_INFO_CMD * recv_msg = (MSG_AG_ZONE_PVP_INFO_CMD *)pMsg;

	GameZone* zone = g_GameZoneManager.GetZone(recv_msg->m_RoomKey);
	if (zone == NULL || zone->GetZoneType() != eZONETYPE_PVP)
	{
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Is not pvp zone : %d", 
            zone->GetZoneType()
            );
		return;
	}

    if (recv_msg->m_Count < 0 || recv_msg->m_Count > _countof(recv_msg->m_Info))
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : User count is over : %d", 
            recv_msg->m_Count);
        return;
    }

	GamePVPRoom* pvp_room = static_cast<GamePVPRoom *>(zone);
	for (int i = 0 ; i < recv_msg->m_Count ; ++i)
    {
		pvp_room->GetPVPManager()->AddPVPInfo(recv_msg->m_Info[i].m_dwPlayerKey, 
                                              recv_msg->m_Info[i].m_Team );
    }

	pvp_room->GetPVPManager()->StartPVP();
}


Handler_AG_ZONE_IMPL( AG_ZONE_PORTAL_FIELD_MOVE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_PORTAL_FIELD_MOVE_SYN* pRecvMsg = (MSG_AG_ZONE_PORTAL_FIELD_MOVE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_PORTAL_FIELD_MOVE_SYN )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	//1. ��Ż������ ������ �´�.
	sMAP_ENTERENCE_PORTAL* pPortalInfo = MapInfoParser::Instance()->GetMapPortal( pRecvMsg->m_wIndex );
	if( !pPortalInfo )
	{
		MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_INVALID_MAPCODE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
    const MAPCODE to_field_mapcode = static_cast<MAPCODE>(pPortalInfo->wFieldTo);
    GameZone* const to_field_zone = g_GameZoneManager.GetFieldVillageZone(to_field_mapcode);

	GameZone*  pZone  = pPlayer->GetGameZonePtr();
	GameField* pField = pPlayer->GetField();

#ifdef __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
    if (!pZone)
    {
        MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
        msg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }

    if ( pZone->GetMapCode() != HardCode::MAPCODE_for_TUTORIAL_FIELD )
    {
        if( !pField || 
            (pZone->GetMapCode()    != pPortalInfo->wWorldFrom) ||
            (pField->GetFieldCode() != pPortalInfo->wFieldFrom) )
        {
            MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
            msg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
            pPlayer->SendPacket( &msg, sizeof(msg) );
            return;
        }
    }
#else
    if( !pZone  || 
        !pField || 
        (pZone->GetMapCode()    != pPortalInfo->wWorldFrom) ||
        (pField->GetFieldCode() != pPortalInfo->wFieldFrom) )
    {
        MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
        msg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }
#endif //__NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT

    eZONE_STATE ZoneState = pPlayer->GetGameZoneState();
    // ���� �÷��̾ ������ �ְ�, ���� �����̸�.. �̵����� �ʴ´�.
    // #3149 �������̸� �׾������� �ʵ�� ���ư��Ⱑ �����ؾ� �Ѵ�. (��ȹ�� Ȯ��)
    if ((pPlayer->GetHP() == 0) &&
        (ZoneState == ePRS_AT_VILLAGE ||
         ZoneState == ePRS_AT_FIELD   /*||
         ZoneState == ePRS_AT_DOMINATION_FIELD*/))        // _NA001385_20090924_DOMINATION_BASE (�ڵ�����)
    {
        MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
        msg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
        pPlayer->SendPacket( &msg, sizeof(msg) );
        return;
    }
         
	// ��������Ʈ�̸�, ����Ǿ� �ִ��� üũ
	//if( pPortalInfo->bMoveType == sMAP_ENTERENCE_PORTAL::MOVE_TYPE_WAYPOINT &&
	//	FALSE == pPlayer->GetWayPoint()->IsExist(pPortalInfo->wWorldTo) )
	//{
	//	MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
	//	msg.m_byErrorCode = RC::RC_ROOM_DONT_EXIST_WAYPOINT;
	//	pPlayer->SendPacket( &msg, sizeof(msg) );
	//	return;
	//}
	// ��������Ʈ�̸�, ī���� �̿��� �� ����.	
	if( pPortalInfo->bMoveType == sMAP_ENTERENCE_PORTAL::MOVE_TYPE_WAYPOINT && !pPlayer->GetChaoState().CanUseFuction( CHAO_USE_PORTAL ) )
	{
		MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;		
	}
	if( pPortalInfo->bMoveType == sMAP_ENTERENCE_PORTAL::MOVE_TYPE_WAYPOINT && pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING )
	{
		MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}
	// ����üũ
	//2. �̼� Ŭ���� üũ
	//3. �ش� ����Ʈ üũ
	//4. ����üũ
	//5. ������ üũ
	//6. �� üũ

	//2. �̼� Ŭ���� üũ
	if( 0 != pPortalInfo->byMissionCode &&
		0 == pPlayer->GetMissionManager()->GetMissionPoint( pPortalInfo->byMissionCode ) )
	{
		MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_ROOM_PREVIOUS_MISSION_CLEAR;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	//3. ����Ʈ üũ
	//////////////////////////////////////////////////////////////////////////
	if( 0 != pPortalInfo->wQuestCode )
	{
		QUESTPART_COMPL * pComplQuest = pPlayer->GetQuestManager()->FindComplQuest( pPortalInfo->wQuestCode );
		if( !pComplQuest || pComplQuest->m_Num == 0  )
		{
			MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
			nmsg.m_byErrorCode = RC::RC_ROOM_PREVIOUS_MISSION_CLEAR;
			pPlayer->SendPacket( &nmsg, sizeof(nmsg) );

			return;
		}
	}

	//4. ����üũ
	if( (pPlayer->GetLevel() < pPortalInfo->byMin_Level) || 
		(0 != pPortalInfo->byMax_Level &&							//< pPortalInfo->byMax_Level 0 Max LV üũ ���ϰ�
		(pPlayer->GetLevel() > pPortalInfo->byMax_Level)) )
	{
		MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_ROOM_INVALID_LIMITLEVEL;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if(!pPlayer->GetBetaKeyInfo()->CanAbilityContent(pPlayer->GetGameZonePtr()->GetMAPINFO()->nBetaKey))
    {        
        MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
        nmsg.m_byErrorCode = RC::RC_ROOM_CANNOT_BETAKEY;
        pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
        return;
    }
    DWORD portal_heim = BetaKeyImpl::GetPortalMoney(pPlayer->GetBetaKeyInfo(), pPortalInfo);
#else
    DWORD portal_heim = pPortalInfo->dwHeim;
#endif

	//5. �� üũ��
	if( 0 != portal_heim &&
		portal_heim > pPlayer->GetMoney() )
	{
		MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
		nmsg.m_byErrorCode = RC::RC_ROOM_INSUFFICIENT_MONEY;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	//5. ������ üũ
	//////////////////////////////////////////////////////////////////////////
	// ���? ������? ���? ���ǵȰ� ����.  �켱�� ������ �ڵ带 ������, 1���� �����϶� üũ�Ѵ�.
	if( 0 != pPortalInfo->wItemCode &&
		0 != pPortalInfo->wItemNum )
	{
		if( pPortalInfo->bWaste_Item )
		{
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            if (to_field_zone == NULL) {
                return;
            }

            MSG_CG_ITEM_LOSE_ACK ack_msg;
            POSTYPE ItemNum = (POSTYPE)pPortalInfo->wItemNum;
            WORD item_code = (WORD)pPortalInfo->wItemCode;
            SCItemSlot deleted_item_slot;
            if(to_field_zone->GetZoneType() == eZONETYPE_DOMINATION_FIELD)
            {
                GameDominationField* const domination_field = static_cast<GameDominationField*>(to_field_zone);
                if (domination_field && 
                    (domination_field->GetEventState() == DOMINATION_EVENT_START ||
                    domination_field->GetEventState() == DOMINATION_EVENT_WAIT))
                {
                    if( RC::RC_ITEM_SUCCESS != 
                        pPlayer->GetItemManager()->DeleteAllKindOfItem( \
                                                    1, &pPortalInfo->wItemCode, &ItemNum, 
                                                    &(ack_msg.m_ItemInfo), &deleted_item_slot ) )
                    {
                        MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
                        nmsg.m_byErrorCode = RC::RC_ROOM_NEED_ITEM;
                        pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
                        return;
                    }
                }
            }
            else
            {
                if( RC::RC_ITEM_SUCCESS != 
                    pPlayer->GetItemManager()->DeleteAllKindOfItem( \
                                                1, &pPortalInfo->wItemCode, &ItemNum, \
                                                &(ack_msg.m_ItemInfo), &deleted_item_slot ) )
                {
                    MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
                    nmsg.m_byErrorCode = RC::RC_ROOM_NEED_ITEM;
                    pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
                    return;
                }
            }
            pPlayer->SendPacket(&ack_msg, ack_msg.GetSize());
            GAMELOG->LogItem(ITEM_USE, pPlayer, &deleted_item_slot, 0, ItemNum);
#else
            POSTYPE ItemNum = (POSTYPE)pPortalInfo->wItemNum;
            INVEN_DELETE_TOTAL_INFO tmp_info;	//< ������ �ʴ� �ӽ� ����ü
            if( RC::RC_ITEM_SUCCESS != 
                pPlayer->GetItemManager()->DeleteAllKindOfItem( 1, &pPortalInfo->wItemCode, &ItemNum, &tmp_info ) )
            {
                MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
                nmsg.m_byErrorCode = RC::RC_ROOM_NEED_ITEM;
                pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
                return;
            }
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
        }
        else
        {
            if( pPortalInfo->wItemNum != 
                pPlayer->GetItemManager()->ExistItem( SI_INVENTORY, pPortalInfo->wItemCode, pPortalInfo->wItemNum ) )
            {
                MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
                nmsg.m_byErrorCode = RC::RC_ROOM_NEED_ITEM;
                pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
                return;
            }
        }
    }

    // â�� ���� üŷ �߰� : �������� �̿��� �̵��ô� üũ �ϰ� ������,
    // ��������Ʈ�� ��� Ŭ�󿡼�  NPC���� �Ÿ��� üũ [8/11/2009 lst1024]

    //6. â�� ���� üũ 
    //////////////////////////////////////////////////////////////////////////
    const RC::ePORTAL_RESULT retValue = pPlayer->CanPortal();
    if (RC::RC_PORTAL_CAN_SUCCESS != retValue)
    {
        MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nmsg;
        nmsg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
        pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
        return;
    }

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //������ ������ �̵��� ���
    GameDominationManager* const domination_manager = GameDominationManager::Instance();
    if (domination_manager->IsDominationArea(to_field_mapcode) == true)
    {
        if (domination_manager->CanEnterDominationField(pPlayer, to_field_mapcode) == false)
        {
            MSG_AG_ZONE_PORTAL_FIELD_MOVE_NAK nak_msg;
            nak_msg.m_byErrorCode = RC::RC_ROOM_ISNOT_JOINMEMBER;
            pPlayer->SendPacket(&nak_msg, sizeof(nak_msg));
			return;
        }
    }

	// ������ : �� ����
	if( 0 != portal_heim )
	{
		pPlayer->MinusMoney( portal_heim );
        // CHANGES: f110103.2L
        if (ns_object::PlayerZoneTransaction* trans_info = pPlayer->GetZoneTransactionInfo())
        {
            ns_object::PlayerZoneTransaction::Data data = { 0, };
            data.consumed_money = portal_heim;
            trans_info->BeginTransaction(data);
        };
		GAMELOG->LogMoney( MONEY_SPEND_WAYPOINT, pPlayer, portal_heim );
        g_pGameServer->NotifyHeimConsumption(pPlayer->GetCharGuid(), eHEIM_CONSUMPTION_WAYPOINT, portal_heim);
    }

	MSG_AG_ZONE_PORTAL_FIELD_MOVE_ACK ack;
	ack.m_wIndex = pRecvMsg->m_wIndex;
	pPlayer->SendPacket( &ack, sizeof(ack) );
	return;
}


Handler_AG_ZONE_IMPL( AG_ZONE_RETURN_VILLAGE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_RETURN_VILLAGE_SYN* pRecvMsg = (MSG_AG_ZONE_RETURN_VILLAGE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_RETURN_VILLAGE_SYN )" );
#endif

	DWORD UserKey = pRecvMsg->m_dwKey;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( UserKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	// � ������ ������ ���⼭ üũ�Ѵ�.
	if( pPlayer->GetHP() != 0 )
	{
		MSG_AG_ZONE_RETURN_VILLAGE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}

	MSG_AG_ZONE_RETURN_VILLAGE_ACK ack;
	ack.m_wMapCode = pPlayer->GetPreVillageMapCode();
	pPlayer->SendPacket( &ack, sizeof(ack) );
}

Handler_AG_ZONE_IMPL( AG_ZONE_CHANGE_SPECIALTYPE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CHANGE_SPECIALTYPE_CMD* pRecvMsg = (MSG_AG_ZONE_CHANGE_SPECIALTYPE_CMD*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_CHANGE_SPECIALTYPE_CMD )" );
#endif

	GameZone * pZone = g_GameZoneManager.GetZone( pRecvMsg->m_RoomKey );
	if( !pZone )
	{
		SUNLOG( eCRITICAL_LOG, "[AG_ZONE_CHANGE_SPECIALTYPE_CMD]Room[%d]IsNotFound", pRecvMsg->m_RoomKey );
		return;
	}

	// ���۹����� ������ m_SpecialType �� �Ϲ����� �ٲ۴�.
	pZone->SetSpecialType( (eROOM_SPECIAL_TYPE)pRecvMsg->m_SpecialType );
}


Handler_AG_ZONE_IMPL(AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN* const pRecvMsg =
		static_cast<MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN*>(pMsg);
#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "Handler_AG_ZONE_IMPL("__FUNCTION__")");
#endif

	const DWORD user_key = pRecvMsg->m_dwKey;
	Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
	if(!pPlayer)
		return;

	MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK msgNAK;
	BYTE error_group = msgNAK.eErr_ItemRC;
	
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
	{
		MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK msgACK;

        ItemManager* const pItemManager = pPlayer->GetItemManager();
        error_group = msgNAK.eErr_ItemRC;
        RC::eITEM_RESULT ret_CanUseItemForIndun =
            pItemManager->GetItemFunctionBattlezoneEnterance().CanUseItemForTopOfTrial(
            pRecvMsg->m_ItemPos, msgACK.m_MapCode, msgACK.m_UseCharge); // side-effect: item lock
        IfFailThenBreak(RC::RC_ITEM_SUCCESS == ret_CanUseItemForIndun, ret_CanUseItemForIndun);

        // (BUGFIX) (091211) (WAVERIX) ��ȹ���� �� ������ ������ �������� ���� ��ʰ� ���� �����Ѵ�.
        // �ݵ�� �ذ��ϰ� �Ѿ���� ������, ������ ��� �׽�Ʈ�� �̷����ٰ� �� ���� ���� �����
        // ���� ó���� �����ϱ�� �Ѵ�.
        error_group = msgNAK.eErr_RoomRC;
        const sMAPINFO* const pMapInfo = MapInfoParser::Instance()->FindMapInfo(msgACK.m_MapCode);
        IfFailThenBreak(pMapInfo != NULL, RC::RC_ROOM_INVALID_MAPCODE);

        const eZONE_RULESET& ruleset = pMapInfo->Ruleset;
        if(ruleset.rules & ruleset.ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK)
        {
            // ��� ���� �˻�
            error_group = msgNAK.eErr_RoomRC;
            const BASE_ITEMINFO* const pItemInfo = pPlayer->GetEquipItemInfo(eEQUIPCONTAINER_WEAPON);
            IfFailThenBreak(!pItemInfo, RC::RC_ROOM_NEED_UNEQUIP_WEAPON_FOR_ENTER);
        }

        BOOLEAN partyProblemOccurred = 0;
		//(lst1024)(090410) �߱� ������ ���� ���� ����
		// ��Ƽ ������ �̵� �غ� �Ǿ� �ִ��� üũ.
		const WORD partyKey = pPlayer->GetPartyState().GetPartyKey();
		if(0 != partyKey)
		{
            // (WAVERIX) (WOPS:3943) �� �̵��� ���� ��Ƽ �˻� ���� ��, ����� �� �̵� �Ұ� ����
            //���� ���� ����, ResultCode Group���� ���� ����
            error_group = msgNAK.eErr_RoomRC;
			CheckReadyPartyMember check_ready;
			if(NewGamePartyManager::Instance()->For_eachPlayers(partyKey, check_ready))
			{
                IfFailThenBreak(check_ready.m_AllReady, RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER);
			}
            else
                partyProblemOccurred = TRUE;
		}

        if(partyProblemOccurred)
        {
            SUNLOG(eCRITICAL_LOG,
                   "[AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN] [%s]�� ��Ƽ�� ã���� ����. "
                   "partyKey[%d]",
                   pPlayer->GetCharName(), partyKey);
        }


        // NOTE, __NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
        // ��Ƽ���� �������� �˻� (Timeline - Quest|Mission|FreePass|...)
        // CanUseItemForTopOfTrial�� ���º����� ����Ű�� �Լ���. �ܼ�ȭ ��Ű�� ����� ������ ��.
        //BOOLEAN checkParty = FALSE == (ruleset.rules & ruleset.ZONE_RULE_INDUN_SINGLE_PLAYING_MODE);
        ;{
            // (CHANGES) (f100514.3L) add the flag to prevent ANY actions that
            // a player do a delayed spiking whether the player request a LEAVE.
            pPlayer->GetStatusManager()->ChangeInterState(eCHAR_STATE_ZONE_TRANSACTION, true);
        };
        pPlayer->SendPacket(&msgACK, sizeof(msgACK));

		return /*SUCCESS RESULT*/;
	}
	FLOWCONTROL_END;

    SUNLOG(eCRITICAL_LOG,
           "["__FUNCTION__"][ErrCode:%2u] {%s} {CharName:%s,Party:%u}",
           flow.GetErrorCode(), flow.GetErrorString(),
           pPlayer->GetCharName(), pPlayer->GetPartyState().GetPartyKey());
	
	msgNAK.m_ErrorKind   = (BYTE)error_group;
	msgNAK.m_dwErrorCode = flow.GetErrorCode();
	pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
}

Handler_AG_ZONE_IMPL( AG_ZONE_INSTANCE_CREATE_CMD )
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_INSTANCE_CREATE_CMD * pRecvMsg = (MSG_AG_ZONE_INSTANCE_CREATE_CMD *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_INSTANCE_CREATE_CMD )" );
#endif

	GameInstanceDungeon* pZone = static_cast<GameInstanceDungeon *>(
		g_GameZoneManager.CreateZone(
			eZONETYPE_INSTANCE
			, pRecvMsg->m_RoomKey
			, pRecvMsg->m_MapCode
			));

	if( pZone )
	{
		//pZone->SetRoomTitle(pRecvMsg->m_pszRoomTitle);
		//pZone->SetRoomPWD(pRecvMsg->m_pszRoomPWD);
		pZone->SetBaseRoomInfo(&pRecvMsg->m_RoomInfo);
		pZone->SetMasterUserKey(pRecvMsg->m_dwKey);
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        pZone->SetPartyZoneType(pRecvMsg->party_zone_info_);
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

		BOOST_STATIC_ASSERT( INDUN_KIND_MAX == 7 );	// �߰� Ÿ�� ����� �ϴ� ���� �߰��� ��
		const eINSTANCE_DUNGEON_KIND kindOfIndun = (eINSTANCE_DUNGEON_KIND)pRecvMsg->m_TypeOfIndun;
		switch( kindOfIndun )
		{
		case INDUN_KIND_TOP_OF_TRIAL:
		case INDUN_KIND_SS_QUEST:
		case INDUN_KIND_100LEVEL_QUEST:
		case INDUN_KIND_EXT_03:
		case INDUN_KIND_EXT_04:
		case INDUN_KIND_MAYA_SCROLL:
            pZone->SetTypeOfIndun( kindOfIndun );
            break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        case INDUN_KIND_CHAOS_ZONE:
            pZone->SetTypeOfIndun(kindOfIndun);
            pZone->SetChaosZoneType(pRecvMsg->chaos_zone_type_);
            break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

		default:
			ASSERT( !"Invalid TypeOfIndun : INSTANCE_CREATE" );
			pZone->SetTypeOfIndun( INDUN_KIND_MAX );
			break;
		}
		pZone->OnCreate();
	}
}

Handler_AG_ZONE_IMPL( AG_ZONE_TUTORIAL_SKIP_SYN )
{
    // NOTE: f110809.6L, some cases like China version, many players will play at once.
    // according to planner's request, this handler restored.
    __TOUCHED((pServerSession, pMsg, wSize));
#ifdef __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
    GameField* const game_field = player ? player->GetField() : NULL;
    if (FlowControl::FCAssert(player != NULL && game_field != NULL) == false) {
        return;
    };

    GameZone* const game_zone = game_field->GetGameZone();
    const MAPCODE entered_map_code = game_zone->GetMapCode();
    if (entered_map_code != HardCode::MAPCODE_for_TUTORIAL_FIELD) {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=violated game rule, CharName='%s', CurrendMap=%d|"),
               player->GetCharName(), entered_map_code);
        return;
    };

    WzVector standing_pos = { 0.0f };
    player->Portal(ePORTAL_TYPE_GM, HardCode::MAPCODE_for_VALDCHEN, standing_pos, 0);
#endif
#if SUN_CODE_BACKUP // NOTE: old-contents support
    const MAPCODE HARDCODED_TUTORIAL_MISSION_MAPCODE = 31;

    const DWORD CHAR_VILLAGE_TRINGGEL_BITMAP = (1 << eCHAR_BERSERKER | 1 << eCHAR_ELEMENTALIST);
    const DWORD CHAR_VILLAGE_VALDCHEN_BITMAP = (1 << eCHAR_DRAGON | 1 << eCHAR_VALKYRIE |
                                                1 << eCHAR_SHADOW);


    MSG_AG_ZONE_TUTORIAL_SKIP_SYN* pRecvMsg = (MSG_AG_ZONE_TUTORIAL_SKIP_SYN*)pMsg;
    DWORD dwUserKey = pRecvMsg->m_dwKey;
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(dwUserKey);
    if(!pPlayer) return;

    GameZone* pEnterZone = NULL;
    MAPCODE EnterFieldCode = 0;
    eZONE_STATE eRoomState = pPlayer->GetGameZone(pEnterZone, EnterFieldCode);

    if(HardCode::MAPCODE_for_TUTORIAL_FIELD != EnterFieldCode)
    {
        // error ó��?
        return;
    }

    DWORD dwCharClass = (DWORD)pPlayer->GetCharType();
    dwCharClass = 1 << dwCharClass;

    MAPCODE mcRETURN_MAPCODE
        = CHAR_VILLAGE_TRINGGEL_BITMAP & dwCharClass ? HardCode::MAPCODE_for_TRINGGEL
        : CHAR_VILLAGE_VALDCHEN_BITMAP & dwCharClass ? HardCode::MAPCODE_for_VALDCHEN
        : 0;

    if(mcRETURN_MAPCODE)
    {
        WzVector vecDestPos;
        ZeroMemory(&vecDestPos, sizeof(vecDestPos));
        pPlayer->Portal(ePORTAL_TYPE_GM, mcRETURN_MAPCODE, vecDestPos, 0);
    }
#endif
}



Handler_AG_ZONE_IMPL( AG_ZONE_RETURN_TO_ROOM_INFO_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_RETURN_TO_ROOM_INFO_SYN* pRecvMsg = (MSG_AG_ZONE_RETURN_TO_ROOM_INFO_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG( eFULL_LOG, "Handler_AG_ZONE_IMPL( AG_ZONE_RETURN_TO_ROOM_INFO_SYN )" );
#endif

	PlayerManager* pPlayerManager = PlayerManager::Instance();

	DWORD dwUserKey = pRecvMsg->m_dwKey;
	Player* pPlayer = pPlayerManager->FindPlayerByUserKey( dwUserKey );

	MSG_AG_ZONE_RETURN_TO_ROOM_INFO_ACK msgACK;

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
        IfFailThenBreak(pPlayer, 10);

        IfFailThenBreak(g_FunctionalRestrictMgr.IsPossibleFunction(
            eFUNCTIONALRESTRICT_PARTY_RETURN_FOR_UCUSER), 20);

        // (WAVERIX) (090224) (NOTE) ��ǻ� Abusing ���� ó�� ���� ������ ���簡 �ʿ� ��������.
        // ������ �� Flow�� �״�� ���ܵ� ������ �߰��� ��� ó���� �䱸�� �� ���� ��,
        // ���� ���� �� �ִ� �÷ο츦 �����ϱ� �����̴�. ���� ���Ӽ����� �ʵ� ���Ἲ �˻絵
        // ����� �ʿ��ϴ�. (�߰� ����... 2�� ��Ƽ��� ���������̴�...�Ф�)
        NewGamePartyManager* pPartyManager = NewGamePartyManager::Instance();
        WORD partyKey = pRecvMsg->PARTY_KEY();
        IfFailThenBreak(pPartyManager->FindParty(partyKey), 30);
        //
        msgACK.m_byResult = MSG_AG_ZONE_RETURN_TO_ROOM_INFO_ACK::SUCCESS;
        msgACK.REQUEST_USER_KEY() = pRecvMsg->REQUEST_USER_KEY();
        msgACK.PARTY_KEY() = partyKey;
        pPlayer->SendPacket(&msgACK, msgACK.GetSize());

		return;
    }
    FLOWCONTROL_END;

	msgACK.m_byResult = MSG_AG_ZONE_RETURN_TO_ROOM_INFO_ACK::FAIL;
	msgACK.REQUEST_USER_KEY() = pRecvMsg->REQUEST_USER_KEY();

	if(pPlayer)
		pPlayer->SendPacket(&msgACK, msgACK.GetSize());
	MessageOut(eCRITICAL_LOG,
               _T("MSG_AG_ZONE_RETURN_TO_ROOM_INFO_SYN - ERROR %s"),
               flow.GetErrorString());
}


Handler_AG_ZONE_IMPL( AG_ZONE_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_ZONE_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN* pRecvMsg = (MSG_AG_ZONE_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN*)pMsg;
	DWORD userKey = pRecvMsg->m_dwKey;
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( userKey );
	if( !pPlayer ) return;

	ItemManager* pItemMGR = pPlayer->GetItemManager();
	MAPCODE rMapCode = 0;
    RC::eITEM_RESULT rt = pItemMGR->GetItemFunctionBattlezoneEnterance().CanUseItemForPortalQuest(
        pRecvMsg->m_ItemPos, rMapCode);

	//(lst1024)(090410) �߱� ������ ���� ���� ����
	BOOL bAllReady = TRUE;
	if( rt == RC::RC_ITEM_SUCCESS )
	{	
		// ��Ƽ ������ �̵� �غ� �Ǿ� �ִ��� üũ.
		const WORD PartyKey = pPlayer->GetPartyState().GetPartyKey();

		if( 0 != PartyKey )
		{
			CheckReadyPartyMember opr;
			if( NewGamePartyManager::Instance()->For_eachPlayers(PartyKey,opr) )
				bAllReady = opr.m_AllReady;
		}
	}

	MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK msgACK;

    if (RC::RC_ITEM_SUCCESS == rt && bAllReady && rMapCode) {
        // (CHANGES) (f100514.3L) add the flag to prevent ANY actions that
        // a player do a delayed spiking whether the player request a LEAVE.
        pPlayer->GetStatusManager()->ChangeInterState(eCHAR_STATE_ZONE_TRANSACTION, true);
		msgACK.SetSuccessResult(rMapCode);
    }
    else if (bAllReady) {
        msgACK.SetFailResult(MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::RET_ITEM,
                             static_cast<DWORD>(rt));
    }
    else {
        msgACK.SetFailResult(MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::RET_ROOM,
                             RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER);
    }

// 	MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK msgACK;
// 
// 	RC::RC_ITEM_SUCCESS == rt && rMapCode ?
// 		msgACK.SetSuccessResult( rMapCode )
// 		: msgACK.SetFailResult( MSG_AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK::eFAIL_RC_CODE::RET_ITEM, (DWORD)rt );

	pPlayer->SendPacket( &msgACK, msgACK.GetSize() );
}


Handler_AG_ZONE_IMPL( AG_ZONE_SSQ_JOIN_INFO_SYN )
{
	GameWarControlManager::OnRecv( pMsg, wSize, GameSSQSessionCmd::FromAgent( pServerSession ) );
}

Handler_AG_ZONE_IMPL( AG_ZONE_SSQ_JOIN_SYN )
{
	GameWarControlManager::OnRecv( pMsg, wSize, GameSSQSessionCmd::FromAgent( pServerSession ) );
}

Handler_AG_ZONE_IMPL( AG_ZONE_SSQ_ROOM_CTRL_CMD )
{
	GameWarControlManager::OnRecv( pMsg, wSize, GameSSQSessionCmd::FromAgent( pServerSession ) );
}

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

namespace nsQuestPortal {

typedef MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN   ENTER_SYNC_TYPE;
struct EnterSynArg
{
    const sMAPINFO* pMapInfo;
    const sMAP_ENTERENCE_PORTAL* pMapPortal;
    const MSG_AG_ZONE_QUEST_PORTAL_ENTER_SYN* pRecvMsg; // ���Ǹ� �˻�ÿ��� NULL�� �� �ִ�.
    MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK* pReplyMsg;      // ���Ǹ� �˻�ÿ��� NULL�� �� �ִ�.
};

static BOOLEAN
IsValidPlayerState(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    GameField* pGameField = pPlayer->GetField();
    if(pGameField == NULL)
    {
        if(pArg->pReplyMsg)
        {
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_INVALIDROOMTYPE;
            pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
        }
        return FALSE;
    }

    eZONE_STATE zoneState = pPlayer->GetGameZoneState();
    BOOLEAN isValidState = zoneState == ePRS_AT_VILLAGE ||
                           zoneState == ePRS_AT_DOMINATION_FIELD ||
                           zoneState == ePRS_AT_FIELD;

    if(isTempPortal == FALSE )
    {
        if (isValidState)
        {
            GameZone* game_zone = pGameField->GetGameZone();
            bool validated_field_check = (pGameField->GetFieldCode() == pArg->pMapPortal->wFieldFrom);
        #ifdef __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
            if ((validated_field_check == false) &&
                (game_zone->GetMapCode() == HardCode::MAPCODE_for_TUTORIAL_FIELD))
            {
                validated_field_check = true;
            }
        #endif
            isValidState = validated_field_check &&
                           game_zone->GetMapCode() == pArg->pMapPortal->wWorldFrom;
        };
    }
    isValidState = isValidState && pPlayer->IsAlive();

    if(isValidState == FALSE)
    {
        if(pArg->pReplyMsg)
        {
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
            pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
        }
        return FALSE;
    }

    if(isTempPortal == FALSE )
    {
        if(pArg->pMapPortal->bMoveType == sMAP_ENTERENCE_PORTAL::MOVE_TYPE_WAYPOINT)
        {
            BOOLEAN isDisableFuncChao = pPlayer->GetChaoState().CanUseFuction(CHAO_USE_PORTAL);
            // �Ʒ��� ��Ȳ�� ���ؼ��� ���� WAYPOINT�� �ƴϾ ������ �� �ѵ�... ������ ����.
            BOOLEAN isUnexpected = pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING;
            if(isDisableFuncChao || isUnexpected)
            {
                if(pArg->pReplyMsg)
                {
                    pArg->pReplyMsg->errorCode = isDisableFuncChao
                                                    ?   RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE
                                                    :   RC::RC_ROOM_INVALID_PLAYER_STATE;
                    pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
                }
                return FALSE;
            }
        }
    }
    return TRUE;
}

static BOOLEAN
IsEnterableMap(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    if(pArg->pMapInfo == NULL ||
       !(pArg->pMapInfo->byMKind == eZONETYPE_MISSION ||
         pArg->pMapInfo->byMKind == eZONETYPE_INSTANCE))
    {
        if(pArg->pReplyMsg)
        {
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_INVALID_MAPCODE;
            pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
        }
        return FALSE;
    }
    return TRUE;
}

static BOOLEAN
IsValidPosition(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    BOOLEAN isPositionPass = FALSE;
    if(isTempPortal)
    {
        isPositionPass = TRUE;  // �׳� ���� �˻�ÿ��� pArg->pRecvMsg == NULL�� �� �ִ�.
        if(pArg->pRecvMsg)
        {
            WzVector portalPosition = pArg->pRecvMsg->portalPosision;
            portalPosition -= *pPlayer->GetPosPtr();
            const FLOAT LengthSquareLimit = 10.0f * 10.0f;
            isPositionPass = VectorLength2(&portalPosition) <= LengthSquareLimit;
        }
    }
    else
    {
        if (pArg->pMapPortal)
        {
            GameField* pGameField = pPlayer->GetField();
            GameZone* game_zone = pGameField->GetGameZone();
            bool validated_field_check = (pGameField->GetFieldCode() == pArg->pMapPortal->wFieldFrom);
        #ifdef __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
            if ((validated_field_check == false) &&
                (game_zone->GetMapCode() == HardCode::MAPCODE_for_TUTORIAL_FIELD))
            {
                validated_field_check = true;
            }
        #endif
            isPositionPass = validated_field_check;
        };
    }

    if(isPositionPass == FALSE)
    {
        if(pArg->pReplyMsg)
        {
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_FAILED; // �Ÿ�, ��ġ ����
            pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
        }
    }
    return isPositionPass;
}

// (WAVERIX) (090330) (LOGIC-UPDATE) �̼Ǻ��� ó���� �Ϸ���� ���� ��� ���� ���� ������� ó��
static BOOLEAN
ProcessEnterRestrictCondition(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    MissionManager* pMissionManager = pPlayer->GetMissionManager();
    BOOLEAN isOpened = !!pMissionManager->EnableWindowOpen();
    if(isOpened && pArg->pReplyMsg)
    {
        pArg->pReplyMsg->errorCode = RC::RC_ROOM_FAILED; // ���� ����
        pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
    }

    if(isOpened)
        TriggerManagerEx::Process_RewardWindow(pPlayer);

    return isOpened == FALSE;
}

static BOOLEAN
IsFreePassLevel(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    const LEVELTYPE playerLevel = pPlayer->GetLevel();
    return (pArg->pMapInfo->freePassLV <= playerLevel);
}

struct QuestOrMissionResult
{
    BOOLEAN bQuestCheckSkip;
    BOOLEAN bMissionCheckSkip;
    BOOLEAN isQuestPass;
    BOOLEAN isMissionPass;
};

static BOOLEAN
IsMissionOrQuestPass(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg,
                     QuestOrMissionResult* pResult)
{
    ZeroMemory(pResult, sizeof(*pResult));
    //BOOLEAN bQuestCheckSkip = FALSE;
    //BOOLEAN bMissionCheckSkip = FALSE;
    //BOOLEAN isQuestPass = FALSE;
    //BOOLEAN isMissionPass = FALSE;
    QuestManager_Concrete* pQuestManager = pPlayer->GetQuestManager();
    MissionManager* pMissionManager = pPlayer->GetMissionManager();

    if(isTempPortal)
    {
        pResult->bQuestCheckSkip = pArg->pMapInfo->completeQCode == 0;
        if(pArg->pMapInfo->completeQCode)
        {
          QUESTPART_COMPL* pComplete = pQuestManager->FindComplQuest(pArg->pMapInfo->completeQCode);
          pResult->isQuestPass = pComplete && pComplete->m_Num;
        }
        pResult->bMissionCheckSkip = pArg->pMapInfo->completeMCode == 0;
        if(pArg->pMapInfo->completeMCode)
            pResult->isMissionPass =
                !!pMissionManager->GetMissionPoint((BYTE)pArg->pMapInfo->completeMCode);
    }
    else
    {
        pResult->bQuestCheckSkip = pArg->pMapPortal->wQuestCode == 0;
        if(pArg->pMapPortal->wQuestCode)
        {
            QUESTPART_COMPL* pComplete = pQuestManager->FindComplQuest(pArg->pMapPortal->wQuestCode);
            pResult->isQuestPass = pComplete && pComplete->m_Num;
        }
        pResult->bMissionCheckSkip = pArg->pMapPortal->byMissionCode == 0;
        if(pArg->pMapPortal->byMissionCode)
            pResult->isMissionPass =
                !!pMissionManager->GetMissionPoint(pArg->pMapPortal->byMissionCode);
    }

    if(pResult->isQuestPass || pResult->isMissionPass)
      return TRUE;
    if(pResult->bQuestCheckSkip && pResult->bMissionCheckSkip)
      return TRUE;

    // ����Ʈ | �̼� ���� ó���� ��.
    if(pArg->pReplyMsg)
    {
        if(pResult->bQuestCheckSkip == FALSE && pResult->isQuestPass == FALSE)
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_PREVIOUS_QUEST_CLEAR;
        else if(pResult->bMissionCheckSkip == FALSE && pResult->isMissionPass == FALSE)
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_NEED2_CLEAR_PRE_MISSION;
        pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
    }

    return FALSE;
}

static BOOLEAN
IsLevelPass(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    const LEVELTYPE playerLevel = pPlayer->GetLevel();

    BOOLEAN isLevelPass = FALSE;
    if(isTempPortal)
    {
        LEVELTYPE minLevel = pArg->pMapInfo->minLV;
        LEVELTYPE maxLevel = pArg->pMapInfo->maxLV;
        isLevelPass = minLevel <= playerLevel &&
                                  playerLevel <= maxLevel;
    }
    else
    {
        LEVELTYPE minLevel = pArg->pMapPortal->byMin_Level;
        LEVELTYPE maxLevel = pArg->pMapPortal->byMax_Level;
        isLevelPass = minLevel <= playerLevel && (maxLevel &&
                                  playerLevel <= maxLevel);
    }

    if(isLevelPass == FALSE)
    {
        if(pArg->pReplyMsg)
        {
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_INVALID_LIMITLEVEL;
            pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
        }
    }
    return isLevelPass;
}

static BOOLEAN
IsAcceptHeim(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    const MONEY playerHeim = pPlayer->GetMoney();
    BOOLEAN isAcceptHeim = FALSE;
    if(isTempPortal)
    {
        return TRUE;
    }
    else
    {
        MONEY heim = pArg->pMapPortal->dwHeim;
        isAcceptHeim = !heim || heim <= playerHeim;
    }

    if(isAcceptHeim == FALSE)
    {
        if(pArg->pReplyMsg)
        {
            pArg->pReplyMsg->errorCode = RC::RC_ROOM_INSUFFICIENT_MONEY;
            pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
        }
    }
    return isAcceptHeim;
}

static BOOLEAN
ProcessItemAndHeim(BOOLEAN isTempPortal, Player* pPlayer, EnterSynArg* pArg)
{
    if(isTempPortal)
        return TRUE;

    // (WAVERIX) (NOTE) AG_ZONE_PORTAL_FIELD_MOVE_SYN�� ������ �����Դ�.
    //  ...
    // ���? ������? ���? ���ǵȰ� ����.  �켱�� ������ �ڵ带 ������, 1���� �����϶� üũ�Ѵ�.
    BOOLEAN needItemDelete = pArg->pMapPortal->wItemCode &&
                             pArg->pMapPortal->wItemNum;
    if(needItemDelete)
    {
        BOOLEAN isItemProcessed = FALSE;
        SLOTCODE itemCode = (SLOTCODE)pArg->pMapPortal->wItemCode;
        POSTYPE itemNum = (POSTYPE)pArg->pMapPortal->wItemNum;
        ItemManager* pItemManager = pPlayer->GetItemManager();
        INVEN_DELETE_TOTAL_INFO tmp_info;	//< ������ �ʴ� �ӽ� ����ü
        if(pArg->pMapPortal->bWaste_Item)
        {
            isItemProcessed =
                RC::RC_ITEM_SUCCESS == pItemManager->DeleteAllKindOfItem(1, &itemCode, &itemNum,
                                                                         &tmp_info);
        }
        else
        {
            isItemProcessed =
                itemNum == pItemManager->ExistItem(SI_INVENTORY, itemCode, itemNum);
        }

        if(isItemProcessed == FALSE)
        {
            if(pArg->pReplyMsg)
            {
                pArg->pReplyMsg->errorCode = RC::RC_ROOM_NEED_ITEM;
                pPlayer->SendPacket(pArg->pReplyMsg, sizeof(*pArg->pReplyMsg));
            }
            return isItemProcessed;
        }
    }

    if(const MONEY decreaseHeim = pArg->pMapPortal->dwHeim)
    {
        pPlayer->MinusMoney(decreaseHeim);
        // CHANGES: f110103.2L
        if (ns_object::PlayerZoneTransaction* trans_info = pPlayer->GetZoneTransactionInfo())
        {
            ns_object::PlayerZoneTransaction::Data data = { 0, };
            data.consumed_money = decreaseHeim;
            trans_info->BeginTransaction(data);
        };
        GAMELOG->LogMoney(MONEY_SPEND_WAYPOINT, pPlayer, decreaseHeim); // waypoint�� �ƴ���...
        g_pGameServer->NotifyHeimConsumption(pPlayer->GetCharGuid(), eHEIM_CONSUMPTION_WAYPOINT, decreaseHeim);
    }
    return TRUE;
}

}; // end of namespace nsQuestPortal

Handler_AG_ZONE_IMPL( AG_ZONE_QUEST_PORTAL_ENTER_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_QUEST_PORTAL_ENTER_SYN* pRecvMsg =
        static_cast<MSG_AG_ZONE_QUEST_PORTAL_ENTER_SYN*>(pMsg);

    const DWORD userKey = pRecvMsg->m_dwKey;
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(userKey);
    if(pPlayer == NULL)
        return;

    using nsQuestPortal::ENTER_SYNC_TYPE;

    const ENTER_SYNC_TYPE::eSYNC syncType = ENTER_SYNC_TYPE::eSYNC(pRecvMsg->syncType);
    const BOOLEAN isTempPortal = syncType == ENTER_SYNC_TYPE::eSYNC_TEMP_PORTAL;

    MSG_AG_ZONE_QUEST_PORTAL_ENTER_ACK msg;
    msg.errorType = msg.eRESULT_ROOM;
    msg.errorCode = RC::RC_ROOM_SUCCESS;
    msg.syncType = syncType;
    msg.portalKey = pRecvMsg->portalKey;
    msg.mapEnterancePortalIndex = 0;

    if(!isTempPortal)
    {
        if(syncType != ENTER_SYNC_TYPE::eSYNC_FIXED_PORTAL ||
           !FlowControl::FCAssert(pRecvMsg->mapEnterancePortalIndex))
        {
            msg.errorCode = RC::RC_ROOM_FAILED; // ����Ʈ | �̼� ���� ó���� ��.
            pPlayer->SendPacket(&msg, sizeof(msg));
            return;
        }
    }

    MapInfoParser* const pMapInfoParser = MapInfoParser::Instance();
    // Agent���� enterableMapCode�� �����Ѵ�.
    nsQuestPortal::EnterSynArg arg = { 0, };
    arg.pMapInfo = pMapInfoParser->FindMapInfo(pRecvMsg->enterableMapCode);
    if(!isTempPortal)
        arg.pMapPortal = pMapInfoParser->GetMapPortal(pRecvMsg->mapEnterancePortalIndex);
    arg.pRecvMsg = pRecvMsg;
    arg.pReplyMsg = &msg;

    if(nsQuestPortal::IsValidPlayerState(isTempPortal, pPlayer, &arg) == FALSE)
        return;

    if(nsQuestPortal::IsEnterableMap(isTempPortal, pPlayer, &arg) == FALSE)
        return;

    BOOLEAN isLevelPass = FALSE;

    if(nsQuestPortal::IsValidPosition(isTempPortal, pPlayer, &arg) == FALSE)
        return;

    // (WAVERIX) (090330) (LOGIC-UPDATE) �̼Ǻ��� ó���� �Ϸ���� ���� ��� ���� ���� ����
    if(nsQuestPortal::ProcessEnterRestrictCondition(isTempPortal, pPlayer, &arg) == FALSE)
        return;

    const BOOLEAN isFreePass = nsQuestPortal::IsFreePassLevel(isTempPortal, pPlayer, &arg);
    if(isFreePass == FALSE)
    {
        nsQuestPortal::QuestOrMissionResult result;
        if(nsQuestPortal::IsMissionOrQuestPass(isTempPortal, pPlayer, &arg, &result) == FALSE)
            return;
        if(nsQuestPortal::IsLevelPass(isTempPortal, pPlayer, &arg) == FALSE)
            return;
    }

    if(nsQuestPortal::IsAcceptHeim(isTempPortal, pPlayer, &arg) == FALSE)
        return;

    if(nsQuestPortal::ProcessItemAndHeim(isTempPortal, pPlayer, &arg) == FALSE)
        return;

    // Success
    pPlayer->SendPacket(&msg, sizeof(msg));
}
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

//{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__

//_NA_20110630_BATTLE_ZONE_RENEWAL
void
CheckMemberConditionToChangeOptionOperator::operator()(_PARTY_MEMBER_INFO* party_memeber_info)
{
    DWORD player_key = party_memeber_info->m_dwMemberKey;

    if (m_ExceptKey && m_ExceptKey == player_key)
    {
        return;
    }

    if (m_SuccessFlag == FALSE)
    {
        return;
    }

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(player_key);
    if (player == NULL)
    {
        return;
    }

    if (_countof(list_of_msg_->member_state_) < number_of_list_)
    {
        return;
    }

    typedef MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK MemberListMsg;
    MemberListMsg::MemberState* member_state = &(list_of_msg_->member_state_[number_of_list_++]);
    member_state->object_key_ = static_cast<WORD>(player_key);
    member_state->status_ = list_of_msg_->eStatus_PartialOk;

    const bool is_free_pass_level = nsPartyRoomMission::IsFreePassLevel(player, enter_arg_);
    if (is_free_pass_level == FALSE)
    {
        nsPartyRoomMission::QuestOrMissionResult result;
        if (nsPartyRoomMission::IsMissionOrQuestPass(player, enter_arg_, &result) == false)
        {
            if(result.quest_check_skip_ == false && result.is_quest_pass_ == false)
            {
                member_state->status_ = list_of_msg_->eStatus_RequireQuestClear;
            }
            else if(result.mission_check_skip_ == false && result.is_mission_pass_ == false)
            {
                member_state->status_ = list_of_msg_->eStatus_RequireMissionClear;
            }
        }

        if (nsPartyRoomMission::IsLevelPass(player, enter_arg_) == false)
        {
            member_state->status_ = list_of_msg_->eStatus_LevelViolation;
        }
    }
}

void
CheckMemberConditionToEnterRoomOperator::operator()(_PARTY_MEMBER_INFO* party_memeber_info)
{
    DWORD player_key = party_memeber_info->m_dwMemberKey;

    if (m_ExceptKey && m_ExceptKey == player_key)
    {
        return;
    }

    if (m_SuccessFlag == FALSE)
    {
        return;
    }

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(player_key);
    if (player == NULL)
    {
        return;
    }

    if (_countof(list_of_msg_->member_info_) < number_of_list_)
    {
        return;
    }

    typedef MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK MemberListMsg;
    MemberListMsg::MemberInfo* member_info = &(list_of_msg_->member_info_[number_of_list_++]);
    member_info->object_key_ = static_cast<WORD>(player_key);
    member_info->status_ = list_of_msg_->eStatus_PartialOk;

    if (nsPartyRoomMission::ProcessEnterRestrictCondition(player, enter_arg_) == false)
    {
        member_info->status_ = list_of_msg_->eStatus_PartialFail;
        return;
    }

    const bool is_free_pass_level = nsPartyRoomMission::IsFreePassLevel(player, enter_arg_);
    if (is_free_pass_level == FALSE)
    {
        if (nsPartyRoomMission::IsLevelPass(player, enter_arg_) == false)
        {
            member_info->status_ = list_of_msg_->eStatus_LevelViolation;
        }
    }
}

void
GameMemberEnterConditonOperator::operator()(_PARTY_MEMBER_INFO* pMemberInfo)
{
    if(m_ExceptKey && m_ExceptKey == pMemberInfo->m_dwMemberKey)
        return;

    // �̹� ������ üũ��ƾ�̹Ƿ� �� �̻� �� �ʿ䰡 ����.
    if(m_SuccessFlag == FALSE)
        return;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(pMemberInfo->m_dwMemberKey);
    if(pPlayer == NULL)
        return;

    operator()(pPlayer, pMemberInfo->m_dwMemberKey);
}

void
GameMemberEnterConditonOperator::operator()(Player* pPlayer, DWORD playerKey)
{
    const BOOLEAN isTempPortal = TRUE;

    if(!FlowControl::FCAssert(_countof(m_pListMsg->m_CheckArgMembers.members) >= m_ListNumber))
        return;

    MSG_AG_ZONE_CAN_CREATE_ROOM_ACK::MemberInfo* pMemberInfo =
        &m_pListMsg->m_CheckArgMembers.members[m_ListNumber++];

    pMemberInfo->objectKey = (WORD)playerKey;
    pMemberInfo->status = m_pListMsg->eStatus_PartialOk;

    // (WAVERIX) (090330) (LOGIC-UPDATE) �̼Ǻ��� ó���� �Ϸ���� ���� ��� ���� ���� ����
    if(nsQuestPortal::ProcessEnterRestrictCondition(isTempPortal, pPlayer, m_pEnterArg) == FALSE)
    {
        pMemberInfo->status = m_pListMsg->eStatus_PartialFail;
        return;
    }

    // ī��, GetBehave()���� ���´� �ϴ� �����ϱ�� �Ѵ�. ��� ��ʿ� ���� �߰� �� �ʿ� ����
    const BOOLEAN isFreePass = nsQuestPortal::IsFreePassLevel(isTempPortal, pPlayer, m_pEnterArg);
    if(isFreePass == FALSE)
    {
        nsQuestPortal::QuestOrMissionResult result;
        if(nsQuestPortal::IsMissionOrQuestPass(isTempPortal, pPlayer, m_pEnterArg, &result) == FALSE)
        {
            if(result.bQuestCheckSkip == FALSE && result.isQuestPass == FALSE)
                pMemberInfo->status = m_pListMsg->eStatus_RequireQuestClear;
            else if(result.bMissionCheckSkip == FALSE && result.isMissionPass == FALSE)
                pMemberInfo->status = m_pListMsg->eStatus_RequireMissionClear;
        }
        if(nsQuestPortal::IsLevelPass(isTempPortal, pPlayer, m_pEnterArg) == FALSE)
            pMemberInfo->status = m_pListMsg->eStatus_LevelViolation;
    }
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
Handler_AG_ZONE_IMPL(AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_SYN* recv_msg =
        static_cast<MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_SYN*>(pMsg);

    PlayerManager* player_manager = PlayerManager::Instance();
    MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK msg;
    msg.Clear();
    msg.result_value_ = msg.eResult_NoMemberFail;
    msg.party_key_ = recv_msg->party_key_;

    const DWORD user_key = recv_msg->m_dwKey;
    Player* request_player = player_manager->FindPlayerByUserKey(user_key);
    if (request_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : %d", user_key);
        return;
    }

    MapInfoParser* const map_info_parser = MapInfoParser::Instance();
    nsPartyRoomMission::EnterSynArg arg = { 0, };
    arg.map_info_ = map_info_parser->FindMapInfo(recv_msg->map_code_);
    if (arg.map_info_ == NULL)
    {
        msg.result_value_ = msg.eResult_InvalidMap;
        request_player->SendPacket(&msg, msg.GetSize());
        return;
    }

    msg.map_code_ = recv_msg->map_code_;
    if (nsPartyRoomMission::IsEnterableMap(&arg) == false)
    {
        msg.result_value_ = msg.eResult_InvalidMap;
        request_player->SendPacket(&msg, msg.GetSize());
        return;
    }

    const MAPCODE map_code = msg.map_code_;
    const SLOTCODE item_code = recv_msg->require_item_;
    const PartyZoneType party_room_type = recv_msg->party_room_type_;
    msg.is_cash_item_ = false;
    if (item_code != 0)
    {
        BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
        if (item_info == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found mission item : %d", item_code);
            return;
        }
        msg.is_cash_item_ = item_info->IsChargeItem();
    }

    RC::ePARTY_RESULT result_code;
    NewGamePartyManager* const party_manager = NewGamePartyManager::Instance();
    result_code = party_manager->CheckRoomTerms(request_player,
                                                item_code, 
                                                1, 
                                                party_room_type, 
                                                recv_msg->map_code_);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        msg.result_value_ = msg.eResult_FailCheckTerms;
        request_player->SendPacket(&msg, msg.GetSize());
        return;
    }

    static CheckMemberConditionToEnterRoomOperator oper;
    oper.Init(eGAMEPARTY_OPERATOR_ENTER_ZONE_CONDITION_PARTYMEMBER_INFO, 0, &msg, &arg);
    msg.result_value_ = msg.eResult_CheckedMembers;
    msg.number_of_partymember_ = party_manager->GetMemberConditionForEnterRoom(oper, recv_msg->party_key_);
    msg.require_item_ = recv_msg->require_item_;
    if (msg.number_of_partymember_ == 0 || msg.number_of_partymember_ > _countof(msg.member_info_))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : An exception data of member count : %d", 
               msg.number_of_partymember_);
        return;
    }
    request_player->SendPacket(&msg, msg.GetSize());
}

Handler_AG_ZONE_IMPL(AG_ZONE_CAN_CREATE_ROOM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_CAN_CREATE_ROOM_SYN* pRecvMsg =
        static_cast<MSG_AG_ZONE_CAN_CREATE_ROOM_SYN*>(pMsg);

    PlayerManager* pPlayerManager = PlayerManager::Instance();
    MSG_AG_ZONE_CAN_CREATE_ROOM_ACK msg;
    msg.Clear();
    msg.m_CheckMethod = msg.eCheck_None;
    msg.m_Result = msg.eResult_NoMemberFail;
    msg.m_CheckArgMembers.partyKey = pRecvMsg->m_CheckArgMembers.partyKey;
    msg.m_CheckArgMembers.roomKey = pRecvMsg->m_CheckArgMembers.roomKey;

    const DWORD userKey = pRecvMsg->m_dwKey;
    Player* pRequester = pPlayerManager->FindPlayerByUserKey(userKey);
    if(pRequester == NULL) // pPlayer is MasterUser
        return; // ��... in transaction�� ��Ȳ���� ��¿ �� ���� ���ΰ�?

    MapInfoParser* const pMapInfoParser = MapInfoParser::Instance();
    // Agent���� enterableMapCode�� �����Ѵ�.
    nsQuestPortal::EnterSynArg arg = { 0, };
    arg.pMapInfo = pMapInfoParser->FindMapInfo(pRecvMsg->m_MapCode);
    if(!FlowControl::FCAssert(arg.pMapInfo != NULL))
    {
        msg.m_Result = msg.eResult_InvalidMap;
        pRequester->SendPacket(&msg, msg.GetSize());
        return;
    }
    msg.m_MapCode = pRecvMsg->m_MapCode;

    //IsEnterableMap()�� arg�� sMAPINFO* pMapInfo�� ��� �Ѵ�.
    if(nsQuestPortal::IsEnterableMap(FALSE, pRequester, &arg) == FALSE)
    {
        msg.m_Result = msg.eResult_InvalidMap;
        pRequester->SendPacket(&msg, msg.GetSize());
        return;
    }

    static GameMemberEnterConditonOperator oper;
    oper.Init(eGAMEPARTY_OPERATOR_ENTER_ZONE_CONDITION_MEMBER_INFO, 0, &msg, &arg);

    msg.m_CheckMethod = pRecvMsg->m_CheckMethod ;
    msg.m_Result = msg.eResult_CheckedMembers;

    if(msg.m_CheckMethod == msg.eCheck_Party)
    {
        //To party memeber check
        NewGamePartyManager::Instance()->GetEnterRoomConditionInfoCollect(oper, pRequester);
    }
    else if(msg.m_CheckMethod == msg.eCheck_Members)
    {
        //To Zone memeber check (ex : lobby)
        const MSG_AG_ZONE_CAN_CREATE_ROOM_SYN::CheckArgMembers& rMembers = pRecvMsg->m_CheckArgMembers;

        for(int i = 0; i < _countof(rMembers.members); ++i)
        {
            const DWORD playerKey = rMembers.members[i].objectKey;
            if(playerKey == 0)
                break;

            Player* pPlayer = pPlayerManager->FindPlayerByObjKey(playerKey);
            if(pPlayer != 0)
                oper(pPlayer, playerKey);
        }
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Incorrect check method type : %d", msg.m_CheckMethod);
        return;
    }

    pRequester->SendPacket(&msg, msg.GetSize());
}


Handler_AG_ZONE_IMPL(AG_ZONE_LOBBY_CREATE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_LOBBY_CREATE_SYN* pRecvMsg =
        static_cast<MSG_AG_ZONE_LOBBY_CREATE_SYN*>(pMsg);

    PlayerManager* pPlayerManager = PlayerManager::Instance();
    const DWORD userKey = pRecvMsg->m_dwKey;
    Player* pRequester = pPlayerManager->FindPlayerByUserKey(userKey);
    if(pRequester == NULL) // pPlayer is MasterUser
        return; // ��... in transaction�� ��Ȳ���� ��¿ �� ���� ���ΰ�?

    MSG_AG_ZONE_LOBBY_CREATE_ACK msg;
    static_cast<MSG_AG_ZONE_LOBBY_CREATE_SYN&>(msg) = *pRecvMsg;
    new (&msg) MSG_AG_ZONE_LOBBY_CREATE_ACK; // NOTE: changes protocol
    msg.m_RoomInfo = pRecvMsg->m_RoomInfo;

    msg.m_1stCheckResultInGameServer = RC::RC_ROOM_SUCCESS;

    MapInfoParser* const pMapInfoParser = MapInfoParser::Instance();
    //// Agent���� enterableMapCode�� �����Ѵ�.
    nsQuestPortal::EnterSynArg arg = { 0, };
    arg.pMapInfo = pMapInfoParser->FindMapInfo(pRecvMsg->m_MapCode);
    if(!FlowControl::FCAssert(arg.pMapInfo != NULL))
    {
        msg.m_1stCheckResultInGameServer = RC::RC_ROOM_INVALID_MAPCODE;
        pRequester->SendPacket(&msg, msg.GetSize());
        return;
    }

    if(nsQuestPortal::IsEnterableMap(TRUE, pRequester, &arg) == FALSE)
    {
        msg.m_1stCheckResultInGameServer = RC::RC_ROOM_INVALID_MAPCODE;
        pRequester->SendPacket(&msg, msg.GetSize());
        return;
    }

    // (WAVERIX) (090330) (LOGIC-UPDATE) �̼Ǻ��� ó���� �Ϸ���� ���� ��� ���� ���� ����
    if(nsQuestPortal::ProcessEnterRestrictCondition(TRUE, pRequester, &arg) == FALSE)
    {
        msg.m_1stCheckResultInGameServer = RC::RC_ROOM_FAILED;
        pRequester->SendPacket(&msg, msg.GetSize());
        return;
    }

    const BOOLEAN isFreePass = nsQuestPortal::IsFreePassLevel(TRUE, pRequester, &arg);
    if(isFreePass == FALSE)
    {
        nsQuestPortal::QuestOrMissionResult result;
        if(nsQuestPortal::IsMissionOrQuestPass(TRUE, pRequester, &arg, &result) == FALSE)
        {
            if(result.bQuestCheckSkip == FALSE && result.isQuestPass == FALSE)
                msg.m_1stCheckResultInGameServer = RC::RC_ROOM_PREVIOUS_QUEST_CLEAR;
            else if(result.bMissionCheckSkip == FALSE && result.isMissionPass == FALSE)
                msg.m_1stCheckResultInGameServer = RC::RC_ROOM_NEED2_CLEAR_PRE_MISSION;
        }
        if(nsQuestPortal::IsLevelPass(TRUE, pRequester, &arg) == FALSE)
            msg.m_1stCheckResultInGameServer = RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
    }

    pRequester->SendPacket(&msg, msg.GetSize());
}

//_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
// (ADD)(20100318)(hukim)(REQ_0038) �̼ǹ� ���� ���� ���θ� �˻��ϴ� ������ �ߺ� �Ǵ°���
// ���� ���� �Լ��� �и�

RC::eROOM_RESULT IsEnterableMissionRoom(const MAPCODE map_code, Player* const player)
{
    MapInfoParser* const map_info_parser = MapInfoParser::Instance();
    nsQuestPortal::EnterSynArg enter_sync_argument = { 0, };
    enter_sync_argument.pMapInfo = map_info_parser->FindMapInfo(map_code);    
    
    if (FlowControl::FCAssert(enter_sync_argument.pMapInfo != NULL) == FALSE)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if (nsQuestPortal::IsEnterableMap(TRUE, player, &enter_sync_argument) == FALSE)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if (nsQuestPortal::ProcessEnterRestrictCondition(TRUE, player, &enter_sync_argument) == FALSE)
    {
        return RC::RC_ROOM_FAILED;
    }

    if (nsQuestPortal::IsFreePassLevel(TRUE, player, &enter_sync_argument) == FALSE)
    {
        nsQuestPortal::QuestOrMissionResult result;
        if (nsQuestPortal::IsMissionOrQuestPass(TRUE, player, &enter_sync_argument, &result) == FALSE)
        {
            if (result.bQuestCheckSkip == FALSE && result.isQuestPass == FALSE)
            {
                return RC::RC_ROOM_PREVIOUS_QUEST_CLEAR;
            }
            else if (result.bMissionCheckSkip == FALSE && result.isMissionPass == FALSE)
            {
                return RC::RC_ROOM_NEED2_CLEAR_PRE_MISSION;
            }
        }
        if (nsQuestPortal::IsLevelPass(TRUE, player, &enter_sync_argument) == FALSE)
        {
            return RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
        }
    }
    return RC::RC_ROOM_SUCCESS;
}

Handler_AG_ZONE_IMPL(AG_ZONE_LOBBY_JOIN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_AG_ZONE_LOBBY_JOIN_SYN* const receive_msg = 
        static_cast<MSG_AG_ZONE_LOBBY_JOIN_SYN*>(pMsg);

    const PlayerManager* const player_manager = PlayerManager::Instance();
    const DWORD user_key = receive_msg->m_dwKey;
    Player* const player = player_manager->FindPlayerByUserKey(user_key);
    if (player == NULL) // pPlayer is MasterUser
    {
        return; // ��... in transaction�� ��Ȳ���� ��¿ �� ���� ���ΰ�?
    }

    MSG_AG_ZONE_LOBBY_JOIN_ACK msg;
    static_cast<MSG_AG_ZONE_LOBBY_JOIN_SYN&>(msg) = *receive_msg;
    new (&msg) MSG_AG_ZONE_LOBBY_JOIN_ACK; // NOTE: changes protocol

    msg.m_1stCheckResultInGameServer = IsEnterableMissionRoom(receive_msg->m_MapCode, player);
    player->SendPacket(&msg, msg.GetSize());
}

//}__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__

//_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
Handler_AG_ZONE_IMPL(AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN* const receive_msg = 
        static_cast<MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN*>(pMsg);

    const DWORD user_key = receive_msg->m_dwKey;
    const PlayerManager* const player_manager = PlayerManager::Instance();
    Player* const player = player_manager->FindPlayerByUserKey(user_key);
    if (player == NULL)
    {
        return;
    }

    MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK msg;
    msg.current_room_count_ = receive_msg->current_room_count_;
    
    // (NOTE)(20100322)(hukim)(REQ_0038) dest�� �����÷ο츦 �˻��Ѵ�.
    BOOST_STATIC_ASSERT(_countof(msg.room_info_list_) >= _countof(receive_msg->room_info_list_));
    
    for (int i = 0; i < receive_msg->current_room_count_; ++i)
    {
        MISSION_ROOMINFO& room_info = receive_msg->room_info_list_[i];
        if (room_info.m_RoomInfo.can_join_room_ == true)
        {
            if (IsEnterableMissionRoom(room_info.m_MapCode, player) != RC::RC_ROOM_SUCCESS)
            {
                room_info.m_RoomInfo.can_join_room_ = false;
            }
        }
        // (NOTE)(20100318)(hukim)(REQ_0038) ����� �˻��� �� MapCode�� �ʿ��ѵ� ������
        // ��ü�� �ְ� �޴� ����� ���ſ� ���� �� �ִ�. �׷��� ���� �ణ ����Ǿ�
        // ��� �濡 ���� ������ �� �ʿ��� ���ٸ� �׶����� �������� ������ �ʿ��ϴ�.
        // ���� �濡 ���� ���� ��ü�� �ѱ�� ����� ������ �� ũ�ٰ� �Ǵ��ߴ�.
        // �� ������ �ִ� 40�� ���Ͽ��� �� �پ��� �ǹǷ� ���ɻ� ������ ũ�� �ʴٰ�
        // �Ǵ��ߴ�.

        msg.room_info_list_[i] = room_info;
    }
    
    player->SendPacket(&msg, msg.GetSize());
}

Handler_AG_ZONE_IMPL(AG_ZONE_CHANGE_MONSTER_ABILITY_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_AG_ZONE_CHANGE_MONSTER_ABILITY_CMD* pRecvMsg =
        static_cast<MSG_AG_ZONE_CHANGE_MONSTER_ABILITY_CMD*>(pMsg);

    GameZone* pGameZone = g_GameZoneManager.GetZone(pRecvMsg->roomKey);
    if(pGameZone == 0)
        return;

    const DWORD AcceptMask = (1 << eZONETYPE_MISSION)
                           | (1 << eZONETYPE_HUNTING)
                           | (1 << eZONETYPE_CHUNTING)
                           | (1 << eZONETYPE_INSTANCE);

    DWORD zone_type_flag = pGameZone->GetZoneType();
          zone_type_flag = 1 << zone_type_flag;
    if((AcceptMask & zone_type_flag) == 0)
        return;

    // (WAVERIX) (090622) (NOTE) �̰��� �������� Room�� �ο����� ���� ���� �ɷ�ġ�� ���ϴ�
    // ó���� �����ϱ� ����.

    GameRoom* pGameRoom = static_cast<GameRoom*>(pGameZone);
    pGameRoom->PrepareNumberOfExpectedPlayers(pRecvMsg->number_of_players);
    g_ChangeMonsterAbility.UpdateAllMonster(pGameRoom);
}
