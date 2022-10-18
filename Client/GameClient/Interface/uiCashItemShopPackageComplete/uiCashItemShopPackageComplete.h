#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 김태우
//    uiCashItemShopPackageComplete class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPackageComplete: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE
	{
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_EXIT = 0,			//	X 버튼

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_TXT_PRICE,			//	패키지 가격

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_TXT_MYMOMEY,		//	현재 보유 금액

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_BTN_OK,			//	확인 버튼

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_MAX,
	};

public:
	uiCashItemShopPackageComplete(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopPackageComplete();

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

	//--------------------------------------------------------------------------
	// dialog functions
	//
public:
	void					ShowPackageComplete(stPackageItemInfo* pInfo);

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();

	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_MAX];

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

	CCtrlStaticWZ*			m_pTxtCtrlPrice;
	CCtrlStaticWZ*			m_pTxtCtrlMyMoney;

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stPackageItemInfo		m_PackageItemInfo;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopPackageComplete::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopPackageComplete::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_MAX);

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopPackageComplete::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
