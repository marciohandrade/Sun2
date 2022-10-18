#include "stdafx.h"

#include "IdleState.h"
#include "GameField.h"
#include "BattleRecord.h"
#include "TargetManager.h"
#include "StatusManager.h"
#ifdef _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE
#include "../MovingPlanner.h"
#include "StatusManager.h"
#endif //_NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE

IdleState::IdleState()
{
}

IdleState::~IdleState()
{
}

void
IdleState::Init()
{
}

void
IdleState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    DWORD delay = param1
                ?   (DWORD)param1
                :   (DWORD)dRandom(m_pAITypeInfo->m_wIDLE_MIN_TIME, m_pAITypeInfo->m_wIDLE_MAX_TIME);
    m_IdleTimer.SetTimer(m_IdleTimer.eStaticTimer_Idle, delay);
    m_SearchTimer.SetTimer(m_SearchTimer.eStaticTimer_Search, 0);
    m_pNPC->SetTargetChar(NULL);
}

void
IdleState::OnExit()
{
    State::OnExit();
}

BOOL
IdleState::OnUpdate(DWORD dwDeltaTick)
{
    if (m_pNPC->ISGMIdleOn())
        return true;

    if (m_IdleTimer.IsExpired())
    {
        m_pNPC->ChangeState(STATE_ID_WANDER);
        return true;
    }

#ifdef _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE
    //자신의 위치가 안전지대 타일이면 원위치로 돌아간다.
    const nsAI::TileEvent* tile_event = 0;

    if (m_pNPC->GetStatusManager()->CanMove() && 
        (m_pNPC->PlanEvent->GetTileEvent(&tile_event) == false || (tile_event->Attr & PTA_SAFETY_ZONE)))
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_RETURN);
        
        return true;
    }
#endif //_NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE

    if (m_SearchTimer.IsExpired() && m_pNPC->CanAttack())
    {
        if (!m_pNPC->GetStatusManager()->Status.IsPassConstraint(m_pNPC, (SkillScriptInfo*)0))
            return true;

        Character* pTargetChar = m_pNPC->SearchTarget(); // find nearest target
        if (pTargetChar == NULL)
            return true;

        // 그룹의 일원이면 멤버들에게 적 발견 메세지를 보낸다.
        if (m_pNPC->IsGroupMember())
        {
            AI_MSG_ENEMY_FOUND aiMsg;
            aiMsg.dwTargetObjectKey = pTargetChar->GetObjectKey();
            m_pNPC->SendAIMessageToGroupExceptMe(&aiMsg, sizeof(aiMsg));
        }

        // 타겟을 변경하고 상태를 전환한다.
        m_pNPC->SetTargetChar(pTargetChar);
        m_pNPC->ChangeState(STATE_ID_TRACK);
    }
    return true;
}

void
IdleState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (m_pNPC->IsFriend(pAttacker) == USER_RELATION_ENEMY)
    {
        m_pNPC->SetTargetChar(pAttacker);
        m_pNPC->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

void
IdleState::OnMsgLetsGo(const AI_MSG* pMsg)
{
    State::OnMsgLetsGo(pMsg);

    m_pNPC->ChangeState(STATE_ID_WANDER, 0, 0, pMsg->dwMsgID);
}

