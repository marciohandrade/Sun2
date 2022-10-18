#include "stdafx.h"
#include "StyleThrust.h"
#include "StatusManager.h"


VOID StyleThrust::Init( Character *pOwner, WORD wStateID, int iApplicationTime, int iPeriod )
{
	m_wDownTimeAfterThrust = 0;

	EtcStatus::Init( pOwner, wStateID, iApplicationTime, iPeriod );
}

VOID StyleThrust::Start()
{
	EtcStatus::Start();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 공격 딜레이를 초기화한다.
        owner->SetActionDelay( 0 );
    }
}

VOID StyleThrust::End()
{
	EtcStatus::End();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // Thrust가 끝난 후에 다운이 되는 경우가 있다.
        if( m_wDownTimeAfterThrust )
        {
            owner->GetStatusManager()->AllocStatus( eCHAR_STATE_STYLE_DOWN, m_wDownTimeAfterThrust, 0 );
        }
    }
}