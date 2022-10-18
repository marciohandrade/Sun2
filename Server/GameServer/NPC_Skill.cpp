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

	// ��밡�� ��ų �߰�
	SkillScriptInfo *pBaseSkillInfo = NULL;
	for( int i = 0; i < BASE_NPCINFO::MAX_SKILL_NUM; ++i )
	{
		if( !m_pBaseInfo->m_wSkillCode[i] )		continue;

		pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( m_pBaseInfo->m_wSkillCode[i] );
		if( !pBaseSkillInfo )	continue;

#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
        // �������� '�ҿ���Ʈ��'�� ���� ����ó��
        // '��ȯü'�� ������ �ȵǴ� ��ų ����ó��
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
            SUNLOG(eCRITICAL_LOG, "��ų�� �ߺ� �߰� �õ� npc[%s] skill_code[%d]",
                GetCharName() , m_pBaseInfo->m_wSkillCode[i] );
        }

		// ��Ÿ�� �߰�
		SetSkillCoolTime( m_pBaseInfo->m_wSkillCode[i], 0 );
	}

	// Ư�� ��ų�鿡 ���ؼ��� ��Ÿ���� �߰��Ѵ�.
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
	// �ϴ� �⺻������ �����Ѵ�.
	m_SelectedSkillCode = SKILLCODE_NORMAL_ATTACK;
	m_wSelectedSkillDelay = 0;

	// Ư����ų�� ���� �ֱ⸶�� ��ų�ߵ� ������ üũ�Ѵ�.(��Ȱ, ��, ��ȯ)
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

	// ��Ȱ ������ �����ϴ��� üũ
	SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( pBaseInfo->m_wReviveCode );
	if( pBaseSkillInfo && pBaseInfo->m_byReviveCondition && DrawLots( pBaseInfo->m_byReviveRate ) )
	{
		if( CheckCondition( pBaseInfo->m_byReviveCondition, pBaseInfo->m_wReviveConditionParam ) )
		{
			// �ֺ��� ��ü�� ������ �� ��ü�� Ÿ���� �ٲٰ� �ش� ��Ȱ ��ų ����
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

	// �� ������ �����ϴ��� üũ
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

	// ��ȯ ������ �����ϴ��� üũ
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

	// ��ų ��� Ȯ���� ���� "�Ϲ� ����" �Ǵ� "��ų ����" ����
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

		// �����ڰ� �̵��ϴ� ��ų�� ���
		if( pBaseSkillInfo->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY )
		{
			// �и��� ó���� ���ش�.
			WzVector wvDiff = skill_info_data.main_target_pos_ - vAttackerPos;
			ExecuteThrust( FALSE, &wvDiff, skill_info_data.dest_pos_, 0, FALSE );
		}
	}
	else
	{   //__NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__
		skill_info_data.main_target_pos_ = vAttackerPos;
	}

	// ��ų����� ���������ϴ� GM �����
	if(GetTargetChar() && GetTargetChar()->IsEqualObjectKind(PLAYER_OBJECT))
	{
		Player* pPlayer = (Player*)GetTargetChar();
		if( eDEBUGINFO_TYPE_MONSTERSKILL == pPlayer->GetDebugInfoFlag() )
		{
			MSG_CG_ETC_DEBUGINFO_CMD sendMsg;
			sendMsg.m_dwObjectKey = GetObjectKey();
			int written = _snprintf( sendMsg.m_pszChatMsg, MSG_CG_ETC_DEBUGINFO_CMD::_MAX_CHATMSG_SIZE, "%s ���!!", pBaseSkillInfo->m_szSkillName );

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

	// ��ų�� ����� �� �ִ� ĳ�������� üũ�Ѵ�.
	if( !CheckClassDefine( SkillCode, TRUE ) )
	{
		return RC::RC_SKILL_CHAR_CLASS_LIMIT;
	}

	// ��Ÿ�� ó��
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