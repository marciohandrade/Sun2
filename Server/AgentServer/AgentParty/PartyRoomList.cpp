#include "stdafx.h"

//_NA_20110630_BATTLE_ZONE_RENEWAL

#include "..\Party\PartyRoom.h"
#include "PartyRoomList.h"
#include "BattlezoneMissionTreeParser.h"

PartyRoomList::PartyRoomList()
{

}

PartyRoomList::~PartyRoomList()
{

}

void
PartyRoomList::DestroyPartyRoom(WORD party_key)
{
    return _Delete(party_key);
}


bool
PartyRoomList::UpdatePartyRoomInfo(const PartyRoomInfo& update_room_info)
{
    PartyRoom* party_room = _FindEditableNode(update_room_info.room_key_);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail update party room info : %d", 
               update_room_info.room_key_);
        return false;
    }

    party_room->SetPartyRoomInfo(update_room_info);

    return true;
}

bool
PartyRoomList::UpdateBasePartyRoomInfo(const BasePartyRoomInfo& update_base_room_info, 
                                       WORD party_key)
{
    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        return false;
    }

    BattlezoneMissionTreeParser* const parser = BattlezoneMissionTreeParser::Instance();

    MAPCODE map_code = update_base_room_info.map_code_;    
    const BattlezoneMissionTreeInfo* parser_info = parser->FindData(map_code);
    if (parser_info == NULL)
    {
        return false;
    }

    party_room->SetBasePartyRoomInfo(update_base_room_info);

    return true;
}

BYTE 
PartyRoomList::CalcAllPageOfRoomList()
{
    RoomList::size_type all_data_count = party_room_list_.size();
    BYTE number_of_page = static_cast<BYTE>(all_data_count / MAX_PARTYROOM_LIST) + 1;

    return ((all_data_count % MAX_PARTYROOM_LIST) != 0) ? number_of_page : number_of_page - 1;
}

BYTE
PartyRoomList::ArchiveRoomList(PartyRoomInfo* OUT party_room_info, 
                               BYTE request_count, BYTE request_page)const
{
    //1. 요청 페이지의 첫번째 인덱스를 구한다.
    RoomList::size_type request_index = (request_page - 1) * MAX_PARTYROOM_LIST;

    if ((request_count > MAX_PARTYROOM_LIST) && (request_index < 0))
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Request count, index is incorrect : count : %d, index : %d",
               request_count, request_index);

        return 0;
    }

    //2. 요청 인덱스의 값이 전체 데이터 갯수보다 많으면(index로 코드상 같은 수까지 포함) 0 리턴
    const RoomList::size_type max_list_count = party_room_list_.size();
    if (request_index >= max_list_count)
    {
        return 0;
    }

    //3. 요청된 페이지 부터 최신화된 리스트(즉, 높은 키값 부터)를 요청 갯수만큼 보낸다.
    RoomList::const_reverse_iterator begin_iterator = party_room_list_.rbegin(),
                                     end_iterator = party_room_list_.rend();
    RoomList::size_type dest_index = 0;
    for (begin_iterator += request_index; 
         dest_index < request_count && begin_iterator != end_iterator; 
         ++begin_iterator, ++dest_index)
    {
        const PartyRoom& agent_party_room = *(begin_iterator);
        const PartyRoomInfo* room_info = agent_party_room.GetPartyRoomInfo();
        party_room_info[dest_index] = *(room_info);
    }

    return static_cast<BYTE>(dest_index);
}

BYTE
PartyRoomList::SearchRoomByMapCode(PartyRoomInfo* OUT party_room_info, 
                                   MAPCODE map_code,
                                   PartyZoneType party_room_type,
                                   BYTE request_count)const
{
    RoomList::const_reverse_iterator begin_iterator = party_room_list_.rbegin(),
                                     end_iterator = party_room_list_.rend();
    
    RoomList::size_type index = 0;
    for (; index < request_count && begin_iterator != end_iterator; ++begin_iterator)
    {
        const PartyRoom& party_room = *(begin_iterator);
        if (party_room.GetMapCode() == map_code && 
            party_room.GetPartyRoomType() == party_room_type)
        {
            const PartyRoomInfo* room_info = party_room.GetPartyRoomInfo();
            party_room_info[index] = *(room_info);
            ++index;
        }
    }

    return static_cast<BYTE>(index);
}

