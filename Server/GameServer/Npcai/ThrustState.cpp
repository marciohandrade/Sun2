#include "stdafx.h"
#include "ThrustState.h"
#include "GameField.h"
#include <Timer.h>
#include <Define.h>
#include "AIParamParser.h"

ThrustState::ThrustState()
{
    m_dwDownTimeAfterThrust = 0;
}

ThrustState::~ThrustState()
{
}

void
ThrustState::Init()
{
    __noop;
}

void
ThrustState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_bDownAfterThrust = (BOOL)param1;

    eCHAR_MOVE_STATE move_state = m_bDownAfterThrust ? CMS_KNOCKBACK_DOWN : CMS_KNOCKBACK;
    m_pNPC->SetMoveState(move_state);

    // 밀리는 상태는 1.5초후에 종료
    //m_ThrustTimer.SetTimer(AIParamParser::AICommonInfo.m_wTHRUST_TIME);
    m_ThrustTimer.SetTimer(m_ThrustTimer.eStaticTimer_Thrust, 0);
}

void
ThrustState::OnExit()
{
    State::OnExit();

    m_pNPC->StopMoving();
}

// 상태 버그 문제 [5/14/2010 lst1024]
// 버서커의 태클 스킬의 경우 넉백, 트러스트, 스킬 공격 데미지, 3가지의 어빌리티가 있다.
// 넉백 어빌리티는 npc를 KnockDownState로 만들고,
// 트러스트는 npc를 ThrustState로,
// 스킬 공격 데미지는 관련 상태 유발 없이 현재 상태에 OnMsgAttacked 를 보낸다. 
// 
// 문제 1. 넉백과 트러스트는 공격자에 대한 셋팅을 하지 않는다. 의도된 것인가?
// 문제 2. 태클의 경우 npc가 넉백과 트러스트상태를 유발, 이후에 들어오는 스킬 공격 데미지에 의한 OnMsgAttacked 을 무시한다.  


BOOL
ThrustState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (m_ThrustTimer.IsExpired())
    {
        if (m_bDownAfterThrust)
        {
            m_pNPC->ChangeState(STATE_ID_KNOCKDOWN, m_dwDownTimeAfterThrust);
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


// 밀리는 도중에 날아온 다운 메세지는 무시
void
ThrustState::OnMsgKnockDown(const AI_MSG* pMsg)
{
    const AI_MSG_KNOCKDOWN* pRecvMsg = static_cast<const AI_MSG_KNOCKDOWN*>(pMsg);
    m_bDownAfterThrust = true;
    m_dwDownTimeAfterThrust = pRecvMsg->dwKnockDownTick;
}

void
ThrustState::OnMsgStun(const AI_MSG* pMsg)
{
    m_bDownAfterThrust = true;
}

// 밀리기가 끝나면 Follow(Idle)상태로 복귀한다.
void
ThrustState::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    m_pNPC->SetTargetChar(NULL);
}

#ifdef _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE
void ThrustState::OnMsgAttacked( const AI_MSG* pMsg )
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
    State::SetTarget(pMsg);
#else
    if (m_pNPC->IsFriend(pAttacker) == USER_RELATION_ENEMY)
    {
        m_pNPC->SetTargetChar(pAttacker);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

}
#endif //_NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE