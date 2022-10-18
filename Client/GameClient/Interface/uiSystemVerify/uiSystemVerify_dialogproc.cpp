//------------------------------------------------------------------------------
//  uiSystemVerify_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemVerify.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Input.h"
#include "Application.h"
#include "GameFramework.h"
#include "globalfunc.h"
#include "CharacterScene.h"
#include "LoadScene.h"
#include "uiLoginAccount/uiLoginAccount.h"
#ifdef _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
#include "uiLoginMan/uiLoginMan.h"
#endif //_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
//------------------------------------------------------------------------------
WzID uiSystemVerify::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_VERIFY=0,
    StrToWzID("L001"),  // DLGPOS_LST_VERIFY_MSG,
};

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::OnShowWindow( BOOL val )
{
    if (val)
    {
		g_CharSelectScene.m_bVerifyLock  =TRUE;

        RECT rc;
        GetDialogWZ()->GetDlgSize(&rc);

        long width = rc.right;
        long height = rc.bottom;

        // TODO:
        // 전체 화면 해상도를 참조해서 다이얼로그를 화면 중앙에 위치시킨다.
        // ex) 1024x768

        long mw = width >> 1;
        long mh = height >> 1;

        

		POINT pt = g_pApplication->Get2DModeMapping();

        long sw;
        long sh;
        
        sw = pt.x; sh = pt.y;
        
        long pw = (sw >> 1) - mw;
        long ph = (sh >> 1) - mh;

        switch (GameFramework::GetCurScene() )
        {
        case SCENE_TYPE_LOGIN:
            {
                ph += 75;
            }
            break;
        }

        GetDialogWZ()->MoveWindowWZ( (float)pw, (float)ph );

        this->OffLock();
        this->OffPending();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_system_verify;
        msg.wParam = InterfaceManager::DIALOG_SYSTEM_VERIFY;
        msg.DoSomething = GlobalFunc::DoSystemVerify;
        g_KeyQueueManager.PushBack(msg);

        this->m_bReadyWarmup = false;
        m_timerWarmup.On(1);
    }
    else
    {
		if (m_bShutdownAfterVerify)
		{
			Exit();
		}
		else
		{
			CloseWindow();
		}
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::Exit()
{
	CloseWindow();

	SetDestroyWindow(TRUE);					

    if (this->m_bShutdownAfterVerify)
    {
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
		GlobalFunc::LogOutGame();
#else //_COMMON_CHANNELING_LOGIC_DEFINE
    #ifdef _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
        if (uiLoginMan::IsUseParameterAuthInfo() == true)
        {
            GlobalFunc::LogOutGame();
        }
        else
    #endif _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
        {
            // 게임중에 팅긴 것이라면 로그인 화면으로 이동하게 한다.
            if( m_CloseType == CT_GOTOLOGIN )
            {
                if( GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_LOGIN )
                {
                    GlobalFunc::GoToServerList();
                }
            }
            else
            {
                GlobalFunc::LogOutGame();
            }
        }
#endif //_COMMON_CHANNELING_LOGIC_DEFINE		
    }
    else if (m_fnSysVerify)
    {
        this->m_fnSysVerify();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_VERIFY:
                {
                    this->Exit();
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
uiSystemVerify::Process(DWORD dwTick)
{
    if (!this->IsVisible())
        return;


	if( m_bShutdownAfterVerify )	// shotdown시에는 시간이 지나면 자동으로 닫게 한다.
	{
		int timestate = m_timerExit.Check();
		if( timestate != TIMER_STAT_NONE )
		{
			if( timestate == TIMER_STAT_FINISH )
			{
				m_timerExit.Off();
				this->Exit();
			}
		}
	}

    int timerStat = m_timerWarmup.Check();
    if (timerStat != TIMER_STAT_NONE)
    {
        if (timerStat == TIMER_STAT_FINISH)
        {
            this->m_bReadyWarmup = true;
            m_timerWarmup.Off();
        }
    }

    if (this->m_bReadyWarmup)
    {
        if (g_Input.GetState (DIK_RETURN, KS_UP))
        {
            this->Exit();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::OnUpdateSolarDialog()
{
    CCtrlStaticWZ* pControl = static_cast<CCtrlStaticWZ *>(getControl(DLGPOS_BTN_VERIFY));
    if (pControl)
    {
        if (this->m_bReadyWarmup)
            pControl->SetTextColor(RGBA( 240, 201, 121, 255 ));
        else
            pControl->SetTextColor(RGBA(128, 128, 128, 128));
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiSystemVerify::OnRenderSolarDialog()
{
	this->GetDrawBase()->x_pManagerTextOut->StartToUseFont( c_wiSystemMsgFont );
    RECT rc;

	if (m_bLockingEffect)
	{
		POINT ScreenSize = g_pApplication->Get2DModeMapping();
		SetRect(&rc,
			0,
			0,
			ScreenSize.x,
			ScreenSize.y);

		RenderRect(rc, WzColor_RGBA(0,0,0,128));

	}

    GetDialogWZ()->OnDraw(true);

    RECT rc2;
	CControlWZ * pLstControl = getControl( DLGPOS_LST_VERIFY_MSG );
	if ( pLstControl )
	{
		rc2 = pLstControl->GetSizeRect();
	}

	int iExtraInfoHeight = 0;

	SIZE FontSize;
	if ( this->m_strTitle.size() )
	{
		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont( c_wiSystemMsgFont );
		this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);

		SetRect(&rc,
            rc2.left,
            rc2.top + (FontSize.cy*0),
            rc2.right,
            rc2.top + (FontSize.cy*0) + FontSize.cy);

		iExtraInfoHeight += FontSize.cy+8;

        TCHAR *pStr = const_cast<TCHAR *>(this->m_strTitle.c_str());

        if (pStr) 
		{
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc,
                WzColor_RGBA(240,201,121,255),
                WzColor_RGBA(0,0,0,0),
                TP_HCENTER);
        }

		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
	}
#ifdef _INTERNATIONAL_UI
	 for (size_t i=0; i<this->m_vSubstr.size(); ++i)
	 {
		ENUM_DRAWTEXTSTYLE eAlignStyle  = ( (m_vSubstr.size()>1) ? (TP_HLEFT):(TP_HCENTER));

		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);
		this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);

		SetRect(&rc,
            rc2.left,
            rc2.top  + (FontSize.cy*(i+1)),
            rc2.right,
            rc2.top  + (FontSize.cy*(i+1))+ FontSize.cy);

        TCHAR * pStr = const_cast<TCHAR *>(this->m_vSubstr[i].c_str());

        if (pStr) 
		{
		   iExtraInfoHeight += FontSize.cy;

           this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc,
                WzColor_RGBA(255, 133, 50, 255),
                WzColor_RGBA(0,0,0,0),
                eAlignStyle);
        }

		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
	 }
#else
	if ( this->m_strSubject.size() )
	{
		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);
		this->GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint(_T("1"), 1, &FontSize);

		SetRect(&rc,
            rc2.left,
            rc2.top  + (FontSize.cy)+8,
            rc2.right,
            rc2.top  + (FontSize.cy) + FontSize.cy+8);

		iExtraInfoHeight += FontSize.cy+8;

        TCHAR * pStr = const_cast<TCHAR *>(this->m_strSubject.c_str());

        if (pStr) 
		{
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                pStr,
                &rc,
                WzColor_RGBA(255, 133, 50, 255),
                WzColor_RGBA(0,0,0,0),
                TP_HCENTER);
        }

		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
	}
#endif//_INTERNATIONAL_UI
    for (size_t i=0; i<this->m_vstr.size(); ++i)
    {
		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(c_wiSystemMsgFont);
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
		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
    }

	this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
}

//------------------------------------------------------------------------------
/**
*/
void 
uiSystemVerify::RenderRect(RECT rc,WzColor color)
{
	g_pSunRenderer->Set2DMode(TRUE);
	ENUM_ALPHABLEND	eAlphablendMode =  g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );

	g_pSunRenderer->ColorFill(color,0,0,(float)rc.right,(float)rc.bottom);

	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode );
	g_pSunRenderer->Set2DMode(FALSE);

}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiSystemVerify::SetTitle(LPCTSTR pszTitle)
{
	this->m_strTitle = pszTitle;
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiSystemVerify::SetSubject(LPCTSTR pszSubject)
{
	this->m_strSubject = pszSubject;

#ifdef _INTERNATIONAL_UI
	CControlWZ * pLstControl = getControl( DLGPOS_LST_VERIFY_MSG );

	if( pLstControl )
	{
		RECT rcCtrl = pLstControl->GetSizeRect();

		int iCtrlWidth = rcCtrl.right - rcCtrl.left - 10;
	
		m_vSubstr.clear();

		GlobalFunc::ProcessWordWrapByFontSize( &this->m_vSubstr, const_cast<TCHAR *>(pszSubject), c_wiSystemMsgFont, _tcslen(pszSubject), iCtrlWidth );
	}
#endif//_INTERNATIONAL_UI
}

//-------------------------------------------------------------------------------------------
/**
*/
void					
uiSystemVerify::SetBtnStringID(DWORD dwOkID)
{
	this->m_dwOKStrID = dwOkID;
	if (m_pCtrlOKStatic)
	{
		m_pCtrlOKStatic->SetTextID(this->m_dwOKStrID);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void uiSystemVerify::CloseWindow()
{
	g_CharSelectScene.m_bVerifyLock  =FALSE;

	g_KeyQueueManager.DeleteMsg(eDoType_dialog_system_verify);

	this->GetUIMan()->UnLockDlgs();
	this->GetUIMan()->DeAttachFirstDialog(InterfaceManager::DIALOG_SYSTEM_VERIFY);

	m_timerWarmup.Off();
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