BYTE
PartyRoomList::SearchRoomByWaiting(PartyRoomInfo* OUT party_room_info, BYTE request_count)const
{
    RoomList::const_reverse_iterator begin_iterator = party_room_list_.rbegin(),
                                     end_iterator = party_room_list_.rend();

    RoomList::size_type index = 0;
    for (; index < request_count && begin_iterator != end_iterator; ++begin_iterator)
    {
        const PartyRoom& party_room = *(begin_iterator);
        if (party_room.GetIsMissionStart() == false)
        {
            const PartyRoomInfo* room_info = party_room.GetPartyRoomInfo();
            party_room_info[index] = *(room_info);
            ++index;
        }
    }

    return static_cast<BYTE>(index);
}

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
bool
PartyRoomList::CreatePartyRoom(const BasePartyRoomInfo& create_basic_room_info, 
                               const WORD create_party_key, 
                               WORD& OUT create_room_key)
{
    const KEYTYPE room_key = _AllocKey();
    PartyRoom create_party_room;
    _MakingRoomInfo(
        create_party_room, 
        create_basic_room_info, 
        room_key, 
        create_party_key
        );
  
    _Insert(create_party_room);

    create_room_key = room_key;

    return true;
}
#else
bool
PartyRoomList::CreatePartyRoom(const BasePartyRoomInfo& create_basic_room_info, WORD create_party_key)
{
    const KEYTYPE room_key = _AllocKey();
    PartyRoom create_party_room;
    _MakingRoomInfo(create_party_room, 
                    create_basic_room_info, 
                    room_key, 
                    create_party_key);

    _Insert(create_party_room);

    return true;
}
#endif  //_NA_004694_20120412_1202_PARTY_ROOM_KEY

BYTE 
PartyRoomList::GetMaxPlayerCount(WORD party_key)
{
    const PartyRoom* party_room = FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        return 0;
    }
    
    return party_room->GetPartyRoomInfo()->max_user_num_;
}

bool 
PartyRoomList::SetMaxPlayerCount(WORD party_key, BYTE max_count)
{
    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        return false;
    }

    party_room->SetPlayerMaxCount(max_count);
    return true;
}

bool 
PartyRoomList::JoinPartyRoom(User* user, WORD party_key)
{
    __UNUSED(user);
    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        return false;
    }

    party_room->JoinRoom();

    if (party_room->GetPartyRoomType() == PartyOfPvPByTeam)
    {
        const BYTE number_of_team = 2;
        user->SetTeam(party_room->SelectPVPTeam(number_of_team));
    }

    return true;
}

bool 
PartyRoomList::LeavePartyRoom(WORD party_key)
{
    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return false;
    }

    party_room->LeaveRoom();

    return true;
}

void
PartyRoomList::SetMissionCode(WORD party_key, MAPCODE mission_code)
{
    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        return;
    }

    party_room->SetMapCode(mission_code);

    return;
}

MAPCODE 
PartyRoomList::GetMapCodeOfPartyRoom(WORD party_key)const
{
    const PartyRoom* party_room = FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        return 0;
    }

    return party_room->GetPartyRoomInfo()->base_party_room_info.map_code_;
}

const PartyRoom* 
PartyRoomList::FindPartyRoom(WORD find_party_key)const
{
    RoomList::size_type size = party_room_list_.size();

    for (RoomList::size_type index = 0; index < size; ++index)
    {
        const PartyRoom& party_room = party_room_list_[index];
        if (party_room.GetPartyKey() == find_party_key)
        {
            return &party_room;
        }
    }

    return NULL;
}

const PartyRoom*
PartyRoomList::FindPartyRoomByRoomKey(KEYTYPE find_room_key)const
{
    RoomList::const_iterator begin_iterator = party_room_list_.begin(),
                             end_iterator = party_room_list_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const PartyRoom& find_party_room = *(begin_iterator);
        if (find_party_room.GetRoomKey() == find_room_key)
        {
            return &(find_party_room);
        }
    }

    return NULL;
}

void 
PartyRoomList::SetPartyRoomAddInfOfRequireItem(WORD party_key, SLOTCODE item_code)
{
    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found party room : key : %d",
               party_key);
        return;
    }

    party_room->SetPartyRoomAddInfoOfRequireItem(item_code);
}

void 
PartyRoomList::SetPartyRoomStateForWaitingToStart(WORD party_key)
{
    if (_SetPartyRoomStartState(party_key) == true)
    {
        _InsertToWaitingList(party_key);
    }
}

//////////////////////////////////////////////////////////////////////////
bool 
PartyRoomList::_SetPartyRoomStartState(WORD party_key)
{
    const BYTE start_count = 5;

    PartyRoom* party_room = _FindEditableNode(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found party room : key : %d",
               party_key);
        return false;
    }

    party_room->SetPartyRoomToStartState(start_count);
    return true;
}


void 
PartyRoomList::_InsertToWaitingList(WORD insert_party_key)
{
    if (waiting_room_list_.InsertPartyRoomToWaitingList(insert_party_key) == false)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Exist party key : %d",
               insert_party_key);
        return;
    }
}

void 
PartyRoomList::_MakingRoomInfo(PartyRoom& OUT create_party_room, 
                               const BasePartyRoomInfo& base_room_info,
                               KEYTYPE room_key, WORD party_key)
{
    MAPCODE map_code = base_room_info.map_code_;
    BattlezoneMissionTreeParser* const parser = BattlezoneMissionTreeParser::Instance();

    PartyRoomInfo create_room_info;
    create_room_info.base_party_room_info = base_room_info;
    create_room_info.base_party_room_info.party_room_type_ = base_room_info.party_room_type_;
    create_room_info.current_user_num_ = 0;
    create_room_info.max_user_num_ = MAX_PARTYMEMBER_NUM;
    create_room_info.is_mission_start_ = false;
    create_room_info.room_key_ = room_key;
    create_room_info.link_party_key_ = party_key;
    create_party_room.SetPartyRoomInfo(create_room_info);

    //타입별 추가 정보 입력 : 
    eZONETYPE zone_type = parser->GetZoneType(map_code);
    switch(zone_type)
    {
    case eZONETYPE_MISSION:
        break;
    case eZONETYPE_INSTANCE:
        break;
    case eZONETYPE_PVP:
        {
            if (base_room_info.party_room_type_ == PartyOfPvPByPersonal)
            {
                create_party_room.SetPvPMode(ePVP_PERSONAL_MODE);
            }
            else if (base_room_info.party_room_type_ == PartyOfPvPByTeam)
            {
                create_party_room.SetPvPMode(ePVP_TEAM_MODE);
            }
        }
        break;
    }
}

void 
PartyRoomList::_Insert(const PartyRoom& insert_room_info)
{
    party_room_list_.push_back(insert_room_info);
}

#ifdef _DEBUG
void 
PartyRoomList::Test_Insert(const PartyRoom& insert_room_info)
{
    party_room_list_.push_back(insert_room_info);
}
#endif

void
PartyRoomList::_Delete(WORD delete_party_key)
{
    RoomList::iterator begin_iterator = party_room_list_.begin(),
                       end_iterator = party_room_list_.end();

    if (begin_iterator == end_iterator)
    {
        SUNLOG(eFULL_LOG, __FUNCTION__" : Not exist party : %d", delete_party_key);
        return;
    }

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const PartyRoom& party_room = *(begin_iterator);
        if (party_room.GetPartyKey() == delete_party_key)
        {
            _FreeKey(party_room.GetRoomKey());
            party_room_list_.erase(begin_iterator);
            return;
        }
    }
}

PartyRoom* 
PartyRoomList::_FindEditableNode(WORD find_party_key)
{
    RoomList::size_type size = party_room_list_.size();

    for (RoomList::size_type index = 0; index < size; ++index)
    {
        PartyRoom& party_room = party_room_list_[index];
        if (party_room.GetPartyKey() == find_party_key)
        {
            return &party_room;
        }
    }

    return NULL;
}

#ifdef _DEBUG
// TestUnit, added by gamelulu
void TestPartyRoomList::Test_InsertRoomList(BYTE data_count)
{
    for (int i=0; i < data_count; ++i)
    {
        BasePartyRoomInfo base_data;
        base_data.map_code_ = i;
        _snprintf(base_data.party_room_title_, _countof(base_data.party_room_title_),
                  "Test Room 00%d", i);
        base_data.party_room_title_[_countof(base_data.party_room_title_) - 1] = '\0';

        PartyRoomInfo create_room_info;
        create_room_info.base_party_room_info = base_data;
        create_room_info.base_party_room_info.party_room_type_ = (PartyZoneType)(i%7+1);
        create_room_info.current_user_num_ = i;
        create_room_info.max_user_num_ = MAX_PARTYMEMBER_NUM;
        create_room_info.is_mission_start_ = !!(i%2);
        create_room_info.room_key_ = 15000+i;
        create_room_info.link_party_key_ = 1000+i;

        PartyRoom create_party_toom;
        create_party_toom.SetPartyRoomInfo(create_room_info);
        
        AgentPartyManager::Instance()->GetPartyRoomList()->Test_Insert(create_party_toom);
    }
}
#endif

