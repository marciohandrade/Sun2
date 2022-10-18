#include "SunClientPrecompiledHeader.h"

#include "GlobalFunc.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiGuideInfomationDialog/uiGuideInfomationDialog.h"
#include "TotalGuideParser.h"
#include "GuideMessageParser.h"

#include "GraphicResourceList.h"

#ifdef _YMS_TREECONTROL
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#endif//_YMS_TREECONTROL
#include "hero.h"

const int kMaxLineAboutInfoImage = 11;
const int kMaxLineInfoRect = 23;
const TCHAR* kReturnFormatedString = "<RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/>";
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);//�ݱ�
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);//��
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);//��
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("VS00"), kControl_Scroll_VS00);//��ũ�ѹ�
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("V000"), kControl_Scroll_V000);//��ũ�ѹ�
#ifdef _YMS_TREECONTROL
    tree_menu().Release();
#endif//_YMS_TREECONTROL
}

uiGuideInfomationlDialog::uiGuideInfomationlDialog(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
    RegisterControl();
}

uiGuideInfomationlDialog::~uiGuideInfomationlDialog(void)
{
    Release();
}

#ifdef _YMS_TREECONTROL
//! static function
void uiGuideInfomationlDialog::Callback_InfoTreeRender(CCtrlListWZ* list_control, 
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
    //! Ŀ�� �׸���
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

    DWORD notice_image_code = 0;
    notice_image_code = BASE_TreeMenuInfo::kIconType_CheckOn;
    BASE_TextureListInfo* tex_info = NULL;
    HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[notice_image_code], 
        tex_info);        //����ǥ
    notice_image_code = BASE_TreeMenuInfo::kIconType_CheckOff;
    BASE_TextureListInfo* tex_info2 = NULL;
    HANDLE tex_handle2 = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[notice_image_code], 
        tex_info2);        //����ǥ���� ����

    if (tree_object->is_notice())
    {
        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
            RECT dest_area = render_area;
            dest_area.right = tex_info->tex_size.right;
            dest_area.bottom = tex_info->tex_size.bottom;

            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)tex_info->tex_size.left,
                (float)tex_info->tex_size.top,
                (float)tex_info->tex_size.right,
                (float)tex_info->tex_size.bottom);

            render_area.left += dest_area.right;
        }
    }
    else
    {        
        if ((tex_handle2 != INVALID_HANDLE_VALUE) && (tex_info2 != NULL))
        {
            RECT dest_area = render_area;
            dest_area.right = tex_info->tex_size.right;
            dest_area.bottom = tex_info->tex_size.bottom;

            g_pSunRenderer->RenderTexture(tex_handle2, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)tex_info2->tex_size.left,
                (float)tex_info2->tex_size.top,
                (float)tex_info2->tex_size.right,
                (float)tex_info2->tex_size.bottom);

            render_area.left += dest_area.right;
        }
    }
    
    tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[image_type], 
        tex_info);
    if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
    {
        RECT dest_area = render_area;
        dest_area.right = tex_info->tex_size.right;
        dest_area.bottom = tex_info->tex_size.bottom;

        g_pSunRenderer->RenderTexture(tex_handle, 
            (float)dest_area.left, 
            (float)dest_area.top, 
            (float)dest_area.right,
            (float)dest_area.bottom,
            (float)tex_info->tex_size.left,
            (float)tex_info->tex_size.top,
            (float)tex_info->tex_size.right,
            (float)tex_info->tex_size.bottom);


        render_area.left += dest_area.right;
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
        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &render_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}
#endif//_YMS_TREECONTROL

void uiGuideInfomationlDialog::Init(CDrawBase* drawbase)
{
#ifdef _YMS_TREECONTROL
    // ��Ʈ�� ��������
    std::vector<BASE_TreeMenuInfo*> value_vector;
    if (TreeMenuInfoParser::Instance()->GetTreeMenu(value_vector, BASE_TreeMenuInfo::kGroupType_Infomation) > 0)
    {
        std::vector<BASE_TreeMenuInfo*>::iterator begin = value_vector.begin();
        std::vector<BASE_TreeMenuInfo*>::iterator end = value_vector.end();
        for (; begin != end; ++begin)
        {
            StrnCopy((*begin)->string_name, 
                        GuideMessageParser::Instance()->GetGuideMessage((*begin)->string_code),
                        BASE_TreeMenuInfo::kNameLength);
        }
    }

    //! tree menu
    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_Infomation, 
                    GetListControlWZ(GetControlID_byPosition(kControl_List_L000)), 
                    GetVScrollControlWZ(GetControlID_byPosition(kControl_Scroll_V000)), 
                    true, 
                    uiGuideInfomationlDialog::Callback_InfoTreeRender);
    tree_menu().set_menu_height_gab(20);
#endif//_YMS_TREECONTROL

    ui_guide_system_manager_ = static_cast<uiGuideSystemMan*>(GetMan());
    assert(ui_guide_system_manager_);

    guide_page_ = NULL;
    page_infomation_ = NULL;
    current_line_ = 0;

    ftext_parser_.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB

    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
    if (control_ptr)
    {
        text_rect_ = control_ptr->GetSizeRect();
        view_rect_ = text_rect_;
    }
    ftext_render_.Init(GetDrawBase(), view_rect_);

    control_ptr = GetControlWZ_byPosition(kControl_Picture_P001);
    if (control_ptr)
    {
        image_rect_ = control_ptr->GetSizeRect();
        view_rect_.top = image_rect_.top;

        line_height_ = static_cast<float>(image_rect_.bottom - image_rect_.top);
        line_height_ = (line_height_ / kMaxLineAboutInfoImage);
    }

    ftext_render_.SetRect(view_rect_);
    image_texture_ = INVALID_HANDLE_VALUE;
    last_page_ = 0;

    for (BYTE i = 0; i < TotalGuideNotice::kMaxSize; ++i)
    {
        list_notice_count_[i] = 0;
    }
}

void uiGuideInfomationlDialog::ResetData()
{
    guide_page_ = NULL;
    page_infomation_ = NULL;
    current_line_ = 0;

    ftext_render_.SetRect(view_rect_);
    last_page_ = 0;

    for (BYTE i = 0; i < TotalGuideNotice::kMaxSize; ++i)
    {
        list_notice_count_[i] = 0;
    }

    AllTreeHide();
    InitFromData();
}

void uiGuideInfomationlDialog::Release()
{
    ftext_render_.ClearSentence();
    guide_page_ = NULL;
    page_infomation_ = NULL;

    if (image_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(image_texture_);
        image_texture_ = INVALID_HANDLE_VALUE;
    }
#ifdef _YMS_TREECONTROL
    tree_menu().Release();
#endif//_YMS_TREECONTROL
}

//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::AllTreeHide()
{
#ifdef _YMS_TREECONTROL
    const TotalGuideCategoryManager& guide_list = TotalGuideParser::Instance()->GetCategoryManager();
    const TotalGuideCategoryManager::CategoryMap& category_map = guide_list.category_map;
    TotalGuideCategoryManager::CategoryMap::const_iterator it = category_map.begin();

    TreeMenuGroup::TreeObject* tree_object = NULL;

    for (; it != category_map.end(); ++it) // �������� ��� �߰�
    {
        const TotalGuideCategory* const category = it->second;
        if (category != NULL)
        {
            TotalGuideCategory::GuideMap::const_iterator iter = category->guide_map.begin();
            BYTE category_code = static_cast<BYTE>(category->code);

            if (tree_object = tree_menu().Find(category_code, 0, 0, 0))
            {
                tree_object->is_visible = false;//���� �޴� �Ⱥ��̱�
            }
            for (; iter != category->guide_map.end(); ++iter)
            {
                const TotalGuide* const guide = iter->second;

                if (guide == NULL)
                {
                    break; // ����Ұ�
                }
                
                if (tree_object = tree_menu().Find(category_code, (guide->code%100), 0, 0))
                {
                    tree_object->is_visible = false;//�����޴� �Ⱥ��̱�
                }
            }
        }
    }
#endif//_YMS_TREECONTROL
}
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::InitFromData()
{
    LIBRARY_LIST_DATA* list_data = NULL;

    if (ui_guide_system_manager_)
    {
        list_data = ui_guide_system_manager_->library_list_data();
        if (list_data == NULL || list_data->library_data_header.packet_ver <= 100)// NULL �̸� || ���� ������ ���� ���� �������� list_data->... ���� ���� ����
        {
            //ó���� ��� ���� ������ ����Ѵ�.
            list_data->library_data_header.packet_ver = LIBRARY_DATA_PACKET_VER;
            list_data->library_data_header.library_list_count = 0;
            InitFromLevel();
            return;
        }

        TotalGuide* guide = NULL;        
        TotalGuide::Index code = 0;
        BYTE category_code = 0;

        for (int i = 0; i < list_data->library_data_header.library_list_count; ++i)
        {
            code = list_data->library_list[i].library_list_data;
            guide = TotalGuideParser::Instance()->GetGuide(code);
            if (guide == NULL)
            {
                continue;
            }
            AddData(code);
            if (list_data->library_list[i].is_show)
            {
                DeleteData(guide->category_code, code); //�� ��
            }
        }
    }
}
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::InitFromLevel()
{
    LIBRARY_LIST_DATA* list_data = NULL;
    const TotalGuideCategoryManager& guide_list = TotalGuideParser::Instance()->GetCategoryManager();
    const TotalGuideCategoryManager::CategoryMap& category_map = guide_list.category_map;
    TotalGuideCategoryManager::CategoryMap::const_iterator it = category_map.begin();

    if (g_pHero == NULL)
    {
        return;
    }
    for (; it != category_map.end(); ++it) // �������� ��� �߰�
    {
        const TotalGuideCategory* const category = it->second;
        if (category != NULL)
        {
            TotalGuideCategory::GuideMap::const_iterator iter = category->guide_map.begin();
            BYTE category_code = static_cast<BYTE>(category->code);
            
            for (; iter != category->guide_map.end(); ++iter)
            {
                const TotalGuide* const guide = iter->second;

                if (guide == NULL)
                {
                    break; // ����Ұ�
                }
                if (guide->level <= g_pHero->GetLevel())//�ش� ���� ���� ��� �߰�, �ߺ�üũ�� AddData->SaveData����
                {
                    AddData(guide->code);//������ �߰�
                }        
            }
        }
    }


}
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::SelectList(TotalGuideNotice::Index code)
{
#ifdef _YMS_TREECONTROL
    TreeMenuGroup::TreeObject* tree_object = NULL;
    TotalGuide* guide = TotalGuideParser::Instance()->GetGuide(code);
    if (guide)
    {
        tree_object = tree_menu().Find(static_cast<BYTE>(guide->category_code), code%100, 0, 0);
        tree_menu().Select(tree_object);
    }
#endif//_YMS_TREECONTROL
}
//------------------------------------------------------------------------------
bool uiGuideInfomationlDialog::AddData(TotalGuideNotice::Index code)
{
#ifdef _YMS_TREECONTROL
    TreeMenuGroup::TreeObject* tree_object = NULL;
    TotalGuide* guide = TotalGuideParser::Instance()->GetGuide(code);
    if (guide)
    {
        //�����Ϳ� ���� �߰� code, is_show = false
        bool is_saved = SaveData(guide->category_code, code, false);

        if (tree_object = tree_menu().Find(static_cast<BYTE>(guide->category_code), 0, 0, 0))
        {
            tree_object->is_visible = true;// �ϴ� �߰��Ǹ� ���̰� ����Ʈ
        }
        if (tree_object = tree_menu().Find(static_cast<BYTE>(guide->category_code), code%100, 0, 0))
        {
            tree_object->is_visible = true;// �ϴ� �߰��Ǹ� ���̰�
        }

        if (is_saved == false) //�̹� �ִ� ������ �߰��� �� ����.
        {
            return is_saved;
        }

        if (tree_object = tree_menu().Find(static_cast<BYTE>(guide->category_code), code%100, 0, 0))
        {
            //Ʈ���� ���̰� �Ѵ�.
            tree_object->is_visible = true;
            //New ����ǥ �߰��ϱ�
            tree_object->add_data_flag(TreeMenuGroup::kDataFlag_Notice);

            if (tree_object = tree_menu().Find(static_cast<BYTE>(guide->category_code), 0, 0, 0)) // ���� �޴� ������ ���̱�, notice ���� �߰�
            {
                tree_object->is_visible = true;
                //New ����ǥ �߰��ϱ�
                tree_object->add_data_flag(TreeMenuGroup::kDataFlag_Notice);
            }
        }
    }
#endif//_YMS_TREECONTROL
    return false;
}

