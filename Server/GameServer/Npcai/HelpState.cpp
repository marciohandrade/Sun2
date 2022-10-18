#include "stdafx.h"
#include <Timer.h>
#include "HelpState.h"
#include "GameField.h"
#include "AITypes.h"
#include <PacketStruct_CG.h>
#include "Const.h"
#include "AIParamParser.h"
#include "TargetManager.h"
#include "StatusManager.h"
#include "Skill.h"

HelpState::HelpState()
{
}

HelpState::~HelpState()
{
}

void
HelpState::Init()
{
}

void
HelpState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    GameField* pField = m_pNPC->GetField();
    if (pField)
    {
        // Help상태로 전환되었음을 주변에 알려준다.
        MSG_CG_STATUS_NPC_STATE_CHANGE_CMD sendMsg;
        sendMsg.m_dwObjectKey = m_pNPC->GetObjectKey();
        sendMsg.m_byState = eNPC_STATE_HELP_RESPONSE;
        m_pNPC->SendPacketAround(&sendMsg, sizeof(sendMsg));
    }

    m_pNPC->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_HELP);

    // Help상태에서는 기존의 BattleRecord 정보를 초기화하고, Help를 요청하게 만든 타겟만 설정한다.
    Character* pTarget = m_pNPC->GetTargetChar();
    m_pNPC->GetTargetManager()->ClearBattleRecord();
    m_pNPC->SetTargetChar(pTarget);

    m_HelpTimer.SetTimer(m_pAITypeInfo->m_wSEARCH_PERIOD);
    // 공격 방법 선택되어 있지 않으면 선택
    if (m_pNPC->GetSelectedSkill() == SKILLCODE_INVALID)
    {
        m_pNPC->SelectSkill();
    }
}

void
HelpState::OnExit()
{
    State::OnExit();

    m_pNPC->GetStatusManager()->Remove(eCHAR_STATE_ETC_HELP);
}

BOOL
HelpState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    ASSERT(NULL != m_pNPC->GetTargetChar());
    Character* target = m_pNPC->GetTargetChar();
    if (target == NULL)
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return true;
    }

    // 타겟과의 거리 구하기
    const float fDistToTarget = m_pNPC->GetDist(target);
    

    // 타겟이 시야 범위를 벗어나면 방황 상태로 전이
    if (m_pNPC->IsGroupMember() == false) {
        float sight_range = m_pNPC->GetSightRange();
        ;     sight_range *= AIParamParser::AICommonInfo.m_fHELP_SIGHTRANGE_RATIO;

        if (fDistToTarget > sight_range) {
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_IDLE);
            return true;
        }
    }

    // 타겟이 공격 사거리 내에 들어있으면 공격 상태로 전이
    if (fDistToTarget <= m_pNPC->GetAttackRange())
    {
        m_pNPC->ChangeState(STATE_ID_ATTACK);
        return true;
    }

    if (m_pNPC->IsMoving())
    {
        // 1초마다 타겟의 좌표로 다시 한번 길찾기
        if (m_HelpTimer.IsExpired())
        {
            DoTrack();
        }
    }
    else
    {       
        // 타겟이 공격 사거리 내에 들어있지 않으면 다시 길찾기
        DoTrack();
    }
    return true;
}

void
HelpState::DoTrack()
{
    Character* target = m_pNPC->GetTargetChar();
    ASSERT(NULL != target);
    if (target == NULL)
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return;
    }

    GameField* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    if (m_pNPC->IsSamePositionWithTarget())
    {
        // 플레이어 위치와 정확히 같다면 상태 전이
        m_pNPC->ChangeState(STATE_ID_ATTACK);
        return;
    }

    // 새로운 Track Position을 찾는다.
    // (NOTE) thinking... relation with target position
    WzVector vTrackPos;
    m_pNPC->GetTrackPos(vTrackPos);

    // 길찾기..실패시 상태 전이
    if (!m_pNPC->MoveAndBroadcast(&vTrackPos, CMS_RUN))
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);
    }
}

void
HelpState::OnMsgForceAttack(const AI_MSG* pMsg)
{
    const AI_MSG_FORCE_ATTACK* pAttackMsg = static_cast<const AI_MSG_FORCE_ATTACK*>(pMsg);

    // 현재 공격하고 있는 대상이라면 원래상태를 유지한다.
    Character* pCurTarget = m_pNPC->GetTargetChar();
    if (pCurTarget && pCurTarget->GetObjectKey() == pAttackMsg->dwTargetKey)
    {
        return;
    }

    State::OnMsgForceAttack(pMsg);
}

