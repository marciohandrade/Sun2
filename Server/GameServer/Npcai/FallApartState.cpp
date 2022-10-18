#include "stdafx.h"
#include <Timer.h>
#include "FallApartState.h"
#include "GameField.h"
#include <PacketStruct_CG.h>
#include <SinTable.h>
#include <3DEngine/3DTerrain/PathExplorer.h>
#include <math.h>
#include "BattleRecord.h"
#include "AIParamParser.h"


FallApartState::FallApartState()
{
    m_bFindDestPos = false;
}

FallApartState::~FallApartState()
{
}

void
FallApartState::Init()
{
}

void
FallApartState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    m_bFindDestPos = false;

    State::OnEnter(param1, param2, param3);
}

void
FallApartState::OnExit()
{
    State::OnExit();
}

BOOL
FallApartState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    // 목적지를 찾았으면
    if (m_bFindDestPos)
    {
        // 이동이 끝날 때까지 기다린다.
        if (!m_pNPC->IsMoving())
        {
            if (m_pNPC->GetTargetChar())
            {
                m_pNPC->ChangeState(STATE_ID_ATTACK);
            }
            else
            {
                m_pNPC->ChangeState(STATE_ID_IDLE);
            }
        }
    }
    // 못 찾았으면 다시 시도한다.
    else
    {
        FallApart();
    }
    return true;
}

void
FallApartState::FallApart()
{
    // 타겟이 없으면 Idle상태로 돌아간다.
    Character* pTargetChar = m_pNPC->GetTargetChar();
    if (pTargetChar == NULL)
    {
        SUNLOG(eFULL_LOG, "[FallApart] m_pTargetChar is NULL!");
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return;
    }

    const WzVector& vTargetPos = *pTargetChar->GetPosPtr();

    int iAngle = rand() % 360;
    float fDistance = (float)dRandom(AIParamParser::AICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT * 1.1f,
                                     AIParamParser::AICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT * 1.5f);
    WzVector vMovement;
    {
        vMovement.x = (float)(fDistance * g_SinTable.FCOS(iAngle));
        vMovement.y = (float)(fDistance * g_SinTable.FSIN(iAngle));
        vMovement.z = 0.0f;
    }

    WzVector& vTrackPos = vTargetPos + vMovement;

    if (m_pNPC->MoveAndBroadcast(&vTrackPos, CMS_RUN))
    {
        m_bFindDestPos = true;
    }
}

void
FallApartState::OnMsgForceAttack(const AI_MSG* pMsg)
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

