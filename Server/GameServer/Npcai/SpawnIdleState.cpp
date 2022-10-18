#include "stdafx.h"
#include "SpawnIdleState.h"
#include "GameField.h"
#include "BattleRecord.h"
#include <Timer.h>
#include "AIParamParser.h"
#include "Monster.h"
#include "PublicMath.h"
#include "TargetManager.h"
#ifdef _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE
#include "../MovingPlanner.h"
#include "StatusManager.h"
#endif //_NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE

SpawnIdleState::SpawnIdleState()
{
}

SpawnIdleState::~SpawnIdleState()
{
}

void
SpawnIdleState::Init()
{
}

void
SpawnIdleState::SetNPC(NPC* pNpc)
{
    m_pMonster = DOWNCASTER_SELECT<Monster*>(pNpc);
    if (m_pMonster == NULL || pNpc->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void
SpawnIdleState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    // 스폰위치로 가지 못했다면 돌아간다.
    const WzVector& vRegenPos = m_pMonster->GetRegenPos();
    const WzVector& vCurPos = *m_pMonster->GetPosPtr();

    float fDistance2 = Math_Vector2DLength2(&(vRegenPos - vCurPos));
    if ((fDistance2 > AIParamParser::AICommonInfo.m_fMIN_MOVABLE_DISTANCE *
                    AIParamParser::AICommonInfo.m_fMIN_MOVABLE_DISTANCE) &&
         m_pMonster->GetField() != NULL)
    {
        m_pMonster->ChangeState(STATE_ID_RETURN, CMS_RUN);
        return;
    }

    m_SearchTimer.SetTimer(m_SearchTimer.eStaticTimer_Search, 0);
}

void
SpawnIdleState::OnExit()
{
    State::OnExit();
}

BOOL
SpawnIdleState::OnUpdate(DWORD dwDeltaTick)
{
    if (m_pMonster->ISGMIdleOn())
        return true;

    __UNUSED(dwDeltaTick);

#ifdef _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE
    //자신의 위치가 안전지대 타일이면 원위치로 돌아간다.
    const nsAI::TileEvent* tile_event = 0;
    if (m_pNPC->GetStatusManager()->CanMove() && 
        (m_pNPC->PlanEvent->GetTileEvent(&tile_event) == false || (tile_event->Attr & PTA_SAFETY_ZONE)))
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_RETURN);
        
        return true;
    }
#endif //_NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE

    if (m_SearchTimer.IsExpired())
    {
        Character* pTargetChar = m_pMonster->SearchTarget();
        if (pTargetChar == NULL)
            return true;

        // 그룹의 일원이면 멤버들에게 적 발견 메세지를 보낸다.
        if (m_pMonster->IsGroupMember())
        {
            AI_MSG_ENEMY_FOUND aiMsg;
            aiMsg.dwTargetObjectKey = pTargetChar->GetObjectKey();
            m_pMonster->SendAIMessageToGroupExceptMe(&aiMsg, sizeof(aiMsg));
        }

        // 타겟을 변경하고 상태를 전환한다.
        m_pMonster->SetTargetChar(pTargetChar);
        m_pMonster->ChangeState(STATE_ID_TRACK);
    }
    return true;
}

void
SpawnIdleState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pMonster->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 현재 선택된 타겟이 없고 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (NULL == m_pMonster->GetTargetChar() && 
        USER_RELATION_ENEMY == m_pMonster->IsFriend(pAttacker))
    {
        m_pMonster->SetTargetChar(pAttacker);
        m_pMonster->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

}

