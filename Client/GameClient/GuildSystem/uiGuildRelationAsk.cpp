#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildInfoParser.h"
#include "uiGuildRelationAsk.h"
#include "InterfaceManager.h"
#include "globalfunc.h"

const DWORD uiGuildRelationAsk::kDialog_ID = StrToWzID("7311");

//----------------------------------------------------------------------------
/**
*/
uiGuildRelationAsk::uiGuildRelationAsk(InterfaceManager* ui_manager)
:uiBase(ui_manager)
{
}


//----------------------------------------------------------------------------
/**
*/
uiGuildRelationAsk::~uiGuildRelationAsk(void) 
{
    Release();
}


//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B000"), kControl_Text_B000);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);

    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control != NULL)
    {
        edit_control->SetLimitText(MAX_CHARNAME_LENGTH);
        edit_control->SetLimitText_MultibyteCheck(true);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::Release()
{
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::MessageProc(SI_MESSAGE* message)
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

//----------------------------------------------------------------------------
/**
*/
bool uiGuildRelationAsk::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B001:
        {
            OnClickConfirmButton();
        }
        break;
    case kControl_Button_B002:
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    }

    return result;
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::OnClickConfirmButton()
{
    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control == NULL)
    {
        ShowInterface(FALSE);
        return;
    }

    DWORD error_code = 0;
    GUILD_RELATION* guild_relation = NULL;
    TCHAR edit_text[INTERFACE_STRING_LENGTH] = {'\0', };
    edit_control->GetTextWZ(edit_text, INTERFACE_STRING_LENGTH - 1);
    switch (guild_ask_type())
    {
    case kGuild_Alliance_Relation:
    case kGuild_Hostility_Relation:
    case kGuild_Answer_Hostility_Relation:
    case kGuild_Release_Alliance_Relation:
    case kGuild_Release_Hostility_Relation:
    case kGuild_Cancel_Hostility_Relation:
        {
            error_code = ProcessGuildRelation(edit_text);
        }
        break;
    case kGuild_Invite:
        {
            GuildSystem::PacketFunction::SEND_CG_GUILD_INVITE_SYN(edit_text);
        }
        break;
    case kGuild_Rename:
        {
            temp_string() = edit_text;
            // 7331 정말 길드명을 %s로 변경하시겠습니까?
            g_InterfaceManager.GetInterfaceStringFormat(edit_text, 
                INTERFACE_STRING_LENGTH, 
                7331, 
                temp_string().c_str());
            // 길드명 변경
            GlobalFunc::ConfirmMessage(edit_text, 
                g_InterfaceManager.GetInterfaceString(1660), 
                GuildSystem::Function::Callback_GuildRename, 
                true);
        }
        break;
    }

    if (error_code != 0)
    {
        GlobalFunc::VerifyMessage(error_code);
    }
    ShowInterface(FALSE);
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::OnShowWindow(BOOL val)
{
    if (val)
    {
        InitAskInfo();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildRelationAsk::kDialog_ID;
        msg.wParam = uiGuildRelationAsk::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildRelationAsk::kDialog_ID);
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::OnUpdateSolarDialog()
{
    UpdateInputNotice();
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::InitAskInfo()
{
    TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
    DWORD title_code = 0;
    DWORD message_code = 0;
    DWORD input_notice_code = 0;
    switch (guild_ask_type())
    {
    case kGuild_Alliance_Relation:
        {
            title_code = 1048;
            message_code = 7333;
            input_notice_code = 7335;
        }
        break;
    case kGuild_Hostility_Relation:
        {
            title_code = 1049;
            input_notice_code = 7335;

            TCHAR count_string[INTERFACE_STRING_LENGTH] = {'\0', };
            int max_count = GuildInfoParser::Instance()->GetGuildInfo().max_apply_hostile_;
            int remain_count = 
                max_count - GuildSystem::GetGuildSystemData().GetRelationProclaimCount();
            _itot(remain_count, count_string, 10);
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1,
                7338, 
                count_string);
        }
        break;
    case kGuild_Answer_Hostility_Relation:
        {
            title_code = 7346;
            message_code = 7343;
            input_notice_code = 7345;
        }
        break;
    case kGuild_Release_Alliance_Relation:
        {
            title_code = 1172;
            message_code = 7334;
            input_notice_code = 7345;
        }
        break;
    case kGuild_Release_Hostility_Relation:
        {
            title_code = 1173 ;
            message_code = 7339;
            input_notice_code = 7345;
        }
        break;
    case kGuild_Cancel_Hostility_Relation:
        {
            title_code = 7347;
            message_code = 7342;
            input_notice_code = 7345;
        }
        break;
    case kGuild_Invite:
        {
            title_code = 1010;
            message_code = 7314;
            input_notice_code = 7315;
        }
        break;
    case kGuild_Rename:
        {
            title_code = 1660;
            input_notice_code = 1658;
    
            g_InterfaceManager.GetInterfaceStringFormat(
                message_string, 
                INTERFACE_STRING_LENGTH - 1,
                7330, 
                GuildSystem::GetGuildSystemData().GetGuildInfo().m_tszGuildName);
        }
        break;
    }

    CCtrlStaticWZ* text_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T001));
    if (text_control != NULL)
    {
        text_control->SetTextID(title_code);
    }
    text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_B000));
    if (text_control != NULL)
    {
        if (message_string[0] != '\0')
        {
            text_control->SetTextWZ(message_string);
        }
        else
        {
            text_control->SetTextID(message_code);
        }
        
        text_control->SetStyle(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
    }
    text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (text_control != NULL)
    {
        text_control->SetTextID(input_notice_code);
    }

    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control != NULL)
    {
        edit_control->ClearText();
        edit_control->SetFocusWZ();
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiGuildRelationAsk::UpdateInputNotice()
{
    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    CCtrlStaticWZ* text_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    CCtrlButtonWZ* button_control = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001));
    if ((edit_control == NULL) || 
        (text_control == NULL) || 
        (button_control == NULL))
    {
        assert(edit_control);
        assert(text_control);
        assert(button_control);
        
        return;
    }

    DWORD input_notice_code = 0;
    DWORD input_confirm_code = 0;
    switch(guild_ask_type())
    {
    case kGuild_Alliance_Relation:
        {
            input_notice_code = 7335;
            input_confirm_code = 7336;
        }
        break;
    case kGuild_Hostility_Relation:
        {
            input_notice_code = 7335;
            input_confirm_code = 7340;
        }
        break;
    case kGuild_Answer_Hostility_Relation:
        {
            input_notice_code = 7335;
            input_confirm_code = 7345;
        }
        break;
    case kGuild_Release_Alliance_Relation:
        {
            input_notice_code = 7335;
            input_confirm_code = 7337;
        }
        break;
    case kGuild_Release_Hostility_Relation:
        {
            input_notice_code = 7335;
            input_confirm_code = 7341;
        }
        break;
    case kGuild_Cancel_Hostility_Relation:
        {
            input_notice_code = 7335;
            input_confirm_code = 7344;
        }
        break;
    case kGuild_Invite:
        {
            input_notice_code = 7315;
            input_confirm_code = 7316;
        }
        break;
    case kGuild_Rename:
        {
            input_notice_code = 1658;
            input_confirm_code = 1658;
        }
        break;
    }

    TCHAR edit_text[INTERFACE_STRING_LENGTH] = {'\0', };
    edit_control->GetTextWZ(edit_text, INTERFACE_STRING_LENGTH - 1);
    if (_tcslen(edit_text) < 1)
    {
        text_control->SetTextID(input_notice_code);
        GlobalFunc::EnableControl(button_control, false);
    }
    else
    {
        text_control->SetTextID(input_confirm_code);
        GlobalFunc::EnableControl(button_control, true);
    }
}

DWORD uiGuildRelationAsk::ProcessGuildRelation(const TCHAR* guild_name)
{
    DWORD error_code = 0;

    if (GuildSystem::Function::IsGuildMember(guild_name) == true)
    {
        //7217 자신의 길드와는 어떤 관계도 맺을 수 없습니다.
        error_code = 7217;

        return error_code;
    }

    GUILD_RELATION* guild_relation = GuildSystem::GetGuildSystemData().GetGuildRelation(guild_name);
    switch (guild_ask_type())
    {
    case kGuild_Alliance_Relation:
        {
            if (guild_relation == NULL)
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_SYN(eGUILD_RELATION_ALLIENCE, guild_name);

                TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
                // 6176 %s 길드에 동맹관계 신청 메시지를 정상적으로 발송하였습니다.
                g_InterfaceManager.GetInterfaceStringFormat(
                    message_string, 
                    INTERFACE_STRING_LENGTH - 1,
                    6176, 
                    guild_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
            }
            else if (guild_relation->m_eRelationType == eGUILD_RELATION_ALLIENCE)
            {
                // 7362 이미 동일한 관계
                error_code = 7362;
            }
            else
            {
                // 7180 길드 관계를 맺으려는 길드와 이미 다른 관계가 성립되어 있습니다.
                error_code = 7180;
            }
        }
        break;
    case kGuild_Hostility_Relation:
        {
            if (guild_relation == NULL)
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_SYN(eGUILD_RELATION_PROCLAIM_HOSTILITY, guild_name);
            }
            else if (guild_relation->m_eRelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY)
            {
                // 7362 이미 동일한 관계
                error_code = 7362;
            }
            else
            {
                // 7180 길드 관계를 맺으려는 길드와 이미 다른 관계가 성립되어 있습니다.
                error_code = 7180;
            }
        }
        break;
    case kGuild_Answer_Hostility_Relation:
        {
            if ((guild_relation != NULL) && 
                (guild_relation->m_eRelationType == eGUILD_RELATION_ONE_SIDE_HOSTILITY))
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_BOTH_AGAINST_SYN(guild_name);
            }
            else
            {
                // 7363 적대 선포를 받지 않아 적대수락 못함
                error_code = 7363;
            }
        }
        break;
    case kGuild_Release_Alliance_Relation:
        {
            if ((guild_relation != NULL) && 
                (guild_relation->m_eRelationType == eGUILD_RELATION_ALLIENCE))
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_DELETE_SYN(eGUILD_RELATION_ALLIENCE, guild_name);
            }
            else
            {
                // 6173 동맹관계가 아닙니다.
                error_code = 6173;
            }
        }
        break;
    case kGuild_Release_Hostility_Relation:
        {
            if ((guild_relation != NULL) && 
                (guild_relation->m_eRelationType == eGUILD_RELATION_BOTH_HOSTILITY))
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_SYN(eGUILD_RELATION_CANCEL_BOTH_HOSTILITY, guild_name);

                TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
                // 7353 %s 길드에 적대관계 해제를 요청하였습니다.
                g_InterfaceManager.GetInterfaceStringFormat(
                    message_string, 
                    INTERFACE_STRING_LENGTH - 1,
                    7353, 
                    guild_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message_string);
            }
            else
            {
                // 6232 적대관계가 아닙니다.
                error_code = 6232;
            }
        }
        break;
    case kGuild_Cancel_Hostility_Relation:
        {
            if ((guild_relation != NULL) && 
                (guild_relation->m_eRelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY))
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_RELATION_DELETE_SYN(eGUILD_RELATION_PROCLAIM_HOSTILITY, guild_name);
            }
            else
            {
                // 6233 적대 선포중이 아닙니다.
                error_code = 6233;
            }
        }
        break;
    }

    return error_code;
}

void uiGuildRelationAsk::SetEditText(TCHAR* edit_text)
{
    if (edit_text == NULL)
    {
        return;
    }

    CCtrlDxIMEEditBox* edit_control = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (edit_control != NULL)
    {
        edit_control->SetTextWZ(edit_text, true);
    }
}
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
