#include "SunClientPrecompiledHeader.h"


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemPurchaseDlg.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "interfacemanager.h"
#include "GlobalFunc.h"
#include "uiSystemMan/uiSystemMan.h"
//------------------------------------------------------------------------------
WzID uiCashItemPurchaseDlg::controls_id_[DIALOG_CASH_ITEM_PURCHASE_DLG_MAX] = 
{
	StrToWzID("B999"), //	DIALOG_CASH_ITEM_SHOP_DLG_BTN_EXIT = 0,	
    StrToWzID("B002"), //   DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_CANCLE,
    StrToWzID("B003"), //   DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_CCOIN,
    StrToWzID("BT00"), //   DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_PPCOIN,
};
//------------------------------------------------------------------------------
/**
*/
uiCashItemPurchaseDlg::uiCashItemPurchaseDlg(InterfaceManager *ui_manager) : uiBase(ui_manager)
{
    this->cash_item_shop_manager_ = static_cast<uiCashItemShopMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
    assert(cash_item_shop_manager_);

    if (!this->cash_item_shop_manager_)
    {
#ifdef _DEBUG
		 uiSystemMan* system_manager = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(system_manager)
				system_manager->Sys_VerifyLock(_T("m_pCashItemShopMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }
}
//------------------------------------------------------------------------------
/**
*/
uiCashItemPurchaseDlg::~uiCashItemPurchaseDlg()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemPurchaseDlg::Init(CDrawBase* drawbase)
{
    uiBase::Init(drawbase);
    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemPurchaseDlg::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemPurchaseDlg::InitControls()
{
}

//------------------------------------------------------------------------------
void uiCashItemPurchaseDlg::OnShowWindow(BOOL value)
{
    if (value)
    {
		this->MoveDefaultWindowPos();
        keyMsg message;
        ZeroMemory(&message,sizeof(message));
        message.dwType = eDoType_dialog_cash_item_purchase;
        message.wParam = InterfaceManager::DIALOG_CASH_ITEM_PUSCHASE_DLG;
        message.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(message);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_purchase);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemPurchaseDlg::MessageProc(SI_MESSAGE* message)
{
	switch (message->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
			int	iCtrlID = getCtrlIDToPos(message->dwCtrlID);
			switch (iCtrlID)
            {
            case DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_EXIT:
            case DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_CANCLE:
                {
                    ShowWindow(FALSE);
                }
                break;
            case DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_CCOIN:
                {
                    if (GetManager())
                    {
                        GetManager()->SetUseCashType(CASH_ITEM_SHOP_CCASH);
                        GetManager()->OpenBuyWindowFromCashType();
                    }
                    ShowWindow(FALSE);
                }
                break;
            case DIALOG_CASH_ITEM_PURCHASE_DLG_BTN_PPCOIN:
                {
                    if (GetManager())
                    {
                        GetManager()->SetUseCashType(CASH_ITEM_SHOP_PCASH);
                        GetManager()->OpenBuyWindowFromCashType();
                    }
                    ShowWindow(FALSE);
                }
                break;
            }
        }
	default:
		{
			SolarDialog::MessageProc(message);
		}
		break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemPurchaseDlg::OnUpdateSolarDialog()
{
	if (!IsVisible())
    {
		this->ShowInterface(FALSE);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemPurchaseDlg::MouseOver_MsgProc(SI_MESSAGE* message)
{
	switch (message->eResultMsg) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE controls_position = getCtrlIDToPos(message->dwCtrlID);
		}
		break;
	}
}


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

