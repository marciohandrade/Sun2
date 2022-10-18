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
        // ���� �����̸� �ʱ�ȭ�Ѵ�.
        owner->SetActionDelay( 0 );
    }
}

VOID StyleThrust::End()
{
	EtcStatus::End();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // Thrust�� ���� �Ŀ� �ٿ��� �Ǵ� ��찡 �ִ�.
        if( m_wDownTimeAfterThrust )
        {
            owner->GetStatusManager()->AllocStatus( eCHAR_STATE_STYLE_DOWN, m_wDownTimeAfterThrust, 0 );
        }
    }
}