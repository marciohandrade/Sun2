//------------------------------------------------------------------------------
//  uiLoginBack_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginBack.h"
#include "uiLoginMan/uiLoginMan.h"
#include "Application.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ClientVer.h"

TCHAR szREGISTER_URL[512];

//------------------------------------------------------------------------------
WzID uiLoginBack::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("L000"),  // DLGPOS_LST_VERSION
#ifdef _REMOVE_REGISTER_BUTTON_FOR_CHANNELING
#else
    StrToWzID("TI01"),  // DLGPOS_BTN_REGISTER,
#endif
    StrToWzID("TI02"),  // DLGPOS_BTN_EXIT,
};

//------------------------------------------------------------------------------
/**
*/
VOID
uiLoginBack::OnShowWindow(BOOL val)
{
    uiBase::OnShowWindow(val);

    if (val)
    {
        if (!this->CheckControls())
            return;

		//MoveWindowWZ( 0, 0 );

        int buildtypeID = c_sBUILDTYPE_DEBUG;
#ifdef _DEBUG
        buildtypeID = c_sBUILDTYPE_DEBUG;
#else
        buildtypeID = c_sBUILDTYPE_RELEASE;
#endif

        m_timerConnectLoginFronTimeout.On(5);

    }
    else
    {
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginBack::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
#ifdef _REMOVE_REGISTER_BUTTON_FOR_CHANNELING
#else
            case DLGPOS_BTN_REGISTER:
                {
                    SHELLEXECUTEINFO ShExecInfo;

                    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
                    ShExecInfo.fMask = NULL;
                    ShExecInfo.hwnd = g_hWndMain;
                    ShExecInfo.lpVerb = NULL;
					g_InterfaceManager.GetInterfaceString(2000, szREGISTER_URL, 512);
                    ShExecInfo.lpFile = szREGISTER_URL;
                    ShExecInfo.lpParameters = NULL;
                    ShExecInfo.lpDirectory = NULL;
                    ShExecInfo.nShow = SW_MAXIMIZE;
                    ShExecInfo.hInstApp = NULL;

                    ShellExecuteEx(&ShExecInfo);
                }
                break;
#endif
            case DLGPOS_BTN_EXIT:
                {
                    SendMessage(g_hWndMain, WM_CLOSE, 0, 0);
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

extern TCHAR	g_szSystemCheckMessage[];
//------------------------------------------------------------------------------
/**
*/
void
uiLoginBack::Process(DWORD dwTick)
{
    if (!this->IsVisible())
        return;

    if (this->GetManager()->IsAuthConnected())
        m_timerConnectLoginFronTimeout.Off();

    int timerStat = m_timerConnectLoginFronTimeout.Check();
    if (timerStat != TIMER_STAT_NONE)
    {
        if (timerStat == TIMER_STAT_FINISH)
        {
			uiSystemMan * pSystemMan = GET_DIALOG_MANAGER( uiSystemMan, UIMAN_SYSTEM );

			if(pSystemMan)
			{
#ifdef	_KOREA
				pSystemMan->Sys_VerifyLock( g_szSystemCheckMessage, TRUE );
#else
				TCHAR szBuff[INTERFACE_STRING_LENGTH + 1];
				g_InterfaceManager.GetInterfaceString( 5262, szBuff );
				pSystemMan->Sys_VerifyLock( szBuff, TRUE );
#endif
			}

            m_timerConnectLoginFronTimeout.Off();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginBack::OnRenderSolarDialog()
{
    int buildtypeID = c_sBUILDTYPE_DEBUG;
#ifdef _DEBUG
    buildtypeID = c_sBUILDTYPE_DEBUG;
#else

	if(LOGINPARAM->IsLiveTestServer())	// 테섭실행모드
	{
		buildtypeID = c_sBUILDTYPE_TEST_RELEASE;
	}
	else
	{
		buildtypeID = c_sBUILDTYPE_RELEASE;
	}

#endif

	TCHAR	szBuildVersion[INTERFACE_STRING_LENGTH] ={0,};
	StrnCopy( szBuildVersion , c_tcBUILD_VERSION , INTERFACE_STRING_LENGTH -1);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    Sprintf( szMessage, _T("%s - Ver.%s %s"), 
			g_InterfaceManager.GetInterfaceString( 1661 ),
			CLIENTINFO->GetClientVersion(), c_tsBUILD_TYPE[buildtypeID],
			C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO );

   
    POINT pt = g_pApplication->Get2DModeMapping();
    pt.x = 17;    
    // 2013.04.02 / 홍성현 / pt.y가 해상도를 가져오기 때문에 pt.y를 해줌으로써 밑에 부분에 고정되어서 출력되게 된다.
    pt.y -= 98;

    RECT rc;

    SetRect(&rc,
            pt.x,
            pt.y,
            pt.x + 700,
            pt.y + 30);

    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255,162,38, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT,
        ETS_OUTLINE);

	// 컴파일 버전도 출력합시다~ (유저가 사용하고 있는 버전이 무엇인지 바로 알 수 있게) : 컴파일 버전은 노출되어도 괜찮습니다.
	Sprintf( szMessage, _T("Protocol [%d.%d.%d] / Build [%s]"), 
			C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, szBuildVersion);

    // 2013.04.02 / 홍성현 / pt.y에서 20 플러스 한 결과 이므로 pt.y 아래 20 플러스 해준다.
    pt.y += 20;

    SetRect(&rc,
            pt.x,
            pt.y,
            pt.x + 700,
            pt.y + 30);

    this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255,162,38, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT,
        ETS_OUTLINE);    

#ifdef _CHINA
    {
        // StringTable의 \n는 문자 그대로 읽어오기때문에 개행문자로 직접변환해준다
        std::string Message = g_InterfaceManager.GetInterfaceString(5645);

        std::string Result;

        // "\n" 문자를 '\n'으로 변환
        size_t cur = 0;
        size_t pos = 0;
        pos = Message.find( "\\n", cur );
        while( pos != std::string::npos )
        {
            Result += Message.substr(cur, pos-cur);
            Result += '\n';

            cur = pos+2;

            if( cur >= Message.size() )
                break;

            pos = Message.find( "\\n", cur );
        }

        if( cur < Message.size() )
        {
            Result += Message.substr( cur, Message.size()-cur );
        }
        /////


        POINT pt = g_pApplication->Get2DModeMapping();
        pt.x = 17;
        pt.y -= 98 + 90;

        RECT rc;
        SetRect(&rc,pt.x,pt.y, pt.x + 700, pt.y + 90 );

        GetDrawBase()->x_pManagerTextOut->DrawTextA(Result.c_str(), &rc, WzColor_RGBA(0xff,0xe4, 0xb5, 0xff), WzColor_RGBA(0,0,0,0), TP_HLEFT, ETS_BASE );
    }
#endif // _CHINA


}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
