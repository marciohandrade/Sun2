#include "stdafx.h"
#include "Skill_Info.h"

#include <SkillInfoParser.h>

//==================================================================================================


//--------------------------------------------------------------------------------------------------
namespace nsSkill {
;

static bool skill_info_initialize();
//
static bool skill_info_initialize_check = skill_info_initialize();
static uint8_t default_damage_args_data_[sizeof(DamageArgs)];

// CHANGES: f110303.3L, added transaction block to prevent sending dead notification on skill updating
static int SkillExecutionTransaction_transaction_ = 0;

};

//--------------------------------------------------------------------------------------------------
const DamageArgs& DamageArgs::kDefaultDamageArgs = \
    reinterpret_cast<DamageArgs&>(nsSkill::default_damage_args_data_);

// CHANGES: f110303.3L, added transaction block to prevent sending dead notification on skill updating
const int& SkillExecutionTransaction::kIsTransaction =
    nsSkill::SkillExecutionTransaction_transaction_;

SkillExecutionTransaction::SkillExecutionTransaction() {
    ++nsSkill::SkillExecutionTransaction_transaction_;
}

SkillExecutionTransaction::~SkillExecutionTransaction() {
    if (nsSkill::SkillExecutionTransaction_transaction_) {
        --nsSkill::SkillExecutionTransaction_transaction_;
    }
}


//==================================================================================================
//
SkillInfo::SkillInfo(Player* player, const MSG_CG_SKILL_ACTION_SYN& client_request)
    : character_(player)
{
    ZeroMemory(field_start_, static_cast<size_t>(field_end_ + sizeof(field_end_) - field_start_));
#ifdef _DEBUG
    field_start_[0] = 0x0A;
#endif
    skill_code_ = client_request.m_SkillCode;
    client_serial_ = client_request.m_dwClientSerial;
    main_target_key_ = client_request.m_dwMainTargetKey;
    //attack_sequence_ =
    attack_propensity_ = client_request.m_byAttackPropensity;
    //
    cur_pos_ = client_request.m_wvCurPos;
    dest_pos_ = client_request.m_wvDestPos;
    main_target_pos_ = client_request.m_wvMainTargetPos;
    //
    AlignSkillInfo(skill_code_);
}

SkillInfo::SkillInfo(Player* player, const MSG_CG_BATTLE_PLAYER_ATTACK_SYN& client_request)
    : character_(player)
{
    ZeroMemory(field_start_, static_cast<size_t>(field_end_ + sizeof(field_end_) - field_start_));
#ifdef _DEBUG
    field_start_[0] = 0x0A;
#endif
    //skill_code_ = 
    client_serial_ = client_request.dwClientSerial;
    main_target_key_ = client_request.dwTargetKey;
    attack_sequence_ = client_request.byAttackType;
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    style_presentation_ = client_request.style_presentation;
#endif
    attack_propensity_ = client_request.byAttackPropensity;
    //
    cur_pos_ = client_request.wvCurPos;
    dest_pos_ = client_request.wvDestPos;
    //
    //AlignSkillInfo() <- called by a protocol handler related to a style attack
}

SkillInfo::SkillInfo(Player* player, const MSG_CG_ITEM_USE_SKILL_ACTION_SYN& client_request)
    : character_(player)
{
    ZeroMemory(field_start_, static_cast<size_t>(field_end_ + sizeof(field_end_) - field_start_));
#ifdef _DEBUG
    field_start_[0] = 0x0A;
#endif
    const WzVector* cur_pos = character_->GetPosPtr();
    ;{
        main_target_key_ = 
            (client_request.target_code_ != 0) ? client_request.target_code_ : character_->GetObjectKey();
        skill_factor_ = SKILL_FACTOR_ITEM;
        attack_propensity_ = client_request.byAttackPropensity;
    };
    ;{
        cur_pos_ = *cur_pos;
        dest_pos_ = *cur_pos;
        main_target_pos_ = client_request.m_wvMainTargetPos;
    };
}

