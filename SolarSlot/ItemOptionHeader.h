#ifndef _SOLAR_SLOT_ITEMOPTIONHEADER_H
#define _SOLAR_SLOT_ITEMOPTIONHEADER_H
#pragma once

//==================================================================================================
/// 아이템 옵션 처리 부분 공통 헤더
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
// 옵션관련 구조체
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
    RANK_F = -1,        //< 레어 아이템 등급시 사용
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
// 쟈드 등급 (나중에 등급이 늘어나도 쉽게 추가하기 위해 이름을 숫자 형식으로 붙임)
enum eSOCKET_LEVEL
{
    kZardGradeUnused = 0, // 쟈드 등급 사용 안함
    kZardGrade1 = 1, // 하급
    kZardGrade2 = 2, // 중급
    kZardGrade3 = 3, // 상급
    kZardGrade4 = 4, // 최상급
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
// db 2비트 정보
enum eSOCKET_IDENTIFY_TYPE
{
    eSOCKET_IDENTIFY_NOT_PROCESS = 0, // 소켓 아이템이 아닌경우(일반 아이템), 아직 결정되지 않은 경우(드랍등)
    eSOCKET_UNIDENTIFIED_ITEM,        // 미확인 소켓 아이템 (1차 성공)
    eSOCKET_FAIL_IDENTIFY_PROCESS,    // 소켓 아이템이 되지 못한 아이템 (1차 미확인 소켓 생성 과정에서 실패)
    eSOCKET_IDENTIFIED_ITEM,          // 소켓 갯수 확인한 아이템
};

enum sLIMITED_NUM
{
    LIMITED_1 = 0,
    LIMITED_2,
    LIMITED_3,
    LIMITED_MAX,
};

// _NA_003740_20111122_RANDOM_ITEM
// 랜덤 아이템 값 종류 등급
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

// 랜덤 아이템 값 등급
struct RandomValueGrade
{
    enum Value
    {
        kNone = 0, // 등급 없음
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
// 기본 옵션 정보
// eATTR_TYPE의 많은 속성들 중에서 아이템에서 사용할 것만 따로 뺀 것이다.
// 스킬에서는 모든 속성들을 사용하고 있다.
//-------------------------------------------------------------------------------
// 주의 : 디파인 걸지 않기.
// 서로 다른 디파인 작업으로 인하여 인덱스가 틀려짐
//-------------------------------------------------------------------------------
// NOTE: [V]=value type, [R]=ratio type
//-------------------------------------------------------------------------------
const eATTR_TYPE g_ITEM_OPTION[] = 
{
    eATTR_TYPE_INVALID,                         //[  0] 없음 (초기값)
    eATTR_ATTACK_SPEED,                         //[  1] 전체공격속도추가
    eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,        //[  2] 전체공격성공율추가율
    eATTR_MAGICAL_FIRE_ATTACK_POWER,            //[  3] [V] 원소(불) 공격력
    eATTR_MAGICAL_WATER_ATTACK_POWER,           //[  4] [V] 원소(물) 공격력
    eATTR_MAGICAL_EARTH_ATTACK_POWER,           //[  5] [V] 원소(바람) 공격력
    eATTR_MAGICAL_WIND_ATTACK_POWER,            //[  6] [V] 원소(대지) 공격력
    eATTR_MAGICAL_DARKNESS_ATTACK_POWER,        //[  7] [V] 원소(암흑) 공격력
    eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,    //[  8] [R] 공격대상 물리 방어력 감소율
    eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,        //[  9] [R] 공격대상 원소(불) 방어력 감소율
    eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,       //[ 10] [R] 공격대상 원소(물) 방어력 감소율
    eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,       //[ 11] [R] 공격대상 원소(바람) 방어력 감소율
    eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,        //[ 12] [R] 공격대상 원소(대지) 방어력 감소율
    eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,    //[ 13] [R] 공격대상 원소(암흑) 방어력 감소율
    eATTR_MOVE_SPEED,                           //[ 14] 이동속도추가
    eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,          //[ 15] [R] 공격 방어율 추가율
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_DEL_FIRE_DAMAGE,                      //[ 16] [R] EP2: 원소(불)에 대한 데미지 차감률
    eATTR_DEL_WATER_DAMAGE,                     //[ 17] [R] EP2: 원소(물)에 대한 데미지 차감률
    eATTR_DEL_WIND_DAMAGE,                      //[ 18] [R] EP2: 원소(바람)에 대한 데미지 차감률
    eATTR_DEL_EARTH_DAMAGE,                     //[ 19] [R] EP2: 원소(대지)에 대한 데미지 차감률
    eATTR_DEL_DARKNESS_DAMAGE,                  //[ 20] [R] EP2: 원소(암흑)에 대한 데미지 차감률
#else
    eATTR_MAGICAL_FIRE_DEFENSE_POWER,           //[ 16] EP2:unused, [V] 마법속성저항(불)
    eATTR_MAGICAL_WATER_DEFENSE_POWER,          //[ 17] EP2:unused, [V] 마법속성저항(물)
    eATTR_MAGICAL_EARTH_DEFENSE_POWER,          //[ 18] EP2:unused, [V] 마법속성저항(대지)
    eATTR_MAGICAL_WIND_DEFENSE_POWER,           //[ 19] EP2:unused, [V] 마법속성저항(바람)
    eATTR_MAGICAL_DARKNESS_DEFENSE_POWER,       //[ 20] EP2:unused, [V] 마법속성저항(암흑)
#endif
    eATTR_OPTION_ALL_ATTACK_POWER,              //[ 21] EP2:unused, 옵션 전체 물리 공격력, EP2: 47과 동일
    eATTR_OPTION_ALL_DEFENSE_POWER,             //[ 22] EP2:unused, 옵션 전체 물리 방어력, EP2: 49와 동일
    eATTR_STR,                                  //[ 23] 스탯(힘)
    eATTR_DEX,                                  //[ 24] 스탯(민첩)
    eATTR_INT,                                  //[ 25] 스탯(지력)
    eATTR_VIT,                                  //[ 26] 스탯(체력)
    eATTR_SPR,                                  //[ 27] 스탯(정신력)
    eATTR_MAX_HP,                               //[ 28] 최대 HP
    eATTR_MAX_MP,                               //[ 29] 최대 MP/SP
    eATTR_ADD_SKILL_ATTACK_POWER,               //[ 30] 스킬 추가 공격력
    eATTR_ADD_SKILL_DAMAGE_RATIO,               //[ 31] 스킬 데미지 추가율
    eATTR_INCREASE_STAT_POINT,                  //[ 32] 모든 스텟
    //[2006 오후 1:23:22
    eATTR_DECREASE_LIMIT_STAT,                  //[ 33] 스텟 제한 수치 감소
    eATTR_MP_SPEND_INCREASE,                    //[ 34] MP 소비 증감
    eATTR_MAGICAL_ALL_ATTACK_POWER,             //[ 35] 모든 원소 공격력
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_DEL_MAGICAL_ALL_DAMAGE,               //[ 36] [R] EP2: 원소(모든)에 대한 데미지 차감률
#else
    eATTR_MAGICAL_ALL_DEFENSE_POWER,            //[ 36] EP1: 모든 원소 방어력
#endif
    //[20061206 오전 10:30:01
    eATTR_ABSORB_HP,                            //[ 37] HP 흡수
    eATTR_ABSORB_MP,                            //[ 38] MP/SP 흡수
    eATTR_RECOVERY_HP,                          //[ 39] HP 회복량
    eATTR_RECOVERY_MP,                          //[ 40] MP 회복량
    eATTR_ADD_ALL_CRITICAL_RATIO,               //[ 41] 전체 크리티컬 확률
    eATTR_CRITICAL_DAMAGE_CHANGE,               //[ 42] 크리티컬 데미지
    eATTR_REFLECT_DAMAGE_RATIO,                 //[ 43] 데미지 반사
    eATTR_BONUS_MONEY_RATIO,                    //[ 44] 하임 증가
    eATTR_BONUS_EXP_RATIO,                      //[ 45] 경험치 증가
    eATTR_AREA_ATTACK_RATIO,                    //[ 46] 다중공격 확률
    eATTR_OPTION_PHYSICAL_ATTACK_POWER,         //[ 47] 물리 공격력
    eATTR_OPTION_MAGICAL_ATTACK_POWER,          //[ 48] EP2:unused, 마법 공격력
    eATTR_OPTION_PHYSICAL_DEFENSE_POWER,        //[ 49] 물리 방어력
    eATTR_OPTION_MAGICAL_DEFENSE_POWER,         //[ 50] EP2:unused, 마법 방어력
    eATTR_DECREASE_DAMAGE,                      //[ 51] 데미지 감소 (적용 시점: 적용할 데미지 자체를 감소시킨다.)
    eATTR_RESURRECTION_RATIO,                   //[ 52] 사망시 스스로 부활 할 확률
    eATTR_DOUBLE_DAMAGE_RATIO,                  //[ 53] 데미지의 두배가 될 확률
    eATTR_LUCKMON_INC_DAMAGE,                   //[ 54] 럭키 몬스터 공격시 추가 데미지
    eATTR_COPOSITE_INC_RATIO,                   //[ 55] 조합 성공률 증가
    eATTR_BYPASS_DEFENCE_RATIO,                 //[ 56] 방어력 무시 확율
    eATTR_INCREASE_MIN_DAMAGE,                  //[ 57] 최소 데미지 증가
    eATTR_INCREASE_MAX_DAMAGE,                  //[ 58] 최대 데미지 증가
    eATTR_DECREASE_ITEMDURA_RATIO,              //[ 59] 아이템 내구력 감소무시 확율
    eATTR_RESISTANCE_BADSTATUS_RATIO,           //[ 60] 스킬 효과 무시확율
    eATTR_INCREASE_SKILLDURATION,               //[ 61] 스킬 지속 시간 증가 (버프 계열)
    eATTR_DECREASE_SKILL_SKILLDURATION,         //[ 62] 스킬 지속 시간 증감 (디버프 계열) < f110531.6L, changed from '감소'
    eATTR_OPTION_ETHER_DAMAGE_RATIO,            //[ 63] 에테르웨폰 데미지 변화 (EP2:reserve deletion)
    eATTR_INCREASE_RESERVE_HP,                  //[ 64] 적립 HP 증가
    eATTR_BONUS_CASTING_TIME,                   //[ 65] 스킬 캐스팅 시간 증감.
    eATTR_RESIST_HOLDING,                       //[ 66] 홀딩 내성
    eATTR_RESIST_SLEEP,                         //[ 67] 슬립 내성
    eATTR_RESIST_POISON,                        //[ 68] 중독 내성
    eATTR_RESIST_KNOCKBACK,                     //[ 69] 넉백 내성
    eATTR_RESIST_DOWN,                          //[ 70] 다운 내성
    eATTR_RESIST_STUN,                          //[ 71] 스턴 내성
    eATTR_DECREASE_PVPDAMAGE,                   //[ 72] PVP데미지 감소 (적용 시점:[51:데미지감소] 적용시 PvP한정 추가 계산)
    eATTR_ADD_DAMAGE,                           //[ 73] 추가데미지 (적용 시점: 기본 데미지 공식 마지막)
    eATTR_AUTO_ITEM_PICK_UP,                    //[ 74] Item 자동 줍기
    // __NA_001244_20090417_ATTACK_RESIST
    // CHANGES: f110103.4L, block contents about PvP resist attributes. ranges = [75, 92]
    eATTR_BONUS_SKILL_COOL_TIME,                //[ 75] [R] 스킬 쿨타임 증감 (EP2)
    eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO,        //[ 76] 에테르웨폰 데미지 변화 (PvE)
#ifdef _NA_003966_20111227_ADD_ENCHANT
    eATTR_INCREASE_ENCHANT_RATIO, // [77] 인첸트 성공률 증가
#else
    eATTR_TYPE_INVALID, // [77]  eATTR_RESIST_ALLATTACK_ALLCHAR, //  77 모든케릭 모든공격 내성.
#endif
    eATTR_TYPE_INVALID, // [78]  eATTR_RESIST_NOMALATTACK_BERSERKER, //  78 버서커 일반공격 내성. 
    eATTR_TYPE_INVALID, // [79]  eATTR_RESIST_SKILLATTACK_BERSERKER, //  79 버서커 스킬공격 내성.
    eATTR_TYPE_INVALID, // [80]  eATTR_RESIST_ALLATTACK_BERSERKER, //  80 버서커 모든공격 내성. 
    eATTR_TYPE_INVALID, // [81]  eATTR_RESIST_NOMALATTACK_VALKYRIE, //  81 발키리 일반공격 내성. 
    eATTR_TYPE_INVALID, // [82]  eATTR_RESIST_SKILLATTACK_VALKYRIE, //  82 발키리 스킬공격 내성.
    eATTR_TYPE_INVALID, // [83]  eATTR_RESIST_ALLATTACK_VALKYRIE, //  83 발키리 모든공격 내성. 
    eATTR_TYPE_INVALID, // [84]  eATTR_RESIST_NOMALATTACK_DRAGON, //  84 드래곤 일반공격 내성. 
    eATTR_TYPE_INVALID, // [85]  eATTR_RESIST_SKILLATTACK_DRAGON, //  85 드래곤 스킬공격 내성.
    eATTR_TYPE_INVALID, // [86]  eATTR_RESIST_ALLATTACK_DRAGON, //  86 드래곤 모든공격 내성. 
    eATTR_TYPE_INVALID, // [87]  eATTR_RESIST_NOMALATTACK_ELEMENTALIST, //  87 엘리 일반공격 내성.  
    eATTR_TYPE_INVALID, // [88]  eATTR_RESIST_SKILLATTACK_ELEMENTALIST, //  88 엘리 스킬공격 내성.  
    eATTR_TYPE_INVALID, // [89]  eATTR_RESIST_ALLATTACK_ELEMENTALIST, //  89 엘리 모든공격 내성.  
    eATTR_TYPE_INVALID, // [90]  eATTR_RESIST_NOMALATTACK_SHADOW, //  90 쇄도우 일반공격 내성.     
    eATTR_TYPE_INVALID, // [91]  eATTR_RESIST_SKILLATTACK_SHADOW, //  91 쇄도우 스킬공격 내성. 
    eATTR_TYPE_INVALID, // [92]  eATTR_RESIST_ALLATTACK_SHADOW, //  92 쇄도우 모든공격 내성. 
    // __NA_001290_20090525_SHIELD_SYSTEM
    eATTR_MAX_SD,                               //[ 93] 최대 SD
    eATTR_RECOVERY_SD,                          //[ 94] SD 회복량
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO, //[ 95] [R] 공격대상 원소(모든) 방어력 감소율 (be changed type from [V])
#else
    eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO,     //[ 95] EP2:unused, 공격대상 마법 방어력 감소
#endif
    eATTR_ENEMY_CRITICAL_RATIO_CHANGE,              //[ 96] 피격 시 상대의 크리티컬 확률 감소
    eATTR_PREMIUMSERVICE_PCBANG,                //[ 97] PC방 효과

    //_NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    eATTR_ATTACK_DAMAGE_ABSORB_SD_RATIO,         //[ 98] 가해 데미지 SD 전환량
    eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO,         //[ 99] 가해 데미지 HP 전환량
    
    //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE,             //[100]피격 시 상대의 크리티컬 데미지 증감
    eATTR_CRAFT_COST_RATIO,                         // [101] 제작 비용 증감
    eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO,  // [102] 제작 실패시 재료 소멸 방지
    eATTR_ENCHANT_COST_RATIO,                       // [103] 인챈트 비용 증감
    eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO, // [104] 인챈트 실패시 아이템 소멸, 다운 방지
    eATTR_RECOVER_POTION_COOLTIME_RATIO,            // [105] 회복 포션 쿨타임 감소
    eATTR_RECOVER_POTION_RECOVERY_RATIO,            // [106] 회복 포션 회복량 증가
    eATTR_QUEST_REWARD_EXP_RATIO,                   // [107] 퀘스트 보상 경험치 증가
    eATTR_MAX_DAMAGE_RATIO,                         // [108] 최대 데미지 발생확률 증감
    eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO,        // [109] 공성 오브젝트 데미지 증감
    eATTR_SHOP_REPAIR_HEIM_RATIO,                   // [110] 상점 수리 하임 증감
    eATTR_SHOP_BUY_HEIM_RATIO,                      // [111] 상점 구매 하임 증감

    eATTR_MAX_FP,                                   // [112] 위치블레이드 최대FP
    eATTR_RECOVERY_FP,                              // [113] 위치블레이드 FP회복량 (공격시)
    eATTR_INCREASE_DAMAGE_RATIO,                    // [114] 데미지 증가 
    //_NA_008124_20150313_AWAKENING_SYSTEM
    eATTR_AWAKENING_PROBABILITY,                    // [115] 각성 확률 증감

    //_NA_008486_20150914_TOTAL_BALANCE
    eATTR_DEBUFF_DURATION,                          // [116] 자산이ㅡ 디버프기술 효과지속시간 강화(ms)
    
    //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    eATTR_DECREASE_DAMAGE_NPC,                      // [117] npc가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_BERSERKER,                // [118] 버서커가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_DRAGONKNIGHT,             // [119] 드래곤나이트가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_VALKYRIE,                 // [120] 발키리가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_ELEMENTALIST,             // [121] 엘리멘탈리스트가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_SHADOW,                   // [122] 섀도우가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_MYSTIC,                   // [123] 미스틱이 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_HELLROID,                 // [124] 헬로이드가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_WITCHBLADE,               // [125] 위치블레이드가 주는 데미지 감소
}; // (WARNING) 127까지 사용이 가능하다. 그 뒤로는 스트림 확장(골 때리는)이 이뤄지므로, 주의할 것.

// 순서가 바뀌면 안됨!!!!
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
    // OptionIndex 유효성 체크
    if (bool out_of_ranges = (option_index >= _countof(g_ITEM_OPTION))) {
        return false;
    };
    bool result = CheckAttrValidity(g_ITEM_OPTION[option_index], option_type, option_value);
    return result;
}

//==================================================================================================

#endif //_SOLAR_SLOT_ITEMOPTIONHEADER_H


