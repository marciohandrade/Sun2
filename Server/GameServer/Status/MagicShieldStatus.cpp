#include "stdafx.h"
#include "Ability.h"
#include "MagicShieldStatus.h"

VOID MagicShieldStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
	AbilityStatus::Init( pOwner, pAttacker, pAbility );

	const BASE_ABILITYINFO *pBaseAbilityInfo = pAbility->GetBaseAbilityInfo();
	m_iShieldHP = pBaseAbilityInfo->m_iParam[1];
    
    //!~ MP 감소 포인트에서 비율로 변경 - arycoat 2013.04.08.
#ifdef _NA_006621_20130408_MAGICSHILD_MP_DECREASE
	m_iDecreaseMP = 0;
    m_fDecreaseMpRatio = pBaseAbilityInfo->m_iOption2 / 1000.f;
#else
    m_iDecreaseMP = pBaseAbilityInfo->m_iOption2;
    m_fDecreaseMpRatio = 0;
#endif //_NA_006621_20130408_MAGICSHILD_MP_DECREASE
    //~!

	m_fAbsorbRatio = pBaseAbilityInfo->m_iParam[0] / 1000.f;
}

VOID MagicShieldStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
	// 마법쉴드 셋팅( ShieldHP, DecreaseMP, AbsorbRatio )
	owner->SetShield( m_iShieldHP, m_iDecreaseMP, m_fAbsorbRatio, m_fDecreaseMpRatio );
}

VOID MagicShieldStatus::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 쉴드 초기화
        owner->SetShield( 0, 0, 0, 0 );
    }
	AbilityStatus::End();
}



















