#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildPoint.h"
#include "pointsystem/PacketStruct_CG_PointSystem.h"
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#include "GuildInfoParser.h"
#include "CurrencyInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "FTextSentence.h"
#include "FTextParser.h"

const DWORD uiGuildPoint::kDialog_ID = StrToWzID("7446");

uiGuildPoint::uiGuildPoint( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildPoint::~uiGuildPoint()
{

}

void uiGuildPoint::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B000"), kControl_Text_B000);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);

    // 정규화 텍스트 세팅
    CCtrlStaticWZ* notice_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_B000));
    if (notice_control != NULL)
    {
        FTextParser parser;
        FTextSentence sentence;
        RECT notice_rect = notice_control->GetSizeRect();

        donate_notice().Init(GetDrawBase(), notice_rect);
        donate_notice().SetVCenterAlign(true);
        donate_notice().ClearSentence();
        parser.SetDefaultFont(notice_control->GetFontID(), notice_control->GetTextColor());

        TCHAR message_string[INTERFACE_STRING_LENGTH] = {'\0', };
        // 2327 길드 코인 기부 안내문
        g_InterfaceManager.GetInterfaceString(2327, message_string);
        STLX_STRING convert_string;
        for (int char_index = 0; *(message_string + char_index) != '\0'; ++char_index)
        {
            if ((message_string[char_index] == '\\') && 
                (message_string[char_index + 1] == 'n'))
            {
                convert_string += "<RETURN=1/>";
                ++char_index;
            }
            else
            {
                convert_string += message_string[char_index];
            }
        }

        // 코인기부제한 출력
        TCHAR donation_limit[INTERFACE_STRING_LENGTH] = {'\0', };
        _itot(GuildInfoParser::Instance()->GetGuildInfo().max_guild_coin_donation_, donation_limit, 10);
        Snprintf(message_string, INTERFACE_STRING_LENGTH - 1, convert_string.c_str(), donation_limit);

        BOOL return_value = parser.Parser(message_string, sentence);
        if (return_value == TRUE)
        {
            donate_notice().Append(sentence);
        }
        donate_notice().SetBeginLineIndex(0);

        notice_control->ShowWindowWZ(WZ_HIDE);
    }
}

void uiGuildPoint::Release()
{

}

void uiGuildPoint::MessageProc( SI_MESSAGE* message )
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
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildPoint::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B001:
        {
            OnClickDonateButton();
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

bool uiGuildPoint::OnEditEnterDown( SI_MESSAGE* message )
{
    OnClickDonateButton();

    return true;
}

void uiGuildPoint::OnClickDonateButton()
{
    if (donation_coin() == 0)
    {
        // 7307 길드코인 기부는 최소 1개 이상 가능합니다.
        GlobalFunc::VerifyMessage(7307, true);
    }
    else if (donation_coin() > currenct_coin())
    {
        // 7308 길드 코인 부족 안내문
        GlobalFunc::VerifyMessage(7308, true);
    }
    else
    {
        SEND_CG_POINTWALLET_GUILDCOIN_DONATION_SYN(donation_coin());
        ShowInterface(FALSE);
    }    
}

void uiGuildPoint::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        UpdateCoinInfo();
        InitEditControl();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildPoint::kDialog_ID;
        msg.wParam = uiGuildPoint::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildPoint::kDialog_ID);
    }
}

void uiGuildPoint::OnUpdateSolarDialog()
{
    UpdateCoinInfo();

    CCtrlDxIMEEditBox* coin_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (coin_edit != NULL)
    {
        TCHAR money_coin[INTERFACE_STRING_LENGTH] = {'\0', };
        coin_edit->GetTextWZ(money_coin, INTERFACE_STRING_LENGTH - 1);
        DWORD temp_coin = _ttoi(money_coin);
        if (donation_coin() != temp_coin)
        {
            if (temp_coin > 10)
            {
                temp_coin %= 10;
            }
            if (temp_coin < 1)
            {
                temp_coin = 1;
            }
            if (temp_coin > GuildInfoParser::Instance()->GetGuildInfo().max_guild_coin_donation_)
            {
                temp_coin = GuildInfoParser::Instance()->GetGuildInfo().max_guild_coin_donation_;
            }
            set_donation_coin(static_cast<DWORD>(temp_coin));
            Snprintf(money_coin, INTERFACE_STRING_LENGTH - 1, "%d", temp_coin);
        }
        coin_edit->SetTextWZ(money_coin, true);
    }
}

void uiGuildPoint::OnRenderSolarDialog()
{
    CCtrlStaticWZ* notice_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_B000));
    if (notice_control != NULL)
    {
        RECT notice_rect = notice_control->GetSizeRect();
        donate_notice().SetRect(notice_rect);
        donate_notice().convertLineInfo();
        donate_notice().Render();
    }
}

void uiGuildPoint::UpdateCoinInfo()
{
    // 현재 소지 코인 세팅
    set_currenct_coin(0);
    uiCharacterStatusCurrencyInfo* ui_currency_info = 
        GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
    if (ui_currency_info != NULL)
    {
        uiCharacterStatusCurrencyInfo::CurrencyPointInfo* currency_point = 
            ui_currency_info->GetCurrencyPointInfo(CURRENCY_INFO_GUILD_COIN);
        if (currency_point != NULL)
        {
            set_currenct_coin(currency_point->currency_point);
        }
    }

    CCtrlStaticWZ* coin_text_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
    if (coin_text_control != NULL)
    {
        TCHAR coin_string[INTERFACE_STRING_LENGTH] = {'\0', };

        Snprintf(coin_string, 
            INTERFACE_STRING_LENGTH - 1, 
            "%d", 
            currenct_coin());
        coin_text_control->SetMoneyType(true);
        coin_text_control->SetTextWZ(coin_string);
    }
}

void uiGuildPoint::InitEditControl()
{
    // 입력 에디트 초기화
    set_donation_coin(1);
    CCtrlDxIMEEditBox* coin_edit = 
        static_cast<CCtrlDxIMEEditBox*>(GetControlWZ_byPosition(kControl_Edit_E000));
    if (coin_edit != NULL)
    {
        coin_edit->SetNumberic(true);
        coin_edit->SetLimitText(kMaxEditLength);
        coin_edit->SetTextWZ("1", true);
        coin_edit->SetColorSelBack(RGBA(0,0,0,0));
        coin_edit->SetColorSelText(RGBA(0,0,0,0));
        coin_edit->SetFocusWZ();
    }
}

void uiGuildPoint::SEND_CG_POINTWALLET_GUILDCOIN_DONATION_SYN( DWORD donation_code )
{
    MSG_CG_POINTWALLET_GUILDCOIN_DONATION_SYN send_packet;
    send_packet.point_ = donation_code;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
}
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL