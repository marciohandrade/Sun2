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
        // Help���·� ��ȯ�Ǿ����� �ֺ��� �˷��ش�.
        MSG_CG_STATUS_NPC_STATE_CHANGE_CMD sendMsg;
        sendMsg.m_dwObjectKey = m_pNPC->GetObjectKey();
        sendMsg.m_byState = eNPC_STATE_HELP_RESPONSE;
        m_pNPC->SendPacketAround(&sendMsg, sizeof(sendMsg));
    }

    m_pNPC->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_HELP);

    // Help���¿����� ������ BattleRecord ������ �ʱ�ȭ�ϰ�, Help�� ��û�ϰ� ���� Ÿ�ٸ� �����Ѵ�.
    Character* pTarget = m_pNPC->GetTargetChar();
    m_pNPC->GetTargetManager()->ClearBattleRecord();
    m_pNPC->SetTargetChar(pTarget);

    m_HelpTimer.SetTimer(m_pAITypeInfo->m_wSEARCH_PERIOD);
    // ���� ��� ���õǾ� ���� ������ ����
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

    // Ÿ�ٰ��� �Ÿ� ���ϱ�
    const float fDistToTarget = m_pNPC->GetDist(target);
    

    // Ÿ���� �þ� ������ ����� ��Ȳ ���·� ����
    if (m_pNPC->IsGroupMember() == false) {
        float sight_range = m_pNPC->GetSightRange();
        ;     sight_range *= AIParamParser::AICommonInfo.m_fHELP_SIGHTRANGE_RATIO;

        if (fDistToTarget > sight_range) {
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_IDLE);
            return true;
        }
    }

    // Ÿ���� ���� ��Ÿ� ���� ��������� ���� ���·� ����
    if (fDistToTarget <= m_pNPC->GetAttackRange())
    {
        m_pNPC->ChangeState(STATE_ID_ATTACK);
        return true;
    }

    if (m_pNPC->IsMoving())
    {
        // 1�ʸ��� Ÿ���� ��ǥ�� �ٽ� �ѹ� ��ã��
        if (m_HelpTimer.IsExpired())
        {
            DoTrack();
        }
    }
    else
    {       
        // Ÿ���� ���� ��Ÿ� ���� ������� ������ �ٽ� ��ã��
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
        // �÷��̾� ��ġ�� ��Ȯ�� ���ٸ� ���� ����
        m_pNPC->ChangeState(STATE_ID_ATTACK);
        return;
    }

    // ���ο� Track Position�� ã�´�.
    // (NOTE) thinking... relation with target position
    WzVector vTrackPos;
    m_pNPC->GetTrackPos(vTrackPos);

    // ��ã��..���н� ���� ����
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

    // ���� �����ϰ� �ִ� ����̶�� �������¸� �����Ѵ�.
    Character* pCurTarget = m_pNPC->GetTargetChar();
    if (pCurTarget && pCurTarget->GetObjectKey() == pAttackMsg->dwTargetKey)
    {
        return;
    }

    State::OnMsgForceAttack(pMsg);
}

