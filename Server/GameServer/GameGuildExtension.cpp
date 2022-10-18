#include "StdAfx.h"

#include <SolarDateTime.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_GZ.h>

#include "ItemManager.h"
#include "ItemInfoParser.h"
#include "GameGuild.h"
#include "GameGuildExtension.h"

RC::eGUILD_RESULT GuildSmallContentEntry::GuildPenalty::
PlayerPenaltyCheck(const BASE_PLAYERINFO* play_info)
{
	PenaltyTime current_Time = 0;
	PenaltyTime penalty_time = play_info->m_GuildPenaltyTime;
	if(penalty_time >= util::GetDateTime_YYYYMMDDHHMM(current_Time))
	{
		eGUILD_PENALTY type = play_info->m_ePenaltyType;
		switch(type)
		{
		case eGUILD_DESTROY : 
			return RC::RC_GUILD_DESTROY_PENALTY_TIME;
		case eGUILD_WITHDRAW :
			return RC::RC_GUILD_WITHDRAW_PENALTY_TIME;
		case eGUILD_KICK: 
			return RC::RC_GUILD_KICK_PENALTY_TIME;
		}
	}

	return RC::RC_GUILD_SUCCESS;
}

void GuildSmallContentEntry::GuildMark::
RegisterMark(Player* pPlayer, const GuildMarkInfo& MarkInfo)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{
		pGuild->RegisterGuildMark(MarkInfo.background_index, MarkInfo.pattern_index);

		// 요청한 플레이어가 있는 서버 요청 플레이어의 길드 인장 삭제
		// 길드 마크 등록 신청을 보낼대 서버세션 인덱스도 보내서
		// 요청 플레이어가 있는 서버에서 플레이어가 사라졌을때의 로그도 기록하게끔 하자...
		if(pPlayer)
		{
            // 길드 인장 아이템 사용 로그
            ItemManager*   pItemManager = pPlayer->GetItemManager();
            SCSlotContainer* pContainer = pItemManager->GetItemSlotContainer( SI_INVENTORY );
            SCItemSlot&       rItemSlot = (SCItemSlot &)pContainer->GetSlot( MarkInfo.posType );
            
            GAMELOG->LogItem( ITEM_USE, pPlayer, &rItemSlot ); 

			RC::eITEM_RESULT rt = pPlayer->GetItemManager()->DeleteWastedItem(MarkInfo.slot_Index, MarkInfo.posType);
			if(RC::RC_ITEM_SUCCESS != rt) 
			{
				MSG_CG_GUILD_GUILDMARK_DELETE_NAK msg;
				msg.m_bErrorCode = RC::RC_GUILD_NOT_EXIST_GUILD_SEAL;
				pPlayer->SendPacket(&msg, sizeof(msg));
				SUNLOG(eCRITICAL_LOG, "GuildMarkItem Del Fail(%d)(%d)", pPlayer->GetCharGuid(), pGuild->GetGuildGuid());
			}
		}

		MSG_CG_GUILD_GUILDMARK_REGISTER_BRD msgBRD;
		msgBRD.m_GuildIndex = MarkInfo.guild_Index;
		msgBRD.m_PatternIndex = MarkInfo.pattern_index;
		msgBRD.m_BackGroundIndex = MarkInfo.background_index;
		pGuild->SendPacketMemberAround(msgBRD, INVALID_GUILDGUID);
	}
}

void GuildSmallContentEntry::GuildMark::
DeleteMark(const GuildMarkInfo& MarkInfo)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{	
		pGuild->DeleteGuildMark();
		
		MSG_CG_GUILD_GUILDMARK_DELETE_BRD msgBRD;
		msgBRD.m_GuildIndex = MarkInfo.guild_Index;
		msgBRD.m_PatternIndex = MarkInfo.pattern_index;
		msgBRD.m_BackGroundIndex = MarkInfo.background_index;
		pGuild->SendPacketMemberAround(msgBRD, INVALID_GUILDGUID);

        return;
	}
}

void GuildSmallContentEntry::GuildMark::
GetGuildMark(GUILDGUID guild_index, GUILDMARKIDX& backImg, GUILDMARKIDX& pattImg)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(guild_index);
	if(pGuild)
	{
		pGuild->GetGuildMark(backImg, pattImg);
	}
}

void GuildSmallContentEntry::GuildMark::
GuildInfoForRender(GUILD_RENDER_INFO& renderInfo, GUILDGUID other_guildIndex)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(renderInfo.m_GuildGuid);
	if(pGuild)
	{
		pGuild->GetGuildMark(renderInfo.m_BackGroundIndex, renderInfo.m_PatternIndex);
		renderInfo.m_RelationType = GuildSmallContentEntry::RelationSystem::GetRelationType(
            renderInfo.m_GuildGuid, other_guildIndex);
	}

	//GameGuild* pOtherGuild = g_GameGuildManager.FindGuild(other_guildIndex);
	//if(pOtherGuild)
	//{
	//	renderInfo.m_OtherGuildGrade = pOtherGuild->GetGuildGrade();
	//}
}

GuildSmallContentEntry::GuildMark::IsRegister
GuildSmallContentEntry::GuildMark::IsRegisterMark(const GuildMarkInfo& MarkInfo)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(MarkInfo.guild_Index);
	if(pGuild)
	{
		GUILDMARKIDX back_mark, pattern_mark;
		pGuild->GetGuildMark(back_mark, pattern_mark)	;

		if(MarkInfo.background_index == back_mark &&
			MarkInfo.pattern_index == pattern_mark)
		{
			return Aleady_Register;
		}
	}

	return None_Register;
}

RC::eGUILD_RESULT GuildSmallContentEntry::GuildMark::
CanRegister(Player* pRequestPlayer, const GuildMarkInfo& MarkInfo)
{
	if(MarkInfo.guild_Index != pRequestPlayer->GetCharInfo()->m_GuildGuid)
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 길드인장 유효성 검사
	if(RC::RC_ITEM_SUCCESS != 
		pRequestPlayer->GetItemManager()->IsValidItem(MarkInfo.slot_Index, MarkInfo.posType, eITEMTYPE_GUILDMARK_SEAL))
		return RC::RC_GUILD_NOT_EXIST_GUILD_SEAL;

	// 등록할 마크의 유효성 검사
	if(MarkInfo.background_index <= 0 || MarkInfo.pattern_index <=0)
		return RC::RC_GUILD_MARK_INVALID_TYPE;

	// 등록되어 있는 마크와 같은지 검사
	if(GuildMark::Aleady_Register == GuildMark::IsRegisterMark(MarkInfo))
		return RC::RC_GUILD_MARK_ALEADY_EXIST;
	
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT GuildSmallContentEntry::GuildMark::
CanDelete(Player* pRequestPlayer, const GuildMarkInfo& MarkInfo)
{
	if(MarkInfo.guild_Index != pRequestPlayer->GetCharInfo()->m_GuildGuid)
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 삭제할 마크의 유효성 검사
	if(MarkInfo.background_index <= 0 || MarkInfo.pattern_index <=0)
		return RC::RC_GUILD_MARK_INVALID_TYPE;

	// 등록되어 있는 마크인지 검사
	if(GuildMark::None_Register == GuildMark::IsRegisterMark(MarkInfo))
		return RC::RC_GUILD_MARK_INVALID_TYPE;

	return RC::RC_GUILD_SUCCESS;
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT  
GuildSmallContentEntry::RelationSystem::AddRelation(GUILDGUID request_guild_guid, 
                                                    GUILDGUID target_guild_guid, 
                                                    eGUILD_RELATION relation_type,
                                                    DWORD64 relation_apply_time,
                                                    DWORD64 relation_change_time,
                                                    std::string response_guild_name, 
                                                    std::string request_guild_name)
{
    //Request guild
    GameGuild* request_guild = g_GameGuildManager.FindGuild(request_guild_guid);
    ePROCLAIM_GUILD pro_claim = NONE_PROCLAIM_GUILD;

    if(request_guild)
    {
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
            relation_change_time,
            response_guild_name
            );

        if (relation_type == eGUILD_RELATION_BOTH_HOSTILITY)
        {
            MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD msg;
            msg.m_BothGuildGuid	= target_guild_guid;
            msg.m_RelationApplyTime = relation_apply_time;
            msg.m_RelationChangeTime = relation_change_time;
            msg.m_eMOVType	 = eGUILD_RELATION_BOTH_HOSTILITY;
            msg.m_eDELType	 = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
            request_guild->SendPacketMember(msg);
        }
        else
        {
            MSG_CG_GUILD_RELATION_BRD msg;
            msg.m_stRELGuild.m_GuildGuid = target_guild_guid;
            msg.m_stRELGuild.m_eRelationType = relation_type;
            msg.m_stRELGuild.m_eProClaim = pro_claim;
            msg.m_stRELGuild.m_RelationApplyTime = relation_apply_time;
            msg.m_stRELGuild.m_RelationChangeTime = relation_change_time;
            util::GUILDNAMECOPY(msg.m_stRELGuild.m_GuildName, response_guild_name.c_str());

            request_guild->SendPacketMember(msg);
        }
    }


    //Target guild
    GameGuild* target_guild = g_GameGuildManager.FindGuild(target_guild_guid);
    if(!target_guild)
    {
        return (relation_type == eGUILD_RELATION_ALLIENCE) ? 
            RC::RC_GUILD_NOT_EXIST_GUILD : RC::RC_GUILD_SUCCESS;
    }

    GuildRelationSystem::GuildRelationEntry* target_entry = target_guild->GetRelationEntry();
    //적대 길드 선포일 경우에는 적대 선포 당함으로 설정, 동맹일 경우에는 기본값 세팅
    pro_claim = (PROCLAIM_GUILD == pro_claim) ? ONESIDE_GUILD : NONE_PROCLAIM_GUILD;

    if (relation_type == eGUILD_RELATION_PROCLAIM_HOSTILITY)
    {
        relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
    }
    target_entry->RelationInsert(
        request_guild_guid, 
        relation_type, 
        pro_claim, 
        relation_apply_time,
        relation_change_time,
        request_guild_name
        );

    if(relation_type == eGUILD_RELATION_BOTH_HOSTILITY)
    {
        MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD msg;
        msg.m_BothGuildGuid	= request_guild_guid;
        msg.m_RelationApplyTime = relation_apply_time;
        msg.m_RelationChangeTime = relation_change_time;
        msg.m_eMOVType = eGUILD_RELATION_BOTH_HOSTILITY;
        msg.m_eDELType = eGUILD_RELATION_PROCLAIM_HOSTILITY;
        target_guild->SendPacketMember(msg);
    }
    else
    {
        MSG_CG_GUILD_RELATION_BRD msg;
        msg.m_stRELGuild.m_GuildGuid = request_guild_guid;
        msg.m_stRELGuild.m_eRelationType = relation_type;
        msg.m_stRELGuild.m_eProClaim = pro_claim;
        msg.m_stRELGuild.m_RelationApplyTime = relation_apply_time;
        msg.m_stRELGuild.m_RelationChangeTime = relation_change_time;
        util::GUILDNAMECOPY(msg.m_stRELGuild.m_GuildName, request_guild_name.c_str());
        target_guild->SendPacketMember(msg);
    }

    return RC::RC_GUILD_SUCCESS;
}
#else//_NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT  
GuildSmallContentEntry::RelationSystem::AddRelation(GUILDGUID request_guild_index, 
                                                    GUILDGUID response_guild_index, 
                                                    eGUILD_RELATION relation_type, 
                                                    std::string response_guild_name, 
                                                    std::string request_guild_name)
{
    // 요청한 길드
    GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(request_guild_index);
    //if(!pRequestGuild) return RC::RC_GUILD_NOT_EXIST_GUILD;
    ePROCLAIM_GUILD eProClaim = NONE_PROCLAIM_GUILD;

    if(pRequestGuild)
    {
        GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
        if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type) eProClaim = PROCLAIM_GUILD;
        pRequestEntry->RelationInsert(response_guild_index, relation_type, eProClaim, response_guild_name);

        // 길드 관계 렌더 정보 Broad --------------------------------------------------
        if(relation_type == eGUILD_RELATION_BOTH_HOSTILITY)
        {
            MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD msg;
            msg.m_BothGuildGuid	= response_guild_index;
            msg.m_eMOVType	 = eGUILD_RELATION_BOTH_HOSTILITY;
            msg.m_eDELType	 = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
            pRequestGuild->SendPacketMember(msg);
        }
        else
        {
            MSG_CG_GUILD_RELATION_BRD msg;
            msg.m_stRELGuild.m_GuildGuid = response_guild_index;
            msg.m_stRELGuild.m_eRelationType = relation_type;
            msg.m_stRELGuild.m_eProClaim = eProClaim;
            util::GUILDNAMECOPY(msg.m_stRELGuild.m_GuildName, response_guild_name.c_str());

            pRequestGuild->SendPacketMember(msg);

            if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type)
            {
                MSG_AG_PARTY_LEAVE_BY_GUILDRELATION_SYN msg;
                pRequestGuild->PartyToLeaveByRelation(response_guild_index);
            }
        }
    }


    // 요청 받은 길드 ---------------------------------------------------------------------
    // 적대 선포를 요청 받는 길드는 일방적 적대 관계 상태가 된다.
    GameGuild* pResponseGuild = g_GameGuildManager.FindGuild(response_guild_index);
    if(!pResponseGuild)
    {
        return (relation_type == eGUILD_RELATION_ALLIENCE) ?
            RC::RC_GUILD_NOT_EXIST_GUILD : RC::RC_GUILD_SUCCESS;
    }

    GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
    eProClaim = (PROCLAIM_GUILD == eProClaim) ? ONESIDE_GUILD : NONE_PROCLAIM_GUILD;
    if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type) relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
    pResponseEntry->RelationInsert(request_guild_index, relation_type, eProClaim, request_guild_name);

    // 길드 관계 렌더 정보 Broad --------------------------------------------------
    if(relation_type == eGUILD_RELATION_BOTH_HOSTILITY)
    {
        MSG_CG_GUILD_RELATION_BOTH_AGAINST_BRD msg;
        msg.m_BothGuildGuid	= request_guild_index;
        msg.m_eMOVType	 = eGUILD_RELATION_BOTH_HOSTILITY;
        msg.m_eDELType = eGUILD_RELATION_PROCLAIM_HOSTILITY;
        pResponseGuild->SendPacketMember(msg);
    }
    else
    {
        MSG_CG_GUILD_RELATION_BRD msg;
        msg.m_stRELGuild.m_GuildGuid = request_guild_index;
        msg.m_stRELGuild.m_eRelationType = relation_type;
        msg.m_stRELGuild.m_eProClaim = eProClaim;
        util::GUILDNAMECOPY(msg.m_stRELGuild.m_GuildName, request_guild_name.c_str());
        pResponseGuild->SendPacketMember(msg);
    }

    return RC::RC_GUILD_SUCCESS;
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

RC::eGUILD_RESULT  GuildSmallContentEntry::RelationSystem::
RemoveRelation(GUILDGUID request_guild_index, 
			   GUILDGUID response_guild_index, eGUILD_RELATION relation_type)
{
	// 요청한 길드
	GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(request_guild_index);
	//if(!pRequestGuild) return RC::RC_GUILD_NOT_EXIST_GUILD;
	if(pRequestGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
		bool is_notify = pRequestEntry->RelationDelete(response_guild_index, relation_type, true);
        if (is_notify)
        {
            if(eGUILD_RELATION_BOTH_HOSTILITY == relation_type)
            {
                MSG_CG_GUILD_RELATION_MOVE_BRD msg;
                msg.m_GuildGuid	= response_guild_index;
                msg.m_eDELType	= eGUILD_RELATION_BOTH_HOSTILITY;
                msg.m_eMOVType	= eGUILD_RELATION_ONE_SIDE_HOSTILITY;
                pRequestGuild->SendPacketMember(msg);
            }
            else
            {
                MSG_CG_GUILD_RELATION_DELETE_BRD msg;
                msg.m_eRelType = relation_type;
                msg.m_RelGuildGuid =response_guild_index;
                pRequestGuild->SendPacketMember(msg);
            }
        }
	}

	// 요청 받은 길드
	GameGuild* pResponseGuild = g_GameGuildManager.FindGuild(response_guild_index);
	if(!pResponseGuild)
	{
		return (relation_type == eGUILD_RELATION_ALLIENCE) ?
			RC::RC_GUILD_NOT_EXIST_GUILD : RC::RC_GUILD_SUCCESS;
	}

	GuildRelationSystem::GuildRelationEntry* pResponseEntry = pResponseGuild->GetRelationEntry();
	bool is_notify = pResponseEntry->RelationDelete(request_guild_index, relation_type, false);
    if (is_notify)
    {
        if(eGUILD_RELATION_BOTH_HOSTILITY == relation_type)
        {
            MSG_CG_GUILD_RELATION_MOVE_BRD msg;
            msg.m_GuildGuid	= request_guild_index;
            msg.m_eDELType	= eGUILD_RELATION_BOTH_HOSTILITY;
            msg.m_eMOVType	= eGUILD_RELATION_PROCLAIM_HOSTILITY;
            pResponseGuild->SendPacketMember(msg);
        }
        else
        {
            if(eGUILD_RELATION_PROCLAIM_HOSTILITY == relation_type)
                relation_type = eGUILD_RELATION_ONE_SIDE_HOSTILITY;

            MSG_CG_GUILD_RELATION_DELETE_BRD msg;
            msg.m_eRelType = relation_type;
            msg.m_RelGuildGuid =request_guild_index;
            pResponseGuild->SendPacketMember(msg);
        }
    }

	return RC::RC_GUILD_SUCCESS;
}

void GuildSmallContentEntry::RelationSystem::
SaveRelationInfo(BASE_GUILD_ADD_INFO* Info)
{
	GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(Info->m_GuildGuid);
	if(pRequestGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	
		int total_count = Info->m_byRelationGuildCnt;
		GUILD_RELATION* pRelationList = Info->m_stRelationGuild;
		for(int i = 0; i < total_count; ++i)
		{
			GUILDGUID guild_index = pRelationList[i].m_GuildGuid;
			ePROCLAIM_GUILD eProClaim = pRelationList[i].m_eProClaim;
			eGUILD_RELATION relation_type = pRelationList[i].m_eRelationType;
			std::string guild_name = pRelationList[i].m_GuildName;
#ifdef _NA_003923_20120130_GUILD_RENEWAL 
            DWORD64 relation_apply_time = pRelationList[i].m_RelationApplyTime;
            DWORD64 relation_change_time = pRelationList[i].m_RelationChangeTime;
            pRequestEntry->RelationInsert(
                guild_index, 
                relation_type, 
                eProClaim, 
                relation_apply_time,
                relation_change_time,
                guild_name
                );
#else
            pRequestEntry->RelationInsert(guild_index, relation_type, eProClaim, guild_name);
#endif
		}
	}
}

BOOL GuildSmallContentEntry::RelationSystem::
LoadRelationInfo(BASE_GUILD_ADD_INFO* Info)
{
	GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(Info->m_GuildGuid);
	if(!pRequestGuild) return false;
	
	GuildRelationSystem::GuildRelationEntry* pRequestEntry = pRequestGuild->GetRelationEntry();
	int info_count = pRequestEntry->LoadRelation(Info);
	Info->SetSize(Info->m_byAllienceGuildCnt, Info->m_byHostilityGuildCnt, Info->m_byOneSideGuildCnt);

	return pRequestEntry->IsSaveComplete();
}

bool GuildSmallContentEntry::RelationSystem::
IsHostilityRelation(GUILDGUID AttkUserGuildGuid, GUILDGUID DefUserGuildGuid)
{
		GameGuild* pGuild = g_GameGuildManager.FindGuild(AttkUserGuildGuid);
		if(pGuild)
		{
			GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
			eGUILD_RELATION type = pEntry->GetRelationType(DefUserGuildGuid);
#ifdef _NA_003923_20120130_GUILD_RENEWAL
            if(eGUILD_RELATION_BOTH_HOSTILITY == type)
                return true;
#else
            if(eGUILD_RELATION_PROCLAIM_HOSTILITY == type || eGUILD_RELATION_BOTH_HOSTILITY == type)
                return true;
#endif
		}
		
		return false;
}

bool GuildSmallContentEntry::RelationSystem::
IsAllHostilityRelation(GUILDGUID AttkUserGuildGuid, GUILDGUID DefUserGuildGuid)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(AttkUserGuildGuid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
		eGUILD_RELATION type = pEntry->GetRelationType(DefUserGuildGuid);
		if(eGUILD_RELATION_PROCLAIM_HOSTILITY == type 
			|| eGUILD_RELATION_BOTH_HOSTILITY == type || eGUILD_RELATION_ONE_SIDE_HOSTILITY == type)
			return true;
	}

	return false;
}

void  GuildSmallContentEntry::RelationSystem::
RemainRelationDestroy(GUILDGUID guildguid)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(guildguid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
		pEntry->RelationGuildsNotify(eGUILD_RELATION_ONE_SIDE_HOSTILITY, guildguid);
	}
	/*	해당 채널에 해산 신청을 한 길드가 존재하지 않으면, 
		해당 채널에 존재하는 모든 길드에 BRD한다..(이 무슨 미친짓이냐...)
	*/
	else
	{
		GUILD_HASH* pGuildHash = g_GameGuildManager.GetGuildHash();
		for( GUILD_HASH::iterator it = pGuildHash->begin() ; it != pGuildHash->end() ; ++it )
		{
			GameGuild* pGuild = *it;
			if(!pGuild) continue;
			
			GuildRelationSystem::GuildRelationEntry* pRelationEntry = pGuild->GetRelationEntry();
			if(eGUILD_RELATION_PROCLAIM_HOSTILITY == 
				pRelationEntry->GetRelationType(guildguid))
			{
				MSG_CG_GUILD_DESTROY_WAIT_BRD msg;
				msg.m_RelGuildGuid = guildguid;
				pGuild->SendPacketAll(&msg, sizeof(msg));
			}
			
		}
	}
}

eGUILD_RELATION GuildSmallContentEntry::RelationSystem::
GetRelationType(GUILDGUID guild_index, GUILDGUID other_guild_index)
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild(guild_index);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pEntry = pGuild->GetRelationEntry();
		return  (guild_index != other_guild_index)  ? 
			pEntry->GetRelationType(other_guild_index) : eGUILD_RELATION_ALLIENCE;
	}

	return eGUILD_RELATION_NONE;
}

void GuildSmallContentEntry::RelationSystem::DestroyWaitNotify(
    const GuildRelationSystem::GuildRelationEntry::ByRelationTypeIterator::value_type& info,
    const GUILDGUID index)
{
	GuildRelationSystem::GuildRelationInfo* item = info;
	GameGuild* pGuild = g_GameGuildManager.FindGuild(item->GuildGuid);
	if(pGuild)
	{
		GuildRelationSystem::GuildRelationEntry* pRelationEntry = pGuild->GetRelationEntry();
		pRelationEntry ->RelationDelete(index, eGUILD_RELATION_ONE_SIDE_HOSTILITY, false);

		MSG_CG_GUILD_DESTROY_WAIT_BRD msg;
		msg.m_RelGuildGuid = index;
		pGuild->SendPacketAll(&msg, sizeof(msg));
	}
}
