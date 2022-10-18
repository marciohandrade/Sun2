#include "stdafx.h"
#include "KnockDownState.h"
#include "GameField.h"
#include <Timer.h>
#include "AIParamParser.h"

KnockDownState::KnockDownState()
{
}

KnockDownState::~KnockDownState()
{
}

void
KnockDownState::Init()
{
    __noop;
}

void
KnockDownState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    DWORD delay = param1
                ?   static_cast<DWORD>(param1)
                :   AIParamParser::AICommonInfo.m_wKNOCKDOWN_TIME;

    m_DownTimer.SetTimer(delay);
}

void
KnockDownState::OnExit()
{
    State::OnExit();
}

BOOL
KnockDownState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (m_DownTimer.IsExpired())
    {
        if (m_pNPC->GetTargetChar())
        {
            m_pNPC->ChangeState(STATE_ID_TRACK);
        }
        else
        {
            m_pNPC->ChangeState(STATE_ID_WANDER);
        }
    }
    return true;
}

// 다운상태가 끝나면 Follow(Idle)상태로 복귀한다.
void
KnockDownState::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    m_pNPC->SetTargetChar(NULL);
}

void
KnockDownState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 상대가 적군이고 현재 설정된 타겟이 없을 경우에만 타겟 설정을 해둔다. 깨어나면 추적!
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg);
#else
    if (USER_RELATION_ENEMY == m_pNPC->IsFriend(pAttacker))
    {
        if (m_pNPC->GetTargetChar() == NULL)
            m_pNPC->SetTargetChar(pAttacker);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

}

void
KnockDownState::OnMsgKnockDown(const AI_MSG* pMsg)
{
    const AI_MSG_KNOCKDOWN* pRecvMsg = static_cast<const AI_MSG_KNOCKDOWN*>(pMsg);

    if (pRecvMsg->dwKnockDownTick)
    {
        m_DownTimer.SetTimer(pRecvMsg->dwKnockDownTick);
    }
    else
    {
        m_DownTimer.SetTimer(AIParamParser::AICommonInfo.m_wKNOCKDOWN_TIME);
    }
}


