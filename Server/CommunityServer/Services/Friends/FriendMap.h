#pragma once
#ifndef WORLDSERVER_FRIEND_MAP_H
#define WORLDSERVER_FRIEND_MAP_H

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING

//==================================================================================================
/*

작성자: 문상현
제작일: 2009-11-04

간단한설명: WorldUser 클래스의 친구 맵 관리 부분을 이쪽으로 옮겨온다.
    @history:
        - 11.08.12, waverix, code arrangement for the renewal world server

*/
//==================================================================================================

namespace ns_functor {
;

struct DBFriendsListFilter
{
    typedef STRUCT_FRIEND_DB_INFO MemberInfoArray[MAX_FRIENDBLOCK_STAT_NUM];
};

struct FriendListFilter
{
    // NOTE: MAX_FRIENDBLOCK_STAT_NUM = { friends, blocks }
    typedef STRUCT_FRIEND_INFO MemberInfoArray[MAX_FRIENDBLOCK_STAT_NUM];
};

struct BlockCharListFilter
{
    typedef STRUCT_BLOCK_INFO MemberInfoArray[MAX_BLOCK_STAT_NUM];
};

struct RevengeListFilter
{
    typedef STRUCT_REVENGE_INFO MemberInfoArray[MAX_REVENGE_STAT_NUM];
    template<size_t _Size>
    MemberInfoArray& GetRedirectedArray(STRUCT_REVENGE_INFO (&array)[_Size]) const;
};

}; //end of namespace

//==================================================================================================

namespace ns_world {
;

class FriendMap
{
public:
    //typedef ulong CHARSEED; // CHARSEED = util::StreamHashKey::GetStringHash(char_name)
    typedef ns_world::FriendLink::FRIEND_INFO_MAP FRIEND_INFO_MAP;
    typedef ns_world::BlockCharLink::BLOCK_INFO_MAP BLOCK_INFO_MAP;
    typedef ns_world::RevengeLink::REVENGE_INFO_MAP REVENGE_INFO_MAP;
    //
    typedef STLX_MAP<USERGUID, FRIEND_INFO_MAP*> FRIEND_TABLE;
    typedef STLX_MAP<USERGUID, BLOCK_INFO_MAP*> BLOCK_CHAR_TABLE;
    typedef STLX_MAP<USERGUID, REVENGE_INFO_MAP*> REVENGE_TABLE;
    //typedef STLX_HASH_MAP<CHARSEED, CHARGUID> FRIEND_KEY_PAIR_TABLE;

    static FriendMap* Instance();
    //----------------------------------------------------------------------------------------------
    static bool PreparedFriendService(ns_world::WorldUser* user);
    // NOTE: f110905.1L, this initialize the user linked storage on the first enterance
    bool PrepareFriendServices(ns_world::WorldUser* user);
    bool NotifyOnlineStatusChangedFriendData(ns_world::WorldUser* user, bool online);
    bool ReleaseFriendData(ns_world::WorldUser* user);
    //----------------------------------------------------------------------------------------------
    static bool IsFriendTransaction(ns_world::WorldUser* user);
    static bool StartFriendTransaction(ns_world::WorldUser* user);
    static bool EndFriendTransaction(ns_world::WorldUser* user);
    static RC::eFRIEND_RESULT CheckAccessibleFriendInfo(
        ns_world::WorldUser* user, CHARGUID related_char_guid);
    //----------------------------------------------------------------------------------------------
    // friend interfaces
    static int GetNumberOfFriends(ns_world::WorldUser* user);
    static void ClearFriends(ns_world::WorldUser* user);
    static const STRUCT_FRIEND_INFO* FindFriend(
        ns_world::WorldUser* user, USERGUID friend_char_guid);
    static const STRUCT_FRIEND_INFO* FindFriendByCharName(
        ns_world::WorldUser* user, const util::StrArrayForCharName& friend_char_name);
    static bool RemoveFriend(ns_world::WorldUser* user, CHARGUID friend_char_guid);
    static bool RemoveFriend(ns_world::WorldUser* user,
        const util::StrArrayForCharName& friend_char_name);
    //
    //----------------------------------------------------------------------------------------------
    // character block list interfaces
    static int GetNumberOfBlocks(ns_world::WorldUser* user);
    static void ClearBlocks(ns_world::WorldUser* user);
    static const STRUCT_BLOCK_INFO* FindBlockChar(
        ns_world::WorldUser* user, USERGUID block_char_guid);
    static const STRUCT_BLOCK_INFO* FindBlockCharByCharName(
        ns_world::WorldUser* user, const util::StrArrayForCharName& block_char_name);
    static bool RemoveBlockChar(ns_world::WorldUser* user, CHARGUID blocked_char_guid);
    static bool RemoveBlockChar(ns_world::WorldUser* user,
        const util::StrArrayForCharName& block_char_name);
    //----------------------------------------------------------------------------------------------
    // revenge interfaces
    static int GetNumberOfRevenges(ns_world::WorldUser* user);
    static void ClearRevenges(ns_world::WorldUser* user);
    static const STRUCT_REVENGE_INFO* FindRevengeChar(
        ns_world::WorldUser* user, USERGUID revenge_char_guid);
    static const STRUCT_REVENGE_INFO* FindRevengeCharByCharName(
        ns_world::WorldUser* user, const util::StrArrayForCharName& revenged_char_name);
    static bool RemoveRevengeChar(ns_world::WorldUser* user, CHARGUID revenged_char_guid);
    static bool RemoveRevengeChar(ns_world::WorldUser* user,
        const util::StrArrayForCharName& revenge_char_name);
    //
    static uint8_t GetRevengePoint(ns_world::WorldUser* user);
    static void DecreaseRevengePoint(ns_world::WorldUser* user);
    //----------------------------------------------------------------------------------------------
    // NOTE: send to all online users
    //  event broadcasters  = { friend, block, revenge, friend_invitation }
    bool SendToAllFriends(ns_world::WorldUser* user,
        MSG_BASE* msg_base, uint16_t msg_size) const;
    bool SendToAllBlockChars(ns_world::WorldUser* user,
        MSG_BASE* msg_base, uint16_t msg_size) const;
    bool SendToAllRevengeChars(ns_world::WorldUser* user,
        MSG_BASE* msg_base, uint16_t msg_size) const;
    //----------------------------------------------------------------------------------------------
    // FriendMap_DataIo.cpp
    bool StoreTotalFriendContentsList(ns_world::WorldUser* const owner,
        const ns_functor::DBFriendsListFilter::MemberInfoArray& friend_array,
        size_t number_of_nodes);
    bool LoadFriendList(ns_world::WorldUser* user,
        ns_functor::FriendListFilter::MemberInfoArray& result_friend_list,
        uint8_t* const result_number_of_friend_nodes) const;
    bool LoadBlockList(ns_world::WorldUser* user,
        ns_functor::BlockCharListFilter::MemberInfoArray& result_blockchar_list,
        uint8_t* const result_number_of_blockchar_nodes) const;
    bool LoadRevengeList(ns_world::WorldUser* user,
        ns_functor::RevengeListFilter::MemberInfoArray& result_revenge_list,
        uint8_t* const result_number_of_revenge_nodes,
        uint8_t* const result_revenge_points) const;
    //
    bool InsertNewFriend(ns_world::WorldUser* const owner,
        const STRUCT_FRIEND_INFO& filtered_friend_info);
    bool InsertNewBlockChar(ns_world::WorldUser* const owner,
        const STRUCT_BLOCK_INFO& filtered_block_info);
    bool InsertNewRevengeChar(ns_world::WorldUser* const owner,
        const STRUCT_REVENGE_INFO& filtered_revenge_info);
private:
    //
    FriendMap();
    ~FriendMap();
    static FriendMap* CreateInstance();
    //
    void RegisterCharStatusDataNodeAccessor(ICharStatusDataNodeAccessor* accessor);
    //FRIEND_KEY_PAIR_TABLE key_pair_table_;
    ns_world::ICharStatusDataNodeAccessor* char_status_data_accessor_;
    //
    FRIEND_TABLE friend_table_;
    BLOCK_CHAR_TABLE block_char_table_;
    REVENGE_TABLE revenge_table_;
    //
    // last order...
    static FriendMap* instance_static_;
    //
    friend class ns_world::WorldUserManager;
    __DISABLE_COPY(FriendMap);
}; //end of class 'FriendMap'

}; //end of namespace

//==================================================================================================

inline ns_world::FriendMap* ns_world::FriendMap::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

//==================================================================================================

template<>
inline ns_functor::RevengeListFilter::MemberInfoArray&
ns_functor::RevengeListFilter::GetRedirectedArray<MAX_REVENGE_STAT_NUM>(
    STRUCT_REVENGE_INFO (&array)[MAX_REVENGE_STAT_NUM]) const
{
    MemberInfoArray* convert = &array;
    return *convert;
}


//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //_NA_0_20091104_FIRENDMAP_REFACTORYING


#endif //WORLDSERVER_FRIEND_MAP_H