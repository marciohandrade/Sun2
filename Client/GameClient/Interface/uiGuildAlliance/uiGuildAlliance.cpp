#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiGuildAlliance/uiGuildAlliance.h"
#include "uiGuildHostility/uiGuildHostility.h"
#include "uiGuildOneSideHostility/uiGuildOneSideHostility.h"
#include "uiGuildRelationAsk/uiGuildRelationAsk.h"
#include "uiGuildMain/uiGuildMain.h"
#include "GlobalFunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Mouse.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiGuildAlliance::m_wzID[eDIALOG_GUILD_ALLIANCE_MAX] = 
{
		StrToWzID("CT00"),//eDIALOG_GUILD_ALLIANCE_TAB,
		StrToWzID("CT02"),//eDIALOG_GUILD_HOSTILITY_TAB,
		StrToWzID("CT03"),//eDIALOG_GUILD_ONE_SIDE_HOSTILITY_TAB,

		StrToWzID("BT01"),//eDIALOG_GUILD_ALLIANCE_RELATION_BTN_FOR_SORT,
		StrToWzID("BT02"),//eDIALOG_GUILD_ALLIANCE_STATE_BTN_FOR_SORT,
		StrToWzID("BT03"),//eDIALOG_GUILD_ALLIANCE_EXIT_BTN,

		StrToWzID("VS00"),//eDIALOG_GUILD_ALLIANCE_SCROLL,
		StrToWzID("BT04"),//eDIALOG_GUILD_ALLIANCE_APPLY_BTN,
		StrToWzID("BT05"),//eDIALOG_GUILD_ALLIANCE_CANCEL_BTN,

		StrToWzID("LS00"),//eDIALOG_GUILD_ALLIANCE_MAIN_LIST,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildAlliance::uiGuildAlliance(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildAlliance::~uiGuildAlliance()
{
	Release();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}

	UpdateToolTip();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::UpdateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	if(m_pAlianceTab && m_pHostilityTab && m_pOneSideHostilityTab)
	{
		RECT rcAlliance  = m_pAlianceTab->GetSizeRect();
		RECT rcHostility = m_pHostilityTab->GetSizeRect();
		RECT rcOneSideHostility = m_pOneSideHostilityTab->GetSizeRect();

		if(rcAlliance.left <= iMouseX
			&& rcAlliance.right >= iMouseX
			&& rcAlliance.top <= iMouseY 
			&& rcAlliance.bottom >= iMouseY)
		{
			m_eRenderToolTip = eGUILD_RELATION_ALLIENCE;
		}
		else if(rcHostility.left <= iMouseX
			&& rcHostility.right >= iMouseX
			&& rcHostility.top <= iMouseY 
			&& rcHostility.bottom >= iMouseY)
		{
			m_eRenderToolTip = eGUILD_RELATION_BOTH_HOSTILITY;
		}
		else if(rcOneSideHostility.left <= iMouseX
			&& rcOneSideHostility.right >= iMouseX
			&& rcOneSideHostility.top <= iMouseY 
			&& rcOneSideHostility.bottom >= iMouseY)
		{
			m_eRenderToolTip = eGUILD_RELATION_ONE_SIDE_HOSTILITY;
		}	
		else if(1)
		{
			m_eRenderToolTip = eGUILD_RELATION_NONE;
		}

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::RenderTooltip()
{
	uiToolTipMan* pToolTipMan = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
	if(pToolTipMan)
	{
		switch(m_eRenderToolTip)
		{
		case eGUILD_RELATION_ALLIENCE:
			{
				TCHAR szTemp[INTERFACE_STRING_LENGTH+1] = {0,};
				//11321 동맹관계을 맺은 길드
				g_InterfaceManager.GetInterfaceString(11321, szTemp);
				pToolTipMan->RegisterTooltip(szTemp, _tcsclen(szTemp), 25);
				pToolTipMan->SetTooltipDialogID( GetDialogKey() );
			}
			break;
		case eGUILD_RELATION_BOTH_HOSTILITY:
			{
				TCHAR szTemp[INTERFACE_STRING_LENGTH+1] = {0,};
				//11322 적대선포 및 쌍방적대관계를 맺은 길드
				g_InterfaceManager.GetInterfaceString(11322, szTemp);
				pToolTipMan->RegisterTooltip(szTemp, _tcsclen(szTemp), 25);
				pToolTipMan->SetTooltipDialogID( GetDialogKey() );
			}
			break;

		case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
			{
				TCHAR szTemp[INTERFACE_STRING_LENGTH+1] = {0,};
				//11323 일방적 적대 선포를 한 길드
				g_InterfaceManager.GetInterfaceString(11323, szTemp);
				pToolTipMan->RegisterTooltip(szTemp, _tcsclen(szTemp), 25);
				pToolTipMan->SetTooltipDialogID( GetDialogKey() );
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::OnRenderSolarDialog()
{
	RenderTooltip();

	if( GetManager() == NULL )
		return;

	std::list<GUILD_RELATION*>& rAllianceCon = GetManager()->GetAllianceCon();

	if( rAllianceCon.empty() )
		return;

	RECT rc_listctrl;
	RECT rc_GuildName;
	RECT rc_Relation;

	rc_Relation = rc_GuildName = rc_listctrl = this->m_pMainList->GetSizeRect();

	CControlWZ * pButton1 = getControl( eDIALOG_GUILD_ALLIANCE_RELATION_BTN_FOR_SORT );

	rc_GuildName.top += 5;
	rc_Relation.top  += 5;
	rc_GuildName.right = rc_GuildName.left + pButton1->GetSize().right;
	rc_Relation.left   = rc_GuildName.right;

	DWORD dwNameColor = WzColor_RGBA (255, 255, 255, 255);

	std::list<GUILD_RELATION*>::iterator iter;
	std::list<GUILD_RELATION*>::iterator end = rAllianceCon.end();

	for( iter = rAllianceCon.begin(); iter != end; ++iter )
	{
		GUILD_RELATION* pTemp = (*iter);

		TCHAR szGuildName[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szAlliance[INTERFACE_STRING_LENGTH + 1] = {0,};
		//1400 동맹중
		g_InterfaceManager.GetInterfaceString(1400, szAlliance);
		StrnCopy(szGuildName, pTemp->m_GuildName, INTERFACE_STRING_LENGTH);

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "mn12" );

		this->GetDrawBase()->x_pManagerTextOut->DrawText(szGuildName,
			&rc_GuildName,
			dwNameColor,
			WzColor_RGBA(  0,  0,  0,   0),
			TP_HCENTER);

		this->GetDrawBase()->x_pManagerTextOut->DrawText(szAlliance,
			&rc_Relation,
			dwNameColor,
			WzColor_RGBA(  0,  0,  0,   0),
			TP_HCENTER);

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		rc_GuildName.top += 15;
		rc_Relation.top += 15;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::OnShowWindow(BOOL val)
{
	if (val)
	{
		uiGuildMain* pGuildMain = (uiGuildMain *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_GUILD_MAIN);
		RECT rt;
		pGuildMain->GetDialogWZ()->GetClientRect(&rt);
		this->MoveWindowWZ(rt.right, rt.top);

		m_pAlianceTab->SetCheckState(true);
		m_pHostilityTab->SetCheckState(false);
		m_pOneSideHostilityTab->SetCheckState(false);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_Alliance;
		msg.wParam = InterfaceManager::DIALOG_GUILD_ALLIANCE;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = TRUE;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_Alliance);
		m_bShow = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildAlliance::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_GUILD_ALLIANCE_TAB:
				{
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_TAB:
				{
					OnShowWindow(FALSE);

					uiBase * pGuildHostility = GET_DIALOG( IM_GUILD_MANAGER::GUILD_HOSTILITY );
					
					if(pGuildHostility)
					{
						pGuildHostility->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_ONE_SIDE_HOSTILITY_TAB:
				{
					OnShowWindow(FALSE);

					uiBase * pGuildOneSideHostility = GET_DIALOG( IM_GUILD_MANAGER::GUILD_ONE_SIDE_HOSTILITY );
					
					if(pGuildOneSideHostility)
					{
						pGuildOneSideHostility->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_RELATION_BTN_FOR_SORT:
				{
					BOOL bTemp = GetManager()->GetbForSort();
					GetManager()->SortAllianceCon(bTemp);
					bTemp ^= 1;
					GetManager()->SetbForSort(bTemp);
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_STATE_BTN_FOR_SORT:
				{
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_SCROLL:
				{
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_APPLY_BTN:
				{
					uiGuildRelationAsk* pGuildRelationAsk = GET_CAST_DIALOG( uiGuildRelationAsk, IM_GUILD_MANAGER::GUILD_RELATION_ASK );
					
					if(pGuildRelationAsk)
					{
						pGuildRelationAsk->SetbtRelation(eGUILD_RELATION_ALLIENCE);
						pGuildRelationAsk->SetbRelationState(TRUE);
						pGuildRelationAsk->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_CANCEL_BTN:
				{
					uiGuildRelationAsk* pGuildRelationAsk = GET_CAST_DIALOG( uiGuildRelationAsk, IM_GUILD_MANAGER::GUILD_RELATION_ASK );
					
					if(pGuildRelationAsk)
					{
						pGuildRelationAsk->SetbtRelation(eGUILD_RELATION_ALLIENCE);
						pGuildRelationAsk->SetbRelationState(FALSE);
						pGuildRelationAsk->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_ALLIANCE_MAIN_LIST:
				{
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

void	uiGuildAlliance::InitControl()
{
	m_pAlianceTab		   = static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_TAB));
	assert(m_pAlianceTab);
	m_pHostilityTab		   = static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_TAB));
	assert(m_pHostilityTab);
	m_pOneSideHostilityTab = static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_GUILD_ONE_SIDE_HOSTILITY_TAB));
	assert(m_pOneSideHostilityTab);

	m_pOKBtn			   = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_APPLY_BTN));
	assert(m_pOKBtn);
	m_pCancelBtn		   = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_CANCEL_BTN));
	assert(m_pCancelBtn);
	m_pExitBtn			   = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_EXIT_BTN));
	assert(m_pExitBtn);

	m_pRelationBtnForSort = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_RELATION_BTN_FOR_SORT));
	assert(m_pRelationBtnForSort);
	m_pStateBtnForSort    = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_STATE_BTN_FOR_SORT));
	assert(m_pStateBtnForSort);

	m_pMainList     = static_cast<CCtrlListWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_MAIN_LIST));
	assert(m_pMainList);
	m_pMainList->SetHideReportRender( true );

	m_pScroll					  = static_cast<CCtrlVScrollWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_SCROLL));
	assert(m_pScroll);

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
