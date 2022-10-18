#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatusHonorInfo.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiCharacterStatus/uiCharacterStatusHonorDescription.h"
#include "HonorSystem/HonorSystem.h"
#include "FameListParser.h"
#include "ReputeListParser.h"
#include "TextureListInfoParser.h"
#include "TextureListInfoHandleManager.h"

const DWORD uiCharacterStatusHonorInfo::kDialog_ID = g_IDGenerator.GenerateID();
const int kListSellHeight = 32;
const BYTE kFameMenuIndex = 255; // 0xff

uiCharacterStatusHonorInfo::uiCharacterStatusHonorInfo(InterfaceManager* interface_manager)
:uiBase(interface_manager)
{

}

uiCharacterStatusHonorInfo::~uiCharacterStatusHonorInfo()
{

}

void uiCharacterStatusHonorInfo::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);

    honor_info_menu().Release();
}

void uiCharacterStatusHonorInfo::Release()
{
    honor_info_menu().Release();
}

void uiCharacterStatusHonorInfo::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        RefreshTab();
    }
    else
    {
        uiCharacterStatusHonorDescription* honor_description_ui = GET_CAST_DIALOG(uiCharacterStatusHonorDescription, uiCharacterStatusHonorDescription::kDialog_ID);
        if (honor_description_ui != NULL)
        {
            honor_description_ui->ShowInterface(FALSE);
        }
    }
}

void uiCharacterStatusHonorInfo::OnUpdateSolarDialog()
{
    honor_info_menu().UpdateMouseWheel();

    uiCharacterStatusHonorDescription* honor_description_ui = GET_CAST_DIALOG(uiCharacterStatusHonorDescription, uiCharacterStatusHonorDescription::kDialog_ID);
    if (honor_description_ui != NULL)
    {
        RECT control_rect;
        GetDialogWZ()->GetClientRect(&control_rect);

        honor_description_ui->MoveWindowWZ(control_rect.right, control_rect.top);
    }
}

void uiCharacterStatusHonorInfo::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetMan() == NULL)
    {
        return;
    }

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (honor_info_menu().MessageProc(message, OUT tree_object) == true)
    {
        if (tree_object != NULL)
        {
            switch (message->eResultMsg)
            {
            case RT_MSG_LBUTTONCLICK:
                {
                    OnClickHonorInfo(tree_object->info);
                }
                break;
            default:
                {
                }
            }
        }
    }

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

bool uiCharacterStatusHonorInfo::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	case kControl_CheckBT_C004:
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
        {
            uiCharacterStatusMan* ui_status_manager = GET_DIALOG_MANAGER(uiCharacterStatusMan, UIMAN_CHARACTERSTATUS);
            if (ui_status_manager != NULL)
            {
                TabCheckButtonControllManager& tab_manager = ui_status_manager->status_dialog_tab_manager();
                tab_manager.OnClickTab(control_position - kControl_CheckBT_C000);
            }
            RefreshTab();
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiCharacterStatusHonorInfo::OnClickHonorInfo(BASE_TreeMenuInfo* tree_menu_info)
{
    if (tree_menu_info == NULL)
    {
        return;
    }

    if (tree_menu_info->is_folder() == true)
    {
        return;
    }

    uiCharacterStatusHonorDescription* honor_description_ui = GET_CAST_DIALOG(uiCharacterStatusHonorDescription, uiCharacterStatusHonorDescription::kDialog_ID);
    if (honor_description_ui == NULL)
    {
        return;
    }

    BYTE honor_type = 0;
    WORD honor_code = 0;
    if (tree_menu_info->menu_depth[3] == kFameMenuIndex)
    {
        // 명성
        honor_type = HONOR_POINT_INFO::FAME;
        honor_code = tree_menu_info->menu_depth[2];
    }
    else
    {
        honor_type = HONOR_POINT_INFO::REPUTE;
        honor_code = tree_menu_info->menu_depth[2];
    }

    honor_description_ui->SetHonorDescription(honor_type, honor_code);

    return;
}

void uiCharacterStatusHonorInfo::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C003 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((position == kControl_CheckBT_C002) ? (true) : (false));
    }
}

void uiCharacterStatusHonorInfo::InitHonorListInfo()
{
    honor_info_menu().Release();

    // 명성/평판 메뉴 정보 갱싱
    HonorSystem::HonorSystemFunction::RefreshHonorPointListMenu();

    // 명성/평판 리스트 메뉴 초기화
    honor_info_menu().Init(BASE_TreeMenuInfo::kGroupType_Honor_Info, 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000)), 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000)), 
        false, 
        uiCharacterStatusHonorInfo::Callback_HonorInfoMenuRender);

    honor_info_menu().set_menu_height_gab(kListSellHeight);

    // 리스트컨트롤에 명성/평판 리스트 채우기
    honor_info_menu().Fill();
    
    TreeMenuGroup::TreeObject* default_select =  honor_info_menu().Find(kFameMenuIndex, 1, 0, 0);
    if (default_select != NULL)
    {
        honor_info_menu().Select(default_select);
        
    }
}

void uiCharacterStatusHonorInfo::OnChangeHonorInfo(DWORD changed_data)
{

}

void uiCharacterStatusHonorInfo::Callback_HonorInfoMenuRender( CCtrlListWZ* list_control, CCtrlListWZ::CListSellData* sell_data, int sell_index, RECT& render_area )
{
    if (list_control == NULL || sell_data == NULL || sell_data->user_data == NULL) 
    {
        return;
    }

    TreeMenuGroup::TreeObject* tree_object = static_cast<TreeMenuGroup::TreeObject*>(sell_data->user_data);
    if (tree_object == NULL)
    {
        return;
    }

    
    int select_sell = list_control->GetCurSel();
    int over_sell = list_control->GetOverSel();

    bool is_folder = tree_object->info->is_folder();
    BASE_TreeMenuInfo::Icon_Type image_type = BASE_TreeMenuInfo::kIconType_Enable; 
    BASE_TextureListInfo* tex_info = NULL; 
    HANDLE tex_handle = INVALID_HANDLE_VALUE;
    BYTE menu_index = tree_object->info->menu_depth[3];
    
    DWORD rating = 0;
    DWORD current_value = 0;
    DWORD max_value = 1;

    if (is_folder == false)
    {
        if (menu_index == kFameMenuIndex)
        {
            HonorSystem::FamePointInfo* fame_point_info = (HonorSystem::FamePointInfo*)(tree_object->info->user_data);
            current_value = fame_point_info->fame_point_value;
            rating = FameListParser::GetFameRating(fame_point_info->fame_info, current_value);
            max_value = fame_point_info->fame_info->fame_rating_point[(min(rating, FameListParser::kMaxFameRating -1))];
            if (rating == FameListParser::kMaxFameRating)
            {
                current_value = min(current_value, max_value);
            }
            else if (rating != 0)
            {
                current_value -= fame_point_info->fame_info->fame_rating_accumulate_point[rating - 1];
            }
        }
        else
        {
            HonorSystem::ReputePointInfo* repute_point_info = (HonorSystem::ReputePointInfo*)(tree_object->info->user_data);
            current_value = repute_point_info->repute_point_value;
            rating = ReputeListParser::GetReputeRating(repute_point_info->repute_info, current_value);
            max_value = repute_point_info->repute_info->repute_rating_point[(min(rating, ReputeListParser::kMaxReputeRating -1))];
            if (rating == ReputeListParser::kMaxReputeRating)
            {
                current_value = min(current_value, max_value);
            }
            else if (rating != 0)
            {
                current_value -= repute_point_info->repute_info->repute_rating_accumulate_point[rating - 1];
            }
        }
    }    
    
    // 이미지 출력
    if (is_folder == true)
    {
        // 폴더 정보이면
        // 폴더 이미지 출력
        image_type = tree_object->is_unfold ? BASE_TreeMenuInfo::kIconType_Unfold : BASE_TreeMenuInfo::kIconType_Collpase;

        tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(tree_object->info->image_code[image_type], 
            tex_info);
        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
            RECT dest_area = render_area;
            int offset = (kListSellHeight - tex_info->tex_size.bottom) / 2;
            dest_area.left += offset;
            dest_area.top += offset;
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
        }
    }
    else
    {
        // 수치 정보이면
        if (over_sell == sell_index)
        {
            // 오버 이미지 출력
            image_type = BASE_TreeMenuInfo::kIconType_CheckOn;
        }
        else if (select_sell == sell_index)
        {
            // 선택 이미지 출력
            image_type = BASE_TreeMenuInfo::kIconType_Cursor;
        }

        // 배경
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
        }

        // 게이지
        tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(10024, 
            tex_info);
        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL) && (current_value > 0))
        {
            RECT dest_area = render_area;
            dest_area.right = (LONG)((float)(tex_info->tex_size.right) * current_value / max_value);
            dest_area.bottom = tex_info->tex_size.bottom;
            dest_area.left += 288;
            dest_area.top += 8;

            g_pSunRenderer->RenderTexture(tex_handle, 
                (float)dest_area.left, 
                (float)dest_area.top, 
                (float)dest_area.right,
                (float)dest_area.bottom,
                (float)tex_info->tex_size.left,
                (float)tex_info->tex_size.top,
                (float)dest_area.right,
                (float)tex_info->tex_size.bottom);
        }
    }

    DWORD font_ID = StrToWzID("mn12");
    COLORREF font_color = sell_data->SelFontColor;
    if (over_sell == sell_index)
    {
        font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Select];
        font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Select];
    }
    else
    {
        font_ID = tree_object->info->font_id[BASE_TreeMenuInfo::kFontType_Enable];
        font_color = tree_object->info->font_color[BASE_TreeMenuInfo::kFontType_Enable];
    }

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);
    {
        RECT dest_area = render_area;
        dest_area.left += kListSellHeight;

        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &dest_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);

        if (is_folder == false)
        {
            dest_area = render_area;
            dest_area.left += 288;

            TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
            if (over_sell == sell_index)
            {
                Snprintf(message, INTERFACE_STRING_LENGTH - 1, "%d/%d", current_value, max_value);
            }
            else
            {
                int rating_name_code = 0;
                if (tree_object->info->menu_depth[3] == kFameMenuIndex)
                {
                    rating_name_code = 6501 + rating;
                }
                else
                {
                    rating_name_code = 6508 + rating;
                }
                g_InterfaceManager.GetInterfaceString(rating_name_code, message);
            }

            g_pSunRenderer->x_pManagerTextOut->DrawText(message,
                &dest_area,
                font_color,
                WzColor_RGBA(0, 0, 0, 0),
                TP_HCENTER | TP_VCENTER);
        }
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

#endif //_NA_003027_20111013_HONOR_SYSTEM

