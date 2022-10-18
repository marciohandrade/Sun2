#pragma once


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


//------------------------------------------------------------------------------
//
//    작업자: KTW
//    uiCashItemShopExtensionBuy class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"
#include "SCItemSlotContainer.h"

class uiCashItemShopMan;

class uiCashItemShopExtensionBuy: public uiBase, public SCItemSlotContainer
{
	enum eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY
	{
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_IMG_ITEM = 0,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_BTN_EXIT,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC1,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC2,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC3,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC4,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC5,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC6,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC7,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC8,
		
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE1,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE2,	
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE3,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_SALE4,
		
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_NAME,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL2,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL3,
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL4,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_BTN_EXTENSION,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_MAX,
	};

	enum eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC
	{
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC1,	
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC2,	
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC3,	
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC4,	
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC5,	
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC6,	
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC7,
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC8,

		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_DESC_MAX,
	};

	enum eCASH_ITEM_SHOP_EXTENSION_BUY_TXT
	{
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_NAME,
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_ITEM_COST_MONEY,
		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MONEY,

		eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MAX,
	};

	enum eCASH_ITEM_SHOP_EXTENSION_BUY_CHK
	{
		eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL1,
		eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL2,
		eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL3,
		eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_SEL4,

		eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_MAX,
	};

public:
	uiCashItemShopExtensionBuy(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopExtensionBuy();

	//--------------------------------	------------------------------------------
	// init/release
	//
public:
	virtual void            Init(CDrawBase* pDrawBase);
	virtual void            Release();

protected:
	void                    InitControls();

	//--------------------------------------------------------------------------
	// helper/comms.
	//
public:
	uiCashItemShopMan*		GetManager();

	//--------------------------------------------------------------------------
	// functions
	//
private:
	BOOL					_isEnable();
	void					_refresh();

	void					_clearItem();
	void					_setItem();
	void					_setIndex( void );
	void					_addIconImage(DWORD dwCode, CODETYPE itemCode);
	void					_removeIconImage();

	void					_renderTooltip();
	void					_renderIconImage();

public:
	void					ClearItem()			{_clearItem();}
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & rSlot );									// 슬롯에 객체 추가
	BOOL									DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	void					SetTargetInfo(SLOTIDX fromSlotIdx, POSTYPE fromPos);
	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	void					ShowBuyItem(stCashItemInfo* pInfo);

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	uiCashItemShopMan*		m_pCashItemShopMan;

	CCtrlStaticWZ*			m_pTxtCtrlDesc[_MAX_PRICE_INFO][eExtensionItemDescMax];
	CCtrlStaticWZ*			m_pTxtCtrlSale[_MAX_PRICE_INFO];
	CCtrlStaticWZ*			m_pTxtCtrl[eCASH_ITEM_SHOP_EXTENSION_BUY_TXT_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckTabs[eCASH_ITEM_SHOP_EXTENSION_BUY_CHK_MAX];
	CCtrlButtonWZ*			m_pImgCtrl;
	CCtrlButtonWZ*			m_pBtnListArrow;

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stRenderTooltipInfo		m_ItemCodeForTooltip;
	CASH_ITEM_IMG			m_BuyItemImg;

	stCashItemInfo			m_BuyCashItemInfo;
	stCashItemInfo			m_BuyCashItemCheckInfo;

	MONEY					total_cash_;
	MONEY					total_heim_;

	int						m_iTipListCurLine;				//	팁 리스트의 현재 스크롤 라인 위치
	int						m_iSelectIndex;

	POSTYPE					m_FromTargetPos;
	POSTYPE					m_PendingTargetPos;
	int						m_iCurIndex;
	SLOTIDX					m_fromSlotIdx;
	POSTYPE					m_fromPos;
	ItemUnitRenderer *		m_pItemUnitRenderer;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopExtensionBuy::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopExtensionBuy::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_EXTENSION_BUY_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopExtensionBuy::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

