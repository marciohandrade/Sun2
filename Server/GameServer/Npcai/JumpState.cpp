#include "stdafx.h"
#include "JumpState.h"

#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include <PacketControl/WarMessages_Sync.h>
#include "CharacterFormula.h"
#include "TargetManager.h"
#include "Skill.h"

JumpState::JumpState()
{
}

JumpState::~JumpState()
{
}

void
JumpState::Init()
{   
}

void
JumpState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    Field* pField = m_pNPC->GetField();
    if ((pField && m_pNPC->GetTargetChar()) == false)
    {
        m_pNPC->ChangeState(STATE_ID_IDLE);
        return;
    }

    {
        BOOLEAN is_jump_attack = !!param1;
        SetJumpAttack(is_jump_attack);
    };
    SetSendAttackResult(false);
    m_wvDestPos = m_pNPC->GetJumpPos();

    const WzVector& wvCurPos = *m_pNPC->GetPosPtr();
    m_dwTotDistance2 = (DWORD)VectorLength2(&(m_wvDestPos - wvCurPos));

    // 점프 패킷 보내기
    {
        MSGSUB_SYNC_NPC_JUMP_BRD msg;
        msg.m_bJumpAttack = !!m_bJumpAttack;
        msg.m_dwObjectKey = m_pNPC->GetObjectKey();
        msg.m_wvEndPos    = m_wvDestPos;
        m_pNPC->SendPacketAroundForSync(&msg);
    };

    // 타이머 셋팅
    DWORD interval = static_cast<DWORD>(param2);
    m_JumpTimer.SetTimer(interval);
}

void
JumpState::OnExit()
{
    State::OnExit();
}

BOOL
JumpState::OnUpdate(DWORD dwDeltaTick)
{
    BOOL bJumpEnd = false;

    if (m_bJumpAttack)
    {
        // 어느정도 이동하면 미리 공격패킷을 날려준다.
        if (IsAttackDistance())
            AttackResult();

        // 이동이 끝나면 점프상태를 벗어난다.
        if (m_pNPC->IsMoving() == false)
            bJumpEnd = true;
    }
    else if (m_JumpTimer.IsExpired())
    {
        bJumpEnd = true;
    }

    if (bJumpEnd)
    {
        m_pNPC->SetPos(&m_wvDestPos);

        if (m_pNPC->GetTargetChar())
            m_pNPC->ChangeState(STATE_ID_TRACK);
        else
            m_pNPC->ChangeState(STATE_ID_IDLE);
    }

    return true;
}

void
JumpState::AttackResult()
{
    Character* pTarget = m_pNPC->GetTargetChar();

    DWORD wTargetHP = 0;
    BYTE  bySkillEffect = 0;
    DWORD dwTargetObjKey = 0;
    BYTE  byAttackType = (State::GetStateRandomNo() & 1) + 1;

    // f100908.1L
    DamageArgs damage_args(m_pNPC, m_pNPC->GetWeaponBaseAttType());

    if (pTarget)
    {
        dwTargetObjKey = pTarget->GetObjectKey();

        // 사거리 체크
        const WzVector& vTargetPos = *pTarget->GetPosPtr();
        float fDistToTarget = VectorLength(&(m_wvDestPos-vTargetPos));
        if (fDistToTarget <= m_pNPC->GetAttackRange())
        {

            WORD wCriticalRatioBonus = 0; 
            BOOL bCritical = false;
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
            damage_args.damage_ = ep2::CalcNormalDamage(m_pNPC, pTarget,
                m_pNPC->GetWeaponBaseAttType(), true, wCriticalRatioBonus, &bySkillEffect);
        #else
            damage_args.damage_ = ep1::CalcNormalDamage(m_pNPC, pTarget,
                m_pNPC->GetWeaponBaseAttType(), m_pNPC->GetWeaponMagicalAttType(),
                wCriticalRatioBonus, &bySkillEffect);
        #endif
            ;{
                damage_args.option_.is_critical_ = (bySkillEffect & SKILL_EFFECT_CRITICAL) != 0;
                //damage_args.option_.is_mirror_;
                //damage_args.limit_hp_;
                //damage_args.attack_regist_kind_;
                //damage_args.sd_apply_;
                //damage_args.damage_;
                //damage_args.damage_sd_;
            };
            pTarget->Damaged(&damage_args);
            m_pNPC->OnAttack(pTarget, SKILLCODE_NORMAL_ATTACK, damage_args.damage_);
        }

        wTargetHP = pTarget->GetHP();
    }

    // NPC 현재 위치 얻기
    const WzVector& wvCurPos = *m_pNPC->GetPosPtr();
    ;{
        MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD msg_brd;
        msg_brd.Clear();
        msg_brd.m_dwObjectKey       = m_pNPC->GetObjectKey();
        msg_brd.m_dwTargetObjectKey = dwTargetObjKey;
        msg_brd.m_byAttackType      = byAttackType;
        msg_brd.m_wvCurPos          = wvCurPos;
        msg_brd.m_wDamage           = damage_args.damage_;
        msg_brd.m_wTargetHP         = wTargetHP;
        msg_brd.m_byEffect          = bySkillEffect;

        m_pNPC->SendPacketAroundForSync(&msg_brd);
    };
    m_bSendAttackResult = true;
}

BOOLEAN
JumpState::IsAttackDistance()
{
    if (m_bSendAttackResult)
        return false; 

    // 남은 거리
    const WzVector& vCurPos = *m_pNPC->GetPosPtr();
    DWORD dwLeavedDistance2 = (DWORD)VectorLength2(&(m_wvDestPos-vCurPos));

    // 남은 거리가 전체거리의 40% 이하이면 공격!
    if (dwLeavedDistance2 <= m_dwTotDistance2 * (0.4f * 0.4f))
        return true;

    return false;
}

// 점프 상태에서는 LeaveField 외에는 다 무시
void
JumpState::OnAIMsg(const AI_MSG* pMsg)
{
    switch (pMsg->dwMsgID)
    {
    case AI_MSG_ID_LEAVE_FIELD:
        OnMsgLeaveField(pMsg);
        break;
    }
}

// 점프상태가 끝나면 Follow(Idle)상태로 복귀한다.
void
JumpState::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    m_pNPC->SetTargetChar(NULL);
}

