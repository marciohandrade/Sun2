#ifndef _SOLAR_SLOT_SCSLOTSTRUCT_H
#define _SOLAR_SLOT_SCSLOTSTRUCT_H
#pragma once

#include "SCSlotHeader.h"
#include "SCSlotStruct_Version.h"
// (CHANGES) move to a pre-include section
class SCItemSlot;
#include "SCItemStreamSerializer.h" //internal include file

//==================================================================================================
// CHANGES: f090923.x, waverix, BASE_ITEMINFO의 Pack구조 변환, 자주 사용하는 건데... (389->412)
// CHANGES: f100520.3L, critical changes : changes code about a 'pragma pack'
#pragma pack(push)
#pragma pack()
//==================================================================================================
struct BASE_ITEMINFO
{
    //----------------------------------------------------------------------------------------------
    // Common parameters
    struct {
        SLOTCODE m_Code;    //< 코드
        BYTE m_ChargeType;  //< ref: eCHARGE_TYPE
        BYTE m_ChargeSubType; //< ref: eCHARGE_SUB_TYPE
        //
        nsSlot::ItemRuleInfo item_rule_info;
        //
        WORD m_wType;       //< ref: eITEMTYPE
        WORD m_wSubType;    //< eITEMSUBTYPE (ex: 0=일반 1=엘리트, ...)
        ItemSubTypeDetail sub_type_detailed; // f110414.1L, integrated sub_type and level
        //
        POSTYPE m_wEqPos;   //< 장비 위치 (ref: eEQUIPCONTAINER_POS)
        BYTE m_byEventCode; //< rarely used cases
        // CHANGES: f110408.3L, added tag for external control facilities like a 'SunEventInfo'.
        uint8_t time_expiration_item; // _SERVER factility
        LEVELTYPE m_LV;     //< 레벨
    };
#if defined(_DEBUG) // to support catchable point in of debug view ranges 
    char m_pszName[MAX_ITEMNAME_LENGTH]; //< 디버그용 이름(사용안함)
#endif

    //----------------------------------------------------------------------------------------------
    // Functionality parameters
    typedef BYTE ZardGrade;
    typedef BYTE ZardOption;
    typedef WORD ZardRatio;

    struct {
        // CHANGES: f100510.1L, change the field name to replace use a field
        WORD trade_sell_type; // Use SCItemSlot::CanTradeSell(), ref: eITEM_TRADESELL_TYPE
        BYTE m_byMaterialType; // ref: eITEM_MATERIAL_TYPE, 장비사용여부
        BYTE m_byMixture; //< Use SCItemSlot::CanUseMixture(), ref: eMIXTURE_FLAG, 각종 가능 여부(랭크업,조합,인첸트,결정화...etc)
        BYTE m_byWasteType; // eWASTETYPE
        BYTE m_bPossessionItem; //< 귀속 아이템인가 BOOL로 이용하지만 사이즈 위해 BYTE 사용
        // _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_ (add) +
        // _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM (extend)
        eEtherealizeMode etherealization_mode_;
    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        ZardGrade zard_grade; // ref: eSOCKET_LEVEL, 쟈드 등급
        ZardOption zard_item_option_kind; // 쟈드의 아이템 기준 옵션 종류 (g_ITEM_OPTION)
        ZardRatio zard_success_per; // 쟈드 합성 추가 성공률
    #else
        WORD m_wMaxRank;    //< 최대 랭크??
        WORD m_SocketHole;  //< ver1.1 : 뚫여진 소켓 구멍 개수
        WORD m_SocketNum;
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
    };
    //----------------------------------------------------------------------------------------------
    // Durability parameters
    struct {
        DURATYPE m_Dura;    //< 내구도
        //__NA_001062_20080623_STRENGTH_PENALTY
        uint8_t m_dwStrengthPenaltyIdx;//< 내구도 패널티 인덱스
        DWORD m_dwDuraStr;  //< 내구도 강도
    };
    //----------------------------------------------------------------------------------------------
    // Set Item parameters
    // (WAVERIX) (ADD-IN) eSetItemOption추가. 하드코딩된 값 대체하기로 한다. 작업 매크로에서도 볼 수
    // 있듯이 너무 무성의하게 작업한 경향이 있다.
    enum eSetItemOption { eSetItem_None, eSetItem_Active, eSetItem_Special, eSetItem_Count };
    struct {
        BYTE m_bySetOptionType; //< eSetItemOption = { 0=비활성, 1=활성, 2=스페셜세트 아이템 }
        WORD set_group_code;   //< ver1.2 SetItemOptionInfo.txt::wSetGCode 필드와 같음
    };
    //----------------------------------------------------------------------------------------------
    // multi-purpose usages
    struct {
        WORD m_wSkillCode;
    };
    //----------------------------------------------------------------------------------------------
    // worth parameters of item
    struct {
        DWORD m_DropLV;     //< 드랍레벨
    };

    //----------------------------------------------------------------------------------------------
    // default item option parameters by script definitions
    struct {
        WORD m_wPhyAttRate;     // EP2:applied, percentage (0.95 -> 95%)
        WORD m_wPhyAttSpeed;
        WORD m_wPhyDef;
        WORD m_wPhyAvoid;       // EP2:applied, percentage (0.95 -> 95%)

        BYTE m_byBaseAttType;   //< 기본   공격타입(근거리, 원거리, 마법)
        BYTE m_byMeleeType;     //< 근거리 공격 타입(1:관통, 2:타격, 3:베기)
        BYTE m_byMagicalAttType; //< 속성   공격타입(물, 불, ...)

        float m_fAttRange;
        BYTE m_byArmorType;     //< 아머 타입(1:Hard, 2:Medium, 3:Soft, 4:Siege, 5:Unarmer)

        WORD m_wMinDamage;
        WORD m_wMaxDamage;

        WORD m_wMagicAttMinDamgage;
        WORD m_wMagicAttMaxDamgage;
        WORD m_wMagicAttSpeed;
        WORD m_wMagicDef;

        WORD m_wSpeed;
    };
    //----------------------------------------------------------------------------------------------
    // additional item attribute parameters by script definitions
    //  ver1.1 : 옵션 개수 2->3로 추가
    //  CHANGES: type definition|resorting
    enum { _MAX_OPTION_NUM = 5 };
    // CHANGES: f110329.5L, moved from implementation
    typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_INVEN,
                                BASE_ITEMINFO::_MAX_OPTION_NUM> OPTION_EFFECT_INVEN_MASK;
    typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_EQUIP,
                                BASE_ITEMINFO::_MAX_OPTION_NUM> OPTION_EFFECT_EQUIP_MASK;

    typedef BYTE OptionEffects[_MAX_OPTION_NUM]; // eITEM_EFFECT_ON_(EQUIP|INVEN)
    typedef WORD OptionIndexes[_MAX_OPTION_NUM]; // g_ITEM_OPTION[value] => eATTR_TYPE
    typedef BYTE OptionTypes[_MAX_OPTION_NUM];   // eVALUE_TYPE, use = { 1, 2, don't use etc }
    typedef int  OptionValues[_MAX_OPTION_NUM];
    struct {
        // Normal options
        OptionEffects m_byOptionExEffect;
        OptionIndexes m_wOptionIndex;
        OptionTypes   m_byOptionType;
        OptionValues  m_iOptionValue;
        // Pc-방 options
        OptionEffects m_byPCOptionExEffect;
        OptionIndexes m_wPCOptionIndex;
        OptionTypes   m_byPCOptionType;
        OptionValues  m_iPCOptionValue;
    };

