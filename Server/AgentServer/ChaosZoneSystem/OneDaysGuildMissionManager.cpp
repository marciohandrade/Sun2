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
	// Solarauth로 스크립트를 로드할 경우는 Init()시 맵 코드가 0일 수 있다.(스크립트 로드가 서버가 올라간뒤 이루워진다.)
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
		//zone_key가 0일 경우 들어오는 함수는 여기가 아니다
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
		//zone_key가 0일 경우 들어오는 함수는 여기가 아니다
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
	// Solarauth로 스크립트를 로드할 경우 발생할 수 있다(Init() 시점에서 로드를 못함으로 여기서 다시 로드). 해당 맵코드를 다시 읽는 루틴을 추가
	if(0 == OneDaysGuildMission_mapcode_)
	{
		OneDaysGuildMission_mapcode_ = ChaosZoneInfoParser::Instance()->GetOneDaysGuildmissionMapCode();
	}
#endif

	return OneDaysGuildMission_mapcode_;
}

RC::eONEDAYSGUILDMISSION_RESULT OneDaysGuildMissionManager::CreateOneDaysGuildMissionRoom(User* user, BOOL is_gm_cmd /*= FALSE*/)
{
	// 유저가 생성 가능한 상태인지 검사
	RC::eONEDAYSGUILDMISSION_RESULT check_result = UserCanCreateARoom(user, is_gm_cmd);
	if(check_result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		return check_result;
	}

	// 맵코드
	const MAPCODE map_code = GetOneDaysGuildMissionMapCode();

	user->BeginTransaction(TR_CREATE_INSTANCE_DUNGEON);

	// Instance Dungeon 생성
	InstanceDungeon* dungeon = NULL;
	const RC::eROOM_RESULT result = 
		g_ZoneManager.CreateInstanceDungeon(user->GetZoneKey(), user, map_code, (InstanceDungeon*&)dungeon, 
		INDUN_KIND_CHAOS_ZONE, FALSE);
	if(dungeon == NULL || result != RC::RC_ROOM_SUCCESS)
	{
		//instance_dungeon생성실패
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

	// OneDaysGuildMissionRoom 생성
	KEYTYPE room_key = dungeon->GetKey();
	OneDaysGuildMissionRoom* new_room = new OneDaysGuildMissionRoom(room_key, user->GetSelectedCharGuildGuid(), map_info->byMaxUserNum);
	if(!new_room)
	{
		user->EndTransaction();
		return RC::RC_ONEDAYSGUILDMISSION_FAILED;
	}

	//Map에 Room 등록
	room_map_[room_key] = new_room;

	user->EndTransaction();

	return RC::RC_ONEDAYSGUILDMISSION_SUCCESS;
}

RC::eONEDAYSGUILDMISSION_RESULT OneDaysGuildMissionManager::UserCanCreateARoom( User* user, BOOL is_gm_cmd /*= FALSE*/ )
{
	//// GM명령에 의한 생성이 아닐 경우 추가 검사
	//if(is_gm_cmd == FALSE)
	//{
	//	// 방입장 조건: 1. 인원이 다 찼는가, 2. 다 찾을 경우 재접속 리스트 상에 내가 있는가?
	//	
	//	// 해당 조건에 맞는유저가 아니면 리턴 시키고 클라이언트에게 못들어 가는 이유를 알리자!!
	//}

	// 방을 생성하려는 유저가 길드에 가입된 상태인지 체크
	if(user->GetSelectedCharGuildGuid() == 0)
	{
		return RC::RC_ONEDAYSGUILDMISSION_NOT_GUILD_USER;
	}

	RC::eONEDAYSGUILDMISSION_RESULT result = _CheckUserProcessOneDaysGuildMission(user);
	if(result != RC::RC_ONEDAYSGUILDMISSION_SUCCESS)
	{
		return result;
	}

	// 해당 길드의 방 생성 여부 확인
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
	// 룸 존재 유무 확인
	OneDaysGuildMissionRoom* room = FindRoomUsingGuildGuid(user->GetSelectedCharGuildGuid());
	if(room == NULL)
	{
		//SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] OneDaysGuildMissionRoom is NULL! [guild:%d]", user->GetSelectedCharGuildGuid());
		return RC::RC_ONEDAYSGUILDMISSION_ROOM_NOT_EXIST;
	}

	// 이미 입장한 플레이어인가?
	if(room->FindUser(user->GetPlayerKey()) != NULL)
	{
		return RC::RC_ONEDAYSGUILDMISSION_ALREADY_ENTERED;
	}

	// 플레이어 숫자가 제한 인원보다 넘는다면 룸 생성을 막자...
	if(room->IsMaxStandByMemberCnt())
	{
		// 제한인원이 넘은 상태에서 만약 스텐바이 유저 목록상에 있으면 입장 허용
		if(!room->IsStandByMember(user->GetSelectedCharGuid()))
		{
			return RC::RC_ONEDAYSGUILDMISSION_OVER_MAX_LIMIT_ENTERED_CNT;
		}
	}

	// GM명령에 의한 생성이 아닐 경우 추가 검사
	if(is_gm_cmd == FALSE)
	{
		// 방입장 조건: 1. 인원이 다 찼는가, 2. 다 찾을 경우 재접속 리스트 상에 내가 있는가?

		// 해당 조건에 맞는유저가 아니면 리턴 시키고 클라이언트에게 못들어 가는 이유를 알리자!!
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
	// 점령전 채널에서만 가능
	AgentServer* pAgentServer = AgentServer::GetInstance();
	SERVER_KEY rKey(pAgentServer->GetKey());

	SERVER_COMMON_OPTION& server_option = ServerOptionParserEx::Instance()->GetServerOption();
	if( rKey.GetChannelID() != server_option.m_byDominationChannel )
	{
		return RC::RC_ONEDAYSGUILDMISSION_NOT_DOMINATION_CHANNEL;
	}

	// 다른행동중이면 (거래/상점) 실패
	if(user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
	{
		return RC::RC_ONEDAYSGUILDMISSION_FAIL_TRANSACTION;
	}

	// 전장 예약상태 확인
	BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
	if(battle_ground_manager)
	{
		if (battle_ground_manager->IsValidReservationMember(user->GetUserKey()))
		{
			return RC::RC_ONEDAYSGUILDMISSION_BATTLEGROUND_RESERVATION_STATE;
		}
	}

	// 트랜잭션상태
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

	// 이동하려는 곳의 instance_dungeon
	InstanceDungeon* const target_dungeon = \
		static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(zone_key));
	if(target_dungeon == NULL)
	{
		return FALSE;
	}

	//현재 위치한 Zone
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