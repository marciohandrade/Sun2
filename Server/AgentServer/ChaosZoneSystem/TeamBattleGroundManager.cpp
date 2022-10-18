#include "StdAfx.h"

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include "ChaosZoneSystem/TeamBattleGroundManager.h"
#include <ChaosZoneSystem/TeamBattleGroundMatchingSystem.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>
#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentPartyOperator.h>


TeamBattleGroundManager::TeamBattleGroundManager()
{
    team_battle_ground_team_info_pool_ = NULL;
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    matching_system_ = NULL;
}

TeamBattleGroundManager::~TeamBattleGroundManager()
{

}

void TeamBattleGroundManager::Init()
{
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    matching_system_ = new TeamBattleGroundMatchingSystem();
    matching_system_->Init();

    team_battle_ground_team_info_pool_ = new TeamBattleGroundTeamInfoPool();
    team_battle_ground_team_info_pool_->Initialize(10);
}

void TeamBattleGroundManager::Release()
{
    team_battle_ground_team_info_pool_->Release();
    SAFE_DELETE(team_battle_ground_team_info_pool_);
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    matching_system_->Release();
    SAFE_DELETE(matching_system_);
}

void TeamBattleGroundManager::Update()
{
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    matching_system_->Update();
}

bool TeamBattleGroundManager::LeaveRoom(User* const user, const ZONEKEY zone_key)
{
    if (user == NULL)
    {
        return false;
    }

    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    TeamMatchingGroup* const matching_group = matching_system_->FindMatchingGroupByZoneKey(zone_key);

    if (matching_group == NULL) {
        return false;
    }

    if (matching_system_->RemoveMemberMatchingGroup(
        user->GetCharGUID(user->GetSelectedCharIndex()), matching_group->group_key) == false) {
            return false;
    }

    if (matching_group->matching_group_list.size() <= 0)
    {
        if (matching_system_->RemoveMatchigGroup(matching_group->group_key) == false) {
            return false;
        }
    }

    // 그룹키 및 팀 초기화..
    user->SetGroupKey(0);
    user->SetTeam(kChaosZoneTeamNone);
    return true;
}

bool TeamBattleGroundManager::InsertReservationTeam(const CHARGUID char_guid, 
                                                    const ChaosZoneTeam team_division, 
                                                    const MAPCODE map_code)
{
    UserManager* const user_manager = UserManager::Instance();
    User* const master_user = user_manager->GetUserByCharGuid(char_guid);
    if (master_user == NULL)
    {
        return false;
    }

    TeamBattleGroundTeamInfoList::iterator itr = team_battle_ground_team_info_list_.begin();
    for (; itr != team_battle_ground_team_info_list_.end(); ++itr)
    {
        TeamBattleGroundTeamInfo* team_info = *itr;
        if (team_info == NULL)
        {
            continue;
        }
        if (team_info->master_user_guid == char_guid)
        {
            return false;
        }
    }

    const AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const BaseParty* const base_party = \
        party_manager->FindParty(master_user->GetPartyState().GetPartyKey());
    if (base_party == NULL)
    {
        return false;
    }

    TeamBattleGroundTeamInfo* team_info = \
        static_cast<TeamBattleGroundTeamInfo*>(team_battle_ground_team_info_pool_->Alloc());
    team_info->master_user_guid = char_guid;
    team_info->team_division = team_division;
    team_info->map_code = map_code;
    
    const BaseParty::MEMBER_MAP& member_list = base_party->GetMemberList();
    BaseParty::MEMBER_MAP::const_iterator const_member_itr = member_list.begin();
    for (; const_member_itr != member_list.end(); ++const_member_itr)
    {
        _PARTY_MEMBER_INFO member_info = const_member_itr->second;
        User* const user = user_manager->FindUserForCharName(member_info.m_szCharName);
        if (user == NULL)
        {
            continue;
        }
        user->SetTeam(team_division);
        team_info->member_list.push_back(user->GetCharGUID(user->GetSelectedCharIndex()));
    }

    team_battle_ground_team_info_list_.push_back(team_info);

    return true;
}

bool TeamBattleGroundManager::RemoveReservationTeam(const CHARGUID char_guid)
{
    TeamBattleGroundTeamInfoList::iterator itr = team_battle_ground_team_info_list_.begin();
    for (; itr != team_battle_ground_team_info_list_.end(); ++itr)
    {
        TeamBattleGroundTeamInfo* team_info = *itr;
        if (team_info == NULL)
        {
            continue;
        }
        if (team_info->master_user_guid == char_guid)
        {
            team_battle_ground_team_info_pool_->Free(team_info);
            team_battle_ground_team_info_list_.erase(itr);
            return true;
        }
    }
    return false;
}

bool TeamBattleGroundManager::InsertReservation(const CHARGUID char_guid, const ChaosZoneTeam team_division, const MAPCODE map_code)
{
    UserManager* const user_manager = UserManager::Instance();
    User* const master_user = user_manager->GetUserByCharGuid(char_guid);
    if (master_user == NULL)
    {
        return false;
    }

    const AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const BaseParty* const base_party = \
        party_manager->FindParty(master_user->GetPartyState().GetPartyKey());
    if (base_party)
    {
        return false;
    }
    if (team_division == kRed)
        red_team_battle_ground_member_list_.push_back(char_guid);
    else if(team_division == kBlue)
        blue_team_battle_ground_member_list_.push_back(char_guid);
    return true;
}

bool TeamBattleGroundManager::RemoveReservation(const CHARGUID char_guid)
{
    TeamBattleGroundMemberList::iterator red_itr = red_team_battle_ground_member_list_.begin();
    TeamBattleGroundMemberList::iterator blue_itr = blue_team_battle_ground_member_list_.begin();

    for (; red_itr != red_team_battle_ground_member_list_.end(); ++red_itr)
    {
        if (char_guid == *red_itr)
        {
            red_team_battle_ground_member_list_.erase(red_itr);
            return true;
        }
    }

    for (; blue_itr != blue_team_battle_ground_member_list_.end(); ++blue_itr)
    {
        if (char_guid == *blue_itr)
        {
            blue_team_battle_ground_member_list_.erase(blue_itr);
            return true;
        }
    }
    return false;
}

bool TeamBattleGroundManager::CreateTeam(const MAPCODE map_code)
{
    if (red_team_battle_ground_member_list_.size() == 0 ||
        blue_team_battle_ground_member_list_.size() == 0)
    {
        return false;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();

    ;{
        //red_team << team_key : 1
        BaseParty* red_base_party = NULL;
        CHARGUID red_char_guid = 0;
        User* red_user = NULL;
        TeamBattleGroundMemberList::const_iterator red_itr = red_team_battle_ground_member_list_.begin();
        while (red_itr != red_team_battle_ground_member_list_.end())
        {
            TeamBattleGroundMemberList::const_iterator cur_itr = red_itr++; //!!!!!
            red_char_guid = *cur_itr;
            red_user = UserManager::Instance()->GetUserByCharGuid(red_char_guid);

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
            }

            if (red_base_party) {
                party_manager->JoinParty(red_base_party->GetPartyKey(), red_user);
            }
        }
        InsertReservationTeam(red_char_guid, kRed, map_code);
    };

    ;{
        //blue_team << team_key : 2
        BaseParty* blue_base_party = NULL;
        CHARGUID blue_char_guid = 0;
        User* blue_user = NULL;
        TeamBattleGroundMemberList::const_iterator blue_itr = blue_team_battle_ground_member_list_.begin();
        while (blue_itr != blue_team_battle_ground_member_list_.end())
        {
            TeamBattleGroundMemberList::const_iterator cur_itr = blue_itr++; //!!!!!
            blue_char_guid = *cur_itr;

            blue_user = UserManager::Instance()->GetUserByCharGuid(blue_char_guid);
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
            }

            if (blue_base_party) {
                party_manager->JoinParty(blue_base_party->GetPartyKey(), blue_user);
            }
        }
        InsertReservationTeam(blue_char_guid, kBlue, map_code);
    };

    return true;
}

bool TeamBattleGroundManager::RemoveReservationTeamAll()
{
    TeamBattleGroundTeamInfoList::iterator itr = team_battle_ground_team_info_list_.begin();
    TeamBattleGroundTeamInfoList::iterator erase_itr;
    for (; itr != team_battle_ground_team_info_list_.end();)
    {
        TeamBattleGroundTeamInfo* team_info = *itr;
        if (team_info == NULL)
        {
            ++itr;
            continue;
        }

        erase_itr = itr++;
        team_battle_ground_team_info_pool_->Free(team_info);
        team_battle_ground_team_info_list_.erase(erase_itr);
    }
    return true;
}

void TeamBattleGroundManager::SendPacketAllMember(const TeamBattleGroundMemberList* member_list, MSG_BASE* msg, WORD size)
{
    UserManager* const user_manager = UserManager::Instance();
    TeamBattleGroundMemberList::const_iterator const_itr = member_list->begin();
    for (; const_itr != member_list->end(); ++const_itr)
    {
        CHARGUID char_guid = *const_itr;
        User* const user = user_manager->GetUserByCharGuid(char_guid);
        if (user == NULL)
        {
            continue;
        }
        user->SendPacket(msg, size);
    }
}

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
bool TeamBattleGroundManager::CheckMatching( const MAPCODE map_code )
{
    return matching_system_->CheckMatching(map_code);
}

bool TeamBattleGroundManager::IsValidZoneKey( const KEYTYPE zone_key ) const
{
    return matching_system_->IsValidZoneKey(zone_key);
}

KEYTYPE TeamBattleGroundManager::GetZoneKey() const
{
    return matching_system_->GetZoneKey();
}

#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

