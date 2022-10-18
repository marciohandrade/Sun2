#pragma once
#ifndef WORLDSERVER_WORLD_GUILD_MEMBER_H
#define WORLDSERVER_WORLD_GUILD_MEMBER_H

#include <Struct.h>
#include "./WorldGuildCommon.h"

namespace ns_world {
;

class WorldGuildMember
{
    __DECL_GUILDPOOL(WorldGuildMember);
public:
    WorldGuildMember();
    virtual ~WorldGuildMember();

    void Release();
    CHARGUID GetCharGuid() const;

    void SetCorpsNum(BYTE corps_index);
    BYTE GetCorpsNum() const;
    void Serialize(INOUT BASE_GUILD_MEMBER_INFO& base_guild_member_info, eSERIALIZE se);
    void SetDuty(eGUILD_DUTY guild_duty);
    eGUILD_DUTY GetDuty() const;
    const char* GetCharName() const;

private:
    BASE_GUILD_MEMBER_INFO base_guild_member_info_;
};

}; //end of namespace

//==================================================================================================

inline void ns_world::WorldGuildMember::Release() {}

inline CHARGUID ns_world::WorldGuildMember::GetCharGuid() const
{
    return base_guild_member_info_.m_CharGuid;
}

inline void ns_world::WorldGuildMember::SetCorpsNum(BYTE corps_index)
{
    base_guild_member_info_.m_byCorpsNum = corps_index;
}

inline BYTE ns_world::WorldGuildMember::GetCorpsNum() const
{
    return base_guild_member_info_.m_byCorpsNum;
}

inline void ns_world::WorldGuildMember::SetDuty(eGUILD_DUTY guild_duty)
{
    base_guild_member_info_.m_eGuildDuty = guild_duty;
}

inline eGUILD_DUTY ns_world::WorldGuildMember::GetDuty() const
{
    return base_guild_member_info_.m_eGuildDuty;
}

inline const char* ns_world::WorldGuildMember::GetCharName() const
{
    return base_guild_member_info_.m_tszCharName;
}

//==================================================================================================

#endif //WORLDSERVER_WORLD_GUILD_MEMBER_H