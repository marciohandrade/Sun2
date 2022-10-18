#include "stdafx.h"
#include <Timer.h>
#include "RetreatState.h"
#include "GameField.h"
#include <PacketStruct_CG.h>
#include <SinTable.h>
#include <3DEngine/3DTerrain/PathExplorer.h>
#include <math.h>
#include "AIParamParser.h"
#include "PublicMath.h"


RetreatState::RetreatState()
{
    m_bFindDestPos = false;
    m_bTrialCount = 0;
    m_fMaxDistance = 0;
}

RetreatState::~RetreatState()
{
}

void
RetreatState::Init()
{   
}

void
RetreatState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    m_bFindDestPos = false;
    m_bTrialCount = 0;

    if (param1)
        m_fMaxDistance = static_cast<float>(param1);
    else
        m_fMaxDistance = m_pNPC->GetAttackRange();

    m_bIdleAfterRetreat = (BOOL)param2;

    State::OnEnter(param1, param2, param3);
}

void
RetreatState::OnExit()
{
    State::OnExit();
}

BOOL
RetreatState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    // 목적지를 찾았으면
    if (m_bFindDestPos)
    {
        // 이동이 끝날 때까지 기다린다.
        if (!m_pNPC->IsMoving())
        {
            if (m_bIdleAfterRetreat == false && m_pNPC->GetTargetChar())
                m_pNPC->ChangeState(STATE_ID_ATTACK);
            else
                m_pNPC->ChangeState(STATE_ID_IDLE);
        }
    }
    // 못 찾았으면 다시 시도한다.
    else
    {
        Retreat();
    }
    return true;
}

void
RetreatState::Retreat()
{
    // 타겟이 없으면 Idle상태로 돌아간다.
    Character* pTargetChar = m_pNPC->GetTargetChar();
    if (pTargetChar == NULL)
    {
        SUNLOG(eFULL_LOG, "[Retreat] m_pTargetChar is NULL!");
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return;
    }

    ++m_bTrialCount;

    const WzVector& vCurPos = *m_pNPC->GetPosPtr();
    const WzVector& vTargetPos = *pTargetChar->GetPosPtr();
    WzVector vNormal;
    VectorNormalize(&vNormal, &(vCurPos - vTargetPos));

    // 이동 거리를 구한다.
    float fMinDistance = m_fMaxDistance *
                         AIParamParser::AICommonInfo.m_fRETREAT_MIN_DISTANCE_LIMIT_RATIO;
    float fDistance = (float)dRandom(fMinDistance, m_fMaxDistance);

    // 이동해야할 방향의 30*m_bTrialCount도 안팍으로 랜덤한 위치를 얻어온다.
    int iHalfAngle = min(180, 30 * m_bTrialCount);
    int iRotateAngle = random(0, iHalfAngle * 2);

    if (iRotateAngle > iHalfAngle)
        iRotateAngle = 360 + iHalfAngle - iRotateAngle;

    WzVector vTrackPos;
    //                    in        out         in
    Math_ZRotateVector(&vNormal, vTrackPos, iRotateAngle);

    vTrackPos = vTargetPos + vTrackPos * fDistance;

    if (m_pNPC->MoveAndBroadcast(&vTrackPos, CMS_RUN))
    {
        m_bFindDestPos = true;
    }
}

void
RetreatState::OnMsgForceAttack(const AI_MSG* pMsg)
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

