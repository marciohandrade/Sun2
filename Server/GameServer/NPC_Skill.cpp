#include "stdafx.h"
#include "NPC.h"
#include "Battle.h"
#include "SkillInfoParser.h"
#include "Skill.h"
#include "Player.h"
#include "SkillManager.h"

VOID NPC::AddDefaultSkill()
{
	m_SkillSelector.Release();

	// 사용가능 스킬 추가
	SkillScriptInfo *pBaseSkillInfo = NULL;
	for( int i = 0; i < BASE_NPCINFO::MAX_SKILL_NUM; ++i )
	{
		if( !m_pBaseInfo->m_wSkillCode[i] )		continue;

		pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( m_pBaseInfo->m_wSkillCode[i] );
		if( !pBaseSkillInfo )	continue;

#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
        // 쉐도우의 '소울컨트롤'을 위한 예외처리
        // '소환체'가 가져선 안되는 스킬 예외처리
        if (IsEqualObjectKind(SUMMON_OBJECT) == true) 
        {
            if (pBaseSkillInfo->GetAbilityInfo(eABILITY_SUMMON) != NULL) {
                continue;
            }
            else if(pBaseSkillInfo->GetAbilityInfo(eABILITY_ABNORMAL_STATUS) &&
                pBaseSkillInfo->GetAbilityInfo(eABILITY_ABNORMAL_STATUS)->m_wStateID == eCHAR_STATE_CHAOS) {
                    continue;
            }

        }
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL

		if (!m_SkillSelector.Add( m_pBaseInfo->m_wSkillCode[i],	m_pBaseInfo->m_bySkillRate[i]))
        {
            SUNLOG(eCRITICAL_LOG, "스킬의 중복 추가 시도 npc[%s] skill_code[%d]",
                GetCharName() , m_pBaseInfo->m_wSkillCode[i] );
        }

		// 쿨타임 추가
		SetSkillCoolTime( m_pBaseInfo->m_wSkillCode[i], 0 );
	}

	// 특수 스킬들에 대해서도 쿨타임을 추가한다.
	SetSkillCoolTime( m_pBaseInfo->m_wReviveCode, 0 );
	SetSkillCoolTime( m_pBaseInfo->m_wHealCode, 0 );
	SetSkillCoolTime( m_pBaseInfo->m_wSummonCode, 0 );
}

VOID NPC::ResetSelectedSkill()
{ 
	m_SelectedSkillCode = SKILLCODE_INVALID; 
}

VOID NPC::SelectSkill( Character *pTarget, SLOTCODE SkillCode
#ifdef _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
                      , WORD SelectedSkillDelay 
#endif // _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
                      )
{
	SetTargetChar( pTarget );
	m_SelectedSkillCode = SkillCode;
#ifdef _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
    m_wSelectedSkillDelay = SelectedSkillDelay;
#else
    m_wSelectedSkillDelay = 0;
#endif // _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
}

VOID NPC::SelectSkill()
{
	// 일단 기본공격을 셋팅한다.
	m_SelectedSkillCode = SKILLCODE_NORMAL_ATTACK;
	m_wSelectedSkillDelay = 0;

	// 특수스킬은 일정 주기마다 스킬발동 조건을 체크한다.(부활, 힐, 소환)
	if( !m_pSkillSelectTimer->IsExpired() )		return;

	BASE_NPCINFO *pBaseInfo = GetBaseInfo();
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    WORD wSkillUpdateTime=0;
    if ((eSKILL_UPDATE_CONDITION)pBaseInfo->skill_update_condition ==
        eSKILL_UPDATE_CONDITION_FIXED)
    {
        wSkillUpdateTime = pBaseInfo->m_wSkillUpdateTime;
    }
    else {
        wSkillUpdateTime = (WORD)dRandom( pBaseInfo->m_wSkillUpdateTime, pBaseInfo->m_wSkillUpdateTime*2 );
    }
#else
    WORD wSkillUpdateTime = (WORD)dRandom( pBaseInfo->m_wSkillUpdateTime, pBaseInfo->m_wSkillUpdateTime*2 );
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
	m_pSkillSelectTimer->SetTimer( wSkillUpdateTime );

	// 부활 조건을 만족하는지 체크
	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( pBaseInfo->m_wReviveCode );
	if( pBaseSkillInfo && pBaseInfo->m_byReviveCondition && DrawLots( pBaseInfo->m_byReviveRate ) )
	{
		if( CheckCondition( pBaseInfo->m_byReviveCondition, pBaseInfo->m_wReviveConditionParam ) )
		{
			// 주변에 시체가 있으면 그 시체로 타겟을 바꾸고 해당 부활 스킬 선택
			Character *pCorpse = SelectSkillTarget( pBaseInfo->m_wReviveCode );
			if( pCorpse )
			{
				SetTargetChar( pCorpse );
				m_SelectedSkillCode = pBaseInfo->m_wReviveCode;
				m_wSelectedSkillDelay = pBaseInfo->m_wReviveDelay;
				return;
			}
		}
	}

	// 힐 조건을 만족하는지 체크
	pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( pBaseInfo->m_wHealCode );
	if( pBaseSkillInfo && pBaseInfo->m_byHealCondition && DrawLots( pBaseInfo->m_byHealRate ) )
	{
		Character *pWeakestChar = SelectSkillTarget( pBaseInfo->m_wHealCode, TRUE );
		if( pWeakestChar )
		{
			if( pWeakestChar->CheckCondition( pBaseInfo->m_byHealCondition, pBaseInfo->m_wHealConditionParam ) )
			{
				SetTargetChar( pWeakestChar );
				m_SelectedSkillCode = pBaseInfo->m_wHealCode;
				m_wSelectedSkillDelay = pBaseInfo->m_wHealDelay;
				return;
			}
		}
	}

	// 소환 조건을 만족하는지 체크
	pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( pBaseInfo->m_wSummonCode );
	if( pBaseSkillInfo && pBaseInfo->m_bySummonCondition && !m_dwSummonerKey && DrawLots( pBaseInfo->m_bySummonRate ) )
	{
		if( CheckCondition( pBaseInfo->m_bySummonCondition, pBaseInfo->m_wSummonConditionParam ) )
		{
			m_SelectedSkillCode = pBaseInfo->m_wSummonCode;
			m_wSelectedSkillDelay = pBaseInfo->m_wSummonDelay;
			return;
		}
	}

	// 스킬 사용 확률에 따라 "일반 공격" 또는 "스킬 공격" 선택
	if( m_SkillSelector.GetTotalRatio() && DrawLots( pBaseInfo->m_bySkillUsePossible ) )
	{
        DWORD ratio = rand() % m_SkillSelector.GetTotalRatio();

		SLOTCODE skillCode = SKILLCODE_INVALID;
		m_SkillSelector.Select( ratio, skillCode );
		Character *pSkillTarget = SelectSkillTarget( skillCode );
		if( !pSkillTarget )		return;

		SetTargetChar( pSkillTarget );
		m_SelectedSkillCode = skillCode;
		m_wSelectedSkillDelay = 1000;

		for( int i = 0; i < BASE_NPCINFO::MAX_SKILL_NUM; ++i )
		{
			if( pBaseInfo->m_wSkillCode[i] == skillCode )	
			{
				m_wSelectedSkillDelay = pBaseInfo->m_wSkillDelay[i];
				break;
			}
		}
	}
}

BOOL NPC::UseSkill( SLOTCODE SkillCode )
{
	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );
	if( !pBaseSkillInfo )	return FALSE;

	WzVector vAttackerPos;	GetPos(&vAttackerPos);
	
	SkillInfo skill_info_data(this, SkillCode, 0);
    skill_info_data.SetArgsObjectInfo(0, &vAttackerPos, &vAttackerPos, NULL);
    skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
                                    skill_info_data.kDefaultSkillEffect,
                                    GetSelectedSkillDelay(),
                                    skill_info_data.kDefaultSkillFactor,
                                    skill_info_data.kDefaultPropensity,
                                    skill_info_data.kDefaultEvent);

	if( GetTargetChar() )
	{
		skill_info_data.main_target_key_	= GetTargetChar()->GetObjectKey();
		GetTargetChar()->GetPos( &skill_info_data.main_target_pos_ );

		// 공격자가 이동하는 스킬인 경우
		if( pBaseSkillInfo->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY )
		{
			// 밀리기 처리를 해준다.
			WzVector wvDiff = skill_info_data.main_target_pos_ - vAttackerPos;
			ExecuteThrust( FALSE, &wvDiff, skill_info_data.dest_pos_, 0, FALSE );
		}
	}
	else
	{   //__NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__
		skill_info_data.main_target_pos_ = vAttackerPos;
	}

	// 스킬결과를 출력해줘야하는 GM 모드라면
	if(GetTargetChar() && GetTargetChar()->IsEqualObjectKind(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)GetTargetChar();
		if( eDEBUGINFO_TYPE_MONSTERSKILL == pPlayer->GetDebugInfoFlag() )
		{
			MSG_CG_ETC_DEBUGINFO_CMD sendMsg;
			sendMsg.m_dwObjectKey = GetObjectKey();
			int written = _snprintf( sendMsg.m_pszChatMsg, MSG_CG_ETC_DEBUGINFO_CMD::_MAX_CHATMSG_SIZE, "%s 사용!!", pBaseSkillInfo->m_szSkillName );

			if( written > 0 )
			{
				sendMsg.m_byMsgLength = written;
				sendMsg.m_pszChatMsg[written] = '\0';
				pPlayer->SendPacket( &sendMsg, sendMsg.GetSize() );
			}
		}
	}

	return GetSkillManager()->RegisterSkill( SKILL_TYPE_ACTIVE, &skill_info_data );
}


RC::eSKILL_RESULT NPC::CanUseSkill( SLOTCODE SkillCode, BOOL bCoolTimerReset )
{
	RC::eSKILL_RESULT rcResult = Character::CanUseSkill( SkillCode, bCoolTimerReset );
	if( rcResult != RC::RC_SKILL_SUCCESS )	return rcResult;

	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );

	// 스킬을 사용할 수 있는 캐릭터인지 체크한다.
	if( !CheckClassDefine( SkillCode, TRUE ) )
	{
		return RC::RC_SKILL_CHAR_CLASS_LIMIT;
	}

	// 쿨타임 처리
	if( bCoolTimerReset && 
		FALSE == CoolTimerReset(SkillCode) )
		return RC::RC_SKILL_COOLTIME_ERROR;

	return RC::RC_SKILL_SUCCESS;
}

BOOL NPC::UseResurrectionSkill()
{
	if( GetSelectedSkill() == SKILLCODE_INVALID || GetSelectedSkill() == SKILLCODE_NORMAL_ATTACK )
		return FALSE;

	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( GetSelectedSkill() );
	if( !pBaseSkillInfo )
		return FALSE;

	if( pBaseSkillInfo->m_byTarget == SKILL_TARGET_FRIEND_CORPSE )
		return TRUE;

	return FALSE;
}