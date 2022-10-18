#ifndef _SOLAR_SLOT_SCSLOTHEADER_H
#define _SOLAR_SLOT_SCSLOTHEADER_H
#pragma once

//==================================================================================================
/// 슬롯 헤더 
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 공통적으로 사용하는 슬롯 관련 공통 상수
    @note
    - 
    @history 
    - 09.09.06 | waverix | code rearrangement
*/
//==================================================================================================

//enum eUPDATE_BIT
//{
//  UB_TYPE         = 1,
//  UB_SERIAL       = (1<<1),
//  UB_CODE         = (1<<2),
//  UB_DURA         = (1<<3),
//
//  UB_ALL          = ((1<<4)-1),           ///< 모든 인자 업데이트
//};

enum eSlotContainerIndex // (WAVERIX) (CHANGES) //eSlotContainerIndex -> eSlotContainerIndex부활.
{
    SI_TEMPINVENTORY = 0,
    SI_INVENTORY,
    SI_EQUIPMENT,
    SI_WAREHOUSE,
    SI_SKILL,
    SI_QUICK,
    SI_STYLE,
    SI_GUILDWAREHOUSE,
    SI_MAX, ///< 클라이언트에서만 사용하기때문에 > SI_MAX
    SI_EVENT_INVENTORY,
    SI_TRADE,
    SI_NPCSTORE,            
    SI_ENCHANT,
    SI_ENCHANT_MATERIAL,// 강화 재료
    SI_ENCHANT_OPTION,  // 강화 추가 첨가물 (최상급 강화용)
    SI_RANKUP,
    SI_VENDOR_SELL,
    SI_VENDOR_BUY,  
    SI_ITEM_COMPOSE,
    SI_ITEM_CRYSTALIZE,
    SI_ITEM_ETC_COMPOSE,
    SI_QUEST_REWARD,
    SI_QUEST_LIST_CONTENT,
    SI_EXTENSION,
    SI_LOTTERY, 
    SI_LOTTO,
    SI_RIDER,
    SI_DOMINATION_TOKEN,
    SI_AUTOMATIC_INFO,  //!< //! 2010.8.9 / i4u4me / 오토툴
    SI_CUBE,
    SI_PERK,
    SI_SOCKET_COMPOSITE,
    SI_GM_ITEM,
    SI_VENDOR_SEARCH,
    SI_CHAOSZONE_BATTLEGROUND,
    SI_CHAOSZONE_SSQ,
    SI_SMARTNPC,
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    SI_CHAOSZONE_GOLDRUSH,
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	SI_EQUIPMENT_AWAKENING,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	SI_EQUIPMENT_EVOLUTION,
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
    SI_MAX_TOTAL    // (WAVERIX) (ADD-IN) 상한 설정용. 
};

enum eSlotType
{
    ST_NONE,
    ST_ITEM,
    ST_SKILL,
    ST_QUICK,
    ST_STYLE_QUICK,
    ST_DUMMY_ITEM,
    ST_TRADE,
    ST_VENDOR,
    ST_EVENT,
    ST_LOTTERY,
	ST_VENDORSEARCH,
    ST_MAX,
};

enum eDecDuraType
{
    DECDURA_TYPE_ATTACK,        // 공격하거나 공격당할 때 내구도 감소
    DECDURA_TYPE_LEAVE_ROOM,    // 미션중 고의적으로 나갈 때 내구도 감소
};

enum eSLOT_SERIALIZE
{
    eSLOT_SERIALIZE_LOAD        = 1 << 1,
    eSLOT_SERIALIZE_SAVE        = 1 << 2,

    eSLOT_SERIALIZE_CLIENT_LOAD = eSLOT_SERIALIZE_LOAD | ( 1 << 4),
    eSLOT_SERIALIZE_RENDER_LOAD = eSLOT_SERIALIZE_LOAD | ( 2 << 4),

    eSLOT_SERIALIZE_CLIENT_SAVE = eSLOT_SERIALIZE_SAVE | (10 << 4),
    eSLOT_SERIALIZE_RENDER_SAVE = eSLOT_SERIALIZE_SAVE | (11 << 4),
};

enum eCHARGE_TYPE
{
    eCHARGE_GENERAL,
    eCHARGE_CHARGE, 
    eCHARGE_CHARGE_PPCARD,  //_NA00XXXX_090111_CASH_TYPE_SHARED_
};

//--------------------------------------------------------------------------------------------------
// (REFERENCE) 'ChargeSubType' in ItemScripts
// (DESCRIPTION)
//  - 소멸 방식 (시간 기반)
// (NOTE)
//  - eCHARGE_SUB_GENERAL | eCHARGE_PC_ROOM
//      기능은 동일: PC방 구분 여부 (PC방 전용 아이템)
//  - eCHARGE_SUB_FIXED_AMOUNT
//
//  - eCHARGE_SUB_FIXED_AMOUNT_EX
//
//  - eCHARGE_SUB_FIXED_QUANTITY
//
//  - eCHARGE_SUB_COORDINATE
//      이건 왜 했는지 모르는 타입 - 누가 이렇게 한 거야? - 소멸 방식이 시간 기반이 아니다.
//  - eCHARGE_SUB_FIXED_AMOUNT_WAIT
//      일정 기간 사용 후, 만료시 7일 대기 후 삭제
//  - eCHARGE_SUB_FATE
//      러키 아이템, 인증 이후 기한 설정되며, 기한 만료시 사용 불가
//  - eCHARGE_SUB_COORDINATE_EX
//      개인 웨이포인트 정보 제거 이후 재 등록할 수 있는 시점까지의 패널티 타임 +
//      사용 기간 만료 후, 만료시 7일 대기 후 삭제
//  - eCHARGE_SUB_RIDER_WITH_PART
//      Rider Item : 입수 시점부터 일정 기간 사용 후, 사용 불가 형태로 설정
//      Rider Parts Item : 확장 정액제(최대 30일) - 장착 시점부터 일정 기간 사용 후,
//                         사용 불가 형태로 설정
//  - eCHARGE_SUB_FIXED_AMOUNT_EX2
//      획득시부터 일정기간 사용후 삭제
enum eCHARGE_SUB_TYPE
{
    eCHARGE_SUB_GENERAL  = 0,       //< [00]제한 없음
    // 최소 단위는 : 시간, 계산은 : 초
    eCHARGE_SUB_FIXED_AMOUNT,       //< [01]정액제 : 일정 기간제 (30일=30*24시간 ->(생성시) 2007-06-07 24:12:00)
    eCHARGE_SUB_FIXED_AMOUNT_EX,    //< [02]확장정액제 : 착용시 부터 일정 기간 사용  (30시간 ->(처음장착시) 2007-06-07 24:12:00)
    eCHARGE_SUB_FIXED_QUANTITY,     //< [03]정량제 : 사용 기간제 (30시간)
    eCHARGE_SUB_COORDINATE,         //< [04]좌표
    eCHARGE_SUB_FIXED_AMOUNT_WAIT,  //< [05]정액제 : 일정 기간제 + 7일 대기
    eCHARGE_SUB_FATE,               //< [06]페이트 아이템(Lucky Item)
    eCHARGE_PC_ROOM,                //< [07]PC방 아이템
    //__NA001164_20081024_CHAR_WAY_POINT_ITEM
    eCHARGE_SUB_COORDINATE_EX,      //< [08]좌표 리스트 링크된 아이템.
    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    eCHARGE_SUB_RIDER_WITH_PART,    //< [09]정액제 + 러키아이템 + 파츠는 확장 정액제(최대 30일)
    eCHARGE_SUB_RIDER_WITH_PART_NOTIMER, //< [10] [09]의 변형, 탈것|파츠가 기간이 없는 경우 - 연관 타이머로 동작하기에 0으로 설정하기 애매
    eCHARGE_SUB_FIXED_AMOUNT_EX2,   //< [11]확장정액제2 : 획득시 부터 일정 기간 사용 //_NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    eCHARGE_SUB_IMMEDIATELY_USE,    //< [12]즉시사용 : 획득시 즉시 사용되는 아이템
    eCHARGE_SUB_IMMEDIATELY_USE_AND_DELETE, //< [13]즉시사용삭제 : 획득시 즉시 사용되며 무조건 삭제되는 아이템
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    eCHARGE_SUB_COUNTS //upperbound
};

enum eMIXTURE_FLAG
{ // (WAVERIX) (NOTE) 설정되면, Block contents
    MIXTURE_ALL              = 0,
    MIXTURE_ENCHANT          = (1 << 0),     //인첸트 가능
    MIXTURE_RANKUP           = (1 << 1),     //랭크업 가능
    MIXTURE_CRYSTALIZE       = (1 << 2),     //결정화 가능
    MIXTURE_COMPOSE          = (1 << 3),     //조합 가능
    MIXTURE_EXTRASTONE       = (1 << 4),
    MIXTURE_SOCKET_HOLE_FILL = (1 << 5),     //소켓 뚫기 채우기
    MIXTURE_SOCKET_EXTRACT   = (1 << 6),     //소켓 속성 제거
    MIXTURE_PERIOD_EXTEND    = (1 << 7),     //기간 연장
    MIXTURE_EXCEPTALL        = ((1<< 8)-1), //모두 사용안함
};

//__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
// 라이더 아이템의 파츠 인덱스, 해당 아이템은 내부적으로 3개의 아이템을 포함할 수 있다.
enum eRIDER_PARTS_SLOT_IDX {
    eRIDER_PARTS_SLOT_IDX_HEAD   = 0,
    eRIDER_PARTS_SLOT_IDX_BODY   ,
    eRIDER_PARTS_SLOT_IDX_LEGS   ,
    eRIGER_PARTS_SLOT_IDX_COUNT  // == 3
};

//_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
struct eEtheriaHistory
{
    enum eHistory {
        None     = 0,  // default spec.
        Combined, // the combination item, this item is combined from etheria
        Shell,    // the shell item, this item is extracted options
        Etheria,  // the etheria item, this item has contained all options of the item
    };
    typedef uint8_t value_type;
    value_type value;
};

//_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
struct eEtherealizeMode
{
    enum eMode {
        NoFilter = 0, // keep the kind of item
        CantEtherealize, // can't etherealize
        ExtractEtheriaOnly, // extract to the etheria item not the shell(skin)
        ExtractShellOnly, // extract to the skin item not the etheria
        Mode_Counts
    };
    typedef uint8_t value_type;
    eEtherealizeMode() : value(0)
    {
    }
    value_type value;
};

struct KindOfItemType
{
    typedef uint8_t value_type;
    enum {
        Default = 0,
        Divine,
        Etheria,
        EtheriaWithDivine,
        Shell,
        Counts,
    };
    value_type value;
};

// reference : _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
struct KindOfItemTag
{
    typedef uint8_t value_type;
    enum {
        Tag_None = 0,
        Tag_TradeSkip = 0x01,
        Tag_PCoinPurchase = Tag_TradeSkip
        // upperbound. this tag is used in a binary stream of itemslots. the tag size is '1'bit
    };
    value_type value;
};

// CHANGES: f110329.5L, moved from const.h
enum eITEM_EXCERCISE_EFFECT
{
	eITEM_EFFECT_NONE			= 0,
	eITEM_EFFECT_ON_EQUIP		,		// 장비해야지만 효과 발휘
	eITEM_EFFECT_ON_INVEN		,		// 인벤에 있는 것만으로도 효과 발휘
	eITEM_EFFECT_UPPERBOUND		,
};

//==================================================================================================
//#define DEPRECATE_RESERVED  __declspec(deprecated)
#define DEPRECATE_RESERVED

#define DEPRECATE_CODE_DELETE_RESERVED (1)
//==================================================================================================

#endif //_SOLAR_SLOT_SCSLOTHEADER_H

