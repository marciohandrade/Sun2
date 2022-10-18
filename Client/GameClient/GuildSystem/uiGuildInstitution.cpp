#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildInstitution.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildFacilityImageParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GuildFacilityInfoParser.h"
#include "Hero.h"
#include "SCSlotContainer.h"
#include "OptionListParser.h"
#include "ItemInfoParser.h"
#include "SkillInfoParser.h"
#include "SkillTextInfoParser.h"
#include "StateInfoParser.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const DWORD uiGuildInstitution::kDialog_ID = StrToWzID("7343");

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
WzID uiGuildInstitution::wz_id_[kControl_Size] = 
{ 
    StrToWzID("C000"),
    StrToWzID("C001"),
    StrToWzID("C003"),
    StrToWzID("C004"),
    StrToWzID("C002"),

    StrToWzID("B001"),
    StrToWzID("B002"),
    StrToWzID("B005"),
    StrToWzID("B000"),
    StrToWzID("B004"),
    StrToWzID("B007"),

    StrToWzID("I000"),
    StrToWzID("I001"),
    StrToWzID("I002"),

    StrToWzID("I003"),
    StrToWzID("I004"),
    StrToWzID("I005"),

    StrToWzID("S000"),

    StrToWzID("L000"),
    StrToWzID("L004"),
    StrToWzID("L005"),

    StrToWzID("L001"),
    StrToWzID("L002"),
    StrToWzID("L003"),

    StrToWzID("P012"),
    StrToWzID("P015"),
    StrToWzID("P016"),

    StrToWzID("V000"),
};
#else
const DWORD kInstitutionNumber = 5;
const DWORD kInstitutionDescriptionCode[kInstitutionNumber] = {
    7324, 
    7325, 
    7326, 
    7327, 
    7328};
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
const int kImageWidth = 107;
const int kImageHeight = 60;

uiGuildInstitution::uiGuildInstitution( InterfaceManager* ui_manager )
{
    institution_image_handle() = INVALID_HANDLE_VALUE;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    tooltip_manager_ptr_ = NULL;
    mouse_over_pos_ = kControl_Size;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

uiGuildInstitution::~uiGuildInstitution()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (institution_image_handle_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(institution_image_handle_);
        institution_image_handle_ = INVALID_HANDLE_VALUE;
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInstitution::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
#ifdef _NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
#endif //_NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("L003"), kControl_List_L003);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("I000"), kControl_TextImage_I000);
    RegisterControlPosition(StrToWzID("I001"), kControl_TextImage_I001);
    RegisterControlPosition(StrToWzID("I002"), kControl_TextImage_I002);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    RegisterControlPosition(StrToWzID("I003"), kControl_TextImage_I003);
    RegisterControlPosition(StrToWzID("I004"), kControl_TextImage_I004);
    RegisterControlPosition(StrToWzID("I005"), kControl_TextImage_I005);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L004"), kControl_List_L004);
    RegisterControlPosition(StrToWzID("L005"), kControl_List_L005);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        HASH_TABLE_GUILD_FACILITY_IMAGE_INFO* hash_table = 
            GuildFacilityImageParser::Instance()->GetHashTable();
        guild_facility_max_size_ = hash_table->size();
        
        int max_range = guild_facility_max_size_ - kListNumber;
#else
        int max_range = (kInstitutionNumber - kListNumber);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        max_range = max(max_range, 0);
        scroll_control->SetScrollRangeWZ(0, max_range);
        scroll_control->SetScrollPosWZ(0);
        scroll_control->SetChangeSizeThumb(33);
    }

    institution_image_handle() = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\all_split01.tga", 
        TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    tooltip_manager_ptr_ = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    assert(tooltip_manager_ptr_);
    mouse_over_pos_ = kControl_Size;

    for (DWORD index = 0; index < kListNumber; ++index)
    {
        CCtrlListWZ* list_control_ptr = 
            static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000 + index));
        if (list_control_ptr)
        {
            list_control_ptr->SetSellHeight(13);
            list_control_ptr->SetColorSelBar(RGBA(150, 10, 10, 125));
            list_control_ptr->SetDiscardMessage(true);
        }
        list_control_ptr = 
            static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001 + index));
        if (list_control_ptr)
        {
            list_control_ptr->SetSellHeight(13);
            list_control_ptr->SetColorSelBar(RGBA(150, 10, 10, 125));
            list_control_ptr->SetDiscardMessage(true);
        }
    }

    CControlWZ* institution_control = NULL;
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        // 일반효과
        institution_control = GetControlWZ_byPosition(kControl_TextImage_I000 + index);
        if (institution_control != NULL)
        {
            // 7138 : 일반효과
            institution_control->SetTextID(7138);
        }
        // 특수효과
        institution_control = GetControlWZ_byPosition(kControl_TextImage_I003 + index);
        if (institution_control != NULL)
        {
            // 7140 : 특수효과
            institution_control->SetTextID(7140);
        }
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInstitution::Release()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    tooltip_manager_ptr_ = NULL;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInstitution::MessageProc( SI_MESSAGE* message )
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
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case RT_MSG_MOUSEOVER:
        {
            OnMouseOver(message);
        }
        break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case RT_MSG_SB_LINETOP:
    case RT_MSG_SB_PAGETOP:
        OnVScrollUp(message);
        break;
    case RT_MSG_SB_LINEBOTTOM:
    case RT_MSG_SB_PAGEBOTTOM:
        OnVScrollDown(message);
        break;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION        
    case RT_MSG_SB_THUMBTRACK:    // 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION: // 썸을 드래그 한 후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:     // 스크롤이 종료 되었다.
        {
                UpdateAll();
        }
        break;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildInstitution::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case kControl_CheckBT_C005:
#endif //_NA_008334_20150608_SONNENSCHEIN
        {
            uiGuildInfo* ui_guild_info = GET_CAST_DIALOG(uiGuildInfo, uiGuildInfo::kDialog_ID);
            if (ui_guild_info != NULL)
            {
                ui_guild_info->tab_button_controller().OnClickTab(
                    control_position - kControl_CheckBT_C000);
            }
            RefreshTab();
        }
    case kControl_Button_B001:
    case kControl_Button_B000:
    case kControl_Button_B002:
    case kControl_Button_B004:
    case kControl_Button_B005:
    case kControl_Button_B007:
        {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            ProcessButtonClick(control_position);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
        break;
    }

    return true;
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void uiGuildInstitution::ProcessButtonClick( const DWORD& control_position )
{
    int scroll_position = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control == NULL)
    {
        return;        
    }

    scroll_position = scroll_control->GetScrollPosWZ();
    CCtrlButtonWZ* institution_control = NULL;
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        if (control_position == (kControl_Button_B001 + index))
        {
            // 업그레이드 버튼
            BASE_GuildFacilityImageInfo* info_ptr = 
                GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);            
            
            GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;
            if (info_ptr && 
                GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)info_ptr->index_, facility_info))
            {
                AskFacilityUpgrade(facility_info.facility_code_);
            }
        }        
        if (control_position == (kControl_Button_B000 + index))
        {
            // 활성화 버튼
            BASE_GuildFacilityImageInfo* info_ptr = 
                GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);            

            GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;
            if (info_ptr && 
                GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)info_ptr->index_, facility_info))
            {
                AskFacilityActive(facility_info.facility_code_, facility_info.facility_level_);                
            }
        }
    }
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

bool uiGuildInstitution::OnVScrollUp( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        int range_min = 0;
        int range_max = 0;
        int currenct_position = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        --currenct_position;
        currenct_position = max(currenct_position, 0);
        currenct_position = min(currenct_position, range_max);
        scroll_control->SetScrollPosWZ(currenct_position);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION        
        UpdateAll();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }

    return true;
}

bool uiGuildInstitution::OnVScrollDown( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        int range_min = 0;
        int range_max = 0;
        int currenct_position = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        ++currenct_position;
        currenct_position = max(currenct_position, 0);
        currenct_position = min(currenct_position, range_max);
        scroll_control->SetScrollPosWZ(currenct_position);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        UpdateAll();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }

    return true;
}

void uiGuildInstitution::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        RefreshTab();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        mouse_over_pos_ = kControl_Size;
        UpdateAll();        
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    }
    else
    {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        mouse_over_pos_ = kControl_Size;        
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }
}

void uiGuildInstitution::OnUpdateSolarDialog()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    UpdateWheelScroll();
    UpdateActiveButtonState();
#else
    UpdateInstitutionInfo();
    UpdateButtonState();
    UpdateWheelScroll();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInstitution::OnRenderSolarDialog()
{
    // 시설 이미지 출력
    int scroll_position = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        scroll_position = scroll_control->GetScrollPosWZ();
    }

    CControlWZ* picture_control = NULL;
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        picture_control = GetControlWZ_byPosition(kControl_Picture_P012 + index);
        if (picture_control != NULL)
        {
            RECT control_rect = picture_control->GetSizeRect();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            BASE_GuildFacilityImageInfo* info_ptr = 
                GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);
            if (info_ptr == NULL)
                continue;
            g_pSunRenderer->RenderTexture(
                institution_image_handle(),
                static_cast<float>(control_rect.left),
                static_cast<float>(control_rect.top),
                static_cast<float>(control_rect.right - control_rect.left),
                static_cast<float>(control_rect.bottom - control_rect.top),
                static_cast<float>(info_ptr->x_),
                static_cast<float>(info_ptr->y_),
                static_cast<float>(info_ptr->width_),
                static_cast<float>(info_ptr->height_));
#else
            g_pSunRenderer->RenderTexture(
                institution_image_handle(), 
                static_cast<float>(control_rect.left), 
                static_cast<float>(control_rect.top), 
                static_cast<float>(control_rect.right - control_rect.left), 
                static_cast<float>(control_rect.bottom - control_rect.top), 
                0.0f, 
                static_cast<float>(kImageHeight * (scroll_position + index)), 
                static_cast<float>(kImageWidth), 
                static_cast<float>(kImageHeight));
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (tooltip_manager_ptr_ == NULL)
    {
        return;
    }

    RenderTooltip(scroll_position); // 툴팁 출력
#else
    // // [GUILD_RENEWAL] 길드리뉴얼 2차 - 시설 비활성 음영이미지
    CControlWZ* image_control = GetControlWZ(StrToWzID("P006"));
    if (image_control != NULL)
    {
        RECT control_rect = image_control->GetSizeRect();
        g_InterfaceManager.RenderRect(control_rect, WzColor_RGBA(0, 0, 0, 125));
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void uiGuildInstitution::RenderTooltip( const int& scroll_position )
{
    // 툴팁 계산을 위해서
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();
    CControlWZ* control_ptr = NULL;
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        control_ptr = GetControlWZ_byPosition(kControl_Text_S000);
        if (control_ptr)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                ProcessGuildPointTooltip();
                break;
            }
        }
        control_ptr = GetControlWZ_byPosition(kControl_Picture_P012 + index);
        if (control_ptr != NULL)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                ProcessTooltip(scroll_position + index + 1, TOOLTIP_TYPE_IMAGE);
                break;
            }
        }
        control_ptr = GetControlWZ_byPosition(kControl_TextImage_I000 + index);
        if (control_ptr)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                ProcessTooltip(scroll_position + index + 1, TOOLTIP_TYPE_NORMAL_EFFECT);
                break;
            }
        }
        control_ptr = GetControlWZ_byPosition(kControl_TextImage_I003 + index);
        if (control_ptr)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                ProcessTooltip(scroll_position + index + 1, TOOLTIP_TYPE_SPECIAL_EFFECT);
                break;
            }
        }
        control_ptr = GetControlWZ_byPosition(kControl_Button_B001 + index); // 업그레이드 버튼
        if (control_ptr)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                ProcessTooltip(scroll_position + index + 1, TOOLTIP_TYPE_UPGRADE);
                break;
            }
        }
        control_ptr = GetControlWZ_byPosition(kControl_Button_B000 + index); // 활성화 버튼
        if (control_ptr)
        {
            RECT control_rect = control_ptr->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX
                && control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
            {
                ProcessTooltip(scroll_position + index + 1, TOOLTIP_TYPE_ACTIVE);
                break;
            }
        }
    }
}

void uiGuildInstitution::UpdateFacilityListInfo()
{
    int scroll_position = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        scroll_position = scroll_control->GetScrollPosWZ();
    }    

    // 리스트 업데이트
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };
        CCtrlListWZ* list_control_ptr = 
            static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000 + index));
        if (list_control_ptr)
        {
            list_control_ptr->DeleteItemAll();
            for (DWORD i = 0; i < 5; ++i)
            {
                BASE_GuildFacilityImageInfo* info_ptr = 
                    GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);                

                GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;

                if (info_ptr == NULL)
                    continue;

                if (!(GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)info_ptr->index_, facility_info)))
                {
                    continue;
                }

                const FACILITY_INFO* facility_parser_info = 
                    GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(facility_info.facility_code_, i + 1);
                if (facility_parser_info == NULL)
                    continue;

                BASE_OptionList* option_list = OptionListParser::Instance()->GetOptionList(facility_parser_info->passive_option.option_type);
                if (option_list == NULL)
                    continue;

                int list_index = list_control_ptr->InsertItemBack();
                TCHAR level_string[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };                
                TCHAR option_string[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR option_result[INTERFACE_STRING_LENGTH] = {0, };
                // 7118 : Lv%s : %s
                g_InterfaceManager.GetInterfaceString(7118, format_string);
                g_InterfaceManager.GetInterfaceString(option_list->mdwNameCode, option_string);
                if (facility_parser_info->passive_option.option_value_type == 1) // 수치
                    Snprintf(option_result, INTERFACE_STRING_LENGTH, "%s %+d", option_string, facility_parser_info->passive_option.option_value);
                if (facility_parser_info->passive_option.option_value_type == 2 || facility_parser_info->passive_option.option_value_type == 5) // 비율
                    Snprintf(option_result, INTERFACE_STRING_LENGTH, "%s %+d%%", option_string, facility_parser_info->passive_option.option_value / 10);
                    
                Snprintf(level_string, INTERFACE_STRING_LENGTH, "%d", facility_parser_info->facility_level);
                Snprintf(temp_message, INTERFACE_STRING_LENGTH, format_string, level_string, option_result);

                if (facility_info.facility_level_ == list_index+1)
                {
                    list_control_ptr->SetItemWZ(list_index,
                        0,
                        temp_message,
                        StrToWzID("st11"),
                        DT_CENTER | DT_VCENTER,
                        RGBA(0, 0, 0, 0),
                        RGBA(255, 255, 0, 255),
                        RGBA(255, 255, 255, 255));                    
                }
                else
                {
                    list_control_ptr->SetItemWZ(list_index,
                        0,
                        temp_message,
                        StrToWzID("st11"),
                        DT_CENTER | DT_VCENTER,
                        RGBA(0, 0, 0, 0),
                        RGBA(128, 128, 128, 100),
                        RGBA(255, 255, 255, 255));
                }
            }
        }
        list_control_ptr = 
            static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001 + index));
        if (list_control_ptr)
        {
            list_control_ptr->DeleteItemAll();
            for (DWORD i = 0; i < 5; ++i)
            {
                BASE_GuildFacilityImageInfo* info_ptr = 
                    GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);

                GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;

                if (info_ptr == NULL)
                    continue;

                if (!(GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)info_ptr->index_, facility_info)))
                {
                    continue;
                }

                const FACILITY_INFO* facility_parser_info = 
                    GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(facility_info.facility_code_, i + 1);
                if (facility_parser_info == NULL)
                    continue;
                
                SkillScriptInfo* skill_info_ptr = SkillInfoParser::Instance()->GetSkillInfo(facility_parser_info->active_option.active_skill_code);

                if (skill_info_ptr == NULL)
                    continue;

                BASE_ABILITYINFO* ability_info = NULL;
                ability_info = skill_info_ptr->GetAbilityInfoByIndex(0);

                if (ability_info == NULL)
                    continue;

                TCHAR level_string[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR option_string[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR option_result[INTERFACE_STRING_LENGTH] = {0, };
                // 7118 : Lv%s : %s
                g_InterfaceManager.GetInterfaceString(7118, format_string);
                BASE_SkillTextInfo* skill_text_info = SkillTextInfoParser::Instance()->GetSkillTextInfo(skill_info_ptr->m_dwSkillDesccode);
                if (skill_text_info == NULL)
                    continue;
                g_InterfaceManager.GetInterfaceString(skill_text_info->m_dw_DescCode, option_string);
                switch (ability_info->m_iParam[0])
                {
                case VALUE_TYPE_VALUE:
                    {
                        // 그냥 수치
                        Snprintf(option_result, INTERFACE_STRING_LENGTH, "%s %+d", option_string, ability_info->m_iParam[1]);
                    }
                    break;
                case VALUE_TYPE_PERCENT_PER_CUR:
                    {
                        Snprintf(option_result, INTERFACE_STRING_LENGTH, "%s %+d%%", option_string, (int)(ability_info->m_iParam[1] / 10.0f));
                    }
                    break;
                case VALUE_TYPE_PERCENT_PER_MAX:
                    {
                        Snprintf(option_result, INTERFACE_STRING_LENGTH, "%s %+d%%", option_string, (int)(ability_info->m_iParam[1] / 10.0f));
                    }
                    break;
                case VALUE_TYPE_RATIO_VALUE:
                    {
                        Snprintf(option_result, INTERFACE_STRING_LENGTH, "%s %+d%%", option_string, (int)(ability_info->m_iParam[1] / 10.0f));
                    }
                }

                Snprintf(level_string, INTERFACE_STRING_LENGTH, "%d", facility_parser_info->facility_level);
                Snprintf(temp_message, INTERFACE_STRING_LENGTH, format_string, level_string, option_result);

                int list_index = list_control_ptr->InsertItemBack();
                if (facility_info.facility_level_ == list_index+1
                    && facility_info.facility_active_ == true)
                {
                    list_control_ptr->SetItemWZ(list_index,
                        0,
                        temp_message,
                        StrToWzID("st11"),
                        DT_CENTER | DT_VCENTER,
                        RGBA(0, 0, 0, 0),
                        RGBA(255, 255, 0, 255),
                        RGBA(255, 255, 255, 255));                    
                }
                else
                {
                    list_control_ptr->SetItemWZ(list_index,
                        0,
                        temp_message,
                        StrToWzID("st11"),
                        DT_CENTER | DT_VCENTER,
                        RGBA(0, 0, 0, 0),
                        RGBA(128, 128, 128, 100),
                        RGBA(255, 255, 255, 255));
                }
            }
        }
    }
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

void uiGuildInstitution::UpdateInstitutionInfo()
{
    TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    // 현재 길드 포인트
    CCtrlStaticWZ* guild_point = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (guild_point != NULL)
    {
        TCHAR guild_point_value[INTERFACE_STRING_LENGTH] = {'\0', };
        _itot(GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGP, guild_point_value, 10);
        g_InterfaceManager.GetInterfaceStringFormat(
            info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            2303, 
            guild_point_value);
        guild_point->SetTextWZ(info_string);
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    int scroll_position = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        scroll_position = scroll_control->GetScrollPosWZ();
    }

    CControlWZ* institution_control = NULL;
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        // 시설명
        institution_control = GetControlWZ_byPosition(kControl_TextImage_I000 + index);
        if (institution_control != NULL)
        {
            DWORD string_code = GuildSystem::Function::GetInstitutionNameCode(scroll_position + index);
            institution_control->SetTextID(string_code);
        }

        // 시설 설명
        institution_control = GetControlWZ_byPosition(kControl_Text_S002 + index);
        if (institution_control != NULL)
        {
            institution_control->SetTextID(kInstitutionDescriptionCode[scroll_position + index]);
            institution_control->SetStyle(DT_LEFT | DT_TOP | DT_WORDBREAK);
        }
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInstitution::UpdateButtonState()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    int scroll_position = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control == NULL)
    {
        return;        
    }
    scroll_position = scroll_control->GetScrollPosWZ();
    int guild_point = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGP;
    // 업그레이드 버튼 활성화 비활성화 체크
    for (DWORD index = 0; index < kListNumber; ++index)
    {
        // 업그레이드 버튼
        CControlWZ* control_ptr = 
            GetControlWZ_byPosition(kControl_Button_B001 + index);
        if (control_ptr)
        {
            BASE_GuildFacilityImageInfo* info_ptr = 
                GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);            

            GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;
            if (info_ptr && 
                GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)info_ptr->index_, facility_info))
            {
                const FACILITY_INFO* facility_parser_info = 
                    GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(facility_info.facility_code_, facility_info.facility_level_ + 1);
                if (facility_parser_info == NULL)
                {
                    GlobalFunc::EnableControl(control_ptr, false);

                    continue;
                }

                if (facility_info.facility_level_ >= kMaxFacilityLevel)
                {
                    GlobalFunc::EnableControl(control_ptr, false);
                }
                else
                {
                    bool is_possible = false;                    
                    switch (facility_parser_info->upgrade_type)
                    {
                    case FUI_ONLY_GUILDPOINT: // 길드포인트로만 업그레이드
                        {
                            if (guild_point >= (int)facility_parser_info->upgrade_guild_point)
                                is_possible = true;
                        }
                        break;
                    case FUI_ONLY_ITEM: // 아이템만 사용함
                        {
                            is_possible = HaveItem(facility_parser_info->upgrade_item_code);
                        }
                        break;
                    case FUI_GUILDPOINTANDITEM: // 길드포인트, 아이템 둘다 사용
                        {                            
                            bool is_have_item = HaveItem(facility_parser_info->upgrade_item_code);
                            if (guild_point >= (int)facility_parser_info->upgrade_guild_point && 
                                is_have_item == true)
                            {
                                is_possible = true;
                            }
                        }
                        break;
                    }

                    GlobalFunc::EnableControl(control_ptr, is_possible);
                }
            }
            else
            {
                GlobalFunc::EnableControl(control_ptr, false);
            }
        }
    }
#else
    // [GUILD_RENEWAL] 길드리뉴얼 2차
    CCtrlButtonWZ* button_control = NULL;

    for (DWORD position = kControl_Button_B001; position < (kControl_Button_B007 + 1); ++position)
    {
        button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(position));
        if (button_control == NULL)
        {
            continue;
        }

        GlobalFunc::EnableControl(button_control, false);
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInstitution::UpdateWheelScroll()
{
    CControlWZ* wheel_rect_control = GetControlWZ(StrToWzID("P006"));
    if (wheel_rect_control == NULL)
    {
        return;
    }

    RECT wheel_rect	= wheel_rect_control->GetSizeRect();
    switch (GetMouseWheelStatus(&wheel_rect))
    {
    case MOUSE_WHEEL_UP:
        OnVScrollUp(NULL);
        break; 
    case MOUSE_WHEEL_DOWN:
        OnVScrollDown(NULL);
        break;
    }
}

void uiGuildInstitution::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C002 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((position == kControl_CheckBT_C003) ? (true) : (false));
    }
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void uiGuildInstitution::ProcessTooltip( const int& facility_order, const FACILITY_TOOLTIP_TYPE& type )
{
    BASE_GuildFacilityImageInfo* facility_image_info 
        = GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(facility_order);
    if (facility_image_info == NULL)
    {
        return;
    }

    int facility_name = 0;
    int facility_explanation = 0;
    switch (type)
    {
    case TOOLTIP_TYPE_NONE:
        {
            // no operation
            return;
        }
        break;
    case TOOLTIP_TYPE_IMAGE:
        {            
            facility_name = facility_image_info->facility_name_code_;
            facility_explanation = facility_image_info->facility_explanation_code_;
            ProcessResultTooltip(facility_name, facility_explanation);
        }
        break;
    case TOOLTIP_TYPE_NORMAL_EFFECT:
        {            
            // 7138 : 일반효과
            facility_name = 7138;
            // 7139 : 시설을 업그레이드 하면 모든 길드원이 해당 효과를 받을 수 있습니다.
            facility_explanation = 7139;
            ProcessResultTooltip(facility_name, facility_explanation);
        }
        break;
    case TOOLTIP_TYPE_SPECIAL_EFFECT:
        {            
            // 7140 : 특수효과
            facility_name = 7140;
            // 7141 : 업그레이드 된 시설을 활성화 하면 모든 길드원이 해당 효과를 받을 수 있습니다.
            facility_explanation = 7141;
            ProcessResultTooltip(facility_name, facility_explanation);
        }
        break;
    case TOOLTIP_TYPE_UPGRADE:
        {
            facility_name = facility_image_info->facility_name_code_;
            facility_explanation = facility_image_info->facility_explanation_code_;
            ProcessResultTooltip(facility_name, facility_explanation, facility_order, TOOLTIP_TYPE_UPGRADE);
        }
        break;
    case TOOLTIP_TYPE_ACTIVE:
        {
            facility_name = facility_image_info->facility_name_code_;
            facility_explanation = facility_image_info->facility_explanation_code_;
            ProcessResultTooltip(facility_name, facility_explanation, facility_order, TOOLTIP_TYPE_ACTIVE);
        }
        break;
    }
    tooltip_manager_ptr_->SetTooltipDialogID(GetDialogKey());
}

void uiGuildInstitution::ProcessGuildPointTooltip()
{
    TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
    
    // 7127 : 길드 포인트는 길드가 레벨업을 할 때마다 1포인트씩 획득할 수 있습니다.
    g_InterfaceManager.GetInterfaceString(7127, result_message);
    tooltip_manager_ptr_->InitTooltip();
    tooltip_manager_ptr_->RegisterTooltip(result_message,
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(255, 255, 255, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

    tooltip_manager_ptr_->AddEmptyLineForTooltip();

    tooltip_manager_ptr_->SetTooltipDialogID(GetDialogKey());

}

void uiGuildInstitution::ProcessResultTooltip( const int& facility_name_index, const int& facility_explanation_index )
{
    TCHAR facility_effect_name[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR facility_effect_explanation[INTERFACE_STRING_LENGTH] = {0, };

    // 이름
    g_InterfaceManager.GetInterfaceString(facility_name_index, facility_effect_name);
    // 설명
    g_InterfaceManager.GetInterfaceString(facility_explanation_index, facility_effect_explanation);

    tooltip_manager_ptr_->InitTooltip();

    // 효과 이름
    tooltip_manager_ptr_->RegisterTooltip(facility_effect_name, 
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(217, 117, 0, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

    tooltip_manager_ptr_->AddEmptyLineForTooltip();

    // 효과 설명
    tooltip_manager_ptr_->RegisterTooltip(facility_effect_explanation,
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(255, 255, 255, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
}

void uiGuildInstitution::ProcessResultTooltip( const int& facility_name_index, const int& facility_explanation_index, const int& facility_order, const FACILITY_TOOLTIP_TYPE& type )
{
    TCHAR facility_effect_name[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR facility_effect_explanation[INTERFACE_STRING_LENGTH] = {0, };

    // 이름
    g_InterfaceManager.GetInterfaceString(facility_name_index, facility_effect_name);
    // 설명
    g_InterfaceManager.GetInterfaceString(facility_explanation_index, facility_effect_explanation);

    tooltip_manager_ptr_->InitTooltip();

    // 효과 이름
    tooltip_manager_ptr_->RegisterTooltip(facility_effect_name, 
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(217, 117, 0, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

    tooltip_manager_ptr_->AddEmptyLineForTooltip();

    // 효과 설명
    tooltip_manager_ptr_->RegisterTooltip(facility_effect_explanation,
        -1,
        22,
        StrToWzID("s210"),
        WzColor_RGBA(255, 255, 255, 255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

    BASE_GuildFacilityImageInfo* facility_image_info 
        = GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(facility_order);
    if (facility_image_info == NULL)
        return;

    GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;    
    if (!(GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)facility_image_info->index_, facility_info)))
        return;

    int current_facility_level = facility_info.facility_level_;

    tooltip_manager_ptr_->AddEmptyLineForTooltip();

    TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR guild_point_value[INTERFACE_STRING_LENGTH] = {0, };    
    if (type == TOOLTIP_TYPE_UPGRADE)
    {
        const FACILITY_INFO* facility_parser_info = 
            GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo((FCODE)facility_image_info->index_, (FLEVEL)current_facility_level + 1);
        if (facility_parser_info == NULL)
            return;
        _itot(facility_parser_info->upgrade_guild_point, guild_point_value, 10);
        TCHAR condition_string[INTERFACE_STRING_LENGTH] = {0, };

        // 7119 : 업그레이드 필요조건
        g_InterfaceManager.GetInterfaceString(7119, condition_string);
        tooltip_manager_ptr_->RegisterTooltip(condition_string,
            -1,
            22,
            StrToWzID("s210"),
            WzColor_RGBA(217, 117, 0, 255),
            TOOLTIP_BG_WZCOLOR,
            c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

        int current_guild_point = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGP;
        switch (facility_parser_info->upgrade_type)
        {
        case FUI_ONLY_GUILDPOINT:
            {                
                // 7120 : 길드포인트 : %s
                g_InterfaceManager.GetInterfaceString(7120, format_string);                
                Snprintf(condition_string, INTERFACE_STRING_LENGTH, format_string, guild_point_value);
                tooltip_manager_ptr_->RegisterTooltip(condition_string,
                    -1,
                    22,
                    StrToWzID("s210"),
                    //WzColor_RGBA(255, 255, 255, 255),
                    current_guild_point >= (int)facility_parser_info->upgrade_guild_point ? WzColor_RGBA(0, 255, 0, 255) : WzColor_RGBA(128, 128, 128, 100),
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            }
            break;
        case FUI_ONLY_ITEM:
            {
                TCHAR item_name[MAX_ITEMNAME_LENGTH] = {0, };
                TCHAR num_string[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetItemName(facility_parser_info->upgrade_item_code, item_name);
                // 7121 : 필요아이템 : %s(%s)
                g_InterfaceManager.GetInterfaceString(7121, format_string);
                Snprintf(num_string, INTERFACE_STRING_LENGTH, "%d/%d", HaveItemNum(facility_parser_info->upgrade_item_code), facility_parser_info->upgrade_item_num);
                Snprintf(condition_string, INTERFACE_STRING_LENGTH, format_string, item_name, num_string);
                tooltip_manager_ptr_->RegisterTooltip(condition_string,
                    -1,
                    22,
                    StrToWzID("s210"),                    
                    HaveItemNum(facility_parser_info->upgrade_item_code) >= facility_parser_info->upgrade_item_num ? WzColor_RGBA(0, 255, 0, 255) : WzColor_RGBA(128, 128, 128, 100),
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            }
            break;
        case FUI_GUILDPOINTANDITEM:
            {
                // 7120 : 길드포인트 : %s
                g_InterfaceManager.GetInterfaceString(7120, format_string);                
                Snprintf(condition_string, INTERFACE_STRING_LENGTH, format_string, guild_point_value);
                tooltip_manager_ptr_->RegisterTooltip(condition_string,
                    -1,
                    22,
                    StrToWzID("s210"),                    
                    current_guild_point >= (int)facility_parser_info->upgrade_guild_point ? WzColor_RGBA(0, 255, 0, 255) : WzColor_RGBA(128, 128, 128, 100),
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

                TCHAR item_name[MAX_ITEMNAME_LENGTH] = {0, };
                TCHAR num_string[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetItemName(facility_parser_info->upgrade_item_code, item_name);
                // 7121 : 필요아이템 : %s(%s)
                g_InterfaceManager.GetInterfaceString(7121, format_string);
                Snprintf(num_string, INTERFACE_STRING_LENGTH, "%d/%d", HaveItemNum(facility_parser_info->upgrade_item_code), facility_parser_info->upgrade_item_num);
                Snprintf(condition_string, INTERFACE_STRING_LENGTH, format_string, item_name, num_string);
                tooltip_manager_ptr_->RegisterTooltip(condition_string,
                    -1,
                    22,
                    StrToWzID("s210"),                    
                    HaveItemNum(facility_parser_info->upgrade_item_code) >= facility_parser_info->upgrade_item_num ? WzColor_RGBA(0, 255, 0, 255) : WzColor_RGBA(128, 128, 128, 100),
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            }
            break;
        }
    }
    else if (type == TOOLTIP_TYPE_ACTIVE)
    {
        BASE_GuildFacilityImageInfo* facility_image_info 
            = GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(facility_order);
        if (facility_image_info == NULL)
            return;

        GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;
        if (!(GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)facility_image_info->index_, facility_info)))
            return;

        int current_facility_level = facility_info.facility_level_;
        if (current_facility_level <= 0)
            current_facility_level = GetCurrentFacilityActiveLevel(facility_info.facility_code_);
        
        const FACILITY_INFO* facility_parser_info = 
            GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo((FCODE)facility_image_info->index_, (FLEVEL)current_facility_level);
        TCHAR condition_string[INTERFACE_STRING_LENGTH] = {0, };
        if (facility_parser_info == NULL)
            return;

        TCHAR num_string[INTERFACE_STRING_LENGTH] = {0, };
        FCODE current_active_level = GetCurrentFacilityActiveLevel((FCODE)facility_parser_info->facility_code);
        if (current_active_level > 0)
        {
            TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
            // 7126 : 길드시설 %s레벨이 활성화 중입니다.
            g_InterfaceManager.GetInterfaceString(7126, format_string);
            Snprintf(num_string, INTERFACE_STRING_LENGTH, "%d", current_active_level);
            Snprintf(condition_string, INTERFACE_STRING_LENGTH, format_string, num_string);
            tooltip_manager_ptr_->RegisterTooltip(condition_string,
                -1,
                22,
                StrToWzID("s210"),
                WzColor_RGBA(0, 255, 0, 255),
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
        }
        else
        {
            // 7122 : 활성화 필요조건
            g_InterfaceManager.GetInterfaceString(7122, condition_string);
            tooltip_manager_ptr_->RegisterTooltip(condition_string,
                -1,
                22,
                StrToWzID("s210"),
                WzColor_RGBA(217, 117, 0, 255),
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

            TCHAR item_name[MAX_ITEMNAME_LENGTH] = {0, };
            g_InterfaceManager.GetItemName(facility_parser_info->active_option.active_item_code, item_name);
            // 7121 : 필요아이템 : %s(%s)
            g_InterfaceManager.GetInterfaceString(7121, format_string);
            Snprintf(num_string, INTERFACE_STRING_LENGTH, "%d/%d", 
                HaveItemNum(facility_parser_info->active_option.active_item_code), 
                facility_parser_info->active_option.active_item_count);
            Snprintf(condition_string, INTERFACE_STRING_LENGTH, format_string, item_name, num_string);
            tooltip_manager_ptr_->RegisterTooltip(condition_string,
                -1,
                22,
                StrToWzID("s210"),
                HaveItemNum(facility_parser_info->active_option.active_item_code) >= 
                facility_parser_info->active_option.active_item_count ? WzColor_RGBA(0, 255, 0, 255) : WzColor_RGBA(128, 128, 128, 100),
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
        }
    }
}

bool uiGuildInstitution::OnMouseOver( SI_MESSAGE* message )
{
    switch( message->eResultMsg ) 
    {
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE pos = getCtrlIDToPos( message->dwCtrlID );

            if ( pos >= kControl_CheckBT_C000 && pos < kControl_Size )
            {
                mouse_over_pos_ = pos;
            }
            else
            {
                mouse_over_pos_ = kControl_Size;
            }
        }
        break;
    }

    return true;
}

void uiGuildInstitution::AskFacilityUpgrade(const DWORD& facility_code)
{
    uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager_ptr)
    {
        TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
        // 7108 : 해당 시설을 업그레이드 하시겠습니까?
        g_InterfaceManager.GetInterfaceString(7108, result_message);        
        ui_system_manager_ptr->Sys_SystemMessage(result_message, 
            NULL, 
            GuildSystem::Function::Callback_GuildFacilityUpgrade,
            uiSystemMessage::kConfirm_Type,
            true,
            0,
            100,
            155536,
            155537,
            facility_code
            );
    }
}

void uiGuildInstitution::AskFacilityActive( const DWORD& facility_code, const DWORD& facility_level )
{
    const FACILITY_INFO* facility_parser_info = 
        GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo((FCODE)facility_code, (FLEVEL)facility_level);
    if (facility_parser_info == NULL)
        return;

    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(facility_parser_info->active_option.active_item_code);
    if (item_info == NULL)
        return;

    uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager_ptr)
    {
        TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR item_name[INTERFACE_STRING_LENGTH] = {0, };
        // 7112 : 해당 시설을 활성화 하기 위해서는 %s 아이템이 필요합니다.\n시설 활성화를 진행 하시겠습니까?
        g_InterfaceManager.GetInterfaceString(7112, format_string);
        g_InterfaceManager.GetItemName( facility_parser_info->active_option.active_item_code, item_name, INTERFACE_STRING_LENGTH );
        Snprintf(result_message, INTERFACE_STRING_LENGTH, format_string, item_name);
        ui_system_manager_ptr->Sys_SystemMessage(result_message, 
            NULL, 
            GuildSystem::Function::Callback_GuildFacilityActive,
            uiSystemMessage::kConfirm_Type,
            true,
            0,
            100,
            155536,
            155537,
            facility_code
            );
    }
}

bool uiGuildInstitution::HaveItem( const CODETYPE& item_code )
{
    SCSlotContainer* container_ptr = g_pHero->GetSlotContainer(SI_INVENTORY);
    POSTYPE start = 0;
    POSTYPE total = 0;
    if (container_ptr)
    {
        total = container_ptr->GetMaxSlotNum();
    }

    BOOL	bVal = FALSE;

    for (POSTYPE i = start; i < total; ++i)
    {
        if ( !container_ptr->IsEmpty(i) )
        {
            SCSlot& slot = container_ptr->GetSlot(i);
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo( slot.GetCode() );
            if(item_info)
            {
                if(item_info->m_Code == item_code)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void uiGuildInstitution::UpdateAll()
{
    UpdateInstitutionInfo();
    UpdateButtonState();
    UpdateFacilityListInfo();
}

DWORD uiGuildInstitution::HaveItemNum( const CODETYPE& item_code )
{
    DWORD item_num = 0;
    SCSlotContainer* container_ptr = g_pHero->GetSlotContainer(SI_INVENTORY);
    POSTYPE start = 0;
    POSTYPE total = 0;
    if (container_ptr)
    {
        total = container_ptr->GetMaxSlotNum();
    }    

    for (POSTYPE i = start; i < total; ++i)
    {
        if ( !container_ptr->IsEmpty(i) )
        {
            SCSlot& slot = container_ptr->GetSlot(i);
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo( slot.GetCode() );
            if(item_info)
            {
                if(item_info->m_Code == item_code)
                {
                    item_num += slot.GetNum();                    
                }
            }
        }
    }

    return item_num;
}

void uiGuildInstitution::UpdateActiveButtonState()
{
    int scroll_position = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control == NULL)
    {
        return;
    }
    scroll_position = scroll_control->GetScrollPosWZ();

    for (DWORD index = 0; index < kListNumber; ++index)
    {
        // 활성화 버튼
        CCtrlButtonWZ* control_ptr = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B000 + index));
        if (control_ptr)
        {
            // 2305 : 활성화
            control_ptr->SetTextID(2305);
            control_ptr->SetDiscardMessage(false);
            GlobalFunc::EnableControl(control_ptr, true);

            BASE_GuildFacilityImageInfo* info_ptr = 
                GuildFacilityImageParser::Instance()->GetGuildFacilityImageInfo(scroll_position + index + 1);

            GuildSystem::GuildSystemData::GuildFacilityInfoForClient facility_info;
            if (info_ptr && 
                GuildSystem::GetGuildSystemData().get_guild_facility_info((FCODE)info_ptr->index_, facility_info))
            {
                bool zero_level_and_active = false;
                FLEVEL current_active_level = GetCurrentFacilityActiveLevel((FCODE)info_ptr->index_);
                if (current_active_level > 0 && facility_info.facility_level_ <= 0)
                {
                    zero_level_and_active = true;
                }
                const FACILITY_INFO* facility_parser_info = NULL;
                if (zero_level_and_active)
                {
                    facility_parser_info = GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(facility_info.facility_code_, current_active_level);
                }
                else
                {
                    facility_parser_info = GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(facility_info.facility_code_, facility_info.facility_level_);
                }
                 
                if (facility_parser_info == NULL)
                {
                    GlobalFunc::EnableControl(control_ptr, false);
                    continue;
                }

                SkillScriptInfo* skill_info_ptr = 
                    SkillInfoParser::Instance()->GetSkillInfo(facility_parser_info->active_option.active_skill_code);
                if (skill_info_ptr == NULL)
                {
                    GlobalFunc::EnableControl(control_ptr, false);
                    continue;
                }

                BASE_ABILITYINFO* ability_info = NULL;
                ability_info = skill_info_ptr->GetAbilityInfoByIndex(0);
                if (ability_info == NULL)
                {
                    GlobalFunc::EnableControl(control_ptr, false);
                    continue;
                }

                // 일단 현재 활성화 중인 상태에 대한 체크로 시간을 셋팅한다.
                SKILL_EFFECT* skill_effect_ptr = g_pHero->FindSkillEffect((eCHAR_STATE_TYPE)ability_info->m_wStateID);
                if (skill_effect_ptr)
                {
                    GuildSystem::GetGuildSystemData().set_element_guild_facility_active(facility_info.facility_code_, true);
                    TCHAR time_string[INTERFACE_STRING_LENGTH] = {0, };
                    if(skill_effect_ptr->iRemainTime > 0)
                    {
                        int sec, min, hr;

                        sec = skill_effect_ptr->iRemainTime / 1000;
                        min = sec / 60;
                        sec = sec % 60;

                        hr = min / 60;
                        min = min % 60;
                        
                        Snprintf(time_string, INTERFACE_STRING_LENGTH, "%02d:%02d:%02d", hr, min, sec);
                        control_ptr->SetTextWZ(time_string);
                    }
                    else
                    {
                        Snprintf(time_string, INTERFACE_STRING_LENGTH, "%02d:%02d:%02d", 0, 0, 0);
                        control_ptr->SetTextWZ(time_string);
                    }
                    //GlobalFunc::EnableControl(control_ptr, true);
                    control_ptr->SetDiscardMessage(true);
                }
                else if ( facility_info.facility_level_ > 0)
                {
                    GuildSystem::GetGuildSystemData().set_element_guild_facility_active(facility_info.facility_code_, false);                    
                }
                else
                {
                    GuildSystem::GetGuildSystemData().set_element_guild_facility_active(facility_info.facility_code_, false);                    
                    GlobalFunc::EnableControl(control_ptr, false);
                }
            }
            else
            {
                GlobalFunc::EnableControl(control_ptr, false);                
            }
        }
    }
}

FLEVEL uiGuildInstitution::GetCurrentFacilityActiveLevel(const FCODE& facility_index)
{
    FLEVEL current_active_level = 0;
    for (BYTE active_level = 1; active_level <= 5; ++active_level)
    {
        const FACILITY_INFO* facility_parser_info = GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(facility_index, active_level);
        if (facility_parser_info == NULL)
            continue;

        SkillScriptInfo* skill_info_ptr = 
            SkillInfoParser::Instance()->GetSkillInfo(facility_parser_info->active_option.active_skill_code);
        if (skill_info_ptr == NULL)
            continue;

        BASE_ABILITYINFO* ability_info = NULL;
        ability_info = skill_info_ptr->GetAbilityInfoByIndex(0);
        if (ability_info == NULL)
            continue;

        SKILL_EFFECT* skill_effect_ptr = g_pHero->FindSkillEffect((eCHAR_STATE_TYPE)ability_info->m_wStateID);
        if (skill_effect_ptr == NULL)
            continue;
        if (skill_effect_ptr->dwSkillID == facility_parser_info->active_option.active_skill_code)
        {
            current_active_level = active_level;
            break;
        }
    }

    return current_active_level;
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL