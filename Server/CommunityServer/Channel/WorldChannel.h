#pragma once
#ifndef WORLDSERVER_WORLD_CHANNEL_H
#define WORLDSERVER_WORLD_CHANNEL_H

#include "./ICommunity.h"

//==================================================================================================

namespace ns_world { namespace community {
;
class WorldChannel : public ns_world::community::ICommunity
{
public:
    typedef STLX_HASH_MAP<DWORD, ns_world::community::ZoneCommunity*> ZoneTable;
    typedef util::SolarHashTable<ns_world::community::PartyCommunity*, DWORD> PartyTable;
    //typedef STLX_HASH_MAP<DWORD, ns_world::community::ChatRoomCommunity*> ZoneTable;
    //
    WorldChannel(int channel_id, ns_world::ServerSessionInWorld* binded_session);
    virtual ~WorldChannel();

    // ICommunity interface
    virtual eCommunityType GetCommunityType() const;
    virtual bool EnabledChat() const;
    virtual void ConfigureChatEnabled(bool enabled);
    // ICommunity interface
    virtual bool Enter(WorldUser* user, const ns_world::ConnectedCharInfo& char_info);
    // ICommunity interface
    virtual bool Leave(WorldUser* user, ns_world::ConnectedCharInfo* const backup_info);

    // ICommunity interface
    virtual bool SendToAll(MSG_BASE* msg_base, uint16_t msg_size) const;
    //
    void SendPacket(MSG_BASE* msg_base, uint16_t msg_size) const;
    void SendToStaticFields(MSG_BASE* msg_base, uint16_t msg_size) const;
    void SendToBattleZones(MSG_BASE* msg_base, uint16_t msg_size) const;
    // NOTE: f110923.1L, migrated from WorldServer(old)::SendNormalChatToAll
    bool SendToAllInShoutRanges(MSG_BASE* msg_base, uint16_t msg_size,
        ns_world::WorldUser* shouter) const;
    //
    int GetChannelID() const;
    ns_world::ServerSessionInWorld* GetBindedSession() const;
    ns_world::community::ZoneCommunity* FindZone(DWORD zone_key) const;
    // NOTE: f110928.1L, side-effect, create zone when the entering zone is not found
    bool EnterZone(ns_world::WorldUser* const user,
        const ns_world::ConnectedCharInfo& char_info);
    // NOTE: f110928.1L, side-effect, destroy zone when the number of entered player is empty
    bool LeaveZone(ns_world::WorldUser* const user);
    // must be succeeded, side-effect: if the party already exist, clear all party members
    void CreateParty(uint16_t party_key);
    bool JoinParty(uint16_t party_key, ns_world::WorldUser* const join_user);
    bool WithdrawParty(uint16_t party_key, ns_world::WorldUser* const withdraw_user);
    bool DestroyParty(uint16_t party_key);
    //void RestoreParty(uint16_t party_key, ns_world::WorldUser* user);
    //
    void DisplayInfo();
#if WORLD_CONTENTS_BLOCK
    // 일반 유저, PC방 유저 전체 숫자 갱신
    void RefreshUserNum();
#endif
private:
    bool BuildMapInfo();
    void LinkShoutRange(ns_world::community::ZoneCommunity* zone) const;
    // data fields...
    int channel_id_;
    ns_world::ServerSessionInWorld* binded_session_;
    util::__LList<ChannelUserNode> user_root_;

    ZoneTable static_field_table_;
    ZoneTable dynamic_field_table_;
    PartyTable party_table_;
    ShoutRangeTable shout_range_table_;
    //
    bool blocked_chat_;
    //
    friend class ns_world::WorldChannelManager;
    __DISABLE_COPY(WorldChannel);
};

}}; //end of namespace


inline int ns_world::community::WorldChannel::GetChannelID() const {
    return channel_id_;
};

inline ns_world::ServerSessionInWorld*
ns_world::community::WorldChannel::GetBindedSession() const
{
    return binded_session_;
}

//==================================================================================================

#if WORLD_CONTENTS_BLOCK

namespace ns_world { namespace community {
;

class WorldChannel
{
public:
    WorldChannel();
    ~WorldChannel();

    void Init();
    void Release();
    void Update();

    BOOL LoadMap();
    BOOL AddShoutLinkedZone(DWORD dwMapCode, ChatZone *pLinkZone);
    BOOL AddBattleShoutLinkedZone(DWORD dwMapCode, ChatZone *pLinkZone);
    void RemoveBattleShoutLinkedZone(ChatZone *pLinkZone);

    ChatZone*               AddChatZone(eZONETYPE eZoneType, DWORD dwZoneKey, DWORD dwMapCode);
    void AddVillage(DWORD dwZoneKey, ChatZone *pVillage);
    void AddField(DWORD dwZoneKey, ChatZone *pField);
    void AddDominationField(DWORD dwZoneKey, ChatZone *pField);
    ChatZone*               AddFriendChat(DWORD dwZoneKey);

    void RemoveChatZone(DWORD dwZoneKey);
    void RemoveFriendChat(DWORD dwZoneKey);

    ChatZone*               FindZone(DWORD dwZoneKey);
    VillageZone*            FindVillage(DWORD dwMapCode);
    ChatZone*               FindFriendChat(DWORD dwZoneKey);

    void AddUser(ns_world::WorldUser *pUser, eZONETYPE userStatus, DWORD dwZoneKey);
    void RemoveUser(ns_world::WorldUser *pUser, BOOL bIterErase = true);
    BOOL UserEnterZone(ns_world::WorldUser *pUser, eZONETYPE eZoneType, DWORD dwZoneKey = 0, DWORD dwMapCode = 0);

    void SendToAll(MSG_BASE* msg_base, uint16_t msg_size);
    void SendToVillages(MSG_BASE* msg_base, uint16_t msg_size);
    void SendToBattleZones(MSG_BASE* msg_base, uint16_t msg_size);

    inline void SetChannelID(DWORD dwChannelID) { m_dwChannelID = dwChannelID; }
    inline DWORD GetChannelID() { return m_dwChannelID; }

    void DisplayInfo();

    void UserLeavePrevZone(ns_world::WorldUser *pUser);

    inline void ClearZoneUserNum()
    {
        for(USHORT i = eZONETYPE_LOBBY; i < eZONETYPE_MAX; i++)
        {
            m_dwZoneUserNum[i] = 0;
            m_dwZonePCRoomUserNum[i] = 0;
        }
    }
    inline void ClearZoneNum()
    {
        for(USHORT i = eZONETYPE_LOBBY; i < eZONETYPE_MAX; i++)
        {
            m_dwZoneNum[i] = 0;
        }
    }

    inline void IncZoneNum(eZONETYPE eZoneType) { m_dwZoneNum[eZoneType]++; }
    inline void DecZoneNum(eZONETYPE eZoneType) { if(m_dwZoneNum[eZoneType] > 0) m_dwZoneNum[eZoneType]--; }

    inline DWORD GetTotalUserNum() { return (DWORD)m_mapUsers.size(); }
    inline DWORD GetZoneUserNum(eZONETYPE eZoneType) { return m_dwZoneUserNum[eZoneType]; }
    inline DWORD GetZonePCRoomUserNum(eZONETYPE eZoneType) { return m_dwZonePCRoomUserNum[eZoneType]; }
    inline DWORD GetZoneCount(eZONETYPE eZoneType) { return m_dwZoneNum[eZoneType]; }

    //inline KEYTYPE            AllocKey()                      { return m_ChatRoomKeyGenerator.GetKey(); }
    //inline void FreeKey(KEYTYPE Key)          { m_ChatRoomKeyGenerator.RestoreKey(Key); }

    void CreateParty(WORD wPartyKey);
    BOOL JoinParty(WORD wPartyKey, ns_world::WorldUser* pJoinUser);
    BOOL WithdrawParty(ns_world::WorldUser* pWithdrawUser);
    void DestroyParty(WORD wPartyKey);
    void RestoreParty(WORD wPartyKey, ns_world::WorldUser *pUser);

    // 유저 숫자 통계 관련...아따 귀찮다...-_-;
    void RefreshUserNum();
    DWORD GetNumerOfNormalUsers() { return m_dwNormalUserNum; }
    void IncNumberOfNormalUsers() { m_dwNormalUserNum++; }
    void DecNumberOfNormalUsers() { if(m_dwNormalUserNum > 0) m_dwNormalUserNum--; }
    DWORD GetNumerOfPCRoomUsers() { return m_dwPCRoomUserNum; }
    void IncNumberOfPCRoomUsers() { m_dwPCRoomUserNum++; }
    void DecNumberOfPCRoomUsers() { if(m_dwPCRoomUserNum > 0) m_dwPCRoomUserNum--; }

private:

    // 
    ZONE_MAP m_mapZones;


    //util::CObjKeyGenerator<KEYTYPE>   m_ChatRoomKeyGenerator; // 채팅방 키 제조기
    ZONE_MAP m_mapFriendChat;        // 친구 채팅용
    USER_MAP m_mapUsers;
    DWORD m_dwChannelID;

    DWORD m_dwZonePCRoomUserNum[eZONETYPE_MAX];   // 각 지역의 PC방 유저 넘버
    DWORD m_dwZoneUserNum[eZONETYPE_MAX];
    DWORD m_dwZoneNum[eZONETYPE_MAX];

    DWORD m_dwNormalUserNum; // 접속중인 전체 일반 유저 숫자
    DWORD m_dwPCRoomUserNum; // 접속중인 전체 PC방 유저 숫자

    PartyManager* m_pPartyManager;
    VILLAGE_MAP m_mapVillages;  // 마을/필드 지역 임시 저장용 맵
};

}}; //end of namespace

#endif //WORLD_CONTENTS_BLOCK

#endif //WORLDSERVER_WORLD_CHANNEL_H