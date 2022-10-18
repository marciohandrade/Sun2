#include "stdafx.h"
#include "AttackState.h"

//==================================================================================================
#include <PacketStruct_CG.h>
#include <AIParamParser.h>
#include <SkillInfoParser.h>
//==================================================================================================
#include "CharacterFormula.h"
#include "GameField.h"
#include "Player.h"

#include "Skill.h"

#include "BattleRecord.h"
#include "TargetFinder.h"
#include "TargetManager.h"
#include "StatusManager.h"

#include "Summoned.h"

//==================================================================================================
//
class SendBattlePointOpr
{
public:
    SendBattlePointOpr(NPC* pNPC, const BattleRecord* pRecord)
        : m_pNPC(pNPC)
        , m_pRecord(pRecord)
    {}
    ~SendBattlePointOpr() {}

    void operator() (const BattleRecord::Node* pRecord) const;

private:
    NPC* m_pNPC;
    const BattleRecord* m_pRecord;
    mutable MSG_CG_ETC_DEBUGINFO_CMD m_ChatMsg;
};

//

void
SendBattlePointOpr::operator() (const BattleRecord::Node* pRecord) const
{
    GameField* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pRecord->GetObjectKey());
    if (pAttacker == NULL)
        return;

    const BattleRecord::Node* pUnitRecord = m_pRecord->Get(pAttacker->GetObjectKey());    

    m_ChatMsg.m_dwObjectKey = pAttacker->GetObjectKey();
    int written = _snprintf(m_ChatMsg.m_pszChatMsg, _countof(m_ChatMsg.m_pszChatMsg),
                            "|Total=%u|Class=%u|is1st=%u|Distance=%u|LV=%u|HP=%u|Damage=%u|",
                            pUnitRecord->GetBattlePoint(BATTLE_POINT_TOTAL), pUnitRecord->GetBattlePoint(BATTLE_POINT_CLASS),
                            pUnitRecord->GetBattlePoint(BATTLE_POINT_FIRST), pUnitRecord->GetBattlePoint(BATTLE_POINT_DISTANCE),
                            pUnitRecord->GetBattlePoint(BATTLE_POINT_LEVEL), pUnitRecord->GetBattlePoint(BATTLE_POINT_HP),
                            pUnitRecord->GetBattlePoint(BATTLE_POINT_DAMAGE));

    if (written < 0)
        return;

    m_ChatMsg.m_byMsgLength = (BYTE)written;
    m_ChatMsg.m_pszChatMsg[written] = '\0';

    pAttacker->SendPacket(&m_ChatMsg, m_ChatMsg.GetSize());
}

//==================================================================================================
//
const DWORD
AttackState::FALL_APART_DELAY = 2 * 1000;

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void
AttackState::Init()
{
    //_NA_007053_20140210_BUG_SUMMONED_MONSTER_ATTACKSPEED
    m_ActionDelay.SetTimer(0);
}

void
AttackState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_pNPC->StopMoving();

    m_UpdateTimer.SetTimer(m_UpdateTimer.eStaticTimer_MinSyncInterval, 0);
    m_FallApartTimer.SetTimer(m_FallApartTimer.eStaticTimer_FallApart, 0);
    // TODO: 이건 후퇴 확률로 들어가야 될 듯하다.
    m_RetreatTimer.SetTimer(m_RetreatTimer.eStaticTimer_Retreat, m_pAITypeInfo->m_wRETREAT_PERIOD);
    interval_regulator_ = nsAI::Npc75PercentageRegulator();
}

void
AttackState::OnExit()
{
    State::OnExit();
}

BOOL
AttackState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);
    if (interval_regulator_.Hit())
    {
        if (m_UpdateTimer.IsExpired() == false) {
            return true;
        };
    };
    // 딜레이동안 아무것도 안함
    if (!m_ActionDelay.IsExpired()) 
        return true;

    Character* pTargetChar = m_pNPC->GetTargetChar();
    GameField* pGameField = NULL;

    // 타겟이 NULL이거나 필드에 없으면 상태 전이
    if ((pTargetChar && (pGameField = pTargetChar->GetField())) == false)
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_WANDER);
        return true;
    }
    else if ((pTargetChar->CanBeAttacked() || m_pNPC->UseResurrectionSkill()) == false)
    {   // Target이 현재 공격할 수 없는 상태라면 상태 전이
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_WANDER);
        return true;
    }

    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE 추가에 따른 AttackState유지 상태를
    // 바꿀 필요성이 생겼다.
    {
        SECTOR_IDX sector_index = m_pNPC->GetSectorIndex();
        Sector* sector = sector_index != INVALID_SECTOR_INDEX ? pGameField->GetSector(sector_index)
                                                              : NULL;
        // 'sector == NULL' 발생해서는 안되지만, 경우에 따라
        // (3DEngine에서 미묘한 계산 오차에 따른 사례 존재) 가능성이 존재한다.
        DWORD references = sector != NULL ? sector->GetReferenceCount() : 0;
        if (references == 0)
        {
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_WANDER); // if summoned then changes to summon_idle state
            return true;
        }
    };

    StatusField& rStatus = pTargetChar->GetStatusManager()->Status;

    if (!!(rStatus.FIELD & rStatus.ENABLE_CHARACTER_TRANSPARENT))
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_WANDER);
        return true;
    }

    // (NOTE) thinking stack tracing for return state
    const nsAI::TileEvent* target_tile_event = 0;
    if (pTargetChar->PlanEvent->GetTileEvent(&target_tile_event) == false ||
       (target_tile_event->Attr & PTA_SAFETY_ZONE))
    {
        m_pNPC->SetTargetChar(NULL);
        // no change status, expect reentrant attack state
        return true;
    }

    //SUNKR-114 문상현 소환수를 이용하여 적을 공격하고 안전지대로 이동하여도 소환수는 계속 공격하는 오류 수정
    if(m_pNPC->IsEqualObjectKind(SUMMON_OBJECT))
    {
        Player* const owner_player = m_pNPC->GetPlayerOwner();
        if(owner_player)
        {
            if (owner_player->PlanEvent->GetTileEvent(&target_tile_event) == false ||
                (target_tile_event->Attr & PTA_SAFETY_ZONE))
            {
                m_pNPC->SetTargetChar(NULL);                
                return true;
            }
        }
    }

    if (m_pNPC->GetSelectedSkill() == SKILLCODE_INVALID)
    {   // 공격 방법 선택되어 있지 않으면 선택
        m_pNPC->SelectSkill();
    }

    // (NOTE) 하기 내용에 관련해서는 로직 평가 순서를 일부 바꿀 필요가 있어 보인다.

    // 현재 공격할 수 없는 상황이면 리턴 
    if (!m_pNPC->CanAttack())
        return true;

    if (!m_pNPC->GetStatusManager()->Status.IsPassConstraint(m_pNPC, (SkillScriptInfo*)0))
        return true;

    BOOLEAN is_movable_cur_tile = true;
    ;       is_movable_cur_tile = !!nsAI::PlanEventObject::AcceptableTarget(m_pNPC, 0, 0);

    // 타겟이 공격 사거리를 벗어나면 추적 상태로 전이
    if (m_pNPC->GetDist(pTargetChar) >= (m_pNPC->GetAttackRange() +
                                     AIParamParser::AICommonInfo.m_fRANGE_TOLERANCE))
    {
        m_pNPC->ChangeState(STATE_ID_TRACK);
        return true;
    }

    if (is_movable_cur_tile != false) {
        // 너무 가까우면 좀 멀어지기
        if (m_FallApartTimer.IsExpired() && m_pNPC->IsTooCloseToTarget()) {
            m_pNPC->ChangeState(STATE_ID_FALL_APART);
            return true;
        }

        // 내 공격거리에 비해 너무 가까우면 조금 후퇴하자.
        if (m_RetreatTimer.IsExpired() && m_pNPC->NeedToRetreatFromTarget()) {
            m_pNPC->ChangeState(STATE_ID_RETREAT);
            return true;
        }
    }

    // 일반공격이나 스킬공격을 한다.
    DoAction();

    return true;
}

void
AttackState::DoAction()
{
    const SLOTCODE selectedSkillCode = m_pNPC->GetSelectedSkill();

    ASSERT(selectedSkillCode != SKILLCODE_INVALID);

    Character* pTarget = m_pNPC->GetTargetChar();
    if (pTarget == NULL)
    {
		m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_WANDER);
        return;
    }

    if (selectedSkillCode == SKILLCODE_NORMAL_ATTACK) {
        DoNormalAttack();
    }
    else {
        UseSkill(selectedSkillCode);
    }

    m_pNPC->ResetSelectedSkill();
    m_pNPC->ForcedAttack(pTarget); // PK 공격이라면 관련처리를 한다.

    if (selectedSkillCode == SKILLCODE_NORMAL_ATTACK) {
    }
    else {
        // 스킬 사용 후의 처리
        ActionAfterUseSkill();
    }

    // NPC가 GMIdle 상태일 경우 스킬구사 후 Idle 상태로 돌아온다.
    if (m_pNPC->ISGMIdleOn())
    {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_IDLE);
    }
}

void
AttackState::DoNormalAttack()
{
    // 공격
    Character* pTargetCharacter = m_pNPC->GetTargetChar();
    SASSERT(pTargetCharacter != NULL, "[AttackState::DoNormalAttack] pTargetCharacter == NULL");
    if (pTargetCharacter == NULL) {
        m_pNPC->SetTargetChar(NULL);
        m_pNPC->ChangeState(STATE_ID_WANDER);
        return;
    }
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    // 소환수가 공격하는 타겟이 소환수의 주인과같은 파티원일 경우..상태 전이..
    if (m_pNPC->IsEqualObjectKind(SUMMON_OBJECT) && 
        pTargetCharacter->GetPlayerOwner() && 
        pTargetCharacter->GetPlayerOwner()->GetPartyState().GetPartyKey() != 0)
    {
        Player* const target_owner = pTargetCharacter->GetPlayerOwner();
        Player* const summon_owner = m_pNPC->GetPlayerOwner();
        if (target_owner && summon_owner && 
            (target_owner->GetPartyState().GetPartyKey() == summon_owner->GetPartyState().GetPartyKey())
            )
        {
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_WANDER);
            return;
        }
    }
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    BYTE byAttackType = (State::GetStateRandomNo() & 1) + 1;

    DWORD dwActionDelay = 0;//캐릭터 애니메이션툴에서 나온 값으로 기획팀에서 스크립트에 세팅한 값
    switch (byAttackType)
    {
    case 1:
        dwActionDelay = m_pNPC->GetBaseInfo()->m_wAttSpeed;
        break;
    case 2:
        dwActionDelay = m_pNPC->GetBaseInfo()->m_wAttSpeed2;
        break;
    }

    if (float physical_atk_spd = m_pNPC->GetPhysicalAttackSpeed()) {
        dwActionDelay = (DWORD)(dwActionDelay / physical_atk_spd);
    }
    else
    {
        dwActionDelay = 1000;
        SUNLOG(eMIDDLE_LOG, "[DoNormalAttack] PhysicalAttackSpeed is 0");
    }

    // (CHANGES) (f100506.3L) fix up a next timeout setting code that
    // twice expiration events has occurred by already set a timeout value of the time 
    // because of the 'SetInterval' function has changed next interval value only.
    m_ActionDelay.SetTimer(dwActionDelay);
    m_pNPC->StopMoving();

    const eATTACK_TYPE base_attack_type = m_pNPC->GetWeaponBaseAttType();
    WORD wCriticalRatioBonus = 0;
    BYTE bySkillEffect = 0;
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    int damage = ep2::CalcNormalDamage(m_pNPC, pTargetCharacter,
        base_attack_type, true, wCriticalRatioBonus, &bySkillEffect);
#else
    DAMAGETYPE  damage  = ep1::CalcNormalDamage(m_pNPC, pTargetCharacter,
        base_attack_type, m_pNPC->GetWeaponMagicalAttType(),
        wCriticalRatioBonus, &bySkillEffect);
#endif
    // f100908.1L
    DamageArgs damage_args(m_pNPC, base_attack_type);
    ;{
        damage_args.option_.is_critical_ = !!(bySkillEffect & SKILL_EFFECT_CRITICAL);
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
        //damage_args.attack_regist_kind_;
        if (Character::IsPCObject(m_pNPC)) {
            damage_args.sd_apply_ = eSD_APPLY_DO;
        }
        damage_args.damage_ = damage;
        //damage_args.damage_sd_;
    };

    pTargetCharacter->Damaged(&damage_args);
    m_pNPC->OnAttack(pTargetCharacter, SKILLCODE_NORMAL_ATTACK, damage_args.damage_);

    // NPC 현재 위치 얻기
    const WzVector& wvCurPos = *m_pNPC->GetPosPtr();

    ;{
        MSGSUB_BATTLE_MONSTER_ATTACK_BRD msg_brd;
        msg_brd.Clear();
        msg_brd.m_dwObjectKey = m_pNPC->GetObjectKey();
        msg_brd.m_dwTargetObjectKey = pTargetCharacter->GetObjectKey();
        msg_brd.m_byAttackType      = byAttackType;
        msg_brd.m_wvCurPos          = wvCurPos;
        msg_brd.m_wDamage           = damage_args.damage_;
        msg_brd.m_wTargetHP         = pTargetCharacter->GetHP();
        msg_brd.m_wTargetReserveHP  = pTargetCharacter->GetReserveHP();
        msg_brd.m_byEffect          = bySkillEffect;
        m_pNPC->SendPacketAroundForSync(&msg_brd);
    };

    // 공격당한 NPC에게 AIMessage를 날린다.
    AI_MSG_ATTACKED AIAttackedMsg;
    AIAttackedMsg.dwAttackerKey = m_pNPC->GetObjectKey();
    AIAttackedMsg.wDamage = damage_args.damage_; // f100908, fix an invalid usage of a wDamage ... correct?
    pTargetCharacter->SendAIMessage(&AIAttackedMsg, sizeof(AIAttackedMsg));

    if (pTargetCharacter->IsEqualObjectKind(PLAYER_OBJECT))
    {
        Player* pPlayer = static_cast<Player*>(pTargetCharacter);
        if (eDEBUGINFO_TYPE_AGGRO == pPlayer->GetDebugInfoFlag())
        {
            BattleRecord* pRecord = m_pNPC->GetTargetManager()->GetBattleRecord();
            if (pRecord)
            {
                SendBattlePointOpr Opr(m_pNPC, pRecord);
                pRecord->ForEachRecord(Opr);
            }
        }
    }
}

BOOL
AttackState::UseSkill(SLOTCODE skillCode)
{
    // 스킬을 사용가능한지 체크
    RC::eSKILL_RESULT rcResult = m_pNPC->CanUseSkill(skillCode);
    if (rcResult != RC::RC_SKILL_SUCCESS)
    {
        return false;
    }

    // 스킬을 사용한다.
    m_pNPC->UseSkill(skillCode);

    // 스킬사용 딜레이를 준다.
    SkillScriptInfo* pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(skillCode);
    DWORD delay = m_pNPC->GetSelectedSkillDelay() +
                  pBaseSkillInfo->m_wSkillCasting +
                  pBaseSkillInfo->m_wFlyingLifeTime;
    delay = max(500, delay);
    m_ActionDelay.SetTimer(delay);

    return true;
}

void
AttackState::ActionAfterUseSkill()
{
    // 소환체가 스킬사용 명령모드일 경우에는 스킬구사 후 Idle 상태로 돌아온다.
    if (m_pNPC->IsEqualObjectKind(SUMMON_OBJECT))
    {
        Summoned* pSummoned = (Summoned*)m_pNPC;

        if (!pSummoned->IsOffensive() && pSummoned->GetSkillUseDelegate())
        {
            pSummoned->SetSkillUseDelegate(false);
            pSummoned->SetTargetChar(NULL);
            pSummoned->ChangeState(STATE_ID_IDLE);
            return;
        }
    }

    Character* pTargetChar = m_pNPC->GetTargetChar();

    // 스킬을 적용한 대상이 아군이라면 타겟을 다시 찾는다.
    if (m_pNPC->IsFriend(pTargetChar) == USER_RELATION_FRIEND)
    {
        // 상태 버그 문제 [5/14/2010 lst1024]
        // npc가 자신에게 스킬 사용시 현재 타겟은 자신으로 변경된다.
        // 이후 npc는 공격 대상을 재선정 하게 된다.
        // 만약 후공이라면, SearchTarget 을 통해 공격 대상의 선정에 실패 하고 idle 상태로 변경된다.
        pTargetChar = m_pNPC->SearchTarget();   

        if (pTargetChar)
        {
            m_pNPC->SetTargetChar(pTargetChar);
            m_pNPC->ChangeState(STATE_ID_TRACK);
        }
        else
        {
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_IDLE);
        }
    }
}

BOOL
AttackState::IsReachableSkillRange(Character* pTargetChar, float fSkillRange)
{
    if (pTargetChar)
    {
        return m_pNPC->GetDist(pTargetChar) <= fSkillRange;
    }

    return true;
}

void
AttackState::OnMsgForceAttack(const AI_MSG* pMsg)
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

void
AttackState::OnMsgChangeToRetreatState(const AI_MSG* pMsg)
{
    const AI_MSG_CHANGETORETREATSTATE* pRecvMsg = static_cast<const AI_MSG_CHANGETORETREATSTATE*>(pMsg);

    m_pNPC->ChangeState(STATE_ID_RETREAT,
                        pRecvMsg->fMaxDistance,
                        (LPARAM)!!pRecvMsg->bIdleAfterRetreat,
                        pRecvMsg->dwMsgID);
}










