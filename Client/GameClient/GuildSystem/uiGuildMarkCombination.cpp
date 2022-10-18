#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMarkCombination.h"
#include "GuildSystem/uiGuildMarkRegister.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "InterfaceManager.h"
#include "GuildMarkInfoParser.h"
#include "GlobalFunc.h"

const DWORD uiGuildMarkCombination::kDialog_ID = StrToWzID("7329");

uiGuildMarkCombination::uiGuildMarkCombination( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildMarkCombination::~uiGuildMarkCombination()
{

}

void uiGuildMarkCombination::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031);
    RegisterControlPosition(StrToWzID("B032"), kControl_Button_B032);
    RegisterControlPosition(StrToWzID("B033"), kControl_Button_B033);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B034"), kControl_Button_B034);
    RegisterControlPosition(StrToWzID("B035"), kControl_Button_B035);
    RegisterControlPosition(StrToWzID("B036"), kControl_Button_B036);
    RegisterControlPosition(StrToWzID("B037"), kControl_Button_B037);
    RegisterControlPosition(StrToWzID("B038"), kControl_Button_B038);
    RegisterControlPosition(StrToWzID("B039"), kControl_Button_B039);
    RegisterControlPosition(StrToWzID("B040"), kControl_Button_B040);
    RegisterControlPosition(StrToWzID("B041"), kControl_Button_B041);
    RegisterControlPosition(StrToWzID("B042"), kControl_Button_B042);
    RegisterControlPosition(StrToWzID("B043"), kControl_Button_B043);
    RegisterControlPosition(StrToWzID("B044"), kControl_Button_B044);
    RegisterControlPosition(StrToWzID("B045"), kControl_Button_B045);
    RegisterControlPosition(StrToWzID("B046"), kControl_Button_B046);
    RegisterControlPosition(StrToWzID("B047"), kControl_Button_B047);
    RegisterControlPosition(StrToWzID("B048"), kControl_Button_B048);
    RegisterControlPosition(StrToWzID("B049"), kControl_Button_B049);
    RegisterControlPosition(StrToWzID("B050"), kControl_Button_B050);
    RegisterControlPosition(StrToWzID("B051"), kControl_Button_B051);
    RegisterControlPosition(StrToWzID("B052"), kControl_Button_B052);
    RegisterControlPosition(StrToWzID("B053"), kControl_Button_B053);
    RegisterControlPosition(StrToWzID("B054"), kControl_Button_B054);
    RegisterControlPosition(StrToWzID("B055"), kControl_Button_B055);
    RegisterControlPosition(StrToWzID("B056"), kControl_Button_B056);
    RegisterControlPosition(StrToWzID("B057"), kControl_Button_B057);
    RegisterControlPosition(StrToWzID("B058"), kControl_Button_B058);
    RegisterControlPosition(StrToWzID("B059"), kControl_Button_B059);
    RegisterControlPosition(StrToWzID("B060"), kControl_Button_B060);
    RegisterControlPosition(StrToWzID("B061"), kControl_Button_B061);
    RegisterControlPosition(StrToWzID("B062"), kControl_Button_B062);
    RegisterControlPosition(StrToWzID("VS02"), kControl_VScroll_VS02);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);

    ParseGuildMarkInfo();
}

void uiGuildMarkCombination::Release()
{

}

void uiGuildMarkCombination::MessageProc( SI_MESSAGE* message )
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
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(message);
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(message);
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildMarkCombination::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C004:
    case kControl_CheckBT_C002:
        {
            OnClickBackGroundTab(control_position - kControl_CheckBT_C001);
            UpdateCheckTab();
        }
        break;
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C005:
        {
            OnClickPatternTab(control_position - kControl_CheckBT_C000);
            UpdateCheckTab();
        }
        break;
    case kControl_Button_B005:
        {
            // 확인
            OnClickRegisterButton();
        }
        break;
    case kControl_Button_B006:
    case kControl_Button_B999:
        {
            // 취소/닫기
            ShowInterface(FALSE);
        }
        break;

    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B010:
    case kControl_Button_B011:
    case kControl_Button_B012:
    case kControl_Button_B013:
    case kControl_Button_B014:
    case kControl_Button_B015:
    case kControl_Button_B016:
    case kControl_Button_B017:
    case kControl_Button_B018:
    case kControl_Button_B019:
    case kControl_Button_B020:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
    case kControl_Button_B024:
    case kControl_Button_B025:
    case kControl_Button_B026:
    case kControl_Button_B027:
    case kControl_Button_B028:
    case kControl_Button_B029:
    case kControl_Button_B030:
    case kControl_Button_B031:
    case kControl_Button_B032:
    case kControl_Button_B033:
        {
            // 배경 아이콘 클릭
            OnClickBackGroundButton(control_position - kControl_Button_B001);
        }
        break;
    case kControl_Button_B004:
    case kControl_Button_B034:
    case kControl_Button_B035:
    case kControl_Button_B036:
    case kControl_Button_B037:
    case kControl_Button_B038:
    case kControl_Button_B039:
    case kControl_Button_B040:
    case kControl_Button_B041:
    case kControl_Button_B042:
    case kControl_Button_B043:
    case kControl_Button_B044:
    case kControl_Button_B045:
    case kControl_Button_B046:
    case kControl_Button_B047:
    case kControl_Button_B048:
    case kControl_Button_B049:
    case kControl_Button_B050:
    case kControl_Button_B051:
    case kControl_Button_B052:
    case kControl_Button_B053:
    case kControl_Button_B054:
    case kControl_Button_B055:
    case kControl_Button_B056:
    case kControl_Button_B057:
    case kControl_Button_B058:
    case kControl_Button_B059:
    case kControl_Button_B060:
    case kControl_Button_B061:
    case kControl_Button_B062:
        {
            // 문양 아이콘 클릭
            OnClickPatternButton(control_position - kControl_Button_B004);
        }
        break;
    }

    return result;
}

bool uiGuildMarkCombination::OnVScrollUp( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));
    if (scroll_control == NULL)
    {
        return false;
    }

    int range_min = 0;
    int range_max = 0;
    int current_position = scroll_control->GetScrollPosWZ();
    --current_position;
    scroll_control->GetScrollRangeWZ(&range_min, &range_max);
    current_position = max(current_position, range_min);
    current_position = min(current_position, range_max);
    scroll_control->SetScrollPosWZ(current_position);

    return true;
}

bool uiGuildMarkCombination::OnVScrollDown( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(message->dwCtrlID));
    if (scroll_control == NULL)
    {
        return false;
    }

    int range_min = 0;
    int range_max = 0;
    int current_position = scroll_control->GetScrollPosWZ();
    ++current_position;
    scroll_control->GetScrollRangeWZ(&range_min, &range_max);
    current_position = max(current_position, range_min);
    current_position = min(current_position, range_max);
    scroll_control->SetScrollPosWZ(current_position);

    return true;
}

void uiGuildMarkCombination::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        InitMarkCombination();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildMarkCombination::kDialog_ID;
        msg.wParam = uiGuildMarkCombination::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildMarkCombination::kDialog_ID);

        set_background_tab(static_cast<DWORD>(-1));
        set_pattern_tab(static_cast<DWORD>(-1));
    }
}

void uiGuildMarkCombination::OnUpdateSolarDialog()
{
    UpdateCheckTab();
}

void uiGuildMarkCombination::OnRenderSolarDialog()
{
    // 마우스 휠 처리
    // 배경 영역
    SI_MESSAGE temp_message;
    CControlWZ* rect_control = GetControlWZ_byPosition(kControl_Picture_P002);
    if (rect_control != NULL)
    {
        RECT control_rect = rect_control->GetSizeRect();
        temp_message.dwCtrlID = GetControlID_byPosition(kControl_VScroll_VS02);
        switch (GetMouseWheelStatus(&control_rect))
        {
        case MOUSE_WHEEL_UP:
            {
                OnVScrollUp(&temp_message);
            }
            break;
        case MOUSE_WHEEL_DOWN:
            {
                OnVScrollDown(&temp_message);
            }
            break;
        }
    }
    // 문양 영역
    rect_control = GetControlWZ_byPosition(kControl_Picture_P012);
    if (rect_control != NULL)
    {
        RECT control_rect = rect_control->GetSizeRect();
        temp_message.dwCtrlID = GetControlID_byPosition(kControl_VScroll_V000);
        switch (GetMouseWheelStatus(&control_rect))
        {
        case MOUSE_WHEEL_UP:
            {
                OnVScrollUp(&temp_message);
            }
            break;
        case MOUSE_WHEEL_DOWN:
            {
                OnVScrollDown(&temp_message);
            }
            break;
        }
    }

    // 현재 선택된 마크를 그리기
    CCtrlButtonWZ* mark_button = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B000));
    if (mark_button != NULL)
    {
        RECT button_rect = mark_button->GetSizeRect();
        if (selected_background() != 0)
        {
            GuildSystem::Function::RenderGuildMark(selected_background(), button_rect);
        }
        
        if (selected_pattern() != 0)
        {
            GuildSystem::Function::RenderGuildMark(selected_pattern(), button_rect);
        }
    }

    // 마크 팔레트 그리기
    RenderBackGroundPalette();
    RenderPatternPalette();

    CControlWZ* register_button = GetControlWZ_byPosition(kControl_Button_B005);
    if (register_button != NULL)
    {
        if ((selected_background() == 0) || 
            (selected_pattern() == 0))
        {
            GlobalFunc::EnableControl(register_button, false);
        }
        else
        {
            GlobalFunc::EnableControl(register_button, true);
        }
    }    
}

void uiGuildMarkCombination::OnClickRegisterButton()
{
    // 길드 마크 등록창 출력
    uiGuildMarkRegister* mark_register = 
        GET_CAST_DIALOG(uiGuildMarkRegister, uiGuildMarkRegister::kDialog_ID);
    if (mark_register != NULL)
    {
        mark_register->SetSelectedGuildMark(selected_background(), selected_pattern());
        mark_register->ShowInterface(TRUE);
    }
}

void uiGuildMarkCombination::OnClickBackGroundTab( DWORD tab_index )
{
    if (tab_index == background_tab())
    {
        return;
    }

    set_background_tab(tab_index);

    int mark_count = 0;
    STLX_VECTOR<BASE_GuildMarkInfo*>* background_container = GetcurrentBackGroundContainer();
    if (background_container != NULL)
    {
        mark_count = background_container->size();
    }

    CCtrlVScrollWZ* background_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    if (background_scroll != NULL)
    {
        mark_count -= (kMarkNumberByLine * kLineNumber);
        mark_count = max(mark_count, 0);
        int scroll_max = mark_count / kMarkNumberByLine;
        if (mark_count % kMarkNumberByLine > 0)
        {
            ++scroll_max;
        }

        background_scroll->SetScrollRangeWZ(0, scroll_max);
        background_scroll->SetScrollPosWZ(0);
    }
}

void uiGuildMarkCombination::OnClickPatternTab( DWORD tab_index )
{
    if (tab_index == pattern_tab())
    {
        return;
    }

    set_pattern_tab(tab_index);

    int mark_count = 0;
    STLX_VECTOR<BASE_GuildMarkInfo*>* pattern_container = GetcurrentPatternContainer();
    if (pattern_container != NULL)
    {
        mark_count = pattern_container->size();
    }

    CCtrlVScrollWZ* pattern_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (pattern_scroll != NULL)
    {
        mark_count -= (kMarkNumberByLine * kLineNumber);
        mark_count = max(mark_count, 0);
        int scroll_max = mark_count / kMarkNumberByLine;
        if (mark_count % kMarkNumberByLine > 0)
        {
            ++scroll_max;
        }

        pattern_scroll->SetScrollRangeWZ(0, scroll_max);
        pattern_scroll->SetScrollPosWZ(0);
    }
}

void uiGuildMarkCombination::OnClickBackGroundButton( DWORD button_index )
{
    CCtrlVScrollWZ* background_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    if (background_scroll == NULL)
    {
        return;
    }

    DWORD type_index = 
        static_cast<DWORD>(background_scroll->GetScrollPosWZ() * kMarkNumberByLine) + button_index;
    DWORD selected_mark_index = 0;

    STLX_VECTOR<BASE_GuildMarkInfo*>* background_container = GetcurrentBackGroundContainer();
    if (background_container != NULL)
    {
        if (background_container->size() > type_index)
        {
            selected_mark_index = (*background_container)[type_index]->dwIndex;
        }       
    }

    set_selected_background(selected_mark_index);
}

void uiGuildMarkCombination::OnClickPatternButton( DWORD button_index )
{
    CCtrlVScrollWZ* pattern_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (pattern_scroll == NULL)
    {
        return;
    }

    DWORD type_index = 
        static_cast<DWORD>(pattern_scroll->GetScrollPosWZ() * kMarkNumberByLine) + button_index;
    DWORD selected_mark_index = 0;
    
    STLX_VECTOR<BASE_GuildMarkInfo*>* pattern_container = GetcurrentPatternContainer();
    if (pattern_container != NULL)
    {
        if (pattern_container->size() > type_index)
        {
            selected_mark_index = (*pattern_container)[type_index]->dwIndex;
        }        
    }

    set_selected_pattern(selected_mark_index);
}

void uiGuildMarkCombination::UpdateCheckTab()
{
    CCtrlButtonCheckWZ* tab_button = NULL;
    for (int tab_index = 0; tab_index < kTabNumber; ++tab_index)
    {
        tab_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C001 + tab_index));
        if (tab_button != NULL)
        {
            if (tab_index == background_tab())
            {
                tab_button->SetCheckState(true);
            }
            else
            {
                tab_button->SetCheckState(false);
            }
        }

        tab_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C000 + tab_index));
        if (tab_button != NULL)
        {
            if (tab_index == pattern_tab())
            {
                tab_button->SetCheckState(true);
            }
            else
            {
                tab_button->SetCheckState(false);
            }
        }
    }
}

void uiGuildMarkCombination::ParseGuildMarkInfo()
{
    HASH_TABLE_GUILD_MARK* gulid_mark_table = GuildMarkInfoParser::Instance()->GetHashTable();
    if (gulid_mark_table == NULL)
    {
        return;
    }

    HASH_TABLE_GUILD_MARK_ITR begin_itr = gulid_mark_table->begin();
    HASH_TABLE_GUILD_MARK_ITR end_itr = gulid_mark_table->end();
    BASE_GuildMarkInfo* guild_mark_info = NULL;
    for ( ; begin_itr != end_itr; ++begin_itr)
    {
        guild_mark_info = (*begin_itr);
        if (guild_mark_info == NULL)
        {
            continue;
        }

        switch (guild_mark_info->bySubCategory)
        {
        case eSUB_TYPE_BACKGROUND_CIRCUL:
            {
                mark_type_circle().push_back(guild_mark_info);
            }
            break;
        case eSUB_TYPE_BACKGROUND_SHIELD:
            {
                mark_type_shield().push_back(guild_mark_info);
            }
            break;
        case eSUB_TYPE_BACKGROUND_ETC:
            {
                mark_type_bg_etc().push_back(guild_mark_info);
            }
            break;
        case eSUB_TYPE_PATTERN_FIGURE:
            {
                mark_type_figure().push_back(guild_mark_info);
            }
            break;
        case eSUB_TYPE_PATTERN_SYMBOL:
            {
                mark_type_symbol().push_back(guild_mark_info);
            }
            break;
        case eSUB_TYPE_PATTERN_ETC:
            {
                mark_type_pt_etc().push_back(guild_mark_info);
            }
            break;
        }
    }
}

void uiGuildMarkCombination::InitMarkCombination()
{
    OnClickBackGroundTab(0);
    OnClickPatternTab(0);

    set_selected_background(0);
    set_selected_pattern(0);
}

void uiGuildMarkCombination::RenderBackGroundPalette()
{
    CCtrlVScrollWZ* background_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    if (background_scroll == NULL)
    {
        return;
    }

    STLX_VECTOR<BASE_GuildMarkInfo*>* background_container = GetcurrentBackGroundContainer();
    if (background_container == NULL)
    {
        return;
    }

    DWORD start_index = static_cast<DWORD>(background_scroll->GetScrollPosWZ() * kMarkNumberByLine);
    BASE_GuildMarkInfo* guild_mark_info = NULL;
    CCtrlButtonWZ* mark_button = NULL;
    for (int button_index = 0; button_index < (kMarkNumberByLine * kLineNumber); ++button_index)
    {
        if ((start_index + button_index) >= background_container->size())
        {
            break;
        }

        guild_mark_info = (*background_container)[start_index + button_index];
        if (guild_mark_info == NULL)
        {
            continue;
        }

        mark_button = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001 + button_index));
        if (mark_button == NULL)
        {
            continue;
        }

        RECT mark_rect = mark_button->GetSizeRect();
        GuildSystem::Function::RenderGuildMark(guild_mark_info, mark_rect);
    }
}

void uiGuildMarkCombination::RenderPatternPalette()
{
    CCtrlVScrollWZ* pattern_scroll = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (pattern_scroll == NULL)
    {
        return;
    }

    STLX_VECTOR<BASE_GuildMarkInfo*>* pattern_container = GetcurrentPatternContainer();
    if (pattern_container == NULL)
    {
        return;
    }

    DWORD start_index = static_cast<DWORD>(pattern_scroll->GetScrollPosWZ() * kMarkNumberByLine);
    BASE_GuildMarkInfo* guild_mark_info = NULL;
    CCtrlButtonWZ* mark_button = NULL;
    for (int button_index = 0; button_index < (kMarkNumberByLine * kLineNumber); ++button_index)
    {
        if ((start_index + button_index) >= pattern_container->size())
        {
            break;
        }

        guild_mark_info = (*pattern_container)[start_index + button_index];
        if (guild_mark_info == NULL)
        {
            continue;
        }

        mark_button = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B004 + button_index));
        if (mark_button == NULL)
        {
            continue;
        }

        RECT mark_rect = mark_button->GetSizeRect();
        GuildSystem::Function::RenderGuildMark(guild_mark_info, mark_rect);
    }
}

STLX_VECTOR<BASE_GuildMarkInfo*>* uiGuildMarkCombination::GetcurrentBackGroundContainer()
{
    switch (background_tab())
    {
    case 0:
        {
            return &mark_type_circle();
        }
        break;
    case 1:
        {
            return &mark_type_shield();
        }
        break;
    case 2:
        {
            return &mark_type_bg_etc();
        }
        break;
    }

    return NULL;
}

STLX_VECTOR<BASE_GuildMarkInfo*>* uiGuildMarkCombination::GetcurrentPatternContainer()
{
    switch (pattern_tab())
    {
    case 0:
        {
            return &mark_type_figure();
        }
        break;
    case 1:
        {
            return &mark_type_symbol();
        }
        break;
    case 2:
        {
            return &mark_type_pt_etc();
        }
        break;
    }

    return NULL;
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL