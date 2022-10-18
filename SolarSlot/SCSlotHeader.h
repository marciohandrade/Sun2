#ifndef _SOLAR_SLOT_SCSLOTHEADER_H
#define _SOLAR_SLOT_SCSLOTHEADER_H
#pragma once

//==================================================================================================
/// ���� ��� 
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - ���������� ����ϴ� ���� ���� ���� ���
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
//  UB_ALL          = ((1<<4)-1),           ///< ��� ���� ������Ʈ
//};

enum eSlotContainerIndex // (WAVERIX) (CHANGES) //eSlotContainerIndex -> eSlotContainerIndex��Ȱ.
{
    SI_TEMPINVENTORY = 0,
    SI_INVENTORY,
    SI_EQUIPMENT,
    SI_WAREHOUSE,
    SI_SKILL,
    SI_QUICK,
    SI_STYLE,
    SI_GUILDWAREHOUSE,
    SI_MAX, ///< Ŭ���̾�Ʈ������ ����ϱ⶧���� > SI_MAX
    SI_EVENT_INVENTORY,
    SI_TRADE,
    SI_NPCSTORE,            
    SI_ENCHANT,
    SI_ENCHANT_MATERIAL,// ��ȭ ���
    SI_ENCHANT_OPTION,  // ��ȭ �߰� ÷���� (�ֻ�� ��ȭ��)
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
    SI_AUTOMATIC_INFO,  //!< //! 2010.8.9 / i4u4me / ������
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
    SI_MAX_TOTAL    // (WAVERIX) (ADD-IN) ���� ������. 
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
    DECDURA_TYPE_ATTACK,        // �����ϰų� ���ݴ��� �� ������ ����
    DECDURA_TYPE_LEAVE_ROOM,    // �̼��� ���������� ���� �� ������ ����
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
//  - �Ҹ� ��� (�ð� ���)
// (NOTE)
//  - eCHARGE_SUB_GENERAL | eCHARGE_PC_ROOM
//      ����� ����: PC�� ���� ���� (PC�� ���� ������)
//  - eCHARGE_SUB_FIXED_AMOUNT
//
//  - eCHARGE_SUB_FIXED_AMOUNT_EX
//
//  - eCHARGE_SUB_FIXED_QUANTITY
//
//  - eCHARGE_SUB_COORDINATE
//      �̰� �� �ߴ��� �𸣴� Ÿ�� - ���� �̷��� �� �ž�? - �Ҹ� ����� �ð� ����� �ƴϴ�.
//  - eCHARGE_SUB_FIXED_AMOUNT_WAIT
//      ���� �Ⱓ ��� ��, ����� 7�� ��� �� ����
//  - eCHARGE_SUB_FATE
//      ��Ű ������, ���� ���� ���� �����Ǹ�, ���� ����� ��� �Ұ�
//  - eCHARGE_SUB_COORDINATE_EX
//      ���� ��������Ʈ ���� ���� ���� �� ����� �� �ִ� ���������� �г�Ƽ Ÿ�� +
//      ��� �Ⱓ ���� ��, ����� 7�� ��� �� ����
//  - eCHARGE_SUB_RIDER_WITH_PART
//      Rider Item : �Լ� �������� ���� �Ⱓ ��� ��, ��� �Ұ� ���·� ����
//      Rider Parts Item : Ȯ�� ������(�ִ� 30��) - ���� �������� ���� �Ⱓ ��� ��,
//                         ��� �Ұ� ���·� ����
//  - eCHARGE_SUB_FIXED_AMOUNT_EX2
//      ȹ��ú��� �����Ⱓ ����� ����
enum eCHARGE_SUB_TYPE
{
    eCHARGE_SUB_GENERAL  = 0,       //< [00]���� ����
    // �ּ� ������ : �ð�, ����� : ��
    eCHARGE_SUB_FIXED_AMOUNT,       //< [01]������ : ���� �Ⱓ�� (30��=30*24�ð� ->(������) 2007-06-07 24:12:00)
    eCHARGE_SUB_FIXED_AMOUNT_EX,    //< [02]Ȯ�������� : ����� ���� ���� �Ⱓ ���  (30�ð� ->(ó��������) 2007-06-07 24:12:00)
    eCHARGE_SUB_FIXED_QUANTITY,     //< [03]������ : ��� �Ⱓ�� (30�ð�)
    eCHARGE_SUB_COORDINATE,         //< [04]��ǥ
    eCHARGE_SUB_FIXED_AMOUNT_WAIT,  //< [05]������ : ���� �Ⱓ�� + 7�� ���
    eCHARGE_SUB_FATE,               //< [06]����Ʈ ������(Lucky Item)
    eCHARGE_PC_ROOM,                //< [07]PC�� ������
    //__NA001164_20081024_CHAR_WAY_POINT_ITEM
    eCHARGE_SUB_COORDINATE_EX,      //< [08]��ǥ ����Ʈ ��ũ�� ������.
    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    eCHARGE_SUB_RIDER_WITH_PART,    //< [09]������ + ��Ű������ + ������ Ȯ�� ������(�ִ� 30��)
    eCHARGE_SUB_RIDER_WITH_PART_NOTIMER, //< [10] [09]�� ����, Ż��|������ �Ⱓ�� ���� ��� - ���� Ÿ�̸ӷ� �����ϱ⿡ 0���� �����ϱ� �ָ�
    eCHARGE_SUB_FIXED_AMOUNT_EX2,   //< [11]Ȯ��������2 : ȹ��� ���� ���� �Ⱓ ��� //_NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    eCHARGE_SUB_IMMEDIATELY_USE,    //< [12]��û�� : ȹ��� ��� ���Ǵ� ������
    eCHARGE_SUB_IMMEDIATELY_USE_AND_DELETE, //< [13]��û����� : ȹ��� ��� ���Ǹ� ������ �����Ǵ� ������
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    eCHARGE_SUB_COUNTS //upperbound
};

enum eMIXTURE_FLAG
{ // (WAVERIX) (NOTE) �����Ǹ�, Block contents
    MIXTURE_ALL              = 0,
    MIXTURE_ENCHANT          = (1 << 0),     //��þƮ ����
    MIXTURE_RANKUP           = (1 << 1),     //��ũ�� ����
    MIXTURE_CRYSTALIZE       = (1 << 2),     //����ȭ ����
    MIXTURE_COMPOSE          = (1 << 3),     //���� ����
    MIXTURE_EXTRASTONE       = (1 << 4),
    MIXTURE_SOCKET_HOLE_FILL = (1 << 5),     //���� �ձ� ä���
    MIXTURE_SOCKET_EXTRACT   = (1 << 6),     //���� �Ӽ� ����
    MIXTURE_PERIOD_EXTEND    = (1 << 7),     //�Ⱓ ����
    MIXTURE_EXCEPTALL        = ((1<< 8)-1), //��� ������
};

//__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
// ���̴� �������� ���� �ε���, �ش� �������� ���������� 3���� �������� ������ �� �ִ�.
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
	eITEM_EFFECT_ON_EQUIP		,		// ����ؾ����� ȿ�� ����
	eITEM_EFFECT_ON_INVEN		,		// �κ��� �ִ� �͸����ε� ȿ�� ����
	eITEM_EFFECT_UPPERBOUND		,
};

//==================================================================================================
//#define DEPRECATE_RESERVED  __declspec(deprecated)
#define DEPRECATE_RESERVED

#define DEPRECATE_CODE_DELETE_RESERVED (1)
//==================================================================================================

#endif //_SOLAR_SLOT_SCSLOTHEADER_H

