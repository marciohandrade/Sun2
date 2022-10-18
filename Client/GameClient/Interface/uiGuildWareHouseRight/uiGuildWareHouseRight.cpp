#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMain/uiGuildMain.h"
#include "uiGuildWareHouseRight/uiGuildWareHouseRight.h"
#include "InterfaceManager.h"
#include "KeyQueueManager.h"
#include "GlobalFunc.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiGuildWareHouseRight::m_wzID[eDIALOG_GUILD_WAREHOUSE_RIGHT_MAX] = 
{
		StrToWzID("C100"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN0,
		StrToWzID("CT00"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN1,
		StrToWzID("CT01"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN2,
		StrToWzID("CT02"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN3,
		StrToWzID("CT03"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN4,
		StrToWzID("CT04"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN5,
		StrToWzID("CT05"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN6,

		StrToWzID("B100"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_EXIT_BTN,
		StrToWzID("B101"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_OK_BTN,
		StrToWzID("B102"), //eDIALOG_GUILD_WAREHOUSE_RIGHT_CANCEL_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildWareHouseRight::uiGuildWareHouseRight(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_btCurCheckState = eGUILD_DUTY_BIT_MAX;

	this->m_pGuildMan =
		static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
	assert (m_pGuildMan);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildWareHouseRight::~uiGuildWareHouseRight()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseRight::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseRight::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseRight::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseRight::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseRight::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;

		SetCheckBtn();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_WareHouse_Right;
		msg.wParam = InterfaceManager::DIALOG_GUILD_WAREHOUSE_RIGHT;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_WareHouse_Right);
		m_bShow   = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildWareHouseRight::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_EXIT_BTN:
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CANCEL_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN0:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_MASTER;
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN1:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_SUBMASTER;
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN2:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_STRATEGIST;
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN3:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_COMMANDER;
				}
				break;

			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN4:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_CAMP_LEADER;
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN5:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_REGULAR_SOLDIER;
				}
				break;
			case eDIALOG_GUILD_WAREHOUSE_RIGHT_CHECK_BTN6:
				{
					m_btCurCheckState ^= eGUILD_DUTY_BIT_CAMP_SOLDIER;
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

void uiGuildWareHouseRight::SetCheckBtn()
{
	for(int i = 0; i < MAX_GUILD_DUTY_NUM; ++i)
	{
		if(m_btCurCheckState&(1<<i))
			m_pctrlDutyCheckBtn[i]->SetCheckState(TRUE);
		else
			m_pctrlDutyCheckBtn[i]->SetCheckState(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void	uiGuildWareHouseRight::InitControl()
{
	for(int i = 0; i < eMAX_RIGHT_CHECK_BTN_NUM; ++i)
	{
		m_pctrlDutyCheckBtn[i] = (CCtrlButtonCheckWZ*)getControl(i);
		assert(m_pctrlDutyCheckBtn);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
