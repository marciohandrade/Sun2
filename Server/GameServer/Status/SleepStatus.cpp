#include "stdafx.h"
#include "Ability.h"
#include "SleepStatus.h"


VOID SleepStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	// 이동중이라면 멈춘다.
	owner->StopMoving();

	// 공격 딜레이를 초기화한다.
	owner->SetActionDelay( 0 );
}

// 스킬에서 슬립되자마자 데미지를 처리할 때 슬립상태에서 해제되어 버린다.
// 즉, 스킬 결과 패킷을 보내기 전에 해제 패킷을 보냄으로써 클라에서는 해제가 되지 않는다.
// 따라서 공격받더라도 슬립이 걸린지 1초는 지난 후에 해제하게되면 이런 문제가 해결된다. 꼼수임!
BOOL SleepStatus::CanRemove()
{
	// 만료까지 남은 시간
	int iLeftTick = (int)( GetExpiredTime() - GetTickCount() );

	// 상태가 걸린 시간
	int iProcessTick = GetApplicationTime() - iLeftTick;

	if( iProcessTick >= 1000 )
		return TRUE;
	else
		return FALSE;
}



















