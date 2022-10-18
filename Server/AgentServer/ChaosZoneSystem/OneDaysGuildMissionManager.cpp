#include "stdafx.h"

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include "OneDaysGuildMissionManager.h"
#include "OneDaysGuildMissionRoom.h"

#include "ChaosZoneInfoParser.h"

#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>
#include <Zone/InstanceDungeon.h>
#include <Zone/ZoneManager.h>
#include "ServerOptionParserEx.h"
#include "AgentServer.h"
#include "MapInfoParser.h"

void OneDaysGuildMissionManager::Init()
{
	OneDaysGuildMission_mapcode_ = ChaosZoneInfoParser::Instance()->GetOneDaysGuildmissionMapCode();

#if defined(_KOREA)
	// Solarauth�� ��ũ��Ʈ�� �ε��� ���� Init()�� �� �ڵ尡 0�� �� �ִ�.(��ũ��Ʈ �ε尡 ������ �ö󰣵� �̷������.)
	if (0 == OneDaysGuildMission_mapcode_) {
		SUNLOG(eCRITICAL_LOG, "OneDaysGuildMission Mapcode Error! Check 'ChaosZoneInfo.txt'");
	}
#endif
	room_map_.clear();
}

void OneDaysGuildMissionManager::DestroyRoom(KEYTYPE zone_key)
{
	ONEDAYSGUILDMISSION_ROOM_MAP::iterator itr = room_map_.find(zone_key);
	if(itr != room_map_.end())
	{
		OneDaysGuildMissionRoom* room = itr->second;
		if(room)
		{
			room_map_.erase(itr);
			SAFE_DELETE(room);
		}
	}
}

void OneDaysGuildMissionManager::DestroyRoom( GUILDGUID guild_guid )
{
	KEYTYPE zone_key = GetZoneKeyUsingGuildGuid(guild_guid);
	if(zone_key != 0)
	{
		DestroyRoom(zone_key);
	}
}

void OneDaysGuildMissionManager::Update()
{
	ONEDAYSGUILDMISSION_ROOM_MAP::iterator itr = room_map_.begin();
	for (itr; itr != room_map_.end(); ++itr)
	{
		OneDaysGuildMissionRoom* room = itr->second;
		if(room)
		{
			ZoneInterface* dungeon = g_ZoneManager.FindZone(room->GetZoneKey());
			if (NULL == dungeon) {
				DestroyRoom(room->GetZoneKey());

				return;
			}
		}
	}
}

bool OneDaysGuildMissionManager::AllPlayerLeaveRoom(const ZONEKEY zone_key)
{
	if (zone_key == 0) {
		//zone_key�� 0�� ��� ������ �Լ��� ���Ⱑ �ƴϴ�
		return false;
	}

	ONEDAYSGUILDMISSION_ROOM_MAP::iterator itr = room_map_.find(zone_key);
	if (itr != room_map_.end())
	{
		OneDaysGuildMissionRoom* room = itr->second;
		if(room)
		{
			room->KickAllUsers();
		}
	}

	return true;
}

bool OneDaysGuildMissionManager::LeaveRoom(const ZONEKEY zone_key, User* const user)
{
	if (zone_key == 0) {
		//zone_key�� 0�� ��� ������ �Լ��� ���Ⱑ �ƴϴ�
		return false;
	}

	ONEDAYSGUILDMISSION_ROOM_MAP::iterator itr = room_map_.find(zone_key);
	if (itr != room_map_.end())
	{
		OneDaysGuildMissionRoom* room = itr->second;
		if(room)
		{
			room->_KickUser(user);
			
			INT CurrentPlayerCnt = 0;
			room->GetCurrentPlayerCnt(CurrentPlayerCnt);

			if(CurrentPlayerCnt <= 0)
			{
				DestroyRoom(zone_key);
			}
		}
	}

	return true;
}

MAPCODE OneDaysGuildMissionManager::GetOneDaysGuildMissionMapCode()
{
#if defined(_KOREA)
#else
	// Solarauth�� ��ũ��Ʈ�� �ε��� ��� �߻��� �� �ִ�(Init() �������� �ε带 �������� ���⼭ �ٽ� �ε�). �ش� ���ڵ带 �ٽ� �д� ��ƾ�� �߰�
	if(0 == OneDaysGuildMission_mapcode_)
	{
		OneDaysGuildMission_mapcode_ = ChaosZoneInfoParser::Instance()->GetOneDaysGuildmissionMapCode();
	}
#endif

	return OneDaysGuildMission_mapcode_;
}

RC::eONEDAYSGUILDMISSION_RESULT OneDaysGuildMissionManager::CreateOneDaysGuildMissionRoom(User* user, BOOL is_gm_cmd /*= FALSE*/)
{
	// ������ ���� ������ �������� �˻�
	RC::eONEDAYSGUILDMISSION_RESULT check_result = UserCanCreateARoom(user, is_gm_cmd);
	if(check_result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		return check_result;
	}

	// ���ڵ�
	const MAPCODE map_code = GetOneDaysGuildMissionMapCode();

	user->BeginTransaction(TR_CREATE_INSTANCE_DUNGEON);

	// Instance Dungeon ����
	InstanceDungeon* dungeon = NULL;
	const RC::eROOM_RESULT result = 
		g_ZoneManager.CreateInstanceDungeon(user->GetZoneKey(), user, map_code, (InstanceDungeon*&)dungeon, 
		INDUN_KIND_CHAOS_ZONE, FALSE);
	if(dungeon == NULL || result != RC::RC_ROOM_SUCCESS)
	{
		//instance_dungeon��������
		user->EndTransaction();
		return RC::RC_ONEDAYSGUILDMISSION_ROOM_CREATE_FAILED;
	}

	dungeon->SetRoomState(eROOMSTATE_GENERNAL);

	const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
	if (map_info == NULL)
	{
		user->EndTransaction();
		return RC::RC_ONEDAYSGUILDMISSION_FAILED;
	}

	// OneDaysGuildMissionRoom ����
	KEYTYPE room_key = dungeon->GetKey();
	OneDaysGuildMissionRoom* new_room = new OneDaysGuildMissionRoom(room_key, user->GetSelectedCharGuildGuid(), map_info->byMaxUserNum);
	if(!new_room)
	{
		user->EndTransaction();
		return RC::RC_ONEDAYSGUILDMISSION_FAILED;
	}

	//Map�� Room ���
	room_map_[room_key] = new_room;

	user->EndTransaction();

	return RC::RC_ONEDAYSGUILDMISSION_SUCCESS;
}

RC::eONEDAYSGUILDMISSION_RESULT OneDaysGuildMissionManager::UserCanCreateARoom( User* user, BOOL is_gm_cmd /*= FALSE*/ )
{
	//// GM��ɿ� ���� ������ �ƴ� ��� �߰� �˻�
	//if(is_gm_cmd == FALSE)
	//{
	//	// ������ ����: 1. �ο��� �� á�°�, 2. �� ã�� ��� ������ ����Ʈ �� ���� �ִ°�?
	//	
	//	// �ش� ���ǿ� �´������� �ƴϸ� ���� ��Ű�� Ŭ���̾�Ʈ���� ����� ���� ������ �˸���!!
	//}

	// ���� �����Ϸ��� ������ ��忡 ���Ե� �������� üũ
	if(user->GetSelectedCharGuildGuid() == 0)
	{
		return RC::RC_ONEDAYSGUILDMISSION_NOT_GUILD_USER;
	}

	RC::eONEDAYSGUILDMISSION_RESULT result = _CheckUserProcessOneDaysGuildMission(user);
	if(result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		return result;
	}

	// �ش� ����� �� ���� ���� Ȯ��
	OneDaysGuildMissionRoom* room = FindRoomUsingGuildGuid(user->GetSelectedCharGuildGuid());
	if(room != NULL)
	{
		//SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] OneDaysGuildMissionRoom is ALREADY Exist! [guild:%d]", user->GetSelectedCharGuildGuid());
		return RC::RC_ONEDAYSGUILDMISSION_ALEADY_CREATED;
	}

	return RC::RC_ONEDAYSGUILDMISSION_SUCCESS;
}

RC::eONEDAYSGUILDMISSION_RESULT OneDaysGuildMissionManager::CanAUserEntersTheRoom( User* user, BOOL is_gm_cmd /*= FALSE*/ )
{
	// �� ���� ���� Ȯ��
	OneDaysGuildMissionRoom* room = FindRoomUsingGuildGuid(user->GetSelectedCharGuildGuid());
	if(room == NULL)
	{
		//SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] OneDaysGuildMissionRoom is NULL! [guild:%d]", user->GetSelectedCharGuildGuid());
		return RC::RC_ONEDAYSGUILDMISSION_ROOM_NOT_EXIST;
	}

	// �̹� ������ �÷��̾��ΰ�?
	if(room->FindUser(user->GetPlayerKey()) != NULL)
	{
		return RC::RC_ONEDAYSGUILDMISSION_ALREADY_ENTERED;
	}

	// �÷��̾� ���ڰ� ���� �ο����� �Ѵ´ٸ� �� ������ ����...
	if(room->IsMaxStandByMemberCnt())
	{
		// �����ο��� ���� ���¿��� ���� ���ٹ��� ���� ��ϻ� ������ ���� ���
		if(!room->IsStandByMember(user->GetSelectedCharGuid()))
		{
			return RC::RC_ONEDAYSGUILDMISSION_OVER_MAX_LIMIT_ENTERED_CNT;
		}
	}

	// GM��ɿ� ���� ������ �ƴ� ��� �߰� �˻�
	if(is_gm_cmd == FALSE)
	{
		// ������ ����: 1. �ο��� �� á�°�, 2. �� ã�� ��� ������ ����Ʈ �� ���� �ִ°�?

		// �ش� ���ǿ� �´������� �ƴϸ� ���� ��Ű�� Ŭ���̾�Ʈ���� ����� ���� ������ �˸���!!
	}

	RC::eONEDAYSGUILDMISSION_RESULT result = _CheckUserProcessOneDaysGuildMission(user);
	if(result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		return result;
	}

	return RC::RC_ONEDAYSGUILDMISSION_SUCCESS;
}


