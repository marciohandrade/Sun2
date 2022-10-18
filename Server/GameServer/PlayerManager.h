#pragma once
#ifndef GAMESERVER_PLAYER_MANAGER_H
#define GAMESERVER_PLAYER_MANAGER_H

//==================================================================================================
//
#include <Singleton.h>
#include "ObjectDefine.h"
#include <ObjectKeyStream.h>

class Player;
class ObjectFactory;

struct PlayerObjectKeyStreamAddin
{
    DWORD CharGuid;
    PlayerObjectKeyStreamAddin()
        : CharGuid(0)
    {}
    PlayerObjectKeyStreamAddin(const DWORD char_guid)
        : CharGuid(char_guid)
    {}
    PlayerObjectKeyStreamAddin(const PlayerObjectKeyStreamAddin& rhs)
        : CharGuid(rhs.CharGuid)
    {}

    PlayerObjectKeyStreamAddin& operator=(const PlayerObjectKeyStreamAddin& rhs)
    {
        CharGuid = rhs.CharGuid;
        return *this;
    }
};

//==================================================================================================
// <PlayerManager>
//
class PlayerManager : public util::Singleton<PlayerManager>
{
public:
    PlayerManager();
    ~PlayerManager();

    void Update(const util::ServerTimeInfoPerFrame* time_info);

    bool AddPlayer(Player* player);
    //  (NOTICE)
    //  - CharName을 아는 시점은 DB로 부터 정보를 받고 난 후의 시점이며, 따라서 이 시점에 CharGuid도 알게 된다.
    bool AddPlayerName(Player* player);
    bool RemovePlayer(Player* player);

    void Disconnect(Player* player);
    void AllDisconnect();

    Player* FindPlayerByUserKey(const DWORD user_guid) const;
    Player* FindPlayerByCharKey(const DWORD char_guid) const;
    Player* FindPlayerByObjKey(const DWORD object_key) const;
    Player* FindPlayerByName(const char* char_name) const;
    Player* FindPlayerByNameHash(const DWORD char_name_hash) const;
    DWORD GetNumberOfPlayers() const;

    //////////////////////////////////////////////////////////////////////////
    //  통화량 관련
    MONEY GetAveUserTotalMoney() const;      // 유저 평균 통화량 계산
    MONEY GetUserTotalMoney() const;         // 유저 총 보유 량 계산
    MONEY GetAveWarehouseTotalMoney() const; // 유저 평균 통화량 계산(창고)
    MONEY GetWarehouseTotalMoney() const;    // 유저 총 보유 량 계산(창고)

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
    VOID  SendPacketToAllPlayer(MSG_BASE_FORWARD* msg, size_t size);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

private:
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    void AttendanceUpdate(const util::ServerTimeInfoPerFrame* time_info); // 접속 중인유저 출석 갱신
    int m_attendance_day;
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

    typedef ObjectKeyStream<Player,
        nsKeyInfo::PLAYER_OBJECT_KEY_START,
        nsKeyInfo::PLAYER_OBJECT_KEY_LAST,
        PlayerObjectKeyStreamAddin>     ObjectKeyStream;
    typedef STLX_HASH_MAP<DWORD, DWORD> PLAYER_INDEX_HASH;
    // CHANGES: f110517.3L, changes hash finding accessibility about a character name
    typedef PLAYER_INDEX_HASH PLAYER_NAME_HASH;
    typedef STLX_HASH_MAP<DWORD, Player*> PLAYER_HASHMAP;

    void ReleasePlayer(Player* player);

    ObjectKeyStream*  object_key_stream_;
    PLAYER_HASHMAP    user_guid_table_;
    PLAYER_INDEX_HASH char_guid_table_;
    PLAYER_NAME_HASH  char_name_table_;
    DWORD sector_delay_interval_;

    // Update Verification Fields
    DWORD m_dwDeltaTick;
    BOOL m_SectorUpdateOnUpdate;
};

//==================================================================================================
// inline implements

inline DWORD PlayerManager::GetNumberOfPlayers() const
{
    return static_cast<DWORD>(user_guid_table_.size());
}

inline Player* PlayerManager::FindPlayerByUserKey(const DWORD user_guid) const
{
    PLAYER_HASHMAP::const_iterator it = user_guid_table_.find(user_guid);
    if (it == user_guid_table_.end())
    {
        return NULL;
    }
    Player* const player = it->second;
    if (player != NULL && player->IsDeleted())
    {
        return NULL;
    }
    return player;
}

inline Player* PlayerManager::FindPlayerByCharKey(const DWORD char_guid) const
{
    PLAYER_INDEX_HASH::const_iterator it = char_guid_table_.find(char_guid);
    if (it == char_guid_table_.end())
    {
        return NULL;
    }
    const DWORD object_key = it->second;
    const ObjectKeyStream::sNode* const object_node = object_key_stream_->GetObjectInfo(object_key);
    if (object_node == NULL)
    {
        return NULL;
    };
    Player* const player = object_node->pObject;
    if (player != NULL && player->IsDeleted())
    {
        return NULL;
    }
    return player;
}

inline Player* PlayerManager::FindPlayerByObjKey(const DWORD object_key) const
{
    const ObjectKeyStream::sNode* const object_node = object_key_stream_->GetObjectInfo(object_key);
    if (object_node == NULL) 
    {
        return NULL;
    };
    Player* const player = object_node->pObject;
    if (player != NULL && player->IsDeleted())
    {
        return NULL;
    }
    return player;
}

inline Player* PlayerManager::FindPlayerByNameHash(const DWORD char_name_hash) const
{
    PLAYER_NAME_HASH::const_iterator it = char_name_table_.find(char_name_hash);
    if (it == char_name_table_.end())
    {
        return NULL;
    }
    const DWORD object_key = it->second;
    const ObjectKeyStream::sNode* const object_node = object_key_stream_->GetObjectInfo(object_key);
    if (object_node == NULL) 
    {
        return NULL;
    };
    Player* const player = object_node->pObject;
    if (player != NULL && player->IsDeleted())
    {
        return NULL;
    }
    return player;
}

//==================================================================================================
//========== Player Cast Template ==========
#define PLAYER_PTR_USERKEY(type, obj, Value) \
    type* obj = PlayerManager::Instance()->FindPlayerByUserKey(Value); \
    if (!obj || !obj->GetField()) \
    { \
        SUNLOG(eDEV_LOG,  "Player Not Exist"); \
        return; \
    }
//==================================================================================================

#endif //GAMESERVER_PLAYER_MANAGER_H