#include <stdafx.h>

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include "OneDaysGuildMission.h"
#include "Player.h"

#include <GameInstanceDungeon.h>
#include <TriggerManagerEx.h>
#include "StatusManager.h"
#include "MapInfoParser.h"
#include "DominationAuction.h"
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "Monster.h"
#include "RewardManager.h"
#include "MissionManager.h"
#include "MissionRewardManager.h"
#include "ChaosZoneInfoParser.h"
#include <ItemManager.h>

#include "PacketStruct_AG_ChaosZoneSystem.h"
#include "PacketStruct_GZ.h"
#include <../ProgramCommon/ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <../Server/Common/PacketStruct_AG_ChaosZoneSystem.h>
#include <../ProgramCommon/ChaosZoneSystem/OneDaysGuildMissioninfoParser.h>
#include "World/ChaosZoneSystem/OneDaysGuildMissionResult.h"

void OneDaysGuildMission::Init(GameInstanceDungeon* const instance_dungeon)
{
	// 초기값 셋팅
	instance_dugeon_ = instance_dungeon;
	CHARGUID user_key = instance_dugeon_->GetMasterUserKey();

	onedays_guild_mission_result_ = new OneDaysGuildMissionResult();
	onedays_guild_mission_result_->Init(true);
	onedays_guild_mission_result_->SetOneDayGuildMissin(this);

	// 스크립트 Load
	MAPCODE map_code = instance_dugeon_->GetMapCode();
	const OneDaysGuildMissionInfoParser* const one_day_Guild_mission_info_parser = OneDaysGuildMissionInfoParser::Instance();
	if(onedays_guild_mission_info_ == NULL)
	{
		const OneDaysGuildMissionInfo* onedays_guild_mission_info = one_day_Guild_mission_info_parser->FindData(map_code);
		if(onedays_guild_mission_info)
		{
			onedays_guild_mission_info_ = onedays_guild_mission_info;
		}
		else
		{
			SUNLOG(eCRITICAL_LOG,  
				"[OneDaysGuildMission::Init()] OneDaysGuildMissionInfo is NULL. MapCode[%d]. ",map_code);
		}
	}

	is_stamp_state_ = false;

	stand_by_member_pool_ = new StandByMemberPool();

	stand_by_member_pool_->Initialize(200);

	// 방 생성 후 게임 시작 시간 셋팅
	start_wait_timer_.SetTimer(onedays_guild_mission_info_->start_wait_time * kSecond); // script..
	time_sync_cycle_.SetTimer(TIME_SYNC_CYCLE);

	SetOneDaysGuildMissionState(eGUILDMISSION_ENTERWAIT);
}

void OneDaysGuildMission::Release()
{
	// 자료구조 관련해서 삭제해주자....
	SAFE_RELEASENDELETE(onedays_guild_mission_result_);
	guild_mission_state_ = eGUILDMISSION_NONE;
	stamp_start_time_ = 0;

	// 전체 스타트 업 유저 리스트 Release
	StandByMemberMap::const_iterator const_itr_members = stand_by_total_members_.begin();
	for (; const_itr_members != stand_by_total_members_.end(); ++const_itr_members)
	{
		StandByMemberInfo* const stand_by_member = const_itr_members->second;
		if (stand_by_member == NULL)
		{
			continue;
		}
		stand_by_member_pool_->Free(stand_by_member);
	}

	stand_by_total_members_.clear();
}

void OneDaysGuildMission::Update()
{
	switch (guild_mission_state_)
	{
	case eGUILDMISSION_ENTERWAIT:
		{
			// 채집을 성공했거나 시작 대기시간이 모두 지났을 경우 강제 시작 시킨다.
			if((is_stamp_state_) || (start_wait_timer_.GetRemainedTime() <= 0))
			{
				// 전장 시작
				_Start();
			}
		}
		break;
	case eGUILDMISSION_MATCHED:
		{
			// 일일 길드미션 제한 시간이 다 되었는지 체크하여 상태 변경
			if(_IsGuildMissionEnd())
			{
				SetOneDaysGuildMissionState(eGUILDMISSION_MATCHEDEND);
			}
		}
		break;
	case eGUILDMISSION_MATCHEDEND:
		{
			// 일일길드미션 상태 변경
			SetOneDaysGuildMissionState(eGUILDMISSION_RESULT);
		}
		break;
	case eGUILDMISSION_RESULT:
		{
			// 종료 처리 및 보상 처리
			_Result();

			// 일일 길드 미션 종료 후 대기 시간
			end_wait_timer_.SetTimer(onedays_guild_mission_info_->end_wait_time * kSecond);

			// 일일 길드미션 상태 변경
			SetOneDaysGuildMissionState(eGUILDMISSION_KICK_USER);
		}
		break;
	case eGUILDMISSION_KICK_USER:
		{
			// 플레이어 강제 퇴장
			if (end_wait_timer_.IsExpiredManual(FALSE) == TRUE)
			{
				MSG_AG_ONEDAYSGUILDMISSION_KICKUSER_CMD cmd_msg;
				cmd_msg.room_key = instance_dugeon_->GetKey();
				g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));

				// 어느정도 뒤에 방을 파괴하자. 유저들 내보낸뒤에 내보내자.
				room_destroy_timer_.SetTimer(ROOM_DESTROY_WAIT_TIME * kSecond);

				// 일일 길드미션 상태 변경
				SetOneDaysGuildMissionState(eGUILDMISSION_DESTROY);
			}
		}
	case eGUILDMISSION_DESTROY:
		{
			if(room_destroy_timer_.IsExpiredManual(FALSE) == TRUE)
			{

				MSG_AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD cmd_msg;
				cmd_msg.room_key = instance_dugeon_->GetKey();
				g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));

				// 일일 길드미션 상태 변경
				guild_mission_state_ = eGUILDMISSION_END;
			}
		}
		break;
	case eGUILDMISSION_END:
		{
			// 종료
		}
		break;
	}

	// 방의 남은 시간 정보를 보내주자. 게임이 진행 중일때만
	eGUILDMISSION_STATE state = guild_mission_state_;
	if(state == eGUILDMISSION_MATCHED)
	{
		if (time_sync_cycle_.IsExpired() == TRUE)
		{
			_SendTimeSync();

			time_sync_cycle_.Reset();
		}

		// Agent가 가지고 있는 시간 갱신
		SetOneDaysGuildMissionState(eGUILDMISSION_MATCHED);
	}
}

bool OneDaysGuildMission::EnterPlayer(Player* const player)
{
	// player 유효성 체크
	if((player == NULL) || player->IsDeleted())
	{
		return false;
	}

	// 플레이어 추가
	player_hash_.insert(PLAYER_HASH::value_type(player->GetCharGuid(), player));

	if(guild_mission_state_ >= eGUILDMISSION_MATCHED)
	{
		if(!IsStandByMember(player->GetCharGuid()))  // 스텐바이 유저일 경우 해당 아이템을 삭제하지 않는다.
		{
			// 아이템 삭제
			if(!_ItemDelete(player))
				return false;
		}
	}

	// 비정상 접속 종료자.
	bool is_abnormal_leave = false;

	if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE) != NULL || //도망자 버프가 있을 경우..
		guild_mission_state_ == eGUILDMISSION_MATCHED) // 존 이동 도중 비정상 종료되어 전장 시합 중에 들어왔을 경우..
	{
		is_abnormal_leave = true;

		// 상태제거 (도망자 버프가 걸린 사람은 예약이 안되기 때문에 어뷰징에 해당하지 않음)
		if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE))
		{
			if (!player->GetStatusManager()->Remove(eCHAR_STATE_FUGITIVE))
			{
				MSG_CG_STATUS_REMOVE_BRD BrdMsg;
				BrdMsg.m_dwTargetKey = player->GetObjectKey();
				BrdMsg.m_dwStatusCode = eCHAR_STATE_FUGITIVE;
				player->SendPacketAround(&BrdMsg, BrdMsg.GetSize());
			}
		}

		// 입장한 유저에게 방 정보 전달
		_SendTimeSyncForPlayer(player);
	}
	else if (guild_mission_state_ == eGUILDMISSION_MATCHED) // 전장 들어오기전 비정상접송종료자 처리..
	{
		is_abnormal_leave = true;
	}

	{
		// 입장한 유저들에 대한 Log
		TCHAR add_info[MAX_ADDINFO_SIZE] = { 0, };
		// add_info 상의 내용은 추후 정리하여 넣자...
		add_info[MAX_ADDINFO_SIZE-1] = _T('\0');

		GAMELOG->LogChaosZone(ACT_ONE_DAYS_GUILD_MISSION_ENTER, player, NULL, add_info);
	}

	return true;
}

void OneDaysGuildMission::LeavePlayer(Player* const player)
{
	// player 유효성 체크
	if((player == NULL) || player->IsDeleted())
	{
		return;
	}

	// Agent 쪽에 해당 유저가 나간것을 알려줌(방에 아무도 없을 경우 삭제하기 위해)
	MSG_AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD cmd_msg;
	cmd_msg.zone_key = instance_dugeon_->GetKey();
	player->SendPacket(&cmd_msg,sizeof(cmd_msg));

	// 플레이어 삭제
	player_hash_.erase(player->GetCharGuid());

	// 퇴장한 유저들에 대한 Log
	{
		// 퇴장한 유저들에 대한 Log
		TCHAR add_info[MAX_ADDINFO_SIZE] = { 0, };
		// add_info 상의 내용은 추후 정리하여 넣자...
		add_info[MAX_ADDINFO_SIZE-1] = _T('\0');

		GAMELOG->LogChaosZone(ACT_ONE_DAYS_GUILD_MISSION_LEAVE, player, NULL, add_info);
	}
}

void OneDaysGuildMission::EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list)
{
	return;
}

eUSER_RELATION_TYPE OneDaysGuildMission::IsFriendPVP(Player* attacker,Player* target)
{
	return USER_RELATION_FRIEND;
}

void OneDaysGuildMission::DeadCharacter(Character* const dead_character, Character* const kill_character)
{
	// 유효성 검사
	if((dead_character == NULL) || (dead_character->IsDeleted()))
	{
		return;
	}

	// 이 방에서는 플레어이가 플레이어를 죽이는 일이 없다...
	if(dead_character->IsEqualObjectKind(PLAYER_OBJECT) == false)
	{
		return;
	}

	return;
}

void OneDaysGuildMission::SetOneDaysGuildMissionState(eGUILDMISSION_STATE state)
{
	guild_mission_state_ = state;

	// Agent에게 상태값 갱신 전달
	MSG_AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD cmd_msg;
	cmd_msg.room_key_ = instance_dugeon_->GetKey();
	cmd_msg.event_state_ = guild_mission_state_;
	if(guild_mission_state_ == eGUILDMISSION_MATCHED)
	{
		cmd_msg.remain_time_ = mission_limit_timer_.GetRemainedTime();
	}
	else
	{
		cmd_msg.remain_time_ = 0;
	}

	g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));
}

void OneDaysGuildMission::SuccessStamp(Player* player)
{
#ifdef _DEBUG
	if(player)
	{
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] OneDaysGuildMission Stamp Success![Guild:%d]", player->GetGuild());
	}
#endif //(_DEBUG)

	// 채집(각인)에 성공하면 미션 상태값을 변경
	is_stamp_state_ = true;
}

void OneDaysGuildMission::SetStampStartTime()
{
	stamp_start_time_ = GetTickCount();
}

bool OneDaysGuildMission::InsertStandByMember(const CHARGUID guid)
{
	StandByMemberInfo* const stand_by_member = 
		static_cast<StandByMemberInfo*>(stand_by_member_pool_->Alloc());

	stand_by_member->user_guid = guid;

	stand_by_total_members_.insert( \
		StandByMemberMap::value_type(guid, stand_by_member));

	return true;
}

void OneDaysGuildMission::RemoveStandByMember(const CHARGUID guid)
{
	// 형식을 맞추기 위해 넣었다...
	StandByMemberMap::iterator itr = stand_by_total_members_.find(guid);
	if (itr == stand_by_total_members_.end())
	{
		// 리스트에 없으니 지울것도 없다..
		return;
	}

	StandByMemberInfo* const stand_by_member = itr->second;

	stand_by_member_pool_->Free(stand_by_member);
	stand_by_total_members_.erase(itr);
}

bool OneDaysGuildMission::IsStandByMember(const CHARGUID guid)
{
	StandByMemberMap::const_iterator itr = stand_by_total_members_.find(guid);
	if (itr != stand_by_total_members_.end())
	{
		// 이미 스타트업 리스트에 들어있음..
		return true;
	}

	return false;
}

void OneDaysGuildMission::_Start()
{
	// 게임 진행이 되면 시작 트리거를 발동 시킨다.
	WzID trjgger_id = StrToWzID("STR0");
	if(instance_dugeon_->TriggerActive(trjgger_id) == false)
	{
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] 트리거(%g)를 액티브하는데 실패하였습니다.", trjgger_id);
	}

	// 게임 플레이 시간 설정
	mission_limit_timer_.SetTimer(onedays_guild_mission_info_->play_time * kSecond);

	// 전장 진행 상태
	SetOneDaysGuildMissionState(eGUILDMISSION_MATCHED);

	SUNBATTLEGROUNDLOG(
		eCRITICAL_LOG, 
		TEXT("| [ZONE_KEY(%d)] | Start | 전장 시작!"), 
		instance_dugeon_->GetKey());

	// 전장에 참가한 유저들의 입장 아이템 소모
	PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
	for(; const_itr != player_hash_.end(); ++const_itr)
	{
		Player* const player = const_itr->second;
		if (player == NULL)
			continue;

		if(!_ItemDelete(player))
			return;
	}
}

void OneDaysGuildMission::_Result()
{
	// 죽어있으면 살리고 각자 안전지대로 이동시킨다.
	PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
	for(; const_itr != player_hash_.end(); ++const_itr)
	{
		Player* const player = const_itr->second;
		if (player == NULL || player->IsDeleted())
			continue;

		OnMoveToSafezone(player);

		if (player->IsDead())
		{
			player->OnResurrection( 0, 1.f, 1.f );
		}

		// 보상..
		if (onedays_guild_mission_info_->base_reward_item_code == 0)
		{
			return;
		}

		FlowControl flow;
		FLOWCONTROL_START()
		{
			MissionManager* const mission_manager = player->GetMissionManager();
			mission_manager->CheckEnableWindowOpen(); // 그럴리 없다고 단정지어진 것이지만, 중복된 보상이 존재할 경우를 대비
			if (mission_manager->EnableWindowOpen())
			{
				TriggerManagerEx::Check_RewardWindow(player, true);
				return;
			}

			DWORD mission_number = onedays_guild_mission_info_->base_reward_item_code;
			
			MissionRewardManager* reward_manager = MissionRewardManager::Instance();
			reward_manager->Init();

			MissionRewardArgs& mission_reward_args = reward_manager->GetMissionRewardArgs();
			mission_reward_args.SetPlayer(player);
			mission_reward_args.SetMissionManager(mission_manager);
			mission_reward_args.SetNum(mission_number);

			const WORD reward_code = reward_manager->GetRewardCode(true);
			ASSERT(0xFFFF != reward_code);
			if (0xFFFF != reward_code)
			{
				MSG_CG_TRIGGER_REWARD_WINDOW_CMD cmsg;
				cmsg.m_RewardCode = reward_code;
				player->SendPacket(&cmsg, sizeof(cmsg));
			}

			mission_manager->SetMissionState(mission_number, MISSION_REWARD);
			mission_manager->SetMissionPoint(mission_number, 1);

		}
		FLOWCONTROL_END;
	}
}

bool OneDaysGuildMission::_IsGuildMissionEnd()
{
	// 일일 길드미션 제한 시간이 다 되었는지 체크
	if(mission_limit_timer_.IsExpiredManual(false))
	{
		return true;
	}

	return false;
}

void OneDaysGuildMission::_SentToAllPlayerInfo(BYTE infoflag, CHARGUID user_guid)
{
	// 플레이어 정보를 보내자... 이탈자 또는 내용 변경이 있을 경우
	MSG_CG_ONEDAYSGUILDMISSION_PLAYER_INFO_BRD brd_msg;

	switch (infoflag)
	{
	case 1:
		{
			// 입장한 유저의 정보 전달
			brd_msg.infoflag = infoflag;
			brd_msg.enter_player_guid = user_guid;
		}
	default:
		{
			// 여기에 들어오면 할게 없음...
			return;
		}
		break;
	}

	_SendPacketToAllEx(&brd_msg, sizeof(brd_msg), user_guid);
}

void OneDaysGuildMission::_SendTimeSync()
{
	MSG_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD brd_msg;
	brd_msg.event_state_ = guild_mission_state_;
	brd_msg.remain_time_ = mission_limit_timer_.GetRemainedTime();

	GameInstanceDungeon* dungeon = GetInstanceDungeon();
	dungeon->SendPacketToAll(&brd_msg, sizeof(brd_msg));	
}

void OneDaysGuildMission::_SendTimeSyncForPlayer(Player* player)
{
	MSG_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD brd_msg;
	brd_msg.event_state_ = guild_mission_state_;
	brd_msg.remain_time_ = mission_limit_timer_.GetRemainedTime();

	player->SendPacket(&brd_msg, sizeof(brd_msg));
}

bool OneDaysGuildMission::_ItemDelete(Player* player)
{
	// 아이템 삭제
	ChaosZoneInfoParser* chaoszone_parser = ChaosZoneInfoParser::Instance();
	MAPCODE mapcode = chaoszone_parser->GetOneDaysGuildmissionMapCode();
	const ChaosZoneInfo* mission_info = chaoszone_parser->FindData(mapcode);
	if (mission_info == NULL)
	{
		return false;
	}

	MSG_CG_ITEM_LOSE_ACK item_msg;
	POSTYPE item_count = 1;    //1개 고정 (스크립트에도 수량정보는 포함되어있지 않다)
	WORD item_code = mission_info->require_item[0];
	SCItemSlot deleted_item_slot;

	//입장아이템이 필요한 미션인가
	if (chaoszone_parser->IsMissionOfRequireItem(mission_info->map_code, eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION) == false)
	{
		return false;
	}

	if (false == chaoszone_parser->IsMissionItem(mission_info->map_code, eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION, item_code))
	{
		return false;
	}

	//3. 플레이어의 해당아이템 소지여부 검사
	INT having_item_num = player->GetItemManager()->ExistItem(SI_INVENTORY, item_code, item_count);
	if (having_item_num < item_count)
	{
		return false;
	}

	// 아이템을 가지고 있는지만 입장시 체크하고 게임 진행이 되는 시점에서 소모 시킨다.
	//4. 아이템 소모시키고 ACK
	if ( RC::RC_ITEM_SUCCESS != player->GetItemManager()->DeleteAllKindOfItem( \
		1, &item_code, &item_count, &(item_msg.m_ItemInfo), &deleted_item_slot ) )
	{
		return false;
	}

	//RC_ITEM_SUCCESS
	player->SendPacket(&item_msg, sizeof(item_msg));
	GAMELOG->LogItem(ITEM_USE, player, &deleted_item_slot, 0, item_count);

	// 스텐바이 유저 목록에 추가
	InsertStandByMember(player->GetCharGuid());

	return true;
}

void OneDaysGuildMission::_SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size)
{
	// 모든 플레이어에게 패킷보내기
	GameInstanceDungeon* dungeon = GetInstanceDungeon();
	dungeon->SendPacketToAll(msg, msg_size);
}

void OneDaysGuildMission::_SendPacketToAllEx(MSG_BASE_FORWARD* msg, WORD msg_size, CHARGUID charguid)
{
	PLAYER_HASH::const_iterator const_itr = player_hash_.begin();
	for(; const_itr != player_hash_.end(); ++const_itr)
	{
		Player* const player = const_itr->second;
		if (player == NULL || player->IsDeleted())
		{
			continue;
		}

		if(player->GetCharGuid() != charguid)
		{
			player->SendPacket(msg, msg_size);
		}
	}
}

RC::eSTATUS_RESULT OneDaysGuildMission::ResurrectionAtSafezone( Player *player )
{
	//무적 상태,...?
	OnMoveToSafezone(player);

	if (player->IsDead())
	{
		player->OnResurrection( 0, 1.f, 1.f );
	}

	return RC::RC_STATUS_SUCCESS;
}

RC::eSTATUS_RESULT OneDaysGuildMission::OnMoveToSafezone(Player* const player)
{
	GameField* const field = player->GetField();
	if (field == NULL)
	{
		return RC::RC_STATUS_FIELD_IS_NULL;
	}

	const sMAPINFO* const map_info = \
		MapInfoParser::Instance()->FindMapInfo(this->GetInstanceDungeon()->GetMapCode());
	if (map_info == NULL)
	{
		return RC::RC_STATUS_MAPINFO_IS_NULL;
	}

	DWORD area_id = map_info->wStartAreaID;

	WzVector dest_pos;
	if (field->GetAreaRandomStartingVector(area_id, dest_pos) == false)
	{
		return RC::RC_STATUS_IS_NOT_SAFEZONE;
	}

	// 시작위치로 텔레포트 시킨다.
	if(field->TeleportObject( player, &dest_pos, TRUE ) == false)
	{
		return RC::RC_STATUS_TELEPORT_FAIL;
	}

	return RC::RC_STATUS_SUCCESS;
}

Player* OneDaysGuildMission::_FindPlayer(const DWORD char_guid)
{
	PLAYER_HASH::iterator itr = player_hash_.find(char_guid);
	if (itr == player_hash_.end())
	{
		return NULL;
	}
	return itr->second;
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
