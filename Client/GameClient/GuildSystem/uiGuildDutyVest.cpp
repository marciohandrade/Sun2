#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildDutyVest.h"
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/uiGuildAuthority.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

const DWORD uiGuildDutyVest::kDialog_ID = StrToWzID("7323");

uiGuildDutyVest::uiGuildDutyVest( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildDutyVest::~uiGuildDutyVest()
{

}

void uiGuildDutyVest::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);

    CCtrlStaticWZ* member_name_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (member_name_control != NULL)
    {
        member_name_control->SetTextWZ("");
    }
}

void uiGuildDutyVest::Release()
{

}

void uiGuildDutyVest::MessageProc( SI_MESSAGE* message )
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

bool uiGuildDutyVest::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
        {
            OnClickCheckDuty(control_position - kControl_CheckBT_C000);
        }
        break;
    case kControl_Button_B000:
    case kControl_Button_B002:
    case kControl_Button_B003:
        {
            OnClickViewAuthorityButton(control_position - kControl_Button_B000);
        }
        break;
    case kControl_Button_B001:
        {
            OnClickVerify();
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B004:
    case kControl_Button_B005:
        {
            ShowInterface(FALSE);
        }
        break;
    }

    return result;
}

void uiGuildDutyVest::OnClickCheckDuty( int check_index )
{
    set_current_check_index(check_index);
}

void uiGuildDutyVest::OnClickViewAuthorityButton( int button_index )
{
    // 각 직책에 따른 권한정보UI 출력
    uiGuildAuthority* ui_guild_authority = GET_CAST_DIALOG(uiGuildAuthority, uiGuildAuthority::kDialog_ID);
    if (ui_guild_authority != NULL)
    {
        eGUILD_DUTY guild_duty = eGUILD_DUTY_NONE;
        switch(button_index)
        {
        case kSubMaster:
            {
                guild_duty = eGUILD_DUTY_SUBMASTER;
            }
            break;
        case kCommander:
            {
                guild_duty = eGUILD_DUTY_LEADER;
            }
            break;
        case kSoldier:
            {
                guild_duty = eGUILD_DUTY_REGULAR_SOLDIER;
            }
            break;
        }

        if (guild_duty != eGUILD_DUTY_NONE)
        {
            ui_guild_authority->set_guild_duty(guild_duty);
            ui_guild_authority->ShowInterface(TRUE);
        }        
    }
}

void uiGuildDutyVest::OnClickVerify()
{
    uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
    if (ui_guild_member == NULL)
    {
        return;
    }

    GUILD_MEMBER_INFO* member_info = ui_guild_member->GetSelectedMemberInfo();
    if (member_info == NULL)
    {
        return;
    }

    BYTE change_duty = eGUILD_DUTY_NONE;
    switch (current_check_index())
    {
    case kSubMaster:
        {
            change_duty = eGUILD_DUTY_SUBMASTER;
        }
        break;
    case kCommander:
        {
            change_duty = eGUILD_DUTY_LEADER;
        }
        break;
    case kSoldier:
        {
            change_duty = eGUILD_DUTY_REGULAR_SOLDIER;
        }
        break;
    }

    if (change_duty == member_info->m_eGuildDuty)
    {
        // 현재 직책과 같으므로 플로우 생략
        return;
    }

    if (change_duty != eGUILD_DUTY_NONE)
    {
        GuildSystem::PacketFunction::SEND_CG_GUILD_CHANGE_DUTY_SYN(member_info, change_duty);
    }
}

void uiGuildDutyVest::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
        if (ui_guild_member != NULL)
        {
            GUILD_MEMBER_INFO* member_info = ui_guild_member->GetSelectedMemberInfo();
            if (member_info != NULL)
            {
                switch (member_info->m_eGuildDuty)
                {
                case eGUILD_DUTY_SUBMASTER:
                    {
                        set_current_check_index(kSubMaster);
                    }
                    break;
                case eGUILD_DUTY_LEADER:
                    {
                        set_current_check_index(kCommander);
                    }
                    break;
                case eGUILD_DUTY_REGULAR_SOLDIER:
                    {
                        set_current_check_index(kSoldier);
                    }
                    break;
                default:
                    {
                        set_current_check_index(kMaxIndex);
                    }
                    break;
                }
            }
        }

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildDutyVest::kDialog_ID;
        msg.wParam = uiGuildDutyVest::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildDutyVest::kDialog_ID);
    }
}

void uiGuildDutyVest::OnUpdateSolarDialog()
{
    UpdateButtonState();
}

void uiGuildDutyVest::UpdateButtonState()
{
    uiGuildMember* ui_guild_member = GET_CAST_DIALOG(uiGuildMember, uiGuildMember::kDialog_ID);
    if (ui_guild_member == NULL)
    {
        return;
    }

    GUILD_MEMBER_INFO* member_info = ui_guild_member->GetSelectedMemberInfo();

    CCtrlStaticWZ* member_name_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (member_name_control != NULL)
    {
        if (member_info == NULL)
        {
            member_name_control->SetTextWZ("");
        }
        else
        {
            member_name_control->SetTextWZ(member_info->m_tszCharName);
        }
    }

    eGUILD_DUTY guild_duty = eGUILD_DUTY_NONE;
    if (member_info != NULL)
    {
        guild_duty = member_info->m_eGuildDuty;
    }
    
    CCtrlButtonCheckWZ* duty_check_button = NULL;
    for (int check_index = 0; check_index < kMaxIndex; ++check_index)
    {
        duty_check_button = static_cast<CCtrlButtonCheckWZ*>(
            GetControlWZ_byPosition(kControl_CheckBT_C000 + check_index));
        if (duty_check_button == NULL)
        {
            continue;
        }
        
        if (current_check_index() == check_index)
        {
            duty_check_button->SetCheckState(true);
        }
        else
        {
            duty_check_button->SetCheckState(false);
        }
    }
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL