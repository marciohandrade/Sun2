#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiCashItemShopMileageCheck class
//------------------------------------------------------------------------------

#include "uibase.h"

#ifdef __NA_00387_20070605_SHOP_MILEAGE2

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopMileageCheck: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK
	{
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_EXIT = 0,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_OK,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_CANCEL,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_LST,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_LST_ALERT,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR_ALERT,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_TXT_MONEY,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_MAX,
	};


public:
	uiCashItemShopMileageCheck(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopMileageCheck();

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
	uiCashItemShopMan*      GetManager();

	//--------------------------------------------------------------------------
	// functions
	//
private:
	BOOL					_isEnable();
	void					_refresh();

public:

	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	void					ShowBuyItemCheck(stCashItemInfo* pInfo);


protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_MAX];

	//--------------------------------------------------------------------------
	// procedure
	//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

	//--------------------------------------------------------------------------
	// member vars
	//
private:
	uiCashItemShopMan*		m_pCashItemShopMan;

	CCtrlListWZ*            m_pListItem;
	CCtrlVScrollWZ*			m_pScroll;

	CCtrlListWZ*            m_pListAlert;
	CCtrlVScrollWZ*			m_pScrollAlert;

	CCtrlStaticWZ*			m_pTxtCtrlMoney;

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stCashItemInfo			m_BuyCashItemCheckInfo;

	int						m_iTotalPage;
	int						m_iTotalPageAlert;

};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopMileageCheck::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopMileageCheck::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopMileageCheck::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
