//------------------------------------------------------------------------------
//  uiSystemConfirm_dialogproc.cpp
//  
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemInputMessageConfirm.h"
#include "Nstring.h"
#include "Application.h"
#include "globalfunc.h"
#include "GameConst_Color.h"

//------------------------------------------------------------------------------
WzID uiSystemInputMessageConfirm::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_YES=0,
    StrToWzID("TI01"),  // DLGPOS_BTN_NO,
	StrToWzID("E000"),  // DLGPOS_EDIT,

    StrToWzID("T001"),  // DLGPOS_TEXT_CONFIRM_MSG,
	StrToWzID("T999"),	// DLGPOS_TITLE
	StrToWzID("BT00"),	// DLGPOS_CLOSE
};

//------------------------------------------------------------------------------
/**
*/
void
uiSystemInputMessageConfirm::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

		CCtrlDxEditBox *pEdit=(CCtrlDxEditBox *)getControl( DLGPOS_EDIT );
		if(pEdit)
		{
			pEdit->ClearText();
			pEdit->SetFontID(c_wiSystemMsgFont);
			pEdit->SetColorCaret(c_InterfaceCaretColor);
		}

        this->OffLock();
        this->OffPending();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_System_Message_Confrim;
		msg.wParam=InterfaceManager::DIALOG_SYSTEM_INPUTMSG_CONFIRM;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

    }
    else
    {
		g_KeyQueueManager.DeleteMsg(eDoType_System_Message_Confrim);
        this->GetUIMan()->UnLockDlgs();
        this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_SYSTEM_INPUTMSG_CONFIRM);

        SetNumberic(false);
    }
}

void uiSystemInputMessageConfirm::SetEditMessage(TCHAR* szEditMessage)
{
	CCtrlDxEditBox *pEdit=(CCtrlDxEditBox *)getControl( DLGPOS_EDIT );
	pEdit->SetTextWZ(szEditMessage);
}

void uiSystemInputMessageConfirm::SetTitleMessage(TCHAR* szTitleMessage)
{
	CCtrlStaticWZ *pEdit=(CCtrlStaticWZ *)getControl( DLGPOS_TITLE );
	pEdit->SetTextWZ(szTitleMessage);
}

//------------------------------------------------------------------------------
void uiSystemInputMessageConfirm::SetNumberic(bool numberic)
{
    CCtrlDxEditBox* edit_control =(CCtrlDxEditBox *)getControl(DLGPOS_EDIT);
    if(edit_control)
    {
        edit_control->SetNumberic(numberic);
    }

}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemInputMessageConfirm::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_YES:
                {
					CCtrlDxEditBox *pEdit=(CCtrlDxEditBox *)getControl( DLGPOS_EDIT );
					char buff[256];
					pEdit->GetTextWZA(buff,sizeof(buff));

					nString idNum=buff;
					
					if(idNum.Length()==0)
						break;

					SetDestroyWindow(TRUE);

                    if (this->m_fnSysConfirmString)
						this->m_fnSysConfirmString(true,(const char *)buff);
                }
                break;

            case DLGPOS_BTN_NO:
			case DLGPOS_CLOSE:
                {
                    SetDestroyWindow(TRUE);

                    if (this->m_fnSysConfirmString)
                        this->m_fnSysConfirmString(false,NULL);
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
void
uiSystemInputMessageConfirm::OnRenderSolarDialog()
{
    RECT rc;

	if (m_bLockingEffect)
	{

		POINT pt = g_pApplication->Get2DModeMapping();


		SetRect(&rc,
			0,
			0,
			pt.x,
			pt.y);
		RenderRect(rc, WzColor_RGBA(0,0,0,128));

	}

    GetDialogWZ()->OnDraw(true);

    CCtrlStaticWZ* pTextCtrl = (CCtrlStaticWZ *)getControl(DLGPOS_TEXT_CONFIRM_MSG);
	if(!pTextCtrl)
		return;

	RECT rcCtrl = pTextCtrl->GetSizeRect();
	int iCtrlWidth = rcCtrl.right - rcCtrl.left - 5;

    for (size_t i=0; i<this->m_vstr.size(); ++i)
    {
    	  SetRect(&rc,
				rcCtrl.left + 2,
				rcCtrl.top + (16*i),
				rcCtrl.right,
				rcCtrl.top + (16*i) + 16);

        TCHAR *pStr = const_cast<TCHAR *>(this->m_vstr[i].c_str());

        if (pStr) 
		{
            this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc,
                WzColor_RGBA(255, 133, 50, 255),
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
uiSystemInputMessageConfirm::RenderRect(RECT rc,WzColor color)
{

	g_pSunRenderer->Set2DMode(TRUE);
	ENUM_ALPHABLEND	eAlphablendMode =  g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );

	g_pSunRenderer->ColorFill(color,0,0,(float)rc.right,(float)rc.bottom);

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
	g_pSunRenderer->Set2DMode(FALSE);

}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
