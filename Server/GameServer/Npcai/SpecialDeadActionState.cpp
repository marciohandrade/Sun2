#include "stdafx.h"
#include "SpecialDeadActionState.h"

//_NA_0_20100514_NPC_DEAD_STATE_TIME

void SpecialDeadActionState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    ASSERT(param1);

    State::OnEnter(param1, param2, param3);

    special_condition_ = reinterpret_cast<SPECIAL_CONDITION*>(param1);
    once_ = false;
}

// OnExit()는 발생 하지 않을 수 있다.
void SpecialDeadActionState::OnExit()
{
    State::OnExit();
}

BOOL SpecialDeadActionState::OnUpdate(DWORD dwDeltaTick)
{
    const BOOLEAN isExpired = self_destruction_timer_.IsExpired();
    if (once_ && isExpired)
    {
        m_pNPC->ChangeState(STATE_ID_DESTROY_OBJECT);
        return true;
    }

    if (!once_)
    {
        once_ = true;
        m_pNPC->SpecialAction(*special_condition_);

        self_destruction_timer_.SetTimer(interval_for_self_destruction);
    }

    return true;
}