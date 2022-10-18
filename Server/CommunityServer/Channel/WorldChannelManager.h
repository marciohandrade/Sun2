#pragma once
#ifndef WORLDSERVER_WORLD_CHANNEL_MANAGER_H
#define WORLDSERVER_WORLD_CHANNEL_MANAGER_H

// 통계 간격
#include <Timer.h>
#include <ObjKeyGenerator.h>

#include <ServerStruct.h>
#include <ScriptCode.IParser.h>

#include "./ICommunity.h"

//==================================================================================================

namespace ns_functor {
;

struct ChatRoomMemberListFilter
{
    // NOTE: MAX_FRIENDBLOCK_STAT_NUM = { friends, blocks }
    typedef util::StrArrayForCharName MemberInfoArray[MAX_CHATZONE_USER];
};

}; //end of namespace
//==================================================================================================

namespace ns_world { namespace community {
;

class WorldChannel;
class FieldInfo;

}}; //end of namespace

//==================================================================================================

namespace ns_world {
;

class WorldChannelManager : public IParser
                          , public ns_world::IShutdown
{
public:
    struct ChannelPair {
        ns_world::community::WorldChannel* channel;
        ns_world::ServerSessionInWorld* session;
    };
    typedef STLX_LIST<ChannelPair> CHANNEL_MAP;
    typedef STLX_HASH_MAP<DWORD, ns_world::community::FieldInfo*> FieldInfoTable;

    static WorldChannelManager* Instance();

    virtual void Init();
    virtual void Release();
    // ns_world::IShutdown interface
    virtual void Shutdown();
    void Update();

    bool AddChannel(DWORD channel_id, ns_world::ServerSessionInWorld* binding_session);
    bool RemoveChannel(ns_world::ServerSessionInWorld* binding_session);
    static ns_world::community::WorldChannel* FindChannel(
        ns_world::ServerSessionInWorld* agent_session);
    ns_world::community::WorldChannel* FindChannel(DWORD channel_id) const;

    const CHANNEL_MAP& GetChannelMap() const;
//  void SendToAllChannel(BYTE *pMsg, WORD wSize);
    // 'NOTICE' facility, send to 'field server'
    void SendToStaticFields(MSG_BASE* msg_base, uint16_t msg_size);
    // 'NOTICE' facility, send to 'battle server'
    void SendToBattleZones(MSG_BASE* msg_base, uint16_t msg_size);
    //----------------------------------------------------------------------------------------------
    // ChatRoom service interfaces
    static ns_world::community::ChatRoomCommunity* GetLinkedChatRoom(ns_world::WorldUser* user);
    static ulong GetChatRoomGuid(const util::StrArrayForRoomTitle& chat_room_name);
    ns_world::community::ChatRoomCommunity* GetChatRoomCommunity(ulong chatroom_guid) const;
    ns_world::community::ChatRoomCommunity* GetChatRoomCommunityByName(
        const util::StrArrayForRoomTitle& chat_room_name) const;
    // NOTE: this method has internally message handling
    bool EnterChatRoomCommunity(ns_world::WorldUser* user,
        const util::StrArrayForRoomTitle& chat_room_name);
    bool LeaveChatRoomCommunity(ns_world::WorldUser* user);
    bool LoadChatRoomMembers(ns_world::WorldUser* user,
        ns_functor::ChatRoomMemberListFilter::MemberInfoArray& result_member_list,
        uint8_t* const result_number_of_members) const;
    //----------------------------------------------------------------------------------------------
    //
    void DisplayChannelInfo();
    bool LoadFieldInfo();

    ns_world::community::FieldInfo* GetFieldInfo(DWORD field_code);
    const ns_world::community::ShoutRangeTable& GetSkeletonShoutRangeTable() const;
    //BOOL FindLinkedZoneInfo(DWORD dwMapCode, ns_world::community::ShoutRange& LinkedZone);
    // IParser interface
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);

private:
    typedef util::CObjKeyGenerator<KEYTYPE> CommunityKeyGenerator;
    typedef STLX_HASH_MAP<ulong, ns_world::community::ChatRoomCommunity*> ChatRoomTable;
    //
    WorldChannelManager(void);
    ~WorldChannelManager(void);
    static WorldChannelManager* CreateInstance();

    //ns_world::community::WorldChannel* AllocChannel();
    //void FreeChannel(ns_world::community::WorldChannel* channel);
    bool _LoadLinkedZoneFile(bool reloaded); // load the 'shoutrange.txt' script

    // data fields...
    bool shutdown_process_;
    CHANNEL_MAP channel_list_;
    ChatRoomTable chat_room_table_;
    util::Timer statistics_timer_;

    ns_world::community::ShoutRangeTable shout_range_table_;
    CommunityKeyGenerator community_key_generator_; // = { ChatRoomCommunity (not Zone, Party) };
    FieldInfoTable field_info_table_;
    // last order...
    static WorldChannelManager* instance_static_;
    //
    __DISABLE_COPY(WorldChannelManager);
};

}; //end of namespace

//==================================================================================================

inline ns_world::WorldChannelManager* ns_world::WorldChannelManager::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

inline const ns_world::WorldChannelManager::CHANNEL_MAP&
ns_world::WorldChannelManager::GetChannelMap() const {
    return channel_list_;
}

inline const ns_world::community::ShoutRangeTable&
ns_world::WorldChannelManager::GetSkeletonShoutRangeTable() const {
    return shout_range_table_;
};

#endif //WORLDSERVER_WORLD_CHANNEL_MANAGER_H