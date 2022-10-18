#include "stdafx.h"
#include "State.h"

//==================================================================================================

#include <3DEngine/3DTerrain/PathExplorer.h>
//==================================================================================================
#include <SinTable.h>
#include <PublicMath.h>
#include <AIParamParser.h>
//==================================================================================================
#include "GameField.h"
#include "Player.h"
#include "PlayerManager.h"
#include "TargetFinder.h"
#include "TargetManager.h"
#include "NPCStateManager.h"
#include "GameServerEx.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Summoned.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

//==================================================================================================
//
DWORD
State::ms_increment_random = 0;

// (CHANGES) (091222) (WAVERIX) 'SetNPC' moved to code-file section which is appended 'AI type info'
void
State::SetNPC(NPC* pNPC) {
    m_pNPC = pNPC;

    const BASE_NPCINFO* pNpcInfo = m_pNPC->GetBaseInfo();
    m_pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(pNpcInfo->m_wAICode);
    if (m_pAITypeInfo == NULL)
        __debugbreak();

    m_StateID = STATE_ID_UNKNOWN;
    m_LinkStateID = STATE_ID_UNKNOWN;
}

/*
void
State::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    __UNUSED(param1);
    __UNUSED(param2);
    __UNUSED(param3);
}

void
State::OnExit()
{
}
*/

void
State::OnAIMsg(const AI_MSG* pMsg)
{
    switch (pMsg->dwMsgID)
    {
    case AI_MSG_ID_FORCE_ATTACK:
        OnMsgForceAttack(pMsg);
        break;

    case AI_MSG_ID_ATTACKED:
        OnMsgAttacked(pMsg);
        break;

    case AI_MSG_ID_HELP_REQUEST:
        OnMsgHelpRequest(pMsg);
        break;

    case AI_MSG_ID_LEAVE_FIELD:
        OnMsgLeaveField(pMsg);
        break;

    case AI_MSG_ID_THRUST:
        OnMsgThrust(pMsg);
        break;

    case AI_MSG_ID_GROUP_C0MMAND:
        OnMsgGroupCommand(pMsg);

    case AI_MSG_ID_FLYING:
        OnMsgFlying(pMsg);
        break;

    case AI_MSG_ID_KNOCKDOWN:
        OnMsgKnockDown(pMsg);
        break;

    case AI_MSG_ID_STUN:
        OnMsgStun(pMsg);
        break;

    case AI_MSG_ID_LETSGO:
        OnMsgLetsGo(pMsg);
        break;

    case AI_MSG_ID_CHANGE_RETURNSTATE:
        OnMsgChangeToReturnState(pMsg);
        break;

    case AI_MSG_ID_CHANGE_RETREATSTATE:
        OnMsgChangeToRetreatState(pMsg);
        break;

    case AI_MSG_ID_ENEMY_FOUND:
        OnMsgEnemyFound(pMsg);
        break;

    case AI_MSG_ID_RUNAWAY:
        OnMsgRunAway(pMsg);
        break;

    case AI_MSG_ID_CHAOS:
        OnMsgChaos(pMsg);
        break;

    case AI_MSG_ID_COMMAND_FOLLOW:
        OnMsgCommandFollow(pMsg);
        break;
    case AI_MSG_ID_USE_SKILL:
        OnMsgUseSkill(pMsg);
        break;

    case AI_MSG_ID_COMMAND_NONPVP:
        OnMsgNONPVP(pMsg);
        break;
    case AI_MSG_ID_BLIND:
        OnMsgBlind(pMsg);
        break;
    default:
        SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"] AIMsgID doesn't exist! ID:(%u)", pMsg->dwMsgID);
        break;
    }
}

void
State::OnMsgForceAttack(const AI_MSG* pMsg)
{
    const AI_MSG_FORCE_ATTACK* pAttackMsg = static_cast<const AI_MSG_FORCE_ATTACK*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == 0)
        return;

    // 공격대상을 찾는다.
    Character* pTarget = pField->FindCharacter(pAttackMsg->dwTargetKey);
    if (pTarget == 0)
        return;

    // 상대가 적군이면 공격 상태로 전이
    if (m_pNPC->IsFriend(pTarget) == USER_RELATION_ENEMY)
    {
        /*m_pNPC->SetTargetChar(pTarget);*/
        m_pNPC->SelectSkill(pTarget, SKILLCODE_INVALID );
        m_pNPC->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);

        // 그룹원이면 그룹 멤버들에게 적 발견 메세지 보냄
        if (m_pNPC->IsGroupMember())
        {
            AI_MSG_ENEMY_FOUND aiMsg;
            aiMsg.dwTargetObjectKey = pAttackMsg->dwTargetKey;
            m_pNPC->SendAIMessageToGroupExceptMe(&aiMsg, sizeof(aiMsg));
        }
    }
}

void
State::OnMsgAttacked(const AI_MSG* pMsg)
{
    const AI_MSG_ATTACKED* pAttackedMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == 0)
        return;

    if (m_pNPC->GetBaseInfo()->m_SpecialCondition[0].wActionType == 1)
    {
        NPCStateManager* state_manager = m_pNPC->GetStateManager();
        if (state_manager->IsRequestHelp())
            return;

        // (CHANGES) (WAVERIX) (081106) logic-change for debug
        const double curHPf = m_pNPC->GetHP();
        const DWORD maxHP = m_pNPC->GetMaxHP();
        if (curHPf <= (maxHP * m_pAITypeInfo->m_fHELP_REQUEST_HP_PERCENT))
            //if (m_pNPC->GetHP() <= m_pNPC->GetMaxHP() * pAITypeInfo->m_fHELP_REQUEST_HP_PERCENT)
        {
            AI_MSG_HELP_REQUEST help_msg;
            help_msg.dwAttackerKey = pAttackedMsg->dwAttackerKey;
            help_msg.dwTargetKey = m_pNPC->GetObjectKey();

            m_pNPC->SendAIMessageAroundExceptMe(&help_msg, sizeof(help_msg));

            state_manager->SetRequestHelp(true);
        }
    }
}

void
State::OnMsgHelpRequest(const AI_MSG* pMsg)
{
    const AI_MSG_HELP_REQUEST* pHelpMsg = static_cast<const AI_MSG_HELP_REQUEST*>(pMsg);

    GameField* const pField = m_pNPC->GetField();
    if (pField == 0)
        return;

    // added a rule by __NA000000_090327_MONSTER_HELP_FILTER__
    const eNPC_GRADE npcGrade = eNPC_GRADE(m_pNPC->GetBaseInfo()->m_byGrade);
    if (npcGrade == eNPC_BOSS ||
       npcGrade == eNPC_LUCKY_MONSTER)
    {
        return;
    }

    Character* const pAttacker = pField->FindCharacter(pHelpMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // (BUGFIX) (WAVERIX) (081106) (WOPS:2741)
    const eUSER_RELATION_TYPE relation = m_pNPC->IsFriend(pAttacker);
    if (relation != USER_RELATION_ENEMY)
        return;

    // 플레이어가 이미 여러NPC로부터 공격을 받고 있으면 Help요청에 지원을 가지 않는다.
    ObjectList& observers = pAttacker->GetObservers();
    if (observers.GetSize() >= (size_t)AIParamParser::AICommonInfo.m_byMAX_OBSERVERS_PER_PLAYER)
    {
        return;
    }

    // 공격자가 적일 경우에만 지원을 간다.
    // (deleted)

    // Help 요청에 응답하는 속성이 있을 경우에만 지원을 간다.
    const float distance_with_target = m_pNPC->GetDistToNewTarget(pAttacker);
    const float sight_range = m_pNPC->GetSightRange();
    if (distance_with_target <= (sight_range * AIParamParser::AICommonInfo.m_fHELP_SIGHTRANGE_RATIO))
    {
        m_pNPC->SetTargetChar(pAttacker);
        m_pNPC->ChangeState(STATE_ID_HELP, 0, 0, pHelpMsg->dwMsgID);
    }
}

void
State::OnMsgLeaveField(const AI_MSG* pMsg)
{
    const AI_MSG_LEAVE_FIELD* pLeaveMsg = static_cast<const AI_MSG_LEAVE_FIELD*>(pMsg);

    // 현재 타겟이 나가는 경우
    Character* pTarget = m_pNPC->GetTargetChar();
    if (pTarget && pTarget->GetObjectKey() == pLeaveMsg->dwObjectKey)
    {
        m_pNPC->ChangeState(STATE_ID_IDLE, 0, 0, pLeaveMsg->dwMsgID);
    }

    m_pNPC->GetTargetManager()->RemoveTarget(pLeaveMsg->dwObjectKey);
}

void
State::OnMsgThrust(const AI_MSG* pMsg)
{
    // Thrust 상태가 끝난 후에 다운을 시켜줄 것인가를 THRUST 상태의 첫번째 파라미터로 전달
    const AI_MSG_THRUST* pRecvMsg = static_cast<const AI_MSG_THRUST *>(pMsg); 

    LPARAM arg2nd = pRecvMsg->bDownAfterThrust ? STATE_ID_KNOCKDOWN : 0;
    m_pNPC->ChangeState(STATE_ID_THRUST, arg2nd, 0, pRecvMsg->dwMsgID);
}

void
State::OnMsgFlying(const AI_MSG* pMsg)
{
    const AI_MSG_FLYING* pRecvMsg = static_cast<const AI_MSG_FLYING *>(pMsg);

    m_pNPC->ChangeState(STATE_ID_FLYING, (LPARAM)pRecvMsg->dwFlyingTick, 0, pRecvMsg->dwMsgID);
}

void
State::OnMsgKnockDown(const AI_MSG* pMsg)
{
    const AI_MSG_KNOCKDOWN* pRecvMsg = static_cast<const AI_MSG_KNOCKDOWN*>(pMsg);

    m_pNPC->ChangeState(STATE_ID_KNOCKDOWN, (LPARAM)pRecvMsg->dwKnockDownTick, 0, pRecvMsg->dwMsgID);
}

void
State::OnMsgStun(const AI_MSG* pMsg)
{
    const AI_MSG_STUN* pRecvMsg = static_cast<const AI_MSG_STUN*>(pMsg);

    m_pNPC->ChangeState(STATE_ID_KNOCKDOWN, (LPARAM)pRecvMsg->dwStunTick, 0, pRecvMsg->dwMsgID);
}

void
State::OnMsgGroupCommand(const AI_MSG* pMsg)
{
    const AI_MSG_GROUP_C0MMAND* pGroupCmdMsg = static_cast<const AI_MSG_GROUP_C0MMAND*>(pMsg);

    switch (pGroupCmdMsg->byType)
    {
    case GROUP_CMD_TYPE_ATTACK:
        OnMsgGroupCommand_Attack(pGroupCmdMsg->dwTargetKey);
        break;
    case GROUP_CMD_TYPE_STOP_ATTACK:
        OnMsgGroupCommand_StopAttack();
        break;
    default:
        SUNLOG(eMIDDLE_LOG, "[State::OnMsgGroupCommand] Wrong GroupCommand Type! ");
    }
}

void
State::OnMsgGroupCommand_Attack(DWORD dwTargetKey)
{
    GameField* pField = m_pNPC->GetField();
    if (pField == 0)
        return;

    Character* pTargetChar = pField->FindCharacter(dwTargetKey);
    if (pTargetChar == NULL)
        return;

    m_pNPC->SetTargetChar(pTargetChar);
    m_pNPC->ChangeState(STATE_ID_HELP, 0, 0, AI_MSG_ID_GROUP_C0MMAND);
}

void
State::OnMsgGroupCommand_StopAttack()
{
    m_pNPC->SetTargetChar(NULL);
    m_pNPC->ChangeState(STATE_ID_IDLE, 0, 0, AI_MSG_ID_GROUP_C0MMAND);
}

void
State::OnMsgLetsGo(const AI_MSG* pMsg)
{
    const AI_MSG_LETSGO* pRecvMsg = static_cast<const AI_MSG_LETSGO*>(pMsg);

    const WzVector& wvCurPos = *m_pNPC->GetPosPtr();
    WzVector wvCommanderDest = pRecvMsg->wvDestPos;

    // 리더랑 동일 좌표로 안가기 위해서 근처좌표를 다시 찾는다.
    float fMoveLength = (float)dRandom(AIParamParser::AICommonInfo.m_fGROUP_FOLLOWER_MIN_RADIUS,
                                       AIParamParser::AICommonInfo.m_fGROUP_FOLLOWER_MAX_RADIUS);
    int iAngle = rand() % 360;

    WzVector wvRandomDistance;
    { // init
        wvRandomDistance.x = (float)(fMoveLength * g_SinTable.FCOS(iAngle));
        wvRandomDistance.y = (float)(fMoveLength * g_SinTable.FSIN(iAngle));
        wvRandomDistance.z = 0.0f;
    };

    WzVector& wvDestPos = wvCommanderDest + wvRandomDistance;

    const eCHAR_MOVE_STATE move_state = (eCHAR_MOVE_STATE)pRecvMsg->byMoveState;
    // 길찾기
#ifdef _SUSUNLOVE_TEST_CODE_
    BOOL bTouchedSomething; WzVector wvRotatedVector;
    if (m_pNPC->ThrustMoveAndBroadcast(&wvDestPos, move_state, bTouchedSomething, wvRotatedVector))
        return;

    // 실패시 리더와 동일 좌표로 다시 길찾기
    m_pNPC->ThrustMoveAndBroadcast(&wvCommanderDest, move_state, bTouchedSomething, wvRotatedVector);
#else
    if (m_pNPC->MoveAndBroadcast(&wvDestPos, move_state))
        return;

    // 실패시 리더와 동일 좌표로 다시 길찾기
    m_pNPC->MoveAndBroadcast(&wvCommanderDest, move_state);
#endif
}

void
State::OnMsgChangeToReturnState(const AI_MSG* pMsg)
{
    const AI_MSG_CHANGETORETURNSTATE* pRecvMsg = static_cast<const AI_MSG_CHANGETORETURNSTATE*>(pMsg);

    m_pNPC->ChangeState(STATE_ID_RETURN, 0, 0, pRecvMsg->dwMsgID);
    m_pNPC->SetTargetChar(NULL);
}

void
State::OnMsgEnemyFound(const AI_MSG* pMsg)
{
    const AI_MSG_ENEMY_FOUND* pRecvMsg = static_cast<const AI_MSG_ENEMY_FOUND*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == 0)
        return;

    Character* pTarget = pField->FindCharacter(pRecvMsg->dwTargetObjectKey);
    if (pTarget == 0)
        return;

    // 현재 선택된 타겟이 없고 상대가 적군이면 공격 상태로 전이

    if (m_pNPC->GetTargetChar() == NULL &&
       m_pNPC->IsFriend(pTarget) == USER_RELATION_ENEMY)
    {
        m_pNPC->SetTargetChar(pTarget);
        m_pNPC->ChangeState(STATE_ID_TRACK, 0, 0, pRecvMsg->dwMsgID);
    }
}

void
State::OnMsgRunAway(const AI_MSG* pMsg)
{
    const AI_MSG_RUNAWAY* pRecvMsg = static_cast<const AI_MSG_RUNAWAY*>(pMsg);

    // (CHANGES) (091223) remove __NA_S00015_20080828_NEW_HOPE_SHADOW
    m_pNPC->ChangeState(STATE_ID_RUNAWAY,
                        pRecvMsg->dwTargetKey,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        reinterpret_cast<LPARAM>(pRecvMsg),
#else  // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        pRecvMsg->dwRunAwayTime,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        pRecvMsg->wStateID); // change 'MsgID' -> 'StateID'
}

void
State::OnMsgChaos(const AI_MSG* pMsg)
{
    const AI_MSG_CHAOS* pRecvMsg = static_cast<const AI_MSG_CHAOS*>(pMsg);

    m_pNPC->ChangeState(STATE_ID_CHAOS, pRecvMsg->dwChaosTime, 0, pRecvMsg->dwMsgID);
}

void
State::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    const AI_MSG_COMMAND_FOLLOW* pRecvMsg = static_cast<const AI_MSG_COMMAND_FOLLOW*>(pMsg);

    // 각종 이상상태(다운, 밀리기, 혼란 등등)가 아니라면 IDLE 상태로 복귀한다.
    m_pNPC->SetTargetChar(NULL);
    m_pNPC->ChangeState(STATE_ID_IDLE, 0, 0, pRecvMsg->dwMsgID);
}

void
State::OnMsgUseSkill(const AI_MSG* pMsg)
{
    const AI_MSG_USE_SKILL* pSkillMsg = static_cast<const AI_MSG_USE_SKILL*>(pMsg);

    GameField* pField = m_pNPC->GetField();
    if (pField == 0)
        return;

    // 공격대상을 찾는다.
    Character* pTarget = pField->FindCharacter(pSkillMsg->dwTargetKey);
    if (pTarget == 0)
        return;

    if ((pTarget->CanBeAttacked() == false) && (m_pNPC->UseResurrectionSkill() == true))
        return;
    // 상대가 적군이면 공격 상태로 전이
    //  if (!m_pNPC->IsFriend(pTarget))
    {
        m_pNPC->SelectSkill(pTarget, pSkillMsg->dwSkillCode
#ifdef _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
            , pSkillMsg->dwSelectedSkillDelay
#endif // _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
            );

        //TrackState -> TrackState 상태전환을 방지한다.
        if (m_pNPC->GetCurStateID() == STATE_ID_TRACK)
            return;
        m_pNPC->ChangeState(STATE_ID_TRACK, 0, 0, pSkillMsg->dwMsgID);
    }
}

void
State::OnMsgNONPVP(const AI_MSG* pMsg)
{
    const AI_MSG_COMMAND_NONPVP* pRecvMsg = static_cast<const AI_MSG_COMMAND_NONPVP*>(pMsg);

    Character* pCharacter = m_pNPC->GetTargetChar();

    if (pCharacter && pCharacter->GetPlayerOwner())
    {
        pCharacter->SetTargetChar(NULL);
        pCharacter->ChangeState(STATE_ID_IDLE, 0, 0, pRecvMsg->dwMsgID);
    }
}

void
State::OnMsgBlind(const AI_MSG* pMsg)
{
    const AI_MSG_BLIND* pRecvMsg = static_cast<const AI_MSG_BLIND*>(pMsg);

    m_pNPC->SetTargetChar(NULL);
    m_pNPC->ChangeState(STATE_ID_BLIND, pRecvMsg->dwBlindTime, 0, pRecvMsg->dwMsgID);
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
BOOL State::SetTarget( const AI_MSG* pMsg, ENUM_STATD_ID next_state /*= 0*/ )
{
    const AI_MSG_ATTACKED* attack_msg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* field = m_pNPC->GetField();
    if (field == NULL) {
        return FALSE;
    }
    Character* target = field->FindCharacter(attack_msg->dwAttackerKey);
    if (target == NULL) {
        return FALSE;
    }

    // 상대가 적군이면 공격 상태로 전이

    //타겟이 고정형 소환수일 경우, 그 주인으로 타겟을 변경한다
    if (m_pNPC->GetTargetChar() == NULL &&
        m_pNPC->IsFriend(target) == USER_RELATION_ENEMY)
    {
        if (target->IsEqualObjectKind(SUMMON_OBJECT) == true)
        {
            Summoned* summoned = reinterpret_cast<Summoned*>(target);
            if (summoned != NULL && summoned->GetSummonType() == eSUMMON_IMMOVABLE)
            {
                target = summoned->GetSummoner();
            }
        }

        m_pNPC->SetTargetChar(target);
        if (next_state != STATE_ID_UNKNOWN) {
            m_pNPC->ChangeState(next_state, 0, 0, attack_msg->dwMsgID);
        }
        return TRUE;
    }
    return FALSE;
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
