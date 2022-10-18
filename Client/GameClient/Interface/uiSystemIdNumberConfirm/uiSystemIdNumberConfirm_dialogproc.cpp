//------------------------------------------------------------------------------
//  uiSystemConfirm_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemIdNumberConfirm.h"
#include "Nstring.h"
#include "Application.h"
#include "GameConst_Color.h"

//------------------------------------------------------------------------------
WzID uiSystemIdNumberConfirm::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_YES=0,
    StrToWzID("TI01"),  // DLGPOS_BTN_NO,
	StrToWzID("E000"),  // DLGPOS_EDIT,

    StrToWzID("L001"),  // DLGPOS_LST_CONFIRM_MSG,
};

//------------------------------------------------------------------------------
/**
*/
void
uiSystemIdNumberConfirm::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

		CCtrlDxEditBox *pEdit=(CCtrlDxEditBox *)getControl( DLGPOS_EDIT );
		if(pEdit)
		{
			pEdit->ClearText();
			pEdit->SetColorCaret(c_InterfaceCaretColor);
			pEdit->SetFontID(c_wiSystemMsgFont);
			pEdit->SetFocusWZ();
		}

        this->OffLock();
        this->OffPending();
    }
    else
    {
        this->GetUIMan()->UnLockDlgs();
        this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_SYSTEM_IDNUMBER_CONFIRM);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemIdNumberConfirm::MessageProc( SI_MESSAGE * pMessage )
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
					pEdit->GetTextWZA(buff,256);

					nString idNum=buff;
					
					if(idNum.Length()==0)
						break;

#if defined(__KR) || defined(_KOREA)
	
					if(idNum.IsNumberic())
					{
						this->ShowInterface(FALSE);

						if (this->m_fnSysConfirmString)
							this->m_fnSysConfirmString(true,(const char *)buff);
					}
					else
					{
						if (this->m_fnSysConfirmString)
							this->m_fnSysConfirmString(false,(const char *)buff);

						//	숫자를 입력하세요.
						TCHAR	szMessage[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString( eST_INPUT_NUMBER, szMessage, INTERFACE_STRING_LENGTH );
						g_InterfaceManager.ShowMessageBox(szMessage, 2000, NULL, 0 );
					}
#else //#if defined(__KR) || defined(_KOREA)
					// 해외버전은 주민번호 뒷자리에 문자가 올수도 있다.
					this->ShowInterface(FALSE);
					if (this->m_fnSysConfirmString)
						this->m_fnSysConfirmString(true,(const char *)buff);	
#endif //#if defined(__KR) || defined(_KOREA)

					SetDestroyWindow(TRUE);					
                }
                break;

            case DLGPOS_BTN_NO:
                {
                    this->ShowInterface(FALSE);

                    if (this->m_fnSysConfirmString)
                        this->m_fnSysConfirmString(false,NULL);

					SetDestroyWindow(TRUE);		
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
uiSystemIdNumberConfirm::OnRenderSolarDialog()
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

    RECT rc2;
    GetDialogWZ()->GetClientRect(&rc2);
    
    for (size_t i=0; i<this->m_vstr.size(); ++i)
    {
        SetRect(&rc,
            rc2.left + 25,
            rc2.top + 50 + (16*i),
            rc2.right - 10,
            rc2.top + 50 + (16*i) + 16);

        TCHAR *pStr = const_cast<TCHAR *>(this->m_vstr[i].c_str());

        if (pStr) {
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
uiSystemIdNumberConfirm::RenderRect(RECT rc,WzColor color)
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
