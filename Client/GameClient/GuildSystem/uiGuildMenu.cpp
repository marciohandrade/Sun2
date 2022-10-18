#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMenu.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildCreate.h"
#include "GuildRightInfoParser.h"
#include "Hero.h"
#include "GlobalFunc.h"

const DWORD uiGuildMenu::kDialog_ID = StrToWzID("0731");

uiGuildMenu::uiGuildMenu( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildMenu::~uiGuildMenu()
{

}

void uiGuildMenu::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
}

void uiGuildMenu::Release()
{

}

void uiGuildMenu::MessageProc( SI_MESSAGE* message )
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

bool uiGuildMenu::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B001:
        {
            OnGuildCreateNDestroy();
        }
        break;
    case kControl_Button_B028:
        {
            GuildSystem::Function::ShowGuildRanking();
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B011:
        {
            GuildSystem::Function::ShowNoticeRegisterGuildMark();
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B012:
        {
            // ���â��
            GuildSystem::Function::ShowNoticeUseGuildWareHouse();
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B004:
        {
            ShowInterface(FALSE);
        }
        break;
    }

    return result;
}

void uiGuildMenu::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        UpdateGuildMenu();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildMenu::kDialog_ID;
        msg.wParam = uiGuildMenu::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildMenu::kDialog_ID);
    }
}

void uiGuildMenu::OnUpdateSolarDialog()
{
    UpdateGuildMenu();
}

void uiGuildMenu::OnGuildCreateNDestroy()
{
    if (GuildSystem::GetGuildSystemData().is_recv_guild_info() == true)
    {
        if (GuildSystem::Function::GetGuildDestroyFlag() == eGUILD_DELCHK_DESTROY_WAIT)
        {
            // �ػ� ���
            GlobalFunc::ConfirmMessage(6231, 0, GuildSystem::Function::Callback_GuildDestroyWait);
        }
        else
        {
            // �ػ� ��û
            // �����谡 �ִ��� üũ
            if (GuildSystem::GetGuildSystemData().IsHaveGuildRelation() == true)
            {
                // 7234 �����踦 ��� �����ؾ� ��� �ػ� ��û�� �����մϴ�.
                GlobalFunc::VerifyMessage(7234);
            }
            else
            {
                GlobalFunc::ConfirmMessage(6212, 0, GuildSystem::Function::Callback_GuildDestroy);
            }
        }
    }
    else
    {
        // ��� â��
        ShowInterface(FALSE);
        
        uiBase* ui_guild_create = GET_DIALOG(uiGuildCreate::kDialog_ID);
        if (ui_guild_create != NULL)
        {
            ui_guild_create->ShowInterface(TRUE);
        }
    }
}

void uiGuildMenu::UpdateGuildMenu()
{
    if (g_pHero == NULL)
    {
        return;
    }

    CCtrlButtonWZ* create_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001));
    CCtrlButtonWZ* mark_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B011));
    CCtrlButtonWZ* warehouse_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B012));
    CCtrlStaticWZ* message_text = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S002));

    if ((create_button == NULL) || 
        (mark_button == NULL) || 
        (warehouse_button == NULL) || 
        (message_text == NULL))
    {
        assert(create_button);
        assert(mark_button);
        assert(warehouse_button);
        assert(message_text);

        return;
    }

    DWORD button_text_id = 0;
    DWORD message_text_id = 0;
    bool enable_guild_menu = false;

    // ��尡 �ִ���
    if (GuildSystem::GetGuildSystemData().is_recv_guild_info() == true)
    {
        // ��� �ػ� ��� ������
        bool have_right = false;
        eGUILD_DUTY guild_duty = static_cast<eGUILD_DUTY>(g_pHero->GetGuildRenderInfo().m_GuildDuty);
        eGUILD_DELCHK guild_destroy_flag = GuildSystem::Function::GetGuildDestroyFlag();
        if (guild_destroy_flag == eGUILD_DELCHK_DESTROY_WAIT)
        {
            // 6230 ��� �ػ� ����
            create_button->SetTextID(6230);
            have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
                eGUILD_RIGHTS_DESTROY, guild_duty);
            GlobalFunc::EnableControl(create_button, have_right);

            // �ػ� ������̱⿡ ���� ��� ��Ȱ��
            GlobalFunc::EnableControl(mark_button, false);
            GlobalFunc::EnableControl(warehouse_button, false);
        }
        else
        {
            // 2291 ��� �ػ�
            create_button->SetTextID(2291);
            have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
                eGUILD_RIGHTS_DESTROY, guild_duty);
            GlobalFunc::EnableControl(create_button, have_right);

            // ��� ��� Ȱ��
            have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
                eGUILD_RIGHTS_GUILD_MARK, guild_duty);
            GlobalFunc::EnableControl(mark_button, have_right);
            have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
                eGUILD_RIGHTS_WAREHOUSE_SAVE, guild_duty);
            have_right |= GuildRightInfoParser::Instance()->CheckDutyForRight(
                eGUILD_RIGHTS_WAREHOUSE_OUT, guild_duty);
            GlobalFunc::EnableControl(warehouse_button, have_right);
        }

        // 7256 ��� ������ �ȳ� �޽���
        message_text_id = 7256;
    }
    else
    {
        // 4530 ��� â��
        create_button->SetTextID(4530);
        GlobalFunc::EnableControl(create_button, true);
        // 7254 ��� �̰����� �ȳ� �޽���
        message_text_id = 7254;

        // ��� �̰��Ի��·� ��� ��Ȱ��
        GlobalFunc::EnableControl(mark_button, false);
        GlobalFunc::EnableControl(warehouse_button, false);
    }

    message_text->SetTextID(message_text_id);
    message_text->SetStyle(DT_CENTER | DT_VCENTER | DT_WORDBREAK);

    
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
