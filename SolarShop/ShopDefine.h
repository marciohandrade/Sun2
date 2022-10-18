
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#pragma once

#include <list>

// ���� typedef 
typedef INT		ITEMGUID;
typedef INT		PRICEGUID;
typedef INT		GIFTGUID;


class ShopItem;
class ShopItemPrice;
class PackageItem;
typedef std::list<ShopItem*>						SHOPITEM_LIST;
typedef std::list<ShopItemPrice*>					SHOPITEMPRICE_LIST;
typedef std::list<PackageItem*>						PACKITEM_LIST;





//#ifdef _NA_0_20101015_SOLAR_SHOP_REFACTORING
//  ��� ������  : ��ü����/����/�̺�Ʈ
//  ��       : ��ü����/���/����/�尩/����/����/��������/��Ʈ/����
//  ��Ų         : ��ü����/���/����/�尩/����/����/��������/��Ʈ/����
//  Ŀ���͸���¡ : ��ü����/���/��/��Ÿ
//  ������       : ��ü����/����/����/����/����Ƿ�/�ɷ�ġ��ȭ��/��ɼ���/���Ǽ���
//  ��           : ��ü����/���/��/��Ų
//  Ż��         : ��ü����/Ż��/�Ӹ�����/��������/�ٸ�����
//  PC�� ����    : ��ü����/Ż��/�Ӹ�����/��������/�ٸ�����



enum eSHOP_PARTITION
{
    SHOP_PARTITION_SINGLEITEM,         // ĳ�� ������
    SHOP_PARTITION_PACKAGEITEM,        // ��Ű�� ������
    SHOP_PARTITION_MILEAGEITEM,        // ����Ʈ ������
//    SHOP_PARTITION_PRESENTITEM,        // ���� ������
    SHOP_PARTITION_MAXCOUNT,
};


enum eSHOP_FILTER
{
    SHOP_FILTER_NONE               = 0,
    SHOP_FILTER_CHAR_BERSERKER     = (1 << 0),
    SHOP_FILTER_CHAR_ELEMENTALIST  = (1 << 1),
    SHOP_FILTER_CHAR_SHADOW        = (1 << 2),
    SHOP_FILTER_CHAR_PALADIN       = (1 << 3),
    SHOP_FILTER_CHAR_VALKYRIE      = (1 << 4),

    SHOP_FILTER_EVENT              = (1 << 9),  // �̺�Ʈ
    SHOP_FILTER_DISCOUNT           = (1 << 10),  // ����
    SHOP_FILTER_NEWARRIVAL         = (1 << 11),  // �ű�
    SHOP_FILTER_POPULAR            = (1 << 12),  // �α�

    SHOP_FILTER_CHAR_ALL           = (SHOP_FILTER_CHAR_BERSERKER | SHOP_FILTER_CHAR_ELEMENTALIST |
                                     SHOP_FILTER_CHAR_SHADOW | SHOP_FILTER_CHAR_PALADIN | SHOP_FILTER_CHAR_VALKYRIE),         // => SHOP_FILTER_CHAR_NONE

    SHOP_FILTER_SLOGAN_ALL         = (SHOP_FILTER_EVENT | SHOP_FILTER_DISCOUNT | SHOP_FILTER_NEWARRIVAL | SHOP_FILTER_POPULAR),
};

//#endif //_NA_0_20101015_SOLAR_SHOP_REFACTORING


// ����!
// ī�װ� ��ȣ�� ������ �ֱ⶧����
// �� DB�� ����� �Ѵ�
enum eSHOP_CATEGORY
{
	CT_ALLITEM			= 0,			//< ��� ������
	
	// �� Ÿ��
	CT_NEWARRIVAL		= 1,			//< �ű� ��ǰ
	CT_POPULARITEM		= 2,			//< �α� ��ǰ
	CT_DISCOUNT			= 3,			//< ����
	CT_EVENT			= 4,			//< �̺�Ʈ
	CT_TEMP				= 5,			//< ������ PC���̾����� ���� �����ϴ� �� Ÿ��
	
	// ī�װ�
	CT_WASTEITEM		= 6,			//< �Ҹ�
	CT_ARMOR			= 7,			//< ��
	CT_FUNCTIONAL		= 8,			//< ��ɼ� 
	CT_ETC				= 9,			//< ��Ÿ
	CT_PACKAGE			= 10,			//< ��Ű��

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	CT_MILEAGE			= 11,			//< ���ϸ���
#endif

