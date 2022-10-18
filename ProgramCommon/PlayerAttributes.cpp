#include "stdafx.h"
#include "PlayerAttributes.h"

//==================================================================================================
#include "Struct.h"
#include "SCPlayer.h"
#include "CommonCharacterFormula.h"
#include "KBMoveCheckManager.h"

//==================================================================================================

PlayerAttributes::PlayerAttributes()
    : owner_(NULL)
{
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    socket_option_attributes_.Clear();
#endif
}

PlayerAttributes::~PlayerAttributes()
{
}

void PlayerAttributes::SetOwner(IPlayerAttributeLinker* owner)
{
    owner_ = owner;
}

//==================================================================================================
// CHANGES: f110217.2L, add clone interface to support stat configuration preview
void PlayerAttributes::Clone(PlayerAttributes* dest)
{
    CopyMemory(dest->attr_values_, attr_values_,
        static_cast<size_t>(last_field_end_ - reinterpret_cast<int8_t*>(attr_values_)));
    dest->RegisterAll(true);
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    socket_option_attributes_.Clone(&dest->socket_option_attributes_);
#endif
#ifdef _NA_002935_20110704_ITEM_SCORE
    set_option_attributes_.Clone(&dest->set_option_attributes_);
#endif
    dest->owner_ = this->owner_;
    CheckMemoryTest();
}

// CHANGES: f110802.2L
void PlayerAttributes::CheckMemoryTest()
{
#if defined(_NA000000_SPEED_HACK_PREVENTION_SUPPORT) && !defined(_SERVER)
    uint8_t test_block[sizeof(*this)];
    PlayerAttributes* const dummy = reinterpret_cast<PlayerAttributes*>(test_block);
    //
    CopyMemory(&dummy->m_STR, &this->m_STR,
        static_cast<size_t>(dummy->last_field_end_ - reinterpret_cast<int8_t*>(&dummy->m_STR)));

    if (AttrValue* const attr_value = &dummy->m_DEX)
    {
        attr_value->bound_type_ = eBOUNDTYPE_ZP_1000;
        attr_value->update_type_ = eUPDATETYPE_SUM_RATIO;
        attr_value->Update();
    };
    if (AttrValue* const attr_value = &dummy->m_MoveSpeedRatio)
    {
        attr_value->bound_type_ = eBOUNDTYPE_ZP_1000;
        attr_value->update_type_ = eUPDATETYPE_NOTHING;
        attr_value->Update();
        int calc_value = ns_formula::internal::CalcMoveSpeedRatio(
            dummy->m_DEX.calc_value_.value_,
            attr_value->item_value_.value_,
            attr_value->skill_value_.value_);

        calc_value *= (100 + static_cast<const AttrValue&>(*attr_value)[eATTR_KIND_RATIO]);
        calc_value /= 100;
        attr_value->calc_value_.value_ = calc_value;
        attr_value->Update();
    };
    if (AttrValue* const attr_value = &dummy->m_AttSpeedRatio)
    {
        attr_value->bound_type_ = eBOUNDTYPE_ZP_1000;
        attr_value->update_type_ = eUPDATETYPE_NOTHING;
        attr_value->Update();
        int calc_value = ns_formula::internal::CalcAttackSpeedRatio(
            owner_->GetCharType(),
            owner_->GetWeaponKind(),
            dummy->m_DEX.calc_value_.value_,
            attr_value->item_value_.value_,
            attr_value->skill_value_.value_);

        calc_value *= (100 + static_cast<const AttrValue&>(*attr_value)[eATTR_KIND_RATIO]);
        calc_value /= 100;
        attr_value->calc_value_.value_ = calc_value;
        attr_value->Update();
    };

    if (::memcmp(&dummy->m_STR, &m_STR,
            static_cast<size_t>(dummy->last_field_end_ -
                                reinterpret_cast<int8_t*>(&dummy->m_STR))) != 0)
    {
        nsSync::MemoryHackMonitor::Instance()->ForceIncreaseClientTestSerial();
    };
    // forcely cleared.
    RtlSecureZeroMemory(test_block, sizeof(test_block));
#endif
}

//==================================================================================================

void PlayerAttributes::SetCharInfo(const BASE_PLAYERINFO* player_info,
                                   int recover_hp, int recover_mp, int recover_sd)
{
    assert(player_info != NULL);

    // �⺻ ���ȵ� [eATTR_KIND_BASE]
    m_STR.base_value_.value_ = player_info->m_sStrength;
    m_DEX.base_value_.value_ = player_info->m_sDexterity;
    m_VIT.base_value_.value_ = player_info->m_sVitality;
    m_INT.base_value_.value_ = player_info->m_sInteligence;
    m_SPR.base_value_.value_ = player_info->m_sSpirit;
    ;{  // __NA_S00015_20080828_NEW_HOPE_SHADOW
        m_STR.Update();
        m_DEX.Update();
        m_VIT.Update();
        m_INT.Update();
        m_SPR.Update();
    };
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // ���õ�
    m_Experty1.base_value_.value_ = player_info->m_sSkillStat1;
    m_Experty2.base_value_.value_ = player_info->m_sSkillStat2;
#endif
    // �þ�
    m_SightRange.base_value_.value_ = 130;

    // HP, MP ȸ����
    m_RecoverHP.base_value_.value_ = recover_hp;
    m_RecoverMP.base_value_.value_ = recover_mp;
    m_RecoverSD.base_value_.value_ = recover_sd;

    // �ִ� HP, MP
    const eCHAR_TYPE char_type = owner_->GetCharType();
    const LEVELTYPE char_level = owner_->GetLevel();
    m_MaxHP.base_value_.value_ = CalcHP(char_type, char_level, GetVIT());
    m_MaxMP.base_value_.value_ = CalcMP(char_type, char_level, GetSPR());
    m_MaxSD.base_value_.value_ = CalcSD(char_type, char_level);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // �ִ� FP, FPȸ����
    m_RecoverFP.base_value_.value_ = 0;
    m_MaxFP.base_value_.value_ = CalcFP(char_type);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    m_RiderSpeedRatio.Init();
}

