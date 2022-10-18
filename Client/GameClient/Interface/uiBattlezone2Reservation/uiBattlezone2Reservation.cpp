#include "SunClientPrecompiledHeader.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Tree/uiBattlezone2Tree.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "uiBattlezone2Reservation/uiBattlezone2Reservation.h"

#include "BattlezoneMissionTreeParser.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "Mouse.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiToolTipMan/uiToolTipMan.h"

#ifdef _YMS_TREECONTROL
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#endif//_YMS_TREECONTROL
#include "QuestManager_Concrete.h"
#include "MissionManager_Concrete.h"

const int kDefaultRoomNameStartIndex_Mission = 1941;
const int kDefaultRoomNameStartIndex_PVP = 1981;
const int kDefaultRoomNameNumMax = 19;
//------------------------------------------------------------------------------
void uiBattlezone2Reservation::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
}

uiBattlezone2Reservation::uiBattlezone2Reservation(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
    ui_battlezone2_manager_ = NULL;
}

uiBattlezone2Reservation::~uiBattlezone2Reservation(void)
{
}

void uiBattlezone2Reservation::Init(CDrawBase* draw_base)
{
    RegisterControl();

    //! tree menu
    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_BattlezoneTreeReservation, 
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000)), 
        GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001)), 
        true, 
        NULL);
    tree_menu().set_menu_height_gab(20);
    tree_menu().set_simple_menu(false);

    SetMode(kReservationMode);
    is_mouse_over_ = false;

    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        selected_rooms_[i].menu_index = 0;
        checked_rooms_[i].menu_index = 0;
    }
    selected_count_ = 0;
    old_selected_count_ = 0;
    created_room_.menu_index = 0;
    is_stop_ = false;
    is_reservation_ = false;

    set_save_party_key(0);
    ShowButton(kControl_Button_B000, false);
    ShowButton(kControl_Button_B003, true);
}

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
void uiBattlezone2Reservation::MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string, const DWORD* font_colors)
#else
void uiBattlezone2Reservation::MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string)
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
{
    BASE_TreeMenuInfo tree_menu_info;
    ZeroMemory(&tree_menu_info, sizeof(tree_menu_info));

    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Enable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Disable] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Over] = StrToWzID("st10");
    tree_menu_info.font_id[BASE_TreeMenuInfo::kFontType_Select] = StrToWzID("st10");
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    if (depth_1 > 0)
    {
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Enable] = font_colors[BattlezoneMissionTreeInfo::kFontType_Enable];
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = font_colors[BattlezoneMissionTreeInfo::kFontType_Disable];
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Over] = font_colors[BattlezoneMissionTreeInfo::kFontType_Over];
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Select] = font_colors[BattlezoneMissionTreeInfo::kFontType_Select];
    }
    else
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
    {
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Enable] = 0xffffffff;
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = WzColor_RGBA(155,155,155,0);
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Over] = 0xffffffff;
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Select] = 0xffffffff;
    }

    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = 10007;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = 10002;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = 10003;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 0;
    tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Cursor] = 10008;

    //if (TreeMenuInfoParser::GetFirstTreeMenu(OUT tree_menu_info, BASE_TreeMenuInfo::Group_Type::kGroupType_BattlezoneTree) == true)
    {
        tree_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_BattlezoneTreeReservation;
        tree_menu_info.menu_depth[3] = depth_3;
        tree_menu_info.menu_depth[2] = depth_2;
        tree_menu_info.menu_depth[1] = depth_1;
        tree_menu_info.menu_depth[0] = depth_0;
        tree_menu_info.string_code = string_code;
        //tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = 10007;
        //tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = WzColor_RGBA(155,155,155,255);

        if (string_code == 0)
        {
            strncpy(tree_menu_info.string_name, string, sizeof(tree_menu_info.string_name));
        }

        switch (depth)
        {
        case 0:
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
            break;
        case 1:
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
            tree_menu_info.menu_depth_width = 20;
            break;
        case 2:
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
            tree_menu_info.menu_depth_width = 40;
            
            tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 10006;
            tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 10005;
            break;
        default:
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_None;
            break;
        }

        TreeMenuInfoParser::Instance()->Insert(tree_menu_info);

    }
}

void uiBattlezone2Reservation::Clear()
{
    selected_count_ = 0;
    old_selected_count_ = 0;
    set_save_party_key(0);
    is_stop_ = false;
    is_reservation_ = false;

    ShowButton(kControl_Button_B000, false);
    ShowButton(kControl_Button_B003, true);

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (created_room_.menu_index != 0)
    {
        tree_object = tree_menu().Find(created_room_.menu_index);
        if (tree_object)
        {
            tree_object->is_check = false;
            created_room_.menu_index = 0;
        }
    }

    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        if (selected_rooms_[i].menu_index != 0)
        {
            tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
            if (tree_object)
            {
                tree_object->is_check = false;
                selected_rooms_[i].menu_index = 0;
                checked_rooms_[i].menu_index = 0;
            }
        }
    }
}

void uiBattlezone2Reservation::Release()
{
    SelectRoomClear();
    created_room_.menu_index = 0;
    Clear();
}

void uiBattlezone2Reservation::OnUpdateSolarDialog()
{
#ifdef _YMS_TREECONTROL
    tree_menu().UpdateMouseWheel();
#endif//_YMS_TREECONTROL

    if (selected_count_ <= 0)
    {
        SetButton(kControl_Button_B000, false);
        SetButton(kControl_Button_B003, false);
        SetButton(kControl_Button_B002, false);
        SetButton(kControl_Button_B005, false);
    }
    else
    {
        if (is_reservation_)
        {
            /*if (is_stop_)
            {
                SetButton(kControl_Button_B002, false);
            }
            else
            {
                SetButton(kControl_Button_B002, true);  
            }*/
            SetButton(kControl_Button_B000, true);
            SetButton(kControl_Button_B003, true);
            SetButton(kControl_Button_B005, true);
        }
        else
        {
            //SetButton(kControl_Button_B002, true);
            SetButton(kControl_Button_B000, false);
            SetButton(kControl_Button_B003, false);
            SetButton(kControl_Button_B005, false);
        }

        /*if (!is_stop_ && old_selected_count_ != selected_count_)
        {
            SetButton(kControl_Button_B002, true);
        }
        else
        {
            SetButton(kControl_Button_B002, false);
        }*/
    }

    if (GetDialogWZ()->HaveFocus() == true)
    {
        GlobalFunc::MoveToTopDialog(IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    }

    VScrollDialog::Update();
}

bool uiBattlezone2Reservation::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}

void uiBattlezone2Reservation::OnMouseWheelScroll(BYTE mouse_wheel_state)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_VScroll_V001);
    if (control_ptr)
    {
        if (control_ptr->GetState() == WZ_HIDE)
        {
            return;
        }
    }
    else
    {
        return;
    }

    if (mouse_wheel_state == MOUSE_WHEEL_UP)
    {
        OnVScrollUp(NULL);
    }
    else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
    {
        OnVScrollDown(NULL);
    }
}

//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiBattlezone2Reservation::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
}

bool uiBattlezone2Reservation::OnVScrollUp(SI_MESSAGE* message)
{
    bool result = true;

    if (VScrollDialog::OnVScrollThumb(message) == true)
    {
        WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll_V001);
        CCtrlVScrollWZ* scroll_control = 
            static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
        if (scroll_control != NULL)
        {
            int scroll_min = 0;
            int scroll_max = 0;
            int current_scroll_pos = scroll_control->GetScrollPosWZ();
            scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
            --current_scroll_pos;
            current_scroll_pos = max(0, current_scroll_pos);
            scroll_control->SetScrollPosWZ(current_scroll_pos);
            GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(scroll_control->GetScrollPosWZ());
        }
    }

    return result;
}


bool uiBattlezone2Reservation::OnVScrollDown(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll_V001);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        ++current_scroll_pos;
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(scroll_control->GetScrollPosWZ());
    }

    return result;
}

bool uiBattlezone2Reservation::OnVScrollThumb(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll_V001);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        current_scroll_pos = max(0, current_scroll_pos);
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(scroll_control->GetScrollPosWZ());
    }
    return result;
}

void uiBattlezone2Reservation::OnRenderSolarDialog()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (is_mouse_over_ && tooltip_manager != NULL)
    {
        const BattlezoneMissionTreeInfo* find_info;
        TreeMenuGroup::TreeObject* tree_object = tree_menu().OverTreeObject();

        if (tree_object && tree_object->info)
        {
            find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(tree_object->info->menu_depth[3], 
                tree_object->info->menu_depth[2], tree_object->info->menu_depth[1]);

            if (find_info)
            {
                tooltip_manager->RegisterTooltipBattlezoneMap(find_info);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());

            }
        }
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    CControlWZ* control = GetControlWZ_byPosition(kControl_List_L000);
    if (control)
    {
        RECT position = control->GetSizeRect();
        if (position.left > mouse_x || position.right < mouse_x ||
            position.top > mouse_y || position.bottom < mouse_y)
        {
            is_mouse_over_ = false;
        }
    }
}

void uiBattlezone2Reservation::MouseOver_MsgProc(SI_MESSAGE* message)
{

}

void uiBattlezone2Reservation::SetDefaultRoomName(int select/* = -1 */)
{
}

uiBattlezone2Man* uiBattlezone2Reservation::GetManager()
{
    if (ui_battlezone2_manager_ == NULL)
    {
        ui_battlezone2_manager_ = static_cast<uiBattlezone2Man*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
        assert (ui_battlezone2_manager_);
    }
    return ui_battlezone2_manager_;
}


void uiBattlezone2Reservation::OnShowWindow(BOOL is_show)
{
    static bool once_make_list = false;

	TCHAR string_buffer[BASE_TreeMenuInfo::kNameLength] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

    if (is_show)
    {
        g_InterfaceManager.GetInterfaceString(1942, temp_string);

        BYTE* menu_depth_count = BattlezoneMissionTreeParser::Instance()->max_menu_depth();
        const BattlezoneMissionTreeInfo* find_info;

        //트리 리스트 생성하기
        if (once_make_list == false)
        {
            once_make_list = true;

            TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_BattlezoneTreeReservation);

            for (BYTE LGroupCount = 1; LGroupCount <= menu_depth_count[0]; ++LGroupCount)
            {
                for (BYTE SGroupCount = 1; SGroupCount <= menu_depth_count[1]; ++SGroupCount)
                {
                    for (BYTE OrderCount = 1; OrderCount <= menu_depth_count[2]; ++OrderCount)
                    {
                        find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(LGroupCount, SGroupCount, OrderCount);
                        if (find_info != NULL)
                        {
                            if (SGroupCount == 1 && OrderCount == 1)
                            {
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
                                MakeBattlezoneTree(LGroupCount, 0, 0, 0, 0, find_info->title_string_code, NULL, find_info->font_color);
#else
                                MakeBattlezoneTree(LGroupCount, 0, 0, 0, 0, find_info->title_string_code, NULL);
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
                            }
                            if (OrderCount == 1)
                            {
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
                                MakeBattlezoneTree(LGroupCount, SGroupCount, 0, 0, 1, find_info->subtitle_string_code, NULL, find_info->font_color);
#else
                                MakeBattlezoneTree(LGroupCount, SGroupCount, 0, 0, 1, find_info->subtitle_string_code, NULL);
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
                            }

                            g_InterfaceManager.GetInterfaceString(find_info->mission_string_code, temp_string);
                            //Snprintf(string_buffer, sizeof(string_buffer), "%s(%d~%d)", temp_string, find_info->min_level, find_info->max_level);
                            Snprintf(string_buffer, sizeof(string_buffer), temp_string);
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
                            MakeBattlezoneTree(LGroupCount, SGroupCount, OrderCount, 0, 2, 0, string_buffer, find_info->font_color);
#else
                            MakeBattlezoneTree(LGroupCount, SGroupCount, OrderCount, 0, 2, 0, string_buffer);
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            tree_menu().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_BattlezoneTreeReservation);
            tree_menu().Fill();
        }

        //레벨업에 따른 락제거
        TreeMenuGroup::TreeObject* tree_object = NULL;
        for (BYTE LGroupCount = 1; LGroupCount <= menu_depth_count[0]; ++LGroupCount)
        {
            for (BYTE SGroupCount = 1; SGroupCount <= menu_depth_count[1]; ++SGroupCount)
            {
                for (BYTE OrderCount = 1; OrderCount <= menu_depth_count[2]; ++OrderCount)
                {
                    find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(LGroupCount, SGroupCount, OrderCount);
                    if (find_info != NULL)
                    {
                        tree_object = tree_menu().Find(LGroupCount,SGroupCount,OrderCount,0);
                        if (tree_object == NULL)
                        {
                            continue;
                        }
                        
                        bool is_enable = false;
                        sMAPINFO* map_info =
                            MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(find_info->map_code));
                        if ((map_info == NULL) || (g_pQMGR_Real == NULL))
                        {
                            tree_object->is_enable = false;
                            continue;
                        }

                        // 레벨 조건이 참인지
                        if (g_pHero->GetLevel() >= map_info->freePassLV)
                        {
                            is_enable = true;
                        }

                        // 미션을 완료했었는지
                        if (g_pMission_Concrete && (is_enable == false))
                        {
                            is_enable =
                                !!g_pMission_Concrete->GetMissionPoint(static_cast<BYTE>(map_info->completeMCode));
                        }

                        // 선행 퀘스트를 완료했는지
                        if (g_pQMGR_Real && (is_enable == false))
                        {
                            QUESTPART_COMPL* questpart_complete =
                                g_pQMGR_Real->FindComplQuest(map_info->completeQCode);
                            if (questpart_complete && questpart_complete->m_Num > 0) 
                            {
                                is_enable = true;
                            }
                        }

                        tree_object->is_enable = is_enable;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        /*
        AllCheckOff();
        if (battlezone_tree_mode_ == kReservationMode)
        {
            SetReservationCheck();
        }
        else
        {
            SetCreationCheck();
            SetDefaultRoomName();
        }*/  
    }
    else
    {
        is_mouse_over_ = false;

        if (CheckingSelect())//예약상황이 다르다면 복구
        {
            RestoreReservation();
        }
    }
}

void uiBattlezone2Reservation::RestoreReservation()
{
    TreeMenuGroup::TreeObject* tree_object = NULL;
    
    for (int i = 0; i < selected_count_; ++i)
    {
        tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
        if (tree_object)
        {
            tree_object->is_check = false;//일단 모두를 끈다.
        }
        selected_rooms_[i].menu_index = 0;
    }
    for (int i = 0; i < old_selected_count_; ++i)
    {
        tree_object = tree_menu().Find(checked_rooms_[i].menu_index);
        selected_rooms_[i].menu_index = checked_rooms_[i].menu_index;
        if (tree_object)
        {
            tree_object->is_check = true;//복구를한다.
        }
    }

    selected_count_ = old_selected_count_;
}

void uiBattlezone2Reservation::SetReservationCheck()
{
    if (created_room_.menu_index == 0)
    {
        return;
    }
    TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(created_room_.menu_index);
    if (tree_object)
    {
        tree_object->is_check = true;
    }
}

void uiBattlezone2Reservation::SetCreationCheck()
{
    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        if (selected_rooms_[i].menu_index == 0)
        {
            continue;
        }
        TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
        if (tree_object)
        {
            tree_object->is_check = true;
        }
    }
}

void uiBattlezone2Reservation::AllCheckOff()
{
    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
        if (tree_object)
        {
            tree_object->is_check = false;
        }
    }
}

void uiBattlezone2Reservation::SetMode(BYTE mode)
{
    if (mode < kCreateMode || mode >= kModeMax)
    {
        return;
    }

    battlezone_tree_mode_ = mode;

    /*CControlWZ* control_ptr = NULL;
    control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Button_B002);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Button_B003);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    switch (mode)
    {
    case kCreateMode:
        {
            SelectRoomClear();
            control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
        }        
        break;
    case kReservationMode:
        {
            TreeMenuGroup::TreeObject* find_tree_object = tree_menu().Find(created_room_.menu_index);
            //이전에 선택했던 것은 체크 끄기
            if (find_tree_object)
            {
                find_tree_object->is_check = false;
            }

            control_ptr = GetControlWZ_byPosition(kControl_Button_B002);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
        }
        break;
    case kModificationMode:
        {
            SelectRoomClear();
            control_ptr = GetControlWZ_byPosition(kControl_Button_B003);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
        }
        break;
    }*/
}

bool uiBattlezone2Reservation::ShowEditControl(BYTE type, bool is_show)
{
    return true;
}

void uiBattlezone2Reservation::Process(DWORD tick_count)
{

}
//------------------------------------------------------------------------------
void uiBattlezone2Reservation::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    /*if (GetManager() == NULL)
        return;*/

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_MOUSEOVER:
        {
            is_mouse_over_ = true;
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}

bool uiBattlezone2Reservation::ReservationStandByRoom()
{
    MSG_CG_PARTY_STANDBY_ROOM_SYN send_packet;
    send_packet.number_of_mission_ = selected_count_;

    for (int i = 0; i < selected_count_; ++i)
    {
        const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(selected_rooms_[i].menu_depth[3],
            selected_rooms_[i].menu_depth[2], selected_rooms_[i].menu_depth[1]);//little 엔디안 1번째는 3번째임
        if (find_info)
        {
            send_packet.map_code_[i] = static_cast<MAPCODE>(find_info->map_code);
        }
        else
        {
            send_packet.map_code_[i] = 0;
#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "uiBattlezone2Reservation::ReservationRoom(Can not found mapcode)");
#endif//
            //return false;
        }
    }
    //send_packet.check_type_ = MSG_CG_PARTY_CHECK_ROOM_SYN::CheckStanByType;

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    return true;
}

bool uiBattlezone2Reservation::ReservationRoom()
{
    if (is_reservation_ == false)
    {
        return false;
    }

    MSG_CG_PARTY_CHECK_ROOM_SYN send_packet;
    send_packet.select_mission_num_ = selected_count_;

    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        checked_rooms_[i].menu_index = selected_rooms_[i].menu_index;
    }
    SetButton(kControl_Button_B002, false);

    for (int i = 0; i < selected_count_; ++i)
    {
        const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(selected_rooms_[i].menu_depth[3],
            selected_rooms_[i].menu_depth[2], selected_rooms_[i].menu_depth[1]);//little 엔디안 1번째는 3번째임
        if (find_info)
        {
            send_packet.map_code_[i] = static_cast<MAPCODE>(find_info->map_code);
        }
        else
        {
            send_packet.map_code_[i] = 0;
#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "uiBattlezone2Tree::ReservationRoom(Can not found mapcode)");
#endif//
            //return false;
        }
    }
    send_packet.check_type_ = MSG_CG_PARTY_CHECK_ROOM_SYN::CheckStanByType;

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    return true;
}

bool uiBattlezone2Reservation::ResetReservationRoom()
{
    /* MSG_CG_PARTY_CHECK_ROOM_SYN send_packet;
    send_packet.select_mission_num_ = 0;
    */
    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        TreeMenuGroup::TreeObject* find_tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
        if (find_tree_object)
        {
            find_tree_object->is_check = false;
            selected_rooms_[i].menu_index = 0;
            checked_rooms_[i].menu_index = 0;
        }
    }
    selected_count_ = 0;
    old_selected_count_ = 0;
    is_stop_ = false;

    MSG_CG_PARTY_STANDBY_ROOM_SYN send_packet;
    send_packet.number_of_mission_ = 0;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, send_packet.GetSize());
    return true;
}

bool uiBattlezone2Reservation::ClearReservationRoom(bool value)
{
    /*MSG_CG_PARTY_CHECK_ROOM_SYN send_packet;
    send_packet.select_mission_num_ = 0;*/
    MSG_CG_PARTY_STANDBY_PAUSE_SYN send_packet;
    send_packet.set_value_ = value;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    return true;
}

bool uiBattlezone2Reservation::CreateRoom()
{
    return true;
}

bool uiBattlezone2Reservation::SelectCreate(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object == NULL)
    {
        return false;
    }
    if (tree_object->info && tree_object->info->menu_depth[1] <= 0)//little 엔디안 1번째는 3번째임
    {
        return false;//항목이 아니다. 폴더이다.
    }
    if (tree_object->is_enable == false)
    {
        return false;//비활성 항목 이다.
    }

    TreeMenuGroup::TreeObject* find_tree_object = tree_menu().Find(created_room_.menu_index);
    //이전에 선택했던 것은 체크 끄기
    if (find_tree_object)
    {
        find_tree_object->is_check = false;
    }
    //선택한것은 항상 체크
    created_room_.menu_index = tree_object->info->menu_index;
    if (tree_object)
    {
        tree_object->is_check = true;
    }
    return true;
}

void uiBattlezone2Reservation::SelectRoomClear()
{
    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        if (selected_rooms_[i].menu_index == 0)
        {
            continue;
        }
        TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
        if (tree_object)
        {
            tree_object->is_check = false;
        }
        selected_rooms_[i].menu_index = 0;
        checked_rooms_[i].menu_index = 0;
    }
    selected_count_ = 0;
}

bool uiBattlezone2Reservation::CheckingSelect()
{
    bool is_find = false;
    bool is_different = false;//안틀리다.    
    
    if (selected_count_ != old_selected_count_) //개수가 다르다면 일단 다름
    {
        return true;
    }

    //개수가 같은 경우 있는지 비교
    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        is_find = false;

        if (selected_rooms_[i].menu_index == 0)
        {
            break;
        }

        for (int j = 0; j < TREE_MAX_ROOM; ++j)
        {
            if (checked_rooms_[j].menu_index == 0)
            {
                break;
            }
            if (selected_rooms_[i].menu_index == checked_rooms_[j].menu_index)
            {
                is_find = true;
                break;
            }
        }

        if (is_find == false)
        {
            is_different = true;
            break;
        }
    }
    return is_different;
}

bool uiBattlezone2Reservation::SelectMenu(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object == NULL)
    {
        return false;
    }
    if (tree_object->info->menu_depth[1] <= 0)//little 엔디안 1번째는 3번째임
    {
        return false;//항목이 아니다. 폴더이다.
    }
    if (tree_object->is_enable == false)
    {
        return false;//비활성 항목 이다.
    }
    if (is_stop_)//중단중에는 사용 불가
    {
        if (!tree_object->is_check)//체크된 항목이면 해제
        {
            tree_object->is_check = true;
        }
        else//체크 안된 항목이면 선택
        {
            tree_object->is_check = false;
        }
        return false;
    }

    if (!tree_object->is_check)//체크된 항목이면 해제
    {
        for (int i = 0; i < selected_count_; ++i)//찾기
        {
            if (selected_rooms_[i].menu_index == tree_object->info->menu_index)
            {
                //selected_count_번째 데이터를 i번째에 덮어쓰고 selected_count_번째는 제거
                selected_rooms_[i].menu_index = selected_rooms_[selected_count_-1].menu_index;
                selected_rooms_[selected_count_-1].menu_index = 0;
                break;
            }
        }
        --selected_count_;
    }
    else//체크 안된 항목이면 선택
    {
        if (selected_count_ < TREE_MAX_ROOM)
        {
            selected_rooms_[selected_count_].menu_index = tree_object->info->menu_index;
            ++selected_count_;
        }
        else
        {
#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Can't select more mission");
#endif//
            tree_object->is_check = false;
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
void uiBattlezone2Reservation::ShowButton(BYTE position, bool is_show)
{
    CControlWZ* control = GetControlWZ_byPosition(position);
    if (control)
    {
        control->ShowWindowWZ((is_show)?WZ_SHOW:WZ_HIDE);
    }
}
//------------------------------------------------------------------------------
void uiBattlezone2Reservation::SetButton(BYTE position, bool is_enable)
{
    CControlWZ* control = GetControlWZ_byPosition(position);
    if (control)
    {
        control->EnableWindowWZ(is_enable);
    }
}
//------------------------------------------------------------------------------
bool uiBattlezone2Reservation::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;

#ifdef _YMS_TREECONTROL
    //! 트리 메뉴 처리
    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true && tree_object != NULL)
    {
        if (battlezone_tree_mode_ == kReservationMode)
        {
            SelectMenu(tree_object);

            if (CheckingSelect())
            {
                SetButton(kControl_Button_B002, true);
            }
            else
            {
                SetButton(kControl_Button_B002, false);
            }
        }
        else//생성, 변경 모두
        {
            SelectCreate(tree_object);
        }
    }
#endif//_YMS_TREECONTROL

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004: //닫기
    case kControl_Button_B001:
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B000://일시정지해제
        {
            is_stop_ = false;
            //ReservationRoom();
            ClearReservationRoom(true);
            ShowButton(kControl_Button_B000, false);
            ShowButton(kControl_Button_B003, true);
        }
        break;
    case kControl_Button_B003://예약일시정지
        {
            is_stop_ = true;
            ClearReservationRoom(false);
            ShowButton(kControl_Button_B000, true);
            ShowButton(kControl_Button_B003, false);
            //SetButton(kControl_Button_B002, false);
        }
        break;
    case kControl_Button_B002://예약
        {
            is_reservation_ = true;
            old_selected_count_ = selected_count_;
            ReservationRoom();
        }
        break;
    case kControl_Button_B005: //예약전체취소
        {
            is_reservation_ = false;
            ResetReservationRoom();
            ShowButton(kControl_Button_B000, false);
            ShowButton(kControl_Button_B003, true);
            //SetButton(kControl_Button_B002, false);
            //SetButton(kControl_Button_B000, false);
            //SetButton(kControl_Button_B004, false);
        }
        break;
    case kControl_List_L000:
        {
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiBattlezone2Reservation::NetworkProc(MSG_BASE* message_ptr)
{
    //배틀존 패킷만 처리한다
    if (message_ptr && message_ptr->m_byCategory != CG_PARTY)
    {
        return;
    }

    switch (message_ptr->m_byProtocol)
    {
    case CG_PARTY_STANDBY_PAUSE_ACK:
        {
            MSG_CG_PARTY_STANDBY_PAUSE_ACK* recv_packet = (MSG_CG_PARTY_STANDBY_PAUSE_ACK*)message_ptr;
            //nop
        }
    }
}
#endif //_DH_BATTLEZONE2_