#include "SunClientPrecompiledHeader.h"
#include "maindialog2.h"
#include "HotKeyManager.h"
#include "InterfaceManager.h"
#include "Hero.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "castleavefielddialog.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif

#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
  #include "uiRadarMan/uiRadarMan.h"
#else
  #include "uiCommunityMan/uiCommunityMan.h"
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
#include "ContinentMapDlg.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Icon/uiBattlezone2Icon.h"
#include "Map.h"

#include "InstanceDungeonScene.h"
#include "SSQScene.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "BattleGroundInfoParser.h"
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "uiAttendanceCheck/uiAttendanceCheckMan.h"
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiMiniGameMan/uiMiniGameMan.h"
#include "uiBingoGameDialog/uiBingoGameDialog.h"
#include "SunEventInfoParser.h"
#endif // _NA_008405_20150716_MINIGAME_BINGO

#ifdef _NA_000000_20130114_RENEWER_UI

#include "HotKeyManager.h"
#include "Input/KeyCommands.h"
#include "input/HotKeyInfoParser.h"
#include "uiRadarMan/uiRadarMan.h"
#include "RadarFullDialog.h"

//해당툴팁에 상응하는 인덱스
const HOT_KEY_FUNCTION_INDEX hotkey_func_index_[] = 
{
	//핫키와 ID가 매칭되게 넣는다
	kOpen_CashShop_UI,
	kKeyFunctionMax,
	kKeyFunctionMax,
	kModeChange_FullRadar_UI,
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
    kOpen_ContinentMap_UI,
#else
	kOpen_Community_UI,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    kOpen_Attendance_UI,
#else
  #ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
        kOpen_Attendance_UI,
  #endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	kOpen_SUN_Ranking_UI,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	kKeyFunctionMax, 
#endif // _NA_008405_20150716_MINIGAME_BINGO
};

WzID MainDialog2::m_wzId[MAIN_MAX] = 
{ 
	StrToWzID("TI00"),//	MAIN_BTN_ITEMSHOP_TI00,
	StrToWzID("TI04"),//	MAIN_BTN_BATTLEZONE_TI04,
	StrToWzID("TI03"),//	MAIN_BTN_CHAOSZONE_TI03,
	StrToWzID("TI02"),//	MAIN_BTN_WORLDMAP_TI02,
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
    StrToWzID("TI01"),//	MAIN_BTN_CONTINENT_TI01,
#else
    StrToWzID("TI01"),//	MAIN_BTN_COMMUNITY_TI01,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    StrToWzID("I000"),//	MAIN_BTN_ATTENDANCE_I000,
#else
  #ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
    StrToWzID("I000"),//	MAIN_BTN_ATTENDANCE_I000,
  #endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	StrToWzID("I001"),//    MAIN_BTN_SUN_RANKING_I001,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	StrToWzID("I002"),//    MAIN_BTN_MINIGAME_I002, 
#endif // _NA_008405_20150716_MINIGAME_BINGO
};

MainDialog2::MainDialog2()
{
	m_DialogKey = 0;
}

MainDialog2::~MainDialog2()
{

}
VOID MainDialog2::Init( CDrawBase* pDrawBase )
{
	chaos_zone_button_ = (CCtrlButtonWZ*)getControl(MAIN_BTN_CHAOSZONE_TI03);
	battle_zone_button_= (CCtrlButtonWZ*)getControl(MAIN_BTN_BATTLEZONE_TI04);

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
    if (getControl(MAIN_BTN_ATTENDANCE_I000))
    {
        getControl(MAIN_BTN_ATTENDANCE_I000)->ShowWindowWZ(WZ_HIDE);
    }
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008405_20150716_MINIGAME_BINGO
	mini_game_button_  = (CCtrlButtonWZ*)getControl(MAIN_BTN_MINIGAME_I002); 
#endif // _NA_008405_20150716_MINIGAME_BINGO

}

VOID MainDialog2::Release()
{

}

VOID MainDialog2::MessageProc( SI_MESSAGE* pMessage )
{
	switch (pMessage->eResultMsg) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch (getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case MAIN_BTN_ITEMSHOP_TI00:
				{
					if (g_pHero)
					{
						if (g_pHero->IsShowFullMap())
						{
							return;
						}
					}

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

#if defined(_SUNGAME_VER)
					if (PROGRAMPARAM->is_block_cash_shop() == false)
#endif //defined(_SUNGAME_VER)
					{
						ConvenienceStore::CashShop::instance()->ToggleOpenKey();
					}
#else
					uiCashItemShopMan* pMan = static_cast<uiCashItemShopMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
					if (pMan)
					{
						pMan->ToggleCashItemShopDlg();
					}
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
				}
				break;
			case MAIN_BTN_BATTLEZONE_TI04:
				{
					if (g_pHero)
					{
						if (g_pHero->IsShowFullMap())
						{
							return;
						}
					}

#ifdef _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
                    if ((g_pMap->IsVill() == FALSE) && 
                        (g_pMap->IsField() == FALSE &&
                         g_pMap->IsTutorialField() == FALSE))
#else
					if ((g_pMap->IsVill() == FALSE) && 
						(g_pMap->IsField() == FALSE))
#endif // _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
					{
						return;
					}

					uiBattlezone2Man* ui_battlezone2_manager = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
					if (ui_battlezone2_manager != NULL)
					{
						uiBattlezone2Man::ToggleBattlezoneDialog();
					}
				}
				break;
			case MAIN_BTN_CHAOSZONE_TI03:
				{
					if (g_pHero)
					{
						if (g_pHero->IsShowFullMap())
						{
							return;
						}
					}

					uiChaosSystemMan* chaos_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
					if (chaos_manager)
					{
						chaos_manager->ToggleChaoszoneDialog();
					}
				}
				break;
			case MAIN_BTN_WORLDMAP_TI02:
				{

					if (!GlobalFunc::IsSSQScene())	// 타락한 사원에서는 맵을 열 수 없다.
					{
						RadarFullDialog* pRadarFullDlg = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
						if (pRadarFullDlg)
						{
							pRadarFullDlg->IncreaseRadarState();
							if(eFULL_RADAR_STATE_MAX != pRadarFullDlg->GetRadarState())
							{
								pRadarFullDlg->ShowDialog(TRUE);
							}
							else
							{
								pRadarFullDlg->ShowDialog(FALSE);
							}
						}
					}
				}
				break;
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
            case MAIN_BTN_CONTINENT_TI01:
                {
                    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
                    if (ui_radar_manager != NULL)
                    {
                        ui_radar_manager->ToggleShowContinentMap();
                    }
                }
                break;
#else
			case MAIN_BTN_COMMUNITY_TI01:
				{
					if (g_pHero)
					{
						if (g_pHero->IsShowFullMap())
						{
							return;
						}
					}

					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if (pCommMan)
					{
						pCommMan->ShowCommunityDlg();
					}
				}
				break;
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
            case MAIN_BTN_ATTENDANCE_I000:
                {
                    // 출석체크
                    uiAttendanceCheckMan* attendance_man = GET_DIALOG_MANAGER(uiAttendanceCheckMan, UIMAN_ATTENDANCE_CHECK);
                    if (attendance_man)
                    {
                        attendance_man->toggle_window();
                    }
                }
                break;
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
			case MAIN_BTN_SUN_RANKING_I001:
				{
					// SUN 랭킹
					uiSUNRankingMan* ui_sun_ranking_manager =
						GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
					if (ui_sun_ranking_manager)
					{
						ui_sun_ranking_manager->ToggleSUNRankingDialog();
					}
				}
				break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
			case MAIN_BTN_MINIGAME_I002:
				{
					// MINI GAME
					uiMiniGameMan* ui_mini_game_manager = 
						GET_DIALOG_MANAGER(uiMiniGameMan, UIMAN_MINIGAME);
					if (ui_mini_game_manager)
					{
						ui_mini_game_manager->ToggleBingoGameDialog();
					}
				}
				break;
#endif // _NA_008405_20150716_MINIGAME_BINGO
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
	}
}

VOID MainDialog2::OnRenderSolarDialog()
{
	RenderToolTipBattle();
	RenderToolTipChaos();
#ifdef _NA_008405_20150716_MINIGAME_BINGO
	RenderToolTipMiniGame();
#endif // _NA_008405_20150716_MINIGAME_BINGO

	int x	= Mouse::Instance()->GetMouseX();
	int y	= Mouse::Instance()->GetMouseY();

	for (int i = 0 ; i < MAIN_MAX ; ++i)
	{
		if(tooltip_control_center_[i].isToolTip)
		{
			CControlWZ* Control_Key = getControl(tooltip_control_center_[i].ui_position_);

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
            if (tooltip_control_center_[i].ui_position_ == MAIN_BTN_ATTENDANCE_I000)
            {
                continue;
            }
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE

			RECT pos_control = Control_Key->GetSizeRect();

			if( x > pos_control.left && x < pos_control.right && 
				y > pos_control.top &&  y < pos_control.bottom )
			{
				uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
				if (tooltip_manager)
				{
					tooltip_manager->RegisterTooltip(tooltip_control_center_[i].tooltipString);
					tooltip_manager->SetTooltipDialogID( GetDialogKey() );
				}
			}

		}
	}
}

void MainDialog2::RenderToolTipBattle( void )
{
	if(battle_zone_button_ == NULL)
	{
		return;
	}

	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();
	RECT rc_battle_ = battle_zone_button_->GetSizeRect();
	if(battle_zone_button_ &&
	iMouseX > rc_battle_.left && 
	iMouseX < rc_battle_.right && 
	iMouseY > rc_battle_.top && 
	iMouseY < rc_battle_.bottom)
	{
		uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
		if (tooltip_manager)
		{
			tooltip_manager->RegisterTooltipWithStringCode(828);
		}
		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR type_name_string[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR last_string[INTERFACE_STRING_LENGTH] = {0,};

		int hour = 0;
		int min = 0;
		int sec = 0;
		int checkTime = 0;

		checkTime = (int)(g_InstanceDungeonScene.GetStartCheckTime() + 
			(g_InstanceDungeonScene.GetStartProcessTick() - clock_function::GetTickCount())) / 1000;

		if(checkTime < 0)
		{
			g_InstanceDungeonScene.Send_MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN();
			return;
		}

		hour = checkTime / 3600;
		checkTime %= 3600;
		min = checkTime / 60;
		checkTime %= 60;
		sec = checkTime % 60;

		if(g_InstanceDungeonScene.GetJoinState() == eJION_TOP_STATE_NONADMISSION)
		{
			//	11352	(입장까지 남은 시간 : %d시간 %d분 %d초)
			g_InterfaceManager.GetInterfaceString(11352, szInterfaceString, INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szInterfaceString, hour, min, sec);
		}
		else if(g_InstanceDungeonScene.GetJoinState() == eJION_TOP_STATE_ADMISSION)
		{
			//	11353	(입장 가능 시간 : %d분 %d초)
			g_InterfaceManager.GetInterfaceString(11353, szInterfaceString, INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szInterfaceString, min, sec);
		}
		else
			return;

#if defined(_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND) && defined(_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION)
		//50129 저주받은탑
		g_InterfaceManager.GetInterfaceString(50129, type_name_string);
		Sprintf(last_string, "%s %s", type_name_string, szMessage);
		tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
			26,
			StrToWzID("mn12"),
			WzColor_RGBA(0, 255, 0, 255),
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
		tooltip_manager->SetTooltipDialogID( GetDialogKey() );
#else 
		//50129 저주받은탑
		g_InterfaceManager.GetInterfaceString(50129, type_name_string);
		Sprintf(last_string, "%s %s", type_name_string, szMessage);
		tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
			12,
			StrToWzID("mn12"),
			WzColor_RGBA(0, 255, 0, 255),			
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
	//55015 타락한사원
		g_InterfaceManager.GetInterfaceString(55015, type_name_string);
		g_SsqScene.Func_Output_with_internal_timer(szMessage);

		Sprintf(last_string, "%s %s", type_name_string, szMessage);
		tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
			12,
			StrToWzID("mn12"),
			WzColor_RGBA(0, 255, 0, 255),		
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
		tooltip_manager->AddEmptyLineForTooltip();
		tooltip_manager->SetTooltipDialogID( GetDialogKey() );
#endif
	}
}

void MainDialog2::RenderToolTipChaos( void )
{
	if(chaos_zone_button_ == NULL)
	{
		return;
	}

	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();
	RECT rc_battle_ = chaos_zone_button_->GetSizeRect();
	if(chaos_zone_button_ &&
		iMouseX > rc_battle_.left && 
		iMouseX < rc_battle_.right && 
		iMouseY > rc_battle_.top && 
		iMouseY < rc_battle_.bottom)
	{
#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
		uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
		if (tooltip_manager)
        {
            // 2693 카오스 존 (Shift+F11): 다른 플레이어와 PVP를 할 수 있습니다.
            tooltip_manager->RegisterTooltipWithStringCode(2693);

            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR type_name_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR last_string[INTERFACE_STRING_LENGTH] = {0,};

            if (ChaosSystem::GetBattleGroundData().is_on_reservation())
            {
                DWORD select_mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                const BattleGroundInfo* battle_ground_info =
                    BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(select_mapcode));
                if (battle_ground_info)
                {
                    WORD current_member_count = ChaosSystem::GetBattleGroundData().reservation_member_count() / 2;
                    current_member_count = max(current_member_count,1);

                    TCHAR current_team_string[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR max_team_string[INTERFACE_STRING_LENGTH] = {0,};
                    _itot(battle_ground_info->team_max, max_team_string, 10);
                    _itot(current_member_count, current_team_string, 10);

                    // 2595 팀 구성 현황: %s/%s명
                    g_InterfaceManager.GetInterfaceStringFormat(last_string, INTERFACE_STRING_LENGTH-1,
                        2595, current_team_string, max_team_string);

                    tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
                        16,
                        StrToWzID("mn12"),
                        WzColor_RGBA(0, 255, 0, 255),
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
                }

                // 전장 대기 소요 시간
                ChaosSystem::GetBattleGroundData().GetEnterWaitTime(last_string);
                if (szMessage != NULL)
                {
                    tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
                        16,
                        StrToWzID("mn12"),
                        WzColor_RGBA(0, 255, 0, 255),
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
                }
            }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            if (ChaosSystem::GetGoldRushData().is_on_reservation())
            {
                DWORD select_mapcode = ChaosSystem::GetGoldRushData().select_mapcode();
                typedef BattleGroundInfo GoldRushInfo;
                const GoldRushInfo* gold_rush_info =
                    BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(select_mapcode));
                if (gold_rush_info)
                {
                    WORD current_member_count = ChaosSystem::GetGoldRushData().reservation_member_count() / 2;
                    current_member_count = max(current_member_count,1);

                    TCHAR current_team_string[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR max_team_string[INTERFACE_STRING_LENGTH] = {0,};
                    _itot(gold_rush_info->team_max, max_team_string, 10);
                    _itot(current_member_count, current_team_string, 10);

                    // 2595 팀 구성 현황: %s/%s명
                    g_InterfaceManager.GetInterfaceStringFormat(last_string, INTERFACE_STRING_LENGTH-1,
                        2595, current_team_string, max_team_string);

                    tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
                        16,
                        StrToWzID("mn12"),
                        WzColor_RGBA(0, 255, 0, 255),
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
                }

                // 골드러쉬 대기 소요 시간
                ChaosSystem::GetGoldRushData().GetEnterWaitTime(last_string);
                if (szMessage != NULL)
                {
                    tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
                        16,
                        StrToWzID("mn12"),
                        WzColor_RGBA(0, 255, 0, 255),
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
                }
            }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
            // 전장 오픈 타임
            ChaosSystem::GetBattleGroundData().GetBattleGroundOpenTime(szMessage);
            if (szMessage[0] != '\0')
            {
                tooltip_manager->RegisterTooltip(szMessage, _tcslen(szMessage),
                    16,
                    StrToWzID("mn12"),
                    WzColor_RGBA(0, 255, 0, 255),
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
                //tooltip_manager->AddEmptyLineForTooltip();
            }
#endif //_NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

            //55015 타락한사원
            g_InterfaceManager.GetInterfaceString(55015, type_name_string);
            g_SsqScene.Func_Output_with_internal_timer(szMessage);

            Sprintf(last_string, "%s %s", type_name_string, szMessage);
            tooltip_manager->RegisterTooltip(last_string, _tcslen(last_string),
                16,
                StrToWzID("mn12"),
                WzColor_RGBA(0, 255, 0, 255),
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            tooltip_manager->SetTooltipDialogID( GetDialogKey() );

            tooltip_manager->AddEmptyLineForTooltip();
        }
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
	}
}

#ifdef _NA_008405_20150716_MINIGAME_BINGO
void MainDialog2::RenderToolTipMiniGame( void )
{
	if(mini_game_button_ == NULL)
	{
		return;
	}

	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();
	RECT rc_battle_ = mini_game_button_->GetSizeRect();
	if(mini_game_button_ &&
		iMouseX > rc_battle_.left && 
		iMouseX < rc_battle_.right && 
		iMouseY > rc_battle_.top && 
		iMouseY < rc_battle_.bottom)
	{
		uiToolTipMan* tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP));
		if ( tooltip_manager )
		{
			EVENT_INFO* event_info = SunEventInfoParser::Instance()->FindEventInfo(201);
			if ( event_info )
			{
				struct tm tmDate = { };

				struct tm* t;
				__time64_t ti = event_info->m_end_time;
				t = _localtime64(&ti);

				TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
				TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
				TCHAR date_string[INTERFACE_STRING_LENGTH] = {0, };
				g_InterfaceManager.GetInterfaceString(41863, format_string);

				Snprintf(date_string, INTERFACE_STRING_LENGTH-1, _T("%d/%d/%d"), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

				Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, date_string);

				tooltip_manager->RegisterTooltip(result_string);
				tooltip_manager->SetTooltipDialogID( GetDialogKey() );
			}
			else
			{
				TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
				// 4022 : 이벤트 중이 아닙니다.
				g_InterfaceManager.GetInterfaceString(4022, result_string);

				tooltip_manager->RegisterTooltip(result_string);
				tooltip_manager->SetTooltipDialogID( GetDialogKey() );
			}
		}
	}
}
#endif // _NA_008405_20150716_MINIGAME_BINGO

CControlWZ* MainDialog2::getControl( POSTYPE AtPos )
{
	assert( AtPos < MAIN_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE MainDialog2::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < MAIN_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return MAIN_MAX;
}

void MainDialog2::BattleZoneToDisAbleButton( bool state )
{	
	if(chaos_zone_button_)
	{
		chaos_zone_button_->EnableWindowWZ(state);
	}
	if(battle_zone_button_)
	{
		battle_zone_button_->EnableWindowWZ(state);
	}
}

#ifdef _NA_008405_20150716_MINIGAME_BINGO
void MainDialog2::MiniGameToDisAbleButton( bool state )
{
	if ( mini_game_button_ )
	{
		mini_game_button_->EnableWindowWZ(state);
		uiBingoGameDialog* ui_bingo_game_dialog = GET_CAST_DIALOG(uiBingoGameDialog, IM_MINIGAME_MANAGER::BINGOGAME_DIALOG);
		if (ui_bingo_game_dialog)
		{
			ui_bingo_game_dialog->set_is_enable(state);
		}
	}
}
#endif // _NA_008405_20150716_MINIGAME_BINGO

VOID MainDialog2::OnUpdateSolarDialog()
{

}

VOID MainDialog2::OnShowWindow( BOOL val )
{
	if(val)
	{
		
	}
	else
	{

	}
}

void MainDialog2::CheckModifyOption()
{
	memset(tooltip_control_center_,0,sizeof(stTOOLTIPCONTROL)*MAIN_MAX);

	DWORD text_string[] = 
	{
		2670,
		0,
		0,
		2680,
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
        2673,
#else
		2674,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        3236,
#else
  #ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
        3236,
  #endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
		3620,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
		0, 
#endif // _NA_008405_20150716_MINIGAME_BINGO
	};
	for (int i = 0 ; i < MAIN_MAX ; ++i)
	{
		if(hotkey_func_index_[i] == kKeyFunctionMax)
		{
			continue;
		}

		KEY_BIND_PAIR_MAP::iterator found_itr = 
			HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map().find(hotkey_func_index_[i]);

		KEY_BIND_PAIR temp_key_bind_pair = found_itr->second;

		STLX_STRING hot_key_string;
		DWORD string_code = 0;

		HOT_KEY_CODE hot_ket_code(temp_key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey));
		if (hot_ket_code.key_code_ != 0)
		{
			if (hot_ket_code.ctrl_key_ == 1)
			{
				hot_key_string += g_InterfaceManager.GetInterfaceString(8218);
				hot_key_string += " + ";
			}

			if (hot_ket_code.alt_key_ == 1)
			{
				hot_key_string += g_InterfaceManager.GetInterfaceString(8219);
				hot_key_string += " + ";
			}

			if (hot_ket_code.shift_key_ == 1)
			{
				hot_key_string += g_InterfaceManager.GetInterfaceString(8217);
				hot_key_string += " + ";
			}

			string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);

			//\ 이거만 예외 처리됨 ㅠㅠ
			if(strstr(g_InterfaceManager.GetInterfaceString(string_code),"Back Slash"))
			{
				hot_key_string += "\\";
			}
			else
				hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
		}

		CControlWZ* Control_Key = getControl(i);
		if(Control_Key)
		{
			DWORD ControlTextID = text_string[i];
			g_InterfaceManager.GetInterfaceStringFormat(tooltip_control_center_[i].tooltipString
				,256,ControlTextID,hot_key_string.c_str());

			tooltip_control_center_[i].isToolTip = true;
			tooltip_control_center_[i].TooltipPos = Control_Key->GetSizeRect();
			tooltip_control_center_[i].ui_position_ = i;
		}
	}
	
}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI