#pragma once
#ifndef WORLDSERVER_WORLD_GUILD_MANAGER_H
#define WORLDSERVER_WORLD_GUILD_MANAGER_H

//==================================================================================================
/// WorldGuildManager class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2005. 12. 27
    @remark
        - 게임서버에서 길드를 관리하는 클래스
    @note
        - 길드서버에서 받아온 길드정보 클래스를 관리한다.
        - 1. 게임서버에 접속한 캐릭터가 길드를 Find
        - 2. 메니저에 길드 정보가 있으면 그대로 사용하고, 없으면 길드서버에 요청한다.
    @history 
        -  2006. 01. 16 Modified by Sung Hyun Chang < whatthehell@webzen.co.kr > (월드 서버용으로 수정)
*/
//==================================================================================================

#include <struct.h>
#include <SolarHashTable.h>
#include <ResultCode.h>
#include <Singleton.h>

#include "Services/Guild/WorldGuild.h"
#include "Services/Guild/WorldGuildMember.h"

struct MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD; // packet, information messenger
struct PACKET_FOR_GUILD;

namespace ns_world {
;

class WorldGuild;

class WorldGuildManager : public util::Singleton<WorldGuildManager>
{
    enum { MAX_GUILD_POOL_NUM = 1000, };
public:
    typedef util::SolarHashTable<WorldGuild*> GUILD_HASH;

    WorldGuildManager();
    virtual ~WorldGuildManager();

    void Release();

    RC::eGUILD_RESULT SelectGuildInfo(ns_world::WorldUser* user, PACKET_FOR_GUILD& OUT rGuildInfo);
    RC::eGUILD_RESULT DestroyGuild(ns_world::WorldUser* user);

    void DisplayerGuildInfo();

    ns_world::WorldGuild* FindGuild(GUILDGUID guild_guid) const;

    //void SendGuildMessage(MSG_BASE* msg_base, uint16_t msg_size);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    void SetChuntingPVPInfo(GUILDGUID guild_guid, CHUNTING_PVP_INFO& IN PVPInfo);
#endif

    BOOL ChangeMemberDuty(GUILDGUID guild_guid, CHARGUID target_char_guid, eGUILD_DUTY guild_duty);
    BOOL SendGuildPacket(GUILDGUID guild_guid, MSG_BASE* msg_base, uint16_t msg_size);
    //void GetCharName(GUILDGUID guild_guid, CHARGUID CharGuid, OUT TCHAR* pszCharName, IN BYTE byBufSize);

    BOOL CreateRearCamp(GUILDGUID guild_guid, BYTE corps_index);
    BOOL DestroyRearCamp(GUILDGUID guild_guid, BYTE corps_index);
    void ChangeCommander(GUILDGUID guild_guid,  CHARGUID target_char_guid, BYTE corps_index);
    void ChangeCorps(GUILDGUID guild_guid, CHARGUID target_char_guid, BYTE corps_index);
    void CreateGuild(const WORLD_GUILD_INFO& rGuildInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo);
    void LoginGuild(const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& memberInfo);
    RC::eGUILD_RESULT LogoutGuild(GUILDGUID guild_guid, CHARGUID CharGuid);
    void RequestGuildRestore(GUILDGUID guild_guid, CHARGUID CharGuid);
    void GuildRestore(const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& memberInfo);
    void SerializeWorldGuildInfo(GUILDGUID guild_guid, INOUT WORLD_GUILD_INFO& Info, eSERIALIZE se);
    // NOTE: f110915.1L, replacement about SerializeBaseMemberInfo(SERIALIZE_STORE)
    void StoreBaseMemberInfo(GUILDGUID guild_guid, const BASE_GUILD_MEMBER_INFO& member_info);
    void GetCharName(GUILDGUID guild_guid, CHARGUID CharGuid, OUT char* pszCharName, int nSize);
    void KickMember(GUILDGUID guild_guid, CHARGUID CharGuid);
    void JoinGuild(GUILDGUID guild_guid, const BASE_GUILD_MEMBER_INFO& rMemberInfo);
    void WithdrawGuild(GUILDGUID guild_guid, CHARGUID CharGuid);
    void DestroyGuild(GUILDGUID guild_guid);
    RC::eGUILD_RESULT CanChangeNotice(GUILDGUID guild_guid, CHARGUID oper_char_guid,
        const util::StrArrayForGuildNotice& guild_notice, OUT size_t& notice_len_with_null);
    BOOL ChangeNotice(GUILDGUID guild_guid, const util::StrArrayForGuildNotice& pszNotice);
    void SendDominationHeimReturnMemo(const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD* msg);
    BOOL GetValueInMemo(const TCHAR* pszMemo, eSYSTEM_MEMO_TYPE& OUT type, MAPCODE& OUT mapcode);

protected:
    void RemoveGuild(GUILDGUID guild_guid);
    void AddGuild(WorldGuild* guild, GUILDGUID guild_guid);

private:
    GUILD_HASH guild_table_;
};

}; //end of namespace

//==================================================================================================

inline ns_world::WorldGuild* ns_world::WorldGuildManager::FindGuild(GUILDGUID guild_guid) const
{
    return guild_table_.GetData(guild_guid);
}

inline void ns_world::WorldGuildManager::RemoveGuild(GUILDGUID guild_guid)
{
    guild_table_.Remove(guild_guid);
}

inline void ns_world::WorldGuildManager::AddGuild(WorldGuild* guild, GUILDGUID guild_guid)
{
    guild_table_.Add(guild, guild_guid);
}

//extern WorldGuildManager g_WorldGuildManager;
#endif //WORLDSERVER_WORLD_GUILD_MANAGER_H