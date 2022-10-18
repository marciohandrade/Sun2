#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMarkRegister.h"
#include "GuildSystem/uiGuildMarkCombination.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildMarkInfoParser.h"
#include "ItemManager.h"
#include "GlobalFunc.h"

const DWORD uiGuildMarkRegister::kDialog_ID = StrToWzID("7328");

uiGuildMarkRegister::uiGuildMarkRegister( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildMarkRegister::~uiGuildMarkRegister()
{

}

void uiGuildMarkRegister::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
}

void uiGuildMarkRegister::Release()
{

}

void uiGuildMarkRegister::MessageProc( SI_MESSAGE* message )
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

bool uiGuildMarkRegister::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B005:
        {
            OnClickRegisterButton();
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B006:
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    }
    return result;
}

void uiGuildMarkRegister::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildMarkRegister::kDialog_ID;
        msg.wParam = uiGuildMarkRegister::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildMarkRegister::kDialog_ID);
    }
}

void uiGuildMarkRegister::OnRenderSolarDialog()
{
    CControlWZ* mark_button = GetControlWZ_byPosition(kControl_Button_B000);
    if (mark_button != NULL)
    {
        RECT mark_rect = mark_button->GetSizeRect();
        GuildSystem::Function::RenderGuildMark(
            selected_background(), 
            mark_rect);
        GuildSystem::Function::RenderGuildMark(
            selected_pattern(), 
            mark_rect);
    }
}

void uiGuildMarkRegister::SetSelectedGuildMark( DWORD backgound_index, DWORD pattern_index )
{
    set_selected_background(backgound_index);
    set_selected_pattern(pattern_index);
}

void uiGuildMarkRegister::OnClickRegisterButton()
{
    // 길드인장 아이템이 있느지 체크
    POSTYPE item_slot_position;
    BOOL exsist_item = 
        ItemManager::Instance()->FindFirstItem(eITEMTYPE_GUILDMARK_SEAL, item_slot_position);
    if (exsist_item == FALSE)
    {
        // 6161 길드 인장이 인벤토리에 존재하지 않습니다.
        GlobalFunc::VerifyMessage(6161, 0, GuildSystem::Function::Callback_DeleteGuildMark, true);

        return;
    }

    // 현재 길드 마크와 같은지 체크
    if ((GuildSystem::GetGuildSystemData().GetGuildInfo().m_BackGroundIdx == selected_background()) && 
        (GuildSystem::GetGuildSystemData().GetGuildInfo().m_PatternIdx == selected_pattern()))
    {
        // 7200 이미 등록되어 있는 길드 마크입니다.
        GlobalFunc::VerifyMessage(7200, 0, GuildSystem::Function::Callback_DeleteGuildMark, true);

        return;
    }

    GuildSystem::PacketFunction::SEND_CG_GUILD_GUILDMARK_REGISTER_SYN(
        selected_background(), 
        selected_pattern(), 
        item_slot_position);
}
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
