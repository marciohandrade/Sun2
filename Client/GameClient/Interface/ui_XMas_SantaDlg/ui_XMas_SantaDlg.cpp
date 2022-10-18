#include "SunClientPrecompiledHeader.h"

#include "InterfaceManager.h"
#include "uiEventMan/uiEventMan.h"
#include "ui_XMas_SantaDlg/ui_XMas_SantaDlg.h"

#include "ui_XMas_SantaReward/ui_XMas_SantaReward.h"

#include "GlobalFunc.h"
#include "Mouse.h"
#include "FTextRender.h"
#include "FTextSentence.h"

const int c_Str_Interval = 13;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID ui_XMas_SantaDlg::m_wzID[eDIALOG_XMAS_SANTA_DLG_MAX] = 
{
	StrToWzID("LS00"), //		eDIALOG_XMAS_SANTA_DLG_TEXT_LIST,
	StrToWzID("BT01"), //		eDIALOG_XMAS_SANTA_DLG_OK_BTN,
	StrToWzID("BT00"), //		eDIALOG_XMAS_SANTA_DLG_EXIT_BTN,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_SantaDlg::ui_XMas_SantaDlg(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ui_XMas_SantaDlg::~ui_XMas_SantaDlg()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaDlg::Init(CDrawBase* pDrawBase)
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

void ui_XMas_SantaDlg::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaDlg::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaDlg::OnRenderSolarDialog()
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

void ui_XMas_SantaDlg::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaDlg::OnShowWindow(BOOL val)
{
	if( val )
	{
		MoveCenter();
		m_bShow = TRUE;
		m_bTextLoad = TRUE;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDotype_Event_X_Mas_SantaDlg;
		msg.wParam=InterfaceManager::DIALOG_EVENT_X_MAS_SANTADLG;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDotype_Event_X_Mas_SantaDlg);
		m_bShow   = FALSE;
		m_bTextLoad = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaDlg::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_XMAS_SANTA_DLG_OK_BTN:
				{
					ui_XMas_SantaReward * pSantaReward = GET_CAST_DIALOG( ui_XMas_SantaReward, IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_REWARD );
					
					if( pSantaReward )
					{
						pSantaReward->ShowInterface(TRUE);
					}

					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_XMAS_SANTA_DLG_EXIT_BTN:
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

void	ui_XMas_SantaDlg::InitControl()
{
	m_ctrlMainList = static_cast<CCtrlListWZ*>(getControl(eDIALOG_XMAS_SANTA_DLG_TEXT_LIST));
	assert(m_ctrlMainList);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void ui_XMas_SantaDlg::AddFormatText()
{
	FTextParser		FParser;
	BOOL			ret;

	RECT rtMain		   = m_ctrlMainList->GetSizeRect();
	rtMain.right -= 20;
	TCHAR szMainFinal[INTERFACE_STRING_LENGTH + 1]	= {0,};
	g_InterfaceManager.GetInterfaceString(203103, szMainFinal);

	//∏ﬁ¿Œ Desc
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
