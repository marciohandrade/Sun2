#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    �۾���: ���¿�
//    uiCashItemShopPackageCheck class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopPackageCheck: public uiBase
{
	enum eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK
	{
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_EXIT = 0,			//	X ��ư

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_LST_DESC,			//	��Ű�� ��� ����Ʈ

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_VSCR,				//  ��Ű�� ��� ��ũ��

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_TXT_PRICE,			//	��Ű�� ����

		
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_BTN_OK,			//	Ȯ�� ��ư
		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_BTN_CANCLE,		//	��� ��ư

		eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_MAX,
	};

public:
	uiCashItemShopPackageCheck(InterfaceManager *pUIMan);
	virtual ~uiCashItemShopPackageCheck();

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
	void					ShowPackageCheck(stPackageItemInfo* pInfo);

protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnUpdateSolarDialog();

protected:
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );
	static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_MAX];

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

	CCtrlListWZ*            m_pListDesc;

	CCtrlVScrollWZ *		m_pScroll;

	CCtrlStaticWZ*			m_pTxtCtrlPrice;

	bool					m_bShow;
	bool                    m_bControlsInitialized;

	stPackageItemInfo		m_PackageItemInfo;
};


//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopPackageCheck::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopPackageCheck::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_PACKAGE_CHECK_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopPackageCheck::GetManager()
{
	return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
