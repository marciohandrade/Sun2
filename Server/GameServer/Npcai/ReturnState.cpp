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
    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE 추가에 따른 상태 반영
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

    // 리턴상태에서는 기존의 BattleRecord 정보를 초기화한다.
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
    // 와우처럼 HP full로 채워주는 것도 좋지 않을까;;
    // 전투정보 초기화
    //m_pMonster->GetBattleRecord().Init();
    m_pMonster->GetStatusManager()->Remove(eCHAR_STATE_ETC_RETURNING);

    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE 추가에 따른 상태 반영
    const sPOLICY_CHARACTER& CharPolicyment = m_pNPC->Policyment;
    m_pNPC->ChangePolicyPostEnterField(CharPolicyment.PCC_DISABLE_ALL,
                                       CharPolicyment.PCC_CONTINUING_NPC_UPDATE,
                                       NULL);
}

BOOL
ReturnState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    // 이동이 끝나면 다시 RegenPos로 움직인다.
    if (m_TrackTimer.IsExpired() && !m_pMonster->IsMoving())
    {
        // 목적지에 도달하면 Idle상태로 전이
        if (m_bRegenPosSelected)
        {
            MSGSUB_SYNC_STOP_BRD msg;
            msg.m_dwObjectKey = m_pMonster->GetObjectKey();
            msg.m_wvCurPos    = *m_pMonster->GetPosPtr();
            m_pMonster->SendPacketAroundForSync(&msg);

            m_pMonster->ChangeState(STATE_ID_IDLE);
            return true;
        }
        // 목적지에 도달할 때까지 다시 이동
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
    // (CHANGES) (091229) (WAVERIX) 자기 자신의 적절한 상태를 평가할 필요 있다.
    // 선행 조건으로 충분하므로 미리 계산해 둔다.
    // track->return:change regeneration pos
    // track->attack
    is_movable_cur_tile = !!nsAI::PlanEventObject::AcceptableTarget(m_pMonster, 0, 0);

    // 이동할 거리를 구한다.
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
            // RegenPos를 향해 조금씩 길찾기한다.
            VectorNormalize(&wvNormal, &diff);
            // (CHANGES) (091229) (WAVERIX) remove z=0, 좀 더 높이 검증이 정밀해졌기 때문에...
            //wvNormal.z = 0;
            wvDestPos = wvCurPos + wvNormal * MoveDistance;
        }
    }
    else { // is_movable_cur_tile == false
        need_change_regen_pos = true;
    }

    if (is_movable_cur_tile != false) {
        // 이동가능 여부를 체크후 패킷을 날린다.
        if (m_pMonster->MoveAndBroadcast(&wvDestPos, CMS_RUN) == false) {
            // 이동이 실패하면 직접 리젠좌표로 이동해본다.
            if (m_pMonster->MoveAndBroadcast(&regen_pos, CMS_RUN) == false) {
                need_change_regen_pos = true;
            }
        }
    }

#ifdef _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE
    if (need_change_regen_pos != false) 
    {
        GameField* cur_field = m_pMonster->GetField();
        // 필드가 존재하고, 현재위치가 안전지대이면 텔레포트를 시켜버린다
        if (cur_field != NULL)
        {
            cur_field->TeleportObject(m_pMonster, &regen_pos);
            m_pMonster->ChangeState(STATE_ID_IDLE);
        }
        else
        {
            // 그래도 실패하면 리젠좌표를 현재 좌표로 셋팅해 버린다!
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
        // 그래도 실패하면 리젠좌표를 현재 좌표로 셋팅해 버린다!
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

