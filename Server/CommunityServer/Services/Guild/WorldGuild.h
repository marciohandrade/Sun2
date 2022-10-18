#pragma once
#ifndef WORLDSERVER_WORLD_GUILD_H
#define WORLDSERVER_WORLD_GUILD_H
//==================================================================================================
/// WorldGuild class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2005. 12. 27
    @remark
        - 길드 정보를 활용하는 목적의 게임 길드용 클래스
    @note
        - 
    @history 
        -  2006. 01. 16 Modified by Sung Hyun Chang < whatthehell@webzen.co.kr > (월드 서버용으로 수정)
        - 2011.08.25, waverix, migrated to the renewaled world server
*/
//==================================================================================================

#include <Struct.h>
#include "WorldGuildCommon.h"

struct PACKET_FOR_GUILD;
struct PACKET_FOR_GUILD_MEMBER;

//==================================================================================================

namespace ns_world {
;
class WorldGuildMember;

class WorldGuild
{
private:
    __DECL_GUILDPOOL(WorldGuild);
    enum { _MAX_GUILDMEMBER_NUM = 140, };
public:
    typedef util::SolarHashTable<ns_world::WorldGuildMember*> GUILDMEMBER_HASH;
    //
    WorldGuild();
    virtual ~WorldGuild();

    void Destroy();
    void Withdraw(CHARGUID char_guid);

    GUILDGUID GetGuildGuid() const;
    DWORD GetMemberNum() const;
    bool IsFullMember() const;
    const util::StrArrayForGuildName& GetGuildName() const;
    void ChangeGuildName(const util::StrArrayForGuildName& guild_name);
    ns_world::WorldGuildMember* FindMember(CHARGUID guild_guid) const;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    void SetChuntingPVPInfo(CHUNTING_PVP_INFO& IN PVPInfo);
#endif
    //void OnGuildAdditionInfo(BYTE byRelCnt, BYTE byCorpsCnt, const BASE_GUILD_ADD_INFO & IN m_AddInfo);
    BOOL ChangeMemberDuty(CHARGUID target_char_guid, eGUILD_DUTY guild_duty);

    void CreateRearCamp(BYTE corps_index);
    void DestroyRearCamp(BYTE corps_index);
    void Create(const WORLD_GUILD_INFO& world_guild_info, const BASE_GUILD_MEMBER_INFO& member_info);
    void SendAllMember(MSG_BASE* msg_base, uint16_t msg_size);
    void SendCorpsMember(BYTE corps_index, MSG_BASE* msg_base, uint16_t msg_size);
    void SerializeWorldGuildInfo(INOUT WORLD_GUILD_INFO& world_guild_info, eSERIALIZE se);
    // NOTE: f110915.1L, replacement about SerializeBaseMemberInfo(SERIALIZE_STORE)
    void StoreBaseMemberInfo(const BASE_GUILD_MEMBER_INFO& member_info);
    void Join(const BASE_GUILD_MEMBER_INFO& member_info);
    void CreateMember(const BASE_GUILD_MEMBER_INFO& member_info);
    void DestroyMember(const CHARGUID& char_guid);

    const util::StrArrayForGuildNotice& GetGuildNotice();
    void SetGuildNotice(const util::StrArrayForGuildNotice& ptszNotice);

public:
    void RemoveMember(CHARGUID char_guid);
    void AddMember(WorldGuildMember* guild, CHARGUID char_guid);

private:
    GUILDMEMBER_HASH member_table_;
    WORLD_GUILD_INFO world_guild_info_;
    bool rear_camp_[MAX_GUILD_CORPS_NUM]; //훈련캠프 창설여부.
    //
    __DISABLE_COPY(WorldGuild);
};

}; //end of namespace

//==================================================================================================
// inlining implements
inline GUILDGUID ns_world::WorldGuild::GetGuildGuid() const
{
    return world_guild_info_.m_GuildGuid;
}

inline DWORD ns_world::WorldGuild::GetMemberNum() const
{
    return member_table_.GetDataNum();
}

inline bool ns_world::WorldGuild::IsFullMember() const
{
    return true;
}

inline const util::StrArrayForGuildName& ns_world::WorldGuild::GetGuildName() const
{
    return world_guild_info_.m_tszGuildName;
}

inline ns_world::WorldGuildMember*
ns_world::WorldGuild::FindMember(CHARGUID guild_guid) const
{
    return member_table_.GetData(guild_guid);
}

inline void ns_world::WorldGuild::RemoveMember(CHARGUID char_guid)
{
    member_table_.Remove(char_guid);
}

inline void ns_world::WorldGuild::AddMember(WorldGuildMember* guild, CHARGUID char_guid)
{
    member_table_.Add(guild, char_guid);
}


//==================================================================================================

#endif //WORLDSERVER_WORLD_GUILD_H