SkillInfo::SkillInfo(Character* character, SLOTCODE skill_code, DWORD client_serial_key)
    : character_(character)
{
    ZeroMemory(field_start_, static_cast<size_t>(field_end_ + sizeof(field_end_) - field_start_));
#ifdef _DEBUG
    field_start_[0] = 0x0A;
#endif
    ;{
        skill_code_ = skill_code;
        client_serial_ = client_serial_key;
    };
    //
    AlignSkillInfo(skill_code_);
}

void SkillInfo::SetArgsObjectInfo(DWORD main_target_key,
                                  const WzVector* cur_pos,
                                  const WzVector* dest_pos,
                                  const WzVector* main_target_pos)
{
#ifdef _DEBUG
    if (field_start_[0] != 0x0A) {
        __debugbreak();
    }
#endif
    ;{
        main_target_key_ = main_target_key;
    };
    if (cur_pos) {
        cur_pos_ = *cur_pos;
    };
    if (dest_pos) {
        dest_pos_ = *dest_pos;
    };
    if (main_target_pos) {
        main_target_pos_ = *main_target_pos;
    };
}

void SkillInfo::SetArgsSkillExt(eATTACK_SEQUENCE attack_sequence,
                                uint8_t skill_effect,
                                uint16_t skill_delay,
                                eSKILL_FACTOR_TYPE skill_factor,
                                eATTACK_PROPENSITY attack_propensity,
                                uint8_t event_code)
{
    BOOST_STATIC_ASSERT((SkillOption::eOption_DBStoreRangeMask & 0xFF) ==
                         SkillOption::eOption_DBStoreRangeMask);
    BOOST_STATIC_ASSERT(kDefaultSequence == 0   && kDefaultSkillEffect == 0 &&
                        kDefaultSkillDelay == 0 && kDefaultSkillFactor == 0 &&
                        kDefaultPropensity == 0 && kDefaultEvent == 0);
    //
#ifdef _DEBUG
    if (field_start_[0] != 0x0A) {
        __debugbreak();
    }
#endif
    //
    attack_sequence_ = static_cast<uint8_t>(attack_sequence);
    // NOTE, this method is called by an internal skill invoker
    // so that in most usage of attack_propensity_ is a default(ATTACK_PROPENSITY_NORMAL).
    attack_propensity_ = static_cast<uint8_t>(attack_propensity);
    skill_factor_ = static_cast<uint8_t>(skill_factor);
    skill_effect_ = skill_effect;
    skill_delay_ = skill_delay;
    event_code_ = event_code;
}

bool SkillInfo::AlignSkillInfo(SLOTCODE skill_code)
{
#ifdef _DEBUG
    if (field_start_[0] != 0x0A) {
        __debugbreak();
    }
#endif
    if (skill_code == 0) {
        return false;
    };
    //
    const bool change_skill = (skill_code_ != skill_code);
    if (change_skill || root_skill_info_ == NULL)
    {
        skill_code_ = skill_code;
        const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
        root_skill_info_ = skill_parser->GetInfo(skill_code_);
        return root_skill_info_ != NULL;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
namespace nsSkill {
;

static bool skill_info_initialize()
{
    // initialize data to check access violation
    DamageArgs* damage_arg = reinterpret_cast<DamageArgs*>(default_damage_args_data_);
    BOOST_STATIC_ASSERT(sizeof(int) == sizeof(damage_arg->attack_type_) &&
                        sizeof(int) == sizeof(damage_arg->attack_regist_kind_) &&
                        sizeof(int) == sizeof(damage_arg->sd_apply_) &&
                        eATTACK_RESIST_NOMAL == 0);
    int offset = 0;
    int* modify_field = NULL;
    ;{
        offset = static_cast<size_t>((uint8_t*)(&damage_arg->attack_type_) -
                                     default_damage_args_data_);
        modify_field = reinterpret_cast<int*>(&default_damage_args_data_[offset]);
        *modify_field = eATTACK_TYPE_INVALID;
    };
    ;{
        offset = static_cast<size_t>((uint8_t*)(&damage_arg->sd_apply_) -
                                     default_damage_args_data_);
        modify_field = reinterpret_cast<int*>(&default_damage_args_data_[offset]);
        *modify_field = eSD_APPLY_EMPTY;
    };

    return true;
}

};

//==================================================================================================
