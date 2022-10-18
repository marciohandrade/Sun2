#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatusTitleList.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "HonorSystem/HonorSystem.h"
#include "TitleListParser.h"
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "InterfaceManager.h"
#include "Hero.h"
#include "GlobalFunc.h"

const DWORD uiCharacterStatusTitleList::kDialog_ID = StrToWzID("0405");

uiCharacterStatusTitleList::uiCharacterStatusTitleList(InterfaceManager* interface_manager)
:uiBase(interface_manager),
confirm_title_code_(0),
confirm_dialog_key_(0)
{

}

uiCharacterStatusTitleList::~uiCharacterStatusTitleList()
{

}

void uiCharacterStatusTitleList::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);

    title_list_menu().Release();

    TCHAR no_title_name[INTERFACE_STRING_LENGTH] = {'\0', };
    g_InterfaceManager.GetInterfaceString(HonorSystem::kNoTitleMenuStringCode, no_title_name);
    GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(no_title_name);
}

void uiCharacterStatusTitleList::Release()
{
    title_list_menu().Release();
}

void uiCharacterStatusTitleList::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        InitTitleListInfo();

        // 키큐등록
        keyMsg key_message;
        ZeroMemory(&key_message, sizeof(key_message));
        key_message.dwType = eDoType_HonorTitleList_Dialog;
        key_message.wParam = uiCharacterStatusTitleList::kDialog_ID;
        key_message.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(key_message);
    }
    else
    {
        OnClose();
        // 키큐해제
        g_KeyQueueManager.DeleteMsg(eDoType_HonorTitleList_Dialog);
    }
}

void uiCharacterStatusTitleList::OnUpdateSolarDialog()
{
    title_list_menu().UpdateMouseWheel();
}

void uiCharacterStatusTitleList::MessageProc(SI_MESSAGE* message)
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
    if (title_list_menu().MessageProc(message, OUT tree_object) == true)
    {
        if (tree_object != NULL)
        {
            DWORD title_code = g_pHero->GetTitleCode();
            if (title_code == HonorSystem::kNoTitleCode)
            {
                title_code = HonorSystem::kNoTitleMenuIndex;
            }
            TreeMenuGroup::TreeObject* menu_info = title_list_menu().Find(title_code);
            switch (message->eResultMsg)
            {
            case RT_MSG_LBUTTONCLICK:
                {
                    tree_object->is_check = false;
                    if (menu_info != NULL)
                    {
                        menu_info->is_check = true;
                    }
                }
                break;
            case RT_MSG_LBUTTONDCLICK:
                {
                    OnDClickToUseTitle(static_cast<WORD>(tree_object->info->menu_index));
                    tree_object->is_check = false;
                    if (menu_info != NULL)
                    {
                        menu_info->is_check = true;
                    }
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

bool uiCharacterStatusTitleList::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004:
        {
            // 닫기 버튼
            ShowInterfaceWithSound(FALSE);
        }
        break;
    default:
        {
        }
    }
    return result;
}

void uiCharacterStatusTitleList::InitTitleListInfo()
{
    // 리스트 메뉴 해제
    title_list_menu().Release();

    if (g_pHero == NULL)
    {
        return;
    }

    // 칭호 리스트 메뉴 정보 갱신
    HonorSystem::HonorSystemFunction::RefreshTitleListMenu();

    // 칭호 리스트 메뉴 초기화
    title_list_menu().Init(BASE_TreeMenuInfo::kGroupType_Title_List, 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000)), 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000)), 
        false, 
        uiCharacterStatusTitleList::Callback_TitleListMenuRender);


    // 리스트컨트롤에 칭호 리스트 채우기
    title_list_menu().Fill();

    // 현재 사용중인 칭호 선택/체크되어 있도록
    TreeMenuGroup::TreeObject* current_menu_info = NULL;
    DWORD current_title_code = g_pHero->GetTitleCode();
    if (current_title_code == HonorSystem::kNoTitleCode)
    {
        current_menu_info = title_list_menu().Find(HonorSystem::kNoTitleMenuIndex);
    }
    else
    {
        current_menu_info = title_list_menu().Find(current_title_code);
    }
    
    if (current_menu_info != NULL)
    {
        title_list_menu().Select(current_menu_info);
        current_menu_info->is_check = true;
        
        TCHAR title_name[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(current_menu_info->info->string_code, title_name, INTERFACE_STRING_LENGTH - 1);
        GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(title_name);
    }
}

void uiCharacterStatusTitleList::OnChangeTitle(WORD title_code)
{
    if ((IsVisible() == FALSE) || 
        (g_pHero == NULL))
    {
        return;
    }
    
    TreeMenuGroup::TreeObject* menu_info = title_list_menu().Find(prev_title_code_);
    if (menu_info != NULL)
    {
        menu_info->is_check = false;
    }

    // 새로 선택된 칭호
    if (title_code == HonorSystem::kNoTitleCode)
    {
        title_code = HonorSystem::kNoTitleMenuIndex;
    }
    menu_info = title_list_menu().Find(title_code);
    if (menu_info != NULL)
    {
        title_list_menu().Select(menu_info);
        menu_info->is_check = true;

        TCHAR title_name[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(menu_info->info->string_code, title_name, INTERFACE_STRING_LENGTH - 1);
        GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(title_name);
    }

    prev_title_code_ = title_code;
}

void uiCharacterStatusTitleList::OnDClickToUseTitle(WORD title_code)
{
    if (g_pHero == NULL)
    {
        return;
    }

    if (confirm_dialog_key_ != 0)
    {
        return;
    }

    if (title_code == HonorSystem::kNoTitleMenuIndex)
    {
        title_code = HonorSystem::kNoTitleCode;
    }

    WORD current_title_code = g_pHero->GetTitleCode();
    if (current_title_code == title_code)
    {
        return;
    }

    prev_title_code_ = current_title_code;
    if (prev_title_code_ == HonorSystem::kNoTitleCode)
    {
        prev_title_code_ = HonorSystem::kNoTitleMenuIndex;
    }

    uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if ((GENERALPARAM->IsNet() == TRUE) && 
        (system_manager != NULL))
    {
        // 팝업/콜백을 통한 패킷 보내기
        TCHAR confirm_message[INTERFACE_STRING_LENGTH] = {'\0', };
        
        if (title_code == HonorSystem::kNoTitleCode)
        {
            g_InterfaceManager.GetInterfaceString(6766, confirm_message, INTERFACE_STRING_LENGTH - 1);
        }
        else
        {
            TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(title_code);
            if (title_info != NULL)
            {
                TCHAR title_name[INTERFACE_STRING_LENGTH] = {'\0', };

                g_InterfaceManager.GetInterfaceString(title_info->title_name_code, title_name, INTERFACE_STRING_LENGTH - 1);
                g_InterfaceManager.GetInterfaceStringFormat(confirm_message, INTERFACE_STRING_LENGTH - 1, 6765, title_name);
            }
        }

        confirm_title_code_ = title_code;
        confirm_dialog_key_ = system_manager->Sys_Confirm(confirm_message, 
            uiCharacterStatusTitleList::CallBack_ConfirmUseTitle, 
            NULL, 
            NULL, 
            100, 
            101, 
            true);
    }
    else
    {
        HonorSystem::HonorSystemFunction::SetTitleName(g_pHero->GetObjectKey(), title_code);
        HonorSystem::HonorSystemFunction::OnChangeTitle(title_code);
    }
    
}

void uiCharacterStatusTitleList::OnClose()
{
    // 종료 처리 - 팝업창 닫기 등
    uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (system_manager != NULL)
    {
        system_manager->Destroy_Sys_Confirm(confirm_dialog_key_);
    }

    confirm_title_code_ = 0;
    confirm_dialog_key_ = 0;
}

void uiCharacterStatusTitleList::Callback_TitleListMenuRender(CCtrlListWZ* list_control, CCtrlListWZ::CListSellData* sell_data, int sell_index, RECT& render_area)
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
    
    // 선택되어 있는 칭호에 대한 선택이미지 출력
    bool is_check = tree_object->is_check;
    if (is_check == true)
    {
        DWORD checkon_image_code = tree_object->info->image_code[BASE_TreeMenuInfo::kIconType_CheckOn];

        BASE_TextureListInfo* tex_info = NULL;
        HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(checkon_image_code, tex_info);;

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

    //! 글자
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
        g_pSunRenderer->x_pManagerTextOut->DrawText(sell_data->stdStrText.c_str(),
            &render_area,
            font_color,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HCENTER | TP_VCENTER);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

void uiCharacterStatusTitleList::CallBack_ConfirmUseTitle(bool is_yes)
{
    uiCharacterStatusTitleList* title_list = GET_CAST_DIALOG(uiCharacterStatusTitleList, uiCharacterStatusTitleList::kDialog_ID);
    if (title_list == NULL)
    {
        return;
    }

    if (is_yes == true)
    {
        HonorSystem::HonorSystemFunction::UseTitleNameForHero(title_list->confirm_title_code());
    }

    title_list->reset_confirm_dialog_key();
}

#endif //_NA_003027_20111013_HONOR_SYSTEM