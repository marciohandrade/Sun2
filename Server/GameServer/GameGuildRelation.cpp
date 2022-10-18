#include "stdafx.h"
#include "GameGuildRelation.h"
#include "GameGuildExtension.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL 
void 
GuildRelationSystem::GuildRelationEntry::RelationInsert(GUILDGUID guild_guid, 
                                                        eGUILD_RELATION relation_type, 
                                                        ePROCLAIM_GUILD pro_claim, 
                                                        DWORD64 relation_apply_time,
                                                        DWORD64 relation_change_time,
                                                        std::string guild_name)
{
    GuildRelationInfo* new_item = NULL;
    ByGuildGuidIterator it = indexedListByGuildGuid_.find(guild_guid);
    if(it != indexedListByGuildGuid_.end())
    {
        new_item = *it;
        new_item->RelationType = relation_type;
        new_item->GuildGuid = guild_guid;
        new_item->RelationApplyTime = relation_apply_time;
        new_item->RelationChangeTime = relation_change_time;

        if(false == relationContainer_.replace(it, new_item))
            SUNLOG(eDEV_LOG, "RelationInsert_replace Error");
    }
    else
    {
        new_item = construct();
        if(new_item)
        {
            new_item->RelationType = relation_type;
            new_item->GuildGuid = guild_guid;
            new_item->eProClaim = pro_claim;
            util::GUILDNAMECOPY(new_item->GuildName, guild_name.c_str());
            new_item->RelationApplyTime = relation_apply_time;
            new_item->RelationChangeTime = relation_change_time;

            relationContainer_.insert(new_item);
        }
    }
}
#else//_NA_003923_20120130_GUILD_RENEWAL
void GuildRelationSystem::GuildRelationEntry::RelationInsert(
    GUILDGUID GuildGuid, eGUILD_RELATION type, ePROCLAIM_GUILD eProClaim, std::string GuildName)
{
    GuildRelationInfo* newItem(NULL);
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
#endif//_NA_003923_20120130_GUILD_RENEWAL

bool GuildRelationSystem::GuildRelationEntry::RelationDelete(
    GUILDGUID GuildGuid, eGUILD_RELATION deleteType, bool IsRequestGuild)
{
	ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
	if(it != indexedListByGuildGuid_.end())
	{
		GuildRelationInfo* item = *it;
		
#ifndef _NA_003923_20120130_GUILD_RENEWAL
        if(eGUILD_RELATION_BOTH_HOSTILITY  == deleteType)
        {
            //쌍방 적대 해제 요청한 길드는 ProcClaim이 ONESIDE_GUILD 
            ePROCLAIM_GUILD eProClaim = 
                (IsRequestGuild) ? ONESIDE_GUILD : PROCLAIM_GUILD;
            eGUILD_RELATION change_type = 
                (IsRequestGuild) ? eGUILD_RELATION_ONE_SIDE_HOSTILITY  : eGUILD_RELATION_PROCLAIM_HOSTILITY;

            RelationInsert(GuildGuid, change_type, eProClaim);
            return true;
        }
#endif
		if(*it) destroy(*it);
		indexedListByGuildGuid_.erase(it);

        return true;
	}

    return false;
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
	//if(max_count < indexedListByProcClaimType_.count(TRUE))
	//	return true;

	//if(max_count <= indexedListByRelationType_.count(type))
	//	return true;

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

int GuildRelationSystem::GuildRelationEntry::LoadRelation(BASE_GUILD_ADD_INFO* Info)
{
	int LoadCount = 0;
	for(ByGuildGuidIterator it = indexedListByGuildGuid_.begin(); it != indexedListByGuildGuid_.end(); ++it)
	{
		GuildRelationInfo* item = *it;
		Info->m_stRelationGuild[LoadCount].m_GuildGuid = item->GuildGuid;
		Info->m_stRelationGuild[LoadCount].m_eRelationType = item->RelationType;
		Info->m_stRelationGuild[LoadCount].m_eProClaim = item->eProClaim;
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        Info->m_stRelationGuild[LoadCount].m_RelationApplyTime = item->RelationApplyTime;
        Info->m_stRelationGuild[LoadCount].m_RelationChangeTime = item->RelationChangeTime;
#endif
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

void GuildRelationSystem::GuildRelationEntry::RelationRelease()
{
	ByGuildGuidIterator it;
	for(it = indexedListByGuildGuid_.begin(); it != indexedListByGuildGuid_.end();)
	{
		destroy(*it++);
	}

	indexedListByGuildGuid_.clear();
}

void GuildRelationSystem::GuildRelationEntry::RelationGuildsNotify(
    eGUILD_RELATION relation_type, GUILDGUID myGuildGuid)
{
	std::pair<ByRelationTypeIterator, ByRelationTypeIterator> p 
		= indexedListByRelationType_.equal_range(relation_type);

	std::for_each(p.first, p.second,
        boost::bind(&GuildSmallContentEntry::RelationSystem::DestroyWaitNotify,
            _1, myGuildGuid));

	RelationRelease();
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

	return vGuildGuid.size();
}
