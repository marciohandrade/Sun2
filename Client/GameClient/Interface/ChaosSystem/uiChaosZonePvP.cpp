#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZonePVP.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/uiChaosZonePassword.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "GlobalFunc.h"
#include "Hero.h"


uiChaosZonePVP::uiChaosZonePVP(InterfaceManager* ui_manager) :
list_control_(NULL),
scroll_control_(NULL),
effect_texture_(INVALID_HANDLE_VALUE),
last_scroll_position_(0),
title_edit_control_(NULL),
title_static_control_(NULL),
password_edit_control_(NULL),
password_static_control_(NULL),
first_request_(true)
{
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V001"), kControl_VScroll_V001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);
}
//------------------------------------------------------------------------------
uiChaosZonePVP::~uiChaosZonePVP()
{
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::Init(CDrawBase* draw_base)
{
    //��ư ��Ȱ��ȭ
    CCtrlButtonCheckWZ* check_control = NULL;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // ��� ���� Ȱ��ȭ    
#else
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B001)));
    if (check_control)
    {
        check_control->EnableWindowWZ(false);
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B002)));
    if (check_control)
    {   
        check_control->EnableWindowWZ(false);
    }
#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    // ���� Ȱ��ȭ
#else
    // ���� ��Ȱ��ȭ
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B000)));
    if (check_control)
    {   
        check_control->EnableWindowWZ(false);
    }
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B003)));
    if (check_control)
    {   
        check_control->EnableWindowWZ(false);
    }
    check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B007)));
    if (check_control)
    {   
        check_control->EnableWindowWZ(false);
    }
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    // ��ũ��
    scroll_control_ = GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
    if (scroll_control_)
    {
        scroll_control_->SetScrollPosWZ(0);
        scroll_control_->SetScrollRangeWZ(0,1);
    }

    // ����Ʈ
    list_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000)));
    if (list_control_)
    {
        GlobalFunc::SetReportButtonInfo(list_control_, FREEPVP_LIST_NUM, 45, NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, FREEPVP_LIST_TITLE, 250, NULL);
        GlobalFunc::SetReportButtonInfo(list_control_, FREEPVP_LIST_MEMBER, 84, NULL); //W : 379

        list_control_->DeleteItemAll();
        list_control_->SetCurSel(-1);
        list_control_->SetColorSelBar(RGBA(74,53,15,255));
        list_control_->SetUseReport(true);
        list_control_->SetReportUseCnt(3);
        list_control_->SetHideReportRender(true);
        list_control_->SetSellHeight(free_pvp_list_height);
        list_control_->SetStartLineGapTop(2);
    }

    // ����Ʈ�ڽ� ����
    title_edit_control_ =
        static_cast<CCtrlDxEditBox*>(GetControlWZ(GetControlID_byPosition(kControl_Edit_E000)));
    if (title_edit_control_)
    {
        title_edit_control_->SetLimitText(MAX_ROOMTITLE_LENGTH-1);
    }
    title_static_control_ =
        static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S002)));
    if (title_static_control_)
    {
        title_static_control_->SetTextColor(RGBA(255,255,255,255));
    }
    password_edit_control_ =
        static_cast<CCtrlDxEditBox*>(GetControlWZ(GetControlID_byPosition(kControl_Edit_E001)));
    if (password_edit_control_)
    {
        password_edit_control_->SetNumberic(true);
    }
    password_static_control_ =
        static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S003)));
    if (password_static_control_)
    {
        password_static_control_->SetTextColor(RGBA(255,255,255,255));
    }

    // �游��� ��Ȱ��ȭ
    GetControlWZ(GetControlID_byPosition(kControl_Button_B006))->EnableWindowWZ(false);

    InitTextures();
    animation_refresh_timer_.On(refresh_animation_time);
    ShowAnimation(false);

}
//------------------------------------------------------------------------------
void uiChaosZonePVP::Release()
{
    if(effect_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(effect_texture_);
        effect_texture_ = INVALID_HANDLE_VALUE;
    }
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::InitTextures()
{
    // �ִϸ��̼� �ؽ��� 2�� �ε�
    effect_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\Loading.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

    // �ִϸ��̼� �ؽ��� ��ġ (12������) �����Ӵ� (40 * 40) 4 x 3 ��
    int index = 0;
    float width = 0.0f;
    float height = 0.0f;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            width = j * refresh_animation_frame_width;
            height = i * refresh_anumation_frame_height;

            texture_rectangles_[index].left = width;
            texture_rectangles_[index].top = height;
            texture_rectangles_[index].right = refresh_animation_frame_width;
            texture_rectangles_[index].bottom = refresh_anumation_frame_height;

            ++index;
            if (index >= refresh_animation_frame)
            {
                break;
            }
        }
    }
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::NetworkProc(struct MSG_BASE *message)
{
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::OnRenderSolarDialog()
{
    RenderTooltip();
    PlayAnimation();
    RenderIcon();
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::OnUpdateSolarDialog()
{
    // ���ΰ�ħ �ִϸ��̼� Ÿ�̸�
    if (animation_refresh_timer_.Check() != TIMER_STAT_NONE)
    {
        if (animation_refresh_timer_.Check() == TIMER_STAT_FINISH)
        {
            animation_refresh_timer_.Off();
            animation_refresh_timer_.On(refresh_animation_time);
        }
    }

    // ���ΰ�ħ ����Ʈ Ÿ�̸�
    if (list_refresh_timer_.Check() != TIMER_STAT_NONE)
    {
        if (list_refresh_timer_.Check() == TIMER_STAT_FINISH)
        {
            RequestRoomList();
        }
        else if (list_refresh_timer_.SpentTime() > 1200 && animatino_show_)
        {
            RequestRoomList();
        }
    }

    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    title_edit_control_->GetTextWZ(string, INTERFACE_STRING_LENGTH-1);
    if (_tcslen(string) == 0)
    {
        //��Ȱ��ȭ
        GetControlWZ(GetControlID_byPosition(kControl_Button_B006))->EnableWindowWZ(false);
    }
    else
    {
        //Ȱ��ȭ
        GetControlWZ(GetControlID_byPosition(kControl_Button_B006))->EnableWindowWZ(true);
    }

    VScrollDialog::Update();
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {
        //üũ��ư
        CCtrlButtonCheckWZ* check_control = NULL;
        for (int i = kControl_Button_B000;i <= kControl_Button_B007; ++i)
        {
            check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(i)));
            if (check_control)
            {
                check_control->SetCheckState(false);
            }
        }

        check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B004)));
        if (check_control)
        {
            check_control->SetCheckState(true);
        }

        SetPVPRoomList();

        if (first_request_) // �ּ��ϸ� ���� ������ ���ſ�û
        {
            first_request_ = false;
            RequestRoomList(true);
        }
        else
        {
            RequestRoomList();
        }

        list_refresh_timer_.On(refresh_room_list_time);

        ShowTitleEditControl(true); // ���� ���ָ� �⺻ �� ������ ���� �游��� Ȱ��ȭ
        ShowTitleEditControl(false);
        ShowPasswordEditControl(false);
    }
}
//------------------------------------------------------------------------------
void uiChaosZonePVP::MessageProc(SI_MESSAGE* message)
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
    case RT_MSG_LBUTTONDCLICK:
        OnDoubleButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollUp(message);
            }
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollDown(message);
            }
        }
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_THUMBPOSITION:   //!< ���� �巡������ ���콺�� ���Ҵ�.
    case RT_MSG_SB_ENDSCROLL:       //!< ��ũ���� ���� �Ǿ���.
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_VScroll_V001)
            {
                OnVScrollThumb(message);
            }
        }
        break;
    case RT_MSG_EDIT_TABKEYDOWN:
        {
            OnTapKeyDown(message);
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}


//------------------------------------------------------------------------------
bool uiChaosZonePVP::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    int tab_index = -1;

    uiChaosSystemMan* chaos_manager = static_cast<uiChaosSystemMan*>(GetMan());
    if (chaos_manager == NULL)
    {
        return false;
    }

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            chaos_manager->ShowChaosZone(IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP, false);
        }
        break;
    case kControl_Button_B005:
        {
            //�����ϱ�
            EnterRoom();
        }
        break;
    case kControl_Button_B006:
        {
            //�� �����
            CreateRoom();
        }
        break;
    case kControl_Button_B008:
        {
            //���ΰ�ħ
            RequestRoomList();
        }
        break;
    case kControl_Button_B000:
        {
            tab_index = 0;
        }
        break;
    case kControl_Button_B001:
        {
            tab_index = 1;
        }
        break;
    case kControl_Button_B002:
        {
            tab_index = 2;
        }
        break;
    case kControl_Button_B003:
        {
            tab_index = 3;
        }
        break;
    case kControl_Button_B004:
        {
            tab_index = 4;
        }
        break;
    case kControl_Button_B007:
        {
            tab_index = 5;
        }
        break;
    //case kControl_List_L000:
    //case kControl_Edit_E000:
    //case kControl_Edit_E001:

    default:
        {
        }
        break;
    }

    // Ŭ���� ���� ����Ʈ ��Ʈ�� ����
    RECT title_static_rect = title_static_control_->GetSizeRect();
    RECT password_static_rect = password_static_control_->GetSizeRect();
    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();
    if (mouse_x > title_static_rect.left && mouse_x < title_static_rect.right
        && mouse_y > title_static_rect.top && mouse_y < title_static_rect.bottom)
    {
        ShowTitleEditControl(true);
        ShowPasswordEditControl(false);
    }
    else if (mouse_x > password_static_rect.left && mouse_x < password_static_rect.right
        && mouse_y > password_static_rect.top && mouse_y < password_static_rect.bottom)
    {
        ShowTitleEditControl(false);
        ShowPasswordEditControl(true);
    }
    else
    {
        ShowTitleEditControl(false);
        ShowPasswordEditControl(false);
    }

    if (list_control_)
    {
	    // �����ϱ� ��ư
	    int index = list_control_->GetCurSel();
	    if (index == -1)
	    {
	        //��Ȱ��ȭ
	        GetControlWZ(GetControlID_byPosition(kControl_Button_B005))->EnableWindowWZ(false);
	    }
	    else
	    {
	        //Ȱ��ȭ
	        GetControlWZ(GetControlID_byPosition(kControl_Button_B005))->EnableWindowWZ(true);
	    }
    }
    // �� ��ư Ŭ�� ��
    if (tab_index != -1)
    {
        chaos_manager->OpenChaosZone(tab_index, true);
    }
    return result;
}

//-----------------------------------------------------------------------------
bool uiChaosZonePVP::OnDoubleButtonClick(SI_MESSAGE* message)
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
                    EnterRoom();
                }
            }
        }
        break;
    }
 
    return result;
}

//-----------------------------------------------------------------------------
void uiChaosZonePVP::RenderTooltip()
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager == NULL)
    {
        return;
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    // ���� Ȱ��ȭ
    int tap_controls[] = {
        kControl_Button_B000,
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        kControl_Button_B001,
#else
        0/*kControl_Button_B001*/,
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        0/*kControl_Button_B002*/,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B007
    };
#else
    // ���� ��Ȱ��ȭ
    int tap_controls[] = {
        0/*kControl_Button_B000*/,
        0 /*kControl_Button_B001*/,
        0 /*kControl_Button_B002*/,
        0/*kControl_Button_B003*/,
        kControl_Button_B004,
        0/*kControl_Button_B007*/
    };
#endif //_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    
    CControlWZ* control;
    for (int index = 0; index < sizeof(tap_controls)/sizeof(tap_controls[0]); ++index)
    {
        if (tap_controls[index] == 0)
        {
            continue;
        }
        control = GetControlWZ_byPosition(tap_controls[index]);
        if (control)
        {
            RECT position = control->GetSizeRect();
            if (position.left < mouse_x && position.right > mouse_x &&
                position.top < mouse_y && position.bottom > mouse_y)
            {
                // 2571 ~ 2576 / �̽������� ����~ ī���� �����丮
                tooltip_manager->RegisterTooltipWithStringCode(2571 + index);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}
//-----------------------------------------------------------------------------

void uiChaosZonePVP::SetPVPRoomList()
{
    if (list_control_ == NULL)
    {
        return;
    }

    int row = 0;
    if (list_control_ != NULL)
    {
        list_control_->DeleteItemAll();
        list_control_->SetCurSel(-1);
        list_control_->SetColorSelBar(RGBA(74,53,15,255));
        list_control_->SetUseReport(true);
        list_control_->SetReportUseCnt(3);
        list_control_->SetHideReportRender(true);
        
        list_control_->SetSellHeight(free_pvp_list_height);
        list_control_->SetStartLineGapTop(2);


        //=====================================================================================
        //	1.����Ʈ��Ʈ�� �����ͻ���
        //=====================================================================================
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};

        ChaosSystem::FreePVPData::FreePVPRoomContainer::iterator begin =
            ChaosSystem::GetFreePVPData().free_pvp_room_vector()->begin();
        ChaosSystem::FreePVPData::FreePVPRoomContainer::iterator end =
            ChaosSystem::GetFreePVPData().free_pvp_room_vector()->end();

        int index = 0;
        DWORD font_color = RGBA(255,255,255,255);
        list_control_->SetBeginDrawIndex(0);
        for (; begin != end; ++begin)
        {
            FreePVPRoomInfo data = (*begin);
            index = list_control_->InsertItemBack();
            list_control_->SetItemData(index, (void*)&(*begin));
            sprintf(string, "%d", (data.room_key - free_pvp_base_room_key)); //BASE_ROOM_KEY
            GlobalFunc::SetItem(list_control_, index, FREEPVP_LIST_NUM,
                string,
                StrToWzID("m212"),
                DT_RIGHT,
                RGBA(255,255,255,0),
                font_color, 
                font_color);

            sprintf(string, data.base_free_pvp_room_info.room_title);
            GlobalFunc::SetItem(list_control_, index, FREEPVP_LIST_TITLE,
                string,
                StrToWzID("m212"),
                DT_CENTER,
                RGBA(255,255,255,0),
                font_color, 
                font_color);

            sprintf(string, "%d/%d", data.current_user_count, data.max_user_count);
            GlobalFunc::SetItem(list_control_, index, FREEPVP_LIST_MEMBER,
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

    if (vscroll_control())
    {
        vscroll_control()->SetScrollPosWZ(last_scroll_position_);
    }
    
    // �����ϱ� ��ư ��Ȱ��ȭ
    GetControlWZ(GetControlID_byPosition(kControl_Button_B005))->EnableWindowWZ(false);

}
CCtrlVScrollWZ* uiChaosZonePVP::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V001));
}
bool uiChaosZonePVP::GetScrollArea(RECT& area)
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}
bool uiChaosZonePVP::OnVScrollThumb(SI_MESSAGE* msg)
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
bool uiChaosZonePVP::OnVScrollUp(SI_MESSAGE* msg)
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
bool uiChaosZonePVP::OnVScrollDown(SI_MESSAGE* msg)
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
bool uiChaosZonePVP::OnTapKeyDown(SI_MESSAGE* message)
{
    bool result = true;
    if (GetControlPosition_byID(message->dwCtrlID) == kControl_Edit_E000)
    {
        ShowTitleEditControl(false);
        ShowPasswordEditControl(true);
    }
    else if (GetControlPosition_byID(message->dwCtrlID) == kControl_Edit_E001)
    {
        ShowTitleEditControl(true);
        ShowPasswordEditControl(false);
    }

        
    return result;
}

//------------------------------------------------------------------------------ 
void uiChaosZonePVP::RequestRoomList(bool is_fast /* = false */)
{
    if (is_fast == false)//��� �����Ͱ� �ƴ϶��
    {
        if (list_refresh_timer_.SpentTime() < 1200)// 1.2�� �̳��� ���û �Ұ�
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

    // ����Ʈ ��û
    ChaosSystem::PacketFunction::SEND_CG_FREE_PVP_ROOM_LIST_SYN();

    list_refresh_timer_.Off();
    list_refresh_timer_.On(refresh_room_list_time);
}
void uiChaosZonePVP::EnterRoom()
{
    if (list_control_ == NULL)
    {
        return;
    }
    int index = list_control_->GetCurSel();
    if (index == -1)
    {
        // �����Ͻ� ���� �����ϴ�.
        TCHAR message[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(eST_NO_SELECTED_ROOM, message, INTERFACE_STRING_LENGTH);
        uiSystemMan* system_manager = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
        if (system_manager)
        {
            system_manager->Sys_VerifyLock(message);
        }
        return;
    }

    if (g_pHero && g_pHero->IsParty())
    {
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        uiSystemMan* system_manager =
            static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
        if(system_manager)
        {
            // �� ����� ���� ��Ƽ�� Ż�� �˴ϴ�. ��� �Ͻðڽ��ϱ�? 2475
            g_InterfaceManager.GetInterfaceString(2475, string, INTERFACE_STRING_LENGTH);
            system_manager->Sys_Confirm(string, GlobalFunc::PartyLeaveAndFreePVPJoinConfirm, NULL, NULL, 100/*Ȯ��*/, 101/*���*/);
        }
    }
    else
    {
        SendFreePVPJoinPacket();
    }
}
void uiChaosZonePVP::SendFreePVPJoinPacket()
{
    if (list_control_ == NULL)
    {
        return;
    }

    int index = list_control_->GetCurSel();
    if (index != -1)
    {
        FreePVPRoomInfo* data = static_cast<FreePVPRoomInfo*>(list_control_->GetItemData(index));
        if (data && !_tcslen(data->base_free_pvp_room_info.room_password))
        {
            ChaosSystem::PacketFunction::SEND_CG_FREE_PVP_JOIN_ROOM_SYN(data->room_key, false, _T("\0"));
        }
        else
        {
            //��й�ȣ ui
            uiChaosZonePassword* chaos_password = GET_CAST_DIALOG(uiChaosZonePassword, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PASSWORD);
            if (chaos_password)
            {
                chaos_password->set_room_key(data->room_key);
                chaos_password->ShowInterface(TRUE);
            }
        }
    }
}
void uiChaosZonePVP::SendFreePVPCreatePacket()
{
    if (title_edit_control_ == NULL || password_edit_control_ == NULL)
    {
        return;
    }

    TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR password_string[INTERFACE_STRING_LENGTH] = {0,};

    title_edit_control_->GetTextWZ(title_string, INTERFACE_STRING_LENGTH);
    password_edit_control_->GetTextWZ(password_string, INTERFACE_STRING_LENGTH);
    
    // �������� �ִ� ������(20 byte)�� �Ѿ����ϴ�.
    if(!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)title_edit_control_,
        title_string,
        MAX_ROOMTITLE_LENGTH-1,
        5791))
    {
        return;
    }

    // ��й�ȣ�� �ִ� ������(7 byte)�� �Ѿ����ϴ�.
    if(!GlobalFunc::CompareLimitText((CCtrlDxIMEEditBox*)password_edit_control_,
        password_string,
        MAX_ROOMPASSWORD_LENGTH,
        5792))
    {
        return;
    }

    // �����ܾ� check
    if(GlobalFunc::IsSlang(title_string, false) == TRUE)
    {
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(eST_CANNOT_USE_WORD, temp_string, MAX_MESSAGE_LENGTH);
        uiSystemMan* system_mananger = static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
        if (system_mananger)
        {
            system_mananger->Sys_VerifyLock(temp_string);
            return;
        }
    }

    // �� ����
    BaseFreePVPRoomInfo base_room_info;
    base_room_info.map_code = 40051;
    _tcsncpy(base_room_info.room_title, title_string, MAX_ROOMTITLE_LENGTH-1);
    _tcsncpy(base_room_info.room_password, password_string, MAX_ROOMPASSWORD_LENGTH);

    ChaosSystem::PacketFunction::SEND_CG_FREE_PVP_CREATE_ROOM_SYN(&base_room_info);

}

void uiChaosZonePVP::CreateRoom()
{
    if (g_pHero && g_pHero->IsParty())
    {
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        uiSystemMan* system_manager =
            static_cast<uiSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
        if(system_manager)
        {
            // �� ����� ���� ��Ƽ�� Ż�� �˴ϴ�. ��� �Ͻðڽ��ϱ�? 2475
            g_InterfaceManager.GetInterfaceString(2475, string, INTERFACE_STRING_LENGTH);
            system_manager->Sys_Confirm(string, GlobalFunc::PartyLeaveAndFreePVPCreateConfirm, NULL, NULL, 100/*Ȯ��*/, 101/*���*/);
        }
    }
    else
    {
        SendFreePVPCreatePacket();
    }
}

void uiChaosZonePVP::ShowAnimation(bool is_value)
{
    animatino_show_ = is_value;
}
void uiChaosZonePVP::PlayAnimation()
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

    img_rect.left = img_rect.left + ((img_rect.right - img_rect.left - refresh_animation_frame_width) / 2) + 0.5f;
    img_rect.top = img_rect.top + ((img_rect.bottom - img_rect.top - refresh_anumation_frame_height) / 2) + 0.5f;

    int frame = 30;//�ʴ� �ִϸ��̼� ������
    int count = ((animation_refresh_timer_.SpentTime() / frame) % refresh_animation_frame);

    if (animatino_show_)
    {
        g_pSunRenderer->RenderTexture(effect_texture_, img_rect.left, img_rect.top, 
            refresh_animation_frame_width, refresh_anumation_frame_height, 
            texture_rectangles_[count].left, texture_rectangles_[count].top, 
            texture_rectangles_[count].right, texture_rectangles_[count].bottom);
        ++count;
    }
    else
    {
        //nop
    }
}

