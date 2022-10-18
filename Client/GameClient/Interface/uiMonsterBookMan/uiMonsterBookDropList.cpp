#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007086_20140318_MONSTERBOOK

#include "uiMonsterBookDropList.h"
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#include "GlobalFunc.h"
#include "uiMonsterBookMan/MonsterBookDropOrderParser.h"
#include "uiMonsterBookMan/MonsterBookDropItemParser.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "TextureListInfoHandleManager.h"
#include "ItemInfoParser.h"
#include "GameFunc.h"
#include "NPCInfoParser.h"

uiMonsterBookDropList::MONTERBOOK_LOAD_ICON_LIST uiMonsterBookDropList::loaded_icon_list_;
DWORD uiMonsterBookDropList::tooltip_itemcode_ = 0;

uiMonsterBookDropList::uiMonsterBookDropList(InterfaceManager* ui_manager)
{
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);    // lv %s
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);    // �̸�
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);

    tooltip_item_slot_.SetSlotType(ST_ITEM);
}
//------------------------------------------------------------------------------
uiMonsterBookDropList::~uiMonsterBookDropList()
{
    UnloadIconTexture();
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::Init(CDrawBase* draw_base)
{
    uiMonsterBookBase::Init(draw_base);

    tree_drop_list().Init(BASE_TreeMenuInfo::kGroupType_MonsterBook_DropList,
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000)),
        GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000)),
        false, 
        Callback_DropListRender); // ��ο� ��
    tree_drop_list().set_menu_height_gab(26);
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::Release()
{
    uiMonsterBookBase::Release();

    tree_drop_list().Release();
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::NetworkProc(struct MSG_BASE *message)
{
    uiMonsterBookBase::NetworkProc(message);
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::OnRenderSolarDialog()
{
    uiMonsterBookBase::OnRenderSolarDialog();
    RenderTooltip();
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::OnUpdateSolarDialog()
{
    uiMonsterBookBase::OnUpdateSolarDialog();

    tree_drop_list().UpdateMouseWheel();
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::OnShowWindow(BOOL is_show)
{
    uiMonsterBookBase::OnShowWindow(is_show);

    if (is_show)
    {
        set_tap_control(MonsterBookTap_DropList);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_MonsterBook_DropList;
        msg.wParam = IM_MONSTERBOOK_MANAGER::MONSTERBOOK_DROPLIST;
        msg.DoSomething = GlobalFunc::DoMonsterBookDialog;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_MonsterBook_DropList);
    }
}
//------------------------------------------------------------------------------
void uiMonsterBookDropList::MessageProc(SI_MESSAGE* message)
{
    uiMonsterBookBase::MessageProc(message);

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
    case RT_MSG_SB_LINEBOTTOM:
    case RT_MSG_SB_THUMBTRACK:      //!< ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_THUMBPOSITION:   //!< ���� �巡������ ���콺�� ���Ҵ�.
    case RT_MSG_SB_ENDSCROLL:       //!< ��ũ���� ���� �Ǿ���.
        {
            TreeMenuGroup::TreeObject* tree_object = NULL;
            tree_drop_list().MessageProc(message, tree_object);
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}


//------------------------------------------------------------------------------
bool uiMonsterBookDropList::OnLButtonClick(SI_MESSAGE* message)
{
    if (GetMan() == NULL)
    {
        return false;
    }

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_drop_list().MessageProc(message, OUT tree_object) == true)
    {
        ClickTreeDropList(tree_object);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
bool uiMonsterBookDropList::ClickTreeDropList(TreeMenuGroup::TreeObject* tree_object)
{
    if (tree_object && tree_object->info && tree_object->info->user_data_ptr)
    {
        MonsterBookDropOrderInfo* drop_order_info =
            static_cast<MonsterBookDropOrderInfo*>(tree_object->info->user_data_ptr);
        if (drop_order_info)
        {
            drop_order_info->tree_unfold = (tree_object->is_unfold) ? TRUE : FALSE;
        }
    }
    

    return true;
}
//-----------------------------------------------------------------------------
void uiMonsterBookDropList::UnloadIconTexture()
{
    MONTERBOOK_LOAD_ICON_LIST::iterator itr = loaded_icon_list_.begin();
    MONTERBOOK_LOAD_ICON_LIST::iterator itr_end = loaded_icon_list_.end();

    for (; itr != itr_end; ++itr)
    {
        IconImage* icon_image = itr->second;
        ICONMGR()->RemoveIcon(icon_image);
    }

    loaded_icon_list_.clear();
}
//-----------------------------------------------------------------------------
void uiMonsterBookDropList::RenderTooltip()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if ((tooltip_manager == NULL) || (tooltip_itemcode_ == 0))
    {
        return;
    }

    tooltip_item_slot_.SetCode(static_cast<SLOTCODE>(tooltip_itemcode_));
    //item_slot.SetEnchant();
    //item_slot.SetSocketNum();
    //item_slot.SetSocketIdentifyType();

    //static DWORD SERIAL = 10000000;
    //item_slot.SetSerial(++SERIAL);

    tooltip_manager->RegisterTooltipItem(&tooltip_item_slot_, FALSE);

    tooltip_itemcode_ = 0;

    //tree_object->add_data_flag(TreeMenuGroup::kDataFlag_IconBlink);
}
//-----------------------------------------------------------------------------
void uiMonsterBookDropList::SetMonsterInfo(DWORD monster_code)
{
    // �ǿ� �´� �������� ����
    if (current_window_state_.select_monster_ == monster_code)
    {
        return;
    }
    current_window_state_.select_monster_ = monster_code;
    select_monster_code_ = monster_code;

    BASE_NPCINFO* monster_info = NPCInfoParser::Instance()->GetNPCInfo(monster_code);

    if (monster_info == NULL)
    {
        return;
    }

    // �ؽ�ó ��ε�
    UnloadIconTexture();

    // ���� �̸�
    TCHAR result_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string2[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR grade_string[INTERFACE_STRING_LENGTH] = {0,};

    BOOL bGrade = TRUE;
    switch (monster_info->m_byGrade)
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

    // ���� ���� (���)
    if (bGrade == TRUE)
    {
        Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d %s",
            monster_info->m_DisplayLevel, 
            grade_string);
    }
    else
    {
        Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d",
            monster_info->m_DisplayLevel);
    }

    // 3240 LV %s
    g_InterfaceManager.GetInterfaceString(3240, temp_string);
    Snprintf(result_string, INTERFACE_STRING_LENGTH-1, temp_string, temp_string2);

    CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S007));
    if (control)
    {
        control->SetTextWZ(result_string);
    }

    // �����̸�
    control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S018));
    if (control)
    {
        g_InterfaceManager.GetInterfaceString(monster_info->m_NCode, result_string);
#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_NPC_ID
        if (CGeneralGameParam::Instance()->IsShowNpcId())
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy(buffer_string, result_string, INTERFACE_STRING_LENGTH-1); 
            Snprintf(result_string, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                buffer_string, monster_info->m_MonsterCode);
        }
        //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
        control->SetTextWZ(result_string);
    }

    // �������Ʈ ����
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_MonsterBook_DropList);
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
    new_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_MonsterBook_DropList;

    // �������Ʈ Ʈ�� ����
    int current_tree_index1 = 0;     // �̹��� ������ Ʈ���ε���. depth1
    int current_tree_index2 = 0;     // �̹��� ������ Ʈ���ε���. depth2
    int last_category_index = 0;    // ���������� ������ ����Ʈ���� ī�װ� ��ȣ
    for (int i = 1; i < 999; ++i)
    {
        MonsterBookDropOrderInfo* drop_order_info = 
            MonsterBookDropOrderParser::Instance()->GetMonsterBookDropOrderInfo(i);

        if (drop_order_info == NULL)
        {
            break;
        }

        if (drop_order_info->show_type == 0)
        {
            continue;
        }

        MONSTERBOOK_DROPITEM_LIST* item_list =
            MonsterBookDropItemParser::Instance()->GetMonsterBookDropItem(monster_code, drop_order_info->item_type);

        if (item_list == NULL || item_list->empty())
        {
            continue;
        }

        if (last_category_index != drop_order_info->category_group)
        {
            // ���������� ������ ī�װ� ��ȣ�� �ٸ��� ���� Ʈ�� ��������
            ++current_tree_index1;
            current_tree_index2 = 0;
            new_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
            new_menu_info.menu_depth[3] = current_tree_index1;
            new_menu_info.menu_depth[2] = 0;
            new_menu_info.menu_depth[1] = 0;
            new_menu_info.menu_depth[0] = 0;
            new_menu_info.string_code = drop_order_info->category_string;
            new_menu_info.user_data = 0;
            new_menu_info.user_data_ptr = drop_order_info;
            new_menu_info.menu_depth_width = 3;
            TreeMenuInfoParser::Instance()->Insert(new_menu_info);

            last_category_index = drop_order_info->category_group;
        }

        for (size_t j = 0; j < item_list->size(); ++j)
        {
            // Ʈ�� �ֱ�
            DWORD item_code = (*item_list)[j];
            new_menu_info.menu_depth[3] = current_tree_index1;
            new_menu_info.menu_depth[2] = (current_tree_index2 * 2) + 1;
            new_menu_info.menu_depth[1] = 0;
            new_menu_info.menu_depth[0] = 0;
            new_menu_info.string_code = item_code;
            new_menu_info.user_data = item_code;
            new_menu_info.menu_depth_width = 5;
            TreeMenuInfoParser::Instance()->Insert(new_menu_info);

            // ���� Ȯ���� ���� Ʈ�� �ֱ�
            new_menu_info.menu_depth[2] += 1;    // ������ ���� ����
            TreeMenuInfoParser::Instance()->Insert(new_menu_info);

            // �ؽ�ó �ε�
            const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
            if (item_info)
            {
                IconImage* icon_image = ICONMGR()->Add(item_info->m_wVICode);
                loaded_icon_list_.insert(MONTERBOOK_LOAD_ICON_LIST::value_type(item_code, icon_image));
            }
        }
    }

    tree_drop_list().Cleanup();
    tree_drop_list().ApplyTreeGroup(BASE_TreeMenuInfo::kGroupType_MonsterBook_DropList);
    tree_drop_list().Fill();

    // ��� �׸� ����
    TreeMenuGroup::TreeObject* find_tree = NULL;
    for (int index = 1; index < 999; ++index)
    {
        find_tree = tree_drop_list().Find(index,0,0,0);
        if (find_tree == NULL)
        {
            break;
        }

        MonsterBookDropOrderInfo* drop_order_info = static_cast<MonsterBookDropOrderInfo*>(find_tree->info->user_data_ptr);
        if (drop_order_info && drop_order_info->tree_unfold == TRUE)
        {
            tree_drop_list().Select(find_tree);
        }
    }

    tree_drop_list().Fill();
}


