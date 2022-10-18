//------------------------------------------------------------------------------
//  uiSystemConfirm_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemConfirm.h"
#include "Application.h"
#include "Hero.h"
#include "Command.h"

//------------------------------------------------------------------------------
WzID uiSystemConfirm::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_YES=0,
    StrToWzID("TI01"),  // DLGPOS_BTN_NO,
    StrToWzID("L000"),  // DLGPOS_LST_CONFIRM_MSG,
    StrToWzID("BT02"),  // DLGPOS_BTN_CLOSE
};

//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::OnShowWindow( BOOL val )
{
    if (val)
    {
		m_bShow =TRUE;
        SolarDialog::MoveCenter();

        //------------------------------------------------------------------------------ 
        // esc 키로 창 닫기 
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_System_Dialog;
        msg.wParam = 0;
        msg.lParam = GetDialogKey();
        msg.DoSomething = GlobalFunc::DoSystemConfirm;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 

        this->OffLock();
        this->OffPending();
    }
    else
    {
		m_bShow = FALSE;

        this->GetUIMan()->UnLockDlgs();
        this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_SYSTEM_CONFIRM);
        g_KeyQueueManager.DeleteMsg(eDoType_System_Dialog);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::MessageProc( SI_MESSAGE * pMessage )
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
                    if( m_pOkCommand != NULL )
                    {
                        m_pOkCommand->Execute();
                    }

                    if (m_fnSysConfirm)
                    {
						m_fnSysConfirm(true);
                    }
                    else if (m_fnSysConfirmCloseButton)
                    {
                        m_fnSysConfirmCloseButton(kConfirmSelect1);
                    }
					m_bShow = FALSE;
					SetDestroyWindow(TRUE);
                }
                break;

            case DLGPOS_BTN_NO:
                {
                    if( m_pCancelCommand != NULL )
                    {
                        m_pCancelCommand->Execute();
                    }

                    if (m_fnSysConfirm)
                    {
						m_fnSysConfirm(false);
                    }
                    else if (m_fnSysConfirmCloseButton)
                    {
                        m_fnSysConfirmCloseButton(kConfirmSelect2);
                    }
					m_bShow = FALSE;
					SetDestroyWindow(TRUE);
                }
                break;
            case DLGPOS_BTN_CLOSE:
                {
                    OnClose();
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
void uiSystemConfirm::OnClose()
{
    if( m_pCancelCommand != NULL )
    {
        m_pCancelCommand->Execute();
    }

    if (m_fnSysConfirm)
    {
        m_fnSysConfirm(false);
    }
    else if (m_fnSysConfirmCloseButton)
    {
        m_fnSysConfirmCloseButton(kConfirmClose);
    }
    m_bShow = FALSE;
    SetDestroyWindow(TRUE);
}
//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::OnUpdateSolarDialog()
{
	// 900 안전지대이동
	if( m_pCtrlStatic[1]->GetTextID() == 900 )
	{
		if( g_pHero && !g_pHero->IsDead() )
		{
			// 5 닫기
			m_bShow = FALSE;
			SetDestroyWindow(TRUE);
			m_pCtrlStatic[1]->SetTextID( 5 );
		}
	}

	if(m_bDestroyWindow)
	{
		if (this->m_fnSysConfirm)
		{
			this->m_fnSysConfirm(false);
		}		
		m_bShow = FALSE;
		SetDestroyWindow(TRUE);

		m_Timer.Clear();
		m_bDestroyWindow = false;
	}

	if( ! m_bShow )
	{
		this->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemConfirm::OnRenderSolarDialog()
{
    RECT rc;

	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);

	if (m_bLockingEffect)
	{
		POINT pt = g_pApplication->Get2DModeMapping();

		SetRect(&rc,
			0,
			0,
			pt.x,
			pt.y);

		RenderRect(rc, WzColor_RGBA(0,0,0,128));

		GetDialogWZ()->OnDraw(true);
	}


	RECT rc2;
	CControlWZ * pLstControl = getControl( DLGPOS_LST_CONFIRM_MSG );
	if ( pLstControl )
	{
		rc2 = pLstControl->GetSizeRect();
	}

	int	 iExtraInfoHeight = 0;
#ifdef _INTERNATIONAL_UI
	const int iLineGap=0;
#else
	const int iLineGap=8;	
#endif//_INTERNATIONAL_UI
	SIZE FontSize;
	if ( this->m_strTitle.size() )
	{
		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);
		this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);

		SetRect(&rc,
            rc2.left,
            rc2.top + (FontSize.cy*0) + m_TitleTextGap,
            rc2.right,
            rc2.top + (FontSize.cy*0) + FontSize.cy+iLineGap + m_TitleTextGap);

		iExtraInfoHeight += FontSize.cy+iLineGap;

        TCHAR *pStr = const_cast<TCHAR *>(this->m_strTitle.c_str());

        if (pStr) 
		{
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc,
                WzColor_RGBA(240,201,121,255 ),
                WzColor_RGBA(0,0,0,0),
                TP_HCENTER);
        }

		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
	}

	if(!m_vSubject.empty())
	{
		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);
		this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);

		for (size_t i=0;i<this->m_vSubject.size(); ++i)
		{
			//두줄이면 왼쪽정렬 한줄이면센터정렬
			ENUM_DRAWTEXTSTYLE eAlignStyle  = ( (m_vSubject.size()>1) ? (TP_HLEFT):(TP_HCENTER));
			
			SetRect(&rc,
				rc2.left,
				rc2.top + (FontSize.cy*(i+1))+iLineGap,
				rc2.right,
				rc2.top + (FontSize.cy*(i+1)) + FontSize.cy+iLineGap);
			
			TCHAR * pStr = const_cast<TCHAR *>(this->m_vSubject[i].c_str());
				
			if (pStr) 
			{                
				iExtraInfoHeight += FontSize.cy+iLineGap;

				this->GetDrawBase()->x_pManagerTextOut->DrawText(
					pStr,
					&rc,
					WzColor_RGBA(255, 133, 50, 255),
					WzColor_RGBA(0,0,0,0),
					eAlignStyle);
			}
		}

		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
	}
	
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);

    for (size_t i=0; i<this->m_vstr.size(); ++i)
    {
		
       SetRect(&rc,
            rc2.left + 2,
            iExtraInfoHeight + rc2.top + (16*i),
            rc2.right,
            iExtraInfoHeight + rc2.top + (16*i) + 16);

        TCHAR *pStr = const_cast<TCHAR *>(this->m_vstr[i].c_str());
        if (pStr) 
		{
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc,
                WzColor_RGBA(255, 133, 50, 255),
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT);
        }
    }

	DWORD dwRTime = m_Timer.GetRemainedTime();
	if(dwRTime && !m_Timer.IsExpired() && !m_bDestroyWindow)
	{
		TCHAR StrTime[512] = "";
		sprintf(StrTime, "(%d/%d)", dwRTime/1000, m_Timer.GetIntervalTime()/1000);
		int i;
		SetRect(&rc,
            rc2.left + 2,
            iExtraInfoHeight + rc2.top + (16*i),
            rc2.right,
            iExtraInfoHeight + rc2.top + (16*i) + 16);

		if (StrTime) 
		{
			this->GetDrawBase()->x_pManagerTextOut->DrawText(
				StrTime,
				&rc,
				WzColor_RGBA(255, 133, 50, 255),
				WzColor_RGBA(0,0,0,0),
				TP_HLEFT);
		}
	}
	else
	{
		if(m_Timer.GetExpireTime())
		{
			m_bDestroyWindow = true;
		}
	}

	this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
}

//------------------------------------------------------------------------------
/**
*/
void 
uiSystemConfirm::RenderRect(RECT rc,WzColor color)
{
	g_pSunRenderer->ColorFill(color,0,0,(float)rc.right,(float)rc.bottom);
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiSystemConfirm::SetTitle(LPCTSTR pszTitle)
{
	this->m_strTitle = pszTitle;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiSystemConfirm::SetSubject(LPCTSTR pszSubject)
{
	CControlWZ * pLstControl = getControl( DLGPOS_LST_CONFIRM_MSG );

	if( pLstControl )
	{
		RECT rcCtrl = pLstControl->GetSizeRect();

		int iCtrlWidth = rcCtrl.right - rcCtrl.left - 10;

		GlobalFunc::ProcessWordWrapByFontSize( &this->m_vSubject, const_cast<TCHAR *>(pszSubject), c_wiSystemMsgFont, _tcslen(pszSubject), iCtrlWidth );
	}

}

//-------------------------------------------------------------------------------------------
/**
*/
void				
uiSystemConfirm::SetBtnStringID(DWORD dwOkID, DWORD dwCancleID)
{
	this->m_dwOKStrID = dwOkID;
	if (m_pCtrlStatic[0])
	{
		m_pCtrlStatic[0]->SetTextID(this->m_dwOKStrID);
#ifdef _INTERNATIONAL_UI
		m_pCtrlStatic[0]->SetFontID(StrToWzID("st10"));
#endif//_INTERNATIONAL_UI
	}

	this->m_dwCancleStrID = dwCancleID;
	
	if (m_pCtrlStatic[1])
	{
		m_pCtrlStatic[1]->SetTextID(this->m_dwCancleStrID);
#ifdef _INTERNATIONAL_UI
		m_pCtrlStatic[1]->SetFontID(StrToWzID("st10"));
#endif//_INTERNATIONAL_UI
	}
}
void
uiSystemConfirm::SetShowCancleButton(bool is_show)
{
    CControlWZ* button = getControl(DLGPOS_BTN_CLOSE);
    if (button)
    {
        button->ShowWindowWZ(is_show ? WZ_SHOW : WZ_HIDE);
    }
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

