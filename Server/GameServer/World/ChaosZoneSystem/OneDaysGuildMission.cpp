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
	// �ʱⰪ ����
	instance_dugeon_ = instance_dungeon;
	CHARGUID user_key = instance_dugeon_->GetMasterUserKey();

	onedays_guild_mission_result_ = new OneDaysGuildMissionResult();
	onedays_guild_mission_result_->Init(true);
	onedays_guild_mission_result_->SetOneDayGuildMissin(this);

	// ��ũ��Ʈ Load
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

	// �� ���� �� ���� ���� �ð� ����
	start_wait_timer_.SetTimer(onedays_guild_mission_info_->start_wait_time * kSecond); // script..
	time_sync_cycle_.SetTimer(TIME_SYNC_CYCLE);

	SetOneDaysGuildMissionState(eGUILDMISSION_ENTERWAIT);
}

void OneDaysGuildMission::Release()
{
	// �ڷᱸ�� �����ؼ� ����������....
	SAFE_RELEASENDELETE(onedays_guild_mission_result_);
	guild_mission_state_ = eGUILDMISSION_NONE;
	stamp_start_time_ = 0;

	// ��ü ��ŸƮ �� ���� ����Ʈ Release
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
			// ä���� �����߰ų� ���� ���ð��� ��� ������ ��� ���� ���� ��Ų��.
			if((is_stamp_state_) || (start_wait_timer_.GetRemainedTime() <= 0))
			{
				// ���� ����
				_Start();
			}
		}
		break;
	case eGUILDMISSION_MATCHED:
		{
			// ���� ���̼� ���� �ð��� �� �Ǿ����� üũ�Ͽ� ���� ����
			if(_IsGuildMissionEnd())
			{
				SetOneDaysGuildMissionState(eGUILDMISSION_MATCHEDEND);
			}
		}
		break;
	case eGUILDMISSION_MATCHEDEND:
		{
			// ���ϱ��̼� ���� ����
			SetOneDaysGuildMissionState(eGUILDMISSION_RESULT);
		}
		break;
	case eGUILDMISSION_RESULT:
		{
			// ���� ó�� �� ���� ó��
			_Result();

			// ���� ��� �̼� ���� �� ��� �ð�
			end_wait_timer_.SetTimer(onedays_guild_mission_info_->end_wait_time * kSecond);

			// ���� ���̼� ���� ����
			SetOneDaysGuildMissionState(eGUILDMISSION_KICK_USER);
		}
		break;
	case eGUILDMISSION_KICK_USER:
		{
			// �÷��̾� ���� ����
			if (end_wait_timer_.IsExpiredManual(FALSE) == TRUE)
			{
				MSG_AG_ONEDAYSGUILDMISSION_KICKUSER_CMD cmd_msg;
				cmd_msg.room_key = instance_dugeon_->GetKey();
				g_pGameServer->SendToServer(AGENT_SERVER, &cmd_msg, sizeof(cmd_msg));

				// ������� �ڿ� ���� �ı�����. ������ �������ڿ� ��������.
				room_destroy_timer_.SetTimer(ROOM_DESTROY_WAIT_TIME * kSecond);

				// ���� ���̼� ���� ����
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

				// ���� ���̼� ���� ����
				guild_mission_state_ = eGUILDMISSION_END;
			}
		}
		break;
	case eGUILDMISSION_END:
		{
			// ����
		}
		break;
	}

	// ���� ���� �ð� ������ ��������. ������ ���� ���϶���
	eGUILDMISSION_STATE state = guild_mission_state_;
	if(state == eGUILDMISSION_MATCHED)
	{
		if (time_sync_cycle_.IsExpired() == TRUE)
		{
			_SendTimeSync();

			time_sync_cycle_.Reset();
		}

		// Agent�� ������ �ִ� �ð� ����
		SetOneDaysGuildMissionState(eGUILDMISSION_MATCHED);
	}
}

bool OneDaysGuildMission::EnterPlayer(Player* const player)
{
	// player ��ȿ�� üũ
	if((player == NULL) || player->IsDeleted())
	{
		return false;
	}

	// �÷��̾� �߰�
	player_hash_.insert(PLAYER_HASH::value_type(player->GetCharGuid(), player));

	if(guild_mission_state_ >= eGUILDMISSION_MATCHED)
	{
		if(!IsStandByMember(player->GetCharGuid()))  // ���ٹ��� ������ ��� �ش� �������� �������� �ʴ´�.
		{
			// ������ ����
			if(!_ItemDelete(player))
				return false;
		}
	}

	// ������ ���� ������.
	bool is_abnormal_leave = false;

	if (player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE) != NULL || //������ ������ ���� ���..
		guild_mission_state_ == eGUILDMISSION_MATCHED) // �� �̵� ���� ������ ����Ǿ� ���� ���� �߿� ������ ���..
	{
		is_abnormal_leave = true;

		// �������� (������ ������ �ɸ� ����� ������ �ȵǱ� ������ ���¡�� �ش����� ����)
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

		// ������ �������� �� ���� ����
		_SendTimeSyncForPlayer(player);
	}
	else if (guild_mission_state_ == eGUILDMISSION_MATCHED) // ���� �������� ���������������� ó��..
	{
		is_abnormal_leave = true;
	}

	{
		// ������ �����鿡 ���� Log
		TCHAR add_info[MAX_ADDINFO_SIZE] = { 0, };
		// add_info ���� ������ ���� �����Ͽ� ����...
		add_info[MAX_ADDINFO_SIZE-1] = _T('\0');

		GAMELOG->LogChaosZone(ACT_ONE_DAYS_GUILD_MISSION_ENTER, player, NULL, add_info);
	}

	return true;
}

void OneDaysGuildMission::LeavePlayer(Player* const player)
{
	// player ��ȿ�� üũ
	if((player == NULL) || player->IsDeleted())
	{
		return;
	}

	// Agent �ʿ� �ش� ������ �������� �˷���(�濡 �ƹ��� ���� ��� �����ϱ� ����)
	MSG_AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD cmd_msg;
	cmd_msg.zone_key = instance_dugeon_->GetKey();
	player->SendPacket(&cmd_msg,sizeof(cmd_msg));

	// �÷��̾� ����
	player_hash_.erase(player->GetCharGuid());

	// ������ �����鿡 ���� Log
	{
		// ������ �����鿡 ���� Log
		TCHAR add_info[MAX_ADDINFO_SIZE] = { 0, };
		// add_info ���� ������ ���� �����Ͽ� ����...
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
	// ��ȿ�� �˻�
	if((dead_character == NULL) || (dead_character->IsDeleted()))
	{
		return;
	}

	// �� �濡���� �÷����̰� �÷��̾ ���̴� ���� ����...
	if(dead_character->IsEqualObjectKind(PLAYER_OBJECT) == false)
	{
		return;
	}

	return;
}

void OneDaysGuildMission::SetOneDaysGuildMissionState(eGUILDMISSION_STATE state)
{
	guild_mission_state_ = state;

	// Agent���� ���°� ���� ����
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

	// ä��(����)�� �����ϸ� �̼� ���°��� ����
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
	// ������ ���߱� ���� �־���...
	StandByMemberMap::iterator itr = stand_by_total_members_.find(guid);
	if (itr == stand_by_total_members_.end())
	{
		// ����Ʈ�� ������ ����͵� ����..
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
		// �̹� ��ŸƮ�� ����Ʈ�� �������..
		return true;
	}

	return false;
}

void OneDaysGuildMission::_Start()
{
	// ���� ������ �Ǹ� ���� Ʈ���Ÿ� �ߵ� ��Ų��.
	WzID trjgger_id = StrToWzID("STR0");
	if(instance_dugeon_->TriggerActive(trjgger_id) == false)
	{
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Ʈ����(%g)�� ��Ƽ���ϴµ� �����Ͽ����ϴ�.", trjgger_id);
	}

	// ���� �÷��� �ð� ����
	mission_limit_timer_.SetTimer(onedays_guild_mission_info_->play_time * kSecond);

	// ���� ���� ����
	SetOneDaysGuildMissionState(eGUILDMISSION_MATCHED);

	SUNBATTLEGROUNDLOG(
		eCRITICAL_LOG, 
		TEXT("| [ZONE_KEY(%d)] | Start | ���� ����!"), 
		instance_dugeon_->GetKey());

	// ���忡 ������ �������� ���� ������ �Ҹ�
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
	// �׾������� �츮�� ���� ��������� �̵���Ų��.
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

		// ����..
		if (onedays_guild_mission_info_->base_reward_item_code == 0)
		{
			return;
		}

		FlowControl flow;
		FLOWCONTROL_START()
		{
			MissionManager* const mission_manager = player->GetMissionManager();
			mission_manager->CheckEnableWindowOpen(); // �׷��� ���ٰ� ���������� ��������, �ߺ��� ������ ������ ��츦 ���
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
	// ���� ���̼� ���� �ð��� �� �Ǿ����� üũ
	if(mission_limit_timer_.IsExpiredManual(false))
	{
		return true;
	}

	return false;
}

void OneDaysGuildMission::_SentToAllPlayerInfo(BYTE infoflag, CHARGUID user_guid)
{
	// �÷��̾� ������ ������... ��Ż�� �Ǵ� ���� ������ ���� ���
	MSG_CG_ONEDAYSGUILDMISSION_PLAYER_INFO_BRD brd_msg;

	switch (infoflag)
	{
	case 1:
		{
			// ������ ������ ���� ����
			brd_msg.infoflag = infoflag;
			brd_msg.enter_player_guid = user_guid;
		}
	default:
		{
			// ���⿡ ������ �Ұ� ����...
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
	// ������ ����
	ChaosZoneInfoParser* chaoszone_parser = ChaosZoneInfoParser::Instance();
	MAPCODE mapcode = chaoszone_parser->GetOneDaysGuildmissionMapCode();
	const ChaosZoneInfo* mission_info = chaoszone_parser->FindData(mapcode);
	if (mission_info == NULL)
	{
		return false;
	}

	MSG_CG_ITEM_LOSE_ACK item_msg;
	POSTYPE item_count = 1;    //1�� ���� (��ũ��Ʈ���� ���������� ���ԵǾ����� �ʴ�)
	WORD item_code = mission_info->require_item[0];
	SCItemSlot deleted_item_slot;

	//����������� �ʿ��� �̼��ΰ�
	if (chaoszone_parser->IsMissionOfRequireItem(mission_info->map_code, eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION) == false)
	{
		return false;
	}

	if (false == chaoszone_parser->IsMissionItem(mission_info->map_code, eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION, item_code))
	{
		return false;
	}

	//3. �÷��̾��� �ش������ �������� �˻�
	INT having_item_num = player->GetItemManager()->ExistItem(SI_INVENTORY, item_code, item_count);
	if (having_item_num < item_count)
	{
		return false;
	}

	// �������� ������ �ִ����� ����� üũ�ϰ� ���� ������ �Ǵ� �������� �Ҹ� ��Ų��.
	//4. ������ �Ҹ��Ű�� ACK
	if ( RC::RC_ITEM_SUCCESS != player->GetItemManager()->DeleteAllKindOfItem( \
		1, &item_code, &item_count, &(item_msg.m_ItemInfo), &deleted_item_slot ) )
	{
		return false;
	}

	//RC_ITEM_SUCCESS
	player->SendPacket(&item_msg, sizeof(item_msg));
	GAMELOG->LogItem(ITEM_USE, player, &deleted_item_slot, 0, item_count);

	// ���ٹ��� ���� ��Ͽ� �߰�
	InsertStandByMember(player->GetCharGuid());

	return true;
}

void OneDaysGuildMission::_SendPacketToAll(MSG_BASE_FORWARD* msg, WORD msg_size)
{
	// ��� �÷��̾�� ��Ŷ������
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
	//���� ����,...?
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

	// ������ġ�� �ڷ���Ʈ ��Ų��.
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
