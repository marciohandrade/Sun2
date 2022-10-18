#include "SunClientPrecompiledHeader.h"
#ifdef _DEV_VER
#include "uiGMItemWindow/uiGMItemWindow.h"
#include "uiGMWindowMan/uiGMWindowMan.h"
#include "uiGMItemOptionWindow/uiGMItemOptionWindow.h"
#include "uiGMItemResetWindow/uiGMItemResetWindow.h"
#include "FTextSentence.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include <SCItemSlot.h>
#include <SCSlotContainer.h>
#include "ItemUnitRenderer.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"
#include "Const.h"
#include "ItemTypeList.h"
#include "AwakeningInfoParser.h"

const long kClientHeight = 518;
const DWORD item_info_font = StrToWzID("mn12");
const DWORD item_text_color[uiGMItemWindow::KMax_Text_Color] = {
    WzColor_RGBA(200, 200, 168, 255), 
    WzColor_RGBA(255, 255, 255, 255), 
    WzColor_RGBA(150, 150, 150, 255),
    WzColor_RGBA(128, 128, 128, 255)};
const DWORD item_text_background_color = WzColor_RGBA(255, 255, 255, 128);
const DWORD item_select_background_color = WzColor_RGBA(255, 255, 255, 0);
const DWORD item_select_text_color = WzColor_RGBA(255, 255, 0, 255);// argb
const DWORD item_select_bar_color = WzColor_RGBA(255, 255, 192, 0);// argb

const int state_string_code[uiGMItemWindow::kFind_Type_MAX] = {9958,//이름
    412,//진행
    54,//타입
    155013};//소모성

class uiSCItemSlot;
void uiGMItemWindow::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00);
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    RegisterControlPosition(StrToWzID("B032"), kControl_Button_B032);
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("B033"), kControl_Button_B033);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_B030);
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031);
    RegisterControlPosition(StrToWzID("T201"), kControl_Text_T201);
    RegisterControlPosition(StrToWzID("ST01"), kControl_Text_ST01);
    RegisterControlPosition(StrToWzID("ST02"), kControl_Text_ST02);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("LS00"), kControl_List_LS00);
    RegisterControlPosition(StrToWzID("LS01"), kControl_List_LS01);
    RegisterControlPosition(StrToWzID("VS00"), kControl_VScroll_VS00);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
}

uiGMItemWindow::uiGMItemWindow(InterfaceManager* ui_manager):
uiBase(ui_manager),
search_gauge_control_size_(0.0f),
search_gauge_texture_size_(0.0f),
item_unit_renderer_(NULL),
search_type_(kFind_From_Name)
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_	= INVALID_HANDLE_VALUE;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

uiGMItemWindow::~uiGMItemWindow(void)
{
    this->Release();

    SAFE_RELEASENDELETE(item_unit_renderer_);
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();
    }
}

void uiGMItemWindow::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    // 슬롯 초기화
    SCItemSlotContainer::Init(kSlotTotalCount, SI_GM_ITEM);

    // 랜더러 초기화
    assert(!item_unit_renderer_);
    item_unit_renderer_ = new ItemUnitRenderer;
    item_unit_renderer_->Init(uiBase::GetDrawBase(), kSlotTotalCount);
    
    item_code_ = 0;
    search_new_ = true;
    set_search_end(true);
    set_search_string("\n");// 입력이 불가능한 문자열을 초기화 해야 검색시 중복되지 않음
    sprintf(pre_find_word_,"\n");
    item_name_info_iterator_ = ItemInfoParser::Instance()->GetItemNamesBegin();
    item_name_info_iterator_end_ = ItemInfoParser::Instance()->GetItemNamesEnd();
    search_index_ = 0;
    search_frame_per_update_time_ = PROGRAMPARAM->search_frame_per_update_time();
    number_of_searching_per_a_frame_ = PROGRAMPARAM->number_of_searching_per_a_frame();
    total_searched_count_ = 0;

    history_scroll_pos_ = 0;
    search_scroll_pos_ = 0;
    bookmark_scroll_pos_ = 0;

    last_make_item_code_ = 0;
    selected_class_ = eCHAR_NONE;

    ui_gm_window_manager_ = static_cast<uiGMWindowMan*>(GetMan());
    RegisterControl();
    
    if (ui_gm_window_manager_)
    {
        searched_results_ = ui_gm_window_manager_->searched_results();
        created_histroys_ = ui_gm_window_manager_->created_histroys();
        bookmark_lists_ = ui_gm_window_manager_->bookmark_lists();
    }

    item_info_text_render_.resize(kDisplay_Item_Info_Num);

    CControlWZ* gauge_control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    if (gauge_control != NULL)
    {
        search_gauge_control_size_ = static_cast<float>(gauge_control->GetSize().right);
        search_gauge_texture_size_ = gauge_control->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth;
    }   
    
    // 컨트롤 Draw Callback 함수 세팅
    const POSTYPE info_control_pos = kControl_Button_B000;
    CControlWZ* item_info_control = NULL;
    for (int index_display = 0; index_display < kDisplay_Item_Info_Num; ++index_display)
    {
        item_info_control = GetControlWZ(GetControlID_byPosition(info_control_pos + index_display));
        if (item_info_control == NULL)
        {
            continue;
        }
        item_info_control->SetCallbackDraw(uiGMItemWindow::CallBackRenderItemInfo, (void*)index_display);
    }
    
    BYTE control_positions[] = {kControl_List_LS01,
        /*kControl_Button_B001, kControl_Button_B021, kControl_Button_B022, kControl_Button_B023,
        kControl_Button_B024, kControl_Button_B025,*/ kControl_Button_B026, /*kControl_Button_B027,
        kControl_Button_B028,*/ kControl_Button_B030, 255};

    BYTE number = 0;
    CControlWZ* hide_control = NULL;
    while (control_positions[number] != 255)
    {
        hide_control = GetControlWZ(GetControlID_byPosition(control_positions[number]));
        if (hide_control)
        {
            hide_control->ShowWindowWZ(WZ_HIDE);
        }
        ++number;
    }

    gauge_control = GetControlWZ(GetControlID_byPosition(kControl_Text_T999));
    if (gauge_control)
    {
        gauge_control->SetTextWZ("GM 아이템 윈도우");
    }

    static_state_control_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_T001)));
   /*if (static_state_control_)
    {
        static_state_control_->SetTextWZ("아이템명을 입력하세요");
    }*/

    RECT rect = { 0, 0, 0, 0 };
    gauge_control = GetControlWZ(GetControlID_byPosition(kControl_Text_ST02));
    if (gauge_control)
    {
        rect = gauge_control->GetSize();
    }

    history_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B029)));
    if (history_button_)
    {
        float width = history_button_->GetSizeWidth() - 25;
        float height = history_button_->GetSizeHeight() - 6;
        history_button_->MoveWindowWZ((float)(rect.left - width - 12 - 55), (float)(rect.top + rect.bottom - height + 2), width, height);
    }

    pickup_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B028)));
    if (pickup_button_)
    {
        // 2013.03.26 줍기 버튼 위치 수정(주석이되어져있는 것은 원래 소스)-홍성현
        //float width = pickup_button_->GetSizeWidth() - 56;
        float width = pickup_button_->GetSizeWidth() - 50;
        float height = pickup_button_->GetSizeHeight() - 6;
        pickup_button_->SetTextWZ("줍기");
        //pickup_button_->MoveWindowWZ((float)(rect.left+78), (float)(rect.top + 40), width, height);
        pickup_button_->MoveWindowWZ((float)(rect.left-140), (float)(rect.top), width, height);
    }

    reset_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B027)));
    if (reset_button_)
    {
        float width = reset_button_->GetSizeWidth() - 25;
        float height = reset_button_->GetSizeHeight() - 6;
        reset_button_->SetTextWZ("필터해제");
        reset_button_->MoveWindowWZ((float)(rect.left - width - 12), (float)(rect.top + rect.bottom - height + 2), width, height);
    }

    gauge_control = GetControlWZ(GetControlID_byPosition(kControl_Button_B031));
    if (gauge_control)
    {
        float width = gauge_control->GetSizeWidth() - 50;
        float height = gauge_control->GetSizeHeight() - 6;
        gauge_control->MoveWindowWZ((float)(rect.left + rect.right), (float)(rect.top + rect.bottom - height + 2), width, height);
        gauge_control->SetTextWZ("옵션");
    }

    button_control_ = (CCtrlButtonWZ*)GetControlWZ(GetControlID_byPosition(kControl_Button_BT00));
    if (button_control_)
    {
        ChangeFindIconImage(button_control_, false);
    }

    CCtrlButtonWZ* class_control = NULL;

    for (int i = eCHAR_BERSERKER; i < kTempClassTypeMax; ++i)
    {
        class_control = static_cast<CCtrlButtonWZ*>
            (GetControlWZ(GetControlID_byPosition(kControl_Button_B001+i-eCHAR_BERSERKER)));
        if (class_control)
        {
            ChangeClassIcon(class_control, i, true);
            //MoveClassIcon(class_control, i);
        }
    }

    static_edit_control_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_T201)));
    {
        static_edit_control_->SetTextWZA("검색어를 입력하세요");
        static_edit_control_->SetTextColor(RGBA(255,255,255,255));
    }
    assert(static_edit_control_);

    //this->GetDialogWZ()->CreateControllWZ(StrToWzID("E002"), 컨트롤 생성    
    edit_control_ = static_cast<CCtrlDxEditBox*>(GetControlWZ(GetControlID_byPosition(kControl_Edit_E000)));
    if (edit_control_)
    {
        edit_control_->SetFontID(gm_win_font);
        edit_control_->SetColorText(WzColor_RGBA(255,255,255,255));
        edit_control_->SetColorCaret(WzColor_RGBA(255,255,255,255));
        edit_control_->SetColorSelBack(WzColor_RGBA(255,50,50,255));
        edit_control_->SetColorSelText(WzColor_RGBA(255,255,255,255));
    }
    assert(edit_control_);

    find_list_ptr_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_LS00)));
    if (find_list_ptr_)
    {
        find_list_ptr_->ShowWindowWZ(WZ_HIDE);
    }
    assert(find_list_ptr_);

    CCtrlListWZ* state_select_list = 
        static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_LS01)));
    if (state_select_list != NULL)
    {
        TCHAR state_string[INTERFACE_STRING_LENGTH] = {0, };
        int list_item_index = 0;
        for (int index = 0; index < kFind_Type_MAX;++index)
        {
            list_item_index = state_select_list->InsertItemBack();
            g_InterfaceManager.GetInterfaceString(state_string_code[index], state_string);
            GlobalFunc::SetItem(state_select_list, list_item_index, state_string, item_info_font, 
                DT_CENTER | DT_VCENTER, item_select_background_color, 
                item_text_color[kComplete_Text_Color], item_select_text_color);
        }
        state_select_list->SetColorSelBar(item_select_bar_color);
        state_select_list->SetColorSelBarOnText(item_select_text_color);
        state_select_list->SetStartLineGapTop(4);
        state_select_list->SetSellHeight(18);
        state_select_list->SetCurSel(0);
    }

    seacrch_type_text_control_ = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_ST01)));
    if (seacrch_type_text_control_ != NULL)
    {
        TCHAR state_string[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(state_string_code[kFind_From_Name], state_string);
        seacrch_type_text_control_->SetTextWZ(state_string);
    }

    // 스크롤상태 초기화
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));
    if (scroll_control != NULL)
    {
        scroll_control->SetScrollPosWZ(0);
    }

    // 달성률 출력
    gauge_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));
    if (gauge_control != NULL)
    {
        gauge_control->SetTextWZ("100.0%% (0)");
        RECT size_rect = gauge_control->GetSize();
        size_rect.left = size_rect.left - 25;
        size_rect.right = size_rect.right + 50;
        gauge_control->MoveWindowWZ((float)size_rect.left, (float)size_rect.top, 
            (float)size_rect.right, (float)size_rect.bottom);
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_);
    assert(texture_information_);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    LoadCreateHistory();    // 생성 히스토리 로드
}

void uiGMItemWindow::ShowClassIcon(bool is_show)
{
    CCtrlButtonWZ* class_control = NULL;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    for (int i = kControl_Button_B001; i <= kControl_Button_B033; ++i)
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    for (int i = kControl_Button_B001; i <= kControl_Button_B032; ++i)
  #else
    for (int i = kControl_Button_B001; i <= kControl_Button_B025; ++i)
  #endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    {
        class_control = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(i)));
        if (class_control)
        {
            class_control->ShowWindowWZ((is_show)?WZ_SHOW:WZ_HIDE);
        }
    }
}

void uiGMItemWindow::LoadCreateHistory()
{
    SetItemWindowState(kState_History);
    size_t size = created_histroys_->size();
    RefreshList(kState_History, size, size);
}

void uiGMItemWindow::RefreshScroll(WORD item_index)
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));
    if (scroll_control == NULL)
    {
        return;
    }

    int scroll_min = 0; 
    int scroll_max = 0;
    int info_list_num = static_cast<int>(GetFocusList()->size());
    int current_scroll_pos = scroll_control->GetScrollPosWZ();
    //if (item_index != 0)
    {
        current_scroll_pos = item_index;
    }
    scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
    scroll_max = max(0, (info_list_num - kDisplay_Item_Info_Num));
    current_scroll_pos = min(current_scroll_pos, scroll_max);

    scroll_control->SetScrollRangeWZ(scroll_min, scroll_max);
    scroll_control->SetScrollPosWZ(current_scroll_pos);
}

void uiGMItemWindow::SetRectItemControl(int index_display, bool enable_reward)
{
    if (index_display >= kDisplay_Item_Info_Num)
    {
        return;
    }

    const POSTYPE info_control_pos = kControl_Button_B000;
    const POSTYPE reward_control_pos = kControl_Button_B001;

    CControlWZ* ac_control = 
        GetControlWZ(GetControlID_byPosition(info_control_pos + index_display));
    if (ac_control == NULL)
    {
        return;
    }

    RECT ac_control_rect = ac_control->GetSizeRect();
    // pos에 해당하는 FTextRender 를 얻어서 RECT세팅
    item_info_text_render_[index_display].Init(GetDrawBase(), ac_control_rect);
}

bool uiGMItemWindow::ShowEditControl(bool is_show)
{
    if (edit_control_ == NULL || static_edit_control_ == NULL)
    {
        return false;
    }

    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (is_show)
    {
        static_edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        static_edit_control_->ShowWindowWZ(WZ_HIDE);

        if (strcmp(string_buffer, "검색어를 입력하세요") == 0)
        {
            edit_control_->SetTextWZ("");
        }
        edit_control_->ShowWindowWZ(WZ_SHOW);
    }
    else
    {
        edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        edit_control_->ShowWindowWZ(WZ_HIDE);
        //HideFindWord();

        if (string_buffer[0] == '\0')
        {
            sprintf(string_buffer, "검색어를 입력하세요");
        }
        static_edit_control_->SetTextWZ(string_buffer);
        static_edit_control_->ShowWindowWZ(WZ_SHOW);
    }
    return true;
}

void uiGMItemWindow::Release()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (handle_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture_);
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void uiGMItemWindow::OnUpdateSolarDialog()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        // UI Inetersect 영역 설정 - 설정한 영역에서만 UI가 동작한다.
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();        

        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();
        enable_rect.bottom = (enable_rect.top + kClientHeight+30);
        intersect_list.push_back(enable_rect);

        // 마우스휠 처리
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);
        OnMouseWheelScroll(mouse_wheel_state);

        if (ui_gm_window_manager_ && ui_gm_window_manager_->IsShowGMWindow(UI_GM_ITEM_OPTION_WINDOW))
        {
            CCtrlButtonWZ* class_control = NULL;
            for (int i = eCHAR_BERSERKER; i < kTempClassTypeMax; ++i)
            {
                class_control = static_cast<CCtrlButtonWZ*>
                    (GetControlWZ(GetControlID_byPosition(kControl_Button_B001+i-eCHAR_BERSERKER)));
                if (class_control)
                {
                    MoveClassIcon(class_control, i);
                    enable_rect = class_control->GetSizeRect();
                    intersect_list.push_back(enable_rect);
                }
            }
        }
    }

    UpdateToolTip();

    if (g_Input.GetState(DIK_ESCAPE, KS_UP, TRUE) || g_Input.GetState(DIK_ESCAPE, IKS_UP, TRUE))
    {
        if (IsVisible() == TRUE)    
        {
            if (!search_end())//검색중이면
            {
                SearchStop();
            }
        }
    }

    /* // 추천 검색어 기능 (몇몇 문제가 있어서 제거: 검색된 아이템 아이콘이 위에 그려짐, \n이 검색어에 추가됨, 클릭했을 때 종종 제대로 적용이 안됨
    if (search_end() && edit_control_ && edit_control_->GetState() == WZ_SHOW)
    {
        TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
        edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);

        size_t string_length = strlen(string_buffer);
        g_InterfaceManager.Trim(string_buffer);
        if (string_length > 0 && strcmp(string_buffer, pre_find_word_) != 0)
        {
            sprintf(pre_find_word_, string_buffer);
            RefreshPreFindList(string_buffer);
        }
        else if (string_length == 0)
        {
            HideFindWord();
        }
    }*/
}

void uiGMItemWindow::RefreshPreFindList(TCHAR* find_string)
{
    BYTE search_count = 0;
    if (ui_gm_window_manager_)
    {
        search_count = ui_gm_window_manager_->PreSearchFindWord(find_string);
    }
    
    if (search_count > 0)
    {
        find_list_ptr_->DeleteItemAll();
        DWORD size = ui_gm_window_manager_->activate_find_list()->size();
        for (DWORD i = 0; i < size; ++i)
        {
            const char* string = ui_gm_window_manager_->GetActivateFindData(i);
            if (string != NULL)
            {
                InsertFindList(string);
            }
        }
        ShowFindWord();
    }
    else
    {
        HideFindWord();
    }
}

void uiGMItemWindow::OnRenderSolarDialog()
{
    if (!item_unit_renderer_)
    {
        return;
    }

    item_unit_renderer_->Render();

    if (this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
    {
        if (eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType)
        {
            uiToolTipMan* tooltip_manager = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if (tooltip_manager)
            {
                tooltip_manager->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());

            }
        }
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Text_ST02));

    RECT rect;
    float x = 0.0f;
    float y = 0.0f;

    if (control)
    {
        rect = control->GetSizeRect();
    }
    else
    {
        this->GetDialogWZ()->GetClientRect(&rect);
    }
    x = static_cast<float>(rect.left) - 14;
    y = static_cast<float>(rect.top) + 2;

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

    ITEM_PARAMETAS* item_parameta;
    if (ui_gm_window_manager_ != NULL)
    {
        item_parameta = ui_gm_window_manager_->item_parametas();
    }

    if (item_parameta->class_type != eCHAR_NONE)
    {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (item_parameta->class_type == eCHAR_WITCHBLADE)
        {
            //직업
            g_pSunRenderer->RenderTexture( 
                handle_texture_,
                x,
                y,
                14.0f,
                14.0f,
                730.0f + (6 * 14),
                297.0f + 28.0f,
                14.0f,
                14.0f,
                NULL,
                0.0f
                );
        }
        else
        {
            //직업
            g_pSunRenderer->RenderTexture( 
                handle_texture_,
                x,
                y,
                14.0f,
                14.0f,
                730.0f + ((item_parameta->class_type - eCHAR_BERSERKER) * 14),
                297.0f + 14.0f,
                14.0f,
                14.0f,
                NULL,
                0.0f
                );
        }
#else
        //직업
        g_pSunRenderer->RenderTexture( 
            handle_texture_,
            x,
            y,
            14.0f,
            14.0f,
            730.0f + ((item_parameta->class_type - eCHAR_BERSERKER) * 14),
            297.0f + 14.0f,
            14.0f,
            14.0f,
            NULL,
            0.0f
            );
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        if (tooltip_manager && IsInRect(x, y, 14.0f, 14.0f))
        {
            tooltip_manager->RegisterTooltip(GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(item_parameta->class_type)));
			tooltip_manager->SetTooltipDialogID( GetDialogKey() );
        }
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void uiGMItemWindow::OnShowWindow(BOOL is_show)
{
    static bool once_move = false;
    if (is_show)
    {
        if (once_move == false)
        {
            this->MoveWindowWZ(20.0f, 100.0f);
            once_move = true;
        }
        RegisterESCKeyClose(true);
        ShowEditControl(false);

        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->SetCaptionFilter();
        }
    }
    else
    {
        //HideFindWord();
        RegisterESCKeyClose(false);
        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_OPTION_WINDOW, false);
        }
    }
}

void uiGMItemWindow::MouseOver_MsgProc(SI_MESSAGE* message)
{
    switch (message->eResultMsg) 
    {
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));
            if (position >= 0 && position < this->GetMaxSlotNum())
            {
                if (!IsEmpty(position))
                {
                    SCSlot& rSlot = GetSlot(position);
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                    m_MouseOverObject.pSlot = &rSlot;
                }
                else
                {
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
                }
            }
            else
            {
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
            }			

        }
        break;
    }
}


void uiGMItemWindow::MessageProc(SI_MESSAGE* message)
{
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONDCLICK:
        OnLButtonDoubleClick(message);
        break;
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_RBUTTONCLICK:
        OnRButtonClick(message);
        break;
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            OnSearch();
        }
        break;
    case RT_MSG_SB_PAGETOP://. 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP://.윗쪽 화살표를 눌렀다.
        OnUpScroll(message);
        break;
    case RT_MSG_SB_PAGEBOTTOM://. 아래쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM://.아랫쪽 화살표를 눌렀다
        OnDownScroll(message);
        break;
    case RT_MSG_SB_THUMBTRACK://. 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION://. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL://. 스크롤이 종료 되었다.
        OnThumbScroll(message);
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGMItemWindow::OnUpScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll_VS00);
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

        SetCurrentScrollPosition(current_scroll_pos);
        RefreshItemInfo();
    }

    return result;
}

bool uiGMItemWindow::OnDownScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll_VS00);
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
        SetCurrentScrollPosition(current_scroll_pos);

        RefreshItemInfo();
    }

    return result;
}

void uiGMItemWindow::SetCurrentScrollPosition(int current_scroll_pos)
{
    switch (item_window_state_)
    {
    case kState_BookMark:
        bookmark_scroll_pos_ = current_scroll_pos;
        break;
    case kState_History:
        history_scroll_pos_ = current_scroll_pos;
        break;
    case kState_Search:
        search_scroll_pos_ = current_scroll_pos;
        break;
    default:
        break;
    }
}

bool uiGMItemWindow::OnThumbScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll_VS00);
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
        SetCurrentScrollPosition(current_scroll_pos);
        RefreshItemInfo();

        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
        }
    }
    return result;
}

void uiGMItemWindow::OnMouseWheelScroll(BYTE mouse_wheel_state)
{
    if (mouse_wheel_state == MOUSE_WHEEL_UP)
    {
        OnUpScroll(NULL);
        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
        }
    }
    else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
    {
        OnDownScroll(NULL);
        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
        }
    }
}

void uiGMItemWindow::ItemSearch(TCHAR* search_word)
{
    if (edit_control_)
    {
        edit_control_->SetTextWZ(search_word);
        OnSearch();
    }
    else if (static_edit_control_)
    {
        static_edit_control_->SetTextWZ(search_word);
        OnSearch();
    }
}

void uiGMItemWindow::OnSearch()
{
    TCHAR find_message[MAX_VENDOR_SEARCH_LENGTH + 1];
    memset(find_message , 0 , sizeof(find_message) );

    if (edit_control_)
    {
        edit_control_->GetTextWZ(find_message, MAX_VENDOR_SEARCH_LENGTH);
    }
    else if (static_edit_control_)
    {
        static_edit_control_->GetTextWZ(find_message, MAX_VENDOR_SEARCH_LENGTH);
    }

    g_InterfaceManager.Trim(find_message);
    find_message[MAX_VENDOR_SEARCH_LENGTH] = '\0';
    int size = _tcslen(find_message);
    
    if (size > 0)   // 개행제거
    {
        if (find_message[size-1] == '\n')
        {
            find_message[size-1] = '\0';
        }
    }

    if (search_end())   // 이전 검색이 끝난 경우만 새로운 검색 가능
    {
        if (_stricmp(find_message, search_string()) != 0)
        {   // 방금 검색한 문자열이 아니라면
            SetItemWindowState(kState_Search);
            set_search_string(find_message);
            set_search_end(false);
            CCtrlListWZ* select_list = 
                static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_LS01)));
            if (select_list)
            {
                select_list->ShowWindowWZ(WZ_HIDE);
            }
            //HideFindWord();
        }
        else
        {
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "GMItemWin: 동일한 검색어로 검색할 수 없습니다.(Warning)", CHAT_TYPE_SYSTEM);
        }
    }
}

bool uiGMItemWindow::GetFilterResult(DWORD item_code)
{
    bool result_value = false;
    ITEM_PARAMETAS* item_parameta;
    if (ui_gm_window_manager_ != NULL)
    {
        item_parameta = ui_gm_window_manager_->item_parametas();
    }

    if (item_parameta->filter_main == FILTER_ALL)
    {
        return true; // 필터없음(전체) 무조건 true (미리 체크해서 getiteminfo하는 시간 절약)
    }

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL)
    {
        return false;
    }

    BYTE sub_filter = item_parameta->filter_subs[item_parameta->filter_main];
    switch (item_parameta->filter_main)
    {
    case FILTER_EQUIP: // 장비 필터
        result_value = GetFilterEquip(item_info, sub_filter);
        break;
    case FILTER_WASTE: // 소모품 필터
        result_value = GetFilterWaste(item_info, sub_filter);
        break;
    case FILTER_GRADE: // 등급 필터
        result_value = GetFilterGrade(item_info, sub_filter);
        break;
    case FILTER_GENDER: // 성별 필터
        result_value = GetFilterGender(item_info, sub_filter);
        break;
    case FILTER_ETC: // 종류 필터
        result_value = GetFilterEtc(item_info, sub_filter);
        break;
    default:
        result_value = true;//알수없는 필터라면 일단 true 해줘서 검색 될 수 있게 한다.
    }
    return result_value;
}

bool uiGMItemWindow::GetClassResult(DWORD item_code)
{
    bool result_value = false;
    ITEM_PARAMETAS* item_parameta;
    if (ui_gm_window_manager_ != NULL)
    {
        item_parameta = ui_gm_window_manager_->item_parametas();
    }

    if (item_parameta->class_type == eCHAR_NONE)
    {
        return true; // 클래스 필터(모두) 무조건 true (미리 체크해서 getiteminfo하는 시간 절약)
    }

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL)
    {
        return false;
    }

    switch (item_parameta->class_type)
    {
    case eCHAR_BERSERKER: //1
        result_value = (item_info->m_wEqClass1 == 1)? true:false;
        break;
    case eCHAR_DRAGON: //2
        result_value = (item_info->m_wEqClass2 == 1)? true:false;
        break;
    case eCHAR_SHADOW: //3
        result_value = (item_info->m_wEqClass3 == 1)? true:false;
        break;
    case eCHAR_VALKYRIE: //4
        result_value = (item_info->m_wEqClass4 == 1)? true:false;
        break;
    case eCHAR_ELEMENTALIST: //5
        result_value = (item_info->m_wEqClass5 == 1)? true:false;
        break;
    case eCHAR_MYSTIC: //6
        result_value = (item_info->m_wEqClass6 == 1)? true:false;
        break;
    case eCHAR_HELLROID: //7    //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        result_value = (item_info->m_wEqClass7 == 1)? true:false;
        break;
    case eCHAR_WITCHBLADE: //8    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        result_value = (item_info->m_wEqClass8 == 1)? true:false;
        break;
    }
    return result_value;
}

void uiGMItemWindow::FillFindListData(TCHAR* search_string)
{
    unsigned int search_count = 0;
    _ITEM_NAME_INFO search_buffer;

    if (search_new_)  // 검색이 처음이라면
    {   // 새로 검색
        DeleteAllSlot();

        item_name_info_iterator_ = ItemInfoParser::Instance()->GetItemNamesBegin();
        search_index_ = 0;
        search_new_ = false;
        total_searched_count_ = 0;
        //ShowFindWord();
        GetFocusList()->clear();

        if (button_control_)
        {
            ChangeFindIconImage(button_control_, true);
        }
        ShowEditControl(false);

        if (search_type_ == kFind_From_Name || search_type_ == kFind_From_Description)
        {
            if (ui_gm_window_manager_)
            {
                ui_gm_window_manager_->AddFindWord(search_string);
            }
        }
    }

    while (item_name_info_iterator_ != item_name_info_iterator_end_)
    {
        search_buffer.m_dwItemCode = item_name_info_iterator_->m_dwItemCode;
        search_buffer.m_stName = item_name_info_iterator_->m_stName.c_str();
        search_buffer.description_string = item_name_info_iterator_->description_string.c_str();
        // 검색 알고리즘 적용
        if (GetFilterResult(search_buffer.m_dwItemCode) == false
            || GetClassResult(search_buffer.m_dwItemCode) == false
            )    // 필터로 걸러낸다
        {
            // 검색 알고리즘 종료
            ++search_count;
            ++item_name_info_iterator_;
            ++total_searched_count_;
            if (search_count >= number_of_searching_per_a_frame_)
            {
                break;
            }
            continue;
        }
        
        if (search_type_ == kFind_From_Name)
        {
            if (search_buffer.m_stName.find(search_string, 0) != -1)
            {
                searched_results_->push_back(search_buffer.m_dwItemCode);
                SetItemData(search_index_, search_index_);
                ++search_index_;
            }
        }
        else if (search_type_ == kFind_From_Description)
        {
            if (search_buffer.description_string.find(search_string, 0) != -1)
            {
                searched_results_->push_back(search_buffer.m_dwItemCode);
                SetItemData(search_index_, search_index_);
                ++search_index_;
            }
        }
        else if (search_type_ == kFind_From_ItemType)
        {
            WORD item_type = static_cast<WORD>(atoi(search_string_));
            BASE_ITEMINFO *item_info = ItemInfoParser::Instance()->GetItemInfo(search_buffer.m_dwItemCode);
            if (item_info && item_info->m_wType == item_type)
            {
                searched_results_->push_back(search_buffer.m_dwItemCode);
                SetItemData(search_index_, search_index_);
                ++search_index_;
            }
        }
        else // if (search_type_ == kFind_From_WasteType)
        {
            BYTE waste_type = static_cast<BYTE>(atoi(search_string_));
            BASE_ITEMINFO *item_info = ItemInfoParser::Instance()->GetItemInfo(search_buffer.m_dwItemCode);
            if (item_info && item_info->m_byWasteType == waste_type)
            {
                searched_results_->push_back(search_buffer.m_dwItemCode);
                SetItemData(search_index_, search_index_);
                ++search_index_;
            }
        }
        // 검색 알고리즘 종료
        ++search_count;
        ++item_name_info_iterator_;
        ++total_searched_count_;
        if (search_count >= number_of_searching_per_a_frame_)
        {
            break;
        }        
    }

    if (item_name_info_iterator_ == item_name_info_iterator_end_)
    {
        set_search_end(true);
        search_new_ = true;     //검색 시작 초기화

        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->SetSearchEnd();//옵션 적용버튼 활성화를 위한 로직
        }

        if (button_control_)
        {
            ChangeFindIconImage(button_control_, false);
        }
    }

    /*SetSearchProgressData(total_searched_count_, search_index_);
    RefreshScroll(searched_results_->size());
    RefreshItemInfo();*/
    RefreshList(kState_Search, total_searched_count_, search_index_);
}
//------------------------------------------------------------------------------
void uiGMItemWindow::UpdateToolTip()
{
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    bool is_mouse_over = false;
    int loop_end = kControl_Size;
    for (POSTYPE position = 0; position < loop_end; ++position)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(position));
        if(control)
        {
            RECT control_rect = control->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX)
            {
                if (control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
                {
                    is_mouse_over = true;
                    break;
                }
            }
        }
    }

    if (!is_mouse_over)
    {
        //tooltip_string_code_ = 0;
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
    else
    {
        /*if (tooltip_string_code_ > 0)
        {
            uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (tooltip_manager)
            {
                tooltip_manager->RegisterTooltipForWareNInventoryLock(tooltip_string_code_);
            }
        }*/
    }
}

