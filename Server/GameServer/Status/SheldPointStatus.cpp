#include "stdafx.h"
#include "SheldPointStatus.h"
#include "StatusManager.h"

// ��ų�� ���� ���°� �ƴ� ��� Ŭ�� ���� ������ ���� �ʴ´�.
// ���� ��ų����ó�� �������� �ο��ϴ� ���� ��θ� Ŭ�� Render INfo�� ������.
// Start���� BRD �Ұ�� enter�� Render INfo �� �ߺ� �߻� �Ҽ� �ִ�.
// ���� ���� ���»����� SendStatusAddBRD�� ���� ���ش�. 

VOID SheldPointStatus::Start()
{
	EtcStatus::Start();
}

VOID SheldPointStatus::End()
{
	EtcStatus::End();
    SendStatusDelBRD();
}

BOOL SheldPointStatus::CanRemove()  
{
    const Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return TRUE;
    }

    if( 0 == owner->GetHP() )        // ������ 
    {
        return TRUE;
    }    

    if( 0 != owner->GetSD() )        // SD�� ���� �ִٸ�, ��������
        return FALSE;

    return TRUE;
}

