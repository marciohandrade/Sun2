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

    // ���� ��� ���õǾ� ���� ������ ����
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
    //�ڽ��� ��ġ�� �������� Ÿ���̸� ����ġ�� ���ư���.
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
    // (CHANGES) (091229) (WAVERIX) �ڱ� �ڽ��� ������ ���¸� ���� �ʿ� �ִ�.
    // ���� �������� ����ϹǷ� �̸� ����� �д�.
    // track->return:change regeneration pos
    // track->attack
    is_movable_cur_tile = !!nsAI::PlanEventObject::AcceptableTarget(m_pNPC, 0, 0);

    // (BUGFIX) (090617) (WAVERIX) �׷� ������ ���� �� leader�� �ƴ� ������ ���� ó��
    //�̷����� �ʴ� ���� ����

    // �ڱ� ������ ����ų� Ÿ�ٰ��� �Ÿ��� �־����� ���� ���·� ����
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
            // ���� �׷��� �����̸� �̵��������׵� �޼��� ����
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
                    if (pLeader == 0) { // leader ���� �ʵ�� ����.
                        member_state = 0;
                    }
                    else {   // target���� �Ÿ���... �̰� ���� ���� �ְڳ�...
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

    // Ÿ���� ������ �� ���� �����̸� ���� ����
    if ((pTargetChar->CanBeAttacked() || m_pNPC->UseResurrectionSkill()) == false)
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return true;
    }

    // Ÿ���� ���� ��Ÿ� ���� ��������� Attack ���·� ����
    // (CHANGES) (091229) (WAVERIX)
    const float attack_range_sq = attack_range * attack_range;
    float distance_sq = m_pNPC->GetDist2(vTargetPos);
    if(distance_sq < attack_range_sq) { // changes '<=' to '<'
        m_pNPC->ChangeState(STATE_ID_ATTACK);
        return true;
    }

    // �����Ӽ��� ���� ��� ���� �Ÿ��� �Ǹ� �����Ѵ�.
    if (m_JumpRatio && m_bJumpTry == false &&
       m_pNPC->CanJumpDist(pTargetChar))
    {
        m_bJumpTry = true;

        // Ȯ�� üũ
        const int ratio = random(1, 100);
        if (ratio <= m_JumpRatio)
        {
            // ��ã�Ⱑ �����ؾ� �������·� ��ȯ�Ѵ�.
            if (m_pNPC->SetJumpPos(pTargetChar))
            {
                m_pNPC->ChangeState(STATE_ID_JUMP, true);
                return true;
            }
        }
    }

    if (is_movable_cur_tile == false) {
        // (NOTE) �� �̻� �̵� �Ұ� ���·� ��� �̵� ���� �����Ǵ� ��Ȳ�� �����Ѵ�.
        // |Current-state=���� �˻縦 ���� �ٸ� ���� ���̸� ���� ���� �߰� ó�� ����|
        // |Action=changes to return-state|Side-effect=change regeneration position|
        // |Other=Group�� ���� ó���� �����Ѵ�. �� �� ����.|
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_RETURN, CMS_RUN);
        return true;
    }

    if (m_pNPC->IsMoving())
    {
        // NPC�� �̹� �ٰ� �ִ� ���

        //-----------------------------------------------------------------------------------------
        // n: ���� NPC ��ǥ
        // p: ���� �÷��̾� ��ǥ
        // d: NPC�� �̵� ���� ��ǥ
        // a: ���� n->d
        // b: ���� n->p
        // ��� �� �� ���� ��ã�� �ϴ� ������ ������ ����.
        //
        // a�� b�� ���� ���� ��ġ �̻� �������� p�� d���� �Ÿ��� NPC ���� ��Ÿ��� ����� ���
        //-----------------------------------------------------------------------------------------

        if (m_TrackTimer.IsExpired())
        {
            WzVector wvNPCPos = *m_pNPC->GetPosPtr();
            WzVector wvNPCDestPos = m_pNPC->GetPathExplorer()->GetTargetPos();

            // �÷��̾ NPC�� �ٷ��� ������ ��ä���� ��� ���� ��ġ�ϴ°�?
            BOOLEAN bOutOfAngle = !Math_IsPositionInFanshapedArea(&vTargetPos, &wvNPCPos,
                &(wvNPCDestPos - wvNPCPos), AIParamParser::AICommonInfo.m_wTRACK_INNER_ANGLE);

            // �÷��̾ NPC�� �ٷ��� ���������� ���� �� ���� �Ÿ��� �ִ°�?
            float attack_range = m_pNPC->GetAttackRange();
            ;     attack_range *= attack_range;

            BOOLEAN bOutOfRange = VectorLength2(&(wvNPCDestPos - vTargetPos)) > attack_range;

            if (bOutOfAngle && bOutOfRange) {
                //  printf("�̵��� Ʈ�� ");
                DoTrack();
            }
        }
    }
    else
    {
        // NPC�� �ٰ� ���� ���� ���
        //  printf("���缭 Ʈ�� ");
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

    // ���� Ÿ�� ���� �������� ���� �Ǵ��� ���� ����!
    if (m_pNPC->IsStandingOnJumpTile() && m_pNPC->IsOnGoodPositionToJump())
    {
        float fJumpWaitingTick = Math_VectorLength(vTargetPos, wvCurPos) * 60.0f;
        m_pNPC->ChangeState(STATE_ID_JUMP, false, (LPARAM)fJumpWaitingTick);
        return;
    }

    // ���ο� Track Position�� ã�´�.
    // (NOTE) �ƹ����� ������ ������ ���̴µ�... 0.1f * fAttackRangeRatio�� �����?
    float fAttackRangeRatio = 1.0f - 0.1f * static_cast<float>(m_TrackCount);
    ;     fAttackRangeRatio = max(0, fAttackRangeRatio);

    WzVector vTrackPos;
    m_pNPC->GetTrackPos(vTrackPos, m_pNPC->GetAttackRange() * fAttackRangeRatio);

    // Ÿ���� �̵����̶�� �̵� ���� �����ؼ� ������ ����
    if (pTargetChar->IsMoving())
    {
        const WzVector& wvPlayerDestPos = pTargetChar->GetPathExplorer()->GetTargetPos();
        // Ÿ�� �÷��̾ NPC�� �ݴ������� �̵��Ϸ��� ��쿡�� ����
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

    // ��ã��
    if (m_pNPC->CanMove() == false) {
        // arycoat, 2012.09.04
        // THRUST, STUN ���� state�� ��� �̵��� �Ұ� �Ҷ��̴�. �� �� ���� �ٽ� �����Ѵ�.
    }
    else if (m_pNPC->MoveAndBroadcast(&vTrackPos, CMS_RUN) == false)
    {
        // ���н� Target�� ���� ��ǥ�� �ٽ� ��ã��
        if (m_pNPC->MoveAndBroadcast(&vTargetPos, CMS_RUN) == false)
        {
            // �׷��� �����ϸ� ���� ����
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

    // ���� �����ϰ� �ִ� ����̶�� �������¸� �����Ѵ�.
    Character* pCurTarget = m_pNPC->GetTargetChar();
    if (pCurTarget && pCurTarget->GetObjectKey() == pAttackMsg->dwTargetKey)
    {
        return;
    }

    State::OnMsgForceAttack(pMsg);
}