//------------------------------------------------------------------------------
bool uiGuideInfomationlDialog::SaveData(TotalGuideNotice::Index category_code, TotalGuideNotice::Index code, bool is_show)
{
#ifdef _YMS_TREECONTROL
    if (ui_guide_system_manager_)
    {
        TreeMenuGroup::TreeObject* tree_object = NULL;
        bool is_have_data = false;
        LIBRARY_LIST_DATA* list_data = ui_guide_system_manager_->library_list_data();
        if (list_data == NULL)
        {
            return false;
        }
        // �ߺ� üũ �� ������ ����
        for (BYTE i = 0; i < list_data->library_data_header.library_list_count; ++i)
        {
            if (list_data->library_list[i].library_list_data == code)
            {
                is_have_data = true; // �̹� �ִ� ������
                if (list_data->library_list[i].is_show == false)//�� �� ��츸 �� ���� ���ð��� ���� �Ⱥ��ɷ� �� �� ����.
                {
                    tree_object = tree_menu().Find(static_cast<BYTE>(category_code), code%100, 0, 0);

                    if (is_show == true)//�� ���, �ִ� �����Ͱ� delete�ɶ�
                    {
                        list_data->library_list[i].is_show = true;   // ���� ���� ����
                    }
                    else//���� ���� ���, add�ɶ�1���� �Ǿ���ϴµ�
                    {
                        if (tree_object && tree_object->is_visible == false)// tree�� �̹� �߰� �Ǿ��ٸ� ��ġ�� ����Ǿ� �ִٴ� ��
                        {
                            ++list_notice_count_[category_code];//notice count����
                            if (tree_object = tree_menu().Find(static_cast<BYTE>(category_code), code%100, 0, 0))
                            {
                                tree_object->is_visible = true; // �޴��� �ϴ� ���̰�, ����ǥ ������
                                tree_object->add_data_flag(TreeMenuGroup::kDataFlag_Notice);
                            }
                        }
                    }
                }
                break;
            }
        }
        
        if (is_have_data == false) // �ߺ� �����Ͱ� ���� ��츸 �߰�
        {
            list_data->library_list[list_data->library_data_header.library_list_count].library_list_data = code;
            list_data->library_list[list_data->library_data_header.library_list_count].is_show = false;//�ߺ������Ͱ� ���°� �Ⱥ���
            ++list_data->library_data_header.library_list_count;//������ ���� ����
            ++list_notice_count_[category_code];//notice count����
            return true;
        }
    }
#endif//_YMS_TREECONTROL
    return false;
}
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::DeleteData(TotalGuideNotice::Index category_code, TotalGuideNotice::Index code)
{
#ifdef _YMS_TREECONTROL
    //�����Ϳ� ���� �߰� code, is_show = true
    bool is_saved = SaveData(category_code, code, true);//����

    if (is_saved == true) // ������ �� ���� �����Ͱ� �����ٴ� ���̹Ƿ� �� ��(is_show) ����.
    {
        return;
    }

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_object = tree_menu().Find(static_cast<BYTE>(category_code), code%100, 0, 0))
    {
        if (tree_object->is_notice() && list_notice_count_[category_code] > 0)
        {
            //New ����ǥ ����
            tree_object->remove_data_flag(TreeMenuGroup::kDataFlag_Notice);
            --list_notice_count_[category_code];//notice count����
        }
    }

    // ����� �����͸� ���� �ش� ī�װ��� New�� �ϳ��� ������ ���� �޴� notice�� ����
    if (list_notice_count_[category_code] == 0)
    {
        //New ����ǥ �����ϱ�
        if (tree_object = tree_menu().Find(static_cast<BYTE>(category_code), 0, 0, 0)) // ���� �޴� ������ ���̱�, notice ���� �߰�
        {
            tree_object->remove_data_flag(TreeMenuGroup::kDataFlag_Notice);
        }
    }
#endif//_YMS_TREECONTROL
}
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::ResetUI()
{
#ifdef _YMS_TREECONTROL
    tree_menu().Fill();
#endif//_YMS_TREECONTROL
}

void uiGuideInfomationlDialog::OnUpdateSolarDialog()
{
#ifdef _YMS_TREECONTROL
    tree_menu().UpdateMouseWheel();
#endif//_YMS_TREECONTROL
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();
        // ���콺�� ó��
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);
        OnMouseWheelScroll(mouse_wheel_state);
    }
}

void uiGuideInfomationlDialog::OnMouseWheelScroll(BYTE mouse_wheel_state)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Scroll_VS00);
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
        OnUpScroll(NULL);
    }
    else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
    {
        OnDownScroll(NULL);
    }
}

HANDLE uiGuideInfomationlDialog::LoadTextureImage(DWORD code)
{
    if (page_infomation_->image_code != 0)
    {
        if (image_texture_ != INVALID_HANDLE_VALUE)
        {
            g_pSunRenderer->x_pManagerTexture->DestroyTexture(image_texture_);
            image_texture_ = INVALID_HANDLE_VALUE;
        }
        const TotalGuideImage* image_name = TotalGuideParser::Instance()->GetImageManager()->GetGuideImage(code);
        if (image_name)
        {
            image_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(image_name->name_str, 
                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        }
        else
        {
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            sprintf(string, "LibraryImageList�� ��ΰ� �����ϴ�. (code %d)", code);
            assert(!string);
        }
        return image_texture_;
    }
    else
    {
        return INVALID_HANDLE_VALUE;
    }
}

void uiGuideInfomationlDialog::OnRenderSolarDialog()
{
    FRECT margin = {0.f,}; // �� ��ġ�� ����
    const float kdefault_width = 512.f;
    const float kdefault_height = 256.f;
    UpdateImageRect();
    ftext_render_.Render();

    int index = 0;
    if (image_texture_ != INVALID_HANDLE_VALUE)
    {
        float image_width = static_cast<float>(image_rect_.right - image_rect_.left);
        float image_height = static_cast<float>(image_rect_.bottom - image_rect_.top);
        float additional_height = (current_line_ * line_height_);
        float image_line_height = (static_cast<float>(image_height_) / kMaxLineAboutInfoImage) * current_line_;

        FRECT image_rect = {0.f,};
#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
         const TotalGuideImage* image  = NULL;

        if ( page_infomation_ != NULL )
        {
            image  = TotalGuideParser::Instance()->GetImageManager()->GetGuideImage(page_infomation_->image_code);
        }

        if (image)
        {
            image_rect.left = static_cast<float>(image->image_rect.left);
            image_rect.top = static_cast<float>(image->image_rect.top);
            image_rect.right = static_cast<float>(image->image_rect.right);
            image_rect.bottom = static_cast<float>(image->image_rect.bottom);// - image_line_height);

            float virtual_image_width = image_width; 
            image_width = image_rect.right - image_rect.left;
            float virtual_image_height = image_height; 
            image_height = image_rect.bottom - image_rect.top;

            if (image_width < virtual_image_width)
            {
                float margin_width = virtual_image_width - image_width;
                margin.left = margin_width/2;
                margin.right = margin_width/2;
            }
            else
            {
                image_width = virtual_image_width;
            }
            if (image_height < virtual_image_height)
            {
                float margin_height = virtual_image_height - image_height;
                margin.top = margin_height/2;
                margin.bottom = margin_height/2;
            }
            else
            {
                image_height = virtual_image_height;
            }

            if (additional_height > 0)
            {
                float temp_height = 0.f;
                temp_height = margin.top - additional_height;

                if (temp_height < 0)
                {
                    additional_height = additional_height - margin.top;
                    margin.top = 0;
                }
                else
                {
                    additional_height = 0;
                    margin.top = temp_height;
                }
            }
        }
#else
        image_rect.left = 0.f;
        image_rect.top = static_cast<float>(image_line_height);
        image_rect.right = static_cast<float>(image_width_);
        image_rect.bottom = static_cast<float>(image_height_ - image_line_height);
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        if (image_rect.bottom <= 0.0f)
        {
            image_rect.bottom = 0.0f;
        }
        //page_infomation_->image_code;
#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        g_pSunRenderer->RenderTexture(image_texture_,
            (float)image_rect_.left + margin.left,
            (float)image_rect_.top + margin.top,
            (float)image_width,
            (float)image_height - additional_height,
            (float)image_rect.left,
            (float)image_rect.top + ((additional_height>0)?image_line_height:0),
            (float)image_rect.right - image_rect.left,
            (float)image_rect.bottom - image_rect.top - ((additional_height>0)?image_line_height:0));
#else
        g_pSunRenderer->RenderTexture(image_texture_,
            (float)image_rect_.left + margin.left,
            (float)image_rect_.top + margin.top,
            (float)image_width,
            (float)image_height - additional_height,
            (float)image_rect.left,
            (float)image_rect.top,
            (float)image_rect.right,
            (float)image_rect.bottom);
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
    }
}

void uiGuideInfomationlDialog::SetData(TotalGuideNotice::Index code, BYTE page)
{
    CControlWZ* control_ptr = NULL;
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR* string_ptr = NULL;
    CODETYPE string_code = 0;
    guide_page_ = TotalGuideParser::Instance()->GetGuide(code);
    current_page_ = page;
    if (guide_page_)
    {
        if (AddData(code)) // �߰��� ��츸
        {
            DeleteData(guide_page_->category_code, code);// �߰����� ���� ���� ���̹Ƿ� ����ǥ ���� ǥ�ø� ���ؼ��� �̿�
        }

        ftext_render_.SetBeginLineIndex(0);
        CCtrlHScrollWZ* scroll_control = static_cast<CCtrlHScrollWZ*>(GetControlWZ_byPosition(kControl_Scroll_VS00));
        if (scroll_control)
        {
            scroll_control->SetScrollPosWZ(0);
        }
        current_line_ = 0;

        control_ptr = GetControlWZ_byPosition(kControl_Text_S002);
        if (control_ptr) // ������ ����
        {
            Sprintf(temp_string, "%d/%d", page, guide_page_->page_size);     
            control_ptr->SetTextWZ(temp_string);

            CControlWZ* control_ptr = NULL;

            if (page == guide_page_->page_size)
            {
                control_ptr = GetControlWZ_byPosition(kControl_Button_B004);
                if (control_ptr)
                {
                    control_ptr->ShowWindowWZ(WZ_HIDE);
                }
                control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
                if (control_ptr)
                {
                    control_ptr->ShowWindowWZ(WZ_SHOW);
                }
            }
            else
            {
                control_ptr = GetControlWZ_byPosition(kControl_Button_B004);
                if (control_ptr)
                {
                    control_ptr->ShowWindowWZ(WZ_SHOW);
                }
            }

            control_ptr = GetControlWZ_byPosition(kControl_Button_B003);
            if (control_ptr)
            {
                if (page == 1)
                {
                    control_ptr->ShowWindowWZ(WZ_HIDE); // 1�������� ������ ��ư �����/���̱�
                }
                else
                {
                    control_ptr->ShowWindowWZ(WZ_SHOW);
                }
            }
        }

        page_infomation_ = guide_page_->GetPage(page);
        if (page_infomation_)
        {
            control_ptr = GetControlWZ_byPosition(kControl_Text_S000);
            if (control_ptr) // ���� ����
            {
                string_ptr = GuideMessageParser::Instance()->GetGuideMessage(page_infomation_->head_str_code);
                if (string_ptr)
                {
                    control_ptr->SetTextWZ(string_ptr);
                }
            }

            control_ptr = GetControlWZ_byPosition(kControl_Picture_P000);
            if (control_ptr) // �̹��� ����
            {
                image_texture_ = LoadTextureImage(page_infomation_->image_code);
#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
                const TotalGuideImage* image = TotalGuideParser::Instance()->GetImageManager()->GetGuideImage(page_infomation_->image_code);
                
                if (image && image_texture_)
                {
                    image_width_ = image->image_rect.right - image->image_rect.left;
                    image_height_ = image->image_rect.bottom - image->image_rect.top;
                }
#else
                St_TextureContainer* texture_infomation = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(image_texture_);
                if (texture_infomation)
                {
                    image_width_ = texture_infomation->m_nWidth;
                    image_height_ = texture_infomation->m_nHeight;
                }
#endif//
                else
                {
                    image_width_ = 0;
                    image_height_ = 0;
                }
            }

            control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
            if (control_ptr) // ���� ����
            {
                string_ptr = GuideMessageParser::Instance()->GetGuideMessage(page_infomation_->content_str_code);
                //control_ptr->SetTextWZ(string);
                ftext_render_.ClearSentence();

                if (page_infomation_->image_code != 0)
                {
                    //8136	                            <RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/>
                    //const TCHAR* kReturnFormatedString = "<RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/>";
                    TCHAR msg_return[INTERFACE_STRING_LENGTH] = {NULL, };
                    if (g_InterfaceManager.GetInterfaceStringWithCheck(8136, msg_return, INTERFACE_STRING_LENGTH) == true)
                    {
                        Sprintf(temp_string, "%s%s", msg_return, string_ptr);
                    }
                    else
                    {
                        Sprintf(temp_string, "%s%s", kReturnFormatedString, string_ptr);
                    }
                    string_ptr = temp_string;
                }

                if (ftext_parser_.Parser(string_ptr, ftext_sentence_))
                {
                    ftext_render_.Append(ftext_sentence_);
                    ftext_sentence_.Release();

                    ftext_param_.eType = FORMAT_TEXT_PARAM_RETURN;
                    ftext_param_.dwValue = 1; // 1ĭ �����.
#ifdef _INTERNATIONAL_UI
                    ftext_param_.wzFont = StrToWzID("st10");
#else
                    ftext_param_.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
                    ftext_sentence_.Pushback(ftext_param_);

                    ftext_render_.Append(ftext_sentence_);
                    ftext_render_.convertLineInfo();

                    ENUM_STATEWZ scrollbar_state = WZ_HIDE;
                    if (ftext_render_.GetMaxLine() > kMaxLineInfoRect)
                    {
                        scrollbar_state = WZ_SHOW;
                        if (scroll_control)
                        {
                            scroll_control->SetScrollRangeWZ(0, ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect());
                        }
                    }
                    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Scroll_VS00);
                    if (control_ptr)
                    {
                        control_ptr->ShowWindowWZ(scrollbar_state);
                    }
                    control_ptr = GetControlWZ_byPosition(kControl_Picture_P000);
                    if (control_ptr)
                    {
                        control_ptr->ShowWindowWZ(scrollbar_state);
                    }
                }
            }
        }
    }
}

bool uiGuideInfomationlDialog::OnUpScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_VS00);
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

        current_line_ = current_scroll_pos;
        ftext_render_.SetBeginLineIndex(current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
    }

    return result;
}

bool uiGuideInfomationlDialog::OnDownScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_VS00);
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

        current_line_ = current_scroll_pos;
        ftext_render_.SetBeginLineIndex(current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
    }

    return result;
}

bool uiGuideInfomationlDialog::OnThumbScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_VS00);
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

        current_line_ = current_scroll_pos;
        ftext_render_.SetBeginLineIndex(current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
    }
    return result;
}

void uiGuideInfomationlDialog::UpdateImageRect()
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
    if (control_ptr)
    {
        text_rect_ = control_ptr->GetSizeRect();
        view_rect_ = text_rect_;
    }

    control_ptr = GetControlWZ_byPosition(kControl_Picture_P001);
    if (control_ptr)
    {
        image_rect_ = control_ptr->GetSizeRect();
    }
    view_rect_.top = image_rect_.top;
    ftext_render_.SetRect(view_rect_);
}

void uiGuideInfomationlDialog::OnShowWindow(BOOL val)
{
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GuideInfomation_Dialog;
        msg.wParam = IM_GUIDE_SYSTEM_MANAGER::INFOMATION;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        InitFromLevel();
        ResetUI();        
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_GuideInfomation_Dialog);
    }
}
//------------------------------------------------------------------------------
void uiGuideInfomationlDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

#ifdef _YMS_TREECONTROL
    //! Ʈ�� �޴� ó��
    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true)
    {
        if (tree_object != NULL)
        {
            //ó��
            const TotalGuideCategory* guide_category = TotalGuideParser::Instance()->GetCategory(tree_object->info->menu_depth[3]);
            if (guide_category)
            {
                TotalGuideCategory::GuideMap guide_map = guide_category->guide_map;
                for (TotalGuideCategory::GuideMap::iterator it = guide_map.begin(); it != guide_map.end(); ++it)
                {
                    TotalGuide* guide = it->second;
                    if (guide && (guide->code%100) == tree_object->info->menu_depth[2])
                    {
                        SetData(guide->code, 1);
                        DeleteData(guide->category_code, guide->code);
                        break;
                    }
                }
            }
        }
    }
#endif//_YMS_TREECONTROL

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
    case RT_MSG_SB_PAGETOP:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_Scroll_VS00)
            {
                CCtrlHScrollWZ* scroll_control = static_cast<CCtrlHScrollWZ*>(GetControlWZ_byPosition(kControl_Scroll_VS00));
                if (scroll_control)
                {
                    int index = scroll_control->GetScrollPosWZ();
                    --index;
                    index = max(0, index);
                    current_line_ = index;
                    ftext_render_.SetBeginLineIndex(index);
                    scroll_control->SetScrollPosWZ(index);
                }
            }
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
    case RT_MSG_SB_PAGEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_Scroll_VS00)
            {
                CCtrlHScrollWZ* scroll_control = static_cast<CCtrlHScrollWZ*>(GetControlWZ_byPosition(kControl_Scroll_VS00));
                if (scroll_control)
                {
                    int index = scroll_control->GetScrollPosWZ();
                    ++index;
                    ++current_line_;
                    index = min((int)(ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect()), index);
                    current_line_ = index;
                    ftext_render_.SetBeginLineIndex(index);
                    scroll_control->SetScrollPosWZ(index);
                }
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
    case RT_MSG_SB_THUMBPOSITION:
        {
            //.���� �巡�� �ϰ� �ִ�.

            if (GetControlPosition_byID(message->dwCtrlID) == kControl_Scroll_VS00)
            {
                CCtrlHScrollWZ* scroll_control = static_cast<CCtrlHScrollWZ*>(GetControlWZ_byPosition(kControl_Scroll_VS00));
                if (scroll_control)
                {
                    int index = scroll_control->GetScrollPosWZ();
                    ftext_render_.SetBeginLineIndex(index);
                    scroll_control->SetScrollPosWZ(index);
                }
            }
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}


//------------------------------------------------------------------------------
bool uiGuideInfomationlDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_BT01://�ݱ�
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B003://����
        {
            if (guide_page_ && 1 < current_page_)
            {
                SetData(guide_page_->code, (current_page_ - 1));
            }
        }
        break;
    case kControl_Button_B004://����
        {
            if (guide_page_->page_size > current_page_)
            {
                SetData(guide_page_->code, (current_page_ + 1));
            }

            if (guide_page_->page_size == current_page_)
            {
                CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
                if (control_ptr)
                {
                    control_ptr->ShowWindowWZ(WZ_SHOW);
                }
            }
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
//------------------------------------------------------------------------------
bool uiGuideInfomationlDialog::IsHaveNoticeGuide()
{
    for (BYTE i = 0; i < TotalGuideNotice::kMaxSize; ++i)
    {
        if (list_notice_count_[i] > 0)
        {
            return true;
        }
    }
    return false;
}
