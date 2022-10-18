#include "stdafx.h"
#include "ChaosState.h"


ChaosState::ChaosState()
{
}

ChaosState::~ChaosState()
{
}

void
ChaosState::Init()
{
    __noop;
}

void
ChaosState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);
}

void
ChaosState::OnExit()
{
    State::OnExit();
}

BOOL
ChaosState::OnUpdate(DWORD dwDeltaTick)
{
    return true;
}

// 혼란상태가 끝나면 Follow(Idle)상태로 복귀한다.
void
ChaosState::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    m_pNPC->SetTargetChar(NULL);
}
