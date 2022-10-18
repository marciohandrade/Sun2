#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildAuthority.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildRightInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

const DWORD uiGuildAuthority::kDialog_ID = StrToWzID("7327");

uiGuildAuthority::uiGuildAuthority( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildAuthority::~uiGuildAuthority()
{

}

void uiGuildAuthority::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009);
    RegisterControlPosition(StrToWzID("C010"), kControl_CheckBT_C010);
    RegisterControlPosition(StrToWzID("C011"), kControl_CheckBT_C011);
    RegisterControlPosition(StrToWzID("C012"), kControl_CheckBT_C012);
    RegisterControlPosition(StrToWzID("C013"), kControl_CheckBT_C013);
    RegisterControlPosition(StrToWzID("C014"), kControl_CheckBT_C014);
    RegisterControlPosition(StrToWzID("C015"), kControl_CheckBT_C015);
    RegisterControlPosition(StrToWzID("C016"), kControl_CheckBT_C016);
    RegisterControlPosition(StrToWzID("C017"), kControl_CheckBT_C017);
    RegisterControlPosition(StrToWzID("C018"), kControl_CheckBT_C018);
    RegisterControlPosition(StrToWzID("C019"), kControl_CheckBT_C019);
    RegisterControlPosition(StrToWzID("C020"), kControl_CheckBT_C020);
    RegisterControlPosition(StrToWzID("C021"), kControl_CheckBT_C021);
    RegisterControlPosition(StrToWzID("C022"), kControl_CheckBT_C022);
    RegisterControlPosition(StrToWzID("C023"), kControl_CheckBT_C023);
    RegisterControlPosition(StrToWzID("C024"), kControl_CheckBT_C024);
    RegisterControlPosition(StrToWzID("C025"), kControl_CheckBT_C025);
    RegisterControlPosition(StrToWzID("C026"), kControl_CheckBT_C026);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("S021"), kControl_Text_S021);
    RegisterControlPosition(StrToWzID("S022"), kControl_Text_S022);
    RegisterControlPosition(StrToWzID("S023"), kControl_Text_S023);
    RegisterControlPosition(StrToWzID("S024"), kControl_Text_S024);
    RegisterControlPosition(StrToWzID("S025"), kControl_Text_S025);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
}

void uiGuildAuthority::Release()
{

}

void uiGuildAuthority::MessageProc( SI_MESSAGE* message )
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

bool uiGuildAuthority::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B001:
    case kControl_Button_B004:
        {
            ShowInterface(FALSE);
        }
        break;
    }

    return result;
}

bool uiGuildAuthority::OnVScrollUp( SI_MESSAGE* message )
{
    //CCtrlVScrollWZ* scroll_control = 
    //    static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    //CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    //if ((scroll_control != NULL) && 
    //    (member_list != NULL))
    //{
    //    int range_min = 0;
    //    int range_max = 0;
    //    int currenct_position = scroll_control->GetScrollPosWZ();
    //    scroll_control->GetScrollRangeWZ(&range_min, &range_max);
    //    --currenct_position;
    //    currenct_position = max(currenct_position, 0);
    //    currenct_position = min(currenct_position, range_max);
    //    scroll_control->SetScrollPosWZ(currenct_position);

    //    member_list->SetBeginDrawIndex(currenct_position);
    //}

    return true;
}

bool uiGuildAuthority::OnVScrollDown( SI_MESSAGE* message )
{
    //CCtrlVScrollWZ* scroll_control = 
    //    static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    //CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    //if ((scroll_control != NULL) && 
    //    (member_list != NULL))
    //{
    //    int range_min = 0;
    //    int range_max = 0;
    //    int currenct_position = scroll_control->GetScrollPosWZ();
    //    scroll_control->GetScrollRangeWZ(&range_min, &range_max);
    //    ++currenct_position;
    //    currenct_position = max(currenct_position, 0);
    //    currenct_position = min(currenct_position, range_max);
    //    scroll_control->SetScrollPosWZ(currenct_position);

    //    member_list->SetBeginDrawIndex(currenct_position);
    //}

    return true;
}

bool uiGuildAuthority::OnVScrollThumb( SI_MESSAGE* message )
{
    //CCtrlVScrollWZ* scroll_control = 
    //    static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    //CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    //if ((scroll_control != NULL) && 
    //    (member_list != NULL))
    //{
    //    int currenct_position = scroll_control->GetScrollPosWZ();
    //    member_list->SetBeginDrawIndex(currenct_position);
    //}

    return true;
}

void uiGuildAuthority::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        InitAuthorityInfo();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildAuthority::kDialog_ID;
        msg.wParam = uiGuildAuthority::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildAuthority::kDialog_ID);
    }
}

void uiGuildAuthority::OnUpdateSolarDialog()
{
    UpdateAuthorityInfo();
}

void uiGuildAuthority::InitAuthorityInfo()
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        GuildRightInfoMap& guild_right_info_map = GuildRightInfoParser::Instance()->GetGuildRightInfoMap();
        int right_number = guild_right_info_map.size();

        right_number -= (kAuthorityNumberByLine * kMaxLineNumber);
        right_number = max(right_number, 0);
        int scroll_max = right_number / kAuthorityNumberByLine;
        if (right_number % kAuthorityNumberByLine > 0)
        {
            ++scroll_max;
        }

        scroll_control->SetScrollRangeWZ(0, scroll_max);
        scroll_control->SetScrollPosWZ(0);

        if (scroll_max < 1)
        {
            GlobalFunc::EnableControl(scroll_control, false);
        }
        else
        {
            GlobalFunc::EnableControl(scroll_control, true);
        }
    }

    CCtrlStaticWZ* duty_name_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (duty_name_control != NULL)
    {
        // Á÷Ã¥¸í Ãâ·Â
        eGUILD_DUTY current_duty = guild_duty();
        DWORD duty_string_code = GuildSystem::Function::GetGuildDutyStringCode(current_duty);
        duty_name_control->SetTextID(duty_string_code);
    }
}

void uiGuildAuthority::UpdateAuthorityInfo()
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    assert(scroll_control);

    GuildRightInfoMap& guild_right_info_map = GuildRightInfoParser::Instance()->GetGuildRightInfoMap();
    size_t right_number = guild_right_info_map.size();
    int current_scroll_position = 0;
    if (scroll_control != NULL)
    {
        current_scroll_position = scroll_control->GetScrollPosWZ();
    }

    CCtrlButtonCheckWZ* right_check = NULL;
    CCtrlStaticWZ* right_text = NULL;
    GuildRightInfoMap::iterator begin_itr = guild_right_info_map.begin();
    GuildRightInfoMap::iterator end_itr = guild_right_info_map.end();
    GuildRightInfo* guild_right_info = NULL;

    for (int loop_count = 0; loop_count < (current_scroll_position * kAuthorityNumberByLine); ++loop_count)
    {
        if (begin_itr == end_itr)
        {
            break;
        }
        ++begin_itr;
    }
    
    for (DWORD control_index = 0; control_index < (kAuthorityNumberByLine* kMaxLineNumber); ++control_index)
    {
        right_check = static_cast<CCtrlButtonCheckWZ*>(
            GetControlWZ_byPosition(kControl_CheckBT_C000 + control_index));
        right_text = static_cast<CCtrlStaticWZ*>(
            GetControlWZ_byPosition(kControl_Text_S002 + control_index));
        if ((right_check == NULL) || 
            (right_text == NULL))
        {
            assert(right_check);
            assert(right_text);
            continue;
        }

        if (begin_itr == end_itr)
        {
            // ÄÁÆ®·Ñ ¼û±è
            right_check->ShowWindowWZ(WZ_HIDE);
            right_text->ShowWindowWZ(WZ_HIDE);
            continue;
        }

        guild_right_info = begin_itr->second;
        if (guild_right_info == NULL)
        {
            continue;
        }

        right_check->ShowWindowWZ(WZ_SHOW);
        right_check->SetCheckState(guild_right_info->guild_right_of_duty_[guild_duty()]);
        right_text->ShowWindowWZ(WZ_SHOW);
        right_text->SetTextWZ(g_InterfaceManager.GetInterfaceString(guild_right_info->guild_right_string_code_));

        ++begin_itr;
    }
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL