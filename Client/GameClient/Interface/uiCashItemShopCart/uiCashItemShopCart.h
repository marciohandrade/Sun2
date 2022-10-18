#pragma once


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCashItemShopCart class
//------------------------------------------------------------------------------

#include "uibase.h"



#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopCart: public uiBase
{
    enum eDIALOG_CASH_ITEM_SHOP_CART
    {
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_EXIT = 0,	

		eDIALOG_CASH_ITEM_SHOP_CART_CHK_CASH_ITEM,	
		eDIALOG_CASH_ITEM_SHOP_CART_CHK_MY_PAGE,	

		eDIALOG_CASH_ITEM_SHOP_CART_CHK_CART,	
		eDIALOG_CASH_ITEM_SHOP_CART_CHK_GIFT,		
		
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY1,	
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY2,	
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY3,	

		eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL1,	
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL2,	
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL3,	

		eDIALOG_CASH_ITEM_SHOP_CART_BTN_DEL_ALL,	
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY_SEL,	
		eDIALOG_CASH_ITEM_SHOP_CART_BTN_BUY_ALL,	

		eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL1,	
		eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL2,	
		eDIALOG_CASH_ITEM_SHOP_CART_CHK_SEL3,	

		eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM1,	
		eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM2,	
		eDIALOG_CASH_ITEM_SHOP_CART_IMG_ITEM3,	

		eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM1,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM2,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_ITEM3,	

		eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM1,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM2,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_MEMO_ITEM3,
	      
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC1,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC2,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC3,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_1_DESC4,	
		
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC1,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC2,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC3,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_2_DESC4,	
		
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC1,
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC2,
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC3,
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_3_DESC4,
		
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_HEIM,	
		eDIALOG_CASH_ITEM_SHOP_CART_TXT_CASH,	

		eDIALOG_CASH_ITEM_SHOP_CART_VSCR,	

		
        eDIALOG_CASH_ITEM_SHOP_CART_MAX,
    };

    enum eCASH_ITEM_SHOP_CART_CHK
    {
        eCASH_ITEM_SHOP_CART_CHK_CASH_ITEM,	
		eCASH_ITEM_SHOP_CART_CHK_MY_PAGE,

		eCASH_ITEM_SHOP_CART_CHK_MAX,
    };

	enum eCASH_ITEM_SHOP_CART_CHK_MENU
    {
		eCASH_ITEM_SHOP_CART_CHK_CART,	
		eCASH_ITEM_SHOP_CART_CHK_GIFT,

		eCASH_ITEM_SHOP_CART_CHK_MENU_MAX,
	};

	enum eCASH_ITEM_SHOP_CART_BTN_BUY
	{
		eCASH_ITEM_SHOP_CART_BTN_BUY1,	
		eCASH_ITEM_SHOP_CART_BTN_BUY2,	
		eCASH_ITEM_SHOP_CART_BTN_BUY3,	

		eCASH_ITEM_SHOP_CART_BTN_BUY_MAX,
	};

	
	enum eCASH_ITEM_SHOP_CART_ITEM
	{
		eCASH_ITEM_SHOP_CART_ITEM1,
		eCASH_ITEM_SHOP_CART_ITEM2,
		eCASH_ITEM_SHOP_CART_ITEM3,
		
		eCASH_ITEM_SHOP_CART_ITEM_MAX,
	};

	enum eCASH_ITEM_SHOP_CART_TXT_DESC
	{
		eCASH_ITEM_SHOP_CART_TXT_DESC1,	
		eCASH_ITEM_SHOP_CART_TXT_DESC2,	
		eCASH_ITEM_SHOP_CART_TXT_DESC3,	
		eCASH_ITEM_SHOP_CART_TXT_DESC4,	
		
		eCASH_ITEM_SHOP_CART_TXT_DESC_MAX,
	};

public:
    uiCashItemShopCart(InterfaceManager *pUIMan);
    virtual ~uiCashItemShopCart();

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
	
	void					_setMenu(int iMenu);
	int						_getMenu()				{ return m_iCurMenu;	}
	void					_updateMenu();

	void					_prepareShow();
	
	void					_clearItem(int index);
	void					_clearAllItem();
	void					_setItem(int index, CODETYPE itemCode);
	
	void					_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode);
	void					_removeIconImage(int iIndex);
	void					_removeAllIconImage();

	void					_renderTooltip();
	void					_renderIconImage();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_CART_MAX];

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

	CCtrlButtonCheckWZ*		m_pBtnCheckMenu[eCASH_ITEM_SHOP_CART_CHK_MENU_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckTabs[eCASH_ITEM_SHOP_CART_CHK_MAX];
	CCtrlImageStaticWZ*		m_pImgCtrl[eCASH_ITEM_SHOP_CART_ITEM_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckSel[eCASH_ITEM_SHOP_CART_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlName[eCASH_ITEM_SHOP_CART_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlMemo[eCASH_ITEM_SHOP_CART_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlSale[eCASH_ITEM_SHOP_CART_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlDesc1[eCASH_ITEM_SHOP_CART_TXT_DESC_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlDesc2[eCASH_ITEM_SHOP_CART_TXT_DESC_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlDesc3[eCASH_ITEM_SHOP_CART_TXT_DESC_MAX];

	CCtrlStaticWZ*			m_pTxtCtrlHeim;
	CCtrlStaticWZ*			m_pTxtCtrlCash;

	CCtrlVScrollWZ*			m_pScroll;

	CASH_ITEM_IMG			m_CartItemImg[eCASH_ITEM_SHOP_CART_ITEM_MAX];

	bool					m_bShow;
    bool                    m_bControlsInitialized;

	stRenderTooltipInfo		m_ItemCodeForTooltip;

	int						m_iCurMenu;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopCart::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_CART_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return eDIALOG_CASH_ITEM_SHOP_CART_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopCart::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_CART_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopCart::GetManager()
{
    return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

