#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiChangeRoomName/uiChangeRoomName.h"
#include "GlobalFunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GameConst_Color.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiChangeRoomName::m_wzID[eDIALOG_CHANGE_ROOM_NAME_MAX] = 
{
	StrToWzID("TI00"),//eDIALOG_CHANGE_ROOM_NAME_OK_BTN,
	StrToWzID("TI01"),//eDIALOG_CHANGE_ROOM_NAME_CANCEL_BTN,
	StrToWzID("E000"),//eDIALOG_CHANGE_ROOM_NAME_EDIT,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiChangeRoomName::uiChangeRoomName()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiChangeRoomName::~uiChangeRoomName()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::Init(CDrawBase* pDrawBase)
{
	memset(m_szRoomTitle, 0, MAX_ROOMTITLE_LENGTH);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();

		if(m_pEdit)
		{
			m_pEdit->SetFocusWZ();
			m_pEdit->SetColorCaret(c_InterfaceCaretColor);
		}

		m_bShow = TRUE;
	}
	else
	{
		m_bShow   = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_CHANGE_ROOM_NAME_OK_BTN:
				{
					OnOKBtnDown();
				}
				break;

			case eDIALOG_CHANGE_ROOM_NAME_CANCEL_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_CHANGE_ROOM_NAME_EDIT:
				{
				}
				break;
			}
		}
		break;

	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_CHANGE_ROOM_NAME_EDIT:
				{
					OnOKBtnDown();
				}
				break;
			}
		}
		break;

	default:
		{
		}
		break;

	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::NetworkProc( MSG_BASE * pMsg )
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void	uiChangeRoomName::InitControl()
{
	m_pOkBtn	 = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_CHANGE_ROOM_NAME_OK_BTN));
	assert(m_pOkBtn);
	m_pCancelBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_CHANGE_ROOM_NAME_CANCEL_BTN));
	assert(m_pCancelBtn);
	m_pEdit		 = static_cast<CCtrlDxIMEEditBox*>(getControl(eDIALOG_CHANGE_ROOM_NAME_EDIT));
	assert(m_pEdit);
	if(m_pEdit)
		m_pEdit->SetLimitText(MAX_ROOMTITLE_LENGTH - 1);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiChangeRoomName::OnOKBtnDown( void )
{
	assert( m_pEdit );

	GlobalFunc::GetText(m_pEdit, m_szRoomTitle, MAX_ROOMTITLE_LENGTH - 1);
	int iRoomTitleLen = strlen(m_szRoomTitle);

	if(0 == iRoomTitleLen)
	{
		//1422 방 제목을 입력하세요.
		TCHAR szMessage[INTERFACE_STRING_LENGTH+1] = {0,};
		g_InterfaceManager.GetInterfaceString(1422, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
	}
	else
	{
		MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN SendPacket;
		memset(SendPacket.m_pszRoomTitle, 0, MAX_ROOMTITLE_LENGTH);
		StrnCopy(SendPacket.m_pszRoomTitle, m_szRoomTitle, MAX_ROOMTITLE_LENGTH - 1);
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

		m_pEdit->ClearText();
		OnShowWindow(FALSE);
	}
}
