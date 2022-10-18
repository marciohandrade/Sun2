#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCashItemShopNpcPopup class
//------------------------------------------------------------------------------

#include "uibase.h"



#include "uiCashItemShopMan/uiCashItemShopDef.h"

class uiCashItemShopMan;

class uiCashItemShopNpcPopup: public uiBase
{
    enum eDIALOG_CASH_ITEM_SHOP_NPC_POPUP
    {
		eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_EXIT,		// X 버튼

		eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_SHOP,		// 유료상점
		eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_GIFT,		// 선물

        eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_LST_STRING,	// NPC 메시지
        
		eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_MAX,
    };


public:
    uiCashItemShopNpcPopup(InterfaceManager *pUIMan);
    virtual ~uiCashItemShopNpcPopup();

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
	void					_setNpcMsg();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_MAX];

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
	CCtrlListWZ*			m_pListCtrl;

	bool					m_bShow;
    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCashItemShopNpcPopup::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCashItemShopNpcPopup::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCashItemShopMan*
uiCashItemShopNpcPopup::GetManager()
{
    return this->m_pCashItemShopMan;
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
