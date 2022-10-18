//------------------------------------------------------------------------------
//  SkillStateDefenceMotion.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateDefenceMotion.h"
#include "Character.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"

//------------------------------------------------------------------------------
/**
*/
SkillStateDefenceMotion::SkillStateDefenceMotion()
{

}

//------------------------------------------------------------------------------
/**
*/
SkillStateDefenceMotion::~SkillStateDefenceMotion()
{
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateDefenceMotion::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pChr->GetCurrentAction()->SKILL.dwSkillID);
	assert(pInfo);

	if(pInfo)
	{
		BASE_ABILITYINFO *pAInfo = pInfo->GetAbilityInfoByIndex(1);
		assert(pAInfo);

		if(pAInfo)
		{
			m_dwlifeTime=pAInfo->m_iParam[2];//세번째 파라미터가 유지 시간이다.
		}
		else
		{
			m_dwlifeTime=0;
		}
	}

	m_dwCurTime=0;
	return StateSkill::OnStart(pOwner,CurTime);
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateDefenceMotion::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;

    EnterProcess(pOwner,CurTime,dwTick);

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	if( GetAnimationRate() == 100.0f )
		SetAnimationRate(50.0f);
#endif // _NA_008472_20150903_IMPROVED_BALANCE

	// 타격점체크
	CheckEvent();

	m_dwCurTime+=dwTick;


	if(pChr->IsDead())
	{
		return STATEPROCESS::END;
	}

	// 종료~
	if (m_dwCurTime >=m_dwlifeTime)//에니메이션 진척도가 100%일때
	{
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		if (pChr->FindSkillEffect(eCHAR_STATE_PROTECTION))
		{
			return STATEPROCESS::HOLD;
		}
		else
		{
			if (pChr->ProcessNextAction())
			{
				return STATEPROCESS::SUSPEND;
			}
			else
			{
				return STATEPROCESS::END;
			}
		}
#else
		if (pChr->ProcessNextAction())
		{
			return STATEPROCESS::SUSPEND;
		}
		else
		{
			return STATEPROCESS::END;
		}
#endif // _NA_008472_20150903_IMPROVED_BALANCE
	}

	return STATEPROCESS::HOLD;
}

void SkillStateDefenceMotion::ProcessExtraCommand()
{
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    // 라이프타임을 0으로 설정해서 종료되도록
    m_dwlifeTime = 0;
#endif
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG
BOOL SkillStateDefenceMotion::CanTransite( Object* owner_ptr, DWORD cur_time, StateBase* to_transite_state )
{
	Character* character_ptr = (Character *)owner_ptr;

	if ( m_dwCurTime >=m_dwlifeTime )
	{
		if (!(character_ptr->FindSkillEffect(eCHAR_STATE_PROTECTION)))
		{
			return TRUE;
		}
	}

	return FALSE;
}
#endif // _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG
