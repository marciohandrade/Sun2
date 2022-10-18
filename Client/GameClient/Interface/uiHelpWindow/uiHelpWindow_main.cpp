#include "SunClientPrecompiledHeader.h"


#include "uiHelpWindow.h"
#include "uiHelpMan/uiHelpMan.h"

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiRadarMan/uiRadarMan.h"

const int c_iPageLine = 10;

//------------------------------------------------------------------------------
WzID uiHelpWindow::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("B999"),	//DLGPOS_BTN_EXIT,
	StrToWzID("B001"),	//DLGPOS_BTN_OK,
	StrToWzID("T001"), 	//DLGPOS_TXT_TITLE,
	StrToWzID("T002"), 	//DLGPOS_TXT_SUBTITLE,
	StrToWzID("T003"), 	//DLGPOS_TXT_DESC,
	StrToWzID("S001"), 	//DLGPOS_SCR_VERTICAL,
};

//------------------------------------------------------------------------------
/**
*/
uiHelpWindow::uiHelpWindow(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pHelpMan =
        static_cast<uiHelpMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_HELP));
    assert (m_pHelpMan);

    if (!this->m_pHelpMan)
    {
#ifdef _DEBUG

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pHelpMan이 없습니다."));

#endif
        this->ShowInterface(FALSE);
        return;
    }

    memset ((CCtrlButtonWZ *)this->m_pBtnControls, NULL, sizeof (this->m_pBtnControls));
	this->m_pScrollControl = NULL;
	
	for (int i=0; i<POS_TXT_SIZE; ++i)
        this->m_pTxtControls[i] = NULL;

	ZeroMemory(&m_HelpInfo, sizeof(m_HelpInfo));

	m_iCurScrollPos = 0;
}

//------------------------------------------------------------------------------
/**
*/
uiHelpWindow::~uiHelpWindow()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::InitControls()
{
    this->m_pScrollControl =
        static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_SCR_VERTICAL));
    assert (this->m_pScrollControl);

    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_EXIT + i));
        assert (this->m_pBtnControls[i]);

        this->m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
    }

	for (int i=0; i<POS_TXT_SIZE; ++i)
    {
        this->m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_TITLE + i));
        assert (this->m_pTxtControls[i]);
    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiHelpWindow::OnShowWindow( BOOL val )
{
    if (val)
    {
		if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		UpdatePosition();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_help_window;
        msg.wParam = InterfaceManager::DIALOG_HELP_WINDOW;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_help_window);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::UpdatePosition()
{
    ////SolarDialog::UpdatePosition();

	int iWidth, iHeight;
	
	RECT rc;
	GetDialogWZ()->GetClientRect(&rc);
	
	iWidth	= rc.right - rc.left;
	iHeight = rc.bottom - rc.top;

	RECT drc;
	if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
		pDlg->GetDialogWZ()->GetClientRect(&drc);
		GetDialogWZ()->MoveWindowWZ((float)(drc.right - iWidth), (float)(drc.bottom + 10)); 
	}
	else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
		pDlg->GetDialogWZ()->GetClientRect(&drc);
		GetDialogWZ()->MoveWindowWZ((float)(drc.right - iWidth), (float)(drc.bottom + 10)); 
	}
	
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::MessageProc( SI_MESSAGE * pMessage )
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
			case DLGPOS_BTN_OK:
                {
                    this->ShowInterface(FALSE);
                }
                break;
            }
        }
        break;

	case RT_MSG_SB_LINETOP:
        {
            //.윗쪽 화살표를 눌렀다.
            m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(m_iCurScrollPos>mmin)    // 현재 입력된 라인수 보다 작으면 
            {
                m_iCurScrollPos--;      // 한줄 감소 시키고 
                this->m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

				m_TextRender.SetBeginLineIndex(m_iCurScrollPos);
            }
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
        {
            //.아랫쪽 화살표를 눌렀다
            m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
            int mmin = 0 , mmax = 0;
            this->m_pScrollControl->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

            if(m_iCurScrollPos<mmax)    // 현재 입력된 라인수 보다 작으면 
            {
                m_iCurScrollPos++;      // 한줄 증가 시키고 
                this->m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

				m_TextRender.SetBeginLineIndex(m_iCurScrollPos);
            }
        }
        break;

	case RT_MSG_SB_THUMBTRACK:
	case RT_MSG_SB_THUMBPOSITION:
	case RT_MSG_SB_ENDSCROLL:
		{
			m_iCurScrollPos = this->m_pScrollControl->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
			
			m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

			m_TextRender.SetBeginLineIndex(m_iCurScrollPos);
		}
		break;

	case RT_MSG_SB_PAGETOP:
		{
			int pos = m_pScrollControl->GetScrollPosWZ() - c_iPageLine;
			if(pos < 0)
				pos = 0;

			m_iCurScrollPos = pos;
			m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

			m_TextRender.SetBeginLineIndex(m_iCurScrollPos);

		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
		{
			int pos = m_pScrollControl->GetScrollPosWZ() + c_iPageLine;
			if(pos > (int)m_TextRender.GetMaxLine())
				pos = m_TextRender.GetMaxLine();

			m_iCurScrollPos = pos;
			m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);

			m_TextRender.SetBeginLineIndex(m_iCurScrollPos);

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
void uiHelpWindow::SetHelpInfo(BASE_HelpInfo& helpInfo)
{
	m_HelpInfo = helpInfo;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		
	g_InterfaceManager.GetInterfaceString( eST_HELP_STRING_INDEX, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption (m_pTxtControls[TXT_TITLE], szMessage);

	g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_TitleStringCode, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption (m_pTxtControls[TXT_SUBTITLE], szMessage);
	
	FTextParser parser;
	FTextSentence Sentence;

	RECT rc = m_pTxtControls[TXT_DESC]->GetSizeRect();

	
	m_TextRender.Init(g_pSunRenderer, rc);
	m_TextRender.SetRect(rc);	

	m_TextRender.ClearSentence();

	parser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
	
	for(int i=0; i<c_iMaxDescCodeNum; ++i)
	{
		if(m_HelpInfo.m_dw_DescStringCode[i])
		{
			TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_DescStringCode[i], szString, INTERFACE_STRING_LENGTH );
	
			BOOL bRet = parser.Parser(szString, Sentence);
			if(bRet)
			{
				m_TextRender.Append(Sentence);
			}
		}
	}

	m_iCurScrollPos = 0;
	m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);
	m_TextRender.SetBeginLineIndex(m_iCurScrollPos);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::SetHelpStringCode(DWORD dwStringCode)
{
	m_HelpInfo.m_dw_HelpIndex = 100;	// 임시

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		
	g_InterfaceManager.GetInterfaceString( eST_HELP_STRING_INDEX, szMessage, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption (m_pTxtControls[TXT_TITLE], szMessage);

	GlobalFunc::SetCaption (m_pTxtControls[TXT_SUBTITLE], _T(""));
	
	FTextParser parser;
	FTextSentence Sentence;

	RECT rc = m_pTxtControls[TXT_DESC]->GetSizeRect();

	m_TextRender.Init(g_pSunRenderer, rc);
	m_TextRender.SetRect(rc);	

	m_TextRender.ClearSentence();

	parser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
	
	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString(dwStringCode, szString, INTERFACE_STRING_LENGTH );

	BOOL bRet = parser.Parser(szString, Sentence);
	if(bRet)
	{
		m_TextRender.Append(Sentence);
	}
	
	int iMaxLine = m_TextRender.GetMaxLine();

	m_iCurScrollPos = 0;
	m_pScrollControl->SetScrollPosWZ(m_iCurScrollPos);
	m_TextRender.SetBeginLineIndex(m_iCurScrollPos);
	

}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::OnRenderSolarDialog()
{
    renderHelpInfo();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpWindow::renderHelpInfo()
{
	if (!_isEnable())
		return;
	

	RECT rc = {0,};

	if(m_HelpInfo.m_dw_HelpIndex)
	{
		int iMaxLine = m_TextRender.GetMaxLine();
		m_pScrollControl->SetScrollRangeWZ(0,iMaxLine);

		rc = m_pTxtControls[TXT_DESC]->GetSizeRect();
		m_TextRender.SetRect(rc);
		m_TextRender.Render();
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiHelpWindow::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if( g_pHero && g_pHero->IsShowFullMap() )
			return FALSE;

		if(!GetManager())
			return FALSE;

		if(!GAMEOPTION->GetShowHelp())
		{
			ShowInterface(FALSE);
			return FALSE;
		}

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
void uiHelpWindow::NetworkProc( MSG_BASE * pMsg )
{
	
}

