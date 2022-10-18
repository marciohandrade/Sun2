#ifndef _SOLAR_SLOT_ITEMOPTIONHEADER_H
#define _SOLAR_SLOT_ITEMOPTIONHEADER_H
#pragma once

//==================================================================================================
/// ������ �ɼ� ó�� �κ� ���� ���
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 
    @note
    - 
    @history 
    - 09.09.06 | waverix | code rearrangement
*/
//==================================================================================================

// NOTE: f101221.3L, add new namespace 'ns_formula' to group calculation logic
namespace ns_formula {};
using namespace ns_formula; // to compatible custom logic

//--------------------------------------------------------------------------------------------------
// �ɼǰ��� ����ü
//-------------------------------------------------------------------------

enum eENCHANTLEVEL
{
    ENCHANT_LOW,        //< 0 ~ 6
    ENCHANT_MIDDLE,     //< 7 ~ 9
    ENCHANT_HIGH,       //< 10 ~ 12
#ifdef _NA_003966_20111227_ADD_ENCHANT
    ENCHANT_HIGHEST, // 13~15
#endif //_NA_003966_20111227_ADD_ENCHANT
    ENCHANT_MAX,
};

enum eRANK
{
    RANK_F = -1,        //< ���� ������ ��޽� ���
    RANK_E = 0,
    RANK_D,
    RANK_C,
    RANK_B,
    RANK_MA,
    RANK_A,
    RANK_PA,
    RANK_MS,
    RANK_S,
    RANK_PS,
    RANK_MAX,
};

enum eRANK_LEVEL
{
    RANK_LOW    = 0,    //< E ~ B
    RANK_MIDDLE = 1,    //< -A ~ +A
    RANK_HIGH   = 2,    //< -S ~ +S
};

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
// ��� ��� (���߿� ����� �þ�� ���� �߰��ϱ� ���� �̸��� ���� �������� ����)
enum eSOCKET_LEVEL
{
    kZardGradeUnused = 0, // ��� ��� ��� ����
    kZardGrade1 = 1, // �ϱ�
    kZardGrade2 = 2, // �߱�
    kZardGrade3 = 3, // ���
    kZardGrade4 = 4, // �ֻ��
    kZardGradeMax, 
};
#else
enum eSOCKET_LEVEL
{
    SOCKETLV_LOW = 0,
    SOCKETLV_HIGH,
    SOCKETLV_MAX,
};
#endif // _NA_000251_20100727_SOCKET_SYSTEM

enum eSOCKET
{
    SOCKET_1 = 0,
    SOCKET_2,
    SOCKET_3,
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    SOCKET_4,
    SOCKET_5,
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    SOCKET_MAX,
};

enum { SOCKET_MAX_V8 = 5 };
enum { SOCKET_MAX_V7 = 3 };


// _NA_0_20100727_SOCKET_SYSTEM_CONVERT
// db 2��Ʈ ����
enum eSOCKET_IDENTIFY_TYPE
{
    eSOCKET_IDENTIFY_NOT_PROCESS = 0, // ���� �������� �ƴѰ��(�Ϲ� ������), ���� �������� ���� ���(�����)
    eSOCKET_UNIDENTIFIED_ITEM,        // ��Ȯ�� ���� ������ (1�� ����)
    eSOCKET_FAIL_IDENTIFY_PROCESS,    // ���� �������� ���� ���� ������ (1�� ��Ȯ�� ���� ���� �������� ����)
    eSOCKET_IDENTIFIED_ITEM,          // ���� ���� Ȯ���� ������
};

enum sLIMITED_NUM
{
    LIMITED_1 = 0,
    LIMITED_2,
    LIMITED_3,
    LIMITED_MAX,
};

// _NA_003740_20111122_RANDOM_ITEM
// ���� ������ �� ���� ���
struct RandomValueTypeGrade
{
    enum Value
    {
        kGrade1 = 1, 
        kGrade2 = 2
    };
    enum Boundary
    {
        kMinGrade = kGrade1, 
        kMaxGrade = kGrade2
    };
}; // RandomValueTypeGrade

// ���� ������ �� ���
struct RandomValueGrade
{
    enum Value
    {
        kNone = 0, // ��� ����
        kGrade1 = 1, 
        kGrade2 = 2, 
        kGrade3 = 3, 
        kGrade4 = 4, 
        kGrade5 = 5,
        kGrade6 = 6
    };
    enum Boundary
    {
        kMinGrade = kGrade1, 
        kMaxGrade = kGrade6
    };
}; // RandomValueGrade

//--------------------------------------------------------------------------------------------------
// �⺻ �ɼ� ����
// eATTR_TYPE�� ���� �Ӽ��� �߿��� �����ۿ��� ����� �͸� ���� �� ���̴�.
// ��ų������ ��� �Ӽ����� ����ϰ� �ִ�.
//-------------------------------------------------------------------------------
// ���� : ������ ���� �ʱ�.
// ���� �ٸ� ������ �۾����� ���Ͽ� �ε����� Ʋ����
//-------------------------------------------------------------------------------
// NOTE: [V]=value type, [R]=ratio type
//-------------------------------------------------------------------------------
const eATTR_TYPE g_ITEM_OPTION[] = 
{
    eATTR_TYPE_INVALID,                         //[  0] ���� (�ʱⰪ)
    eATTR_ATTACK_SPEED,                         //[  1] ��ü���ݼӵ��߰�
    eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,        //[  2] ��ü���ݼ������߰���
    eATTR_MAGICAL_FIRE_ATTACK_POWER,            //[  3] [V] ����(��) ���ݷ�
    eATTR_MAGICAL_WATER_ATTACK_POWER,           //[  4] [V] ����(��) ���ݷ�
    eATTR_MAGICAL_EARTH_ATTACK_POWER,           //[  5] [V] ����(�ٶ�) ���ݷ�
    eATTR_MAGICAL_WIND_ATTACK_POWER,            //[  6] [V] ����(����) ���ݷ�
    eATTR_MAGICAL_DARKNESS_ATTACK_POWER,        //[  7] [V] ����(����) ���ݷ�
    eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,    //[  8] [R] ���ݴ�� ���� ���� ������
    eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,        //[  9] [R] ���ݴ�� ����(��) ���� ������
    eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,       //[ 10] [R] ���ݴ�� ����(��) ���� ������
    eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,       //[ 11] [R] ���ݴ�� ����(�ٶ�) ���� ������
    eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,        //[ 12] [R] ���ݴ�� ����(����) ���� ������
    eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,    //[ 13] [R] ���ݴ�� ����(����) ���� ������
    eATTR_MOVE_SPEED,                           //[ 14] �̵��ӵ��߰�
    eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,          //[ 15] [R] ���� ����� �߰���
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_DEL_FIRE_DAMAGE,                      //[ 16] [R] EP2: ����(��)�� ���� ������ ������
    eATTR_DEL_WATER_DAMAGE,                     //[ 17] [R] EP2: ����(��)�� ���� ������ ������
    eATTR_DEL_WIND_DAMAGE,                      //[ 18] [R] EP2: ����(�ٶ�)�� ���� ������ ������
    eATTR_DEL_EARTH_DAMAGE,                     //[ 19] [R] EP2: ����(����)�� ���� ������ ������
    eATTR_DEL_DARKNESS_DAMAGE,                  //[ 20] [R] EP2: ����(����)�� ���� ������ ������
#else
    eATTR_MAGICAL_FIRE_DEFENSE_POWER,           //[ 16] EP2:unused, [V] �����Ӽ�����(��)
    eATTR_MAGICAL_WATER_DEFENSE_POWER,          //[ 17] EP2:unused, [V] �����Ӽ�����(��)
    eATTR_MAGICAL_EARTH_DEFENSE_POWER,          //[ 18] EP2:unused, [V] �����Ӽ�����(����)
    eATTR_MAGICAL_WIND_DEFENSE_POWER,           //[ 19] EP2:unused, [V] �����Ӽ�����(�ٶ�)
    eATTR_MAGICAL_DARKNESS_DEFENSE_POWER,       //[ 20] EP2:unused, [V] �����Ӽ�����(����)
#endif
    eATTR_OPTION_ALL_ATTACK_POWER,              //[ 21] EP2:unused, �ɼ� ��ü ���� ���ݷ�, EP2: 47�� ����
    eATTR_OPTION_ALL_DEFENSE_POWER,             //[ 22] EP2:unused, �ɼ� ��ü ���� ����, EP2: 49�� ����
    eATTR_STR,                                  //[ 23] ����(��)
    eATTR_DEX,                                  //[ 24] ����(��ø)
    eATTR_INT,                                  //[ 25] ����(����)
    eATTR_VIT,                                  //[ 26] ����(ü��)
    eATTR_SPR,                                  //[ 27] ����(���ŷ�)
    eATTR_MAX_HP,                               //[ 28] �ִ� HP
    eATTR_MAX_MP,                               //[ 29] �ִ� MP/SP
    eATTR_ADD_SKILL_ATTACK_POWER,               //[ 30] ��ų �߰� ���ݷ�
    eATTR_ADD_SKILL_DAMAGE_RATIO,               //[ 31] ��ų ������ �߰���
    eATTR_INCREASE_STAT_POINT,                  //[ 32] ��� ����
    //[2006 ���� 1:23:22
    eATTR_DECREASE_LIMIT_STAT,                  //[ 33] ���� ���� ��ġ ����
    eATTR_MP_SPEND_INCREASE,                    //[ 34] MP �Һ� ����
    eATTR_MAGICAL_ALL_ATTACK_POWER,             //[ 35] ��� ���� ���ݷ�
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_DEL_MAGICAL_ALL_DAMAGE,               //[ 36] [R] EP2: ����(���)�� ���� ������ ������
#else
    eATTR_MAGICAL_ALL_DEFENSE_POWER,            //[ 36] EP1: ��� ���� ����
#endif
    //[20061206 ���� 10:30:01
    eATTR_ABSORB_HP,                            //[ 37] HP ���
    eATTR_ABSORB_MP,                            //[ 38] MP/SP ���
    eATTR_RECOVERY_HP,                          //[ 39] HP ȸ����
    eATTR_RECOVERY_MP,                          //[ 40] MP ȸ����
    eATTR_ADD_ALL_CRITICAL_RATIO,               //[ 41] ��ü ũ��Ƽ�� Ȯ��
    eATTR_CRITICAL_DAMAGE_CHANGE,               //[ 42] ũ��Ƽ�� ������
    eATTR_REFLECT_DAMAGE_RATIO,                 //[ 43] ������ �ݻ�
    eATTR_BONUS_MONEY_RATIO,                    //[ 44] ���� ����
    eATTR_BONUS_EXP_RATIO,                      //[ 45] ����ġ ����
    eATTR_AREA_ATTACK_RATIO,                    //[ 46] ���߰��� Ȯ��
    eATTR_OPTION_PHYSICAL_ATTACK_POWER,         //[ 47] ���� ���ݷ�
    eATTR_OPTION_MAGICAL_ATTACK_POWER,          //[ 48] EP2:unused, ���� ���ݷ�
    eATTR_OPTION_PHYSICAL_DEFENSE_POWER,        //[ 49] ���� ����
    eATTR_OPTION_MAGICAL_DEFENSE_POWER,         //[ 50] EP2:unused, ���� ����
    eATTR_DECREASE_DAMAGE,                      //[ 51] ������ ���� (���� ����: ������ ������ ��ü�� ���ҽ�Ų��.)
    eATTR_RESURRECTION_RATIO,                   //[ 52] ����� ������ ��Ȱ �� Ȯ��
    eATTR_DOUBLE_DAMAGE_RATIO,                  //[ 53] �������� �ι谡 �� Ȯ��
    eATTR_LUCKMON_INC_DAMAGE,                   //[ 54] ��Ű ���� ���ݽ� �߰� ������
    eATTR_COPOSITE_INC_RATIO,                   //[ 55] ���� ������ ����
    eATTR_BYPASS_DEFENCE_RATIO,                 //[ 56] ���� ���� Ȯ��
    eATTR_INCREASE_MIN_DAMAGE,                  //[ 57] �ּ� ������ ����
    eATTR_INCREASE_MAX_DAMAGE,                  //[ 58] �ִ� ������ ����
    eATTR_DECREASE_ITEMDURA_RATIO,              //[ 59] ������ ������ ���ҹ��� Ȯ��
    eATTR_RESISTANCE_BADSTATUS_RATIO,           //[ 60] ��ų ȿ�� ����Ȯ��
    eATTR_INCREASE_SKILLDURATION,               //[ 61] ��ų ���� �ð� ���� (���� �迭)
    eATTR_DECREASE_SKILL_SKILLDURATION,         //[ 62] ��ų ���� �ð� ���� (����� �迭) < f110531.6L, changed from '����'
    eATTR_OPTION_ETHER_DAMAGE_RATIO,            //[ 63] ���׸����� ������ ��ȭ (EP2:reserve deletion)
    eATTR_INCREASE_RESERVE_HP,                  //[ 64] ���� HP ����
    eATTR_BONUS_CASTING_TIME,                   //[ 65] ��ų ĳ���� �ð� ����.
    eATTR_RESIST_HOLDING,                       //[ 66] Ȧ�� ����
    eATTR_RESIST_SLEEP,                         //[ 67] ���� ����
    eATTR_RESIST_POISON,                        //[ 68] �ߵ� ����
    eATTR_RESIST_KNOCKBACK,                     //[ 69] �˹� ����
    eATTR_RESIST_DOWN,                          //[ 70] �ٿ� ����
    eATTR_RESIST_STUN,                          //[ 71] ���� ����
    eATTR_DECREASE_PVPDAMAGE,                   //[ 72] PVP������ ���� (���� ����:[51:����������] ����� PvP���� �߰� ���)
    eATTR_ADD_DAMAGE,                           //[ 73] �߰������� (���� ����: �⺻ ������ ���� ������)
    eATTR_AUTO_ITEM_PICK_UP,                    //[ 74] Item �ڵ� �ݱ�
    // __NA_001244_20090417_ATTACK_RESIST
    // CHANGES: f110103.4L, block contents about PvP resist attributes. ranges = [75, 92]
    eATTR_BONUS_SKILL_COOL_TIME,                //[ 75] [R] ��ų ��Ÿ�� ���� (EP2)
    eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO,        //[ 76] ���׸����� ������ ��ȭ (PvE)
#ifdef _NA_003966_20111227_ADD_ENCHANT
    eATTR_INCREASE_ENCHANT_RATIO, // [77] ��þƮ ������ ����
#else
    eATTR_TYPE_INVALID, // [77]  eATTR_RESIST_ALLATTACK_ALLCHAR, //  77 ����ɸ� ������ ����.
#endif
    eATTR_TYPE_INVALID, // [78]  eATTR_RESIST_NOMALATTACK_BERSERKER, //  78 ����Ŀ �Ϲݰ��� ����. 
    eATTR_TYPE_INVALID, // [79]  eATTR_RESIST_SKILLATTACK_BERSERKER, //  79 ����Ŀ ��ų���� ����.
    eATTR_TYPE_INVALID, // [80]  eATTR_RESIST_ALLATTACK_BERSERKER, //  80 ����Ŀ ������ ����. 
    eATTR_TYPE_INVALID, // [81]  eATTR_RESIST_NOMALATTACK_VALKYRIE, //  81 ��Ű�� �Ϲݰ��� ����. 
    eATTR_TYPE_INVALID, // [82]  eATTR_RESIST_SKILLATTACK_VALKYRIE, //  82 ��Ű�� ��ų���� ����.
    eATTR_TYPE_INVALID, // [83]  eATTR_RESIST_ALLATTACK_VALKYRIE, //  83 ��Ű�� ������ ����. 
    eATTR_TYPE_INVALID, // [84]  eATTR_RESIST_NOMALATTACK_DRAGON, //  84 �巡�� �Ϲݰ��� ����. 
    eATTR_TYPE_INVALID, // [85]  eATTR_RESIST_SKILLATTACK_DRAGON, //  85 �巡�� ��ų���� ����.
    eATTR_TYPE_INVALID, // [86]  eATTR_RESIST_ALLATTACK_DRAGON, //  86 �巡�� ������ ����. 
    eATTR_TYPE_INVALID, // [87]  eATTR_RESIST_NOMALATTACK_ELEMENTALIST, //  87 ���� �Ϲݰ��� ����.  
    eATTR_TYPE_INVALID, // [88]  eATTR_RESIST_SKILLATTACK_ELEMENTALIST, //  88 ���� ��ų���� ����.  
    eATTR_TYPE_INVALID, // [89]  eATTR_RESIST_ALLATTACK_ELEMENTALIST, //  89 ���� ������ ����.  
    eATTR_TYPE_INVALID, // [90]  eATTR_RESIST_NOMALATTACK_SHADOW, //  90 �⵵�� �Ϲݰ��� ����.     
    eATTR_TYPE_INVALID, // [91]  eATTR_RESIST_SKILLATTACK_SHADOW, //  91 �⵵�� ��ų���� ����. 
    eATTR_TYPE_INVALID, // [92]  eATTR_RESIST_ALLATTACK_SHADOW, //  92 �⵵�� ������ ����. 
    // __NA_001290_20090525_SHIELD_SYSTEM
    eATTR_MAX_SD,                               //[ 93] �ִ� SD
    eATTR_RECOVERY_SD,                          //[ 94] SD ȸ����
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO, //[ 95] [R] ���ݴ�� ����(���) ���� ������ (be changed type from [V])
#else
    eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO,     //[ 95] EP2:unused, ���ݴ�� ���� ���� ����
#endif
    eATTR_ENEMY_CRITICAL_RATIO_CHANGE,              //[ 96] �ǰ� �� ����� ũ��Ƽ�� Ȯ�� ����
    eATTR_PREMIUMSERVICE_PCBANG,                //[ 97] PC�� ȿ��

    //_NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    eATTR_ATTACK_DAMAGE_ABSORB_SD_RATIO,         //[ 98] ���� ������ SD ��ȯ��
    eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO,         //[ 99] ���� ������ HP ��ȯ��
    
    //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE,             //[100]�ǰ� �� ����� ũ��Ƽ�� ������ ����
    eATTR_CRAFT_COST_RATIO,                         // [101] ���� ��� ����
    eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO,  // [102] ���� ���н� ��� �Ҹ� ����
    eATTR_ENCHANT_COST_RATIO,                       // [103] ��æƮ ��� ����
    eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO, // [104] ��æƮ ���н� ������ �Ҹ�, �ٿ� ����
    eATTR_RECOVER_POTION_COOLTIME_RATIO,            // [105] ȸ�� ���� ��Ÿ�� ����
    eATTR_RECOVER_POTION_RECOVERY_RATIO,            // [106] ȸ�� ���� ȸ���� ����
    eATTR_QUEST_REWARD_EXP_RATIO,                   // [107] ����Ʈ ���� ����ġ ����
    eATTR_MAX_DAMAGE_RATIO,                         // [108] �ִ� ������ �߻�Ȯ�� ����
    eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO,        // [109] ���� ������Ʈ ������ ����
    eATTR_SHOP_REPAIR_HEIM_RATIO,                   // [110] ���� ���� ���� ����
    eATTR_SHOP_BUY_HEIM_RATIO,                      // [111] ���� ���� ���� ����

    eATTR_MAX_FP,                                   // [112] ��ġ���̵� �ִ�FP
    eATTR_RECOVERY_FP,                              // [113] ��ġ���̵� FPȸ���� (���ݽ�)
    eATTR_INCREASE_DAMAGE_RATIO,                    // [114] ������ ���� 
    //_NA_008124_20150313_AWAKENING_SYSTEM
    eATTR_AWAKENING_PROBABILITY,                    // [115] ���� Ȯ�� ����

    //_NA_008486_20150914_TOTAL_BALANCE
    eATTR_DEBUFF_DURATION,                          // [116] �ڻ��̤� �������� ȿ�����ӽð� ��ȭ(ms)
    
    //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    eATTR_DECREASE_DAMAGE_NPC,                      // [117] npc�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_BERSERKER,                // [118] ����Ŀ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_DRAGONKNIGHT,             // [119] �巡�ﳪ��Ʈ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_VALKYRIE,                 // [120] ��Ű���� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_ELEMENTALIST,             // [121] ������Ż����Ʈ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_SHADOW,                   // [122] �����찡 �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_MYSTIC,                   // [123] �̽�ƽ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_HELLROID,                 // [124] ����̵尡 �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_WITCHBLADE,               // [125] ��ġ���̵尡 �ִ� ������ ����
}; // (WARNING) 127���� ����� �����ϴ�. �� �ڷδ� ��Ʈ�� Ȯ��(�� ������)�� �̷����Ƿ�, ������ ��.

// ������ �ٲ�� �ȵ�!!!!
//--------------------------------------------------------------------------------------------------

namespace ns_formula {
;

// f101221.3L, f090906
// argumens:
//  option_type : eVALUE_TYPE
extern bool CheckAttrValidity(eATTR_TYPE attr_type, int option_type, int option_value);
//                         ref: index of g_ITEM_OPTION
// argumens:
//  option_index : the index of g_ITEM_OPTION
//  option_type : eVALUE_TYPE
extern bool CheckOptionValidity(WORD option_index, int option_type, int option_value);
// NOTE: f110323.3L, this method is planned for item attribute calculation prefetching and
// restriction filter of stable item option calculations
//                         ref: index of g_ITEM_OPTION
extern bool CheckStatsRelatedOption(uint16_t option_index);

}; //end of namespace 'ns_formula'

//==================================================================================================

inline bool ns_formula::CheckOptionValidity(WORD option_index, int option_type, int option_value)
{
    // OptionIndex ��ȿ�� üũ
    if (bool out_of_ranges = (option_index >= _countof(g_ITEM_OPTION))) {
        return false;
    };
    bool result = CheckAttrValidity(g_ITEM_OPTION[option_index], option_type, option_value);
    return result;
}

//==================================================================================================

#endif //_SOLAR_SLOT_ITEMOPTIONHEADER_H


