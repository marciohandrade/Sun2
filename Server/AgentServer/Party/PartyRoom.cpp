#include "stdafx.h"
#include "PartyRoom.h"
#include "BattlezoneMissionTreeParser.h"
#include "AgentParty/AgentParty.h"
#include "AgentParty/AgentPartyManager.h"
#include "MapInfoParser.h"
#include "Timer.h"
 
//_NA_20110630_BATTLE_ZONE_RENEWAL
PartyRoom::PartyRoom()
:pvp_mode_type_(ePVP_MODE_MAX),
party_room_state_(PartyRoomStateNormal)
{
    additional_info_.clear();
}

PartyRoom::~PartyRoom()
{

}

bool
PartyRoom::CheckPassword(const TCHAR* pass_word)const
{
    const BasePartyRoomInfo& base_info = party_room_info_.base_party_room_info;
    if (IsPrivateRoom() == true)
    {
        if (_tcsncmp(base_info.party_room_password_, pass_word, 
                     _countof(base_info.party_room_password_)) == 0)
        {
            return true;
        }

        return false;
    }    

    return true;
}

bool
PartyRoom::CheckLevel(LEVELTYPE user_level)const
{
    const BasePartyRoomInfo& base_info = party_room_info_.base_party_room_info;
    const sMAPINFO* const map_info = MapInfoParser::Instance()->FindMapInfo(base_info.map_code_);
    if (map_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found map info : %d", base_info.map_code_);
        return false;
    }

    if (map_info->minLV <= user_level && user_level <= map_info->maxLV)
    {
        return true;
    }
    
    return false;
}

bool
PartyRoom::IsPrivateRoom()const
{
    const BasePartyRoomInfo& base_info = party_room_info_.base_party_room_info;
    bool password_existence = (base_info.party_room_password_[0] != _T('\0'));
    return password_existence;
}

BYTE 
PartyRoom::SelectPVPTeam(BYTE number_of_team)
{
    //현재 팀은 두개의 팀만 존재 한다.(2011.09.22)
    //일단 10개의 팀까지 지원을 하게 로직을 구성하고 확장이 필요할 경우 수정
    const BYTE max_team = 10;
    if (number_of_team < 0 || number_of_team > max_team)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid data of team count : %d", number_of_team);
        return 0;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const BaseParty* const party = party_manager->FindParty(GetPartyKey());
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", GetPartyKey());
        return 0;
    }

    //[none] + [first team] + [second team] + ...
    BYTE team[max_team + 1] = {0,};

    UserManager* const user_manager = UserManager::Instance();

    typedef BaseParty::MEMBER_MAP MemberList;
    const MemberList& member_list = party->GetMemberList();
    MemberList::const_iterator begin_iterator = member_list.begin(),
                               end_iterator = member_list.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const _PARTY_MEMBER_INFO& member_info = begin_iterator->second;
        User* user = user_manager->GetUserByObjKey(member_info.m_dwMemberKey);
        ++(team[user->GetTeam()]);
    }

    BYTE select_team = 0;
    BYTE compair_value = static_cast<BYTE>(member_list.size());

    for (BYTE i = 1; i < number_of_team+1; ++i)
    {
        if (compair_value > team[i])
        {
            select_team = i;
            compair_value = team[i];
            ++team[i];
        }
    }

    return select_team;
}

void
PartyRoom::JoinRoom()
{
    if (party_room_info_.current_user_num_ >= party_room_info_.max_user_num_)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Invalid current user count : %d", party_room_info_.current_user_num_);
    }

    ++party_room_info_.current_user_num_;
}

void
PartyRoom::LeaveRoom()
{
    if (party_room_info_.current_user_num_ <= 0)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Invalid current user count : %d", party_room_info_.current_user_num_);
    }

    --party_room_info_.current_user_num_;
}

bool
PartyRoom::SetPlayerMaxCount(BYTE max_count)
{
    if (max_count > MAX_PARTYMEMBER_NUM)
    {
        return false;
    }

    party_room_info_.max_user_num_ = max_count;

    return true;
}

eZONETYPE 
PartyRoom::GetZoneType()const
{
    BattlezoneMissionTreeParser* const parser = BattlezoneMissionTreeParser::Instance();
    return parser->GetZoneType(party_room_info_.base_party_room_info.map_code_);
}

void 
PartyRoom::SetSuperRoomInfo(User* user)
{
    if (user == NULL)
    {
        return;
    }

    if (user->CanApplyPCBangBenefit(TRUE))
    {
        party_room_info_.base_party_room_info.special_type_ = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    else if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        party_room_info_.base_party_room_info.special_type_ = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }
    else
    {
        party_room_info_.base_party_room_info.special_type_ = eROOM_SPECIAL_NORMAL;
    }
}

void
PartyRoom::SetPartyRoomToStartState(BYTE count)
{
    _SetPartyRoomState(PartyRoomStateWaitToStart);
    party_room_wait_to_start_timer_.SetTimer(1000 * count);
}

bool
PartyRoom::IsEndRoomForWaitingToStart()
{
    if (_GetPartyRoomState() != PartyRoomStateWaitToStart)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Is not wait room : state : %d", 
               _GetPartyRoomState());
        return false;
    }

    if (party_room_wait_to_start_timer_.IsExpired())
    {
        _SetPartyRoomState(PartyRoomStateNormal);
        party_room_wait_to_start_timer_.DisableCheckTime();
        return true;
    }

    return false;
}

bool
PartyRoom::IsStartWaitingRoom()const
{
    return (_GetPartyRoomState() == PartyRoomStateWaitToStart) ? true : false;
}


bool
PartyRoom::CanJoinPartyRoomByMemberCount()const
{
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    WORD party_key = party_room_info_.link_party_key_;
    const AgentParty* party = static_cast<const AgentParty*>(party_manager->FindParty(party_key));
    if (party == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found party : key : %d",
            party_room_info_.link_party_key_
            );
        return false;
    }

    const BYTE current_member_count = party->GetMemberNum();
    const BYTE max_member_count = party_room_info_.max_user_num_;

    return (current_member_count < max_member_count) ? true : false; 
}

bool
PartyRoom::CanReturnJoinPartyRoomByMemberCount()const
{
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    WORD party_key = party_room_info_.link_party_key_;
    const AgentParty* party = static_cast<const AgentParty*>(party_manager->FindParty(party_key));
    if (party == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found party : key : %d",
            party_room_info_.link_party_key_
            );
        return false;
    }

    const BYTE current_member_count = party->GetActiveMemberCount();
    const BYTE max_member_count = party_room_info_.max_user_num_;

    return (current_member_count < max_member_count) ? true : false; 
}
