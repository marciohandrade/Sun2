#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "uiEventMan/uiEventMan.h"
#include "ui_XMas_Confirm/ui_XMas_Confirm.h"

#include "ui_XMas_SoloCoupleReward/ui_XMas_SoloCoupleReward.h"

#include "GlobalFunc.h"
#include "Mouse.h"
#include "FTextRender.h"
#include "FTextSentence.h"

#include "SunEventInfoParser.h"

const DWORD c_Str_Interval = 13;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID ui_XMas_Confirm::m_wzID[eDIALOG_XMAS_CONFIRM_MAX] = 
{
	StrToWzID("T999"),			//eDIALOG_XMAS_CONFIRM_TITLE_TEXT,	
	StrToWzID("LS00"),			//eDIALOG_XMAS_CONFIRM_TEXT_LIST,
	StrToWzID("BT01"),			//eDIALOG_XMAS_CONFIRM_OK_BTN,
	StrToWzID("BT02"),			//eDIALOG_XMAS_CONFIRM_CANCEL_BTN,
	StrToWzID("BT00"),			//eDIALOG_XMAS_CONFIRM_EXIT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_Confirm::ui_XMas_Confirm(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_eDlgType		  = eXMAS_DLG_MAX;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_Confirm::~ui_XMas_Confirm()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::Init(CDrawBase* pDrawBase)
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

void ui_XMas_Confirm::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::OnRenderSolarDialog()
{
	if(g_pHero)
	{
		if(m_bTextLoad)
		{
			AddFormatText();
			m_bTextLoad = FALSE;
		}
		RECT rt = m_ctrlMainList->GetSizeRect();
		rt.right -= 15;
		m_FtextRender.SetRect(rt);
		m_FtextRender.Render();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		m_bShow = TRUE;
		m_bTextLoad = TRUE;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_Event_X_Mas_Confirm;
		msg.wParam=InterfaceManager::DIALOG_EVENT_X_MAS_CONFIRM;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Event_X_Mas_Confirm);
		m_bShow   = FALSE;
		m_bTextLoad = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_XMAS_CONFIRM_OK_BTN:
				{
					switch(m_eDlgType)
					{
					case eXMAS_DLG_TYPE_PERIOD_ITEM:
						{
							ui_XMas_SoloCoupleReward * pSoloCoupleReward = GET_CAST_DIALOG( ui_XMas_SoloCoupleReward, IM_EVENT_MANAGER::EVENT_X_MAS_SOLO_COUPLE_REWARD );
							
							if(pSoloCoupleReward)
							{
								pSoloCoupleReward->SetSoloCoupleState(ui_XMas_SoloCoupleReward::eSOLO_COUPLE_STATE_SOLO);
								pSoloCoupleReward->ShowInterface(TRUE);
							}

							OnShowWindow(FALSE);
						}
						break;

					case eXMAS_DLG_TYPE_DONATION:
						{
							MoneyProcess();
							MSG_CG_EVENT_DONATION_HEIM_SYN SendMsg;
							EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(c_dw_Donation_NPC_Type);
							//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
							if(NULL != pEventInfoVec)
							{
								EVENT_INFO* pEventInfo = pEventInfoVec->back();
								
								SendMsg.m_EventID = pEventInfo->m_dwEventID; //기부관련 아이디(SunEventInfo 참조)
								SendMsg.m_DonatedHeims = m_CurMoney;
								GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
							}

							OnShowWindow(FALSE);
						}
						break;

					default:
						{
							assert(!"ui_XMas_Confirm::MessageProc");
						}
						break;
					}
				} //case eDIALOG_XMAS_CONFIRM_OK_BTN:
				break;

			case eDIALOG_XMAS_CONFIRM_CANCEL_BTN:
				{
					switch(m_eDlgType)
					{
					case eXMAS_DLG_TYPE_PERIOD_ITEM:
						{
							ui_XMas_SoloCoupleReward * pSoloCoupleReward = GET_CAST_DIALOG( ui_XMas_SoloCoupleReward, IM_EVENT_MANAGER::EVENT_X_MAS_SOLO_COUPLE_REWARD );
							
							if(pSoloCoupleReward)
							{
								pSoloCoupleReward->SetSoloCoupleState(ui_XMas_SoloCoupleReward::eSOLO_COUPLE_STATE_COUPLE);
								pSoloCoupleReward->ShowInterface(TRUE);
							}

							OnShowWindow(FALSE);							
						}
						break;

					case eXMAS_DLG_TYPE_DONATION:
						{
							OnShowWindow(FALSE);
						}
						break;

					default:
						{
							assert(!"ui_XMas_Confirm::MessageProc");
						}
						break;
					}
				} //case eDIALOG_XMAS_CONFIRM_CANCEL_BTN:
				break;

			case eDIALOG_XMAS_CONFIRM_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			} //getControlIDToPos( pMessage->dwCtrlID )
		} //RT_MSG_LBUTTONCLICK
		break;

	default:
		{
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::InitControl()
{
	m_ctrlTitleStatic = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_XMAS_CONFIRM_TITLE_TEXT));
	assert(m_ctrlTitleStatic);
	if(m_ctrlTitleStatic)
	{
		m_ctrlTitleStatic->SetTextWZ("");
	}
	m_ctrlMainList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_CONFIRM_TEXT_LIST));
	assert(m_ctrlMainList);
	m_ctrlOKBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_XMAS_CONFIRM_OK_BTN));
	assert(m_ctrlOKBtn);
	if(m_ctrlOKBtn)
	{
		m_ctrlOKBtn->SetTextWZ("");
	}
	m_ctrlCancelBtn = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_XMAS_CONFIRM_CANCEL_BTN));
	assert(m_ctrlCancelBtn);
	if(m_ctrlCancelBtn)
	{
		m_ctrlCancelBtn->SetTextWZ("");
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_Confirm::AddFormatText()
{
	FTextParser		FParser;
	BOOL			ret;

	RECT rtTitle       = m_ctrlTitleStatic->GetSizeRect();
	RECT rtMain		   = m_ctrlMainList->GetSizeRect();
	rtMain.right -= 15;
	RECT rtOk		   = m_ctrlOKBtn->GetSizeRect();
	RECT rtCancel      = m_ctrlCancelBtn->GetSizeRect();

	TCHAR szTitle[INTERFACE_STRING_LENGTH + 1]		= {0,};
	TCHAR szMain[INTERFACE_STRING_LENGTH + 1]		= {0,};
	TCHAR szMainFinal[INTERFACE_STRING_LENGTH + 1]	= {0,};
	TCHAR szOK[INTERFACE_STRING_LENGTH + 1]			= {0,};
	TCHAR szCancel[INTERFACE_STRING_LENGTH + 1]		= {0,};

	switch(m_eDlgType)
	{
	case eXMAS_DLG_TYPE_PERIOD_ITEM:
		{
			g_InterfaceManager.GetInterfaceString(1856, szTitle);
			g_InterfaceManager.GetInterfaceString(203102, szMainFinal);
			g_InterfaceManager.GetInterfaceString(1858, szOK);
			g_InterfaceManager.GetInterfaceString(1859, szCancel);
		}
		break;

	case eXMAS_DLG_TYPE_DONATION:
		{
			g_InterfaceManager.GetInterfaceString(1860, szTitle);
			g_InterfaceManager.GetInterfaceString(5322, szMain);
			if(0 >= strlen(m_szDonationMoney))
			{
				assert(!"ui_XMas_Confirm::AddFormatText() - String Error");
			}
			Snprintf(szMainFinal, INTERFACE_STRING_LENGTH, szMain, m_szDonationMoney);
			g_InterfaceManager.GetInterfaceString(1861, szOK);
			g_InterfaceManager.GetInterfaceString(1862, szCancel);
		}
		break;

	case eXMAS_DLG_MAX:
	default:
		{
			assert(!"ui_XMas_Confirm::AddFormatText() - Invalid Type");
		}
		break;
	}

	//타이틀
	if(m_ctrlTitleStatic)
	{
		m_ctrlTitleStatic->SetTextWZ(szTitle);
	}
	//OK
	if(m_ctrlOKBtn)
	{
		m_ctrlOKBtn->SetTextWZ(szOK);
	}
	//CANCEL
	if(m_ctrlCancelBtn)
	{
		m_ctrlCancelBtn->SetTextWZ(szCancel);
	}

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

void ui_XMas_Confirm::MoneyProcess()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};

	if (lstrlen(m_szDonationMoney)==0)
	{
		//	금액을 입력해주세요!!
		g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, MAX_MESSAGE_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		
	}
	else
	{
		TCHAR szTemp[128] = {0,};
		int iLen = strlen(m_szDonationMoney);
		int j = 0;
		for(int i = 0; i < iLen; ++i, ++j)
		{
			if(',' != m_szDonationMoney[j])
			{
				szTemp[i] = m_szDonationMoney[j];
			}
			else 
			{
				++j;
				szTemp[i] = m_szDonationMoney[j];
			}
		}
		if(checkDigit(szTemp))
		{
			m_CurMoney = (MONEY)_ttoi64(szTemp);

			SolarDialog::ShowWindow(FALSE);
		}
		else
		{
			//	금액은 숫자로만 입력하십시오
			g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY_IN_FIGURES, szMessage, MAX_MESSAGE_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool ui_XMas_Confirm::checkDigit(const TCHAR *str)
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
