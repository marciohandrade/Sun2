#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildConvertGP.h"
#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "GlobalFunc.h"



WzID uiGuildConvertGP::m_wzId[DLGPOS_MAX] =
{
	StrToWzID("B001"),
	StrToWzID("B002"),
	StrToWzID("B003"),
	StrToWzID("B004"),
	StrToWzID("B005"),
	StrToWzID("B006"),
	StrToWzID("T006"),
};

//////////////////////////////////////////////////////////////////////////
void _Callback_Guild_Convert_GP(bool bYes)
{
	if(bYes)
	{
		uiGuildConvertGP * pDlg = GET_CAST_DIALOG( uiGuildConvertGP, IM_GUILD_MANAGER::GUILD_CONVERT_GP );

		if(pDlg)
		{
			MSG_CG_GUILD_CONVERT_UP2GP_SYN send;
			send.m_ConvertGP = pDlg->GetConverGP();
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
			GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_CONVERT_UP2GP_SYN"));
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
uiGuildConvertGP::uiGuildConvertGP(InterfaceManager *pUIMan) : 
	uiBase(pUIMan) ,
	m_bControlsInitialized(FALSE)
{
	this->m_pGuildMan =
		static_cast<uiGuildMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD));
	assert (m_pGuildMan);
}

//------------------------------------------------------------------------------
/**
*/
uiGuildConvertGP::~uiGuildConvertGP()
{
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildConvertGP::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_pStaticGP = static_cast<CCtrlStaticWZ*>(getControl(DLGPOS_TEXT_GP));
	assert(m_pStaticGP);

	m_bControlsInitialized = TRUE;
	m_GPoint = 0;
	m_MaxGPoint = 0;
	m_bShow = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildConvertGP::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildConvertGP::MessageProc( SI_MESSAGE * pMessage )
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch(  getCtrlIDToPos (pMessage->dwCtrlID ) )
			{
			case DLGPOS_BTN_CANCLE:
			case DLGPOS_BTN_EXIT:
				{
					m_bShow = FALSE;
				}
				break;
			case DLGPOS_BTN_CONVERT:
				{
					if( m_GPoint > 0 )
					{
						TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
						TCHAR szUP[INTERFACE_STRING_LENGTH + 1] = {0,};
						TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						Snprintf(szUP, INTERFACE_STRING_LENGTH, _T("%d,000,000"), m_GPoint);
						//7208 %s UP를 %d GP로 정말 변환하시겠습니까?
						g_InterfaceManager.GetInterfaceString( 7208, szTemp);

						Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp, szUP, m_GPoint);

						GlobalFunc::SysConfirmBox( szMessage, _Callback_Guild_Convert_GP );
					}
					m_bShow = FALSE;
				}
				break;
			case DLGPOS_BTN_DOWN:
				{
					_OnDecrementPoint();
				}
				break;
			case DLGPOS_BTN_UP:
				{
					_OnIncrementPoint();
				}
				break;
			case DLGPOS_BTN_MAXIUM:
				{
					_OnMaxUPoint();
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
void uiGuildConvertGP::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildConvertGP::Process(DWORD dwTick)
{

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildConvertGP::OnUpdateSolarDialog()
{
	if( !m_bShow )
	{
		this->ShowInterface(FALSE);
		return;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildConvertGP::OnShowWindow(BOOL val)
{

	if( val )
	{
		assert (this->m_bControlsInitialized);
		if (!this->m_bControlsInitialized)
		{
			this->ShowInterface(FALSE);
			return;
		}

		m_bShow = TRUE;
		_OnShowWindow();
	}
	else
	{
		m_bShow = FALSE;

	}

}

void uiGuildConvertGP::_OnShowWindow()	
{
	
	m_MaxGPoint = GetManager()->GetGuildInfo().m_UserPoint / (UPTYPE)GUILD_UP2GP_RATE;
	if( m_MaxGPoint > 0)
	{
		GlobalFunc::SetCaption(m_pStaticGP, _T("1"));
		m_GPoint = 1;
	}
	else
	{
		GlobalFunc::SetCaption(m_pStaticGP, _T("0"));
		m_GPoint = 0;
	}
}

void uiGuildConvertGP::_OnMaxUPoint()
{
	m_MaxGPoint = GetManager()->GetGuildInfo().m_UserPoint / (UPTYPE)GUILD_UP2GP_RATE;
	m_GPoint = m_MaxGPoint;
	if( m_MaxGPoint > 0)
	{
		_FillAllTextPoint(m_MaxGPoint);
	}
}


void uiGuildConvertGP::_OnIncrementPoint()
{
	m_MaxGPoint = GetManager()->GetGuildInfo().m_UserPoint / (UPTYPE)GUILD_UP2GP_RATE;
	if( m_GPoint < m_MaxGPoint)
	{
		m_GPoint += 1;
		_FillAllTextPoint(m_GPoint);
	}
}


void uiGuildConvertGP::_OnDecrementPoint()
{
	if ( m_GPoint > 0 )
	{
		m_GPoint -= 1;
		_FillAllTextPoint(m_GPoint);
	}
}

void uiGuildConvertGP::_FillAllTextPoint(GPTYPE iGP)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	Sprintf(szMessage, _T("%d"), iGP);
	GlobalFunc::SetCaption(m_pStaticGP, szMessage);
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
