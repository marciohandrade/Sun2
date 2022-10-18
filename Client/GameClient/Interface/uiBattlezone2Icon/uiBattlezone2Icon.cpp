#include "SunClientPrecompiledHeader.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Icon/uiBattlezone2Icon.h"
#include "uiBattlezone2Reservation/uiBattlezone2Reservation.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "InstanceDungeonScene.h"
#include "Mouse.h"
#include "SSQScene.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "BattleGroundInfoParser.h"
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//------------------------------------------------------------------------------
void uiBattlezone2Icon::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);//클릭
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);//오버
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);//평상시
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
}

uiBattlezone2Icon::uiBattlezone2Icon(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
    ui_battlezone2_manager_ = NULL;
    GetManager();
    is_activate_ = false;
    is_over_ = false;
}

uiBattlezone2Icon::~uiBattlezone2Icon(void)
{
}

void uiBattlezone2Icon::Init(CDrawBase* draw_base)
{
    RegisterControl();
    GetDialogWZ()->SetCallbackTooltip(uiToolTipMan::Callback_Tooltip);

    for (int i = 0; i < kControl_Size; ++i)
    {
        pictures_[i] = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(i));
        if (pictures_[i])
        {
            pictures_[i]->SetDiscardMessage(true);
        }
    }
    ViewIcon(kControl_Picture_P000);
    if (pictures_[kControl_Picture_P001])
    {
        is_icon_rect_[kIcon_Battlezone] = pictures_[kControl_Picture_P001]->GetSizeRect();
    }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ViewIcon(kControl_Picture_P006);
    if (pictures_[kControl_Picture_P005])
    {
        is_icon_rect_[kIcon_BattleRoom] = pictures_[kControl_Picture_P005]->GetSizeRect();
    }
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    is_activate_ = false;
    is_over_ = false;

    animation_refresh_timer_.On(2);
    battlezone_reservation_dialog_ = NULL;
}

BYTE uiBattlezone2Icon::GetMouseoverIconIndex()
{
    int mouse_x, mouse_y;
    mouse_x = Mouse::Instance()->GetMouseX();
    mouse_y = Mouse::Instance()->GetMouseY();

    int dialog_x = static_cast<int>(GetDialogWZ()->GetDlgX());
    int dialog_y = static_cast<int>(GetDialogWZ()->GetDlgY());

    mouse_x -= dialog_x;
    mouse_y -= dialog_y;

    for (BYTE i = kIcon_Battlezone; i < kIcon_Max; ++i)
    {
        if (mouse_x > is_icon_rect_[i].left && mouse_x < is_icon_rect_[i].right &&
            mouse_y > is_icon_rect_[i].top && mouse_y < is_icon_rect_[i].bottom)
        {
            return i;
        }
    }
    return kIcon_Max;
}

void uiBattlezone2Icon::Release()
{

}

void uiBattlezone2Icon::OnUpdateSolarDialog()
{
    float alpha = 0.0f;

    if (is_activate_ == false)
    {
        if (GetMouseoverIconIndex() == kIcon_Battlezone)
        {   
            ViewIcon(kControl_Picture_P001);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            ViewIcon(kControl_Picture_P006);
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            if (is_over_ == false)
            {
                if (pictures_[kControl_Picture_P001])
                {
                    pictures_[kControl_Picture_P001]->SetDiscardMessage(false);
                }
            }
            else
            {
                if (pictures_[kControl_Picture_P001])
                {
                    pictures_[kControl_Picture_P001]->SetDiscardMessage(true);
                }

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

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            }
        }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        else if (GetMouseoverIconIndex() == kIcon_BattleRoom)
        {
            ViewIcon(kControl_Picture_P005);
            ViewIcon(kControl_Picture_P000);
            if (is_over_ == false)
            {
                if (pictures_[kControl_Picture_P005])
                {
                    pictures_[kControl_Picture_P005]->SetDiscardMessage(false);
                }
            }
            else
            {
                if (pictures_[kControl_Picture_P005])
                {
                    pictures_[kControl_Picture_P005]->SetDiscardMessage(true);
                }
            }
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
                    if (last_string != NULL)
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
                else if (ChaosSystem::GetGoldRushData().is_on_reservation())
                {
                    DWORD select_mapcode = ChaosSystem::GetBattleGroundData().select_mapcode();
                    const BattleGroundInfo* battle_ground_info =
                        BattleGroundInfoParser::Instance()->FindData(static_cast<MAPCODE>(select_mapcode));
                    if (battle_ground_info)
                    {
                        WORD current_member_count = ChaosSystem::GetGoldRushData().reservation_member_count() / 2;
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

                    // 골드러쉬 대기 소요 시간
                    ChaosSystem::GetGoldRushData().GetEnterWaitTime(last_string);
                    if (last_string != NULL)
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
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        else
        {
            is_over_ = false;
            ViewIcon(kControl_Picture_P000);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            ViewIcon(kControl_Picture_P006);
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        }
    }
    else
    {
        if (GetMouseoverIconIndex() == kIcon_Battlezone)
        {
            ViewIcon(kControl_Picture_P002);
            ViewIcon(kControl_Picture_P006);
        }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        else if (GetMouseoverIconIndex() == kIcon_BattleRoom)
        {
            ViewIcon(kControl_Picture_P004);//
            ViewIcon(kControl_Picture_P000);
        }
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    }

    if (animation_refresh_timer_.Check() != TIMER_STAT_NONE)
    {
        if (animation_refresh_timer_.Check() == TIMER_STAT_FINISH)
        {
            animation_refresh_timer_.Off();
            animation_refresh_timer_.On(2);
        }
    }

    const int kMaxTimer = 2000;
    const int kHalfTimer = kMaxTimer / 2;
    int spent_time = 0;

    if (is_over_ == false && battlezone_reservation_dialog_ && pictures_[kControl_Picture_P003])
    {
        if (battlezone_reservation_dialog_->is_reservation() && !battlezone_reservation_dialog_->is_stop())
        {
            pictures_[kControl_Picture_P003]->ShowWindowWZ(WZ_SHOW);

            spent_time = kHalfTimer - static_cast<int>(animation_refresh_timer_.SpentTime());
            if (spent_time < 0)
            {
                spent_time *= -1;
            }
            alpha = spent_time * 0.001f;
            pictures_[kControl_Picture_P003]->SetTransprency(alpha);
        }
    }
}

void uiBattlezone2Icon::MouseOver_MsgProc(SI_MESSAGE* message)
{

}

void uiBattlezone2Icon::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {
        if (battlezone_reservation_dialog_ == NULL)
        {
            battlezone_reservation_dialog_ = GET_CAST_DIALOG(uiBattlezone2Reservation, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION);
        }
    }
    else
    {
        is_activate_ = false;
        is_over_ = false;
    }
}

uiBattlezone2Man* uiBattlezone2Icon::GetManager()
{
    if (ui_battlezone2_manager_ == NULL)
    {
        ui_battlezone2_manager_ = static_cast<uiBattlezone2Man*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
        assert (ui_battlezone2_manager_);
    }
    return ui_battlezone2_manager_;
}

void uiBattlezone2Icon::Process(DWORD tick_count)
{
}


void uiBattlezone2Icon::ViewIcon(BYTE index)
{
    if (index < kControl_Picture_P004)
    {
        for (int i = 0; i < kControl_Picture_P004; ++i)
        {
            if (pictures_[i])
            {
                pictures_[i]->ShowWindowWZ((index==i)?WZ_SHOW:WZ_HIDE);
            }
        }
    }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else
    {
        for (int i = kControl_Picture_P004; i < kControl_Size; ++i)
        {
            if (pictures_[i])
            {
                pictures_[i]->ShowWindowWZ((index==i)?WZ_SHOW:WZ_HIDE);
            }
        }
    }
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
}
//------------------------------------------------------------------------------
void uiBattlezone2Icon::MessageProc(SI_MESSAGE* message)
{   
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        RECT now_rect;
        if (pictures_[0])
        {
            now_rect = pictures_[0]->GetSizeRect();
        }
        if (GetMouseoverIconIndex() == kIcon_Battlezone)
        {
            message->dwCtrlID = StrToWzID("P002");
        }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        else if (GetMouseoverIconIndex() == kIcon_BattleRoom)
        {
            message->dwCtrlID = StrToWzID("P004");
        }
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        OnLButtonClick(message);
        is_activate_ = false;
        break;
    case RT_MSG_MOUSEOVER:
        {
            if (GetMouseoverIconIndex() == kIcon_Battlezone)
            {
                if (pictures_[kControl_Picture_P001])
                {
                    pictures_[kControl_Picture_P001]->SetDiscardMessage(true);
                }
                ViewIcon(kControl_Picture_P005);
            }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            else if (GetMouseoverIconIndex() == kIcon_BattleRoom)
            {
                if  (pictures_[kControl_Picture_P005])
                {
                    pictures_[kControl_Picture_P005]->SetDiscardMessage(true);
                }
                ViewIcon(kControl_Picture_P001);
            }
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            is_over_ = true;
        }
        break;
    case RT_MSG_LBUTTONDOWN:
        {
            if (GetMouseoverIconIndex() == kIcon_Battlezone)
            {
                ViewIcon(kControl_Picture_P000);
                ViewIcon(kControl_Picture_P006);
                if (is_activate_ == false)
                {
                    if (pictures_[0])
                    {
                        is_first_click_rect_ = pictures_[0]->GetSizeRect();
                    }
                }
                is_activate_ = true;
            }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            else if (GetMouseoverIconIndex() == kIcon_BattleRoom)
            {
                ViewIcon(kControl_Picture_P004);
                ViewIcon(kControl_Picture_P002);
                if (is_activate_ == false)
                {
                    if (pictures_[4])
                    {
                        is_first_click_rect_ = pictures_[4]->GetSizeRect();
                    }
                }
                is_activate_ = true;
            }
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

//------------------------------------------------------------------------------
bool uiBattlezone2Icon::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Picture_P002://클릭시
    case kControl_Picture_P001://오버시
    case kControl_Picture_P000://평상시
    case kControl_Picture_P003://평상시활성화
        {
            GetManager()->ToggleBattlezoneDialog();
        }
        break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case kControl_Picture_P004://오버시
    case kControl_Picture_P005://평상시
    case kControl_Picture_P006://평상시활성화
        {
            uiChaosSystemMan* chaos_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
            if (chaos_manager)
            {
                chaos_manager->ToggleChaoszoneDialog();
            }
        }
        break;
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    default:
        {
        }
        break;
    }
    return result;
}

#endif //_DH_BATTLEZONE2_