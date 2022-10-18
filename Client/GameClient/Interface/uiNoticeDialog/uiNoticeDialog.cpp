#include "SunClientPrecompiledHeader.h"
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiNoticeDialog/uiNoticeDialog.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "FTextParser.h"
#include "FTextSentence.h"

const DWORD FadeTime = 1500;

//------------------------------------------------------------------------------
 WzID uiNoticeDialog::m_wzId[DIALOG_MAX] = 
 {
 	StrToWzID("ST00"), 
	StrToWzID("BT00"), 
 };

//------------------------------------------------------------------------------
/**
*/
uiNoticeDialog::uiNoticeDialog(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{
	memset(m_szNoticeMessage, 0, sizeof(m_szNoticeMessage));
	m_pNoticeTextControl = NULL;
	m_fFadeValue = 1.0f;
	m_bCloseForNextMessage = false;
}

//------------------------------------------------------------------------------
/**
*/
uiNoticeDialog::~uiNoticeDialog()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeDialog::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	m_pNoticeTextControl = static_cast<CCtrlStaticWZ*>(GetControlWZ(m_wzId[DLGPOS_NOTICE_TEXT]));
	m_pNoticeTextControl->ShowWindowWZ(WZ_HIDE);
	assert(m_pNoticeTextControl);
	m_pCloseButton = static_cast<CCtrlButtonWZ*>(GetControlWZ(m_wzId[DLGPOS_CLOSE_BTN]));
	assert(m_pCloseButton);

	m_pNoticeMan = (uiNoticeMan*)(GetMan());		
}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeDialog::Release()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		g_InterfaceManager.AttachFirstDialog(IM_NOTICE_MANAGER::GM_NOTICE_DIALOG);

        // UI Inetersect ���� ���� - ������ ���������� UI�� �����Ѵ�.
        RECT EnableRect = this->m_pCloseButton->GetSizeRect();
        STD_UI_INTERSECT_RECT& rect_list = GetDialogWZ()->GetIntersectRectList();
        rect_list.clear();
        rect_list.push_back(EnableRect);

	}
	else
	{
		m_bCloseForNextMessage = false;
		m_Timer.Clear();
		
		g_InterfaceManager.DeAttachFirstDialog(IM_NOTICE_MANAGER::GM_NOTICE_DIALOG);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeDialog::OnRenderSolarDialog()
{
	// �ؽ�Ʈ ����
	if (IsVisibleWindow())
	{
		m_FtextRender.SetAlphaModulate(TRUE);
		m_FtextRender.SetAlphaValue((BYTE)(m_fFadeValue*255.0f));
		m_FtextRender.Render();
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeDialog::OnUpdateSolarDialog()
{
	// ���̵� ��/�ƿ� ó��
	DWORD ProgressTime = m_Timer.GetProcessTime();
	m_fFadeValue = 1.0f;
	if (ProgressTime <= FadeTime)
	{
		m_fFadeValue = (float)(ProgressTime)/FadeTime;		
	}
    if (ProgressTime >= m_dwNoticeTime - FadeTime)
	{
		m_fFadeValue = 1.0f - (float)(ProgressTime-(m_dwNoticeTime - FadeTime))/FadeTime;
	}
	GetDialogWZ()->SetTransparency(m_fFadeValue);

	// �ð��� �����ų� ��ưŬ������ ��� â����
	
	if (m_Timer.IsExpired() || m_bCloseForNextMessage)
	{
		ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool uiNoticeDialog::ShowNoticeMessage(const TCHAR* pNoticeMessage, int DisplayTime)
{
	// ���� �����ֱ�	
	ShowInterface(TRUE);
	m_dwNoticeTime = DisplayTime;

	// ����
	GetDialogWZ()->SetTransparency(0.0f);	

	// ����ȭ �ؽ�Ʈ ����
	FTextParser parser;
	FTextSentence Sentence;
    RECT rcCtr = m_pNoticeTextControl->GetSizeRect();

    m_FtextRender.Init(GetDrawBase(), rcCtr);
	m_FtextRender.SetVCenterAlign(true);
	m_FtextRender.ClearSentence();
	parser.SetDefaultFont(m_pNoticeTextControl->GetFontID(), m_pNoticeTextControl->GetTextColor());
	BOOL bRet = parser.Parser(pNoticeMessage, Sentence);
	if(bRet)
	{
		m_FtextRender.Append(Sentence);
	}
	m_FtextRender.SetBeginLineIndex(0);

	m_Timer.SetTimer(m_dwNoticeTime);

	return true;
}

//------------------------------------------------------------------------------
/**
*/
void uiNoticeDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			if (m_wzId[DLGPOS_CLOSE_BTN] == pMessage->dwCtrlID)
			{
				m_bCloseForNextMessage = true;
			}
		}
		break;
	default:
		{
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
