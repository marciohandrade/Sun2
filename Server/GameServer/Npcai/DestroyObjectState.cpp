#include "stdafx.h"
#include "DestroyObjectState.h"

//_NA_0_20100514_NPC_DEAD_STATE_TIME

void DestroyObjectState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);
}

// OnExit()는 발생 하지 않는다.
void DestroyObjectState::OnExit()
{
    State::OnExit();
}

// 이후 npc 객체가 파괴된다
BOOL DestroyObjectState::OnUpdate(DWORD dwDeltaTick)
{
    return false;
}