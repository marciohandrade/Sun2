
#pragma once

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 



#ifdef _UICASHITEMPURCHASEDLG_H
#else
#define _UICASHITEMPURCHASEDLG_H
//------------------------------------------------------------------------------
//
//    작업자: 이도형
//    uiCashItemPurchaseDlg class
//------------------------------------------------------------------------------

#include "uibase.h"
#include "uiCashItemShopMan/uiCashItemShopDef.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"

class uiCashItemShopMan;
class uiCashItemPurchaseDlg: public uiBase
{
    enum DIALOG_CASH_ITEM_PURCHASE_DLG
    {	
        DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_EXIT,
        DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_CANCLE,
        DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_CCOIN,
        DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_PPCOIN,
        DIALOG_CASH_ITEM_PURCHASE_DLG_MAX,
    };
public:
    uiCashItemPurchaseDlg(InterfaceManager *ui_manager);
    virtual ~uiCashItemPurchaseDlg();
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
protected:
	virtual void	        OnShowWindow(BOOL value);
	virtual void            OnUpdateSolarDialog();
protected:
    CControlWZ* 	        getControl(POSTYPE at_position);
    POSTYPE			        getCtrlIDToPos(WzID control_id);
    static WzID		        controls_id_[DIALOG_CASH_ITEM_PURCHASE_DLG_MAX];
    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc(SI_MESSAGE* message);   // Pure
	virtual void			MouseOver_MsgProc(SI_MESSAGE* message);
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiCashItemShopMan*		cash_item_shop_manager_;
};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiCashItemPurchaseDlg::getCtrlIDToPos(WzID control_id)
{
    for(POSTYPE i = 0; i < DIALOG_CASH_ITEM_PURCHASE_DLG_MAX; ++i)
    {
        if(controls_id_[i] == control_id)
        {
            return i;
        }
    }
    return DIALOG_CASH_ITEM_PURCHASE_DLG_MAX;
}
//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* uiCashItemPurchaseDlg::getControl(POSTYPE at_position)
{
    ASSERT(at_position < DIALOG_CASH_ITEM_PURCHASE_DLG_MAX);

    return GetControlWZ(controls_id_[at_position]);
}
//------------------------------------------------------------------------------
/**
*/
inline uiCashItemShopMan* uiCashItemPurchaseDlg::GetManager()
{
    return this->cash_item_shop_manager_;
}
//------------------------------------------------------------------------------
#endif//_UICASHITEMPURCHASEDLG_H


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

