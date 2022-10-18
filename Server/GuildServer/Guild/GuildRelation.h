#pragma once
#ifndef GUILDSERVER_GUILD_RELATION_H
#define GUILDSERVER_GUILD_RELATION_H

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace GuildRelationSystem
{
	struct GuildRelationInfo
	{
		GUILDGUID GuildGuid;
		ePROCLAIM_GUILD eProClaim;
		eGUILD_RELATION RelationType;
		TCHAR GuildName[MAX_GUILDNAME_LENGTH+1];
		
		GuildRelationInfo() : 
			GuildGuid(INVALID_GUILDGUID), 
			eProClaim(NONE_PROCLAIM_GUILD),
			RelationType(eGUILD_RELATION_NONE)
			{}
	};

	class GuildRelationEntry : protected boost::object_pool<GuildRelationInfo>
	{		
		struct TagGuildGuid{};
		struct TagRelationType{};
		struct TagbIsProcClaim{};

		typedef boost::multi_index::multi_index_container<
			GuildRelationInfo*, 
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<TagGuildGuid>,
                        BOOST_MULTI_INDEX_MEMBER(GuildRelationInfo, GUILDGUID, GuildGuid)>,
 				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<TagRelationType>,
                    BOOST_MULTI_INDEX_MEMBER(GuildRelationInfo, eGUILD_RELATION, RelationType)>,
				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<TagbIsProcClaim>,
                    BOOST_MULTI_INDEX_MEMBER(GuildRelationInfo, ePROCLAIM_GUILD, eProClaim)> >
		> GuildRelationContainer;

		typedef boost::multi_index::index<GuildRelationContainer, TagGuildGuid>::type IndexByGuildGuid;
		typedef boost::multi_index::index<GuildRelationContainer, TagRelationType>::type IndexByRelationType;
		typedef boost::multi_index::index<GuildRelationContainer, TagbIsProcClaim>::type IndexByProcClaimType;

		typedef IndexByGuildGuid::iterator ByGuildGuidIterator;
		typedef IndexByProcClaimType::iterator ByGuildProcClaimType;

	public:
		GuildRelationEntry()
			:indexedListByGuildGuid_( boost::multi_index::get<TagGuildGuid>(relationContainer_) ),
			indexedListByRelationType_( boost::multi_index::get<TagRelationType>(relationContainer_) ),
			indexedListByProcClaimType_( boost::multi_index::get<TagbIsProcClaim>(relationContainer_) )
		{
			indexedListByGuildGuid_.clear();
		}

		~GuildRelationEntry() {}

		typedef IndexByRelationType::iterator ByRelationTypeIterator;

	public:
		void RelationRelease();
		void RelationGuildsNotify(eGUILD_RELATION, GUILDGUID);
		void RelationDelete(GUILDGUID, eGUILD_RELATION, bool);
		void RelationInsert(GUILDGUID,  eGUILD_RELATION, ePROCLAIM_GUILD , std::string = "");

		int GetRelationCount(eGUILD_RELATION);
		int LoadRelation(BASE_GUILD_ADD_INFO*);
		bool IsRelationCountOverFlow(eGUILD_RELATION, std::size_t);

		eGUILD_RELATION GetRelationType(GUILDGUID);
		RC::eGUILD_RESULT IsRelationPossibility(GUILDGUID, eGUILD_RELATION);
		bool ChangeGuildName( GUILDGUID GuildGuid, const TCHAR* pszGuildName );
		int	 GetRelationGuid(STLX_VECTOR<GUILDGUID>& vGuildGuid, eGUILD_RELATION RelationType = eGUILD_RELATION_NONE);
	private:
		GuildRelationContainer relationContainer_;
		IndexByGuildGuid& indexedListByGuildGuid_;
		IndexByRelationType& indexedListByRelationType_;
		IndexByProcClaimType& indexedListByProcClaimType_;
	};
}

#endif //GUILDSERVER_GUILD_RELATION_H