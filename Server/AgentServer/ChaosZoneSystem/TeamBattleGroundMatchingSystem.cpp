#include "StdAfx.h"

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include <ChaosZoneSystem/TeamBattleGroundMatchingSystem.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>
#include <AgentParty/AgentPartyManager.h>
#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <BattleGroundInfoParser.h>

void TeamBattleGroundMatchingSystem::Init()
{
    team_matching_group_pool_ = new TeamMatchingGroupPool();
    team_matching_group_pool_->Initialize(10);
    update_timer_.SetTimer(1000);
}

void TeamBattleGroundMatchingSystem::Release()
{
    SAFE_DELETE(team_matching_group_pool_);
}

void TeamBattleGroundMatchingSystem::Update()
{
    if (update_timer_.IsExpired() == false) {
        return;
    }

    CTime current_time = util::TimeSync::_time64(NULL);
    const BattleGroundInfoParser* const battle_ground_Info_parser = BattleGroundInfoParser::Instance();

    TeamMatchingGroupMap::iterator itr = team_matching_groups_map_.begin();
    while (itr != team_matching_groups_map_.end())
    {
        TeamMatchingGroupMap::iterator cur_itr = itr++; //!!!!!
        TeamMatchingGroup* const matching_group = cur_itr->second;
        if (matching_group == NULL) {
            team_matching_groups_map_.erase(cur_itr);
            continue;
        }

        const BattleGroundInfo* battle_ground_Info = \
            battle_ground_Info_parser->FindData(matching_group->map_code);
        if (battle_ground_Info == NULL)
        {
            continue;
        }
        if (matching_group->state == TeamMatchingGroup::kCreatedRoom &&
            current_time >= matching_group->matching_time + 
            CTimeSpan(0, 0, 0, battle_ground_Info->start_time)) // script..)
        {
            if (OnCreateTeam(matching_group) == false ||
                SendMembersToBattleGround(matching_group) == false)
            {
                MatchingGroupCancel(matching_group->group_key);
                continue;
            }
            matching_group->state = TeamMatchingGroup::kStartBattleGround;
        }
    }
}

bool TeamBattleGroundMatchingSystem::CheckMatching(const MAPCODE map_code)
{
    if (CanCreateMatchingGroup(map_code) == false) {
        return false;
    }

    if (CreateMatchingGroup(map_code) == false) {
        return false;
    }
    
    return true;
}

bool TeamBattleGroundMatchingSystem::CanCreateMatchingGroup(const MAPCODE map_code)
{
    TeamBattleGroundManager* const team_battle_ground_manager = \
        TeamBattleGroundManager::Instance();
    if (team_battle_ground_manager->GetTeamSize() < 2)
    {
        if (team_battle_ground_manager->CreateTeam(map_code) == false)
        {
            return false;
        }
    }

    if (team_battle_ground_manager->IsExistTeam(kRed) == false || 
        team_battle_ground_manager->IsExistTeam(kBlue) == false)
    {
        return false;
    }
    return true;
}

bool TeamBattleGroundMatchingSystem::CreateMatchingGroup(const MAPCODE map_code)
{
    TeamMatchingGroup* const matching_group = OnMatching();
    if (matching_group == NULL)
    {
        return false;
    }

    //그룹키 셋팅
    const KEYTYPE group_key = _AllocKey();
    matching_group->group_key = group_key;
    matching_group->state = TeamMatchingGroup::kCreatedRoom;

    matching_group->matching_time = util::TimeSync::_time64(NULL);
    team_matching_groups_map_.insert(TeamMatchingGroupMap::value_type(group_key, matching_group));


    //방생성 요청..
    if (CreateInstanceRoom(group_key, map_code) == false) 
    {
        // 매칭 취소..
        MatchingGroupCancel(group_key);
        return false;
    }

    //BRD.. 매칭그룹에 속한 멤버들에게 1차 매칭이 되었다고 알려줌.
    TeamBattleGroundManager* const team_battle_ground_manager = TeamBattleGroundManager::Instance();
    MSG_CG_BATTLE_GROUND_MATCHING_RESULT_BRD brd_msg;
    {
        brd_msg.result_type = 0;
        brd_msg.map_code = matching_group->map_code;
    }
    team_battle_ground_manager->SendPacketAllMember(&matching_group->matching_group_list, 
                                                    &brd_msg, sizeof(brd_msg));

    return true;
}
bool TeamBattleGroundMatchingSystem::MatchingGroupCancel(const KEYTYPE group_key)
{
    TeamMatchingGroup* const matching_group = FindMatchingGroup(group_key);
    if (matching_group == NULL)
    {
        return false;
    }

    if (RemoveMatchigGroup(group_key) == false)
    {
        return false;
    }
    return true;
}

TeamMatchingGroup* TeamBattleGroundMatchingSystem::FindMatchingGroup(const KEYTYPE group_key)
{
    TeamMatchingGroupMap::const_iterator const_itr = team_matching_groups_map_.find(group_key);
    if (const_itr == team_matching_groups_map_.end())
    {
        return NULL;
    }
    return const_itr->second;
}

TeamMatchingGroup* TeamBattleGroundMatchingSystem::FindMatchingGroupByZoneKey(const KEYTYPE zone_key)
{
    TeamMatchingGroupMap::const_iterator const_itr = team_matching_groups_map_.begin();
    for (; const_itr != team_matching_groups_map_.end(); ++const_itr)
    {
        TeamMatchingGroup* const matching_group = const_itr->second;
        if (matching_group->zone_key != zone_key)
        {
            continue;
        }
        return matching_group;
    }
    return NULL;
}

TeamMatchingGroup* TeamBattleGroundMatchingSystem::OnMatching()
{
    TeamMatchingGroup* matching_group = static_cast<TeamMatchingGroup*>(team_matching_group_pool_->Alloc());
    if (matching_group == NULL) 
    {
        return NULL;
    }
    
    const UserManager* const user_manager = UserManager::Instance();
    //const AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    TeamBattleGroundManager* const team_battle_ground_manager = \
        TeamBattleGroundManager::Instance();

    for (int i = 0; i < 2 ; ++i)
    {
        TeamBattleGroundTeamInfo* const team_info = team_battle_ground_manager->GetTeamInfoFront();
        if (team_info == NULL)
        {
            return NULL;
        }
        TeamBattleGroundMemberList& member_list = team_info->member_list;
        TeamBattleGroundMemberList::iterator itr = member_list.begin();
        for (; itr != member_list.end(); ++itr)
        {
            CHARGUID char_guid = *itr;
            matching_group->matching_group_list.push_back(char_guid);

            //UserManager* const user_manager = UserManager::Instance();
            User* const user = user_manager->GetUserByCharGuid(char_guid);
            if (user == NULL) {
                continue;
            }
            //const ChaosZoneTeamKey team_key = i ? kBlue : kRed;
            const ChaosZoneTeam team_key = team_info->team_division;
            user->SetTeam(team_key);
        }
        team_battle_ground_manager->RemoveReservationTeam(team_info->master_user_guid);
    }

    return matching_group;
}

bool TeamBattleGroundMatchingSystem::RemoveMatchigGroup(const KEYTYPE group_key)
{
    TeamMatchingGroupMap::iterator itr = team_matching_groups_map_.find(group_key);
    if (itr == team_matching_groups_map_.end())
    {
        return false;
    }
    _FreeKey(group_key);
    team_matching_group_pool_->Free(itr->second);
    team_matching_groups_map_.erase(itr);

    return true;
}

bool TeamBattleGroundMatchingSystem::RemoveMemberMatchingGroup(const CHARGUID char_guid, const KEYTYPE group_key)
{
    TeamMatchingGroup* const matching_group = FindMatchingGroup(group_key);
    if (matching_group == NULL) {
        return false;
    }
    TeamBattleGroundMemberList& member_list = matching_group->matching_group_list;
    TeamBattleGroundMemberList::iterator itr = member_list.begin();
    for (; itr != member_list.end(); ++itr)
    {
        if ((*itr) == char_guid) {
            member_list.erase(itr);
            return true;
        }
    }
    return false;
}

bool TeamBattleGroundMatchingSystem::CreateInstanceRoom(const KEYTYPE group_key, const MAPCODE map_code)
{
    InstanceDungeon* instance_dungeon = NULL;
    const RC::eROOM_RESULT rt = g_ZoneManager.CreateInstanceDungeon( \
        0, NULL,
        map_code, (InstanceDungeon *&)instance_dungeon,
        INDUN_KIND_CHAOS_ZONE, FALSE);

    if (!instance_dungeon || rt != RC::RC_ROOM_SUCCESS)
    {
        return false;
    }

    instance_dungeon->SetRoomState(eROOMSTATE_GENERNAL);

    TeamMatchingGroup* const matching_group = FindMatchingGroup(group_key);
    if (matching_group == NULL)
        return false;
    matching_group->zone_key = instance_dungeon->GetKey();
    matching_group->map_code = instance_dungeon->GetMapCode();

    return true;
}

bool TeamBattleGroundMatchingSystem::OnCreateTeam(TeamMatchingGroup* const matching_group)
{
    TeamBattleGroundMemberList red_team;
    TeamBattleGroundMemberList blue_team;

    const UserManager* const user_manager = UserManager::Instance();
    TeamBattleGroundMemberList& member_list = matching_group->matching_group_list;
    TeamBattleGroundMemberList::const_iterator const_itr = member_list.begin();
    for (; const_itr != member_list.end(); ++const_itr)
    {
        const CHARGUID char_guid = *const_itr;
        User* const user = user_manager->GetUserByCharGuid(char_guid);
        if (user == NULL)
        {
            continue;
        }
        if (user->GetTeam() == kBlue)
        {
            blue_team.push_back(char_guid);
        }
        else
        {
            red_team.push_back(char_guid);
        }
        user->SetTRRoomKey(matching_group->zone_key);
        user->SetGroupKey(matching_group->group_key);
        TeamBattleGroundManager::Instance()->RemoveReservation(char_guid);
    }

    if (EnterTeam(red_team) == false || EnterTeam(blue_team) == false) {
        return false;
    }

    return true;
}

bool TeamBattleGroundMatchingSystem::EnterTeam(const TeamBattleGroundMemberList member_list)
{
    if (member_list.size() == 0)
        return false;

    CHARGUID master_guid = member_list.front();
    User* const master_user = UserManager::Instance()->GetUserByCharGuid(master_guid);
    if (master_user == NULL)
        return false;

    TeamMatchingGroup* const matching_group = FindMatchingGroup(master_user->GetGroupKey());
    if (matching_group == NULL)
        return false;
    
    MSG_AG_BATTLE_GROUND_ENTER_TEAM_CMD send_msg;
    send_msg.zone_key = matching_group->zone_key;
    send_msg.team_key = static_cast<ChaosZoneTeam>(master_user->GetTeam());

    TeamBattleGroundMemberList::const_iterator const_itr = member_list.begin();
    BYTE user_count = 0;
    for (; const_itr != member_list.end(); ++const_itr)
    {
        CHARGUID char_guid = *const_itr;
        const User* const user = UserManager::Instance()->GetUserByCharGuid(char_guid);
        const DWORD user_guid = user->GetUserGUID();
        send_msg.user_key_list[user_count] = user_guid;
        ++user_count;
    }

    send_msg.user_count = user_count;

    const ZoneInterface* const zone = g_ZoneManager.FindZone(matching_group->zone_key);
    if (zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[BattleGroundMatchingSystem::EnterTeam] Not exist zone [%d]!!"), matching_group->zone_key);
        return false;
    }

    const InstanceDungeon* const instance_dungeon = static_cast<const InstanceDungeon*>(zone);
    if (instance_dungeon == NULL)
    {
        return false;
    }

    ServerSession* const server_session = instance_dungeon->GetLinkedServer();
    if(server_session == NULL || server_session->GetServerType() != BATTLE_SERVER)
    {
        SUNLOG(eFULL_LOG,  "배틀서버가 아닌 세션을 연결하려한다.2\n");
        return false;
    }

    server_session->SendPacket(&send_msg, sizeof(send_msg));
    return true;
}



bool TeamBattleGroundMatchingSystem::SendMembersToBattleGround(TeamMatchingGroup* const matching_group)
{
    if (matching_group == NULL) {
        return false;
    }

    InstanceDungeon* const target_room = \
        static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(matching_group->zone_key));
    if (target_room == NULL) {
        return false;
    }

    const UserManager* const user_manager = UserManager::Instance();
    TeamBattleGroundMemberList& team_info = matching_group->matching_group_list;
    TeamBattleGroundMemberList::const_iterator const_itr = team_info.begin();
    for (; const_itr != team_info.end(); ++const_itr)
    {
        const CHARGUID char_guid = *const_itr;
        User* const user = user_manager->GetUserByCharGuid(char_guid);
        if (user == NULL)
        {
            continue;
        }

        ZoneInterface* current_zone = g_ZoneManager.FindZone(user->GetZoneKey());
        if (current_zone == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__ " : Not found zone : %d", user->GetUserGUID());
            continue;
        }
        user->BeginTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);

        CheckMoveZonePolicy* const check_move_zone_policy = 
            MoveZonePolicy<MZTF_STATIC_TO_ROOM_FOR_UCUSER>::AlignInfo(user,
            user->GetZoneType(), target_room->GetType(),
            user->GetZoneKey(),  target_room->GetKey());

        RoomArgument argument(eZONEPUBLIC_PUBLIC, NULL);

        RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(check_move_zone_policy, RoomArgument::GetEmpty());
    }

    return true;
}

bool TeamBattleGroundMatchingSystem::IsValidZoneKey(const KEYTYPE zone_key) const
{
    TeamMatchingGroupMap::const_iterator const_itr = team_matching_groups_map_.begin();
    for (; const_itr != team_matching_groups_map_.end(); ++const_itr)
    {
        TeamMatchingGroup* const matching_group = const_itr->second;
        if (matching_group && matching_group->zone_key == zone_key)
        {
            return true;
        }
    }
    return false;
}

KEYTYPE TeamBattleGroundMatchingSystem::GetZoneKey() const
{
    TeamMatchingGroupMap::const_iterator const_itr = team_matching_groups_map_.begin();
    for (; const_itr != team_matching_groups_map_.end(); ++const_itr)
    {
        TeamMatchingGroup* const matching_group = const_itr->second;
        if (matching_group)
            return matching_group->zone_key;
    }
    return 0;
}
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME