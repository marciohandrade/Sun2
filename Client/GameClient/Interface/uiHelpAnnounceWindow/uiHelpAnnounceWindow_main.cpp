#include "SunClientPrecompiledHeader.h"

#include "uiHelpAnnounceWindow.h"
#include "uiHelpMan/uiHelpMan.h"
#include "GraphicResourceList.h"
#include "Hero.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "BattleScene.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiRadarMan/uiRadarMan.h"

#include "uiChatMan/uiChatMan_def.h"
#include "SystemChatDialog.h"
#include "ChatDialog.h"

const int	c_iDefaultShowTime	= 15000;
const int	c_iBottomGap = 70;
const int	c_iRightGap = 10;

//------------------------------------------------------------------------------
WzID uiHelpAnnounceWindow::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("T001"),	//DLGPOS_TXT_TITLE
		StrToWzID("T002"),  //DLGPOS_TXT_DESC1
		StrToWzID("T003"),  //DLGPOS_TXT_DESC2
		StrToWzID("PI00"),	//DLGPOS_IMAGE
		
};

//------------------------------------------------------------------------------
/**
*/
uiHelpAnnounceWindow::uiHelpAnnounceWindow(InterfaceManager *pUIMan) :
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

    memset ((CCtrlStaticWZ *)this->m_pTxtControls, NULL, sizeof (this->m_pTxtControls));
    m_pPicture = NULL;

	m_hHelpImage = NULL;
	m_iSec = 0;
	m_iOldSec = 0;
	m_iShowTime = c_iDefaultShowTime;
}

//------------------------------------------------------------------------------
/**
*/
uiHelpAnnounceWindow::~uiHelpAnnounceWindow()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::Release()
{
	if(m_hHelpImage)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hHelpImage );
        m_hHelpImage = NULL;
	}

    m_vPicControls.clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::InitControls()
{
    m_pPicture =
        static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_IMAGE));
    assert (this->m_pPicture);

    int i;

    for (i=0; i<POS_TXT_SIZE; ++i)
    {
        this->m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TXT_TITLE + i));
        assert (this->m_pTxtControls[i]);
    }

    int numControls = GetDialogWZ()->GetControlCount();
    for (i=0; i<numControls; ++i)
    {
        CControlWZ* pControl = GetDialogWZ()->FindControlWZ(i);
        if (!pControl)
            continue;

        if (!(pControl->GetKind() & CTRL_KIND_PICTURE))
            continue;

        m_vPicControls.push_back(pControl);
    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiHelpAnnounceWindow::OnShowWindow( BOOL val )
{
	uiBase * pDlg = GET_DIALOG(IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG);//(SystemChatDialog * ) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SYSTEM_CHAT);
    if (val)
    {
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

		for (int i=1; i<POS_TXT_SIZE; ++i)
		{
			if(m_pTxtControls[i])
				GlobalFunc::SetCaption (m_pTxtControls[i], _T(""));
		}

		UpdatePosition();

		m_iSec = 0;
		m_iOldSec = 0;
		if( pDlg && pDlg->IsVisibleWindow() == TRUE)
		{
			pDlg->ShowWindow(FALSE);
		}
    }
    else
    {


    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiHelpAnnounceWindow::Process(DWORD dwTick)
{
    if (!this->IsVisible())
        return;

    m_dwTransAcc += dwTick;

    if (m_bTrans)
    {
        if (m_dwTransAcc >= m_dwTransEnd)
        {
            float fPercent =
                ((float)(m_dwTransAcc - m_dwTransEnd) / (float)m_dwTransEnd);

            float fTrans = 1.0f - fPercent;
            if( fTrans < 0.0f )
                fTrans = 0.0f;

            m_fTrans = 1.0f - (fTrans * 1.0f);
        }

        
    }
    else
    {
        if (m_dwTransAcc >= m_dwTransEnd)
        {
            float fPercent =
                ((float)(m_dwTransAcc - m_dwTransEnd) / (float)m_dwTransEnd);

            float fTrans = 1.0f - fPercent;
            if( fTrans < 0.0f )
                fTrans = 0.0f;

            m_fTrans = fTrans;
        }

        if (m_fTrans <= 0.0f)
        {
            m_bTrans = true;
            ShowInterface(FALSE);
			ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
			SystemChatDialog * pSystemChatDlg = GET_CAST_DIALOG(SystemChatDialog, IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG);
			if( pDlg && pDlg->GetShowSystemChat() && g_InterfaceManager.IsShowGameInterface() )
			{
				if(pSystemChatDlg)
				{
					pSystemChatDlg->ShowWindow(TRUE);
				}
			}
        }
    }

    GetDialogWZ()->SetTransparency(m_fTrans);
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::UpdatePosition()
{
    ////SolarDialog::UpdatePosition();

	int iWidth, iHeight;
	
	RECT rc;
	GetDialogWZ()->GetClientRect(&rc);
	
	iWidth	= rc.right - rc.left;
	iHeight = rc.bottom - rc.top;

	POINT pt = g_pApplication->Get2DModeMapping();
	
	RECT drc;
	if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
		pDlg->GetDialogWZ()->GetClientRect(&drc);

		int iTop = pt.y - (iHeight + c_iBottomGap);
		GetDialogWZ()->MoveWindowWZ((float)(drc.right - iWidth), (float)(iTop)); 
	}
	else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
		pDlg->GetDialogWZ()->GetClientRect(&drc);

		int iTop = pt.y - (iHeight + c_iBottomGap);
		GetDialogWZ()->MoveWindowWZ((float)(drc.right - iWidth), (float)(iTop)); 
	}
	else
	{
		int iTop = pt.y - (iHeight + c_iBottomGap);
		int iLeft = pt.x - (iWidth + c_iRightGap);
		GetDialogWZ()->MoveWindowWZ((float)(iLeft), (float)(iTop)); 
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiHelpAnnounceWindow::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(!GetManager())
			return FALSE;

		if(m_HelpInfo.m_dw_CharStatus != eHK_GM_MSG)
		{
			if(!GAMEOPTION->GetShowHelp())
			{
				ShowInterface(FALSE);
				return FALSE;
			}
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
void uiHelpAnnounceWindow::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;

	if( m_ShowTimer.IsExpired() )
	{
		if(this->IsVisible())
		{
            m_bTrans = false;
            m_fTrans = 1.0f;
            m_dwTransAcc = 0;
            m_dwTransEnd = (SEC_PER_TICK);
		}
	}

	if((m_HelpInfo.m_dw_CharStatus == eHK_GM_MSG) && m_iSec)// GM 메시지
	{
		float fRemainRate, fRemainTime;
		int iRemainTime;

		if(!m_Counter.IsExpired())
		{
			fRemainRate = 1.0f - m_Counter.GetProgressRatio();
			fRemainTime = m_iSec * fRemainRate * 1000;

			iRemainTime = int( (fRemainTime / 1000) );
		}
		else
		{
			iRemainTime = 0;
			m_iOldSec = 0;
		}

		if(m_iOldSec && (m_iOldSec != iRemainTime))
		{
			m_iOldSec = iRemainTime;

			FTextParser parser;
			FTextSentence Sentence;

			RECT rc = m_pTxtControls[2]->GetSizeRect();
			
			m_TextRender[1].Init(g_pSunRenderer, rc);
			m_TextRender[1].SetRect(rc);	

			m_TextRender[1].ClearSentence();

			parser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
			
			TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR	szString2[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_DescStringCode[1], szString, INTERFACE_STRING_LENGTH );

			BOOL bRet;
			Snprintf(szString2, INTERFACE_STRING_LENGTH-1, szString, iRemainTime);
			bRet = parser.Parser(szString2, Sentence);

			if(bRet)
			{
				m_TextRender[1].Append(Sentence);
			}

			m_TextRender[1].SetBeginLineIndex(0);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::OnRenderSolarDialog()
{
	renderHelpInfo();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::_dlgSettingforTrans()
{
	m_ShowTimer.SetTimer(m_iShowTime);
    m_bTrans = true;
    m_fTrans = 0;
    m_dwTransAcc = 0;
    m_dwTransEnd = (SEC_PER_TICK);
}

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
//------------------------------------------------------------------------------
void uiHelpAnnounceWindow::RestoreHelpInfo()
{
    if (IsVisible())
    {
        SetHelpInfo(m_HelpInfo);
    }
}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW


//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::SetHelpInfo(BASE_HelpInfo& helpInfo)
{
	m_iShowTime = c_iDefaultShowTime;
	_dlgSettingforTrans();

	m_HelpInfo = helpInfo;
	
	//제목은 일반 텍스트로 출력하고, 
	TCHAR	szTitle[INTERFACE_STRING_LENGTH] = {0,};	
	g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_TitleStringCode, szTitle, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption (m_pTxtControls[DLGPOS_TXT_TITLE], szTitle);

	//나머지 내용은 정규화 텍스트로 출력
	for(int i=0; i<POS_DESC_SIZE; ++i)
	{
		RECT rc = m_pTxtControls[i+1]->GetSizeRect();
		m_TextRender[i].Init(g_pSunRenderer, rc);
		m_TextRender[i].SetRect(rc);	
		m_TextRender[i].ClearSentence();

		if (0 == m_HelpInfo.m_dw_DescStringCode[i])
		{			
			continue;
		}

		FTextParser parser;
		FTextSentence Sentence;
		parser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
		
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_DescStringCode[i], szString, INTERFACE_STRING_LENGTH );

		BOOL bRet = parser.Parser(szString, Sentence);
		if(bRet)
		{
			m_TextRender[i].Append(Sentence);
		}

		m_TextRender[i].SetBeginLineIndex(0);
	}

	for (size_t j=0; j<m_vPicControls.size(); ++j)
    {
        m_vPicControls[j]->ShowWindowWZ(WZ_HIDE);
    }

    CODETYPE dwImageCode = (CODETYPE)m_HelpInfo.m_dw_ImageCode;
    if (dwImageCode && !m_vPicControls.empty())
    {
        CODETYPE dwImageCodeReal = dwImageCode- 1;
        if (m_vPicControls.size() <= dwImageCodeReal)
        {
            /* empty */
        }
        else
        {
            for (size_t j=0; j<m_vPicControls.size(); ++j)
            {
                m_vPicControls[j]->ShowWindowWZ(WZ_HIDE);
            }

            m_vPicControls[dwImageCodeReal]->ShowWindowWZ(WZ_SHOW);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::SetGMHelpInfo(BASE_HelpInfo& helpInfo, int iParam)
{
	m_iShowTime = (iParam) * 1000;
	_dlgSettingforTrans();


	m_iOldSec = iParam;
	m_iSec = iParam;

	m_HelpInfo = helpInfo;
	
	//제목은 일반 텍스트로 출력하고, 
	TCHAR	szTitle[INTERFACE_STRING_LENGTH] = {0,};	
	g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_TitleStringCode, szTitle, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption (m_pTxtControls[DLGPOS_TXT_TITLE], szTitle);

	//나머지 내용은 정규화 텍스트로 출력
	for(int i=0; i<POS_DESC_SIZE; ++i)
	{
		FTextParser parser;
		FTextSentence Sentence;

		RECT rc = m_pTxtControls[i+1]->GetSizeRect();
		
		m_TextRender[i].Init(g_pSunRenderer, rc);
		m_TextRender[i].SetRect(rc);	

		m_TextRender[i].ClearSentence();

		parser.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
		
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szString2[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString( m_HelpInfo.m_dw_DescStringCode[i], szString, INTERFACE_STRING_LENGTH );

		BOOL bRet;
		if(i==0)
		{
			bRet = parser.Parser(szString, Sentence);
		}
		else
		{
			m_Counter.SetTimer(m_iSec * 1000);
			Snprintf(szString2, INTERFACE_STRING_LENGTH-1, szString, m_iSec);
			bRet = parser.Parser(szString2, Sentence);

		}
		
		if(bRet)
		{
			m_TextRender[i].Append(Sentence);
		}

		m_TextRender[i].SetBeginLineIndex(0);
	}

	for (size_t j=0; j<m_vPicControls.size(); ++j)
    {
        m_vPicControls[j]->ShowWindowWZ(WZ_HIDE);
    }

    CODETYPE dwImageCode = (CODETYPE)m_HelpInfo.m_dw_ImageCode;
    if (dwImageCode && !m_vPicControls.empty())
    {
        CODETYPE dwImageCodeReal = dwImageCode- 1;
        if (m_vPicControls.size() <= dwImageCodeReal)
        {
            /* empty */
        }
        else
        {
            m_vPicControls[dwImageCodeReal]->ShowWindowWZ(WZ_SHOW);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::renderHelpInfo()
{
	if (!_isEnable())
		return;
	

	RECT rc = {0,};

    if(m_HelpInfo.m_dw_HelpIndex)
    {
        for(int i=0; i<POS_DESC_SIZE; ++i)
        {
            rc = m_pTxtControls[i+1]->GetSizeRect();
            m_TextRender[i].SetRect(rc);
            m_TextRender[i].Render();
            m_TextRender[i].SetAlphaModulate(TRUE);
            BYTE fadeValue = (BYTE)(m_fTrans*255.0f);
            m_TextRender[i].SetAlphaValue(fadeValue);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::_renderHelpImage()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpAnnounceWindow::NetworkProc( MSG_BASE * pMsg )
{
}
