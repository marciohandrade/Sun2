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

	// ���� �þ߸� ������ �� �þ߸� 1�� ���δ�.
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
        // �þ߸� �����·� �����Ѵ�.
        owner->SetAttr( eATTR_SIGHT_RANGE, eATTR_KIND_BASE, m_iPrevSightRange );
    }
	
	AbilityStatus::End();
}



















