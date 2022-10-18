#include "SunClientPrecompiledHeader.h"

#ifdef _YMS_TREECONTROL

#include "TreeMenuGroup.h"
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "GlobalData.h"
#include "InterfaceManager.h"
#include "uiCharPopupMenu\uiCharPopupMenu_def.h"

//------------------------------------------------------------------------------ 
//! static function
void TreeMenuGroup::Callback_TreeRender(CCtrlListWZ* list_control, 
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

    TreeObject* tree_object = static_cast<TreeObject*>(sell_data->user_data);


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


        render_area.left += dest_area.right + 3; //!< 아이콘 & 글자 3픽셀 갭을 둔다.
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



//------------------------------------------------------------------------------
//! TreeMenuGroup::TreeObject
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject::TreeObject()
{
    parent = NULL;
    info = NULL;
    is_visible = true;
    is_enable = true;
    is_unfold = false;
    data_flag_ = kDataFlag_None;
    menu_list.clear();
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject::TreeObject(TreeObject* _parent, BASE_TreeMenuInfo* _info, 
                                      bool _visible, bool _enable, DWORD _data_flag/* = kDataFlag_None*/)
{
    parent = _parent;
    info = _info;
    is_unfold = false;
    is_visible = _visible;
    is_enable = _enable;
    data_flag_ = _data_flag;
    menu_list.clear();
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject::~TreeObject()
{
    parent = NULL;
    info = NULL;
    is_unfold = false;
    is_visible = true;
    is_enable = true;
    data_flag_ = kDataFlag_None;
    menu_list.clear();
}
//------------------------------------------------------------------------------ 
//! TreeMenuGroup::TreeMenuGroup
//------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeMenuGroup() : 
    callback_listbox_sell_draw_(0)
    ,simple_menu_(0)
    ,extend_always_(0)
{
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::~TreeMenuGroup()
{
    Release();
}
//------------------------------------------------------------------------------ 
void TreeMenuGroup::Release()
{
    Cleanup();
    callback_listbox_sell_draw_ = NULL;
    simple_menu_ = false;
    ListboxScrollControlProduce::Release();
}
//------------------------------------------------------------------------------ 
void TreeMenuGroup::Cleanup()
{
    if (list_control() != NULL)
    {
        list_control()->DeleteItemAll();
    }

    Cleanup(root_menu());
}
//------------------------------------------------------------------------------ 
void TreeMenuGroup::Cleanup(TreeObject& tree_object)
{
    TreeObjectList::iterator begin = tree_object.menu_list.begin();
    TreeObjectList::iterator end = tree_object.menu_list.end();
    for (; begin != end;)
    {
        if (begin->menu_list.size() > 0)
        {
            Cleanup(*begin);
        }

        begin = tree_object.menu_list.erase(begin);
        end = tree_object.menu_list.end();
    }
    tree_object.menu_list.clear();
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::Init(BASE_TreeMenuInfo::Group_Type tree_group, 
                         CCtrlListWZ* _list_control, 
                         CCtrlVScrollWZ* _vscroll_control,
                         bool is_simple_menu,
                         CCtrlListWZ::fn_CallbackListBoxSellDraw fn)
{
    if (ListboxScrollControlProduce::Init(_list_control, _vscroll_control) == false)
    {
        return false;
    }

    set_simple_menu(is_simple_menu);

    if (fn != NULL)
    {
        callback_listbox_sell_draw_ = fn;
    }
    else
    {
        callback_listbox_sell_draw_ = TreeMenuGroup::Callback_TreeRender;
    }

    root_menu().is_visible = true;
    root_menu().is_enable = true;
    root_menu().is_unfold = true;

    if (ApplyTreeGroup(tree_group) == false)
    {
        return false;
    }

    if (list_control() != NULL)
    {
        list_control()->SetUnuseSelBar(false);
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::ApplyTreeGroup(BASE_TreeMenuInfo::Group_Type tree_group)
{
    if ((list_control() == NULL) || 
        (list_control()->GetListRowCnt() > 0) || 
        (root_menu().menu_list.size() > 0))
    {
        ASSERT(false && "before call function : Cleanup();");
        return false;
    }

    std::vector<BASE_TreeMenuInfo*> value_vector;
    if (TreeMenuInfoParser::Instance()->GetTreeMenu(value_vector, tree_group) > 0)
    {
        std::vector<BASE_TreeMenuInfo*>::iterator begin = value_vector.begin();
        std::vector<BASE_TreeMenuInfo*>::iterator end = value_vector.end();
        for (; begin != end; ++begin)
        {
            // 사용할 아이콘 로딩
            for (int i = 0; i < BASE_TreeMenuInfo::kIconType_Max; ++i)
            {
                TextureListInfoHandleManager::Instance()->LoadIcon((*begin)->image_code[i]);
            }

            Insert(root_menu(), *begin, 3);
        }
    }

    return true;
}

//------------------------------------------------------------------------------ 
void TreeMenuGroup::set_menu_height_gab(int val)
{
    if (list_control() != NULL)
    {
        list_control()->SetSellHeight(val);
    }
}

//------------------------------------------------------------------------------ 
bool TreeMenuGroup::Insert(TreeObject& tree_object, BASE_TreeMenuInfo* info, int menu_depth)
{
    if (info == NULL || menu_depth < 0 || menu_depth > 3)
    {
        return false;
    }

    int sub_depth = menu_depth - 1;
    if ((sub_depth >= 0) && 
        (info->menu_depth[sub_depth] > 0) && 
        (tree_object.menu_list.size() > 0))
    {
        return Insert(tree_object.menu_list.back(), info, sub_depth);
    }
    tree_object.menu_list.push_back( TreeObject(&tree_object, info, true, true, TreeMenuGroup::kDataFlag_None) );
    return true;
}

//------------------------------------------------------------------------------ 
void TreeMenuGroup::Fill()
{
    if (list_control() == NULL)
    {
        return;
    }

    list_control()->DeleteItemAll();
    Fill(root_menu());

    ResetVScroll();
}
//------------------------------------------------------------------------------ 
void TreeMenuGroup::Fill(TreeObject& tree_object)
{
    if (list_control() == NULL)
    {
        return;
    }
    TCHAR msg_text[INTERFACE_STRING_LENGTH] = {0,};

    TreeObjectList::iterator begin = tree_object.menu_list.begin();
    TreeObjectList::iterator end = tree_object.menu_list.end();

    for (; begin != end; ++begin)
    {
        if (begin->info == NULL || begin->is_visible == false)
        {
            continue;
        }

        if (_tcslen(begin->info->string_name) > 0)
        {
            StrnCopy(msg_text, begin->info->string_name, BASE_TreeMenuInfo::kNameLength);
        }
        else if (begin->info->string_code > 0)
        {
            g_InterfaceManager.GetInterfaceString(begin->info->string_code, msg_text, INTERFACE_STRING_LENGTH);
        }
        else
        {
            continue;
        }

        int index = list_control()->InsertItemBack();

        list_control()->SetItemWZA(index, 
                                msg_text, 
                                StrToWzID("st10"), 
                                DT_VCENTER | DT_LEFT, 
                                c_dwBackColor, 
                                c_dwTextColor, 
                                c_dwSelTextColor);


        //list_control()->SetItemBKColor(index, 0, 0xaa000000);
        //list_control()->SetItemLeftGab(index, 0, left_gab);
        list_control()->SetItemUserData(index, 0, (void*)&(*begin)); //!< begin == TreeObject
        list_control()->SetItemCallbackDraw(index, 0, callback_listbox_sell_draw_);

        if ((begin->is_unfold == true) && (begin->menu_list.size() > 0))
        {
            Fill(*begin);
        }
    }
}
//------------------------------------------------------------------------------ 
void TreeMenuGroup::Collapse(TreeObject* tree_object)
{
    if (tree_object == NULL)
    {
        return;
    }

    TreeObjectList::iterator begin = tree_object->menu_list.begin();
    TreeObjectList::iterator end = tree_object->menu_list.end();
    for (; begin != end; ++begin)
    {
        if (begin->menu_list.size() > 0)
        {
            Collapse(&(*begin));
        }

        if (begin->info == NULL || begin->info->is_folder())
        {
            begin->is_unfold = false;
        }
    }
}
//------------------------------------------------------------------------------ 
void TreeMenuGroup::Unfold(TreeObject* tree_object)
{
    if (tree_object == NULL)
    {
        return;
    }
    if (tree_object->parent != NULL)
    {
        Unfold(tree_object->parent);
    }

    if (tree_object->info == NULL || tree_object->info->is_folder())
    {
        //! root menu 이거나 폴더가 열렸을경우
        tree_object->is_unfold = true;
    }
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::Select()
{
    TreeObjectList::iterator iter = root_menu().menu_list.begin();
    if (iter != root_menu().menu_list.end())
    {
        return Select(&(*iter));
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::Select(TreeObject* tree_object)
{
    if (list_control() == NULL || tree_object == NULL)
    {
        return false;
    }


    if ( extend_always_ )
    {
        if (is_simple_menu() == true) 
        {
            //! 하위 메뉴 모두 닫는다.
            Collapse(&root_menu());
        }

        //! 해당 메뉴는 보이게 부모 폴더 연다.
        Unfold(tree_object->parent);

        //! 클릭된 메뉴 토글한다.
        if ((tree_object->is_enable == true) && 
            ((tree_object->info != NULL) && (tree_object->info->menu_type != BASE_TreeMenuInfo::kMenuType_None)))
        {
            //! 활성화 되어 있고 Menu_Type == kMenuType_None 이면 토글하지 않는다.
            tree_object->is_check = true;
        }
    }
    else
    {
        bool cur_value = tree_object->is_check; 

        if (is_simple_menu() == true) 
        {
            //! 하위 메뉴 모두 닫는다.
            Collapse(&root_menu());
        }

        //! 해당 메뉴는 보이게 부모 폴더 연다.
        Unfold(tree_object->parent);

        //! 클릭된 메뉴 토글한다.
        if ((tree_object->is_enable == true) && 
            ((tree_object->info != NULL) && (tree_object->info->menu_type != BASE_TreeMenuInfo::kMenuType_None)))
        {
            //! 활성화 되어 있고 Menu_Type == kMenuType_None 이면 토글하지 않는다.
            tree_object->is_check = !(cur_value);

        }
    }

    Fill();

    //------------------------------------------------------------------------------ 
    int item_count = list_control()->GetListRowCnt();
    for (int i = 0; i < item_count; ++i)
    {
        TreeObject* user_data = static_cast<TreeObject*>(list_control()->GetItemUserData(i, 0));
        if ((user_data != NULL) && (user_data == tree_object))
        {
            list_control()->SetCurSel(i);
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::MessageProc(SI_MESSAGE* msg, OUT TreeObject*& tree_object)
{
    switch (msg->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        return OnLButtonClick(msg, OUT tree_object);

    case RT_MSG_LBUTTONDCLICK:
        return OnLButtonDClick(msg, OUT tree_object);
    case RT_MSG_SB_LINETOP:
        return OnVScrollUp(msg);

    case RT_MSG_SB_LINEBOTTOM:
        return OnVScrollDown(msg);

    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        return OnVScrollThumb(msg);
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::OnLButtonClick(SI_MESSAGE* msg, OUT TreeObject*& tree_object)
{
    if ((msg == NULL) || 
        (list_control() == NULL) || 
        (msg->dwCtrlID != list_control()->GetCtrlID()))
    {
        return false;
    }

    tree_object = (TreeObject*)(list_control()->GetItemUserData(list_control()->GetCurSel(), 0));

    return Select(tree_object);
}
//------------------------------------------------------------------------------ 
bool TreeMenuGroup::OnLButtonDClick(SI_MESSAGE* msg, OUT TreeObject*& tree_object)
{
    return OnLButtonClick(msg, tree_object);
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject* TreeMenuGroup::CurrectTreeObject()
{
    if (list_control() != NULL)
    {
        if (list_control()->GetCurSel() < list_control()->GetListRowCnt())
        {
            return (TreeObject*)(list_control()->GetItemUserData(list_control()->GetCurSel(), 0));
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject* TreeMenuGroup::OverTreeObject()
{
    if (list_control() != NULL)
    {
        if (list_control()->GetOverSel() < list_control()->GetListRowCnt())
        {
            return (TreeObject*)(list_control()->GetItemUserData(list_control()->GetOverSel(), 0));
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject* TreeMenuGroup::Find(TreeObject& tree_object, DWORD menu_index)
{
    TreeObjectList::iterator begin = tree_object.menu_list.begin();
    TreeObjectList::iterator end = tree_object.menu_list.end();
    for (; begin != end; ++begin)
    {
        if (begin->info->menu_index == menu_index)
        {
            return &(*begin);
        }
        if (begin->menu_list.size() > 0)
        {
            if (TreeMenuGroup::TreeObject* res = Find((*begin), menu_index))
            {
                return res;
            }
        }
    }

    return NULL;
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject* TreeMenuGroup::Find(BYTE menuidx_1, BYTE menuidx_2, BYTE menuidx_3, BYTE menuidx_4)
{
    DWORD menu_index = menuidx_1 << 24 | menuidx_2 << 16 | menuidx_3 << 8 | menuidx_4 << 0;

    return Find(root_menu(), menu_index);
}
//------------------------------------------------------------------------------ 
TreeMenuGroup::TreeObject* TreeMenuGroup::Find(DWORD menu_index)
{
    return Find(root_menu(), menu_index);
}
#endif //#ifdef _YMS_TREECONTROL