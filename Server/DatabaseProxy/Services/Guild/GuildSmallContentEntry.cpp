#include "stdafx.h"
#include "./GuildSmallContentEntry.h"

#include <VersionControl.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include "GuildRightInfoParser.h"

#include "Guild.h"
#include "GuildManager.h"
#include "Sessions/GameServerSession.h"

#include "QueryObjects/DBHandler.h"
#include "PacketHandler/Handler_Guild.GuildToDbp.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildInfoParser.h"
#include "GuildRightInfoParser.h"
#include "SolarDateTime.h"
#include "GuildInGameLog.h"
#endif

//==================================================================================================
//==================================================================================================
//==================================================================================================

void GuildSmallContentEntry::GuildPenalty::GuildPenaltyMgr::update()
{
    std::for_each(m_GuildWaitDestroyMap.begin(), m_GuildWaitDestroyMap.end(), PenaltyTimeUpdate());
}

void GuildSmallContentEntry::GuildPenalty::GuildPenaltyMgr::erase(int delete_index) 
{ 
    m_GuildWaitDestroyMap.erase(delete_index); 
}

void GuildSmallContentEntry::GuildPenalty::GuildPenaltyMgr::insert(
    const GUILD_DESTROY_WAIT_MAP::value_type& guild_destroy_wait_pair) 
{ 
    g_GuildManager.StartGuildDestroyTimer();

#ifdef _USING_GUILD_DESTROY_LOGGING
    SUNLOG(eCRITICAL_LOG, 
        _T("|길드해산신청|"__FUNCTION__"|GuildGuid:%d, PenaltyTime:%u|"),
        guild_destroy_wait_pair.first, 
        guild_destroy_wait_pair.second);
#endif // _USING_GUILD_DESTROY_LOGGING

    m_GuildWaitDestroyMap.insert(guild_destroy_wait_pair);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================


RC::eGUILD_RESULT GuildSmallContentEntry::GuildPenalty::
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

bool GuildSmallContentEntry::GuildPenalty::CheckGuildPenaltyTime(Guild* pGuild)
{
    __TOUCHED(pGuild);
//(lst1024) #ifdef -> #ifndef 가 맞는듯 하나 일단 그냥 둠. (좀더 정확한 확인 필요)
#ifdef __NA_20070715_GUILD_PENALTY_TEMP_REMOVE
	PenaltyTime current_Time = 0;
	util::GetDateTime_YYYYMMDDHHMMSS(current_Time);
	if(pGuild->GetDestroyTime() >= current_Time)
		return false;
#endif	
	return true;
}

bool GuildSmallContentEntry::GuildPenalty::IsGuildDestroyWaitState(GUILDGUID guild_guid)
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
                _T("|길드해산완료|GuildPenalty::IsGuildDestroyWaitState()|해산시간 및 길드 상태 확인 성공|GuildGuid:%d|"), 
                guild_guid);
#endif // _USING_GUILD_DESTROY_LOGGING

			const GuildMember* guild_master = guild->FindMaster();
			if (guild_master != NULL && guild_master->IsLogIn())
			{

#ifdef _USING_GUILD_DESTROY_LOGGING
                SUNLOG(eCRITICAL_LOG, 
                    _T("|길드해산완료|GuildPenalty::IsGuildDestroyWaitState()|길드 마스터 상태 확인 성공|GuildGuid:%d, MasterCharGuid:%d|"), 
                    guild_guid, guild_master->GetCharGuid());
#endif // _USING_GUILD_DESTROY_LOGGING

				MSG_DZ_GUILD_DESTROY_COMPLETE_SYN msg;
				msg.m_GuildGuid = guild_guid;
				msg.m_MasterCharGuid = guild_master->GetCharGuid();
                Handler_Guild_GuildToDbp::OnDZ_GUILD_DESTROY_COMPLETE_SYN(
                    g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
				SUNLOG(eFULL_LOG, 
                    _T("IsGuildDestroyWaitState 길드 삭제 요청 길드명:%s[%d] 삭제 요청자:%s[%d]"),
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

void GuildSmallContentEntry::GuildPenalty::
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

void GuildSmallContentEntry::GuildPenalty::
InsertGuildDeleteWaitMap(GUILDGUID guild_index)
{
	Guild* pGuild = g_GuildManager.FindGuild(guild_index);
	if(pGuild && eGUILD_DELCHK_DESTROY_WAIT== pGuild->GetDeleteGuildState())
	{
		GuildPenaltyMgr* penaltyMgr = g_GuildManager.GetGuildPenaltyMgr();
		penaltyMgr->insert(GUILD_DESTROY_WAIT_PAIR(guild_index, pGuild->GetDestroyTime()));
	}
}


bool GuildSmallContentEntry::GuildPenalty::
GuildRecovery(CHARGUID char_index, GUILDGUID guild_index)
{
    __UNUSED(char_index);
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

void GuildSmallContentEntry::GuildPenalty::PenaltyTimeUpdate::operator()(
    const GUILD_DESTROY_WAIT_PAIR& guild_destroy_pair) const
{
    const PenaltyTime destory_time = guild_destroy_pair.second;
	PenaltyTime current_time;
    util::GetDateTime_YYYYMMDDHHMM(current_time);
	
	if (destory_time <= current_time)
	{

#ifdef _USING_GUILD_DESTROY_LOGGING
        SUNLOG(eCRITICAL_LOG, 
            _T("|길드해산완료|PenaltyTimeUpdate::operator()|해산시간 확인 성공|GuildGuid:%d|"), 
            guild_destroy_pair.first);
#endif // _USING_GUILD_DESTROY_LOGGING

		const Guild* guild = g_GuildManager.FindGuild(guild_destroy_pair.first);
		if (guild != NULL && (guild->GetDeleteGuildState() == eGUILD_DELCHK_DESTROY_WAIT))
		{

#ifdef _USING_GUILD_DESTROY_LOGGING
            SUNLOG(eCRITICAL_LOG, 
                _T("|길드해산완료|PenaltyTimeUpdate::operator()|길드 상태 확인 성공|GuildGuid:%d|"), 
                guild_destroy_pair.first);
#endif // _USING_GUILD_DESTROY_LOGGING

			const GuildMember* guild_master = guild->FindMaster();
			if (guild_master != NULL && guild_master->IsLogIn())
			{

#ifdef _USING_GUILD_DESTROY_LOGGING
                SUNLOG(eCRITICAL_LOG, 
                    _T("|길드해산완료|PenaltyTimeUpdate::operator()|길드 마스터 상태 확인 성공|GuildGuid:%d, MasterCharGuid:%d|"), 
                    guild_destroy_pair.first, guild_master->GetCharGuid());
#endif // _USING_GUILD_DESTROY_LOGGING
                
				MSG_DZ_GUILD_DESTROY_COMPLETE_SYN msg;
				msg.m_GuildGuid = guild_destroy_pair.first;
				msg.m_MasterCharGuid = guild_master->GetCharGuid();
                Handler_Guild_GuildToDbp::OnDZ_GUILD_DESTROY_COMPLETE_SYN(
                    g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
				SUNLOG(eFULL_LOG, 
                    _T("PenaltyTimeUpdate::operator 길드 삭제 요청 길드명:%s[%d] 삭제 요청자:%s[%d]"),
					guild->GetGuildName(), 
                    guild_destroy_pair.first, 
                    guild_master->GetCharName(), 
                    guild_master->GetCharGuid());
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GuildSmallContentEntry::GuildMark::
RegisterMark(const GuildMarkInfo& MarkInfo)
{
	Guild* pGuild = g_GuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{	
		pGuild->RegisterGuildMark(MarkInfo.background_index, MarkInfo.pattern_index);
    #ifdef _NA_003923_20120130_GUILD_RENEWAL
        pGuild->GetGuildInGameLog()->WriteGuildLog(GuildRegisterMark, pGuild->GetGuildGuid());
    #endif
		return;
	}
}

void GuildSmallContentEntry::GuildMark::
DeleteMark(const GuildMarkInfo& MarkInfo)
{
	Guild* pGuild = g_GuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{	
		pGuild->DeleteGuildMark();
	}

	MessageOut(eDEV_LOG, "DeleteMark fail");
}

RC::eGUILD_RESULT GuildSmallContentEntry::GuildMark::
CanModify(const GuildMarkInfo& MarkInfo)
{
	Guild* pGuild = g_GuildManager.FindGuild(MarkInfo.guild_Index);
	
	if(!pGuild) return RC::RC_GUILD_INVALID_STATE;

    //__NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG
    // 길드 레벨 체크
    GuildRightInfoParser* const pRightInfoParser = GuildRightInfoParser::Instance();
    if (!pRightInfoParser->CheckGradeForRight(eGUILD_RIGHTS_GUILD_MARK,pGuild->GetGuildGrade()))
    {
        return RC::RC_GUILD_MARK_LEVEL_LOW;
    }

	//길드 마크를 변경할 수 있는 권한이 있는 놈인가 검사
	if(!g_GuildManager.CheckRight(pGuild->GetGuildGuid(), 
		MarkInfo.request_player_index, eGUILD_RIGHTS_GUILD_MARK))
		return RC::RC_GUILD_CHANGE_MARK_NOT_RIGHT;

	return RC::RC_GUILD_SUCCESS;
}

void GuildSmallContentEntry::GuildMark::
ResultCastToGameServer(GameServerSession* pServerSession, RC::eGUILD_RESULT result, DWORD userKey)
{
	MSG_GZ_GUILD_GUILDMARK_REGISTER_NAK msg;
	msg.m_dwKey = userKey;
	msg.m_Result = BYTE(result); // NOTE: RC::eGUILD_RESULT -> BYTE
	if(!pServerSession->SendPacket(&msg, sizeof(msg)))
		MessageOut(eDEV_LOG, "ResultCastToGameServer Error");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT 
GuildSmallContentEntry::RelationSystem::CanDoRelationShip(GUILDGUID request_guild_guid, 
													      GUILDGUID target_guild_guid, 
														  eGUILD_RELATION relation_type)
{
	const GuildInfoData& guild_info = GuildInfoParser::Instance()->GetGuildInfo();

	std::size_t max_count = 
		(relation_type == eGUILD_RELATION_ALLIENCE) ? 
		guild_info.max_alliance_ : guild_info.max_apply_hostile_;

    GuildRightInfoParser* const right_parser = GuildRightInfoParser::Instance();

    //Request guild
	Guild* request_guild = g_GuildManager.FindGuild(request_guild_guid);
	if (request_guild == NULL)
	{
		return RC::RC_GUILD_NOT_EXIST_GUILD;
	}

	if (!right_parser->CheckGradeForRight(eGUILD_RIGHTS_RELATION, request_guild->GetGuildGrade()))
	{
		return RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_ME;
	}

	GuildRelationSystem::GuildRelationEntry* request_entry = request_guild->GetRelationEntry();
	if (request_entry->IsRelationCountOverFlow(relation_type, max_count))
	{
		return RC::RC_GUILD_RELATION_ME_FULL;
	}

	RC::eGUILD_RESULT result = 
		request_entry->IsRelationPossibility(target_guild_guid, relation_type);
	if (result != RC::RC_GUILD_SUCCESS) 
	{
		return result;
	}

	//Target guild
	Guild* target_guild = g_GuildManager.FindGuild(target_guild_guid);
	if (relation_type == eGUILD_RELATION_ALLIENCE && target_guild == NULL)
	{
		return RC::RC_GUILD_CURRENT_NONE_MASTER;
	}

    if (!right_parser->CheckGradeForRight(eGUILD_RIGHTS_RELATION, target_guild->GetGuildGrade()))
    {
        return RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER;
    }

	GuildRelationSystem::GuildRelationEntry* target_entry = target_guild->GetRelationEntry();

    if (relation_type == eGUILD_RELATION_PROCLAIM_HOSTILITY) 
    {
        relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
    }

	if (target_entry->IsRelationCountOverFlow(relation_type, max_count))
    {
		return RC::RC_GUILD_RELATION_OTHER_FULL;
    }

	result = target_entry->IsRelationPossibility(request_guild_guid, relation_type);
	if (result != RC::RC_GUILD_SUCCESS) 
    {
        return result;
    }

	return RC::RC_GUILD_SUCCESS;
}

#else//_NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT GuildSmallContentEntry::RelationSystem::
CanDoRelationShip(GUILDGUID request_guild_index, 
				  GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
	std::size_t max_count = (relation_type == eGUILD_RELATION_ALLIENCE) ? 10 : 5;

	// 자신의 길드 관계 조사
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
	
	// 요청 받은 길드 관계 조사
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
    if (relation_type == eGUILD_RELATION_ALLIENCE && pResponseGuild == NULL)
    {
        // 동맹의 경우에는 상대 길드의 동의가 필요하기 때문에 로딩된 길드 정보 필요
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
#endif//_NA_003923_20120130_GUILD_RENEWAL

RC::eGUILD_RESULT  GuildSmallContentEntry::RelationSystem::
CanDoRelationBothHostility(GUILDGUID request_guild_index, GUILDGUID response_guild_index)
{
	// 자신의 길드 관계 조사
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();

	// 쌍방 적대를 신청한 길드는 상대 길드가 일방적 적대 상태여야 한다.
	if(eGUILD_RELATION_ONE_SIDE_HOSTILITY == 
		pRequestEntry->GetRelationType(response_guild_index))
		return RC::RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED;
	
	// 요청 받은 길드 관계 조사
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
	if(!pResponseGuild)
	{
		return RC::RC_GUILD_SUCCESS; //로그 아웃 상태에서도 신청 가능
	}

	GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
    __TOUCHED(pResponseEntry);

	// 쌍방 적대를 요청 받은 길드는 요청한 길드가 적대 선포중 상태여야 한다.
	if(eGUILD_RELATION_PROCLAIM_HOSTILITY == 
		pRequestEntry->GetRelationType(request_guild_index))
		return RC::RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentEntry::RelationSystem::
CanDoRelationDelete(GUILDGUID request_guild_index, 
					GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
    __UNUSED(relation_type);
	// 자신의 길드 관계 조사
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	if(eGUILD_RELATION_NONE == pRequestEntry->GetRelationType(response_guild_index))
		return RC::RC_GUILD_RELATION_NONE_DELETE_NOT;

	// 요청 받은 길드 관계 조사
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
	if(!pResponseGuild)
	{
		return RC::RC_GUILD_SUCCESS; //로그 아웃 상태에서도 신청 가능
	}
	
	GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
	if(eGUILD_RELATION_NONE == pResponseEntry->GetRelationType(request_guild_index))
		return RC::RC_GUILD_RELATION_NONE_DELETE_NOT;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  GuildSmallContentEntry::RelationSystem::
CanDoRelationChange(GUILDGUID request_guild_index, CHARGUID request_player_index)
{
	Guild* guild = g_GuildManager.FindGuild(request_guild_index);
	if (guild == NULL)
    {
		return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	if (guild->CheckGuildDeleteState() == eGUILD_DELCHK_DESTROY_WAIT)
    {
        //길드 삭제 대기상태
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	if (GuildPenalty::CheckGuildPenaltyTime(guild) == false)
    {
        //길드 페널티 상태 체크(중국만 적용중 2012.02.23 )
		return RC::RC_GUILD_RELATION_GUILD_DESTROY_WAIT;
    }

    bool is_right_check = g_GuildManager.CheckRight(request_guild_index, 
                                                    request_player_index, 
                                                    eGUILD_RIGHTS_RELATION);
	if (is_right_check == FALSE)
    {
		return RC::RC_GUILD_RELATION_CHANGE_RIGHT_NOT;
    }

	return RC::RC_GUILD_SUCCESS;
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT  
GuildSmallContentEntry::RelationSystem::AddRelation(GUILDGUID request_guild_guid, 
                                                    GUILDGUID target_guild_guid, 
                                                    eGUILD_RELATION relation_type, 
                                                    DWORD64 relation_apply_time,
                                                    std::string response_guild_name)
{
    // 요청한 길드 ---------------------------------------------------------------------
    Guild* request_guild = g_GuildManager.FindGuild(request_guild_guid);
    if(!request_guild)
        return RC::RC_GUILD_NOT_EXIST_GUILD;

    ePROCLAIM_GUILD pro_claim = NONE_PROCLAIM_GUILD;
    
    GuildRelationSystem::GuildRelationEntry* request_entry = request_guild->GetRelationEntry();
    if (relation_type == eGUILD_RELATION_PROCLAIM_HOSTILITY) 
    {
        pro_claim = PROCLAIM_GUILD;
    }

    request_entry->RelationInsert(
        target_guild_guid,
        relation_type,
        pro_claim,
        relation_apply_time,
        response_guild_name
        );

    // 요청 받은 길드 ---------------------------------------------------------------------
    Guild* target_guild = g_GuildManager.FindGuild(target_guild_guid);
    if (target_guild)
    {
        GuildRelationSystem::GuildRelationEntry* target_entry = target_guild->GetRelationEntry();
        pro_claim = (PROCLAIM_GUILD == pro_claim) ? ONESIDE_GUILD : NONE_PROCLAIM_GUILD;
        if (relation_type == eGUILD_RELATION_PROCLAIM_HOSTILITY ) 
        {
            relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
        }

        target_entry->RelationInsert(
            request_guild_guid, 
            relation_type, 
            pro_claim, 
            relation_apply_time,
            request_guild->GetGuildName()
            );
    }

    GUILDLOG->LogGuildRelation(request_guild, target_guild_guid, target_guild, (BYTE)relation_type);

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    RelationInGameGuildLog(request_guild_guid, target_guild_guid, relation_type, false);
#endif

    return RC::RC_GUILD_SUCCESS;
}
#else
RC::eGUILD_RESULT  GuildSmallContentEntry::RelationSystem::
AddRelation(GUILDGUID request_guild_index, GUILDGUID response_guild_index, 
            eGUILD_RELATION relation_type, std::string response_guild_name)
{
    // 요청한 길드 ---------------------------------------------------------------------
    Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
    if(!pRequestGuild)
        return RC::RC_GUILD_NOT_EXIST_GUILD;

    ePROCLAIM_GUILD eProClaim = NONE_PROCLAIM_GUILD;
    GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
    if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type) eProClaim = PROCLAIM_GUILD;
    pRequestEntry->RelationInsert(response_guild_index, relation_type, eProClaim, response_guild_name);

    // 요청 받은 길드 ---------------------------------------------------------------------
    // 적대 선포를 요청 받는 길드는 일방적 적대 관계 상태가 된다.
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
#endif

RC::eGUILD_RESULT  GuildSmallContentEntry::RelationSystem::
RemoveRelation(GUILDGUID request_guild_index, 
			   GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    RelationInGameGuildLog(request_guild_index, response_guild_index, relation_type, true);
#endif

	// 요청한 길드
	Guild* pRequestGuild = g_GuildManager.FindGuild(request_guild_index);
	if(!pRequestGuild)
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	pRequestEntry->RelationDelete(response_guild_index, relation_type, true);

	// 요청 받은 길드
	Guild* pResponseGuild = g_GuildManager.FindGuild(response_guild_index);
	if (pResponseGuild)
	{
        GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
        pResponseEntry->RelationDelete(request_guild_index, relation_type, false);
	}

	GUILDLOG->LogGuildRelation(pRequestGuild, response_guild_index, pResponseGuild, (BYTE)relation_type);

	return RC::RC_GUILD_SUCCESS;
}

void GuildSmallContentEntry::RelationSystem::
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
#ifdef _NA_003923_20120130_GUILD_RENEWAL 
            DWORD64 relation_apply_time = pRelationList[i].m_RelationApplyTime;
            pRequestEntry->RelationInsert(
                guild_index, 
                relation_type, 
                eProClaim , 
                relation_apply_time,
                guild_name
                );
#else
            pRequestEntry->RelationInsert(guild_index, relation_type, eProClaim , guild_name);
#endif
		}
	}
}

void GuildSmallContentEntry::RelationSystem::
LoadRelationInfo(BASE_GUILD_ADD_INFO* Info)
{
	Guild* pRequestGuild = g_GuildManager.FindGuild(Info->m_GuildGuid);
	if(pRequestGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
		int info_count = pRequestEntry->LoadRelation(Info);
        __TOUCHED(info_count);
		Info->SetSize(Info->m_byAllienceGuildCnt, Info->m_byHostilityGuildCnt, Info->m_byOneSideGuildCnt);
	}
}

eGUILD_RELATION GuildSmallContentEntry::RelationSystem::
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

bool GuildSmallContentEntry::RelationSystem::
Compare(const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type& info,
		const GUILDGUID index, const eGUILD_RELATION compareType)
{
	GuildRelationSystem::GuildRelationInfo* item = info;

	if(compareType == eGUILD_RELATION_ALLIENCE)
		return compareType == GetRelationType(item->GuildGuid, index);
	else
		return compareType < GetRelationType(item->GuildGuid, index);
}

void  GuildSmallContentEntry::RelationSystem::
RemainRelationDestroy(GUILDGUID guildguid)
{
	Guild* pGuild = g_GuildManager.FindGuild(guildguid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
		pEntry->RelationGuildsNotify(eGUILD_RELATION_ONE_SIDE_HOSTILITY, guildguid);
	}
}

void GuildSmallContentEntry::RelationSystem::DestroyWaitNotify(
    const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type& info,
    const GUILDGUID index)
{
	GuildRelationSystem::GuildRelationInfo* item = info;
	Guild* pGuild = g_GuildManager.FindGuild(item->GuildGuid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRelationEntry = pGuild->GetRelationEntry();
		pRelationEntry->RelationDelete(index, eGUILD_RELATION_ONE_SIDE_HOSTILITY, false);
	}
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
DWORD64
GuildSmallContentEntry::RelationSystem::CalcRelationChangeTime(const eGUILD_RELATION relation_type,
                                                               const DWORD64 relation_apply_time)
{
    GuildInfoParser* const guild_info_parser = GuildInfoParser::Instance();
    switch(relation_type)
    {
    case eGUILD_RELATION_PROCLAIM_HOSTILITY:
    case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
        return util::AddMinuteToYYYYDDHHMMSS(
            relation_apply_time, 
            guild_info_parser->GetGuildInfo().time_of_hostile_latency_
            );

    case eGUILD_RELATION_BOTH_HOSTILITY:
        return util::AddMinuteToYYYYDDHHMMSS(
            relation_apply_time, 
            guild_info_parser->GetGuildInfo().time_of_hostile_
            );
    }

    return 0;
}

void 
GuildSmallContentEntry::RelationSystem::RelationInGameGuildLog(const GUILDGUID request_guild_guid,
                                                               const GUILDGUID target_guild_guid,
                                                               eGUILD_RELATION relation_type,
                                                               bool is_delete_relation)
{
    //길드 관계 로그 타입 설정 : 설정상 쌍방 적대 해제의 경우도 관계로 처리한다.
    GuildInGameLogType request_guild_log_type = 
        (relation_type == eGUILD_RELATION_ALLIENCE) ? GuildRelationAllienceSet
        : (relation_type == eGUILD_RELATION_BOTH_HOSTILITY) ? GuildBothHostilitySet
        : (relation_type == eGUILD_RELATION_ONE_SIDE_HOSTILITY) ? GuildSendHostilitySet
        : (relation_type == eGUILD_RELATION_CANCEL_BOTH_HOSTILITY) ? GuildHostilityCancel
        : GuildInGameLogMax;


    GuildInGameLogType target_guild_log_type = 
        (relation_type == eGUILD_RELATION_ALLIENCE) ? GuildRelationAllienceSet
        : (relation_type == eGUILD_RELATION_BOTH_HOSTILITY) ? GuildBothHostilitySet
        : (relation_type == eGUILD_RELATION_ONE_SIDE_HOSTILITY) ? GuildRecvHostilitySet
        : (relation_type == eGUILD_RELATION_CANCEL_BOTH_HOSTILITY) ? GuildHostilityCancel
        : GuildInGameLogMax;

    //동맹 삭제일경우 로그타입 세팅
    if (relation_type == eGUILD_RELATION_ALLIENCE && is_delete_relation == true)
    {
        request_guild_log_type = target_guild_log_type = GuildRelationAllienceCancle;
    }

    if (request_guild_log_type == GuildInGameLogMax || target_guild_log_type == GuildInGameLogMax)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Incorrect log type : request type = %d, target type = %d, rel type = %d",
            request_guild_log_type,
            target_guild_log_type,
            relation_type
            );
        return;
    }

    Guild* request_guild = g_GuildManager.FindGuild(request_guild_guid);
    if (request_guild == NULL)
    {
        return;
    }

    //관계 신청한 길드 로그
    GuildRelationSystem::GuildRelationEntry* relation_entry = request_guild->GetRelationEntry();
    request_guild->GetGuildInGameLog()->WriteGuildLog(
        request_guild_log_type,
        request_guild_guid,
        relation_entry->GetRelationGuildName(target_guild_guid)
        );

    //관계 신청을 받은 길드 로그
    request_guild->GetGuildInGameLog()->WriteGuildLog(
        target_guild_log_type,
        target_guild_guid,
        request_guild->GetGuildName()
        );
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
