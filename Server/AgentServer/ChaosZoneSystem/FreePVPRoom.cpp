#include "StdAfx.h"

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "FreePVPRoom.h"

bool FreePVPRoom::JoinRoom()
{
    if (free_pvp_room_info_.current_user_count >= free_pvp_room_info_.max_user_count)
    {
        return false;
    }
    
    ++(free_pvp_room_info_.current_user_count);
    return true;
}

bool FreePVPRoom::LeaveRoom()
{
    if (free_pvp_room_info_.current_user_count <= 0)
    {
        return false;
    }

    --(free_pvp_room_info_.current_user_count);
    return true;
}

const FreePVPRoomInfo& FreePVPRoom::GetFreePVPRoomInfo() const
{
    return free_pvp_room_info_;
}

void FreePVPRoom::SetFreePVPRoomInfo(const FreePVPRoomInfo& free_pvp_room_info)
{
    free_pvp_room_info_ = free_pvp_room_info;
}

bool FreePVPRoom::SetPlayerMaxCount(BYTE max_count)
{
    if (max_count > 20)    //script..
    {
        return false;
    }

    free_pvp_room_info_.max_user_count = max_count;
    return true;
}

bool FreePVPRoom::IsPrivateRoom() const
{
    bool password_existence = (free_pvp_room_info_.base_free_pvp_room_info.room_password[0] != _T('\0'));
    return password_existence;
}

bool FreePVPRoom::CheckPassword(const TCHAR* password) const
{
    if (IsPrivateRoom())
    {
        if (_tcsncmp(free_pvp_room_info_.base_free_pvp_room_info.room_password, password, 
            _countof(free_pvp_room_info_.base_free_pvp_room_info.room_password)) != 0)
        {
            return false;
        }
    }
    return true;
}

bool FreePVPRoom::CheckLevel(LEVELTYPE user_level) const
{
    //script..
    return true;
}

bool FreePVPRoom::CanJoinRoomByMemberCount() const
{
    return (free_pvp_room_info_.current_user_count < free_pvp_room_info_.max_user_count) ? true : false; 
}
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
