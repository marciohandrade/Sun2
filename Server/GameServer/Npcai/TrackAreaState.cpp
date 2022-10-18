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

    if (m_pNPC->GetRegenID() != 0)  //맵툴로 찍은 몬스터 라면.
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
        // 주변에 적이 있는지 탐색
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

        if (m_pNPC->GetRegenID() != 0)  //맵툴로 찍은 몬스터 라면.
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

    // 이동이 끝나면 다시 m_vAreaPos로 움직인다.
    if (m_TrackTimer.IsExpired() && m_pNPC->IsMoving() == false)
    {
        // 목적지에 도달하면 Idle상태로 전이
        if (m_bRegenPosSelected)
        {
            m_pNPC->ChangeState(STATE_ID_IDLE);
            return true;
        }
        // 목적지에 도달할 때까지 다시 이동
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

    // 이동할 거리를 구한다.
    float MOVE_DISTANCE = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                         AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);
    float fDistFromRegenPos = VectorLength(&(m_vAreaPos - wvCurPos));
    if (MOVE_DISTANCE >= fDistFromRegenPos)
    {
        MOVE_DISTANCE = fDistFromRegenPos;
        m_bRegenPosSelected = true;
    }

    WzVector wvNormal;
    // 특정영역(m_vAreaPos)을 향해 조금씩 길찾기한다.
    VectorNormalize(&wvNormal, &(m_vAreaPos - wvCurPos)); wvNormal.z = 0;
    WzVector& wvNextPos = wvCurPos + wvNormal * MOVE_DISTANCE;

    // 이동가능 여부를 체크후 패킷을 날린다.
    if (!m_pNPC->MoveAndBroadcast(&wvNextPos, CMS_RUN))
    {
        // 이동이 실패하면 직접 m_vAreaPos로 이동해본다.
        if (!m_pNPC->MoveAndBroadcast(&m_vAreaPos, CMS_RUN))
        {
            // 그래도 실패하면 다른 지점을 찾는다.
            pField->RandomAreaPos(m_vAreaPos, m_pNPC->GetStateManager()->GetMoveAreaID());
            return;
        }
    }

    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
}

