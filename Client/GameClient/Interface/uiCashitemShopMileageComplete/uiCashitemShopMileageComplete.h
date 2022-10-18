#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    ÀÛ¾÷ÀÚ: ±èÅÂ¿ì
//    uiCashItemShopMileageComplete class
//------------------------------------------------------------------------------

#include "uibase.h"

#ifdef __NA_00387_20070605_SHOP_MILEAGE2

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopMileageComplete: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE
	{
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_EXIT = 0,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_OK,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_LST,
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_VSCR,

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_COST,	// ºñ¿ë
		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_REMAIN, // ÀÜ¾×

		eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_MAX,
	};

public:
	uiCashItemShopMileageComplete(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopMileageComplete();

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
	uiCashItemShopMan*             GetManager();

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
	void					ShowBuyComplete(stCashItemInfo* pInfo);

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_MAX];

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

	CCtrlStaticWZ*			m_pTxtCtrlCost;
	CCtrlStaticWZ*			m_pTxtCtrlRemain;

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stCashItemInfo			m_DisplayItemInfo;

	BYTE					m_moneyType;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopMileageComplete::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopMileageComplete::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopMileageComplete::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
