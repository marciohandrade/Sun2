#include "stdafx.h"
#include <UserSessions/UserManager.h>
#include <AgentParty/AgentParty.h>
#include <Zone/ZoneManager.h>
#include <Zone/RoomInterface.h>

#include "ClosingUserList.h"


ClosingUserList::ClosingUserList() 
: party_key_(0)
, closing_user_list_(NULL) 
{

}

ClosingUserList::~ClosingUserList()
{

}

void
ClosingUserList::Init(WORD party_key)
{
    party_key_ = party_key;
}

bool
ClosingUserList::UpdateChar(ClosingUserInfo& closing_user_info)
{
    if (closing_user_list_.empty())
    {
        return false;
    }

    ClosingUserInfo* find_user = _FindEditableClosingUser(closing_user_info.char_guid_);
    if (find_user == NULL)
    {
        return false;
    }
#ifdef _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION
    find_user->return_pos_ = closing_user_info.return_pos_;
    find_user->return_field_code_ = closing_user_info.return_field_code_;
    find_user->return_zone_type_ = closing_user_info.return_zone_type_;
    find_user->return_zone_key_ = closing_user_info.return_zone_key_;
#else
    *(find_user) = closing_user_info;
#endif //_NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION

    return true;
}

void
ClosingUserList::AddClosingUser(const DWORD char_guid, 
                                const TCHAR* char_name,
                                PartyReturnInfo& return_info,
                                const _PARTY_MEMBER_INFO& member_info)
{
    if (FindClosingUser(char_guid) != NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Aleady exist closing user : exist char name : %s",
            char_name
            );
        return;
    }

    ClosingUserInfo closing_user(char_guid);
    closing_user.SetCharName(char_name);
    closing_user.return_pos_ = return_info.ReturnPos;
    closing_user.return_field_code_ = return_info.ReturnFieldCode;
    closing_user.return_zone_type_ = return_info.ReturnZoneType;
    closing_user.return_zone_key_ = return_info.ReturnZoneKey;
    return_info.Init();

    closing_user.member_info_ = member_info;
    closing_user.member_info_.m_isReady = false;

    closing_user_list_.push_back(closing_user);
}

void
ClosingUserList::RemoveClosingUser(const DWORD char_guid)
{
    UserList::iterator begin_iterator = closing_user_list_.begin(),
                       end_iterator = closing_user_list_.end();

    while (begin_iterator != end_iterator)
    {
        ClosingUserInfo& user_info = *(begin_iterator);
        if (char_guid == user_info.char_guid_)
        {
            begin_iterator = closing_user_list_.erase(begin_iterator);
            return;
        }
        ++begin_iterator;
    }
}

void
ClosingUserList::RemoveClosingUser(const TCHAR* char_name)
{
    UserList::iterator begin_iterator = closing_user_list_.begin(),
        end_iterator = closing_user_list_.end();

    while (begin_iterator != end_iterator)
    {
        ClosingUserInfo& user_info = *(begin_iterator);
        if (_tcsncmp(char_name, user_info.GetCharName(), MAX_CHARNAME_LENGTH + 1) == 0)
        {
            begin_iterator = closing_user_list_.erase(begin_iterator);
            return;
        }
        ++begin_iterator;
    }
}

const ClosingUserInfo*
ClosingUserList::FindClosingUser(const DWORD char_guid)
{
    UserList::iterator begin_iterator = closing_user_list_.begin(),
                       end_iterator = closing_user_list_.end();

    for (;begin_iterator != end_iterator; ++begin_iterator)
    {
        ClosingUserInfo& user_info = *(begin_iterator);
        if (char_guid == user_info.char_guid_)
        {
            return &(user_info);
        }
    }

    return NULL;
}

const ClosingUserInfo*
ClosingUserList::FindClosingUser(const TCHAR* char_name)
{
    UserList::iterator begin_iterator = closing_user_list_.begin(),
                       end_iterator = closing_user_list_.end();

    for (;begin_iterator != end_iterator; ++begin_iterator)
    {
        ClosingUserInfo& user_info = *(begin_iterator);
        if (_tcsncmp(user_info.GetCharName(), char_name, MAX_CHARNAME_LENGTH + 1) == 0)
        {
            return &(user_info);
        }
    }

    return NULL;
}

ClosingUserInfo*
ClosingUserList::_FindEditableClosingUser(const DWORD char_guid)
{
    UserList::iterator begin_iterator = closing_user_list_.begin(),
                       end_iterator = closing_user_list_.end();

    for (;begin_iterator != end_iterator; ++begin_iterator)
    {
        ClosingUserInfo& user_info = *(begin_iterator);
        if (char_guid == user_info.char_guid_)
        {
            return &(user_info);
        }
    }

    return NULL;
}

BYTE 
ClosingUserList::GetClosingUserCount() const
{ 
    return static_cast<BYTE>(closing_user_list_.size());
}

void 
ClosingUserList::AddClosingUserInfoToMemberList(PARTY_MEMBER_TOTAL_INFO& all_member_list)
{
    UserList::iterator begin_iterator = closing_user_list_.begin(),
                       end_iterator = closing_user_list_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        if (all_member_list.m_Count >= MAX_PARTYMEMBER_NUM) 
        {
            SUNLOG(eCRITICAL_LOG,
                __FUNCTION__" : Member count is over : count : %d",
                all_member_list.m_Count);
            break;
        }

        const ClosingUserInfo& closing_user_info = *(begin_iterator);
        _PARTY_MEMBER_INFO member_info = closing_user_info.member_info_;
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        member_info.m_isConnected = false;
#endif
        all_member_list.m_Slot[all_member_list.m_Count] = member_info;
        ++all_member_list.m_Count;
    }
}

void
ClosingUserList::Update(AgentParty* const party)
{
    UserManager* const user_manager = UserManager::Instance();
    UserList::iterator begin_iterator = closing_user_list_.begin(),
                       end_iterator = closing_user_list_.end();

    while(begin_iterator != end_iterator)
    {
        ClosingUserInfo& closing_user_info = *(begin_iterator);
        User* closing_user = user_manager->GetUserByCharGuid(closing_user_info.char_guid_);

        if(closing_user_info.time_out_.IsExpired())
        {   
            //비정상 접속 종료 유저가 일정 시간이 지나도 접속 않을 경우에 대한 처리
            if (closing_user)
            {
                closing_user->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);
            }

            //비정상 종료된 유저 목록 삭제 요청(파티 맴버 전원 에게)
            MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_CMD cmd_msg;
            cmd_msg.CopyFrom(closing_user_info.GetCharName());
            party->SendPacketAll(&cmd_msg, cmd_msg.GetSize());

            //비정상 종료 유저 정보 삭제로 인한 몬스터및 룸의 설정 변경
            const _PARTY_MEMBER_INFO* member_info = party->GetHeadUser();
            if (member_info == NULL)
            {
                begin_iterator = closing_user_list_.erase(begin_iterator);
                continue;
            }

            User* current_zone_user = user_manager->GetUserByObjKey(member_info->m_dwMemberKey);
            KEYTYPE zone_key = 0;
            if (current_zone_user && 
                (zone_key = current_zone_user->GetZoneKey()) && 
                (zone_key == closing_user_info.return_zone_key_))
            {
                ZoneInterface* current_zone = g_ZoneManager.FindZone(zone_key);
                if (current_zone != NULL&&
                    (PolicymentForRoom::MonsterAbilityChangableRoomsBit & (1 << current_zone->GetType())))
                {
                    RoomInterface* current_room = static_cast<RoomInterface*>(current_zone);
                    BYTE number_of_players = current_room->GetNumberOfExpectedPlayers();
                    if (number_of_players)
                    {
                        current_room->SetNumberOfExpectedPlayers(--number_of_players);
                        g_ZoneManager.ChangeBattlePlayersCount(current_room, number_of_players);
                    }
                }
            }

            begin_iterator = closing_user_list_.erase(begin_iterator);
        }
        else
        {
            if (closing_user && closing_user_info.CanSendRequestReturnParty())
            {
                //비정상 접속 종료후 일정 시간 이내에 재접속을 한 유저에 대한 처리
                User* master_user = user_manager->GetUserByObjKey(party->GetMasterKey());
                if (master_user)
                {
                    if (closing_user->GetPartyState().GetPartyKey() == 0)
                    {
                        //재접속 한 유저가 다른 파티에 가입해있지 않은 경우에 재 가입 요청을 한다.
                        closing_user->SetBehaveState(PLAYER_BEHAVE_UNEXPECTED_WAITING);
                        MSG_CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD cmd_msg;
                        cmd_msg.PARTY_KEY() = master_user->GetPartyState().GetPartyKey();
                        closing_user->SendPacket(&cmd_msg, sizeof(cmd_msg));
                    }
                    else
                    {
                        closing_user_info.time_out_.Instance();
                    }
                }
            }
            ++begin_iterator;
        }
    }
}
