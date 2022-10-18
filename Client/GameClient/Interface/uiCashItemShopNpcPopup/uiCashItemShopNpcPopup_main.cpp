#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopNpcPopup.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "GameFramework.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopNpcPopup::m_wzId[eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_MAX] = 
{
        StrToWzID("B999"),	//eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_EXIT,	// X 버튼

		StrToWzID("B001"),	//eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_SHOP,	// 유료상점
		StrToWzID("B002"),	//eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_GIFT,	// 선물

        StrToWzID("L000"),	//eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_LST_STRING,	// NPC 메시지
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopNpcPopup::uiCashItemShopNpcPopup(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pCashItemShopMan =
        static_cast<uiCashItemShopMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
    assert (m_pCashItemShopMan);

    if (!this->m_pCashItemShopMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pCashItemShopMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	this->m_pListCtrl = NULL;
	m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopNpcPopup::~uiCashItemShopNpcPopup()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopNpcPopup::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopNpcPopup::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopNpcPopup::InitControls()
{
	CCtrlButtonWZ* pBtn = static_cast<CCtrlButtonWZ *>(this->getControl(eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_GIFT));
	assert(pBtn);
	if(pBtn)
		pBtn->ShowWindowWZ(WZ_HIDE);	// 선물하기는 아직 기능이 없다

	this->m_pListCtrl = static_cast<CCtrlListWZ *>(this->getControl(eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_LST_STRING));
	assert(this->m_pListCtrl);

	_setNpcMsg();

    this->m_bControlsInitialized = true;
}

//------------------------------------------------------------------------------
void uiCashItemShopNpcPopup::_setNpcMsg()
{
	if(this->m_pListCtrl)
	{
		std::vector<util::_tstring> vstrNpcMsg;
		vstrNpcMsg.clear();
		TCHAR szNpcMsg[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(201071, szNpcMsg, INTERFACE_STRING_LENGTH);	

		if(!_tcslen(szNpcMsg))
			return;

		RECT rc = m_pListCtrl->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int iListWidth = rc.right - rc.left - 20;
#else
		int iListWidth = rc.right - rc.left - 10;
#endif
		GlobalFunc::ProcessWordWrapByFontSize( &vstrNpcMsg, szNpcMsg, c_wiCashItemShopFont, _tcslen(szNpcMsg), iListWidth );

		m_pListCtrl->DeleteItemAll();
		m_pListCtrl->SetColorSelBar(c_CashItemShopListSelBarColor);
		m_pListCtrl->SetUseReport(false);
		int iListIndex = -1;

		for (size_t i=0; i<vstrNpcMsg.size(); ++i)
		{
			iListIndex = m_pListCtrl->InsertItemBack();

			TCHAR *pStr = const_cast<TCHAR *>(vstrNpcMsg[i].c_str());
			GlobalFunc::SetItem(
					m_pListCtrl, 
					iListIndex, 0, pStr, 
					c_wiCashItemShopFont,
					DT_VCENTER | DT_LEFT, 
					c_CashItemShopListBKColor, 
					c_CashItemShopListTextColor, 
					c_CashItemShopListSelTextColor);
		}   
	}
}

//------------------------------------------------------------------------------
void uiCashItemShopNpcPopup::OnShowWindow( BOOL val )
{
    if (val)
    {
         this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_cash_item_shop_npc_popup;
        msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_NPC_POPUP;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		GetManager()->ShowCashItemShopDlg(false);

		GetManager()->ShowCashItemShopBuyCart(false);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_npc_popup);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopNpcPopup::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopNpcPopup::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	//TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_EXIT:
                {
					m_bShow = false;
                    
                }
                break;

			case eDIALOG_CASH_ITEM_SHOP_NPC_POPUP_BTN_SHOP:
				{
					m_bShow = false;

					// 유료상점창 Show
					this->GetManager()->ShowCashItemShopDlg(TRUE);

				}
				break;

            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopNpcPopup::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

    if (!_isEnable())
        return;
}


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
