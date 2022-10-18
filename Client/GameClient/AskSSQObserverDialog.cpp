#include "SunClientPrecompiledHeader.h"
#include "AskSSQObserverDialog.h"
#include "GlobalFunc.h"
#include <ssq/SSQInfoStruct.h>
#include "FTextParser.h"
#include "FTextSentence.h"


AskSSQObserverDialog::AskSSQObserverDialog(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{
}

AskSSQObserverDialog::~AskSSQObserverDialog(void)
{
}

void AskSSQObserverDialog::Init(CDrawBase* pDrawBase)
{
	AskDialog::Init(pDrawBase);

	//////////////////////////////////////////////////////////////////////////
	// 버튼텍스트 세팅
	if (m_pOKButton)
	{
		m_pOKButton->SetTextID(858);
#ifdef _INTERNATIONAL_UI
		m_pOKButton->SetFontID(StrToWzID("st10"));
#endif//_INTERNATIONAL_UI
	}
	if (m_pCancelButton)
	{
		m_pCancelButton->SetTextID(859);
#ifdef _INTERNATIONAL_UI
		m_pCancelButton->SetFontID(StrToWzID("st10"));
#endif//_INTERNATIONAL_UI
	}

	// 정규화 텍스트 세팅
	if (m_pListCtrl)
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0, };
		FTextParser parser;
		FTextSentence Sentence;
		RECT rcCtr = m_pListCtrl->GetSizeRect();		

		m_FtextRender.Init(GetDrawBase(), rcCtr);
		m_FtextRender.SetVCenterAlign(true);
		m_FtextRender.SetHCenterAlign(true);
		m_FtextRender.SetGap(FTextRender::DEFAULT_CHARACTER_GAP, 4);
		m_FtextRender.ClearSentence();

		g_InterfaceManager.GetInterfaceString(853, szMessage);
		parser.SetDefaultFont(StrToWzID("md12"), WzColor_RGBA(240,108,0,255));
		BOOL bRet = parser.Parser(szMessage, Sentence);
		if(bRet)
		{
			m_FtextRender.Append(Sentence);
		}

		// 새로운 스트링코드로
		g_InterfaceManager.GetInterfaceString(825, szMessage);
#ifdef _INTERNATIONAL_UI
		parser.SetDefaultFont(StrToWzID("st10"), WzColor_RGBA(240,201,121,255));
#else
		parser.SetDefaultFont(StrToWzID("mn12"), WzColor_RGBA(240,201,121,255));
#endif//_INTERNATIONAL_UI		
		bRet = parser.Parser(szMessage, Sentence);
		if(bRet)
		{
			m_FtextRender.Append(Sentence);
		}
	}
}

void AskSSQObserverDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		m_status = ASK_STATUS_NORMAL;

		SolarDialog::MoveCenter();
		// FTextRender Rect 세팅
		if (m_pListCtrl)
		{
			RECT rcCtr = m_pListCtrl->GetSizeRect();
			m_FtextRender.SetRect(rcCtr);		
		}		

		GetUIMan()->AttachFirstDialog(GetDialogWZ()->GetDlgID());

		OffLock();
		OffPending();		
	}
	else
	{
		GetUIMan()->UnLockDlgs();
		GetUIMan()->DeAttachFirstDialog(GetDialogWZ()->GetDlgID());
	}
}

void AskSSQObserverDialog::MessageProc( SI_MESSAGE * pMessage )
{
	if (this->IsLock())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case ASK_OK:
				{
					//관전모드 처리
					m_status = ASK_STATUS_OK;
				}
				break;

			case ASK_CANCEL:
				{
					m_status = ASK_STATUS_CANCEL;
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

void AskSSQObserverDialog::OnUpdateSolarDialog()
{
	if (ASK_STATUS_OK == m_status)
	{
		GlobalFunc::SendObserverPacket( nsSSQ::Observer::Info::eRequest_Observer );
		ShowInterface(FALSE);
	}
	else if (ASK_STATUS_CANCEL == m_status)
	{
		GlobalFunc::SendObserverPacket( nsSSQ::Observer::Info::eRequest_Leave );
		ShowInterface(FALSE);
	}
}

void AskSSQObserverDialog::OnRenderSolarDialog()
{
	if (IsVisibleWindow())
	{
		m_FtextRender.Render();
	}	
}