//------------------------------------------------------------------------------ 
void uiMonsterBookDropList::Callback_DropListRender(CCtrlListWZ* list_control, 
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
        // ���� �׷��̸� ���������� ���
        // ����̹��� ���
        if (uiMonsterBookBase::handle_texture2_ != NULL)
        {
            RECT dest_area = render_area;
            dest_area.right -= dest_area.left;
            dest_area.bottom -= dest_area.top;

            //////////////////////////////////////////////////////////////////////////
            RECT src_area;
            src_area.left = 750; //549
            src_area.top = 172;  //969
            src_area.right = dest_area.right;
            src_area.bottom = dest_area.bottom;
            //src_area.bottom = 52;

            if (tree_object->info->menu_depth[2] % 2 == 0)
            {
                // ¦�� ���̸� �Ʒ��κ�
                src_area.top += 26;
            }

            g_pSunRenderer->RenderTexture(uiMonsterBookBase::handle_texture2_, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)src_area.left,
                (float)src_area.top,
                (float)src_area.right,
                (float)src_area.bottom);
        }

        // ������ �ؽ�Ʈ ���
        const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(tree_object->info->user_data);
        if (item_info != NULL)
        {
            DWORD font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Enable];
            const COLORREF font_color = WzColor_RGBA(200, 200, 168, 255);

            RECT dest_area = render_area;

            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            if (tree_object->info->menu_depth[2] % 2 == 1)
            {
                // Ȧ�� ���̸� ���κ�
                //item_info->m_NCode;
                GameFunc::GetItemName(NULL, item_info, string, INTERFACE_STRING_LENGTH-1);

                static int testx = 53;
                static int testy = 10;
                
                dest_area.left += testx;
                dest_area.top += testy;
            }
            else
            {
                // ¦�� ���̸� �Ʒ��κ�
                g_InterfaceManager.GetInterfaceString(item_info->m_SCNCode, string, INTERFACE_STRING_LENGTH-1);

                static int testx2 = 53;
                static int testy2 = 1;

                dest_area.left += testx2;
                dest_area.top += testy2;
            }

            g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);
            {
                g_pSunRenderer->x_pManagerTextOut->DrawText(string,
                    &dest_area,
                    font_color,
                    WzColor_RGBA(0, 0, 0, 0),
                    TP_HLEFT /*| TP_VCENTER*/);
            }
            g_pSunRenderer->x_pManagerTextOut->EndUseFont();
        }

        // ������ ������ ���
        MONTERBOOK_LOAD_ICON_LIST::iterator itr = loaded_icon_list_.find(tree_object->info->user_data);
        if (itr != loaded_icon_list_.end())
        {
            IconImage* icon_image = itr->second;
            if (icon_image != NULL)
            {
                RECT dest_area = render_area;

                static int gap_x = 10;
                static int gap_y = 10;

                dest_area.left += gap_x;
                dest_area.right = dest_area.left + icon_image->GetSrcWidth();

                RECT src_area;
                src_area.left = 0;
                src_area.top = 0;
                
                if (tree_object->info->menu_depth[2] % 2 == 1)
                {
                    // Ȧ�� ���̸� ���κ�
                    dest_area.top += gap_y;
                }
                else
                {
                    // ¦�� ���̸� �Ʒ��κ�
                    src_area.top += icon_image->GetSrcHeight() / 2;

                    if (dest_area.bottom - dest_area.top - gap_y >= icon_image->GetSrcHeight() / 2)
                    {
                        // ���̰� �Ǹ� �ؿ��� �κи�ŭ ����
                        dest_area.bottom -= gap_y;
                    }

                }

                src_area.right = icon_image->GetSrcWidth();
                src_area.bottom = dest_area.bottom - dest_area.top;

                icon_image->Render(&dest_area, &src_area);


                // ��ǥ ���� �迡 ���� üũ
                if (dest_area.left <= mouse_x && dest_area.right >= mouse_x &&
                    dest_area.top <= mouse_y && dest_area.bottom >= mouse_y)
                {
                    tooltip_itemcode_ = tree_object->info->user_data;
                }
            }

        }
    }
}
//------------------------------------------------------------------------------ 

#endif //_NA_007086_20140318_MONSTERBOOK
