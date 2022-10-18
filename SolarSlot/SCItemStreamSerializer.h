#ifndef _SOLAR_SLOT_SCITEMSTREAMSERIALIZER_H
#define _SOLAR_SLOT_SCITEMSTREAMSERIALIZER_H

#include "./ItemOptionHeader.h"
#include "./SCSlotHeader.h"
#include "./SCSlotStruct_Version.h"

//==================================================================================================
// @history:
//  - 100523|waverix|move a stream parts section from SCSlotStruct.h
#pragma pack(push)
#pragma pack()

class SCItemSlot;
struct BASE_ITEMINFO;

//==================================================================================================
// begin of stream parts
//
#pragma pack(push, 1)

//--------------------------------------------------------------------------------------------------
struct RENDER_ITEMPART
{
    //  DWORD dwSerial; //< 패킷용
    WORD wCode;
    BYTE byDura;
    inline bool IsValid() const { return (wCode != 0); }
};

//--------------------------------------------------------------------------------------------------
struct OPT_ITEMPART
{
    WORD wCode;
    inline bool IsValid() const { return (wCode != 0); }
};

//--------------------------------------------------------------------------------------------------
struct RENDER_OPTIONPART
{
    BYTE Enchant         :6;
    BYTE Set             :2;
    BYTE bEquipDevice    :1;    //__NA_000968_ETHER_EMISSION_DEVICE
    BYTE byBulletIndex   :4;
    BYTE Awakening       :6;
};

//==================================================================================================
//
// 아이템파츠의 집합 스트림
//--------------------------------------------------------------------------------------------------
/* (CHANGES) (f100520.1L) remove an unused section
// 기본 아이템 정보
// Buy, Pick, Obtain, 파티에서 분배
typedef union {
    ITEMPART Part;
    BYTE pStream[sizeof(ITEMPART)];
} ITEMSTREAM ;

//패킷용 아이템 스트림 구조체
typedef union {
    RENDER_ITEMPART Part;
    BYTE pStream[sizeof(RENDER_ITEMPART)];
} RENDER_ITEMSTREAM ;
*/
typedef union {
    struct {
        RENDER_ITEMPART Part;
        OPTIONPART OptionPart;
    };
    BYTE pStream[sizeof(RENDER_ITEMPART) + sizeof(OPTIONPART)];
} RENDER_ITEMSTREAMEX ;


//최적화 기본정보 패킷용 구조체
typedef union
{
    struct {
        OPT_ITEMPART Part;
        RENDER_OPTIONPART OptionPart;
    };
    BYTE pStream[sizeof(OPT_ITEMPART) + sizeof(RENDER_OPTIONPART)];
} OPT_ITEMSTREAMEX;


//--------------------------------------------------------------------------------------------------
// 스킬파츠의 최소 단위
// 스킬 정보 표시 기본 단위
//--------------------------------------------------------------------------------------------------
struct SKILLPART
{
    WORD wCode;
    inline bool IsValid() const { return (wCode != 0); }
};

//--------------------------------------------------------------------------------------------------
// 스킬파츠의 집합 스트림
//--------------------------------------------------------------------------------------------------
typedef union {
    SKILLPART Part;
    BYTE pStream[sizeof(SKILLPART)];
} SKILLSTREAM ;

//--------------------------------------------------------------------------------------------------
// 퀵파츠의 최소 단위
// 퀵 정보 표시 기본 단위
//--------------------------------------------------------------------------------------------------
struct QUICKPART
{
    SLOTIDX SlotIdx;
    SLOTCODE Code;
    BYTE Pos;
    inline bool IsValid() const { return (SlotIdx != 0); }
};

//--------------------------------------------------------------------------------------------------
// 퀵파츠의 집합 스트림
//--------------------------------------------------------------------------------------------------
typedef union {
    QUICKPART Part;
    BYTE pStream[sizeof(QUICKPART)];
} QUICKSTREAM ;

//--------------------------------------------------------------------------------------------------
// 스타일파츠의 최소 단위
// 스타일 정보 표시 기본 단위
//--------------------------------------------------------------------------------------------------
struct STYLEPART
{
    SLOTCODE Code;
    inline bool IsValid() const { return (Code != 0); }
};

//--------------------------------------------------------------------------------------------------
// 스타일파츠의 집합 스트림
//--------------------------------------------------------------------------------------------------
typedef union {
    STYLEPART Part;
    BYTE pStream[sizeof(STYLEPART)];
} STYLESTREAM ;

#pragma pack(pop)

//--------------------------------------------------------------------------------------------------
// (NOTE) declared by _NA002217_100728_EP2_PERK_SYSTEM_
// current state : pack=8
struct PerkID
{
    PerkID() : uid_(0) {}
    PerkID(ulong uid) : uid_(uid) {}
    PerkID(SLOTCODE group, LEVELTYPE level)
        : group_(group), level_(level)
    {}
    union {
        ulong uid_;
        struct {
            SLOTCODE group_; // perk group
            LEVELTYPE level_;  // perk level
        };
    };
};

struct PerkCommand
{
    enum eCmd {
        eCmd_None = 0,
        eCmd_Insert,
        eCmd_Delete,
        eCmd_Register, // change to active
        eCmd_Unregister, // change to inactive
        //
        eCmd_Counts
    };
    enum eResult {
        eResult_Success = 0,
        eResult_Fail, // default fail
        eResult_CantInsert,
        eResult_CantDelete,
        //
        eResult_Counts
    };
    typedef uint8_t cmd_t;
    typedef uint16_t result_t;
    //
    cmd_t cmd_;
    result_t result_value_;
    //
    void InitCommand(cmd_t cmd) {
        ZeroMemory(this, sizeof(*this));
        cmd_ = cmd;
    };
};

// current state : pack=8
// loaded by PerkInfoParser
struct PerkInfo
{
    static const uint16_t kMaxPerkLevel = 5;
    static const int kNumberOfBenefitOptions = 4;
    static const int kNumberOfPenaltyOptions = 2;
    struct OptionNode {
        uint16_t option_;     // ref: eATTR
        uint8_t  item_option_;// ref: item option in script, 'option_ = g_ITEM_OPTION[item_option_]'
        uint8_t  value_type_; // ref: eVALUE_TYPE
        uint8_t  attr_kind_;  // ref: eATTR_KIND
        // (WARNING) this is a double converted value
        // based on the converted result of for a skill process of a result of a item caculator
        // after loading the perk script data
        int32_t  value_;
    };
    // (NOTE) the unique identification of a perk is presented by composition of a group and a level
    PerkID perk_id_;
    uint8_t order_; // sorted value based on a group code, runtime calculating data = { 0, ... }
    bool max_level_; // whether max level perk in same group
    SLOTCODE item_code_; // whether this perk is a base perk or an registerable perk using an item
    DWORD name_code_; // the perk name
    DWORD icon_code_;
    DWORD option_description_code_;
    DWORD penalty_description_code_;
    const PerkInfo* next_perk_;
    const PerkInfo* prev_perk_;
    EXPTYPE exp_value_; // a player needs exp_value_ to acquire this perk by level
    EXPTYPE next_level_exp_; // support value, a perk can level up on reached this value
    //
    OptionNode benefit_option_[kNumberOfBenefitOptions];
    OptionNode penalty_2x_option_[kNumberOfPenaltyOptions];
    OptionNode penalty_3x_option_[kNumberOfPenaltyOptions];
    OptionNode penalty_4x_option_[kNumberOfPenaltyOptions];
};
//
struct PerkData
{
    PerkID perk_id_;
    SLOTCODE link_code_;
    // NOTE: a normal perk differs from a cash perk in below repect
    // a normal perk use the experience but a cash perk use the expiration date.
    DWORD64 exp_;
};
//
//--------------------------------------------------------------------------------------------------
// (NOTE) this data container is a data presentation for the perk slot container.
// the perk slot container have 'active link slot' and 'registered slot'
// the active link slot has unique group code
// that has link point related to a registered slot using 'slot_index_' field.
// the registered slot has an information to use when a player change to active
// (specification)
//  Active Link Slot
//     GroupCode = { 65001, 65002, 65003, 65004 } ; // Unique Active Slot (1~4)
//               = { 65010, } ; // Unique Active Slot for cash event(1)
//     SlotIndex = slot index link for active slots of register slots
//
struct PerkDataList
{
    static const int kNumberOfDefaultSlots = 15;
    static const int kNumberOfCashSlots = 3;
    static const int kNumberOfActiveSlots = 5;
    static const int kMaxNumberOfFixedSlots = 40; // reference : SCPerkSlotContainer.h
    static const int kMaxNumberOfNodes = kNumberOfDefaultSlots +
                                         kNumberOfCashSlots +
                                         kNumberOfActiveSlots;
    enum eGroupCodeActiveSlot {
        eActiveGroup01 = 65001,
        eActiveGroup02 = 65002,
        eActiveGroup03 = 65003,
        eActiveGroup04 = 65004,
        eActiveGroup10 = 65010, // cash slot linker
    };
    //
    int number_of_nodes_;
    int reference_;
    PerkData perk_data_[kMaxNumberOfNodes];
    //
    static const eGroupCodeActiveSlot kGroupCodeActiveSlots[kNumberOfActiveSlots];
    static const POSTYPE kDefaultSlotPositions[kNumberOfDefaultSlots];
    static const POSTYPE kCashSlotPositions[kNumberOfCashSlots];
    static const POSTYPE kActiveSlotPositions[kNumberOfActiveSlots];
    //
    static SLOTCODE GetActiveGroupCodeByIndex(int index);
    static int GetIndexByActiveGroupCode(int active_group_code);
    static bool CanRegisterToActiveSlot(const PerkInfo* perk_info, int active_group_code_index);
    //
    uint16_t GetSize() const {
        const int kMinSize = sizeof(*this) - sizeof(perk_data_);
        return static_cast<uint16_t>(kMinSize + (sizeof(perk_data_[0]) * number_of_nodes_));
    }
};

inline SLOTCODE PerkDataList::GetActiveGroupCodeByIndex(int index)
{
    if (!FlowControl::FCAssert(index >= 0 && index < _countof(kGroupCodeActiveSlots))) {
        return 0;
    }
    return static_cast<SLOTCODE>(kGroupCodeActiveSlots[index]);
}

inline int PerkDataList::GetIndexByActiveGroupCode(int active_group_code)
{
    const PerkDataList::eGroupCodeActiveSlot* it = kGroupCodeActiveSlots,
        * end = &kGroupCodeActiveSlots[_countof(kGroupCodeActiveSlots)];
    for ( ; it != end; ++it) {
        if (*it == active_group_code) {
            return static_cast<int>(it - kGroupCodeActiveSlots);
        }
    }
    assert(!"index out of range");
    return _countof(kGroupCodeActiveSlots);
}

inline bool PerkDataList::CanRegisterToActiveSlot(const PerkInfo* perk_info,
                                                  int active_group_code_index)
{
    if (active_group_code_index < 0 ||
        active_group_code_index >= _countof(kGroupCodeActiveSlots))
    {
        return false;
    }
    const int kCashActiveSlotIndex = (_countof(kGroupCodeActiveSlots) - 1);
    const bool cash_perk = (perk_info->item_code_ != 0);
    if (cash_perk) {
        return active_group_code_index == kCashActiveSlotIndex;
    }
    // non-cash
    return active_group_code_index < kCashActiveSlotIndex;
}

//
// end of stream parts
//==================================================================================================
struct SlotDeleter
{
    template<class T>
    void operator()(T *& p) const
    {
        delete p;
        p = NULL;
    }
};

//==================================================================================================

namespace nsSlot {
;

//--------------------------------------------------------------------------------------------------
// (CHANGES) (f100802.4L) moved from code file to support type version control
struct Binder;

// NOTE, f100826.3L, add an interface which is needed between two binders
// when an version upgrade is processing
struct IBinderPerVersion {
    virtual bool ModifySerializeBlock(const Binder* binder,
                                      const BASE_ITEMINFO* item_info) const = 0;
};
//
//--------------------------------------------------------------------------------------------------
//                          ulong    used_bits
#define SLOT_BITBOUND_CHECK(_from_, _bit_count_)    \
    ( ((_from_) & util::bit_range_mask_<(_bit_count_)>::value) == (_from_) )

#define SLOT_BITBOUND_CHECK_ASSERT(_from_, _bit_count_)    \
    { assert(SLOT_BITBOUND_CHECK(_from_, _bit_count_)); }

//                        ulong(i/o)  used_bits
#define SLOT_BITBOUND_SET(_from_to_, _bit_count_) {   \
    SLOT_BITBOUND_CHECK_ASSERT(_from_to_, _bit_count_); \
    (_from_to_) &= util::bit_range_mask_<(_bit_count_)>::value; }


//                              byte    used_bits
#define SLOT_BITBOUND_CHECK_1B(_from_, _bit_count_)    \
    ( ((_from_) & (BYTE)util::bit_range_mask_<(_bit_count_)>::value) == (_from_) )

#define SLOT_BITBOUND_CHECK_ASSERT_1B(_from_, _bit_count_)    \
    { assert(SLOT_BITBOUND_CHECK_1B(_from_, _bit_count_)); }

//                           byte(i/o)  used_bits
#define SLOT_BITBOUND_SET_1B(_from_to_, _bit_count_) {   \
    SLOT_BITBOUND_CHECK_ASSERT_1B(_from_to_, _bit_count_); \
    (_from_to_) &= (BYTE)util::bit_range_mask_<(_bit_count_)>::value; }

#define SP_CONST    const

//==================================================================================================
//
struct SerializeItemOption
{
    //----------------------------------------------------------------------------------------------
    // Shared Block & normal item (no equipment)
    struct sDefault {
        ulong ParseType;            //8

        // eParse_CantParse(Old)일 경우, Block에서부터 연속 CopyStream으로 처리
        ulong Block;                //1
        ulong UserBlock;            //1
        // the item purchase history to support distinction of a C cash or a PP cash (use 1bit)
        ulong Tag;                  //2 - renaming of a 'History'
    };
    //----------------------------------------------------------------------------------------------
    struct sTime : sDefault {
        ulong IsExpired;            //1 시간 만료 여부
        ulong IsProgressTime;       //1
        ulong IsExpiredWaitTime;    //1
        UINT64 DateTime;            //45
    };
    //----------------------------------------------------------------------------------------------

    // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    struct sExtendData : sDefault {
        ulong data;                 //16    
    };

    struct sBackupStream {
        ulong ParseType;
        BYTE stream[sizeof(OPTIONPART) - 1]; // Version5 | 6 - can't convert | can't find item_code
    };
    //----------------------------------------------------------------------------------------------
    struct RandomSetting
    {
        enum { kMaxOptionSize = 5 };
        enum { kOptionBitSize = 16 };
    };
    //----------------------------------------------------------------------------------------------
    struct sEquip : sDefault {
        ulong IsIdentifyPossession; //1
        ulong IsDestroyed;          //1
        ulong NOption;              //3
        ulong Rank;                 //4
        ulong RankOption[RANK_MAX]; //7 x 10
        ulong EnchantGrade;         //4
        ulong IsDivine;             //1 old 'Set'
        //
        ulong SocketIdentifyType;   //2 - _NA_0_20100727_SOCKET_SYSTEM_CONVERT
        ulong NumberOfSockets;      //3
        ulong SocketOption[SOCKET_MAX]; //10 x 5 // 
        //
        ulong IsEquipEtherDevice;   //1
        ulong EtherBulletIndex;     //4
        //
        // the specification to support item customizing contents
        ulong History;              //2
        ulong ImageCode;            //16 _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        // _NA_003740_20111122_RANDOM_ITEM
        ulong RandomOptions[RandomSetting::kMaxOptionSize];
        ulong AwakeningGrade;       //4 // _NA_008124_20150313_AWAKENING_SYSTEM
    };
    //----------------------------------------------------------------------------------------------
    struct sEquipEtc : sDefault {
        ulong IsIdentifyPossession; //1
        ulong IsDestroyed;          //1
        ulong NOption;              //3
        ulong EnchantGrade;         //4 - restored by _NA002823_WINGS_EQUIPMENTS_EXTENTION
        ulong IsDivine;             //1
        ulong Rank;                 //4
        ulong RankOption[RANK_MAX]; //7 x 10
        // _NA_003740_20111122_RANDOM_ITEM
        ulong RandomOptions[RandomSetting::kMaxOptionSize];
    };
    //----------------------------------------------------------------------------------------------
    struct sEquipTime : sDefault {
        ulong IsIdentifyPossession; //1
        ulong IsDestroyed;          //1
        ulong NOption;              //3
        ulong Rank;                 //4
        ulong RankOption[RANK_MAX]; //7 x 10
        ulong EnchantGrade;         //4
        //
        ulong IsDivine;             //1 old 'Set'
        //
        ulong SocketIdentifyType;   //2 - _NA_0_20100727_SOCKET_SYSTEM_CONVERT
        //
        ulong NumberOfSockets;      //3
        ulong SocketOption[SOCKET_MAX]; //10 x 5
        //
        ulong IsExpired;            //1 시간 만료 여부
        ulong IsProgressTime;       //1
        ulong IsExpiredWaitTime;    //1
        UINT64 DateTime;            //45
        //
        ulong IsEquipEtherDevice;   //1
        ulong EtherBulletIndex;     //4
        // _NA_003740_20111122_RANDOM_ITEM
        ulong RandomOptions[RandomSetting::kMaxOptionSize];
        ulong AwakeningGrade;       //4 // _NA_008124_20150313_AWAKENING_SYSTEM
    };
    //----------------------------------------------------------------------------------------------
    struct sEquipEtcTime : sDefault {
        ulong IsIdentifyPossession; //1
        ulong IsDestroyed;          //1
        ulong NOption;              //3
        ulong EnchantGrade;         //4 - restored by _NA002823_WINGS_EQUIPMENTS_EXTENTION
        ulong IsDivine;             //1
        ulong Rank;                 //4
        ulong RankOption[RANK_MAX]; //7 x 10
        //
        ulong IsExpired;            //1 시간 만료 여부
        ulong IsProgressTime;       //1
        ulong IsExpiredWaitTime;    //1
        UINT64 DateTime;            //45
        // _NA_003740_20111122_RANDOM_ITEM
        ulong RandomOptions[RandomSetting::kMaxOptionSize];
    };
    //----------------------------------------------------------------------------------------------
    struct sLimited : sDefault {
        // Limited (no-divine(?) + armors)
        ulong IsIdentifyPossession; //1
        ulong IsDestroyed;          //1
        ulong NOption;              //3
        ulong EnchantGrade;         //4
        //
        ulong IsDivine;             //1 old 'Set'
        //
        ulong SocketIdentifyType;   //2 - _NA_0_20100727_SOCKET_SYSTEM_CONVERT
        //
        ulong NumberOfSockets;      //3
        ulong SocketOption[SOCKET_MAX];//10 x 5
        //
        ulong Index;                //16
        ulong DurationForRestore;   //9 최고 내구도(회복용)
        ulong Duration;             //9 최고 내구도
        ulong NumberOfRepairs;      //4 수리 회수
        ulong DefenceRatio;          //9 공격력 비율(방어구는 방어력) 
        ulong LimitValueRatio;      //9 제한 수치 비율

        /*
        // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
        struct sOption {
            ulong Option;           //7
            ulong Value;            //6
        } LimitedOption[LIMITED_MAX];
        */
        // the specification to support item customizing contents
        ulong History;              //2
        ulong ImageCode;            //16 _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        // _NA_003740_20111122_RANDOM_ITEM
        ulong RandomOptions[RandomSetting::kMaxOptionSize];
    };
    //----------------------------------------------------------------------------------------------
    struct sFate : sDefault {
        // Fate (Lucky item)
        ulong IsIdentifyPossession; //1
        ulong IsDestroyed;          //1
        ulong NOption;              //3
        ulong EnchantGrade;         //4
        //
        ulong SocketIdentifyType;   //2 - _NA_0_20100727_SOCKET_SYSTEM_CONVERT
        //
        ulong NumberOfSockets;      //3
        ulong SocketOption[SOCKET_MAX]; //10 x 5

        ulong Index;                //16
        ulong AtkDefRatio;          //9
        ulong Duration;             //9 최대 내구도
        ulong IsIdentify;           //1

        ulong IsEquipEtherDevice;   //1 -- f101105.3L, unused
        ulong EtherBulletIndex;     //4 -- f101105.3L, unused

        UINT64 DateTime;            //45
        ulong IsExpired;            //1 시간 만료 여부 (not used)
        ulong IsExpiredWaitTime;    //1
        ulong Period;               //14
        // the specification to support item customizing contents
        ulong History;              //2
        ulong ImageCode;            //16 _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        // _NA_003740_20111122_RANDOM_ITEM
        ulong RandomOptions[RandomSetting::kMaxOptionSize];
    };
    //----------------------------------------------------------------------------------------------
    struct sLottery : sDefault {
        // Lottery
        ulong Index;                //16 LotteryScript Index
        ulong Identity;             //8 Identity State
        ulong Count;                //8 Item Count
        struct sSlot {
            ulong Random;           //1 Is Random?
            ulong Exist;            //1 Is Exist?
            ulong Position;         // if(Exist) Part Index [1, 8]
        } Slots[MAX_PANDORA_SLOT_NUM];
    };
    //----------------------------------------------------------------------------------------------
    struct sCoord : sDefault {
        ulong MapCode;              //16 좌표
        ulong X;                    //16
        ulong Y;                    //16
        ulong Z;                    //16
    };
    //----------------------------------------------------------------------------------------------
    struct sWayPoint : sDefault {
        // WayPoint // 좌표리스트 링크된 아이템 기능들의 활성, 비활성 근거를 가진다.
        ulong IsExpired;            //1 시간 만료 여부 : 아이템 자체 (lucky item처럼 사용 불가 상태?)
        ulong IsProgress;           //1
        UINT64 DateTime;            //45
        ulong WayPointDeleteEnabled;//1  웨이포인트 목록에서 특정 항목 삭제 뒤 블록 상태 여부
        UINT64 WayPointDeleteDateTime;//45 웨이포인트 목록에서 특정 항목 삭제 뒤 다음 삭제 가능까지 대기시간
    };
    //----------------------------------------------------------------------------------------------
    struct sPet : sDefault {
        ulong Fullness;             //8 포만감
        ulong Intimacy;             //8 친밀도
        ulong IsExpired;            //1
        UINT64 DateTime;            //45
        ulong IsSetPetName;         //4 펫네임 설정 (Ver5에서 Enchant영역 사용)
        char Name[MAX_PET_NAME_LENGTH]; //8 x 10
        // the specification to support item customizing contents
        ulong History;              //2
        ulong ImageCode;            //16 _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    };
    //----------------------------------------------------------------------------------------------
    struct sRider : sDefault {
        //ulong IsIdentifyPossession; //1
        //ulong IsDestroyed;          //1
        ulong IsUnlimitedPeriod;    //1  기한 설정 없음.
        ulong IsExpiredWaitTime;    //1  기한 만료 후 대기 상태
        UINT64 DateTime;            //45 
        struct sPart { // migration from sRiderParts
            ulong ItemCode;         //16 라이더 파츠 아이템 코드
            ulong EnchantGrade;     //3  (<- 4), (CHANGES) (f100803.2L)
            ulong Tag;              //1
            ulong IsUnlimitedPeriod;//1  기한 설정 없음.
            ulong IsProgress;       //1  장착 시점부터 시간 계산
            ulong IsExpiredWaitTime;//1  기한 만료 후 대기 상태
            ulong DateOffset;       //16 + 1 (기간 연장 고려 +/- 30일)(분 단위:60x12x30 x sign)
        } Parts[eRIGER_PARTS_SLOT_IDX_COUNT];
        //
        // DateOffset Description - used(16) 최장 (+/-)45일 까지는 가능하지만 undocumented option으로....
        // (0/1) = +/-
        //      1010 1000 1100 0000 = 30일

        // (090916) (CURRENT)
        // Rider      : used( 49) = 49
        // RiderParts : used(120) = 40 * 3
        // Total      : used(169) = Rider + RiderParts
    };
    //----------------------------------------------------------------------------------------------
    // (NOTE) Enchant는 이펙트 툴에서 설정하는 단계 = {0, 1, 2, 3, 4}로 설정된다. 하지만 혼란 방지 목적
    // 으로 입출력은 다른 것과 같은 [0, 12] + 1(일본 최강 이펙트)와 같이 설정한다.
    struct sRiderParts : sDefault { //
        ulong EnchantGrade;         //4  왁스에 의한 인챈츠, 해당 값의 재 설정 필요하다.
        ulong IsUnlimitedPeriod;    //1  기한 설정 없음.
        ulong IsProgress;           //1  장착 여부
        ulong IsExpiredWaitTime;    //1  기한 만료 후 대기 상태
        UINT64 DateTime;            //45 독립 아이템으로 존재시에는 일반적인 SolarDateTime
        ulong Period;               //14
    };
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    union {
        //------------------------------------------------------------------------------------------
        sDefault Default;
        sTime Time;
        sExtendData ExtendData; //_NA_0_20100727_SOCKET_SYSTEM_CONVERT
        sEquip Equip;
        sEquipEtc EquipEtc;
        sEquipTime EquipTime;
        sEquipEtcTime EquipEtcTime;

        sCoord Coord;
        sLimited Limited;
        sFate Fate;
        sLottery Lottery;
        sWayPoint WayPoint;
        sPet Pet;
        sRider Rider;
        sRiderParts RiderParts;

        sBackupStream BackupStream;
    }; //end of union

    SerializeItemOption() {}
    SerializeItemOption(const SerializeItemOption& r) {
        CopyMemory(this, &r, sizeof(*this));
    }
    SerializeItemOption& operator=(const SerializeItemOption& r) {
        CopyMemory(this, &r, sizeof(*this));
        return *this;
    }

    void Clear() {
        ZeroMemory(this, sizeof(*this));
    }
    // (CHANGES) (f100802.4L) add an interface to acquire multiple list based on type and version
    static bool GetBinder(
        const ItemParseType::ValueType parse_type,
        const int version,
        const Binder** result_binder, 
        const IBinderPerVersion** modifier
    );
};

//==================================================================================================
// 해당 슬롯에 적재된 아이템 정보에 따른 기능 제한.
struct ItemFuncAttr 
{
    enum Function
    {
        eEnable_None        = 0,
        eEnable_Divine      = 1 <<  1,
        eEnable_NOption     = 1 <<  2,
        eEnable_Rank        = 1 <<  3,
        eEnable_Enchant     = 1 <<  4,
        eEnable_Socket      = 1 <<  5,
        eEnable_Possession  = 1 <<  6, //장착 계열
        eEnable_Timer       = 1 <<  7, //타이머 설정 가능 아이템
        eEnable_Coord       = 1 <<  8,
        eEnable_Etherealize = 1 <<  9, // the means of Konglish. can extract option to the etheria item
        eEnable_AddData     = 1 << 10, // ex) Zard item with identification information
        eEnable_Randomize   = 1 << 11, // 능력치 랜덤화 가능
        eEnable_Awakening   = 1 << 12, // 각성 가능 // _NA_008124_20150313_AWAKENING_SYSTEM
    };
    enum ItemFunctionSet
    {
        kNoneItem = 0,
        kDefaultItem = eEnable_None,
        kExtendDataItem = eEnable_AddData,
        kEquipItem = eEnable_NOption|eEnable_Enchant|eEnable_Rank|eEnable_Socket|eEnable_Divine|
                     eEnable_Randomize
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
                     | eEnable_Awakening
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
                     ,
        kEquipEtcsItem = eEnable_NOption|eEnable_Rank|eEnable_Randomize,
        kFateItem = eEnable_NOption|eEnable_Enchant|eEnable_Socket|eEnable_Timer|eEnable_Randomize,
        kLimitedItem = eEnable_NOption|eEnable_Enchant|eEnable_Socket|eEnable_Divine|eEnable_Randomize,
        kLotteryItem = eEnable_None,
        kPetItem = eEnable_None, // apply timer by ItemInfoParser rule
        kCordItem = eEnable_Coord,
        kWaypointItem = eEnable_Timer,
        kRiderItem = eEnable_None,
        kRiderPartsItem = eEnable_Enchant,
    };
    ItemFuncAttr() : value(0)
    {
    }
    bool IsEnable(const Function func) const
    {
        return !!(value & func);
    }
    uint16_t value;
};

//==================================================================================================
//  자주 사용되는 아이템 분류 데이터
//  @history
//  - 09.09.23|waverix|created and preparation for the item stream version 7|
//  - 09.10.03|waverix|updates all sections to support the item stream version 7|
//  - 10.05.06|waverix|add an edit rule to support the etheria item control|
//
//  (Item Rule)
//
//  eParse_CantParse - 스크립트에 의한 것도 있고, 정보 보존 차원에서라도 유지할 필요 있다.
//
//  eParse_Limited
//      { NOption(O), Divine(O), Rankup(X), Enchant(O), Socket(O), Possession(O), DateTime(X) }
//  eParse_Pet
//      { Pet(O), PetName(O) }
//  eParse_Lottery
//      { MiniSlot(O) }
//
//  #else  base on ChargeSubType
//      ChargeSubType(eCHARGE_SUB_GENERAL)
//          eCHARGE_SUB_GENERAL
//          eCHARGE_PC_ROOM                                            PC방 아이템은 필요 없다.
//          { NOption(O), Divine(O), Rankup(O), Enchant(O), Socket(O), Possession(O), DateTime(X) }
//              eParse_EquipWeapon
//              eParse_EquipArmor
//              eParse_EquipEtcs
//
//          eCHARGE_SUB_FIXED_AMOUNT:
//          eCHARGE_SUB_FIXED_AMOUNT_EX:
//          eCHARGE_SUB_FIXED_QUANTITY:
//          eCHARGE_SUB_FIXED_AMOUNT_WAIT:
//          { NOption(O), Divine(O), Rankup(X), Enchant(O), Socket(O), DateTime(O) }
//
//          eCHARGE_SUB_COORDINATE
//          { Coordinate(O) }
//
//          eCHARGE_SUB_FATE
//          { NOption(X), Divine(X), Rankup(X), Enchant(O), Socket(O), DateTime(O) }
//
//          eCHARGE_SUB_COORDINATE_EX
//          { DateTime(O) }
struct ItemRuleInfo
{
    ItemParseType::ValueType parse_type;
    uint8 charge_sub_type;
    ItemFuncAttr func_attr; // 해당 슬롯에 적재된 아이템 정보에 따른 기능 제한.
    
    struct
    {
        bool is_valid_item;
        bool is_can_overlap;
        bool is_use_shared;
        bool is_editable;
        bool is_etheria;
        bool is_shell;
        bool is_normal;
        bool is_limited;
        bool is_fate;
        bool is_armor;
        bool is_weapon;
        bool is_accessory;
        bool is_accessory_special;
        bool is_accessory_wings;
        bool is_lottery;
        bool is_pet;
        bool is_waypoint;
        bool is_coord;
        bool is_rider;
        bool is_rider_parts;
        bool is_extend_data;
        bool is_costume;
        //_NA_008610_20160104_ADD_DECORATECOSTUME
        bool is_deco_costume;
        bool is_toggle_item;
    };
    
    ItemRuleInfo() : 
        parse_type(0), 
        charge_sub_type(0),     
        is_valid_item(false), 
        is_can_overlap(false), 
        is_use_shared(false), 
        is_editable(false), 
        is_etheria(false), 
        is_shell(false), 
        is_normal(false), 
        is_limited(false), 
        is_fate(false), 
        is_armor(false), 
        is_weapon(false), 
        is_accessory(false), 
        is_accessory_special(false), 
        is_accessory_wings(false), 
        is_lottery(false), 
        is_pet(false), 
        is_waypoint(false), 
        is_coord(false), 
        is_rider(false), 
        is_rider_parts(false), 
        is_extend_data(false),
        is_costume(false),
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        is_deco_costume(false),
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
        is_toggle_item(false)
    {
    }

    // true:  pass constraints
    // false: violation item constraints
    static bool GetRuleInfo(ItemRuleInfo* OUT result, const BASE_ITEMINFO* base_item_info,
                            bool is_limited_set); // limited 설정은 외부 제어다.
    // true:  pass constraints
    // false: violation item constraints
    // limited 설정은 외부 제어다.
    static bool GetRuleInfo(
        ItemRuleInfo* OUT result, 
        const SCItemSlot& ref_item_slot,
        bool is_limited_set, 
        const ItemParseType::ValueType* const parent_parse_type
    );
    // (CHANGES) (f100523.2L) prefetch the rule for a item information
    static bool PrefetchRuleInfo(ItemRuleInfo* OUT result, const BASE_ITEMINFO* item_info);

    // (NOTE) 외부 easiable access interface
    // SCItemSlot 내부에서는 바로 접근해서 사용 중에 있다.
    bool IsEnableDivine() const { return !!(func_attr.value & func_attr.eEnable_Divine); }
    bool IsEnableExtraOption() const { return !!(func_attr.value & func_attr.eEnable_NOption); }
    bool IsEnableRank() const { return !!(func_attr.value & func_attr.eEnable_Rank); }
    bool IsEnableEnchant() const { return !!(func_attr.value & func_attr.eEnable_Enchant); }
    bool IsEnableSocket() const { return !!(func_attr.value & func_attr.eEnable_Socket); }
    bool IsEnablePossession() const { return !!(func_attr.value & func_attr.eEnable_Possession); }
    bool IsEnableTimer() const { return !!(func_attr.value & func_attr.eEnable_Timer); }
    bool IsEnableCoord() const { return !!(func_attr.value & func_attr.eEnable_Coord); }
    // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    bool IsEnableEtherealize() const {
        return (func_attr.value & func_attr.eEnable_Etherealize) != 0;
    }
    bool IsEnableRandomize() const 
    {
        return (!!is_editable && func_attr.IsEnable(ItemFuncAttr::eEnable_Randomize));
    }
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    bool IsEnableAwakening() const
    {
        return (!!func_attr.value & func_attr.IsEnable(ItemFuncAttr::eEnable_Awakening));
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
};

//
//==================================================================================================
// 공유 정보를 사용하고 있는 아이템일 경우 설정되는 정보
// SCItemSlot::shared_
//----------------------------------------------------------------------------------------------
//    // Sharing Part - address union block
//    union {
//        SCLimitedInfo   limited_info_;
//        SCFateInfo      fate_info_;
//        SCPetInfo       pet_info_;
//        SCWayPointInfo  waypoint_info_;
//        SCLottoInfo     lotto_info_;
//        nsSlot::SCCoordinateInfo coordinate_info_;
//        SCRiderInfo      rider_info_;
//        SCRiderPartsInfo rider_parts_info_;
//    } shared_;
struct SCSharedInfoCheck
{
    BOOLEAN value; // true : used | false : unused
};

}; //end of namespace 'nsSlot'
//
//==================================================================================================
//==================================================================================================

namespace nsSlot {
;
// for internal operation
// (Specification)
//    [0]       [1]     [2]     [3]    [4]    [5]    [6]    [7]
// add_rider sub_rider add_p0 add_p1 add_p2 sub_p0 sub_p1 sub_p2
struct RiderUpdateBits {
    // 0   - add apply rider option
    // 1   - sub apply rider option
    // 2~4 - add apply parts option
    // 5~7 - sub apply parts option
    union {
        struct {
            BYTE value;
        };
        struct {
            BYTE add_rider  : 1;
            BYTE sub_rider  : 1;
            BYTE add_parts0 : 1;
            BYTE add_parts1 : 1;
            BYTE add_parts2 : 1;
            BYTE sub_parts0 : 1;
            BYTE sub_parts1 : 1;
            BYTE sub_parts2 : 1;
        };
    };

    static const BYTE NoOperation = 0;
    static const BYTE AllAddParts = (1 << 2) | (1 << 3) | (1 << 4);
    static const BYTE AllSubParts = (1 << 5) | (1 << 6) | (1 << 7);

    static const BYTE AllAddOptions = (1 << 0) | AllAddParts;
    static const BYTE AllSubOptions = (1 << 1) | AllSubParts;
};
}; //end of namespace

#pragma pack(pop)

#endif //_SOLAR_SLOT_SCITEMSTREAMSERIALIZER_H