void uiGMItemWindow::HideFindWord()	// 추천어 자동완성
{
    if (find_list_ptr_)
    {
        find_list_ptr_->ShowWindowWZ(WZ_HIDE);
        find_list_ptr_->SetCurSel(-1);
    }
}

void uiGMItemWindow::ShowFindWord()	// 추천어 자동완성
{
    if (find_list_ptr_)
    {
        find_list_ptr_->ShowWindowWZ(WZ_SHOW);
    }
}

void uiGMItemWindow::Process(DWORD tick_count)
{
    static DWORD delay_tick = 0;

    if (!search_end())   // 검색이 끝인가?
    {
        delay_tick += tick_count;
        if (delay_tick > search_frame_per_update_time_)
        {
            // 하던 검식 계속 진행
            FillFindListData(search_string());
            delay_tick = 0;
        }
    }
}

bool uiGMItemWindow::OnLButtonDoubleClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_id = message->dwCtrlID;
    DWORD control_postion = GetControlPosition_byID(control_id);    
    switch (control_postion)
    {
    case kControl_Button_B000:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B005:
    case kControl_Button_B006:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B010:
        {
            int current_scroll_pos = 0;
            CCtrlVScrollWZ* scroll_control = 
                static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));
            if (scroll_control != NULL)
            {
                current_scroll_pos = scroll_control->GetScrollPosWZ();
            }
            int position = (control_postion - kControl_Button_B000);

            ITEM_PARAMETAS* item_parameta = NULL;
            if (ui_gm_window_manager_ == NULL)
            {
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "GMItemWin: GM 매니저를 찾을 수 없습니다.(Error)", CHAT_TYPE_SYSTEM);
                result = false;
            }
            else
            {
                item_parameta = ui_gm_window_manager_->item_parametas();
                size_t focus = (current_scroll_pos + position);
                if (GetFocusList()->size() > focus)
                {
                    DWORD item_code = GetFocusData(focus);
                    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
                    CreateItem(item_info, item_parameta);
                }
            }
        }
        break;
    default:
        break;
    }
    return result;
}

bool uiGMItemWindow::OnRButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_id = message->dwCtrlID;
    DWORD control_postion = GetControlPosition_byID(control_id);    
    switch (control_postion)
    {
    case kControl_Button_B000:       // 항목버튼#1
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B005:
    case kControl_Button_B006:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B010:
        {
            if (ui_gm_window_manager_)
            {
                int current_scroll_pos = 0;
                CCtrlVScrollWZ* scroll_control = 
                    static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));
                if (scroll_control != NULL)
                {
                    current_scroll_pos = scroll_control->GetScrollPosWZ();
                }
                int position = (control_postion - kControl_Button_B000);

                size_t focus = (current_scroll_pos + position);
                if (GetFocusList()->size() <= focus)
                {
                    result = false;
                    if (ui_gm_window_manager_)
                    {
                        ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
                    }
                }
                else
                {
                    DWORD item_code = GetFocusData(focus);
                    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
                    bool is_bookmark_state = (item_window_state_ == kState_BookMark)? true : false;
                    ui_gm_window_manager_->SetPopupUpdateMenus(item_info, is_bookmark_state);
                }
            }
        }
        break;
    default:
        {
            if (ui_gm_window_manager_)
            {
                ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
            }
        }
        break;
    }
    return result;
}

bool uiGMItemWindow::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_id = message->dwCtrlID;
    DWORD control_postion = GetControlPosition_byID(control_id);    
    switch (control_postion)
    {
    case kControl_Button_B999://닫기 버튼
    {
        ShowInterface(FALSE);
    }
    case kControl_Edit_E000:// 입력창
    {
        ShowEditControl(true);
    }
    break;
    case kControl_Button_BT01://검색어 타입 버튼
    {
        ToggleShowControl(kControl_List_LS01);
    }
    break;
    case kControl_Button_B029:
    {
        if (search_end())
        {
            if (item_window_state_ == kState_Search)
            {
                SetItemWindowState(kState_History);
                size_t size = created_histroys_->size();
                RefreshList(kState_History, size, size);
            }
            else if (item_window_state_ == kState_History)
            {
                SetItemWindowState(kState_BookMark);
                size_t size = bookmark_lists_->size();
                RefreshList(kState_BookMark, size, size);
            }
            else //kState_BookMark
            {
                SetItemWindowState(kState_Search);
                DWORD total_count = ItemInfoParser::Instance()->GetItemAllCount(); 
                RefreshList(kState_Search, total_count, search_index_);
            }
        }
    }
    break;
    case kControl_Button_B028: //모두줍기
    {
        MSG_CG_GM_STRING_CMD_SYN msg;
        ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
        StrnCopy(msg.m_szStringCmd,"줍기",MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
        GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
    }
    break;
    case kControl_Button_B027: //필터초기화
    {
        if (ui_gm_window_manager_ && search_end())//검색중엔 필터 초기화 불가
        {
            uiGMItemResetWindow* reset_window = static_cast<uiGMItemResetWindow*>(ui_gm_window_manager_->GetGMWindow(UI_GM_ITEM_RESET_WINDOW));
            if (reset_window)
            {
                reset_window->ResetFilter();
                ui_gm_window_manager_->SetItemParametas(ui_gm_window_manager_->GetItemOption());
                SetFilterName("필터 없음");
            }
        }
        else
        {
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "검색 중엔 필터를 초기화 할 수 없습니다.", CHAT_TYPE_SYSTEM);
        }
    }
    break;
    case kControl_Button_B031: //목록변경
    {
        if (ui_gm_window_manager_)
        {
            ui_gm_window_manager_->ToggleGMWindow(UI_GM_ITEM_OPTION_WINDOW);
        }
    }
    break;
    case kControl_Button_BT00:
    {
        if (search_end()) 
        {
            OnSearch();
        }
        else
        {   //검색중이면 멈추기
            SearchStop();
        }
    }
    break;
    case kControl_List_LS00:// 추천검색어 표시 리스트
    {
        if (find_list_ptr_ && edit_control_)
        {
            size_t select_string_number = find_list_ptr_->GetCurSel();
            TCHAR select_string[MAX_VENDOR_SEARCH_LENGTH+1] = "";

            if (ui_gm_window_manager_)
            {
                const char* string = ui_gm_window_manager_->GetActivateFindData(select_string_number);
                if (string != NULL)
                {
                    static_edit_control_->SetTextWZ(string);
                    edit_control_->SetTextWZ(string);
                    sprintf(pre_find_word_, string);
                    OnSearch();
                    HideFindWord();
                }
            }
        }
    }
    break;
    case kControl_List_LS01:// 검색타입 표시 리스트
    {
        CCtrlListWZ* select_list = 
            static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_LS01)));
        BYTE select_index = static_cast<BYTE>(select_list->GetCurSel());
        if (select_list != NULL)
        {
            if (select_index >= 0 && 
                select_index < kFind_Type_MAX)
            {
                if (search_type_ != select_index)
                {
                    set_search_string("\n");
                }
                search_type_ = select_index;
                if (seacrch_type_text_control_ != NULL)
                {
                    TCHAR state_string[INTERFACE_STRING_LENGTH] = {0, };
                    g_InterfaceManager.GetInterfaceString(state_string_code[search_type_], state_string);
                    seacrch_type_text_control_->SetTextWZ(state_string);
                }
            }             
            select_list->ShowWindowWZ(WZ_HIDE);
        }
    }
    break;
    case kControl_Button_B001:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
    case kControl_Button_B024:
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    case kControl_Button_B025:
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case kControl_Button_B032:
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case kControl_Button_B033:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    {
        BYTE class_type = static_cast<BYTE>(control_postion-kControl_Button_B001+eCHAR_BERSERKER);
        SetClass(class_type);
    }
    break;
    default:
    {
        RECT static_edit_rect = static_edit_control_->GetSizeRect();
        int mouse_x = Mouse::Instance()->GetMouseX();
        int mouse_y = Mouse::Instance()->GetMouseY();
        if (mouse_x > static_edit_rect.left && mouse_x < static_edit_rect.right
            && mouse_y > static_edit_rect.top && mouse_y < static_edit_rect.bottom)
        {
            ShowEditControl(true);//kControl_Text_T201:// 입력창에 있는 static 창
        }
        else
        {
            if (find_list_ptr_->GetState() != WZ_SHOW)
            {
                ShowEditControl(false);
            }
        }
    }
    break;
    }

    if (control_postion != kControl_Button_BT01)
    {
        ShowControl(kControl_List_LS01, false);
    }
    if (ui_gm_window_manager_)
    {
        ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_MENU_WINDOW, false);
    }
    return result;
}

void uiGMItemWindow::SetClass(BYTE class_type)
{
    BYTE pre_selected_class = selected_class_;
    selected_class_ = class_type;

    if (pre_selected_class == selected_class_)
    {
        selected_class_ = eCHAR_NONE;
    }

    for (int i = eCHAR_BERSERKER; i < kTempClassTypeMax; ++i)
    {        
        CCtrlButtonWZ* class_control = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B001+i-eCHAR_BERSERKER)));
        if (class_control)
        {
            bool is_checked = false;

            if (eCHAR_NONE == selected_class_ || i == selected_class_)
            {
                is_checked = true;
            }
            else
            {
                is_checked = false;
            }
            ChangeClassIcon(class_control, i, is_checked);
        }
    }
    uiGMItemOptionWindow* dialog = GET_CAST_DIALOG(uiGMItemOptionWindow, IM_GM_WINDOW_MANAGER::GM_ITEM_OPTION_WINDOW);
    if (dialog)
    {
        dialog->SetClassType(selected_class_);
    }
}

