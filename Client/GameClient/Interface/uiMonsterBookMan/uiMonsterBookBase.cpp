#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007086_20140318_MONSTERBOOK


#include "uiMonsterBookBase.h"
#include "MonsterBookParser.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "TextureListInfoHandleManager.h"
#include "Mouse.h"
#include "InterfaceManager.h"
#include "NPCInfoParser.h"
#include "ToggleEditBox.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"

BOOL uiMonsterBookBase::tree_loaded_ = FALSE;
HANDLE uiMonsterBookBase::handle_texture_ = INVALID_HANDLE_VALUE;
HANDLE uiMonsterBookBase::handle_texture2_ = INVALID_HANDLE_VALUE;
HANDLE uiMonsterBookBase::handle_active_texture_ = INVALID_HANDLE_VALUE;
DWORD uiMonsterBookBase::select_monster_code_ = 0;
BOOL uiMonsterBookBase::character_first_show_ = TRUE;
DWORD uiMonsterBookBase::tooltip_montercode_ = 0;


bool _sort_monsterbook_for_index(MonsterBookInfo* a, MonsterBookInfo* b)
{
    return (a->index_ < b->index_);
}

uiMonsterBookBase::uiMonsterBookBase()
{
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);     //kControl_List_L001,    // ����Ʈ - �����޴�
    RegisterControlPosition(StrToWzID("L005"), kControl_List_L005);     //kControl_List_L005,    // ����Ʈ - ���͸���Ʈ
    RegisterControlPosition(StrToWzID("V002"), kControl_VScroll_V002);  //kControl_VScroll_V002, // ��ũ��
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);     //kControl_Edit_E000,    // �˻� ����Ʈ 
    RegisterControlPosition(StrToWzID("S019"), kControl_Static_S019);   //kControl_Static_S005,  // �˻� �ؽ�Ʈ 
    RegisterControlPosition(StrToWzID("B086"), kControl_Button_B086);   //kControl_Button_B086,  // �ݱ�
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);   //kControl_Button_B002,  // �˻�
    
    RegisterControlPosition(StrToWzID("C000"), kControl_Check_C000);    //kControl_Check_C000,   // ��1
    //RegisterControlPosition(StrToWzID("C001"), kControl_Check_C001);    //kControl_Check_C001,   // ��2
    RegisterControlPosition(StrToWzID("C002"), kControl_Check_C002);    //kControl_Check_C002,   // ��3

    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);    // Ȯ�εȸ��� %s/%s

#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    toggle_edit_box_ = NULL;
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
    is_search_mode_ = FALSE;
}
//------------------------------------------------------------------------------
uiMonsterBookBase::~uiMonsterBookBase()
{
    if (handle_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture_);
        handle_texture_ = INVALID_HANDLE_VALUE;
    }
    if (handle_texture2_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture2_);
        handle_texture2_ = INVALID_HANDLE_VALUE;
    }
    if (handle_active_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_active_texture_);
        handle_active_texture_ = INVALID_HANDLE_VALUE;
    }

#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    if (toggle_edit_box_ != NULL)
    {
        delete toggle_edit_box_;
        toggle_edit_box_ = NULL;
    }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void uiMonsterBookBase::Init(CDrawBase* draw_base)
{
    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_MonsterBook_Menu,
        GetListControlWZ(GetControlID_byPosition(kControl_List_L001)),
        NULL,
        true, 
        NULL);
    tree_menu().set_menu_height_gab(20);

    tree_monster_list().Init(BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList,
        GetListControlWZ(GetControlID_byPosition(kControl_List_L005)),
        GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V002)),
        false, 
        Callback_MonsterListRender); // ��ο� ��
    tree_monster_list().set_menu_height_gab(28);


    if (tree_loaded_ == FALSE)
    {
        // ����ī�װ� Ʈ�� ����
        InitMonsterBookTree();
        
        handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
            "Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
        handle_texture2_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
            "Data\\Interface\\Common_bg02.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
        handle_active_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
            "Data\\Interface\\etc\\Help_Focus.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        tree_loaded_ = TRUE;
    }

    tree_menu().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_MonsterBook_Menu);
    tree_menu().Fill();

#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    if (GetControlWZ_byPosition(kControl_Edit_E000) && GetControlWZ_byPosition(kControl_Static_S019))
    {
        toggle_edit_box_ = new ToggleEditBox(GetControlWZ_byPosition(kControl_Edit_E000),
            GetControlWZ_byPosition(kControl_Static_S019));
    }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::Release()
{
    tree_menu().Release();
    tree_monster_list().Release();
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::OnRenderSolarDialog()
{
    RenderTooltip();
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::OnUpdateSolarDialog()
{
    VScrollDialog::Update();
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::RefreshMonsterCount()
{
    // Ȯ�ε� ���� �� ����
    uiMonsterBookMan* manager = static_cast<uiMonsterBookMan*>(GetMan());
    if (manager)
    {
        CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
        if (control)
        {
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
            Snprintf(temp_string, INTERFACE_STRING_LENGTH-1, "%d / %d",
                manager->identified_monster_count(), manager->total_monster_count());
            control->SetTextWZ(temp_string);
        }
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::SelectFirstMonster()
{
    // ����ī�װ� Ʈ�� ù��° �׸����� ����
    uiMonsterBookMan* monsterbook_man = static_cast<uiMonsterBookMan*>(GetMan());
    uiMonsterBookMan::MonsterBook_Tree::iterator iter =
        monsterbook_man->monsterbook_tree().find(1);
    if (iter != monsterbook_man->monsterbook_tree().end())
    {
        uiMonsterBookMan::MonsterBook_Group* monster_group = iter->second;
        SetMonsterList(monster_group);

        // ù��° �׸��� ù�����ڵ�� �ʱ⼼��
        TreeMenuGroup::TreeObject* first_tree = NULL;
        first_tree = tree_monster_list().Find(1,1,0,0);
        if (first_tree != NULL)
        {
            DWORD first_monster_code = first_tree->info->image_code[0];
            set_is_search_mode(FALSE);
            SetMonsterInfo(first_monster_code);
        }
    }
    
    // ����ī�װ��� 1��° �׸�����
    TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(1,0,0,0);
    if (tree_object != NULL)
    {
        tree_menu().Select(tree_object);
        current_window_state_.menu_select_ = tree_object->info->menu_index;
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::SetWindowState(WindowState& window_state)
{
    // ������ ���Ͱ� �ٲ������ ����
    if (current_window_state_.select_monster_ != window_state.select_monster_)
    {
        SetMonsterInfo(window_state.select_monster_);
    }

    // ���� ���� ���� ������
    current_window_state_ = window_state;

    // �޴� Ʈ�� ���� ����
    TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(current_window_state_.menu_select_);
    if (tree_object != NULL)
    {
        tree_object->is_check = false;
        tree_menu().Select(tree_object);
    }

    // Ʈ�� ����, üŷ�� ����
    tree_monster_list().Cleanup();
    tree_monster_list().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList);
    tree_monster_list().Fill();

    TreeMenuGroup* tree_menu_group = current_window_state_.list_tree_;
    TreeMenuGroup::TreeObject* src_tree = NULL;
    TreeMenuGroup::TreeObject* dest_tree = NULL;
    for (int index = 1; index < 999; ++index)
    {
        src_tree = tree_menu_group->Find(index,0,0,0);
        dest_tree = tree_monster_list().Find(index,0,0,0);
        if ((src_tree == NULL) || (dest_tree == NULL))
        {
            break;
        }
        dest_tree->is_check = src_tree->is_check;
    }
    // ��ũ�� ��ġ ����
    tree_monster_list().Fill();
    tree_monster_list().vscroll_control()->SetScrollPosWZ(current_window_state_.list_scroll_pos_);
    tree_monster_list().list_control()->SetBeginDrawIndex(current_window_state_.list_scroll_pos_);
 
    // �˻� ����Ʈ ����
    if (current_window_state_.search_string_ != NULL)
    {
#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
        if (toggle_edit_box_ && toggle_edit_box_->edit_control())
        {
            toggle_edit_box_->SetText(current_window_state_.search_string_);
            toggle_edit_box_->ActiveEditControl(false);
        }
#else
        CCtrlDxEditBox* edit_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
        if (edit_control)
        {
            edit_control->SetTextWZ(current_window_state_.search_string_);
        }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {
        if (character_first_show_ == TRUE)
        {
            SelectFirstMonster();
            character_first_show_ = FALSE;
        }

        if ((current_window_state_.window_pos_x_ >= 0) || (current_window_state_.window_pos_y_ >= 0))
        {
            MoveWindowWZ(current_window_state_.window_pos_x_, current_window_state_.window_pos_y_);
        }

        // Ȯ�ε� ���� �� ����
        RefreshMonsterCount();

#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
        if (toggle_edit_box_ && toggle_edit_box_->edit_control())
        {
            toggle_edit_box_->ActiveEditControl(false);
        }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
    }
    else
    {
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookBase::MessageProc(SI_MESSAGE* message)
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
        {
            OnSearch();
            
            ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
            if (chat_dialog != NULL)
            {
                // �̹� ������ ���Ϳ� ä��â�� �ȿ����� ����
                chat_dialog->m_preShow = TRUE;
            }
        }
        break;
    case RT_MSG_SB_LINETOP:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002)
            {
                OnVScrollUp(message);
            }
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002)
            {
                OnVScrollDown(message);
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_THUMBPOSITION:   //!< ���� �巡������ ���콺�� ���Ҵ�.
    case RT_MSG_SB_ENDSCROLL:       //!< ��ũ���� ���� �Ǿ���.
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V002)
            {
                OnVScrollThumb(message);
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}

bool uiMonsterBookBase::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;

    if (GetMan() == NULL)
    {
        return false;
    }

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true)
    {
        ClickTreeMenu(tree_object);
    }
    if (tree_monster_list().MessageProc(message, OUT tree_object) == true)
    {
        ClickMonsterList(tree_object);
    }
    
#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    if (toggle_edit_box_ &&toggle_edit_box_->MessageProc(message) == true)
    {
    }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Check_C000:   // ��1
    //case kControl_Check_C001:   // ��2
    case kControl_Check_C002:   // ��3
        {
            BYTE index = static_cast<BYTE>(GetControlPosition_byID(message->dwCtrlID) - kControl_Check_C000);
            uiMonsterBookMan* manager = static_cast<uiMonsterBookMan*>(GetMan());
            if (manager)
            {
                RECT rc;
                GetDialogWZ()->GetClientRect(&rc);
                current_window_state_.window_pos_x_= rc.left;
                current_window_state_.window_pos_y_ = rc.top;
                current_window_state_.list_tree_ = &tree_monster_list();
                current_window_state_.list_scroll_pos_ = tree_monster_list().vscroll_control()->GetScrollPosWZ();
#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
                if (toggle_edit_box_ && toggle_edit_box_->edit_control())
                {
                    toggle_edit_box_->GetText(current_window_state_.search_string_, MAX_PATH);
                }
#else
                CCtrlDxEditBox* edit_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
                if (edit_control)
                {
                    edit_control->GetTextWZ(current_window_state_.search_string_, MAX_PATH-1);
                }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL

                manager->click_tap_button(index);
            }
        }
        break;
    case kControl_Button_B086:
        {
            uiMonsterBookMan* manager = static_cast<uiMonsterBookMan*>(GetMan());
            if (manager)
            {
                manager->toggle_monster_book(TRUE);
            }
        }
        break;
    case kControl_Button_B002:
        {
            OnSearch();
        }
        break;
    default:
        {
        }
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------
void uiMonsterBookBase::RenderTooltip()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if ((tooltip_manager == NULL) || (tooltip_montercode_ == 0))
    {
        return;
    }

    BASE_NPCINFO* npc_info = NPCInfoParser::Instance()->GetNPCInfo(tooltip_montercode_);
    if (npc_info)
    {
        // ���� �̸�
        TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string2[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR grade_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR name_string[INTERFACE_STRING_LENGTH] = {0,};

        BOOL bGrade = TRUE;
        switch (npc_info->m_byGrade)
        {
        case eNPC_GENERAL: //11501	�Ϲ�
        case eNPC_MAPOBJECT_NPC:
            g_InterfaceManager.GetInterfaceString(11501, grade_string);
            break;
        case eNPC_ELITE: //11503	����
            g_InterfaceManager.GetInterfaceString(11503, grade_string);
            break;
        case eNPC_LEADER: //11502	����
            g_InterfaceManager.GetInterfaceString(11502, grade_string);
            break;
        case eNPC_MIDDLEBOSS: //11504	ĸƾ
            g_InterfaceManager.GetInterfaceString(11504, grade_string);
            break;
        case eNPC_BOSS: //11505	����
            g_InterfaceManager.GetInterfaceString(11505, grade_string);
            break;
        case eNPC_LUCKY_MONSTER: //11500	��Ű
            g_InterfaceManager.GetInterfaceString(11500, grade_string);
            break;
        case eNPC_MERCHANT_NPC:
            bGrade = FALSE;
            break;
        default:
            break;
        }

        g_InterfaceManager.GetInterfaceString(npc_info->m_NCode, name_string, INTERFACE_STRING_LENGTH);
#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_NPC_ID
        if (CGeneralGameParam::Instance()->IsShowNpcId())
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy(buffer_string, name_string, INTERFACE_STRING_LENGTH-1); 
            Snprintf(name_string, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                buffer_string, npc_info->m_MonsterCode);
        }
        //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

        // 3240 LV %s
        g_InterfaceManager.GetInterfaceString(3240, temp_string);

        if (bGrade == TRUE)
        {
            Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d %s(%s)",
                npc_info->m_DisplayLevel, 
                name_string,
                grade_string);
        }
        else
        {
            Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d %s",
                npc_info->m_DisplayLevel, 
                name_string);
        }
        Snprintf(result_string, INTERFACE_STRING_LENGTH-1, temp_string, temp_string2);

        tooltip_manager->RegisterTooltipSimpleText(result_string);
    }

    tooltip_montercode_ = 0;

    //tree_object->add_data_flag(TreeMenuGroup::kDataFlag_IconBlink);
}

//-----------------------------------------------------------------------------
CCtrlVScrollWZ* uiMonsterBookBase::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V002));
}
//------------------------------------------------------------------------------
bool uiMonsterBookBase::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L005)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L005)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool uiMonsterBookBase::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L005))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiMonsterBookBase::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L005))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiMonsterBookBase::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L005))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}

