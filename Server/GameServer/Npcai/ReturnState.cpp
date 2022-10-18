#include "stdafx.h"
#include "ReturnState.h"

#include <SinTable.h>
#include "AIParamParser.h"
#include "PublicMath.h"
#include <PacketControl/WarMessages_Sync.h>

#include "GameField.h"

#include "../MovingPlanner.h"

#include "BattleRecord.h"
#include "StatusManager.h"
#include "Monster.h"
#include "TargetManager.h"



ReturnState::ReturnState()
{
}

ReturnState::~ReturnState()
{
}

void
ReturnState::Init()
{   
}

void
ReturnState::SetNPC(NPC* pNpc)
{
    m_pMonster = DOWNCASTER_SELECT<Monster*>(pNpc);
    if (m_pMonster == NULL || pNpc->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void
ReturnState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);
    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE �߰��� ���� ���� �ݿ�
    const sPOLICY_CHARACTER& CharPolicyment = m_pNPC->Policyment;
    m_pNPC->ChangePolicyPostEnterField(CharPolicyment.PCC_CONTINUING_NPC_UPDATE,
                                       CharPolicyment.PCC_DISABLE_ALL,
                                       NULL);

    if (m_pMonster->MoveAndBroadcast(&m_pMonster->GetRegenPos(), CMS_RUN))
    {
        m_bRegenPosSelected = true;
    }
    else
    {
        m_bRegenPosSelected = false;
    }

    m_pMonster->GetStatusManager()->AllocStatus(eCHAR_STATE_ETC_RETURNING);

    // ���ϻ��¿����� ������ BattleRecord ������ �ʱ�ȭ�Ѵ�.
    m_pMonster->GetTargetManager()->ClearBattleRecord();

    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);

    // (CHANGES) (091231) (WAVERIX) return command by leader control
    if (m_pMonster->IsLeader()) {
        AI_MSG_CHANGETORETURNSTATE returnMsg;
        m_pNPC->SendAIMessageToGroupExceptMe(&returnMsg, sizeof(returnMsg),
            (WORD)dRandom(AIParamParser::AICommonInfo.m_wGROUP_AIMESSAGE_MIN_DELAY,
                          AIParamParser::AICommonInfo.m_wGROUP_AIMESSAGE_MAX_DELAY));
    }
}

void
ReturnState::OnExit()
{
    State::OnExit();
    // �Ϳ�ó�� HP full�� ä���ִ� �͵� ���� ������;;
    // �������� �ʱ�ȭ
    //m_pMonster->GetBattleRecord().Init();
    m_pMonster->GetStatusManager()->Remove(eCHAR_STATE_ETC_RETURNING);

    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE �߰��� ���� ���� �ݿ�
    const sPOLICY_CHARACTER& CharPolicyment = m_pNPC->Policyment;
    m_pNPC->ChangePolicyPostEnterField(CharPolicyment.PCC_DISABLE_ALL,
                                       CharPolicyment.PCC_CONTINUING_NPC_UPDATE,
                                       NULL);
}

BOOL
ReturnState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    // �̵��� ������ �ٽ� RegenPos�� �����δ�.
    if (m_TrackTimer.IsExpired() && !m_pMonster->IsMoving())
    {
        // �������� �����ϸ� Idle���·� ����
        if (m_bRegenPosSelected)
        {
            MSGSUB_SYNC_STOP_BRD msg;
            msg.m_dwObjectKey = m_pMonster->GetObjectKey();
            msg.m_wvCurPos    = *m_pMonster->GetPosPtr();
            m_pMonster->SendPacketAroundForSync(&msg);

            m_pMonster->ChangeState(STATE_ID_IDLE);
            return true;
        }
        // �������� ������ ������ �ٽ� �̵�
        else
        {
            DoReturn();
        }
    }
    return true;
}

void
ReturnState::DoReturn()
{
    const WzVector& wvCurPos = *m_pMonster->GetPosPtr();
    WzVector regen_pos = m_pMonster->GetRegenPos();

    BOOLEAN need_change_regen_pos = false;
    BOOLEAN is_movable_cur_tile = true;
    // (CHANGES) (091229) (WAVERIX) �ڱ� �ڽ��� ������ ���¸� ���� �ʿ� �ִ�.
    // ���� �������� ����ϹǷ� �̸� ����� �д�.
    // track->return:change regeneration pos
    // track->attack
    is_movable_cur_tile = !!nsAI::PlanEventObject::AcceptableTarget(m_pMonster, 0, 0);

    // �̵��� �Ÿ��� ���Ѵ�.
    const float MoveDistance = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                              AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

    const WzVector& diff = regen_pos - wvCurPos;
    float fDistFromRegenPosSq = VectorLength2(&diff);

    WzVector wvDestPos;

    if (is_movable_cur_tile != false) {
        if ((MoveDistance * MoveDistance) >= fDistFromRegenPosSq)
        {
            wvDestPos = regen_pos;
            m_bRegenPosSelected = true;
        }
        else
        {
            WzVector wvNormal;
            // RegenPos�� ���� ���ݾ� ��ã���Ѵ�.
            VectorNormalize(&wvNormal, &diff);
            // (CHANGES) (091229) (WAVERIX) remove z=0, �� �� ���� ������ ���������� ������...
            //wvNormal.z = 0;
            wvDestPos = wvCurPos + wvNormal * MoveDistance;
        }
    }
    else { // is_movable_cur_tile == false
        need_change_regen_pos = true;
    }

    if (is_movable_cur_tile != false) {
        // �̵����� ���θ� üũ�� ��Ŷ�� ������.
        if (m_pMonster->MoveAndBroadcast(&wvDestPos, CMS_RUN) == false) {
            // �̵��� �����ϸ� ���� ������ǥ�� �̵��غ���.
            if (m_pMonster->MoveAndBroadcast(&regen_pos, CMS_RUN) == false) {
                need_change_regen_pos = true;
            }
        }
    }

#ifdef _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE
    if (need_change_regen_pos != false) 
    {
        GameField* cur_field = m_pMonster->GetField();
        // �ʵ尡 �����ϰ�, ������ġ�� ���������̸� �ڷ���Ʈ�� ���ѹ�����
        if (cur_field != NULL)
        {
            cur_field->TeleportObject(m_pMonster, &regen_pos);
            m_pMonster->ChangeState(STATE_ID_IDLE);
        }
        else
        {
            // �׷��� �����ϸ� ������ǥ�� ���� ��ǥ�� ������ ������!
            //
            m_pMonster->SetRegenPos(wvCurPos);
            m_pMonster->ChangeState(STATE_ID_IDLE);

            MSGSUB_SYNC_STOP_BRD msg;
            msg.m_dwObjectKey = m_pMonster->GetObjectKey();
            msg.m_wvCurPos    = wvCurPos;
            m_pMonster->SendPacketAroundForSync(&msg);
        }
        return;
    }
#else
    if (need_change_regen_pos != false) {
        // �׷��� �����ϸ� ������ǥ�� ���� ��ǥ�� ������ ������!
        //
        m_pMonster->SetRegenPos(wvCurPos);
        m_pMonster->ChangeState(STATE_ID_IDLE);

        MSGSUB_SYNC_STOP_BRD msg;
        msg.m_dwObjectKey = m_pMonster->GetObjectKey();
        msg.m_wvCurPos    = wvCurPos;
        m_pMonster->SendPacketAroundForSync(&msg);
        return;
    }
#endif //_NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE

    m_TrackTimer.SetTimer(m_TrackTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
}

