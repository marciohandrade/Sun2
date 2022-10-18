#include "SunClientPrecompiledHeader.h"
#include "herogaugedialog2control.h"
#include "Hero.h"
#include "CommonCharacterFormula.h"
#include "InterfaceManager.h"
#include "Mouse.h"
#include "HeroActionInput.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiCommunityDlg/uiCommunityDlg.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"

#include "StateInfoParser.h"
#include "GameOptionParser.h"
#include "StatusIconManager.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif //_DH_BATTLEZONE2_

#include "uiPerkDialog/uiPerkDialog.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "SCPerkSlot.h"
#include "ItemUnitRenderer.h"
#include "GlobalFunc.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20130114_RENEWER_UI

WzID HeroGaugeDialog2Control::m_wzId[HGAUGE_MAX] = 
{ 
		StrToWzID("T012"),//eControl_TEXT_LevelT012

		StrToWzID("B100"),//eControl_Button_B100
		StrToWzID("P029"),//eControl_Picture_P029
		StrToWzID("B101"),//eControl_Button_B101

		StrToWzID("C100"),//eControl_Button_C100
		StrToWzID("C200"),//eControl_Button_C200
		StrToWzID("CT01"),//eControl_Button_CT01
};

HeroGaugeDialog2Control::HeroGaugeDialog2Control(void):
		tooltip_string_code_(0)
{
}

HeroGaugeDialog2Control::~HeroGaugeDialog2Control(void)
{
}

void HeroGaugeDialog2Control::Process( DWORD dwTick )
{
	CheckChattingMessage(dwTick);
	CheckNoteMessage(dwTick);
	CheckPresentMessage(dwTick);
}

VOID HeroGaugeDialog2Control::Init( CDrawBase * pDrawBase )
{
	m_pChkChatMsg = static_cast<CCtrlButtonCheckWZ*>(getControl(eControl_Button_C100));
	if(m_pChkChatMsg)
	{
		m_pChkChatMsg->SetCheckState(false);
	}

	m_pChkPresentMsg = static_cast<CCtrlButtonCheckWZ*>(getControl(eControl_Button_C200));
	if(m_pChkPresentMsg)
	{
		m_pChkPresentMsg->SetCheckState(false);
#ifdef __CHINA_BLOCK_ITEM_SHOP_PRESENT
		m_pChkPresentMsg->ShowWindowWZ( WZ_HIDE );
		m_pChkPresentMsg->EnableWindowWZ( FALSE );
#endif
	}

	m_pChkNoteMsg	= static_cast<CCtrlButtonCheckWZ*>( getControl( eControl_Button_CT01 ) );
	if( m_pChkNoteMsg != NULL )
	{
		m_pChkNoteMsg->SetCheckState( false );
	}

	tooltip_index_map_.clear();

	tooltip_index_map_[eControl_Button_C100] = 5921;
	tooltip_index_map_[eControl_Button_C200] = 5980;
	tooltip_index_map_[eControl_Button_CT01] = TOOLTIP_STRING_NOTE_CODE;
}

VOID HeroGaugeDialog2Control::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			switch( pos )
			{
			case eControl_Button_B101:
				{
					uiPartyMan * pPartyMan = (uiPartyMan *)g_InterfaceManager.GetUserInterfaceManager( UIMAN_PARTY );
					if ((pPartyMan != NULL) && (pPartyMan->IsParty() == false) && (g_pHero != NULL))
					{
						g_pHero->SendZONE_EX_QUEST_PORTAL_ENTER_SYN(); 
					}
					else
					{
						LeaveParty();
					}
				}
				break;


			case eControl_Button_C100:
				{
					if (GameFramework::GetCurScene() == SCENE_TYPE_SSQ)
					{
						break;
					}
					uiCommunityMan* pCommMan = 
						static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if(pCommMan)
					{
						pCommMan->ShowFriendChatDlg(NULL);
					}
				}
				break;

			case eControl_Button_C200:
				{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

#if defined(_SUNGAME_VER)
					if (PROGRAMPARAM->is_block_cash_shop() == false)
#endif //defined(_SUNGAME_VER)
					{
						ConvenienceStore::CashShop::instance()->OpenCashShopWithGiftMenu();

					}
#else
					uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
					if(pMan && pMan->GetNewPresentRcvCount())
					{
						pMan->SetRqstState(eRqstState_Present_List);
						pMan->Send_CG_SHOP_CASH_REQ_SYN();
					}
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
				}
				break;

			case eControl_Button_CT01:
				{
					if (GameFramework::GetCurScene() == SCENE_TYPE_SSQ)
					{
						break;
					}

					uiCommunityMan* pCommMan = GET_DIALOG_MANAGER( uiCommunityMan, UIMAN_COMMUNITY );

					if( pCommMan )
					{
						pCommMan->ShowCommunityDlg();

						uiCommunityDlg *pCommunityDlg =(uiCommunityDlg*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_COMMUNITY_DLG);

						if(pCommunityDlg)
						{
							pCommunityDlg->ChangeMode(eCM_MAIL);
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;

	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			tooltip_string_code_ = 0;

			map<int,int>::iterator iter;
			iter = tooltip_index_map_.find(pos);
			if (iter != tooltip_index_map_.end())
			{
				tooltip_string_code_ = iter->second;
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

VOID HeroGaugeDialog2Control::NetworkProc( MSG_BASE * pMsg )
{

}

VOID HeroGaugeDialog2Control::OnRenderSolarDialog()
{
    Render_Tooltip();
}

VOID HeroGaugeDialog2Control::OnUpdateSolarDialog()
{
	if(g_pHero)
	{
		LEVELTYPE CurLevel = g_pHero->GetLevel();
		SetLevel(CurLevel);

		uiPartyMan* partyMan =
			static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
		CControlWZ* is_hide = getControl(eControl_Picture_P029);

		CControlWZ* is_exit_party = getControl(eControl_Button_B101);


		if (partyMan && is_hide)
		{
			if(partyMan->IsLeader())
			{
				//일딴 이미지 마크를 그려준다
				is_hide->ShowWindowWZ(WZ_SHOW);

			}
			else
			{
				is_hide->ShowWindowWZ(WZ_HIDE);
			}
		}

		uiPartyMan * pPartyMan = (uiPartyMan *)g_InterfaceManager.GetUserInterfaceManager( UIMAN_PARTY );
		if ( pPartyMan )
		{
			if ( GetCtrlLeavePartyBtn() )
			{
				if ((g_pHero != NULL) && (g_pHero->FixedPortalState.has_portal_info() == true))
				{
					ShowLeavePartyBtn(TRUE);
				}
				else
				{
					ShowLeavePartyBtn( pPartyMan->IsParty() );
				}
			}			
		}

		if ( GetCtrlPCModeBtn() )
		{
			ShowPCModeBtn( g_pHero->GetPCBangMode() );
		}
	}
}

void HeroGaugeDialog2Control::CheckChattingMessage( DWORD Tick )
{
	static DWORD show_tick = 0;
	static bool  check_chatting_state = false;

	uiCommunityMan* pCommMan = GET_DIALOG_MANAGER( uiCommunityMan, UIMAN_COMMUNITY );

	if( pCommMan == NULL || m_pChkChatMsg == NULL )
		return;

	if( pCommMan->GetNewFriendChatCount() && !pCommMan->GetRejectChat() )
	{
		m_pChkChatMsg->ShowWindowWZ( WZ_SHOW );

		show_tick += Tick;

		if( show_tick >= BLINK_RANGE_TICK )
		{
			show_tick = 0;
			check_chatting_state = !check_chatting_state;
		}

		m_pChkChatMsg->SetCheckState( check_chatting_state );

	}
	else
	{
		m_pChkChatMsg->ShowWindowWZ( WZ_HIDE );
	}
}

void HeroGaugeDialog2Control::CheckNoteMessage( DWORD Tick )
{
	static DWORD show_tick = 0;
	static bool  Note_state = false;

	uiCommunityMan* pCommMan = GET_DIALOG_MANAGER( uiCommunityMan, UIMAN_COMMUNITY );

	if( pCommMan == NULL || m_pChkNoteMsg == NULL )
		return;

	if( pCommMan->HaveNewNote() )
	{
		m_pChkNoteMsg->ShowWindowWZ( WZ_SHOW );

		show_tick += Tick;

		if( show_tick >= BLINK_RANGE_TICK )
		{
			show_tick = 0;
			Note_state = !Note_state;
		}

		m_pChkNoteMsg->SetCheckState( Note_state );

	}
	else
	{
		m_pChkNoteMsg->ShowWindowWZ( WZ_HIDE );
	}
}

void HeroGaugeDialog2Control::CheckPresentMessage( DWORD Tick )
{
	static DWORD show_tick = 0;
	static bool  present_state = false;

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
	//! 2011.7.22 / i4u4me / 캐시샵 작업 
	if( m_pChkPresentMsg == NULL )
		return;

	ConvenienceStore::CashShop* pInstance = ConvenienceStore::CashShop::instance();
	if( pInstance == NULL )
		return;

	if( pInstance->HasNewPresent() )
	{
		m_pChkPresentMsg->ShowWindowWZ( WZ_SHOW );

		show_tick += Tick;

		if( show_tick >= BLINK_RANGE_TICK )
		{
			show_tick = 0;
			present_state = !present_state;
		}

		m_pChkPresentMsg->SetCheckState(present_state);
	}
	else
	{
		m_pChkPresentMsg->ShowWindowWZ( WZ_HIDE );
	}


#else

	uiCashItemShopMan *pMan = GET_DIALOG_MANAGER( uiCashItemShopMan, UIMAN_CASH_ITEM_SHOP );

	if( pMan == NULL ||  m_pChkPresentMsg == NULL )
		return;

	if( pMan->GetNewPresentRcvCount() )
	{
		m_pChkPresentMsg->ShowWindowWZ( WZ_SHOW );

		show_tick += Tick;

		if( show_tick >= BLINK_RANGE_TICK )
		{
			show_tick = 0;
			present_state = !present_state;
		}

		m_pChkPresentMsg->SetCheckState( present_state );
	}
	else
	{
		m_pChkPresentMsg->ShowWindowWZ( WZ_HIDE );
	}

#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
}

void HeroGaugeDialog2Control::LeaveParty()
{
	// 파티 Leave
	if ( (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE)
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
		|| (GameFramework::GetCurScene() == SCENE_TYPE_FREEPVP)
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
		)
	{
		// 마을 
		uiPartyMan* partyMan =
			static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
		if (partyMan)
		{
			partyMan->LeaveParty();
		}
	}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if ((GameFramework::GetCurScene() == SCENE_TYPE_BATTLEGROUND) &&
        (ChaosSystem::GetBattleGroundData().battle_ground_progress() != ChaosSystem::BattleGroundData::PROGRESS_END))
    {
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        // 2720 정말 파티 떠남?
        g_InterfaceManager.GetInterfaceString(2720, temp_string, INTERFACE_STRING_LENGTH);					
        
        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (system_manager)
        {
            system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundBackToTheVillageConfirm);
        }
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    else if ((GameFramework::GetCurScene() == SCENE_TYPE_GOLDRUSH) &&
        (ChaosSystem::GetGoldRushData().gold_rush_progress() != ChaosSystem::GoldRushData::PROGRESS_END))
    {
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
        // 2720 파티를 떠나면 전장에서 이탈하게 됩니다. 전장 이탈시 도망자 패널티를 받게 되며, 일정 시간동안 전장에 입장 할 수 업습니다. 정말 파티를 떠나시겠습니까?
        g_InterfaceManager.GetInterfaceString(2720, temp_string, INTERFACE_STRING_LENGTH);

        uiSystemMan* system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (system_manager_ptr)
        {
            system_manager_ptr->Sys_Confirm(temp_string, GlobalFunc::BattleGroundBackToTheVillageConfirm);
        }
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	else 
	{
		// 기타 베틀
		uiBase *partyLeavePopup = GET_DIALOG( IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );

		if (partyLeavePopup)
		{
			partyLeavePopup->ShowInterface(TRUE);
		}

	}	
}

void HeroGaugeDialog2Control::SetLevel( DWORD dwLevel )
{
	TCHAR level_string[64];
	Sprintf( level_string, _T("%d"), dwLevel );

	CControlWZ * pControl = getControl(eControl_TEXT_LevelT012);

	if( pControl )
	{
		pControl->SetTextWZ( level_string );
	}
}

CControlWZ * HeroGaugeDialog2Control::getControl( POSTYPE AtPos )
{
	assert( AtPos < HGAUGE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE HeroGaugeDialog2Control::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < HGAUGE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return HGAUGE_MAX;
}

void HeroGaugeDialog2Control::GetMessage( DWORD messageID,LPARAM lParam,WPARAM wParam )
{
	if(WM_CLICK_EVENT)
	{
#if WZENGINEVER >= 590
		CDialogManager* pDialogManager = g_InterfaceManager.GetDialogManager();
		pDialogManager->MoveTail( GetDialogKey() );
#endif // WZENGINEVER >= 590
	}
}

void HeroGaugeDialog2Control::MoveWindowForce( float addx, float addy )
{
	float x = this->GetDialogWZ()->GetDlgX();
	float y = this->GetDialogWZ()->GetDlgY();

	x += addx;
	y += addy;

	this->GetDialogWZ()->MoveWindowWZ(x,y);
}

VOID HeroGaugeDialog2Control::OnShowWindow( BOOL val )
{
	if(val)
	{
		
	}
}

void HeroGaugeDialog2Control::MoveDefaultWindowPos()
{
	uiBase::MoveDefaultWindowPos();
	MoveWindowForce(0.0f,-6.0f);
}

void HeroGaugeDialog2Control::Render()
{
	CControlWZ* constroller = getControl(eControl_TEXT_LevelT012);
	if(constroller)
	{
		constroller->OnDraw(true);
	}

	constroller = getControl(eControl_Button_B100);
	if(constroller)
	{
		constroller->OnDraw(true);
	}
	constroller = getControl(eControl_Picture_P029);
	if(constroller)
	{
		constroller->OnDraw(true);
	}
	constroller = getControl(eControl_Button_B101);
	if(constroller)
	{
		constroller->OnDraw(true);
	}
	constroller = getControl(eControl_Button_C100);
	if(constroller)
	{
		constroller->OnDraw(true);
	}
	constroller = getControl(eControl_Button_C200);
	if(constroller)
	{
		constroller->OnDraw(true);
	}
	constroller = getControl(eControl_Button_CT01);
	if(constroller)
	{
		constroller->OnDraw(true);
	}
}

void HeroGaugeDialog2Control::Render_Tooltip()
{
    uiToolTipMan * pUIMan = GET_DIALOG_MANAGER( uiToolTipMan, UIMAN_TOOLTIP );

    if( pUIMan == NULL )
        return;

    int			x = Mouse::Instance()->GetMouseX();
    int			y = Mouse::Instance()->GetMouseY();
    CControlWZ*  constroller = getControl(eControl_Button_C100);
    RECT        postion;

    if(constroller && constroller->GetState() == WZ_SHOW)
    {
        postion = constroller->GetSizeRect();
        if( x > postion.left && x < postion.right && 
            y > postion.top &&  y < postion.bottom )
        {
            uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
            if(pCommMan)
            {
                if(pCommMan->GetFriendNameForTooltip())
                {
                    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

                    g_InterfaceManager.GetInterfaceString(5921, szTemp, INTERFACE_STRING_LENGTH);
                    Sprintf(szMessage, szTemp, pCommMan->GetFriendNameForTooltip());

                    pUIMan->RegisterTooltipSimpleText(szMessage);
                    pUIMan->SetTooltipDialogID(GetDialogKey());

                }
            }
        }
    }

    constroller = getControl(eControl_Button_C200);
    if(constroller && constroller->GetState() == WZ_SHOW)
    {
        postion = constroller->GetSizeRect();
        if( x > postion.left && x < postion.right && 
            y > postion.top &&  y < postion.bottom )
        {
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            //! 2011.7.22 / i4u4me / 캐시샵 작업 
            if( ConvenienceStore::CashShop* pInstance = ConvenienceStore::CashShop::instance() )
            {
                if( pInstance->HasNewPresent() )
                {
                    pUIMan->RegisterTooltipWithStringCode(tooltip_string_code_);
                    pUIMan->SetTooltipDialogID(GetDialogKey());
                }
            }

#else
            uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
            if(pMan && pMan->GetNewPresentRcvCount())
            {
                pUIMan->RegisterTooltipWithStringCode(tooltip_string_code_);
                pUIMan->SetTooltipDialogID(GetDialogKey());
            }

#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        }
    }

    constroller = getControl(eControl_Button_CT01);
    if(constroller && constroller->GetState() == WZ_SHOW)
    {
        postion = constroller->GetSizeRect();
        if( x > postion.left && x < postion.right && 
            y > postion.top &&  y < postion.bottom )
        {
            if( m_pChkNoteMsg != NULL )
            {
                if( m_pChkNoteMsg->GetState() == WZ_SHOW )
                {
                    pUIMan->RegisterTooltipWithStringCode( tooltip_string_code_ );
                    pUIMan->SetTooltipDialogID(GetDialogKey());
                }
            }
        }
    }

}
#endif// _NA_000000_20130114_RENEWER_UI