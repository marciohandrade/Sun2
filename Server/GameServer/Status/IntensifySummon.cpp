#include "stdafx.h"

#include "./IntensifySummon.h"
#include "SummonedNPCs.h"

//==================================================================================================

struct IntensifySummon::ChangeSummonAttr
    : public std::unary_function<const SUMMONED_MAP_PAIR&, void>
{
    ChangeSummonAttr(SLOTCODE skill_code, bool applied, Player* owner,
                     const nsSkill::AdrenalineConfiguration* applied_data);

    void operator()(const SUMMONED_MAP_PAIR& value) const;

private:
    void BuildMessage(NPC* summoned, int original_max_hp) const;
    //
    bool applied_;
    SLOTCODE skill_code_;
    Player* owner_;
    PlayerAttributes& owner_attributes_;
    const nsSkill::AdrenalineConfiguration* const applied_data_;
    mutable MSG_CG_SUMMON_CURRENT_HP_CMD msg_;
    //
    //__DISABLE_COPY(ChangeSummonAttr);
};

//==================================================================================================

inline IntensifySummon::ChangeSummonAttr::ChangeSummonAttr(
    SLOTCODE skill_code, bool applied, Player* owner,
    const nsSkill::AdrenalineConfiguration* applied_data)
    : skill_code_(skill_code)
    , applied_(applied)
    , owner_(owner)
    , owner_attributes_(owner->GetPlayerAttr())
    , applied_data_(applied_data)
{
}

inline void IntensifySummon::ChangeSummonAttr::BuildMessage(
    NPC* summoned, int original_max_hp) const
{
    const int cur_hp = summoned->GetHP();
    const int changed_max_hp = summoned->GetMaxHP();
    int calc_value = applied_ ? (cur_hp + (changed_max_hp - original_max_hp))
                              : (min(cur_hp, changed_max_hp));
    summoned->SetHP(max(0, calc_value));

    msg_.m_dwSummonedObjKey = summoned->GetObjectKey();
    msg_.m_dwMaxHP = changed_max_hp;
    msg_.m_dwHP = summoned->GetHP();
    msg_.m_dwSpeedRatio = summoned->GetMoveSpeedRatio();
}

void IntensifySummon::ChangeSummonAttr::operator()(const SUMMONED_MAP_PAIR& node) const
{
    NPC* const summoned = node.second;
    const int original_max_hp = summoned->GetMaxHP();
    ;{
        SkillAttrCalculator calculator(summoned->GetAttr());
        // CHANGES: f110808.5L, changed the adrenaline calculator method.
        //calculator.ApplyAdrenaline(applied_, skill_code_, owner_attributes_);
        calculator.ApplyAdrenaline(applied_, skill_code_, *applied_data_);
    };

    this->BuildMessage(summoned, original_max_hp);
    owner_->SendPacket(&msg_, sizeof(msg_));
}

//==================================================================================================

IntensifySummon::IntensifySummon(void)
{
}

IntensifySummon::~IntensifySummon(void)
{
}

void IntensifySummon::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    // ��ȯ���� Attr�� ��ȭ���Ѿ� �ϱ� ������
    // AbilityStatus::Start()�� ȣ������ �ʰ� ������ �ؼ� ����Ѵ�.
    if (owner->IsEqualObjectKind(PLAYER_OBJECT) == false) 
    {
        return;
    };
    
    // CHANGES: f110808.5L
    SkillAttrCalculator::GetCurrentAdrenalineConfiguration(
        &applied_data_, GetSkillCode(), owner->GetAttr());
    
    ApplyAtrributesForAllSummoned(true);
    SetApply(true);
    //AbilityStatus::Start(); // ������ ���� Ʋ���� ���� ó���� ���� �ʴ´�.
}

void IntensifySummon::Execute()
{
}

void IntensifySummon::End()
{
    const Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    if (owner->IsEqualObjectKind(PLAYER_OBJECT) == false) 
    {
        return;
    };
    
    ApplyAtrributesForAllSummoned(false);

    //AbilityStatus::End(); // ������ ���� Ʋ���� ���� ó���� ���� �ʴ´�.
    if (IsApply()) // NOTE: f110623.1L, ...?
    {
        SendStatusDelBRD();
        SetApply(false);
    };
}

//==================================================================================================
// NOTE: public interface. this status already has the calculated adrenaline data.
void IntensifySummon::ApplyAttributes(NPC* const summoned)
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    if (owner->IsEqualObjectKind(PLAYER_OBJECT) == false) 
    {
        return;
    };
    Player* const player = static_cast<Player*>(owner);
    IntensifySummon::ChangeSummonAttr change_attr_op(GetSkillCode(), true, player, &applied_data_);
    change_attr_op.operator()(SUMMONED_MAP::value_type(summoned->GetObjectKey(), summoned));
}

//==================================================================================================

void IntensifySummon::ApplyAtrributesForAllSummoned(bool applied)
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    
    // assert (m_pOwner->IsEqualObjectKind(PLAYER_OBJECT) != false);
    const DWORD owner_player_key = owner->GetObjectKey();
    SummonedNPCs* const summoned_npcs = SummonManager::Instance()->FindSummonNPCs(owner_player_key);
    if (summoned_npcs == NULL) 
    {
        return;
    };
    if (summoned_npcs->IsSummoner(owner_player_key) == false) 
    {
        return;
    };
    Player* const player = static_cast<Player*>(owner);
    IntensifySummon::ChangeSummonAttr change_attr_op(GetSkillCode(), applied, player, &applied_data_);
    summoned_npcs->ForeachSummonedList(change_attr_op);
}

//==================================================================================================
