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

#ifdef _YMS_TREECONTROL
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#endif//_YMS_TREECONTROL

#include "ItemInfoParser.h"
#include "ItemManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "QuestManager_Concrete.h"
#include "MissionManager_Concrete.h"
#include "./TreeMenuGroup.h"

const int kDefaultRoomNameStartIndex_Mission = 1941;
const int kDefaultRoomNameStartIndex_PVP = 1981;
const int kDefaultRoomNameNumMax = 19;
//------------------------------------------------------------------------------
void uiBattlezone2Tree::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);//생성
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);//입장예약(닫기)
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);//예약
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);//머징(변경)
    RegisterControlPosition(StrToWzID("TI00"), kControl_Text_TI00);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);//이름(static)
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);//비번(static)
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);//이름
    RegisterControlPosition(StrToWzID("E002"), kControl_Edit_E002);//비번
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("V001"), kControl_Scroll_V001);
}

uiBattlezone2Tree::uiBattlezone2Tree(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
    ui_battlezone2_manager_ = NULL;

    for (int i = kTitleEditControl; i < kEditControlTypeMax; ++i)
    {
        edit_controls_[i] = NULL;
        static_edit_controls_[i] = NULL;
    }
}

uiBattlezone2Tree::~uiBattlezone2Tree(void)
{
}

void uiBattlezone2Tree::Init(CDrawBase* draw_base)
{
    RegisterControl();

    //! tree menu
    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_BattlezoneTree, 
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000)), 
        GetVScrollControlWZ(GetControlID_byPosition(kControl_Scroll_V001)), 
        true,
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        uiBattlezone2Tree::Callback_TreeRender
#else
        NULL
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        );
    tree_menu().set_menu_height_gab(20);
    tree_menu().set_simple_menu(false);

    SetMode(kCreateMode);

    is_creating_ = false;
    is_mouse_over_ = false;
    selected_create_room_.menu_index = 0;
    set_last_create_room_ = 0;

/*    for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        selected_rooms_[i].menu_index = 0;
    }*/
    selected_count_ = 0;
    created_room_.menu_depth[3] = 1;//menu_index = 0;
    created_room_.menu_depth[2] = 0;//
    created_room_.menu_depth[1] = 0;//
    created_room_.menu_depth[0] = 0;//

    edit_controls_[kTitleEditControl] = static_cast<CCtrlDxEditBox*>(GetControlWZ_byPosition(kControl_Edit_E001));//검색
    if (edit_controls_[kTitleEditControl])
    {
        edit_controls_[kTitleEditControl]->ShowWindowWZ(WZ_HIDE);
        //edit_controls_[kTitleEditControl]->SetLimitText(MAX_ROOMTITLE_LENGTH-1);//20자 제한
    }
    assert(edit_controls_[kTitleEditControl]);
    edit_controls_[kPasswordEditControl] = static_cast<CCtrlDxEditBox*>(GetControlWZ_byPosition(kControl_Edit_E002));//비번
    if (edit_controls_[kPasswordEditControl])
    {
        edit_controls_[kPasswordEditControl]->ShowWindowWZ(WZ_HIDE);
        edit_controls_[kPasswordEditControl]->SetNumberic(true);
    }
    assert(edit_controls_[kPasswordEditControl]);

    static_edit_controls_[kTitleEditControl] = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (static_edit_controls_[kTitleEditControl])
    {
        static_edit_controls_[kTitleEditControl]->ShowWindowWZ(WZ_HIDE);
    }
    assert(static_edit_controls_[kTitleEditControl]);

    static_edit_controls_[kPasswordEditControl] = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S002));
    if (static_edit_controls_[kPasswordEditControl])
    {
        static_edit_controls_[kPasswordEditControl]->ShowWindowWZ(WZ_HIDE);
    }
    assert(static_edit_controls_[kPasswordEditControl]);

    set_save_party_key(0);
}

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
void uiBattlezone2Tree::MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string, const DWORD* font_colors)
#else
void uiBattlezone2Tree::MakeBattlezoneTree(BYTE depth_3, BYTE depth_2, BYTE depth_1, BYTE depth_0, int depth, DWORD string_code, TCHAR* string)
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
        tree_menu_info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = WzColor_RGBA(155,155,155,255);
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
        tree_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_BattlezoneTree;
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

        if (depth_3 == 1)//미션없음 예외사항
        {
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
            tree_menu_info.menu_depth_width = 0;
            tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 10001;
            tree_menu_info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 10000;
        }
        TreeMenuInfoParser::Instance()->Insert(tree_menu_info);
    }
}

void uiBattlezone2Tree::Clear()
{
    is_creating_ = false;
    selected_count_ = 0;
    set_save_party_key(0);
    selected_create_room_.menu_index = 0;

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

    tree_object = tree_menu().Find(created_room_.menu_index);
    if (tree_object)
    {
        tree_object->is_check = true;
    }
    created_room_.menu_depth[3] = 1;//menu_index = 0;
    created_room_.menu_depth[2] = 0;//
    created_room_.menu_depth[1] = 0;//
    created_room_.menu_depth[0] = 0;//

    set_last_create_room_ = 0;

    /*for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        if (selected_rooms_[i].menu_index != 0)
        {
            tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
            if (tree_object)
            {
                tree_object->is_check = false;
                selected_rooms_[i].menu_index = 0;
            }
        }
    }*/
}

void uiBattlezone2Tree::Release()
{
    SelectRoomClear();
    created_room_.menu_index = 0;
}

void uiBattlezone2Tree::OnUpdateSolarDialog()
{
#ifdef _YMS_TREECONTROL
    tree_menu().UpdateMouseWheel();
#endif//_YMS_TREECONTROL

    CControlWZ* control = NULL;
    CControlWZ* control_modify = NULL;
    control = GetControlWZ(GetControlID_byPosition(kControl_Button_B000));
    control_modify = GetControlWZ(GetControlID_byPosition(kControl_Button_B003));

    if (control && control_modify)
    {
        if (set_last_create_room_ != created_room_.menu_index)//인덱스가 바뀐경우만 체크 1회 업데이트 마다 안함
        {
            set_last_create_room_ = created_room_.menu_index;
            if (created_room_.menu_index == 0)
            {
                control->EnableWindowWZ(false);
                control_modify->EnableWindowWZ(false);
            }
            else
            {
                control->EnableWindowWZ(true);
                control_modify->EnableWindowWZ(true);
            }

            const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(created_room_.menu_depth[3],
                created_room_.menu_depth[2], created_room_.menu_depth[1]);//little 엔디안 1번째는 3번째임

            bool is_have = false;

            TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(created_room_.menu_index);
            if (find_info->require_item[0] != 0 && tree_object && tree_object->is_enable)
            {
                for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
                {
                    if (find_info && find_info->require_item[i])// (필요 아이템 있는 상태에서 확인버튼 켜고 아이템이 없어졌다면, 서버 오류메시지 이용함)
                    {
                        POSTYPE inventory_position = INVALID_POSTYPE_VALUE;
                        BOOL result = ItemManager::Instance()->FindFirstItem(find_info->require_item[i], inventory_position);

                        if (result && inventory_position != INVALID_POSTYPE_VALUE)//있음
                        {
                            is_have = true;
                            break;
                        }
                    }
                }
            }

            if (find_info->require_item[0] == 0 && tree_object && tree_object->is_enable)//첫번째 것이 0이라면 true
            {
                is_have = true;
            }

            if (is_have == false)
            {
                control->EnableWindowWZ(false);
                control_modify->EnableWindowWZ(false);
            }
        }
    }

    SetDefaultSpecialType();

    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);

    DWORD focus_dialog = 0;
    if (battlezone2_list && battlezone2_list->IsVisible())
    {
        focus_dialog = IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST;
    }
    else if (battlezone2_lobby && battlezone2_lobby->IsVisible())
    {
        focus_dialog = IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY;
    }
    else
    {
        focus_dialog = 0;
    }

    if (GetDialogWZ()->HaveFocus() == true && focus_dialog)
    {
        GlobalFunc::MoveToTopDialog(focus_dialog);
    }

    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    if (edit_controls_[kTitleEditControl])
    {
        edit_controls_[kTitleEditControl]->GetTextWZ(szMessage, INTERFACE_STRING_LENGTH-1);
    }
    
    VScrollDialog::Update();
}

bool uiBattlezone2Tree::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}

void uiBattlezone2Tree::OnMouseWheelScroll(BYTE mouse_wheel_state)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Scroll_V001);
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
CCtrlVScrollWZ* uiBattlezone2Tree::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_Scroll_V001));
}

bool uiBattlezone2Tree::OnVScrollUp(SI_MESSAGE* message)
{
    bool result = true;

    if (VScrollDialog::OnVScrollThumb(message) == true)
    {
        WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V001);
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

bool uiBattlezone2Tree::OnVScrollDown(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V001);
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

bool uiBattlezone2Tree::OnVScrollThumb(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V001);
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

void uiBattlezone2Tree::OnRenderSolarDialog()
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

void uiBattlezone2Tree::MouseOver_MsgProc(SI_MESSAGE* message)
{
}

void uiBattlezone2Tree::SetDefaultRoomName(int select/* = -1 */)
{
    int start_string_code = 0;
    int room_type = eCREATE_BATTLE_MISSION_TYPE;

    /*TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(created_room_.menu_index);
    if (tree_object)
    {
        room_type = tree_object->info->
    }*/

    switch (room_type)
    {
    case eCREATE_BATTLE_MISSION_TYPE: 
        {
            start_string_code = kDefaultRoomNameStartIndex_Mission;
        }
        break;
    case eCREATE_BATTLE_PVP_TYPE: 
        { 
            start_string_code = kDefaultRoomNameStartIndex_PVP;
        } 
        break;
    default:
        return;	// 그 외에는 그냥 리턴
    }

    TCHAR room_name[INTERFACE_STRING_LENGTH] = {0,};
    int count = 0;
    for (int i = start_string_code; i< (start_string_code + kDefaultRoomNameNumMax); ++i)
    {
        if (g_InterfaceManager.GetInterfaceStringWithCheck(i, room_name, INTERFACE_STRING_LENGTH))
        {
            ++count;
        }
    }

    if (count && edit_controls_ && static_edit_controls_)
    {
        int index = clock_function::GetTickCount() % count;

        g_InterfaceManager.GetInterfaceString(start_string_code + index, room_name, INTERFACE_STRING_LENGTH);
        edit_controls_[kTitleEditControl]->SetTextWZ(room_name);
        static_edit_controls_[kTitleEditControl]->SetTextWZ(room_name);
    }
}

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
void uiBattlezone2Tree::SetRoomPassword(TCHAR* password)
{
    if (edit_controls_[kPasswordEditControl])
    {
        edit_controls_[kPasswordEditControl]->SetTextWZ(password);
    }
}
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

uiBattlezone2Man* uiBattlezone2Tree::GetManager()
{
    if (ui_battlezone2_manager_ == NULL)
    {
        ui_battlezone2_manager_ = static_cast<uiBattlezone2Man*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
        assert (ui_battlezone2_manager_);
    }
    return ui_battlezone2_manager_;
}


void uiBattlezone2Tree::OnShowWindow(BOOL is_show)
{
    static bool once_make_list = false;

	TCHAR string_buffer[BASE_TreeMenuInfo::kNameLength] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

    if (is_show)
    {
        g_InterfaceManager.GetInterfaceString(1942, temp_string);
        if (static_edit_controls_[kTitleEditControl])
        {
            static_edit_controls_[kTitleEditControl]->SetTextWZ(temp_string);
            ShowEditControl(kTitleEditControl, false);
        }
        if (static_edit_controls_[kPasswordEditControl])
        {
            static_edit_controls_[kPasswordEditControl]->SetTextWZ("");
            ShowEditControl(kPasswordEditControl, false);
        }

        BYTE* menu_depth_count = BattlezoneMissionTreeParser::Instance()->max_menu_depth();
        const BattlezoneMissionTreeInfo* find_info;

        TreeMenuGroup::TreeObject* tree_object = NULL;

        //트리 리스트 생성하기
        if (once_make_list == false)
        {
            once_make_list = true;

            TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_BattlezoneTree);

            for (BYTE LGroupCount = 1; LGroupCount <= menu_depth_count[0]; ++LGroupCount)
            {
                if (LGroupCount == 1)//1, 0, 0 미션없음 1회 예외처리
                {
                    find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(LGroupCount, 0, 0);
                    if (find_info)
                    {
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
                        MakeBattlezoneTree(LGroupCount, 0, 0, 0, 0, find_info->title_string_code, NULL, find_info->font_color);
#else
                        MakeBattlezoneTree(LGroupCount, 0, 0, 0, 0, find_info->title_string_code, NULL);
#endif//_NA_003649_20111115_BATTLE_ZONE_HARDMODE
                    }
                }
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
            tree_menu().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_BattlezoneTree);
            tree_menu().Fill();

            tree_object = tree_menu().Find(1,0,0,0);
            if (tree_object != NULL)//미션없음 최초 선택
            {
                tree_object->is_check = true;
            }
        }

        //레벨업에 따른 락제거
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

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
                        // 모두분배 방인지 체크
                        if (find_info->item_obtain_ == kObtainType_Enable)
                        {
                            tree_object->add_data_flag(TreeMenuGroup::kDataFlag_Notice);
                        }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        //AllCheckOff();
        if (battlezone_tree_mode_ == kReservationMode)
        {
            SetReservationCheck();
        }
        else if (battlezone_tree_mode_ == kCreateMode)
        {
            SetCreationCheck();
            SetDefaultRoomName();
        }
        else
        {
            // set last data
            SetLastData();
        }
    }
    else
    {
        is_mouse_over_ = false;
    }
}

void uiBattlezone2Tree::SetLastData()
{
    PartyRoomInfo* party_room_info = GetManager()->current_room_info();

    if (party_room_info == NULL)
    {
        return;
    }
    //created_room_.menu_index != 0 && selected_create_room_.menu_index != 0 &&// 둘다 0이 아니고
    if (created_room_.menu_index != selected_create_room_.menu_index)//둘이 다를때
    {
        TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(created_room_.menu_index);
        if (tree_object)
        {
            tree_object->is_check = false;
        }
        tree_object = tree_menu().Find(selected_create_room_.menu_index);
        if (tree_object)
        {
            tree_object->is_check = true;
            created_room_.menu_index = selected_create_room_.menu_index;
        }
    }

    if (edit_controls_[kTitleEditControl])
    {
        edit_controls_[kTitleEditControl]->SetTextWZ(_T(party_room_info->base_party_room_info.party_room_title_));
    }
    if (static_edit_controls_[kTitleEditControl])
    {
        static_edit_controls_[kTitleEditControl]->SetTextWZ(_T(party_room_info->base_party_room_info.party_room_title_));
    }

    if (edit_controls_[kPasswordEditControl])
    {
        edit_controls_[kPasswordEditControl]->SetTextWZ(_T(party_room_info->base_party_room_info.party_room_password_));
    }
    if (static_edit_controls_[kPasswordEditControl])
    {
        static_edit_controls_[kPasswordEditControl]->SetTextWZ(_T(party_room_info->base_party_room_info.party_room_password_));
    }
}

void uiBattlezone2Tree::SetReservationCheck()
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

void uiBattlezone2Tree::SetCreationCheck()
{
    /*for (int i = 0; i < TREE_MAX_ROOM; ++i)
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
    }*/
}

void uiBattlezone2Tree::AllCheckOff()
{
    /*for (int i = 0; i < TREE_MAX_ROOM; ++i)
    {
        TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(selected_rooms_[i].menu_index);
        if (tree_object)
        {
            tree_object->is_check = false;
        }
    }*/
}

void uiBattlezone2Tree::SetDefaultSpecialType()
{
    POSTYPE OutPos;
    DWORD string_id = 0;

#ifndef __CHINA_PCBANG_SUPER_ROOM_BLOCK
    if (g_pHero && g_pHero->GetPCBangMode())
    {
        //155303	가맹 PC방
        string_id = 155303;
    }
    else
#endif
    if (ItemManager::Instance()->FindActiveChargeItem(eITEMTYPE_SUPERMASTER, OutPos))
    {
        // 155302	명지휘관의 지휘봉
        string_id = 155302;
    }
    else
    {
        //155301	기본 방
        string_id = 155301;
    }

    CCtrlStaticWZ* control_ptr = NULL;
    control_ptr = (CCtrlStaticWZ*)(GetControlWZ_byPosition(kControl_Text_TI00));
    if (control_ptr)
    {
        control_ptr->SetTextID(string_id);
    }
}

void uiBattlezone2Tree::SetMode(BYTE mode)
{
    if (mode < kCreateMode || mode >= kModeMax)
    {
        return;
    }

    battlezone_tree_mode_ = mode;

    CControlWZ* control_ptr = NULL;
    control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    /*control_ptr = GetControlWZ_byPosition(kControl_Button_B002);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }*/
    control_ptr = GetControlWZ_byPosition(kControl_Button_B003);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    CCtrlStaticWZ* static_control_ptr = NULL;
    switch (mode)
    {
    case kCreateMode:
        {
            SelectRoomClear();
            static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Button_B000));
            if (static_control_ptr)
            {
                static_control_ptr->ShowWindowWZ(WZ_SHOW);
            }
            static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
            //2903 방 만들기
            if (static_control_ptr)
            {
                static_control_ptr->SetTextID(2903);
            }
        }        
        break;
    case kReservationMode:
        {
            /*TreeMenuGroup::TreeObject* find_tree_object = tree_menu().Find(created_room_.menu_index);
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
            
            for (int i = kTitleEditControl; i < kEditControlTypeMax; ++i)
            {
                if (edit_controls_[i])
                {
                    edit_controls_[i]->SetTextWZ(_T(""));
                    static_edit_controls_[i]->SetTextWZ(_T(""));
                    ShowEditControl(i, false);
                }
            }*/
        }
        break;
    case kModificationMode:
        {
            SelectRoomClear();
            static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Button_B003));
            if (static_control_ptr)
            {
                static_control_ptr->ShowWindowWZ(WZ_SHOW);
            }
            static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
            //2910	설정 변경
            if (static_control_ptr)
            {
                static_control_ptr->SetTextID(2910);
            }
        }
        break;
    }
}

bool uiBattlezone2Tree::ShowEditControl(BYTE type, bool is_show)
{
    if (type < 0 || type >= kEditControlTypeMax)
    {
        return false;
    }

    if (edit_controls_[type] == NULL)
    {
        return false;
    }

    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (is_show && battlezone_tree_mode_ != kReservationMode)// 예약모드일때는 방제목, 비밀번호에 포커스가 가지 못하도록 한다.
    {
        static_edit_controls_[type]->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        static_edit_controls_[type]->ShowWindowWZ(WZ_HIDE);
        edit_controls_[type]->ShowWindowWZ(WZ_SHOW);
        edit_controls_[type]->SetFocusWZ();
    }
    else
    {
        edit_controls_[type]->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        edit_controls_[type]->ShowWindowWZ(WZ_HIDE);
        static_edit_controls_[type]->SetTextWZ(string_buffer);
        static_edit_controls_[type]->ShowWindowWZ(WZ_SHOW);
    }
    return true;
}

void uiBattlezone2Tree::Process(DWORD tick_count)
{

}
//------------------------------------------------------------------------------
void uiBattlezone2Tree::MessageProc(SI_MESSAGE* message)
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
    case RT_MSG_LBUTTONDCLICK:
        {
        OnDoubleLButtonClick(message);
        }
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

bool uiBattlezone2Tree::ReservationRoom()//예약창이랑 다름
{
    MSG_CG_PARTY_CHECK_ROOM_SYN send_packet;
    send_packet.select_mission_num_ = 1;

    const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(created_room_.menu_depth[3],
        created_room_.menu_depth[2], created_room_.menu_depth[1]);//little 엔디안 1번째는 3번째임
    if (find_info)
    {
        send_packet.map_code_[0] = static_cast<MAPCODE>(find_info->map_code);
    }
    else
    {
        send_packet.map_code_[0] = 0;
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "uiBattlezone2Tree::ReservationRoom(Can not found mapcode)");
#endif//
        //return false;
    }
    send_packet.check_type_ = MSG_CG_PARTY_CHECK_ROOM_SYN::CheckCreateType;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    return true;
}

bool uiBattlezone2Tree::ModificationRoom()
{
    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    MSG_CG_PARTY_CHANGE_ROOM_OPTION_SYN send_packet;

    const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(created_room_.menu_depth[3],
        created_room_.menu_depth[2], created_room_.menu_depth[1]);//little 엔디안 1번째는 3번째임
    send_packet.change_party_room_info_.map_code_ = 0;
    if (find_info)
    {
        send_packet.change_party_room_info_.map_code_ = static_cast<MAPCODE>(find_info->map_code);
        send_packet.change_party_room_info_.party_room_type_ = static_cast<PartyZoneType>(find_info->kind_of_room);
    }
    else
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Can't select mission for creating");
#endif//
        return false;
    }

    edit_controls_[kTitleEditControl]->GetTextWZ(\
        send_packet.change_party_room_info_.party_room_title_,
        _countof(send_packet.change_party_room_info_.party_room_title_)); 
    send_packet.change_party_room_info_.party_room_title_[\
        _countof(send_packet.change_party_room_info_.party_room_title_) - 1] = _T('\0');

    edit_controls_[kPasswordEditControl]->GetTextWZ(
        send_packet.change_party_room_info_.party_room_password_,
        _countof(send_packet.change_party_room_info_.party_room_password_));
    send_packet.change_party_room_info_.party_room_password_[\
        _countof(send_packet.change_party_room_info_.party_room_password_) - 1] = _T('\0');

    if (strlen(send_packet.change_party_room_info_.party_room_title_) <= 0)
    {
        g_InterfaceManager.GetInterfaceString(eST_INPUT_ROOM_NAME, message, INTERFACE_STRING_LENGTH);
        uiSystemMan* system_mananger = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
        if (system_mananger)
        {
            system_mananger->Sys_VerifyLock(message);
            return false;
        }
    }
    else
    {
        // 방제목의 최대 사이즈(20 byte)를 넘었습니다.
        if(!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)edit_controls_[kTitleEditControl], 
            send_packet.change_party_room_info_.party_room_title_, _countof(send_packet.change_party_room_info_.party_room_title_), 5791))
        {
            return false;
        }

        // 비밀번호의 최대 사이즈(7 byte)를 넘었습니다.
        if(!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)edit_controls_[kPasswordEditControl], 
            send_packet.change_party_room_info_.party_room_password_, _countof(send_packet.change_party_room_info_.party_room_password_), 5792))
        {
            return false;
        }

        // 금지단어 check
        if(GlobalFunc::IsSlang(send_packet.change_party_room_info_.party_room_title_, false) == TRUE)
        {
            g_InterfaceManager.GetInterfaceString(eST_CANNOT_USE_WORD, message, MAX_MESSAGE_LENGTH);
            uiSystemMan* system_mananger = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
            if (system_mananger)
            {
                system_mananger->Sys_VerifyLock(message);
                return false;
            }
        }
    }

    send_packet.require_item_ = static_cast<SLOTCODE>(GetRequireItem(find_info));
    //send_packet.change_party_room_info_.party_room_type_ = eZONETYPE_MISSION;
    //send_packet.change_party_room_info_.special_type_ = eROOM_SPECIAL_NORMAL;

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

    return true;
}

DWORD uiBattlezone2Tree::GetRequireItem(const BattlezoneMissionTreeInfo* find_info)
{
    DWORD item_code = 0;
    for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
    {
        if (find_info && find_info->require_item[i] != 0)
        {
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(find_info->require_item[i]);
            if (item_info)
            {
                //아이템 찾기
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, static_cast<SLOTCODE>(find_info->require_item[i]));
                if (item_slot && item_slot->GetNum() > 0)
                {
                    //nop
                    item_code = find_info->require_item[i];
                    break;
                }
            }
        }
    }
    return item_code;
}

bool uiBattlezone2Tree::CreateRoom()
{
    if (is_creating_ != true)
    {
        return false;
    }
    is_creating_ = false;

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    MSG_CG_PARTY_CREATE_ROOM_SYN send_packet;

    const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(created_room_.menu_depth[3],
        created_room_.menu_depth[2], created_room_.menu_depth[1]);//little 엔디안 1번째는 3번째임

    send_packet.require_item_ = static_cast<SLOTCODE>(GetRequireItem(find_info));
    send_packet.party_room_info_.map_code_ = 0;
    if (find_info)
    {
        send_packet.party_room_info_.map_code_ = static_cast<MAPCODE>(find_info->map_code);
    }
    else
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Can't select mission for creating");
#endif//
        return false;
    }
    
    if (edit_controls_[kTitleEditControl])
    {
        edit_controls_[kTitleEditControl]->GetTextWZ(\
            send_packet.party_room_info_.party_room_title_,
            _countof(send_packet.party_room_info_.party_room_title_)); 
    
        send_packet.party_room_info_.party_room_title_[\
            _countof(send_packet.party_room_info_.party_room_title_) - 1] = _T('\0');
    }

    if (edit_controls_[kPasswordEditControl])
    {
        edit_controls_[kPasswordEditControl]->GetTextWZ(
            send_packet.party_room_info_.party_room_password_,
            _countof(send_packet.party_room_info_.party_room_password_));
        send_packet.party_room_info_.party_room_password_[\
            _countof(send_packet.party_room_info_.party_room_password_) - 1] = _T('\0');
    }

    if (strlen(send_packet.party_room_info_.party_room_title_) <= 0)
    {
        g_InterfaceManager.GetInterfaceString(eST_INPUT_ROOM_NAME, message, INTERFACE_STRING_LENGTH);
        uiSystemMan* system_mananger = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
        if (system_mananger)
        {
            system_mananger->Sys_VerifyLock(message);
            return false;
        }
    }
    else
    {
        // 방제목의 최대 사이즈(30 byte)를 넘었습니다.
        if(!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)edit_controls_[kTitleEditControl], send_packet.party_room_info_.party_room_title_, 
                MAX_ROOMTITLE_LENGTH - 1, 5791))
        {
            return false;
        }

        // 비밀번호의 최대 사이즈(7 byte)를 넘었습니다.
        if(!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)edit_controls_[kPasswordEditControl], send_packet.party_room_info_.party_room_password_, 
            MAX_ROOMPASSWORD_LENGTH, 5792))
        {
            return false;
        }

        // 금지단어 check
        if(GlobalFunc::IsSlang(send_packet.party_room_info_.party_room_title_, false) == TRUE)
        {
            g_InterfaceManager.GetInterfaceString(eST_CANNOT_USE_WORD, message, MAX_MESSAGE_LENGTH);
            uiSystemMan* system_mananger = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
            if (system_mananger)
            {
                system_mananger->Sys_VerifyLock(message);
                return false;
            }
        }
    }

    send_packet.party_room_info_.party_room_type_ = static_cast<PartyZoneType>(find_info->kind_of_room);
    //send_packet.party_room_info_.special_type_ = eROOM_SPECIAL_NORMAL;

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

    return true;
}

bool uiBattlezone2Tree::SelectCreate(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object->info->menu_depth[3] == 1)
    {
        //미션없음 예외사항
    }
    else if (tree_object->info->menu_depth[1] <= 0)//little 엔디안 1번째는 3번째임
    {
        return false;//항목이 아니다. 폴더이다.
    }

    /*if (tree_object->is_enable == false)
    {
        return false;//비활성 항목 이다.
    }*/

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

    if (battlezone_tree_mode_ == kCreateMode)//created_room_.menu_index && 
    {
        selected_create_room_.menu_index = created_room_.menu_index;
    }
    return true;
}

void uiBattlezone2Tree::SelectRoomClear()
{
    /*for (int i = 0; i < TREE_MAX_ROOM; ++i)
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
    }
    selected_count_ = 0;*/
}
bool uiBattlezone2Tree::SelectMenu(TreeMenuGroup::TreeObject* tree_object)
{
    /*if (tree_object->info->menu_depth[1] <= 0)//little 엔디안 1번째는 3번째임
    {
        return false;//항목이 아니다. 폴더이다.
    }
    if (tree_object->is_enable == false)
    {
        return false;//비활성 항목 이다.
    }

    if (!tree_object->is_check)//체크된 항목이면 해제
    {
        for (int i = 0; i < selected_count_; ++i)//찾기
        {
            if (selected_rooms_[i].menu_index == tree_object->info->menu_index)
            {
                //selected_count_번째 데이터를 i번째에 덮어쓰고 selected_count_번째는 제거
                selected_rooms_[i].menu_index = selected_rooms_[selected_count_].menu_index;
                selected_rooms_[selected_count_].menu_index = 0;
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
    }*/
    return true;
}
//------------------------------------------------------------------------------
bool uiBattlezone2Tree::OnDoubleLButtonClick(SI_MESSAGE* message)
{
    bool result = true;

#ifdef _YMS_TREECONTROL
    //! 트리 메뉴 처리
    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true && tree_object != NULL)
    {
        if (tree_object->info->menu_depth[3] == 1)
        {
            //미션없음 예외사항
        }
        else if (tree_object->info->menu_depth[1] <= 0)//little 엔디안 1번째는 3번째임
        {
            return false;//항목이 아니다. 폴더이다.
        }
        else
        {
            const BattlezoneMissionTreeInfo* find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(created_room_.menu_depth[3],
                created_room_.menu_depth[2], created_room_.menu_depth[1]);//little 엔디안 1번째는 3번째임
            bool is_have = false;

            if (find_info == NULL)
            {
                return false;
            }

            TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(created_room_.menu_index);
            if (find_info->require_item[0] != 0 && tree_object && tree_object->is_enable)//첫번째 것이 0이라면 true && lock가 아니라면 체크
            {
	            for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
	            {
	                if (find_info->require_item[i] != 0)// (필요 아이템 있는 상태에서 확인버튼 켜고 아이템이 없어졌다면, 서버 오류메시지 이용함)
	                {
	                    POSTYPE inventory_position = INVALID_POSTYPE_VALUE;
	                    BOOL result = ItemManager::Instance()->FindFirstItem(find_info->require_item[i], inventory_position);
	
	                    if (result && inventory_position != INVALID_POSTYPE_VALUE)//있음
	                    {
	                        is_have = true;
	                        break;
	                    }                    
	                }
                    else//if (find_info->require_item[i] == 0)
                    {
                        break;
                    }
	            }
	            if (is_have == false)
	            {
	                return false;
	            }
            }
        }
    }
#endif//_YMS_TREECONTROL

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_List_L000:
        {
            CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
            if (control_ptr)
            {
                if (control_ptr->GetState() == WZ_SHOW)
                {
                    is_creating_ = true;
                    ReservationRoom();
                }
                else
                {
                    ModificationRoom();
                }
            }
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

bool uiBattlezone2Tree::OnLButtonClick(SI_MESSAGE* message)
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
    case kControl_Button_B000:// 생성
        {
            is_creating_ = true;
            ReservationRoom();
            //CreateRoom();
        }
        break;
    case kControl_Button_B003://변경
        {
            ModificationRoom();
        }
        break;
    case kControl_Button_B002://예약
        {
            ReservationRoom();
        }
        break;
    case kControl_Text_TI00:
        {
        }
        break;
    case kControl_Text_S000:
        {
        }
        break;
    case kControl_Text_S001:
        {
            ShowEditControl(kTitleEditControl, true);
        }
        break;
    case kControl_Text_S002:
        {
            ShowEditControl(kPasswordEditControl, true);
        }
        break;
    case kControl_Text_T001:
        {
        }
        break;
    case kControl_Edit_E001://방제목
        {
            ShowEditControl(kTitleEditControl, true);
        }
        break;
    case kControl_Edit_E002://비밀번호
        {
            ShowEditControl(kPasswordEditControl, true);
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

    for (int i = kTitleEditControl; i < kEditControlTypeMax; ++i)
    {
        RECT static_edit_rect = static_edit_controls_[i]->GetSizeRect();
        int mouse_x = Mouse::Instance()->GetMouseX();
        int mouse_y = Mouse::Instance()->GetMouseY();
        if (mouse_x > static_edit_rect.left && mouse_x < static_edit_rect.right
            && mouse_y > static_edit_rect.top && mouse_y < static_edit_rect.bottom)
        {
            ShowEditControl(i, true);//kControl_Text_T201:// 입력창에 있는 static 창
        }
        else
        {
            ShowEditControl(i, false);
        }
    }

    return result;
}

void uiBattlezone2Tree::NetworkProc(MSG_BASE* message_ptr)
{
    //배틀존 패킷만 처리한다
    if (message_ptr && message_ptr->m_byCategory != CG_PARTY)
    {
        return;
    }

    TCHAR format_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR map_name[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR add_party_string[INTERFACE_STRING_LENGTH] = {0,};

    switch (message_ptr->m_byProtocol)
    {
    case CG_PARTY_ASK_JOIN_ROOM_BRD:
        {
            MSG_CG_PARTY_ASK_JOIN_ROOM_BRD* recv_packet = (MSG_CG_PARTY_ASK_JOIN_ROOM_BRD*)message_ptr;
            if (recv_packet->party_key_ != 0)
            {
                set_save_party_key(recv_packet->party_key_);
                uiSystemMan* system_manager =
                    static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                if(system_manager)
                {
                    // %s방이 생성 되었습니다. 입장 하시겠습니까?
                    g_InterfaceManager.GetInterfaceString(2486, string, INTERFACE_STRING_LENGTH);
                    const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)recv_packet->map_code_);
                    if (map_info)
                    {
                        g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name);
                    }
                    else
                    {
                        sprintf(map_name, "");
                    }
                    Snprintf(format_string, INTERFACE_STRING_LENGTH, string, map_name);
                    system_manager->Sys_Confirm(format_string, GlobalFunc::PartyRoomJoinConfirm, NULL, NULL, 100/*확인*/, 101/*취소*/);
                }
            }
        }
    case CG_PARTY_CHECK_ROOM_ACK:      
        {
			bool isMakeRoom = false;
            MSG_CG_PARTY_CHECK_ROOM_ACK* recv_packet = (MSG_CG_PARTY_CHECK_ROOM_ACK*)message_ptr;
            if (recv_packet->find_party_key_ != 0)
            {
                set_save_party_key(recv_packet->find_party_key_);
                uiSystemMan* system_manager =
                    static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                if(system_manager)
                {
                    if (recv_packet->check_type_ == MSG_CG_PARTY_CHECK_ROOM_ACK::CheckCreateType)
                    {
                        //2491 %s 방이 이미 존재 합니다. 방 만들기를 취소하고 입장 하시겠습니까?
                        g_InterfaceManager.GetInterfaceString(2491, string, INTERFACE_STRING_LENGTH);
						isMakeRoom = true;
                    }
                    else
                    {
                        //2492 %s 방이 이미 존재 합니다. 지금 바로 입장 하시겠습니까?
                        g_InterfaceManager.GetInterfaceString(2492, string, INTERFACE_STRING_LENGTH);
                    }
                    
                    const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)recv_packet->map_code_);
                    if (map_info)
                    {
                        g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name);
                    }
                    else
                    {
                        sprintf(map_name, "");
                    }

                    if (g_pHero->IsParty())
                    {
                        g_InterfaceManager.GetInterfaceString(2476, add_party_string);
                    }

					Snprintf(buffer_string, INTERFACE_STRING_LENGTH, string, map_name);
					Snprintf(format_string, INTERFACE_STRING_LENGTH, "%s %s", buffer_string, add_party_string);

					if (isMakeRoom)
					{
						system_manager->Sys_Confirm(format_string,
							GlobalFunc::PartyRoomJoinConfirm, NULL, NULL, 10502/*기존방입장*/, 10503/*새로 만들기*/);
					}
					else
					{
						system_manager->Sys_Confirm(format_string, 
							GlobalFunc::PartyRoomJoinConfirm, NULL, NULL, 100/*확인*/, 101/*취소*/);
					}
                }
            }
            else	
            {
                switch (recv_packet->check_type_)
                {
                case MSG_CG_PARTY_CHECK_ROOM_ACK::CheckCreateType:
                    if (g_pHero->IsParty())
                    {
                        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                        uiSystemMan* system_manager =
                            static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                        if(system_manager)
                        {
                            // 방 입장시 현재 파티가 탈퇴 됩니다. 계속 하시겠습니까? 2475
                            g_InterfaceManager.GetInterfaceString(2475, string, INTERFACE_STRING_LENGTH);
                            system_manager->Sys_Confirm(string, GlobalFunc::PartyLeaveAndJoinConfirm, NULL, NULL, 100/*확인*/, 101/*취소*/);
                        }
                    }
                    else
                    {
                        CreateRoom();
                    }
                    break;
                case MSG_CG_PARTY_CHECK_ROOM_ACK::CheckStanByType:
                    {
                        uiBattlezone2Reservation* battlezone2_reservation = GET_CAST_DIALOG(uiBattlezone2Reservation, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION);
                        if (battlezone2_reservation)
                        {
                            battlezone2_reservation->ReservationStandByRoom();
                        }
                    }
                    break;
                }
            }
            //is_creating_ = false;
        }
        break;
    case CG_PARTY_CHANGE_ROOM_OPTION_ACK:
        {
            MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK* recv_packet = (MSG_CG_PARTY_CHANGE_ROOM_OPTION_ACK*)message_ptr;
            if (recv_packet->result_code_ == RC::RC_PARTY_SUCCESS)
            {
                // 변경 성공
                selected_create_room_.menu_index = created_room_.menu_index;
#ifdef _DEV_VER 
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "CG_PARTY_CHANGE_ROOM_OPTION_ACK:Success result code(%d)", recv_packet->result_code_);
#endif //_DEV_VER
            }
            else
            {   // 변경 실패
#ifdef _DEV_VER 
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "CG_PARTY_CHANGE_ROOM_OPTION_ACK:Fail result code(%d)", recv_packet->result_code_);
#endif //_DEV_VER
            }
        }
        break;
    }
}

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
void uiBattlezone2Tree::Callback_TreeRender(CCtrlListWZ* list_control, 
                                        CCtrlListWZ::CListSellData* sell_data, 
                                        int sell_index, 
                                        RECT& render_area)
{
    if (list_control == NULL || sell_data == NULL || sell_data->user_data == NULL) 
    {
        return;
    }

    int select_sell = list_control->GetCurSel();
    int over_sell = list_control->GetOverSel();

    TreeMenuGroup::TreeObject* tree_object = static_cast<TreeMenuGroup::TreeObject*>(sell_data->user_data);


    //------------------------------------------------------------------------------ 
    //! 커서 그리기
    DWORD cursor_image_code = tree_object->info->image_code[BASE_TreeMenuInfo::kIconType_Cursor];
    if ((select_sell == sell_index) && (cursor_image_code > 0))
    {
        BASE_TextureListInfo* tex_info = NULL;
        HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(cursor_image_code, tex_info);;

        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
            RECT dest_area = render_area;
            dest_area.right -= dest_area.left;
            dest_area.bottom -= dest_area.top;

            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)tex_info->tex_size.left,
                (float)tex_info->tex_size.top,
                (float)tex_info->tex_size.right,
                (float)tex_info->tex_size.bottom);
        }
    }

    render_area.left += tree_object->info->menu_depth_width;

    //------------------------------------------------------------------------------ 
    //! 아이콘 이미지.
    BASE_TreeMenuInfo::Icon_Type image_type = BASE_TreeMenuInfo::kIconType_Max;
    if (tree_object->is_enable)
    {
        if (tree_object->info->menu_type == BASE_TreeMenuInfo::kMenuType_Group)
        {
            image_type = tree_object->is_unfold ? BASE_TreeMenuInfo::kIconType_Unfold : BASE_TreeMenuInfo::kIconType_Collpase;
        }
        else if (tree_object->info->menu_type == BASE_TreeMenuInfo::kMenuType_CheckButton)
        {
            image_type = tree_object->is_check ? BASE_TreeMenuInfo::kIconType_CheckOn : BASE_TreeMenuInfo::kIconType_CheckOff;
        }
        else
        {
            image_type = BASE_TreeMenuInfo::kIconType_Enable;
        }
    }
    else
    {
        image_type = BASE_TreeMenuInfo::kIconType_Disable;
    }

    BASE_TextureListInfo* tex_info = NULL; 
    HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[image_type], 
        tex_info);
    if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
    {
        //! 버튼 아이콘
        RECT dest_area = render_area;
        dest_area.right = tex_info->tex_size.right;
        dest_area.bottom = tex_info->tex_size.bottom;

        BYTE alpha = 255;
        if (tree_object->is_icon_blink())
        {
            DWORD mod_tick_cycle = clock_function::GetTickCount() % 18000;//(1000 * 6);
            float sin_value = sin(WZDEG2RAD(DWORD(mod_tick_cycle * 0.2f)));
            alpha = BYTE(abs(sin_value) * 255.0f);
        }
        WzColor color[4] = {WzColor_RGBA(255, 255, 255, alpha),
            WzColor_RGBA(255, 255, 255, alpha),
            WzColor_RGBA(255, 255, 255, alpha),
            WzColor_RGBA(255, 255, 255, alpha)};

        g_pSunRenderer->RenderTexture(tex_handle, 
            (float)dest_area.left, 
            (float)dest_area.top, 
            (float)dest_area.right,
            (float)dest_area.bottom,
            (float)tex_info->tex_size.left,
            (float)tex_info->tex_size.top,
            (float)tex_info->tex_size.right,
            (float)tex_info->tex_size.bottom, 
            color);


        render_area.left += dest_area.right;
    }
    
    //------------------------------------------------------------------------------ 
    //! 모두분배 아이콘 그리기
    if (tree_object->is_notice())
    {
        //! 모두분배 아이콘
        static int test_a = 3;
        static int test_b = 2;
        const int c_zoneIconHeight = 16;
        const int icon_gap_height = 2;

        RECT dest_area = render_area;
        dest_area.top += icon_gap_height;
        dest_area.right = dest_area.left + c_zoneIconHeight;
        dest_area.bottom = dest_area.top + c_zoneIconHeight;

        g_InterfaceManager.RenderBattleRoomIcon(
            InterfaceManager::eBATTLE_ICON_PARTY_OBTAIN_ROOM,
            &dest_area);

        render_area.left += c_zoneIconHeight + 2; //!< 아이콘 & 글자 3픽셀 갭을 둔다.
    }

    //------------------------------------------------------------------------------ 
    //! 글자
    DWORD font_ID = StrToWzID("mn12");
    COLORREF font_color = sell_data->SelFontColor;
    if (tree_object->is_enable)
    {
        if (select_sell == sell_index)
        {
            font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Select];
            font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Select];
        }
        else if (over_sell == sell_index)
        {
            font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Over];
            font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Over];
        }
        else
        {
            font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Enable];
            font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Enable];
        }
    }
    else
    {
        font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Disable];
        font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Disable];
    }

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);
    {
        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &render_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

#endif //_DH_BATTLEZONE2_