#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneInfo.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "BattleGroundInfoParser.h"
#include "Map.h"
#include "GlobalFunc.h"

const DWORD uiChaosZoneInfo::kDialog_ID = StrToWzID("0644");

uiChaosZoneInfo::uiChaosZoneInfo(InterfaceManager* ui_manager)
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    RECT rc1 = {250, 8, 220+150, 8+18};
    RECT rc2 = {250, 36, 220+150, 36+18};
    gold_static_control_rect_[eS001Rect] = rc1;
    gold_static_control_rect_[eS004Rect] = rc1;
    gold_static_control_rect_[eS003Rect] = rc2;
    gold_static_control_rect_[eS005Rect] = rc2;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}
//------------------------------------------------------------------------------
uiChaosZoneInfo::~uiChaosZoneInfo()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneInfo::Init(CDrawBase* draw_base)
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR* zero_string = "0";
    // 2600	거점: %s  자원: %s/%s
    g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2600,
        zero_string, zero_string, zero_string);

    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
    if (static_control)
    {
        static_control->SetTextWZ(string);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        battle_static_control_rect_[eS004Rect] = static_control->GetSizeRect();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
    if (static_control)
    {
        static_control->SetTextWZ(string);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        battle_static_control_rect_[eS005Rect] = static_control->GetSizeRect();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (static_control)
    {
        static_control->SetTextWZ(string);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        battle_static_control_rect_[eS001Rect] = static_control->GetSizeRect();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    if (static_control)
    {
        static_control->SetTextWZ(string);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        battle_static_control_rect_[eS003Rect] = static_control->GetSizeRect();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }

    this->Lock();
}
//------------------------------------------------------------------------------
void uiChaosZoneInfo::Release()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneInfo::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneInfo::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneInfo::OnUpdateSolarDialog()
{
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    if (GlobalFunc::IsBattleGroundScene() == FALSE && 
        GlobalFunc::IsGoldRushScene() == FALSE)
    {
        this->ShowInterface(FALSE);
        return;
    }
#else
    if (GlobalFunc::IsBattleGroundScene() == FALSE)
    {
        this->ShowInterface(FALSE);
        return;
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    if (GlobalFunc::IsBattleGroundScene() == TRUE)
    {
        // 목표 점수
        DWORD goal_score = 99999;
        const BattleGroundInfo* battle_ground_info =
            BattleGroundInfoParser::Instance()->FindData(g_pMap->GetCurrentFieldID());
        if (battle_ground_info)
        {
            goal_score = battle_ground_info->goal_score;
        }

        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR point_number_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR score_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR goal_score_string[INTERFACE_STRING_LENGTH] = {0,};
        // 블루 팀 점수 갱신
        CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
        CCtrlStaticWZ* static_control_back = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
        if (static_control && static_control_back)
        {
            BATTLE_GROUND_TEAM_SCORE_INFO* const score_info =
                ChaosSystem::GetBattleGroundData().GetTeamScore(kBlue);
            if (score_info)
            {
                _itot(score_info->strongpoint_count, point_number_string, 10);
                Snprintf(score_string, INTERFACE_STRING_LENGTH, "%3d", score_info->victory_point);
                Snprintf(goal_score_string, INTERFACE_STRING_LENGTH, "%3d", goal_score);
                // 2600	거점: %s  자원: %s/%s
                g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2600,
                    point_number_string, score_string, goal_score_string);
                static_control->SetTextWZ(string);
                static_control_back->SetTextWZ(string);
            }
        }
        // 레드 팀 점수 갱신
        static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
        static_control_back = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
        if (static_control && static_control_back)
        {
            BATTLE_GROUND_TEAM_SCORE_INFO* const score_info =
                ChaosSystem::GetBattleGroundData().GetTeamScore(kRed);
            if (score_info)
            {
                _itot(score_info->strongpoint_count, point_number_string, 10);
                Snprintf(score_string, INTERFACE_STRING_LENGTH, "%3d", score_info->victory_point);
                Snprintf(goal_score_string, INTERFACE_STRING_LENGTH, "%3d", goal_score);
                // 2600	거점: %s  자원: %s/%s
                g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2600,
                    point_number_string, score_string, goal_score_string);
                static_control->SetTextWZ(string);
                static_control_back->SetTextWZ(string);
            }
        }
    }
    else if (GlobalFunc::IsGoldRushScene() == TRUE)
    {
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR score_string[INTERFACE_STRING_LENGTH] = {0,};
        // 블루 팀 점수 갱신
        CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
        CCtrlStaticWZ* static_control_back = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
        if (static_control && static_control_back)
        {
            GOLDRUSH_TEAM_SCORE_INFO* const score_info =
                ChaosSystem::GetGoldRushData().GetTeamScore(kBlue);
            if (score_info)
            {
                GlobalFunc::SetPointText(score_string, score_info->team_gold_point);
                g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2795, score_string);
                static_control->SetTextWZ(string);
                static_control_back->SetTextWZ(string);
            }
        }
        // 레드 팀 점수 갱신
        static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
        static_control_back = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
        if (static_control && static_control_back)
        {
            GOLDRUSH_TEAM_SCORE_INFO* const score_info =
                ChaosSystem::GetGoldRushData().GetTeamScore(kRed);
            if (score_info)
            {
                GlobalFunc::SetPointText(score_string, score_info->team_gold_point);
                g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2795, score_string);
                static_control->SetTextWZ(string);
                static_control_back->SetTextWZ(string);
            }
        }
    }
#else
    // 목표 점수
    DWORD goal_score = 99999;
    const BattleGroundInfo* battle_ground_info =
        BattleGroundInfoParser::Instance()->FindData(g_pMap->GetCurrentFieldID());
    if (battle_ground_info)
    {
        goal_score = battle_ground_info->goal_score;
    }

    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR point_number_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR score_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR goal_score_string[INTERFACE_STRING_LENGTH] = {0,};
    // 블루 팀 점수 갱신
    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
    CCtrlStaticWZ* static_control_back = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (static_control && static_control_back)
    {
        BATTLE_GROUND_TEAM_SCORE_INFO* const score_info =
            ChaosSystem::GetBattleGroundData().GetTeamScore(kBlue);
        if (score_info)
        {
            _itot(score_info->strongpoint_count, point_number_string, 10);
            Snprintf(score_string, INTERFACE_STRING_LENGTH, "%3d", score_info->victory_point);
            Snprintf(goal_score_string, INTERFACE_STRING_LENGTH, "%3d", goal_score);
            // 2600	거점: %s  자원: %s/%s
            g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2600,
                point_number_string, score_string, goal_score_string);
            static_control->SetTextWZ(string);
            static_control_back->SetTextWZ(string);
        }
    }
    // 레드 팀 점수 갱신
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
    static_control_back = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    if (static_control && static_control_back)
    {
        BATTLE_GROUND_TEAM_SCORE_INFO* const score_info =
            ChaosSystem::GetBattleGroundData().GetTeamScore(kRed);
        if (score_info)
        {
            _itot(score_info->strongpoint_count, point_number_string, 10);
            Snprintf(score_string, INTERFACE_STRING_LENGTH, "%3d", score_info->victory_point);
            Snprintf(goal_score_string, INTERFACE_STRING_LENGTH, "%3d", goal_score);
            // 2600	거점: %s  자원: %s/%s
            g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2600,
                point_number_string, score_string, goal_score_string);
            static_control->SetTextWZ(string);
            static_control_back->SetTextWZ(string);
        }
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}
//------------------------------------------------------------------------------
void uiChaosZoneInfo::OnRenderSolarDialog()
{

}
//------------------------------------------------------------------------------
bool uiChaosZoneInfo::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    int tab_index = -1;

    uiChaosSystemMan* chaos_manager = static_cast<uiChaosSystemMan*>(GetMan());
    if (chaos_manager == NULL)
    {
        return false;
    }

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004:
        {
        }
        break;
    case kControl_Text_S001:
        {
        }
        break;
    case kControl_Text_S003:
        {
        }
        break;
    case kControl_Text_S004:
        {
        }
        break;
    case kControl_Text_S005:
        {
        }
        break;
    case kControl_Picture_P000:
        {
        }
        break;
    case kControl_Picture_P001:
        {
        }
        break;
    default:
        {
        }
        break;
    }

    return result;
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
void uiChaosZoneInfo::BattleGroundSetting()
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR* zero_string = "0";
    // 2600	거점: %s  자원: %s/%s
    g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2600,
        zero_string, zero_string, zero_string);
    CDialogWZ* dialog_ = GetDialogWZ();

    SCTRL_INFO* control_info_ptr = NULL;
    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
    if (static_control)
    {
        RECT rc = battle_static_control_rect_[eS004Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_RIGHT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
    if (static_control)
    {
        RECT rc = battle_static_control_rect_[eS005Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_RIGHT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (static_control)
    {
        RECT rc = battle_static_control_rect_[eS001Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_RIGHT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    if (static_control)
    {
        RECT rc = battle_static_control_rect_[eS003Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_RIGHT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }

    this->Lock();
}

void uiChaosZoneInfo::GoldRushSetting()
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR score_string[INTERFACE_STRING_LENGTH] = {0, };
    Snprintf(score_string, INTERFACE_STRING_LENGTH, "%13d", 0);
    g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2795, score_string);
    CDialogWZ* dialog_ = GetDialogWZ();
    
    SCTRL_INFO* control_info_ptr = NULL;
    CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
    if (static_control)
    {
        RECT rc = gold_static_control_rect_[eS004Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_LEFT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
    if (static_control)
    {
        RECT rc = gold_static_control_rect_[eS005Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_LEFT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (static_control)
    {
        RECT rc = gold_static_control_rect_[eS001Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_LEFT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    if (static_control)
    {
        RECT rc = gold_static_control_rect_[eS003Rect];
        static_control->SetTextWZ(string);
        static_control->SetStyle(DT_LEFT);
        control_info_ptr = static_control->GetCtrlInfo();
        // 레퍼런스이기 때문에 적용된다.
        if (control_info_ptr)
        {
            control_info_ptr->m_sSizeRect.fTopX = dialog_->GetDlgX() + static_cast<float>(rc.left);
            control_info_ptr->m_sSizeRect.fTopY = dialog_->GetDlgY() + static_cast<float>(rc.top);
            control_info_ptr->m_sSizeRect.fWidth = static_cast<float>(rc.right - rc.left);
            control_info_ptr->m_sSizeRect.fHeight = static_cast<float>(rc.bottom - rc.top);
        }
    }

    this->Lock();
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH