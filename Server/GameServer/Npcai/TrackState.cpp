#include "stdafx.h"
#include "TrackState.h"

#include "PublicMath.h"
#include "AIParamParser.h"

#include "GameField.h"
#include "FieldInfo.h"
#include "TargetManager.h"
#include "Skill.h"
#include "../MovingPlanner.h"
#ifdef _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE
#include "StatusManager.h"
#endif //_NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE

TrackState::TrackState()
{
}

TrackState::~TrackState()
{
}

void
TrackState::Init()
{
}

void
TrackState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3); 

    //m_TrackTimer.SetTimer(m_pAITypeInfo->m_wTRACK_PERIOD);
    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
    m_ReturnCheckTimer.SetTimer(m_ReturnCheckTimer.eStaticTimer_ReturnCheck, 0);

    m_JumpRatio = m_pNPC->GetBaseInfo()->m_byJumpRatio;
    m_bJumpTry = false;
    m_TrackCount = 0;

    // 공격 방법 선택되어 있지 않으면 선택
    if (m_pNPC->GetSelectedSkill() == SKILLCODE_INVALID)
    {
        m_pNPC->SelectSkill();
    }
}

void
TrackState::OnExit()
{
    State::OnExit();
}

BOOL
TrackState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    Character* pTargetChar = m_pNPC->GetTargetChar();
    if (pTargetChar == NULL)
#ifdef _RU_006484_20130225_VALKYRIES_SUMMONS_BUG
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);

        return true;
    }
#else
        return true;
#endif // _RU_006484_20130225_VALKYRIES_SUMMONS_BUG

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

    WzVector vTargetPos = *pTargetChar->GetPosPtr();
    const float attack_range = m_pNPC->GetAttackRange();

    BOOLEAN is_movable_cur_tile = true;
    // (CHANGES) (091229) (WAVERIX) 자기 자신의 적절한 상태를 평가할 필요 있다.
    // 선행 조건으로 충분하므로 미리 계산해 둔다.
    // track->return:change regeneration pos
    // track->attack
    is_movable_cur_tile = !!nsAI::PlanEventObject::AcceptableTarget(m_pNPC, 0, 0);

    // (BUGFIX) (090617) (WAVERIX) 그룹 설정된 몬스터 중 leader가 아닌 몬스터의 복귀 처리
    //이뤄지지 않는 현상 수정

    // 자기 영역을 벗어나거나 타겟과의 거리가 멀어지면 리턴 상태로 전이
    BOOLEAN isReturnCheckTimeout = !!m_ReturnCheckTimer.IsExpired();
    if (isReturnCheckTimeout)
    {
        BOOL isOutOfRange = m_pNPC->IsOutOfRegenLocationLimit(vTargetPos);
        if (isOutOfRange)
        {
            // (WAVERIX) (090617) (NOTE) return AI policy state diagram
            // member_state | leader_state
            //       0              0       - return
            //       0              1       - x
            //       1              0       - follower : return by command
            //       1              1       - leader group commander : return

            BOOLEAN member_state = !!m_pNPC->IsGroupMember();
            BOOLEAN leader_state = 0;
            // 만약 그룹의 리더이면 쫄따구들한테도 메세지 보냄
            if (member_state)
            {
                if (leader_state = !!m_pNPC->IsLeader())
                {
                    // (CHANGES) (091231) (WAVERIX) return command by leader control
                    // moved to return_state
                    /*
                    AI_MSG_CHANGETORETURNSTATE returnMsg;
                    m_pNPC->SendAIMessageToGroupExceptMe(&returnMsg, sizeof(returnMsg),
                        (WORD)dRandom(AIParamParser::AICommonInfo.m_wGROUP_AIMESSAGE_MIN_DELAY,
                                      AIParamParser::AICommonInfo.m_wGROUP_AIMESSAGE_MAX_DELAY));
                    */
                }
                else
                {
                    NPC* pLeader = m_pNPC->GetLeaderInField();
                    if (pLeader == 0) { // leader 현재 필드상에 없다.
                        member_state = 0;
                    }
                    else {   // target과의 거리라... 이게 나을 수도 있겠네...
                        float length_sq = Math_VectorLength2(vTargetPos, *pLeader->GetPosPtr());
                        if (50.f * 50.f < length_sq)
                            member_state = 0;
                    }
                }
            }

            if ((member_state ^ leader_state) == 0)
            {
                m_pNPC->SetTargetChar(NULL);
                m_pNPC->ChangeState(STATE_ID_RETURN, CMS_RUN);
            }
            return true;
        }
    }

    // 타겟을 공격할 수 없는 상태이면 상태 전이
    if ((pTargetChar->CanBeAttacked() || m_pNPC->UseResurrectionSkill()) == false)
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return true;
    }

    // 타겟이 공격 사거리 내에 들어있으면 Attack 상태로 전이
    // (CHANGES) (091229) (WAVERIX)
    const float attack_range_sq = attack_range * attack_range;
    float distance_sq = m_pNPC->GetDist2(vTargetPos);
    if(distance_sq < attack_range_sq) { // changes '<=' to '<'
        m_pNPC->ChangeState(STATE_ID_ATTACK);
        return true;
    }

    // 점프속성이 있을 경우 일정 거리가 되면 점프한다.
    if (m_JumpRatio && m_bJumpTry == false &&
       m_pNPC->CanJumpDist(pTargetChar))
    {
        m_bJumpTry = true;

        // 확률 체크
        const int ratio = random(1, 100);
        if (ratio <= m_JumpRatio)
        {
            // 길찾기가 성공해야 점프상태로 전환한다.
            if (m_pNPC->SetJumpPos(pTargetChar))
            {
                m_pNPC->ChangeState(STATE_ID_JUMP, true);
                return true;
            }
        }
    }

    if (is_movable_cur_tile == false) {
        // (NOTE) 더 이상 이동 불가 상태로 계속 이동 상태 유지되는 상황을 방지한다.
        // |Current-state=선행 검사를 통해 다른 상태 전이를 유도 이후 추가 처리 상태|
        // |Action=changes to return-state|Side-effect=change regeneration position|
        // |Other=Group과 별개 처리로 수행한다. 별 수 없다.|
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_RETURN, CMS_RUN);
        return true;
    }

    if (m_pNPC->IsMoving())
    {
        // NPC가 이미 뛰고 있는 경우

        //-----------------------------------------------------------------------------------------
        // n: 현재 NPC 좌표
        // p: 현재 플레이어 좌표
        // d: NPC의 이동 목적 좌표
        // a: 벡터 n->d
        // b: 벡터 n->p
        // 라고 할 때 새로 길찾기 하는 조건은 다음과 같다.
        //
        // a와 b의 각이 일정 수치 이상 벌어지고 p와 d와의 거리가 NPC 공격 사거리를 벗어나는 경우
        //-----------------------------------------------------------------------------------------

        if (m_TrackTimer.IsExpired())
        {
            WzVector wvNPCPos = *m_pNPC->GetPosPtr();
            WzVector wvNPCDestPos = m_pNPC->GetPathExplorer()->GetTargetPos();

            // 플레이어가 NPC가 뛰려는 방향의 부채꼴을 벗어난 각에 위치하는가?
            BOOLEAN bOutOfAngle = !Math_IsPositionInFanshapedArea(&vTargetPos, &wvNPCPos,
                &(wvNPCDestPos - wvNPCPos), AIParamParser::AICommonInfo.m_wTRACK_INNER_ANGLE);

            // 플레이어가 NPC가 뛰려는 목적지에서 때릴 수 없는 거리에 있는가?
            float attack_range = m_pNPC->GetAttackRange();
            ;     attack_range *= attack_range;

            BOOLEAN bOutOfRange = VectorLength2(&(wvNPCDestPos - vTargetPos)) > attack_range;

            if (bOutOfAngle && bOutOfRange) {
                //  printf("이동중 트랙 ");
                DoTrack();
            }
        }
    }
    else
    {
        // NPC가 뛰고 있지 않은 경우
        //  printf("멈춰서 트랙 ");
        DoTrack();
    }

    return true;
}

void
TrackState::DoTrack()
{
    Character* pTargetChar = m_pNPC->GetTargetChar();
    ASSERT(NULL != pTargetChar);
    if (pTargetChar == NULL)
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_RETURN, CMS_RUN);
        return;
    }

    ++m_TrackCount;

    WzVector vTargetPos = *pTargetChar->GetPosPtr();
    WzVector wvCurPos = *m_pNPC->GetPosPtr();

    // 점프 타일 위에 서있으면 높이 판단후 강제 점프!
    if (m_pNPC->IsStandingOnJumpTile() && m_pNPC->IsOnGoodPositionToJump())
    {
        float fJumpWaitingTick = Math_VectorLength(vTargetPos, wvCurPos) * 60.0f;
        m_pNPC->ChangeState(STATE_ID_JUMP, false, (LPARAM)fJumpWaitingTick);
        return;
    }

    // 새로운 Track Position을 찾는다.
    // (NOTE) 아무래도 계산식이 위험해 보이는데... 0.1f * fAttackRangeRatio의 결과는?
    float fAttackRangeRatio = 1.0f - 0.1f * static_cast<float>(m_TrackCount);
    ;     fAttackRangeRatio = max(0, fAttackRangeRatio);

    WzVector vTrackPos;
    m_pNPC->GetTrackPos(vTrackPos, m_pNPC->GetAttackRange() * fAttackRangeRatio);

    // 타겟이 이동중이라면 이동 방향 감안해서 추적점 보정
    if (pTargetChar->IsMoving())
    {
        const WzVector& wvPlayerDestPos = pTargetChar->GetPathExplorer()->GetTargetPos();
        // 타겟 플레이어가 NPC의 반대쪽으로 이동하려는 경우에만 보정
        if (!Math_IsPositionInFanshapedArea(&wvCurPos, &vTargetPos,
                                           &(wvPlayerDestPos - vTargetPos),
                                           180))
        {
            WzVector wvNormal;
            VectorNormalize(&wvNormal, &(wvPlayerDestPos - vTargetPos));
            wvNormal = wvNormal * 2.0f;
            vTrackPos = vTrackPos + wvNormal;
        }
    }

    // 길찾기
    if (m_pNPC->CanMove() == false) {
        // arycoat, 2012.09.04
        // THRUST, STUN 등의 state로 잠시 이동이 불가 할때이다. 한 턴 쉬고 다시 추적한다.
    }
    else if (m_pNPC->MoveAndBroadcast(&vTrackPos, CMS_RUN) == false)
    {
        // 실패시 Target과 동일 좌표로 다시 길찾기
        if (m_pNPC->MoveAndBroadcast(&vTargetPos, CMS_RUN) == false)
        {
            // 그래도 실패하면 상태 전이
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_RETURN, CMS_RUN);
        }
    }

    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
}

void
TrackState::OnMsgForceAttack(const AI_MSG* pMsg)
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