//==================================================================================================

#if defined(_SERVER)

// NOTE, f100828.1L, change a name of 'GetCharInfo' to 'SynchronizeBaseStatus'.
// this method use to synchronizing
// between a base status value of BASE_PLAYERINFO and it of Attributes.
void PlayerAttributes::SynchronizeBaseStatus() //GetCharInfo(BASE_PLAYERINFO* OUT pInfo);
{
    BASE_PLAYERINFO* player_info = owner_->GetCharacterInfo();
    assert(player_info != NULL);
    player_info->m_sStrength    = static_cast<short>(m_STR.base_value_.value_);
    player_info->m_sDexterity   = static_cast<short>(m_DEX.base_value_.value_);
    player_info->m_sVitality    = static_cast<short>(m_VIT.base_value_.value_);
    player_info->m_sInteligence = static_cast<short>(m_INT.base_value_.value_);
    player_info->m_sSpirit      = static_cast<short>(m_SPR.base_value_.value_);
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    player_info->m_sSkillStat1  = 0;
    player_info->m_sSkillStat2  = 0;
#else
    player_info->m_sSkillStat1  = static_cast<short>(m_Experty1.base_value_);
    player_info->m_sSkillStat2  = static_cast<short>(m_Experty2.base_value_);
#endif
    player_info->m_iMaxHP       = m_MaxHP.base_value_.value_;
    player_info->m_iMaxMP       = m_MaxMP.base_value_.value_;
    player_info->m_iMaxSD       = m_MaxSD.base_value_.value_;
}
#else //if !defined(_SERVER)
// NOTE, f100829.1L, rollback to support a client logic
void PlayerAttributes::GetCharInfo(BASE_PLAYERINFO* OUT player_info)
{
    player_info->m_sStrength    = static_cast<short>(m_STR.base_value_.value_);
    player_info->m_sDexterity   = static_cast<short>(m_DEX.base_value_.value_);
    player_info->m_sVitality    = static_cast<short>(m_VIT.base_value_.value_);
    player_info->m_sInteligence = static_cast<short>(m_INT.base_value_.value_);
    player_info->m_sSpirit      = static_cast<short>(m_SPR.base_value_.value_);

  #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    player_info->m_sSkillStat1 = 0;
    player_info->m_sSkillStat2 = 0;
  #else
    player_info->m_sSkillStat1  = static_cast<short>(m_Experty1.base_value_);
    player_info->m_sSkillStat2  = static_cast<short>(m_Experty2.base_value_);
  #endif

    player_info->m_iMaxHP       = m_MaxHP.base_value_.value_;
    player_info->m_iMaxMP       = m_MaxMP.base_value_.value_;
    player_info->m_iMaxSD       = m_MaxSD.base_value_.value_;
}
#endif

//==================================================================================================

