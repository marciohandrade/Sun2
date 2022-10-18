#include "stdafx.h"
#include "DeadState.h"
#include "GameField.h"
#include "AIParamParser.h"
#include "TargetManager.h"

DeadState::DeadState()
{
}

DeadState::~DeadState()
{
}

void
DeadState::Init()
{
    __noop;
}

void
DeadState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);
    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE 추가에 따른 상태 반영
    const sPOLICY_CHARACTER& kCharPolicy = m_pNPC->Policyment; __TOUCHED(kCharPolicy);
    m_pNPC->ChangePolicyPostEnterField(kCharPolicy.PCC_CONTINUING_NPC_UPDATE,
                                       kCharPolicy.PCC_DISABLE_ALL,
                                       NULL);
    // CHANGES: f110720.2L, remove assertion check. a MapObject npc don't have no dead interval.
    //ASSERT(param1);
    DWORD interval = static_cast<DWORD>(param1);
    m_DeadTimer.SetTimer(interval);
}

void
DeadState::OnExit()
{
    State::OnExit();

    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE 추가에 따른 상태 반영
    // CHANGES: f110228.1L, remove deletion state to prevent skip update routine if a monster is dead
}

BOOL
DeadState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);
    //  (WAVERIX)(080923)(NOTE) 현재로서는 구조가 좋지 않다. 일단 기존 코드에 묻어가는 식의 땜빵 처리로 변경한다.

    const BOOLEAN isExpired = m_DeadTimer.IsExpired();

    if (isExpired)
    {
        GameField* const pField = m_pNPC->GetField();
        if (!pField)
        {
            SUNLOG(eCRITICAL_LOG, "[DeadState::OnUpdate] GameField is NULL");
            m_pNPC->ChangeState(STATE_ID_DESTROY_OBJECT);
            return true;
        }

        const BOOLEAN exist_trans   = !!m_pNPC->IsSpecialAction(eNPC_SPECIAL_ACTION_TRANSFORMATION);
        const BOOLEAN exist_selfdest= !exist_trans && 
                                      m_pNPC->IsSpecialAction(eNPC_SPECIAL_ACTION_SELP_DESTRUCTION);

        if (exist_trans ^ exist_selfdest)
        {
            SPECIAL_CONDITION* special_condition = m_pNPC->GetDrawLotsSpecialAction();
            if (special_condition)
            {
                m_pNPC->ChangeState(STATE_ID_SPECIAL_DEAD_ACTION, 
                                    reinterpret_cast<LPARAM>(special_condition));
                return true;
            }
        }

        m_pNPC->ChangeState(STATE_ID_DESTROY_OBJECT);
    }

    return true;
}

void
DeadState::OnAIMsg(const AI_MSG* pMsg)
{
    switch (pMsg->dwMsgID)
    {
    case AI_MSG_ID_LEAVE_FIELD:
        OnMsgLeaveField(pMsg);
        break;
    }
}

void
DeadState::OnMsgLeaveField(const AI_MSG* pMsg)
{
    const AI_MSG_LEAVE_FIELD* pLeaveMsg = static_cast<const AI_MSG_LEAVE_FIELD*>(pMsg);

    m_pNPC->GetTargetManager()->RemoveTarget(pLeaveMsg->dwObjectKey);
}

