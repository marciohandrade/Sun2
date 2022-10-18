#include "stdafx.h"

//_NA_20110630_BATTLE_ZONE_RENEWAL

#include "PartyRoomFinder.h"
#include "BattlezoneMissionTreeParser.h"
#include "UserSessions/User.h"
#include <ServerSessions/ServerSessionEx.h>
#include "AgentPartyManager.h"
#include "Party/PartyRoom.h"

PartyRoomFinder::PartyRoomFinder()
{

}

PartyRoomFinder::~PartyRoomFinder()
{
    _Realese();
}

void
PartyRoomFinder::NotifyCreatePartyRoom(MAPCODE map_code, WORD party_key)
{
    _NotifyCreateRoomToStandByUser(map_code, party_key);
}

WORD
PartyRoomFinder::GetPartyKeyInMissionList(User* user, 
                                          const MAPCODE* mission_array, 
                                          BYTE check_mission_num)const
{
    WORD find_party_key;

    for (int index = 0; index < check_mission_num; ++index)
    {
        const PartyKeyList* party_key_list = _FindPartyKeyList(mission_array[index]);
        if (party_key_list == NULL || party_key_list->size() == 0)
        {
            continue;
        }
#ifdef _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY
        PartyKeyList::const_reverse_iterator rbegin_iterator = party_key_list->rbegin(),
                                             rend_iterator = party_key_list->rend();
        for(; rbegin_iterator != rend_iterator; ++rbegin_iterator)
        {
            find_party_key = *(rbegin_iterator);
#else
        PartyKeyList::const_iterator begin_iterator = party_key_list->begin(),
                                     end_iterator = party_key_list->end();
        for(; begin_iterator != end_iterator; ++begin_iterator)
        {
            find_party_key = *(begin_iterator);
#endif //_NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY
#ifdef _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
            const PartyRoom* party_room = AgentPartyManager::Instance()->FindPartyRoom(find_party_key);
            if (party_room != NULL &&
                _CanJoinParty(user, find_party_key) == true && 
                party_room->IsPrivateRoom() == false)
            {
                return find_party_key;
            }
#else
            if (_CanJoinParty(user, find_party_key) == true)
            {
                return find_party_key;
            }
#endif  // _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
        }
    }
    return 0;
}

bool 
PartyRoomFinder::InsertPartyKeyToMissionList(MAPCODE map_code, WORD insert_party_key)
{
    if (BattlezoneMissionTreeParser::Instance()->FindData(map_code) == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : This map code is incorrect : %d", map_code);
        return false;
    }

    _InsertPartyKey(map_code, insert_party_key);
    return true;
}

void
PartyRoomFinder::DeletePartyKeyFromMissionList(MAPCODE map_code, WORD delete_party_key)
{
    _DeletePartyKey(map_code, delete_party_key);
}

bool 
PartyRoomFinder::InsertCharGuidToMissionList(CHARGUID insert_char_guid, 
                                             const MAPCODE* mission_list, 
                                             BYTE count)
{
    if (count > MAX_MISSION_COUNT)
    {
        return false;
    }

    DeleteAllCharGuidFromMissionList(insert_char_guid);

    for (int index = 0; index < count; ++index)
    {
        if (BattlezoneMissionTreeParser::Instance()->FindData(mission_list[index]) == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found mission code : %d", mission_list[index]);
            continue;
        }

        _InsertCharGuid(mission_list[index], insert_char_guid);
    }

    return true;
}

void 
PartyRoomFinder::DeleteAllCharGuidFromMissionList(CHARGUID delete_char_guid)
{
    MissionPartyInfoMap::iterator begin_iterator = mission_party_map_.begin(),
                                  end_iterator = mission_party_map_.end();
    
    if (begin_iterator == end_iterator)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" :Probably party room finder fail Initialize");
        return;
    }

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        MAPCODE map_code = begin_iterator->first;
        if (_DeleteCharGuid(map_code, delete_char_guid) == false)
        {
            continue;
        }
    }
}
#ifdef _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
bool
PartyRoomFinder::UpdateSelectedMapCodeOfParty(MAPCODE delete_map_code, MAPCODE insert_map_code, 
                                              const WORD party_key, const BOOL check_secret_number)
{
    if (delete_map_code != 0)
    {
        DeletePartyKeyFromMissionList(delete_map_code, party_key);
    }

    if (InsertPartyKeyToMissionList(insert_map_code, party_key) == false)
    {
        return false;
    }

    // 비밀방이 아닐경우 통보하고 비밀방일 경우 통보하지 않는다..
    if (check_secret_number == false)
    {
        _NotifyCreateRoomToStandByUser(insert_map_code, party_key);
    }

    return true;
}
#else
bool
PartyRoomFinder::UpdateSelectedMapCodeOfParty(MAPCODE delete_map_code, 
                                              MAPCODE insert_map_code, WORD party_key)
{
    if (delete_map_code != 0)
    {
        DeletePartyKeyFromMissionList(delete_map_code, party_key);
    }

    if (InsertPartyKeyToMissionList(insert_map_code, party_key) == false)
    {
        return false;
    }

    _NotifyCreateRoomToStandByUser(insert_map_code, party_key);
    
    return true;
}
#endif  // _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE

//////////////////////////////////////////////////////////////////////////
bool
PartyRoomFinder::_CanJoinParty(User* user, WORD party_key)const
{
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        return false;
    }

    if (party_room->GetMapCode() != 0 &&
        party_room->CheckLevel(user->GetCharLV()) == false)
    { 
        return false;
    }

    if (party_room->CanJoinPartyRoomByMemberCount() == false)
    {
        return false;
    }

    if (party_room->GetIsMissionStart() == true)
    {
        return false;
    }

    return true;
}

void 
PartyRoomFinder::_Realese()
{
    MissionPartyInfoMap::iterator begin_iterator = mission_party_map_.begin(),
                                  end_iterator = mission_party_map_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        PartyRoomListInfo& delete_info = begin_iterator->second;
        _DeAllocate(&delete_info);
    }
}

void 
PartyRoomFinder::_NotifyCreateRoomToStandByUser(MAPCODE map_code, WORD party_key)
{
     const CharGuidList* char_guid_list = _FindCharGuidList(map_code);
    if (char_guid_list == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found char guid list : %d", map_code);
        return;
    }

    MSG_CG_PARTY_ASK_JOIN_ROOM_BRD brd_msg;
    brd_msg.map_code_ = map_code;
    brd_msg.party_key_ = party_key;

    CharGuidList::const_iterator begin_iterator = char_guid_list->begin(),
                                 end_iterator = char_guid_list->end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        CHARGUID char_guid = (*begin_iterator);
        User* notify_user = UserManager::Instance()->GetUserByCharGuid(char_guid);
        if (notify_user == NULL || notify_user->GetIsRecvInvitePartyRoom() == false)
        {
            continue;
        }
    
        ServerSessionEx* server_session = notify_user->GetLinkedServer();
        if(server_session)
        {
            if (server_session->GetServerType() == FIELD_SERVER)
            {
                notify_user->SendPacket(&brd_msg, sizeof(brd_msg));
            }
        }
    }
}

const PartyKeyList* 
PartyRoomFinder::_FindPartyKeyList(MAPCODE map_code)const
{
    MissionPartyInfoMap::const_iterator find_iterator = mission_party_map_.find(map_code);
    if (find_iterator == mission_party_map_.end())
    {
        return NULL;
    }

    const PartyRoomListInfo& party_room_info_list = find_iterator->second;

    return party_room_info_list.party_key_list_;
}

PartyKeyList* 
PartyRoomFinder::_FindEditablePartyKeyList(MAPCODE map_code)
{
    MissionPartyInfoMap::iterator find_iterator = mission_party_map_.find(map_code);
    if (find_iterator == mission_party_map_.end())
    {
        return NULL;
    }

    const PartyRoomListInfo& party_room_info_list = find_iterator->second;

    return party_room_info_list.party_key_list_;
}

void
PartyRoomFinder::_InsertPartyKey(MAPCODE map_code, WORD insert_party_key)
{
    PartyKeyList* party_key_list = _FindEditablePartyKeyList(map_code);
    if (party_key_list == NULL)
    {
        PartyRoomListInfo party_room_info_list = {};
        _Allocate(&party_room_info_list);
        party_room_info_list.party_key_list_->push_back(insert_party_key);

        mission_party_map_.insert(std::make_pair(map_code, party_room_info_list));
    }
    else 
    {
        party_key_list->push_back(insert_party_key);
    }
}

void 
PartyRoomFinder::_DeletePartyKey(MAPCODE map_code, WORD delete_party_key)
{
    PartyKeyList* party_key_list = _FindEditablePartyKeyList(map_code);
    if (party_key_list == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party key list : %d", map_code);
        return;
    }

    PartyKeyList::iterator begin_iterator = party_key_list->begin(),
                           end_iterator = party_key_list->end();

    while (begin_iterator != end_iterator)
    {
        WORD search_party_key = (*begin_iterator);
        if (search_party_key == delete_party_key)
        {
            begin_iterator = party_key_list->erase(begin_iterator);
            continue;
        }
        ++begin_iterator;
    }
}

const CharGuidList* 
PartyRoomFinder::_FindCharGuidList(MAPCODE map_code)const
{
    MissionPartyInfoMap::const_iterator find_iterator = mission_party_map_.find(map_code);
    if (find_iterator == mission_party_map_.end())
    {
        return NULL;
    }

    const PartyRoomListInfo& party_room_info_list = find_iterator->second;

    return party_room_info_list.char_guid_list_;
}

CharGuidList* 
PartyRoomFinder::_FindEditableCharGuidList(MAPCODE map_code)
{
    MissionPartyInfoMap::iterator find_iterator = mission_party_map_.find(map_code);
    if (find_iterator == mission_party_map_.end())
    {
        return NULL;
    }

    PartyRoomListInfo& party_room_info_list = find_iterator->second;

    return party_room_info_list.char_guid_list_;
}

void
PartyRoomFinder::_InsertCharGuid(MAPCODE map_code, CHARGUID insert_char_guid)
{
    CharGuidList* char_guid_list = _FindEditableCharGuidList(map_code);
    if (char_guid_list == NULL)
    {
        PartyRoomListInfo party_room_info_list = {};
        _Allocate(&party_room_info_list);
        party_room_info_list.char_guid_list_->push_back(insert_char_guid);

        mission_party_map_.insert(std::make_pair(map_code, party_room_info_list));
    }
    else 
    {
        char_guid_list->push_back(insert_char_guid);
    }
}

bool 
PartyRoomFinder::_DeleteCharGuid(MAPCODE map_code, CHARGUID delete_char_guid)
{
    CharGuidList* char_guid_list = _FindEditableCharGuidList(map_code);
    if (char_guid_list == NULL)
    {
        return false;
    }

    CharGuidList::iterator begin_iterator = char_guid_list->begin(),
                           end_iterator = char_guid_list->end();

    if (begin_iterator == end_iterator)
    {
        return false;
    }

    while (begin_iterator != end_iterator)
    {
        CHARGUID search_char_guid = (*begin_iterator);
        if (search_char_guid == delete_char_guid)
        {
            char_guid_list->erase(begin_iterator++);
            continue;
        }
        ++begin_iterator;
    }

    return true;
}

void 
PartyRoomFinder::_Allocate(PartyRoomListInfo* party_room_info_list)
{
    party_room_info_list->char_guid_list_ = new CharGuidList;
    party_room_info_list->party_key_list_ = new PartyKeyList;
}

void 
PartyRoomFinder::_DeAllocate(PartyRoomListInfo* party_room_info_list)
{
    SAFE_DELETE(party_room_info_list->char_guid_list_);
    SAFE_DELETE(party_room_info_list->party_key_list_);
}

