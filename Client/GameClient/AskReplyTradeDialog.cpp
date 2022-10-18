//------------------------------------------------------------------------------
//  AskReplyTradeDialog.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "AskReplyTradeDialog.h"
#include "GameFramework.h"
#include "interfacemanager.h"
#include "uiTradeMan/uiTradeMan.h"
#include "globalfunc.h"
#include "uiVendorMan/uiVendorMan_def.h"
//------------------------------------------------------------------------------
/**
*/
AskReplyTradeDialog::AskReplyTradeDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
AskReplyTradeDialog::~AskReplyTradeDialog(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void AskReplyTradeDialog::OnShowWindow(BOOL val)
{
    AskDialog::OnShowWindow(val);

    if (val)
    {
        //MoveWindowWZ(0.0f, 0.0f);
		SolarDialog::MoveCenter();

        bDestroy = true;
        bDestroyTimer = false;
        dwDestroyTick = 0;
        dwDestroyTickOver = 0;

		GET_DIALOG(IM_VENDOR_MANAGER::VENDOR_SELL)->ShowWindow(FALSE);		
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskReplyTradeDialog::Process(DWORD dwTick)
{
    if (this->GetStatus() != ASK_STATUS_NORMAL)
        return;

    if (bDestroyTimer)
    {
        dwDestroyTick = clock_function::GetTickCount();

        if (dwDestroyTick > dwDestroyTickOver)
        {
            ShowInterface(FALSE);

            this->m_status = ASK_STATUS_CANCEL;

            uiTradeMan* tradeMan =
                static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
            tradeMan->SendTradeReply(false);

            bDestroyTimer = false;
        }
    }

    if (bDestroy)
    {
        dwDestroyTick = clock_function::GetTickCount();
        dwDestroyTickOver = clock_function::GetTickCount() + (15*1000);
        bDestroy = false;
        bDestroyTimer = true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void AskReplyTradeDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case ASK_OK:
                {
                    this->m_status = ASK_STATUS_OK;
                }
                break;

            case ASK_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;
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
void AskReplyTradeDialog::OnUpdateSolarDialog()
{
    CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( ASK_LIST );
    pLCtrl->DeleteItemAll();
    pLCtrl->SetBeginDrawIndex(0);
    pLCtrl->SetFontID(StrToWzID("mn12"));

    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    int idx;

    //	귀하에게 거래를 요청했습니다.
    g_InterfaceManager.GetInterfaceString( eST_NAMECODE_TRADE_YOU, szMessage, INTERFACE_STRING_LENGTH );
    pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
    idx = pLCtrl->InsertItemBack();
	GlobalFunc::SetItemText( pLCtrl, idx, szMessage );
    pLCtrl->SetItemFontID( idx, 0, StrToWzID("mn12"));
    pLCtrl->SetItemBKColor( idx, 0, WzColor_RGBA(0,0,0,0));
    pLCtrl->SetItemTextColor( idx, 0, WzColor_RGBA( 152, 208, 142, 255 ));

    //	수락하시겠습니까?
    g_InterfaceManager.GetInterfaceString( eST_ACCEPT_INVITE, szMessage, INTERFACE_STRING_LENGTH );
    pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
    idx = pLCtrl->InsertItemBack();
	GlobalFunc::SetItemText( pLCtrl, idx, szMessage );
    pLCtrl->SetItemFontID( idx, 0, StrToWzID("mn12"));
    pLCtrl->SetItemBKColor( idx, 0, WzColor_RGBA(0,0,0,0));
    pLCtrl->SetItemTextColor( idx, 0, WzColor_RGBA( 152, 208, 142, 255 ));

    //	남은 시간.... ( %d )
    g_InterfaceManager.GetInterfaceString( eST_REMAIN_TIME, szText, INTERFACE_STRING_LENGTH );
    Sprintf(szMessage, szText, ((dwDestroyTickOver - dwDestroyTick)/1000)+1);
    pLCtrl->SetColorText(WzColor_RGBA( 152, 208, 142, 255 ));
    idx = pLCtrl->InsertItemBack();
	GlobalFunc::SetItemText( pLCtrl, idx, szMessage );
    pLCtrl->SetItemFontID( idx, 0, StrToWzID("mn12"));
    pLCtrl->SetItemBKColor( idx, 0, WzColor_RGBA(0,0,0,0));
    pLCtrl->SetItemTextColor( idx, 0, WzColor_RGBA( 152, 208, 142, 255 ));

    pLCtrl->SetUnuseSelBar(TRUE);
    pLCtrl->OnFocusOut();

    uiTradeMan* tradeMan =
        static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));

    if (this->GetStatus() == ASK_STATUS_OK)
    {
        tradeMan->SendTradeReply(true);
		ShowInterface(FALSE);
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        tradeMan->SendTradeReply(false);
		ShowInterface(FALSE);
    }
}


