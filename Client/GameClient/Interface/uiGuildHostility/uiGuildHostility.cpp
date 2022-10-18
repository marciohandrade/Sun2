#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiGuildHostility/uiGuildHostility.h"
#include "uiGuildAlliance/uiGuildAlliance.h"
#include "uiGuildOneSideHostility/uiGuildOneSideHostility.h"
#include "uiGuildRelationAsk/uiGuildRelationAsk.h"
#include "uiGuildMain/uiGuildMain.h"
#include "GlobalFunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Mouse.h"

const int MAX_PRINT_RELATION_CNT = 12;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiGuildHostility::m_wzID[eDIALOG_GUILD_HOSTILITY_MAX] = 
{
	StrToWzID("CT00"),//eDIALOG_GUILD_ALLIANCE_TAB,
	StrToWzID("CT02"),//eDIALOG_GUILD_HOSTILITY_TAB,
	StrToWzID("CT03"),//eDIALOG_GUILD_ONE_SIDE_HOSTILITY_TAB,

	StrToWzID("BT01"),//eDIALOG_GUILD_HOSTILITY_RELATION_BTN_FOR_SORT,
	StrToWzID("BT02"),//eDIALOG_GUILD_HOSTILITY_STATE_BTN_FOR_SORT,
	StrToWzID("BT03"),//eDIALOG_GUILD_HOSTILITY_EXIT_BTN,

	StrToWzID("VS00"),//eDIALOG_GUILD_HOSTILITY_SCROLL,
	StrToWzID("BT04"),//eDIALOG_GUILD_HOSTILITY_APPLY_BTN,
	StrToWzID("BT05"),//eDIALOG_GUILD_HOSTILITY_CANCEL_BTN,

	StrToWzID("LS00"),//eDIALOG_GUILD_HOSTILITY_MAIN_LIST,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildHostility::uiGuildHostility(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_bShow			  = FALSE;
	m_eRenderToolTip  = eGUILD_RELATION_NONE;
	m_iPreRelationCnt = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiGuildHostility::~uiGuildHostility()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;
	m_eRenderToolTip  = eGUILD_RELATION_NONE;
	m_iPreRelationCnt = 0;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::Release()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}

	UpdateToolTip();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::UpdateToolTip()
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

void uiGuildHostility::RenderTooltip()
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

void uiGuildHostility::OnRenderSolarDialog()
{
	RenderTooltip();

	if( GetManager() == NULL )
		return;

	std::list<GUILD_RELATION*>& rHostilityCon = this->GetManager()->GetHostilityCon();

	if( rHostilityCon.empty() )
		return;



	RECT rc_listctrl;
	RECT rc_GuildName;
	RECT rc_Relation;

	rc_Relation = rc_GuildName = rc_listctrl = this->m_pMainList->GetSizeRect();

	CControlWZ * pButton = getControl( eDIALOG_GUILD_HOSTILITY_RELATION_BTN_FOR_SORT );
	rc_GuildName.top	+= 5;
	rc_Relation.top		+= 5;
	rc_GuildName.right	= rc_GuildName.left + pButton->GetSize().right;
	rc_Relation.left	= rc_GuildName.right;

	DWORD dwNameColor = WzColor_RGBA(255, 255, 255, 255);
	DWORD dwProclaimHostilityNameColor = WzColor_RGBA(255, 0, 0, 255);


	int iRelCnt = rHostilityCon.size();
	int iTemp = iRelCnt - MAX_PRINT_RELATION_CNT;
	if(m_pScroll && (m_iPreRelationCnt != iRelCnt))
	{
		if(0 < iTemp)
			m_pScroll->SetScrollRangeWZ(0, iTemp);
		else
			m_pScroll->SetScrollRangeWZ(0, 0);
	}

	m_iPreRelationCnt = iRelCnt;

	std::list<GUILD_RELATION*>::iterator iter;
	std::list<GUILD_RELATION*>::iterator end = rHostilityCon.end();
	int i = 0;

	for( iter = rHostilityCon.begin(); iter != end; ++iter )
	{
		if(m_pScroll)
		{
			int pos = m_pScroll->GetScrollPosWZ();
			if(i < pos)
			{
				++i;
				continue;
			}
		}

		GUILD_RELATION* pTemp = (*iter);

		TCHAR szGuildName[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szHostility[INTERFACE_STRING_LENGTH + 1] = {0,};

		StrnCopy(szGuildName, pTemp->m_GuildName, INTERFACE_STRING_LENGTH);

		if(eGUILD_RELATION_BOTH_HOSTILITY == pTemp->m_eRelationType)
		{
			//1199 쌍방 적대중
			g_InterfaceManager.GetInterfaceString(1199, szHostility);
		}
		else if(eGUILD_RELATION_PROCLAIM_HOSTILITY == pTemp->m_eRelationType)
		{
			//1200 적대 선포중
			g_InterfaceManager.GetInterfaceString(1200, szHostility);
		}

		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "mn12" );

		if(PROCLAIM_GUILD == pTemp->m_eProClaim) //최초 적대 선포한 길드
		{
			this->GetDrawBase()->x_pManagerTextOut->DrawText(szGuildName,
				&rc_GuildName,
				dwProclaimHostilityNameColor,
				WzColor_RGBA(  0,  0,  0,   0),
				TP_HCENTER);
		}
		else
		{
			this->GetDrawBase()->x_pManagerTextOut->DrawText(szGuildName,
				&rc_GuildName,
				dwNameColor,
				WzColor_RGBA(  0,  0,  0,   0),
				TP_HCENTER);
		}

		this->GetDrawBase()->x_pManagerTextOut->DrawText(szHostility,
			&rc_Relation,
			dwNameColor,
			WzColor_RGBA(  0,  0,  0,   0),
			TP_HCENTER);

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		rc_GuildName.top += 15;
		rc_Relation.top += 15;

		++i;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::Process(DWORD dwTick)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::OnShowWindow(BOOL val)
{
	if (val)
	{
		uiGuildMain* pGuildMain = (uiGuildMain *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_GUILD_MAIN);
		RECT rt;
		pGuildMain->GetDialogWZ()->GetClientRect(&rt);
		this->MoveWindowWZ(rt.right, rt.top);

		m_pAlianceTab->SetCheckState(false);
		m_pHostilityTab->SetCheckState(true);
		m_pOneSideHostilityTab->SetCheckState(false);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_Hostility;
		msg.wParam = InterfaceManager::DIALOG_GUILD_HOSTILITY;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = TRUE;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_Hostility);
		m_bShow = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiGuildHostility::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_SB_LINETOP:
	case RT_MSG_SB_PAGETOP:
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();
				int mmin = 0 , mmax = 0;
				m_pScroll->GetScrollRangeWZ( &mmin, &mmax );

				if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
				{
					pos--;//한줄 감소 시키고 
					m_pScroll->SetScrollPosWZ(pos);
				}
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
	case RT_MSG_SB_PAGEBOTTOM:
		{
			if(m_pScroll)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos<mmax)// 맥스값 보다 작으면 
				{
					pos++;//한줄 증가 시키고 
					m_pScroll->SetScrollPosWZ(pos);
				}

			}
		}
		break;

	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
			
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_GUILD_ALLIANCE_TAB:
				{
					OnShowWindow(FALSE);

					uiBase * pGuildAlliance = GET_DIALOG( IM_GUILD_MANAGER::GUILD_ALLIANCE );
					
					if(pGuildAlliance)
					{
						pGuildAlliance->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_TAB:
				{
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

			case eDIALOG_GUILD_HOSTILITY_RELATION_BTN_FOR_SORT:
				{
					BOOL bTemp = GetManager()->GetbForSort();
					GetManager()->SortHostilityCon(bTemp, TRUE);
					bTemp ^= 1;
					GetManager()->SetbForSort(bTemp);
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_STATE_BTN_FOR_SORT:
				{
					BOOL bTemp = GetManager()->GetbForSort();
					GetManager()->SortHostilityCon(bTemp, FALSE);
					bTemp ^= 1;
					GetManager()->SetbForSort(bTemp);
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_SCROLL:
				{
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_APPLY_BTN:
				{
					uiGuildRelationAsk* pGuildRelationAsk = GET_CAST_DIALOG( uiGuildRelationAsk, IM_GUILD_MANAGER::GUILD_RELATION_ASK );
					
					if(pGuildRelationAsk)
					{
						pGuildRelationAsk->SetbtRelation(eGUILD_RELATION_PROCLAIM_HOSTILITY);
						pGuildRelationAsk->SetbRelationState(TRUE);
						pGuildRelationAsk->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_CANCEL_BTN:
				{
					uiGuildRelationAsk* pGuildRelationAsk = GET_CAST_DIALOG( uiGuildRelationAsk, IM_GUILD_MANAGER::GUILD_RELATION_ASK );
					if(pGuildRelationAsk)
					{
						pGuildRelationAsk->SetbtRelation(eGUILD_RELATION_PROCLAIM_HOSTILITY);
						pGuildRelationAsk->SetbRelationState(FALSE);
						pGuildRelationAsk->ShowInterface(TRUE);
					}
				}
				break;

			case eDIALOG_GUILD_HOSTILITY_MAIN_LIST:
				{
				}
				break;
			}
		}
		break;

	default:
		{
			m_eRenderToolTip = eGUILD_RELATION_NONE;
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void	uiGuildHostility::InitControl()
{
	m_pAlianceTab					= static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_GUILD_ALLIANCE_TAB));
	assert(m_pAlianceTab);
	m_pHostilityTab					= static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_TAB));
	assert(m_pHostilityTab);
	m_pOneSideHostilityTab			= static_cast<CCtrlButtonCheckWZ*>(getControl(eDIALOG_GUILD_ONE_SIDE_HOSTILITY_TAB));
	assert(m_pOneSideHostilityTab);

	m_pOKBtn						= static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_APPLY_BTN));
	assert(m_pOKBtn);
	m_pCancelBtn					= static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_CANCEL_BTN));
	assert(m_pCancelBtn);
	m_pExitBtn						= static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_EXIT_BTN));
	assert(m_pExitBtn);

	m_pRelationBtnForSort			= static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_RELATION_BTN_FOR_SORT));
	assert(m_pRelationBtnForSort);
	m_pStateBtnForSort				= static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_STATE_BTN_FOR_SORT));
	assert(m_pStateBtnForSort);

	m_pMainList						= static_cast<CCtrlListWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_MAIN_LIST));
	assert(m_pMainList);
	m_pMainList->SetHideReportRender( true );

	m_pScroll						= static_cast<CCtrlVScrollWZ*>(getControl(eDIALOG_GUILD_HOSTILITY_SCROLL));
	assert(m_pScroll);
	if(m_pScroll)
		m_pScroll->SetScrollRangeWZ(0, 0);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
