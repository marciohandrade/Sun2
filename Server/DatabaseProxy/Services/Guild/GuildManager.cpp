#include "stdafx.h"
#include "./GuildManager.h"

#include <Struct.h>
#include "PacketStruct_GZ.h"

#include <GuildRightInfoParser.h>
#include <MaxGuildMemberInfoParser.h>

#include "PacketHandler/PacketHandler.h"
#include "QueryObjects/DBHandler.h"

#include "GuildMember.h"
#include "Guild.h"
#include "GuildSmallContentEntry.h"

#include "GuildInfoParser.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildRelation.h"
#include "GuildInGameLog.h"
#endif
#include <Services/GuildPointSystem/GuildPointSystem.h>
#include <PacketStruct_PointSystem.h>

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilities.h"
#include "misc.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "DominationInfoParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM
#include "GuildRelation.h"
#endif // _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM

GuildManager g_GuildManager;
GuildManager::GuildManager(void)
{
	guild_hash_.Initialize( _MAX_GUILD_POOL_NUM );
	guild_name_hash_.clear();

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild_relation_timer_.SetTimer(MAX_GUILDRELATION_TIME);
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    guild_join_request_timer_.SetTimer(MAX_GUILD_JOIN_REQUEST_TIME);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    sschein_update_timer_.SetTimer(UPDATE_TIME);
    event_loaded_ = FALSE;
#endif //_NA_008334_20150608_SONNENSCHEIN
}

GuildManager::~GuildManager(void)
{
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild_relation_timer_.Clear();
#endif
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    guild_join_request_timer_.Clear();
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

//==================================================================================================
void GuildManager::Release()
{
    GUILD_HASH::iterator begin_it = guild_hash_.begin();
    const GUILD_HASH::iterator end_it = guild_hash_.end();

    for (; begin_it != end_it; ++begin_it)
    {
        Guild* current_guild = *begin_it;
        current_guild->ReleaseGuild();
        Guild::FREE(current_guild);
    }
    guild_hash_.clear();
}

bool 
GuildManager::Update()
{
    if (guild_destroy_timer_.IsExpired())
    {
        UpdateForGuildDestroy();
        
    };

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    if (guild_relation_timer_.IsExpired())
    {
        UpdateGuildRelation();
    }
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (guild_join_request_timer_.IsExpired())
    {
        UpdateGuildJoinRequest();
    }
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    if (sschein_update_timer_.IsExpired() == TRUE) {
        _UpdateSonnenSchein();
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
    return true;
}

//==================================================================================================

#ifdef _NA_003923_20120130_GUILD_RENEWAL
void 
GuildManager::UpdateGuildRelation()
{
    GUILD_HASH::iterator begin_it = guild_hash_.begin();
    const GUILD_HASH::iterator end_it = guild_hash_.end();

    DWORD dwNum = 0;
    for (; begin_it != end_it; ++begin_it)
    {
        Guild* current_guild = *begin_it;
        GuildRelationSystem::GuildRelationEntry* relation_entry = current_guild->GetRelationEntry();
        relation_entry->UpdateRelation(current_guild->GetGuildGuid());
    }
}
#endif

void 
GuildManager::StartGuildDestroyTimer()
{
    guild_destroy_timer_.SetTimer(MAX_GUILDDESTROY_TIME);
}

void 
GuildManager::StopGuildDestroyTimer()
{
    guild_destroy_timer_.Clear();
}

#if SUN_CODE_BACKUP
RC::eGUILD_RESULT 
GuildManager::ChangeGuildNotice(DWORD dwKey, GUILDGUID guild_guid, TCHAR *ptszNotice)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL)
    {
        return RC::RC_GUILD_FAILED;
    }

	return RC::RC_GUILD_SUCCESS;
}
#endif

RC::eGUILD_RESULT	
GuildManager::CanRankUp(GUILDGUID guild_guid, 
                        CHARGUID request_char_guid, 
                        OUT GuildRankInfo& guild_rank_info)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL )
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    if (CheckRight(guild_guid,request_char_guid,eGUILD_RIGHTS_RANKUP) == false)
    {
        return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	if (guild->CheckGuildDeleteState() == eGUILD_DELCHK_DESTROY_WAIT)
    {
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	return guild->CanRankUp(guild_rank_info);
}

void 
GuildManager::RankUpGuild(GUILDGUID guild_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL)
	{
		SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid : %u", 
            guild_guid 
            );
		return;
	}

	guild->RankUp();
}

void 
GuildManager::SetGuildTransaction(GUILDGUID guild_guid, eGUILD_TRANSACTION tran)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found guild : guid : %d, transaction : %d",
            guild_guid, 
            tran
            );
        return;
    }
}

bool		
GuildManager::IsGuildTransaction(GUILDGUID guild_guid, eGUILD_TRANSACTION trans)
{
    __TOUCHED(trans);
	Guild *guild = FindGuild(guild_guid);
	if (guild == NULL)
    {
        return true;
    }

	return true;
}

void 
GuildManager::OnChangeNoticeGuild(bool bSuccess, 
                                  GUILDGUID guild_guid, 
                                  TCHAR * OUT ptszChangedNotice)
{
    __UNUSED(bSuccess);__TOUCHED(ptszChangedNotice);
	Guild *guild = FindGuild( guild_guid );
	ASSERT(guild);
}

RC::eGUILD_RESULT 
GuildManager::ChangeChaoState(GUILDGUID guild_guid, CHARGUID char_guid, BYTE chao_state)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL)
	{
		return RC::RC_GUILD_NOT_EXIST_GUILD;
	}

	GuildMember* member = guild->FindMember(char_guid);
	if (member == NULL)
	{
		return RC::RC_GUILD_FAILED;
	}

	member->SetChaoState(chao_state);

	return RC::RC_GUILD_SUCCESS;
}


void 
GuildManager::DisplayGuildInfo()
{
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
RC::eGUILD_RESULT 
GuildManager::ChuntingPVPResult(GUILDGUID WinGuildGuid, GUILDGUID LoseGuildGuid, GUILDGUID DisGuildGuid)
{
	Guild *pWinGuild = FindGuild( WinGuildGuid );
	if( !pWinGuild )	return RC::RC_GUILD_NOT_EXIST_GUILD;
	CHUNTING_PVP_INFO *pWinPVPInfo = pWinGuild->GetChuntingPVPInfo();

	Guild *pLoseGuild = NULL;
	if( DisGuildGuid )	pLoseGuild = FindGuild( DisGuildGuid );
	else				pLoseGuild = FindGuild( LoseGuildGuid );
	if( !pLoseGuild )	return RC::RC_GUILD_NOT_EXIST_GUILD;
	CHUNTING_PVP_INFO *pLosePVPInfo = pLoseGuild->GetChuntingPVPInfo();

	// 길드 승률
	float fWinGuildWinRatio = 1 / ( 1.0f + pow( 10.0f, (pLosePVPInfo->m_CHuntingPVPPoint-pWinPVPInfo->m_CHuntingPVPPoint) / 400.0f ) );
	float fLoseGuildWinRatio = 1 / ( 1.0f + pow( 10.0f, (pWinPVPInfo->m_CHuntingPVPPoint-pLosePVPInfo->m_CHuntingPVPPoint) / 400.0f ) );

	// 변수 K 결정
	int WinGuildK = 0, LoseGuildK = 0;
	int PVPPointDiff = pWinPVPInfo->m_CHuntingPVPPoint-pLosePVPInfo->m_CHuntingPVPPoint;
	if( PVPPointDiff >= 400 )							WinGuildK = 0;
	else if( pWinPVPInfo->m_CHuntingPVPPoint >= 2400 )	WinGuildK = 20;
	else if( pWinPVPInfo->m_CHuntingTotCount < 30 )		WinGuildK = 50;
	else												WinGuildK = 30;

	if( PVPPointDiff >= 400 )							LoseGuildK = 0;
	else if( pLosePVPInfo->m_CHuntingPVPPoint >= 2400 )	LoseGuildK = 20;
	else if( pLosePVPInfo->m_CHuntingTotCount < 30 )	LoseGuildK = 50;
	else												LoseGuildK = 30;

	// 평가포인트를 적용한다.
	if( DisGuildGuid )
	{
		// Dis의 경우 패배했을 경우보다 2배를 차감시킨다.
		pLosePVPInfo->m_CHuntingPVPPoint -= (int)( LoseGuildK * fLoseGuildWinRatio * 2 );
		pLosePVPInfo->m_CHuntingPVPPoint = max( 0, pLosePVPInfo->m_CHuntingPVPPoint );

		// 전적을 갱신한다.
		pWinPVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingDisCount++;
	}
	else
	{
		pWinPVPInfo->m_CHuntingPVPPoint += (int)( WinGuildK * ( 1 - fWinGuildWinRatio ) );
		pLosePVPInfo->m_CHuntingPVPPoint -= (int)( LoseGuildK * fLoseGuildWinRatio );
		pLosePVPInfo->m_CHuntingPVPPoint = max( 0, pLosePVPInfo->m_CHuntingPVPPoint );

		// 전적을 갱신한다.
		pWinPVPInfo->m_CHuntingTotCount++;
		pWinPVPInfo->m_CHuntingVCount++;
		pLosePVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingLCount++;
	}

	return RC::RC_GUILD_SUCCESS;
}
#endif

//길드 직책 변경
bool
GuildManager::ChangeMemberDuty(GUILDGUID guild_guid, CHARGUID target_char_guid, eGUILD_DUTY duty)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid = %d", 
            guild_guid 
            );
        return false;
    }

    return guild->SetMemberDuty(target_char_guid, duty);
}

GuildMember*	
GuildManager::FindGuildMemberByName(GUILDGUID guild_guid, TCHAR* member_name)
{
    Guild *guild = FindGuild(guild_guid);
    if (guild == NULL)	
    {
        return NULL;
    }

    return guild->FindMemberByCharName(member_name);
}

GuildMember*	
GuildManager::FindGuildMember(GUILDGUID guild_guid, CHARGUID char_guid)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)	
    {
        return NULL;
    }

    return guild->FindMember(char_guid);
}

RC::eGUILD_RESULT	
GuildManager::CanChangeDuty(GUILDGUID guild_guid, 
                            CHARGUID request_char_guid, 
                            CHARGUID target_char_guid, 
                            eGUILD_DUTY change_duty)
{
    if (request_char_guid == target_char_guid)
    {
        return RC::RC_GUILD_CANNOT_COMMAND_MYSELF;
    }

    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)	
    {
        return RC::RC_GUILD_INVALID_STATE;
    }

    GuildMember* request_member = guild->FindMember(request_char_guid);
    if (request_member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

    GuildMember* target_member = guild->FindMember( target_char_guid );
    if (target_member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;	
    }

    if (CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_CHANGE_DUTY) == false)
    {
        return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

    if (change_duty == target_member->GetDuty())
    {
        //변경 직책이 현제 직책과 동일한 경우 실패
        return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY;
    }

    if (request_member->GetDuty() >= target_member->GetDuty())
    {
        //하위직책의 멤버가 상위직책의 멤버의 직책으로 변경 불가
        return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

    if (request_member->GetDuty() >= change_duty)
    {
        //나보다 상위 직책으로 변경 할 수 없다.
        return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

    if (change_duty == eGUILD_DUTY_MASTER || change_duty == eGUILD_DUTY_COMMANDER)
    {
        //마스터나 군단장 변경은 이 함수로 체크 하지 않는다.
        return RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;
    }

    if (change_duty == eGUILD_DUTY_SUBMASTER || 
        change_duty == eGUILD_DUTY_STRATEGIST )
    {
        if (guild->IsExistMemberDuty(change_duty, 0))
        {
            //부마스터와 군사는 한명만 존재해야 한다.
            return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER;
        }

        if (target_member->GetCorpsNumber() != 0)
        {
            //대상이 직할 길드에 있어야만 한다
            return RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;
        }
    }

    if (request_member->GetDuty() == eGUILD_DUTY_COMMANDER || 
        request_member->GetDuty() == eGUILD_DUTY_CAMP_LEADER)
    {
        BYTE corps_num = request_member->GetCorpsNumber();

        if (corps_num != target_member->GetCorpsNumber())
        {
            //명령을 내리는 사람은 현재 군단장 or 훈련대장 뿐이다.
            //따라서 군단장의 군단과 대상의 군단이 같아야 한다.
            return RC::RC_GUILD_CORPS_NOT_FOUND;
        }

        if (corps_num == 0)
        {
            //직할군단은 군장장이나, 훈련대장이 존재하지 않는다.
            return RC::RC_GUILD_INVALID_DUTY_IN_BASE_CORPS;
        }

        GuildCorps* request_corps = guild->FindCorps(corps_num);
        if (request_corps == NULL)
        {
            //군단 존재하는지 체크
            return RC::RC_GUILD_CORPS_NOT_FOUND;
        }

        if (request_corps->FindMember(target_member->GetCharGuid()) == NULL)
        {
            //직책을 바꾸려는 대상이 군단내에 존재 해야 한다
            return RC::RC_GUILD_NOT_GUILD_MEMBER;
        }
    }

    // 길드 마스터는 육성캠프 없어도 훈련대장 직책 변경 가능한 버그 수정 [8/5/2009 lst1024] 
    // 훈련대장 관련 체크 위치 이동
    if (change_duty == eGUILD_DUTY_CAMP_LEADER || change_duty == eGUILD_DUTY_CAMP_SOLDIER)
    {
        BYTE corps_num = target_member->GetCorpsNumber();

        if (corps_num == 0)
        {
            //직할군단은 육성캠프가 없다.
            return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;
        }

        GuildCorps* request_corps = guild->FindCorps( corps_num );
        if (request_corps == NULL)
        {
            //군단 존재하는지 체크.
            return RC::RC_GUILD_CORPS_NOT_FOUND;
        }

        if (request_corps->IsExistRearCamp() == FALSE)
        {
            //훈련대장이나 훈련병으로 바꾸려면 현재 육성캠프가 활성화 되어 있어야 한다.
            return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;
        }

        if (target_member->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER)
        {
            //훈련병이나 훈련대장으로 바꿀때는 현재 직책이 정예병이여야 한다.
            return RC::RC_GUILD_NOT_SOLDIER;
        }
    }

    if (change_duty == eGUILD_DUTY_CAMP_SOLDIER)
    {
        if (target_member->GetLevel() >= 16)
        {
            //훈련병 레벨 체크... 16레벨 보다 크면 훈련병으로 변경하지 못한다.
            return RC::RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL;
        }
    }

    if (change_duty == eGUILD_DUTY_CAMP_SOLDIER)
    {
        //훈련병 변경은 군단장이나 훈련대장만 할 수 있다.
        if (request_member->GetDuty() != eGUILD_DUTY_COMMANDER && 
            request_member->GetDuty() != eGUILD_DUTY_CAMP_LEADER)
        {
            return RC::RC_GUILD_HAVE_NOT_RIGHT;
        }
    }

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    //일반병사의 경우 길드내 직책변경 최대 인원 체크는 필요없다.
    if (change_duty != eGUILD_DUTY_REGULAR_SOLDIER && 
        guild->IsFullMemberOfDuty(change_duty) == true)
    {
        return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER;
    }
#else
    if (guild->IsFullCorpsMemberByDuty( target_member->GetCorpsNumber(), change_duty ))
    {
        //인원 체크 실패
        return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER;
    }
#endif
    return RC::RC_GUILD_SUCCESS;
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL 
//해당 유저가 해당 권한을 실행할 수 있는지 체크한다.
bool		
GuildManager::CheckRight(GUILDGUID guild_guid, CHARGUID char_guid, eGUILD_RIGHTS guild_right)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return false;
    }

    GuildMember* guild_member = guild->FindMember(char_guid);
    if (guild_member == NULL)	
    {
        return false;
    }

    //스크립트 상의 길드 등급별 권한을 체크한다.
    if (!GuildRightInfoParser::Instance()->CheckGradeForRight(guild_right, guild->GetGuildGrade()))
    {
        return false;
    }

    //위임받은 권한이 없으면 스크립트 상에서 직책별 권한이 있는지 체크한다.
    if (!GuildRightInfoParser::Instance()->CheckDutyForRight(guild_right, guild_member->GetDuty()))
    {
        return false;
    }

    return true;
}
#else//_NA_003923_20120130_GUILD_RENEWAL
//해당 유저가 해당 권한을 실행할 수 있는지 체크한다.
bool		
GuildManager::CheckRight(GUILDGUID guild_guid, CHARGUID char_guid, eGUILD_RIGHTS guild_right)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return false;
    }

    GuildMember* guild_member = guild->FindMember(char_guid);
    if (guild_member == NULL)	
    {
        return false;
    }

    //스크립트 상의 길드 등급별 권한을 체크한다.
    if (!GuildRightInfoParser::Instance()->CheckGradeForRight(guild_right, guild->GetGuildGrade()))
    {
        return false;
    }

    //현재 유저의 직책이 현재 길드등급에 등록되어 있는 것인지 체크한다.
    GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild->GetGuildGrade());
    if (guild_rank_info == NULL)
    {
        return false;
    }

    //유저의 직책이 길드에서 사용하는 직책인지 체크
    GUILD_DUTY_USE_INFO* duty_use_info = guild_rank_info->Find(guild_member->GetDuty());
    if (duty_use_info == NULL || duty_use_info->m_bUse == false )
    {
        return false;
    }

    //서브 마스터와 군사에게만 위임된 권한이 존재한다.
    if (guild_member->GetDuty() == eGUILD_DUTY_SUBMASTER || 
        guild_member->GetDuty() == eGUILD_DUTY_STRATEGIST)
    {
        //유저에게 위임받은 권한이 있는지 체크.
        if(guild_member->CheckDelegateRight(guild_right))
        {
            return true;
        }
    }

    //위임받은 권한이 없으면 스크립트 상에서 직책별 권한이 있는지 체크한다.
    if (!GuildRightInfoParser::Instance()->CheckDutyForRight(guild_right, guild_member->GetDuty()))
    {
        return false;
    }

    return true;
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

void 
GuildManager::GetCharName(GUILDGUID guild_guid, 
                          CHARGUID char_guid, 
                          OUT TCHAR* char_name, 
                          IN BYTE buf_size)
	{
		if (buf_size == 0)	
        {
            return;
        }

		char_name[0] = '\0';

		Guild *guild = FindGuild(guild_guid);
		if (guild == NULL)
        {
            return;
        }

		GuildMember* member = guild->FindMember(char_guid);
		if (member == NULL)
        {
            return;
        }

		strncpy(char_name, member->GetCharName(), buf_size);
	}

RC::eGUILD_RESULT	
GuildManager::IsDelegateChar(GUILDGUID guild_guid, CHARGUID target_char_guid)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid = %d", 
            guild_guid 
            );
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    GuildMember* member = guild->FindMember( target_char_guid );
    if (member == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild member : char guid = %d", 
            guild_guid
            );
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

    if (CheckDelegatedRight(member->GetDuty()) == false)
    {
        return RC::RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER;
    }

    return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::IsDelegateChar(GUILDGUID guild_guid, TCHAR* target_char_name)
{
    Guild *guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid = %d", 
            guild_guid 
            );
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    GuildMember* member = guild->FindMemberByCharName(target_char_name);
    if (member == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild member : char guid = %d", 
            guild_guid 
            );
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

    if (CheckDelegatedRight(member->GetDuty()) == false)
    {
        return RC::RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER;
    }

    return RC::RC_GUILD_SUCCESS;
}

bool 
GuildManager::CheckDelegatedRight(const eGUILD_DUTY& duty)
{
    if (duty == eGUILD_DUTY_SUBMASTER || duty == eGUILD_DUTY_STRATEGIST)
    {
        return true;
    }

    return false;
}

void 
GuildManager::GetDelegateRight(GUILDGUID guild_guid, 
                               TCHAR* target_char_name, 
                               OUT GUILD_RIGHT_USE_INFO* delegate_right)
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid = %d", 
            guild_guid
            );
        return;
    }

    GuildMember* member = guild->FindMemberByCharName(target_char_name);
    if (member == NULL)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found member : guild Guid = %d", 
            guild_guid 
            );
        return;
    }

    member->GetDelegateRight(delegate_right);
}

bool 
GuildManager::IsDelegateRight(const GUILD_RIGHT_USE_INFO* right_info)
{
    for (int i = 0; i < MAX_GUILD_DELEGATE_RIGHT; i++)
    {
        switch(right_info[i].m_Right)
        {
        case eGUILD_RIGHTS_GUILD_MARK:
            break;
        case eGUILD_RIGHTS_RANKUP:
            break;
        case eGUILD_RIGHTS_RELATION:
            break;
        case eGUILD_RIGHTS_SEIGE_WARFARE :
            break;
        case eGUILD_RIGHTS_UP2GP:
            break;
        case eGUILD_RIGHTS_USE_UPGP:
            break;
        default:
            return false;
            break;
        }
    }

    return true;
}


void
GuildManager::ChangeDelegateRight(GUILDGUID guild_guid, 
                                  CHARGUID target_char_guid, 
                                  GUILD_RIGHT_USE_INFO* delegate_right)
{
    Guild*guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid = %d", 
            guild_guid 
            );
        return;
    }

    GuildMember* target_member = guild->FindMember( target_char_guid );
    if (target_member == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild member : member guid = %d", 
            target_char_guid 
            );
        return;
    }

    if (delegate_right == NULL)
    {
        return;
    }

    target_member->SetDelegateRight(delegate_right);
    GUILDLOG->LogDelegation(
        guild, 
        target_member, 
        static_cast<BYTE>(delegate_right->m_Right)
        );
}

bool		
GuildManager::ChangeMaster(GUILDGUID guild_guid, 
                           CHARGUID old_master_guid, 
                           CHARGUID new_master_guid, 
                           INT64 master_change_time)
{
	Guild *guild = FindGuild(guild_guid);
	if (guild == NULL)
	{
		SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild : guild guid = %d", 
            guild_guid
            );
		return false;
	}

	GuildMember* old_master = guild->FindMember(old_master_guid);
	if (old_master == NULL)
	{
		SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild member : member guid = %d", 
            old_master_guid 
            );
		return false;
	}

	GuildMember* new_master = guild->FindMember(new_master_guid);
	if (new_master == NULL)
	{
		SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found guild member : member guid = %d", 
            new_master_guid 
            );
		return false;
	}

	//1. 기존의 마스터는 정예로 되고, 새로운 마스터의 직책을 마스터로 변경한다
    const eGUILD_DUTY prev_duty_of_new_master = new_master->GetDuty();
	old_master->SetDuty( eGUILD_DUTY_REGULAR_SOLDIER );
	new_master->SetDuty( eGUILD_DUTY_MASTER );

	//2. 위임된 권한은 둘다 초기화 시키자.
	old_master->InitDelegateRight();
	new_master->InitDelegateRight();

	//시간을 세팅해야 되지 않을까?
	guild->SetMasterChangeTime( master_change_time );

    // 길드마스터 변경에 의한 길드 직책 변경 로그 추가
    GUILDLOG->LogDutyChange(guild, old_master, eGUILD_DUTY_MASTER);
    GUILDLOG->LogDutyChange(guild, new_master, prev_duty_of_new_master);

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild->GetGuildInGameLog()->WriteGuildLog(
        GuildChangeMaster,
        guild->GetGuildGuid(),
        new_master->GetCharName()
        );
#endif

	return true;
}

RC::eGUILD_RESULT	
GuildManager::CanChangeMaster(GUILDGUID guild_guid, 
                              CHARGUID old_master_guid , 
                              TCHAR* new_master_name)
{
	//1. 길드가 있는지 체크
	Guild* guild = g_GuildManager.FindGuild(guild_guid);
	if (guild == NULL)
    {
		return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 둘다 길드 멤버인지 체크
	GuildMember* old_master = guild->FindMember(old_master_guid);
	if (old_master == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	GuildMember* new_master = guild->FindMemberByCharName(new_master_name);
	if (new_master == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	// 직책 비교 ( 마스터가 아니면 안된다. )
	if (old_master->GetDuty() != eGUILD_DUTY_MASTER || 
        old_master->GetCorpsNumber() != 0 )
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	// 이미 마스터 인데 마스터가 되면 안된다
	if (new_master->GetDuty() == eGUILD_DUTY_MASTER)
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	// 직할길드가 아니면 안된다.
	if (new_master->GetCorpsNumber() != 0)
    {
		return RC::RC_GUILD_NOT_BASE_CORPS;
    }

	return RC::RC_GUILD_SUCCESS;			 

}

RC::eGUILD_RESULT	
GuildManager::CanRightDelegate(GUILDGUID guild_guid, 
                               CHARGUID request_char_guid, 
                               TCHAR* target_char_name, 
                               const GUILD_RIGHT_USE_INFO* pRightInfo)
{
	//1. 길드가 있는지 체크
	Guild* guild = g_GuildManager.FindGuild(guild_guid);
	if (guild == NULL)
    {
		return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	//2. 타켓대상이 있는지 확인한다.
	GuildMember* target_member = guild->FindMemberByCharName( target_char_name );
	if (target_member == NULL)
    {
		return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//3. 리스트 요청자가 리스트를 요청해도 되는 사람인지 체크한다.
	if (!g_GuildManager.CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_DELEGATION_RIGHT))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//4. 권한위임대상이 위임대상이 되는지 체크.
	RC::eGUILD_RESULT rc = g_GuildManager.IsDelegateChar(guild_guid, target_member->GetCharGuid());	
	if (rc != RC::RC_GUILD_SUCCESS)
    {
        return rc;
    }

	//4. 현재 위임하려는 권한들이 정확한 위임권한인지 체크.
	if (!g_GuildManager.IsDelegateRight( pRightInfo ))
    {
		return RC::RC_GUILD_INVALID_DELEGATE_RIGHT;
    }

	return RC::RC_GUILD_SUCCESS;
}

CHARGUID GuildManager::FindRightMember(GUILDGUID guild_guid, eGUILD_DUTY duty)
{
	Guild* guild = g_GuildManager.FindGuild(guild_guid);
	if (guild)
	{
		GuildMember* Member = guild->FindMemberByDuty(duty);
		if (Member && Member->IsLogIn()) 
        {
            return Member->GetCharGuid();
        }
	}

	return 0;
}

// 길드 군단 생성
RC::eGUILD_RESULT GuildManager::CreateCorps(GUILDGUID guild_guid, 
                                            BYTE corps_num, 
                                            CHARGUID target_char_guid)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildCorps* corps = guild->CreateCorps(corps_num);
	if (corps == NULL) 
    {
        return RC::RC_GUILD_CREATE_CORPS_FAILED;
    }

	RC::eGUILD_RESULT rc = 
        g_GuildManager.ChangeCommander(
        guild_guid, 
        corps_num, 
        target_char_guid 
        );

	if (rc != RC::RC_GUILD_SUCCESS)
    {
		return rc;
    }

	if (GuildMember* member = g_GuildManager.FindGuildMember(guild_guid, target_char_guid))
	{
		GUILDLOG->LogCorpsCreation(guild, member, corps_num);
	}

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	
GuildManager::CanCreateCorps(GUILDGUID guild_guid, 
                             CHARGUID request_char_guid, 
                             const TCHAR* target_char_name, 
                             OUT BYTE& corps_num)
{
	// 권한체크..
	//1. 길드가 있는지 체크
	Guild* guild = g_GuildManager.FindGuild(guild_guid);
	if (guild == NULL)
    {
		return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	//2. 군단생성 요청자가 해당명령을 요청해도 되는 사람인지 체크한다.
	if (!g_GuildManager.CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_CREATE_CORPS))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//3. 현재 군단 창설 개수를 체크하여 창설 가능한지 확인한다.
	const GuildRankInfo* rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo( guild->GetGuildGrade() );
	if (rank_info == NULL)
    {
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;
    }

	corps_num = guild->GetEmptyCorpsNumBySequence();
	if (corps_num == 0)
    {
		return RC::RC_GUILD_CORPS_NUM_OVERFLOW;
    }

	// 군단장이 될 사람이 현재 길드 멤버인지 확인한다.
	GuildMember* target_member = guild->FindMemberByCharName(target_char_name);
	if (target_member == NULL)
    {
		return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	// 군단장 될 사람이 직할 군단인지 체크
	if (target_member->GetCorpsNumber() != 0)
    {
		return RC::RC_GUILD_NOT_BASE_CORPS;
    }

	// 군단장 될 사람이 정예병인지 체크.
	if (target_member->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER)
    {
		return RC::RC_GUILD_NOT_SOLDIER;
    }

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	
GuildManager::CanDestoryCorps( GUILDGUID guild_guid, CHARGUID request_char_guid, BYTE corps_num )
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL ) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildMember* request_member = guild->FindMember(request_char_guid);
	if (request_member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//1. 직할 군단이면 안된다.
	if (corps_num == 0)
    {
		return RC::RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS;
    }

	//2. 권한체크.
	if (!CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_DISMISS_CORPS))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	GuildCorps* corps = guild->FindCorps( corps_num );
	if (corps == NULL)
    {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	// 군단 해산 조건
	// 1. 군단내에 인원이 없어야 한다.
	if (corps->GetMemberCount() > 0)
    {
		return RC::RC_GUILD_EXIST_MEMBER;
    }

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT 
GuildManager::DestoryCorps(GUILDGUID guild_guid, 
                                             BYTE corps_num, 
                                             CHARGUID request_char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	guild->DestroyCorps(corps_num);

    GuildMember* member = guild->FindMember(request_char_guid);
    if (member)
    {
    	GUILDLOG->LogCorpsBreakUp(guild, member, corps_num);
    }

	return RC::RC_GUILD_SUCCESS;
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT	
GuildManager::CanCreateGuild(LEVELTYPE master_level)
{
    LEVELTYPE guild_create_level = GuildInfoParser::Instance()->GetGuildInfo().guild_create_level_;
    if (master_level < guild_create_level)
    {
        return RC::RC_GUILD_CANNOT_CREAT_LEVEL_STATE;
    }

    //하임/카오상태 체크, 게임서버에서 하임 체크 하고 온다.
    return RC::RC_GUILD_SUCCESS;
}
#else//_NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT	
GuildManager::CanCreateGuild(LEVELTYPE master_level)
{
	//생성이므로 Legion의 정보를 가지고 온다.
	GuildRankInfo* rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(GRADE_AT_GUILD_CREATE);
	if (rank_info == NULL)
    {
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;
    }

	//길드마스터 레벨 체크 
	if (master_level < CREATE_GUILD_LIMIT_LEVEL)
    {
		return RC::RC_GUILD_CANNOT_CREAT_LEVEL_STATE;
    }

	//하임/카오상태 체크, 게임서버에서 하임 체크 하고 온다.
	return RC::RC_GUILD_SUCCESS;
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

RC::eGUILD_RESULT		
GuildManager::CreateGuild(GUILDGUID guild_guid, 
                          const TCHAR* guild_name, 
                          BYTE corps_num, 
                          CHARGUID master_guid, 
                          const TCHAR* master_name, 
                          eGUILD_DUTY master_duty, 
                          BYTE master_class, 
                          LEVELTYPE master_level, 
                          BYTE is_master_chao_state,
                          INT64 guild_panalty_time, 
                          INT64 destroy_request_time, 
                          INT64 master_change_time )
{
	if (FindGuild(guild_guid))
	{
		SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Fail create guild, already exist guild : guild guid = %d",
            guild_guid 
            );
		return RC::RC_GUILD_ALREADY_EXIST_GUILD;
	}

	Guild* guild = Guild::ALLOC();

	guild->InitializeGuild();
    
    guild->CreateGuild( 
        guild_guid, 
        guild_name, 
        corps_num, 
        master_guid, 
        master_name, 
        master_duty, 
        master_class, 
        master_level,
		is_master_chao_state, 
        guild_panalty_time, 
        destroy_request_time, 
        master_change_time 
        );

	AddGuild(guild, guild->GetGuildGuid());
	AddGuildName(guild->GetGuildName(), guild->GetGuildGuid());

	GUILDLOG->LogGuildCreation(guild);

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild->GetGuildInGameLog()->WriteGuildLog(
        GuildCreate,
        guild->GetGuildGuid(),
        guild->GetGuildName()
        );
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 길드를 창설하였으므로 다른 길드의 가입 요청 목록에서 모두 지우기 위한 절차이다
    InitailizeGuildJoinRequest(master_guid);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	return RC::RC_GUILD_SUCCESS;
}

GuildMember*
GuildManager::FindMaster(GUILDGUID guild_guid)
{
	Guild* guild = FindGuild(guild_guid);

	if (guild == NULL)
    {
        return NULL;
    }

	return guild->FindMaster();
}

RC::eGUILD_RESULT  	
GuildManager::CanDestroyGuild(GUILDGUID guild_guid, CHARGUID request_char_guid)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	//길드 해체는 마스터만이 할 수 있다.
	GuildMember* master = guild->FindMaster();
	if (master == NULL)
    {
        return	RC::RC_GUILD_CURRENT_NONE_MASTER;
    }

	if (master->GetCharGuid() != request_char_guid)
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	// 길드 삭제 대기 상태면 Destroy할 수 없다.
	if (eGUILD_DELCHK_DESTROY_WAIT == guild->CheckGuildDeleteState())
    {
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	return RC::RC_GUILD_SUCCESS;
}

void 
GuildManager::DestroyGuild(GUILDGUID guild_guid)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return;
    }

	GUILDLOG->LogGuildBreakUpCompleted(guild);

	RemoveGuild(guild_guid);
	RemoveGuildName(guild->GetGuildName());
	guild_penalty_mgr_.erase(guild_guid);
    
    guild->ReleaseGuild();
	Guild::FREE(guild);
}

BYTE 
GuildManager::GetEmptyCommanderInfo(GUILDGUID guild_guid, OUT BYTE* empty_commander_info)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return 0;
    }

	return guild->GetEmptyCommanderInfo(empty_commander_info);
}

RC::eGUILD_RESULT  
GuildManager::CanChangeCommander(GUILDGUID guild_guid, 
                                 CHARGUID request_char_guid, 
                                 const TCHAR* master_name, 
                                 BYTE corps_num)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	//직할 군단은 군단장이 없다.
	if (corps_num == 0)
    {
		return RC::RC_GUILD_INVALID_DUTY_IN_BASE_CORPS;
    }

	GuildMember* request_member = guild->FindMember( request_char_guid );
	if (request_member == NULL)
    {
		return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	if (!CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_CHANGE_DUTY))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	GuildMember* target_member = guild->FindMemberByCharName( master_name );
	if (target_member == NULL)
    {
		return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//현재 군단의 군단장이 있으면 변경을 하지 못한다.
	GuildCorps* corps = guild->FindCorps(corps_num);
	if (corps == NULL)
    {
		return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	if (corps->FindCommander())
    {
		return RC::RC_GUILD_ALREADY_EXIST_COMMANDER;
    }

	// 현재 군단장 시키려는 자의 직책이 군단장이면 안된다.
	if (target_member->GetDuty() == eGUILD_DUTY_COMMANDER)
    {
		return RC::RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER;
    }

	//군단장은 1명이므로 인원수 체크 안한다.

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT
GuildManager::ChangeCommander(GUILDGUID guild_guid, BYTE corps_num, CHARGUID commander_guid)
{
	Guild* guild = FindGuild( guild_guid );
    if (guild == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildCorps* corps = guild->FindCorps( corps_num );
	if (corps == NULL)
    {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	GuildMember* commander = guild->FindMember( commander_guid );
	if (commander == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//군단을 바꾼다.
	RC::eGUILD_RESULT rt =guild->ChangeCorps( commander_guid, corps_num );
	if (rt != RC::RC_GUILD_SUCCESS)
    {
		return rt;
    }

	//2. 직책를 바꾸어 주고
	commander->SetDuty( eGUILD_DUTY_COMMANDER );

	return RC::RC_GUILD_SUCCESS;
}

// 군단 변경
RC::eGUILD_RESULT  	
GuildManager::CanChangeCorps(GUILDGUID guild_guid, 
                             CHARGUID request_char_guid, 
                             const TCHAR* master_name, 
                             BYTE corps_num)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL ) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildMember* request_member = guild->FindMember(request_char_guid);
	if (request_member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	GuildMember* target_member = guild->FindMemberByCharName(master_name);
	if (target_member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	// 옮기려는 유저의 직책 체크 ( 반드시 정예병 이어야 한다. )
	if (target_member->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER)
    {
		return RC::RC_GUILD_NOT_SOLDIER;
    }

	if (!CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_CHANGE_CORPS))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//변경하려는 군단과 현재 타겟유저의 군단 번호가 같으면 안된다. )
	if (target_member->GetCorpsNumber() == corps_num)
    {
		return RC::RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS;
    }

	//현재 타겟유저와 옮길려는 군단이 존재해야 한다.
	if (!guild->FindCorps( target_member->GetCorpsNumber()))
    {
		return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	GuildCorps* target_corps = guild->FindCorps( corps_num );
	if (target_corps == NULL)
    {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	//인원 체크 한다.
	if (guild->IsFullCorpsMemberByDuty( corps_num, target_member->GetDuty()))
    {
		return RC::RC_GUILD_CORPS_FULL_MEMBER;
    }

	return RC::RC_GUILD_SUCCESS;
}

//군단간 변경
RC::eGUILD_RESULT	
GuildManager::ChangeCorps(GUILDGUID guild_guid, CHARGUID target_char_guid, BYTE corps_num )
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	return guild->ChangeCorps(target_char_guid, corps_num);
}

//군단 육성 캠프 생성
RC::eGUILD_RESULT  	
GuildManager::CanCreateRearCamp(GUILDGUID guild_guid, 
                                CHARGUID request_char_guid, 
                                const TCHAR* target_char_name, 
                                OUT BYTE& corps_num)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	//멤버 확인
	GuildMember* request_member = guild->FindMember( request_char_guid );
	if (request_member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//1. 권한체크.
	if (!CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_CREATE_CAMP))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//2. 군단존재 확인
	GuildCorps* corps = guild->FindCorps( request_member->GetCorpsNumber() );
	if (corps == NULL)
    {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	//3. 훈련대장 될 놈이 같은 군단인지 확인.
	GuildMember* target_member = corps->FindMember( target_char_name );
	if (target_member == NULL)
    {
        return RC::RC_GUILD_NOT_CORPS_MEMBER;
    }

	//4. 직책이 군단장인지 확인..
	if (request_member->GetDuty() != eGUILD_DUTY_COMMANDER)
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//5. 타겟멤버가 정예병인지 확인.
	if (target_member->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER)
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//6. 현재 육성캠프가 있는지 체크.
	if (corps->IsExistRearCamp())
    {
		return RC::RC_GUILD_ALREADY_EXIST_REAR_CAMP;
    }

	//군단번호 리턴..
	corps_num = request_member->GetCorpsNumber();

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	
GuildManager::CreateRearCamp( GUILDGUID guild_guid, BYTE corps_num, CHARGUID target_char_guid )
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	if (guild->CreateRearCamp(corps_num) == FALSE)
    {
		return RC::RC_GUILD_FAILED;
    }

	//1. 멤버의 군단변경
	RC::eGUILD_RESULT rc = ChangeCorps(guild_guid, target_char_guid, corps_num);
	if (rc != RC::RC_GUILD_SUCCESS)
    {
		return rc;
    }

	//2. 멤버의 직책변경( 훈련대장 )
	GuildMember* member = FindGuildMember( guild_guid, target_char_guid );
	if (member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	member->SetDuty( eGUILD_DUTY_CAMP_LEADER );

	GUILDLOG->LogCampCreation(guild, member, corps_num);

	return RC::RC_GUILD_SUCCESS;
}

// 원래 해산은 군단장만이 할 수 있으므로 군단번호가 필요 없을거 같지만,
// 나중에 길마나 또는 누군가가 할 수 있을지도 모른다. 
// 그래서 군단번호를 받자.
RC::eGUILD_RESULT  	
GuildManager::CanDestroyRearCamp(GUILDGUID guild_guid, CHARGUID request_char_guid, BYTE corps_num)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildMember* request_member = guild->FindMember(request_char_guid);
	if (request_member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//1. 직할 군단이면 안된다.
	if (corps_num == 0)
    {
		return RC::RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS;
    }

	//2. 권한체크.
	if (CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_DISMISS_CAMP) == false)
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	// 훈련캠프 해산 조건
	// 1. 훈련대장이랑 훈련병이 없어야 한다.
	GuildCorps* corps = guild->FindCorps(corps_num);
	if (corps == NULL)
    {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

	// 현재 훈련캠프가 있는지 체크.
	if (corps->IsExistRearCamp() == FALSE)
    {
		return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;
    }

	// 훈련대장이 있으면 안된다.
	if (corps->GetMemberCountByDuty( eGUILD_DUTY_CAMP_LEADER ) > 0)
    {
		return RC::RC_GUILD_EXIST_MEMBER;
    }

	// 훈련병이 있으면 안된다.
	if (corps->GetMemberCountByDuty( eGUILD_DUTY_CAMP_SOLDIER ) > 0)
    {
		return RC::RC_GUILD_EXIST_MEMBER;
    }

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	
GuildManager::DestroyRearCamp(GUILDGUID guild_guid, BYTE corps_num, CHARGUID request_char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	if (guild->DestroyRearCamp( corps_num ) == false)
    {
		return RC::RC_GUILD_FAILED;
    }

    GuildMember* member = guild->FindMember(request_char_guid);
    if (member)
    {
    	GUILDLOG->LogCampBreakUp(guild, member, corps_num);
    }

	return RC::RC_GUILD_SUCCESS;
}

//길드초대가 가능한가?
RC::eGUILD_RESULT	GuildManager::CanInviteGuild(const GUILDGUID guild_guid, 
                                                 const CHARGUID request_guid, 
                                                 const CHARGUID target_guid, 
                                                 const LEVELTYPE target_level )
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	if (CheckRight(guild_guid, request_guid, eGUILD_RIGHTS_JOIN) == false)
    {
        //권한체크
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	if (guild->CheckGuildDeleteState() == eGUILD_DELCHK_DESTROY_WAIT)
    {
        //길드 삭제 대기 상태면 Join할 수 없다.
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	//길드가 현재 패널티에 적용을 받고 있는 상태인지
	if (GuildSmallContentEntry::GuildPenalty::CheckGuildPenaltyTime(guild) == false)
    {
		return RC::RC_GUILD_WITHDRAW_PENALTY_TIME;
    }

	// TODO : 군단 패널티도 체크..현재는 DB에서만 체크하고 있음

	//해당 멤버가 다른 길드원이면 가입하지 못한다. 
	//다른 길드의 멤버인지 체크는 DB에서 처리 한다. 
    //여기서는 해당 길드에 길드원인지만 체크한다.

	GuildMember* request_member = guild->FindMember(request_guid);
	if (request_member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	GuildMember* target_member = guild->FindMember(target_guid);
	if (target_member)
    {
        return RC::RC_GUILD_ALREADY_GUILD_MEMBER;
    }

	eGUILD_DUTY guild_duty = eGUILD_DUTY_REGULAR_SOLDIER;
	if (request_member->GetDuty() == eGUILD_DUTY_CAMP_LEADER)
	{
        //가입시킨놈이 훈련대장이고,
		if (target_level >= 16)
        {
            return RC::RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL;
        }

		guild_duty = eGUILD_DUTY_CAMP_SOLDIER;
	}

	//3. 군단결정(가입시키려는 놈이 군단장이나, 훈련대장이면 같은 군단에 가입하는거다.)
	BYTE corps_number = 0;
	if (request_member->GetDuty() == eGUILD_DUTY_CAMP_LEADER || 
        request_member->GetDuty() == eGUILD_DUTY_COMMANDER)
    {
		corps_number = request_member->GetCorpsNumber();
    }

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    if (guild->IsFullMember() == true)
    {
        return RC::RC_GUILD_FULL_MEMBER_STATE;
    }
#else
    //인원 체크 한다.
    if (guild->IsFullCorpsMemberByDuty(corps_number, guild_duty))
    {
        return RC::RC_GUILD_CORPS_FULL_MEMBER;
    }
#endif
	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	GuildManager::CanJoinGuild(const GUILDGUID& guild_guid, 
                                               const CHARGUID& request_char_guid, 
                                               const CHARGUID& target_char_guid, 
                                               const LEVELTYPE& target_level, 
                                               OUT eGUILD_DUTY& duty, 
                                               OUT BYTE& corps_num )
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 권한체크 .
	if (!CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_JOIN ))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	// 길드 삭제 대기 상태면 Join할 수 없다.
	if (eGUILD_DELCHK_DESTROY_WAIT == guild->CheckGuildDeleteState())
    {
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	if (!GuildSmallContentEntry::GuildPenalty::CheckGuildPenaltyTime(guild))
    {
		return RC::RC_GUILD_KICK_PENALTY_TIME;
    }

	// 해당 멤버가 다른 길드원이면 가입하지 못한다. 
	// 이거 처리 어디서 해줄 것인가? DB에서 해주자.. 여기서는 해당 길드에 길드원인지만 체크한다.
	// 그 전에 게임서버에서 해당 캐릭터에 대한 길드 체크를 하고 여기 온다.
	GuildMember* target_member = guild->FindMember(target_char_guid);
	if (target_member)	
    {
        return RC::RC_GUILD_ALREADY_GUILD_MEMBER;
    }

	//2. 타겟멤버의 직책을 결정한다.
	GuildMember* request_member = guild->FindMember(request_char_guid);
	if (request_member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//가입시킨놈이 훈련대장이고,
	duty = eGUILD_DUTY_REGULAR_SOLDIER;
	if (request_member->GetDuty() == eGUILD_DUTY_CAMP_LEADER)
	{
		if (target_level >= 16)
        {
			return RC::RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL;
        }

		duty = eGUILD_DUTY_CAMP_SOLDIER;
	}

	//3. 군단결정( 가입시키려는 놈이 군단장이나, 훈련대장이면 같은 군단에 가입하는거다. )
	corps_num = 0;
	if (request_member->GetDuty() == eGUILD_DUTY_CAMP_LEADER || 
        request_member->GetDuty() == eGUILD_DUTY_COMMANDER)
    {
		corps_num = request_member->GetCorpsNumber();
    }

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    if (guild->IsFullMember() == true)
    {
        return RC::RC_GUILD_FULL_MEMBER_STATE;
    }
#else
    //인원 체크 한다.
    if (guild->IsFullCorpsMemberByDuty(corps_num, duty))
    {
        return RC::RC_GUILD_CORPS_FULL_MEMBER;
    }
#endif
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::JoinGuild(GUILDGUID guild_guid, 
                        BYTE corps_num, 
                        CHARGUID char_guid, 
                        const TCHAR* char_name, 
                        eGUILD_DUTY duty, 
                        BYTE char_class, 
                        LEVELTYPE Level, 
                        BYTE chao_state)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }
	
	RC::eGUILD_RESULT result = 
        guild->JoinGuild( corps_num, char_guid, char_name, duty, char_class, Level, chao_state);
	if(RC::RC_GUILD_SUCCESS == result)
	{
		GUILDLOG->LogMemberJoin(guild, char_guid, char_name);
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        guild->GetGuildInGameLog()->WriteGuildLog(
            GuildMemberJoin,
            guild->GetGuildGuid(),
            char_name
            );
#endif
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // 길드에 가입하였으므로 다른 길드의 가입 요청 목록에서 지우기 위한 절차이다
        InitailizeGuildJoinRequest(char_guid);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	}

	return result;
}

// #길드탈퇴#
RC::eGUILD_RESULT	
GuildManager::CanLeaveGuild(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }
	
	// 길드 삭제 대기 상태면 탈퇴할 수 없다.
	if (eGUILD_DELCHK_DESTROY_WAIT == guild->CheckGuildDeleteState())
    {
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	//1. 해당 길드의 길드원이냐?
	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//2. 해당 길드의 마스터만 빼고는 전부 탈퇴 가능하다.
	GuildMember* master = guild->FindMaster();
	if (master == NULL)	
    {
        return RC::RC_GUILD_CURRENT_NONE_MASTER;
    }

	if (member->GetCharGuid() == master->GetCharGuid())
    {
		return RC::RC_GUILD_CANNOT_LEAVE_MASTER;
    }
		
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::LeaveGuild( GUILDGUID guild_guid, CHARGUID char_guid )
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	//1. 해당 길드의 길드원이냐?
	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	GUILDLOG->LogMemberLeave(guild, member);
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild->GetGuildInGameLog()->WriteGuildLog(
        GuildMemberOut,
        guild->GetGuildGuid(),
        member->GetCharName()
        );
#endif

	//DestroyMember는 다른 리턴도 있는데 무조건 성공리턴??
	guild->DestroyMember(char_guid);
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::CanKickMember(GUILDGUID guild_guid, CHARGUID request_char_guid, TCHAR* char_name)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;		
    }

	// 길드 삭제 대기 상태면 Kick 할 수 없다.
	if (eGUILD_DELCHK_DESTROY_WAIT == guild->CheckGuildDeleteState())
    {
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	// 권한체크 .
	if (!CheckRight(guild_guid, request_char_guid, eGUILD_RIGHTS_KICK))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	GuildMember* request_member = guild->FindMember( request_char_guid );
	if (request_member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	GuildMember* target_member = guild->FindMemberByCharName( char_name );
	if (target_member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	// 자기 자신은 탈퇴시킬 수 없다.
	if (request_member->GetCharGuid() == target_member->GetCharGuid())
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	//명령자보다 상위직책을 제명할 수 없다.
	if (request_member->GetDuty() > target_member->GetDuty())
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	if (target_member->GetDuty() == eGUILD_DUTY_MASTER)
    {
		return RC::RC_GUILD_CANNOT_LEAVE_MASTER;
    }

	return RC::RC_GUILD_SUCCESS;
}

// #길드강퇴#
RC::eGUILD_RESULT	
GuildManager::KickMember(GUILDGUID guild_guid, 
                         CHARGUID request_char_guid, 
                         CHARGUID target_char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL )
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    GuildMember* request_member = guild->FindMember( request_char_guid );
    if (request_member == NULL) 
    { 
        return RC::RC_GUILD_NOT_GUILD_MEMBER; 
    }

	GuildMember* target_member = guild->FindMember( target_char_guid );
    if (target_member == NULL) 
    { 
        return RC::RC_GUILD_NOT_GUILD_MEMBER; 
    }

	GUILDLOG->LogMemberKickOut(guild, request_member, target_member);

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild->GetGuildInGameLog()->WriteGuildLog(
        GuildMemberKick,
        guild->GetGuildGuid(),
        target_member->GetCharName()
        );
#endif
	
	//DestroyMember는 다른 리턴도 있는데 무조건 성공리턴??
	guild->DestroyMember( target_char_guid );
	return RC::RC_GUILD_SUCCESS;
}

//Serialize
bool		
GuildManager::SerializeBaseGuildInfo(GUILDGUID guild_guid, 
                                     INOUT BASE_GUILD_INFO& info, 
                                     eSERIALIZE se)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL ) 
    {
        return false;
    }

	guild->SerializeBaseGuildInfo( info, se );
	return true;
}

bool		
GuildManager::SerializeGameGuildInfo(GUILDGUID guild_guid, 
                                     INOUT GAME_GUILD_INFO& info, 
                                     eSERIALIZE se)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return false;
    }

	guild->SerializeGameGuildInfo(info, se);
	return true;
}

bool		
GuildManager::SerializeGuildInfo(GUILDGUID guild_guid, INOUT GUILD_INFO& info, eSERIALIZE se)
{
	if (se == SERIALIZE_LOAD)
	{
		Guild* guild = FindGuild( guild_guid );
		if (guild == NULL)
        {
            return false;
        }

		guild->SerializeGuildInfo(info, se);
	}
	else
	{
		Guild* guild = FindGuild( guild_guid );
		if (guild == NULL)
		{
			Guild* new_guild = Guild::ALLOC();
			new_guild->InitializeGuild();
			new_guild->SerializeGuildInfo(info, se);
			AddGuild(new_guild, new_guild->GetGuildGuid());
			AddGuildName(new_guild->GetGuildName(), new_guild->GetGuildGuid());
		}
		else
		{
			guild->SerializeGuildInfo(info, se);
		}
	}

	return true;
}

bool		
GuildManager::SerializeCorpsInfo(GUILDGUID guild_guid, 
                                 INOUT GUILD_CORPS_TOTAL_INFO& info, 
                                 eSERIALIZE se)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return false;
    }

	guild->SerializeCorpsInfo(info, se);
	return true;
}


bool		
GuildManager::SerializeBaseMemberTotalInfo(GUILDGUID guild_guid, 
                                           INOUT INOUT BASE_GUILD_MEMBER_TOTAL_INFO& info, 
                                           eSERIALIZE se)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return false;
    }

	guild->SerializeBaseMemberTotalInfo(info, se);
	return true;
}

bool		
GuildManager::SerializeMemberTotalInfo(GUILDGUID guild_guid, 
                                       INOUT GUILD_MEMBER_TOTAL_INFO& info, 
                                       eSERIALIZE se)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return false;
    }

	guild->SerializeMemberTotalInfo(info, se);
	return true;
}

bool		
GuildManager::SerializeServerMemberTotalInfo(GUILDGUID guild_guid, 
                                             INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& info, 
                                             eSERIALIZE se)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return false;
    }

	guild->SerializeServerMemberTotalInfo(info, se);
	return true;
}

bool		
GuildManager::GetGuildBaseMemberInfo(GUILDGUID guild_guid, 
                                     CHARGUID char_guid, 
                                     OUT BASE_GUILD_MEMBER_INFO& info)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return false;
    }

	return guild->GetBaseMemberInfo(char_guid, info);
}

bool		
GuildManager::GetGuildMemberInfo(GUILDGUID guild_guid, 
                                 CHARGUID char_guid, 
                                 OUT GUILD_MEMBER_INFO& info)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return false;
    }

	return guild->GetMemberInfo(char_guid, info);
}


RC::eGUILD_RESULT	
GuildManager::CanLogin(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	if (member->IsLogIn() == TRUE)
    {
		return RC::RC_GUILD_ALREADY_LOGIN_MEMBER;
    }

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::CanMoveServer(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* member = guild->FindMember(char_guid);
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	if (member->IsLogIn() == FALSE)
	{
		SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : Not login user : guild guid = %u, char guid = %u", 
            guild_guid, 
            char_guid 
            );
	}

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	
GuildManager::Login(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	member->SetLogin( TRUE );
	member->SetTotalInfoReqCnt( 0 );	

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT 
GuildManager::Logout(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	member->SetLogin( FALSE ); 
	member->SetTotalInfoReqCnt( 0 );	

	return RC::RC_GUILD_SUCCESS;
}

// UP/GP 변환
// NowUP : 현재 길드의 UP
// ResultUP : 변환후 UP
// ResultGP : 변환후 GP
RC::eGUILD_RESULT	
GuildManager::CanConvertUP2GP(GUILDGUID guild_guid, CHARGUID request_char_guid, GPTYPE convert_gp)
{
	Guild* guild = FindGuild( guild_guid );
	if( !guild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* member = guild->FindMember( request_char_guid );
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	// 권한이 있는지 체크한다.
	// 권한체크 .
	if (!CheckRight( guild_guid, request_char_guid, eGUILD_RIGHTS_UP2GP))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	if (convert_gp*GUILD_UP2GP_RATE > guild->GetGuildUP())	
    {
        return RC::RC_GUILD_NOT_ENOUGH_UP;
    }

	return RC::RC_GUILD_SUCCESS;
}

bool	
GuildManager::SetPoint(GUILDGUID guild_guid, GPTYPE gp, UPTYPE up)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return false;
    }

	guild->SetGuildGP( gp );
	guild->SetGuildUP( up );

	return true;
}

RC::eGUILD_RESULT	
GuildManager::CanDonateUP(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	if (!CheckRight(guild_guid, char_guid, eGUILD_RIGHTS_DONATEUP))
    {
		return RC::RC_GUILD_HAVE_NOT_RIGHT;
    }

	// 캐릭터가 소유한 UP는 게임서버에서 판별하고 온다.
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::DonateUP(GUILDGUID guild_guid, UPTYPE total_up, CHARGUID char_guid, UPTYPE donate_up)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    GuildMember* member = guild->FindMember( char_guid );
    if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

    GUILDLOG->LogUPDonation(guild, member, donate_up, total_up);

	guild->SetGuildUP(total_up);

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	
GuildManager::LevelUpMember(GUILDGUID guild_guid, CHARGUID char_guid, LEVELTYPE level)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	if (member->GetLevel() >= level)
    {
		return RC::RC_GUILD_CANNOT_LEVELUP;
    }

	member->SetLevel(level);
	if (level >= 31 && member->GetDuty() == eGUILD_DUTY_CAMP_SOLDIER)
    {
		return RC::RC_GUILD_KICK_GUILD_BY_LEVELUP;
    }

	return RC::RC_GUILD_SUCCESS;
}

bool			
GuildManager::GetWorldGuildInfo(GUILDGUID guild_guid, OUT WORLD_GUILD_INFO& info)
{
	Guild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return false;
    }

	guild->GetWorldGuildInfo(info);
	return true;
}

RC::eGUILD_RESULT	
GuildManager::CanRequestTotalInfo(GUILDGUID guild_guid, CHARGUID char_guid)
{
	Guild* guild = FindGuild( guild_guid );
	if (guild == NULL ) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	GuildMember* member = guild->FindMember( char_guid );
	if (member == NULL)	
    {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	//현재 보낸 횟수가 3회 이상이면..
	if (member->GetTotalInfoReqCnt() > 3)
	{
		SUNLOG( 
            eCRITICAL_LOG, 
            __FUNCTION__" : over count to request : guild guid = %u, char guid = %u, name = %s",
			guild_guid, 
            char_guid, 
            member->GetCharName()
            );
		return RC::RC_GUILD_OVER_TOTALINFO_REQ_CNT;
	}

	member->SetTotalInfoReqCnt(member->GetTotalInfoReqCnt() + 1);

	return RC::RC_GUILD_SUCCESS;
}

void	 
GuildManager::SetGuildNotice(GUILDGUID guild_guid, const util::StrArrayForGuildNotice& notice)
{
	Guild* guild = g_GuildManager.FindGuild(guild_guid);
	if (guild == NULL)	
    {
        return;
    }

	guild->SetGuildNotice(notice);

}

void 
GuildManager::AddGuildName(std::string name, GUILDGUID idx) 
{
	guild_name_hash_[name] = idx;
}

void 
GuildManager::RemoveGuildName(std::string guild_name) 
{
	GUILD_NAME_HASH::iterator it = guild_name_hash_.find(guild_name);
	if (it != guild_name_hash_.end()) 
    {
		guild_name_hash_.erase(it);
    }
}
	
Guild* 
GuildManager::FindGuildName(std::string guild_name) 
{
	GUILDGUID guild_index(0);
	GUILD_NAME_HASH::const_iterator it = guild_name_hash_.find(guild_name);
	if (it != guild_name_hash_.end())  
	{
		guild_index  =  it->second;
	}
	
	return FindGuild(guild_index );
}

GUILDGUID GuildManager::GetGuildGuidByGuildName(std::string guild_name)
{
	GUILDGUID guild_index(0);
	GUILD_NAME_HASH::const_iterator it = guild_name_hash_.find(guild_name);
	if (it != guild_name_hash_.end())  
	{
		guild_index  =  it->second;
	}

	return guild_index ;
}

CHARGUID GuildManager::GetGuildMasterByGuildName(std::string guild_name)
{
	GUILDGUID guild_index(0);
	GUILD_NAME_HASH::const_iterator it = guild_name_hash_.find(guild_name);
	if (it != guild_name_hash_.end())  
    {
		guild_index  =  it->second;
    }
	
	CHARGUID guild_master_index(0);
	Guild* guild = FindGuild(guild_index);
	if(guild)
	{
		GuildMember* member = guild->FindMaster();
		if(member && member->IsLogIn())
		{
			guild_master_index = member->GetCharGuid();
		}
	}
	
	return guild_master_index;	
}

RC::eGUILD_RESULT GuildManager::CanChangeGuildName( GUILDGUID guild_guid, CHARGUID request_char_guid )
{
	Guild *guild = FindGuild(guild_guid);
	if (guild == NULL)
    {
		return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	if (GuildSmallContentEntry::GuildPenalty::IsGuildDestroyWaitState(guild_guid)) 
    {
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;
    }

	if (request_char_guid != 0)
	{
        //길드 마스터만 변경 가능. (스크립트로 안하기로함)
		GuildMember* member = guild->FindMember( request_char_guid );
		if (member == NULL)
        {
			return RC::RC_GUILD_NOT_GUILD_MEMBER;
        }

		if (eGUILD_DUTY_MASTER != member->GetDuty())
        {
			return RC::RC_GUILD_NAME_CHANGE_CANNOT_GRADE;
        }
	}

	return RC::RC_GUILD_SUCCESS;
}

void GuildManager::ChangeGuildName(GUILDGUID guid, const TCHAR* guild_name)
{
	//현재 길드 서버의 길드명을 바꾼다.
	//길드 정보가 없는경우 일주일간 길드원이 한번도 접속 하지 않았다면, 길드 정보는 없다.
	//웁스를 통해 들어올 경우 길드원 접속 안할수 있다.
	Guild* guild = FindGuild(guid);
	if (guild == NULL)
    {
		return;
    }

	RemoveGuildName(guild->GetGuildName());
	guild->ChangeGuildName(guild_name);
	AddGuildName(guild->GetGuildName(), guild->GetGuildGuid());

    // 로그를 먼저 남기고 길드관계 정보를 갱신한다.
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild->GetGuildInGameLog()->WriteGuildLog(
        GuildChangeName,
        guild->GetGuildGuid(),
        guild_name
        );
#endif // _NA_003923_20120130_GUILD_RENEWAL

	// 길드 관계에 있는 길드(적대 or 동맹)가 가진 길드 관계정보를 바꾼다.
	STLX_VECTOR<GUILDGUID> vGuildGuid;
	if (guild->GetRelationGuid(vGuildGuid) == 0)
    {
		return;
    }
	STLX_VECTOR<GUILDGUID>::iterator it = vGuildGuid.begin();
	for (; it != vGuildGuid.end() ; ++it)
	{
		Guild* guild = FindGuild(*it);
		if (guild)
        {
			guild->GetRelationEntry()->ChangeGuildName(guid, guild_name);
        }
	}
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
void GuildManager::ProcessGuildCoinChange(GUILDGUID guid, CHARGUID uuid, DWORD code, INT point)
{
    if(code != CURRENCY_INFO_DONATION_GUILDCOIN)
        return;

    Guild* guild = FindGuild(guid);
    if (!guild)
        return;

    g_GuildPointSystem.GuildPointChange(guid, uuid, code, point);

    INT user_point  = g_GuildPointSystem.GetGuildPoint(guid, uuid, code);
    INT point_total = g_GuildPointSystem.GetGuildPointTotal(guid, code);

    ;{
        guild->SetGuildCoin(point_total);

        GuildMember* guild_member = guild->FindMember(uuid);
        if (guild_member) {
            guild_member->SetGuildCoin(user_point);
        }
        //else {
        //    // the member already leaved...
        //}
    };
    ;{
        MSG_ZG_GUILDPOINT_CHANGE_BRD user_msg( \
            guid, uuid, code, user_point);
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &user_msg, sizeof(user_msg));

        MSG_ZG_GUILDPOINT_CHANGE_BRD guild_msg( \
            guid, GuildPointSystem::GUILD_TOTAL_UUID, code, point_total);
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &guild_msg, sizeof(guild_msg));
    };

    //////////////////////////////////////////////////////////////////////////
    // 2. 길드의 길드원 공여도 총합(uuid:0,code:10001)에 따라 레벨이 오르거나 내린다.
    GuildRankInfoParser* const guild_rankinfo_paser = GuildRankInfoParser::Instance();
    BYTE current_level = guild->GetGuildGrade();
    BYTE changed_level = guild_rankinfo_paser->GetMatchedRankInfoByGuildCoin(point_total);
    ;{
        if ((changed_level - current_level > 1) ||
            //current_guild_level - changed_level > 1 || // 4레벨에서 1레벨로 다운가능하므로 제거
            !(changed_level >= 1 && changed_level <= guild_rankinfo_paser->GetGuildMaxGrade())) 
        {
            // something wrong...
            return;
        }
    };

    // level changed...
    if (current_level != changed_level)
    {
        BYTE previous_level = current_level;

        guild->SetGuildGrade(changed_level);
        GUILDLOG->LogRankUp(guild, current_level);

        GuildInGameLogType log_type = 
            (current_level < changed_level) ? GuildLevelUp : GuildLevelDown;
        guild->GetGuildInGameLog()->WriteGuildLog(
            log_type,
            guild->GetGuildGuid(),
            NULL,
            guild->GetGuildGrade()
            );

        // ZD level change
        MSG_ZD_GUILDPOINT_GUILD_LEVEL_CHANGE_CMD msg(guid, changed_level, previous_level);
        g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));

        MSG_GZ_GUILD_RANKUP_BRD	sendMsg;
        sendMsg.m_dwKey = 0;
        sendMsg.m_GuildGuid = guid;
        sendMsg.m_byLevel = (BYTE)changed_level;
        sendMsg.m_RemainGP = guild->GetGuildGP();
        sendMsg.m_RemainUP = guild->GetGuildUP();
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &sendMsg, sizeof(sendMsg));

        const DWORD guild_point_code = CURRENCY_INFO_GUILD_POINT;

        if (changed_level < previous_level)
        {
            // level down...
            for(BYTE iter_level = previous_level; iter_level != changed_level; --iter_level)
            {
                const GuildRankInfo* const guild_rankinfo = \
                    guild_rankinfo_paser->FindGuildRankInfo(iter_level);
                if(!guild_rankinfo || guild_rankinfo->reward_gp_by_levelup_ == 0)
                    continue;

                INT guild_point = g_GuildPointSystem.GetGuildPointTotal( \
                    guid, guild_point_code);
                if (guild_point >= guild_rankinfo->reward_gp_by_levelup_)
                {
                    g_GuildPointSystem.GuildPointChange( \
                        guid, GuildPointSystem::GUILD_TOTAL_UUID, \
                        guild_point_code, (-guild_rankinfo->reward_gp_by_levelup_));
                }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
                else    //잔여 길드 포인트보다 레벨 다운시 차감 길드 포인트가 크므로 길드 시설을 다운그레이드 시켜야한다.
                {
                    g_GuildPointSystem.GuildPointChange( \
                        guid, GuildPointSystem::GUILD_TOTAL_UUID, \
                        guild_point_code, (-guild_point));
                    INT less_guild_point = guild_rankinfo->reward_gp_by_levelup_ - guild_point;
                    if (RandomDowngradeGuildFacility(guid, less_guild_point))
                    {
                        if (less_guild_point > 0)
                        {
                            //길드 시설을 다운그레이드 하고도 차감해야할 길드 포인트가 남은 상태
                            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Guild point error  (%d, %d, %d %d)", 
                                guid, uuid, code, point);
                        }
                    }
                    else
                    {
                        //길드 시설 다운그레이드 실패
                        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Failed random downgrade guild facility (%d, %d, %d %d)", 
                            guid, uuid, code, point);
                    }
                }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            }

#ifdef _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM
            //스크립트 상의 길드 관계 권한을 체크해 권한이 없을 경우 모든 관계를 삭제한다.
            if (!GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, guild->GetGuildGrade()))
            {
                GuildRelationSystem::GuildRelationEntry* guild_relation_entry = guild->GetRelationEntry();
                if (guild_relation_entry)
                {
                    guild_relation_entry->DeleteAllRelation(guild->GetGuildGuid());
                }
            }
#endif // _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM
        }
        else if(changed_level > previous_level)
        {
            // level up...
            for(BYTE iter_level = changed_level; iter_level != previous_level; --iter_level)
            {
                const GuildRankInfo* const guild_rankinfo = \
                    guild_rankinfo_paser->FindGuildRankInfo(iter_level);
                if(!guild_rankinfo || guild_rankinfo->reward_gp_by_levelup_ == 0)
                    continue;

                ;{
                    g_GuildPointSystem.GuildPointChange( \
                        guid, GuildPointSystem::GUILD_TOTAL_UUID, \
                        guild_point_code, guild_rankinfo->reward_gp_by_levelup_);
                };
            }
        }

        INT guild_point_total = g_GuildPointSystem.GetGuildPointTotal(guid, guild_point_code);
        if (guild_point_total >= 0)
            guild->SetGuildPoint(guild_point_total);

        MSG_ZG_GUILDPOINT_CHANGE_BRD guild_msg( \
            guid, GuildPointSystem::GUILD_TOTAL_UUID, guild_point_code, guild_point_total);
        g_DBProxyServer.SendToSessionType(GAME_SERVER, &guild_msg, sizeof(guild_msg));
    };
}

void GuildManager::ProcessGuildRankingUpdate(GUILD_RANKING_INFO* guild_ranking_info)
{
    guild_ranking_data_.clear();

    //////////////////////////////////////////////////////////////////////////
    //
    GUILD_RANKING_INFO* begin = &guild_ranking_info[0];
    GUILD_RANKING_INFO* end = &guild_ranking_info[GUILD_RANKING_INFO::MAX_GUILD_RANKING_INFO];

    for(GUILD_RANKING_INFO* it = begin; it != end; ++it)
    {
        GUILDGUID guild_guid = it->guild_guid;
        if (guild_guid == 0)
            continue;

        Guild* guild = g_GuildManager.FindGuild(guild_guid);
        if(guild == NULL)
        {
            MSG_DZ_GUILD_SELECT_SYN msg;
            msg.m_dwKey = 0;
            msg.m_dwSessionIndex = 0;
            msg.m_GuildGuid = guild_guid;
            msg.m_bySelectType = 1;		//1은 로그인시 SELECT를 뜻한다.
            msg.m_LoginCharGuid	= 0;

            g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));
        }

        WORD guild_ranking = 1 + (it - begin);
        guild_ranking_data_.insert( \
            GUILD_RANKING_MAP::value_type(guild_guid, guild_ranking));
    }
}

WORD GuildManager::GetGuildRanking( GUILDGUID guid ) const
{
    GUILD_RANKING_MAP::const_iterator it = guild_ranking_data_.find(guid);
    if(it == guild_ranking_data_.end())
        return 0;

    return it->second;
}

bool GuildManager::GetGuildInfoExtra( GUILDGUID guid, GUILD_INFO_EXTRA& guild_info_extra )
{
    Guild* guild = g_GuildManager.FindGuild(guid);
    if(guild == NULL)
        return false;

    guild_info_extra.ranking_ = GetGuildRanking(guid);
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    strncpy(guild_info_extra.introduction_strings_, \
        guild->GetGuildIntroduction(), \
        MAX_GUILD_INTRODUCTION_LENGTH);
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

    return true;
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
void GuildManager::SetGuildIntroduction( GUILDGUID guid, CHAR* introduction )
{
    Guild* guild = g_GuildManager.FindGuild(guid);
    if(guild == NULL)
        return;

    guild->SetGuildIntroduction(introduction);
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const void GuildManager::UpdateGuildJoinRequest()
{
    GUILD_HASH::iterator cur_iter = guild_hash_.begin();
    GUILD_HASH::iterator end_iter = guild_hash_.end();

    time_t current_time = 0;
    util::TimeSync::time(&current_time);

    for (; cur_iter != end_iter; ++cur_iter)
    {
        Guild* current_guild = *cur_iter;
        if (current_guild == NULL)
        {
            continue;
        }

        current_guild->_UpdateGuildJoinRequest(current_time);
    }
}

const void GuildManager::GetGuildJoinRequestTotalInfo( const GUILDGUID guild_guid, 
                                                       OUT GUILD_JOIN_REQUEST_TOTAL_INFO& guild_join_request_total_info )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return;
    }

    guild->_GetGuildJoinRequestTotalInfo(guild_join_request_total_info);
}

const RC::eGUILD_RESULT GuildManager::CanGuildJoinRequest( const CHARGUID char_guid, const GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD;
    }

    RC::eGUILD_RESULT result = guild->_CanGuildJoinRequest(char_guid);
    if (result == RC::RC_GUILD_SUCCESS)
    {
        int count = guild_join_request_char_list_.count(char_guid);
        if (count >= MAX_GUILD_JOIN_REQUEST_PLAYER_NUM)
        {
            return RC::RC_GUILD_JOIN_REQUEST_PLAYER_MAX;
        }
    }

    return result;
}

const RC::eGUILD_RESULT GuildManager::InsertGuildJoinRequest( GUILD_JOIN_REQUEST_INFO* guild_join_request_info )
{
    GUILDGUID guild_guid = guild_join_request_info->m_guild_guid;
    CHARGUID char_guid = guild_join_request_info->m_char_guid;

    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD;
    }

    RC::eGUILD_RESULT result = guild->_InsertGuildJoinRequest(guild_join_request_info);
    if (result == RC::RC_GUILD_SUCCESS)
    {
        guild_join_request_char_list_.insert(std::make_pair<CHARGUID, GUILDGUID>(char_guid, guild_guid));
    }
    return result;
}

const RC::eGUILD_RESULT GuildManager::DeleteGuildJoinRequest( const CHARGUID char_guid, const GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return RC::RC_GUILD_JOIN_REQUEST_INVALID_GUILD;
    }

    RC::eGUILD_RESULT result = guild->_DeleteGuildJoinRequest(char_guid);
    if (result == RC::RC_GUILD_SUCCESS)
    {
        GUILD_JOIN_REQUEST_CHAR_MAP::_Pairii ranges = guild_join_request_char_list_.equal_range(char_guid);

        GUILD_JOIN_REQUEST_CHAR_MAP::iterator cur_iter = ranges.first;
        GUILD_JOIN_REQUEST_CHAR_MAP::iterator end_iter = ranges.second;

        GUILD_JOIN_REQUEST_INFO guild_join_request_info;
        guild_join_request_info.m_char_guid = char_guid;

        for (; cur_iter != end_iter; ++cur_iter)
        {
            if (guild_guid == cur_iter->second)
            {
                guild_join_request_char_list_.erase(cur_iter);
                break;
            }
        }
    }
    return result;
}

const void GuildManager::InitailizeGuildJoinRequest( const CHARGUID char_guid )
{
    GUILD_JOIN_REQUEST_CHAR_MAP::iterator iter = guild_join_request_char_list_.find(char_guid);
    if (iter == guild_join_request_char_list_.end())
    {
        return;
    }

    GUILD_JOIN_REQUEST_CHAR_MAP::_Pairii ranges = guild_join_request_char_list_.equal_range(char_guid);

    GUILD_JOIN_REQUEST_CHAR_MAP::iterator cur_iter = ranges.first;
    GUILD_JOIN_REQUEST_CHAR_MAP::iterator end_iter = ranges.second;

    GUILD_JOIN_REQUEST_INFO guild_join_request_info;
    guild_join_request_info.m_char_guid = char_guid;

    for (; cur_iter != end_iter; ++cur_iter)
    {
        GUILDGUID guild_guid = cur_iter->second;
        Guild* guild = FindGuild(guild_guid);
        if (guild == NULL)
        {
            continue;
        }

        if (guild->_CanGuildJoinRequest(char_guid) == RC::RC_GUILD_JOIN_REQUEST_ALREADY_EXIST)
        {
            MSG_GZ_GUILD_JOIN_REQUEST_REJECT_SYN msg;
            msg.reject_type = eGUILD_JOIN_REJECT_TYPE_FAILED;
            msg.guild_guid = guild_guid;
            msg.target_guid = char_guid;
            g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));
        }
    }
}

const void GuildManager::UpdateGuildJoinRequestCharacter( const CHARGUID char_guid, LEVELTYPE char_level, const TCHAR* char_name /* = NULL */ )
{
    GUILD_JOIN_REQUEST_CHAR_MAP::iterator iter = guild_join_request_char_list_.find(char_guid);
    if (iter == guild_join_request_char_list_.end())
    {
        return;
    }

    GUILD_JOIN_REQUEST_CHAR_MAP::_Pairii ranges = guild_join_request_char_list_.equal_range(char_guid);

    GUILD_JOIN_REQUEST_CHAR_MAP::iterator cur_iter = ranges.first;
    GUILD_JOIN_REQUEST_CHAR_MAP::iterator end_iter = ranges.second;

    GUILD_JOIN_REQUEST_INFO guild_join_request_info;
    guild_join_request_info.m_char_guid = char_guid;
    guild_join_request_info.m_char_level = char_level;

    //char_name이 NULL일 경우는 이름이 변경 되지 않은 경우이다.
    if (char_name != NULL)
    {
        _tcsncpy(guild_join_request_info.m_char_name, char_name, MAX_CHARNAME_LENGTH);
    }

    for (; cur_iter != end_iter; ++cur_iter)
    {
        GUILDGUID guild_guid = cur_iter->second;
        Guild* guild = FindGuild(guild_guid);
        if (guild == NULL)
        {
            continue;
        }
        GUILD_JOIN_REQUEST_INFO prev_info;
        if (guild->_GetGuildJoinRequest(char_guid, prev_info) == RC::RC_GUILD_SUCCESS)
        {
            guild_join_request_info.m_guild_guid = guild_guid;
            guild_join_request_info.m_char_class = prev_info.m_char_class;
            guild_join_request_info.m_request_time = prev_info.m_request_time;
            guild_join_request_info.m_remain_time = prev_info.m_remain_time;

            DeleteGuildJoinRequest(char_guid, guild_guid);
            InsertGuildJoinRequest(&guild_join_request_info);

            MSG_GZ_GUILD_JOIN_REQUEST_ACK msg;
            msg.request_info = guild_join_request_info;
            msg.result_ = RC::RC_GUILD_SUCCESS;
            g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));
        }
    }
}

GUILDGUID GuildManager::FindCharGuildGuid( CHARGUID char_guid )
{
    GUILD_HASH::iterator cur_iter = guild_hash_.begin();
    GUILD_HASH::iterator end_iter = guild_hash_.end();

    time_t current_time = 0;
    util::TimeSync::time(&current_time);

    for (; cur_iter != end_iter; ++cur_iter)
    {
        Guild* current_guild = *cur_iter;
        if (current_guild == NULL)
        {
            continue;
        }

        GuildMember* guild_member = current_guild->FindMember(char_guid);
        if (guild_member == NULL)
        {
            continue;
        }

        return current_guild->GetGuildGuid();
    }
    return 0;
}

const void GuildManager::GetGuildFacilityTotalInfo( const GUILDGUID guild_guid, OUT GUILD_FACILITY_TOTAL_INFO& total )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return;
    }

    GuildFacilities*const facilities = guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        return;
    }

    facilities->GetGuildFacilityTotalInfo(total);
}

const bool GuildManager::ProcessGuildPointConsumption( INT consumption_point, GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return false;
    }

    const DWORD guild_point_code = CURRENCY_INFO_GUILD_POINT;

    INT guild_point = g_GuildPointSystem.GetGuildPointTotal(guild_guid, guild_point_code);

    if (guild_point >= consumption_point)
    {
        g_GuildPointSystem.GuildPointChange(guild_guid, GuildPointSystem::GUILD_TOTAL_UUID,
                                            guild_point_code, (-consumption_point));
    }
    else
    {
        return false;
    }

    INT guild_point_total = g_GuildPointSystem.GetGuildPointTotal(guild_guid, guild_point_code);
    if (guild_point_total >= 0)
    {
        guild->SetGuildPoint(guild_point_total);
    }

    MSG_ZG_GUILDPOINT_CHANGE_BRD guild_msg(guild_guid, GuildPointSystem::GUILD_TOTAL_UUID, guild_point_code, guild_point_total);
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &guild_msg, sizeof(guild_msg));

    return true;
}

const RC::eGUILD_RESULT GuildManager::UpdateGuildFacility( GUILD_FACILITY_INFO info, FLEVEL add_level, GUILDGUID guild_guid, CHARGUID char_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] not found guild (guild %d)", guild_guid);
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    GuildFacilities*const facilities = guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] not found guild facilities (guild %d)", guild_guid);
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    if (add_level > 0)  //시설 업그레이드 일 경우에만 
    {
        const FACILITY_INFO* next_info = GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(info.code, info.level);
        if (next_info == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] not found facility info (code %d)", info.code);
            return RC::RC_GUILD_NOT_EXIST_FACILITY;
        }
        switch(next_info->upgrade_type)
        {
        case FUI_ONLY_GUILDPOINT: // 길드포인트로 업그레이드
        case FUI_GUILDPOINTANDITEM: // 둘다 필요
            ProcessGuildPointConsumption(next_info->upgrade_guild_point, guild_guid);
            break;
        case FUI_ONLY_ITEM: // 아이템으로 업그레이드 // GameServer에서 처리된다
            break;
        default: // 
            {
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Invalid facility upgrade type(%d)",next_info->upgrade_type);
                return RC::RC_GUILD_NOT_EXIST_FACILITY;
            }
            break;
        }
    }

	//길드 시설 업/다운 그레이드 로그
    GUILDLOG->LogGuildFacilityUpgrade(guild, guild->FindMember(char_guid),info.code, info.level, (add_level > 0));
    
    facilities->SetGuildFacility(info);
    return RC::RC_GUILD_SUCCESS;
}

const bool GuildManager::RandomDowngradeGuildFacility( GUILDGUID guild_guid,  IN OUT INT& guild_point )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return false;
    }

    GuildFacilities*const facilities = guild->GetGuildFacilities();
    const GUILD_FACILITY_MAP* facility_map = facilities->GetGuildFacilityMap();

    if (facility_map->size() <= 0)
    {
        return false;
    }

    GuildFacilityInfoParser* parser = GuildFacilityInfoParser::Instance();
    GUILD_FACILITY_MAP::const_iterator cur_iter = facility_map->begin();
    GUILD_FACILITY_MAP::const_iterator end_iter = facility_map->end();
    GUILD_FACILITY_MAP dummy_map;
    for (; cur_iter != end_iter; ++cur_iter)    // 1레벨 이상의 시설을 더미 맵에 복사한다
    {
        if (cur_iter->second.level <= 0)
        {
            continue;
        }
        dummy_map.insert(std::make_pair(cur_iter->second.code, cur_iter->second));
    }

    for (int i = 0; i < guild_point; ++i)
    {
        if (dummy_map.size() <= 0)
        {
            return false;
        }

        GUILD_FACILITY_MAP::iterator dummy_cur_iter = dummy_map.begin();

        int random_facility = random(1, dummy_map.size());  //임의의 시설을 정한다
        for (int iter_count = 1; iter_count < random_facility; ++iter_count)
        {
            ++dummy_cur_iter;
        }

        FCODE fcode = dummy_cur_iter->second.code;
        FLEVEL flevel = dummy_cur_iter->second.level;
        const FACILITY_INFO* facility_info = parser->GetGuildFacilityInfo(fcode, flevel);
        if (facility_info != NULL)
        {
            guild_point -= facility_info->upgrade_guild_point;
            MSG_DZ_GUILD_FACILITY_UPDATE_SYN msg;
            msg.char_guid = 0;
            msg.add_level = -1;
            msg.current_level = flevel;
            msg.facility_code = fcode;
            msg.guild_guid = guild_guid;
            g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg)); //삭제 패킷 전송
        }
        dummy_map.erase(dummy_cur_iter);
    }

    return true;
}

const RC::eGUILD_RESULT GuildManager::CanUpdateGuildFacility( FCODE code, FLEVEL add_level, GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

    GuildFacilities*const facilities= guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY;
    }

    FLEVEL next_level = add_level;
    const GUILD_FACILITY_INFO* cur_facility = facilities->GetGuildFacility(code);
    if (cur_facility != NULL)
    {
        next_level += cur_facility->level;
    }

    GuildFacilityInfoParser* parser = GuildFacilityInfoParser::Instance();
    const FACILITY_INFO* next_info = parser->GetGuildFacilityInfo(code, next_level);
    if (next_info == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY;
    }

    switch(next_info->upgrade_type)
    {
    case FUI_ONLY_GUILDPOINT: // 길드포인트로 업그레이드
    case FUI_GUILDPOINTANDITEM: // 둘다 필요
        {
            INT guild_point = g_GuildPointSystem.GetGuildPointTotal(guild_guid, CURRENCY_INFO_GUILD_POINT);
            if (guild_point < next_info->upgrade_guild_point)
            {
                return RC::RC_GUILD_NOT_ENOUGH_GP;
            }
        }
        break;
    case FUI_ONLY_ITEM: // 아이템으로 업그레이드
        break;
    default: // 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Invalid facility upgrade type(%d)",next_info->upgrade_type);
            return RC::RC_GUILD_NOT_EXIST_FACILITY;
        }
        break;
    }

    return RC::RC_GUILD_SUCCESS;
}

const GUILD_FACILITY_INFO* GuildManager::GetGuildFacility( FCODE code, GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return NULL;
    }

    GuildFacilities*const facilities= guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        return NULL;
    }

    return facilities->GetGuildFacility(code);
}

const FLEVEL GuildManager::GetGuildFacilityLevel( FCODE code, GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return NULL;
    }

    GuildFacilities*const facilities= guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        return NULL;
    }

    const GUILD_FACILITY_INFO* info = facilities->GetGuildFacility(code);
    if (info == NULL)
    {
        return NULL;
    }
    
    return info->level;
}

const void GuildManager::SetGuildFacility( GUILD_FACILITY_INFO info, GUILDGUID guild_guid )
{
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return;
    }

    GuildFacilities*const facilities= guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        return;
    }

    facilities->SetGuildFacility(info);
}

const bool GuildManager::InitializeGuildPointAndFacility( GUILDGUID guild_guid )
{
    //길드 시설을 초기화 한다.
    Guild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        return false;
    }

    GuildFacilities*const facilities = guild->GetGuildFacilities();
    const GUILD_FACILITY_MAP* facility_map = facilities->GetGuildFacilityMap();
    if (facility_map->size() <= 0)
    {
        return false;
    }

    GUILD_FACILITY_MAP::const_iterator cur_iter = facility_map->begin();
    GUILD_FACILITY_MAP::const_iterator end_iter = facility_map->end();

    GuildFacilityInfoParser* parser = GuildFacilityInfoParser::Instance();

    for (; cur_iter != end_iter; ++cur_iter)
    {
        FCODE fcode = cur_iter->second.code;
        FLEVEL flevel = cur_iter->second.level;
        if (flevel >= 1)
        {
            MSG_DZ_GUILD_FACILITY_UPDATE_SYN msg;
            msg.char_guid = 0;
            msg.add_level = -flevel;
            msg.current_level = flevel;
            msg.facility_code = fcode;
            msg.guild_guid = guild_guid;
            g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));
        }
    }
    
    //길드 포인트를 초기화한다.
    GuildRankInfoParser* const guild_rankinfo_paser = GuildRankInfoParser::Instance();
    INT point_total = g_GuildPointSystem.GetGuildPointTotal(guild_guid, CURRENCY_INFO_DONATION_GUILDCOIN);
    BYTE guild_level = guild_rankinfo_paser->GetMatchedRankInfoByGuildCoin(point_total);

    GPTYPE new_point_total = 0;
    for(BYTE iter_level = 1; iter_level <= guild_level; ++iter_level)
    {
        const GuildRankInfo* const guild_rankinfo = \
            guild_rankinfo_paser->FindGuildRankInfo(iter_level);
        if(!guild_rankinfo || guild_rankinfo->reward_gp_by_levelup_ == 0)
            continue;

        new_point_total += guild_rankinfo->reward_gp_by_levelup_;
    }

    g_GuildPointSystem.SetGuildPoint(guild_guid, GuildPointSystem::GUILD_TOTAL_UUID, \
        CURRENCY_INFO_GUILD_POINT, new_point_total);

    MSG_ZG_GUILDPOINT_CHANGE_BRD guild_msg( \
        guild_guid, GuildPointSystem::GUILD_TOTAL_UUID, CURRENCY_INFO_GUILD_POINT, new_point_total);
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &guild_msg, sizeof(guild_msg));
    return true;
}

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID GuildManager::InitializeAllSonnenScheinPhase()
{
    GUILD_HASH::iterator itr = guild_hash_.begin();
    for (itr; itr != guild_hash_.end(); ++itr)
    {
        Guild* guild = *itr;
        guild->SetSonnenScheinPhase(0);
    }
    SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Initialize All SonnenSchein Phase");
}

BOOL GuildManager::SetGuildSonnenScheinPhase( GUILDGUID guid, BYTE phase )
{
    Guild* guild = FindGuild(guid);
    if (guild == NULL) {
        return FALSE;
    }
    guild->SetSonnenScheinPhase(phase);

    return TRUE;
}

VOID GuildManager::_UpdateSonnenSchein()
{
    if (event_loaded_ == FALSE) {
        return;
    }

    // 시간에 따른 SonnenSchein상태 전환 처리---------------------------------------------
    DWORD cur_time = 0; //현재시간
    const WORD day_of_week = util::GetDateTime_Week_HHMMSS(cur_time);

    //요일체크
    if (day_of_week != sschein_event_info_.m_wDayOfWeek) {
        return;
    }

    //시간체크
    // 다음 소넨샤인이 시작될 날짜와(next_sonnenschein_date_) 현재 날짜(cur_time)이 같다면
    // 등급초기화 진행한다.
    // 시간에 대한 검사는 Between()함수쪽에서 처리하게 된다.
    // 등급초기화 이후에, 그 다음 소넨샤인이 시작될 날짜를 갱신한다.
    DWORD battle_start_time = sschein_event_info_.m_dwTime[DOMINATION_EVENT_START];
    DWORD battle_end_time = sschein_event_info_.m_dwTime[DOMINATION_EVENT_END];

    if (Between(cur_time, battle_start_time, battle_end_time) == TRUE) 
    {
        //날짜체크
        CTime cur_time = util::TimeSync::_time64(NULL);
        if (next_sonnenschein_date_.GetYear() == cur_time.GetYear() &&
            next_sonnenschein_date_.GetMonth() == cur_time.GetMonth() &&
            next_sonnenschein_date_.GetDay() == cur_time.GetDay())
        {
            //SONNENSCHEIN_EVENT_STATE_START
            MSG_DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN msg;
            g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));

            _CheckLastSonnenScheinDate();
        }

        
    }
}

VOID GuildManager::_InitSonnenScheinInfo()
{
    //몬트샤인 eventinfo를 저장해둔다
    //sonnenschein은 몬트샤인과 동시간대에 진행되기 때문에 같은 event time을 사용한다
    DominationInfoParser* domi_parser = DominationInfoParser::Instance();
    sschein_event_info_ = domi_parser->GetMondScheinEventInfo();
    if (sschein_event_info_.m_wMapCode != 0)  {
        event_loaded_ = TRUE;
    }

    _CheckLastSonnenScheinDate();
}

VOID GuildManager::OnScriptLoaded()
{
    _InitSonnenScheinInfo();
}

VOID GuildManager::_CheckLastSonnenScheinDate()
{
    //다음에 진행될 소넨샤인 시작 날짜를 구한다
    DWORD battle_start_time = sschein_event_info_.m_dwTime[DOMINATION_EVENT_START];
    INT hour = battle_start_time/10000;
    INT minute = (battle_start_time%10000) / 100;
    INT second = battle_start_time % 100;
    INT sschein_week = sschein_event_info_.m_wDayOfWeek +1;

    CTime last_time = util::TimeSync::_time64(NULL);
    INT cur_day_of_week = last_time.GetDayOfWeek();

    //요일계산->'몇일 후' 소넨샤인이 시작되는지 계산
    INT next_sschein_week = sschein_week - cur_day_of_week;
    if (next_sschein_week < 0) {
        next_sschein_week += 7;
    }
    else if (next_sschein_week == 0)
    {
        if (last_time.GetHour() >= hour &&
            last_time.GetMinute() >= minute)
        {
            next_sschein_week += 7;
        }
    }

    last_time += CTimeSpan(next_sschein_week, 0, 0, 0);
    next_sonnenschein_date_ = CTime(last_time.GetYear(), last_time.GetMonth(), last_time.GetDay(),
        hour, minute, second);
}

#endif //_NA_008334_20150608_SONNENSCHEIN
