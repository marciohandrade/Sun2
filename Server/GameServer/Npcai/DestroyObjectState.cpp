#include "stdafx.h"
#include "DestroyObjectState.h"

//_NA_0_20100514_NPC_DEAD_STATE_TIME

void DestroyObjectState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);
}

// OnExit()�� �߻� ���� �ʴ´�.
void DestroyObjectState::OnExit()
{
    State::OnExit();
}

// ���� npc ��ü�� �ı��ȴ�
BOOL DestroyObjectState::OnUpdate(DWORD dwDeltaTick)
{
    return false;
}