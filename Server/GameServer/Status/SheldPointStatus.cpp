#include "stdafx.h"
#include "SheldPointStatus.h"
#include "StatusManager.h"

// 스킬로 인한 상태가 아닌 경우 클라에 랜더 인포를 주지 않는다.
// 기존 스킬상태처럼 서버에서 부여하는 상태 모두를 클라에 Render INfo를 보낸다.
// Start에서 BRD 할경우 enter시 Render INfo 와 중복 발생 할수 있다.
// 게임 도중 상태생성시 SendStatusAddBRD를 직접 해준다. 

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

    if( 0 == owner->GetHP() )        // 죽을때 
    {
        return TRUE;
    }    

    if( 0 != owner->GetSD() )        // SD가 남아 있다면, 삭제금지
        return FALSE;

    return TRUE;
}

