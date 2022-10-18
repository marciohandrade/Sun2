#ifndef __GAMESERVER_SKILL_INFO_H
#define __GAMESERVER_SKILL_INFO_H
#pragma once

#include <Const.h>
#include <ProgramCommon/WzDataType.h>

//==================================================================================================
//  (History)
//  - 09.07.09, waverix, move to Skill.h -> Skill_Info.h
//

//==================================================================================================
//  (DECLARATION)
//
//  ���� �ִ� const�� define.h �� �Ű���ϴ�.

//  ��ų�� �Ӽ�
#define SKILL_ATTRIBUTION_INVISIBLE     0x01
#define SKILL_ATTRIBUTION_TARGETSTOP    0x02
#define SKILL_ATTRIBUTION_TARGETFLYING  0x04

class Character;
class SkillScriptInfo;
class GameField;
class Ability;
struct tagEtherBulletInfo;
//
struct MSG_CG_SKILL_ACTION_SYN;

//==================================================================================================
//  (NOTE) ���� ��ų ���� �ɼ� ��� ���� �÷� ���� ����.
//  __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
struct SkillOption
{
    enum eOption {
        eOption_None                = 0,
        eOption_ForcedAttack        = 1 << 0, // replaced by 'attack_propencity'
        // 1 << 1 - empty, enables runtime option
        // 1 << 2 - empty, enables runtime option
        //-------------------------------------------------------------------------
        // STATE_DETAIL_INFO::m_byAbilityIndex 5Bit [3, 7] = { 3,4,5,6,7 }
        // 1 << 3 - reserved, enables to store db
        // 1 << 4 - reserved, enables to store db
        eOption_HoldupBuffPostDead  = 1 << 5,
        // 1 << 6 - reserved, enables to store db
        // 1 << 7 - reserved, enables to store db
        eOption_DBStoreRangeMask    = (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7),
        //-------------------------------------------------------------------------
    };
    BYTE option_;
    //
    void Clear();
    // about 'Option'
    void ApplyOption(const SkillOption& rhs);
    void AddOption(SkillOption::eOption option);
};

//==================================================================================================
//
struct SkillInfo
{
    static const eATTACK_SEQUENCE kDefaultSequence = eATTACK_SEQUENCE_FIRST;
    static const uint8_t kDefaultSkillEffect = 0;
    static const uint16_t kDefaultSkillDelay = 0;
    static const eSKILL_FACTOR_TYPE kDefaultSkillFactor = SKILL_FACTOR_NOMAL;
    static const eATTACK_PROPENSITY kDefaultPropensity = ATTACK_PROPENSITY_NORMAL;
    static const uint8_t kDefaultEvent = 0;
    //
    SkillInfo();
    SkillInfo(const SkillInfo& rhs);
    SkillInfo& operator=(const SkillInfo& rhs);
    SkillInfo(Player* player, const MSG_CG_SKILL_ACTION_SYN& client_request);
    SkillInfo(Player* player, const MSG_CG_BATTLE_PLAYER_ATTACK_SYN& client_request);
    SkillInfo(Player* player, const MSG_CG_ITEM_USE_SKILL_ACTION_SYN& client_request);
    // CHANGES, f100831.3L, change a SkillInfo constructor argument setup process.
    SkillInfo(Character* character, SLOTCODE skill_code, DWORD client_serial_key);
    void SetArgsObjectInfo(DWORD main_target_key,
                           const WzVector* cur_pos, const WzVector* dest_pos,
                           const WzVector* main_target_pos);
    void SetArgsSkillExt(eATTACK_SEQUENCE attack_sequence, // = kDefaultSequence
                         uint8_t skill_effect, // = kDefaultSkillEffect
                         uint16_t skill_delay, // = kDefaultSkillDelay
                         eSKILL_FACTOR_TYPE skill_factor, // = kDefaultSkillFactor
                         eATTACK_PROPENSITY attack_propensity, // = kDefaultPropensity
                         uint8_t event_code);  // = kDefaultEvent
    //
    bool AlignSkillInfo(SLOTCODE skill_code);
    //
    //---------------------------------------------------------------------------------------------
    Character* const character_; // assert(player_ is not null)
    //
    uint8_t field_start_[1];
    // {----------------------------------------------------------------------
    SLOTCODE skill_code_;   // Skill �ڵ�
    const ROOT_SKILLINFO* root_skill_info_;
    DWORD client_serial_;   // Skill �̺�Ʈ�� ���� ���� ��ȣ
    DWORD main_target_key_; // �ְ��ݴ���� ������Ʈ Ű
    //
    SkillOption skill_option_;  //__NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
    uint8_t attack_sequence_;   // ref : eATTACK_SEQUENCE
    uint8_t style_presentation_; // passing data: client usages, _NA003109_STYLE_PRESENTATION_IMPROVEMENT
    uint8_t attack_propensity_; // ���� Ÿ��(eATTACK_PROPENSITY)
    uint8_t skill_factor_; // ������ ������� ���� ��ų�ߵ�(eSKILL_FACTOR_TYPE)
    uint8_t skill_effect_; // ��ų ����Ʈ
    uint8_t event_code_; // to support '__NA_1233_2009_NEW_YEARS_DAY_EVENT'
    uint16_t skill_delay_; // ��ų ������
    //
    WzVector cur_pos_;  // Ŭ���̾�Ʈ�� �ִ� �������� ���� ��ǥ
    WzVector dest_pos_; // �����ڰ� �̵��� ��ǥ
    WzVector main_target_pos_; // �ְ��ݴ���� ��ġ
    //  ----------------------------------------------------------------------}
    uint8_t field_end_[1];

};

//==================================================================================================
// declared to replace 'ArgDamage'
struct DamageArgs
{
    DamageArgs(Character* actor, eATTACK_TYPE attack_type);
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs(Character* actor, eATTACK_TYPE attack_type, SLOTCODE skill_code_);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs(const DamageArgs& rhs);
    DamageArgs& operator=(const DamageArgs& rhs);
    //
    Character* const actor_;
    const eATTACK_TYPE attack_type_;
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    SLOTCODE skill_code_;
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    //
    struct {
        ulong is_critical_ : 1;
        ulong is_mirror_ : 1;
    } option_;
    //
    int limit_hp_;
    eATTACK_RESIST attack_regist_kind_; // registered by __NA_001244_20090417_ATTACK_RESIST
    eSD_APPLY sd_apply_;
    //
    //  ------ result block -------
    DAMAGETYPE damage_;
    // �÷��̾ ���� ������
    // SD, ���忡 �� ���� ��� ���Ե� ���������� ���� ����
    DAMAGETYPE damage_first_;
    //
    static const DamageArgs& kDefaultDamageArgs;
};

//--------------------------------------------------------------------------------------------------
// Ÿ�ٿ��� �������� ����Ǵ� �����Ƽ ȿ��
struct BONUS_ABILITY_EFFECT
{
    DWORD m_dwObjectKey;          // 0 �̸� ��� Ÿ�ٿ��� ����
    int   m_iSkillAttackPower;    // ��ų ���ݷ�
    float m_fSkillPercentDamage;  // ��ų %�߰� ������

    void Init();
};

//--------------------------------------------------------------------------------------------------
// CHANGES: f110303.3L, added transaction block to prevent sending dead notification on skill updating
struct SkillExecutionTransaction
{
    SkillExecutionTransaction();
    ~SkillExecutionTransaction();
    //
    static const int& kIsTransaction;
};


//==================================================================================================
#define SKILL_SYSTEM_VERSION_ROUTING_INL
    #include "./SkillSystem/Skill_Info.inl"
#undef SKILL_SYSTEM_VERSION_ROUTING_INL
//==================================================================================================

#endif //__GAMESERVER_SKILL_INFO_H
