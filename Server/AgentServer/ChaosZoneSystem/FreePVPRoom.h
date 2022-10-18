#pragma once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

class FreePVPRoom
{
public:
    FreePVPRoom()
    {
    }
    ~FreePVPRoom()
    {
    }

    bool JoinRoom();
    bool LeaveRoom();

    const FreePVPRoomInfo& GetFreePVPRoomInfo()const;
    void SetFreePVPRoomInfo(const FreePVPRoomInfo& free_pvp_room_info);

    bool SetPlayerMaxCount(BYTE max_count);
    
    bool IsPrivateRoom()const;
    bool CheckPassword(const TCHAR* password) const;
    bool CheckLevel(LEVELTYPE user_level) const;

    bool CanJoinRoomByMemberCount() const;

    inline MAPCODE GetMapCode()const { return free_pvp_room_info_.base_free_pvp_room_info.map_code; }
    inline void SetMapCode(MAPCODE map_code){ free_pvp_room_info_.base_free_pvp_room_info.map_code = map_code; }

    inline KEYTYPE GetRoomKey()const { return free_pvp_room_info_.room_key; }
    
    inline KEYTYPE GetZoneKey()const { return free_pvp_room_info_.zone_key; }
    inline void SetZoneKey(KEYTYPE zone_key) { free_pvp_room_info_.zone_key = zone_key; }
    
    inline TCHAR* GetPassword() { return free_pvp_room_info_.base_free_pvp_room_info.room_password; }
 
    inline BYTE GetCurrentUserCount() { return free_pvp_room_info_.current_user_count; }

private:
    FreePVPRoomInfo free_pvp_room_info_;
};

#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP