#pragma once


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


//------------------------------------------------------------------------------
//
//    ÀÛ¾÷ÀÚ: ±èÅÂ¿ì
//    uiCashItemShopExtensionComplete class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopExtensionComplete: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE
	{
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_BTN_EXIT = 0,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_BTN_OK,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_DESC,

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_COST,	// ºñ¿ë
		eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_REMAIN, // ÀÜ¾×

		eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_MAX,
	};

public:
	uiCashItemShopExtensionComplete(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopExtensionComplete();

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
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_MAX];

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

	CCtrlStaticWZ*			m_pTxtCtrlDesc;

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
uiCashItemShopExtensionComplete::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopExtensionComplete::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopExtensionComplete::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

