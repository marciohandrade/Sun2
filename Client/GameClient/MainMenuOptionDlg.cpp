#include "SunClientPrecompiledHeader.h"
#include "mainmenuoptiondlg.h"
#include "SoundEffect.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
    #include "uiGuildMain/uiGuildMain.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#ifdef _KIKI_UI_WEB_DIALOG
#include "uiWebDialogMan/uiWebDialogMan.h"
#endif

#include "Hero.h"
#include "uiQuestMan/uiQuestMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "RadarFullDialog.h"
#include "Interface/uiTradeMan/uiTradeMan.h"

#include "ContinentMapDlg.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"

#include "uiMenuMan\uiMenuMan.h"

  #include "uiGuideSystemMan/uiGuideSystemMan.h"
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif

#ifdef _NA_000000_20130114_RENEWER_UI
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Icon/uiBattlezone2Icon.h"
#include "Map.h"

#include "HotKeyManager.h"
#include "Input/KeyCommands.h"
#include "input/HotKeyInfoParser.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#endif

#ifdef _NA_000000_20130114_RENEWER_UI
#include "uiOptionMan.h"
#include "uiOptionView.h"
#endif


const HOT_KEY_FUNCTION_INDEX hotkey_func_index_[] = 
{
	//핫키와 ID가 매칭되게 넣는다
	kOpen_CashShop_UI,
	kOpen_BattleZone_UI,
	kOpen_ChaosZone_UI,
	kOpen_Inventory_UI,
	kOpen_Quest_UI,
	kOpen_Skill_UI,
	kOpen_CharacterStatus_UI,
	kOpen_AreaConquest_UI,
	kOpen_ContinentMap_UI,
	kModeChange_FullRadar_UI,
	kOpen_Community_UI,
	kOpen_Guild_UI,
	kOpen_GuideInfomation_UI,
	kKeyFunctionMax, //NotData
	kOpen_SystemMenu_UI,
	kOpen_SystemMenu_N_Cancel,

};

WzID MainMenuOptionDlg::m_wzId[] = 
{
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("TI13"),//MAIN_MENU_OP_CASH_SHOP,//TI13
	StrToWzID("I001"),//MAIN_MENU_OP_BATTLE_ZONE,//I001
	StrToWzID("I002"),//MAIN_MENU_OP_CHAOS_ZONE,//I002
	StrToWzID("TI00"),//MAIN_MENU_OP_INVENTORY,//TI00
	StrToWzID("TI03"),//MAIN_MENU_OP_QUEST,//TI03
	StrToWzID("TI01"),//MAIN_MENU_OP_SKILL,//TI01
	StrToWzID("TI02"),//MAIN_MENU_OP_CHARACTER,//TI02
	StrToWzID("TI12"),//MAIN_MENU_OP_AC,//TI12
	StrToWzID("I003"),//MAIN_MENU_OP_FULL_MAP,//I003
	StrToWzID("TI10"),//MAIN_MENU_OP_MAP,//TI10
	StrToWzID("TI05"),//MAIN_MENU_OP_FRIEND,//TI05
	StrToWzID("TI06"),//MAIN_MENU_OP_GUILD,//TI06
	StrToWzID("TI11"),//MAIN_MENU_OP_HELP,//TI11
	StrToWzID("TI07"),//MAIN_MENU_SNS,TI07
	StrToWzID("TI08"),//MAIN_MENU_OP_CONFIG,//TI08
	StrToWzID("TI09"),//MAIN_MENU_OP_LOGOUT,//TI09
#else	
	StrToWzID("TI11"),  //MAIN_MENU_OP_HELP,			
	StrToWzID("TI00"),  //MAIN_MENU_OP_INVENTORY,		
	StrToWzID("TI01"),  //MAIN_MENU_OP_SKILL,			
	StrToWzID("TI02"),  //MAIN_MENU_OP_CHARACTER,		
	StrToWzID("TI03"),  //MAIN_MENU_OP_QUEST,			
	StrToWzID("TI10"),  //MAIN_MENU_OP_FULL_MAP			
	StrToWzID("TI04"),  //MAIN_MENU_OP_MAP,				
	StrToWzID("TI05"),  //MAIN_MENU_OP_FRIEND,			
	StrToWzID("TI06"),  //MAIN_MENU_OP_GUILD,			
//	StrToWzID("TI07"),  //MAIN_MENU_OP_WEB,
//	StrToWzID("TI07"),  //MAIN_MENU_OP_IGC,

	StrToWzID("TI08"),  //MAIN_MENU_OP_CONFIG,			
	StrToWzID("TI09"),  //MAIN_MENU_OP_LOGOUT,	
	StrToWzID("TI12"),  //MAIN_MENU_OP_AC,	

	StrToWzID("TI13"),	//MAIN_MENU_OP_CASH_SHOP,

	StrToWzID("TI14"),  //MAIN_MENU_CONTINENT_MAP,

	StrToWzID("TI15"),  //MAIN_MENU_CREDIT,
    StrToWzID("TI07"),  //MAIN_MENU_SNS,
#endif
};						  


//---------------------------------------------------------------------------
/**
*/
MainMenuOptionDlg::MainMenuOptionDlg(void)
{
}


//---------------------------------------------------------------------------
/**
*/
MainMenuOptionDlg::~MainMenuOptionDlg(void)
{
}



//---------------------------------------------------------------------------
/**
*/
VOID	
MainMenuOptionDlg::Init( CDrawBase * pDrawBase )
{

}


//---------------------------------------------------------------------------
/**
*/
VOID	
MainMenuOptionDlg::Release()
{

}


//---------------------------------------------------------------------------
/**
*/
VOID	
MainMenuOptionDlg::MessageProc( SI_MESSAGE * pMessage )
{
	POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

	CControlWZ * pControl = getControl(pos);
	if (!pControl)
	{
		return;
	}

	if( g_pHero )
	{
		if( g_pHero->IsShowFullMap() )
		{
			return;
		}
	}

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch(pos)
			{
			case MAIN_MENU_OP_HELP:
				{
                    //열람실을 연다
                    uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
                    if (guide_manager)
                    {
                        guide_manager->OpenGuideInfoDialog();
                    }
                }
				break;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			case MAIN_MENU_CREDIT:
				{
					uiBase * pDlg = GET_DIALOG( IM_MENU_MANAGER::CREDIT_DLG );
					if( pDlg )
					{
						BOOL b = pDlg->IsVisibleWindow();
						pDlg->ShowInterface( !b );
					}
				}
				break;
#endif
			case MAIN_MENU_OP_INVENTORY:
				{
					this->ToggleShowWindow(InterfaceManager::DIALOG_EQUIP);
				}
				break;
			case MAIN_MENU_OP_SKILL:
				{
                    uiCharacterStatusMan::ShowCharacterStatusDialog(false);

					uiBase* pUIBase = g_InterfaceManager.GetUISkillCurrentDialog();
					
					if (pUIBase)
					{
						BOOL bShow = pUIBase->IsVisible();
						pUIBase->ShowInterfaceWithSound(!bShow);
					}
				}
				break;
			case MAIN_MENU_OP_CHARACTER:
				{
					uiBase* pUIBase = g_InterfaceManager.GetUISkillCurrentDialog();

					if (pUIBase)
					{
						if (pUIBase->IsVisible())
						{
							pUIBase->ShowInterface(FALSE);
						}
					}	

                    uiCharacterStatusMan::ToggleCharacterStatusDialog();
				}
				break;

			case MAIN_MENU_OP_QUEST:
				{
					uiQuestMan * pQuestMan = 
						(uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
					if (pQuestMan)
					{
						pQuestMan->OpenQuestList();
					}					
				}
				break;

			case MAIN_MENU_OP_FULL_MAP:
				{
#ifdef _NA_000000_20130114_RENEWER_UI
					if (!GlobalFunc::IsSSQScene())	// 타락한 사원에서는 맵을 열 수 없다.
					{
						uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
						if (ui_radar_manager != NULL)
						{
							ui_radar_manager->ToggleShowContinentMap();
						}
					}
#else
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
#endif
				}
				break;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			case MAIN_MENU_CONTINENT_MAP:
				{
					if (!GlobalFunc::IsSSQScene())	// 타락한 사원에서는 맵을 열 수 없다.
					{
                        uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
                        if (ui_radar_manager != NULL)
                        {
                            ui_radar_manager->ToggleShowContinentMap();
                        }
					}
				}
				break;
#endif
			case MAIN_MENU_OP_MAP:
				{
#ifdef _NA_000000_20130114_RENEWER_UI
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
#else
					if (!GlobalFunc::IsSSQScene())	// 타락한 사원에서는 맵을 열 수 없다.
					{
						BOOL bRadarShow		= GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->IsVisible();
						BOOL bRadarPlusShow = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible();
						BOOL bRadarFullShow = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_FULL)->IsVisible();

						if (!bRadarShow && !bRadarPlusShow)
						{
							// 둘다 닫혀있는 상태라면
							GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(TRUE);
							g_SoundEffect.PlayUI( UI_SOUND_MAP_ZOOM_IN );

						}
						else if (bRadarShow)
						{
							GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(FALSE);
						}
						else
						{
							GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->ShowWindow(FALSE);
							GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(TRUE);
						}
					}
#endif
				}
				break;

			case MAIN_MENU_OP_FRIEND:
				{
					uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
					if (pCommMan)
					{
						pCommMan->ShowCommunityDlg();
					}
				}
				break;

			case MAIN_MENU_OP_GUILD:
				{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                    GuildSystem::Function::ToggleGuildInfoDialog();
#else
                    // 길드 메뉴
                    uiGuildMan* guildMan =
                        static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                    if (guildMan)
                    {
                        guildMan->SetShowGuildMain();
                        guildMan->SEND_CG_GUILD_TOTALINFO_REQ_SYN();
                    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER                    
				}
				break;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
			case MAIN_MENU_OP_CONFIG:
				{
#ifdef _NA_000000_20130114_RENEWER_UI
                    uiOptionMan::ShowOptionDialog(0);
					//GET_DIALOG( IM_OPTION_MANAGER::kOptionView_ID )->ShowInterface( TRUE );
#else
					//	옵션 다이얼로그
					GET_DIALOG( IM_OPTION_MENU_MANAGER::OPTION_MENU_DIALOG )->ShowInterface( TRUE );			
#endif
					ShowWindow( FALSE );
				}
				break;
			case MAIN_MENU_OP_LOGOUT:
				{
#ifdef _GS_GSP_BLOCK_SYSTEMDLG_THEN_DEAD
                    if(GlobalFunc::IsDeadField())
                    {   
                        //필드사망시 시스템창띄우기막기
                        break;
                    }
#endif//_GS_GSP_BLOCK_SYSTEMDLG_THEN_DEAD

                    SolarDialog* trade_dialog = g_InterfaceManager.GetDialog(IM_TRADE_MANAGER::TRADE);
                    if ((trade_dialog != NULL) && (trade_dialog->IsVisible() == TRUE))
                    {
                        TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };

                        //! 5392	거래중에는 사용할 수 없는 기능입니다.
                        g_InterfaceManager.GetInterfaceString(5392, message, INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
                        break;
                    }

					g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_SYSTEM, TRUE);
				}
				break;
			case MAIN_MENU_OP_AC:
				{
					uiBase* pDlg = GET_DIALOG(IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG);

					if (pDlg) {	
						if (pDlg->IsVisibleWindow())
						{
							pDlg->ShowDialog(FALSE);				
						}
						else
						{
							pDlg->ShowDialog(TRUE);				

						}
					}
				}
				break;
#ifdef _NA_000000_20130114_RENEWER_UI
			case MAIN_MENU_OP_BATTLE_ZONE:
				{
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
			case MAIN_MENU_OP_CHAOS_ZONE:
				{
#ifdef _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
                    if ((g_pMap->IsVill() == FALSE && 
                         g_pMap->IsField() == FALSE &&
                         g_pMap->IsTutorialField() == FALSE))
#else
                    if ((g_pMap->IsVill() == FALSE && 
                         g_pMap->IsField() == FALSE))
#endif // _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE
                    {
                        return;
                    }

					uiChaosSystemMan* chaos_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
					if (chaos_manager)
					{
						chaos_manager->ToggleChaoszoneDialog();
					}
				}
				break;
#endif
			case MAIN_MENU_OP_CASH_SHOP:
				{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

    #if defined(_SUNGAME_VER)
                    if (PROGRAMPARAM->is_block_cash_shop() == false)
    #endif //defined(_SUNGAME_VER)
                    {
                        ConvenienceStore::CashShop::instance()->ToggleOpenKey();
                    }
#else
                    uiCashItemShopMan *pMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
                    if(pMan)
                        pMan->ToggleCashItemShopDlg();
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
				}
				break;
            case MAIN_MENU_SNS:
                {
                    //sns 바로가기
                    if (g_pSunRenderer && g_pSunRenderer->IsFullScreen())
                    {
                        //전체모드
                        // 5265 : 전체화면에서는 SNS 바로가기 기능을 사용할 수 없습니다. 창모드 화면에서만 사용할 수 있습니다.
                        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                        g_InterfaceManager.GetInterfaceString(5265, message, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message );
                    }
                    else
                    {
                        //창모드, 전체창모드
                        TCHAR register_url[INTERFACE_STRING_LENGTH];
                        // 85230 : http://www.facebook.com/SUNOnline1
                        g_InterfaceManager.GetInterfaceString(85230, register_url, INTERFACE_STRING_LENGTH);
                        SHELLEXECUTEINFO shell_execute_info;

                        shell_execute_info.cbSize = sizeof(SHELLEXECUTEINFO);
                        shell_execute_info.fMask = NULL;
                        shell_execute_info.hwnd = g_hWndMain;
                        shell_execute_info.lpVerb = NULL;
                        shell_execute_info.lpFile = register_url;
                        shell_execute_info.lpParameters = NULL;
                        shell_execute_info.lpDirectory = NULL;
                        shell_execute_info.nShow = SW_MAXIMIZE;
                        shell_execute_info.hInstApp = NULL;

                        ShellExecuteEx(&shell_execute_info);
                        ShowWindow(FALSE);
                    }

                }

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


//---------------------------------------------------------------------------
/**
*/
VOID			
MainMenuOptionDlg::ToggleShowWindow(InterfaceManager::eDIALOG_ID id)
{
	BOOL bShow = g_InterfaceManager.IsShowDialog(id);
	g_InterfaceManager.ShowDialog(id, !bShow );
}

//---------------------------------------------------------------------------
/**
*/
VOID			
MainMenuOptionDlg::ToggleShowWindow(WzID id)
{
	uiBase* pDlg = GET_DIALOG(id);
	pDlg->ShowDialog(!(pDlg->IsVisibleWindow()));
}

//---------------------------------------------------------------------------
/**
*/
VOID	
MainMenuOptionDlg::NetworkProc( MSG_BASE * pMsg )
{

}

//---------------------------------------------------------------------------
/**
*/
CControlWZ *	
MainMenuOptionDlg::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


//---------------------------------------------------------------------------
/**
*/
POSTYPE			
MainMenuOptionDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

//---------------------------------------------------------------------------
/**
*/
VOID	
MainMenuOptionDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		//RECT rcMain;
		//RECT rcMainOption;
		//g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_MAIN)->GetDialogWZ()->GetDlgSize(&rcMain);
		//GetDialogWZ()->GetDlgSize(&rcMainOption);

		//int x, y;
		//x = 1024 - (rcMainOption.right+17);
		//y = 768 - (rcMain.bottom + rcMainOption.bottom-10);

		//MoveWindowWZ((long)x, (long)y);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Main_Option;
		msg.wParam=InterfaceManager::DIALOG_MAIN_OPTION;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);


		CheckModifyOption();

	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Main_Option);
	}
}

VOID MainMenuOptionDlg::OnUpdateSolarDialog()
{

}
#ifdef _NA_000000_20130114_RENEWER_UI
void MainMenuOptionDlg::CheckModifyOption()
{
	//HotKeyManager::Instance()->temp_key_bind_info().key_bind_pair_map()
	for (int i = 0 ; i < Dialog_Max ; ++i)
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
			DWORD ControlTextID = Control_Key->GetTextID();

			TCHAR OutputText[256];
			g_InterfaceManager.GetInterfaceStringFormat(OutputText,256,ControlTextID,hot_key_string.c_str());

			Control_Key->SetTextWZA(OutputText);
		}

	}
}
#endif