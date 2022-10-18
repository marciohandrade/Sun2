#include "stdafx.h"
#include "BaseEffect.h"



VOID BaseEffect::Init( SLOTCODE SkillCode, int iApplicationTime, int iPeriod, Character *pAttacker, WzVector vCurPos, float fRadius )
{
	m_SkillCode = SkillCode;
	m_vCurPos = vCurPos;
	m_fRadius = fRadius;

	m_pField = pAttacker->GetField();
	m_dwSectorIndex = pAttacker->GetSectorIndex();

	BaseStatus::Init( pAttacker, GetStateID(), iApplicationTime, iPeriod );
}

VOID BaseEffect::Execute()
{
	BaseStatus::Execute();
}

#ifdef _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME
BOOL BaseEffect::Update( DWORD cur_time )
{
    Character* owner = GetOwner();
    if (owner != NULL && owner->IsDead()) {
        return FALSE;
    }
    return BaseStatus::Update(cur_time);
}
#endif //_NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME








