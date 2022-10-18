#include "SunClientPrecompiledHeader.h"
#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "uiBattlezone2Tree/uiBattlezone2Tree.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#include "uiBattlezone2Reservation/uiBattlezone2Reservation.h"
#include "uiBattlezone2Password/uiBattlezone2Password.h"

#include "Hero.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "BattleScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "ObjectManager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiBattleZoneList/uiBattleZoneList_def.h"
#include "BattlezoneMissionTreeParser.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"
#include "ItemManager.h"
#include "SsqScene.h"

const int c_zoneListCount = 20;
const int c_AnimationFullTime = 1;

const int kListWidth[] = {
42,	//BATTLEZONE2_LIST_NUM,= 0,
58,//BATTLEZONE2_LIST_TYPE,
58,//BATTLEZONE2_LIST_STATE,
228,	//BATTLEZONE2_LIST_TITLE,
58,	//BATTLEZONE2_LIST_MEMBER
162,	//BATTLEZONE2_LIST_MAPNAME,
};

//const int c_iCYCLE_REFRESH_ROOM_LIST = 30;
const DWORD c_ListSelBarColor	= RGBA(74,53,15,255);
//------------------------------------------------------------------------------
void uiBattlezone2List::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004); // 닫기
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);//전체 방보기
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);//입장 가능 방 보기
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);//방만들기
    //RegisterControlPosition(StrToWzID("ST20"), kControl_Text_ST20);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);

    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);//페이지표시
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);//앞
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);//뒤
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);

    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
}

uiBattlezone2List::uiBattlezone2List(InterfaceManager* ui_manager):
uiBase(ui_manager),
list_control_(NULL),
find_list_ptr_(NULL)
{
    ui_battlezone2_manager_ = NULL;
    static_edit_control_ =  NULL;
    edit_control_ = NULL;
    static_page_control_ = NULL;
    order_by_ = true;
    GetManager();

    last_sort_by_function_ = kControl_Button_B012;
    is_first_ = true;//처음 여는 것인지
    last_party_info_ = NULL;
    follow_party_key_ = 0;
}

uiBattlezone2List::~uiBattlezone2List(void)
{
}

void uiBattlezone2List::Init(CDrawBase* draw_base)
{
    RegisterControl();

    list_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000)));
    assert(list_control_);

    edit_control_ = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(GetControlID_byPosition(kControl_Edit_E000)));
    assert(edit_control_);

    static_edit_control_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S002)));
    assert(static_edit_control_);
    
    static_page_control_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S008)));
    assert(static_page_control_);
    if (static_page_control_)
    {
        static_page_control_->SetTextWZ("1/1");//처음 페이지
    }

    battlezone_list_.clear();

    search_count_ = 0;
    //TODO:검색 기능 추후에
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_List_L001));
    if (control)
    {
        control->ShowWindowWZ(WZ_HIDE);
    }

    find_list_ptr_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L002)));
    if (find_list_ptr_)
    {
        find_list_ptr_->ShowWindowWZ(WZ_HIDE);
    }

    is_searched_ = false;
    is_ssq_open_ = false;
    bool is_result = true;
    last_scroll_position_ = 0;
    current_page_ = 1; //현재페이지
    max_page_ = 1; //최대페이지
    order_by_ = true;

    if (is_result)
    {
        //is_result = dropdown_class_filter().Init(GetStaticControlWZ(GetControlID_byPosition(kControl_Text_ST20)), 
        is_result = dropdown_class_filter().Init(GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B016)), 
            GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B005)), 
            GetListControlWZ(GetControlID_byPosition(kControl_List_L001)),
            NULL, 
            GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P010)));

        if (dropdown_class_filter().list_control())
        {
            dropdown_class_filter().list_control()->SetUnuseSelBar(true);
            dropdown_class_filter().list_control()->DeleteItemAll();
            dropdown_class_filter().list_control()->SetStartLineGapTop(10);
            dropdown_class_filter().list_control()->SetSellHeight(16);
        }

        // 0868 맵이름
        // 0636 아이디로 찾기
        // 0575 번호로 찾기
        dropdown_class_filter().InsertItem(868, (void*)BATTLEZONE2_FIND_BY_MAP_NAME, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(636, (void*)BATTLEZONE2_FIND_BY_ID, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(575, (void*)BATTLEZONE2_FIND_BY_NUMBER, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        //dropdown_class_filter().ResetVScroll();
        dropdown_class_filter().Select(0);
    }
    ToggleViewRoom(true);
    ShowEditControl(false);
    ShowFindWords(false);
    MoveDefaultWindowPos();

    InitTextures();
    animation_refresh_timer_.On(c_AnimationFullTime);
    ShowAnimation(false);

    if (find_vscroll_control())
    {
        find_vscroll_control()->ShowWindowWZ(WZ_HIDE);
    }
    control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P006));
    if (control)
    {
        control->ShowWindowWZ(WZ_HIDE);
    }
}

void uiBattlezone2List::Clear()
{
    if (static_edit_control_)
    {
        static_edit_control_->SetTextWZ(_T(""));
    }
    if (edit_control_)
    {
        edit_control_->SetTextWZ(_T(""));
    }
    if (static_page_control_)
    {
        static_page_control_->SetTextWZA(_T("1/1"));
    }
    last_party_info_ = NULL;
    ShowEditControl(false);
    is_searched_ = false;
    last_scroll_position_ = 0;
    search_count_ = 0;
    follow_party_key_ = 0;
    if (vscroll_control())
    {
        vscroll_control()->SetScrollPosWZ(0);
    }
    is_ssq_open_ = false;
    current_page_ = 1; //현재페이지
    max_page_ = 1; //최대페이지
    order_by_ = true;
    ShowAnimation(false);
    ShowWaitingButton(true);
    is_first_ = true;//처음 여는 것인지

    if (find_vscroll_control())
    {
        find_vscroll_control()->ShowWindowWZ(WZ_HIDE);
        find_vscroll_control()->SetScrollPosWZ(0);
    }
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P006));
    if (control)
    {
        control->ShowWindowWZ(WZ_HIDE);
    }
}

//------------------------------------------------------------------------------
void uiBattlezone2List::InitTextures()
{
    // 애니메이션 텍스쳐 2장 로드
    effect_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\Loading.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

    // 애니메이션 텍스쳐 위치 (12프레임) 프레임당 (40 * 40) 4 x 3 개
    int index = 0;
    float width = 0.0f;
    float height = 0.0f;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            width = j * ANIMATION_FRAME_WIDTH;
            height = i * ANIMATION_FRAME_HEIGHT;

            texture_rectangles_[index].left = width;
            texture_rectangles_[index].top = height;
            texture_rectangles_[index].right = ANIMATION_FRAME_WIDTH;
            texture_rectangles_[index].bottom = ANIMATION_FRAME_HEIGHT;
            
            ++index;
            if (index >= ANIMATION_FRAME)
            {
                break;
            }
        }
    }
}

void uiBattlezone2List::ShowAnimation(bool is_value)
{
    animatino_show_ = is_value;
}

void uiBattlezone2List::PlayAnimation()
{
    static FRECT img_rect = {0,};
    img_rect.left = 
        (float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().left;
    img_rect.top = 
        (float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().top;
    img_rect.right = 
        (float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().right;
    img_rect.bottom = 
        (float)GetControlWZ(GetControlID_byPosition(kControl_List_L000))->GetSizeRect().bottom;

    img_rect.left = img_rect.left + ((img_rect.right - img_rect.left - ANIMATION_FRAME_WIDTH) / 2) + 0.5f;
    img_rect.top = img_rect.top + ((img_rect.bottom - img_rect.top - ANIMATION_FRAME_HEIGHT) / 2);

    int frame = 30;//초당 애니메이션 프래임
    int count = ((animation_refresh_timer_.SpentTime() / frame) % ANIMATION_FRAME);
    
    if (animatino_show_)
    {
        g_pSunRenderer->RenderTexture(effect_texture_, img_rect.left, img_rect.top, 
            ANIMATION_FRAME_WIDTH, ANIMATION_FRAME_HEIGHT, 
            texture_rectangles_[count].left, texture_rectangles_[count].top, 
            texture_rectangles_[count].right, texture_rectangles_[count].bottom);
        ++count;
    }
    else
    {
        //nop
    }
}

void uiBattlezone2List::SetBattlezoneList()
{
    if (list_control_ == NULL)
    {
        return;
    }

    int row = 0;
    if (list_control_ != NULL)
    {
        GlobalFunc::SetReportButtonInfo(list_control_, BATTLEZONE2_LIST_NUM, 
            kListWidth[BATTLEZONE2_LIST_NUM], NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, BATTLEZONE2_LIST_TYPE, 
            kListWidth[BATTLEZONE2_LIST_TYPE], NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, BATTLEZONE2_LIST_STATE, 
            kListWidth[BATTLEZONE2_LIST_STATE], NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, BATTLEZONE2_LIST_TITLE, 
            kListWidth[BATTLEZONE2_LIST_TITLE], NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, BATTLEZONE2_LIST_MEMBER, 
            kListWidth[BATTLEZONE2_LIST_MEMBER], NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, BATTLEZONE2_LIST_MAPNAME, 
            kListWidth[BATTLEZONE2_LIST_MAPNAME], NULL);

        list_control_->DeleteItemAll();
        list_control_->SetCurSel(-1);
        list_control_->SetColorSelBar(c_ListSelBarColor);
        list_control_->SetUseReport(true);
        list_control_->SetReportUseCnt(7);
        list_control_->SetHideReportRender(true);
        list_control_->SetSellHeight(20);

        int list_size = battlezone_list_.size();

        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        
        //=====================================================================================
        //	1.리스트컨트롤 데이터삽입
        //=====================================================================================
        std::vector<PartyRoomInfo>::iterator begin = battlezone_list_.begin();
        std::vector<PartyRoomInfo>::iterator end = battlezone_list_.end();

        DWORD font_color = RGBA(255,255,255,0);

        int index = 0;
        search_count_ = 0;

        list_control_->SetBeginDrawIndex(0);
        for (; begin != end; ++begin)
        {
            PartyRoomInfo data = (*begin);
            index = list_control_->InsertItemBack();
            list_control_->SetItemData(index, (void*)&(*begin));

#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#else
            if (data.room_key_ == BASE_ROOM_KEY)//타락한 사원
            {
                font_color = RGBA(232,0,0,255);
            }
            else
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
            {
                if (data.is_mission_start_)
                {
                    font_color = RGBA(154,154,154,255);
                }
                else
                {
                    font_color = RGBA(255,255,0,255);
                }
            }

            ++search_count_;

            sprintf(string, "%d", (data.room_key_ - BASE_ROOM_KEY));
            GlobalFunc::SetItem(list_control_, index, BATTLEZONE2_LIST_NUM,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0), 
                font_color, 
                font_color);

            int room_type_string_code;

            const PartyZoneType zone_type = BattlezoneMissionTreeParser::Instance()->GetType(data.base_party_room_info.map_code_);
            switch (zone_type)
            {
            case PartyOfMission: 
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
            case PartyOfSpecialMission:
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
                {
                    room_type_string_code = eST_MISSION;
                } 
                break;
            case PartyOfPvPByPersonal: 
            case PartyOfPvPByTeam:
                {
                    room_type_string_code = eST_BATTLE_PVP;
                }
                break;
            case PartyOfTopOfTrialByCash: 
            case PartyOfTopOfTrialByNormal:
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
            case PartyOfTopOfTrialLight_cash:
            case PartyOfTopOfTrialLight_normal:
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
                {
                    room_type_string_code = 2498;
                }
                break;//저주받은탑 2498
            case PartyOfEvent:
                {
                    room_type_string_code = 2499;
                }
                break;//이벤트 2499
            //case PartyOfNone: //기본
            default: {room_type_string_code = 0;} break;
            }

            if (room_type_string_code)
            {
                g_InterfaceManager.GetInterfaceString(room_type_string_code, string,INTERFACE_STRING_LENGTH);
            }
            else
            {
                sprintf(string, "-");
            }
            GlobalFunc::SetItem(list_control_, index, BATTLEZONE2_LIST_TYPE,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0), 
                font_color, 
                font_color);

            if (data.is_mission_start_)
            {
                g_InterfaceManager.GetInterfaceString(eST_GOING, string, INTERFACE_STRING_LENGTH);
            }
            else
            {
                g_InterfaceManager.GetInterfaceString(eST_WAITING, string, INTERFACE_STRING_LENGTH);
            }
            GlobalFunc::SetItem(list_control_, index, BATTLEZONE2_LIST_STATE,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0), 
                font_color, 
                font_color);

            sprintf(string, data.base_party_room_info.party_room_title_);
            GlobalFunc::SetItem(list_control_, index, BATTLEZONE2_LIST_TITLE,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0), 
                font_color, 
                font_color);

#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#else
            if (data.room_key_ == BASE_ROOM_KEY)
            {
                sprintf(string, "70");
            }
            else
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
            {
                sprintf(string, "%d/%d", data.current_user_num_, data.max_user_num_);
            }
            GlobalFunc::SetItem(list_control_, index, BATTLEZONE2_LIST_MEMBER,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0), 
                font_color, 
                font_color);

            const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(data.base_party_room_info.map_code_);
            if (mapInfo)
            {
                if (data.base_party_room_info.party_room_type_ == PartyOfTopOfTrialByCash
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
                    || (data.base_party_room_info.party_room_type_ == PartyOfTopOfTrialLight_cash)
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
                    )
                {
                    TCHAR temp[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
                    g_InterfaceManager.GetInterfaceString(mapInfo->dwNCode, temp);
                    // 2443 (바로입장)
                    g_InterfaceManager.GetInterfaceString(2443, buffer_string);
                    sprintf(string, "%s%s", temp, buffer_string);
                }
                else
                {
                    g_InterfaceManager.GetInterfaceString(mapInfo->dwNCode, string, INTERFACE_STRING_LENGTH);
                }
            }
            else
            {
                //788 미션 없음
                g_InterfaceManager.GetInterfaceString(788, string, INTERFACE_STRING_LENGTH);
            }
            GlobalFunc::SetItem(list_control_, index, BATTLEZONE2_LIST_MAPNAME,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0), 
                font_color, 
                font_color);
        }
    }

#if WZENGINEVER >= 400
    int max_line_count = list_control_->GetListRowCnt();
    int draw_line_count = list_control_->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);
    VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);
#endif //#if WZENGINEVER >= 400
}

bool uiBattlezone2List::ShowEditControl(bool is_show)
{
    if (edit_control_ == NULL)
    {
        return false;
    }

    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (is_show)
    {
        static_edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        static_edit_control_->ShowWindowWZ(WZ_HIDE);

        edit_control_->ShowWindowWZ(WZ_SHOW);
        edit_control_->SetFocusWZ();
    }
    else
    {
        edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        edit_control_->ShowWindowWZ(WZ_HIDE);

        static_edit_control_->SetTextWZ(string_buffer);
        static_edit_control_->ShowWindowWZ(WZ_SHOW);
    }
    return true;
}

void uiBattlezone2List::Release()
{
    battlezone_list_.clear();
    if (list_control_)
    {
        list_control_->DeleteItemAll();
        list_control_->SetCurSel(-1);
    }
    dropdown_class_filter().Release();

    if(effect_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(effect_texture_);
        effect_texture_ = INVALID_HANDLE_VALUE;
    }
}

void uiBattlezone2List::ShowFindWords(bool is_value)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P006));

    if (find_list_ptr_ && find_vscroll_control() && control)
    {
        if (find_list_ptr_->GetState() == WZ_SHOW && is_value == false)
        {
            find_list_ptr_->ShowWindowWZ(WZ_HIDE);
            find_vscroll_control()->ShowWindowWZ(WZ_HIDE);
            control->ShowWindowWZ(WZ_HIDE);
        }
        else if (find_list_ptr_->GetState() == WZ_HIDE && is_value == true)
        {
            find_list_ptr_->ShowWindowWZ(WZ_SHOW);
            find_vscroll_control()->ShowWindowWZ(WZ_SHOW);
            control->ShowWindowWZ(WZ_SHOW);
        }
    }
}

void uiBattlezone2List::OnUpdateSolarDialog()
{
    dropdown_class_filter().Update();

    if (animation_refresh_timer_.Check() != TIMER_STAT_NONE)
    {
        if (animation_refresh_timer_.Check() == TIMER_STAT_FINISH)
        {
            animation_refresh_timer_.Off();
            animation_refresh_timer_.On(c_AnimationFullTime);
        }
    }

    if (is_searched_ != true)
    {
        if (list_refresh_timer_.Check() != TIMER_STAT_NONE)
        {
            if (list_refresh_timer_.Check() == TIMER_STAT_FINISH)
            {
                RequestBattlezoneList();
                //list_refresh_timer_.Off();
                //list_refresh_timer_.On(c_iCYCLE_REFRESH_ROOM_LIST);
            }
            else if (list_refresh_timer_.SpentTime() > 1200 && animatino_show_)
            {
                RequestBattlezoneList();
            }
        }
    }

    RECT control_rect;
    uiBattlezone2Tree* battlezone2_tree = GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);
    uiBattlezone2Reservation* battlezone2_reservation = GET_CAST_DIALOG(uiBattlezone2Reservation, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION);
    GetDialogWZ()->GetClientRect(&control_rect);

    DWORD focus_dialog_id = 0;

    if (battlezone2_tree && battlezone2_tree->IsVisible())
    {
        battlezone2_tree->MoveWindowWZ(control_rect.right, control_rect.top);
        focus_dialog_id = IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE;
    }
    else if (battlezone2_reservation && battlezone2_reservation->IsVisible())
    {
        battlezone2_reservation->MoveWindowWZ(control_rect.right, control_rect.top);
        focus_dialog_id = IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION;
    }
    else
    {
        focus_dialog_id = 0;
    }

    if (GetDialogWZ()->HaveFocus() == true && focus_dialog_id)
    {
        GlobalFunc::MoveToTopDialog(focus_dialog_id);
    }

    /*static TCHAR last_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (edit_control_)
    {
        edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        if (dropdown_class_filter().CurrentUserData() == BATTLEZONE2_FIND_BY_MAP_NAME &&
            edit_control_->GetState() == WZ_SHOW && strlen(string_buffer) > 0)
        {
            ShowFindWords(true);
            if (strcmp(last_string, string_buffer) != 0)
            {
                sprintf(last_string, string_buffer);
                FillFindListData(string_buffer);
            }
        }
        else
        {
            ShowFindWords(false);
        }
    }*/

    VScrollDialog::Update();
}

void uiBattlezone2List::OnRenderSolarDialog()
{
    RenderIcon();
    PlayAnimation();

	dropdown_class_filter().OnDraw();

	CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_List_L001));
	if (control)
	{
		control->OnDraw(true);
	}
}

void uiBattlezone2List::MouseOver_MsgProc(SI_MESSAGE* message)
{

}

void uiBattlezone2List::OnShowWindow(BOOL is_show)
{
    RECT control_rect;
    uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
    if (is_show)
    {
        if (g_SsqScene.is_ssq_state() == true)
        {
            SetIsSSQOpen(true);
        }
        LoadMapNameCode();//한번만 실행됨
        SetBattlezoneList();
        if (this->IsVisible() && battlezone2_lobby && battlezone2_lobby->GetDialogWZ())
        {
            battlezone2_lobby->GetDialogWZ()->GetClientRect(&control_rect);
            this->MoveWindowWZ(control_rect.left, control_rect.top);
        }

        dropdown_class_filter().Select(0);
        ShowEditControl(false);

        if (is_first_)
        {
            RequestBattlezoneList(true);
            is_first_ = false;
        }
        else
        {
            RequestBattlezoneList();
        }
        list_refresh_timer_.On(c_iCYCLE_REFRESH_ROOM_LIST);
        is_searched_ = false;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_Battlezone2List_Dialog;
        msg.wParam=IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        if (this->IsVisible() && battlezone2_lobby && battlezone2_lobby->GetDialogWZ())
        {
            this->GetDialogWZ()->GetClientRect(&control_rect);
            battlezone2_lobby->MoveWindowWZ(control_rect.left, control_rect.top);
        }
        
        if (edit_control_)
        {
            edit_control_->SetTextWZ("");
        }
        ShowWaitingButton(true);

        if (battlezone2_lobby)
        {
            battlezone2_lobby->set_is_ready(false);//레디 풀어주기)
        }
        
        if (GetManager())
        {
            GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_TREE, false);
            GetManager()->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_RESERVATION, false);
        }
        g_KeyQueueManager.DeleteMsg(eDoType_Battlezone2List_Dialog);
    }
}

uiBattlezone2Man* uiBattlezone2List::GetManager()
{
    if (ui_battlezone2_manager_ == NULL)
    {
        ui_battlezone2_manager_ = static_cast<uiBattlezone2Man*>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
        assert (ui_battlezone2_manager_);
    }
    return ui_battlezone2_manager_;
}

void uiBattlezone2List::Process(DWORD tick_count)
{
}

void uiBattlezone2List::OnSearch()
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};

    if (edit_control_)
    {
        edit_control_->GetTextWZ(string, INTERFACE_STRING_LENGTH-1);
    }

    //검색
    switch (dropdown_class_filter().CurrentUserData())
    {
        case BATTLEZONE2_FIND_BY_MAP_NAME:
        {
            // 캐릭터명의 최대 사이즈(16 byte)를 넘었습니다.
            /*if(!GlobalFunc::CompareLimitText(edit_control_, string, MAX_CHARNAME_LENGTH, 5790))
            {
                return;
            }*/

            if (lstrcmp (string, _T("")))
            {
                is_searched_ = true;
                SearchMap(string);
            }
        }
        break;
        case BATTLEZONE2_FIND_BY_ID:
        {
            if (lstrcmp (string, _T("")))
            {
                is_searched_ = true;
                SearchID(string);
            }
        }
        break;
        case BATTLEZONE2_FIND_BY_NUMBER:
        {
            if (lstrcmp (string, _T("")))
            {
                bool is_alpha = true;
                for (int i = 0; i < lstrlen(string); ++i)
                {
                    if (!(string[i]>=_T('0') && string[i]<=_T('9')))
                    {
                        is_alpha = false;
                    }
                }

                int value = _ttoi(string);
                if (is_alpha)
                {
                    is_searched_ = true;
    #if 0 //클라에서 검색시
                    RequestBattlezoneList(true);//검색전에 한번 요청
    #endif//
                    SearchNo(value);
                }
                else
                {
                    g_InterfaceManager.GetInterfaceString(5211, message, INTERFACE_STRING_LENGTH);
                    uiSystemMan* system_manager = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if (system_manager)
                    {
                        system_manager->Sys_VerifyLock(message);
                    }

                }
            }
        }
        break;
    }
        
    //ShowEditControl(false);
}
//------------------------------------------------------------------------------
void uiBattlezone2List::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    DWORD user_data = 0;
    eCHAR_TYPE menu_type = static_cast<eCHAR_TYPE>(user_data);

    bool is_change = false;
    if (dropdown_class_filter().MessageProc(message, OUT is_change))
    {
        if (is_change)
        {
            eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(dropdown_class_filter().CurrentUserData());
        }

        if (GetControlPosition_byID(message->dwCtrlID) == kControl_List_L001)
        {
            if (edit_control_ && static_edit_control_)//드랍박스가 변경되면 검색어 초기화
            {
                edit_control_->SetTextWZ("");
                static_edit_control_->SetTextWZ("");
            }
        }
        return;
    }
    /*if (GetManager() == NULL)
        return;*/

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONDCLICK:
        OnDoubleButtonClick(message);
        break;
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            OnSearch();
        }
        break;
    case RT_MSG_SB_LINETOP:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollUp(message);
            }   
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnFindVScrollUp(message);
            }            
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollDown(message);
            }   
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnFindVScrollDown(message);
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollThumb(message);
            }   
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V000)
            {
                OnFindVScrollThumb(message);
            }
        }
        break;

    default:
        SolarDialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiBattlezone2List::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiBattlezone2List::find_vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollThumb(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }
    if (VScrollDialog::OnVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::GetFindScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L002)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L002)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::OnFindVScrollThumb(SI_MESSAGE* msg)
{
    if (find_vscroll_control() == NULL)
    {
        return false;
    }
    if (OnVScrollThumb(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L002))->SetBeginDrawIndex(find_vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------ 
bool uiBattlezone2List::FindVScrollUp(SI_MESSAGE* msg)
{
    if (find_vscroll_control() == NULL)
    {
        return false;
    }

    int pos = find_vscroll_control()->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    find_vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {
        --pos;
        find_vscroll_control()->SetScrollPosWZ(pos);
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::FindVScrollDown(SI_MESSAGE* msg)
{
    if (find_vscroll_control() == NULL)
    {
        return false;
    }

    int pos = find_vscroll_control()->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    find_vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);

    if (pos < mmax)
    {
        ++pos;
        find_vscroll_control()->SetScrollPosWZ(pos);
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::OnFindVScrollUp(SI_MESSAGE* msg)
{
    if (find_vscroll_control() == NULL)
    {
        return false;
    }
    if (FindVScrollUp(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L002))->SetBeginDrawIndex(find_vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiBattlezone2List::OnFindVScrollDown(SI_MESSAGE* msg)
{
    if (find_vscroll_control() == NULL)
    {
        return false;
    }
    if (FindVScrollDown(msg) == true)
    {
        GetListControlWZ(GetControlID_byPosition(kControl_List_L002))->SetBeginDrawIndex(find_vscroll_control()->GetScrollPosWZ());
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void uiBattlezone2List::ShowBattlezoneTree(BYTE mode)
{
    uiBattlezone2Tree* battlezone_tree_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);

    uiBattlezone2Reservation* battlezone_reservation_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Reservation, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_RESERVATION);

    if (mode == uiBattlezone2Tree::kReservationMode)
    {
        battlezone_tree_dialog->ShowInterface(FALSE);
        battlezone_reservation_dialog->SetMode(mode);
        if (battlezone_reservation_dialog->IsVisible())
        {
            battlezone_reservation_dialog->ShowInterface(FALSE);
        }
        else
        {
            battlezone_reservation_dialog->ShowInterface(TRUE);
        }
    }
    else
    {
        battlezone_reservation_dialog->ShowInterface(FALSE);
        battlezone_tree_dialog->SetMode(mode);
        if (battlezone_tree_dialog->IsVisible())
        {
            battlezone_tree_dialog->ShowInterface(FALSE);
        }
        else
        {
            battlezone_tree_dialog->ShowInterface(TRUE);
        }
    }
    /*
    uiBattlezone2Tree* battlezone_tree_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);
    if (battlezone_tree_dialog == NULL)
    {
        return;
    }
    if (battlezone_tree_dialog->IsVisible())
    {
        if (battlezone_tree_dialog->battlezone_tree_mode() == mode)
        {
            battlezone_tree_dialog->ShowInterface(FALSE);
        }
        else
        {
            battlezone_tree_dialog->SetMode(mode);
        }
    }
    else
    {
        battlezone_tree_dialog->ShowInterfaceWithSound(TRUE);
        battlezone_tree_dialog->SetMode(mode);        
    }*/
}

bool uiBattlezone2List::EnterRoom(WORD party_key, const TCHAR* password, PartyRoomInfo* party_info)
{
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    if (party_key != 0xFF)
    {
        if (party_info == NULL)
        {
            party_info = last_party_info_;
        }
        if (party_info)
        {
            if (party_key == 0)
            {
                party_key = party_info->link_party_key_;
            }
            /*if (party_info->current_user_num_ >= party_info->max_user_num_)
            {   // 인원수에 위배
                //2477 입장 인원이 초과 되었습니다. RC_PARTY_FAIL_MAX_MEMBER_COUNT
                g_InterfaceManager.GetInterfaceString(2477, string, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, string);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, string);
                return false;
            }*/

			const BattlezoneMissionTreeInfo* find_info = NULL;
			if(party_info)
			{
				find_info = BattlezoneMissionTreeParser::Instance()->FindData(party_info->base_party_room_info.map_code_);    
			}

            if (find_info)
            {
                const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)find_info->map_code);
                if (map_info != NULL)
                {
                    if (party_info->base_party_room_info.map_code_ != 0 && g_pHero && 
                        (map_info->maxLV < g_pHero->GetLevel() || map_info->minLV > g_pHero->GetLevel()))
                    {   // 레벨에 위배
                        //2493 레벨 조건에 맞지 않는다.
                        g_InterfaceManager.GetInterfaceString(2493, string, INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, string);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, string);
                        return false;
                    }
                }
                else
                {
                    //맵 못찾음
                }
            }
        }
        MSG_CG_PARTY_JOIN_ROOM_SYN send_packet;
        send_packet.join_party_key_ = party_key;
        if (password)
        {
            send_packet.is_private_room_ = true;
            Snprintf(send_packet.password_, MAX_ROOMPASSWORD_LENGTH + 1, password);
        }
        else
        {
            send_packet.is_private_room_ = false;
        }
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        return true;
    }
#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#else
    else if (party_info && party_info->room_key_ == BASE_ROOM_KEY)
    {
#ifndef _DEV_VER //#if'N'def
        if (!g_SsqScene.Func_Output_with_internal_timer(string, true))
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            // 2474 타락한 사원 입장 시간이 아닙니다.
            g_InterfaceManager.GetInterfaceString(2474, message);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, _T(message));
        }
        else
#endif//_DEV_VER
        {
            //타락한 사원
            MSG_CG_ZONE_EX_SSQ_JOIN_INFO_SYN ssq_send_packet;
            POSTYPE output_position = INVALID_SLOT_POS;
            ssq_send_packet.m_ItemPos = 0;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ssq_send_packet, sizeof(ssq_send_packet));
        }
    }
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    return false;
}

bool uiBattlezone2List::OnDoubleButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_List_L000:
        {
            if (list_control_)
            {
                int index = list_control_->GetCurSel();
                if (index != -1)
                {
                    PartyRoomInfo* data = static_cast<PartyRoomInfo*>(list_control_->GetItemData(index));
                    last_party_info_ = data;
                    if (data && !strlen(data->base_party_room_info.party_room_password_))
                    {
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
                            EnterRoom(data->link_party_key_, NULL, data);
                        }
                    }
                    else
                    {
                        //EnterRoom(data->link_party_key_);
                        uiBattlezone2Password* battlezone2_password = GET_CAST_DIALOG(uiBattlezone2Password, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_PASSWORD);
                        if (battlezone2_password)
                        {
                            battlezone2_password->set_save_party_key(data->link_party_key_);
                            battlezone2_password->set_party_info(data);
                            battlezone2_password->ShowInterface(TRUE);
                        }
                    }
                }
                else
                {
                    //선택하신 방이 없습니다.
                    /*TCHAR message[INTERFACE_STRING_LENGTH];
                    g_InterfaceManager.GetInterfaceString(eST_NO_SELECTED_ROOM, message, INTERFACE_STRING_LENGTH);
                    uiSystemMan* system_manager = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
                    if (system_manager)
                    {
                        system_manager->Sys_VerifyLock(message);
                    }*/
                    result = false;
                }
            }
        }
        break;
    }
    return result;
}

bool GetBattlezone2List()
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    return (battlezone2_list != NULL)?true:false;
}
bool SortMember(const PartyRoomInfo &a, const PartyRoomInfo &b)
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (battlezone2_list == NULL)
    {
        return false;
    }

    int first_remain_member = a.max_user_num_ - a.current_user_num_;
    int second_remain_member = b.max_user_num_ - b.current_user_num_;

    if (battlezone2_list->order_by())
    {
        if (a.max_user_num_ == b.max_user_num_)
        {
            return (first_remain_member < second_remain_member);
        }
        return (a.max_user_num_ < b.max_user_num_);
    }
    else
    {
        if (a.max_user_num_ == b.max_user_num_)
        {
            return (first_remain_member > second_remain_member);
        }
        return (a.max_user_num_ > b.max_user_num_);
    }
}
bool SortNumber(const PartyRoomInfo &a, const PartyRoomInfo &b)
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (battlezone2_list == NULL)
    {
        return false;
    }

    if (battlezone2_list->order_by())
    {
        return  (a.room_key_ < b.room_key_);
    }
    else
    {
        return  (a.room_key_ >= b.room_key_);
    }
}
bool SortState(const PartyRoomInfo &a, const PartyRoomInfo &b)
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (battlezone2_list == NULL)
    {
        return false;
    }

    if (a.is_mission_start_ == b.is_mission_start_)// 같으면 남은 인원수
    {
        return SortMember(a, b);
    }

    if (battlezone2_list->order_by())
    {
        return  (a.is_mission_start_ < b.is_mission_start_);
    }
    else
    {
        return  (a.is_mission_start_ >= b.is_mission_start_);
    }
}
bool SortType(const PartyRoomInfo &a, const PartyRoomInfo &b)
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (battlezone2_list == NULL)
    {
        return false;
    }

    if (a.base_party_room_info.party_room_type_ == b.base_party_room_info.party_room_type_)// 같으면 진행여부
    {
        return SortState(a, b);
    }

    if (battlezone2_list->order_by())
    {
        return  (a.base_party_room_info.party_room_type_ < b.base_party_room_info.party_room_type_);
    }
    else
    {
        return  (a.base_party_room_info.party_room_type_ >= b.base_party_room_info.party_room_type_);
    }
}
bool SortTitle(const PartyRoomInfo &a, const PartyRoomInfo &b)
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (battlezone2_list == NULL)
    {
        return false;
    }

    if (strcmp(a.base_party_room_info.party_room_title_, b.base_party_room_info.party_room_title_) == 0)// 같으면 진행여부
    {
        return SortState(a, b);
    }

    if (battlezone2_list->order_by())
    {
        return (strcmp(a.base_party_room_info.party_room_title_, b.base_party_room_info.party_room_title_) < 0);
    }
    else
    {
        return (strcmp(a.base_party_room_info.party_room_title_, b.base_party_room_info.party_room_title_) >= 0);
    }
}
bool SortMapName(const PartyRoomInfo &a, const PartyRoomInfo &b)
{
    uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
    if (battlezone2_list == NULL)
    {
        return false;
    }

    TCHAR first_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR second_string[INTERFACE_STRING_LENGTH] = {0,};
    const sMAPINFO* first_mapInfo = MapInfoParser::Instance()->FindMapInfo(a.base_party_room_info.map_code_);
    if (first_mapInfo)
    {
        g_InterfaceManager.GetInterfaceString(first_mapInfo->dwNCode, first_string, INTERFACE_STRING_LENGTH);
    }
    else
    {
        //788 미션 없음
        g_InterfaceManager.GetInterfaceString(788, first_string, INTERFACE_STRING_LENGTH);
    }

    const sMAPINFO* second_mapInfo = MapInfoParser::Instance()->FindMapInfo(b.base_party_room_info.map_code_);
    if (second_mapInfo)
    {
        g_InterfaceManager.GetInterfaceString(second_mapInfo->dwNCode, second_string, INTERFACE_STRING_LENGTH);
    }
    else
    {
        //788 미션 없음
        g_InterfaceManager.GetInterfaceString(788, second_string, INTERFACE_STRING_LENGTH);
    }

    if (strcmp(first_string, second_string) == 0)// 같으면 남은 진행여부
    {
        return SortState(a, b);
    }

    if (battlezone2_list->order_by())
    {
        return (strcmp(first_string, second_string) < 0);
    }
    else
    {
        return (strcmp(first_string, second_string) >= 0);
    }
}

void uiBattlezone2List::SortList(int sort_by, bool order_by)
{
    bool (*SortByFunction)(const PartyRoomInfo& a, const PartyRoomInfo& b);

    if(battlezone_list_.size() <= 1)
    {
        return;
    }

    last_sort_by_function_ = sort_by;
    order_by_ = order_by;

    switch (sort_by)
    {
    case kControl_Button_B012:
        SortByFunction = SortNumber;
        break;
    case kControl_Button_B003:
        SortByFunction = SortType;
        break;
    case kControl_Button_B011:
        SortByFunction = SortState;
        break;
    case kControl_Button_B013:
        SortByFunction = SortTitle;
        break;
    case kControl_Button_B014:
        SortByFunction = SortMember;
        break;
    case kControl_Button_B015:
        SortByFunction = SortMapName;
        break;
    default:
        SortByFunction = NULL;
        break;
    }

    if (SortByFunction != NULL)
    {
        std::sort(battlezone_list_.begin(), battlezone_list_.end(), SortByFunction);
        SetBattlezoneList();
    }
}
//------------------------------------------------------------------------------
bool uiBattlezone2List::OnLButtonClick(SI_MESSAGE* message)
{
    bool is_find_button = false;
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004: // 닫기
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B000://입장예약
        {
            ShowBattlezoneTree(uiBattlezone2Tree::kReservationMode);
        }
        break;
    case kControl_Button_B010://방만들기
        {
            ShowBattlezoneTree(uiBattlezone2Tree::kCreateMode);
        }
        break;
    case kControl_Button_B001://앞 페이지
        {
            if (is_searched_ == false && current_page_ > 1)
            {
                --current_page_;
                RequestBattlezoneList(true);
            }
        }
        break;
    case kControl_Button_B002://뒤 페이지
        {
            if (is_searched_ == false && current_page_ < max_page_)
            {
                ++current_page_;
                RequestBattlezoneList(true);
            }
        }
        break;
    case kControl_Button_B005:
        {
            //검색 옵션 팝업
            // 0636 아이디로 찾기
            // 0575 번호로 찾기
        }
        break;
    case kControl_VScroll_V000:
        {
            is_find_button = true;
        }
        break;
    case kControl_Button_B006:
        {
            if (dropdown_class_filter().CurrentUserData() == BATTLEZONE2_FIND_BY_MAP_NAME)
            {
                is_find_button = true;
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                if (edit_control_)
                {
                    edit_control_->GetTextWZ(string, INTERFACE_STRING_LENGTH-1);
                }
                
                if (lstrcmp (string, _T("")))
                {
                    is_searched_ = true;
                    SearchMap(string, true);
                }
            }
            else
            {
                OnSearch();
            }
        }
        break;
    case kControl_Button_B007:
        {
            ToggleViewRoom(true);
        }
        break;
    case kControl_Button_B008://새로고침
        {
            is_searched_ = false;
            ShowWaitingButton(true);
            RequestBattlezoneList();

            if (edit_control_ && static_edit_control_)//드랍박스가 변경되면 검색어 초기화
            {
                edit_control_->SetTextWZ("");
                static_edit_control_->SetTextWZ("");
            }
        }
        break;
    case kControl_Button_B009:
        {
            ToggleViewRoom(false);
        }
        break;
    case kControl_Edit_E000://입력창
        {
            ShowEditControl(true);
        }
        break;
    case kControl_List_L000:
        {
            /*int a = 0;
            if (list_control_)
            {
                int index = list_control_->GetCurSel();
                EnterRoom(index);
            }*/
        }
        break;
    case kControl_List_L001:
        {
        }
        break;
    case kControl_Button_B012:
    case kControl_Button_B003:
    case kControl_Button_B011:
    case kControl_Button_B013:
    case kControl_Button_B014:
    case kControl_Button_B015:
        SortList(GetControlPosition_byID(message->dwCtrlID), !order_by());
        break;
    case kControl_List_L002:
        {   // 검색어 추가해주기
            if (find_list_ptr_ && edit_control_)
            {
                int select_index = find_list_ptr_->GetCurSel();

                if (select_index >= 0)
                {
                    BATTLEZONE_MAP_INFO select_map_name_info = searched_results_[select_index];
                    SendMapFind(&select_map_name_info);
                }
            }
        }
        break;
    default:   
        {
        }
        break;
    }

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
        ShowEditControl(false);
    }
    
    if (is_find_button == false)
    {
        ShowFindWords(false);
    }

    return result;
}

void uiBattlezone2List::SendMapFind(BATTLEZONE_MAP_INFO* select_map_name_info)
{
    TCHAR select_map_name[INTERFACE_STRING_LENGTH] = {0,};
    DWORD map_code = select_map_name_info->map_code;
    const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)map_code);

    if (map_info)
    {
        if (select_map_name_info->type == PartyOfTopOfTrialByCash
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
            || (select_map_name_info->type == PartyOfTopOfTrialLight_cash)
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
            )
        {
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(map_info->dwNCode, temp_string);
            // 2443 (바로입장)
            g_InterfaceManager.GetInterfaceString(2443, string);
            sprintf(select_map_name, "%s%s", temp_string, string);
        }
        else
        {
            g_InterfaceManager.GetInterfaceString(map_info->dwNCode, select_map_name);
        }
        GlobalFunc::SetText(edit_control_, select_map_name);
        edit_control_->SetTextWZ(select_map_name);
    }

    //패킷
    MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN send_packet;
    send_packet.request_map_code_ = static_cast<MAPCODE>(map_code);
    send_packet.party_room_type_ = static_cast<PartyZoneType>(select_map_name_info->type);
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

    ShowAnimation(true);
    ShowFindWords(false);
}

void uiBattlezone2List::ShowWaitingButton(bool is_show)
{
    CCtrlButtonWZ* button = NULL;
    button = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B009)));
    if (button)
    {
        button->ShowWindowWZ(is_show?WZ_SHOW:WZ_HIDE);
    }

    button = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B007)));
    if (button)
    {
        button->ShowWindowWZ(is_show?WZ_HIDE:WZ_SHOW);
    }
}

void uiBattlezone2List::ToggleViewRoom(bool is_show)
{
    ShowWaitingButton(is_show);
    if (is_show)
    {
        //if (is_fast == false)//긴급 데이터가 아니라면
        {
            if (list_refresh_timer_.SpentTime() < 1200)// 1.2초 이내에 재요청 불가
            {
                ShowAnimation(true);
                return;

            }
        }
        is_searched_ = false;
        RequestBattlezoneList();
    }
    else
    {
        ShowAnimation(true);
        is_searched_ = true;
        MSG_CG_PARTY_FIND_WAITING_ROOM_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }

    if (edit_control_ && static_edit_control_)//드랍박스가 변경되면 검색어 초기화
    {
        edit_control_->SetTextWZ("");
        static_edit_control_->SetTextWZ("");
    }
}

void uiBattlezone2List::RequestBattlezoneList(bool is_fast /* = false */)
{
    if (is_fast == false)//긴급 데이터가 아니라면
    {
        if (list_refresh_timer_.SpentTime() < 1200)// 1.2초 이내에 재요청 불가
        {
            ShowAnimation(true);
            return;
        }
    }

    if (vscroll_control())
    {
        last_scroll_position_ = vscroll_control()->GetScrollPosWZ();
    }

    ShowAnimation(true);
#ifdef _DH_BATTLEZONE2_LIST_PAGE
    MSG_CG_PARTY_ROOM_LIST_SYN send_packet;
    send_packet.request_page_ = current_page_;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
#else
    MSG_CG_PARTY_BATTLE_ZONE_INFO_SYN send_packet;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
#endif//_DH_BATTLEZONE2_LIST_PAGE
    list_refresh_timer_.Off();
    list_refresh_timer_.On(c_iCYCLE_REFRESH_ROOM_LIST);
}

void uiBattlezone2List::ShowBattlezoneLobby()
{
    uiBattlezone2Lobby* battlezone_lobby_dialog = 
        GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
    if (battlezone_lobby_dialog == NULL)
    {
        return;
    }

    UserSlot EnterUser;
    EnterUser.SetPlayerKey(g_pHero->GetObjectKey());

    if (ui_battlezone2_manager_)
    {
        ui_battlezone2_manager_->set_battlezone_room_state(uiBattlezone2Man::ROOM_STATE_LOBBY);
    }
    ShowInterface(FALSE);
    battlezone_lobby_dialog->ShowInterfaceWithSound(TRUE);
}

void uiBattlezone2List::SearchResult(DWORD result_code, PartyRoomInfo* party_room_info)
{
    switch (result_code)
    {
    case RC::RC_PARTY_SUCCESS:
        {
            battlezone_list_.clear();
            battlezone_list_.push_back(*party_room_info);
            SetBattlezoneList();
            if (vscroll_control())
            {
#ifdef _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
				int mmin = 0, mmax = 0;
				vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
				if (last_scroll_position_ <= mmax && last_scroll_position_ >= mmin)
				{
					vscroll_control()->SetScrollPosWZ(last_scroll_position_);
				}
				else
				{
					last_scroll_position_ = 0;
					vscroll_control()->SetScrollPosWZ(last_scroll_position_);
				}
				GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
#else
                vscroll_control()->SetScrollPosWZ(last_scroll_position_);
#endif // _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
            }
            is_searched_ = true;
        }
        break;
    case RC::RC_PARTY_NOT_FOUND_USER:
        {
            battlezone_list_.clear();
            SetBattlezoneList();
            if (vscroll_control())
            {
#ifdef _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
				int mmin = 0, mmax = 0;
				vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
				if (last_scroll_position_ <= mmax && last_scroll_position_ >= mmin)
				{
					vscroll_control()->SetScrollPosWZ(last_scroll_position_);
				}
				else
				{
					last_scroll_position_ = 0;
					vscroll_control()->SetScrollPosWZ(last_scroll_position_);
				}
				GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
#else
                vscroll_control()->SetScrollPosWZ(last_scroll_position_);
#endif // _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
            }
            is_searched_ = true;
        }
        break;
    default:
        {
            is_searched_ = false;
        }
    }
}

void uiBattlezone2List::RecvBattlezoneMapNameList(MSG_BASE* message_ptr)
{
    ShowAnimation(false);
    //배틀존 리스트
    MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK* recv_packet = (MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK*)message_ptr;

#ifdef _DEV_VER
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK:%d개"), recv_packet->number_of_list_);
#endif
    battlezone_list_.clear();

    for (int i = 0; i < recv_packet->number_of_list_; ++i)
    {
        battlezone_list_.push_back(recv_packet->party_room_info_[i]);
    }

    SetPage(1,1);//current_page_, recv_packet->all_page_num_);

    //정렬
    SortList(last_sort_by_function_, order_by());

    SetBattlezoneList();
    if (vscroll_control())
    {
#ifdef _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
		int mmin = 0, mmax = 0;
		vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
		if (last_scroll_position_ <= mmax && last_scroll_position_ >= mmin)
		{
			vscroll_control()->SetScrollPosWZ(last_scroll_position_);
		}
		else
		{
			last_scroll_position_ = 0;
			vscroll_control()->SetScrollPosWZ(last_scroll_position_);
		}
		GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
#else
        vscroll_control()->SetScrollPosWZ(last_scroll_position_);
#endif // _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
    }
}

void uiBattlezone2List::RecvBattlezoneWatingList(MSG_BASE* message_ptr)
{
    ShowAnimation(false);
    //배틀존 리스트
    MSG_CG_PARTY_FIND_WAITING_ROOM_ACK* recv_packet = (MSG_CG_PARTY_FIND_WAITING_ROOM_ACK*)message_ptr;

#ifdef _DEV_VER
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_PARTY_FIND_WAITING_ROOM_ACK:%d개"), recv_packet->number_of_list_);
#endif
    battlezone_list_.clear();

#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#else
    //if (is_ssq_open_)//타락한 사원 입력
    //{
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        PartyRoomInfo ssq_room_info;
        ssq_room_info.current_user_num_ = 0;
        ssq_room_info.is_mission_start_ = false;
        ssq_room_info.link_party_key_ = 0xFFFF;
        ssq_room_info.max_user_num_ = 15;
        ssq_room_info.room_key_ = BASE_ROOM_KEY;
        ssq_room_info.base_party_room_info.map_code_ = 12001;//타락한 사원 맵코드
        sprintf(ssq_room_info.base_party_room_info.party_room_password_, "");

        g_InterfaceManager.GetInterfaceString(55015,string);//타락한 사원
        g_SsqScene.Func_Output_with_internal_timer(message, true);
        Snprintf(ssq_room_info.base_party_room_info.party_room_title_,MAX_ROOMTITLE_LENGTH, "%s %s", string, message);

        ssq_room_info.base_party_room_info.party_room_type_ = PartyOfNone;
        ssq_room_info.base_party_room_info.special_type_ = eROOM_SPECIAL_NORMAL;
        battlezone_list_.push_back(ssq_room_info);
    //}
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION

    for (int i = 0; i < recv_packet->number_of_list_; ++i)
    {
        //for (int j = 0; j < 30; ++j)//리스트 갯수 증가 test용 코드
        {
            battlezone_list_.push_back(recv_packet->party_room_info_[i]);
        }
    }

    //current_page_ = current_page_;
    //max_page_ = recv_packet->all_page_num_;
    SetPage(1,1);//current_page_, recv_packet->all_page_num_);

    //정렬
    SortList(last_sort_by_function_, order_by());

    //if (is_searched_ == false)
    {
        SetBattlezoneList();
        if (vscroll_control())
        {
#ifdef _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
			int mmin = 0, mmax = 0;
			vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
			if (last_scroll_position_ <= mmax && last_scroll_position_ >= mmin)
			{
				vscroll_control()->SetScrollPosWZ(last_scroll_position_);
			}
			else
			{
				last_scroll_position_ = 0;
				vscroll_control()->SetScrollPosWZ(last_scroll_position_);
			}
			GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
#else
            vscroll_control()->SetScrollPosWZ(last_scroll_position_);
#endif // _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
        }
    }
}

void uiBattlezone2List::RecvBattlezoneList(MSG_BASE* message_ptr)
{
    ShowAnimation(false);
    //배틀존 리스트
#ifdef _DH_BATTLEZONE2_LIST_PAGE
    MSG_CG_PARTY_ROOM_LIST_ACK* recv_packet = (MSG_CG_PARTY_ROOM_LIST_ACK*)message_ptr;
#else
    MSG_CG_PARTY_BATTLE_ZONE_INFO_ACK* recv_packet = (MSG_CG_PARTY_BATTLE_ZONE_INFO_ACK*)message_ptr;
#endif//_DH_BATTLEZONE2_LIST_PAGE

#ifdef _DEV_VER
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("CG_PARTY_ROOM_LIST_ACK or CG_PARTY_BATTLE_ZONE_INFO_ACK:%d개"), recv_packet->number_of_list_);
#endif
    battlezone_list_.clear();

#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
#else
    //if (is_ssq_open_)//타락한 사원 입력
    //{
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        PartyRoomInfo ssq_room_info;
        ssq_room_info.current_user_num_ = 0;
        ssq_room_info.is_mission_start_ = false;
        ssq_room_info.link_party_key_ = 0xFF;
        ssq_room_info.max_user_num_ = 15;
        ssq_room_info.room_key_ = BASE_ROOM_KEY;
        ssq_room_info.base_party_room_info.map_code_ = 12001;//타락한 사원 맵코드
        sprintf(ssq_room_info.base_party_room_info.party_room_password_, "");
        g_InterfaceManager.GetInterfaceString(55015,string);
        g_SsqScene.Func_Output_with_internal_timer(message, true);
        Snprintf(ssq_room_info.base_party_room_info.party_room_title_,MAX_ROOMTITLE_LENGTH, "%s %s", string, message);

        ssq_room_info.base_party_room_info.party_room_type_ = PartyOfNone;
        ssq_room_info.base_party_room_info.special_type_ = eROOM_SPECIAL_NORMAL;
        battlezone_list_.push_back(ssq_room_info);
    //}
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION

    for (int i = 0; i < recv_packet->number_of_list_; ++i)
    {
        //for (int j = 0; j < 15; ++j)
        {
            battlezone_list_.push_back(recv_packet->party_room_info_[i]);
        }
    }

    //current_page_ = current_page_;
    max_page_ = recv_packet->all_page_num_;
    SetPage(current_page_, recv_packet->all_page_num_);

    //정렬
    SortList(last_sort_by_function_, order_by());

    if (is_searched_ == false)
    {
        SetBattlezoneList();
        if (vscroll_control())
        {
#ifdef _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
			int mmin = 0, mmax = 0;
			vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
			if (last_scroll_position_ <= mmax && last_scroll_position_ >= mmin)
			{
				vscroll_control()->SetScrollPosWZ(last_scroll_position_);
			}
			else
			{
				last_scroll_position_ = 0;
				vscroll_control()->SetScrollPosWZ(last_scroll_position_);
			}
			GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(vscroll_control()->GetScrollPosWZ());
#else
            vscroll_control()->SetScrollPosWZ(last_scroll_position_);
#endif // _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG
        }
    }
}

void uiBattlezone2List::SetPage(int current_page, int max_page)//ui에 갱신
{
    if (max_page <= 0)
    {
        max_page = 1;
    }

    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(string, _T("%d/%d"), current_page, max_page);
    if (static_page_control_)
    {
        static_page_control_->SetTextWZ(string);
    }
}

void uiBattlezone2List::NetworkProc(MSG_BASE* message_ptr)
{
    //배틀존 패킷만 처리한다
    if (message_ptr->m_byCategory != CG_PARTY)
    {
        return;
    }

    switch (message_ptr->m_byProtocol)
    {
    case CG_PARTY_INVITE_PARTY_ROOM_REQ:
        {
            TCHAR format_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR master_name[INTERFACE_STRING_LENGTH] = {0,};
            uiSystemMan* system_manager =
                static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
            uiPartyMan* party_manager =
                static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

            MSG_CG_PARTY_INVITE_PARTY_ROOM_REQ* recv_packet = (MSG_CG_PARTY_INVITE_PARTY_ROOM_REQ*)message_ptr;
            if(system_manager && party_manager)
            {
                DWORD master_key = party_manager->GetMasterKey();

                uicPartyMemberInfoPlus* member_info = party_manager->GetPartyMemberInfo(party_manager->GetMasterKey());
                if (member_info)
                {
                    sprintf(master_name, member_info->Get().m_szCharName);
                }

                // 2483 %s님이 배틀존에 방을 생성하셨습니다. 따라 가시겠습니까?
                g_InterfaceManager.GetInterfaceString(2483, string, INTERFACE_STRING_LENGTH);
                
                Snprintf(format_string, INTERFACE_STRING_LENGTH, string, master_name);
                follow_party_key_ = recv_packet->invite_party_key_;
                system_manager->Sys_Confirm(format_string, GlobalFunc::PartyContinueConfirm, NULL, NULL, 100/*확인*/, 101/*취소*/);
            }
        }
        break;
    case CG_PARTY_FIND_ROOM_BY_MISSION_NAME_ACK:
        {
            RecvBattlezoneMapNameList(message_ptr);
            ShowWaitingButton(true);
            ShowEditControl(false);
        }
        break;
    case CG_PARTY_FIND_ROOM_BY_CHARID_ACK:
        {
            MSG_CG_PARTY_FIND_ROOM_BY_CHARID_ACK* recv_packet = (MSG_CG_PARTY_FIND_ROOM_BY_CHARID_ACK*)message_ptr;
            SearchResult(recv_packet->result_code_, &recv_packet->party_room_info_);
            ShowWaitingButton(true);
            ShowEditControl(false);
        }
        break;
    case CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_ACK:
        {
            MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_ACK* recv_packet = (MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_ACK*)message_ptr;
            SearchResult(recv_packet->result_code_, &recv_packet->party_room_info_);
            ShowWaitingButton(true);
            ShowEditControl(false);
        }
        break;
    case CG_PARTY_FIND_WAITING_ROOM_ACK:
        {
            RecvBattlezoneWatingList(message_ptr);
        }
        break;
    case CG_PARTY_ROOM_LIST_ACK:
        {
            RecvBattlezoneList(message_ptr);//페이지 요청시는 여기서 리스트를 처리
        }
        break;
    case CG_PARTY_BATTLE_ZONE_INFO_ACK:      
        {
#ifdef _DH_BATTLEZONE2_LIST_PAGE
            //페이지 요청시 전체 페이지 처리 및 추후 필요한 리스트 정보처리
            MSG_CG_PARTY_BATTLE_ZONE_INFO_ACK* recv_packet = (MSG_CG_PARTY_BATTLE_ZONE_INFO_ACK*)message_ptr;
            max_page_ = recv_packet->all_page_num_;
            SetPage(current_page_, max_page_);
#else
            RecvBattlezoneList(message_ptr); //페이지 요청이 아닌 경우 여기서 리스트를 처리
#endif//_DH_BATTLEZONE2_LIST_PAGE
        }
        break;
    case CG_PARTY_JOIN_ROOM_ACK:
        {
            MSG_CG_PARTY_JOIN_ROOM_ACK* recv_packet = (MSG_CG_PARTY_JOIN_ROOM_ACK*)message_ptr;

            if (recv_packet->result_code_ != RC::RC_PARTY_SUCCESS)
            {
                uiPartyMan* party_manager =
                    static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                if (party_manager)
                {
                    party_manager->Reason(recv_packet->result_code_);
                }
#ifdef _DEV_VER
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("CG_PARTY_JOIN_ROOM_ACK: result_code = %d"), recv_packet->result_code_);
#endif
                return;
            }
            
            if (!g_pHero)
                return;

#ifdef _DH_BATTLEZONE2_LOBBY_SLOT
            uiPartyMan* partyMan =
                static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            DWORD master_key = partyMan->GetMasterKey();
            if (ui_battlezone2_manager_ && partyMan)
            {
                ui_battlezone2_manager_->SetLobbyMasterPlayerKey(partyMan->GetMasterKey());
            }
            else
            {
                assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_JOIN_ROOM_ACK): Can't not find master key");
                return;
            }

            UserSlot EnterUser;
            EnterUser.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            EnterUser.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            ui_battlezone2_manager_->AddPlayer(EnterUser);

            UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(EnterUser.GetPlayerKey());
            if (user_slot != NULL)
            {
                if( user_slot->GetBattleZonePlayerKey() == 0 )
                {
                    DWORD battlezone_object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
                    Player* player = GameFunc::MakePlayer(battlezone_object_key, g_pHero, true);
                    if (player != NULL)
                    {
                        user_slot->SetBattleZonePlayerKey(battlezone_object_key);
                    }
                }
            }

            // 나를 제외한 나머지 플레이어
            if (ui_battlezone2_manager_ && partyMan)
            {
                for (int i = 0; i < partyMan->GetPartyNum(); ++i)
                {
                    const uicPartyMemberInfoPlus member_info = partyMan->GetParty(i);
                    const _PARTY_MEMBER_INFO party_member_info = member_info.Get();

                    DWORD member_key = party_member_info.m_dwMemberKey;
                    if (member_key != 0 && member_key != g_pHero->GetObjectKey())
                    {
                        UserSlot EnterUser;
                        EnterUser.SetPlayerKey(member_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        EnterUser.SetPlayerName(party_member_info.m_szCharName);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        ui_battlezone2_manager_->AddPlayer(EnterUser);
                    }
                }
            }
#else
            uiPartyMan* partyMan =
                static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            DWORD master_key = partyMan->GetMasterKey();
            if (ui_battlezone2_manager_ && partyMan)
            {
                ui_battlezone2_manager_->SetLobbyMasterPlayerKey(partyMan->GetMasterKey());
                UserSlot EnterUser;
                EnterUser.SetPlayerKey(partyMan->GetMasterKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                EnterUser.SetPlayerName(partyMan->GetPartyLeader()->Get().m_szCharName);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                ui_battlezone2_manager_->AddPlayer(EnterUser);
            }
            else
            {
                assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_JOIN_ROOM_ACK): Can't not find master key");
                return;
            }

            UserSlot EnterUser;
            EnterUser.SetPlayerKey(g_pHero->GetObjectKey());
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            EnterUser.SetPlayerName(g_pHero->GetName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            ui_battlezone2_manager_->AddPlayer(EnterUser);

            UserSlot* user_slot = ui_battlezone2_manager_->FindPlayer(EnterUser.GetPlayerKey());
            if (user_slot != NULL)
            {
                if( user_slot->GetBattleZonePlayerKey() == 0 )
                {
                    DWORD battlezone_object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
                    Player* player = GameFunc::MakePlayer(battlezone_object_key, g_pHero, true);
                    if (player != NULL)
                    {
                        user_slot->SetBattleZonePlayerKey(battlezone_object_key);
                    }
                }
            }

            // 파티장, 나를 제외한 나머지 플레이어
            if (ui_battlezone2_manager_ && partyMan)
            {
                for (int i = 0; i < partyMan->GetPartyNum(); ++i)
                {
                    const uicPartyMemberInfoPlus member_info = partyMan->GetParty(i);
                    const _PARTY_MEMBER_INFO party_member_info = member_info.Get();

                    DWORD member_key = party_member_info.m_dwMemberKey;
                    if (member_key != 0 && member_key != g_pHero->GetObjectKey() && member_key != master_key)
                    {
                        UserSlot EnterUser;
                        EnterUser.SetPlayerKey(member_key);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        EnterUser.SetPlayerName(party_member_info.m_szCharName);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                        ui_battlezone2_manager_->AddPlayer(EnterUser);
                    }
                }
            }
#endif//_DH_BATTLEZONE2_LOBBY_SLOT
            ShowBattlezoneLobby();  

            uiBattlezone2Lobby* battlezone_lobby_dialog = 
                GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
            if (battlezone_lobby_dialog == NULL)
            {
                assert(!"uiBattlezone2List::NetworkProc(CG_PARTY_JOIN_ROOM_ACK): Can't not find lobby dialog");
                return;
            }
            else
            {
                ui_battlezone2_manager_->set_current_room_info(&recv_packet->party_room_info_);
                battlezone_lobby_dialog->SetBaseInfo(ui_battlezone2_manager_->current_room_info());
            }

            ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
            if (chat_dialog)
            {
                chat_dialog->OnPartyChat();
            }
        }
    }
}

void uiBattlezone2List::SetIsSSQOpen(bool val)
{
    is_ssq_open_ = val;
    RequestBattlezoneList(true);//리스트 재요청
}

void uiBattlezone2List::RenderIcon()
{
    g_pSunRenderer->Set2DMode(TRUE);

    std::vector<PartyRoomInfo>::iterator begin = battlezone_list_.begin();
    std::vector<PartyRoomInfo>::iterator end = battlezone_list_.end();

    int list_count = -1;
    const int c_zoneListHeight = 20;
    const int c_zoneIconHeight = 16;
    const int c_zoneIconGap_height = 2;
    const int c_zoneIconGap = 2; // 슈퍼방장 아이콘과 모두획득 아이콘 사이 간격
    
    for (; begin != end; ++begin)
    {
        PartyRoomInfo data = (*begin);

        list_count += 1;

        if (is_searched_ && search_count_ < list_count)
        {
            break;
        }
        int scroll_pos = 0;
        if (vscroll_control() && (scroll_pos = vscroll_control()->GetScrollPosWZ()) > list_count)
        {
            continue;
        }
        if ((scroll_pos + c_zoneListCount) < list_count)//20개 까지만 출력
        {
            break;
        }

        RECT rect = list_control_->GetSizeRect();
        rect.left += (kListWidth[0] + kListWidth[1] + kListWidth[2]); // 제목위치에 보내기
        rect.top += ((list_count - scroll_pos) * c_zoneListHeight) + c_zoneIconGap_height;
        rect.right = rect.left + c_zoneIconHeight;
        rect.bottom = rect.top + c_zoneIconHeight;

        // 슈퍼방장이나 락 아이콘 그리기
        if (strlen(data.base_party_room_info.party_room_password_))
        {
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_LOCKED_ROOM,
                &rect);
            rect.left += c_zoneIconHeight + c_zoneIconGap;
            rect.right = rect.left + c_zoneIconHeight;
        }
        else if (data.base_party_room_info.special_type_ == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
        {
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
                &rect);
            rect.left += c_zoneIconHeight + c_zoneIconGap;
            rect.right = rect.left + c_zoneIconHeight;
        }
        else if (data.base_party_room_info.special_type_ == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
        {
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
                &rect);
            rect.left += c_zoneIconHeight + c_zoneIconGap;
            rect.right = rect.left + c_zoneIconHeight;
        }

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        // 모두분배 아이콘 그리기
        MAPCODE map_code = data.base_party_room_info.map_code_;
        if ((map_code != 0) &&
            (BattlezoneMissionTreeParser::Instance()->GetObtainType(
            map_code,data.base_party_room_info.party_room_type_) == kObtainType_Enable))
        {
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_PARTY_OBTAIN_ROOM,
                &rect);
        }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

        
//#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
//            RECT rect_guild_pvp = rt;
//#ifndef  __CHINA_PCBANG_SUPER_ROOM_BLOCK
//        if( battlezone_list_[i]->IsSpecialType(eROOM_SPECIAL_PCBANG) )
//        {
//            g_InterfaceManager.RenderBattleRoomIcon(
//                InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
//                &rt);
//            rect_guild_pvp.left  += 20;
//            rect_guild_pvp.right += 20;
//        }
//        else
//#endif
//            if ( pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_ITEM) )
//            {
//                g_InterfaceManager.RenderBattleRoomIcon(
//                    InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
//                    &rt);
//                rect_guild_pvp.left  += 20;
//                rect_guild_pvp.right += 20;
//            }
//
//            if( pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
//            {
//                g_InterfaceManager.RenderBattleRoomIcon(
//                    InterfaceManager::eBATTLE_ICON_GUILD_PVP_ROOM,
//                    &rect_guild_pvp);
//            }
//#else
//        switch ( pBaseRoomInfo->m_SpecialType )
//        {
//#ifndef  __CHINA_PCBANG_SUPER_ROOM_BLOCK
//        case eROOM_SPECIAL_PCBANG_SUPER_ROOM:
//            {
//                  g_InterfaceManager.RenderBattleRoomIcon(
//                      InterfaceManager::eBATTLE_ICON_PCBANG_SUPERMASTER_ROOM,
//                      &rt);
//            }
//            break;
//#endif
//        case eROOM_SPECIAL_ITEM1_SUPER_ROOM:
//            {
//                g_InterfaceManager.RenderBattleRoomIcon(
//                    InterfaceManager::eBATTLE_ICON_ITEM_SUPERMASTER_ROOM,
//                    &rt);
//            }
//            break;
//        }
//#endif

    }
}

void uiBattlezone2List::SearchMap(TCHAR* search_map, bool is_button /* = false */)
{
    static TCHAR last_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (edit_control_)
    {
        edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        g_InterfaceManager.Trim(string_buffer);
        if (dropdown_class_filter().CurrentUserData() == BATTLEZONE2_FIND_BY_MAP_NAME &&
            (is_button == true || edit_control_->GetState() == WZ_SHOW) && strlen(string_buffer) > 0)
        {
            ShowFindWords(true);

            //if (strcmp(last_string, string_buffer) != 0)
            {
                sprintf(last_string, string_buffer);
                FillFindListData(string_buffer);
            }
        }
        else
        {
            ShowFindWords(false);
        }
    }
}

void uiBattlezone2List::SearchID(TCHAR* search_id)
{
    MSG_CG_PARTY_FIND_ROOM_BY_CHARID_SYN sync;
    StrnCopy(sync.find_char_name_, search_id, MAX_CHARNAME_LENGTH);
    GlobalFunc::SendPacketToGameServer(&sync, sizeof(sync));
}

void uiBattlezone2List::SearchNo(WORD number)
{
#if 0 // 클라 자체 검색(안씀)
    std::list<PartyRoomInfo>::iterator begin = battlezone_list_.begin();
    std::list<PartyRoomInfo>::iterator end = battlezone_list_.end();

    int index = 0;

    list_control_->SetBeginDrawIndex(0);
    for (; begin != end; ++begin)
    {
        PartyRoomInfo data = (*begin);
        if (data.room_key_ == (number + BASE_ROOM_KEY))
        {
            battlezone_list_.clear();
            battlezone_list_.push_back(data);
            is_searched_ = true;
            SetBattlezoneList();
            return;
        }
    }

    if (begin == end)
    {
        battlezone_list_.clear();//못찾으면 클리어
        //is_searched_ = false;
        SetBattlezoneList();
    }
#else
    // 방 번호로 찾기 
    MSG_CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN sync;
    sync.find_room_key_ = (number + BASE_ROOM_KEY);
    GlobalFunc::SendPacketToGameServer(&sync, sizeof(sync));
#endif//
}

DWORD uiBattlezone2List::LoadMapNameCode()
{
    TCHAR map_name[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};

    static bool once_make_list = false;
    BYTE* menu_depth_count = BattlezoneMissionTreeParser::Instance()->max_menu_depth();
    const BattlezoneMissionTreeInfo* find_info;

    //트리 리스트 생성하기
    if (once_make_list == false)
    {
        once_make_list = true;
        for (BYTE LGroupCount = 1; LGroupCount <= menu_depth_count[0]; ++LGroupCount)
        {
            BATTLEZONE_MAP_INFO data;

            if (LGroupCount == 1)//1, 0, 0 미션없음 1회 예외처리
            {
                find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(LGroupCount, 0, 0);
                if (find_info)
                {
                    const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)find_info->map_code);
                    if (map_info != NULL)
                    {
                        g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name);
                    }
                    g_InterfaceManager.Trim(map_name);
                    data.map_code = static_cast<MAPCODE>(find_info->map_code);
                    data.type = find_info->kind_of_room;
                    sprintf(data.map_name, map_name);
                    map_infomations_.push_back(data);
                }
            }
            for (BYTE SGroupCount = 1; SGroupCount <= menu_depth_count[1]; ++SGroupCount)
            {
                for (BYTE OrderCount = 1; OrderCount <= menu_depth_count[2]; ++OrderCount)
                {
                    find_info = BattlezoneMissionTreeParser::Instance()->GetTreeMenu(LGroupCount, SGroupCount, OrderCount);
                    if (find_info != NULL)
                    {
                        const sMAPINFO* map_info= MapInfoParser::Instance()->FindMapInfo((MAPCODE)find_info->map_code);
                        if (map_info != NULL)
                        {
                            g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name);
                        }
                        g_InterfaceManager.Trim(map_name);
                        data.map_code = static_cast<MAPCODE>(find_info->map_code);
                        data.type = find_info->kind_of_room;
                        if (find_info->kind_of_room == PartyOfTopOfTrialByCash
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
                            || (find_info->kind_of_room == PartyOfTopOfTrialLight_cash)
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
                            )
                        {
                            // 2443 (바로입장)
                            g_InterfaceManager.GetInterfaceString(2443, string);
                            sprintf(data.map_name, "%s%s", map_name, string);
                        }
                        else
                        {
                            sprintf(data.map_name, map_name);
                        }
                        map_infomations_.push_back(data);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

void uiBattlezone2List::InsertFindList(const char* item_name)
{
    if (find_list_ptr_ == NULL)
    {
        return;
    }

    int index = find_list_ptr_->InsertItemBack();
#ifdef _INTERNATIONAL_UI
    DWORD dwListFont = StrToWzID("st10");
#else
    DWORD dwListFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

    TCHAR szTitle[MAX_VENDOR_TITLE_LENGTH+1] = {0,};
    Snprintf(szTitle , MAX_VENDOR_TITLE_LENGTH , item_name);
    GlobalFunc::SetItem(find_list_ptr_ , index , 1,  szTitle ,dwListFont ,DT_VCENTER| DT_CENTER ,
        RGBA(0,0,0,0), 
        RGBA(180,180,180,255), 
        RGBA(100,100,180,255));
}

void uiBattlezone2List::FillFindListData(TCHAR* map_name)
{    
    unsigned int search_count = 0;
    BATTLEZONE_MAP_INFO search_buffer;
    BATTLEZONE_MAP_INFO first_search_buffer;
    std::vector<BATTLEZONE_MAP_INFO>::iterator battlezone_find_iterator;

    g_InterfaceManager.Trim(map_name);
    int search_index_ = 0;

    //if (search_new_)  // 검색이 처음이라면
    {   // 새로 검색
        find_list_ptr_->DeleteItemAll(); // 기존에 리스트 모두 지워버린다.
        battlezone_find_iterator = map_infomations_.begin();
        /*search_index_ = 0;
        search_new_ = false;*/
        //ShowFindWords(true);
        searched_results_.clear();
    }

    while (battlezone_find_iterator != map_infomations_.end())
    {
        search_buffer.map_code = battlezone_find_iterator->map_code;
        sprintf(search_buffer.map_name, battlezone_find_iterator->map_name);
        search_buffer.type = battlezone_find_iterator->type;
        
        // 검색 알고리즘 적용
        if (strstr(search_buffer.map_name, map_name) != NULL)
        {
            if (search_index_ == 0)
            {
                first_search_buffer = search_buffer;
            }
            searched_results_.push_back(search_buffer);
            InsertFindList(search_buffer.map_name);
            find_list_ptr_->SetItemData(search_index_, &(searched_results_[search_index_]));
            ++search_index_;
        }
        // 검색 알고리즘 종료
        //++search_count;
        ++battlezone_find_iterator;
        /*if (search_count >= number_of_searching_per_a_frame_)
        {
            break;
        }*/       
    }

    const int find_size = 6;
    int list_size = search_index_ - find_size;

    if (find_vscroll_control())
    {
        find_vscroll_control()->SetScrollRangeWZ(0, (list_size>0)?list_size:0);
        find_vscroll_control()->SetScrollPosWZ(0);
    }

    if (search_index_ == 1)
    {
        ShowFindWords(false);
        SendMapFind(&first_search_buffer);
    }
    /*if (item_name_info_iterator_ == item_name_info_iterator_end_)
    {
        set_search_end(true);
        search_new_ = true;     //검색 시작 초기화
        if (search_index_ > 0 && find_list_ptr_ && m_pEdit)
        {
            _ITEM_NAME_INFO select_item_name_info = searched_results_[0];
            TCHAR select_item_name[MAX_VENDOR_SEARCH_LENGTH+1] = "";
            find_list_ptr_->SetCurSel(0);
            item_code_ = select_item_name_info.m_dwItemCode;
            strncpy(select_item_name, select_item_name_info.m_stName.c_str(), MAX_VENDOR_SEARCH_LENGTH);
            GlobalFunc::SetText(m_pEdit, select_item_name);
            m_pEdit->SetTextWZ(select_item_name);
        }
        else
        {
            HideFindWord();
        }
    }*/

    /*find_scroll_current_value_ = 0;
    find_scroll_value_max_ = search_index_ - MAX_VISIBLE_FINDLIST_NUM;
    // 스크롤 범위 지정.
    find_scroll_->SetScrollPosWZ(find_scroll_current_value_);
    find_scroll_->SetScrollRangeWZ(0,find_scroll_value_max_);*/
    find_list_ptr_->SetBeginDrawIndex(0/*find_scroll_current_value_*/);
}
#endif //_DH_BATTLEZONE2_