void uiGMItemWindow::set_search_string(TCHAR *search_string)
{ 
    strncpy(search_string_, search_string, MAX_CHAT_LEN);    
    search_string_[MAX_CHAT_LEN] = '\0';     
}

void uiGMItemWindow::InsertFindList(const char* item_name)
{
    int index = find_list_ptr_->InsertItemBack();

#ifdef _INTERNATIONAL_UI
    DWORD list_font = StrToWzID("st10");
#else
    DWORD list_font = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

    TCHAR string_title[MAX_VENDOR_TITLE_LENGTH+1] = {0,};
    Snprintf(string_title , MAX_VENDOR_TITLE_LENGTH , item_name);
    GlobalFunc::SetItem(find_list_ptr_, index, 1, string_title, list_font, DT_VCENTER| DT_CENTER ,
        RGBA(0,0,0,0), RGBA(180,180,180,255), RGBA(100,100,180,255));
}


void uiGMItemWindow::RenderItemInfoText(DWORD index_display)
{
    const POSTYPE info_control_pos = kControl_Button_B000;
    CControlWZ* control = 
        GetControlWZ(GetControlID_byPosition(kControl_Button_B000 + index_display));
    if (control != NULL)
    {
        item_info_text_render_[index_display].SetRect(control->GetSizeRect());
        item_info_text_render_[index_display].convertLineInfo();
        item_info_text_render_[index_display].Render();
    }
}

void uiGMItemWindow::ClearItemInfoToDisplay(int index_display)
{
    SetRectItemControl(index_display, false);
    SetItemDataString(index_display, NULL);
    AddItem(index_display, 0);
}

void uiGMItemWindow::RefreshItemInfo()
{
    // 취합된 항목을 출력
    int current_scroll_pos = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));
    if (scroll_control != NULL)
    {
        current_scroll_pos = scroll_control->GetScrollPosWZ();
    }

    int index_info = current_scroll_pos; // 스크롤 위치부터
    int num_info = GetFocusList()->size();
    for (int index_display = 0; index_display < kDisplay_Item_Info_Num; ++index_display, ++index_info)
    {
        if (index_info >= num_info)
        {
            ClearItemInfoToDisplay(index_display);
        }
        else
        {
            SetItemData(index_display, index_info);
        }
    }
}

void _copy2Clipboard(const char* string_message)
{
    static char* char_ptr = NULL;
    TCHAR cheat_message[INTERFACE_STRING_LENGTH] = {0,};    
    sprintf(cheat_message, "//%s", string_message);

    if(OpenClipboard(NULL))
    {
        HGLOBAL handle_clipboard_data = NULL;
        EmptyClipboard();
        handle_clipboard_data = GlobalAlloc(GHND | GMEM_DDESHARE, strlen(cheat_message) + 1);
        if (handle_clipboard_data)
        {
            char_ptr = (char*)GlobalLock(handle_clipboard_data);
            strcpy(char_ptr, cheat_message);
            GlobalUnlock(handle_clipboard_data);
            SetClipboardData(CF_TEXT, handle_clipboard_data);
        }
        CloseClipboard();
    }
}

void uiGMItemWindow::CreateItem(BASE_ITEMINFO* item_info, ITEM_PARAMETAS* item_parameta, BYTE create_type/*= CREATE_TYPE_NORMAL*/, BYTE lucky_expiry/*= 0*/)
{
    TCHAR message_string[256] = {0,};

    if (item_info == NULL)
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "GMItemWin: 아이템 정보를 찾을 수 없습니다.(Error)", CHAT_TYPE_SYSTEM);
        return;
    }

    CODETYPE item_code = item_info->m_Code;
    //수량
    DWORD item_count = item_parameta->item_count;
    if (item_count == 0)
    {

        if (item_info->m_byDupNum == 0)
        {
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "GMItemWin: %d 아이템 스크립트(DupNum이 0입니다.)(Error)", CHAT_TYPE_SYSTEM);
            item_count = 1;
        }
        else
        {
            item_count = item_info->m_byDupNum;
        }
    }
    
    // 중첩
    BYTE overlapping = (item_parameta->overlapping == true)?1:0;
    if (ui_gm_window_manager_)
    {
        ui_gm_window_manager_->AddHistory(item_info->m_Code);
    }

    switch (create_type)
    {
    case CREATE_TYPE_LIMITED:
        {
            BYTE is_divine = 0;
            if (item_parameta->item_type == ITEM_TYPE_DIVINE || item_parameta->item_type == ITEM_TYPE_DIVINE_ETHERIA)
            {
                is_divine = 1;
            }
            Sprintf(message_string,"createlimited %d 2%d %d %d %d %d", item_code, item_code,
                item_count, is_divine, item_parameta->enchant_level, item_parameta->socket_count);
        }
        break;
    case CREATE_TYPE_LUCKY:
        {
            Sprintf(message_string,"createlucky %d %d %d %d %d %d", item_code, item_count, kGradeLevel4, 
                kGradeLevel4, lucky_expiry, item_parameta->item_type);
        }
        break;
    default: //CREATE_TYPE_NORMAL
        {
            if (item_parameta->socket_count <= SOCKET_MAX) // 소켓 1S~5S, 0이면 미확인
            {
                Sprintf(message_string,"make %d %d %d %d %d %d %d", item_code, item_count, overlapping,
                    item_parameta->item_type, item_parameta->enchant_level, item_parameta->socket_count, item_parameta->awakening_level);
            }
			else if (item_parameta->awakening_level > 0 && item_parameta->socket_count > SOCKET_MAX) // 각성 레벨이 0보다 크고 소켓이 5보다 크면 소켓 없게
			{
				Sprintf(message_string,"make %d %d %d %d %d %d %d", item_code, item_count, overlapping,
					item_parameta->item_type, item_parameta->enchant_level, 0, item_parameta->awakening_level);
			}
            else // 소켓 6이면 소켓 없게
            {
                Sprintf(message_string,"make %d %d %d %d %d", item_code, item_count, overlapping,
                    item_parameta->item_type, item_parameta->enchant_level);// 소켓 없게
            }
        }
    }

    _copy2Clipboard(message_string); // 클립보드에 복사
    last_make_item_code_ = item_code;
    if (ui_gm_window_manager_)
    {
        ui_gm_window_manager_->SetItemOptionMakeCaptions(message_string);
    }

    MSG_CG_GM_STRING_CMD_SYN msg;
    ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
    StrnCopy(msg.m_szStringCmd,message_string,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
    GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
}

void uiGMItemWindow::SetItemData(int index_display, int index_info)
{
    // index_display => control_pos
    const POSTYPE icon_control_pos = kControl_Button_B011;
    const POSTYPE reward_control_pos = kControl_Button_B001;

    int ac_info_list_size = static_cast<int>(GetFocusList()->size());
    if (index_info >= ac_info_list_size)
    {
        ShowControl(reward_control_pos + index_display, false);
        return;
    }

    bool enable_reward = false;

    BASE_ITEMINFO* item_info = 
        ItemInfoParser::Instance()->GetItemInfo(GetFocusData(index_info));	//.m_dwItemCode

    // 내용세팅
    SetRectItemControl(index_display, false);
    SetItemDataString(index_display, item_info);
    SetItemSlot(index_display, item_info);
}
void uiGMItemWindow::SetItemSlot(int index_display, BASE_ITEMINFO* item_info)
{
    if (index_display >= kDisplay_Item_Info_Num)
    {
        return;
    }

    if (item_info == NULL)
    {
        AddItem(index_display, 0);
        return;
    }

    AddItem(index_display, item_info->m_Code);
}

void uiGMItemWindow::SetItemDataString(int index_display, BASE_ITEMINFO* item_info)
{
    // pos에 해당하는 데이터/FTextRender 를 얻어서 세팅
    if (index_display >= kDisplay_Item_Info_Num)
    {
        return;
    }

    FTextRender* item_info_text_render_ptr = &(item_info_text_render_[index_display]);
    item_info_text_render_ptr->ClearSentence();

    if (item_info == NULL)
    {
        return;
    }

    FTextParser parser;
    FTextSentence sentence;
    RECT text_rect = item_info_text_render_ptr->GetRect();
    int text_color_index = kComplete_Text_Color;

    TCHAR item_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR item_real_name[INTERFACE_STRING_LENGTH] = {0,};

    //GetACObjectText(ac_info, ac_string);
    g_InterfaceManager.GetItemName(item_info->m_Code, item_real_name, INTERFACE_STRING_LENGTH);
    Sprintf(item_string, "(ID:%d) %s", item_info->m_Code, item_real_name);
    GlobalFunc::CovertSimpleText(item_string, INTERFACE_STRING_LENGTH, item_info_font, text_rect);
    parser.SetDefaultFont(item_info_font, item_text_color[text_color_index]);
    BOOL result = parser.Parser(item_string, sentence);
    if (result == TRUE)
    {
        item_info_text_render_ptr->Append(sentence);
    }

    ZeroMemory(item_string, INTERFACE_STRING_LENGTH);

    TCHAR* additional_string = "";
    if (item_info->IsArmor())
    {
        switch (item_info->GetSubTypeDetail())
        {
        case ItemSubTypeDetail::kGeneral:
            if (item_info->m_Code < 10000) //리미티드는 4자리수 방어구만 됨
            {
                additional_string = " [리밋가능]";
            }
            break;
        case ItemSubTypeDetail::kFate:
            additional_string = " [럭키]";
            break;
        }
    }
    Sprintf(item_string, "%s%s", item_info->m_pszName, additional_string);
    
    GlobalFunc::CovertSimpleText(item_string, INTERFACE_STRING_LENGTH, item_info_font, text_rect);
    parser.SetDefaultFont(item_info_font, item_text_color[kDisable_Text_Color]);
    result = parser.Parser(item_string, sentence);
    if (result == TRUE)
    {
        item_info_text_render_ptr->Append(sentence);
    }

    item_info_text_render_ptr->SetMargin(8, 4, 8, 3);
    item_info_text_render_ptr->convertLineInfo();
}

void uiGMItemWindow::CallBackRenderItemInfo(CControlWZ* control_ptr, void* parameter)
{
    uiGMItemWindow* gm_item_window = 
        GET_CAST_DIALOG(uiGMItemWindow, IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW);
    if (gm_item_window == NULL)
    {
        return;
    }

    gm_item_window->RenderItemInfoText((DWORD)parameter);
}

void uiGMItemWindow::SetSearchProgressData(DWORD search_count, DWORD find_count)
{
    float gauge_ratio = 0.0f;
    DWORD total_count = ItemInfoParser::Instance()->GetItemAllCount();
    if (item_window_state_ != kState_Search)
    {
        total_count = search_count;
    }

    // 게이지 출력
    TCHAR data_string[INTERFACE_STRING_LENGTH] = {0, };
    CControlWZ* data_control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    if (data_control != NULL)
    {
        RECT gauge_rect = data_control->GetSizeRect();

        if (total_count > 0)
        {
            gauge_ratio = (float)search_count / total_count;
            if (gauge_ratio < 0.01f)
            {
                gauge_ratio = 0.01f;
            }
            else if (gauge_ratio > 1.0f)
            {
                gauge_ratio = 1.0f;
            }
        }
        else
        {
            gauge_ratio = 1.0f;
        }

        float gauge_width = search_gauge_control_size_ * gauge_ratio;
        float texture_width = search_gauge_texture_size_ * gauge_ratio;
        data_control->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth = texture_width;
        data_control->MoveWindowWZ(static_cast<float>(gauge_rect.left), 
            static_cast<float>(gauge_rect.top), 
            gauge_width, 
            static_cast<float>(gauge_rect.bottom - gauge_rect.top));
    }

    // 달성률 출력
    data_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));
    if (data_control != NULL)
    {
        Snprintf(data_string, INTERFACE_STRING_LENGTH - 1, "%.1f%% (%d)", (gauge_ratio * 100.0f), find_count);
        data_control->SetTextWZ(data_string);
    }
}
//------------------------------------------------------------------------------ 
bool uiGMItemWindow::AddItem(POSTYPE at_pos, SLOTCODE item_code)
{
    if (item_code == 0)
    {
        DeleteSlot(at_pos, NULL);
        return false;
    }

    BASE_ITEMINFO* base_item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);

    if (!base_item_info)
    {
        assert(!"GM_Item_Window(존재하지 않는 아이템입니다)");
        return false;
    }

    static DWORD SERIAL = 10000000;
    SCItemSlot item_slot;
    item_slot.SetSlotType(ST_ITEM);
    item_slot.SetCode(item_code);

    WORD item_count = 1;
    ITEM_PARAMETAS* item_parameta;
    if (ui_gm_window_manager_ != NULL)
    {
        item_parameta = ui_gm_window_manager_->item_parametas();
    }
    if (item_parameta->item_count == 0)
    {
        if (base_item_info)
        {
            item_count = base_item_info->m_byDupNum;
        }
        else
        {
            item_count = 1;
        }
    }
    else
    {
        item_count = item_parameta->item_count;
        if (item_count > 255)   // 최대겹치기는 byte
        {
            item_count = 255;
        }
    }
    item_slot.SetNum((DURATYPE)item_count);
    
    bool is_equip = false;
    if (base_item_info->IsWeapon() || base_item_info->IsArmor())
    {
        if (item_parameta->item_type == ITEM_TYPE_NORMAL || item_parameta->item_type == ITEM_TYPE_DIVINE)
        {
            item_slot.SetEnchant(item_parameta->enchant_level);
			const AwakeningInfo* awakening_info = AwakeningInfoParser::Instance()->FindData(item_slot.GetCode());
			if ( awakening_info )
			{
				item_slot.SetAwakening(item_parameta->awakening_level);
			}

            eSOCKET_IDENTIFY_TYPE socket_type = eSOCKET_IDENTIFIED_ITEM;
            if (item_parameta->socket_count <= SOCKET_MAX)
            {
                item_slot.SetSocketNum(item_parameta->socket_count);
            }
            else
            {   // 소켓 없는 아이템
                socket_type = eSOCKET_FAIL_IDENTIFY_PROCESS;
            }
            item_slot.SetSocketIdentifyType(socket_type);
        }
        is_equip = true;
    }

    if (is_equip || base_item_info->m_wType == eITEMTYPE_PET)
    {
        KindOfItemType::value_type item_type = KindOfItemType::Default;
        switch (item_parameta->item_type)
        {
        case ITEM_TYPE_DIVINE: item_type = KindOfItemType::Divine;
            break;
        case ITEM_TYPE_ETHERIA: item_type = KindOfItemType::Etheria;
            break;
        case ITEM_TYPE_DIVINE_ETHERIA: item_type = KindOfItemType::EtheriaWithDivine;
            break;
        case ITEM_TYPE_SKIN: item_type = KindOfItemType::Shell;
            break;
        }
        nsSlot::ItemTypeChanger::ChangeItemByType(&item_slot, item_type);
    }

    item_slot.SetSerial(++SERIAL);

    DeleteSlot(at_pos, NULL);
    InsertSlot(at_pos, item_slot);
    return true;
}
//------------------------------------------------------------------------------ 
RC::eSLOT_INSERT_RESULT uiGMItemWindow::InsertSlot(POSTYPE at_pos, SCSlot& slot)
{
    /// 슬롯에 넣음
    RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot(at_pos, slot);

    // HERO이기 때문에 Render

    // 시리얼은 설정되서 온다.

    /// 인벤토리
    CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(at_pos));
    RECT rect = control_ptr->GetSizeRect();

    SCSlot& real_slot = GetSlot(at_pos);

    if(item_unit_renderer_)
    {
        item_unit_renderer_->AddItemUnit(slot.GetSerial(), slot.GetCode(), &rect, &real_slot ,control_ptr);
    }
    else
    {
        assert(item_unit_renderer_);
    }
    ReFreshRenderSlot();

    return eInsertSlotResult;
}
//------------------------------------------------------------------------------ 
BOOL uiGMItemWindow::DeleteSlot(POSTYPE at_pos, SCSlot* slot_out)
{
    if (item_unit_renderer_)
    {
        item_unit_renderer_->RemoveItemUnit(GetSlot(at_pos).GetSerial());
    }
    SCSlotContainer::DeleteSlot(at_pos, slot_out);
    ReFreshRenderSlot();
    return TRUE;
}
//------------------------------------------------------------------------------
void uiGMItemWindow::DeleteAllSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = kControl_Button_B011; slot_pos <= kControl_Button_B020; ++slot_pos)
    {
        this->DeleteSlot(slot_pos, NULL);
    }
}

void uiGMItemWindow::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit* item_unit_render = NULL;
    RECT slot_rect;
    CControlWZ* control_ptr = NULL;

    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        POSTYPE current_scroll_pos = scroll_control->GetScrollPosWZ();
        SCSlot& rSlot = GetSlot(i);
        if (IsEmpty(i))
        {
            continue;
        }
        /*if (GetCurInvenContainer() == 0 && // 0: InventoryDialog::INVENTORY
            (i < EXTEND_INVENTORY_MAX))*/
        {
            // 보여줘야할 목록
            if(item_unit_renderer_ == NULL)
            {
                continue;
            }
            item_unit_render = item_unit_renderer_->GetItemUnit(rSlot.GetSerial());

            control_ptr = GetControlWZ(GetControlID_byPosition(i)); //i가 바로 위치
            slot_rect = control_ptr->GetSizeRect();
            if (item_unit_render == NULL)
            {
                item_unit_render = item_unit_renderer_->AddItemUnit(rSlot.GetSerial(), 
                    rSlot.GetCode(), &slot_rect, &rSlot ,control_ptr);
            }
            else
            {
                item_unit_renderer_->ChangePositionItemUnit(rSlot.GetSerial(), &slot_rect, control_ptr);
            }
        }
    }
}

void uiGMItemWindow::ChangeFindIconImage(CControlWZ* control, bool is_searching)
{
    // 여기 좌표 잘 찍어 보자
    SI_TFORMAT sImageFormat;    
    sImageFormat.m_sImageState[E_BASE].m_sImageName = "all_butten4.tga";
    sImageFormat.m_sImageState[E_BASE].m_blIs = TRUE;
    sImageFormat.m_sImageState[E_ON].m_sImageName = "all_butten4.tga";
    sImageFormat.m_sImageState[E_ON].m_blIs = TRUE;
    sImageFormat.m_sImageState[E_CLICK].m_sImageName = "all_butten4.tga";
    sImageFormat.m_sImageState[E_CLICK].m_blIs = TRUE;    

    SI_ToolTip tooltip;
    tooltip.initialize();
    tooltip.m_blUse = true;
    tooltip.m_dwFontID = item_info_font;
    tooltip.m_coBack = item_text_background_color;
    tooltip.m_coText = item_text_color[kInComplete_Text_Color];
    
    if (is_searching)
    {
        sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopX = 410;
        sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopY = 195;

        sImageFormat.m_sImageState[E_ON].m_sFRect.fTopX = 425;
        sImageFormat.m_sImageState[E_ON].m_sFRect.fTopY = 195;

        sImageFormat.m_sImageState[E_CLICK].m_sFRect.fTopX = 440;
        sImageFormat.m_sImageState[E_CLICK].m_sFRect.fTopY = 195;

        tooltip.m_dwStrID = 11260;   //11260	정지
    }
    else
    {
        sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopX = 410;
        sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopY = 195;

        sImageFormat.m_sImageState[E_ON].m_sFRect.fTopX = 425;
        sImageFormat.m_sImageState[E_ON].m_sFRect.fTopY = 195;

        sImageFormat.m_sImageState[E_CLICK].m_sFRect.fTopX = 440;
        sImageFormat.m_sImageState[E_CLICK].m_sFRect.fTopY = 195;

        tooltip.m_dwStrID = 9707;   //9707	검색
    }
    
    sImageFormat.m_sImageState[E_BASE].m_sFRect.fWidth = 15;
    sImageFormat.m_sImageState[E_BASE].m_sFRect.fHeight = 15;
    sImageFormat.m_sImageState[E_ON].m_sFRect.fWidth = 15;
    sImageFormat.m_sImageState[E_ON].m_sFRect.fHeight = 15;
    sImageFormat.m_sImageState[E_CLICK].m_sFRect.fWidth = 15;
    sImageFormat.m_sImageState[E_CLICK].m_sFRect.fHeight = 15;

    control->SetToolTipInfo(&tooltip);
    control->SetCtrlImageFormat(&sImageFormat);
}

void uiGMItemWindow::MoveClassIcon(CControlWZ* control, BYTE class_type)
{
    if (control == NULL)
    {
        return;
    }

    RECT rect = { 0, 0, 0, 0 };
    rect = control->GetSizeRect();

    LONG long_width = 40;
    float width = 40.f;
    float height = 40.f;

    uiGMItemOptionWindow* dialog = GET_CAST_DIALOG(uiGMItemOptionWindow, IM_GM_WINDOW_MANAGER::GM_ITEM_OPTION_WINDOW);
    
    RECT rc;
    dialog->GetDialogWZ()->GetClientRect(&rc);
    LONG dialog_width = (rc.right - rc.left);
    rect.top = rc.bottom;
    rect.left = rc.left + ((class_type-eCHAR_BERSERKER) * (long_width+3)) + 
        (dialog_width - ((long_width + 3) * (eCHAR_TYPE_MAX - 1)))/2;

    control->MoveWindowWZ((float)(rect.left), (float)(rect.top), width, height);
}

void uiGMItemWindow::ChangeClassIcon(CControlWZ* control, BYTE class_type, bool is_checked)
{
    SI_TFORMAT sImageFormat;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    float x_positions[kTempClassTypeMax] = { 0.f, 614.f, 654.f, 694.f, 734.f, 774.f, 814.f, 947.f , 987.f };
    float y_positions[E_NUM] = { 371.f, 411.f, 451.f, 371.f, 491.f};
    float y_helloid_position[E_NUM] = { 708.f, 748.f, 788.f, 708.f, 828.f };
    float y_witchblade_position[E_NUM] = { 240, 280, 240, 240, 280 };
#else
  #ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    float x_positions[kTempClassTypeMax] = { 0.f, 614.f, 654.f, 694.f, 734.f, 774.f, 814.f, 947.f };
    float y_positions[E_NUM] = { 371.f, 411.f, 451.f, 371.f, 491.f};
    float y_helloid_position[E_NUM] = { 708.f, 748.f, 788.f, 708.f, 828.f };
  #else
    float x_positions[kTempClassTypeMax] = { 0.f, 614.f, 654.f, 694.f, 734.f, 774.f, 814.f};
    float y_positions[E_NUM] = { 371.f, 411.f, 451.f, 371.f, 491.f};
  #endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
  #else
    float x_positions[kTempClassTypeMax] = { 0.f, 322.f, 353.f, 384.f, 415.f, 446.f};
    float y_positions[E_NUM] = { 2.f, 33.f, 64.f, 95.f, 126.f};
  #endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    /*    
    E_BASE,                     // 기본 상태   
    E_ON,                       // 마우스 ON   
    E_CLICK,                    // 마우스 Click 
    E_DISABLE,                  // 비활성화 상태
    E_CHECK,                    // 체크 버튼
    E_NUM,
    */
    
    for (int i = E_BASE; i < E_NUM; ++i)
    {
        sImageFormat.m_sImageState[i].m_blIs = TRUE;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        sImageFormat.m_sImageState[i].m_sImageName = "Common_bg01.tga";
        sImageFormat.m_sImageState[i].m_sFRect.fWidth = 40;
        sImageFormat.m_sImageState[i].m_sFRect.fHeight = 40;
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID

        if (class_type == eCHAR_HELLROID)
        {
            switch (i)
            {
            case E_BASE:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_helloid_position[E_ON];
                break;
            case E_ON:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_helloid_position[E_CHECK];
                break;
            case E_CLICK:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_helloid_position[E_CLICK];
                break;
            case E_DISABLE:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_helloid_position[E_DISABLE];
                break;
            case E_CHECK:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_helloid_position[E_ON];
                break;
            }

            if (!is_checked && i == E_BASE)
            {
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_helloid_position[E_BASE];
            }
        }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        else if (class_type == eCHAR_WITCHBLADE)
        {
            sImageFormat.m_sImageState[i].m_sImageName = "Common_bg02.tga";
            switch (i)
            {
            case E_BASE:
                sImageFormat.m_sImageState[i].m_sFRect.fTopX = 788;
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_witchblade_position[E_ON];
                break;
            case E_ON:
                sImageFormat.m_sImageState[i].m_sFRect.fTopX = 868;
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_witchblade_position[E_CHECK];
                break;
            case E_CLICK:
                sImageFormat.m_sImageState[i].m_sFRect.fTopX = 868;
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_witchblade_position[E_CLICK];
                break;
            case E_DISABLE:
                sImageFormat.m_sImageState[i].m_sFRect.fTopX = 788;
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_witchblade_position[E_DISABLE];
                break;
            case E_CHECK:
                sImageFormat.m_sImageState[i].m_sFRect.fTopX = 788;
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_witchblade_position[E_ON];
                break;
            }

            if (!is_checked && i == E_BASE)
            {
                sImageFormat.m_sImageState[i].m_sFRect.fTopX = 788;
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_witchblade_position[E_BASE];
            }
        }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        else
        {
            switch (i)
            {
            case E_BASE:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_ON];
                break;
            case E_ON:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_CHECK];
                break;
            case E_CLICK:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_CLICK];
                break;
            case E_DISABLE:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_DISABLE];
                break;
            case E_CHECK:
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_ON];
                break;
            }

            if (!is_checked && i == E_BASE)
            {
                sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_BASE];
            }
        }
#else

        switch (i)
        {
        case E_BASE:
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_ON];
            break;
        case E_ON:
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_CHECK];
            break;
        case E_CLICK:
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_CLICK];
            break;
        case E_DISABLE:
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_DISABLE];
            break;
        case E_CHECK:
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_ON];
            break;
        }

        if (!is_checked && i == E_BASE)
        {
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_BASE];
        }
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#else
        sImageFormat.m_sImageState[i].m_sImageName = "Dlg3_6.tga";
        sImageFormat.m_sImageState[i].m_sFRect.fWidth = 27;
        sImageFormat.m_sImageState[i].m_sFRect.fHeight = 27;

        if (is_checked && i == E_BASE)
        {
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[E_CHECK];
        }
        else
        {
            sImageFormat.m_sImageState[i].m_sFRect.fTopY = y_positions[i];
        }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (class_type != eCHAR_WITCHBLADE)
    {
        sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopX = x_positions[class_type];
        sImageFormat.m_sImageState[E_ON].m_sFRect.fTopX = x_positions[class_type];
        sImageFormat.m_sImageState[E_CLICK].m_sFRect.fTopX = x_positions[class_type];
        sImageFormat.m_sImageState[E_DISABLE].m_sFRect.fTopX = x_positions[class_type];
        sImageFormat.m_sImageState[E_CHECK].m_sFRect.fTopX = x_positions[class_type];
    }
#else
    sImageFormat.m_sImageState[E_BASE].m_sFRect.fTopX = x_positions[class_type];
    sImageFormat.m_sImageState[E_ON].m_sFRect.fTopX = x_positions[class_type];
    sImageFormat.m_sImageState[E_CLICK].m_sFRect.fTopX = x_positions[class_type];
    sImageFormat.m_sImageState[E_DISABLE].m_sFRect.fTopX = x_positions[class_type];
    sImageFormat.m_sImageState[E_CHECK].m_sFRect.fTopX = x_positions[class_type];
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    SI_ToolTip tooltip;
    tooltip.initialize();
    tooltip.m_blUse = true;
    tooltip.m_dwFontID = item_info_font;
    tooltip.m_coBack = item_text_background_color;
    tooltip.m_coText = item_text_color[kInComplete_Text_Color];

    /*  10001	버서커
        10003	드래곤나이트 eCHAR_BERSERKER
        10005	섀도우 eCHAR_SHADOW
        10007	발키리 eCHAR_VALKYRIE
        10009	엘리멘탈리스트 eCHAR_ELEMENTALIST
        10002	미스틱
        10006   헬로이드
        2685   위치블레이드
    */
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    DWORD class_string[kTempClassTypeMax+1] = { 0, 10001, 10003, 10005, 10007, 10009, 10002, 10006, 2685 };
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    DWORD class_string[kTempClassTypeMax+1] = { 0, 10001, 10003, 10005, 10007, 10009, 10002, 10006 };
  #else
    DWORD class_string[kTempClassTypeMax+1] = { 0, 10001, 10003, 10005, 10007, 10009, 10002 };
  #endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    tooltip.m_dwStrID = class_string[class_type];
    control->SetTextWZ("");
    control->SetToolTipInfo(&tooltip);
    control->SetCtrlImageFormat(&sImageFormat);
}

void uiGMItemWindow::ESCKeyProcess(DWORD wParam,DWORD lParam)
{
    uiGMItemWindow* dialog = GET_CAST_DIALOG(uiGMItemWindow, IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW);
    if (dialog == NULL)
    {
        return;
    }

    if (dialog->search_end()) 
    {
        dialog->ShowInterface(FALSE);
    }
    else
    {   //검색중이면 멈추기
        dialog->SearchStop();
    }
}

void uiGMItemWindow::SearchStop()
{
    set_search_end(true);
    search_new_ = true;     //검색 시작 초기화
    set_search_string("\n");// 입력이 불가능한 문자열을 초기화 해야 검색시 중복되지 않음

    if (button_control_)
    {
        ChangeFindIconImage(button_control_, false);
    }
}

void uiGMItemWindow::RegisterESCKeyClose(bool is_value)
{
    if (is_value)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GM_Item_Window;
        msg.wParam = IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW;
        msg.DoSomething = uiGMItemWindow::ESCKeyProcess;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_GM_Item_Window);
    }
}

void uiGMItemWindow::SetFilterName(TCHAR *filter_name)
{
    CCtrlStaticWZ* filter_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_ST02)));
    if (filter_control != NULL)
    {
        filter_control->SetTextWZ(filter_name);
    }
    set_search_string("\n");// 옵션이 변경된 경우 검색어 중복방지도 리셋
}

bool uiGMItemWindow::GetFilterEquip(BASE_ITEMINFO* item_infomation, BYTE sub_filter)
{
    switch (sub_filter)
    {
    case EQUIP_ALL:
        if (item_infomation->IsWeapon() || item_infomation->IsArmor() ||
            item_infomation->IsAccessory() || item_infomation->IsSpecialAccessory())
        {
            return true;
        }
        break;
    case EQUIP_WEAPON:
        if (item_infomation->IsWeapon())
        {
            return true;
        }
        break;
    case EQUIP_ARMOR:
        if (item_infomation->IsArmor())
        {
            return true;
        }
        break;
    case EQUIP_ACCESSORY:
        if (item_infomation->IsAccessory())
        {
            return true;
        }
        break;
    case EQUIP_SPECIALACCESSORY:
        if (item_infomation->IsSpecialAccessory())
        {
            return true;
        }
        break;
    default:
        assert(!"잘못된 아이템 장비 서브 필터");
        return true;   // 알 수 없는 서브 필터 (일단 검색되게 한다)
        break;
    }
    return false;
}

bool uiGMItemWindow::GetFilterWaste(BASE_ITEMINFO *item_infomation, BYTE sub_filter)
{
    switch (sub_filter)
    {
    case WASTE_ALL:
        if (item_infomation->m_byWasteType != 0)
        {
            return true;
        }
        break;
    case WASTE_CAN_USE:
        if (item_infomation->IsCanUseWaste())
        {
            return true;
        }
        break;
    case WASTE_ONLY_WASTE:
        if (item_infomation->IsCanOnlyUseWaste())
        {
            return true;
        }
        break;
    case WASTE_POTION:
        if (item_infomation->IsPotion())
        {
            return true;
        }
        break;
    default:
        assert(!"잘못된 아이템 소모품 서브 필터");
        return true;   // 알 수 없는 서브 필터 (일단 검색되게 한다)
        break;
    }
    return false;
}

bool uiGMItemWindow::GetFilterGrade(BASE_ITEMINFO *item_infomation, BYTE sub_filter)
{
    ItemSubTypeDetailValue sub_type = item_infomation->GetSubTypeDetail();
    ItemSubTypeDetailValue filter_type = ItemSubTypeDetail::kInvalid;
    switch (sub_filter)
    {
    case GRADE_NORMAL:// 1~10등급
        filter_type = ItemSubTypeDetail::kGeneral;
        break;
    case GRADE_ULTIMATE:// 얼티메이트(일본최강)
        filter_type = ItemSubTypeDetail::kSuperJapan;
        break;
    case GRADE_ELITE1:// 엘리트 1차
        filter_type = ItemSubTypeDetail::kElite1_0st;
        break;
    case GRADE_CORRUPT1:// 저주받은 엘리트 1차
        filter_type = ItemSubTypeDetail::kCurruptTemple;
        break;
    case GRADE_ELITE1_5:// 엘리트 1.5차
        filter_type = ItemSubTypeDetail::kElite1_5st;
        break;
    case GRADE_ELITE2:// 엘리트 2차
        filter_type = ItemSubTypeDetail::kElite2_0st;
        break;
	case GRADE_ELITE2_5:// 엘리트 2.5차
		filter_type = ItemSubTypeDetail::kElite2_5st;
		break;
    case GRADE_UNIQUE1:// 유니크 1차
        filter_type = ItemSubTypeDetail::kUnique1_0st;
        break;
    case GRADE_UNIQUE1_5:// 유니크 1.5차
        filter_type = ItemSubTypeDetail::kUnique1_5st;
        break;
    case GRADE_UNIQUE2:// 유니크 2차
        filter_type = ItemSubTypeDetail::kUnique2_0st;
        break;
    case GRADE_UNIQUE2_5:// 유니크 2.5차
        filter_type = ItemSubTypeDetail::kUnique2_5st;
        break;
    case GRADE_LUCKY:// 럭키 아이템
        filter_type = ItemSubTypeDetail::kFate;
        break;
	case GRADE_FIGTER_ELITE2://투사의 엘리트 2차
		filter_type = ItemSubTypeDetail::kFigter_Elite2_0st;
		break;
    case GRADE_ELITE3://엘리트 3차
        filter_type = ItemSubTypeDetail::kElite3_0st;
        break;
    case GRADE_ELITE3_5://엘리트 3.5차
        filter_type = ItemSubTypeDetail::kElite3_5st;
        break;
	case GRADE_ELITE4://엘리트 4차
		filter_type = ItemSubTypeDetail::kElite4_0st;
		break;
    default:
        assert(!"잘못된 아이템 등급 서브 필터");
        return true;   // 알 수 없는 서브 필터 (일단 검색되게 한다)
        break;
    }
    return (sub_type == filter_type)?true:false;
}

bool uiGMItemWindow::GetFilterGender(BASE_ITEMINFO *item_infomation, BYTE sub_filter)
{
    // (0:모두, 1:남성, 2:여성 Const.h GenderType 참고)
    switch (sub_filter)
    {
    case GENDER_FREE:// 성별 관계없는 아이템
        if (item_infomation->gender_type == 0)
        {
            return true;
        }
        break;
    case GENDER_MALE:
        if (item_infomation->IsCanUseGender(1))
        {
            return true;
        }
        break;
    case GENDER_FEMALE:
        if (item_infomation->IsCanUseGender(2))
        {
            return true;
        }
        break;
    case GENDER_MALE_ONLY:
        if (item_infomation->gender_type == 1)
        {
            return true;
        }
        break;
    case GENDER_FEMAIL_ONLY:
        if (item_infomation->gender_type == 2)
        {
            return true;
        }
        break;
    default:
        assert(!"잘못된 아이템 성별 서브 필터");
        return true;   // 알 수 없는 서브 필터 (일단 검색되게 한다)
        break;
    }
    return false;
}

bool uiGMItemWindow::GetFilterEtc(BASE_ITEMINFO *item_infomation, BYTE sub_filter)
{
    switch (sub_filter)
    {
    case ETC_NORMAL: // 일반 아이템
        if (item_infomation->m_SCNCode == 40011) // string_code 40011: 일반 아이템
        {
            return true;
        }
        break;
    case ETC_QUEST: // 퀘스트
        if (item_infomation->m_wType == eITEMTYPE_QUEST || item_infomation->m_wQuestCode != 0)
        {
            return true;   //퀘스트 아이템
        }
    case ETC_AC: // AC 아이템
        if (item_infomation->m_wType == eITEMTYPE_AC)
        {
            return true;
        }
        break;
    case ETC_ENCHANT_COMPOSITE: // 인첸트 조합 아이템
        if (item_infomation->m_wType == eITEMTYPE_ENCHANT || item_infomation->m_wType == eITEMTYPE_CRYSTAL ||
            item_infomation->m_wType == eITEMTYPE_SOCKET || item_infomation->m_wType == eITEMTYPE_SOCKETMATERIAL ||
            item_infomation->m_wType == eITEMTYPE_ACCESSORYMATERIAL || item_infomation->m_wType == eITEMTYPE_SACCESSORYMATERIAL)
        {
            return true;
        }
        break;
    case ETC_ZARD: // 쟈드 아이템
        if (item_infomation->m_wType == eITEMTYPE_SOCKET_ZARD || item_infomation->m_wType == eITEMTYPE_SOCKET_IDENTIFY ||
            item_infomation->m_wType == eITEMTYPE_SOCKET_COMPOSE_ADDITIVE ||
            item_infomation->m_wType == eITEMTYPE_STAT_CHANGE || item_infomation->m_wType == eITEMTYPE_SOCKETEXTRACT)
        {
            return true;
        }
        break;
    case ETC_CHARGE: // 캐쉬 아이템
        if (item_infomation->IsChargeItem())
        {
            return true;
        }
        break;
    case ETC_PET: // 펫 아이템
        if (item_infomation->m_wType == eITEMTYPE_PET || item_infomation->m_wType == eITEMTYPE_PET_FEED || 
            item_infomation->m_wType == eITEMTYPE_PET_NAME_CARD)
        {
            return true;
        }
        break;
    case ETC_RIDER: // 탈 것
        if (item_infomation->IsRiderItem() || item_infomation->IsRiderPartsItem() ||
            item_infomation->m_wType == eITEMTYPE_EXTEND_DATE_RIDER || item_infomation->m_wType == eITEMTYPE_WAX)
        {
            return true;
        }
        break;
    case ETC_CHARACTER: // 캐릭터 관련
        if (item_infomation->m_wType == eITEMTYPE_CHANGE_CHARACTER_GENDER || item_infomation->m_wType == eITEMTYPE_EXTEND_CHARACTER_SLOT ||
            item_infomation->m_wType == eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL || item_infomation->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE ||
            item_infomation->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR ||  item_infomation->m_wType == eITEMTYPE_CHANGE_CHARACTER_HEIGHT)
        {
            return true;
        }
        break;
    case ETC_RANDOM: // 랜덤화아이템
        if (item_infomation->m_wType == eITEMTYPE_RANDOMIZE_ITEM)
        {
            return true;
        }
        break;
    case ETC_LOTTO: // 복권
        if (item_infomation->m_wType == eITEMTYPE_LOTTO)
        {
            return true;
        }
        break;
    case ETC_COSTUM: // 코스튬
        if (item_infomation->m_wType == eITEMTYPE_COSTUME)
        {
            return true;
        }
        break;
    default:
        assert(!"잘못된 아이템 종류 서브 필터");
        return true;   // 알 수 없는 서브 필터 (일단 검색되게 한다)
        break;
    }
    return false;
}

void uiGMItemWindow::SetItemWindowState(BYTE state)
{
    item_window_state_ = state;

    if (history_button_ == NULL)
    {
        return;
    }

    // 스크롤상태 초기화
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));

    if (state == kState_History)
    {
        if (scroll_control)
        {
            search_scroll_pos_ = scroll_control->GetScrollPosWZ();
        }
        static_state_control_->SetTextWZ("히스토리 목록");
        history_button_->SetTextWZ("즐겨찾기");
    }
    else if (state == kState_BookMark)
    {
        if (scroll_control)
        {
            history_scroll_pos_ = scroll_control->GetScrollPosWZ();
        }
        static_state_control_->SetTextWZ("즐겨찾기 목록");
        history_button_->SetTextWZ("검색보기");
    }
    else //if (state == kState_Search)
    {
        if (scroll_control)
        {
            bookmark_scroll_pos_ = scroll_control->GetScrollPosWZ();
        }
        static_state_control_->SetTextWZ("최근 검색결과");
        history_button_->SetTextWZ("기록보기");
    }
}

void uiGMItemWindow::RefreshList(BYTE state, DWORD search_count, DWORD find_size)
{
    WORD scroll_pos = 0;
    
    switch (state)
    {
    case kState_History:
        scroll_pos = history_scroll_pos_;
        break;
    case kState_BookMark:
        scroll_pos = bookmark_scroll_pos_;
        break;
    case kState_Search:
        scroll_pos = search_scroll_pos_;
        break;
    }
    if (item_window_state_ == state)
    {
        SetSearchProgressData(search_count, find_size);
        RefreshScroll(scroll_pos);
        RefreshItemInfo();
    }
}

void uiGMItemWindow::CreateItemFromList()
{
    ITEM_PARAMETAS* item_parameta = ui_gm_window_manager_->item_parametas();
    
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_VS00)));
    if (scroll_control == NULL)
    {
        return;
    }
    int current_scroll_pos = scroll_control->GetScrollPosWZ();

    size_t  focus = 0;
    for (int index = 0; index < kDisplay_Item_Info_Num; ++index)
    {
        focus = (current_scroll_pos + index);
        if (GetFocusList()->size() > focus)
        {
            DWORD item_code = GetFocusData(focus);
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
            CreateItem(item_info, item_parameta);
        }
        else
        {
            break;
        }
    }
}
#endif //_DEV_VER