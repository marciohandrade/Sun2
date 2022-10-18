#include "stdafx.h"
#include "AbilityFactory.h"

#include <boost/shared_ptr.hpp>

#include "SkillInfoParser.h"
#include "Skill.h"
#include "BuffStatusAbility.h"
#include "DamageAbility.h"
#include "PeriodicDamageAbility.h"
#include "ThrustAbility.h"
#include "ExhaustAbility.h"
#include "AggroAbility.h"
#include "FightingEnergyAbility.h"
#include "TeleportAbility.h"
#include "ResurrectionAbility.h"
#include "PeriodicEffectAbility.h"
#include "SPDamageAbility.h"
#include "WeaponMasteryAbility.h"
#include "MonsterTransAbility.h"
#include "WindShieldAbility.h"
#include "PassiveStatusAbility.h"
#include "StunAbility.h"
#include "DragonTransAbility.h"
#include "SummonAbility.h"
#include "BombEffectAbility.h"
#include "AbsorbAbility.h"
#include "SummonToDeathAbility.h"
#include "AbnormalAbility.h"
#include "MirrorAbility.h"
#include "MagicShieldAbility.h"
#include "LowHPAbility.h"
#include "CureAbility.h"
#include "BonusDamageAbility.h"
#include "CureAbility.h"
#include "InformationAbility.h"
#include "SummonDeadMonsterAbility.h"
#include "BlockSkillAbility.h"
#include "SuctionAbility.h"
#include "CancelStatusAbility.h"
#include "VitalSuctionAbility.h"
#include "TransparentAbility.h"
#include "SelfDestructionAbility.h"
#include "ChangeAttrAbility.h"
#include "SummonCrystalWarpAbility.h"
#include "BonusDamagePerStatusAbility.h"
#include "SkillSystem/Ability/CurrentHPIncreaseAbility.h"
#include "QuickStepAbility.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "SkillSystem/Ability/FPChangeAbility.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

class AbilityFactory::Allocator
{
public:
    Allocator()
    {
    }
    ~Allocator()
    {
    }

public:
    void Init()
    {
        // 개별적으로 구현된 특화 어빌리티 풀을 등록한다.
        pools_[eABILITY_DAMAGE] = PoolPtr(new AbilityPool<DamageAbility>());
        pools_[eABILITY_DAMAGE_PER_TIME] = PoolPtr(new AbilityPool<PeriodicDamageAbility>());
        pools_[eABILITY_CUR_HP_INCREASE] = PoolPtr(new AbilityPool<CurrnetHPIncreaseAbility>());
        pools_[eABILITY_KNOCKBACK] = PoolPtr(new AbilityPool<ThrustAbility>());
        pools_[eABILITY_KNOCKBACK2] = PoolPtr(new AbilityPool<ThrustAbility>());
        pools_[eABILITY_SELF_DESTRUCTION] = PoolPtr(new AbilityPool<ThrustAbility>());
        pools_[eABILITY_EXHAUST_HP] = PoolPtr(new AbilityPool<ExhaustAbility>());
        pools_[eABILITY_EXHAUST_MP] = PoolPtr(new AbilityPool<ExhaustAbility>());
        pools_[eABILITY_AGGROPOINT_INCREASE] = PoolPtr(new AbilityPool<AggroAbility>());
        pools_[eABILITY_FIGHTING_ENERGY_NUM_INCREASE] = PoolPtr(new AbilityPool<FightingEnergyAbility>());
        pools_[eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY] = PoolPtr(new AbilityPool<FightingEnergyAbility>());
        pools_[eABILITY_TELEPORT] = PoolPtr(new AbilityPool<TeleportAbility>());
        pools_[eABILITY_RESURRECTION] = PoolPtr(new AbilityPool<ResurrectionAbility>());
        pools_[eABILITY_BUFF_RANGE_DAMAGE] = PoolPtr(new AbilityPool<PeriodicEffectAbility>());
        pools_[eABILITY_AURORA] = PoolPtr(new AbilityPool<AuroraAbility>());
        pools_[eABILITY_BONUS_DAMAGE_PER_SP] = PoolPtr(new AbilityPool<SPDamageAbility>());
        pools_[eABILITY_BONUS_DAMAGE_PER_STATUS] = PoolPtr(new AbilityPool<BonusDamagePerStatusAbility>());
        pools_[eABILITY_WEAPON_MASTERY] = PoolPtr(new AbilityPool<WeaponMasteryAbility>());
        pools_[eABILITY_MONSTER_TRANSFORMATION] = PoolPtr(new AbilityPool<MonsterTransAbility>());
        pools_[eABILITY_WIND_SHIELD] = PoolPtr(new AbilityPool<WindShieldAbility>());
        pools_[eABILITY_STUN_STATUS] = PoolPtr(new AbilityPool<StunAbility>());
        pools_[eABILITY_DRAGON_TRANSFORMATION1] = PoolPtr(new AbilityPool<DragonTransAbility>());
        pools_[eABILITY_SUMMON] = PoolPtr(new AbilityPool<SummonAbility>());
        pools_[eABILITY_RANDOM_AREA_ATTACK] = PoolPtr(new AbilityPool<BombEffectAbility>());
        pools_[eABILITY_ATTACK_DAMAGE_HP_ABSORPTION] = PoolPtr(new AbilityPool<AbsorbAbility>());
        pools_[eABILITY_ATTACK_DAMAGE_MP_ABSORPTION] = PoolPtr(new AbilityPool<AbsorbAbility>());
        pools_[eABILITY_ATTACKED_DAMAGE_HP_ABSORPTION] = PoolPtr(new AbilityPool<AbsorbAbility>());
        pools_[eABILITY_ATTACKED_DAMAGE_MP_ABSORPTION] = PoolPtr(new AbilityPool<AbsorbAbility>());
        pools_[eABILITY_SUMMON_TO_DIE] = PoolPtr(new AbilityPool<SummonToDeathAbility>());
        pools_[eABILITY_ABNORMAL_STATUS] = PoolPtr(new AbilityPool<AbnormalAbility>());
        pools_[eABILITY_FEAR] = PoolPtr(new AbilityPool<AbnormalAbility>());
        pools_[eABILITY_REFLECT_DAMAGE] = PoolPtr(new AbilityPool<MirrorAbility>());
        pools_[eABILITY_REFLECT_STUN] = PoolPtr(new AbilityPool<MirrorAbility>());
        pools_[eABILITY_REFLECT_FEAR] = PoolPtr(new AbilityPool<MirrorAbility>());
        pools_[eABILITY_REFLECT_FROZEN] = PoolPtr(new AbilityPool<MirrorAbility>());
        pools_[eABILITY_REFLECT_SLOW] = PoolPtr(new AbilityPool<MirrorAbility>());
        pools_[eABILITY_REFLECT_SLOWDOWN] = PoolPtr(new AbilityPool<MirrorAbility>());
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        pools_[eABILITY_REFLECT_LINK_SKILL] = PoolPtr(new AbilityPool<MirrorAbility>());
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        pools_[eABILITY_MAGIC_SHIELD] = PoolPtr(new AbilityPool<MagicShieldAbility>());
        pools_[eABILITY_LOWHP_ATTACKPOWER_CHANGE] = PoolPtr(new AbilityPool<LowHPAbility>());
        pools_[eABILITY_LOWHP_DEFENSE_CHANGE] = PoolPtr(new AbilityPool<LowHPAbility>());
        pools_[eABILITY_STATUS_HEAL] = PoolPtr(new AbilityPool<CureAbility>());
        pools_[eABILITY_BONUS_DAMAGE_PER_ATTACK] = PoolPtr(new AbilityPool<BonusDamageAbility>());
        pools_[eABILITY_PIERCE_ARROW] = PoolPtr(new AbilityPool<InformationAbility>());
        pools_[eABILITY_SUMMON_CHANGE_STATUS] = PoolPtr(new AbilityPool<InformationAbility>());
        pools_[eABILITY_DRAGON_TRANSFORMATION2] = PoolPtr(new AbilityPool<InformationAbility>());
        pools_[eABILITY_DRAGON_TRANSFORMATION3] = PoolPtr(new AbilityPool<InformationAbility>());
        pools_[eABILITY_SUMMON_DEAD_MONSTER] = PoolPtr(new AbilityPool<SummonDeadMonsterAbility>());
        pools_[eABILITY_BLOCK_SKILL] = PoolPtr(new AbilityPool<BlockSkillAbility>());
        pools_[eABILITY_SUCTION_HPMP] = PoolPtr(new AbilityPool<SuctionAbility>());
        pools_[eABILITY_CHANGE_ATTR] = PoolPtr(new AbilityPool<ChangeAttrAbility>());
        pools_[eABILITY_CANCEL_STATUS] = PoolPtr(new AbilityPool<CancelStatusAbility>());
        pools_[eABILITY_VITAL_SUCTION] = PoolPtr(new AbilityPool<VitalSuctionAbility>());
        pools_[eABILITY_HIDE] = PoolPtr(new AbilityPool<TransparentAbility>());
        pools_[eABILITY_DARK_BREAK] = PoolPtr(new AbilityPool<SelfDestructionAbility>());
        pools_[eABILITY_SUMMON_CRYSTALWARP] = PoolPtr(new AbilityPool<SummonCrystalWarpAbility>());
        pools_[eABILITY_SUMMON_CRYSTALWARP_DESTROY] = PoolPtr(new AbilityPool<SummonCrystalWarpAbility>());
        pools_[eABILITY_PULLING] = PoolPtr(new AbilityPool<ThrustAbility>());
        pools_[eABILITY_ROLLING_BOMB] = PoolPtr(new AbilityPoolNoParent<Ability>());
        pools_[eABILITY_QUICKSTEP] = PoolPtr(new AbilityPool<QuickStepAbility>());
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
        //eABILITY_CUR_HP_INCREASE와 같은 어빌리티로 생성
        pools_[eABILITY_VARIATION_PUREVALUE_HP] = PoolPtr(new AbilityPool<CurrnetHPIncreaseAbility>());
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP

        // 기본 구현을 사용하는 어빌리티 풀을 등록한다.
        pools_[kAbilityComboSkillEffectAddDamage] = PoolPtr(new AbilityPoolNoParent<Ability>());
        pools_[kAbilityComboSkillEffectAddBadStatus] = PoolPtr(new AbilityPoolNoParent<Ability>());
        pools_[kAbilityIncreseSkillDamage] = PoolPtr(new AbilityPoolNoParent<Ability>());

        // 버프 상태를 부여하는 어빌리티 풀을 등록한다.
        pools_[eABILITY_MAX_HP_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_RECOVER_HP_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MAX_MP_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_CUR_MP_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_RECOVER_MP_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_PHYSICAL_ATTACKPOWER_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MAGIC_ATTACKPOWER_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_PHYSICAL_DEFENSE_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_STAT_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_PHYSICAL_ATTACKRATE_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_PHYSICAL_AVOIDRATE_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MOVE_SPEED_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_PHYSICAL_SPEED_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_CASTING_TIME_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SKILLRANGE_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_CRITICAL_RATIO_CHANGE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_CRITICAL_DAMAGE_CHANGE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SKILLDAMAGE_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SKILL_COOL_TIME_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MP_SPEND_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_ADRENALINE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_ATTR_DEFENSIVE_POWER] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_INCREASE_SKILL_ABILITY] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_ENCHANT_POISON] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MAX_SD_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_CUR_SD_INCREASE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[kAbilityIncreseHeal] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[kAbilityActiveComboSkill] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[kAbilityActiveIncreseSkillDamage] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_ATTR_ATTACK_POWER] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_DETECTING_HIDE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_FUGITIVE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SKILL_STATE_IGNORE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        pools_[eABILITY_CRAFT_COST_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_ENCHANT_COST_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_RECOVER_POTION_COOLTIME_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_RECOVER_POTION_RECOVERY_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_QUEST_REWARD_EXP_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MAX_DAMAGE_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_MONEY_RAIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_DOMINATION_MAPOBJECT_DAMAGE_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SHOP_REPAIR_HEIM_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SHOP_BUY_HEIM_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        pools_[eABILITY_FP_CHANGE] = PoolPtr(new AbilityPool<FPChangeAbility>());
        pools_[eABILITY_FP_TOGGLE] = PoolPtr(new AbilityPool<ToggleFPAbility>());
        pools_[eABILITY_INCREASE_DAMAGE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_AUTOCAST_BYATTACK] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_OVERLAP_STATE] = PoolPtr(new AbilityPool<BuffStatusAbility>());
        pools_[eABILITY_SKILL_CONDITION] = PoolPtr(new AbilityPoolNoParent<Ability>());
        pools_[eABILITY_SUMMON_IMMOVABLE] = PoolPtr(new AbilityPool<SummonAbility>());
        pools_[eABILITY_SUMMON_CHANGE_STATUS_EXTEND] = PoolPtr(new AbilityPool<InformationAbility>());
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        pools_[eABILITY_CHARMED] = PoolPtr(new AbilityPool<AbnormalAbility>());
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
        pools_[eABILITY_MONSTER_KILL_EXP_RATIO] = PoolPtr(new AbilityPool<BuffStatusAbility>());
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        pools_[eABILITY_AWAKENING_PROBABILITY] = PoolPtr(new AbilityPool<BuffStatusAbility>());
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        // 패시브 어빌리티 풀을 등록한다.
        pools_[eABILITY_SUMMONMASTERY] = PoolPtr(new AbilityPool<PassiveStatusAbility>());
    }

    Ability* Alloc(const int ability_id)
    {
        Pools::const_iterator it = pools_.find(ability_id);
        if (it == pools_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|등록되지 않은 어빌리티 입니다.|AbilityID = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                ability_id
            );
            return NULL;                         
        }
        PoolPtr pool = it->second;
        return pool->Alloc();
    }

    void Free(Ability* const ability)
    {
        if (ability == NULL)
        {
            return;
        }
        const eABILITY ability_id = ability->GetAbilityID();
        Pools::const_iterator it = pools_.find(ability_id);
        if (it == pools_.end())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|등록되지 않은 어빌리티 입니다.|AbilityID = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                ability_id
            );
            return;
        }
        PoolPtr pool = it->second;
        pool->Free(ability);
    }

private:
    // 메모리풀 인터페이스
    class IPool
    {
    protected:
        IPool()
        {
        }
        virtual ~IPool()
        {
        }
    public:
        virtual Ability* Alloc() = 0;
        virtual void Free(Ability* const ability) = 0;
    };
    typedef boost::shared_ptr<IPool> PoolPtr;
    typedef STLX_HASH_MAP<int, PoolPtr> Pools;

    // 어빌리티 메모리풀
    template <typename ABILITY_T>
    class AbilityPool : public IPool
    {
    public:
        AbilityPool()
        {
        }
        ~AbilityPool()
        {
        }
    public:
        virtual Ability* Alloc()
        {
            return impl_.New();
        }
        virtual void Free(Ability* const ability)
        {
            impl_.DeleteByParent(ability);
        }
    private:
        typedef memory::FixedSizeMemoryPool<ABILITY_T, Ability> Impl;
        Impl impl_;
    };
    template <typename ABILITY_T>
    class AbilityPoolNoParent : public IPool
    {
    public:
        AbilityPoolNoParent()
        {
        }
        ~AbilityPoolNoParent()
        {
        }
    public:
        virtual Ability* Alloc()
        {
            return impl_.New();
        }
        virtual void Free(ABILITY_T* const ability)
        {
            impl_.Delete(ability);
        }
    private:
        typedef memory::FixedSizeMemoryPool<ABILITY_T> Impl;
        Impl impl_;
    };

private:
    Pools pools_;

}; // Allocator


AbilityFactory::AbilityFactory() : 
    allocator_(new Allocator())
{
}

AbilityFactory::~AbilityFactory()
{
}

void AbilityFactory::Init(const DWORD /*pool_size*/)
{
    allocator_->Init();

    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    skill_parser->SetFirst();
    while (const SkillScriptInfo* skill_script_info = skill_parser->GetNextSkill())
    {
        skill_script_info->SetFirst();
        while (const BASE_ABILITYINFO* ability_info = skill_script_info->GetNext())
        {
            Ability* const ability = AllocAbility(
                ability_info->m_wAbilityID, 
                static_cast<eSKILL_TYPE>(skill_script_info->m_bySkillType)
            );
            if (ability == NULL)
            {
                continue;
            }
            SkillInfo skill_info;
            ability->InitDetailed(NULL, skill_script_info->m_SkillCode, &skill_info, 0, ability_info);
            
            if (ability->IsValidStateID() == false)
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|입력된 상태가 올바르지 않습니다.|SkillCode = %d, AbilityID = %d, StateID = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    skill_script_info->m_SkillCode, 
                    ability_info->m_wAbilityID, 
                    ability_info->m_wStateID
                );
            }
            FreeAbility(ability);
        }
    }
}

Ability* AbilityFactory::AllocAbility(const WORD ability_id, const eSKILL_TYPE /*skill_type*/)
{
    return allocator_->Alloc(ability_id);
}

void AbilityFactory::FreeAbility(Ability* const ability)
{
    allocator_->Free(ability);
}