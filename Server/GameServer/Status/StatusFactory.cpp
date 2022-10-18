#include "stdafx.h"
#include "StatusFactory.h"
#include "AbilityStatus.h"
#include "WoundStatus.h"
#include "StunStatus.h"
#include "AbsorbStatus.h"
#include "CureStatus.h"
#include "BlindStatus.h"
#include "MagicShieldStatus.h"
#include "FearStatus.h"
#include "SPBonusStatus.h"
#include "ChaosStatus.h"
#include "WindShieldStatus.h"
#include "MirrorStatus.h"
#include "ThrustStatus.h"
#include "SleepStatus.h"
#include "StyleThrust.h"
#include "StyleStun.h"
#include "StyleDown.h"
#include "RecoverStatus.h"
#include "LowHPStatus.h"
#include "BonusDamageStatus.h"
#include "SteelStatus.h"
#include "DragonTransStatus.h"
#include "SummonStatus.h"
#include "IncompetenceStatus.h"
#include "intensifysummon.h"
#include "SuctionStatus.h"
#include "ChangeAttrStatus.h"
#include "TransparentStatus.h"
#include "VitalSuctionStatus.h"
#include "BattleStatus.h"
#include "EnchantPoisonStatus.h"
#include "SheldPointStatus.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "ToggleFPStatus.h"
#include "AutoCastStatus.h"
#include "OverlapStatus.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

//==================================================================================================
using namespace nsSkill;

StatusFactory::IStatusAllocator::~IStatusAllocator()
{
}

template<class StatusType_>
struct StatusFactory::StatusAllocNode : public StatusFactory::IStatusAllocator
{
    typedef StatusType_ StatusType;
    static const size_t kTypeSize = sizeof(StatusType);

    virtual ~StatusAllocNode()
    {
    }

    virtual BaseStatus* Allocate() const
    {
        void* ptr = TAllocMalloc(kTypeSize);
        ZeroMemory(ptr, kTypeSize);
        return new (ptr) StatusType;
    }
    //
    virtual void Deallocate(BaseStatus* ptr) const
    {
        StatusType* object = static_cast<StatusType*>(ptr);
        object->~StatusType();
        TAllocFree(object);
    }
};

template<>
struct StatusFactory::StatusAllocNode<void> : public StatusFactory::IStatusAllocator
{
    virtual ~StatusAllocNode()
    {
    }

    virtual BaseStatus* Allocate() const
    {
        SUNLOG(eCRITICAL_LOG, _T("|Msg=request an allocation operation about an unknown status type|"));
        return NULL;
    }
    //
    virtual void Deallocate(BaseStatus* ptr) const
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|Msg=request an free operation about an unknown status type (%p)|"), ptr);
    }
};

//==================================================================================================

enum StatusFactory::eStatusPoolIndex
{
    //-----------------------------
    SPI_BASESTATUS          = 0,
    //-----------------------------
    // AbilityBase -> BaseStatus
    SPI_ABILITYSTATUS       ,
    //-----------------------------
    // AbilityBase derived status
    SPI_WOUNDSTATUS         ,
    SPI_STUNSTATUS          ,
    SPI_ABSORBSTATUS        ,
    SPI_CURESTATUS          ,
    SPI_BLINDSTATUS         ,
    SPI_MAGICSHIELDSTATUS   ,
    SPI_FEARSTATUS          ,
    SPI_SPBONUSSTATUS       ,
    SPI_CHAOSSTATUS         ,
    SPI_WINDSHIELDSTATUS    ,
    SPI_MIRRORSTATUS        ,
    SPI_THRUSTSTATUS        ,
    SPI_SLEEPSTATUS         ,
    SPI_LOWHPSTATUS         ,
    SPI_BONUSDAMAGESTATUS   ,
    SPI_DRAGONTRANSSTATUS   ,
    SPI_SUMMONSTATUS        ,
    SPI_INCOMPETENCESTATUS  ,// CHANGUP_IMPOSSIBLE_ATTCAK_STATUS
    SPI_INTENSIFYSUMMON     ,// CHANGUP_RECKLESS_STATUS
    SPI_SUCTIONSTATUS       ,
    SPI_CHANGE_ATTR         ,
    SPI_TRANSPARENT         ,
    SPI_VITALSUCTION_STATUS ,
    SPI_ENCHANTPOISON_STATUS,
    SPI_SHELDPOINTSTATUS    ,
    //-----------------------------
    // EtcStatus -> BaseStatus
    // EtcStatus derived status
    SPI_STYLETHRUST         ,
    SPI_STYLESTUN           ,
    SPI_STYLEDOWN           ,
    SPI_RECOVERSTATUS       ,
    SPI_STEELSTATUS         ,
    SPI_BATTLESTATUS        ,
    //-----------------------------
    SPI_TOGGLEFPSTATUS      ,
    SPI_AUTOCASTBYATTACK    ,
    SPI_OVERLAPSTATUS       ,
    // Upperbound
    SPI_SIZE_MAX
};

//==================================================================================================

namespace nsSkill {
;

static const struct StatusMatchingNode
{
    uint16_t eCST;
    uint16_t eSPI;
} status_matching_nodes_[] =
{
    { eCHAR_STATE_POISON                        ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_WOUND                         ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_FIRE_WOUND                    ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_PERIODIC_DAMAGE               ,   StatusFactory::SPI_WOUNDSTATUS          },

    { eCHAR_STATE_STUN                          ,   StatusFactory::SPI_STUNSTATUS           },
    { eCHAR_STATE_DOWN                          ,   StatusFactory::SPI_STUNSTATUS           },
    { eCHAR_STATE_UPPERDOWN                     ,   StatusFactory::SPI_STUNSTATUS           },  //_NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN
    { eCHAR_STATE_DELAY                         ,   StatusFactory::SPI_STUNSTATUS           },
    { eCHAR_STATE_FROZEN                        ,   StatusFactory::SPI_STUNSTATUS           },
    { eCHAR_STATE_HOLDING                       ,   StatusFactory::SPI_STUNSTATUS           },

    { eCHAR_STATE_ABSORB                        ,   StatusFactory::SPI_ABSORBSTATUS         },
    { eCHAR_STATE_IMMUNITY_DAMAGE               ,   StatusFactory::SPI_ABSORBSTATUS         }, // CHANGUP_IMMUNITY_DAMAGE_STATUS
    { eCHAR_STATE_CURE                          ,   StatusFactory::SPI_CURESTATUS           },
    { eCHAR_STATE_BLIND                         ,   StatusFactory::SPI_BLINDSTATUS          },
    { eCHAR_STATE_POLYMORPH                     ,   StatusFactory::SPI_BLINDSTATUS          },
    { eCHAR_STATE_MAGIC_SHIELD                  ,   StatusFactory::SPI_MAGICSHIELDSTATUS    },

    { eCHAR_STATE_FEAR                          ,   StatusFactory::SPI_FEARSTATUS           },
    { eCHAR_STATE_MP_FEAR2                      ,   StatusFactory::SPI_FEARSTATUS           },
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    { eCHAR_STATE_CHARMED                       ,   StatusFactory::SPI_FEARSTATUS           },
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    { eCHAR_STATE_SP_BONUS                      ,   StatusFactory::SPI_SPBONUSSTATUS        },
    { eCHAR_STATE_CHAOS                         ,   StatusFactory::SPI_CHAOSSTATUS          },
    { eCHAR_STATE_BUF_RANGE_DAMAGE              ,   StatusFactory::SPI_WINDSHIELDSTATUS     },
    { eCHAR_STATE_BUF_RANGE_DAMAGE2             ,   StatusFactory::SPI_WINDSHIELDSTATUS     },
    { eCHAR_STATE_BUF_RANGE_DAMAGE3             ,   StatusFactory::SPI_WINDSHIELDSTATUS     },
    { eCHAR_STATE_BUF_RANGE_DAMAGE4             ,   StatusFactory::SPI_WINDSHIELDSTATUS     },
    { eCHAR_STATE_REFLECT_DAMAGE                ,   StatusFactory::SPI_MIRRORSTATUS         },
    { eCHAR_STATE_REFLECT_SLOW                  ,   StatusFactory::SPI_MIRRORSTATUS         },
    { eCHAR_STATE_REFLECT_FROZEN                ,   StatusFactory::SPI_MIRRORSTATUS         },
    { eCHAR_STATE_REFLECT_SLOWDOWN              ,   StatusFactory::SPI_MIRRORSTATUS         },
    { eCHAR_STATE_REFLECT_STUN                  ,   StatusFactory::SPI_MIRRORSTATUS         },
    { eCHAR_STATE_REFLECT_FEAR                  ,   StatusFactory::SPI_MIRRORSTATUS         },
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    { eCHAR_STATE_REFLECT_LINK_SKILL                ,   StatusFactory::SPI_MIRRORSTATUS         },
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    { eCHAR_STATE_THRUST                        ,   StatusFactory::SPI_THRUSTSTATUS         },
    { eCHAR_STATE_SLEEP                         ,   StatusFactory::SPI_SLEEPSTATUS          },
    { eCHAR_STATE_STAT_LOWHP_ATTACK_DECREASE    ,   StatusFactory::SPI_LOWHPSTATUS          },
    { eCHAR_STATE_STAT_LOWHP_DEFENSE_DECREASE   ,   StatusFactory::SPI_LOWHPSTATUS          },
    { eCHAR_STATE_STAT_LOWHP_ATTACK_INCREASE    ,   StatusFactory::SPI_LOWHPSTATUS          },
    { eCHAR_STATE_STAT_LOWHP_DEFENSE_INCREASE   ,   StatusFactory::SPI_LOWHPSTATUS          },
    { eCHAR_STATE_STAT_DAMAGE_ADD               ,   StatusFactory::SPI_BONUSDAMAGESTATUS    },
    { eCHAR_STATE_STYLE_THRUST                  ,   StatusFactory::SPI_STYLETHRUST          },
    { eCHAR_STATE_STYLE_STUN                    ,   StatusFactory::SPI_STYLESTUN            },
    { eCHAR_STATE_STYLE_DOWN                    ,   StatusFactory::SPI_STYLEDOWN            },
    { eCHAR_STATE_ETC_AUTO_RECOVER_HP           ,   StatusFactory::SPI_RECOVERSTATUS        },
    { eCHAR_STATE_ETC_AUTO_RECOVER_MP           ,   StatusFactory::SPI_RECOVERSTATUS        },
    { eCHAR_STATE_ETC_AUTO_RECOVER_HPMP         ,   StatusFactory::SPI_RECOVERSTATUS        },
    //{__NA_001290_20090525_SHIELD_SYSTEM 
    { eCHAR_STATE_ETC_AUTO_RECOVER_SD           ,   StatusFactory::SPI_RECOVERSTATUS        },
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    { eCHAR_STATE_ETC_AUTO_RECOVER_FP           ,   StatusFactory::SPI_RECOVERSTATUS        },
    { eCHAR_STATE_ETC_EXIST_SHELD_POINT         ,   StatusFactory::SPI_SHELDPOINTSTATUS     },
    //}
    { eCHAR_STATE_ETC_ITEM_RECOVER_HP           ,   StatusFactory::SPI_RECOVERSTATUS        },
    { eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY ,   StatusFactory::SPI_RECOVERSTATUS        },
    { eCHAR_STATE_ETC_ITEM_RECOVER_MP           ,   StatusFactory::SPI_RECOVERSTATUS        },
    { eCHAR_STATE_STEEL                         ,   StatusFactory::SPI_STEELSTATUS          },
    { eCHAR_STATE_TRANSFORMATION                ,   StatusFactory::SPI_DRAGONTRANSSTATUS    },
    { eCHAR_STATE_SUMMON                        ,   StatusFactory::SPI_SUMMONSTATUS         },
    { eCHAR_STATE_ATTACK_IMPOSSIBLE             ,   StatusFactory::SPI_INCOMPETENCESTATUS   },
    { eCHAR_STATE_INTENSIFY_SUMMON              ,   StatusFactory::SPI_INTENSIFYSUMMON      },
    { eCHAR_STATE_STUN2                         ,   StatusFactory::SPI_STUNSTATUS           },
    { eCHAR_STATE_CURSE_INCREASE                ,   StatusFactory::SPI_ABILITYSTATUS        },
    { eCHAR_STATE_PAIN                          ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_FIRE_WOUND2                   ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_PAIN2                         ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_HP_SUCTION                    ,   StatusFactory::SPI_SUCTIONSTATUS        },
    { eCHAR_STATE_MP_SUCTION                    ,   StatusFactory::SPI_SUCTIONSTATUS        },
    { eCHAR_STATE_CHANGE_ATTR                   ,   StatusFactory::SPI_CHANGE_ATTR          },
    { eCHAR_STATE_TRANSPARENT                   ,   StatusFactory::SPI_TRANSPARENT          },
    { eCHAR_STATE_VITAL_SUCTION                 ,   StatusFactory::SPI_VITALSUCTION_STATUS  },
    { eCHAR_STATE_BATTLE                        ,   StatusFactory::SPI_BATTLESTATUS         },
    { eCHAR_STATE_ENCHANT_POISON                ,   StatusFactory::SPI_ENCHANTPOISON_STATUS },
    { eCHAR_STATE_POISON2                       ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_ELECTRICSHOCK                 ,   StatusFactory::SPI_WOUNDSTATUS          },
    { kCharStateIncreseHeal, StatusFactory::SPI_ABILITYSTATUS }, 
    { kCharStateActiveComboSkill, StatusFactory::SPI_ABILITYSTATUS }, 
    { kCharStateIncreseSkillDamage, StatusFactory::SPI_ABILITYSTATUS }, 
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    { eCHAR_STATE_TRANSFORMATION6, StatusFactory::SPI_ABILITYSTATUS }, 
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    { eCHAR_STATE_PHOENIX_BURN                  ,   StatusFactory::SPI_WOUNDSTATUS          },
    { eCHAR_STATE_DARK_OF_LIGHT_ZONE            ,   StatusFactory::SPI_BLINDSTATUS          },
    { eCHAR_STATE_FATAL_POINT                   ,   StatusFactory::SPI_STUNSTATUS           },
    { eCHAR_STATE_FUGITIVE                      ,   StatusFactory::SPI_ABILITYSTATUS           },
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    { eCHAR_STATE_VICE_SWORD                    ,   StatusFactory::SPI_TOGGLEFPSTATUS       },
    { eCHAR_STATE_EVADE                         ,   StatusFactory::SPI_TOGGLEFPSTATUS       },
    { eCHAR_STATE_RISING_FORCE                  ,   StatusFactory::SPI_TOGGLEFPSTATUS       },
    { eCHAR_STATE_INCREASE_DAMAGE               ,   StatusFactory::SPI_ABILITYSTATUS        },
    { eCHAR_STATE_AUTOCAST_BLEEDING_ATTACK      ,   StatusFactory::SPI_AUTOCASTBYATTACK     },
    { eCHAR_STATE_FP_POWER_INCREASE             ,   StatusFactory::SPI_OVERLAPSTATUS        },
    { eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE ,   StatusFactory::SPI_ABSORBSTATUS         },
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    { eCHAR_STATE_PLENTY                        ,   StatusFactory::SPI_ABILITYSTATUS        },
    { eCHAR_STATE_SMART                         ,   StatusFactory::SPI_ABILITYSTATUS        },

#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
    // last point, EOR = end of records
    // f100930.5L, remove an useless section anymore after factories refactoring
    //{ eCHAR_STATE_INVALID                       ,   StatusFactory::SPI_SIZE_MAX             },
};

static const StatusFactory::StatusAllocNode<void> unknown_status_static_;

static struct StatusMappingTable
{
    const StatusFactory::IStatusAllocator* allocators_[StatusFactory::SPI_SIZE_MAX];
} status_mapping_table_;
//

}; //end of namespace

StatusFactory* StatusFactory::status_factory_static_ = NULL;

//==================================================================================================

StatusFactory* StatusFactory::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(StatusFactory** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        StatusFactory** static_table_;
    } auto_closure(&status_factory_static_);
    //
    if (status_factory_static_ != NULL) {
        return status_factory_static_;
    }
    //
    StatusFactory* provider = new StatusFactory;
    // initialization...
    status_factory_static_ = provider;
    //
    return status_factory_static_;
}

bool StatusFactory::SetupMappingTable(StatusFactory* factory)
{
    ;{
        StatusFactory::STATUS_ROUTING_TABLE& router = factory->status_routing_table_;
        const StatusMatchingNode* it = status_matching_nodes_,
                                * end = &status_matching_nodes_[_countof(status_matching_nodes_)];
        for ( ; it != end; ++it) {
            router[it->eCST] = it->eSPI;
        }
    };
    ;{
        ZeroMemory(&status_mapping_table_, sizeof(status_mapping_table_));
        const StatusFactory::IStatusAllocator** it = status_mapping_table_.allocators_,
            ** end = &status_mapping_table_.allocators_[_countof(status_mapping_table_.allocators_)];
        for ( ; it != end; ++it) {
            *it = &unknown_status_static_;
        }
    };
    //
    StatusMappingTable* const table = &status_mapping_table_;
    //
    // BaseStatus
    table->allocators_[SPI_BASESTATUS  ] = new StatusAllocNode<BaseStatus>;
    // AbilityBase -> BaseStatus
    table->allocators_[SPI_ABILITYSTATUS] = new StatusAllocNode<AbilityStatus>;
    // AbilityBase derived status
    table->allocators_[SPI_WOUNDSTATUS ] = new StatusAllocNode<WoundStatus>;
    table->allocators_[SPI_STUNSTATUS  ] = new StatusAllocNode<StunStatus>;
    table->allocators_[SPI_ABSORBSTATUS] = new StatusAllocNode<AbsorbStatus>;
    table->allocators_[SPI_CURESTATUS  ] = new StatusAllocNode<CureStatus>;
    table->allocators_[SPI_BLINDSTATUS ] = new StatusAllocNode<BlindStatus>;
    table->allocators_[SPI_MAGICSHIELDSTATUS] = new StatusAllocNode<MagicShieldStatus>;
    table->allocators_[SPI_FEARSTATUS  ] = new StatusAllocNode<FearStatus>;
    table->allocators_[SPI_SPBONUSSTATUS] = new StatusAllocNode<SPBonusStatus>;
    table->allocators_[SPI_CHAOSSTATUS ] = new StatusAllocNode<ChaosStatus>;
    table->allocators_[SPI_WINDSHIELDSTATUS] = new StatusAllocNode<WindShieldStatus>;
    table->allocators_[SPI_MIRRORSTATUS] = new StatusAllocNode<MirrorStatus>;
    table->allocators_[SPI_THRUSTSTATUS] = new StatusAllocNode<ThrustStatus>;
    table->allocators_[SPI_SLEEPSTATUS ] = new StatusAllocNode<SleepStatus>;
    table->allocators_[SPI_LOWHPSTATUS ] = new StatusAllocNode<LowHPStatus>;
    table->allocators_[SPI_BONUSDAMAGESTATUS] = new StatusAllocNode<BonusDamageStatus>;
    table->allocators_[SPI_DRAGONTRANSSTATUS] = new StatusAllocNode<DragonTransStatus>;
    table->allocators_[SPI_SUMMONSTATUS] = new StatusAllocNode<SummonStatus>;
    table->allocators_[SPI_INCOMPETENCESTATUS] = new StatusAllocNode<IncompetenceStatus>;
    table->allocators_[SPI_INTENSIFYSUMMON] = new StatusAllocNode<IntensifySummon>;
    table->allocators_[SPI_SUCTIONSTATUS] = new StatusAllocNode<SuctionStatus>;
    table->allocators_[SPI_CHANGE_ATTR] = new StatusAllocNode<ChangeAttrStatus>;
    table->allocators_[SPI_TRANSPARENT] = new StatusAllocNode<TransparentStatus>;
    table->allocators_[SPI_VITALSUCTION_STATUS] = new StatusAllocNode<VitalSuctionStatus>;
    table->allocators_[SPI_ENCHANTPOISON_STATUS] = new StatusAllocNode<EnchantPoisonStatus>;
    table->allocators_[SPI_SHELDPOINTSTATUS] = new StatusAllocNode<SheldPointStatus>;

    // EtcStatus -> BaseStatus
    // EtcStatus derived status
    table->allocators_[SPI_STYLETHRUST ] = new StatusAllocNode<StyleThrust>;
    table->allocators_[SPI_STYLESTUN   ] = new StatusAllocNode<StyleStun>;
    table->allocators_[SPI_STYLEDOWN   ] = new StatusAllocNode<StyleDown>;
    table->allocators_[SPI_RECOVERSTATUS] = new StatusAllocNode<RecoverStatus>;
    table->allocators_[SPI_STEELSTATUS ] = new StatusAllocNode<SteelStatus>;
    table->allocators_[SPI_BATTLESTATUS] = new StatusAllocNode<BattleStatus>;
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    table->allocators_[SPI_TOGGLEFPSTATUS] = new StatusAllocNode<ToggleFPStatus>;
    table->allocators_[SPI_AUTOCASTBYATTACK] = new StatusAllocNode<AutoCastStatus>;
    table->allocators_[SPI_OVERLAPSTATUS] = new StatusAllocNode<OverlapStatus>;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    return true;
}

StatusFactory::StatusFactory()
{
    SetupMappingTable(this);
}

StatusFactory::~StatusFactory()
{
    const StatusFactory::IStatusAllocator** it = status_mapping_table_.allocators_,
        ** end = &status_mapping_table_.allocators_[_countof(status_mapping_table_.allocators_)];
    for ( ; it != end; ++it)
    {
        if (*it == &unknown_status_static_) {
            continue;
        }
        delete *it;
    }
    ZeroMemory(&status_mapping_table_, sizeof(status_mapping_table_));
}

void StatusFactory::Release()
{
}

BaseStatus* StatusFactory::AllocStatus(eCHAR_STATE_TYPE state_id, BOOL is_ability)
{
    STATUS_ROUTING_TABLE::const_iterator found = status_routing_table_.find(state_id);
    const eStatusPoolIndex allocator_index = found != status_routing_table_.end()
                                           ?  static_cast<eStatusPoolIndex>(found->second)
                                           :  (is_ability ? SPI_ABILITYSTATUS : SPI_BASESTATUS);
    //
    BaseStatus* status = NULL;
    if (allocator_index < SPI_SIZE_MAX) {
        status = status_mapping_table_.allocators_[allocator_index]->Allocate();
    }
    return status;
}

void StatusFactory::FreeStatus(BaseStatus* status)
{
    const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(status->GetStateID());
    //
    STATUS_ROUTING_TABLE::const_iterator found = status_routing_table_.find(state_id);
    const eStatusPoolIndex allocator_index = found != status_routing_table_.end()
                                           ?  static_cast<eStatusPoolIndex>(found->second)
                                           :  (status->IsAbilityStatus() ? SPI_ABILITYSTATUS
                                                                         : SPI_BASESTATUS);

    if (allocator_index < SPI_SIZE_MAX) {
        status_mapping_table_.allocators_[allocator_index]->Deallocate(status);
    }
}


void StatusFactory::DisplayPoolInfo()
{
}



