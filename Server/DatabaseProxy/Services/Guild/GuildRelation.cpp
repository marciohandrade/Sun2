#include "stdafx.h"
#include "./GuildRelation.h"

#include "GuildSmallContentEntry.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildInfoParser.h"
#include "PacketStruct_DZ.h"
#include "PacketHandler/Handler_Guild.GuildToDbp.h"
#include "QueryObjects/DBHandler.h"
#endif

/*
[일방적 적대 관계에 있는 길드가 쌍방 적대 신청을 하면]

1. 적대 선포한 길드의 컨테이너에는 상대 길드에 대해 
쌍방 적대 타입과 적대 선포 타입이 같이 있어야 한다.(new)

2. 일방적 적대 관계에 있는 길드는 쌍방 적대 신청을 받으면,
일방적 적대 타입이 쌍방 적대 타입으로 바뀐다.(replace)

[나머지 관계들은 기본로직을 따른다.(관계 하나당 길드 인덱스 하나)]
*/

#ifdef _NA_003923_20120130_GUILD_RENEWAL
void 
GuildRelationSystem::GuildRelationEntry::UpdateRelation(GUILDGUID request_guild_guid)
{
    IndexByGuildGuid::const_iterator begin_it = indexedListByGuildGuid_.begin();
    const IndexByGuildGuid::const_iterator end_it = indexedListByGuildGuid_.end();

    for (; begin_it != end_it; ++begin_it)
    {
        GuildRelationSystem::GuildRelationInfo* const item = *begin_it;

        switch (item->RelationType)
        {
        case eGUILD_RELATION_BOTH_HOSTILITY:
            if (CanRelationChange(item->RelationChangeTime) == true)
            {
                MSG_DZ_GUILD_RELATION_SYN msg;
                msg.m_dwKey = 0;
                msg.m_RequestUserKey = 0;
                msg.m_eRelationType = eGUILD_RELATION_CANCEL_BOTH_HOSTILITY;
                msg.m_RequestGuildGuid	= request_guild_guid;
                msg.m_ResponseGuildGuid = item->GuildGuid;
                util::GUILDNAMECOPY(msg.m_ResponseGuildName, item->GuildName);

                Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_SYN(
                    g_DBHandler.GetSharedSession(),
                    &msg, 
                    sizeof(msg)
                    );
            }
            break;
        case eGUILD_RELATION_PROCLAIM_HOSTILITY:
        case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
            if (CanRelationChange(item->RelationChangeTime) == true)
            {
                MSG_DZ_GUILD_RELATION_BOTH_AGAINST_SYN msg;
                msg.m_MasterCharGuid = 0;
                msg.m_GuildGuid = request_guild_guid;
                msg.m_RelGuildGuid = item->GuildGuid;
                util::GUILDNAMECOPY(msg.m_tszRelGuildName, item->GuildName);

                Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_BOTH_AGAINST_SYN(
                    g_DBHandler.GetSharedSession(),
                    &msg, 
                    sizeof(msg)
                    );
            }
            break;
        }
    }
}

bool
GuildRelationSystem::GuildRelationEntry::CanRelationChange(DWORD64 relation_change_time)
{
    if (relation_change_time == 0)
    {
        return false;
    }

    DWORD64 current_time = 0;
    util::GetDateTime_YYYYMMDDHHMMSS(current_time);

    if (relation_change_time >= current_time)
    {
        return false;
    }

    return true;
}

void 
GuildRelationSystem::GuildRelationEntry::RelationInsert(GUILDGUID guild_guid, 
                                                        eGUILD_RELATION relation_type, 
                                                        ePROCLAIM_GUILD pro_claim,
                                                        DWORD64 relation_apply_time,
                                                        std::string guild_name)
{
    GuildRelationSystem::GuildRelationInfo* new_item = NULL;
    ByGuildGuidIterator find_it = indexedListByGuildGuid_.find(guild_guid);
    if (find_it != indexedListByGuildGuid_.end())
    {
        new_item = *find_it;
        new_item->RelationType = relation_type;
        new_item->GuildGuid = guild_guid;
        new_item->RelationApplyTime = relation_apply_time;
        new_item->RelationChangeTime = 
            GuildSmallContentEntry::RelationSystem::CalcRelationChangeTime(
            relation_type, 
            relation_apply_time
            );
        if (false == relationContainer_.replace(find_it, new_item))
        {
            SUNLOG(eDEV_LOG, "RelationInsert_replace Error");
        }
    }
    else
    {
        new_item = construct();

        if(new_item)
        {
            new_item->RelationType = relation_type;
            new_item->GuildGuid = guild_guid;
            new_item->eProClaim = pro_claim;
            new_item->RelationApplyTime = relation_apply_time;
            new_item->RelationChangeTime = 
                GuildSmallContentEntry::RelationSystem::CalcRelationChangeTime(
                relation_type, 
                relation_apply_time
                );
            util::GUILDNAMECOPY(new_item->GuildName, guild_name.c_str());

            relationContainer_.insert(new_item);
        }
    }
}
#else
void 
GuildRelationSystem::GuildRelationEntry::RelationInsert(GUILDGUID GuildGuid, 
                                                        eGUILD_RELATION type, 
                                                        ePROCLAIM_GUILD eProClaim, 
                                                        std::string GuildName)
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
#endif

bool 
GuildRelationSystem::GuildRelationEntry::RelationDelete(GUILDGUID GuildGuid, 
     													eGUILD_RELATION deleteType, 
														bool IsRequestGuild)
{
	ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
	if(it != indexedListByGuildGuid_.end())
	{
#ifndef _NA_003923_20120130_GUILD_RENEWAL
        if(eGUILD_RELATION_BOTH_HOSTILITY  == deleteType)
        {
            //쌍방 적대 해제 요청한 길드는 ProcClaim이 FALSE가 된다.
            ePROCLAIM_GUILD eProClaim = (IsRequestGuild) ? ONESIDE_GUILD : PROCLAIM_GUILD;
            eGUILD_RELATION change_type = (IsRequestGuild) ? eGUILD_RELATION_ONE_SIDE_HOSTILITY  
                                                           : eGUILD_RELATION_PROCLAIM_HOSTILITY;
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

int GuildRelationSystem::GuildRelationEntry::GetRelationCount(eGUILD_RELATION type) const
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

const TCHAR* GuildRelationSystem::GuildRelationEntry::GetRelationGuildName(GUILDGUID GuildGuid)const
{
    ByGuildGuidIterator it = indexedListByGuildGuid_.find(GuildGuid);
    if(it != indexedListByGuildGuid_.end())
    {
        const GuildRelationInfo* item = *it;
        return item->GuildName;
    }

    return NULL;
}

RC::eGUILD_RESULT GuildRelationSystem::GuildRelationEntry::IsRelationPossibility(
    GUILDGUID relation_guild, eGUILD_RELATION type)
{
	/*
	/* 내가 길드 관계를 맺고자 하는 길드가 
	/* 어떤 관계로든지 등록되어 있는 경우 관계를 맺을 수 없다.
	/* 쌍방 적대 검사는 여기서 하지 않는다.
	*/ 
// 	if(eGUILD_RELATION_NONE != GetRelationType(relation_guild))
// 		return RC::RC_GUILD_RELATION_ALEADY_SET;

#ifndef _NA_003923_20120130_GUILD_RENEWAL
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
			&GuildSmallContentEntry::RelationSystem::Compare,
            boost::lambda::_1, relation_guild, eGUILD_RELATION_ALLIENCE)))
			return RC::RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED;

		// 2.
		p = indexedListByRelationType_.equal_range(eGUILD_RELATION_ALLIENCE);
		if(p.second != std::find_if(p.first, p.second, boost::lambda::bind(
			&GuildSmallContentEntry::RelationSystem::Compare,
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
			&GuildSmallContentEntry::RelationSystem::Compare,
            boost::lambda::_1, relation_guild, eGUILD_RELATION_ALLIENCE)))
			return RC::RC_GUILD_RELATION_HOSTILITY_FAILED;
	}
#endif

	return RC::RC_GUILD_SUCCESS;
}

int GuildRelationSystem::GuildRelationEntry::LoadRelation(BASE_GUILD_ADD_INFO* adding_info)
{
    // CHANGES: f111011.2L, added boundary check to prevent stack overflow
    // WARNING: f111012.1L, this method does not handle about adding_info->m_GuildGuid.
    // upperbound = BASE_GUILD_ADD_INFO::kMaxNumberOfRelations
    ;{
        adding_info->m_byRelationGuildCnt = 0;
        adding_info->m_byAllienceGuildCnt = 0;	
        adding_info->m_byHostilityGuildCnt = 0;
        adding_info->m_byOneSideGuildCnt = 0;
    };
    int number_of_nodes = 0;
    //
    IndexByGuildGuid::const_iterator it = indexedListByGuildGuid_.begin();
    const IndexByGuildGuid::const_iterator end = indexedListByGuildGuid_.end();
    for (; it != end; ++it)
    {
        GuildRelationSystem::GuildRelationInfo* const item = *it;
        if (GUILD_RELATION* const dest_it = &adding_info->m_stRelationGuild[number_of_nodes])
        {
		    dest_it->m_GuildGuid = item->GuildGuid;
		    dest_it->m_eRelationType = item->RelationType;
		    dest_it->m_eProClaim = item->eProClaim;
            util::StrArrayForGuildName& dest_name = dest_it->m_GuildName;
		    util::GUILDNAMECOPY(dest_name, item->GuildName);
#ifdef _NA_003923_20120130_GUILD_RENEWAL
            dest_it->m_RelationApplyTime = item->RelationApplyTime;
            dest_it->m_RelationChangeTime = item->RelationChangeTime;
#endif
        };

        switch (item->RelationType)
		{
		case eGUILD_RELATION_ALLIENCE:
			++adding_info->m_byAllienceGuildCnt;
			break;
		case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
			++adding_info->m_byOneSideGuildCnt;
			break;
		case eGUILD_RELATION_BOTH_HOSTILITY:
		case eGUILD_RELATION_PROCLAIM_HOSTILITY:
			++adding_info->m_byHostilityGuildCnt;
			break;
		};

        ++number_of_nodes;
        if (number_of_nodes >= _countof(adding_info->m_stRelationGuild)) {
            break;
        };
    };

    // assertion 'need handling' (number_of_nodes == adding_info->m_byRelationGuildCnt)
    adding_info->m_byRelationGuildCnt = static_cast<uint8_t>(number_of_nodes);

	return number_of_nodes;
}


void GuildRelationSystem::GuildRelationEntry::RelationGuildsNotify(
    eGUILD_RELATION relation_type, GUILDGUID myGuildGuid)
{
	std::pair<ByRelationTypeIterator, ByRelationTypeIterator> p 
		= indexedListByRelationType_.equal_range(relation_type);

	std::for_each(p.first, p.second,
        boost::lambda::bind(&GuildSmallContentEntry::RelationSystem::DestroyWaitNotify,
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
    STLX_VECTOR<GUILDGUID>& vGuildGuid, eGUILD_RELATION RelationType) const
{
    IndexByGuildGuid::const_iterator it = indexedListByGuildGuid_.begin();
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

#ifdef _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM
void GuildRelationSystem::GuildRelationEntry::DeleteAllRelation(GUILDGUID guild_guid)
{
    IndexByGuildGuid::const_iterator begin_it = indexedListByGuildGuid_.begin();
    const IndexByGuildGuid::const_iterator end_it = indexedListByGuildGuid_.end();

    for (; begin_it != end_it; ++begin_it)
    {
        GuildRelationSystem::GuildRelationInfo* const item = *begin_it;

        if (item == NULL)
        {
            continue;
        }
        
        MSG_DZ_GUILD_RELATION_DELETE_SYN msg;
        msg.m_dwKey = 0;
        msg.m_eType = item->RelationType;
        msg.m_GuildGuid = guild_guid;
        msg.m_RelGuildGuid	 = item->GuildGuid;
        Handler_Guild_GuildToDbp::OnDZ_GUILD_RELATION_DELETE_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
    }
}
#endif // _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM
