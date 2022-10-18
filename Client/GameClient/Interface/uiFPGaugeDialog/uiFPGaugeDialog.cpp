#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "uiFPGaugeDialog.h"
#include "HeroGaugeDialog2.h"
#include "EnduranceDialog.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationZoneMarkDlg/uiDominationZoneMarkDlg.h"
#include "Hero.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "StatusEffect.h"
#include "SkillMain.h"
#include "SkillInfoParser.h"
#include "StatusEffect.h"

const int fp_demanage_default = 4;

WzID uiFPGaugeDialog::wz_id_[eControl_Max] = 
{
    StrToWzID("P000"), //eControl_Picture_P000,
    StrToWzID("P001"), //eControl_Picture_P001,
    StrToWzID("P002"), //eControl_Picture_P002,
    StrToWzID("P003"), //eControl_Picture_P003,
    StrToWzID("P004"), //eControl_Picture_P004,
    StrToWzID("P005"), //eControl_Picture_P005,
    StrToWzID("P006"), //eControl_Picture_P006,
    StrToWzID("P007"), //eControl_Picture_P007,
    StrToWzID("P008"), //eControl_Picture_P008,
    StrToWzID("P009"), //eControl_Picture_P009,
    StrToWzID("P010"), //eControl_Picture_P010,
    StrToWzID("B000"), //eControl_Button_B000,
};

uiFPGaugeDialog::uiFPGaugeDialog() : 
tooltip_manager_ptr_(NULL)
{
}

uiFPGaugeDialog::~uiFPGaugeDialog(void)
{
}

VOID uiFPGaugeDialog::Init( CDrawBase* draw_base_ptr )
{
    ClearFP();
    if (tooltip_manager_ptr_ == NULL)
    {
        tooltip_manager_ptr_ = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    }
}

void uiFPGaugeDialog::Release()
{
    tooltip_manager_ptr_ = NULL;
}

void uiFPGaugeDialog::OnShowWindow( BOOL is_show )
{
    if ( is_show )
    {
        DefaultDialogPos();
        ClearFP();
        SetFPGauge(g_pHero->GetFP());
    }
    else
    {
        EnduranceDIalog* endurance_dialog = static_cast<EnduranceDIalog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
        if (endurance_dialog)
        {
            endurance_dialog->MoveDefaultWindowPos();
        }

        uiDominationZoneMarkDlg* domination_zone_mark = GET_CAST_DIALOG(uiDominationZoneMarkDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG);
        if (domination_zone_mark)
        {
            domination_zone_mark->MoveDefaultWindowPos();
        }
    }
}

VOID uiFPGaugeDialog::MessageProc( SI_MESSAGE* message )
{

}

void uiFPGaugeDialog::DefaultDialogPos()
{
    HeroGaugeDialog2* hero_gauge = static_cast<HeroGaugeDialog2*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE));
    EnduranceDIalog* endurance_dialog = static_cast<EnduranceDIalog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
    uiDominationZoneMarkDlg* domination_zone_mark = GET_CAST_DIALOG(uiDominationZoneMarkDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG);
    if (hero_gauge)
    {
        RECT dialog_rect;
        hero_gauge->GetDialogWZ()->GetClientRect(&dialog_rect);
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
        dialog_rect.right -= 16;
#else
        dialog_rect.right -= 25;
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
        dialog_rect.top = -4;
        MoveWindowWZ(dialog_rect.right, dialog_rect.top);

        if (endurance_dialog)
        {
            this->GetDialogWZ()->GetClientRect(&dialog_rect);
            dialog_rect.top = 4;            
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
            endurance_dialog->MoveWindowWZ(dialog_rect.right + 6, dialog_rect.top);
#else
            endurance_dialog->MoveWindowWZ(dialog_rect.right, dialog_rect.top);
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
            if (domination_zone_mark)
            {
                RECT domi_zone_mark_rect;
                endurance_dialog->GetDialogWZ()->GetClientRect(&dialog_rect);
                this->GetDialogWZ()->GetClientRect(&domi_zone_mark_rect);                
                domi_zone_mark_rect.right = dialog_rect.left - 8;
                domi_zone_mark_rect.top = dialog_rect.bottom;
                domination_zone_mark->MoveWindowWZ(domi_zone_mark_rect.right, domi_zone_mark_rect.top);
            }
        }        
    }
}

CControlWZ* uiFPGaugeDialog::getControl( POSTYPE pos )
{
    if ( pos < eControl_Picture_P000 || pos >= eControl_Max )
        return NULL;

    return GetControlWZ( wz_id_[pos] );
}

void uiFPGaugeDialog::SetFPGauge( const PLAYERFP& fp )
{
    CControlWZ* control_ptr = NULL;
    for (int i = eControl_Picture_P000; i <= eControl_Button_B000; ++i)
    {
        control_ptr = getControl(i);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
            if (i < eControl_Button_B000)
            {
                static_cast<CCtrlPictureWZ*>(control_ptr)->EnableInnerClipDraw(false);
                static_cast<CCtrlPictureWZ*>(control_ptr)->SetClipRateHeight(1.0f);
            }            
        }
    }    

    if (fp >= 10.0f)
    {
        control_ptr = getControl(eControl_Button_B000);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        control_ptr = getControl(eControl_Picture_P000);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
            static_cast<CCtrlPictureWZ*>(control_ptr)->EnableBlend(true);
        }
        return;
    }

    int jungsu = static_cast<int>(fp);
    float sosu = fp - static_cast<PLAYERFP>(jungsu);
    if (jungsu > 0)
    {
        for (int i = eControl_Picture_P001; i <= jungsu; ++i)
        {
            control_ptr = getControl(i);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
        }
    }

    CCtrlPictureWZ* picture_control = NULL;
    if (sosu > 0.0f)
    {
        picture_control = static_cast<CCtrlPictureWZ*>(getControl(jungsu + 1));
        if (picture_control)
        {
            picture_control->ShowWindowWZ(WZ_SHOW);
            picture_control->EnableInnerClipDraw(true);
            picture_control->SetClipRateHeight(0.5f);
        }
    }
}

void uiFPGaugeDialog::ClearFP()
{
    CControlWZ* control_ptr = NULL;
    for (int i = eControl_Picture_P000; i <= eControl_Button_B000; ++i)
    {
        control_ptr = getControl(i);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
    }
}

void uiFPGaugeDialog::PrintTooltip()
{
    TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR format_string2[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR int_string[INTERFACE_STRING_LENGTH] = {0, };    

    // 2701 FP. 적에게 피해를 줄 때 차오르며 FP가 많을 수록 적에게 주는 데미지가 증가합니다.\n
    // 일부 스킬들은 발동될 때 FP를 소모합니다.
    g_InterfaceManager.GetInterfaceString(2701, format_string);
    int demage_increase_percent = static_cast<int>(g_pHero->GetFP()) * fp_demanage_default;
    SKILL_EFFECT* skill_effect_ptr = NULL;
    if (g_pHero)
    {
        skill_effect_ptr = g_pHero->FindSkillEffect(eCHAR_STATE_FP_POWER_INCREASE);
        SkillData* skill_data = xGetSkillData()->GetSkillDataBySkillClassCode(eSKILL_WITCHBLADE_1102);
        if( skill_data && skill_effect_ptr != NULL)
        {
            if( SkillScriptInfo* skill_info = skill_data->GetCurSkillInfo() )
            {
                int effect_count = skill_effect_ptr->overlap_status_count_;
                BASE_ABILITYINFO* ability_info = ability_info = skill_info->GetAbilityInfoByIndex(1);
                if (ability_info)
                {
                    demage_increase_percent += ability_info->m_iParam[1] * effect_count;
                }
            }
        }
    }
    // 1600 (현재 데미지 증가량 : %s%)
    Snprintf(int_string, INTERFACE_STRING_LENGTH-1, _T("%d"), demage_increase_percent);
    g_InterfaceManager.GetInterfaceStringFormat(format_string2, INTERFACE_STRING_LENGTH, 1600, int_string);

    tooltip_manager_ptr_->InitTooltip();
    tooltip_manager_ptr_->RegisterTooltip(format_string,
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(255, 255, 255, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
    tooltip_manager_ptr_->RegisterTooltip(format_string2,
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(0, 255, 0, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);

    tooltip_manager_ptr_->SetTooltipDialogID(GetDialogKey());
}

void uiFPGaugeDialog::OnUpdateSolarDialog()
{
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();
    {
        CControlWZ* control_ptr = NULL;
        control_ptr = getControl(eControl_Picture_P000);
        if (control_ptr)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                PrintTooltip();
            }
        }
    }
}

void uiFPGaugeDialog::MoveDefaultWindowPos()
{
    if (IsVisible() == TRUE)
    {
        SolarDialog::MoveDefaultWindowPos();
        DefaultDialogPos();
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
