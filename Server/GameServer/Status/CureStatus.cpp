#include "stdafx.h"
#include "Ability.h"
#include "CureStatus.h"
#include "StatusManager.h"

VOID CureStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
	AbilityStatus::Init( pOwner, pAttacker, pAbility );

	const BASE_ABILITYINFO *pBaseAbilityInfo = pAbility->GetBaseAbilityInfo();
	m_StateValue[0] = pBaseAbilityInfo->m_iOption1;
	m_StateValue[1] = pBaseAbilityInfo->m_iOption2;
	m_StateValue[2] = pBaseAbilityInfo->m_iParam[0];
	m_StateValue[3] = pBaseAbilityInfo->m_iParam[1];
}

VOID CureStatus::Start()
{
	// 지정된 상태 이상을 제거한다.
	RemoveStatus();
}

VOID CureStatus::Execute()
{
	AbilityStatus::Execute();

	// 지정된 상태 이상을 제거한다.
	RemoveStatus();
}

VOID CureStatus::RemoveStatus()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	StatusManager *pStatusManager = owner->GetStatusManager();
	if( !pStatusManager )	return;

    // m_StateValue[0] != 0 이면 m_StateValue[0]~m_StateValue[3]은 eCHAR_STATE_TYPE 이다.(개별상태)
	// m_StateValue[0] == 0 이면 m_StateValue[1]~m_StateValue[3]은 eSTATE_TYPE이고(상태그룹)
	if( m_StateValue[0] != 0)
	{
		for( int i = 0; i < MAX_CURE_STATE_NUM; ++i )
		{
            if( pStatusManager->FindStatus( (eCHAR_STATE_TYPE)m_StateValue[i] ) )
                pStatusManager->Remove( (eCHAR_STATE_TYPE)m_StateValue[i] );
		}
	}
	else
	{
		for( int i = 1; i < MAX_CURE_STATE_NUM; ++i )
		{
            if( (eSTATE_TYPE)m_StateValue[i] )
                pStatusManager->CureAll( (eSTATE_TYPE)m_StateValue[i] );
		}
	}
}