	CT_PERIOD_EXTEND	= 12,			//< �Ⱓ����
	CT_WEAPON			= 13,			//< ����
	CT_PCROOM			= 14,			//< PC��

    CT_PPCASH           = 15,           //< PP cash purchase defined by _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

	CT_MAX,


//#ifdef _NA_0_20101015_SOLAR_SHOP_REFACTORING
    SHOP_CATEGORY_INVALID = CT_MAX + 1, // ��ȿ���� ���� ī�װ�

    SHOP_CATEGORY_ROOT,                 // Root ī�װ�

    SHOP_CATEGORY_ALL,                  // ��� ������

    SHOP_CATEGORY_EQUIP_ALL,            // ���
    SHOP_CATEGORY_EQUIP_HELMET,         // ���
    SHOP_CATEGORY_EQUIP_ARMOR,          // ����
    SHOP_CATEGORY_EQUIP_GLOVES,         // �尩
    SHOP_CATEGORY_EQUIP_PANTS,          // ����
    SHOP_CATEGORY_EQUIP_BOOTS,          // ����
    SHOP_CATEGORY_EQUIP_PROTECTOR,      // ��������
    SHOP_CATEGORY_EQUIP_BELT,           // ��Ʈ
    SHOP_CATEGORY_EQUIP_SHIRTS,         // ����
    SHOP_CATEGORY_EQUIP_WEAPON,         // ����

    SHOP_CATEGORY_SKIN_ALL,             // ��Ų
    SHOP_CATEGORY_SKIN_HELMET,          // ���
    SHOP_CATEGORY_SKIN_ARMOR,           // ����
    SHOP_CATEGORY_SKIN_GLOVES,          // �尩
    SHOP_CATEGORY_SKIN_PENTS,           // ����
    SHOP_CATEGORY_SKIN_BOOTS,           // ����
    SHOP_CATEGORY_SKIN_PROTECTOR,       // ��������
    SHOP_CATEGORY_SKIN_BELT,            // ��Ʈ
    SHOP_CATEGORY_SKIN_SHIRTS,          // ����
    SHOP_CATEGORY_SKIN_WEAPON,          // ����

    SHOP_CATEGORY_CUSTOMIZING_ALL,      // Ŀ���͸���¡ ��ü
    SHOP_CATEGORY_CUSTOMIZING_HAIR,     // �Ӹ�
    SHOP_CATEGORY_CUSTOMIZING_FACE,     // ��
    SHOP_CATEGORY_CUSTOMIZING_ETC,      // ��Ÿ

    SHOP_CATEGORY_ITEM_ALL,            // ������ ��ü
    SHOP_CATEGORY_ITEM_LUCKY,          // ����
    SHOP_CATEGORY_ITEM_MATERIAL,       // ����
    SHOP_CATEGORY_ITEM_GUILD,          // ����
    SHOP_CATEGORY_ITEM_TICKET,         // ����Ƿ�
    SHOP_CATEGORY_ITEM_STRENGTHEN,     // �ɷ�ġ ��ȭ��
    SHOP_CATEGORY_ITEM_FUNCTIONALITY,  // ��ɼ���
    SHOP_CATEGORY_ITEM_CONVENIENCE,    // ���Ǽ���

    SHOP_CATEGORY_PET_ALL,              // �� ��ü
    SHOP_CATEGORY_PET,                  // ��
    SHOP_CATEGORY_PET_FEED,             // ���
    SHOP_CATEGORY_PET_SKIN,             // ��Ų

    SHOP_CATEGORY_VEHICLE_ALL,          // Ż�� ��ü
    SHOP_CATEGORY_VEHICLE,              // Ż��
    SHOP_CATEGORY_VEHICLE_HEAD,         // �Ӹ� ����
    SHOP_CATEGORY_VEHICLE_BODY,         // ���� ����
    SHOP_CATEGORY_VEHICLE_LEGS,         // �ٸ� ����


    SHOP_CATEGORY_PCROOM,               // PC��
    SHOP_CATEGORY_PPCASH,               // PP Cash ���� ������.
    SHOP_CATEGORY_PERIOD_EXTEND,        // �Ⱓ ����


    SHOP_CATEGORY_END,                  // ����Ҽ�����. ī�װ� ���� ������ ���� ������ ��ġ ǥ���̴�.

    SHOP_CATEGORY_MAXCOUNT = SHOP_CATEGORY_END - SHOP_CATEGORY_INVALID,
//#endif // _NA_0_20101015_SOLAR_SHOP_REFACTORING
};


enum eSHOP_CATEGORY_SEPERATE
{
	SHOP_ITEM = 0,
	SHOP_PACKAGE,
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	SHOP_MILEAGE,
#endif
	SHOP_SEPERATE_MAX,
};

// Specification
//  MONEY_CASH_TYPE = { cash, pp cash }
enum eSHOP_MONEY_TYPE
{
	MONEY_CASH_TYPE=1,  // shop�� ppcash�� �����ۺ� ���� ó���� �������� �ʴ´�.
	MONEY_HIME_TYPE,
	MONEY_MILEAGE_TYPE, // useful for __CN_0_20070507_SHOP_MILEAGE
};

enum eSHOP_SERIALIZE
{
	eSHOP_SERIALIZE_LOAD,
	eSHOP_SERIALIZE_SAVE,

	eSHOP_SERIALIZE_CLIENT_LOAD,
	eSHOP_SERIALIZE_CLIENT_SAVE,

	// �߰��Ȱ�
	eSHOP_SERIALIZE_ITEM_LIST_LOAD,
	eSHOP_SERIALIZE_ITEM_LIST_SAVE,

	eSHOP_SERIALIZE_ITEM_LOAD,
	eSHOP_SERIALIZE_ITEM_SAVE,

	eSHOP_SERIALIZE_PACKAGE_LOAD,
	eSHOP_SERIALIZE_PACKAGE_SAVE,
};

namespace RC
{
enum eSHOP_RESULT
{
	RC_SHOP_SUCCESS,
	RC_SHOP_FAILED,
	RC_SHOP_PENDING,
	RC_SHOP_CANNOTBUYITEM,							//< �Ǹ����� �ʴ� �������Դϴ�.
	RC_SHOP_CANNOTBUYITEMPRICE,						//< �Ǹ����� �ʴ� ������ �����Դϴ�.
	RC_SHOP_USERISNOTFOUND,							//< ������ ã�� �� �����ϴ�.
	RC_SHOP_SHOPSERVERDISCONNECTED,					//< �������� ����Ǿ������ʽ��ϴ�.(��õ� �̿����ּ���)
	RC_SHOP_NOTENOUGHCASH,							//< ĳ���� �����մϴ�.
	RC_SHOP_NOTENOUGHMONEY,							//< ������ �����մϴ�.
	RC_SHOP_DB_ERROR,								//< �� DB����
	RC_SHOP_NOTENOUGHINVENTORYSPACE,				//< �κ��丮 ������ �����մϴ�.
	RC_SHOP_CRITICAL_ERROR,							//< ġ������ �����Դϴ�. ����� �����ϼ���!
	RC_SHOP_INVALID_CHAR_NAME_LEN,					//< ĳ���� �̸� ���̸� �������� �ʽ��ϴ�.
	RC_SHOP_STANDBY_SERVICE,						//< �� ���� �غ����Դϴ�. ��õڿ� �̿��ϼ���.(����������)
	RC_SHOP_ALREADY_BUYING_TRANSACTION,				//< �̹� ���� Ʈ������� �������Դϴ�. ��õ� �̿��ϼ���.
	RC_SHOP_SERVICE_STOP,							//< ���� �������Դϴ�. ��õڿ� �̿��ϼ���.
	RC_SHOP_CANNOTBUYBYLIMITCOUNT,					//< ���� ���� ������ �Ѿ�� ������ �� �����ϴ�.
	RC_SHOP_NOTEXPIREDCOOLTIME,						//< ���� ��Ÿ���� ������ �ʾƼ� ������ �� �����ϴ�.
	RC_SHOP_CANNOT_USE_SHOP,						//< ������ ���� �̿��� �� �����ϴ�.

    //2007/1/17 ���� 4:31:52
	RC_SHOP_OVER_BUY_COUNT,							//< �ѹ��� ������ �� �ִ� ������ �Ѿ���ϴ�.
	RC_SHOP_INVALID_GIFT_GUID,						//< ��ȿ���� �ʴ� ���� �Ϸù�ȣ�Դϴ�.
	RC_SHOP_CANNOT_OBTAIN_GIFT,						//< ������ ���� �� �����ϴ�.
	RC_SHOP_CANNOT_REJECT_GIFT,						//< ������ ������ �� �����ϴ�
	RC_SHOP_CANNOT_DISCARD_GIFT,					//< ��������� ������ �� �����ϴ�.
	RC_SHOP_CANNOT_FIND_CHARNAME,					//< ĳ���� �̸��� �ش��ϴ� ĳ���͸� ã�� �� �����ϴ�.
	RC_SHOP_CANNOT_SEND_GIFT_MYSELF,				//< �ڱ��ڽſ��� ������ ���� �� �����ϴ�.
	RC_SHOP_CANNOT_REJECT_GIFT_MYSELF,				//< �ڱ��ڽſ��� ������ �ݼ��� �������ϴ�.
#ifdef __NA_00387_20070605_SHOP_MILEAGE2			
	RC_SHOP_NOTENOUGHMILEAGE,						//< ���ϸ����� �����մϴ�.
#endif

	RC_SHOP_NOT_EXIST_EXTEND_ITEM,					//< �Ⱓ�� ������ �������� �������� �ʴ´�.
	RC_SHOP_CANNOT_BUY_PC_ROOM_ITEM_GENERAL_USER,	//�Ϲ� ������ PC�� ������ ���� �Ұ�
	RC_SHOP_CANNOT_PRESENT_ITEM,					//< �ش� �������� ������ �� ����. __NA001087_080618_APPEND_CASHITEM_PRESENT_PROTECTION_FUNTION__

    //_US_SHOP_GIFT_ERRORCONTROL_ADD �Ϲ� �����ϱ� ���� ��� ��
    RC_SHOP_INVALID_BILLING_PARAM,                  ///>  ���� �Ķ���� ����
    RC_SHOP_BILLING_NETWORK_ERROR,                  ///>  ���� ���� ��Ʈ��ũ ���
    RC_SHOP_BILLING_SERVER_LOGIC_ERROR,             ///>  ���� ���� ���� ���
    RC_SHOP_NOT_EXIST_BILLING_ITEM_CODE,            ///>  ���� ������ ��ϵ��� ���� ��ǰ�ڵ�
    RC_SHOP_NOT_EXIST_BILLING_TRADE_CODE,           ///>  ���� ������ ��ϵ��� ���� �ŷ������ڵ�
    RC_SHOP_EXCEED_LIMIT_CHARGE,                    ///>  �����ѵ��ʰ�
    RC_SHOP_ABNORMALITY_CHARGE_TRANSACTION,         ///>  ���������� ���� Ʈ����� 
    RC_SHOP_GIFT_ITEM_BLACK_LIST,                   ///>  ���� �ڰ� �̴� - black listed
    RC_SHOP_GIFT_ITEM_BANNED_USER,                  ///>  ���� �ڰ� �̴� - banned user
    RC_SHOP_GIFT_ITEM_NOT_EXIST_HISTORY,            ///>  ���� �ڰ� �̴� - �δ޳� �ι��̻� �����̷��� ������
    RC_SHOP_EXCEED_LIMIT_GIFT_ITEM,                 ///>  ���� �ڰ� �̴� - �����ϱ� �ѵ��ʰ�
    RC_SHOP_BILLING_SYSTEM_ERROR,                   ///>  ���� �ý��� ����
    RC_SHOP_BILLING_ETC_ERROR,                      ///>  ���� ��Ÿ����
    RC_SHOP_USERLEVEL_LOW,                          ///>  ���� ���� �� ���� �����ϱ� �Ұ���

    //�Ϲ� ���� C/S ���� �߰�
    RC_SHOP_CANNOT_SEND_GIFT_MYACCOUNT,             ///>  �ڱ� �������� ������ ���� �� �����ϴ�.
    RC_SHOP_CAN_BUY_PP_CASH_ONLY,       ///> �� ��ǰ�� ���� ���縸���� ���Ű� �����մϴ�. (for Global)
    RC_SHOP_CAN_PRESENT_PP_CASH_ONLY,   ///> ������ ���� ����θ� �����մϴ�. (for Global) _NA000424_GLOBAL_CASH_ITEM_RENEWAL_

    RC_SHOP_NOT_FRIEND_ME,                          //���� �޴� ����� �� ģ�� ��Ͽ� ����.                 
    RC_SHOP_NOT_FRINED_YOUR,                        //���� �޴� ����� ģ����Ͽ� ���� ����.
};

};

//==================================================================================================

namespace nsShop {
};







#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL




