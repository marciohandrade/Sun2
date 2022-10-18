#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: KTW
//    uiCashItemShopMileageDlg Class
//------------------------------------------------------------------------------

#include "uibase.h"

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopMileageDlg: public uiBase, public uiCashItemShopBase
{
	enum eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG
	{
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_EXIT = 0,	

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_CASH_ITEM,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_MY_PAGE,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_PACKAGE,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_MILEAGE,	

        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_TOTAL,	
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_ARMOR,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_SKIN,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_CUSTOMIZE,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_ITEM,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_PET,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_RIDER,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_PC,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_EXTENSION,		//	기간연장
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_BUY_WCOIN,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY1,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY2,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY3,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY4,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY5,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY6,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY7,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY8,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY9,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_BUY10,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM1,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM2,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM3,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM4,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM5,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM6,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM7,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM8,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM9,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_ITEM10,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_MILEGE,			//	해방군의 훈장 그림 버튼

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM1,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM2,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM3,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM4,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM5,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM6,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM7,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM8,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM9,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_ITEM10,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM1,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM2,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM3,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM4,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM5,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM6,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM7,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM8,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM9,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_MEMO_ITEM10,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_TXT_MONEY,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_TXT_MILEGE,			//	마일리지

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_PREV_PAGE,	
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_NEXT_PAGE,	

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_TXT_CURR_PAGE,

    	eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_WCOIN_BACK,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_WCOINC_BACK,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_IMG_WCOINP_BACK,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_STXT_WCOINC,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_STXT_WCOINP,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_STXT_MILEAGE,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_TXT_WCOINC,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_TXT_WCOINP,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_WCOINBUY,

        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_BTN_SORTCLASS,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_SORTCLASS,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_TEXT_SORTCLASS,
        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_CHK_PPCARD,	

        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_LIST_SUBMENU,


		eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_MAX,

	};

	enum eCASH_ITEM_SHOP_MILEAGE_DLG_CHK
	{
		eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_CASH_ITEM,
		eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MY_PAGE,	
		eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_PACKAGE,
		eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MILEAGE,

		eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MAX,
	};
public:
	enum eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MENU
	{
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_TOTAL,	
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_ARMOR,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_SKIN,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_CUSTOMIZE,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_ITEM,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_PET,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_RIDER,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_PC,
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_EXTENSION,		//	기간연장
        eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_BUY_WCOIN,

		eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MENU_MAX,  
	};
private:
	enum eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM
	{
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM1,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM2,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM3,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM4,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM5,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM6,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM7,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM8,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM9,
		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM10,

		eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX,
	};

    enum eCASH_ITEM_SHOP_MILEAGE_TXT
	{
        eCASH_ITEM_SHOP_MILEAGE_STXT_WCOINP,
        eCASH_ITEM_SHOP_MILEAGE_STXT_WCOINC,
        eCASH_ITEM_SHOP_MILEAGE_STXT_MILEAGE,
        eCASH_ITEM_SHOP_MILEAGE_TXT_WCOINP,
        eCASH_ITEM_SHOP_MILEAGE_TXT_WCOINC,
        eCASH_ITEM_SHOP_MILEAGE_TXT_MAX,
	};
    enum eCASH_ITEM_SHOP_PACKAGE_IMG
	{
        eCASH_ITEM_SHOP_MILEAGE_BTN_WCOIN_BACK, 
        eCASH_ITEM_SHOP_MILEAGE_IMG_WCOINC_BACK,
        eCASH_ITEM_SHOP_MILEAGE_IMG_WCOINP_BACK,
        eCASH_ITEM_SHOP_IMG_MAX,
	};

public:
	uiCashItemShopMileageDlg(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopMileageDlg();

	//--------------------------------------------------------------------------
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

	void					_setMenu(int iMenu, int iSubMenu = 0, bool force_change = false);
	int						_getMenu()				{ return m_iCurMenu;	}
	void					_updateMenu();

	void					_setPage(int iPage);
	void					_setIndex(int iIndex);
	void					_prepareShow();

	void					_clearItem(int index);
	void					_clearAllItem();
	void					_setItem(int index, CODETYPE itemCode);

	void					_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode);
	void					_removeIconImage(int iIndex);
	void					_removeAllIconImage();

	void					_renderTooltip();
	void					_renderIconImage();
	BYTE					_getCategoryType(int iMenu, int iSubMenu = 0);

public:
	void					SetItem(int index, CODETYPE itemCode);
	void					SetCurPage( void );
	void					RefreshItemPrice();
	void					RefreshSingleItemList();
	void					RefreshItemList();
	void					SetCurMoney( void );

	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	virtual int				GetCurPage()	{return m_iCurPage;}
	virtual int				GetCurMenu()	{return m_iCurMenu;}
	virtual int				GetDlgCheckMenuMax()	{return eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MENU_MAX;}
	virtual BYTE			GetCurCategoryType()	{return _getCategoryType(m_iCurMenu);}
protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();
	virtual void            OnRenderSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );
    void					SetSelectedIndex(int iIndex){ _setIndex(iIndex); }

private:
    void OnClickCombo(eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG e);
    bool OnSelectedCombo(eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG list_control_index, 
                        eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG static_control_index, 
                        int selected_index);

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	uiCashItemShopMan*		m_pCashItemShopMan;

	CCtrlButtonCheckWZ*		m_pBtnCheckMenu[eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MENU_MAX];
	CCtrlButtonCheckWZ*		m_pBtnCheckTabs[eCASH_ITEM_SHOP_MILEAGE_DLG_CHK_MAX];
	CCtrlImageStaticWZ*		m_pImgCtrl[eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX];
	CCtrlListWZ*			m_pListCtrlName[eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX];
	CCtrlListWZ*			m_pListCtrlMemo[eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX];
	CCtrlStaticWZ*			m_pTxtCtrlCurPage;

	CCtrlStaticWZ*			m_pTxtCtrlSale[eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX][_MAX_PRICE_INFO];
	CCtrlStaticWZ*			m_pTxtCtrlDesc[eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX][_MAX_PRICE_INFO][eItemDescMax];
	CCtrlStaticWZ*			m_pTxtCtrlMoney;	// 테섭에선 하임으로, 나머지는 캐시로..
	CCtrlStaticWZ*			m_pTxtCtrlMilege;	// 마일리지
	CCtrlVScrollWZ*			m_pScroll;

    CCtrlStaticWZ*			m_pTxtCtrlPPCoin;	// PP캐시
    CCtrlStaticWZ*			m_pTxtCtrlCCCoin;	// 일반캐시

	CASH_ITEM_IMG			m_CashItemImg[eCASH_ITEM_SHOP_MILEAGE_DLG_ITEM_MAX];

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stRenderTooltipInfo		m_ItemCodeForTooltip;

public:
	int						m_iCurMenu;
    int						m_iCurSubMenu;
	int						m_iCurPage;
	int						m_iCurIndex;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopMileageDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopMileageDlg::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_MILEAGE_DLG_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopMileageDlg::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
