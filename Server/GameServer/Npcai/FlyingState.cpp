#include "stdafx.h"
#include "FlyingState.h"
#include "GameField.h"
#include <Timer.h>
#include "StatusManager.h"


FlyingState::FlyingState()
{
}

FlyingState::~FlyingState()
{
}

void
FlyingState::Init()
{
    __noop;
}

void
FlyingState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    DWORD interval = static_cast<DWORD>(param1);
    m_FlyingTimer.SetTimer(interval);
    // 하늘로 떠있는 속성을 켜준다.
    m_pNPC->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_FLYING);

    m_bKnockDown = false;
}

void
FlyingState::OnExit()
{
    State::OnExit();

    m_bKnockDown = false;

    // 하늘로 떠있는 속성을 꺼준다.
    m_pNPC->GetStatusManager()->Remove(eCHAR_STATE_ETC_FLYING);
}

BOOL
FlyingState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (m_FlyingTimer.IsExpired())
    {
        if (m_bKnockDown)
        {
            m_pNPC->ChangeState(STATE_ID_KNOCKDOWN);
        }
        else
        {
            if (m_pNPC->GetTargetChar())
            {
                m_pNPC->ChangeState(STATE_ID_TRACK);
            }
            else
            {
                m_pNPC->ChangeState(STATE_ID_IDLE);
            }
        }
    }
    return true;
}

void
FlyingState::OnMsgKnockDown(const AI_MSG* pMsg)
{
    m_bKnockDown = true;
}

void
FlyingState::OnMsgStun(const AI_MSG* pMsg)
{
    m_bKnockDown = true;
}

// Flying상태가 끝나면 Follow(Idle)상태로 복귀한다.
void
FlyingState::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    m_pNPC->SetTargetChar(NULL);
}

