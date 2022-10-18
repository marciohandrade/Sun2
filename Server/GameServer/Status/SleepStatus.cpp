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

	// �̵����̶�� �����.
	owner->StopMoving();

	// ���� �����̸� �ʱ�ȭ�Ѵ�.
	owner->SetActionDelay( 0 );
}

// ��ų���� �������ڸ��� �������� ó���� �� �������¿��� �����Ǿ� ������.
// ��, ��ų ��� ��Ŷ�� ������ ���� ���� ��Ŷ�� �������ν� Ŭ�󿡼��� ������ ���� �ʴ´�.
// ���� ���ݹ޴��� ������ �ɸ��� 1�ʴ� ���� �Ŀ� �����ϰԵǸ� �̷� ������ �ذ�ȴ�. �ļ���!
BOOL SleepStatus::CanRemove()
{
	// ������� ���� �ð�
	int iLeftTick = (int)( GetExpiredTime() - GetTickCount() );

	// ���°� �ɸ� �ð�
	int iProcessTick = GetApplicationTime() - iLeftTick;

	if( iProcessTick >= 1000 )
		return TRUE;
	else
		return FALSE;
}



















