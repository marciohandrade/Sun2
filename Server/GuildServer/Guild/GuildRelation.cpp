#include "StdAfx.h"

#include "GuildRelation.h"
#include "GuildSmallContentEntry.h"

/*
[일방적 적대 관계에 있는 길드가 쌍방 적대 신청을 하면]

1. 적대 선포한 길드의 컨테이너에는 상대 길드에 대해 
쌍방 적대 타입과 적대 선포 타입이 같이 있어야 한다.(new)

2. 일방적 적대 관계에 있는 길드는 쌍방 적대 신청을 받으면,
일방적 적대 타입이 쌍방 적대 타입으로 바뀐다.(replace)

[나머지 관계들은 기본로직을 따른다.(관계 하나당 길드 인덱스 하나)]
*/
void GuildRelationSystem::GuildRelationEntry::RelationInsert(
    GUILDGUID GuildGuid, eGUILD_RELATION type, ePROCLAIM_GUILD eProClaim, std::string GuildName)
{
	GuildRelationSystem::GuildRelationInfo* newItem(NULL);
	ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
	if(it != indexedListByGuildGuid_.end())
	{
		newItem = *it;
		newItem->RelationType = type;
		newItem->GuildGuid = GuildGuid;
		//newItem->eProClaim = eProClaim;
		if(false == relationContainer_.replace(it, newItem))
			SUNLOG(eDEV_LOG, "RelationInsert_replace Error");
	}
	else
	{
		newItem = construct();
		if(newItem)
		{
			newItem->RelationType = type;
			newItem->GuildGuid = GuildGuid;
			newItem->eProClaim = eProClaim;
			util::GUILDNAMECOPY(newItem->GuildName, GuildName.c_str());
			relationContainer_.insert(newItem);
		}
	}
}


void GuildRelationSystem::GuildRelationEntry::RelationDelete(
    GUILDGUID GuildGuid, eGUILD_RELATION deleteType, bool IsRequestGuild)
{
	ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
	if(it != indexedListByGuildGuid_.end())
	{
		if(eGUILD_RELATION_BOTH_HOSTILITY  == deleteType)
		{
			//쌍방 적대 해제 요청한 길드는 ProcClaim이 FALSE가 된다.
			ePROCLAIM_GUILD eProClaim = (IsRequestGuild) ? ONESIDE_GUILD : PROCLAIM_GUILD;
			eGUILD_RELATION change_type = (IsRequestGuild) ?  eGUILD_RELATION_ONE_SIDE_HOSTILITY  : eGUILD_RELATION_PROCLAIM_HOSTILITY;
			RelationInsert(GuildGuid, change_type, eProClaim);
			return;
		}

		if(*it) destroy(*it);
		indexedListByGuildGuid_.erase(it);
	}
}

int GuildRelationSystem::GuildRelationEntry::GetRelationCount(eGUILD_RELATION type)
{
	if( eGUILD_RELATION_NONE == type )
		return static_cast<int>(indexedListByRelationType_.size());
	return static_cast<int>(indexedListByRelationType_.count(type));
}

bool GuildRelationSystem::GuildRelationEntry::IsRelationCountOverFlow(
    eGUILD_RELATION type, std::size_t max_count)
{
	if(eGUILD_RELATION_ALLIENCE == type &&  
		max_count <= indexedListByRelationType_.count(type))
		return true;

	if(eGUILD_RELATION_PROCLAIM_HOSTILITY == type &&
		max_count <= indexedListByProcClaimType_.count(PROCLAIM_GUILD))
		return true;

	return false;
}

eGUILD_RELATION GuildRelationSystem::GuildRelationEntry::GetRelationType(GUILDGUID GuildGuid)
{
	ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
	if(it != indexedListByGuildGuid_.end())
	{
		GuildRelationInfo* item = *it;
		return item->RelationType;
	}

	return eGUILD_RELATION_NONE;
}

RC::eGUILD_RESULT GuildRelationSystem::GuildRelationEntry::IsRelationPossibility(
    GUILDGUID relation_guild, eGUILD_RELATION type)
{
	/*
	/* 내가 길드 관계를 맺고자 하는 길드가 
	/* 어떤 관계로든지 등록되어 있는 경우 관계를 맺을 수 없다.
	/* 쌍방 적대 검사는 여기서 하지 않는다.
	*/ 
	if(eGUILD_RELATION_NONE != GetRelationType(relation_guild))
		return RC::RC_GUILD_RELATION_ALEADY_SET;

	std::pair<ByRelationTypeIterator, ByRelationTypeIterator> p;
	
	/*
	/* 내가 동맹을 맺으려는 길드가
	/* 1. 나와 적대 관계를 맺고 있는 길드와
	/*     동맹 관계일때 적대 관계를 맺을 수 없다.
	/* 2. 나와 동맹을 맺고 있는 길드와
	/*     적대 관계일때 동맹 관계를 맺을 수 없다.
	*/
	if(type == eGUILD_RELATION_ALLIENCE)
	{
		// 1.
		ByRelationTypeIterator p_first = indexedListByRelationType_.lower_bound(eGUILD_RELATION_BOTH_HOSTILITY);
		ByRelationTypeIterator p_second = indexedListByRelationType_.upper_bound(eGUILD_RELATION_ONE_SIDE_HOSTILITY);
		
		if(p_second != std::find_if(p_first, p_second, boost::lambda::bind(
			&GuildSmallContentsEntry::RelationSystem::Compare,
            boost::lambda::_1, relation_guild, eGUILD_RELATION_ALLIENCE)))
			return RC::RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED;

		// 2.
		p = indexedListByRelationType_.equal_range(eGUILD_RELATION_ALLIENCE);
		if(p.second != std::find_if(p.first, p.second, boost::lambda::bind(
			&GuildSmallContentsEntry::RelationSystem::Compare,
            boost::lambda::_1, relation_guild, eGUILD_RELATION_BOTH_HOSTILITY)))
			return RC::RC_GUILD_RELATION_ALLIENCE_TYPE2_FAILED;;
	}
	/*
	/* 내가 적대를 맺으려는 길드가
	/* 나와 동맹 관계를 맺고 있는 길드와
	/* 동맹 관계일때 적대 관계를 맺을 수 없다.
	*/
	else
	{
		p = indexedListByRelationType_.equal_range(eGUILD_RELATION_ALLIENCE);
		if(p.second != std::find_if(p.first, p.second, boost::lambda::bind(
			&GuildSmallContentsEntry::RelationSystem::Compare,
            boost::lambda::_1, relation_guild, eGUILD_RELATION_ALLIENCE)))
			return RC::RC_GUILD_RELATION_HOSTILITY_FAILED;
	}

	return RC::RC_GUILD_SUCCESS;
}

int GuildRelationSystem::GuildRelationEntry::LoadRelation(BASE_GUILD_ADD_INFO* Info)
{
	int LoadCount = 0;
	for(ByGuildGuidIterator it = indexedListByGuildGuid_.begin(); it != indexedListByGuildGuid_.end(); ++it)
	{
		GuildRelationInfo* item = *it;
		Info->m_stRelationGuild[LoadCount].m_GuildGuid = item->GuildGuid;
		Info->m_stRelationGuild[LoadCount].m_eRelationType = item->RelationType;
		Info->m_stRelationGuild[LoadCount].m_eProClaim = item->eProClaim;
		util::GUILDNAMECOPY(Info->m_stRelationGuild[LoadCount++].m_GuildName, item->GuildName);
		
		switch(item->RelationType)
		{
		case eGUILD_RELATION_ALLIENCE:
			Info->m_byAllienceGuildCnt++;
			break;
		case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
			Info->m_byOneSideGuildCnt++;
			break;
		case eGUILD_RELATION_BOTH_HOSTILITY:
		case eGUILD_RELATION_PROCLAIM_HOSTILITY:
			Info->m_byHostilityGuildCnt++;
			break;
		}
	}

	return LoadCount;
}


void GuildRelationSystem::GuildRelationEntry::RelationGuildsNotify(
    eGUILD_RELATION relation_type, GUILDGUID myGuildGuid)
{
	std::pair<ByRelationTypeIterator, ByRelationTypeIterator> p 
		= indexedListByRelationType_.equal_range(relation_type);

	std::for_each(p.first, p.second,
        boost::lambda::bind(&GuildSmallContentsEntry::RelationSystem::DestroyWaitNotify,
            boost::lambda::_1, myGuildGuid));

	RelationRelease();
}

void GuildRelationSystem::GuildRelationEntry::RelationRelease()
{
	ByGuildGuidIterator it;
	for(it = indexedListByGuildGuid_.begin(); it != indexedListByGuildGuid_.end();)
	{
		destroy(*it++);
	}

	indexedListByGuildGuid_.clear();
}


bool GuildRelationSystem::GuildRelationEntry::ChangeGuildName(
    GUILDGUID GuildGuid, const TCHAR* pszGuildName)
{
	ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
	if(it != indexedListByGuildGuid_.end())
	{
		GuildRelationInfo* item = *it;
		util::GUILDNAMECOPY(item->GuildName, pszGuildName);

		return true;
	}
	
	return false;
}


int GuildRelationSystem::GuildRelationEntry::GetRelationGuid(
    STLX_VECTOR<GUILDGUID>& vGuildGuid, eGUILD_RELATION RelationType)
{
	ByGuildGuidIterator it = indexedListByGuildGuid_.begin();
	for( ; it != indexedListByGuildGuid_.end(); ++it)
	{
		GuildRelationInfo* item = *it;
		if( eGUILD_RELATION_NONE == RelationType ||	item->RelationType == RelationType )
		{
			vGuildGuid.push_back(item->GuildGuid);
		}
	}

	return static_cast<int>(vGuildGuid.size());
}