void PlayerAttributes::Update()
{
    //----------------------------------------------------------------------------------------------
    // early instance for busy access fields
    const eCHAR_TYPE char_type = owner_->GetCharType();
    const LEVELTYPE char_level = owner_->GetLevel();
    const DWORD weapon_kind = owner_->GetWeaponKind();
    //----------------------------------------------------------------------------------------------
    // �⺻ ���ȵ�
    ;{
        m_STR.Update();
        m_DEX.Update();
        m_VIT.Update();
        m_INT.Update();
        m_SPR.Update();
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        m_Experty1.Update();
        m_Experty2.Update();
    #endif
        // ������ �̿��Ͽ� HP, MP ���
        m_MaxHP.base_value_.value_ = CalcHP(char_type, char_level, GetVIT());
        m_MaxMP.base_value_.value_ = CalcMP(char_type, char_level, GetSPR());
        m_MaxSD.base_value_.value_ = CalcSD(char_type, char_level);
        m_MaxHP.Update();
        m_MaxMP.Update();
        m_MaxSD.Update();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        m_MaxFP.Update();
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        // NOTE, update attribute values to player info. a real process of this call is server only.
        SynchronizeBaseStatus();
    };
    //----------------------------------------------------------------------------------------------
    const short stat_dex = static_cast<short>(GetDEX());

    //----------------------------------------------------------------------------------------------
    // HP, MP, SD ȸ����
    UpdateRecoveries(char_type);
    //----------------------------------------------------------------------------------------------
    // Attack power & Defence power
    ;{
        UpdateAttackPower();
        UpdateDefense();
    };
    //----------------------------------------------------------------------------------------------
    ;{
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        // ���� Ÿ�Ժ� �߰� ����
        for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
        {
            m_BonusDefense[i].Update();
        }
        // �Ƹ� Ÿ�Ժ� �߰� ������(�ۼ�Ʈ)
        for (int i = eARMOR_HARD; i < eARMOR_TYPE_MAX; ++i)
        {
            m_BonusDamage[i].Update();
            m_BonusPercentDamage[i].Update();
        }
        // ������ ����
        for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
        {
            m_ReduceDamage[i].Update();
        }
        //
        for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
        {
            m_ReduceDefenseRate[i].Update();
        }
    #endif
    };
    //----------------------------------------------------------------------------------------------
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    if (AttrValue* attr = &m_PhysicalAttackRate)
    {   // ���� ������ (����:percentage * 100)
        attr->base_value_.value_ = CalcPhysicalAttackRateBase(char_type, char_level, stat_dex);
        double calc = (attr->item_value_.value_ + attr->skill_value_.value_) *
                      ns_formula::AttackRateConstants_EquipmentWeight;
        int ratio = attr->item_ratio_.value_ + attr->skill_ratio_.value_;
        if (ratio != 0 && calc != 0.0f) {
            calc *= (100.0f + ratio);
            calc /= 100.0f;
        }

        attr->calc_value_.value_ = util::LL_Round(attr->base_value_.value_ + calc);
        attr->Update();
    }
    if (AttrValue* attr = &m_PhysicalAvoidRate)
    {   // ���� ����� (����:percentage * 100)
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        attr->base_value_.value_ = CalcPhysicalAvoidRateBase(char_type, char_level, (short)GetVIT());
#else
        attr->base_value_.value_ = CalcPhysicalAvoidRateBase(char_type, char_level, stat_dex);
#endif //_NA_008486_20150914_TOTAL_BALANCE
        // CHANGES: f110209.6L, changes formula by planner request
        int option_sum_value = attr->item_value_.value_ + attr->skill_value_.value_;
        double calc = m_ArmorPhysicalAvoidRate.item_value_.value_ *
                      ns_formula::AttackAvoidRateConstants_EquipmentWeight;
        ;      calc += (attr->base_value_.value_ + option_sum_value);
        int ratio = attr->item_ratio_.value_ + attr->skill_ratio_.value_;
        if (ratio != 0 && calc != 0.0f) {
            calc *= (100.0f + ratio);
            calc /= 100.0f;
        }
        attr->calc_value_.value_ = util::LL_Round(calc);
        attr->Update();
    }
#else
    ;{
        // ���� ������, ȸ����
        m_PhysicalAttackRate.base_value_.value_ = \
            CalcPhysicalAttackRateBase(char_type, char_level, stat_dex);
        m_PhysicalAvoidRate.base_value_.value_ = \
            CalcPhysicalAvoidRateBase(char_type, char_level, stat_dex);
        m_PhysicalAttackRate.Update();
        m_PhysicalAvoidRate.Update();
    };
#endif
    //----------------------------------------------------------------------------------------------
    // range attributes
    ;{
        
        m_MoveSpeedRatio.Update();
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        int calc_value = CalcMoveSpeedRatio(m_MoveSpeedRatio.item_value_.value_, 
                                            m_MoveSpeedRatio.skill_value_.value_, char_level);
#else
        int calc_value = CalcMoveSpeedRatio(stat_dex, \
            m_MoveSpeedRatio.item_value_.value_, m_MoveSpeedRatio.skill_value_.value_);
#endif //_NA_008486_20150914_TOTAL_BALANCE

        calc_value *= (100 + static_cast<const AttrValue&>(m_MoveSpeedRatio)[eATTR_KIND_RATIO]);
        calc_value /= 100;
        m_MoveSpeedRatio.calc_value_.value_ = calc_value;
        m_MoveSpeedRatio.calc_value_.touch(static_cast<eBOUNDTYPE>(m_MoveSpeedRatio.bound_type_));
    };
    ;{
        m_AttSpeedRatio.Update();
        int calc_value = CalcAttackSpeedRatio(char_type, weapon_kind, stat_dex, \
            m_AttSpeedRatio.item_value_, m_AttSpeedRatio.skill_value_);
        calc_value *= (100 + static_cast<const AttrValue&>(m_AttSpeedRatio)[eATTR_KIND_RATIO]);
        calc_value /= 100;
        m_AttSpeedRatio.calc_value_.value_ = calc_value;
        m_AttSpeedRatio.calc_value_.touch(static_cast<eBOUNDTYPE>(m_AttSpeedRatio.bound_type_));
    };
    ;{
        // ��Ÿ� ���ʽ�
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
        {
            eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
            const int apply_value = static_cast<const AttrValue&>(m_AllRangeBonus)[attr_kind];
            if (apply_value)
            {
                m_NormalRangeBonus[attr_kind] += apply_value;
                m_SkillRangeBonus[attr_kind] += apply_value;
            }
        }
        m_NormalRangeBonus.Update();
        m_SkillRangeBonus.Update();
        m_AllRangeBonus.Init(); // <- 

        // �þ�
        m_SightRange.Update();
        m_RiderSpeedRatio.Update();
    };
    ;{
        // ũ��Ƽ�� ������
        m_CriticalRatioChange.base_value_.value_ = \
            CalcPhyCriticalBaseRatio(char_type, char_level, stat_dex);
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        m_MagicCriticalRatio.base_value_.value_ = \
            CalcMagicCriticalBaseRatio(char_type, char_level, GetSPR());
    #endif

        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
        {
            eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
            const int apply_value = \
                static_cast<const AttrValue&>(m_CriticalRatioBonus)[attr_kind];
            if (apply_value)
            {
                m_CriticalRatioChange[attr_kind] += apply_value;
            #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
                m_MagicCriticalRatio[attr_kind] += apply_value;
            #endif
            }
        }
        m_CriticalRatioChange.Update();
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        m_MagicCriticalRatio.Update();
    #endif
        m_CriticalRatioBonus.Init();
        //
        m_CriticalDamageBonus.Update();
    };
    ;{
        // ��ų ���ݷ�, ��ų �ۼ�Ʈ �߰� ������
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        m_SkillAttackPower.base_value_.value_ = CalcSkillAttackPower(char_type, GetINT());
    #endif
        m_SkillAttackPower.Update();
        m_SkillPercentDamage.Update();
    };
    ;{
        m_AbsorbHP.Update();
        m_AbsorbMP.Update();
    };
    ;{
        m_BonusCastingTime.Update();
        m_BonusSkillCoolTime.Update();
        m_IncreaseSkillDuration.Update();
        m_DecreaseSkillDuration.Update();
    };
    ;{
        m_AttackIncRate.Update();
        m_DefenseIncRate.Update();
        m_AreaAttackRatio.Update();
        m_ReflectDamageRatio.Update();
        m_DecDamage.Update();
        m_DoubleDamageRatio.Update();   // �������� �ι谡 ����� Ȯ��
        //m_DoubleDamageRatio.Update();
        m_IncreaseMinDamage.Update();
        m_IncreaseMaxDamage.Update();
        m_AddDamage.Update();
    };
    ;{
        m_EhterDamageRatio.Update();
        m_EhterPvEDamageRatio.Update(); // NOTE: f110601.4L
        m_DecreasePVPDamage.Update();
        m_LuckMonIncDamage.Update();    // ��Ű ���� �߰� ������
        //m_LuckMonIncDamage.Update();
        m_BypassDeffenceRatio.Update();

#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
        m_DecreaseDamageNPC.Update();
        m_DecreaseDamageBerserker.Update();
        m_DecreaseDamageDragonKnight.Update();
        m_DecreaseDamageValkyrie.Update();
        m_DecreaseDamageElementalist.Update();
        m_DecreaseDamageShadow.Update();
        m_DecreaseDamageMystic.Update();
        m_DecreaseDamageHellroid.Update();
        m_DecreaseDamageWitchBlade.Update();
#endif //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    };
    ;{
        m_IncreaseReserveHPRatio.Update();
        m_MPSpendIncrease.Update();
        m_ResistBadStatusRatio.Update();
    };
    ;{
        m_SkillLevelAll.Update();
        m_DecreaseLimitStat.Update();
        m_DecreaseItemDuraRatio.Update();
        m_BonusMoneyRatio.Update();
        m_BonusExpRatio.Update();
        m_ResurrectionRatio.Update();   // ����� ������ ��Ȱ �� Ȯ��
        m_CompositeIncRatio.Update();    // ���� ������ ����    
        m_AutoPickup.Update();
        m_IncreaseEnchantRatio.Update(); // ��þƮ ������ ����
    };
    ;{  //_NA_001231_20081216_ADD_OPTION_KIND
        m_ResistHolding.Update();
        m_ResistSleep.Update();
        m_ResistPoison.Update();
        m_ResistKnockBack.Update();
        m_ResistDown.Update();
        m_ResistStun.Update();
    };

    ;{
        m_PCBang.Update();
    };

#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    m_EnemyCriticalRatioChange.Update();
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    ;{
        m_AttackDamageAbsorbSDRatio.Update();
        m_AttackDamageAbsorbHPRatio.Update();
    };
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ;{
        m_CraftCostRatio.Update();
        m_CraftPreventExtinctionMaterialRatio.Update();
        m_EnchantCostRatio.Update();
        m_EnchantPreventDestroyNDowngradeItemRatio.Update();
        m_RecoverPotionCooltimeRatio.Update();
        m_RecoverPotionRecoveryRatio.Update();
        m_QuestRewardExpRatio.Update();
        m_MaxDamageRatio.Update();
        m_DominationMapobjectDamageRatio.Update();
        m_ShopRepairHeimRatio.Update();
        m_ShopBuyHeimRatio.Update();
    };
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    ;{
        m_IncreaseDamageRatio.Update();
    };
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    ;{
        m_DebuffDuration.base_value_.value_ = CalcDebuffDuration((short)GetSPR());
        m_DebuffDuration.Update();
    };
#endif //_NA_008486_20150914_TOTAL_BALANCE

}

//==================================================================================================

void PlayerAttributes::UpdateAttackPower()
{
    //----------------------------------------------------------------------------------------------
    // early instance for busy access fields
    const eCHAR_TYPE char_type = owner_->GetCharType();
    //const LEVELTYPE char_level = owner_->GetLevel();
    //const DWORD weapon_kind = owner_->GetWeaponKind();
    const short stat_str = static_cast<short>(GetSTR());
    const short stat_dex = static_cast<short>(GetDEX());
    const short stat_int = static_cast<short>(GetINT());
    //----------------------------------------------------------------------------------------------
    // �⺻ ���ݷ� ���
    //-----------------------------------------------------------------
    // �ٰŸ�, ���Ÿ�
    m_BaseMeleeMinAttPower.base_value_.value_ = CalcMinMeleeAttackPower(char_type, stat_str, stat_dex);
    m_BaseMeleeMaxAttPower.base_value_.value_ = CalcMaxMeleeAttackPower(char_type, stat_str, stat_dex);
    m_BaseRangeMinAttPower.base_value_.value_ = CalcMinRangeAttackPower(char_type, stat_str, stat_dex);
    m_BaseRangeMaxAttPower.base_value_.value_ = CalcMaxRangeAttackPower(char_type, stat_str, stat_dex);
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // ����
    m_BaseMagicMinAttPower.base_value_.value_ = CalcMagicAttackPower(true, stat_int);
    m_BaseMagicMaxAttPower.base_value_.value_ = CalcMagicAttackPower(false, stat_int);
#endif
    // ���ݿ� ���� Base + �����ۿ� ���� Base
    m_BaseMeleeMinAttPower.Update();
    m_BaseMeleeMaxAttPower.Update();
    m_BaseRangeMinAttPower.Update();
    m_BaseRangeMaxAttPower.Update();
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    m_BaseMagicMinAttPower.Update();
    m_BaseMagicMaxAttPower.Update();
#endif
    //-----------------------------------------------------------------
    // �ɼ� ���ݷ�
    //-----------------------------------------------------------------
    // �ɼ� ��ü(����, ����) ���ݷ��� �����Ѵ�.
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    m_OptionPhysicalAttPower.Update();
#else
    for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
    {
        eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
        const int apply_value = \
            static_cast<const AttrValue&>(m_OptionAllAttPower)[attr_kind];
        if (apply_value)
        {
            m_OptionPhysicalAttPower[attr_kind] += apply_value;
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            m_OptionMagicAttPower[attr_kind] += apply_value;
        #endif
        };
    }

    // �ɼ� ���ݷ��� eATTR_KIND_CALC ���� ����Ѵ�.
    m_OptionPhysicalAttPower.Update();
    m_OptionMagicAttPower.Update();
    // ��ü �ݿ� �Ķ������ ��� �ʱ�ȭ�� �ؾ� �������� �ٽ� ������ �ʴ´�.
    m_OptionAllAttPower.Init();
#endif
    //
    int iAveragePhyAttPower = (GetBaseMeleeMinAttPower() + GetBaseMeleeMaxAttPower() +
                               GetBaseRangeMinAttPower() + GetBaseRangeMaxAttPower()) / 4;

    m_OptionPhysicalAttPower.calc_value_.value_ += \
        (iAveragePhyAttPower + GetOptionPhysicalAttPower()) * \
        static_cast<const AttrValue&>(m_OptionPhysicalAttPower)[eATTR_KIND_RATIO] / 100;
    m_OptionPhysicalAttPower.calc_value_.touch(\
        static_cast<eBOUNDTYPE>(m_OptionPhysicalAttPower.bound_type_));
    //
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    int iAverageMagicAttPower = (GetBaseMagicMinAttPower() + GetBaseMagicMaxAttPower()) / 2;
    m_OptionMagicAttPower.calc_value_.value_ +=
        (iAverageMagicAttPower + GetOptionMagicAttPower()) *
        static_cast<const AttrValue&>(m_OptionMagicAttPower)[eATTR_KIND_RATIO] / 100;
    m_OptionMagicAttPower.calc_value_.touch(\
        static_cast<eBOUNDTYPE>(m_OptionMagicAttPower.bound_type_));
#endif

    //-----------------------------------------------------------------
    // �Ӽ� ���ݷ�
    //-----------------------------------------------------------------
    // �Ӽ� ��ü ���ݷ��� �����Ѵ�.
    ;{
        int magical_all_attacks[eATTR_KIND_RATIO - eATTR_KIND_ITEM + 1];
        int* apply_value_it = magical_all_attacks;
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i) {
            *apply_value_it++ = \
                static_cast<const AttrValue&>(m_MagicalAllAttPower)[static_cast<eATTR_KIND>(i)];
        }
        for (int j = eATTACK_TYPE_WATER; j <= eATTACK_TYPE_DARKNESS; ++j)
        {
            AttrValue& attr_value = m_MagicalAttPower[j];
            apply_value_it = magical_all_attacks;
            // f100930.6L, fixed a missing increase iteration
            for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i, ++apply_value_it)
            {
                eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
                if (int apply_value = *apply_value_it) {
                    attr_value[attr_kind] += apply_value;
                }
            }
        }
    #if SUN_CODE_BACKUP
        for (int i = eATTACK_TYPE_WATER; i <= eATTACK_TYPE_DARKNESS; ++i)
        {
            for (int j = eATTR_KIND_ITEM; j <= eATTR_KIND_RATIO; ++j)
            {
                eAttrKind = (eATTR_KIND)j;
                m_MagicalAttPower[i][eAttrKind] += m_MagicalAllAttPower[eAttrKind];
            }
        }
    #endif
    };

    // �Ӽ� ���ݷ��� eATTR_KIND_CALC ���� ����Ѵ�.
    for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
    {
        m_MagicalAttPower[i].Update();
    }

    // ��ü �ݿ� �Ķ������ ��� �ʱ�ȭ�� �ؾ� �������� �ٽ� ������ �ʴ´�.
    m_MagicalAllAttPower.Init();
}

//==================================================================================================

void PlayerAttributes::UpdateDefense()
{
    //----------------------------------------------------------------------------------------------
    // early instance for busy access fields
    const eCHAR_TYPE char_type = owner_->GetCharType();
    //const LEVELTYPE char_level = owner_->GetLevel();
    //const DWORD weapon_kind = owner_->GetWeaponKind();
    const int stat_vit = static_cast<short>(GetVIT());
    const short stat_spr = static_cast<short>(GetSPR());
    //----------------------------------------------------------------------------------------------
    ;{
        // �⺻ ���� ���
        //-----------------------------------------------------------------
        // �ٰŸ�, ���Ÿ�
        DWORD physicalDefense = CalcPhyBaseDef(char_type, stat_vit);
        m_BaseMeleeDefPower.base_value_.value_ = physicalDefense;
        m_BaseRangeDefPower.base_value_.value_ = physicalDefense;
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        // ����
        m_BaseMagicDefPower.base_value_.value_ = CalcMagicBaseDef(char_type, stat_spr);
    #endif
        // ���ݿ� ���� Base + �����ۿ� ���� Base
        m_BaseMeleeDefPower.Update();
        m_BaseRangeDefPower.Update();
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        m_BaseMagicDefPower.Update();
    #endif
    };
    ;{
        //-----------------------------------------------------------------
        // �ɼ� ����
        //-----------------------------------------------------------------
        // �ɼ� ��ü(����, ����) ������ �����Ѵ�.
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        m_OptionPhysicalDefPower.Update();
    #else
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i)
        {
            eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
            const int apply_value = \
                static_cast<const AttrValue&>(m_OptionAllDefPower)[attr_kind];
            if (apply_value)
            {
                m_OptionPhysicalDefPower[attr_kind] += apply_value;
            #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
                m_OptionMagicDefPower[attr_kind] += apply_value;
            #endif
            }
        }
        // �ɼ� ���ݷ��� eATTR_KIND_CALC ���� ����Ѵ�.
        m_OptionPhysicalDefPower.Update();
        m_OptionMagicDefPower.Update();
        // ��ü �ݿ� �Ķ������ ��� �ʱ�ȭ�� �ؾ� �������� �ٽ� ������ �ʴ´�.
        m_OptionAllDefPower.Init();
    #endif
        //
        int iAverageDefPower = (GetBaseMeleeDefPower() + GetBaseRangeDefPower()) / 2;
        // �ɼ� ���±��� �����ؼ� ������ �����Ų��.
        m_OptionPhysicalDefPower.calc_value_.value_ += \
            (iAverageDefPower + GetOptionPhysicalDefPower()) * \
            static_cast<const AttrValue&>(m_OptionPhysicalDefPower)[eATTR_KIND_RATIO] / 100;
        m_OptionPhysicalDefPower.calc_value_.touch(\
            static_cast<eBOUNDTYPE>(m_OptionPhysicalDefPower.bound_type_));
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        //
        m_OptionMagicDefPower.calc_value_.value_ += \
            (GetBaseMagicDefPower() + GetOptionMagicDefPower()) * \
            static_cast<const AttrValue&>(m_OptionMagicDefPower)[eATTR_KIND_RATIO] / 100;
        m_OptionMagicDefPower.calc_value_.touch(\
            static_cast<eBOUNDTYPE>(m_OptionMagicDefPower.bound_type_));
    #endif
    };
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    ;{
        // ���� ��� ���� ������ (EP1: same as m_ReduceDefenseRate)
        m_ReducePhysicalTargetDefenseRatio.Update();
#ifdef _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET
        m_ReduceElementTargetDefenseRatioAll.Update();
#endif  // _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET
        const int all_apply_value = m_ReduceElementTargetDefenseRatioAll.calc_value_.value_;
        for (AttrValue* it = m_ReduceElementTargetDefenseRatio;
             it != &m_ReduceElementTargetDefenseRatio[_countof(m_ReduceElementTargetDefenseRatio)];
             ++it)
        {
            it->Update();
            it->calc_value_.value_ += all_apply_value;
            it->calc_value_.touch(static_cast<eBOUNDTYPE>(it->bound_type_));
        }
    };
    ;{
        // ���ݿ� ���� ������ ������
        m_ReducePhysicalDamageRatio.Update();
        m_ReduceElementDamageRatioAll.Update();
        const int all_apply_value = m_ReduceElementDamageRatioAll.calc_value_.value_;
        // ���� ���� ranges = [-100%, +XXX% ]
        for (AttrValue* it = m_ReduceElementDamageRatio;
             it != &m_ReduceElementDamageRatio[_countof(m_ReduceElementDamageRatio)];
             ++it)
        {
            it->Update();
            it->calc_value_.value_ += all_apply_value;
            it->calc_value_.touch(static_cast<eBOUNDTYPE>(it->bound_type_));
        }
    };
#else //if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    ;{
        //-----------------------------------------------------------------
        // �Ӽ� ����
        //-----------------------------------------------------------------
        // �Ӽ� ��ü ������ �����Ѵ�.
        int magical_all_defences[eATTR_KIND_RATIO - eATTR_KIND_ITEM + 1];
        int* apply_value_it = magical_all_defences;
        for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i) {
            *apply_value_it++ = \
                static_cast<const AttrValue&>(m_MagicalAllDefPower)[static_cast<eATTR_KIND>(i)];
        }
        for (int j = eATTACK_TYPE_WATER; j <= eATTACK_TYPE_DARKNESS; ++j)
        {
            AttrValue& attr_value = m_MagicalDefPower[j];
            apply_value_it = magical_all_defences;
            // f100930.6L, fixed a missing increase iteration
            for (int i = eATTR_KIND_ITEM; i <= eATTR_KIND_RATIO; ++i, ++apply_value_it)
            {
                eATTR_KIND attr_kind = static_cast<eATTR_KIND>(i);
                if (int apply_value = *apply_value_it) {
                    attr_value[attr_kind] += apply_value;
                }
            }
        }
    #if SUN_CODE_BACKUP
        for (int i = eATTACK_TYPE_WATER; i <= eATTACK_TYPE_DARKNESS; ++i)
        {
            for (int j = eATTR_KIND_ITEM; j <= eATTR_KIND_RATIO; ++j)
            {
                eAttrKind = (eATTR_KIND)j;
                m_MagicalDefPower[i][eAttrKind] += m_MagicalAllDefPower[eAttrKind];
            }
        }
    #endif
        // �Ӽ� ������ eATTR_KIND_CALC ���� ����Ѵ�.
        for (int i = 0; i < eATTACK_TYPE_MAX; ++i)
        {
            m_MagicalDefPower[i].Update();
        }

        // ��ü �ݿ� �Ķ������ ��� �ʱ�ȭ�� �ؾ� �������� �ٽ� ������ �ʴ´�.
        m_MagicalAllDefPower.Init();
    };
#endif
}

#ifdef _SERVER
//==================================================================================================
// CHANGES: f110413.1L, added interface to support light recovery quantity calculation for server
// NOTE: HP, MP and SD recovery have busy update processes.
// if server is to avoid busy updating, detach the recovery parts from full update calling.
// the usage of this method is calling on changed recoveries in run-time service
// not to initialize like the 'SetCharInfo'
void PlayerAttributes::UpdateChangedRecoveries(const BASE_PLAYERINFO* player_info,
    bool hp_update, int hp_recovery_quantity,
    bool mp_update, int mp_recovery_quantity,
    bool sd_update, int sd_recovery_quantity)
{
    bool need_update = false;
    if (hp_update) {
        m_RecoverHP.base_value_.value_ = hp_recovery_quantity;
        need_update = true;
    };
    if (mp_update) {
        m_RecoverMP.base_value_.value_ = mp_recovery_quantity;
        need_update = true;
    };
    if (sd_update) {
        m_RecoverSD.base_value_.value_ = sd_recovery_quantity;
        need_update = true;
    };

    if (need_update) {
        // HP, MP and SD recovery don't effect other attributes, also don't notify to client
        UpdateRecoveries(static_cast<eCHAR_TYPE>(player_info->m_byClassCode));
    };
}

// CHANGES: f110413.1L
void PlayerAttributes::UpdateRecoveries(eCHAR_TYPE char_type)
{
    m_RecoverHP.Update();
    // CHANGES: f110322.4L, prevent the berserker's recovery quantity
    // that has over than minus value by items or skills attribute value
    // unlike plan's requirements.
    if (char_type != eCHAR_BERSERKER) {
        m_RecoverMP.Update();
    }
    else {
        m_RecoverMP.calc_value_.value_ = -1;
    }
    m_RecoverSD.Update();
}
#endif //

//==================================================================================================

void    PlayerAttributes::Display()
{
    //  (WAVERIX)(NOTE!)
    //      �������� ������ϱ� ���� �� ���� ���� ���� ������ �����ϳ׿�. -.-;;; �����սô�...
    //      ������ ����ڸ� �ϴ°� �ƴմϴ�. �������� ��ų ���� ���Ѿ���. ��¯ ���繬�� ���� �������ֽñ�...
    //      �� �ð� 07/11/13 16:00 - �̰� ���� ���� �� �����׿�...�Ф�
#ifdef _DEBUG
    SUNLOG(eDEV_LOG,
           _T("============================ Display Attribute ==============================="));
    SUNLOG(eDEV_LOG, _T("|MaxHP=%d|MaxMP=%d|MaxSD=%d|"), GetMaxHP(), GetMaxMP(), GetMaxSD());
    SUNLOG(eDEV_LOG, _T("|RecoverHPRatio=%d|RecoverMPRatio=%d|RecoverSDRatio=%d|"),
           GetRecoverHP(), GetRecoverMP(), GetRecoverSD());
           //_T("==============================================================================")

    // ���ݷ� 
    // ���ݿ� ���� Base + �����ۿ� ���� Base
    SUNLOG(eDEV_LOG, "�⺻�����ּ� ���ݷ� = %d, �⺻�����ִ� ���ݷ� = %d, �ɼǹ��� ���ݷ� = %d", 
           GetBaseMeleeMinAttPower(), GetBaseMeleeMaxAttPower(), GetOptionPhysicalAttPower());
    SUNLOG(eDEV_LOG, "�⺻���Ÿ��ּ� ���ݷ�= %d, �⺻���Ÿ��ִ� ���ݷ�= %d",
           GetBaseRangeMinAttPower(), GetBaseRangeMaxAttPower());
    SUNLOG(eDEV_LOG, "�⺻�����ּ� ���ݷ�= %d, �⺻�����ִ� ���ݷ�= %d, �ɼǸ��� ���ݷ�= %d", 
           GetBaseMagicMinAttPower(), GetBaseMagicMaxAttPower(), GetOptionMagicAttPower());

    // ���� 
    // ���ݿ� ���� Base + �����ۿ� ���� Base
    SUNLOG(eDEV_LOG, "�⺻���� ����= %d, �ɼǹ��� ����= %d",
           GetBaseMeleeDefPower(), GetOptionPhysicalDefPower());
    SUNLOG(eDEV_LOG, "�⺻���Ÿ� ����= %d", GetBaseRangeDefPower());
    SUNLOG(eDEV_LOG, "�⺻���� ����= %d", GetBaseMagicDefPower());

    // ���� ������, ȸ����
    SUNLOG(eDEV_LOG, "���ݼ����� = %d, ����ȸ�Ƿ� = %d",
           GetPhysicalAttackRate(), GetPhysicalAvoidRate());
    // �ӵ�(�̵��ӵ�, ���ݼӵ�, �н�Ʈ��Ŀ����)
    SUNLOG(eDEV_LOG, "�̵��ӵ� = %d, ���ݼӵ� = %d ",
           GetMoveSpeedRatio(), GetAttSpeedRatio());
    // ��ų ���ݷ�, ��ų �ۼ�Ʈ �߰� ������
    SUNLOG(eDEV_LOG, "��ų���ݷ� = %d, ��ų �ۼ�Ʈ �߰� ������ = %d ",
           GetSkillAttackPower(), GetSkillPercentDamage());

    // �⺻ ���ȵ�
    SUNLOG(eDEV_LOG, "�� = %d, ��ø = %d, ü�� = %d, ���� = %d, ���ŷ� = %d", 
                      GetSTR(), GetDEX(), GetVIT(), GetINT(), GetSPR());
    SUNLOG(eDEV_LOG, "���õ�1 = %d, ���õ�2 = %d", GetExperty1(), GetExperty2());

    // �Ӽ� ���ݷ�
    SUNLOG(eDEV_LOG, "�Ӽ����ݷ� �� = %d, �� = %d, �ٶ� = %d, ���� = %d, ���� = %d ",
           GetMagicalAttackPower(eATTACK_TYPE_WATER), GetMagicalAttackPower(eATTACK_TYPE_FIRE),
           GetMagicalAttackPower(eATTACK_TYPE_WIND), GetMagicalAttackPower(eATTACK_TYPE_EARTH),
           GetMagicalAttackPower(eATTACK_TYPE_DARKNESS));
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
#else
    //�Ӽ� �濩��
    SUNLOG(eDEV_LOG, "�Ӽ����� �� = %d, �� = %d, �ٶ� = %d, ���� = %d, ���� = %d ",
           GetMagicalDefense(eATTACK_TYPE_WATER), GetMagicalDefense(eATTACK_TYPE_FIRE),
           GetMagicalDefense(eATTACK_TYPE_WIND), GetMagicalDefense(eATTACK_TYPE_EARTH),
           GetMagicalDefense(eATTACK_TYPE_DARKNESS));
#endif
    SUNLOG(
        eDEV_LOG, 
        "Ư�� �ɷ� ��Ȱ = %d, �������� = %d, ��Ű����ݷ� = %d, ���ռ����� = %d, ��þƮ������ = %d",
        GetResurrectionRatio(), 
        GetDoubleDamageRatio(),
        GetLuckMonIncDamage(), 
        GetCompositeIncRatio(), 
        GetIncreaseEnchantRatio()
    );

    SUNLOG(eDEV_LOG, "�ּҵ���������, ����(Ȯ��) = %d, %d, "
                     "�ִ� ������ ����, ����(Ȯ��)= %d, %d, ���¹��� Ȯ�� = %d ",
                     GetIncreaseMinDamage(), GetRatioIncreaseMinDamage(),
                     GetIncreaseMaxDamage(), GetRatioIncreaseMaxDamage(),
                     GetBypassDeffenceRatio());

    SUNLOG(eDEV_LOG, "�������ݷ� = %d, �������ݷ� = %d, "
                     "�������� = %d, �������� = %d, ������ ���� = %d(Ȯ��:%d)",
                     GetOptionPhysicalAttPower(), GetOptionMagicAttPower(),
                     GetOptionPhysicalDefPower(), GetOptionMagicDefPower(),
                     GetDecDamage(), GetRatioDecDamage());

    SUNLOG(eDEV_LOG, "������ = %d, ���ӽð����� = %d, ���ӽð� = %d, (�븻 = %d, ��ų = %d)",
                     GetDecreaseItemDuraRatio(), GetIncreaseSkillDuration(),
                     GetDecreaseSkillDuration(), GetNormalRangeBonus(), GetSkillRangeBonus());

    SUNLOG(eDEV_LOG, "���׸� ������ ��ȭ: %d", GetEhterDamageRatio());

    // �Ӽ� ���ݷ�
    SUNLOG(eDEV_LOG, "���� ����� : ��ü = %d, Ȧ�� = %d, ���� = %d, "
                     "�ߵ� = %d, �˹� = %d, �ٿ� = %d, ���� = %d, pvp ���� = %d(Ȯ��:%d)",
                     GetResistBadStatusRatio(), GetResistHolding(), GetResistSleep(),
                     GetResistPoison(), GetResistKnockBack(), GetResistDown(),
                     GetResistStun(), GetDecreasePVPDamage(), GetRatioDecreasePVPDamage());
#endif //_DEBUG
}

