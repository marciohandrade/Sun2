#pragma once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <Singleton.h>
#include <ChaosZoneSystem/Handler_CG_FREE_PVP.h>
#include <ChaosZoneSystem/Handler_AG_FREE_PVP.h>
#include <ChaosZoneSystem/FreePVPRoom.h>

struct FreePVPRoomInfo;

enum FreePVPUserState
{
    kUserStateNone = 0,
    kWait,
    kJoin,
};
struct FreePVPUserInfo
{
    USERGUID user_guid;
    KEYTYPE room_key;
    util::Timer join_time;
    FreePVPUserState user_state;
    FreePVPUserInfo() :
        user_guid(0),
        room_key(0),
        user_state(kUserStateNone)
    {
    }
};

typedef STLX_HASH_MAP<KEYTYPE, FreePVPRoom> FreePVPRoomHash;
typedef STLX_HASH_MAP<USERGUID, FreePVPUserInfo*> FreePVPUserInfoHash;

class FreePVPManager : public util::Singleton<FreePVPManager>
{
public:
    FreePVPManager() : is_free_pvp_active_(true)
    {
    }
    ~FreePVPManager()
    {
    }
    void Update();

    bool CreateRoom(const BaseFreePVPRoomInfo& base_free_pvp_room_info, KEYTYPE& OUT create_room_key);
    void DestroyRoom(const KEYTYPE room_key);
    FreePVPRoom* FindRoom(const KEYTYPE room_key);

    bool JoinRoom(User* user, const KEYTYPE room_key);
    bool LeaveRoom(User* user, const ZONEKEY zone_key);
    BYTE ArchiveRoomList(FreePVPRoomInfo* OUT free_pvp_room_info, BYTE request_count)const;
    
    void InsertFreePVPUser(const USERGUID guid, const KEYTYPE room_key);
    void RemoveFreePVPUser(const USERGUID guid);
    FreePVPUserInfo* FindUserInfo(const USERGUID guid);

    bool IstMoveZoneWaitPlayer(const USERGUID guid);
    
    inline void SetFreePVPActive(const bool is_free_pvp_active) { is_free_pvp_active_ = is_free_pvp_active; }
    inline bool GetFreePVPActive() { return is_free_pvp_active_; }
    inline DWORD GetRoomCount() { return free_pvp_room_hash_.size(); }

    inline Handler_CG_FREE_PVP& GetCGPacketHandler() { return handler_cg_free_pvp_; }
    inline Handler_AG_FREE_PVP& GetAGPacketHandler() { return handler_ag_free_pvp_; }

private:
    void _MakingRoomInfo(FreePVPRoom& OUT create_free_pvp_room, 
                                    const BaseFreePVPRoomInfo& base_room_info,KEYTYPE room_key);
    void _Insert(const KEYTYPE room_key, FreePVPRoom free_pvp_room); 
    void _Delete(const KEYTYPE room_key);
    bool _SendFreePVPRoom(FreePVPUserInfo* user_info);

    inline KEYTYPE _AllocKey()
    { 
        return static_cast<KEYTYPE>(ObjKeyGender::Instance()->GetKey(OBJKEYTYPE_FREE_PVP_ROOM_KEY)); 
    }
    inline void _FreeKey(KEYTYPE Key) 
    { 
        ObjKeyGender::Instance()->RestoreKey(OBJKEYTYPE_FREE_PVP_ROOM_KEY, Key);
    }

private:
    FreePVPRoomHash free_pvp_room_hash_;
    FreePVPUserInfoHash free_pvp_user_info_hash_;
    bool is_free_pvp_active_;

    Handler_CG_FREE_PVP handler_cg_free_pvp_;
    Handler_AG_FREE_PVP handler_ag_free_pvp_;
};

#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP