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

    // �и��� ���´� 1.5���Ŀ� ����
    //m_ThrustTimer.SetTimer(AIParamParser::AICommonInfo.m_wTHRUST_TIME);
    m_ThrustTimer.SetTimer(m_ThrustTimer.eStaticTimer_Thrust, 0);
}

void
ThrustState::OnExit()
{
    State::OnExit();

    m_pNPC->StopMoving();
}

// ���� ���� ���� [5/14/2010 lst1024]
// ����Ŀ�� ��Ŭ ��ų�� ��� �˹�, Ʈ����Ʈ, ��ų ���� ������, 3������ �����Ƽ�� �ִ�.
// �˹� �����Ƽ�� npc�� KnockDownState�� �����,
// Ʈ����Ʈ�� npc�� ThrustState��,
// ��ų ���� �������� ���� ���� ���� ���� ���� ���¿� OnMsgAttacked �� ������. 
// 
// ���� 1. �˹�� Ʈ����Ʈ�� �����ڿ� ���� ������ ���� �ʴ´�. �ǵ��� ���ΰ�?
// ���� 2. ��Ŭ�� ��� npc�� �˹�� Ʈ����Ʈ���¸� ����, ���Ŀ� ������ ��ų ���� �������� ���� OnMsgAttacked �� �����Ѵ�.  


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


// �и��� ���߿� ���ƿ� �ٿ� �޼����� ����
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

// �и��Ⱑ ������ Follow(Idle)���·� �����Ѵ�.
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

    // ��밡 �����̸� ���� ���·� ����
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