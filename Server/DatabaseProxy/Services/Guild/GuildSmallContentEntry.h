#pragma once
#ifndef DBPROXY_GUILD_SMALL_CONTENTS_ENTRY_H
#define DBPROXY_GUILD_SMALL_CONTENTS_ENTRY_H

#include <SolarDateTime.h>
#include <SCStream.h>

#include "GuildRelation.h"

class Guild;
class GameServerSession;

namespace GuildSmallContentEntry {
    namespace GuildPenalty {};
    namespace GuildMark {};
    namespace RelationSystem {};
};

//==================================================================================================
namespace GuildSmallContentEntry { namespace GuildPenalty {
;

typedef DWORD64 PenaltyTime;
typedef STLX_MAP<GUILDGUID, PenaltyTime> GUILD_DESTROY_WAIT_MAP;
typedef std::pair<GUILDGUID, PenaltyTime> GUILD_DESTROY_WAIT_PAIR;

struct PenaltyTimeUpdate 
{
	void operator()(const GUILD_DESTROY_WAIT_PAIR& data) const ;
};

bool CheckGuildPenaltyTime(Guild*);
bool GuildRecovery(CHARGUID, GUILDGUID);
void InsertGuildDeleteWaitMap(GUILDGUID);
bool IsGuildDestroyWaitState(GUILDGUID);
void SetGuildDeleteWaitMap(GUILDGUID, PenaltyTime);
RC::eGUILD_RESULT CanRecoveryGuild(GUILDGUID);

class GuildPenaltyMgr
{
public:
    typedef GuildSmallContentEntry::GuildPenalty::GUILD_DESTROY_WAIT_MAP
        GUILD_DESTROY_WAIT_MAP;
    void update();
    void erase(int delete_index);
    void insert(const GUILD_DESTROY_WAIT_MAP::value_type& guild_destroy_wait_pair);

private:
    GUILD_DESTROY_WAIT_MAP m_GuildWaitDestroyMap;
};

}}; //end of namespace 'GuildSmallContentsEntry::GuildPenalty'

//==================================================================================================
namespace GuildSmallContentEntry { namespace GuildMark {
;

struct GuildMarkInfo
{
	GUILDGUID guild_Index;
	CHARGUID request_player_index;
	GUILDMARKIDX pattern_index;
	GUILDMARKIDX background_index;
	GuildMarkInfo() : guild_Index(0), background_index(0), pattern_index(0), request_player_index(0){}
};

RC::eGUILD_RESULT CanModify(const GuildMarkInfo&);
void DeleteMark(const GuildMarkInfo& MarkInfo);
void RegisterMark(const GuildMarkInfo& MarkInfo);
void ResultCastToGameServer(GameServerSession*, RC::eGUILD_RESULT, DWORD);

}}; //end of namespace 'GuildSmallContentsEntry::GuildMark'

//==================================================================================================
namespace GuildSmallContentEntry { namespace RelationSystem {
;
void RemainRelationDestroy(GUILDGUID);
void SaveRelationInfo(BASE_GUILD_ADD_INFO* Info);
void LoadRelationInfo(BASE_GUILD_ADD_INFO* Info);
void DestroyWaitNotify(
    const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type&,
    const GUILDGUID);
bool Compare(
    const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type&,
    const GUILDGUID, eGUILD_RELATION);
	
eGUILD_RELATION GetRelationType(GUILDGUID, GUILDGUID);
RC::eGUILD_RESULT RemoveRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION);
#ifdef _NA_003923_20120130_GUILD_RENEWAL
DWORD64 CalcRelationChangeTime(const eGUILD_RELATION relation_type, const DWORD64 relation_apply_time);
RC::eGUILD_RESULT AddRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION, DWORD64, std::string s = " ");
void RelationInGameGuildLog(const GUILDGUID request_guild_guid,
                            const GUILDGUID target_guild_guid,
                            eGUILD_RELATION relation_type,
                            bool is_delete_relation);
#else
RC::eGUILD_RESULT AddRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION, std::string s = " ");
#endif

RC::eGUILD_RESULT CanDoRelationChange(GUILDGUID, CHARGUID);
RC::eGUILD_RESULT CanDoRelationBothHostility(GUILDGUID, GUILDGUID);
RC::eGUILD_RESULT CanDoRelationShip(GUILDGUID, GUILDGUID, eGUILD_RELATION);
RC::eGUILD_RESULT CanDoRelationDelete(GUILDGUID, GUILDGUID, eGUILD_RELATION);

}}; //end of namespace 'GuildSmallContentsEntry::RelationSystem'

#endif //DBPROXY_GUILD_SMALL_CONTENTS_ENTRY_H