#include "SunClientPrecompiledHeader.h"
#include "maindialog.h"
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

#ifdef _NA_000000_20130114_RENEWER_UI
#include "HotKeyManager.h"
#include "Input/KeyCommands.h"
#include "input/HotKeyInfoParser.h"
#endif

#ifdef _NA_007086_20140318_MONSTERBOOK
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
  #include "uiCommunityMan/uiCommunityMan.h"
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON

//키등록 값순서에 따라 메세지 발생시 아뒤가 잡히다 안잡히다 한다.
//TODO: 버그 확인 
WzID MainDialog::m_wzId[MAIN_MAX] = 
{ 
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("TI04"),//	MAIN_BTN_INVENTORY_TI04,
	StrToWzID("TI03"),//	MAIN_BTN_QUEST_TI03,
	StrToWzID("TI02"),//	MAIN_BTN_SKILL_TI02,
	StrToWzID("TI01"),//	MAIN_BTN_MYINFO_TI01,
#ifdef _NA_007086_20140318_MONSTERBOOK
    StrToWzID("I000"),//    MAIN_BTN_MONSTERBOOK_I000,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
    StrToWzID("I001"),//    MAIN_BTN_COMMUNITY_I001
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
	StrToWzID("TI00"),//	MAIN_BTN_MENU_TI00,
#else
	StrToWzID("TI00"),  // MENU
	StrToWzID("B002"),	// MAIN_BTN_CASH_SHOP
	StrToWzID("TI01"),
	StrToWzID("PI00"),
#endif
};

#ifdef _NA_000000_20130114_RENEWER_UI

#endif

MainDialog::MainDialog()
{
	m_DialogKey = 0;
}

MainDialog::~MainDialog()
{
}

VOID MainDialog::Init(CDrawBase* pDrawBase)
{
#ifdef _NA_000000_20130114_RENEWER_UI
	//MoveWindowWZ(0.0f, 0.0f);
#else
	ShowObserverButton();
#endif
}

VOID MainDialog::Release()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();
    }
}

BOOL MainDialog::ResetCheckButton(eMAIN_POS index)
{
#ifdef _NA_000000_20130114_RENEWER_UI
	POSTYPE pos = getCtrlIDToPos( m_wzId[index]);
	CCtrlButtonCheckWZ * pControl = (CCtrlButtonCheckWZ*)getControl(pos);
	
	if(pControl)
	{
		pControl->SetCheckState(false);
	}
#endif

	return FALSE;
}

void MainDialog::ShowObserverButton(BOOL bShow)
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	 CControlWZ* pControl = getControl(MAIN_BTN_EXIT_OBSERVER);
	 CControlWZ* pPicControl = getControl(MAIN_PIC_EXIT_OBSERVER);

	if ((pControl == NULL) || (pPicControl == NULL))
    {
		return;
    }

    if (bShow)
    {
        pControl->ShowWindowWZ(WZ_SHOW);
        pPicControl->ShowWindowWZ(WZ_SHOW);
    }
    else
    {
        pControl->ShowWindowWZ(WZ_HIDE);
        pPicControl->ShowWindowWZ(WZ_HIDE);
    }
#endif
// 
//     CDialogWZ* dialog_ptr = GetDialogWZ();
//     if (dialog_ptr != NULL && pPicControl != NULL)
//     {
//         // UI Inetersect 영역 설정 - 설정한 영역에서만 UI가 동작한다.
//         STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
//         intersect_list.clear();        
// 
//         RECT observer_rect = pPicControl->GetSizeRect();
//         RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();
//         enable_rect.top = observer_rect.bottom;
//         intersect_list.push_back(enable_rect);
// 
//         if (bShow)
//         {
//             intersect_list.push_back(observer_rect);
//         }
//     }
}

VOID MainDialog::MainBtn_StatusSwitch(eMAIN_POS idx, bool bState)
{
    POSTYPE pos = getCtrlIDToPos (m_wzId[idx]);
    CCtrlButtonCheckWZ * pControl = (CCtrlButtonCheckWZ*)getControl(pos);

    if(pControl)
        pControl->SetCheckState(bState);
}

void fnObserserExit(bool b)
{
	if (b)
	{
		StartLeaveFieldCastTimer(eDoType_ObserverMode_Leave, 5000); 
	}
}

void MainDialog::RenderToolTip()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	int x	= Mouse::Instance()->GetMouseX();
	int y	= Mouse::Instance()->GetMouseY();

	for (int i = 0 ; i < MAIN_MAX ; ++i)
	{
		if(tooltip_control_center_[i].isToolTip)
		{
			CControlWZ* Control_Key = getControl(tooltip_control_center_[i].ui_position_);
			
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
#else
	POINT pt;
	pt.x	= Mouse::Instance()->GetMouseX();
	pt.y	= Mouse::Instance()->GetMouseY();

	CControlWZ* pControl = getControl(MAIN_BTN_EXIT_OBSERVER);

	if (pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (PtInRect(&rc, pt))
		{
			uiToolTipMan* pUIMan = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

			if (pUIMan)
			{
				pUIMan->RegisterTooltipWithStringCodeByProcessWordWrap(6428);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
#endif
}


VOID MainDialog::MessageProc(SI_MESSAGE* pMessage)
{

	if (g_pHero)
	{
		if (g_pHero->IsShowFullMap())
		{
			return;
		}
	}


	switch (pMessage->eResultMsg) 
	{
#ifdef _NA_000000_20130114_RENEWER_UI
		case RT_MSG_LBUTTONCLICK:
			{
				switch (getCtrlIDToPos(pMessage->dwCtrlID))
				{
					case MAIN_BTN_INVENTORY_TI04:
						{
							BOOL bShow = g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_EQUIP);
							g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_EQUIP, !bShow );
						}
						break;
					case MAIN_BTN_SKILL_TI02:
						{
							uiBase* pUIBase = g_InterfaceManager.GetUISkillCurrentDialog();

							if (pUIBase)
							{
								BOOL bShow = pUIBase->IsVisible();
								pUIBase->ShowInterfaceWithSound(!bShow);
							}
						}
						break;
					case MAIN_BTN_QUEST_TI03:
						{
							uiQuestMan * pQuestMan = 
								(uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
							if (pQuestMan)
							{
								pQuestMan->OpenQuestList();
							}	
						}
						break;
					case MAIN_BTN_MYINFO_TI01:
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
					case MAIN_BTN_MENU_TI00:
						{
							BOOL bRet = g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_MAIN_OPTION);
							g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_MAIN_OPTION, !bRet);
						}
						break;
#ifdef _NA_007086_20140318_MONSTERBOOK
                    case MAIN_BTN_MONSTERBOOK_I000:
                        {
                            uiMonsterBookMan* manager = GET_DIALOG_MANAGER(uiMonsterBookMan, UIMAN_MONSTERBOOK);
                            if (manager)
                            {
                                manager->toggle_monster_book();
                            }
                        }
                        break;
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
                    case MAIN_BTN_COMMUNITY_I001:
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
				}
			}
		default:
			{
				SolarDialog::MessageProc(pMessage);
			}
			break;
#else
	case RT_MSG_LBUTTONCLICK:
		{
			switch (getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case MAIN_TI_MENU:
				{
					BOOL bRet = g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_MAIN_OPTION);
					g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_MAIN_OPTION, !bRet);
				}
				break;

			case MAIN_BTN_CASH_SHOP:
				{
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

			case MAIN_BTN_EXIT_OBSERVER:
				{
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {'\0',};
					g_InterfaceManager.GetInterfaceString(852, szMessage);
					GlobalFunc::SysConfirmBox(szMessage, fnObserserExit);
				}
				break;


			default:
				break;
			}

		}
		break;

	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
#endif
	}
}	

VOID MainDialog::NetworkProc(MSG_BASE* pMsg)
{

}

POSTYPE MainDialog::getCtrlIDToPos( WzID wzId )
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

CControlWZ * MainDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < MAIN_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

VOID MainDialog::OnRenderSolarDialog()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	RenderToolTip();
#else
	CControlWZ* pControl = getControl(MAIN_BTN_EXIT_OBSERVER);
	if (pControl &&  (pControl->GetState() == WZ_SHOW))
	{
		RECT rc	= pControl->GetSizeRect();
		g_InterfaceManager.RenderActivateMark(rc.left - 13, rc.top - 13);

		RenderToolTip();
	}

    static bool first = true;
    if (first)
    {
        ShowObserverButton(FALSE);
        first = false;
    }
#endif
}

#ifdef _NA_000000_20130114_RENEWER_UI
VOID MainDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		
	}
	else
	{

	}
}

void MainDialog::CheckModifyOption()
{
	memset(tooltip_control_center_,0,sizeof(stTOOLTIPCONTROL)*MAIN_MAX);

	DWORD text_string[] = 
	{
		2675,
		2676,
		2677,
		2678,
#ifdef _NA_007086_20140318_MONSTERBOOK
        2700,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
        2674,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
        0,
	};

	//해당툴팁에 상응하는 인덱스
	HOT_KEY_FUNCTION_INDEX hotkey_func_index_[] = 
	{
		//핫키와 ID가 매칭되게 넣는다
		kOpen_Inventory_UI,
		kOpen_Quest_UI,
		kOpen_Skill_UI,
		kOpen_CharacterStatus_UI,
#ifdef _NA_007086_20140318_MONSTERBOOK
        kOpen_MonsterBook_UI,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
        kOpen_Community_UI,
#endif //_NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON
		kKeyFunctionMax,
	};

	for (int i = 0 ; i < MAIN_MAX ; ++i)
	{
		if(hotkey_func_index_[i] == kKeyFunctionMax)
		{
			continue;
		}

		KEY_BIND_PAIR_MAP::iterator found_itr = 
			HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map().find(hotkey_func_index_[i]);

		if (found_itr == (HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map()).end())
		{
			continue;
		}

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
			
			hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
		}
		else
		{
			//continue;
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

#endif