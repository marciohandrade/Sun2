#include <Stdafx.h>

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
/*--------------------------------------------------------------------------------------------------
	-- Include Defined
--------------------------------------------------------------------------------------------------*/
#include "OneDaysGuildMissionRoom.h"

#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#include <UserSessions/User.h>

OneDaysGuildMissionRoom::~OneDaysGuildMissionRoom()
{
	Release();
}

void OneDaysGuildMissionRoom::Release()
{
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

	InstanceDungeon* dungeon = static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(room_key_));
	if (dungeon != NULL) {
		dungeon->SetRoomState(eROOMSTATE_DESTROY);
	}
}

INT OneDaysGuildMissionRoom::GetPlayerList_CharGuid(CHARGUID* list OUT)
{
	ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
	if (NULL == dungeon)
	{
		return 0;
	}

	const ZoneInterface::ZONE_USER_LIST& user_list = dungeon->GetUserList();
	ZoneInterface::ZONE_USER_LIST::const_iterator itr = user_list.begin();

	INT player_count = 0;
	while (itr != user_list.end())
	{
		User* user = itr->second;
		if (user == NULL)
		{
			continue;
		}

		list[player_count] = user->GetSelectedCharGuid();

		++itr;
		++player_count;
	}

	return player_count;
}

INT OneDaysGuildMissionRoom::Getstand_by_user_list_CharGuid(CHARGUID* list OUT)
{
	ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
	if (NULL == dungeon)
	{
		return 0;
	}

	// ��ü ��ŸƮ �� ���� ����Ʈ
	INT stand_by_user_cnt = 0;

	StandByMemberMap::const_iterator const_itr_members = stand_by_total_members_.begin();
	while (const_itr_members != stand_by_total_members_.end())
	{
		StandByMemberInfo* const stand_by_member = const_itr_members->second;
		if (stand_by_member == NULL)
		{
			continue;
		}

		list[stand_by_user_cnt] = stand_by_member->user_guid;

		++const_itr_members;
		++stand_by_user_cnt;
	}

	return stand_by_user_cnt;
}

User* OneDaysGuildMissionRoom::FindUser( DWORD player_key )
{
	ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
	if (NULL == dungeon) {
		return NULL;
	}

	return dungeon->FindUser(player_key);
}

void OneDaysGuildMissionRoom::KickAllUsers()
{
	ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
	if (NULL == dungeon) {
		return;
	}

	const ZoneInterface::ZONE_USER_LIST& user_list = dungeon->GetUserList();
	ZoneInterface::ZONE_USER_LIST::const_iterator itr = user_list.begin();

	while (itr != user_list.end())
	{
		User* user = itr->second;
		++itr;
		_KickUser(user);
	}
}

void OneDaysGuildMissionRoom::_KickUser(User* user)
{
	// �������� ��ȿ�� �˻�
	if(user == NULL)
	{
		SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] _KickUser() : user pointer is null!!"));
		return;
	}

	if(user->IsBeginTransaction())
	{
		SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] User State Transaction!"));
		return;
	}

	//���ư� ��
	MAPCODE return_map_code = user->GetSelectedCharMapCode();

	ZoneInterface* return_map = NULL;
	return_map = (ZoneInterface*)g_ZoneManager.FindVillage(return_map_code);
	if(return_map == NULL)
	{
		return_map = (ZoneInterface*)g_ZoneManager.FindHuntingField(return_map_code);
		if(return_map == NULL) {
			return;
		}
	}

	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(user,
		//{		From		|			To		}
		eZONETYPE_INSTANCE,   return_map->GetType()   //	eZONETYPE
		, user->GetZoneKey(),	return_map->GetKey()	//	ZoneKey
		);

	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		user->BeginTransaction(TR_LEAVE_INSTANCE_DUNGEON);
		user->SetTRRoomKey(return_map->GetKey());

		GAMELOG->MoveVillage(user, return_map->GetMapCode(), return_map_code, TRUE);
	}
	else {
		SUNLOG(eFULL_LOG,  "["__FUNCTION__"] LeaveRoom Error(%d) ", rt);
	}
}

bool OneDaysGuildMissionRoom::InsertStandByMember(const CHARGUID guid)
{
	StandByMemberInfo* const stand_by_member = 
		static_cast<StandByMemberInfo*>(stand_by_member_pool_->Alloc());

	stand_by_member->user_guid = guid;

	stand_by_total_members_.insert( \
		StandByMemberMap::value_type(guid, stand_by_member));

	stand_by_user_cnt++;

	return true;
}

void OneDaysGuildMissionRoom::RemoveStandByMember(const CHARGUID guid)
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

	stand_by_user_cnt--;
}

bool OneDaysGuildMissionRoom::IsStandByMember(const CHARGUID guid)
{
	StandByMemberMap::const_iterator itr = stand_by_total_members_.find(guid);
	if (itr != stand_by_total_members_.end())
	{
		// �̹� ��ŸƮ�� ����Ʈ�� �������..
		return true;
	}

	return false;
}

void OneDaysGuildMissionRoom::GetCurrentPlayerCnt(INT& current_player_cnt)
{
	ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
	if (NULL == dungeon)
	{
		return;
	}

	const ZoneInterface::ZONE_USER_LIST& user_list = dungeon->GetUserList();
	ZoneInterface::ZONE_USER_LIST::const_iterator itr = user_list.begin();

	current_player_cnt = 0;
	while (itr != user_list.end())
	{
		User* user = itr->second;
		if (user == NULL)
		{
			continue;
		}

		++itr;
		++current_player_cnt;
	}
}

void OneDaysGuildMissionRoom::SetOneDaysGuildMissionState(BYTE state)
{
	onedaysguildmission_state_ = state;
	if(onedaysguildmission_state_ == eGUILDMISSION_MATCHED)
	{
		if(!standbymember_init_check)
		{
			ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
			if (NULL == dungeon)
			{
				return;
			}

			const ZoneInterface::ZONE_USER_LIST& user_list = dungeon->GetUserList();
			ZoneInterface::ZONE_USER_LIST::const_iterator itr = user_list.begin();

			while (itr != user_list.end())
			{
				User* user = itr->second;
				if (user == NULL)
				{
					continue;
				}

				// �濡�� ������ ���� �Ǿ��ٸ� �ش� ������ ���� ���� ��� ����Ʈ�� ����.
				if(!IsStandByMember(user->GetSelectedCharGuid()))
				{
					InsertStandByMember(user->GetSelectedCharGuid());
				}

				++itr;
			}

			standbymember_init_check = true;
		}
	}
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)