RC::eONEDAYSGUILDMISSION_RESULT OneDaysGuildMissionManager::_CheckUserProcessOneDaysGuildMission( User* user )
{
	// ������ ä�ο����� ����
	AgentServer* pAgentServer = AgentServer::GetInstance();
	SERVER_KEY rKey(pAgentServer->GetKey());

	SERVER_COMMON_OPTION& server_option = ServerOptionParserEx::Instance()->GetServerOption();
	if( rKey.GetChannelID() != server_option.m_byDominationChannel )
	{
		return RC::RC_ONEDAYSGUILDMISSION_NOT_DOMINATION_CHANNEL;
	}

	// �ٸ��ൿ���̸� (�ŷ�/����) ����
	if(user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
	{
		return RC::RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION;
	}

	// ���� ������� Ȯ��
	BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
	if(battle_ground_manager)
	{
		if (battle_ground_manager->IsValidReservationMember(user->GetUserKey()))
		{
			return RC::RC_ONEDAYSGUILDMISSION_BATTLEGROUND_RESERVATION_STATE;
		}
	}

	// Ʈ����ǻ���
	if(user->IsBeginTransaction())
	{
		return RC::RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION;
	}

	return RC::RC_ONEDAYSGUILDMISSION_SUCCESS;
}

OneDaysGuildMissionRoom* OneDaysGuildMissionManager::FindRoomUsingGuildGuid( GUILDGUID guild_guid )
{
	ONEDAYSGUILDMISSION_ROOM_MAP::iterator itr = room_map_.begin();
	for (itr; itr != room_map_.end(); ++itr)
	{
		OneDaysGuildMissionRoom* target = itr->second;
		if(target == NULL)
		{
			continue;
		}

		if(guild_guid == target->GetGuildGuid())
		{
			return target;
		}
	}
	return NULL;
}

OneDaysGuildMissionRoom* OneDaysGuildMissionManager::FindRoomUsingRoomKey( KEYTYPE room_key )
{
	ONEDAYSGUILDMISSION_ROOM_MAP::iterator itr = room_map_.find(room_key);

	if(itr != room_map_.end())
	{
		return itr->second;
	}

	return NULL;
}

KEYTYPE OneDaysGuildMissionManager::GetZoneKeyUsingGuildGuid( GUILDGUID guild_guid )
{
	OneDaysGuildMissionRoom* room = FindRoomUsingGuildGuid(guild_guid);
	if(room == NULL)
	{
		return 0;
	}

	return room->GetZoneKey();
}

BOOL OneDaysGuildMissionManager::GetEnterPlayerList_CharGuid( GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT)
{
	OneDaysGuildMissionRoom* room = FindRoomUsingGuildGuid(guild_guid);

	if(room == NULL)
	{
		return FALSE;
	}

	*player_count = room->GetPlayerList_CharGuid(list);

	return TRUE;
}

BOOL OneDaysGuildMissionManager::GetStandByUserList_CharGuid(GUILDGUID guild_guid, CHARGUID* list OUT, INT* player_count OUT)
{
	OneDaysGuildMissionRoom* room = FindRoomUsingGuildGuid(guild_guid);

	if(room == NULL)
	{
		return FALSE;
	}

	*player_count = room->Getstand_by_user_list_CharGuid(list);

	return TRUE;
}

BOOL OneDaysGuildMissionManager::MoveToOneDaysGuildMissionRoom(User* user, BOOL is_gm_cmd /*= FALSE*/)
{
	GUILDGUID user_guild_guid = user->GetSelectedCharGuildGuid();
	if(user_guild_guid == 0)
	{
		return FALSE;
	}

	RC::eONEDAYSGUILDMISSION_RESULT enter_result = CanAUserEntersTheRoom(user, is_gm_cmd);
	if(enter_result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][GM]OneDaysGuildMission Enter Failed!");
		return FALSE;
	}

	KEYTYPE zone_key = GetZoneKeyUsingGuildGuid(user_guild_guid);

	// �̵��Ϸ��� ���� instance_dungeon
	InstanceDungeon* const target_dungeon = \
		static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(zone_key));
	if(target_dungeon == NULL)
	{
		return FALSE;
	}

	//���� ��ġ�� Zone
	ZoneInterface* current_zone = g_ZoneManager.FindZone(user->GetZoneKey());
	if(current_zone == NULL)
	{
		SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : OneDaysGuildMissionRoom Not found zone : %d", user->GetUserGUID());
		return FALSE;
	}

	user->BeginTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);
	user->SetTRRoomKey(zone_key);

	CheckMoveZonePolicy* const check_move_zone_policy = 
		MoveZonePolicy<MZTF_STATIC_TO_ROOM_FOR_UCUSER>::AlignInfo(user,
		user->GetZoneType(), target_dungeon->GetType(),
		user->GetZoneKey(),  target_dungeon->GetKey());

	RoomArgument argument(eZONEPUBLIC_PUBLIC, NULL);

	RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(check_move_zone_policy, RoomArgument::GetEmpty());
	if (rt != RC::RC_ROOM_SUCCESS) {
		user->EndTransaction();
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] OneDaysGuildMission Zone Move Failed!");
		return FALSE;
	}

	return TRUE;
}

void OneDaysGuildMissionManager::ProcessPacket_CG( User* user, MSG_BASE* packet, WORD size )
{
	handler_cg_onedaysguildmission_.OnCG_ProcessPacket(user, packet, size);
}

void OneDaysGuildMissionManager::ProcessPacket_AG( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
	handler_ag_onedaysguildmission_.OnAG_ProcessPacket(server_session, packet, size);
}   

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)