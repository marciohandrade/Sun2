#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildNotice/uiGuildNotice.h"
#include "uiGuildMan/uiGuildMan.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

WzID uiGuildNotice::m_wzID[DLGPOS_MAX] = 
{
		StrToWzID("B002"),	//	DLGPOS_BTN_OK,		
		StrToWzID("B001"),	//	DLGPOS_BTN_CANCLE,
		StrToWzID("ED00"),	//	DLGPOS_LIST,
		StrToWzID("T001"),  //  DLGPOS_TXT_TITLE
		StrToWzID("BT00"),	//	DLGPOS_BTN_CLOSE,

};
//------------------------------------------------------------------------------
/**
*/
uiGuildNotice::uiGuildNotice(InterfaceManager * pUIMan) : uiBase(pUIMan)
{

}
//------------------------------------------------------------------------------
/**
*/
uiGuildNotice::~uiGuildNotice()
{
	m_bShow = FALSE;
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildNotice::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	m_pEdit = (CCtrlDxIMEEditBox *) getControl(DLGPOS_EDIT);
	assert(m_pEdit);
	m_pEdit->SetFontID( StrToWzID("mn12") );
	m_pEdit->SetLimitText(250);
	m_pEdit->SetLimitLine(12); 

	m_pEdit->SetColorText( RGBA( 255, 255, 255, 255));
	m_pEdit->SetColorCaret( RGBA( 255, 255, 255, 255) );
	m_pEdit->SetColorSelText( RGBA( 255, 255, 255, 255) );

	CCtrlStaticWZ * pStatic = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_TITLE);
	if( pStatic )
	{
		pStatic->ShowWindowWZ(WZ_HIDE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildNotice::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildNotice::OnUpdateSolarDialog()
{
	if( !m_bShow )
	{
		ShowInterface(FALSE);
		return;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildNotice::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildNotice::OnShowWindow(BOOL val)
{
	if( val )
	{
		CCtrlStaticWZ * pStatic = (CCtrlStaticWZ *) getControl(DLGPOS_TXT_TITLE);
		pStatic->ShowWindowWZ(WZ_SHOW);

		m_pEdit->ClearText();
		m_bShow = TRUE;

	}
	else
	{
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildNotice::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_BTN_OK:
				{
					SendGuildNoticeChange();
					m_bShow = FALSE;
				}
				break;

			case DLGPOS_BTN_CLOSE:
			case DLGPOS_BTN_CANCLE:
				{
					m_bShow = FALSE;
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
void uiGuildNotice::NetworkProc(MSG_BASE * pMsg)
{

}

void uiGuildNotice::SendGuildNoticeChange()
{
	uiGuildMan * pGuildMan = GET_DIALOG_MANAGER( uiGuildMan, UIMAN_GUILD );
	if( pGuildMan == NULL )
		return;

	WCHAR wszGuildNotice[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szGuildNotice[INTERFACE_STRING_LENGTH] = {0,};

	m_pEdit->GetTextWZW( wszGuildNotice, INTERFACE_STRING_LENGTH );
	GlobalFunc::ChangeMultiEditInput(wszGuildNotice, szGuildNotice);
	pGuildMan->Notice(szGuildNotice);
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
