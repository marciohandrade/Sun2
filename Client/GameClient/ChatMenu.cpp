#include "SunClientPrecompiledHeader.h"
#include "ChatMenu.h"
#include "interfacemanager.h"
WzID ChatMenuDialog::m_wzId[] = 
{
	StrToWzID("C001"),	//eCHATMENU_CHK_NORMAL = 0,
	StrToWzID("C002"),	//eCHATMENU_CHK_GUILD,
	StrToWzID("C003"),	//eCHATMENU_CHK_PARTY,

	StrToWzID("B000"),		//eCHATMENU_BTN_WHISPER,
	StrToWzID("B001"),		//eCHATMENU_BTN_SHOUT,
};	

ChatMenuDialog::ChatMenuDialog(InterfaceManager* pUIMan):
	uiBase(pUIMan),
	m_pChatDialog(NULL)
{

}

ChatMenuDialog::~ChatMenuDialog(void)
{

}


VOID	
ChatMenuDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	this->SetCurType( CHAT_TYPE_GENERAL );
	
	GetControls();
}


VOID	
ChatMenuDialog::Release()
{

}

VOID	
ChatMenuDialog::MessageProc( SI_MESSAGE * pMessage )
{
	if(!m_pChatDialog) return;

	switch( pMessage->eResultMsg ) 
	{
		case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			
			switch (pos)
			{
			case eCHATMENU_CHK_NORMAL:
				{
					m_pChatDialog->UpdateChatInfo(eST_NORMAL_CHAT);
					ShowDialog(FALSE);
				}
				break;

			case eCHATMENU_CHK_GUILD:
				{
					m_pChatDialog->UpdateChatInfo(eST_GUILD_CHAT);
					ShowDialog(FALSE);
				}
				break;

			case eCHATMENU_CHK_PARTY:
				{
					m_pChatDialog->UpdateChatInfo(eST_PARTY_CHAT);
					ShowDialog(FALSE);
				}
				break;

			case eCHATMENU_BTN_WHISPER:
				{
					TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString( 2010, szTemp, INTERFACE_STRING_LENGTH );
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s "), szTemp);
		            m_pChatDialog->SetChatMessage(szMessage);

					ShowDialog(FALSE);
				}
				break;

			case eCHATMENU_BTN_SHOUT:
				{
					TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
					TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
					g_InterfaceManager.GetInterfaceString( eST_CHAT_COMMAND_SHOUT, szTemp, INTERFACE_STRING_LENGTH );
					Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s "), szTemp);
		            m_pChatDialog->SetChatMessage(szMessage);

					ShowDialog(FALSE);
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

VOID	
ChatMenuDialog::NetworkProc( MSG_BASE * pMsg )
{

}

VOID 
ChatMenuDialog::GetControls()
{
	for (int i =0; i < eCHATMENU_MAX; ++i)
	{
		m_pCtrlChatType[i] =static_cast<CCtrlButtonCheckWZ *>(getControl(eCHATMENU_CHK_NORMAL +i));
		assert(m_pCtrlChatType[i]);
	}
}

VOID	
ChatMenuDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		if(m_pChatDialog)
		{
			if(m_pChatDialog->GetFilterMode() == eST_GUILD_CHAT)
			{
				m_pCtrlChatType[eCHATMENU_CHK_NORMAL]->SetCheckState(FALSE);
				m_pCtrlChatType[eCHATMENU_CHK_PARTY]->SetCheckState(FALSE);
				
				m_pCtrlChatType[eCHATMENU_CHK_GUILD]->SetCheckState(TRUE);
			}
			else if(m_pChatDialog->GetFilterMode() == eST_PARTY_CHAT)
			{
				m_pCtrlChatType[eCHATMENU_CHK_NORMAL]->SetCheckState(FALSE);
				m_pCtrlChatType[eCHATMENU_CHK_GUILD]->SetCheckState(FALSE);
				
				m_pCtrlChatType[eCHATMENU_CHK_PARTY]->SetCheckState(TRUE);
			}
			else
			{
				m_pCtrlChatType[eCHATMENU_CHK_GUILD]->SetCheckState(FALSE);
				m_pCtrlChatType[eCHATMENU_CHK_PARTY]->SetCheckState(FALSE);

				m_pCtrlChatType[eCHATMENU_CHK_NORMAL]->SetCheckState(TRUE);
			}
		}
	}
	else
	{
		if (m_pChatDialog && m_pChatDialog->IsVisibleWindow())
		{
			m_pChatDialog->SetFocusChatEdit();
		}
	}
}


CControlWZ *	
ChatMenuDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


POSTYPE			
ChatMenuDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}