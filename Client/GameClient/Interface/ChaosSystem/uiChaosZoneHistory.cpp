#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneHistory.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"

const DWORD uiChaosZoneHistory::kDialog_ID = IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_HISTORY;

uiChaosZoneHistory::uiChaosZoneHistory(InterfaceManager* ui_manager) :
scroll_control_current_(NULL),
scroll_control_total_(NULL),
list_control_current_(NULL),
list_control_total_(NULL)
{
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    //RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("L003"), kControl_List_L003);

    once_make_list_ = false;
    last_select_index_ = 0;
}
//------------------------------------------------------------------------------
uiChaosZoneHistory::~uiChaosZoneHistory()
{
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::Init(CDrawBase* draw_base)
{
    CCtrlStaticWZ* static_control = NULL;
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S000)));
    if (static_control)
    {
        // 2576 카오스 히스토리
        static_control->SetTextID(2576);
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S001)));
    if (static_control)
    {
        // 2591 %s 전적
        static_control->SetTextID(2591);
    }
    static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S005)));
    if (static_control)
    {
        // 2591 %s 전적
        static_control->SetTextID(2591);
    }

    //버튼 비활성화
    CCtrlButtonCheckWZ* check_control = NULL;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B001)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B002)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#else
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B004)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    list_control_current_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L002)));
    if (list_control_current_)
    {
        RECT list_rect = list_control_current_->GetSizeRect();
        ftext_render_current_.Init(draw_base, list_rect);

        ftext_render_current_.SetMargin(0,0,0,-5);
        ftext_render_current_.SetGap(0,0);

        ftext_render_current_.convertLineInfo();
    }
    list_control_total_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L003)));
    if (list_control_total_)
    {
        RECT list_rect = list_control_total_->GetSizeRect();
        ftext_render_total_.Init(draw_base, list_rect);

        ftext_render_total_.SetMargin(0,0,0,-5);
        ftext_render_total_.SetGap(0,0);

        ftext_render_total_.convertLineInfo();
    }
    // 스크롤
    scroll_control_current_ = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
    if (scroll_control_current_)
    {
        scroll_control_current_->SetScrollPosWZ(0);
        scroll_control_current_->SetScrollRangeWZ(0,1);
    }
    scroll_control_total_ = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
    if (scroll_control_total_)
    {
        scroll_control_total_->SetScrollPosWZ(0);
        scroll_control_total_->SetScrollRangeWZ(0,1);
    }

    // 입장 버튼 숨김
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_B005));
    if (control)
    {
        control->ShowWindowWZ(WZ_HIDE);
    }

    // 트리 메뉴
    select_index_.menu_depth[3] = 1;
    select_index_.menu_depth[2] = 1;
    select_index_.menu_depth[1] = 0;
    select_index_.menu_depth[0] = 0;
    last_select_index_ = 0;

    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_Chaoszone_HistoryTree,
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000)),
        NULL,
        true, 
        NULL);
    tree_menu().set_menu_height_gab(20);

	TCHAR string_buffer[BASE_TreeMenuInfo::kNameLength] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

    BYTE* menu_depth_count = ChaosZoneTreeParser::Instance()->max_menu_depth();
    const ChaosZoneTreeInfo* find_info;

    TreeMenuGroup::TreeObject* tree_object = NULL;

    // 트리 리스트 생성하기  /*트리를 3단으로 할 때는 주석을 지워주세요*/
    if (once_make_list_ == false)
    {
        once_make_list_ = true;

        TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_Chaoszone_HistoryTree);

        for (BYTE LGroupCount = 1; LGroupCount <= menu_depth_count[0]; ++LGroupCount)
        {
            for (BYTE SGroupCount = 1; SGroupCount <= menu_depth_count[1]; ++SGroupCount)
            {
                //for (BYTE OrderCount = 1; OrderCount <= menu_depth_count[2]; ++OrderCount)
                {
                    find_info = ChaosZoneTreeParser::Instance()->GetTreeMenu(LGroupCount, SGroupCount, 0/*OrderCount*/);
                    if (find_info != NULL)
                    {
                        if (SGroupCount == 1 /*&& OrderCount == 1*/)
                        {
                            MakeChaoszoneTree(LGroupCount, 0, 0, 0, 0, find_info->title_string_code, NULL, find_info->font_color);
                        }
                        /*if (OrderCount == 1)
                        {
                            MakeChaoszoneTree(LGroupCount, SGroupCount, 0, 0, 1, find_info->subtitle_string_code, NULL, find_info->font_color);
                        }*/

                        g_InterfaceManager.GetInterfaceString(find_info->subtitle_string_code /*find_info->mission_string_code*/, temp_string);
                        Snprintf(string_buffer, sizeof(string_buffer), temp_string);
                        MakeChaoszoneTree(LGroupCount, SGroupCount, 0/*OrderCount*/, 0, 1/*2*/, 0, string_buffer, find_info->font_color);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        tree_menu().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_Chaoszone_HistoryTree);
        tree_menu().Fill();

        tree_object = tree_menu().Find(1,1,0,0);
        if (tree_object != NULL)
        {
            tree_menu().Select(tree_object);
        }
        
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::Release()
{
    tree_menu().Release();
    last_select_index_ = 0;
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::MessageProc(SI_MESSAGE* message)
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
    case RT_MSG_SB_PAGETOP:			//. 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP:			//.윗쪽 화살표를 눌렀다.
        {
            if (scroll_control_current_ &&
                (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001))
            {

                int pos = scroll_control_current_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                int mmin = 0 , mmax = 0;
                scroll_control_current_->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

                if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
                {
                    --pos;      // 한줄 감소 시키고 
                    scroll_control_current_->SetScrollPosWZ(pos);
                    ftext_render_current_.SetBeginLineIndex(pos);
                }
            }
            if (scroll_control_total_ &&
                (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000))
            {

                int pos = scroll_control_total_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                int mmin = 0 , mmax = 0;
                scroll_control_total_->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

                if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
                {
                    --pos;      // 한줄 감소 시키고 
                    scroll_control_total_->SetScrollPosWZ(pos);
                    ftext_render_current_.SetBeginLineIndex(pos);
                }
            }
        }
        break;

    case RT_MSG_SB_PAGEBOTTOM:				//. 아래쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM:				//.아랫쪽 화살표를 눌렀다
        {
            if (scroll_control_current_ &&
                (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001))
            {
                int pos = scroll_control_current_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                int mmin = 0 , mmax = 0;
                scroll_control_current_->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

                if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
                {
                    pos++;      // 한줄 증가 시키고 
                    scroll_control_current_->SetScrollPosWZ(pos);
                    ftext_render_current_.SetBeginLineIndex(pos);
                }
            }
            if (scroll_control_total_ &&
                (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000))
            {
                int pos = scroll_control_total_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                int mmin = 0 , mmax = 0;
                scroll_control_total_->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

                if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
                {
                    pos++;      // 한줄 증가 시키고 
                    scroll_control_total_->SetScrollPosWZ(pos);
                    ftext_render_current_.SetBeginLineIndex(pos);
                }
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
        {
            if (scroll_control_current_ &&
                (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001))
            {
                int pos = scroll_control_current_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                ftext_render_current_.SetBeginLineIndex(pos);
            }
            if (scroll_control_total_ &&
                (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000))
            {
                int pos = scroll_control_total_->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
                ftext_render_current_.SetBeginLineIndex(pos);
            }
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::UpdateMouseWheel()
{
    if ((list_control_current_ == NULL) || (scroll_control_current_ == NULL) ||
        (list_control_total_ == NULL) || (scroll_control_total_ == NULL))
    {
        return;
    }


    int scroll_max_line = 0;
    size_t list_max_line = ftext_render_current_.GetDrawLineInRect();
    if (ftext_render_current_.GetMaxLine() > list_max_line)
    {
        scroll_max_line = ftext_render_current_.GetMaxLine() - list_max_line;
    }
    scroll_control_current_->SetScrollRangeWZ(0, scroll_max_line);

    RECT list_rect = list_control_current_->GetSizeRect();
    ftext_render_current_.SetRect(list_rect);
    switch (GetMouseWheelStatus(&list_rect))
    {
    case MOUSE_WHEEL_UP:
        {
            int index = scroll_control_current_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_current_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index > scroll_min)
            {
                --index;
                ftext_render_current_.SetBeginLineIndex(index);
                scroll_control_current_->SetScrollPosWZ(index);
            }
        }
        break;
    case MOUSE_WHEEL_DOWN:
        {
            int index = scroll_control_current_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_current_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index < scroll_max)
            {
                ++index;
                ftext_render_current_.SetBeginLineIndex(index);
                scroll_control_current_->SetScrollPosWZ(index);
            }
        }
        break;
    default:
        break;
    }

    //
    scroll_max_line = 0;
    list_max_line = ftext_render_total_.GetDrawLineInRect();
    if (ftext_render_total_.GetMaxLine() > list_max_line)
    {
        scroll_max_line = ftext_render_total_.GetMaxLine() - list_max_line;
    }
    scroll_control_total_->SetScrollRangeWZ(0, scroll_max_line);

    list_rect = list_control_total_->GetSizeRect();
    ftext_render_total_.SetRect(list_rect);
    switch (GetMouseWheelStatus(&list_rect))
    {
    case MOUSE_WHEEL_UP:
        {
            int index = scroll_control_total_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_total_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index > scroll_min)
            {
                --index;
                ftext_render_total_.SetBeginLineIndex(index);
                scroll_control_total_->SetScrollPosWZ(index);
            }
        }
        break;
    case MOUSE_WHEEL_DOWN:
        {
            int index = scroll_control_total_->GetScrollPosWZ();
            int scroll_min = 0;
            int scroll_max = 0;
            scroll_control_total_->GetScrollRangeWZ(&scroll_min, &scroll_max);
            if (index < scroll_max)
            {
                ++index;
                ftext_render_total_.SetBeginLineIndex(index);
                scroll_control_total_->SetScrollPosWZ(index);
            }
        }
        break;
    default:
        break;
    }
}
void uiChaosZoneHistory::RefreshHistryInfo()
{
    _CHAOSZONE_TREE_INDEX current_index;
    current_index.menu_index = last_select_index_;

    const ChaosZoneTreeInfo* find_info = ChaosZoneTreeParser::Instance()->GetTreeMenu(current_index.menu_depth[3],
        current_index.menu_depth[2], 0 /*current_index.menu_depth[1]*/);

    if (find_info)
    {
        InitHistroyInfo(find_info);
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::OnUpdateSolarDialog()
{
    UpdateMouseWheel();

    if (last_select_index_ != select_index_.menu_index) //인덱스가 바뀐경우만 체크 1회 업데이트 마다 안함
    {
        last_select_index_ = select_index_.menu_index;

        const ChaosZoneTreeInfo* find_info = ChaosZoneTreeParser::Instance()->GetTreeMenu(select_index_.menu_depth[3],
            select_index_.menu_depth[2], 0 /*select_index_.menu_depth[1]*/);

        if (find_info)
        {
            InitHistroyInfo(find_info);
        }
    }
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::OnRenderSolarDialog()
{    
    ftext_render_current_.Render();
    ftext_render_total_.Render();
    RenderTooltip();
}
void uiChaosZoneHistory::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {
        //체크버튼
        CCtrlButtonCheckWZ* check_control = NULL;
        for (int i = kControl_Button_B000;i <= kControl_Button_B007; ++i)
        {
            check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(i)));
            if (check_control)
            {
                check_control->SetCheckState(false);
            }
        }

        check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B007)));
        if (check_control)
        {
            check_control->SetCheckState(true);
        }
    }
}
//------------------------------------------------------------------------------
bool uiChaosZoneHistory::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    int tab_index = -1;

    uiChaosSystemMan* chaos_manager = static_cast<uiChaosSystemMan*>(GetMan());
    if (chaos_manager == NULL)
    {
        return false;
    }

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true)
    {
        SelectIndex(tree_object);
    }

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            chaos_manager->ShowChaosZone(uiChaosZoneHistory::kDialog_ID, false);
        }
        break;
    case kControl_Button_B005:
        {
        }
        break;
    case kControl_Button_B000:
        {
            tab_index = 0;
        }
        break;
    case kControl_Button_B001:
        {
            tab_index = 1;
        }
        break;
    case kControl_Button_B002:
        {
            tab_index = 2;
        }
        break;
    case kControl_Button_B003:
        {
            tab_index = 3;
        }
        break;
    case kControl_Button_B004:
        {
            tab_index = 4;
        }
        break;
    case kControl_Button_B007:
        {
            tab_index = 5;
        }
        break;
    default:
        {
        }
        break;
    }

    if (tab_index != -1)
    {
        chaos_manager->OpenChaosZone(tab_index, true);
    }

    return result;
}
//------------------------------------------------------------------------------
void uiChaosZoneHistory::MakeChaoszoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string, const DWORD* font_colors)
{
    BASE_TreeMenuInfo tree_menu_info;
    ZeroMemory(&tree_menu_info, sizeof(tree_menu_info));

    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Enable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Disable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Over] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Select] = StrToWzID("st10");
    

    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Enable] = font_colors[ChaosZoneTreeInfo::kFontType_Enable];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = font_colors[ChaosZoneTreeInfo::kFontType_Disable];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Over] = font_colors[ChaosZoneTreeInfo::kFontType_Over];
    tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Select] = font_colors[ChaosZoneTreeInfo::kFontType_Select];


    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = 10007;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = 10002;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = 10003;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Cursor] = 10008;


    tree_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Chaoszone_HistoryTree;
    tree_menu_info.menu_depth[3] = depth_3;
    tree_menu_info.menu_depth[2] = depth_2;
    tree_menu_info.menu_depth[1] = depth_1;
    tree_menu_info.menu_depth[0] = depth_0;
    tree_menu_info.string_code = string_code;

    if (string_code == 0)
    {
        strncpy(tree_menu_info.string_name, string, sizeof(tree_menu_info.string_name));
    }

    switch (depth)
    {
    case 0:
        tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
        break;
        //case 1:
        //tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
        //tree_menu_info.menu_depth_width = 20;
        //break;
    case 1:
        tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
        tree_menu_info.menu_depth_width = 20;

        tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 10006;
        tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 10005;
        break;
    default:
        tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_None;
        break;
    }

    TreeMenuInfoParser::Instance()->Insert(tree_menu_info);

}

//-----------------------------------------------------------------------------
void uiChaosZoneHistory::RenderTooltip()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    int tap_controls[] = {kControl_Button_B000, kControl_Button_B001,0 /*kControl_Button_B002*/,
        kControl_Button_B003, kControl_Button_B004, kControl_Button_B007};
#else
    int tap_controls[] = {kControl_Button_B000,0 /*kControl_Button_B001*/,0 /*kControl_Button_B002*/,
        kControl_Button_B003, kControl_Button_B004, kControl_Button_B007};
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
    int tap_controls[] = {kControl_Button_B000,0 /*kControl_Button_B001*/,0 /*kControl_Button_B002*/,
        kControl_Button_B003,0 /*kControl_Button_B004*/, kControl_Button_B007};
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    CControlWZ* control;
    for (int index = 0; index < sizeof(tap_controls)/sizeof(tap_controls[0]); ++index)
    {
        if (tap_controls[index] == 0)
        {
            continue;
        }
        control = GetControlWZ_byPosition(tap_controls[index]);
        if (control)
        {
            RECT position = control->GetSizeRect();
            if (position.left < mouse_x && position.right > mouse_x &&
                position.top < mouse_y && position.bottom > mouse_y)
            {
                // 2571 ~ 2576 / 이스라필의 대지~ 카오스 히스토리
                tooltip_manager->RegisterTooltipWithStringCode(2571 + index);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}

//------------------------------------------------------------------------------
bool uiChaosZoneHistory::SelectIndex(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object->info->menu_depth[2] <= 0)
    {
        return false;//항목이 아니다. 폴더이다.
    }

    /*if (tree_object->is_enable == false)
    {
    return false;//비활성 항목 이다. 
    }*/

    TreeMenuGroup::TreeObject* find_tree_object = tree_menu().Find(select_index_.menu_index);
    //이전에 선택했던 것은 체크 끄기
    if (find_tree_object)
    {
        find_tree_object->is_check = false;
    }
    //선택한것은 항상 체크
    select_index_.menu_index = tree_object->info->menu_index;
    if (tree_object)
    {
        tree_object->is_check = true;
    }

    return true;
}

void uiChaosZoneHistory::InitHistroyInfo(const ChaosZoneTreeInfo* history_info)
{
    ftext_render_current_.ClearSentence();
    ftext_render_total_.ClearSentence();

    if (history_info == NULL)
    {
        return;
    }

    FTextParser parser;
    FTextSentence sentence;
    FTextSentence temp_sentence;

    TCHAR temp_string[INTERFACE_STRING_LENGTH]= " ";
    TCHAR str_format[INTERFACE_STRING_LENGTH]={0,};
    BOOL result = false;

    // 빈줄
    parser.Parser(temp_string, temp_sentence);

    if (history_info->menu_depth[0] == 1)
    {
        // 전장 전적
        BattleGroundRecord battle_ground_record;
        for (int i = 0; i < CHAOS_ZONE_RECORD_INFOS::kRecordMaxCount; ++i)
        {
            if (history_info->map_code ==
                ChaosSystem::GetChaosZoneData().chaos_zone_record().battle_ground_record[i].GetMapCode())
            {
                battle_ground_record = ChaosSystem::GetChaosZoneData().chaos_zone_record().battle_ground_record[i];
            }
        }

        CCtrlStaticWZ* static_control = NULL;
        static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S001)));
        if (static_control)
        {
            g_InterfaceManager.GetInterfaceString(history_info->subtitle_string_code,
                temp_string, INTERFACE_STRING_LENGTH);

            // 2591 %s 전적
            g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2591, temp_string);
            static_control->SetTextWZ(str_format);
        }

        _itot(battle_ground_record.total_count, temp_string, 10);
        // 2581 참여 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2581, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(battle_ground_record.win_count, temp_string, 10);
        // 2582 승리 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2582, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(battle_ground_record.draw_count, temp_string, 10);
        // 2634 무승부 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2634, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(battle_ground_record.lose_count, temp_string, 10);
        // 2635 패배 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2635, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        float win_ratio = 0.0f;
        if (battle_ground_record.total_count != 0)
        {
            win_ratio = (battle_ground_record.win_count/static_cast<float>(battle_ground_record.total_count)) * 100.0f;
        }
        else
        {
            win_ratio = 0.0f;
        }

        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%.1f", win_ratio);
        // 2583 승률: %s%
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2583, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(battle_ground_record.kill_count, temp_string, 10);
        // 2592 킬: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2592, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(battle_ground_record.death_count, temp_string, 10);
        // 2593 데스: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2593, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        _itot(battle_ground_record.assist_count, temp_string, 10);
        // 2666 어시스트: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2666, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

        float kill_ratio = 0.0f;
        if (battle_ground_record.death_count != 0)
        {
            kill_ratio = battle_ground_record.kill_count/static_cast<float>(battle_ground_record.death_count);
        }
        else
        {
            kill_ratio = battle_ground_record.kill_count;
        }
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%.2f", kill_ratio);
        // 2594 킬/데스: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2594, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        // 빈줄        
        ftext_render_current_.Append(temp_sentence);
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

        static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S005)));
        if (static_control)
        {
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(history_info->title_string_code,
                title_string, INTERFACE_STRING_LENGTH);

            // 2591 %s 전적
            g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2591, title_string);
            static_control->SetTextWZ(string);
        }

        int total_count = 0;
        int total_win_count = 0;
        for (int i = 0; i < CHAOS_ZONE_RECORD_INFOS::kRecordMaxCount; ++i)
        {
            total_count += ChaosSystem::GetChaosZoneData().chaos_zone_record().battle_ground_record[i].total_count;
            total_win_count += ChaosSystem::GetChaosZoneData().chaos_zone_record().battle_ground_record[i].win_count;
        }

        _itot(total_count, temp_string, 10);
        // 2581 참여 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2581, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_total_.Append(sentence);
        }

        _itot(total_win_count, temp_string, 10);
        // 2582 승리 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2582, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_total_.Append(sentence);
        }

        win_ratio = 0.0f;
        if (total_count != 0)
        {
            win_ratio = (total_win_count/static_cast<float>(total_count)) * 100.0f;
        }
        else
        {
            win_ratio = 0.0f;
        }
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%.1f", win_ratio);
        // 2583 승률: %s%
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2583, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_total_.Append(sentence);
        }
    }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    else if (history_info->menu_depth[0] == 3)
    {
        // 골드러시 전적
        GoldRushRecord goldrush_record;
        for (int i = 0; i < CHAOS_ZONE_RECORD_INFOS::kRecordMaxCount; ++i)
        {
            if (history_info->map_code ==
                ChaosSystem::GetChaosZoneData().chaos_zone_record().goldrush_record[i].GetMapCode())
            {
                goldrush_record = ChaosSystem::GetChaosZoneData().chaos_zone_record().goldrush_record[i];
            }
        }

        CCtrlStaticWZ* static_control = NULL;
        static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S001)));
        if (static_control)
        {
            g_InterfaceManager.GetInterfaceString(history_info->subtitle_string_code,
                temp_string, INTERFACE_STRING_LENGTH);

            // 2591 %s 전적
            g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2591, temp_string);
            static_control->SetTextWZ(str_format);
        }

        _itot(goldrush_record.total_count, temp_string, 10);
        // 2581 참여 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2581, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(goldrush_record.win_count, temp_string, 10);
        // 2582 승리 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2582, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(goldrush_record.draw_count, temp_string, 10);
        // 2634 무승부 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2634, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(goldrush_record.lose_count, temp_string, 10);
        // 2635 패배 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2635, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        float win_ratio = 0.0f;
        if (goldrush_record.total_count != 0)
        {
            win_ratio = (goldrush_record.win_count/static_cast<float>(goldrush_record.total_count)) * 100.0f;
        }
        else
        {
            win_ratio = 0.0f;
        }

        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%.1f", win_ratio);
        // 2583 승률: %s%
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2583, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(goldrush_record.kill_count, temp_string, 10);
        // 2592 킬: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2592, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }

        _itot(goldrush_record.death_count, temp_string, 10);
        // 2593 데스: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2593, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        _itot(goldrush_record.assist_count, temp_string, 10);
        // 2666 어시스트: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2666, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

        float kill_ratio = 0.0f;
        if (goldrush_record.death_count != 0)
        {
            kill_ratio = goldrush_record.kill_count/static_cast<float>(goldrush_record.death_count);
        }
        else
        {
            kill_ratio = goldrush_record.kill_count;
        }
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%.2f", kill_ratio);
        // 2594 킬/데스: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2594, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_current_.Append(sentence);
        }
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        // 빈줄        
        ftext_render_current_.Append(temp_sentence);
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

        static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S005)));
        if (static_control)
        {
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(history_info->title_string_code,
                title_string, INTERFACE_STRING_LENGTH);

            // 2591 %s 전적
            g_InterfaceManager.GetInterfaceStringFormat(string, INTERFACE_STRING_LENGTH, 2591, title_string);
            static_control->SetTextWZ(string);
        }

        int total_count = 0;
        int total_win_count = 0;
        for (int i = 0; i < CHAOS_ZONE_RECORD_INFOS::kRecordMaxCount; ++i)
        {
            total_count += ChaosSystem::GetChaosZoneData().chaos_zone_record().goldrush_record[i].total_count;
            total_win_count += ChaosSystem::GetChaosZoneData().chaos_zone_record().goldrush_record[i].win_count;
        }

        _itot(total_count, temp_string, 10);
        // 2581 참여 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2581, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_total_.Append(sentence);
        }

        _itot(total_win_count, temp_string, 10);
        // 2582 승리 횟수: %s
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2582, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_total_.Append(sentence);
        }

        win_ratio = 0.0f;
        if (total_count != 0)
        {
            win_ratio = (total_win_count/static_cast<float>(total_count)) * 100.0f;
        }
        else
        {
            win_ratio = 0.0f;
        }
        Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%.1f", win_ratio);
        // 2583 승률: %s%
        g_InterfaceManager.GetInterfaceStringFormat(str_format, INTERFACE_STRING_LENGTH, 2583, temp_string);
        result = parser.Parser(str_format, sentence);
        if (result == TRUE)
        {
            ftext_render_total_.Append(sentence);
        }
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    ftext_render_current_.convertLineInfo();
    int scroll_range = ftext_render_current_.GetMaxLine() - ftext_render_current_.GetDrawLineInRect();
    scroll_control_current_->SetScrollRangeWZ(0,scroll_range);
    scroll_control_current_->SetScrollPosWZ(0);

    ftext_render_total_.convertLineInfo();
    scroll_range = ftext_render_total_.GetMaxLine() - ftext_render_total_.GetDrawLineInRect();
    scroll_control_total_->SetScrollRangeWZ(0,scroll_range);
    scroll_control_total_->SetScrollPosWZ(0);
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