void uiChaosZonePVP::RenderIcon()
{
    if (list_control_ == NULL)
    {
        return;
    }

    g_pSunRenderer->Set2DMode(TRUE);

    ChaosSystem::FreePVPData::FreePVPRoomContainer::iterator begin =
        ChaosSystem::GetFreePVPData().free_pvp_room_vector()->begin();
    ChaosSystem::FreePVPData::FreePVPRoomContainer::iterator end =
        ChaosSystem::GetFreePVPData().free_pvp_room_vector()->end();

    static int free_pvp_icon_gap = 4;
    static int free_pvp_icon_gap_left = 0;
    int list_count = -1;

    for (; begin != end; ++begin)
    {
        FreePVPRoomInfo data = (*begin);
        list_count += 1;

        int scroll_pos = 0;
        if (vscroll_control() && (scroll_pos = vscroll_control()->GetScrollPosWZ()) > list_count)
        {
            continue;
        }
        if ((scroll_pos + free_pvp_room_list_count) < list_count) //10�� ������ ���
        {
            break;
        }

        RECT rect = list_control_->GetSizeRect();
        rect.left += free_pvp_icon_gap_left;
        rect.top += ((list_count - scroll_pos) * free_pvp_list_height) + free_pvp_icon_gap;
        rect.right = rect.left + free_pvp_icon_height;
        rect.bottom = rect.top + free_pvp_icon_height;

        if (_tcslen(data.base_free_pvp_room_info.room_password))
        {
            g_InterfaceManager.RenderBattleRoomIcon(
                InterfaceManager::eBATTLE_ICON_LOCKED_ROOM,
                &rect);
        }
    }
}

void uiChaosZonePVP::ShowTitleEditControl(bool is_show)
{
    if ((title_edit_control_ == NULL) || (title_static_control_ == NULL))
    {
        return;
    }

    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (is_show)
    {
        title_edit_control_->ShowWindowWZ(WZ_SHOW);
        title_edit_control_->SetFocusWZ();
        title_static_control_->ShowWindowWZ(WZ_HIDE);


        title_edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        if (_tcslen(string_buffer) == 0)
        {
            // ����������
            TCHAR room_name[INTERFACE_STRING_LENGTH] = {0,};
            int count = 0;
            for (int i = default_room_name_start_string; i < (default_room_name_start_string + default_room_name_string_count); ++i)
            {
                if (g_InterfaceManager.GetInterfaceStringWithCheck(i, room_name, INTERFACE_STRING_LENGTH))
                {
                    ++count;
                }
            }

            if (count && title_edit_control_ && title_static_control_)
            {
                int index = clock_function::GetTickCount() % count;
                g_InterfaceManager.GetInterfaceString(default_room_name_start_string + index, room_name, INTERFACE_STRING_LENGTH);
                title_edit_control_->SetTextWZ(room_name);
                title_static_control_->SetTextWZ(room_name);
            }

            // ������
            SI_MESSAGE message;
            message.lnReceiveMsg = WM_CHAR;
            message.wParam = (WCHAR)1;
            title_edit_control_->SendMessageWZ(&message);

            // �游��� ��ư Ȱ��ȭ
            GetControlWZ(GetControlID_byPosition(kControl_Button_B006))->EnableWindowWZ(true);
        }
    }
    else
    {
        title_edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        title_static_control_->SetTextWZ(string_buffer);

        title_edit_control_->ShowWindowWZ(WZ_HIDE);
        title_static_control_->ShowWindowWZ(WZ_SHOW);
    }
}

void uiChaosZonePVP::ShowPasswordEditControl(bool is_show)
{
    if ((password_edit_control_ == NULL) || (password_static_control_ == NULL))
    {
        return;
    }

    if (is_show)
    {
        password_edit_control_->ShowWindowWZ(WZ_SHOW);
        password_edit_control_->SetFocusWZ();
        password_static_control_->ShowWindowWZ(WZ_HIDE);

        //password_edit_control_->SetTextWZ(_T("\0")); // ����ٱ�?
    }
    else
    {
        TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
        password_edit_control_->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        password_static_control_->SetTextWZ(string_buffer);

        password_edit_control_->ShowWindowWZ(WZ_HIDE);
        password_static_control_->ShowWindowWZ(WZ_SHOW);
    }
}
void uiChaosZonePVP::ClearEditText()
{
    if ((title_edit_control_ == NULL) ||
        (title_static_control_ == NULL) ||
        (password_edit_control_ == NULL) ||
        (password_static_control_ == NULL))
    {
        return;
    }

    title_edit_control_->SetTextWZ("\0");
    title_static_control_->SetTextWZ("\0");
    password_edit_control_->SetTextWZ("\0");
    password_static_control_->SetTextWZ("\0");
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
