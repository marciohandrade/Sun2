
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#ifndef __SOLARSHOP_DEFINE_SHOPSERVER_H
#define __SOLARSHOP_DEFINE_SHOPSERVER_H
#pragma once

// (CHANGES) (100118) (WAVERIX) changes pack usage
#pragma pack(push, 1)

// ��� ����
#define MAX_RECORD_SIZE						1024
#define MAX_ITEM_NAME_LENGTH				64
#define MAX_ITEM_DESC_LENGTH        		128
#define MAX_PACKAGE_NAME_LENGTH     		64
#define MAX_PACKAGE_DESC_LENGTH     		128
#define MAX_ITEM_SIZE               		128
#define MAX_PACKAGE_SIZE            		96
#define MAX_BRANCH_ITEM_SIZE	       		512
#define MAX_BUY_ITEM_SIZE					256
#define MAX_PACKAGE_ITEM_SIZE       		16
#define MAX_ITEM_PRICE_SIZE					8
#define MAX_CHARACTER_NAME_LENGTH			96
#define MAX_BUY_REQ_SIZE					16
//#define MAX_GIFT_MESSAGE_LENGTH				96
#define MAX_GIFT_MESSAGE_LENGTH				512	// 07-03-02 ���� ���� �޽��� ���� ��ġ �� �׽�Ʈ(���� ��Ʈ��)
#define MAX_GIFT_ITEM_SIZE					32

#define MAX_ITEM_EXTEND_OPTIONS				3
#define MAX_ITEM_EXTEND_OPTION_SIZE			1024

#define E_EXCEPTION		(DWORD)(~0)

#define GAME_CODE_MU	"01"
#define GAME_CODE_SUN	"02"
/*
	���� ������ ����
 */
enum eGIFT_STATE
{
	eGS_WAIT	= 0,	///> ���
	eGS_ACCEPT,			///> ���� ����
	eGS_REJECT,			///> ���� �ź�
	eGS_DELETE			///> ���� ����
};

/*
	������ ����
 */
enum eDEF_ITEM_TYPE 
{
	eTYPE_ITEM		= 1,
	eTYPE_PACKAGE
};

/*
	������ ����
 */
enum eBUY_TYPE
{
	eBT_CASH		= 1,
	eBT_NON_CASH	= 2,
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	eBT_MILEAGE		= 3,
#endif
};
 
/**
ERROR ����
*/
enum eRET_ERROR_CODE
{	
	eRET_FAIL,							///>  ����        
	eRET_SUCCESS,						///>  ����        
	eRET_NOT_ENOUGH_CASH = 8,			///>  �ܾ� ����   
	eRET_DB_ERROR,						///>  Shop DB ����
	eRET_NOT_FOUND_ITEM,				///>  ���� ���� �ʴ� �Ǹ� ������
	eRET_LIMIT_BUY_COUNT_ERROR,			///>  �Ǹ� ���Ѽ� ����
	eRET_BUY_COOL_TIME_ERROR,			///>  ���� ��Ÿ�� ����
	eRET_INVALID_VALUE,					///>  �߸��� ���� ���
	eRET_INVALID_ID_VALUE,				///>  �߸��� ID�� GUID���� ���r
	eRET_NOT_ENOUGH_MILEAGE,			///>  ���ϸ��� ����			=====> 2007/6/5 �߰�
    eRet_INVALID_BILLING_PARAM,         ///>  ���� �Ķ���� ����
    eRet_BILLING_NETWORK_ERROR,         ///>  ���� ���� ��Ʈ��ũ ���
    eRet_BILLING_SERVER_LOGIC_ERROR,    ///>  ���� ���� ���� ���
    eRet_NOT_EXIST_BILLING_ITEM_CODE,   ///>  ���� ������ ��ϵ��� ���� ��ǰ�ڵ�
    eRet_NOT_EXIST_BILLING_TRADE_CODE,  ///>  ���� ������ ��ϵ��� ���� �ŷ������ڵ�
    eRet_EXCEED_LIMIT_CHARGE,           ///>  �����ѵ��ʰ�
    eRet_ABNORMALITY_CHARGE_TRANSACTION,///>  ���������� ���� Ʈ����� 
    eRet_GIFT_ITEM_BLACK_LIST,          ///>  ���� �ڰ� �̴� - black listed
    eRet_GIFT_ITEM_BANNED_USER,         ///>  ���� �ڰ� �̴� - banned user
    eRet_GIFT_ITEM_NOT_EXIST_HISTORY,   ///>  ���� �ڰ� �̴� - �δ޳� �ι��̻� �����̷��� ������
    eRet_EXCEED_LIMIT_GIFT_ITEM,        ///>  ���� �ڰ� �̴� - �����ϱ� �ѵ��ʰ�
    eRet_BILLING_SYSTEM_ERROR,          ///>  ���� �ý��� ����
    eRet_BILLING_ETC_ERROR,             ///>  ���� ��Ÿ����
};

/**
	�Ǹ� ������ ���� ���� record
 */
typedef struct sellItemPrice
{
	DWORD	dwPriceGuid;						///> �Ǹ� ���� ���� ��ȣ
	DWORD	dwItemGuid;							///> �Ǹ� ���� ��ȣ
	DWORD	dwUseTime;							///> (������:��¥ ������:�ð�)
	DWORD	dwAmount;							///> (�Ҹ�,������:����)
	DWORD	dwPrice;							///> �ǸŰ���
	DWORD	dwSellRate;							///> ������ ���� ������	
} tblSellItemPrice;

/**
	�Ǹ� ������ record
 */
typedef struct sellItem
{
	DWORD				dwItemGuid;							///> �Ǹ� ���� ��ȣ
	DWORD				dwItemCODE;							///> ������ ���� ��ȣ
	DWORD				dwCategoryID;						///> ī�װ� ���̵�
	DWORD				dwUseType;							///> ��� �Ӽ�(1: ������ 2: ������ 3: �Ҹ� 4:������)
	DWORD				dwBuyType; 							///> ���� ����(1: ĳ�� 2: ����)
	DWORD				dwCoolTime;							///> ���� ��Ÿ��
	double				dbVarEndDate;						///> ���� ����
	DWORD				dwLimitSellCount;					///> ������ �� �Ǹ� ����
	DWORD				dwState;							///> ������ ����(1:�Ǹ��� 2:�ǸŴ����)
	char 				szItemName[MAX_ITEM_NAME_LENGTH];	///> ������ �̸�
	char 				szItemDesc[MAX_ITEM_DESC_LENGTH];	///> ������ ����
	DWORD				dwPriceCount;
	tblSellItemPrice	itemPrice[MAX_ITEM_PRICE_SIZE];		///> ���� ����
} tblSellItem;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
//2007/6/5 �߰�
/**
	�Ǹ� ���ϸ��� ������ record
*/
typedef struct sellMileageItem
{
	DWORD		dwItemGuid;									///> �Ǹ� ���� ��ȣ
	DWORD		dwItemCODE;									///> ������ ���� ��ȣ
	DWORD		dwCategoryID;								///> ī�װ� ���̵�
	DWORD		dwUseType;									///> ��� �Ӽ�(1: ������ 2: ������ 3: �Ҹ� 4:������)
	DWORD		dwBuyType; 									///> ���� ����(1: ĳ�� 2: ���� 3: ���ϸ���)
	DWORD		dwCoolTime;									///> ���� ��Ÿ��
	double		dbVarEndDate;								///> ���� ����
	DWORD		dwLimitSellCount;							///> ������ �� �Ǹ� ����
	DWORD		dwState;									///> ������ ����(1:�Ǹ��� 2:�ǸŴ����)
	char 		szItemName[MAX_ITEM_NAME_LENGTH];			///> ������ �̸�
	char 		szItemDesc[MAX_ITEM_DESC_LENGTH];			///> ������ ����
	DWORD		dwPriceCount;
	tblSellItemPrice	itemPrice[MAX_ITEM_PRICE_SIZE];		///> ���� ����
} tblSellMileageItem;
#endif

/**
	�Ǹ� ��Ű�� record
 */
typedef struct	sellPackage
{	
	DWORD				dwPackageGuid;							///> ��Ű�� ������ ���� ��ȣ
	tblSellItemPrice	itemPrice;								///> ��Ű�� ���� ����
	DWORD				dwImageCode;							///> ��Ű�� �̹��� �ڵ�
	DWORD				dwCategory;								///> ī�װ� ���̵�
	DWORD				dwBuyType;								///> ���ż���(1: ĳ�� 2: ����)
	DWORD				dwLimitSellCount;						///> ������ �� �Ǹ� ����
	DWORD				dwInItemCount;							///> ���Ե� ������ ����
	double				dbVarEndDate;							///> ���� ����
	DWORD				dwState;								///> ������ ����(1:�Ǹ��� 2:�ǸŴ����)
	tblSellItemPrice	inItemPrices[MAX_PACKAGE_ITEM_SIZE];	///> ���Ե� ������ ���� ���� ����Ʈ
	char 				szPackageName[MAX_PACKAGE_NAME_LENGTH];	///> ��Ű�� �̸�
	char 				szPackageDesc[MAX_PACKAGE_DESC_LENGTH];	///> ��Ű�� ����
} tblSellPackage;

/**
	������ ���� �̷� record
 */
typedef struct buyItem
{
	DWORD	dwServerGuid;							///> ���� ������ ���� ���� Ű
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	///> ĳ���� �̸�
	char	szItemName[MAX_ITEM_NAME_LENGTH];		///> ������ �̸�
	DWORD	dwPriceGuid;							///> ���Ź�ȣ GUID
	DWORD	dwBuyType;								///> ���� ����
	DWORD	dwPrice;								///> ���� ����
	double	dbVarBuyDate;							///> ������
} tblBuyItem;

/**
	������ �з� ��� record
 */
typedef struct itemBranch
{
	DWORD	dwBranchID;
	DWORD	dwGuid;		///> ������ �� ���� ������ Guid / ��Ű���� ���� ��Ű�� Guid
	DWORD	dwItemType;	///> ������ Ÿ��(1:������ 2:��Ű��)
} tblItemBranch;

/**
	���� ��û Transzction record
 */
typedef struct buyItemTransaction
{
	DWORD	dwTransactionGuid;		///> ���� ��û Transaction Guid
	DWORD	dwPriceGuid;			///> �Ǹ� ���� ���� ��ȣ	
	DWORD	dwResult;				///> ���
#if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    enum { PURCHASE_DEFAULT = 1, PURCHASE_PP_CASH = 2 };
    BYTE    buy_cache_type;         //< ���� ĳ��Ÿ�� (1:ĳ��, 2:����ī��_PP_)
#endif
						/*
							0 : ���� 
							1 : ����
							8 : �ܾ� ����
							9 : Shop DB ����
						*/
} tblBuyItemTransaction;

/**
	���� ���� ��� record
 */
typedef struct recvGiftItem
{
	DWORD 	dwGiftItemID;								///> GiftItemID
	DWORD	dwBuyItemID;								///> BuyItemID
	DWORD	dwPriceGuid;								///> PriceGuid
	char	szSendCharName[MAX_CHARACTER_NAME_LENGTH];	///> ���� ����� ĳ���� �̸�
	char	szSendMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
	DWORD	dwState;									///> ����
	char	szItemName[MAX_ITEM_NAME_LENGTH];			///> ������ �̸�
	double	dbSendDate;									///> ���� ��¥
} tblRecvGiftItem;

/**
	���� ���� ��� record
 */
typedef struct sendGiftItem
{
	DWORD 	dwGiftItemID;								///> GiftItemID
	DWORD	dwBuyItemID;								///> BuyItemID
	DWORD	dwPriceGuid;								///> PriceGuid
	char	szSendMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
	char	szRecvCharName[MAX_CHARACTER_NAME_LENGTH];	///> ���� ����� ĳ���� �̸�
	char	szRecvMessage[MAX_GIFT_MESSAGE_LENGTH];		///> ���� �޼���
	DWORD	dwState;									///> ����
	char	szItemName[MAX_ITEM_NAME_LENGTH];			///> ������ �̸�
	double	dbSendDate;									///> ���� ��¥
} tblSendGiftItem;

// ����, �� Ȯ��ɼ�
typedef struct itemExtendOption
{
	DWORD	dwItemGuid;
	DWORD	dwItemOptions[MAX_ITEM_EXTEND_OPTIONS];		///> 0. ��æƮ���� 1. ��ũ������
}tblItemExtendOption;

#if !defined(__SHOP_CASH_DATA_DECLARATION__)
#define __SHOP_CASH_DATA_DECLARATION__
// presentation by _NA00XXXX_090111_CASH_TYPE_SHARED_
struct CashData
{
    CASH cash_;     // default cash
    CASH pp_cash_;  // pp card cash
    DWORD mileage_; // china mileage service
};
#endif

//#ifdef _NA_0_20101015_SOLAR_SHOP_REFACTORING
typedef struct sellItemContainer
{
    BOOL      is_skin_item;    // ��Ų ������ ����
    DWORD     shopitem_filter;
    DWORD     partition_index;  // ���Ե� ��Ƽ��
    DWORD     category_index;   // ī�װ�
    DWORD     cache_index;

    tblSellItem* singleitem_info;
    tblSellPackage* packageitem_info;
}tblSellItemContainer;
//#endif

//==================================================================================================
#pragma pack(pop)

#endif //__SOLARSHOP_DEFINE_SHOPSERVER_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
