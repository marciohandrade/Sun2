#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildDonateUP.h"
#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"

#include "GlobalFunc.h"

const BYTE c_btMaxEditNum = 19;

WzID uiGuildDonateUP::m_wzId[DLGPOS_MAX] = 
{
	StrToWzID("B001"),
	StrToWzID("B002"),
	StrToWzID("B003"),
	StrToWzID("ED00"),
};

//////////////////////////////////////////////////////////////////////////
void _Callback_Guild_Donation_UP(bool bYes)
{
	if(bYes)
	{
		uiGuildDonateUP* pDlg = GET_CAST_DIALOG( uiGuildDonateUP, IM_GUILD_MANAGER::GUILD_DONATE_UP );

		if(pDlg)
		{
			MSG_CG_GUILD_DONATE_UP_SYN SendMsg;
			SendMsg.m_DonateUP	= pDlg->GetDonateUP();
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof (SendMsg));
			pDlg->ResetDonateUP();
			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_DONATE_UP_SYN"));
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
uiGuildDonateUP::uiGuildDonateUP(InterfaceManager * pUIMan) :
uiBase(pUIMan) ,
m_bControlInitialized(FALSE)
{
	this->m_pGuildMan = static_cast<uiGuildMan *> ( this->GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD) );
	assert( m_pGuildMan);
}

//------------------------------------------------------------------------------
/**
*/
uiGuildDonateUP::~uiGuildDonateUP()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_pEdit = static_cast<CCtrlDxIMEEditBox * > ( getControl(DLGPOS_EDIT_UPOINT));
	assert(m_pEdit);

	m_pEdit->SetFontID(StrToWzID("mn12"));

	m_bControlInitialized = TRUE;

	m_donateUP = 0;
	m_bShow = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::Release()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::MessageProc( SI_MESSAGE * pMessage )
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_BTN_CANCLE:
			case DLGPOS_BTN_EXIT:
				{
					m_bShow = FALSE;
					m_donateUP = 0;
				}
				break;
			case  DLGPOS_BTN_DONATE:
				{
					UPProcess();
					if(m_donateUP > 0)
					{
						int OwnedUP = 0;
						if( g_pHero)
						{
							BASE_PLAYERINFO *	pPlayInfo = g_pHero->GetCharInfo();

							if( pPlayInfo)
							{	
								OwnedUP = pPlayInfo->m_UserPoint;
								if( m_donateUP > 0 &&  OwnedUP >= m_donateUP )
								{
									TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
									TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
									//7209 %s UP를 정말 상납하시겠습니까?
									g_InterfaceManager.GetInterfaceString( 7209, szTemp);

									Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, m_szMoneyNew);

									GlobalFunc::SysConfirmBox( szMessage, _Callback_Guild_Donation_UP );
								}
								else
								{
									GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 5185 ) );
								}
							}
						}
						m_pEdit->ClearText();
						m_bShow = FALSE;
					}
					else
					{
						TCHAR szMessage[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString(eST_INPUT_NUMBER , szMessage , INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage );
						m_pEdit->ClearText();
					}
				}
				break;

			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::Process(DWORD dwTick)
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::OnShowWindow( BOOL val )
{
	if( val)
	{
		assert (this->m_bControlInitialized);
		if (!this->m_bControlInitialized)
		{
			this->ShowInterface(FALSE);
			return;
		}
		m_bShow = TRUE;
	}
	else
	{
		m_pEdit->ClearText();
		m_bShow = FALSE;

	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildDonateUP::OnUpdateSolarDialog()
{
	if( !m_bShow)
	{
		this->ShowInterface(FALSE);
		return;
	}

	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};

	m_pEdit->GetTextWZ(m_szMoneyOld, INTERFACE_STRING_LENGTH);
	int iLen = strlen(m_szMoneyOld);
	int j = 0;
	for(int i = 0; i < iLen; ++i, ++j)
	{
		if(',' != m_szMoneyOld[j])
		{
			szTemp[i] = m_szMoneyOld[j];
		}
		else 
		{
			++j;
			szTemp[i] = m_szMoneyOld[j];
		}
	}
	if(isdigit(szTemp[0]))
	{
		if(c_btMaxEditNum > strlen(szTemp)) //에디트 컨트롤 범위 넘지 않게
		{
			NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
			::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szTemp, &nFmt, m_szMoneyNew, sizeof(m_szMoneyNew));
			m_pEdit->SetTextWZ(m_szMoneyNew);
		}
		else
		{
			m_pEdit->SetTextWZ("");
		}
	}
	else
	{
		m_pEdit->SetTextWZ("");
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void uiGuildDonateUP::UPProcess()
{
	TCHAR	szMessage[MAX_MESSAGE_LENGTH] = {0,};
	TCHAR	pszUP[128] = {0,};

	// 금액의 최대 사이즈(128 byte)를 넘었습니다.
	if(!GlobalFunc::CompareLimitText(m_pEdit, pszUP, 128, 5794))
		return;

	if (lstrlen(pszUP)==0)
	{
		//	금액을 입력해주세요!!
		g_InterfaceManager.GetInterfaceString( eST_INPUT_NUMBER, szMessage, MAX_MESSAGE_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		
	}
	else
	{
		TCHAR szTemp[128] = {0,};
		int iLen = strlen(pszUP);
		int j = 0;
		for(int i = 0; i < iLen; ++i, ++j)
		{
			if(',' != pszUP[j])
			{
				szTemp[i] = pszUP[j];
			}
			else 
			{
				++j;
				szTemp[i] = pszUP[j];
			}
		}
		if(this->IsDigit(szTemp))
		{
			m_donateUP = (UPTYPE)atoi(szTemp);

			SolarDialog::ShowWindow(FALSE);
		}
		else
		{
			//	금액은 숫자로만 입력하십시오
			g_InterfaceManager.GetInterfaceString( eST_INPUT_NUMBER, szMessage, MAX_MESSAGE_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
	}
}

BOOL uiGuildDonateUP::IsDigit(const TCHAR * pszMessage)
{
	int len = lstrlen(pszMessage);
	int i;

	for (i=0; i<len; ++i)
	{
		if (pszMessage[i]<_T('0') || pszMessage[i]>_T('9'))
		{
			return FALSE;
		}
	}

	return TRUE;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
