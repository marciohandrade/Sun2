#include "StdAfx.h"

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneSystem/FreePVPManager.h>
#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#include <ServerOptionParserEx.h>

void FreePVPManager::Update()
{
    FreePVPUserInfoHash::iterator itr = free_pvp_user_info_hash_.begin();
    while (itr != free_pvp_user_info_hash_.end())
    {
        FreePVPUserInfo* const user_info = itr->second;

        switch(user_info->user_state)
        {
        case kJoin:
            {
                //제거..
                FreePVPUserInfoHash::iterator erase_itr = itr++;
                RemoveFreePVPUser((erase_itr->second)->user_guid);
                continue;
            }
            break;
        case kWait:
            {
                if (user_info->join_time.IsExpiredManual(false) == false)
                {
                    break;
                }
                // 시간이 지나면 이동시킨다.
                _SendFreePVPRoom(user_info);
                user_info->user_state = kJoin;
            }
            break;
        }

        ++itr;
    }
}

bool FreePVPManager::CreateRoom(const BaseFreePVPRoomInfo& base_free_pvp_room_info, 
                                                      KEYTYPE& OUT create_room_key)
{
    const KEYTYPE room_key = _AllocKey();
    FreePVPRoom create_party_room;
    _MakingRoomInfo(create_party_room, base_free_pvp_room_info, room_key);
    _Insert(room_key, create_party_room);
    create_room_key = room_key;
    return true;
}

void FreePVPManager::DestroyRoom(const KEYTYPE destroy_key)
{
    _Delete(destroy_key);
}

FreePVPRoom* FreePVPManager::FindRoom(const WORD room_key)
{
    FreePVPRoomHash::iterator itr = free_pvp_room_hash_.find(room_key);
    if (itr == free_pvp_room_hash_.end())
    {
        return NULL;
    }
    return &itr->second;
}

bool FreePVPManager::JoinRoom(User* user, const KEYTYPE room_key)
{
    __UNUSED(user);
    FreePVPRoom* const free_pvp_room = FindRoom(room_key);
    if (free_pvp_room == NULL)
    {
        return false;
    }
    free_pvp_room->JoinRoom();
    return true;
}

bool FreePVPManager::LeaveRoom(User* user, const ZONEKEY zone_key)
{
    FreePVPRoomHash::iterator itr = free_pvp_room_hash_.begin();
    for(; itr != free_pvp_room_hash_.end(); ++itr)
    {
        FreePVPRoom& free_pvp_room = itr->second;
        if (free_pvp_room.GetZoneKey() != zone_key)
        {
            continue;
        }
        free_pvp_room.LeaveRoom();
        
        //방에 아무도 없으면 방 리스트 삭제..
        if (free_pvp_room.GetCurrentUserCount() <= 0)
        {
            DestroyRoom(free_pvp_room.GetRoomKey());
        }
        return true;
    }

    return false;
}

BYTE FreePVPManager::ArchiveRoomList(FreePVPRoomInfo* OUT free_pvp_room_info, BYTE request_count) const
{
    SERVER_COMMON_OPTION& server_option_info = ServerOptionParserEx::Instance()->GetServerOption();
    if (server_option_info.free_pvp_max_room_count > request_count) // script..
    {
        return 0;
    }
    
    uint8_t dest_index = 0;
    FreePVPRoomHash::const_iterator const_itr = free_pvp_room_hash_.begin();
    for (; const_itr != free_pvp_room_hash_.end(); ++const_itr, ++dest_index)
    {
        const FreePVPRoom& free_pvp_room = const_itr->second;
        const FreePVPRoomInfo& room_info = free_pvp_room.GetFreePVPRoomInfo();
        free_pvp_room_info[dest_index] = room_info;

        // 비밀번호가 있으면.. 보안처리..
        bool password_existence = 
            (free_pvp_room_info[dest_index].base_free_pvp_room_info.room_password[0] != _T('\0'));

        if (password_existence)
        {
            _tcsncpy(free_pvp_room_info[dest_index].base_free_pvp_room_info.room_password,_T("hello"), 
                _countof(free_pvp_room_info[dest_index].base_free_pvp_room_info.room_password));
        }
    }

    // 방 최대갯수가 넘어가면 최대치 만큼만 보낸다..
    if (dest_index > server_option_info.free_pvp_max_room_count)
    {
        dest_index = server_option_info.free_pvp_max_room_count;
    }

    return dest_index;
}

void FreePVPManager::InsertFreePVPUser(const USERGUID guid, const KEYTYPE room_key)
{
    FreePVPUserInfoHash::const_iterator const_itr = free_pvp_user_info_hash_.find(guid);
    if (const_itr != free_pvp_user_info_hash_.end())
    {
        return;
    }

    FreePVPUserInfo* user_info = new FreePVPUserInfo();
    user_info->user_guid = guid;
    user_info->room_key = room_key;
    user_info->user_state = kWait;
    user_info->join_time.SetTimer(10 * 1000); //script..
    free_pvp_user_info_hash_.insert(FreePVPUserInfoHash::value_type(guid, user_info));
}

void FreePVPManager::RemoveFreePVPUser(const USERGUID guid)
{
    FreePVPUserInfoHash::iterator itr = free_pvp_user_info_hash_.find(guid);
    if (itr == free_pvp_user_info_hash_.end())
    {
        return;
    }
    delete (itr->second);
    free_pvp_user_info_hash_.erase(itr);
}

FreePVPUserInfo* FreePVPManager::FindUserInfo(const USERGUID guid)
{
    FreePVPUserInfoHash::const_iterator const_itr = free_pvp_user_info_hash_.find(guid);
    if (const_itr == free_pvp_user_info_hash_.end())
    {
        return NULL;
    }
    return const_itr->second;
}

bool FreePVPManager::IstMoveZoneWaitPlayer(const USERGUID guid)
{
    FreePVPUserInfoHash::const_iterator const_itr = free_pvp_user_info_hash_.find(guid);
    if (const_itr == free_pvp_user_info_hash_.end())
    {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
void FreePVPManager::_MakingRoomInfo(FreePVPRoom& OUT create_free_pvp_room, 
                                     const BaseFreePVPRoomInfo& base_room_info,KEYTYPE room_key)
{
    SERVER_COMMON_OPTION& server_option_info = ServerOptionParserEx::Instance()->GetServerOption();

    FreePVPRoomInfo free_pvp_room_info;
    free_pvp_room_info.base_free_pvp_room_info = base_room_info;
    free_pvp_room_info.current_user_count = 0;
    free_pvp_room_info.max_user_count = server_option_info.free_pvp_max_user_count; // script..
    free_pvp_room_info.room_key = room_key;
    free_pvp_room_info.zone_key = 0;
    create_free_pvp_room.SetFreePVPRoomInfo(free_pvp_room_info);
}

void FreePVPManager::_Insert(const KEYTYPE room_key, FreePVPRoom free_pvp_room)
{
    free_pvp_room_hash_.insert(FreePVPRoomHash::value_type(room_key, free_pvp_room)); 
}

void FreePVPManager::_Delete(const KEYTYPE room_key)
{
    FreePVPRoomHash::iterator itr = free_pvp_room_hash_.find(room_key);

    if (itr == free_pvp_room_hash_.end())
    {
        return;
    }

    const FreePVPRoom& free_pvp_room = itr->second;
    if (free_pvp_room.GetRoomKey() != room_key)
    {
        return;
    }

    _FreeKey(free_pvp_room.GetRoomKey());
    free_pvp_room_hash_.erase(itr);
}

bool FreePVPManager::_SendFreePVPRoom(FreePVPUserInfo* user_info)
{
    MSG_CG_FREE_PVP_JOIN_ROOM_ACK ack_msg;
    ack_msg.result_code = RC::RC_FREE_PVP_JOIN_SUCCESS;

    User* const user = UserManager::Instance()->GetUser(user_info->user_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[%s|%d] Not found zone : %d", 
            __FUNCTION__, __LINE__,  user_info->user_guid);
        return false;
    }
    
    // 자유전투실 비활성화.
    if (is_free_pvp_active_ == false)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_INACTIVE;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG, " [%s|%d] FreePVP dont't Active ",
            __FUNCTION__, __LINE__);
        return false;
    }

	// 다른 행동 중 일때..(거래, 상점)
    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG," [%s|%d] PLAYER_BEHAVE_IDLE_STATE : %d ", 
            __FUNCTION__, __LINE__,user->GetUserGUID());
        return false;
    }

    const KEYTYPE room_key = user_info->room_key;
    FreePVPRoom* const free_pvp_room = FindRoom(room_key);
    if (free_pvp_room == NULL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG, " [%s|%d] Don't find FreePvP Room : %d ",
            __FUNCTION__, __LINE__, user->GetUserGUID());
        return false;
    }

    // 존 이동하기 전 인원수 검사..
    if (free_pvp_room->CanJoinRoomByMemberCount() == false) // script..
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_MAX_USER;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG, " [%s|%d] don't join FreePVP Room Max user! : guid = %d ", 
            __FUNCTION__, __LINE__, user->GetUserGUID());
        return false;
    }

    InstanceDungeon* const target_room = \
        static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(free_pvp_room->GetZoneKey()));
    if (target_room == NULL) 
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG,  " [%s|%d] RFREE_PVP_JOIN_FAIL_ISNOT_ROOM : %d ",
            __FUNCTION__, __LINE__, user->GetUserGUID());
        return false;
    }

    // 해당 존 상태가 일반적이지 않을 경우..
    if (target_room->GetRoomState() != eROOMSTATE_GENERNAL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG, " [%s|%d] Room State is not Generanl : CurrentSteate(%d) ", 
            __FUNCTION__, __LINE__, target_room->GetRoomState());
        return false;
    }

    ZoneInterface* current_zone = g_ZoneManager.FindZone(user->GetZoneKey());
    if (current_zone == NULL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG, " [%s|%d] Not found zone : %d", 
            __FUNCTION__, __LINE__, user->GetUserGUID());
        return false;
    }
    user->BeginTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);

    user->SetTRRoomKey(free_pvp_room->GetZoneKey());

    CheckMoveZonePolicy* const check_move_zone_policy = 
        MoveZonePolicy<MZTF_STATIC_TO_ROOM_FOR_UCUSER>::AlignInfo(user,
        user->GetZoneType(), target_room->GetType(),
        user->GetZoneKey(),  target_room->GetKey());

    RoomArgument argument(eZONEPUBLIC_PUBLIC, NULL);

    RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(check_move_zone_policy, RoomArgument::GetEmpty());
    if (rt != RC::RC_ROOM_SUCCESS)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        SUNLOG(eCRITICAL_LOG, " [%s|%d] MoveZone Fail User = %d | Room Key(%d), Result Code(%d) ", 
            __FUNCTION__,
            __LINE__,
            user->GetUserGUID(),
            free_pvp_room->GetZoneKey(),
            rt);

        user->EndTransaction();
        return false;
    }

    ack_msg.free_pvp_room_info = free_pvp_room->GetFreePVPRoomInfo();
    user->SendPacket(&ack_msg, sizeof(ack_msg));
    JoinRoom(user, room_key);

    return true;
}
#endif