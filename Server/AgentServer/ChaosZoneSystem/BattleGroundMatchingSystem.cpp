#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <../Common/PacketStruct_AG_ChaosZoneSystem.h>
#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>
#include <AgentParty/AgentPartyManager.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>
#include <Function/AgentServer.Timer.WorkLoad.h>
#include <BattleGroundInfoParser.h>
#include <../ProgramCommon/PacketStruct_CG.h>
#include <PacketHandler/Handler_FromClient.h>
#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#include <ServerSessions/ServerSessionManager.h>
#include <AgentParty/AgentPartyOperator.h>


void BattleGroundMatchingSystem::Init()
{
    matching_group_pool_ = new MatchingGroupPool();
    matching_group_pool_->Initialize(60);
    update_timer_.SetTimer(1000);
}

void BattleGroundMatchingSystem::Release()
{
    MatchingGroupMap::const_iterator const_itr = matching_groups_map_.begin();
    for (; const_itr != matching_groups_map_.end(); ++const_itr)
    {
        matching_group_pool_->Free(const_itr->second);
    }
    matching_groups_map_.clear();

    SAFE_DELETE(matching_group_pool_);
}

void BattleGroundMatchingSystem::Update()
{
    if (update_timer_.IsExpired() == false) {
        return;
    }
    
    CTime current_time = util::TimeSync::_time64(NULL);
    const BattleGroundInfoParser* const battle_ground_Info_parser = BattleGroundInfoParser::Instance();

    MatchingGroupMap::iterator itr = matching_groups_map_.begin();
    while (itr != matching_groups_map_.end())
    {
        // 매칭그룹
        MatchingGroupMap::iterator cur_itr = itr++; //!!!!!
        MatchingGroup* const matching_group = cur_itr->second;
        if (matching_group == NULL) {
            matching_groups_map_.erase(cur_itr);
            continue;
        }
        
        // 스크립트 데이터
        const BattleGroundInfo* battle_ground_Info = \
            battle_ground_Info_parser->FindData(matching_group->map_code);
        if (battle_ground_Info == NULL)
        {
            continue;
        }

        // 전장 출발 시간이 경과하면 출발..
        if (matching_group->matching_state == kCreatedRoom &&
            current_time >= matching_group->matching_time + 
            CTimeSpan(0, 0, 0, battle_ground_Info->start_time)) // script..
        {
            if (OnTeamMatching(matching_group) == false ||              // 2차 팀 매칭
                OnCreateParty(matching_group) == false ||               // 팀별로 파티구성
                SendMembersToBattleGround(matching_group) == false)     // 유저 이동
            {
                // 매칭 취소
                MatchingGroupCancel(matching_group->group_key);
                continue;
            }

            matching_group->matching_state = kStartBattleGround;
        }
    }
}

bool BattleGroundMatchingSystem::CheckMatching(MAPCODE map_code)
{
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    const BattleGroundMemberList* const member_list = \
        battle_ground_manager->FindStandbyGroupMemberList(map_code);
    
    if (member_list == NULL)
    {
        return false;
    }

    if (CanCreateMatchingGroup(map_code) == false)
    {
        return false;
    }

    if (CreateMatchingGroup(map_code) == false)
    {
        // 메세지 전달.. 매칭그룹 만들기 실패..
        return false;
    }
    return true;
}

bool BattleGroundMatchingSystem::CanCreateMatchingGroup(const MAPCODE map_code)
{
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    const BattleGroundMemberList* const member_list = \
        battle_ground_manager->FindStandbyGroupMemberList(map_code);

    const BattleGroundInfoParser* const battle_ground_info_parser = \
                                                                  BattleGroundInfoParser::Instance();

    const BattleGroundInfo* const battle_ground_Info = battle_ground_info_parser->FindData(map_code);

    int extract_list = static_cast<int>(battle_ground_Info->team_max);
    if ((int)member_list->size() < extract_list * 2)   //script..
    {
        return false;
    }

    return true;
}

bool BattleGroundMatchingSystem::CreateMatchingGroup(const MAPCODE map_code)
{
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    BattleGroundMemberList* const member_list = 
        battle_ground_manager->FindStandbyGroupMemberList(map_code);
    if (member_list == NULL)
    {
        return false;
    }

    // 1차 매칭
    MatchingGroup* const matching_group = OnMatching(map_code, member_list);
    if (matching_group == NULL)
    {
        return false;
    }

    //그룹키 셋팅
    const KEYTYPE group_key = _AllocKey();
    matching_group->group_key = group_key;

    //만들어진 시간 설정
    matching_group->matching_time = util::TimeSync::_time64(NULL);
    matching_group->matching_state = kWaitCreateRoom;
    matching_groups_map_.insert( \
        MatchingGroupMap::value_type(group_key, matching_group));

    //방생성 요청..
    if (CreateInstanceRoom(group_key, map_code) == false) 
    {
        // 매칭 취소..
        MatchingGroupCancel(group_key);
        return false;
    }

    //BRD.. 매칭그룹에 속한 멤버들에게 1차 매칭이 되었다고 알려줌.
    MSG_CG_BATTLE_GROUND_MATCHING_RESULT_BRD brd_msg;
    {
        brd_msg.result_type = 0;
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        brd_msg.map_code = matching_group->map_code;
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    }
    battle_ground_manager->SendPacketAllMember(&matching_group->matching_group_list, 
                                               &brd_msg, sizeof(brd_msg));
    
    return true;
}

MatchingGroup* BattleGroundMatchingSystem::OnMatching(const MAPCODE map_code, 
                                                      const BattleGroundMemberList* member_list)
{
    // 1차 매칭..
    if (member_list == NULL)
    {
        return NULL;
    }
    
    //인원수가 과반이 안넘으면 매칭 실패
    const BattleGroundInfoParser* const battle_ground_info_parser = \
        BattleGroundInfoParser::Instance();
    const BattleGroundInfo* const battle_ground_Info = battle_ground_info_parser->FindData(map_code);
    int extract_list = static_cast<int>(battle_ground_Info->team_max);
    if ((extract_list%2) != 0)
        extract_list += 1;
    
    if ((int)member_list->size() < extract_list)
    {
        return NULL;
    }

    //매칭 그룹 선발
    MatchingGroup* matching_group = 
        static_cast<MatchingGroup*>(matching_group_pool_->Alloc());
    if (matching_group == NULL) {
        return NULL;
    }

    // 대기열 인원이 최대인원이 아니고 홀수 일경우..한명을 제외시킨다.
    int memeber_size = static_cast<int>(member_list->size());
    if (memeber_size%2 != 0)
    {
        memeber_size -= 1;
    }

    for (int i = 0; i < memeber_size; ++i) //script..
    {
        BattleGroundMemberInfo* const member_info = *(member_list->begin());
        if (member_info == NULL)
        {
            continue;
        }
        if (BattleGroundManager::Instance()->IsValidReservationMember(member_info->user_guid) == false)
        {
            continue;
        }
        member_info->member_state = kStandbyPriority;
        matching_group->matching_group_list.push_back(member_info);
        BattleGroundManager::Instance()->RemoveMemberStandbyGroup(member_info);
    }

    return matching_group;
}

bool BattleGroundMatchingSystem::RemoveMatchigGroup(const KEYTYPE group_key)
{
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();

    MatchingGroupMap::iterator itr = matching_groups_map_.find(group_key);
    MatchingGroup* const matching_group = itr->second;
    
    BattleGroundMemberList::const_iterator list_itr = matching_group->matching_group_list.begin();
    for (;list_itr != matching_group->matching_group_list.end(); ++list_itr)
    {
        BattleGroundMemberInfo* const member_info = *list_itr;
        battle_ground_manager->RemoveReservationMember(member_info->user_guid);
    }

    _FreeKey(group_key);
    matching_group_pool_->Free(itr->second);
    matching_groups_map_.erase(itr);
    return true;
}



MatchingGroup* BattleGroundMatchingSystem::FindMatchingGroup(const KEYTYPE group_key)
{
    MatchingGroupMap::const_iterator const_itr = matching_groups_map_.find(group_key);
    if (const_itr == matching_groups_map_.end())
    {
        return NULL;
    }
    return const_itr->second;
}

MatchingGroup* BattleGroundMatchingSystem::FindMatchingGroupByZoneKey(const KEYTYPE zone_key)
{
    MatchingGroupMap::const_iterator const_itr = matching_groups_map_.begin();
    for (; const_itr != matching_groups_map_.end(); ++const_itr)
    {
        MatchingGroup* const matching_group = const_itr->second;
        if (matching_group->zone_key != zone_key)
        {
            continue;
        }
        return matching_group;
    }

    return NULL;
}

bool BattleGroundMatchingSystem::CreateInstanceRoom(const KEYTYPE group_key, const MAPCODE map_code)
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

    MatchingGroup* const matching_group = FindMatchingGroup(group_key);
    matching_group->matching_state = kCreatedRoom;
    matching_group->zone_key = instance_dungeon->GetKey();
    matching_group->map_code = instance_dungeon->GetMapCode();

    return true;
}


class EquipScoreGreater
{
public:
    bool operator () ( BattleGroundMemberInfo *pLeft, BattleGroundMemberInfo *pRight ) const
    {
        return pLeft->equip_item_score > pRight->equip_item_score;
    }
};

namespace SecondTeamMatching
{
    bool TeamMatchingRule( \
        const BattleGroundMemberInfo* player_left, 
        const BattleGroundMemberInfo* player_right)
    {
        if ( player_left->is_healer && !player_right->is_healer)
            return false;
        if (!player_left->is_healer &&  player_right->is_healer)
            return true;

        if ( player_left->is_healer && player_right->is_healer)
            return (player_left->equip_item_score < player_right->equip_item_score);

        return (player_left->equip_item_score > player_right->equip_item_score);
    }

    void TeamSetUp(BattleGroundMemberInfo* member_, MatchingGroup* const matching_group, \
        ChaosZoneTeam team_) 
    {
        member_->team_division = team_;
        member_->group_key = matching_group->group_key;

        UserManager* const user_manager = UserManager::Instance();
        User* const user = user_manager->GetUser(member_->user_guid);
        if (user == NULL) {
            return;
        }
        user->SetTRRoomKey(matching_group->zone_key);
        user->SetGroupKey(matching_group->group_key);
        user->SetTeam(member_->team_division);

		// 상태변경..
        ;{
            user->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);
            MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD cmd_msg;
            cmd_msg.behave_sate = PLAYER_BEHAVE_IDLE_STATE;
            user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
        };
    }
};

bool BattleGroundMatchingSystem::OnTeamMatching(MatchingGroup* const matching_group)
{
    if (matching_group == NULL) {
        return false;
    }

    const BattleGroundMemberList& matching_group_list = matching_group->matching_group_list;

    // 2차 매칭
    if (matching_group_list.size()%2 != 0) {
        return false;
    }

    BattleGroundMemberList member_info_list = matching_group_list; // copy
    std::stable_sort(member_info_list.begin(), member_info_list.end(), &SecondTeamMatching::TeamMatchingRule);

    BattleGroundMemberList::const_iterator it = member_info_list.begin();
    while(it != member_info_list.end())
    {
        std::size_t flag = random(0,1);
        SecondTeamMatching::TeamSetUp((*it++), matching_group, (flag ? kRed  : kBlue));
        SecondTeamMatching::TeamSetUp((*it++), matching_group, (flag ? kBlue : kRed ));
    }


    return true;
}

bool BattleGroundMatchingSystem::SendMembersToBattleGround(MatchingGroup* const matching_group)
{
    if (matching_group == NULL) {
        return false;
    }

    InstanceDungeon* const target_room = \
        static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(matching_group->zone_key));
    if (target_room == NULL) {
        return false;
    }

    BattleGroundMemberList::const_iterator const_itr = matching_group->matching_group_list.begin();
    for (; const_itr != matching_group->matching_group_list.end(); ++const_itr)
    {
        BattleGroundMemberInfo* const find_member = *const_itr;
        User* const user = UserManager::Instance()->GetUser(find_member->user_guid);
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
        //if (rt != RC::RC_ROOM_SUCCESS)
        //{
        //    return false;
        //}
    }
    
    return true;
}

bool BattleGroundMatchingSystem::OnCreateParty(MatchingGroup* const matching_group)
{
    if (matching_group == NULL)
    {
        return false;
    }

    ZoneInterface* const zone_interface = g_ZoneManager.FindZone(matching_group->zone_key);
    if (zone_interface == NULL)
    {
        return false;
    }
    InstanceDungeon* const instance_dungeon = static_cast<InstanceDungeon*>(zone_interface);

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    BattleGroundMemberList red_team;
    BattleGroundMemberList blue_team;

    BattleGroundMemberList::const_iterator const_itr = matching_group->matching_group_list.begin();
    for (; const_itr != matching_group->matching_group_list.end(); ++const_itr)
    {
        BattleGroundMemberInfo* member = *const_itr;
        if (member->team_division == kBlue)
        {
            blue_team.push_back(member);
        }
        else
        {
            red_team.push_back(member);
        }
    }

    std::stable_sort(red_team.begin(), red_team.end(), EquipScoreGreater());
    std::stable_sort(blue_team.begin(), blue_team.end(), EquipScoreGreater());

    // 두팀 중 한명도 없는 팀이 존재한다면.. 전장으로 보내지 않는다..
    if (red_team.size() == 0 || blue_team.size() == 0) {
        return false;
    }

    // GameServer에 팀 정보 전송.
    if (EnterTeam(red_team) == false || EnterTeam(blue_team) == false) {
        return false;
    }

    ;{
        //red_team << team_key : 1
        BaseParty* red_base_party = NULL;
        BattleGroundMemberInfo* red_member = NULL;
        User* red_user = NULL;
        BattleGroundMemberList::const_iterator red_itr = red_team.begin();
        for (; red_itr != red_team.end(); ++red_itr)
        {
            red_member = *red_itr;
            red_user = UserManager::Instance()->GetUser(red_member->user_guid);

            if (red_user == NULL) {
                continue;
            }

            if (red_base_party == NULL)
            {
                red_base_party = \
                    party_manager->CreateParty(red_user->GetPlayerKey(), ePARTY_INSTANCE);
                if (red_base_party == NULL) {
                    continue;
                }

                static JoinPartyOperator Opr;
                Opr.Init(eUSER_OPERATOR_JOINPARTY, red_base_party->GetPartyKey());
                Opr(red_user);
                instance_dungeon->LinkParty(red_base_party->GetPartyKey());
            }

            if (red_base_party) {
                party_manager->JoinParty(red_base_party->GetPartyKey(), red_user);
            }
        }
    };

    ;{
        //blue_team << team_key : 2
        BaseParty* blue_base_party = NULL;
        BattleGroundMemberInfo* blue_member = NULL;
        User* blue_user = NULL;
        BattleGroundMemberList::const_iterator blue_itr = blue_team.begin();
        for (; blue_itr != blue_team.end(); ++blue_itr)
        {
            blue_member = *blue_itr;

            blue_user = UserManager::Instance()->GetUser(blue_member->user_guid);
            if (blue_user == NULL)
            {
                continue;
            }

            if (blue_base_party == NULL)
            {
                blue_base_party = \
                    party_manager->CreateParty(blue_user->GetPlayerKey(), ePARTY_INSTANCE);
                if (blue_base_party == NULL)
                {
                    continue;
                }
                static JoinPartyOperator Opr;
                Opr.Init(eUSER_OPERATOR_JOINPARTY, blue_base_party->GetPartyKey());
                Opr(blue_user);
                instance_dungeon->LinkParty(blue_base_party->GetPartyKey());
            }

            if (blue_base_party)
                party_manager->JoinParty(blue_base_party->GetPartyKey(), blue_user);
        }
    };

    return true;
}

bool BattleGroundMatchingSystem::EnterTeam(const BattleGroundMemberList member_list)
{
    BattleGroundMemberList::const_iterator const_itr = member_list.begin();
    if (const_itr == member_list.end())
    {
        return false;
    }

    BattleGroundMemberInfo* const master_member = *const_itr;
    if (master_member == NULL)
    {
        return false;
    }

    MatchingGroup* const matching_group = FindMatchingGroup(master_member->group_key);
    if (matching_group == NULL)
    {
        return false;
    }

    const BattleGroundInfoParser* const battle_ground_info_parser = \
                                                                 BattleGroundInfoParser::Instance();
    const BattleGroundInfo* const battle_ground_Info = \
                                      battle_ground_info_parser->FindData(matching_group->map_code);

    const ZoneInterface* const zone = g_ZoneManager.FindZone(matching_group->zone_key);
    if (zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[BattleGroundMatchingSystem::EnterTeam] Not exist zone [%d]!!"), matching_group->zone_key);
        return false;
    }

    MSG_AG_BATTLE_GROUND_ENTER_TEAM_CMD send_msg;
    send_msg.zone_key = matching_group->zone_key;
    send_msg.team_key = (*const_itr)->team_division;

    BYTE user_count = 0;
    for (; const_itr != member_list.end(); ++const_itr)
    {
        BattleGroundMemberInfo* member = *const_itr;
        send_msg.user_key_list[user_count] = member->user_guid;
        ++user_count;
    }
    if (user_count > battle_ground_Info->team_max)
    {
        //팀 최대인원수를 넘겼을 경우
        SUNLOG(eCRITICAL_LOG, TEXT("[EnterTeam] BattleGroundTeamCount[%d]!!"), user_count);
    }

    send_msg.user_count = user_count;

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

bool BattleGroundMatchingSystem::MatchingGroupCancel(const KEYTYPE group_key)
{
    MatchingGroup* const matching_group = FindMatchingGroup(group_key);
    if (matching_group == NULL)
    {
        return false;
    }
    
    // 매칭된 멤버들을 대기열로 돌려보낸다.
    if (ReturnUserInsertStandbyGroupList(matching_group) == false)
    {
        return false;
    }
    // 매칭그룹 제거
    if (RemoveMatchigGroup(group_key) == false)
    {
        return false;
    }
    return true;
}

bool BattleGroundMatchingSystem::ReturnUserInsertStandbyGroupList(MatchingGroup* const matching_group)
{
    BattleGroundMemberList::const_iterator const_itr = matching_group->matching_group_list.begin();
    for (; const_itr != matching_group->matching_group_list.end(); ++const_itr)
    {
        BattleGroundMemberInfo* member = *const_itr;
        if (BattleGroundManager::Instance()->ReturnUserInsertStandbyGroupList(member) == false)
        {
            return false;
        }
        // 상태변경..
        ;{
            MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD cmd_msg;
            cmd_msg.behave_sate = PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE;
            User* const user = UserManager::Instance()->GetUser(member->user_guid);
            if (user)
                user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
        };
    }
    return true;
}

bool BattleGroundMatchingSystem::RemoveMemberMatchingGroup(const BattleGroundMemberInfo* member)
{
    if (member == NULL)
    {
        return false;
    }

    MatchingGroup* const matching_group = FindMatchingGroup(member->group_key);
    if (matching_group == NULL) 
    {
        return false;
    }

    BattleGroundMemberList::iterator itr = matching_group->matching_group_list.begin();
    for (;itr != matching_group->matching_group_list.end(); ++itr)
    {
        if ((*itr)->user_guid == member->user_guid)
        {
            matching_group->matching_group_list.erase(itr);
            return true;
        }
    }

    return false;
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND