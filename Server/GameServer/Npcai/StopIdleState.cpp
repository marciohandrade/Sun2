#include "stdafx.h"
#include "StopIdleState.h"
#include "GameField.h"
#include <Timer.h>
#include "AIParamParser.h"
#include "TargetManager.h"
#include "Skill.h"
#include <PacketControl/WarMessages_Sync.h> //_NA_0_20100817_HELLON_LAST_TRIGGER


StopIdleState::StopIdleState()
{
}

StopIdleState::~StopIdleState()
{
}

void
StopIdleState::Init()
{
}

void
StopIdleState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_SearchTimer.SetTimer(m_SearchTimer.eStaticTimer_Search, 0);

    // 타겟이 있으면 공격방법을 찾는다. 그래야 공격사거리를 구할 수가 있다.
    if (m_pNPC->GetTargetChar())
        m_pNPC->SelectSkill();

    // added by _NA_0_20100817_HELLON_LAST_TRIGGER
     if (m_pNPC->IsMoving())
     {
         m_pNPC->StopMoving();
         // 주변에 정지를 알린다.
         MSGSUB_SYNC_STOP_BRD msg;
         msg.m_dwObjectKey = m_pNPC->GetObjectKey();
         msg.m_wvCurPos    = *m_pNPC->GetPosPtr();
         m_pNPC->SendPacketAroundForSync(&msg);
     }
}

void
StopIdleState::OnExit()
{
    State::OnExit();
}

BOOL
StopIdleState::OnUpdate(DWORD dwDeltaTick)
{
    if (m_pNPC->ISGMIdleOn())
        return true;

    __UNUSED(dwDeltaTick);

    if (m_SearchTimer.IsExpired())
    {
        // 타겟이 없으면 시야내의 적군을 찾는다.
        Character* pTargetChar = m_pNPC->GetTargetChar();
        if (pTargetChar == NULL)
        {
            pTargetChar = m_pNPC->SearchTarget();
            if (pTargetChar)
            {
                // 그룹의 일원이면 멤버들에게 적 발견 메세지를 보낸다.
                if (m_pNPC->IsGroupMember())
                {
                    AI_MSG_ENEMY_FOUND aiMsg;
                    aiMsg.dwTargetObjectKey = pTargetChar->GetObjectKey();
                    m_pNPC->SendAIMessageToGroupExceptMe(&aiMsg, sizeof(aiMsg));
                }

                m_pNPC->SetTargetChar(pTargetChar);
                m_pNPC->SelectSkill();
            }
        }

        // 타겟이 있으면
        if (pTargetChar)
        {
            // 공격 방법 선택되어 있지 않으면 선택
            if (m_pNPC->GetSelectedSkill() == SKILLCODE_INVALID)
            {
                m_pNPC->SelectSkill();
            }

            // 타겟이 공격 사거리 내에 들어있으면 Attack 상태로 전이
            float dist_to_target = m_pNPC->GetDist(pTargetChar);
            if (dist_to_target > (m_pNPC->GetAttackRange() +
                                 AIParamParser::AICommonInfo.m_fRANGE_TOLERANCE))
            {
                return true;
            }

            m_pNPC->ChangeState(STATE_ID_ATTACK);
        }
    }
    return true;
}

void
StopIdleState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 현재 선택된 타겟이 없고 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (NULL == m_pNPC->GetTargetChar() &&
        USER_RELATION_ENEMY == m_pNPC->IsFriend(pAttacker))
    {
        m_pNPC->SetTargetChar(pAttacker);
        m_pNPC->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

}