    //----------------------------------------------------------------------------------------------
    // data parameters
    struct {
        uint8_t m_wACCode; // ref: AchievementManager::OBJECT_TYPE_(xxx), changes to smalltype the ep2 AC system
        BYTE m_byACReferenceID;
        //
        eRIDER_CLASS m_RidingClass;
        //
        WORD m_wQuestCode;  //< QCODE, 아이템 사용에 따른 퀘스트 자동 수락시 필요.
        BYTE m_byChaoTimeDec; // number in the minutes, __NA_1323_CHAO_TIME_DECREASE_ITEM

        WORD m_wCoolTimeType; // NOTE: item grouping in items, this items has same cool-time group
        DWORD m_dCoolTime; // the usages same with m_wCoolTimeType
        EXPTYPE m_dwExp; // used only, if m_byWasteType is eITEMWASTE_EXP_POTION

        WORD m_wTimes; // if m_wTimes exist, m_wHealHP and m_wHealHPTime are activated
        WORD m_wHealHP; // used only, if m_byWasteType is eITEMWASTE_(HPPOTION, ...) that is in eWASTETYPE
        DWORD m_wHealHPTime; // the usages same with m_wHealHP

        SLOTCODE m_MaterialCode; // used by decomposition facilities
        SLOTCODE m_ExchangedItem; //< 1:1 교환되는 아이템, (+ zard item exchangable item contents also used)

        INT32 m_betakey; //_NA_20100307_BETAKEY_SYSTEM
    };
    //----------------------------------------------------------------------------------------------
    // default restriction parameters
    struct {
        //_NA_002050_20110216_ADD_GENDER
        BYTE gender_type; // 사용 가능한 성별 (0:모두, 1:남성, 2:여성 Const.h GenderType 참고)
        // NOTE: f110408.2L, added composed bit field = { (1 << eCHAR_BERSERKER) | ... }
        uint16_t equip_class_bits;

        LEVELTYPE m_LimitEqLevel;
        LEVELTYPE m_LimitUseLevel; // restriction about when player use the item, __CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK

        WORD m_wLimitStr;
        WORD m_wLimitDex;
        WORD m_wLimitVit;
        WORD m_wLimitInt;
        WORD m_wLimitSpr;
        WORD m_wLimitSkil1;
        WORD m_wLimitSkil2;

        WORD m_wEqClass1; // Berserker
        WORD m_wEqClass2; // Dragon Knight
        WORD m_wEqClass3; // Shadow
        WORD m_wEqClass4; // Valkyrie
        WORD m_wEqClass5; // Elementalist
        WORD m_wEqClass6; // 미스틱 착용 가능 여부 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        WORD m_wEqClass7; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        WORD m_wEqClass8; //위블 착용 가능 여부 _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
    //----------------------------------------------------------------------------------------------
    // equipments restriction parameters related to item relations
    struct {
        BYTE m_byDupNum;
        BYTE m_byDupEquipNum;
        BYTE m_byDupEquipTypeNum;
        BYTE m_byDupEquipGroup; //__NA_001206_20081112_DUPEQUIP_GROUP
        BYTE m_byDupEquipGroupNum;
    };
    //----------------------------------------------------------------------------------------------
    // Divine 관련 수치
    struct {
        WORD m_wDivLimitStr;
        WORD m_wDivLimitDex;
        WORD m_wDivLimitVit;
        WORD m_wDivLimitInt;
        WORD m_wDivLimitSpr;
        WORD m_wDivLimitSkill1;
        WORD m_wDivLimitSkill2;
        WORD m_wDivPhyMinDamage;
        WORD m_wDivPhyMaxDamage;
        WORD m_wDivMagicMinDamage;
        WORD m_wDivMagicMaxDamage;
        WORD m_wDivPhyDef;
        WORD m_wDivMagicDef;
    };
    //----------------------------------------------------------------------------------------------
    //ver1.1 엘리트 항목 추가
    struct {
        WORD m_wElitePhyMinDamage;
        WORD m_wElitePhyMaxDamage;
        WORD m_wEliteMagicMinDamage;
        WORD m_wEliteMagicMaxDamage;
        WORD m_wElitePhyDef;
        WORD m_wEliteMagicDef;
    };
    //----------------------------------------------------------------------------------------------
    //ver1.3 : 유니크 항목 추가
    struct {
        WORD m_wUniPhyMinDamage;
        WORD m_wUniPhyMaxDamage;
        WORD m_wUniMagicMinDamage;
        WORD m_wUniMagicMaxDamage;
        WORD m_wUniPhyDef;
        WORD m_wUniMagicDef;
    };
    //----------------------------------------------------------------------------------------------
    // UI parameters
    struct {
        CODETYPE m_NCode;   //< 네임코드
        CODETYPE m_SCNCode; //< second 네임코드
        CODETYPE m_TDNCode; //< third 네임코드

        enum { MAX_COLOR_LENGTH = 7 };
        bool m_bUse; //<아이템 네이밍색깔 사용여부
        char m_szColor[MAX_COLOR_LENGTH]; //<아이템 네이밍색깔 RGB

        CODETYPE m_wWICode; // 남성캐릭터에 사용될 그래픽리소스코드
        //_NA_002050_20110216_ADD_GENDER
        CODETYPE m_wWICode2;//여성캐릭터에 사용될 그래픽리소스코드 <추가필드

        CODETYPE m_wVICode; // Icon image code
        CODETYPE m_wDICode; // Drop image code
        int m_nSoundIndex; // 아이템 사운드 인덱스

        // 클라이언트 아이템 사용시 이펙트 출력 용 
        DWORD m_dwEffectCode;   //< 사용시 효과 이펙트 코드   
        BYTE m_byEffectPosition;//< 사용시 효과 이펙트 부착 위치 
        BYTE m_bEfffectBoneRot;

        // 스페셜 아이템 이큅용 
        bool m_bEquipRot;       //< 장착시 본을 붙어 다닐것인가
        DWORD m_dwEquipBoneID;  //< 장착시 붙는 본 아이디  ver 2.3

        BYTE m_bHeadType;   //< 머리형태 (EP2:남성캐릭터)
        BYTE m_bHeadType1;   //< 머리형태, _NA_002050_20110216_ADD_GENDER
        WORD m_wCustomize;  //< 커스터마이즈 (EP2:남성캐릭터, 투구/헤어 관련 정보)
        //_NA_002050_20110216_ADD_GENDER = { m_bHeadType1, m_wCustomize1 }
        WORD m_wCustomize1;  //< 커스터마이즈 < 여성캐릭터에 사용될 투구/헤어 관련 정보

        WORD m_wMissile;
        //BYTE m_byIsDrop;  //< 몬스터 드랍 여부 사용치 않아 아이템 사용시 팝업을 보여줄껀지로 대체
        BYTE m_byPopup;     //< 아이템 사용시 팝업을 보여줄껀지로 대체
        // 클라이언트 출력용
    #if SUN_CODE_BACKUP
        WORD m_wItemListType;
    #endif
        bool m_bApplyItemDistribution; 

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        BYTE dice_cast_type;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    };

    //----------------------------------------------------------------------------------------------
#if !defined(_DEBUG)
    char m_pszName[MAX_ITEMNAME_LENGTH]; //< 디버그용 이름(사용안함)
#endif

    BASE_ITEMINFO()
    {
        ::ZeroMemory(this, sizeof(*this));
    }

    //==============================================================================================
    //==============================================================================================
    //==============================================================================================
    // 관련 메서드
    // @history
    //  - 090906 | waverix | move impl->bottom impl, add attributes 'const' and 'BOOL' -> 'BOOLEAN'
    //  - 110410~110414, waverix, rearrangement of layouts for access rate and group by meanings
    bool IsChargeItem() const;
    bool IsElite() const;
    bool IsUnique() const;
    bool IsRare() const;
    bool IsWeapon() const; //< 무기인가.
    bool IsArmor() const; //< 방어구인가.
    bool IsAccessory() const; //< 장신구인가.
    bool IsSpecialAccessory() const; //< 장신구인가.
    bool IsRing() const;
    bool IsNecklace() const;
    bool IsCanUseWaste() const; //< 사용가능한 소모품인가.
    bool IsCanOnlyUseWaste() const; // 사용 가능, 소모 불가

#ifdef  _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    bool IsDailyQeustResetItem() const;
#endif

#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    bool IsImmediatelyUseItem() const;
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE

    // 사용 가능 여부 상관없이 소모 되는지 검사한다.
    bool IsMaterialWaste() const
    {
        return (m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_USE || 
            m_byMaterialType == eITEM_MATERIAL_TYPE_NOT_USE_CAN_DELETE);
    }
    bool IsPotion() const; //< 물약인가.
    eCHAR_TYPE GetFirstAvailableClass() const;
    bool IsApplyItemDistribution() const;
    // NOTE: the real type of the returned type is BASE_ITEMINFO::eSetItemOption
    BYTE GetApplySetItemOptionType() const;

    bool IsRiderItem() const;
    bool IsRiderPartsItem() const;
    bool CanPeriodExtend() const;
#if !defined(_SERVER) // (WAVERIX) (ADD-IN) Client 코드란다... 서버는 사용하는 사례 없기 때문에 설정
    bool CanRankUpItem() const; //서버팀 사용하지 말것!
#endif

    // [11/02/2009 namka199] _CN_0_20091013_RANKUP_EXTEND 랭크업 관련 부적의돌/초기화의 돌.
    bool IsRankUpRequirement() const;
    bool IsCanUseRankUp() const;
    bool IsRankUpOptionChanger() const;
    bool IsRankUpGradeRaiser() const;
    
    // [10/30/2009 namka199] _NA_0_20091030_RANDOM_ITEM 랜덤 아이템 여부.
    bool IsRandomItem() const;

    // 상세한 아이템 계열값을 얻는다.
    ItemSubTypeDetailValue GetSubTypeDetail() const;

#ifdef _NA_002050_20110216_ADD_GENDER
    // 해당 성별이 사용 가능한지 검사한다.
    bool IsCanUseGender(const BYTE gender) const;
#endif
    // CHANGES: f110329.5L, moved from implementation
    bool IsExistEquipOptions() const;
    bool IsExistInvenOptions() const;
    // NOTE: f110422.2L, f110408.2L, added easy access facility
    bool CheckValidEquipClass(eCHAR_TYPE char_type) const;
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    // NOTE: internal utilities
    void _UpdateSubTypeDetailedValue(); // f110414.1L, call this after loading default script data
}; //end of 'BASE_ITEMINFO'
//
//
//==================================================================================================
//  Inline implementation list
//==================================================================================================
//  BASE_ITEMINFO inline functions

inline bool
BASE_ITEMINFO::IsChargeItem() const {
    return (m_ChargeType == eCHARGE_CHARGE);
}

inline bool
BASE_ITEMINFO::IsElite() const
{
    return (m_wSubType == eITEMSUBTYPE_ELITE ||
            m_wSubType == eITEMSUBTYPE_ELITE_SSQ); // (f100730.3L)
}

inline bool
BASE_ITEMINFO::IsUnique() const
{
    return (m_wSubType == eITEMSUBTYPE_UNIQUE);
}

inline bool
BASE_ITEMINFO::IsRare() const
{
    return (m_wSubType == eITEMSUBTYPE_RARE);
}

inline bool
BASE_ITEMINFO::IsWeapon() const //< 무기인가.
{   // (WAVERIX) (CHANGES) check 'code', because eEQUIPCONTAINER_WEAPON is 0
    return (m_Code != 0 && m_wEqPos == eEQUIPCONTAINER_WEAPON);
}

inline bool
BASE_ITEMINFO::IsArmor() const //< 방어구인가.
{
    return m_wEqPos == eEQUIPCONTAINER_ARMOR ||
           m_wEqPos == eEQUIPCONTAINER_PROTECTOR ||
           m_wEqPos == eEQUIPCONTAINER_HELMET ||
           m_wEqPos == eEQUIPCONTAINER_PANTS ||
           m_wEqPos == eEQUIPCONTAINER_BOOTS ||
           m_wEqPos == eEQUIPCONTAINER_GLOVE ||
           m_wEqPos == eEQUIPCONTAINER_BELT ||
           m_wEqPos == eEQUIPCONTAINER_SHIRTS;
}

inline bool
BASE_ITEMINFO::IsAccessory() const //< 장신구인가.
{
    return m_wType == eITEMTYPE_RING ||
           m_wType == eITEMTYPE_NECKLACE;
}

inline bool
BASE_ITEMINFO::IsSpecialAccessory() const //< 장신구인가.
{
    return m_wType == eITEMTYPE_BERSERKER_SACCESSORY ||
           m_wType == eITEMTYPE_DRAGON_SACCESSORY ||
           m_wType == eITEMTYPE_VALKYRIE_SACCESSORY ||
           m_wType == eITEMTYPE_SHADOW_SACCESSORY ||
           m_wType == eITEMTYPE_ELEMENTALIST_SACCESSORY ||
           m_wType == eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1 || // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
           m_wType == eITEMTYPE_BERSERKER_KALBUS_ACCESSORY ||
           m_wType == eITEMTYPE_DRAGON__KALBUS_ACCESSORY ||
           m_wType == eITEMTYPE_VALKYRIE__KALBU_ACCESSORY ||
           m_wType == eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY ||
           m_wType == eITEMTYPE_SHADOW_KALBU_ACCESSORY || 
           m_wType == eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY2 || // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
           m_wType == eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1 || //_NA_000000_20130812_NEW_CHARACTER_HELLROID
           m_wType == eITEMTYPE_HELLROID_SPECIAL_ACCESSORY2 || //_NA_000000_20130812_NEW_CHARACTER_HELLROID
           m_wType == eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1 || //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
           m_wType == eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY2 || //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

           m_wType == eITEMTYPE_ALLCLASS_CHAOSEYE ||              //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY || //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_BERSERKER_KALVUS_ACCESSORY ||    //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_DRAGON_KALVUS_ACCESSORY ||       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY || //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_MYSTIC_KALVUS_ACCESSORY ||       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY || //_NA_007827_20141117_ADD_ACCESSORY_TYPE
           m_wType == eITEMTYPE_WINGS; 
}

inline bool
BASE_ITEMINFO::IsRing() const {
    return (m_wType == eITEMTYPE_RING);
}

inline bool
BASE_ITEMINFO::IsNecklace() const {
    return (m_wType == eITEMTYPE_NECKLACE);
}

inline bool
BASE_ITEMINFO::IsCanUseWaste() const { //< 사용가능한 소모품인가.
    return m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_USE || 
           m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_BOTH;
}

inline bool
BASE_ITEMINFO::IsCanOnlyUseWaste() const {
    return m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_USE_NOT_DELETE;
}

inline bool
BASE_ITEMINFO::IsPotion() const //< 물약인가.
{
    return m_byWasteType == eITEMWASTE_HPPOTION ||
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
           m_byWasteType == eITEMWASTE_HPMP_POTION ||
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
           m_byWasteType == eITEMWASTE_MPPOTION;
}

inline bool
BASE_ITEMINFO::IsApplyItemDistribution() const
{
    return m_bApplyItemDistribution != false;
}

// NOTE: the real type of the returned type is BASE_ITEMINFO::eSetItemOption
inline BYTE
BASE_ITEMINFO::GetApplySetItemOptionType() const
{
    return m_bySetOptionType;
}

inline bool
BASE_ITEMINFO::IsRiderItem() const
{
    return m_wType == eITEMTYPE_RIDER;
}

inline bool
BASE_ITEMINFO::IsRiderPartsItem() const
{
    return m_wType == eITEMTYPE_RIDER_PARTS;
}

inline bool
BASE_ITEMINFO::CanPeriodExtend() const
{
    if(m_ChargeSubType != eCHARGE_SUB_FIXED_AMOUNT_WAIT &&
       m_ChargeSubType != eCHARGE_SUB_FATE &&
       m_ChargeSubType != eCHARGE_SUB_RIDER_WITH_PART)
    {
        return false;
    }

    return true;
}

// [11/2/2009 namka199] _CN_0_20091013_RANKUP_EXTEND
inline bool 
BASE_ITEMINFO::IsRankUpRequirement() const
{
    return (m_wType == eITEMTYPE_RANKUP_GRADE_RAISER || 
            m_wType == eITEMTYPE_RANKUP_OPTION_CHANGER);
}

// [11/2/2009 namka199] _CN_0_20091013_RANKUP_EXTEND
inline bool 
BASE_ITEMINFO::IsCanUseRankUp() const
{
    return (m_wType == eITEMTYPE_RANKUP_GRADE_RAISER || 
            m_wType == eITEMTYPE_RANKUP_OPTION_CHANGER);
}

// [11/2/2009 namka199] _CN_0_20091013_RANKUP_EXTEND
inline bool
BASE_ITEMINFO::IsRankUpOptionChanger() const
{
    return (m_wType == eITEMTYPE_RANKUP_OPTION_CHANGER);
}

// [11/2/2009 namka199] _CN_0_20091013_RANKUP_EXTEND
inline bool
BASE_ITEMINFO::IsRankUpGradeRaiser() const
{
    return (m_wType == eITEMTYPE_RANKUP_GRADE_RAISER);
}

// [10/30/2009 namka199] _NA_0_20091030_RANDOM_ITEM
inline bool
BASE_ITEMINFO::IsRandomItem() const
{
    return (m_wType == eITEMTYPE_RANDOM);
}

// NOTE: f110414.x, declared by hukim, 아이템 계열 상세 타입
inline ItemSubTypeDetailValue BASE_ITEMINFO::GetSubTypeDetail() const
{
    return static_cast<ItemSubTypeDetailValue>(sub_type_detailed.value);
}

#ifdef _NA_002050_20110216_ADD_GENDER
inline bool BASE_ITEMINFO::IsCanUseGender(const BYTE gender) const
{
    if (gender_type == GenderType::kAll || gender_type == gender)
    {
        return true;
    }
    return false;
}
#endif // _NA_002050_20110216_ADD_GENDER

//==================================================================================================
// CHANGES: f110329.5L, moved from implementation
inline bool BASE_ITEMINFO::IsExistEquipOptions() const
{
    BOOST_STATIC_ASSERT(0 < BASE_ITEMINFO::_MAX_OPTION_NUM &&
                            BASE_ITEMINFO::_MAX_OPTION_NUM <= sizeof(DWORD64));
    const bool is_equip_effect_item = \
        ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_EQUIP_MASK::MASK, m_byOptionExEffect) ||
        ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_EQUIP_MASK::MASK, m_byPCOptionExEffect);
    return is_equip_effect_item;
}

inline bool BASE_ITEMINFO::IsExistInvenOptions() const
{
    const bool is_inven_effect_item = \
        ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, m_byOptionExEffect) ||
        ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, m_byPCOptionExEffect);
    return is_inven_effect_item;
}

// NOTE: f110422.2L, f110408.2L, added easy access facility
inline bool BASE_ITEMINFO::CheckValidEquipClass(eCHAR_TYPE char_type) const
{
    if ((1 << char_type) & equip_class_bits) {
        return true;
    };
    return false;
};

//==================================================================================================

#pragma pack(pop)

#endif //_SOLAR_SLOT_SCSLOTSTRUCT_H
