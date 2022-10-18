#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "uiEventMan/uiEventMan.h"
#include "ui_XMas_DonationConfirm/ui_XMas_DonationConfirm.h"

#include "ui_XMas_Confirm/ui_XMas_Confirm.h"

#include "Mouse.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "FTextRender.h"
#include "FTextSentence.h"

const int c_Str_Interval = 13;
const BYTE c_btMaxEditNum = 19;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID ui_XMas_DonationConfirm::m_wzID[eDIALOG_XMAS_DONATION_CONFIRM_MAX] = 
{
		StrToWzID("LS00"), //eDIALOG_XMAS_DONATION_CONFIRM_TEXT_LIST,
		StrToWzID("ED00"), //eDIALOG_XMAS_DONATION_CONFIRM_EDIT,
		StrToWzID("BT00"), //eDIALOG_XMAS_DONATION_CONFIRM_OK_BTN,
		StrToWzID("B002"), //eDIALOG_XMAS_DONATION_CONFIRM_CANCEL_BTN,
		StrToWzID("B001"), //eDIALOG_XMAS_DONATION_CONFIRM_EXIT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_DonationConfirm::ui_XMas_DonationConfirm(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_DonationConfirm::~ui_XMas_DonationConfirm()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();

	RECT rtMain  = m_ctrlMainList->GetSizeRect();
	m_FtextRender.Init(g_pSunRenderer , rtMain);

	m_bTextLoad = TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}

	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};

	m_ctrlMoneyEdit->GetTextWZ(m_szMoneyOld, INTERFACE_STRING_LENGTH);
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
			m_ctrlMoneyEdit->SetTextWZ(m_szMoneyNew);
		}
		else
		{
			m_ctrlMoneyEdit->SetTextWZ("");
		}
	}
	else
	{
		m_ctrlMoneyEdit->SetTextWZ("");
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::OnRenderSolarDialog()
{
	if(g_pHero)
	{
		if(m_bTextLoad)
		{
			AddFormatText();
			m_bTextLoad = FALSE;
		}

		RECT rt = m_ctrlMainList->GetSizeRect();
		rt.right -= 20;

		m_FtextRender.SetRect(rt);
		m_FtextRender.Render();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		if(m_ctrlMoneyEdit)
			m_ctrlMoneyEdit->ClearText();
		m_bShow = TRUE;
		m_bTextLoad = TRUE;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDotype_Event_X_Mas_Donation_confirm;
		msg.wParam=InterfaceManager::DIALOG_EVENT_X_MAS_DONATION_CONFIRM;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDotype_Event_X_Mas_Donation_confirm);
		m_bShow   = FALSE;
		m_bTextLoad = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_XMAS_DONATION_CONFIRM_OK_BTN:
				{
					ui_XMas_Confirm * pXMasConfirm = GET_CAST_DIALOG( ui_XMas_Confirm, IM_EVENT_MANAGER::EVENT_X_MAS_CONFIRM );
					
					if(pXMasConfirm)
					{
						TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
						TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
						m_ctrlMoneyEdit->GetTextWZ(m_szMoneyOld, INTERFACE_STRING_LENGTH);
						int iLen = strlen(m_szMoneyOld);

						if(0 == iLen)
							break; //금액 기입란이 공란이면 스킵 처리

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
						if(checkDigit(szTemp))
						{
							MONEY mTemp = _atoi64(szTemp);

							if(mTemp > g_pHero->GetMoney())
							{
								//6192	하임이 부족합니다.
								g_InterfaceManager.GetInterfaceString(6192, szMessage);
								GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
							}
							else
							{
								pXMasConfirm->SetDonationMoney(m_szMoneyNew);
								pXMasConfirm->SetDlgType(ui_XMas_Confirm::eXMAS_DLG_TYPE_DONATION);
								pXMasConfirm->ShowInterface(TRUE);
							}
						}
						else
						{
							//5835	금액은 숫자로만 입력하십시오.
							g_InterfaceManager.GetInterfaceString(5835, szMessage);
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						}

						OnShowWindow(FALSE);
					}
				}
				break;

			case eDIALOG_XMAS_DONATION_CONFIRM_CANCEL_BTN:
				{
					m_ctrlMoneyEdit->SetTextWZ("");
				}
				break;

			case  eDIALOG_XMAS_DONATION_CONFIRM_EXIT_BTN:
				{
					OnShowWindow(FALSE);
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

void	ui_XMas_DonationConfirm::InitControl()
{
	m_ctrlMainList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_DONATION_CONFIRM_TEXT_LIST));
	assert(m_ctrlMainList);
	m_ctrlMoneyEdit = static_cast<CCtrlDxIMEEditBox*>(getControl(eDIALOG_XMAS_DONATION_CONFIRM_EDIT));
	assert(m_ctrlMoneyEdit);
	if(m_ctrlMoneyEdit)
	{
		m_ctrlMoneyEdit->SetFontID(StrToWzID("st10"));
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_DonationConfirm::AddFormatText()
{
	FTextParser		FParser;
	BOOL			ret;

	RECT rtMain		   = m_ctrlMainList->GetSizeRect();
	rtMain.right -= 20;
	TCHAR szMainFinal[INTERFACE_STRING_LENGTH + 1]	= {0,};
	g_InterfaceManager.GetInterfaceString(203104, szMainFinal);

	//메인 Desc
	FParser.SetDefaultFont(StrToWzID("mn12") , WzColor_RGBA(235,224,200,255 ));
	ret = FParser.Parser(szMainFinal , m_FSentence );

	if( ret )
	{
		m_FtextRender.ClearSentence();
		m_FtextRender.Append(m_FSentence);
		m_FtextRender.SetRect(rtMain);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool ui_XMas_DonationConfirm::checkDigit(const TCHAR *str)
{
	int len = lstrlen(str);
	int i;

	for (i=0; i<len; ++i)
	{
		if (str[i]<_T('0') || str[i]>_T('9'))
		{
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
