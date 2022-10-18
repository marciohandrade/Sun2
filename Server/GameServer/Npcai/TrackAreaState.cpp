#include "stdafx.h"
#include "TrackAreaState.h"
#include "GameField.h"
#include "AIParamParser.h"
#include "NPCStateManager.h"

TrackAreaState::TrackAreaState()
{
    // (WARNING) initalization problem...
    m_bRegenPosSelected = false; // TODO(WAVERIX)
}

TrackAreaState::~TrackAreaState()
{
}

void
TrackAreaState::Init()
{   
}

void
TrackAreaState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_SearchTimer.SetTimer(m_SearchTimer.eStaticTimer_Search, 0);
    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);

    Field* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    m_bIsSetAreaPos = true;
    NPCStateManager* state_manager = m_pNPC->GetStateManager();

    if (m_pNPC->GetRegenID() != 0)  //������ ���� ���� ���.
    {
        m_vAreaPos = state_manager->GetDestPos();
    }
    else
    {
        if (pField->RandomAreaPos(m_vAreaPos, state_manager->GetMoveAreaID()) == false)
            m_bIsSetAreaPos = false;
    }

    if (m_bIsSetAreaPos)
    {
        if (m_pNPC->MoveAndBroadcast(&m_vAreaPos, CMS_RUN))
            m_bRegenPosSelected = true;
        else
            m_bRegenPosSelected = false;
    }
}

void
TrackAreaState::OnExit()
{
    State::OnExit();
}

BOOL
TrackAreaState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (m_SearchTimer.IsExpired())
    {
        // �ֺ��� ���� �ִ��� Ž��
        Character* pTargetChar = m_pNPC->SearchTarget();
        if (pTargetChar)
        {
            m_pNPC->SetTargetChar(pTargetChar);
            m_pNPC->ChangeState(STATE_ID_TRACK);
            return true;
        }
    }

    Field* pField = m_pNPC->GetField();
    if (pField == NULL)
        return true;

    if (!m_bIsSetAreaPos)
    {
        m_bIsSetAreaPos = true;
        NPCStateManager* state_manager = m_pNPC->GetStateManager();

        if (m_pNPC->GetRegenID() != 0)  //������ ���� ���� ���.
        {
            m_vAreaPos = state_manager->GetDestPos();
        }
        else
        {
            if (pField->RandomAreaPos(m_vAreaPos, state_manager->GetMoveAreaID()) == false)
                m_bIsSetAreaPos = false;
        }

        if (m_bIsSetAreaPos == false)
        {
            m_pNPC->ChangeState(STATE_ID_IDLE);
            return true;
        }
    }

    // �̵��� ������ �ٽ� m_vAreaPos�� �����δ�.
    if (m_TrackTimer.IsExpired() && m_pNPC->IsMoving() == false)
    {
        // �������� �����ϸ� Idle���·� ����
        if (m_bRegenPosSelected)
        {
            m_pNPC->ChangeState(STATE_ID_IDLE);
            return true;
        }
        // �������� ������ ������ �ٽ� �̵�
        else
        {
            DoTrackArea();
        }
    }
    return true;
}

void
TrackAreaState::DoTrackArea()
{
    Field* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    const WzVector& wvCurPos = *m_pNPC->GetPosPtr();

    // �̵��� �Ÿ��� ���Ѵ�.
    float MOVE_DISTANCE = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                         AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);
    float fDistFromRegenPos = VectorLength(&(m_vAreaPos - wvCurPos));
    if (MOVE_DISTANCE >= fDistFromRegenPos)
    {
        MOVE_DISTANCE = fDistFromRegenPos;
        m_bRegenPosSelected = true;
    }

    WzVector wvNormal;
    // Ư������(m_vAreaPos)�� ���� ���ݾ� ��ã���Ѵ�.
    VectorNormalize(&wvNormal, &(m_vAreaPos - wvCurPos)); wvNormal.z = 0;
    WzVector& wvNextPos = wvCurPos + wvNormal * MOVE_DISTANCE;

    // �̵����� ���θ� üũ�� ��Ŷ�� ������.
    if (!m_pNPC->MoveAndBroadcast(&wvNextPos, CMS_RUN))
    {
        // �̵��� �����ϸ� ���� m_vAreaPos�� �̵��غ���.
        if (!m_pNPC->MoveAndBroadcast(&m_vAreaPos, CMS_RUN))
        {
            // �׷��� �����ϸ� �ٸ� ������ ã�´�.
            pField->RandomAreaPos(m_vAreaPos, m_pNPC->GetStateManager()->GetMoveAreaID());
            return;
        }
    }

    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
}

