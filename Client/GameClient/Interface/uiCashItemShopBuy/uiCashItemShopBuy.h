#pragma once


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCashItemShopBuy class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopBuy: public uiBase
{
    enum eDIALOG_CASH_ITEM_SHOP_BUY
    {
        eDIALOG_CASH_ITEM_SHOP_BUY_BTN_EXIT = 0,

        eDIALOG_CASH_ITEM_SHOP_BUY_IMG_ITEM,
		
		eDIALOG_CASH_ITEM_SHOP_BUY_LST_DESC,

		eDIALOG_CASH_ITEM_SHOP_BUY_TIP_LST_DESC,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC1,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC2,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC3,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC4,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC5,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC6,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC7,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC8,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC9,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC10,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC11,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_DESC12,
		
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_SALE1,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_SALE2,	
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_SALE3,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_SALE4,

		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_NAME,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_ITEM_COST_MONEY,
		eDIALOG_CASH_ITEM_SHOP_BUY_TXT_MONEY,

		eDIALOG_CASH_ITEM_SHOP_BUY_CHK_SEL1,
		eDIALOG_CASH_ITEM_SHOP_BUY_CHK_SEL2,
		eDIALOG_CASH_ITEM_SHOP_BUY_CHK_SEL3,
		eDIALOG_CASH_ITEM_SHOP_BUY_CHK_SEL4,

		eDIALOG_CASH_ITEM_SHOP_BUY_BTN_GIFT,
		eDIALOG_CASH_ITEM_SHOP_BUY_BTN_CART,
		eDIALOG_CASH_ITEM_SHOP_BUY_BTN_BUY,

		eDIALOG_CASH_ITEM_SHOP_BUY_VSCR,

		eDIALOG_CASH_ITEM_SHOP_BUY_MAX,
    };

	enum eCASH_ITEM_SHOP_BUY_TXT_DESC
	{
		eCASH_ITEM_SHOP_BUY_TXT_DESC1,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC2,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC3,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC4,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC5,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC6,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC7,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC8,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC9,	
		eCASH_ITEM_SHOP_BUY_TXT_DESC10,
		eCASH_ITEM_SHOP_BUY_TXT_DESC11,
		eCASH_ITEM_SHOP_BUY_TXT_DESC12,
		
		eCASH_ITEM_SHOP_BUY_TXT_DESC_MAX,
	};

	enum eCASH_ITEM_SHOP_BUY_TXT
	{
		eCASH_ITEM_SHOP_BUY_TXT_NAME,
		eCASH_ITEM_SHOP_BUY_TXT_ITEM_COST_MONEY,
		eCASH_ITEM_SHOP_BUY_TXT_MONEY,
		
		eCASH_ITEM_SHOP_BUY_TXT_MAX,
	};

	enum eCASH_ITEM_SHOP_BUY_CHK
	{
		eCASH_ITEM_SHOP_BUY_CHK_SEL1,
		eCASH_ITEM_SHOP_BUY_CHK_SEL2,
		eCASH_ITEM_SHOP_BUY_CHK_SEL3,
		eCASH_ITEM_SHOP_BUY_CHK_SEL4,

		eCASH_ITEM_SHOP_BUY_CHK_MAX,
	};

public:
    uiCashItemShopBuy(InterfaceManager *pUIMan);
    virtual ~uiCashItemShopBuy();

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
	void					_addIconImage(DWORD dwCode, CODETYPE itemCode);
	void					_removeIconImage();
	
	void					_renderTooltip();
	void					_renderIconImage();
public:

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
    static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_BUY_MAX];

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
	
	CCtrlStaticWZ*			m_pTxtCtrlDesc[_MAX_PRICE_INFO][eItemDescMax];
	CCtrlStaticWZ*			m_pTxtCtrlSale[_MAX_PRICE_INFO];

	CCtrlStaticWZ*			m_pTxtCtrl[eCASH_ITEM_SHOP_BUY_TXT_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckTabs[eCASH_ITEM_SHOP_BUY_CHK_MAX];

	CCtrlButtonWZ*			m_pImgCtrl;

	CCtrlListWZ*            m_pListDesc;

	CCtrlListWZ*            m_pTipListDesc;

	CCtrlVScrollWZ *		m_pScroll;

	bool					m_bShow;
    bool                    m_bControlsInitialized;

	stRenderTooltipInfo		m_ItemCodeForTooltip;
	CASH_ITEM_IMG			m_BuyItemImg;

	stCashItemInfo			m_BuyCashItemInfo;
	stCashItemInfo			m_BuyCashItemCheckInfo;
	stPresentItemInfo		m_PresentItemCheckInfo;

	MONEY					total_cash_;
	MONEY					total_heim_;
	int						m_iTipListCurLine;				//	팁 리스트의 현재 스크롤 라인 위치
	int						m_iSelectIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopBuy::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_BUY_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return eDIALOG_CASH_ITEM_SHOP_BUY_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopBuy::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_BUY_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopBuy::GetManager()
{
    return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

