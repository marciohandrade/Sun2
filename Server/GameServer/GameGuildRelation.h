#pragma once
#ifndef GAMESERVER_GUILD_RELATION_H
#define GAMESERVER_GUILD_RELATION_H

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <functional>

namespace GuildRelationSystem {
;

struct GuildRelationInfo
{
    GUILDGUID GuildGuid;
    ePROCLAIM_GUILD eProClaim;
    eGUILD_RELATION RelationType;
    TCHAR GuildName[MAX_GUILDNAME_LENGTH+1];
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    DWORD64 RelationApplyTime;//길드 관계 성립 시간
    DWORD64 RelationChangeTime;//길드 관계 변경 예정 시간
#endif

    GuildRelationInfo() : 
        GuildGuid(INVALID_GUILDGUID), 
        eProClaim(NONE_PROCLAIM_GUILD),
        RelationType(eGUILD_RELATION_NONE)

    {
        ZeroMemory(GuildName, sizeof(GuildName));
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        RelationApplyTime = 0;
        RelationChangeTime = 0;
#endif
    }
};

class GuildRelationEntry : protected boost::object_pool<GuildRelationInfo>
{       
    struct TagGuildGuid{};
    struct TagRelationType{};

    typedef boost::multi_index_container<
        GuildRelationInfo*, 
        boost::multi_index::indexed_by<
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<TagGuildGuid>,
                BOOST_MULTI_INDEX_MEMBER(GuildRelationInfo, GUILDGUID, GuildGuid)>,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<TagRelationType>,
                BOOST_MULTI_INDEX_MEMBER(GuildRelationInfo, eGUILD_RELATION, RelationType)> >
    > GuildRelationContainer;

    typedef boost::multi_index::index<GuildRelationContainer, TagGuildGuid>::type IndexByGuildGuid;
    typedef boost::multi_index::index<GuildRelationContainer, TagRelationType>::type IndexByRelationType;

    typedef IndexByGuildGuid::iterator ByGuildGuidIterator;

public:
    typedef IndexByRelationType::iterator ByRelationTypeIterator;

    GuildRelationEntry()
        :indexedListByGuildGuid_( boost::multi_index::get<TagGuildGuid>(relationContainer_) ),
        indexedListByRelationType_( boost::multi_index::get<TagRelationType>(relationContainer_) ),
        RelationFirstSave_(FALSE)
    {}

    ~GuildRelationEntry() 
    {}

public:
    void RelationRelease();
    bool RelationDelete(GUILDGUID, eGUILD_RELATION, bool);
    void RelationGuildsNotify(eGUILD_RELATION, GUILDGUID);
#ifdef _NA_003923_20120130_GUILD_RENEWAL 
    void RelationInsert(GUILDGUID, eGUILD_RELATION, ePROCLAIM_GUILD, DWORD64, DWORD64, std::string = "");
#else
    void RelationInsert(GUILDGUID,  eGUILD_RELATION, ePROCLAIM_GUILD , std::string = "");
#endif

    int GetRelationCount(eGUILD_RELATION);
    int LoadRelation(BASE_GUILD_ADD_INFO*);
    bool IsRelationCountOverFlow(eGUILD_RELATION, std::size_t);
    void SaveComplete() { RelationFirstSave_ = TRUE; }
    BOOL IsSaveComplete() { return RelationFirstSave_ ; }

    eGUILD_RELATION GetRelationType(GUILDGUID);
    RC::eGUILD_RESULT IsRelationPossibility(GUILDGUID, eGUILD_RELATION);
    bool ChangeGuildName( GUILDGUID GuildGuid, const TCHAR* pszGuildName );
    int  GetRelationGuid(STLX_VECTOR<GUILDGUID>& vGuildGuid, eGUILD_RELATION RelationType = eGUILD_RELATION_NONE);
private:
    GuildRelationContainer relationContainer_;
    IndexByGuildGuid& indexedListByGuildGuid_;
    IndexByRelationType& indexedListByRelationType_;
    BOOL RelationFirstSave_;
};

}; //end of namespace 'GuildRelationSystem'

#endif //GAMESERVER_GUILD_RELATION_H