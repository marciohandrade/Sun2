#include "stdafx.h"
#include "Ability.h"
#include "BlindStatus.h"


VOID BlindStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	// 이전 시야를 저장한 후 시야를 1로 줄인다.
	m_iPrevSightRange = (int)( owner->GetSightRange() * 10 );
	owner->SetAttr( eATTR_SIGHT_RANGE, eATTR_KIND_BASE, 10 );

	AI_MSG_BLIND aiMsg;
	aiMsg.dwBlindTime = GetApplicationTime();
	owner->SendAIMessage( &aiMsg, sizeof(aiMsg) );
}

VOID BlindStatus::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 시야를 원상태로 복구한다.
        owner->SetAttr( eATTR_SIGHT_RANGE, eATTR_KIND_BASE, m_iPrevSightRange );
    }
	
	AbilityStatus::End();
}



















