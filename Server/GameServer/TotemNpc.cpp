#include "stdafx.h"
#include "TotemNpc.h"

#include <SCItemSlot.h>
#include <SkillInfoParser.h>
//
#include "Player.h"
#include "ChaoState.h"
#include "PVPInfo.h"

#include "Skill.h"
#include "SkillManager.h"
#include "GameZone.h"

//==================================================================================================

TotemNpc::TotemNpc() : summoner_(NULL)
{
    SetObjectType(TOTEMNPC_OBJECT);
}

TotemNpc::~TotemNpc()
{

}

void TotemNpc::Release()
{
    summoner_ = NULL;

    NPC::Release();
}

void TotemNpc::InitTotemNpc(WORD npc_code, Player* summoner, SCItemSlot& IN item_slot)
{
    NPC::Init();
    NPC::SetBaseInfo(npc_code, 0, 0);

    SetSummoner(summoner);
    destruction_expired_ = false;
    item_info_ = item_slot.GetItemInfo();
    assert(item_info_ != NULL && item_slot.IsContainValidItem());

    const BASE_NPCINFO* const npc_info = GetBaseInfo();
    BOOST_STATIC_ASSERT(_countof(skill_timers_) == _countof(npc_info->m_wSkillCode) &&
                        _countof(skill_timers_) == _countof(npc_info->m_wSkillDelay));
    // �������� ��ų�� ���� �ٸ� Ÿ�ֿ̹� ���.
    // ��ų üũ(���) �ð� = ��ü ��ų üũ Ÿ�� + �� ��ų�� üũ Ÿ��
    const int skill_update_time = npc_info->m_wSkillUpdateTime;
    for (int i = 0; i < _countof(skill_timers_); ++i)
    {
        const SLOTCODE skill_code = npc_info->m_wSkillCode[i];
        if (skill_code == 0) {
            continue;
        }
        const int skill_delay = npc_info->m_wSkillDelay[i];
        if (util::ITimerBase* const timer = &skill_timers_[i]) {
            timer->SetTimer(skill_update_time + skill_delay);
            timer->ChangeToInstantExpiration();
        }
    }

    // �Ҹ�ð��� ItemInfo�� �������ҷ� ���� ����Ѵ�.
    destroy_timer_.SetTimer(item_info_->m_dwDuraStr);
}

void TotemNpc::GetTotemNPCRenderInfo(TOTEMNPC_RENDER_INFO* result_render_info)
{
    result_render_info->m_dwObjectKey = GetObjectKey();
    GetPos(&result_render_info->m_wvPos);
    result_render_info->m_ItemCode = item_info_->m_Code;
    if (const BASE_NPCINFO* npc_info = GetBaseInfo()) {
        result_render_info->m_dwMonsterCode = npc_info->m_MonsterCode;
    }
}

void TotemNpc::ForcedAttack(Character* target)
{
    // WARNING: (110209 current)... totem forced attack problem
    if (target == NULL) {
        return;
    }

    const bool bApplyGeneralForcedAtkRule = !(Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD);
    if (bApplyGeneralForcedAtkRule == false) {
        return;
    }
    // Ÿ���� �÷��̾����� üũ
    if (target->IsEqualObjectKind(PLAYER_OBJECT) == false) {
        return;
    }
    // ���������� �����Ƿ� ������ ���·� �����.
    if (summoner_->IsForceAttack()) {
        ChaoState& chao_state = summoner_->GetChaoState();
        chao_state.StartBoradoriState(static_cast<Player*>(target));
    }
}

BOOL TotemNpc::IsTotemSkillAreaType()
{
    //eITEMTYPE_TOTEM : ���� ������ �ڽ��� �����̳� ���� ���� ������ �̻���¸� �Ǵ�.
    //eITEMTYPE_TORCH : �̻���¸� ������ �����̳� ����� ������� �ɾ�д�.
    return (item_info_->m_wType == eITEMTYPE_TOTEM) ? true : false;
}

Player* TotemNpc::GetPlayerOwner()
{
    if (summoner_ && summoner_->IsEqualObjectKind(PLAYER_OBJECT)) {
        return static_cast<Player*>(summoner_);
    }
    return NULL;
    // ... ^^
    //if (!summoner_ || summoner_->IsEqualObjectKind(PLAYER_OBJECT)) {
}

BOOL TotemNpc::ChkSkillPvP(Character* target)
{
    if (summoner_ == NULL) {
        return false;
    }

    if (target->IsEqualObjectKind(PLAYER_OBJECT))
    {
        if (GameZone* game_zone = GetGameZonePtr())
        {
            RC::eSKILL_RESULT result;
            return game_zone->CanPVP(summoner_, static_cast<Player*>(target), &result);
        }
        return false;
    }

    return true;
}

BOOL TotemNpc::Update(DWORD delta_tick)
{
    if (IsDeleteReserved() || destroy_timer_.IsExpired()) {
        destruction_expired_ = true;
    }
    // �Ҹ�Ÿ�ֿ̹� �������� ��ų�� �����ִٸ� ���� ������ �Ҹ��Ѵ�.
    // �Ҹ�Ÿ�̹��� �ƴϴ��� ��ų ��� ��ü(�÷��̾�)�� ���ٸ�, �Ҹ��Ѵ�.
    bool removed = destruction_expired_ && (GetSkillManager()->GetActiveSkillNum() == 0);
    if (removed == false && summoner_ == NULL) {
        removed = true;
    };
    if (removed == false) {
        GameZone* game_zone = GetGameZonePtr();
        if (game_zone == NULL || game_zone != summoner_->GetGameZonePtr()) {
            removed = true;
        }
    }
    if (removed)
    {
        // NOTE: hummm... thinking
        WzVector reset_pos;
        SetVector(&reset_pos, 0, 0, 0);
        SetHP(0);
        SetPos(&reset_pos);
        return false;
    }

    // �ֱ������� �� ��ų�ߵ�
    if (destruction_expired_ == false)
    {
        const BASE_NPCINFO* npc_info = GetBaseInfo();
        int processed = 0;
        for (int i = 0; i < _countof(skill_timers_); ++i)
        {
            const SLOTCODE skill_code = npc_info->m_wSkillCode[i];
            if (skill_code == 0) {
                continue;
            }

            util::ITimerBase* timer = &skill_timers_[i];
            if (timer->IsExpired()) {
                UseSkill(skill_code);
            }
        }

    }
    // TODO: need more checked, this object is not call BaseObject::Update
    if (SkillManager* skill_manager = GetSkillManager()) {
        skill_manager->Update();
    }

    return true;
}


void TotemNpc::OnDead()
{
    destroy_timer_.SetTimer(3000);
}

eUSER_RELATION_TYPE TotemNpc::IsFriend(Character* target)
{
    bool is_neutral = (target == NULL);
    if (summoner_ != NULL && summoner_->IsDeleted())
    {
        is_neutral = true;
    }
    if (is_neutral == false && summoner_ == NULL) {
        is_neutral = true;
    };
    if (is_neutral == false) {
        GameZone* game_zone = GetGameZonePtr();
        if (game_zone == NULL || game_zone != summoner_->GetGameZonePtr()) {
            is_neutral = true;
        }
    }
    if (is_neutral) {
        return USER_RELATION_NEUTRALIST;
    }
    // CHANGES: f110327.2L                           added
    if (bool is_friend = (this == target) || (summoner_ == target)) {
        return USER_RELATION_FRIEND;
    };

    if (false == target->IsEqualObjectKind(PLAYER_OBJECT) &&
        false == target->IsEqualObjectKind(SUMMON_OBJECT) &&
        false == target->IsEqualObjectKind(CRYSTALWARP_OBJECT))
    {
        return USER_RELATION_ENEMY;
    }
    // ��ȯ���� ��ȯ���� �Ǿƽĺ��� ������.
    return summoner_->IsFriend(target);
}


BOOL TotemNpc::UseSkill(SLOTCODE skill_code)
{
    const SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (skill_info == NULL) {
        return false;
    }
    GameField* game_field = GetField();
    if (game_field == NULL || game_field->IsReady() == false) {
        return false;
    };
    //
    const sPOLICY_CHARACTER& summoner_policy = summoner_->Policyment;
    const bool bApplyGeneralForcedAtkRule =
        !(summoner_policy.FIELD & summoner_policy.PCC_IN_SSQ_FIELD);
    if (bApplyGeneralForcedAtkRule)
    {
        if (false == ChkSkillPvP(summoner_)) {
            return false;
        }
    }

    const WzVector* item_pos = GetPosPtr();
    SkillInfo skill_info_data(this, skill_code, 0);
    skill_info_data.SetArgsObjectInfo(summoner_->GetObjectKey(), item_pos, item_pos, item_pos);
    skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
        skill_info_data.kDefaultSkillEffect,
        skill_info_data.kDefaultSkillDelay,
        SKILL_FACTOR_ITEM,
        skill_info_data.kDefaultPropensity,
        skill_info_data.kDefaultEvent);

    SkillManager* skill_manager = GetSkillManager();
    return skill_manager->RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);
}
