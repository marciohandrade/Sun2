#include "stdafx.h"

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "PartyRoomWaitingList.h"

bool
PartyRoomWaitingList::InsertPartyRoomToWaitingList(WORD insert_party_key)
{
    if (_Find(insert_party_key) != NULL)
    {
        return false;
    }

    _Insert(insert_party_key);
    return true;
}

PartyRoomWaitingList::StartWaitingRoomList*
PartyRoomWaitingList::GetEditableWaitingRoomList()
{
    return &(wait_room_list_);
}

//////////////////////////////////////////////////////////////////////////

void 
PartyRoomWaitingList::_Insert(WORD insert_party_key)
{
    wait_room_list_.push_back(insert_party_key);
}

void 
PartyRoomWaitingList::_Delete(WORD delete_party_key)
{
    StartWaitingRoomList::iterator begin_iterator = wait_room_list_.begin(),
                                   end_iterator = wait_room_list_.end();
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const WORD& current_party_key = *(begin_iterator);
        if (current_party_key == delete_party_key)
        {
            wait_room_list_.erase(begin_iterator);
            return;
        }
    }
}

const WORD*
PartyRoomWaitingList::_Find(WORD find_party_key)const
{
    StartWaitingRoomList::const_iterator begin_iterator = wait_room_list_.begin(),
                                         end_iterator = wait_room_list_.end();
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const WORD& current_party_key = *(begin_iterator);
        if (current_party_key == find_party_key)
        {
            return &current_party_key;
        }
    }
    return NULL;
}