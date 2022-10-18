#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildCreate.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildMenu.h"
#include "GuildInfoParser.h"
#include "Hero.h"
#include "GlobalFunc.h"

const DWORD uiGuildCreate::kDialog_ID = StrToWzID("0730");
const DWORD kNormalTextColor = RGBA(255, 255, 255, 255);
const DWORD kReaiureTextColor = RGBA(205, 0, 0, 255);

uiGuildCreate::uiGuildCreate( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildCreate::~uiGuildCreate()
{

}

void uiGuildCreate::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("T101"), kControl_Text_T101);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);

    TCHAR reqiure_text[INTERFACE_STRING_LENGTH] = {'\0', };
    CCtrlStaticWZ* text_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_T101));
    if (text_control != NULL)
    {
        // 필요 레벨
        Snprintf(reqiure_text, INTERFACE_STRING_LENGTH - 1, "%d", CREATE_GUILD_LIMIT_LEVEL);
        text_control->SetTextWZ(reqiure_text);
    }

    text_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (text_control != NULL)
    {
        // 필요 하임
        const GuildInfoData& guild_info = GuildInfoParser::Instance()->GetGuildInfo();
        MONEY reqiure_heim = guild_info.guild_create_heim_;
        Snprintf(reqiure_text, INTERFACE_STRING_LENGTH - 1, "%I64u", reqiure_heim);
        text_control->SetMoneyType(true);
        text_control->SetTextWZ(reqiure_text);
    }

    CCtrlDxIMEEditBox* guild_name_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (guild_name_edit != NULL)
    {
        guild_name_edit->SetLimitText_MultibyteCheck(true);
        guild_name_edit->SetLimitText(MAX_GUILDNAME_LENGTH);
    }
}

void uiGuildCreate::Release()
{

}

void uiGuildCreate::MessageProc( SI_MESSAGE* message )
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
    case RT_MSG_EDIT_ENTERKEYDOWN:
        OnEditEnterDown(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildCreate::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B999:
    case kControl_Button_B002:
        {
            ShowInterface(FALSE);

            uiBase* ui_guild_menu = GET_DIALOG(uiGuildMenu::kDialog_ID);
            if (ui_guild_menu != NULL)
            {
                ui_guild_menu->ShowInterface(TRUE);
            }
        }
        break;
    case kControl_Button_B001:
        {
            OnClickGuildCreate();
        }
        break;
    }

    return result;
}

bool uiGuildCreate::OnEditEnterDown( SI_MESSAGE* message )
{
    OnClickGuildCreate();

    return true;
}

void uiGuildCreate::OnClickGuildCreate()
{
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };

    // 카오 상태 체크

    // 길드명 체크
    CCtrlDxIMEEditBox* guild_name_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (guild_name_edit != NULL)
    {
        guild_name_edit->GetTextWZ(temp_string, INTERFACE_STRING_LENGTH - 1);
        size_t guild_name_length = _tcslen(temp_string);
        if (guild_name_length == 0)
        {
            // 70477 길드명을 입력하세요.
            GlobalFunc::VerifyMessage(70477, 0, NULL, true);
        }
        else if (guild_name_length > MAX_GUILDNAME_LENGTH)
        {
            // 70478 16자 까지만 적용됩니다.
            GlobalFunc::VerifyMessage(70478, 0, NULL, true);
        }
        else
        {
            // 금칙어 체크
#if defined(__KR) || defined(_KOREA)
            if ((g_slangNameFilter.IsHaveReplace(temp_string) == true) || 
                (g_slangNameFilter.CheckIncludeString(temp_string) == true))
#else	
            if ((g_slangNameFilter.IsHaveReplace(temp_string) == true) || 
                (GlobalFunc::CheckIncludeStringOnAbroad(temp_string) == true))
#endif	//#if defined(__KR) || defined(_KOREA)
            {
                // 70022 금지 단어 사용
                GlobalFunc::VerifyMessage(70022, 0, NULL, true);
            }
            else
            {
                // 생성 팝업
                GlobalFunc::ConfirmMessage(70433, 0, GuildSystem::Function::Callback_GuildCreate, true);
            }
        }
    }
}

void uiGuildCreate::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        CCtrlDxIMEEditBox* guild_name_edit = 
            static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
        if (guild_name_edit != NULL)
        {
            guild_name_edit->SetTextWZ("");
        }

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildCreate::kDialog_ID;
        msg.wParam = uiGuildCreate::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildCreate::kDialog_ID);
    }
}

void uiGuildCreate::OnUpdateSolarDialog()
{
    UpdateCreateInfo();
}

void uiGuildCreate::UpdateCreateInfo()
{
    CCtrlDxIMEEditBox* guild_name_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    CCtrlStaticWZ* name_notice = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S006));

    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
    bool check_reqiure = true;

    if ((guild_name_edit != NULL) && 
        (name_notice != NULL))
    {
        
        guild_name_edit->GetTextWZA(temp_string, INTERFACE_STRING_LENGTH - 1);

        if (_tcslen(temp_string) == 0)
        {
            name_notice->ShowWindowWZ(WZ_SHOW);
            check_reqiure = false;
        }
        else
        {
            name_notice->ShowWindowWZ(WZ_HIDE);
        }
    }

    if (g_pHero != NULL)
    {
        // 필요 레벨
        CCtrlStaticWZ* text_control = 
            static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S004));
        if (text_control != NULL)
        {
            Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, "%d", g_pHero->GetLevel());
            text_control->SetTextWZA(temp_string);
            
            if (g_pHero->GetLevel() >= CREATE_GUILD_LIMIT_LEVEL)
            {
                text_control->SetTextColor(kNormalTextColor);
            }
            else
            {
                text_control->SetTextColor(kReaiureTextColor);
                check_reqiure = false;
            }
        }

        // 필요 하임
        text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
        if (text_control != NULL)
        {
            Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, "%I64u", g_pHero->GetMoney());
            text_control->SetMoneyType(true);
            text_control->SetTextWZA(temp_string);

            const GuildInfoData& guild_info = GuildInfoParser::Instance()->GetGuildInfo();
            if (g_pHero->GetMoney() >= guild_info.guild_create_heim_)
            {
                text_control->SetTextColor(kNormalTextColor);
            }
            else
            {
                text_control->SetTextColor(kReaiureTextColor);
                check_reqiure = false;
            }
        }
    }

    CCtrlButtonWZ* text_control = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001));
    if (text_control != NULL)
    {
        GlobalFunc::EnableControl(text_control, check_reqiure);
    }    
}

void uiGuildCreate::GetGuildName(TCHAR* OUT guild_name, int buffer_size)
{
    if (guild_name == NULL)
    {
        return;
    }

    CCtrlDxIMEEditBox* guild_name_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (guild_name_edit != NULL)
    {
        guild_name_edit->GetTextWZ(guild_name, buffer_size);
    }
}
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL