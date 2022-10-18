#include "SunClientPrecompiledHeader.h"

#include "uiWebDialog.h"
#include "uiWebDialogMan/uiWebDialogMan.h"
#include "Hero.h"

#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

#include "GameFramework.h"
#include "SceneBase.h"

#ifdef _KIKI_UI_WEB_DIALOG

//------------------------------------------------------------------------------
WzID uiWebDialog::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),  // DLGPOS_BTN_EXIT,
		StrToWzID("WB03"),  // DLGPOS_WEB_CTRL,
};

//------------------------------------------------------------------------------
/**
*/
uiWebDialog::uiWebDialog(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pWebDialogMan =
        static_cast<uiWebDialogMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_WEB));
    assert (m_pWebDialogMan);

    if (!this->m_pWebDialogMan)
    {
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pWebDialogMan이 없습니다."));

#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_pWebControl = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiWebDialog::~uiWebDialog()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialog::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();

	m_pWebControl = static_cast<CCtrlWebBrowserWZ *>(GetControlWZ( m_wzId[DLGPOS_WEB_CTRL] ));
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialog::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialog::InitControls()
{
	
	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiWebDialog::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_web;
        msg.wParam = InterfaceManager::DIALOG_WEB_DIALOG;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_web);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiWebDialog::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if( g_pHero && g_pHero->IsShowFullMap() )
		{
			if(this->IsVisibleWindow())
				ShowInterface(FALSE);	

			return FALSE;
		}

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
void uiWebDialog::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_EXIT:
                {
                    this->ShowInterface(FALSE);
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

void uiWebDialog::Navigate( WCHAR * pURL, BOOL bWindowShow)
{
	if( m_pWebControl )
	{
		m_pWebControl->Navigate( pURL );

		if(bWindowShow)
		{
			ShowInterface(TRUE);

			// 웹 브라우저가 뜨면서 포커스가 이동되어 게임화면을 클릭 안하면 계속 까만 잔상이 남는 현상때문에 씬을 한번 렌더해줌.
			if(g_pSunRenderer->BeginRender() == TRUE)
			{
				DWORD dwTick;
				g_pSunRenderer->x_Clock.CheckFrameSkipping( &dwTick );
				GameFramework::GetCurrentScene()->Render(dwTick);
				g_InterfaceManager.Render();
				g_pSunRenderer->EndRender();
			}
		}
	}
		
}

//------------------------------------------------------------------------------
void uiWebDialog::BlankPage()
{
	if( m_pWebControl )
	{
		WCHAR wszURL[MAX_PATH] = {0,};
		char *pURL = "about:blank";	
		if(pURL)
		{
			ConvertToUNICODE( pURL , wszURL );
			m_pWebControl->Navigate( wszURL );
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiWebDialog::NetworkProc( MSG_BASE * pMsg )
{
}

#endif