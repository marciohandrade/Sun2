#include "stdafx.h"

#include <Timer.h>
#include <SinTable.h>
#include <AIParamParser.h>

#include "WanderState.h"
#include "GameField.h"
#include "Monster.h"
#include "StatusManager.h"

#ifdef _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE
#include "../MovingPlanner.h"
#endif //_NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE

WanderState::WanderState()
{
}

WanderState::~WanderState()
{
}

void
WanderState::Init()
{   
}

void
WanderState::SetNPC(NPC* pNpc)
{
    m_pMonster = DOWNCASTER_SELECT<Monster*>(pNpc);
    if (m_pMonster == NULL || pNpc->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void
WanderState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_bDestSelected     = false;
    m_bTouchedSomething = false;

    //m_SearchTimer.SetTimer(m_pAITypeInfo->m_wSEARCH_PERIOD);
    m_SearchTimer.SetTimer(m_SearchTimer.eStaticTimer_Search, 5000);
}

void
WanderState::OnExit()
{
    State::OnExit();
}

BOOL
WanderState::OnUpdate(DWORD dwDeltaTick)
{
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

    if (m_SearchTimer.IsExpired() && m_pMonster->CanAttack())
    {
        if (!m_pNPC->GetStatusManager()->Status.IsPassConstraint(m_pNPC, (SkillScriptInfo*)0))
            return true;
        // 주변에 적이 있는지 탐색
        Character* pTargetChar = m_pMonster->SearchTarget();
        if (pTargetChar)
        {
            // 그룹의 일원이면 멤버들에게 적 발견 메세지를 보낸다.
            if (m_pMonster->IsGroupMember())
            {
                AI_MSG_ENEMY_FOUND aiMsg;
                aiMsg.dwTargetObjectKey = pTargetChar->GetObjectKey();
                m_pMonster->SendAIMessageToGroupExceptMe(&aiMsg, sizeof(aiMsg));
            }

            m_pMonster->SetTargetChar(pTargetChar);
            m_pMonster->ChangeState(STATE_ID_TRACK);
            return true;
        }

        // 목적지가 없는 상태이면 목적지 선택
        if (m_bDestSelected == false) {
            // (CHANGES) (091231) (WAVERIX) if mob had group, change policy
        #if WAVERIX_CODE_RENEWAL
            BOOLEAN member_state = !!m_pNPC->IsGroupMember();
            BOOLEAN leader_state = false;
            if (member_state) {
                leader_state = !!m_pNPC->IsLeader();
                if (leader_state == false)
                {
                    NPC* leader = m_pNPC->GetLeaderInField();
                    if (leader == NULL) { // leader 현재 필드상에 없다.
                        member_state = false; // change to normal mob state logic
                    }
                }
            }
            // member_state | leader_state
            //       0              0       - wander
            //       0              1       - x
            //       1              0       - follower : return or wander by command
            //       1              1       - wander
            if (!(member_state ^ leader_state)) { // NXOR
                m_bDestSelected = (DoWander() != false);
            }
        #else
            if (m_pMonster->IsLeader() || !m_pMonster->IsLeaderAlive())
            //if (m_pMonster->GetGroupState().IsLeader() || !m_pMonster->GetGroupState().IsLeaderAlive())
            {
                m_bDestSelected = DoWander();
            }
        #endif
        }
    }

    // 목적지에 도달한 상태인가
    if (m_bDestSelected && m_pMonster->IsMoving() == false)
    {
        m_bDestSelected = false;

        // 장애물에 걸려서 조금밖에 이동을 못한 상태가 아니라면 IDLE로 전환한다.
        if (!m_bTouchedSomething)
        {
            m_pMonster->ChangeState(STATE_ID_IDLE);
        }
    }
    return true;
}

inline void
WanderState::_UpdateTouchedSomething(BOOL bSet)
{
    BOOL bSrc = m_bTouchedSomething;
    if ((!!bSet) != (!!bSrc))
        m_bTouchedSomething = !bSet;
}

// 목적지 선택에 성공하면 true를 리턴
BOOL
WanderState::DoWander()
{
    // 이동할 거리를 구한다.
    float MOVE_DISTANCE = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                         AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

    const WzVector& wvCurPos = *m_pMonster->GetPosPtr();

    GameField* pField = m_pMonster->GetField();
    if (pField == NULL)
        return false;

    WzVector wvMovement;
    // 목적지 좌표를 찾는다.
    if (m_bTouchedSomething)
    {
        // 이전 길찾기에서 무언가에 닿은 경우 49도 회전시키고
        m_bTouchedSomething = false;
        wvMovement = m_wvRotatedVector; 
    }
    else if (m_pMonster->IsOutOfWanderRadius())
    {
        // 방황 범위를 벗어났으면 안쪽으로 이동
        WzVector direction = m_pMonster->GetRegenPos();
        ;        direction -= wvCurPos;
        VectorNormalize(&wvMovement, &direction);
        wvMovement *= MOVE_DISTANCE;
    }
    else
    {
        // 랜덤한 방향으로 포인팅
        int iAngle = State::GetStateRandomNo() % 360;
        wvMovement.x = (float)(MOVE_DISTANCE * g_SinTable.FCOS(iAngle));
        wvMovement.y = (float)(MOVE_DISTANCE * g_SinTable.FSIN(iAngle));
        wvMovement.z = 0.0f;
    }

    WzVector wvDestPos = wvCurPos;
    ;        wvDestPos += wvMovement;

    // 이동가능 여부를 체크후 패킷을 날린다.
    BOOL bRef = !!m_bTouchedSomething;
    if (!m_pMonster->ThrustMoveAndBroadcast(&wvDestPos, CMS_WALK, bRef, m_wvRotatedVector))
    {
        _UpdateTouchedSomething(bRef);
        return false;
    }
    _UpdateTouchedSomething(bRef);

    // 그룹의 리더이면 그룹 쫄따구들한테 AI 메세지를 보낸다.
    if (m_pMonster->IsLeader())
        //if (m_pMonster->GetGroupState().IsLeader())
    {
        AI_MSG_LETSGO letsgoMsg;
        letsgoMsg.wvDestPos = wvDestPos;
        letsgoMsg.byMoveState = CMS_WALK;

        m_pMonster->SendAIMessageToGroupExceptMe(&letsgoMsg, sizeof(letsgoMsg),
            (DWORD)dRandom(AIParamParser::AICommonInfo.m_wGROUP_AIMESSAGE_MIN_DELAY,
                           AIParamParser::AICommonInfo.m_wGROUP_AIMESSAGE_MAX_DELAY));
    }

    return true;
}

void
WanderState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pMonster->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (m_pMonster->IsFriend(pAttacker) == USER_RELATION_ENEMY)
    {
        m_pMonster->SetTargetChar(pAttacker);
        m_pMonster->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

void
WanderState::OnMsgLetsGo(const AI_MSG* pMsg)
{
    State::OnMsgLetsGo(pMsg);
    m_bDestSelected = true;
}

