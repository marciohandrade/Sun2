#include "StdAfx.h"

#include <VersionControl.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include "Guild.h"
#include "GuildManager.h"
#include "GuildSmallContentEntry.h"
#include "../Sessions/GameServerSession.h"

#include "GuildRightInfoParser.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RC::eGUILD_RESULT GuildSmallContentsEntry::GuildPenalty::
CanRecoveryGuild(GUILDGUID guild_index)
{
	Guild *pGuild = g_GuildManager.FindGuild(guild_index);
	if(pGuild)
	{
		if(pGuild->CheckGuildDeleteState() == eGUILD_DELCHK_DESTROY_WAIT)
			return RC::RC_GUILD_SUCCESS;
	}

	return RC::RC_GUILD_RECOVERY_FAILED;
}

bool GuildSmallContentsEntry::GuildPenalty::
CheckGuildPenaltyTime(Guild* pGuild)
{
//(lst1024) #ifdef -> #ifndef �� �´µ� �ϳ� �ϴ� �׳� ��. (���� ��Ȯ�� Ȯ�� �ʿ�)
#ifdef __NA_20070715_GUILD_PENALTY_TEMP_REMOVE
	PenaltyTime current_Time = 0;
	util::GetDateTime_YYYYMMDDHHMMSS(current_Time);
	if(pGuild->GetDestroyTime() >= current_Time)
		return false;
#endif	
	return true;
}

bool GuildSmallContentsEntry::GuildPenalty::IsGuildDestroyWaitState(GUILDGUID guild_guid)
{
	const Guild* guild = g_GuildManager.FindGuild(guild_guid);
	if (guild != NULL)
	{
        const PenaltyTime destroy_time = guild->GetDestroyTime();
		PenaltyTime current_time = 0;
		util::GetDateTime_YYYYMMDDHHMMSS(current_time);

		if (destroy_time <= current_time && (eGUILD_DELCHK_DESTROY_WAIT == guild->GetDeleteGuildState()))
		{

#ifdef _USING_GUILD_DESTROY_LOGGING
            SUNLOG(eCRITICAL_LOG, 
                _T("|����ػ�Ϸ�|GuildPenalty::IsGuildDestroyWaitState()|�ػ�ð� �� ��� ���� Ȯ�� ����|GuildGuid:%d|"), 
                guild_guid);
#endif // _USING_GUILD_DESTROY_LOGGING

			const GuildMember* guild_master = guild->FindMaster();
			if (guild_master != NULL && guild_master->IsLogIn())
			{

#ifdef _USING_GUILD_DESTROY_LOGGING
                SUNLOG(eCRITICAL_LOG, 
                    _T("|����ػ�Ϸ�|GuildPenalty::IsGuildDestroyWaitState()|��� ������ ���� Ȯ�� ����|GuildGuid:%d, MasterCharGuid:%d|"), 
                    guild_guid, guild_master->GetCharGuid());
#endif // _USING_GUILD_DESTROY_LOGGING

				MSG_DZ_GUILD_DESTROY_COMPLETE_SYN msg;
				msg.m_GuildGuid = guild_guid;
				msg.m_MasterCharGuid = guild_master->GetCharGuid();
				GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg));

				SUNLOG(eFULL_LOG, 
                    _T("IsGuildDestroyWaitState ��� ���� ��û ����:%s[%d] ���� ��û��:%s[%d]"),
					guild->GetGuildName(), 
                    guild_guid, 
                    guild_master->GetCharName(), 
                    guild_master->GetCharGuid());

				return true;
			}
			InsertGuildDeleteWaitMap(guild_guid);
		}
	}
	return false;
}

void GuildSmallContentsEntry::GuildPenalty::
SetGuildDeleteWaitMap(GUILDGUID guild_index, PenaltyTime penalty_time)
{
	Guild* pGuild = g_GuildManager.FindGuild(guild_index);
	if(pGuild)
	{
		pGuild->SetDestroyTime(penalty_time);
		pGuild->SetDeleteGuildState(eGUILD_DELCHK_DESTROY_WAIT);
		
		GuildPenaltyMgr* penaltyMgr = g_GuildManager.GetGuildPenaltyMgr();
        penaltyMgr->insert(GUILD_DESTROY_WAIT_PAIR(guild_index, penalty_time));

		GUILDLOG->LogGuildBreakUpWaiting(pGuild);
	}
}

void GuildSmallContentsEntry::GuildPenalty::
InsertGuildDeleteWaitMap(GUILDGUID guild_index)
{
	Guild* pGuild = g_GuildManager.FindGuild(guild_index);
	if(pGuild && eGUILD_DELCHK_DESTROY_WAIT== pGuild->GetDeleteGuildState())
	{
		GuildPenaltyMgr* penaltyMgr = g_GuildManager.GetGuildPenaltyMgr();
		penaltyMgr->insert(GUILD_DESTROY_WAIT_PAIR(guild_index, pGuild->GetDestroyTime()));
	}
}


bool GuildSmallContentsEntry::GuildPenalty::
GuildRecovery(CHARGUID char_index, GUILDGUID guild_index)
{
	Guild* pGuild = g_GuildManager.FindGuild(guild_index);
	if(pGuild)
	{
		pGuild->SetDeleteGuildState(eGUILD_DELCHK_NORMAL);
		
		GuildPenaltyMgr* penaltyMgr = g_GuildManager.GetGuildPenaltyMgr();
		penaltyMgr->erase(guild_index);

		GUILDLOG->LogGuildBreakUpCanceled(pGuild);

		return true;
	}

	return false;
}

void GuildSmallContentsEntry::GuildPenalty::PenaltyTimeUpdate::operator()(
    const GUILD_DESTROY_WAIT_PAIR& guild_destroy_pair) const
{
    const PenaltyTime destory_time = guild_destroy_pair.second;
	PenaltyTime current_time;
    util::GetDateTime_YYYYMMDDHHMM(current_time);
	
	if (destory_time <= current_time)
	{

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|����ػ�Ϸ�|PenaltyTimeUpdate::operator()|�ػ�ð� Ȯ�� ����|GuildGuid:%d|"), 
            guild_destroy_pair.first);
#endif // _USING_GUILD_DESTROY_LOGGING

		const Guild* guild = g_GuildManager.FindGuild(guild_destroy_pair.first);
		if (guild != NULL && (guild->GetDeleteGuildState() == eGUILD_DELCHK_DESTROY_WAIT))
		{

#ifdef _USING_GUILD_DESTROY_LOGGING
            SUNLOG(eCRITICAL_LOG, 
                _T("|����ػ�Ϸ�|PenaltyTimeUpdate::operator()|��� ���� Ȯ�� ����|GuildGuid:%d|"), 
                guild_destroy_pair.first);
#endif // _USING_GUILD_DESTROY_LOGGING

			const GuildMember* guild_master = guild->FindMaster();
			if (guild_master != NULL && guild_master->IsLogIn())
			{

#ifdef _USING_GUILD_DESTROY_LOGGING
                SUNLOG(eCRITICAL_LOG, 
                    _T("|����ػ�Ϸ�|PenaltyTimeUpdate::operator()|��� ������ ���� Ȯ�� ����|GuildGuid:%d, MasterCharGuid:%d|"), 
                    guild_destroy_pair.first, guild_master->GetCharGuid());
#endif // _USING_GUILD_DESTROY_LOGGING
                
				MSG_DZ_GUILD_DESTROY_COMPLETE_SYN msg;
				msg.m_GuildGuid = guild_destroy_pair.first;
				msg.m_MasterCharGuid = guild_master->GetCharGuid();
				GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(&msg, sizeof(msg));

				SUNLOG(eFULL_LOG, 
                    _T("PenaltyTimeUpdate::operator ��� ���� ��û ����:%s[%d] ���� ��û��:%s[%d]"),
					guild->GetGuildName(), 
                    guild_destroy_pair.first, 
                    guild_master->GetCharName(), 
                    guild_master->GetCharGuid());
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GuildSmallContentsEntry::GuildMark::
RegisterMark(const GuildMarkInfo& MarkInfo)
{
	Guild* pGuild = g_GuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{	
		pGuild->RegisterGuildMark(MarkInfo.background_index, MarkInfo.pattern_index);
		return;
	}
}

void GuildSmallContentsEntry::GuildMark::
DeleteMark(const GuildMarkInfo& MarkInfo)
{
	Guild* pGuild = g_GuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{	
		pGuild->DeleteGuildMark();
	}

	MessageOut(eDEV_LOG, "DeleteMark fail");
}

RC::eGUILD_RESULT GuildSmallContentsEntry::GuildMark::
CanModify(const GuildMarkInfo& MarkInfo)
{
	Guild* pGuild = g_GuildManager.FindGuild(MarkInfo.guild_Index);
	
	if(!pGuild) return RC::RC_GUILD_INVALID_STATE;

    //__NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG
    // ��� ���� üũ
    if( !GuildRightInfoParser::Instance()->CheckGradeRight(eGUILD_RIGHTS_GUILD_MARK,pGuild->GetGuildGrade()) )
        return RC::RC_GUILD_MARK_LEVEL_LOW;

	//��� ��ũ�� ������ �� �ִ� ������ �ִ� ���ΰ� �˻�
	if(!g_GuildManager.CheckRight(pGuild->GetGuildGuid(), 
		MarkInfo.request_player_index, eGUILD_RIGHTS_GUILD_MARK))
		return RC::RC_GUILD_CHANGE_MARK_NOT_RIGHT;

	return RC::RC_GUILD_SUCCESS;
}

void GuildSmallContentsEntry::GuildMark::
ResultCastToGameServer(GameServerSession* pServerSession, RC::eGUILD_RESULT result, DWORD userKey)
{
	MSG_GZ_GUILD_GUILDMARK_REGISTER_NAK msg;
	msg.m_dwKey = userKey;
	msg.m_Result = result;
	if(!pServerSession->SendPacket(&msg, sizeof(msg)))
		MessageOut(eDEV_LOG, "ResultCastToGameServer Error");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RC::eGUILD_RESULT GuildSmallContentsEntry::RelationSystem::
CanDoRelationShip(GUILDGUID request_guild_index, 
				  GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
	std::size_t max_count = (relation_type == eGUILD_RELATION_ALLIENCE) ? 10 : 5;

	// �ڽ��� ��� ���� ����
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	if(pRequestGuild->GetGuildGrade() <= 2)
		return RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_ME;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	if(pRequestEntry->IsRelationCountOverFlow(relation_type, max_count))
		return RC::RC_GUILD_RELATION_ME_FULL;

	RC::eGUILD_RESULT result = pRequestEntry->IsRelationPossibility(response_guild_index, relation_type);
	if(RC::RC_GUILD_SUCCESS != result) return result;
	
	// ��û ���� ��� ���� ����
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
    if (relation_type == eGUILD_RELATION_ALLIENCE && pResponseGuild == NULL)
    {
        // ������ ��쿡�� ��� ����� ���ǰ� �ʿ��ϱ� ������ �ε��� ��� ���� �ʿ�
        return RC::RC_GUILD_CURRENT_NONE_MASTER;
    }

	if(pResponseGuild->GetGuildGrade() <= 2)
		return RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER;

	GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
	
	if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type) 
		relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;

	if(pResponseEntry->IsRelationCountOverFlow(relation_type, max_count))
		return RC::RC_GUILD_RELATION_OTHER_FULL;

	result = pResponseEntry->IsRelationPossibility(request_guild_index, relation_type);
	if(RC::RC_GUILD_SUCCESS != result) return result;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentsEntry::RelationSystem::
CanDoRelationBothHostility(GUILDGUID request_guild_index, GUILDGUID response_guild_index)
{
	// �ڽ��� ��� ���� ����
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();

	// �ֹ� ���븦 ��û�� ���� ��� ��尡 �Ϲ��� ���� ���¿��� �Ѵ�.
	if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == 
		pRequestEntry->GetRelationType(response_guild_index))
		return RC::RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED;
	
	// ��û ���� ��� ���� ����
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
	if(!pResponseGuild)
	{
		return RC::RC_GUILD_SUCCESS; //�α� �ƿ� ���¿����� ��û ����
	}

	GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();

	// �ֹ� ���븦 ��û ���� ���� ��û�� ��尡 ���� ������ ���¿��� �Ѵ�.
	if(eGUILD_RELATION_PROCLAIM_HOSTILITY == 
		pRequestEntry->GetRelationType(request_guild_index))
		return RC::RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentsEntry::RelationSystem::
CanDoRelationDelete(GUILDGUID request_guild_index, 
					GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
	// �ڽ��� ��� ���� ����
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	if(eGUILD_RELATION_NONE == pRequestEntry->GetRelationType(response_guild_index))
		return RC::RC_GUILD_RELATION_NONE_DELETE_NOT;

	// ��û ���� ��� ���� ����
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
	if(!pResponseGuild)
	{
		return RC::RC_GUILD_SUCCESS; //�α� �ƿ� ���¿����� ��û ����
	}
	
	GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
	if(eGUILD_RELATION_NONE == pResponseEntry->GetRelationType(request_guild_index))
		return RC::RC_GUILD_RELATION_NONE_DELETE_NOT;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentsEntry::RelationSystem::
CanDoRelationChange(GUILDGUID request_guild_index, CHARGUID request_player_index)
{
	Guild* pGuild = g_GuildManager.FindGuild(request_guild_index);
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	// (lst1024) ��� ���� ��� ���¸� RelationChange�� �� ����.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	if(!GuildPenalty::CheckGuildPenaltyTime(pGuild))
		return RC::RC_GUILD_RELATION_GUILD_DESTROY_WAIT;

	if(!g_GuildManager.CheckRight(
		request_guild_index, request_player_index, eGUILD_RIGHTS_RELATION))
		return RC::RC_GUILD_RELATION_CHANGE_RIGHT_NOT;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentsEntry::RelationSystem::
AddRelation(GUILDGUID request_guild_index, GUILDGUID response_guild_index, 
			eGUILD_RELATION relation_type, std::string response_guild_name)
{
	// ��û�� ��� ---------------------------------------------------------------------
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;
	
	ePROCLAIM_GUILD eProClaim = NONE_PROCLAIM_GUILD;
	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type) eProClaim = PROCLAIM_GUILD;
	pRequestEntry->RelationInsert(response_guild_index, relation_type, eProClaim, response_guild_name);

	// ��û ���� ��� ---------------------------------------------------------------------
	// ���� ������ ��û �޴� ���� �Ϲ��� ���� ���� ���°� �ȴ�.
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
    if (pResponseGuild)
    {
        GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
        eProClaim = (PROCLAIM_GUILD == eProClaim) ? ONESIDE_GUILD : NONE_PROCLAIM_GUILD;
        if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type) relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
        pResponseEntry->RelationInsert(request_guild_index, relation_type, eProClaim, pRequestGuild->GetGuildName());
    }

	GUILDLOG->LogGuildRelation(pRequestGuild, response_guild_index, pResponseGuild, (BYTE)relation_type);

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentsEntry::RelationSystem::
RemoveRelation(GUILDGUID request_guild_index, 
			   GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
	// ��û�� ���
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	pRequestEntry->RelationDelete(response_guild_index, relation_type, true);

	// ��û ���� ���
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
	if (pResponseGuild)
	{
        GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
        pResponseEntry->RelationDelete(request_guild_index, relation_type, false);
	}

	GUILDLOG->LogGuildRelation(pRequestGuild, response_guild_index, pResponseGuild, (BYTE)relation_type);

	return RC::RC_GUILD_SUCCESS;
}

void GuildSmallContentsEntry::RelationSystem::
SaveRelationInfo(BASE_GUILD_ADD_INFO* Info)
{
	Guild* pRequestGuild = g_GuildManager.FindGuild(Info->m_GuildGuid);
	if(pRequestGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
		int total_count = Info->m_byRelationGuildCnt;

		GUILD_RELATION* pRelationList = Info->m_stRelationGuild;
		for(int i = 0; i < total_count; ++i)
		{
			GUILDGUID guild_index = pRelationList[i].m_GuildGuid;
			eGUILD_RELATION relation_type = pRelationList[i].m_eRelationType;
			std::string guild_name = pRelationList[i].m_GuildName;
			ePROCLAIM_GUILD eProClaim = pRelationList[i].m_eProClaim;
			pRequestEntry->RelationInsert(guild_index, relation_type, eProClaim , guild_name);
		}
	}
}

void GuildSmallContentsEntry::RelationSystem::
LoadRelationInfo(BASE_GUILD_ADD_INFO* Info)
{
	Guild* pRequestGuild = g_GuildManager.FindGuild(Info->m_GuildGuid);
	if(pRequestGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
		int info_count = pRequestEntry->LoadRelation(Info);
		Info->SetSize(Info->m_byAllienceGuildCnt, Info->m_byHostilityGuildCnt, Info->m_byOneSideGuildCnt);
	}
}

eGUILD_RELATION GuildSmallContentsEntry::RelationSystem::
GetRelationType(GUILDGUID guild_index, GUILDGUID other_guild_index)
{
	Guild* pGuild = g_GuildManager.FindGuild(guild_index);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
		return pEntry->GetRelationType(other_guild_index);
	}

	return eGUILD_RELATION_NONE;
}

bool GuildSmallContentsEntry::RelationSystem::
Compare(const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type& info,
		const GUILDGUID index, const eGUILD_RELATION compareType)
{
	GuildRelationSystem::GuildRelationInfo* item = info;

	if(compareType == eGUILD_RELATION_ALLIENCE)
		return compareType == GetRelationType(item->GuildGuid, index);
	else
		return compareType < GetRelationType(item->GuildGuid, index);
}

void  GuildSmallContentsEntry::RelationSystem::
RemainRelationDestroy(GUILDGUID guildguid)
{
	Guild* pGuild = g_GuildManager.FindGuild(guildguid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
		pEntry->RelationGuildsNotify(eGUILD_RELATION_ONE_SIDE_HOSTILITY, guildguid);
	}
}

void GuildSmallContentsEntry::RelationSystem::DestroyWaitNotify(
    const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type& info,
    const GUILDGUID index)
{
	GuildRelationSystem::GuildRelationInfo* item = info;
	Guild* pGuild = g_GuildManager.FindGuild(item->GuildGuid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRelationEntry = pGuild->GetRelationEntry();
		pRelationEntry ->RelationDelete(index, eGUILD_RELATION_ONE_SIDE_HOSTILITY, false);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