void uiMonsterBookBase::set_tap_control(BYTE index)
{
    // �� ��Ʈ�� ����
    for (int i = kControl_Check_C000; i <= kControl_Check_C002; ++i)
    {
        CCtrlButtonCheckWZ* check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
        if (check_control)
        {
            if (index == i - kControl_Check_C000)
            {
                check_control->SetCheckState(true);
            }
            else
            {
                check_control->SetCheckState(false);
            }
        }
    }
}

void uiMonsterBookBase::InitMonsterBookTree()
{
    // ����ī�װ� Ʈ��
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_MonsterBook_Menu);

    std::vector<BASE_TreeMenuInfo*> template_info_container;
    TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container,
        BASE_TreeMenuInfo::kGroupType_MonsterBook_Template);

    if (template_info_container.begin() == template_info_container.end())
    {
        return;
    }

    BASE_TreeMenuInfo* template_info = template_info_container.front();
    if (template_info == NULL)
    {
        return;
    }

    BASE_TreeMenuInfo new_menu_info = (*template_info);
    new_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_MonsterBook_Menu;

    uiMonsterBookMan* monsterbook_man = static_cast<uiMonsterBookMan*>(GetMan());
    for (int index = 1; index < 999; ++index)
    {
        uiMonsterBookMan::MonsterBook_Tree::iterator iter =
            monsterbook_man->monsterbook_tree().find(index);

        if (iter == monsterbook_man->monsterbook_tree().end())
        {
            break;
        }

        uiMonsterBookMan::MonsterBook_Group* monsterbook_group_tree = iter->second;
        uiMonsterBookMan::MonsterBook_Group::iterator group_iter = monsterbook_group_tree->begin();
        if (group_iter == monsterbook_group_tree->end())
        {
            break;
        }

        MonsterBookInfo* monsterbook_info = group_iter->second;
        if (monsterbook_info == NULL)
        {
            break;
        }

        // ����ī�װ�(����ī�װ�) �߰�
        new_menu_info.menu_depth[3] = index;
        new_menu_info.menu_depth[2] = 0;
        new_menu_info.menu_depth[1] = 0;
        new_menu_info.menu_depth[0] = 0;
        new_menu_info.string_code = monsterbook_info->main_category_string_;

        // ���õ� ���� ����
        TreeMenuInfoParser::Instance()->Insert(new_menu_info);

        // ���� ������ �ε�
        for ( ; group_iter != monsterbook_group_tree->end(); ++group_iter)
        {
            MonsterBookInfo* monsterbook_info = group_iter->second;
            if (monsterbook_info != NULL)
            {
                TextureListInfoHandleManager::Instance()->LoadIcon(monsterbook_info->monster_icon_);
            }
        }
    }
}
bool uiMonsterBookBase::ClickTreeMenu(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object)
    {
        tree_object->is_check = true;
    }

    current_window_state_.menu_select_ = tree_object->info->menu_index;

    // ���ø��� ���;����ܸ���Ʈ
    uiMonsterBookMan* monsterbook_man = static_cast<uiMonsterBookMan*>(GetMan());
    uiMonsterBookMan::MonsterBook_Tree::iterator iter =
        monsterbook_man->monsterbook_tree().find(tree_object->info->menu_depth[3]);
    if (iter != monsterbook_man->monsterbook_tree().end())
    {
        set_is_search_mode(FALSE);
        SetMonsterList(iter->second);
    }

    return true;
}
bool uiMonsterBookBase::ClickMonsterList(TreeMenuGroup::TreeObject* tree_object)
{
    // �ش� �ǿ� �°� ���� ���� ����
    if (tree_object == NULL || tree_object->info == NULL || tree_object->info->menu_depth[2] == 0)
    {
        MonsterBookInfo* monsterbook_info = static_cast<MonsterBookInfo*>(tree_object->info->user_data_ptr);
        if (monsterbook_info)
        {
            monsterbook_info->tree_unfold_ = (tree_object->is_unfold) ? TRUE : FALSE;
        }
        return true;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    int list_rect_left = tree_monster_list().list_control()->GetSizeRect().left;
    const int ICON_ROW_COUNT = 7;   // �� �ٴ� ������ ��
    const int background_image_size = 54; // ����̹��� ũ�� (�׵θ�3)
    //const int image_size = 48;
    const int image_row_gap = 2; // ���� �̹����� ����
    int row_size = background_image_size + image_row_gap;
    int image_pos_left = 0;
    int image_pos_right = 0;
    for (int i = 0; i < ICON_ROW_COUNT; ++i)
    {
        if (tree_object->info->image_code[i] == 0)
        {
            continue;
        }

        const int image_row_width = 10; // �̹��� �� ����
        image_pos_left = list_rect_left + (row_size * i) + image_row_width;
        image_pos_right = image_pos_left + background_image_size;

        if (image_pos_left < mouse_x && image_pos_right > mouse_x)
        {
            SetMonsterInfo(tree_object->info->image_code[i]);
            return true;
        }
    }

    return false;
}
void uiMonsterBookBase::SetMonsterList(uiMonsterBookMan::MonsterBook_Group* monster_group)
{
    // ���� ����Ʈ ���
    const int ICON_ROW_COUNT = 7;   // �� �ٴ� ������ ��
    if (ICON_ROW_COUNT > BASE_TreeMenuInfo::kIconType_Max)
    {
        // Ʈ���� �� ���� �̹����� ������ ���µ� ��ħ Ʈ���� ���� �߿� �������̹��� ������ �����־ �� ������ �����
        SUNLOG(eDEV_LOG, "MonsterBook >> ICON_ROW_COUNT >= BASE_TreeMenuInfo::kIconType_Max");
        return;
    }

    if (monster_group == NULL)
    {
        return;
    }

    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList);
    std::vector<BASE_TreeMenuInfo*> template_info_container;
    TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container,
        BASE_TreeMenuInfo::kGroupType_MonsterBook_Template);
    if (template_info_container.begin() == template_info_container.end())
    {
        return;
    }
    BASE_TreeMenuInfo* template_info = template_info_container.front();
    if (template_info == NULL)
    {
        return;
    }

    BASE_TreeMenuInfo new_menu_info = (*template_info);
    BASE_TreeMenuInfo new_monster_icon_info = (*template_info);
    BASE_TreeMenuInfo temp_icon_info = (*template_info);
    new_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList;
    new_monster_icon_info.tree_group = BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList;
    temp_icon_info.tree_group = BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList;

    for (int index = 1; index < 999; ++index)
    {
        std::pair<uiMonsterBookMan::MonsterBook_Group::iterator,
            uiMonsterBookMan::MonsterBook_Group::iterator> range;
        range = monster_group->equal_range(index);

        if (range.first == range.second)
        {
            break;
        }

        // �ε��� ������ ����
        STLX_VECTOR<MonsterBookInfo*> monster_info_vector;
        uiMonsterBookMan::MonsterBook_Group::iterator temp_iter = range.first;
        for ( ; temp_iter != range.second; ++temp_iter)
        {
            monster_info_vector.push_back(temp_iter->second);
        }
        std::sort(monster_info_vector.begin(), monster_info_vector.end(), _sort_monsterbook_for_index);

        for (int i = 0; i < BASE_TreeMenuInfo::kIconType_Max; ++i)
        {
            // ���� �̹������� image_code[] �迭�� ��� �뵵�� �ٲ㼭 ���
            new_monster_icon_info.image_code[i] = 0;
            temp_icon_info.image_code[i] = 0;
        }

        STLX_VECTOR<MonsterBookInfo*>::iterator iter = monster_info_vector.begin();

        int index2 = 0;
        while (1)
        {
            MonsterBookInfo* monsterbook_info = *iter;

            if (index2 == 0)
            {
                // ����Ʈ��
                new_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
                new_menu_info.menu_depth[3] = index;
                new_menu_info.menu_depth[2] = 0;
                new_menu_info.menu_depth[1] = 0;
                new_menu_info.menu_depth[0] = 0;
                new_menu_info.string_code   = monsterbook_info->sub_category_string_;
                new_menu_info.user_data     = 0;
                new_menu_info.user_data_ptr = is_search_mode() ? NULL : monsterbook_info;
                new_menu_info.menu_depth_width = 5;
                TreeMenuInfoParser::Instance()->Insert(new_menu_info);
            }

            // ���� Ȯ�� Ʈ�� �ֱ�
            // (index2 / ICON_ROW_COUNT) �� ICON_ROW_COUNT ���� ��ŭ �� Ʈ���� ���� ���� 
            // ex. ICON_ROW_COUNT�� 5�̸� 5���� ���ٷ� ������
            //     �ε��� 0 1 2 3 4 5 6 7 8 9 10 11..
            //       ��   0 0 0 0 0 1 1 1 1 1  2  2..
            // �������� "* 2) + 1" �� "* 2) + 2"�� ��¥ Ʈ���� ���� Ʈ���� �߰��� ������.  ¦������ �� �̹��� Ȧ������ �Ʒ� �̹���
            // ������ ����Ʈ ���� Ʈ��
            new_monster_icon_info.menu_depth[3] = index;
            new_monster_icon_info.menu_depth[2] = ((index2 / ICON_ROW_COUNT) * 2) + 1;
            new_monster_icon_info.menu_depth[1] = 0;
            new_monster_icon_info.menu_depth[0] = 0;
            new_monster_icon_info.image_code[index2 % ICON_ROW_COUNT] = monsterbook_info->monster_code_;
            new_monster_icon_info.string_code   = monsterbook_info->monster_code_;
    
            // ���� Ȯ���� �� Ʈ��
            temp_icon_info.menu_depth[3] = index;
            temp_icon_info.menu_depth[2] = ((index2 / ICON_ROW_COUNT) * 2) + 2;
            temp_icon_info.menu_depth[1] = 0;
            temp_icon_info.menu_depth[0] = 0;
            temp_icon_info.image_code[index2 % ICON_ROW_COUNT] = monsterbook_info->monster_code_;
            temp_icon_info.string_code = monsterbook_info->monster_code_;

            // ���ͷ����� ����
            ++iter;

            if (iter == monster_info_vector.end())
            {
                // ���̸� ���õ� ���� ���� �� for������
                TreeMenuInfoParser::Instance()->Insert(new_monster_icon_info);
                TreeMenuInfoParser::Instance()->Insert(temp_icon_info);
                break;
            }
            else
            {
                if (index2 % ICON_ROW_COUNT == ICON_ROW_COUNT-1)
                {
                    // �� �������̸� ���õ� ���� ���� �� ��� ����
                    TreeMenuInfoParser::Instance()->Insert(new_monster_icon_info);
                    TreeMenuInfoParser::Instance()->Insert(temp_icon_info);
                    for (int i = 0; i < BASE_TreeMenuInfo::kIconType_Max; ++i)
                    {
                        // ���� �̹������� image_code[] �迭�� ��� �뵵�� �ٲ㼭 ���
                        new_monster_icon_info.image_code[i] = 0;
                        temp_icon_info.image_code[i] = 0;
                    }
                }
                ++index2;
            }
        }//while (1)
    }

    tree_monster_list().Cleanup();
    tree_monster_list().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_MonsterBook_MonsterList);
    tree_monster_list().Fill();

    // ��� �׸� ����
    TreeMenuGroup::TreeObject* find_tree = NULL;
    for (int index = 1; index < 999; ++index)
    {
        find_tree = tree_monster_list().Find(index,0,0,0);
        if (find_tree == NULL)
        {
            break;
        }

        MonsterBookInfo* monsterbook_info = static_cast<MonsterBookInfo*>(find_tree->info->user_data_ptr);
        if ((is_search_mode() == TRUE) ||
            (monsterbook_info && monsterbook_info->tree_unfold_ == TRUE))
        {
            tree_monster_list().Select(find_tree);
        }
    }

    tree_monster_list().Fill();
}
void uiMonsterBookBase::Callback_MonsterListRender(CCtrlListWZ* list_control, 
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

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }
    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    TreeMenuGroup::TreeObject* tree_object = static_cast<TreeMenuGroup::TreeObject*>(sell_data->user_data);

    if (tree_object->info->menu_depth[2] == 0)
    {

        // ���� �׷��̸�
        // ����ó�� ���
        //------------------------------------------------------------------------------ 
        //! Ŀ�� �׸���
        DWORD cursor_image_code = tree_object->info->image_code[BASE_TreeMenuInfo::kIconType_Cursor];
        if ((tree_object->is_check == true) && (cursor_image_code > 0))
        {
            BASE_TextureListInfo* tex_info = NULL;
            HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(cursor_image_code, tex_info);;

            if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
            {
                RECT dest_area = render_area;
                dest_area.right -= dest_area.left;
                dest_area.bottom -= dest_area.top;

                if (dest_area.bottom > 0)
                {
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
        }

        render_area.left += tree_object->info->menu_depth_width;

        //------------------------------------------------------------------------------ 
        //! ������ �̹���.
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
            //! ��ư ������
            if (render_area.top + 4 < render_area.bottom)
            {
                RECT dest_area = render_area;
                dest_area.top += 4;
                dest_area.right = tex_info->tex_size.right;
                if (dest_area.top + tex_info->tex_size.bottom > dest_area.bottom)
                {
                    dest_area.bottom -= dest_area.top;
                }
                else
                {
                    dest_area.bottom = tex_info->tex_size.bottom;
                }

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

                if (dest_area.bottom < render_area.bottom)
                {
                    g_pSunRenderer->RenderTexture(tex_handle, 
                        (float)dest_area.left,
                        (float)dest_area.top,
                        (float)dest_area.right,
                        (float)dest_area.bottom,
                        (float)tex_info->tex_size.left,
                        (float)tex_info->tex_size.top,
                        (float)tex_info->tex_size.right,
                        (float)dest_area.bottom,
                        color);
                }
                
                render_area.left += dest_area.right + 4;
            }
        }

        //------------------------------------------------------------------------------ 
        //! ��κй� ������ �׸���
        if (tree_object->is_notice())
        {
            //! ��κй� ������
            const int c_zoneIconHeight = 16;
            const int icon_gap_height = 2;

            RECT dest_area = render_area;
            dest_area.top += icon_gap_height;
            dest_area.right = dest_area.left + c_zoneIconHeight;
            dest_area.bottom = dest_area.top + c_zoneIconHeight;

            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_PARTY_OBTAIN_ROOM,
                &dest_area);

            render_area.left += c_zoneIconHeight + 2; //!< ������ & ���� 3�ȼ� ���� �д�.
        }

        //------------------------------------------------------------------------------ 
        //! ����
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
            render_area.top += 7;
            g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
                &render_area,
                font_color,
                WzColor_RGBA(0, 0, 0, 0),
                TP_HLEFT /*| TP_VCENTER*/);
        }
        g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    }
    else
    {
        // ���� �׷��̸� ���͸���Ʈ ���
        BASE_TextureListInfo *temp_tex_info = NULL;
        HANDLE tex_handle = NULL;
        const int ICON_ROW_COUNT = 7;   // �� �ٴ� ������ ��
        for (int i = 0; i < ICON_ROW_COUNT; ++i)
        {
            if (tree_object->info->image_code[i] == 0)
            {
                return;
            }

            const int background_image_size = 54; // ����̹��� ũ�� (�׵θ�3)
            const int image_row_gap = 2; // ���� �̹����� ����
            int row_size = background_image_size + image_row_gap;
            RECT dest_area = render_area;
            
            const int image_row_width = 10; // �̹��� �� ����
            dest_area.left += (row_size * i) + image_row_width;
            dest_area.right = background_image_size;
            
            //////////////////////////////////////////////////////////////////////////
            dest_area.bottom -= dest_area.top + 1;

            RECT src_area;
            src_area.left = 275;
            src_area.top = 913;
            src_area.right = dest_area.right;
            src_area.bottom = dest_area.bottom;

            if (tree_object->info->menu_depth[2] % 2 == 1)
            {
                // Ȧ�� ���̸� ���κ�
                dest_area.top += 1;
            }
            else
            {
                // ¦�� ���̸� �Ʒ��κ�
                src_area.top += background_image_size / 2;
            }

            // ��� �̹��� ���
            if (handle_texture_ != INVALID_HANDLE_VALUE)
            {
                g_pSunRenderer->RenderTexture(handle_texture_,
                    (float)dest_area.left,
                    (float)dest_area.top,
                    (float)dest_area.right,
                    (float)dest_area.bottom,
                    (float)src_area.left,
                    (float)src_area.top,
                    (float)src_area.right,
                    (float)src_area.bottom);
            }
            
            // Ȱ��ȭ �̹��� ���
            if ((tree_object->info->image_code[i] == select_monster_code_) &&
                (handle_active_texture_ != INVALID_HANDLE_VALUE))
            {
                DWORD current_tick = clock_function::GetTickCount();
                const DWORD half_freq = 1000;
                const DWORD stop_time = 1000;

                current_tick %= (half_freq*2 + stop_time);

                BYTE alpha = 255;
                if (current_tick < half_freq)
                {
                    alpha = static_cast<BYTE>(255 * (current_tick / static_cast<float>(half_freq)));
                }
                else if (current_tick < half_freq + stop_time)
                {
                    alpha = 255;
                }
                else
                {
                    alpha = static_cast<BYTE>(255 * (1 - ((current_tick - half_freq - stop_time) / static_cast<float>(half_freq))));
                }
                WzColor color[4] = {WzColor_RGBA(255, 255, 255, alpha),
                    WzColor_RGBA(255, 255, 255, alpha),
                    WzColor_RGBA(255, 255, 255, alpha),
                    WzColor_RGBA(255, 255, 255, alpha)};

                float src_area_y = 1;
                if (tree_object->info->menu_depth[2] % 2 == 0)
                {
                    src_area_y = 32.0f;
                }

                g_pSunRenderer->RenderTexture(handle_active_texture_,
                    (float)dest_area.left,
                    (float)dest_area.top,
                    (float)dest_area.right,
                    (float)dest_area.bottom,
                    1,
                    src_area_y,
                    62.0f,
                    31.0f,
                    color); // �ٱ� 1�ȼ� �� ��. �ձ� �׵θ� ���ַ���..
            }

            // ���� �̹��� ���
            const MonsterBookInfo* book_info = MonsterBookParser::Instance()->FindMonsterBookInfo(tree_object->info->image_code[i]);
            if (book_info == NULL)
            {
                continue;
            }

            dest_area.left += 3;    // �׵θ�(3) �β���ŭ ���δ�
            dest_area.right -= 6;

            tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(book_info->monster_icon_, temp_tex_info);
            if ((tex_handle != INVALID_HANDLE_VALUE) && (temp_tex_info != NULL))
            {
                src_area.left = temp_tex_info->tex_size.left;
                src_area.top = temp_tex_info->tex_size.top;
                src_area.right = temp_tex_info->tex_size.right;

                if (tree_object->info->menu_depth[2] % 2 == 1)
                {
                    // Ȧ�� ���̸� ���κ�
                    dest_area.top += 3;
                    dest_area.bottom -= 3;
                }
                else
                {
                    // ¦�� ���̸� �Ʒ��κ�
                    const int image_half_size = 24;
                    src_area.top += image_half_size;
                    
                    if (dest_area.bottom - 3 >= image_half_size)
                    {
                        // ���̰� �Ǹ� �׵θ� �κи�ŭ ����
                        dest_area.bottom -= 3;
                    }
                }

                src_area.bottom = dest_area.bottom;

                if ((handle_texture_ != INVALID_HANDLE_VALUE) &&
                    (book_info->active_ == TRUE))
                {
                    g_pSunRenderer->RenderTexture(tex_handle,
                        (float)dest_area.left,
                        (float)dest_area.top,
                        (float)dest_area.right,
                        (float)dest_area.bottom,
                        (float)src_area.left,
                        (float)src_area.top,
                        (float)src_area.right,
                        (float)src_area.bottom);
                }
            }

            // ��ǥ ���� �迡 ���� üũ
            if (dest_area.left <= mouse_x && dest_area.left + dest_area.right >= mouse_x &&
                dest_area.top <= mouse_y && dest_area.top + dest_area.bottom >= mouse_y)
            {
                tooltip_montercode_ = book_info->monster_code_;
            }
        }//for()
    }
}
void uiMonsterBookBase::OnSearch()
{
    uiMonsterBookMan* manager = static_cast<uiMonsterBookMan*>(GetMan());
    if (manager == NULL)
    {
        return;
    }

    TCHAR search_string[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL
    if (toggle_edit_box_ && toggle_edit_box_->edit_control())
    {
        toggle_edit_box_->GetText(search_string, MAX_PATH);
        toggle_edit_box_->ActiveEditControl(false);
    }
#else
    CCtrlDxEditBox* edit_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
    if (edit_control)
    {
        edit_control->GetTextWZ(search_string, MAX_PATH);
    }
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL

    manager->Search(search_string);
}
#endif //_NA_007086_20140318_MONSTERBOOK
