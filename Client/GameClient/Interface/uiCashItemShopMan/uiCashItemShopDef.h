#pragma once
//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "ShopManager.h"


#define _ITEM_NUM_PER_PAGE		10

#define _PRESENT_ITEM_PER_PAGE	6

#define _MAX_PRICE_INFO			4

#define _PACKAGE_ITEM_PER_PAGE	3

#define _PACKAGE_CHECK_LIST_LINE_NUM	8


const WzID c_wiCashItemShopFont					= StrToWzID("m212");
const DWORD c_CashItemShopListSelBarColor		= RGBA(255,255,255,0);
const DWORD c_CashItemShopListBKColor			= RGBA(255,255,255,0);
const DWORD c_CashItemShopListTextColor			= RGBA(255,255,255,255);
const DWORD c_CashItemShopListSelTextColor		= RGBA(255,255,255,255);

const DWORD c_CashItemShopListPcBangColor		= RGBA( 211, 191 , 60 , 255);
const DWORD c_CashItemShopListNonPcBangColor	= RGBA( 116 , 116 ,116 ,170);
const DWORD c_CashItemShopListErrorColor		= RGBA( 204, 0, 0, 255 );
const DWORD c_CashItemShopListTradeInErrorColor	= RGBA( 255, 255, 153, 255 );
const DWORD c_CashItemShopListNameColor			= RGBA( 240, 201, 121, 255 );
const DWORD c_CashItemShopListNormalColor		= RGBA( 255, 255, 153, 255 );

const WzID	c_wiMessageFont = StrToWzID("m212");

#define SHOW_DEFAULT_READ_PRESENT_MESSAGE_LINE		4

enum eItemDesc
{
	eItemDescDay,
	eItemDescNum,
	eItemDescMoney,

	eItemDescMax,
};


enum eExtensionItemDesc
{
	eExtensionItemDescDay,
	eExtensionItemDescMoney,

	eExtensionItemDescMax,
};


enum eRqstState
{
	eRqstState_None = 0,
	eRqstState_Buy,
	eRqstState_Package,
	eRqstState_Present_List,
	eRqstState_Present_Send,
	eRqstState_Present_Rcv,
	eRqstState_Present_Discard,
};

enum ePresentState
{
	ePresentState_None = -1,
	ePresentState_Send,
	ePresentState_Rcv,
	ePresentState_Return,
	ePresentState_Returned,
};

enum eCashItemShopDlgState
{
	eCashItemShopDlgState_None = -1,
	eCashItemShopDlgState_CashShop,
	eCashItemShopDlgState_Package,
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	eCashItemShopDlgState_Mileage,
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
	eCashItemShopDlgState_ShopBasket,
	eCashItemShopDlgState_Present,
};


struct stCashPriceInfo
{
	PRICEGUID	priceGuid;
	DWORD		dwUseTime;		//< (정액제, 정량제:시간(분)
	DWORD		dwAmount;		//< 소모성,무제한:수량)
	DWORD		dwPrice;
	BYTE		bySaleRatio;	//< 세일 비율값 ( 0 ~ 100 )
};

struct stCashItemInfo
{
	ITEMGUID	itemguid;
	DWORD		dwItemCode;
	BYTE		moneyType;			//< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
	DWORD		dwCoolTime;			// 구매 쿨타임	
	DWORD64		SellEndDate;		//판매 종료일	//< 20061209243500
	BYTE		byEnchant;			//	인첸트
	BYTE		byRank;				//	랭크

	stCashPriceInfo	stPrice[_MAX_PRICE_INFO];
};

struct stPieceInfo
{
	DWORD		dwUseTime;		//< (정액제, 정량제:시간(분)
	DWORD		dwAmount;		//< 소모성,무제한:수량)
	DWORD		dwItemCode;
	BYTE		byEnchant;			//	인첸트
	BYTE		byRank;				//	랭크

};

struct stPresentItemInfo
{
	GIFTGUID	giftGuid;
	BYTE		moneyType;			//< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
	DWORD		dwCoolTime;			// 구매 쿨타임	
	DWORD64		SellEndDate;		//판매 종료일	//< 20061209243500
	BYTE		byItemType;			//< enum { SINGLEITEM=1, PACKAGEITEM=2,};
	DWORD64		SendDateTime;		//< double	m_dbSendDate;에서 변환 : YYYYMMDDHHMMSS
	BYTE		byPresentState;		//< 상태 enum ePRESENT_STATE { ePS_WAIT	= 0, ePS_ACCEPT, }
	PRICEGUID	priceGuid;
	DWORD		dwPrice;
		
	TCHAR		strPlayerName[MAX_CHARNAME_LENGTH+1];
	TCHAR		strSendMessage[MAX_GIFT_MESSAGE_LENGTH+1];

	int			iPieceCount;

	stPieceInfo stPiece[MAX_PACKAGE_ITEM_SIZE];	//	일단 패키지 아이템은 선물을 안한다고 하지만 어찌될지 몰라서 패키지 대비;;
};

struct stPresentRcvInfo
{
	BYTE	byPage;
};


struct stPackageDetailInfo
{
	DWORD		dwItemCode;
	BYTE		Aoumt;
	DWORD		dwUseTime;
	BYTE		byEnchant;			//	인첸트
	BYTE		byRank;				//	랭크
};

struct stPackageItemInfo
{
	ITEMGUID	itemguid;
	DWORD		dwImageCode;
	TCHAR		szName[MAX_PACKAGE_NAME_LENGTH+1];
	BYTE		saleRatio;
	BYTE		moneyType;			//< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
	DWORD		dwCoolTime;			// 구매 쿨타임	
	DWORD64		SellEndDate;		//판매 종료일	//< 20061209243500
	DWORD		dwPrice;			//	패키지 가격
	PRICEGUID	priceGuid;
	BYTE		byDetailItemCount;
	
	stPackageDetailInfo	stDetailItem[MAX_PACKAGE_ITEM_SIZE];
};

struct stRenderTooltipInfoforPackage
{
	TCHAR		szName[MAX_PACKAGE_NAME_LENGTH+1];
	DWORD		dwItemCode[MAX_PACKAGE_ITEM_SIZE];
	DWORD		dwCoolTime;			// 구매 쿨타임
	DWORD64		SellEndDate;		//판매 종료일

	BYTE		byEnchant;			//	인첸트
	BYTE		byRank;				//	랭크

};

struct stCashShopRcvInfo
{
	BYTE	byCategory;
	BYTE	byPage;
	int		iTotalPage;
};

struct stRenderTooltipInfo
{
	DWORD		dwItemCode;
	DWORD		dwCoolTime;			// 구매 쿨타임
	DWORD64		SellEndDate;		//판매 종료일
	BYTE		byEnchant;			//	인첸트
	BYTE		byRank;				//	랭크
};

struct CASH_ITEM_IMG
{
	DWORD		dwIconCode;
	CODETYPE	itemCode;
    int type;
	IconImage	*pImage;
};

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
