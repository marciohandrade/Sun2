#pragma once

#include <SolarDateTime.h>
#include <GuildServer.h>
#include <SCStream.h>

#include "GuildRelation.h"

class Guild;
class GameServerSession;

namespace GuildSmallContentsEntry {
    namespace GuildPenalty {};
    namespace GuildMark {};
    namespace RelationSystem {};
};

//==================================================================================================
namespace GuildSmallContentsEntry { namespace GuildPenalty {
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
	void update()
	{
		for_each(m_GuildWaitDestroyMap.begin(), m_GuildWaitDestroyMap.end(), PenaltyTimeUpdate());
	}

	void erase(int delete_index) 
	{ 
		m_GuildWaitDestroyMap.erase(delete_index); 
	}

	void insert(const GUILD_DESTROY_WAIT_PAIR& guild_destroy_wait_pair) 
	{ 
		if (m_GuildWaitDestroyMap.size() == 0)
        {
            GuildServer::GetMainFrame()->GuildDestoryTimeStart();
        }

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산신청|GuildPenaltyMgr::insert()|m_GuildWaitDestroyMap.insert()|GuildGuid:%d, PenaltyTime:%u|"), 
               guild_destroy_wait_pair.first, 
               guild_destroy_wait_pair.second);
#endif // _USING_GUILD_DESTROY_LOGGING
        
		m_GuildWaitDestroyMap.insert(guild_destroy_wait_pair); 
	}

private:
	GUILD_DESTROY_WAIT_MAP m_GuildWaitDestroyMap;
};

}}; //end of namespace 'GuildSmallContentsEntry::GuildPenalty'

//==================================================================================================
namespace GuildSmallContentsEntry { namespace GuildMark {
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
namespace GuildSmallContentsEntry { namespace RelationSystem {
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
RC::eGUILD_RESULT AddRelation(GUILDGUID, GUILDGUID, eGUILD_RELATION, std::string s = " ");

RC::eGUILD_RESULT CanDoRelationChange(GUILDGUID, CHARGUID);
RC::eGUILD_RESULT CanDoRelationBothHostility(GUILDGUID, GUILDGUID);
RC::eGUILD_RESULT CanDoRelationShip(GUILDGUID, GUILDGUID, eGUILD_RELATION);
RC::eGUILD_RESULT CanDoRelationDelete(GUILDGUID, GUILDGUID, eGUILD_RELATION);

}}; //end of namespace 'GuildSmallContentsEntry::RelationSystem'

