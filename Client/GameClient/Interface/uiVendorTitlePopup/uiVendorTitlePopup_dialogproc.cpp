//------------------------------------------------------------------------------
//  uiVendorTitlePopup_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan/uiVendorMan.h"
#include "uiVendorTitlePopup.h"
#include "ItemUnitRenderer.h"
#include "MouseHandler.h"
#include "InventoryDialog.h"
#include "SCItemSlot.h"
#include "ItemInfoParser.h"
#include "ItemFormula.h"
#include "uiVendorSell/uiVendorSell.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
WzID uiVendorTitlePopup::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("T001"),  // DIALOG_TXT_HELP_PLZ_INPUT=0,
        StrToWzID("TI00"),  // DIALOG_BTN_OK,
        StrToWzID("TI01"),  // DIALOG_BTN_CANCEL,
        StrToWzID("E000"),  // DIALOG_EDIT_TITLE_MESSAGE,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiVendorTitlePopup::OnShowWindow( BOOL val )
{
    //this->m_bShow = val;

	uiBase *pVendorSell = GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL );

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    if (val)
    {
        RECT rc;
        GetDialogWZ()->GetDlgSize(&rc);

        long width = rc.right;
        long height = rc.bottom;

        // TODO:
        // 전체 화면 해상도를 참조해서 다이얼로그를 화면 중앙에 위치시킨다.
        // ex) 1024x768

        long mw = width >> 1;
        long mh = height >> 1;

        RECT *rcWindow = g_InterfaceManager.GetWindowRC();

        long sw;
        long sh;
        if (rcWindow)
        {
            sw = (rcWindow->right - rcWindow->left);
            sh = (rcWindow->bottom - rcWindow->top);
        }
        else
        {
            sw = 1024; sh = 768;
        }

        long pw = (sw >> 1) - mw;
        long ph = 50;

        GetDialogWZ()->MoveWindowWZ((float)pw, (float)ph);

        CCtrlDxIMEEditBox * m_pChatEditCtrl;
        m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( DIALOG_EDIT_TITLE_MESSAGE );
        m_pChatEditCtrl->ClearText();
		GlobalFunc::SetFocus( (SolarDialog *)this, m_pChatEditCtrl );

        m_pChatEditCtrl->SetColorCaret(RGBA(255,100,100,255));
        m_pChatEditCtrl->SetColorText(RGBA(255,255,255,255));
        m_pChatEditCtrl->SetFontID(StrToWzID("st10"));

        CCtrlStaticWZ* m_pCancelTxtControl =
            (CCtrlStaticWZ *)getControl (DIALOG_BTN_CANCEL);

		g_InterfaceManager.GetInterfaceString( eST_CANCEL, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::SetCaption( m_pCancelTxtControl, szMessage );

        this->m_status = ASK_STATUS_NORMAL;
        this->m_bFocus = true;

        pVendorSell->OnLock();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_vendor_title_popup;
		msg.wParam=InterfaceManager::DIALOG_VENDOR_TITLE_POPUP;
		msg.DoSomething=GlobalFunc::DoVentorTitlePopup;
		g_KeyQueueManager.PushBack(msg);

        g_InterfaceManager.AttachFirstDialog(InterfaceManager::DIALOG_VENDOR_TITLE_POPUP);
    }
    else
    {
        g_InterfaceManager.DeAttachFirstDialog(InterfaceManager::DIALOG_VENDOR_TITLE_POPUP);

        this->m_status = ASK_STATUS_CANCEL;

        pVendorSell->OffLock();

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_vendor_title_popup);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
uiVendorTitlePopup::Exit()
{
	ShowInterface( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorTitlePopup::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DIALOG_EDIT_TITLE_MESSAGE:
				{
					m_bFocus = false;
					SendVendorTitle();
				}
				break;
			}
        }
        break;

	case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
                // 닫기
            case DIALOG_BTN_CANCEL:
                {
                    Exit();
                }
                break;

            case DIALOG_EDIT_TITLE_MESSAGE:
                {
                    m_bFocus = true;
                }
                break;

            case DIALOG_BTN_OK:
                {
                    m_bFocus = false;
					SendVendorTitle();
                    
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

BOOL uiVendorTitlePopup::SendVendorTitle( void )
{
	TCHAR   szTitleMessage[64] = {0,};

    CCtrlDxIMEEditBox * m_pChatEditCtrl;
    m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( DIALOG_EDIT_TITLE_MESSAGE );

	// 개인상점 메시지의 최대 사이즈(64 byte)를 넘었습니다.
	if( !GlobalFunc::CompareLimitText( m_pChatEditCtrl, szTitleMessage, 64, 5793 ) )
		return FALSE;

    if( lstrlen( szTitleMessage ) == 0 )
    {
		//	메시지를 입력해 주세요.
        GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, g_InterfaceManager.GetInterfaceString( 70400 ) );
    }
    else
    {
		uiVendorSell *pUIVendorSell = GET_CAST_DIALOG( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );

        if (pUIVendorSell)
        {
            pUIVendorSell->SetVendorMessage( szTitleMessage );
        }

        this->ShowInterface(FALSE);
    }

	return TRUE;
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
