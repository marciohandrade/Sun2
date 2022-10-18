#pragma once
#ifndef GAMESERVER_GUILD_EXTENTION_H
#define GAMESERVER_GUILD_EXTENTION_H

#include "GameGuildManager.h"
#include "GameGuildRelation.h"

//==================================================================================================
namespace GuildSmallContentEntry {
    namespace GuildPenalty {};
    namespace GuildMark {};
    namespace RelationSystem {};
};
//==================================================================================================
//길드 패널티
namespace GuildSmallContentEntry { namespace GuildPenalty {
;

typedef DWORD64 PenaltyTime;
RC::eGUILD_RESULT PlayerPenaltyCheck(const BASE_PLAYERINFO*);

}}; //end of namespace

//==================================================================================================
namespace GuildSmallContentEntry { namespace GuildMark {
;

enum IsRegister { None_Register,Aleady_Register, };
struct GuildMarkInfo
{
    POSTYPE posType;
    SLOTIDX slot_Index;
    GUILDGUID guild_Index;
    GUILDMARKIDX pattern_index;
    GUILDMARKIDX background_index;

    GuildMarkInfo() : posType(0), slot_Index(0), guild_Index(0),
        background_index(0), pattern_index(0){}
};

void DeleteMark(const GuildMarkInfo& MarkInfo);
void RegisterMark(Player* pRequestPlayer, const GuildMarkInfo& MarkInfo);
void GuildInfoForRender(GUILD_RENDER_INFO&, GUILDGUID);
void GetGuildMark(GUILDGUID, GUILDMARKIDX&, GUILDMARKIDX&);

RC::eGUILD_RESULT CanRegister(Player*, const GuildMarkInfo&);
RC::eGUILD_RESULT CanDelete(Player*, const GuildMarkInfo&);
GuildMark::IsRegister IsRegisterMark(const GuildMarkInfo&);

}}; //end of namespace

//==================================================================================================
namespace GuildSmallContentEntry { namespace RelationSystem {
;

void SaveRelationInfo(BASE_GUILD_ADD_INFO* );
BOOL LoadRelationInfo(BASE_GUILD_ADD_INFO* );
bool IsHostilityRelation(GUILDGUID, GUILDGUID);
bool IsAllHostilityRelation(GUILDGUID, GUILDGUID);
void RemainRelationDestroy(GUILDGUID);
void DestroyWaitNotify(
    const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type&,
    const GUILDGUID);

eGUILD_RELATION GetRelationType(GUILDGUID, GUILDGUID);
RC::eGUILD_RESULT RemoveRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION);

#ifdef _NA_003923_20120130_GUILD_RENEWAL 
RC::eGUILD_RESULT AddRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION, DWORD64, DWORD64, std::string s = " ", std::string s2 = " ");
#else
RC::eGUILD_RESULT AddRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION, std::string s = " ", std::string s2 = " ");
#endif

}}; //end of namespace

//==================================================================================================

#endif //GAMESERVER_GUILD_EXTENTION_H