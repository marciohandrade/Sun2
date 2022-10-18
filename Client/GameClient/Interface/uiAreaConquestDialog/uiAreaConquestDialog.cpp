#include "SunClientPrecompiledHeader.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "AreaConquestDefine.h"
#include "uiAreaConquestDialog.h"
#include "RewardDialog.h"
#include "AchievementParser.h"
#include "InterfaceManager.h"
#include "GraphicResourceList.h"
#include "GlobalFunc.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "RewardInfoList.h"
#include "ItemInfoParser.h"
#include "NPCInfoParser.h"
#include "MapInfoParser.h"
#include "GameFunc.h"
#include "SCItemSlot.h"
#include "ItemTypeList.h"
#include "QuestInfoManagerEx.h"
#include "SkillSystem/PerkInfoParser.h"
#include "hero.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "TitleListParser.h"
#include "FameListParser.h"
#include "ReputeListParser.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#include "Application.h"
#include "Mouse.h"

namespace
{
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    const long kClientHeight = 596;
    const int kInfoDetailWitdh = 0;
    const int kInfoDetailHeight = 0;
    const int kInfoDetailTextMarginTop = 6;
    const int kInfoDetailTextMarginBottom = 3;
    const int kInfoDetailTextMarginLeft = 9;
    const int kInfoDetailTextMarginRight = 8;
    const int kInfoDetailRectMarginBottom = 0;
#else
    const long kClientHeight = 518;
    const int kInfoDetailWitdh = 0;
    const int kInfoDetailHeight = 0;
    const int kInfoDetailTextMarginTop = 8;
    const int kInfoDetailTextMarginBottom = 8;
    const int kInfoDetailTextMarginLeft = 8;
    const int kInfoDetailTextMarginRight = 8;
    const int kInfoDetailRectMarginBottom = 6;
#endif //_NA_002884_AC_DIALOG_RENEWAL
    const DWORD ac_info_font = StrToWzID("mn12");
    const DWORD ac_text_color[uiAreaConquestDialog::KMax_Text_Color] = {
        WzColor_RGBA(247, 251, 190, 255), 
        WzColor_RGBA(150, 150, 150, 255), 
        WzColor_RGBA(150, 150, 150, 255),
        WzColor_RGBA(80, 80, 80, 255)};
    const DWORD ac_text_background_color = WzColor_RGBA(255, 255, 255, 0);
    const DWORD ac_select_text_color = WzColor_RGBA(50, 50, 0, 255);
    const DWORD ac_select_bar_color = WzColor_RGBA(255, 255, 0, 255);

    const int kImage_Size = 32;
    const BYTE kImage_Alpha = 65;
}


uiAreaConquestDialog::uiAreaConquestDialog(InterfaceManager* ui_manager):
uiBase(ui_manager),
ui_ac_manager_(NULL),
selected_area_code_(0),
selected_info_state_(uiAreaConquest::kSelect_Max),
current_scroll_position_(-1),
ac_detail_display_index_(INVALID_SLOT_POS),
pending_reward_ac_index_(0),
ac_gauge_control_size_(0.0f),
ac_gauge_texture_size_(0.0f),
selected_achievement_info_(NULL)
#ifdef _NA_006541_RENEWER_ACDIALOG
,tree_control_ptr_(NULL)
,height_item_(0)
,string_height_(0)
#endif
{
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    // no operation
#else
    info_control_texture_ = INVALID_HANDLE_VALUE;
#endif //_NA_002884_AC_DIALOG_RENEWAL
}

uiAreaConquestDialog::~uiAreaConquestDialog(void)
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();
    }

#ifdef _NA_006541_RENEWER_ACDIALOG
    if(tree_control_ptr_)
    {
        delete tree_control_ptr_;
        tree_control_ptr_ = NULL;
    }
#endif
}

void uiAreaConquestDialog::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    ui_ac_manager_ = static_cast<uiAreaConquestMan*>(GetMan());
    RegisterControl();

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
    CControlWZ* gauge_control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    if (gauge_control != NULL)
    {
        ac_gauge_control_size_ = static_cast<float>(gauge_control->GetSize().right);
        ac_gauge_texture_size_ = gauge_control->GetCtrlImageFormat()->m_sImageState->m_sFRect.fWidth;
    }    
#endif

    CControlWZ* ac_info_control = NULL;
    ac_info_control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_Detail));
    if (ac_info_control != NULL)
    {
        ac_info_control->SetCallbackDraw(uiAreaConquestDialog::CallBackRenderACDetailInfo, this);
    }
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    ac_info_control_width_.resize(kDisplay_AC_Info_Num);
    ac_info_control = GetControlWZ(GetControlID_byPosition(kControl_Picture_BackGround));
    if (ac_info_control != NULL)
    {
        ac_info_control->SetDiscardMessage(true);
    }
#else
    ac_info_text_render_.resize(kDisplay_AC_Info_Num);

    // 컨트롤 Draw Callback 함수 세팅
    const POSTYPE info_control_pos = kControl_Button_Object_01;
    for (int index_display = 0; index_display < kDisplay_AC_Info_Num; ++index_display)
    {
        ac_info_control = GetControlWZ(GetControlID_byPosition(info_control_pos + index_display));
        if (ac_info_control == NULL)
        {
            continue;
        }
        ac_info_control->SetCallbackDraw(uiAreaConquestDialog::CallBackRenderACInfo, 
            (void*)index_display);
    }

    // 텍스쳐 이미지
    if (info_control_texture_ == INVALID_HANDLE_VALUE)
    {
        info_control_texture_ = 
            g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Dlg92.tga", 
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }
#endif //_NA_002884_AC_DIALOG_RENEWAL

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
    bool init_result = false;
    // 드랍다운 지역 선택 리스트
    init_result = dropdown_select_area().Init(
#ifdef _SCJ_111202_UI_RENEWAL
        GetControlWZ(GetControlID_byPosition(kControl_Button_B005)),
#else
        GetControlWZ(GetControlID_byPosition(kControl_Text_T201)),
#endif //_SCJ_111202_UI_RENEWAL
        static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_BT00))),
        static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_LS00))),
        NULL,
        NULL);

    if (dropdown_select_area().list_control())
    {
        dropdown_select_area().DeleteItemAll();
        dropdown_select_area().list_control()->SetStartLineGapTop(6);
    }
    
    if (init_result == true)
    {
        dropdown_select_area().ResetVScroll();
        dropdown_select_area().Select(0);
    }

    // 드랍다운 상태 선택 리스트
    init_result = dropdown_select_stats().Init(
#ifdef _SCJ_111202_UI_RENEWAL
        GetControlWZ(GetControlID_byPosition(kControl_Button_B006)),
#else
        GetControlWZ(GetControlID_byPosition(kControl_Text_ST01)),
#endif //_SCJ_111202_UI_RENEWAL
        static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_BT01))),
        static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_LS01))),
        NULL,
        NULL);

    if (dropdown_select_stats().list_control())
    {
        dropdown_select_stats().DeleteItemAll();
        dropdown_select_stats().list_control()->SetStartLineGapTop(6);
    }

    if (init_result == true)
    {
        dropdown_select_stats().ResetVScroll();
        dropdown_select_stats().Select(0);
    }
#endif

#ifdef _NA_006541_RENEWER_ACDIALOG
    
    tree_control_ptr_ = TreeController::CreateTree(20,1,NULL,
        static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000))));

    TextureListInfoHandleManager::Instance()->LoadIcon(10030);
    TextureListInfoHandleManager::Instance()->LoadIcon(10031);
    TextureListInfoHandleManager::Instance()->LoadIcon(10032);
    TextureListInfoHandleManager::Instance()->LoadIcon(10033);
    TextureListInfoHandleManager::Instance()->LoadIcon(10034);
    TextureListInfoHandleManager::Instance()->LoadIcon(10035);

    //컨트롤 사이즈 저장
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01));
    if(control)
    {
        control_size_ = control->GetSize().bottom + 3;
    }
    
    hide_control_reward.clear();

    scroller_index_ = 0;
    expend_item_ =0;
    post_extend_item_scroll_ = 0;
#endif
}

void uiAreaConquestDialog::Release()
{

}

void uiAreaConquestDialog::OnUpdateSolarDialog()
{
#ifdef _NA_006541_RENEWER_ACDIALOG
    tree_control_ptr_->update();
#endif

    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        // UI Inetersect 영역 설정 - 설정한 영역에서만 UI가 동작한다.
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();        

        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();;
        enable_rect.bottom = (enable_rect.top + kClientHeight);
        intersect_list.push_back(enable_rect);

        // 마우스휠 처리
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);
        OnMouseWheelScroll(mouse_wheel_state);

        // 상세정보컨트롤이 열려있으면 해당 영역도 포함
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_Detail));
        if ((control != NULL) && 
            (control->GetState() == WZ_SHOW))
        {
            enable_rect = control->GetSizeRect();
            intersect_list.push_back(enable_rect);
        }

        // 드랍다운 컨트롤 처리
#ifdef _NA_006541_RENEWER_ACDIALOG
#else
        dropdown_select_area().Update();
        dropdown_select_stats().Update();
#endif
    }
}

void uiAreaConquestDialog::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {

#ifdef _NA_006541_RENEWER_ACDIALOG
        uiAreaConquestMan* area_conquest_manager = GET_DIALOG_MANAGER(uiAreaConquestMan, UIMAN_AREACONQUEST);

        if(area_conquest_manager && string_height_)
        {
            tree_control_ptr_->DeleteAllItem();

            //컨트롤 상태 변경
            for (int i = 0 ; i < (int)area_conquest_manager->GetAcDataList().size() ; ++i)
            {
                area_conquest_manager->GetAcDataList()[i]->item_max_count_ = 0;
                area_conquest_manager->GetAcDataList()[i]->item_current_count_ = 0;

                bool percent = false;
                if(i == 0)
                {
                    percent = true;
                }

                MakeCompoisiteTreeitem(NULL,(void*)area_conquest_manager->GetAcDataList()[i],percent);

                GaugeTreeItem* Gauge_item = (GaugeTreeItem*)tree_control_ptr_->GetTreeItem()->child_list_[i];
                if(Gauge_item)
                {
                    Gauge_item->SetRange(0,area_conquest_manager->GetAcDataList()[i]->item_max_count_);
                    Gauge_item->SetPos(area_conquest_manager->GetAcDataList()[i]->item_current_count_);
                }
            }

            //selected_area_code_ = 0;
            string_height_ = 0;

            tree_control_ptr_->Reflush();
        }

        if(selected_area_code_ != 0)
            AllClearACDialog(selected_area_code_);
#endif

        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_dialog_AreaConquest;
        msg.wParam = IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_AreaConquest);
    }
}

void uiAreaConquestDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

#ifdef _NA_006541_RENEWER_ACDIALOG
    CCtrlListWZ* item = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000)));
    if(tree_control_ptr_ && item )
    {
        RECT pos = item->GetSizeRect();
        int MouseX = Mouse::Instance()->GetMouseX();
        int MouseY = Mouse::Instance()->GetMouseY();

        if( pos.left < MouseX && pos.right > MouseX &&
            pos.top < MouseY && pos.bottom > MouseY)
        {
            tree_control_ptr_->MessageProc(message);
        }
        
    }
    

    if(tree_control_ptr_->GetSelItem() && tree_control_ptr_->GetSelItem()->is_leaf())
    {
        _stAcTreelist* user_ptr = 
            (_stAcTreelist*)tree_control_ptr_->GetSelItem()->tree_data.user_data_Ptr_;

        if(user_ptr && selected_area_code_ != user_ptr->nation_code_)
        {
            ShowControl(kControl_Button_Object_Detail, false);
            ShowControl(kControl_Button_Detail_Reward, false);

            OnSelectArea(user_ptr->nation_code_);
            OnUpdateACInfo();

            InitControlReflush();

            ac_detail_display_index_ = INVALID_SLOT_POS;
            selected_achievement_info_ = NULL;
            expend_item_ = 0;
            check_expend_item_ = 0;
        }
    }

#endif

    bool is_change = false;
#ifdef _NA_006541_RENEWER_ACDIALOG
#else
    if (dropdown_select_area().MessageProc(message, OUT is_change))
    {
        HideACInfoDetail();
        if (is_change)
        {
            WORD selected_area_code = static_cast<WORD>(dropdown_select_area().CurrentUserData());
            if (selected_area_code != 0)
            {
                OnSelectArea(selected_area_code);
            }
            OnUpdateACInfo();
        }
        return;
    }
    if (dropdown_select_stats().MessageProc(message, OUT is_change))
    {
        HideACInfoDetail();
        if (is_change)
        {
            WORD select_index = static_cast<WORD>(dropdown_select_stats().CurrentUserData());
            if (select_index >= 0 &&
                select_index < uiAreaConquest::kSelect_Max)
            {
                OnSelectACInfoState(select_index);
            }
            OnUpdateACInfo();
        }
        return;
    }
#endif

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
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

bool uiAreaConquestDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_id = message->dwCtrlID;
    DWORD control_postion = GetControlPosition_byID(control_id);    
    switch (control_postion)
    {
    case kControl_Button_Close:// 닫기버튼
        {
            ShowInterface(FALSE);
        }
        break;
    default:
        {
            if(tree_control_ptr_->GetSelItem() == NULL)
                return result;

            OnClickACButton(message);
        }        
        break;
    }
    return result;
}

void uiAreaConquestDialog::OnClickACButton(SI_MESSAGE* message)
{
    DWORD control_id = message->dwCtrlID;
    DWORD control_postion = GetControlPosition_byID(control_id);    
    switch (control_postion)
    {
    case kControl_Button_Object_01:// AC항목버튼#1
    case kControl_Button_Object_02:
    case kControl_Button_Object_03:
    case kControl_Button_Object_04:
    case kControl_Button_Object_05:
    case kControl_Button_Object_06:
    case kControl_Button_Object_07:
    case kControl_Button_Object_08:
    case kControl_Button_Object_09:
    case kControl_Button_Object_10:// AC항목버튼#10
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    case kControl_Button_Object_11:// AC항목버튼#11
    case kControl_Button_Object_12:
    case kControl_Button_Object_13:
    case kControl_Button_Object_14:
    case kControl_Button_Object_15:
    case kControl_Button_Object_16:
    case kControl_Button_Object_17:
    case kControl_Button_Object_18:
    case kControl_Button_Object_19:
    case kControl_Button_Object_20:// AC항목버튼#20
#endif //_NA_002884_AC_DIALOG_RENEWAL
        {
            OnClickACInfo(control_postion - kControl_Button_Object_01);
            CCtrlVScrollWZ* scroll_control = 
                static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
            if (scroll_control == NULL)
            {
                post_extend_item_scroll_ = scroll_control->GetScrollPosWZ();
            }
        }
        break;
    case kControl_Button_Image_01:// AC항목아이콘#1
    case kControl_Button_Image_02:
    case kControl_Button_Image_03:
    case kControl_Button_Image_04:
    case kControl_Button_Image_05:
    case kControl_Button_Image_06:
    case kControl_Button_Image_07:
    case kControl_Button_Image_08:
    case kControl_Button_Image_09:
    case kControl_Button_Image_10:// AC항목아이콘#10
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    case kControl_Button_Image_11:// AC항목아이콘#11
    case kControl_Button_Image_12:
    case kControl_Button_Image_13:
    case kControl_Button_Image_14:
    case kControl_Button_Image_15:
    case kControl_Button_Image_16:
    case kControl_Button_Image_17:
    case kControl_Button_Image_18:
    case kControl_Button_Image_19:
    case kControl_Button_Image_20:// AC항목아이콘#20
#endif //_NA_002884_AC_DIALOG_RENEWAL
        {
            OnClickACInfo(control_postion - kControl_Button_Image_01);
            CCtrlVScrollWZ* scroll_control = 
                static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
            if (scroll_control == NULL)
            {
                post_extend_item_scroll_ = scroll_control->GetScrollPosWZ();
            }
        }
        break;
    case kControl_Button_Reward_01:// AC항목보상버튼#1
    case kControl_Button_Reward_02:
    case kControl_Button_Reward_03:
    case kControl_Button_Reward_04:
    case kControl_Button_Reward_05:
    case kControl_Button_Reward_06:
    case kControl_Button_Reward_07:
    case kControl_Button_Reward_08:
    case kControl_Button_Reward_09:
    case kControl_Button_Reward_10:// AC항목보상버튼#10
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    case kControl_Button_Reward_11:// AC항목보상버튼#11
    case kControl_Button_Reward_12:
    case kControl_Button_Reward_13:
    case kControl_Button_Reward_14:
    case kControl_Button_Reward_15:
    case kControl_Button_Reward_16:
    case kControl_Button_Reward_17:
    case kControl_Button_Reward_18:
    case kControl_Button_Reward_19:
    case kControl_Button_Reward_20:// AC항목보상버튼#20
#endif //_NA_002884_AC_DIALOG_RENEWAL
        {
            if(tree_control_ptr_->GetSelItem()->is_leaf())
                OnClickACReward(control_postion - kControl_Button_Reward_01);
    
            if(tree_control_ptr_->GetSelItem())
            {
                GaugeTreeItem* item = (GaugeTreeItem*)tree_control_ptr_->GetSelItem();
                item->MinusCheckItme();
            }
        }
        break;
    case kControl_Button_Object_Detail:// 상세정보출력
        {
            OnClickACInfoDetail();
        }
        break;
    case kControl_Button_Detail_Reward:// 상세정보보상버튼
        {
            if(tree_control_ptr_->GetSelItem()->is_leaf())
                OnClickACReward(ac_detail_display_index_);

            if(tree_control_ptr_->GetSelItem())
            {
                GaugeTreeItem* item = (GaugeTreeItem*)tree_control_ptr_->GetSelItem();
                item->MinusCheckItme();
            }

        }
        break;
    }
}

bool uiAreaConquestDialog::OnUpScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        int changed_scroll_pos = current_scroll_pos;
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        --changed_scroll_pos;
        changed_scroll_pos = max(0, changed_scroll_pos);
        scroll_control->SetScrollPosWZ(changed_scroll_pos);

        if ((ac_detail_display_index_ != INVALID_SLOT_POS) && 
            (current_scroll_pos != changed_scroll_pos))
        {
            if (ac_detail_display_index_ + 1 > kDisplay_AC_Info_Num - 1)
            {
                HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
                InitControlReflush();
#endif
            }
            else
            {
                ++ac_detail_display_index_;
            }
        }

        RefreshACInfo();
    }

    return result;
}

bool uiAreaConquestDialog::OnDownScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        int changed_scroll_pos = current_scroll_pos;
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        ++changed_scroll_pos;
        changed_scroll_pos = min(scroll_max, changed_scroll_pos);
        scroll_control->SetScrollPosWZ(changed_scroll_pos);

        if ((ac_detail_display_index_ != INVALID_SLOT_POS) && 
            (current_scroll_pos != changed_scroll_pos))
        {
            if (ac_detail_display_index_ - 1 < 0)
            {
                HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
                InitControlReflush();
#endif
            }
            else
            {
                --ac_detail_display_index_;
            }
        }

        RefreshACInfo();
    }

    return result;
}

bool uiAreaConquestDialog::OnThumbScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        int position_offset = current_scroll_pos - current_scroll_position_;
        
        if (ac_detail_display_index_ != INVALID_SLOT_POS)
        {
            if ((ac_detail_display_index_ - position_offset < 0) ||
                (ac_detail_display_index_ - position_offset > kDisplay_AC_Info_Num - 1))
            {
                HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
                InitControlReflush();
#endif
            }
            else
            {
                ac_detail_display_index_ -= position_offset;
            }
        }

        RefreshACInfo();
    }

    return result;
}

void uiAreaConquestDialog::OnMouseWheelScroll(BYTE mouse_wheel_state)
{
    if(tree_control_ptr_->GetSelItem() == NULL)
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

void uiAreaConquestDialog::RegisterControl()
{
#ifdef _NA_006541_RENEWER_ACDIALOG
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
#else
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00);
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);

#ifdef _SCJ_111202_UI_RENEWAL
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
#else
    RegisterControlPosition(StrToWzID("T201"), kControl_Text_T201);
    RegisterControlPosition(StrToWzID("ST01"), kControl_Text_ST01);
#endif //_SCJ_111202_UI_RENEWAL
    RegisterControlPosition(StrToWzID("ST02"), kControl_Text_ST02);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("LS00"), kControl_List_LS00);
    RegisterControlPosition(StrToWzID("LS01"), kControl_List_LS01);   
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
#endif

#ifdef _NA_002884_AC_DIALOG_RENEWAL
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_BackGround); 
    
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll); 
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_Close);

    // AC항목버튼
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_Object_01);
    RegisterControlPosition(StrToWzID("B041"), kControl_Button_Object_02);
    RegisterControlPosition(StrToWzID("B042"), kControl_Button_Object_03);
    RegisterControlPosition(StrToWzID("B043"), kControl_Button_Object_04);
    RegisterControlPosition(StrToWzID("B044"), kControl_Button_Object_05);
    RegisterControlPosition(StrToWzID("B045"), kControl_Button_Object_06);
    RegisterControlPosition(StrToWzID("B046"), kControl_Button_Object_07);
    RegisterControlPosition(StrToWzID("B047"), kControl_Button_Object_08);
    RegisterControlPosition(StrToWzID("B048"), kControl_Button_Object_09);
    RegisterControlPosition(StrToWzID("B049"), kControl_Button_Object_10);
    RegisterControlPosition(StrToWzID("B050"), kControl_Button_Object_11);
    RegisterControlPosition(StrToWzID("B051"), kControl_Button_Object_12);
    RegisterControlPosition(StrToWzID("B052"), kControl_Button_Object_13);
    RegisterControlPosition(StrToWzID("B053"), kControl_Button_Object_14);
    RegisterControlPosition(StrToWzID("B054"), kControl_Button_Object_15);
    RegisterControlPosition(StrToWzID("B055"), kControl_Button_Object_16);
    RegisterControlPosition(StrToWzID("B056"), kControl_Button_Object_17);
    RegisterControlPosition(StrToWzID("B057"), kControl_Button_Object_18);
    RegisterControlPosition(StrToWzID("B058"), kControl_Button_Object_19);
    RegisterControlPosition(StrToWzID("B059"), kControl_Button_Object_20);
    // AC항목아이콘
    RegisterControlPosition(StrToWzID("B079"), kControl_Button_Image_01);
    RegisterControlPosition(StrToWzID("B080"), kControl_Button_Image_02);
    RegisterControlPosition(StrToWzID("B081"), kControl_Button_Image_03);
    RegisterControlPosition(StrToWzID("B082"), kControl_Button_Image_04);
    RegisterControlPosition(StrToWzID("B083"), kControl_Button_Image_05);
    RegisterControlPosition(StrToWzID("B084"), kControl_Button_Image_06);
    RegisterControlPosition(StrToWzID("B085"), kControl_Button_Image_07);
    RegisterControlPosition(StrToWzID("B086"), kControl_Button_Image_08);
    RegisterControlPosition(StrToWzID("B087"), kControl_Button_Image_09);
    RegisterControlPosition(StrToWzID("B088"), kControl_Button_Image_10);
    RegisterControlPosition(StrToWzID("B089"), kControl_Button_Image_11);
    RegisterControlPosition(StrToWzID("B090"), kControl_Button_Image_12);
    RegisterControlPosition(StrToWzID("B091"), kControl_Button_Image_13);
    RegisterControlPosition(StrToWzID("B092"), kControl_Button_Image_14);
    RegisterControlPosition(StrToWzID("B093"), kControl_Button_Image_15);
    RegisterControlPosition(StrToWzID("B094"), kControl_Button_Image_16);
    RegisterControlPosition(StrToWzID("B095"), kControl_Button_Image_17);
    RegisterControlPosition(StrToWzID("B096"), kControl_Button_Image_18);
    RegisterControlPosition(StrToWzID("B097"), kControl_Button_Image_19);
    RegisterControlPosition(StrToWzID("B098"), kControl_Button_Image_20);
    // AC항목보상버튼
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_Reward_01);
    RegisterControlPosition(StrToWzID("B060"), kControl_Button_Reward_02);
    RegisterControlPosition(StrToWzID("B061"), kControl_Button_Reward_03);
    RegisterControlPosition(StrToWzID("B062"), kControl_Button_Reward_04);
    RegisterControlPosition(StrToWzID("B063"), kControl_Button_Reward_05);
    RegisterControlPosition(StrToWzID("B064"), kControl_Button_Reward_06);
    RegisterControlPosition(StrToWzID("B065"), kControl_Button_Reward_07);
    RegisterControlPosition(StrToWzID("B066"), kControl_Button_Reward_08);
    RegisterControlPosition(StrToWzID("B067"), kControl_Button_Reward_09);
    RegisterControlPosition(StrToWzID("B068"), kControl_Button_Reward_10);
    RegisterControlPosition(StrToWzID("B069"), kControl_Button_Reward_11);
    RegisterControlPosition(StrToWzID("B070"), kControl_Button_Reward_12);
    RegisterControlPosition(StrToWzID("B071"), kControl_Button_Reward_13);
    RegisterControlPosition(StrToWzID("B072"), kControl_Button_Reward_14);
    RegisterControlPosition(StrToWzID("B073"), kControl_Button_Reward_15);
    RegisterControlPosition(StrToWzID("B074"), kControl_Button_Reward_16);
    RegisterControlPosition(StrToWzID("B075"), kControl_Button_Reward_17);
    RegisterControlPosition(StrToWzID("B076"), kControl_Button_Reward_18);
    RegisterControlPosition(StrToWzID("B077"), kControl_Button_Reward_19);
    RegisterControlPosition(StrToWzID("B078"), kControl_Button_Reward_20);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_Object_Detail);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_Detail_Reward);

#else
    RegisterControlPosition(StrToWzID("VS00"), kControl_VScroll); 
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_Close);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_Object_01);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_Object_02);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_Object_03);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_Object_04);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_Object_05);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_Object_06);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_Object_07);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_Object_08);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_Object_09);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_Object_10);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_Image_01);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_Image_02);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_Image_03);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_Image_04);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_Image_05);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_Image_06);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_Image_07);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_Image_08);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_Image_09);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_Image_10);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_Reward_01);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_Reward_02);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_Reward_03);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_Reward_04);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_Reward_05);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_Reward_06);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_Reward_07);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_Reward_08);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_Reward_09);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_Reward_10);
    RegisterControlPosition(StrToWzID("B030"), kControl_Button_Object_Detail);
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_Detail_Reward);
#endif //_NA_002884_AC_DIALOG_RENEWAL

#ifdef _NA_006541_RENEWER_ACDIALOG
    RegisterControlPosition(StrToWzID("PI11"), kControl_Button_Picture_01);
    RegisterControlPosition(StrToWzID("P033"), kControl_Button_Picture_02);
    RegisterControlPosition(StrToWzID("P034"), kControl_Button_Picture_03);
    RegisterControlPosition(StrToWzID("P035"), kControl_Button_Picture_04);
    RegisterControlPosition(StrToWzID("P036"), kControl_Button_Picture_05);
    RegisterControlPosition(StrToWzID("P037"), kControl_Button_Picture_06);
    RegisterControlPosition(StrToWzID("P038"), kControl_Button_Picture_07);
    RegisterControlPosition(StrToWzID("P039"), kControl_Button_Picture_08);
    RegisterControlPosition(StrToWzID("P040"), kControl_Button_Picture_09);
    RegisterControlPosition(StrToWzID("P041"), kControl_Button_Picture_10);
    RegisterControlPosition(StrToWzID("P042"), kControl_Button_Picture_11);
    RegisterControlPosition(StrToWzID("P043"), kControl_Button_Picture_12);
    RegisterControlPosition(StrToWzID("P044"), kControl_Button_Picture_13);
    RegisterControlPosition(StrToWzID("P045"), kControl_Button_Picture_14);
    RegisterControlPosition(StrToWzID("P046"), kControl_Button_Picture_15);
    RegisterControlPosition(StrToWzID("P047"), kControl_Button_Picture_16);
    RegisterControlPosition(StrToWzID("P048"), kControl_Button_Picture_17);
    RegisterControlPosition(StrToWzID("P049"), kControl_Button_Picture_18);
    RegisterControlPosition(StrToWzID("P050"), kControl_Button_Picture_19);
    RegisterControlPosition(StrToWzID("P051"), kControl_Button_Picture_20);
#endif

}

void uiAreaConquestDialog::InitACDialog()
{
    // TODO:후에 파서를 이용하는 것으로 수정하는 것을 고려
    static bool is_initialize = false;

    if (is_initialize == false)
    {
        // AC 지역 리스트업
        area_conquest_info()->set_achievement(ui_ac_manager_->GetAchievement());
        area_conquest_info()->InitAreaCodeList();

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
        // 지역선택리스트
        AreaCode_Iterator itr = area_conquest_info()->GetAreaCodeBegin();
        AreaCode_Iterator itr_end = area_conquest_info()->GetAreaCodeEnd();
        for ( ; itr != itr_end; ++itr)
        {
            dropdown_select_area().InsertItem(
                *itr,
                (void*)*itr,
                StrToWzID("st10"), 
                DT_VCENTER | DT_CENTER,
                ac_text_background_color,
                ac_text_color[kComplete_Text_Color],
                ac_select_text_color);
        }

        if (dropdown_select_area().list_control() != NULL)
        {
            dropdown_select_area().list_control()->SetColorSelBar(ac_select_bar_color);
            dropdown_select_area().list_control()->SetColorSelBarOnText(ac_select_text_color);
            dropdown_select_area().Select(0);
        }

        // 상태선택리스트
        // 601 전체
        dropdown_select_stats().InsertItem(601, (void*)uiAreaConquest::kSelect_All,
            StrToWzID("st10"), DT_VCENTER | DT_CENTER, ac_text_background_color,
            ac_text_color[kComplete_Text_Color], ac_select_text_color);
        // 590 진행
        dropdown_select_stats().InsertItem(590, (void*)uiAreaConquest::kSelect_Progress,
            StrToWzID("st10"), DT_VCENTER | DT_CENTER, ac_text_background_color,
            ac_text_color[kComplete_Text_Color], ac_select_text_color);
        // 1404 완료
        dropdown_select_stats().InsertItem(1404, (void*)uiAreaConquest::kSelect_Complete,
            StrToWzID("st10"), DT_VCENTER | DT_CENTER, ac_text_background_color,
            ac_text_color[kComplete_Text_Color], ac_select_text_color);

        if (dropdown_select_stats().list_control() != NULL)
        {
            dropdown_select_stats().list_control()->SetColorSelBar(ac_select_bar_color);
            dropdown_select_stats().list_control()->SetColorSelBarOnText(ac_select_text_color);
            dropdown_select_stats().Select(0);
        }

        is_initialize = true;
#endif
    }

    selected_info_state_ = uiAreaConquest::kSelect_Complete;
    selected_area_code_ = area_conquest_info()->GetStartAreaCode();
    HideACInfoDetail();

    info_detail_text_.ClearSentence();
    ac_info_control_width_.clear();
    ac_info_control_width_.resize(kDisplay_AC_Info_Num);

#ifdef _NA_006541_RENEWER_ACDIALOG
    for (int i = 0 ; i < (kControl_Button_Object_20 - kControl_Button_Object_01)+1 ; i++)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + i));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ(GetControlID_byPosition(kControl_Button_Picture_01 + i));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ(GetControlID_byPosition(kControl_Button_Image_01 + i));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
    }

    for (int reward_ID = 0 ; reward_ID < (int)hide_control_reward.size() ; ++reward_ID)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(hide_control_reward[reward_ID]));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
    }

    hide_control_reward.clear();
#endif

    //보상 버튼 다 숨김
    for (int i = 0 ; i < 20 ; ++i)
    {
        ShowControl(kControl_Button_Reward_01+i,false);
        ShowControl(kControl_Button_Image_01+i,false);

        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + i));

        if(control)
        {
            control->SetTextWZ(" ");
        }   
    }

    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control != NULL)
    {
       scroll_control->SetScrollRangeWZ(0, 0);
       scroll_control->SetScrollPosWZ(0);
    }
    
    string_height_ = 1;


#ifdef _NA_006541_RENEWER_ACDIALOG
#else
    if (dropdown_select_area().list_control() != NULL)
    {
        dropdown_select_area().SelectByUserData(area_conquest_info()->GetStartAreaCode());
    }
    if (dropdown_select_stats().list_control() != NULL)
    {
        dropdown_select_stats().SelectByUserData(uiAreaConquest::kSelect_Complete);
    }
#endif

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
    CCtrlEditWZ* edit_control =
        static_cast<CCtrlEditWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Edit_E000)));
    if (edit_control != NULL)
    {
        edit_control->ShowWindowWZ(WZ_HIDE);
    }
#endif


}

void uiAreaConquestDialog::OnSelectArea(WORD area_code)
{
    // 지역이 변경되었다면 업데이트
    if (selected_area_code_ != area_code)
    {
        // 컨트롤 초기화
        WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll);
        CCtrlVScrollWZ* scroll_control = 
            static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
        if (scroll_control != NULL)
        {
            scroll_control->SetScrollPosWZ(0);
        }

        HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
        InitControlReflush();
#endif

        if (dropdown_select_area().list_control() != NULL)
        {
            dropdown_select_area().SelectByUserData(area_code);
        }
    }

    selected_area_code_ = area_code;
}

void uiAreaConquestDialog::OnSelectACInfoState(WORD select_State)
{
    // 상태선택이 변경되었으면 업데이트
    if (selected_info_state_ != select_State)
    {
        // 컨트롤 초기화
        WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll);
        CCtrlVScrollWZ* scroll_control = 
            static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
        if (scroll_control != NULL)
        {
            scroll_control->SetScrollPosWZ(0);
        }

        HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
        InitControlReflush();
#endif
    }

    selected_info_state_ = select_State;

    if (select_State > uiAreaConquest::kSelect_Complete)
    {
        return;
    }
}

void uiAreaConquestDialog::OnSelectAcinfoIndex(WORD info_index)
{
    int found_list_index = 0;
    AchievementInfo_Iterator found_itr = area_conquest_info()->GetAchievementInfoBegin();
    AchievementInfo_Iterator info_itr_end = area_conquest_info()->GetAchievementInfoEnd();
    for ( ; found_itr != info_itr_end; ++found_itr, ++found_list_index)
    {
        if (info_index == (*found_itr)->GetIndex())
        {
            break;
        }
    }

    if (found_itr == info_itr_end)
    {
        return;
    }

    scroller_index_ = found_list_index;
    RefreshScroll(found_list_index);

    RefreshACInfo();
}

void uiAreaConquestDialog::OnUpdateACInfo()
{
    // 선택된 지역 / 선택된 상태 취합

    area_conquest_info()->RefreshAchievementInfo(selected_area_code_, selected_info_state_);

    DWORD accomplishment_ratio = area_conquest_info()->GetAccomplishmentRatio();
    DWORD Info_total_count = area_conquest_info()->GetAchievementInfoTotalCount();
    DWORD accomplishment_count = area_conquest_info()->GetAccomplishmentCount();
    bool is_show_ratio = area_conquest_info()->IsShowRatio();

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
    SetAreaProgressData(accomplishment_ratio, Info_total_count, accomplishment_count, is_show_ratio);
#endif

    RefreshScroll(0);

    RefreshACInfo();
}

void uiAreaConquestDialog::OnClickACInfo(DWORD list_index)
{
    bool isItem = SetACDetailData(list_index);

    if(isItem == false)
    {
        return;
    }

#ifdef _NA_006541_RENEWER_ACDIALOG
     CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_Detail));
    if(control)
    {
        ReflushForceMove(list_index,control->GetSizeRect());
    }
#endif

}

void uiAreaConquestDialog::OnClickACInfoDetail()
{
    HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
    InitControlReflush();
#endif
}

void uiAreaConquestDialog::OnClickACReward(DWORD list_index)
{
    if (list_index >= kDisplay_AC_Info_Num)
    {
        return;
    }

    if ((ui_ac_manager_ != NULL) && 
        (ui_ac_manager_->CheckPacketPending_Reward() == false))
    {
        return;
    }

    int current_scroll_pos = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control != NULL)
    {
        current_scroll_pos = scroll_control->GetScrollPosWZ();
    }

    AchievementInfo* ac_info = area_conquest_info()->GetAchievementInfo(list_index + current_scroll_pos);
    if (ac_info == NULL)
    {
        return;
    }

    if ((ac_info->IsAccomplishment() == true) &&
        (ac_info->IsReward() == false))
    {
        // 보상창 출력
        RewardDialog* ui_reward_dialog = 
            static_cast<RewardDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_REWARD));
        if (ui_reward_dialog == NULL) 
        {
            return;            
        }

        selected_achievement_info_ = ac_info;

        Achievement_ParserInfo_Reward ac_reward_info = ac_info->GetRewardInfo(0);
        if (ac_reward_info.reward_type_ == ACHIEVEMENT_REWARD_CODE)
        {
            ui_reward_dialog->SetACReward(ac_reward_info.reward_code_);
            ui_reward_dialog->ShowDialog(TRUE);
        }
        else
        {
            // 달성률 보상 요청
            ui_ac_manager_->SEND_CG_AREACONQUEST_REWARD_SYN();
        }
    }
}

void uiAreaConquestDialog::OnClickACIcon( DWORD list_index )
{
    if (ac_detail_display_index_ != list_index)
    {
        SetACDetailData(list_index);
    }
    else
    {
        HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
        InitControlReflush();
#endif
    }
}

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
void uiAreaConquestDialog::SetAreaProgressData(DWORD ac_ratio, DWORD total_count, DWORD complete_count, bool is_show_ratio)
{
    // 게이지 출력 ( 달성률 or 완료카운터 )
    TCHAR data_string[INTERFACE_STRING_LENGTH] = {0, };
    CControlWZ* data_control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    if (data_control != NULL)
    {
        RECT gauge_rect = data_control->GetSizeRect();
        float gauge_ratio = 0.0f;
        if (is_show_ratio == true)
        {
            gauge_ratio = (ac_ratio / 10000.0f);
        }
        else
        {
            if (total_count != 0)
            {
                gauge_ratio = static_cast<float>(complete_count) / total_count;
            }
        }
        
        if (gauge_ratio >= 0.01f)
        {
            float gauge_width = ac_gauge_control_size_ * gauge_ratio;
            float texture_width = ac_gauge_texture_size_ * gauge_ratio;
            data_control->GetCtrlImageFormat()->m_sImageState[E_BASE].m_sFRect.fWidth = texture_width;
            data_control->MoveWindowWZ(static_cast<float>(gauge_rect.left), 
                static_cast<float>(gauge_rect.top), 
                gauge_width, 
                static_cast<float>(gauge_rect.bottom - gauge_rect.top));
            data_control->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            data_control->ShowWindowWZ(WZ_HIDE);
        }
    }

    // 달성률 출력 ( 달성률 or 완료카운터 )
    data_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));
    if (data_control != NULL)
    {
        if (is_show_ratio == true)
        {
            Snprintf(data_string, INTERFACE_STRING_LENGTH - 1, "%.1f%%", (ac_ratio / 100.0f));
        }
        else
        {
            Snprintf(data_string, INTERFACE_STRING_LENGTH - 1, "%d / %d", complete_count, total_count);
        }
        
        data_control->SetTextWZ(data_string);
    }

    // 완료/전체 카운터 출력
    data_control = GetControlWZ(GetControlID_byPosition(kControl_Text_ST02));
    if (data_control != NULL)
    {
        Snprintf(data_string, INTERFACE_STRING_LENGTH - 1, "%d/%d", complete_count, total_count);
        data_control->SetTextWZ(data_string);
    }
}
#endif

void uiAreaConquestDialog::SetACData(int index_display, int index_info)
{
    // index_display => control_pos
    const POSTYPE icon_control_pos = kControl_Button_Image_01;
    const POSTYPE reward_control_pos = kControl_Button_Reward_01;

    int ac_info_list_size = area_conquest_info()->GeAchievementInfoSize();
    if (index_info >= ac_info_list_size)
    {
        ShowControl(reward_control_pos + index_display, false);
        return;
    }

    ShowControl(icon_control_pos + index_display, true);

    AchievementInfo* ac_info = area_conquest_info()->GetAchievementInfo(index_info);
    bool IsAccomplishment = ac_info->IsAccomplishment();
    bool enable_reward = false;
    
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(icon_control_pos + index_display));
    if (control != NULL)
    {
        char* image_filename = NULL;
        RESOURCE_INFO* resource_info = GraphicResourceList::Instance()->GetItemInfo(ac_info->GetIconCode());
        if (resource_info != NULL)
        {
            image_filename = resource_info->szFilename;
            SI_TFORMAT* texture_format = control->GetCtrlImageFormat();
            if ((texture_format != NULL) && 
                (strcmp(image_filename, texture_format->m_sImageState[E_BASE].m_sImageName.c_str()) != 0))
            {
                HANDLE texture_handle = g_pSunRenderer->x_pManagerTexture->LoadTexture(
                    image_filename, 
                    TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

                for (int draw_state = E_BASE; draw_state < (E_CLICK + 1); ++draw_state)
                {
                    if ((image_filename == NULL) || 
                        (texture_handle == INVALID_HANDLE_VALUE))
                    {
                        texture_format->m_sImageState[draw_state].m_blIs = FALSE;
                        texture_format->m_sImageState[draw_state].m_color = RGBA(255,255,255,0);
                    }
                    else
                    {
                        BYTE alpha_value = ((IsAccomplishment == true) ? 255 : kImage_Alpha);
                        texture_format->m_sImageState[draw_state].m_blIs = TRUE;
                        texture_format->m_sImageState[draw_state].m_color = RGBA(255,255,255,alpha_value);
                    }        
                    texture_format->m_sImageState[draw_state].m_sFRect.fTopX = static_cast<float>(0);
                    texture_format->m_sImageState[draw_state].m_sFRect.fTopY = static_cast<float>(0);
                    texture_format->m_sImageState[draw_state].m_sFRect.fHeight = static_cast<float>(kImage_Size);
                    texture_format->m_sImageState[draw_state].m_sFRect.fWidth = static_cast<float>(kImage_Size);
                    texture_format->m_sImageState[draw_state].m_nType = 1;

                    texture_format->m_sImageState[draw_state].m_pVoid = texture_handle;
                }
            }
        }
        else
        {
            control->ReleaseImage();
        }
    }

    // 보상버튼 세팅
    if ((IsAccomplishment == true) &&
        (ac_info->IsReward() == false))
    {
        enable_reward = true;
    }
    ShowControl(reward_control_pos + index_display, enable_reward);

    // 내용세팅
    SetRectACControl(index_display, enable_reward);
    SetACDataString(index_display, ac_info);
}

bool uiAreaConquestDialog::SetACDetailData(int index_display)
{
    ac_detail_display_index_ = index_display;
    int current_scroll_pos = 0;
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control != NULL)
    {
        current_scroll_pos = scroll_control->GetScrollPosWZ();
    }

    int index_info = current_scroll_pos + index_display; // 스크롤 위치부터
    int num_info = area_conquest_info()->GeAchievementInfoSize();
    if (index_info >= num_info)
    {
        HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
        InitControlReflush();
#endif
        return false;
    }

    AchievementInfo* ac_info = area_conquest_info()->GetAchievementInfo(index_info);
    if (ac_info == NULL)
    {
        HideACInfoDetail();
#ifdef _NA_006541_RENEWER_ACDIALOG
        InitControlReflush();
#endif
        return false;
    }

    // 기본 위치
    RECT info_detail_rect;
    info_detail_rect.left = 0;
    info_detail_rect.top = 0;
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    info_detail_rect.right = 316;
    info_detail_rect.bottom = 30;
#else
    info_detail_rect.right = 288;
    info_detail_rect.bottom = 42;
#endif //_NA_002884_AC_DIALOG_RENEWAL    

    // 내용 세팅
    {
        TCHAR ac_string[INTERFACE_STRING_LENGTH] = " ";
        FTextParser parser;
        FTextSentence sentence;
        FTextSentence temp_sentence;
        parser.Parser(ac_string, temp_sentence);
        int text_color_index = 
            (ac_info->IsAccomplishment() == true ? kComplete_Text_Color : kInComplete_Text_Color);

        info_detail_text_.Init(GetDrawBase(), info_detail_rect);
        info_detail_text_.ClearSentence();

        // 과제명
        GetACObjectText(ac_info, ac_string);
        parser.SetDefaultFont(ac_info_font, ac_text_color[text_color_index]);
        BOOL result = parser.Parser(ac_string, sentence);
        if (result == TRUE)
        {
            info_detail_text_.Append(sentence);
        }

        // 보상
        ZeroMemory(ac_string, INTERFACE_STRING_LENGTH);
        GetACRewardText(ac_info, ac_string);
        result = parser.Parser(ac_string, sentence);
        if (result == TRUE)
        {
            info_detail_text_.Append(sentence);
        }
		// 빈줄
        //info_detail_text_.Append(temp_sentence);

        // 과제설명
        ZeroMemory(ac_string, INTERFACE_STRING_LENGTH);
        g_InterfaceManager.GetInterfaceString(ac_info->GetToolTipCode(), ac_string);
        result = parser.Parser(ac_string, sentence);
        if (result == TRUE)
        {
            info_detail_text_.Append(sentence);
        }

         // 상세 항목
        ZeroMemory(ac_string, INTERFACE_STRING_LENGTH);
        {
            Achievement_ParserInfo* ac_parser_info = 
                AchievementParser::Instance()->GetAchievement_ParserInfo(ac_info->GetIndex());
            if (ac_parser_info != NULL)
            {
                typedef std::list<Object_Info> Object_Info_List;
                Object_Info_List object_info_list;
                WORD object_code_;

                // 상제과제 및 완료 여부 조사
                for (BYTE object_index = 0; object_index < MAX_ACHIEVEMENT_OBJECT; ++object_index)
                {
                    object_code_ = ac_parser_info->obejcts_[object_index].object_code_;
                    if (object_code_ != 0)
                    {                        
                        Object_Info object_info;
                        object_info.object_code = object_code_;
                        object_info.is_complete = ac_info->IsAccomplishment(object_index);
                        object_info.achievement = ac_info->GetObjectInfo(object_index).Achievement_;
                        object_info.value       = ac_info->GetObjectInfo(object_index).value_;
                        object_info_list.push_back(object_info);
                    }
                }

                // 세부 과제가 2개 이상일 경우 출력 / 1개이하의 경우 미출력
                if (object_info_list.size() > 1
                    || ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME //특별히 1개일 경우에 디테일을 보일 ac타입
                    || ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_AC_CLEARE
                    || ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_PERK_LEVEL
                    || ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_NPC
                    )
                {
                    //info_detail_text_.Append(temp_sentence);

                    WORD object_type = ac_info->GetObjectType();
                    bool is_success = false;
                    Object_Info_List::iterator object_info_itr = object_info_list.begin();
                    Object_Info_List::iterator object_info_itr_end = object_info_list.end();
                    for ( ; object_info_itr != object_info_itr_end; ++object_info_itr)
                    {
                        is_success = GetACObjectDetailText(object_type, object_info_itr->object_code, ac_string, 
                            object_info_itr->achievement,object_info_itr->value);
                        if (is_success)
                        {
                            if ( object_info_itr->is_complete == true)
                            {
                                parser.SetDefaultFont(ac_info_font, ac_text_color[text_color_index]);
                            }
                            else
                            {
                                parser.SetDefaultFont(ac_info_font, ac_text_color[text_color_index + 2]);
                            }
#ifdef _NA_006541_RENEWER_ACDIALOG
                            TCHAR isComma[INTERFACE_STRING_LENGTH];
                            sprintf(isComma," · %s",ac_string);

                            result = parser.Parser(isComma, sentence);
#else
							result = parser.Parser(ac_string, sentence);
#endif
                            if (result == TRUE)
                            {
                                info_detail_text_.Append(sentence);
                            }
                        }
                    }
                }
#ifdef _SCJ_ACSYSTEM_OVERLAP_PARENT
                // 선행과제 목록 추가
                if (ac_info->GetParentMap().size() != 0)
                {
                    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

                    // 빈줄
                    //info_detail_text_.Append(temp_sentence);
                    // 91007 : 완료된 선행 과제:
                    g_InterfaceManager.GetInterfaceString(91007, temp_string);

                    BOOL result = parser.Parser(temp_string, sentence);
                    if (result == TRUE)
                    {
                        info_detail_text_.Append(sentence);
                    }
 
                    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter = ac_info->GetParentMap().begin();
                    ACHIEVEMENT_INFO_MAP_ITER parent_map_iter_end = ac_info->GetParentMap().end();

#ifdef _NA_008226_AC_PARENTMAP_LIST_SKIP
                    //////////////////////////////////////////////////////////////////////////
                    // 선행과제 길어지면 생략 
                    int list_size = ac_info->GetParentMap().size();
                    int current_line = 0;
                    const int max_line = 9; // 줄 제한
                    
                    TCHAR point_string[5] = {0, };

                    // 3639 ·
                    g_InterfaceManager.GetInterfaceString(
                        3639,
                        point_string, 4);

                    for (; parent_map_iter != parent_map_iter_end; ++parent_map_iter)
                    {
                        current_line++;

                        if ((list_size > max_line) && (current_line == 4))
                        {
                            sprintf(ac_string,"    %s  %s  %s    ", point_string,point_string,point_string);
                        }
                        else if ((list_size > max_line) && (3 < current_line && current_line < list_size - 4))
                        {
                            continue;
                        }
                        else
                        {
                            g_InterfaceManager.GetInterfaceString(
                                (*parent_map_iter).second->GetStringCode(),
                                temp_string);
                            sprintf(ac_string," %s %s",point_string, temp_string);
                        }

                        parser.SetDefaultFont(ac_info_font, ac_text_color[text_color_index]);
                        result = parser.Parser(ac_string, sentence);

                        if (result == TRUE)
                        {
                            info_detail_text_.Append(sentence);
                        }
                    }
                    //////////////////////////////////////////////////////////////////////////
#else
                    for(; parent_map_iter != parent_map_iter_end; ++parent_map_iter)
                    {
                        g_InterfaceManager.GetInterfaceString(
                            (*parent_map_iter).second->GetStringCode(),
                            temp_string);
#ifdef _NA_006541_RENEWER_ACDIALOG
                        TCHAR isComma[INTERFACE_STRING_LENGTH];
                        sprintf(isComma," · %s",temp_string);
#endif

                        parser.SetDefaultFont(ac_info_font, ac_text_color[text_color_index]);
#ifdef _NA_006541_RENEWER_ACDIALOG                        
						result = parser.Parser(isComma, sentence);
#else
					    result = parser.Parser(temp_string, sentence);
#endif
                        if (result == TRUE)
                        {
                            info_detail_text_.Append(sentence);
                        }
                    }
#endif //_NA_008226_AC_PARENTMAP_LIST_SKIP
                }
#endif //_SCJ_ACSYSTEM_OVERLAP_PARENT
            }
        }

        info_detail_text_.SetMargin(kInfoDetailTextMarginLeft, 
            7,
            kInfoDetailTextMarginRight, 
            4);

        CControlWZ* base_control = 
            GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01));
        CControlWZ* base_control2 = 
            GetControlWZ(GetControlID_byPosition(kControl_Button_Object_02));

        SIZE FontSize;
        GetDrawBase()->x_pManagerTextOut->StartToUseFont(ac_info_font);
        GetDrawBase()->x_pManagerTextOut->GetTextExtentPoint("임시",
            _tcslen("임시"),
            &FontSize);

        GetDrawBase()->x_pManagerTextOut->EndUseFont();	

        int gap = 7;
        if(base_control && base_control2)
        {
            gap = base_control2->GetSizeRect().top - base_control->GetSizeRect().top;
        }
        gap -= FontSize.cy;

        info_detail_text_.SetGap(0,gap);
        info_detail_text_.convertLineInfo();
    }

    // 화면 밑을 넘어가면 위로 염
    bool is_reverse_window = false;
    LONG detail_control_height = info_detail_text_.GetDrawLineHeight() + 2;

    // 버튼 위치 설정
    CControlWZ* base_control = 
        GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + index_display));

    if ((g_pApplication != NULL) && (base_control != NULL))
    {
        LONG control_bottom = base_control->GetSize().top + detail_control_height;

        POINT point = g_pApplication->Get2DModeMapping();

        if (point.y < control_bottom)
        {
            is_reverse_window = true;
        }
    }

    if (base_control != NULL)
    {
        if (is_reverse_window == true)
        {
            info_detail_rect.left = base_control->GetSize().left;
            info_detail_rect.top = base_control->GetSize().top + base_control->GetSize().bottom - detail_control_height;
            info_detail_rect.bottom = detail_control_height;
        }
        else
        {
            info_detail_rect.left = base_control->GetSize().left;
            info_detail_rect.top = base_control->GetSize().top;
            info_detail_rect.bottom = detail_control_height;
        }
    }

    CControlWZ* ac_info_detail = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_Detail));
    if (ac_info_detail != NULL)
    {
        ac_info_detail->MoveWindowWZ(static_cast<float>(info_detail_rect.left), 
            static_cast<float>(info_detail_rect.top), 
#ifdef _NA_006541_RENEWER_ACDIALOG
            static_cast<float>(info_detail_rect.right)+2, 
#else
			static_cast<float>(info_detail_rect.right),
#endif
            static_cast<float>(info_detail_rect.bottom));
        ac_info_detail->ShowWindowWZ(WZ_SHOW);
    }

    // 보상 버튼 설정
    base_control = GetControlWZ(GetControlID_byPosition(kControl_Button_Reward_01 + index_display));
    ac_info_detail = GetControlWZ(GetControlID_byPosition(kControl_Button_Detail_Reward));
    if (ac_info_detail != NULL)
    {
        if ((ac_info->IsAccomplishment() == true) && 
            (ac_info->IsReward() == false))
        {
            ac_info_detail->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            ac_info_detail->ShowWindowWZ(WZ_HIDE);
        }
        // 보상버튼 위치 설정

        if (base_control != NULL)
        {
            RECT reward_rect = base_control->GetSize();

            if (is_reverse_window == true)
            {
                ac_info_detail->MoveWindowWZ(static_cast<float>(reward_rect.left),
                    static_cast<float>(reward_rect.top + reward_rect.bottom - info_detail_text_.GetDrawLineHeight()),
                    static_cast<float>(reward_rect.right),
                    static_cast<float>(reward_rect.bottom));
            }
            else
            {
                ac_info_detail->MoveWindowWZ(static_cast<float>(reward_rect.left),
                    static_cast<float>(reward_rect.top),
                    static_cast<float>(reward_rect.right),
                    static_cast<float>(reward_rect.bottom));
            }
        }        
    }

    return true;
}

void uiAreaConquestDialog::SetRectACControl(int index_display, bool enable_reward)
{
    if (index_display >= kDisplay_AC_Info_Num)
    {
        return;
    }

    const POSTYPE info_control_pos = kControl_Button_Object_01;
    const POSTYPE reward_control_pos = kControl_Button_Reward_01;

    CControlWZ* ac_control = 
        GetControlWZ(GetControlID_byPosition(info_control_pos + index_display));
    if (ac_control == NULL)
    {
        return;
    }

    RECT ac_control_rect = ac_control->GetSizeRect();

    if (enable_reward == true)
    {
        CControlWZ* ac_reward_control = 
            GetControlWZ(GetControlID_byPosition(reward_control_pos + index_display));
        if (ac_reward_control != NULL)
        {
            RECT ac_reward_control_rect = ac_reward_control->GetSize();
            ac_control_rect.right -= ac_reward_control_rect.right;
        }
    }

#ifdef _NA_002884_AC_DIALOG_RENEWAL
    // pos에 해당하는 컨트롤의 텍스트를 출력할 너비 계산
    ac_info_control_width_[index_display] = (ac_control_rect.right - ac_control_rect.left);
#else
    // pos에 해당하는 FTextRender 를 얻어서 RECT세팅
    ac_info_text_render_[index_display].Init(GetDrawBase(), ac_control_rect);
#endif //_NA_002884_AC_DIALOG_RENEWAL    
}

void uiAreaConquestDialog::SetACDataString(int index_display, AchievementInfo* ac_info)
{
    // pos에 해당하는 데이터/FTextRender 를 얻어서 세팅
    if (index_display >= kDisplay_AC_Info_Num)
    {
        return;
    }

#ifdef _NA_002884_AC_DIALOG_RENEWAL
    CCtrlButtonWZ* ac_info_button = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_Object_01 + index_display));
    if (ac_info_button == NULL)
    {
        return;
    }

    if (ac_info == NULL)
    {
        ac_info_button->SetTextWZ(" ");
        return;
    }

    TCHAR ac_object_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
    int text_color_index = 
        (ac_info->IsAccomplishment() == true ? kComplete_Text_Color : kInComplete_Text_Color);
    RECT text_rect = GetControlWZ_byPosition(kControl_Button_Object_01 + index_display)->GetSizeRect();
    text_rect.right = text_rect.left + ac_info_control_width_[index_display];

    DWORD text_color = GlobalFunc::WzColorToRGBA(ac_text_color[text_color_index]);
    ac_info_button->SetTextColor(text_color);
    ac_info_button->SetTextMouseDownColor(text_color);
    ac_info_button->SetTextMouseOnColor(text_color);
    ac_info_button->SetFontID(ac_info_font);
    ac_info_button->SetStyle(DT_LEFT | DT_VCENTER);

    GetACObjectText(ac_info, temp_string);
    GlobalFunc::CovertSimpleText(temp_string, 
        INTERFACE_STRING_LENGTH, 
        ac_info_button->GetFontID(), 
        text_rect);
    Snprintf(ac_object_string, INTERFACE_STRING_LENGTH - 1, "   %s", temp_string);
    ac_info_button->SetTextWZ(ac_object_string);
#else
    FTextRender* ac_info_text_render_ptr = &(ac_info_text_render_[index_display]);
    ac_info_text_render_ptr->ClearSentence();

    if (ac_info == NULL)
    {
        return;
    }

    FTextParser parser;
    FTextSentence sentence;
    RECT text_rect = ac_info_text_render_ptr->GetRect();
    int text_color_index = 
        (ac_info->IsAccomplishment() == true ? kComplete_Text_Color : kInComplete_Text_Color);

    TCHAR ac_string[INTERFACE_STRING_LENGTH] = {0, };

    GetACObjectText(ac_info, ac_string);
    GlobalFunc::CovertSimpleText(ac_string, INTERFACE_STRING_LENGTH, ac_info_font, text_rect);
    parser.SetDefaultFont(ac_info_font, ac_text_color[text_color_index]);
    BOOL result = parser.Parser(ac_string, sentence);
    if (result == TRUE)
    {
        ac_info_text_render_ptr->Append(sentence);
    }

    ZeroMemory(ac_string, INTERFACE_STRING_LENGTH);

    GetACRewardText(ac_info, ac_string);
    GlobalFunc::CovertSimpleText(ac_string, INTERFACE_STRING_LENGTH, ac_info_font, text_rect);
    result = parser.Parser(ac_string, sentence);
    if (result == TRUE)
    {
        ac_info_text_render_ptr->Append(sentence);
    }

    ac_info_text_render_ptr->SetMargin(8, 4, 8, 3);
    ac_info_text_render_ptr->convertLineInfo();
#endif //_NA_002884_AC_DIALOG_RENEWAL
}

void uiAreaConquestDialog::GetACObjectText(AchievementInfo* ac_info, TCHAR* ac_object_string)
{
    if (ac_info == NULL || ac_object_string == NULL)
    {
        return;
    }

    if (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_ACCOMPLISHMENT //제목만 보여줄 ac타입들 (개수생략)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_BATTLE_HEAL)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_TOP)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_TO_MONSTER)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_BATTLE_DAMAGE_BY_MONSTER)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_BATTLE_KILL)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_BATTLE_DEAD)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_LEVELUP)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_SKILL_STEP)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_PERK_LEVEL)
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_FAME)
        || (ac_info->GetObjectType() == ACHIEVEMENT_OBJECT_REPUTE)
#endif //_NA_003027_20111013_HONOR_SYSTEM
        )
    {
        
        g_InterfaceManager.GetInterfaceString(ac_info->GetStringCode(), ac_object_string);
    }
    else
    {
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(ac_info->GetStringCode(), temp_string);

        int current_value = 0;
        int achievement_value = 0;
        AchievementObjectInfo ac_object_info;
        for (BYTE index = 0; index < MAX_ACHIEVEMENT_OBJECT; ++index)
        {
            ac_object_info = ac_info->GetObjectInfo(index);
            current_value += min(ac_object_info.value_, ac_object_info.Achievement_);
            achievement_value += ac_object_info.Achievement_;
        }

        Snprintf(ac_object_string, INTERFACE_STRING_LENGTH - 1, "%s (%d/%d)", 
                 temp_string, current_value, achievement_value);
    }
}

void uiAreaConquestDialog::GetACRewardText(AchievementInfo* ac_info, TCHAR* ac_reward_string)
{
    if (ac_info == NULL || ac_reward_string == NULL)
    {
        return;
    }

    bool have_reward = false;
    TCHAR temp_reward_string[INTERFACE_STRING_LENGTH] = {' ', };

    TCHAR temp_string1[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR temp_string2[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR temp_string3[INTERFACE_STRING_LENGTH] = {0, };
    Achievement_ParserInfo_Reward reward_info;
    for (int reward_index = 0; reward_index < MAX_ACHIEVEMENT_REWARD; ++reward_index)
    {
        reward_info = ac_info->GetRewardInfo(reward_index);

        switch (reward_info.reward_type_)
        {
        case ACHIEVEMENT_REWARD_NONE:
            continue;
            break;
        case ACHIEVEMENT_REWARD_CODE:
            {
                // 보상스크립트 참조
                RewardInfo* reward_drop_info = 
                    RewardInfoList::Instance()->FindRewardInfo(reward_info.reward_code_);
                if (reward_drop_info == NULL)
                {
                    continue;
                }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
                WORD reward_title_code = reward_drop_info->GetTitle();
                if (reward_title_code != 0)
                {
                    TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(reward_title_code);
                    if (title_info != 0)
                    {
                        g_InterfaceManager.GetInterfaceString(2156, temp_string1);
                        g_InterfaceManager.GetInterfaceString(title_info->title_name_code, temp_string2);
                        Snprintf(temp_string3, INTERFACE_STRING_LENGTH - 1, "%s <%s>", temp_string1, temp_string2);
                        strcat(temp_reward_string, temp_string3);
                        have_reward = true;
                    }
                }
#endif //_NA_003027_20111013_HONOR_SYSTEM
                
                DWORD item_code = 0;
                DWORD item_num = 0;
                SCItemSlot reward_item;
                BASE_ITEMINFO* item_info = NULL;
                g_InterfaceManager.GetInterfaceString(155378, temp_string2);
                for (int drop_index = 0; drop_index < RewardInfo::MAX_DROP_INDEX_NUM; ++drop_index)
                {
                    item_code = reward_drop_info->GetTypeCode(drop_index);
                    item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
                    if (item_info == NULL)
                    {
                        continue;
                    }
                    
                    // 보상 아이템 세팅
                    SCItemSlot reward_item;
                    reward_item.SetCode(static_cast<SLOTCODE>(item_code));
                    DWORD item_type_index = reward_drop_info->GetTypeIndex(drop_index);

                    if (item_type_index >= RewardInfo::MIN_ITEM_TYPECODE)
                    {
                        
                        // 장비/펫의 아이템타입/커스터마이징 아이템 처리
                        const ItemType* item_type_info = ItemTypeList::Instance()->FindItemType(item_type_index);
                        if (item_type_index == NULL)
                        {
                            item_type_index = 1;
                        }
                        else
                        {
                            KindOfItemType::value_type kind_of_item = item_type_info->GetItemType();
                            switch (kind_of_item)
                            {
                            case KindOfItemType::Etheria:
                            case KindOfItemType::EtheriaWithDivine:
                                {
                                    reward_item.ChangeToEtheriaItem();
                                }
                                break;
                            case KindOfItemType::Shell:
                                {
                                    reward_item.ChangeToShellItem();
                                }
                                break;
                            }
                        }
                        item_num = 1;
                    }
                    else
                    {
                        item_num = item_type_index;
                    }
                    
                    GameFunc::GetItemName(&reward_item, item_info, temp_string3, INTERFACE_STRING_LENGTH - 1);                    
                    Snprintf(temp_string1, INTERFACE_STRING_LENGTH, temp_string2, 
                             temp_string3, item_num);
                    
                    if (have_reward == true)
                    {
                        strcat(temp_reward_string, ", ");
                    }
                    strcat(temp_reward_string, temp_string1);
                    have_reward = true;
                }                
            }
            break;
        case ACHIEVEMENT_REWARD_ACCOMPLISHMENT:
            {
                // 91041 달성률
                g_InterfaceManager.GetInterfaceString(91041, temp_string2);
                Snprintf(temp_reward_string, INTERFACE_STRING_LENGTH, " %s %.1f%%", 
                         temp_string2, (reward_info.reward_code_ / 100.0f));
                have_reward = true;
            }
            break;
        default:
            continue;
        }
    }

    if (have_reward == true)
    {
        g_InterfaceManager.GetInterfaceString(90649, ac_reward_string);
        strcat(ac_reward_string, temp_reward_string);
    }
}

bool uiAreaConquestDialog::GetACObjectDetailText(WORD object_type, 
                                                 CODETYPE object_code, 
                                                 TCHAR* object_detail_string,
                                                 WORD achievement,
                                                 WORD value)
{
    bool is_success = false;
    // 과제 타입에 따른 스트링코드를 얻어와야함.
    switch (object_type)
    {
    case ACHIEVEMENT_OBJECT_ITEM:
    case ACHIEVEMENT_OBJECT_COLLECT:
    case ACHIEVEMENT_OBJECT_ITEM_GET:   //아이템 줍기
    case ACHIEVEMENT_OBJECT_ITEM_EQUIP: //아이템 장착
        {
            // 아이템 수집
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(object_code);
            if (item_info != NULL)
            {
                GameFunc::GetItemName(NULL, item_info, object_detail_string, INTERFACE_STRING_LENGTH);
                is_success = true;
            }
        }
        break;
    case ACHIEVEMENT_OBJECT_NPC:
        {
            // 몬스터 사냥
            BASE_NPCINFO* npc_info = NPCInfoParser::Instance()->GetNPCInfo(object_code);
            if (npc_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(npc_info->m_NCode, object_detail_string);
#ifdef _DEV_VER
                //#ifdef _SCJ_SHOW_NPC_ID
                if (CGeneralGameParam::Instance()->IsShowNpcId())
                {
                    TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                    StrnCopy(buffer_string, object_detail_string, INTERFACE_STRING_LENGTH-1); 
                    Snprintf(object_detail_string, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                        buffer_string, npc_info->m_MonsterCode);
                }
                //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
                is_success = true;
            }
        }
        break;
    case ACHIEVEMENT_OBJECT_NPC_AC:
        {
            // AC몬스터 사냥 (캡틴몬스터)
            // 90659 캡틴 배기
            g_InterfaceManager.GetInterfaceString(90659, object_detail_string);
            is_success = true;
        }
        break;
    case ACHIEVEMENT_OBJECT_MISSION:    // 미션 완료
    case ACHIEVEMENT_OBJECT_CURSETOWER: // 저주받은탑 완료
        {
            const sMAPINFO* map_info = 
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(object_code));
            if (map_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(map_info->dwNCode, object_detail_string);
                is_success = true;
            }
        }
        break;
    case ACHIEVEMENT_OBJECT_ACCOMPLISHMENT:
        {
            // 달성률
            // 91041 달성률
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(91041, object_detail_string);
            //g_InterfaceManager.GetInterfaceString(91041, temp_string);
            //Snprintf(object_detail_string, INTERFACE_STRING_LENGTH, "%s %d%%", 
            //         temp_string, (object_code / 100));
            is_success = true;
        }
        break;
    case ACHIEVEMENT_OBJECT_QUEST:
        {
            // 퀘스트 완료
            QuestInfo* quest_info = g_QuestInfoManager.FindQuestInfo(static_cast<QCODE>(object_code));
            if (quest_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(quest_info->GetQNCode(), object_detail_string);
                is_success = true;
            }
        }
        break;    
    case ACHIEVEMENT_OBJECT_MISSION_CLEAR_TIME:
        {
            //시간 내에 미션 클리어
            // 6477 최고기록
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(6477, temp_string);

            WORD value_hour = static_cast<WORD>(value / 3600); 
            WORD value_min = static_cast<WORD>(value/60) % 60;
            WORD value_sec = value % 60;
            Sprintf(object_detail_string,"%s: %03d'%02d\"%02d",temp_string,value_hour,value_min,value_sec);
            //is_success = true;
            return true;//뒤에 수행횟수 미출력을 위해 리턴
        }
        break;
    case ACHIEVEMENT_OBJECT_AC_CLEARE:
        {
            // AC 완료
            Achievement_ParserInfo* ac_parser_info = 
                AchievementParser::Instance()->GetAchievement_ParserInfo(static_cast<WORD>(object_code));
            
            if (ac_parser_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(ac_parser_info->object_string_code_,
                                        object_detail_string);
                return true;//뒤에 수행횟수 미출력을 위해 리턴
            }
        }
        break;
    case ACHIEVEMENT_OBJECT_PERK_LEVEL:
        {            
            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
            PerkID perk_id(static_cast<SLOTCODE>(object_code),achievement);
            const PerkInfo* perk_info = PerkInfoParser::Instance()->FindPerkInfo(perk_id);
            if (perk_info != NULL)
            {
                g_InterfaceManager.GetInterfaceString(perk_info->name_code_,
                    object_detail_string);
                return true;//뒤에 수행횟수 미출력을 위해 리턴
            }
        }
        break;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    case ACHIEVEMENT_OBJECT_FAME:
        {
            FameListParser::FameInfo* fame_info = FameListParser::Instance()->GetFameInfo(static_cast<WORD>(object_code));
            if (fame_info != NULL)
            {
                TCHAR fame_string[INTERFACE_STRING_LENGTH] = {'\0', };
                TCHAR rating_string[INTERFACE_STRING_LENGTH] = {'\0', };
                g_InterfaceManager.GetInterfaceString(fame_info->fame_name_code, fame_string);
                g_InterfaceManager.GetInterfaceString(6501 + achievement, rating_string);
                Sprintf(object_detail_string, "%s %s", fame_string, rating_string);

                return true;
            }
        }
        break;
    case ACHIEVEMENT_OBJECT_REPUTE:
        {
            ReputeListParser::ReputeInfo* repute_info = ReputeListParser::Instance()->GetReputeInfo(static_cast<WORD>(object_code));
            if (repute_info != NULL)
            {
                TCHAR repute_string[INTERFACE_STRING_LENGTH] = {'\0', };
                TCHAR rating_string[INTERFACE_STRING_LENGTH] = {'\0', };
                g_InterfaceManager.GetInterfaceString(repute_info->repute_name_code, repute_string);
                g_InterfaceManager.GetInterfaceString(6508 + achievement, rating_string);
                Sprintf(object_detail_string, "%s %s", repute_string, rating_string);

                return true;
            }
        }
        break;
    case ACHIEVEMENT_OBJECT_FAME_COUNT:
    case ACHIEVEMENT_OBJECT_REPUTE_COUNT:
        {
            int rating_string_code = (object_type == ACHIEVEMENT_OBJECT_FAME_COUNT) ? 6501 : 6508;
            g_InterfaceManager.GetInterfaceString(rating_string_code + object_code, object_detail_string);

            is_success = true;
        }
        break;
    case ACHIEVEMENT_OBJECT_COMPLETE_NORMALQUEST:
    case ACHIEVEMENT_OBJECT_COMPLETE_DAILYQUEST:
    case ACHIEVEMENT_OBJECT_COMPLETE_DAILYQUEST_CONTINOUS:
        {
            int quest_complete_string_code = 6772 + (object_type - ACHIEVEMENT_OBJECT_COMPLETE_NORMALQUEST);
            g_InterfaceManager.GetInterfaceString(quest_complete_string_code, object_detail_string);
        }
       break;
    case ACHIEVEMENT_OBJECT_LOTTO:
        {
            int lotto_string_code = 6775 + object_code - 1;
            g_InterfaceManager.GetInterfaceString(lotto_string_code, object_detail_string);
        }
        break;
    case ACHIEVEMENT_OBJECT_KILL_ALL:
        {
            int kill_string_code = 6778 + object_code - 1;
            g_InterfaceManager.GetInterfaceString(kill_string_code, object_detail_string);
        }
        break;
    case ACHIEVEMENT_OBJECT_DEAD_ALL:
        {
            int dead_string_code = 6781 + object_code - 1;
            g_InterfaceManager.GetInterfaceString(dead_string_code, object_detail_string);
        }
        break;
#endif //_NA_003027_20111013_HONOR_SYSTEM

    }//switch
    if (is_success == true)
    {
        // 수행횟수 출력(완료값이 1이면 생략)
        if ( achievement > 1)
        {
            TCHAR temp_string[INTERFACE_STRING_LENGTH];
            StrCopy(temp_string, object_detail_string);
            Sprintf(object_detail_string, "%s %d/%d", temp_string, value, achievement);
        }
    }

    return is_success;
}

void uiAreaConquestDialog::ClearAcInfoToDisplay(int index_display)
{
    const POSTYPE icon_control_pos = kControl_Button_Image_01;
    const POSTYPE reward_control_pos = kControl_Button_Reward_01;

    ShowControl(reward_control_pos + index_display, false);
    SetControlImage(GetControlID_byPosition(icon_control_pos + index_display), 
                    0, 
                    0, 
                    0, 
                    kImage_Size, 
                    kImage_Size);
    ShowControl(icon_control_pos + index_display,false);


    SetRectACControl(index_display, false);
    SetACDataString(index_display, NULL);
}

void uiAreaConquestDialog::RefreshACInfo()
{
    // 취합된 항목을 출력
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control != NULL)
    {
        current_scroll_position_ = scroll_control->GetScrollPosWZ();
        //current_scroll_pos = scroll_control->GetScrollPosWZ();
    }

#ifdef _NA_006541_RENEWER_ACDIALOG
    const POSTYPE icon_control_pos = kControl_Button_Image_01;
#endif
	int index_info = current_scroll_position_; // 스크롤 위치부터
    int num_info = area_conquest_info()->GeAchievementInfoSize();
    for (int index_display = 0; index_display < kDisplay_AC_Info_Num; ++index_display, ++index_info)
    {
#ifdef _NA_006541_RENEWER_ACDIALOG
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + index_display));
        
        if(control&& control->GetState() == WZ_HIDE)
        {
            index_info--;
            continue;    
        }

        
#endif
        if (index_info >= num_info)
        {
            ClearAcInfoToDisplay(index_display);
        }
        else
        {
            SetACData(index_display, index_info);
        }
    }

    if (ac_detail_display_index_ != INVALID_SLOT_POS)
    {
        OnClickACInfo(ac_detail_display_index_);
    }
}

void uiAreaConquestDialog::RefreshScroll(WORD ac_index_index)
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control == NULL)
    {
        return;
    }

    int scroll_min = 0; 
    int scroll_max = 0;
    int info_list_num = area_conquest_info()->GeAchievementInfoSize();
    int current_scroll_pos = scroll_control->GetScrollPosWZ();
    if (ac_index_index != 0)
    {
        current_scroll_pos = ac_index_index;
    }
    scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
    scroll_max = max(0, (info_list_num - kDisplay_AC_Info_Num));
    current_scroll_pos = min(current_scroll_pos, scroll_max);
    
    scroll_control->SetScrollRangeWZ(scroll_min, scroll_max);
    scroll_control->SetScrollPosWZ(current_scroll_pos);

    current_scroll_position_ = scroll_control->GetScrollPosWZ();
}

#ifdef _NA_002884_AC_DIALOG_RENEWAL
#else
void uiAreaConquestDialog::RenderACInfoText(DWORD index_display)
{
    const POSTYPE info_control_pos = kControl_Button_Object_01;
    CControlWZ* control = 
        GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + index_display));
    if (control != NULL)
    {
        ac_info_text_render_[index_display].SetRect(control->GetSizeRect());
        ac_info_text_render_[index_display].convertLineInfo();
        ac_info_text_render_[index_display].Render();
    }
}

void uiAreaConquestDialog::CallBackRenderACInfo(CControlWZ* control_ptr, void* parameter)
{
    uiAreaConquestDialog* ac_dialog = 
        GET_CAST_DIALOG(uiAreaConquestDialog, IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG);
    if (ac_dialog == NULL)
    {
        return;
    }

    ac_dialog->RenderACInfoText((DWORD)parameter);
}
#endif //_NA_002884_AC_DIALOG_RENEWAL

void uiAreaConquestDialog::RenderACInfoDetailControl()
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_Detail));
    if (control != NULL)
    {
        RECT control_rect = control->GetSizeRect();

#ifdef _NA_002884_AC_DIALOG_RENEWAL
        // no operation
#else
        if (info_control_texture_ != INVALID_HANDLE_VALUE)
        {
            // 컨트롤 배경 이미지 그리기

            static float top_y = 707.5f;
            static float center_y = 718.5f;
            static float bottom_y = 723.5f;

            float offset_over_imgae = 0.0f;
            if (control->GetDrawState() == E_ON)
            {
                offset_over_imgae = 31.0f;
            }

            // top
            g_pSunRenderer->RenderTexture(info_control_texture_, 
                static_cast<float>(control_rect.left) + 0.5f, 
                static_cast<float>(control_rect.top) + 0.5f, 
                static_cast<float>(control_rect.right - control_rect.left), 
                13.0f, 
                2.0f, 
                top_y + offset_over_imgae, 
                static_cast<float>(control_rect.right - control_rect.left), 
                12.0f);

            // center
            g_pSunRenderer->RenderTexture(info_control_texture_, 
                static_cast<float>(control_rect.left) + 0.5f, 
                static_cast<float>(control_rect.top + 13) + 0.5f, 
                static_cast<float>(control_rect.right - control_rect.left), 
                static_cast<float>(control_rect.bottom - control_rect.top - 15), 
                2.0f, 
                center_y + offset_over_imgae, 
                static_cast<float>(control_rect.right - control_rect.left), 
                1.0f);

            // bottom
            g_pSunRenderer->RenderTexture(info_control_texture_, 
                static_cast<float>(control_rect.left)  + 0.5f, 
                static_cast<float>(control_rect.bottom - 13) +0.5f, 
                static_cast<float>(control_rect.right - control_rect.left), 
                13.0f, 
                2.0f, 
                bottom_y + offset_over_imgae, 
                static_cast<float>(control_rect.right - control_rect.left), 
                12.0f);
        }
#endif //_NA_002884_AC_DIALOG_RENEWAL

        // 텍스트 출력
        info_detail_text_.SetRect(control_rect);
        info_detail_text_.convertLineInfo();
        info_detail_text_.Render();
    }
}

void uiAreaConquestDialog::CallBackRenderACDetailInfo(CControlWZ* control_ptr, void* parameter)
{
    uiAreaConquestDialog* ac_dialog = reinterpret_cast<uiAreaConquestDialog*>(parameter);
    if (ac_dialog == NULL)
    {
        return;
    }

    ac_dialog->RenderACInfoDetailControl();
}

#ifdef _NA_006541_RENEWER_ACDIALOG
void uiAreaConquestDialog::MakeCompoisiteTreeitem( HTREEITEM perentItem,void* Item ,bool percent)
{
    _stAcTreelist* item_ptr = (_stAcTreelist*)Item;

    if(!item_ptr)
        return;

    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(item_ptr->iStringCode, temp_string);

    DWORD Info_total_count = 0;
    DWORD accomplishment_count = 0;
    float accomplishment_ratio = 0;
    DWORD info_reward_counter = 0;

    if(item_ptr->isMaxDepth())
    {
        area_conquest_info()->RefreshAchievementInfo(item_ptr->nation_code_, uiAreaConquest::kSelect_All);

        Info_total_count = area_conquest_info()->GetAchievementInfoTotalCount();
        accomplishment_count = area_conquest_info()->GetAccomplishmentCount();

        for (unsigned int i = 0; i < Info_total_count; ++i)
        {
            AchievementInfo* ac_info = area_conquest_info()->GetAchievementInfo(i);
            bool IsAccomplishment = ac_info->IsAccomplishment();
            bool enable_reward = false;

            if(IsAccomplishment && ac_info->IsReward() == false)
            {
                info_reward_counter++;
            }

            if (IsAccomplishment)
            {
                accomplishment_ratio += static_cast<float>(ac_info->GetAccomplishmentRatio() / 100.0f);
            }
        }
       
        if(item_ptr->PerentItem)
        {
            item_ptr->PerentItem->item_max_count_ += Info_total_count;
            item_ptr->PerentItem->item_current_count_ += accomplishment_count;
        }
        
    }

    // 혹시 모를 예외처리
    if (accomplishment_ratio > 100.0f)
    {
        accomplishment_ratio = 100.0f;
    }

    HTREEITEM child_Item = 
        tree_control_ptr_->AddTreeItemGauge(temp_string,
        0,
        0,
        true,
        StrToWzID("st10"),
        255,
        Item
        ,perentItem);

    GaugeTreeItem* Gauge_item = (GaugeTreeItem*)child_Item;
    Gauge_item->IDStatic = item_ptr->index_to_ac_;
    if (percent)
    {
        Gauge_item->SetRange(0, 100);
        Gauge_item->SetPos(static_cast<int>(accomplishment_ratio));
        Gauge_item->SetPercent(percent);
        Gauge_item->SetPercentString(accomplishment_ratio);  
        Gauge_item->SetCheckItem(info_reward_counter);
    }
    else
    {
        Gauge_item->SetRange(0,Info_total_count);
        Gauge_item->SetPos(accomplishment_count);
        Gauge_item->SetPercent(percent);
        Gauge_item->SetCheckItem(info_reward_counter);
    }
    

    for (int i = 0 ; i < (int)item_ptr->childTree.size() ; ++i)
    {
         MakeCompoisiteTreeitem(child_Item,item_ptr->childTree[i],percent);
    }

}
#ifdef _NA_006541_RENEWER_ACDIALOG
void uiAreaConquestDialog::ReflushForceMove( int Cell,RECT size )
{

    InitControlReflush();
    //디테일값 - 숨겨질 위치의 인덱스 검출
    //시작점 +1 ~ size의 디테일값
    
    WzID scroll_control_id = GetControlID_byPosition(kControl_VScroll);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));

    if(Cell == 20)
    {
        return;
    }

    CControlWZ* control = NULL;
    //최소한의 카운터 20반복은 비효용
    int isdetail = (size.bottom / 30) > 20 ? 20 : (size.bottom / 30);
    int index = 0;
    int scroll_add_index = 0;
    for (;;)
    {
        bool isin = false;
        if(isdetail == 0)
        {
            break;
        }

        if(kControl_Button_Object_01 + (index+(Cell+1)) <= kControl_Button_Object_20)
        {
            control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + (index+(Cell+1))));
            isin = isInRecttoRect(size,control->GetSizeRect());
            if(control && isin)
            {
                control->GetSizeRect();
                control->ShowWindowWZ(WZ_HIDE);
                scroll_add_index++;
            }

			if(!isin) 
            {
                break;
            }

			
        }
        if(kControl_Button_Picture_01 + (index+(Cell+1)) <= kControl_Button_Picture_20)
        {
            control = GetControlWZ(GetControlID_byPosition(kControl_Button_Picture_01 + (index+(Cell+1))));
            if(control)
            {
                control->ShowWindowWZ(WZ_HIDE);
            }
        }
        if(kControl_Button_Image_01 + (index+(Cell+1)) <= kControl_Button_Image_20)
        {
            control = GetControlWZ(GetControlID_byPosition(kControl_Button_Image_01 + (index+(Cell+1))));
            if(control)
            {
                control->ShowWindowWZ(WZ_HIDE);
            }
        }
        if(kControl_Button_Reward_01 + (index+(Cell+1)) <= kControl_Button_Reward_20)
        {
            control = GetControlWZ(GetControlID_byPosition(kControl_Button_Reward_01 + (index+(Cell+1))));
            if(control && control->GetState() == WZ_SHOW )
            {
                hide_control_reward.push_back(kControl_Button_Reward_01 + (index+(Cell+1)));
                control->ShowWindowWZ(WZ_HIDE);
            }
        }

        ++index;
        --isdetail;
       
    }

    if(scroll_control)
    {
        
        int min = 0,max = 0,check_expend = 0;
        scroll_control->GetScrollRangeWZ(&min,&max);
        //더 확장해야되는가 줄어야 되는가
        if(expend_item_ != scroll_add_index)
        {
            int recheck_item = expend_item_;
            int scroll_max = 0;
            int info_list_num = area_conquest_info()->GeAchievementInfoSize();
            expend_item_ = scroll_add_index;
            //확장 값 - 현재 맥스값 - 초기맥스값
            check_expend = expend_item_ - recheck_item;
        }
        scroll_control->SetScrollRangeWZ(min,max+check_expend);
    }

    ReflushList();

}

void uiAreaConquestDialog::ReflushList()
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control != NULL)
    {
        current_scroll_position_ = scroll_control->GetScrollPosWZ();
        //current_scroll_pos = scroll_control->GetScrollPosWZ();
    }

    const POSTYPE icon_control_pos = kControl_Button_Image_01;
    int index_info = current_scroll_position_; // 스크롤 위치부터
    int num_info = area_conquest_info()->GeAchievementInfoSize();
    for (int index_display = 0; index_display < kDisplay_AC_Info_Num; ++index_display, ++index_info)
    {

        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + index_display));

        if(control&& control->GetState() == WZ_HIDE)
        {
            index_info--;
            continue;    
        }

        if (index_info >= num_info)
        {
            ClearAcInfoToDisplay(index_display);
        }
        else
        {
            SetACData(index_display, index_info);
        }
    }
}

void uiAreaConquestDialog::InitControlReflush()
{
    //값 초기화
    for (int i = 0 ; i < (kControl_Button_Object_20 - kControl_Button_Object_01)+1 ; i++)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_Object_01 + i));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ(GetControlID_byPosition(kControl_Button_Picture_01 + i));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
        control = GetControlWZ(GetControlID_byPosition(kControl_Button_Image_01 + i));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
    }

    for (int reward_ID = 0 ; reward_ID < (int)hide_control_reward.size() ; ++reward_ID)
    {
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(hide_control_reward[reward_ID]));
        if(control)
        {
            control->ShowWindowWZ(WZ_SHOW);
        }
    }

    hide_control_reward.clear();

    ReflushList();
}

bool uiAreaConquestDialog::isInRecttoRect( const RECT& from,const RECT& to )
{
    int iscomplate = 0;

    if(from.bottom > to.bottom) iscomplate++;
    if(to.top >= from.top) iscomplate++;

    return (iscomplate == 2);
}

void uiAreaConquestDialog::UpdateValue()
{
    uiAreaConquestMan* area_conquest_manager = GET_DIALOG_MANAGER(uiAreaConquestMan, UIMAN_AREACONQUEST);

    if(area_conquest_manager)
    {
        tree_control_ptr_->DeleteAllItem();

        //컨트롤 상태 변경
        for (int i = 0 ; i < (int)area_conquest_manager->GetAcDataList().size() ; ++i)
        {
            area_conquest_manager->GetAcDataList()[i]->item_max_count_ = 0;
            area_conquest_manager->GetAcDataList()[i]->item_current_count_ = 0;

            bool percent = false;
            if(i == 0)
            {
                percent = true;
            }

            MakeCompoisiteTreeitem(NULL,(void*)area_conquest_manager->GetAcDataList()[i],percent);

            GaugeTreeItem* Gauge_item = (GaugeTreeItem*)tree_control_ptr_->GetTreeItem()->child_list_[i];
            if(Gauge_item)
            {
                Gauge_item->SetRange(0,area_conquest_manager->GetAcDataList()[i]->item_max_count_);
                Gauge_item->SetPos(area_conquest_manager->GetAcDataList()[i]->item_current_count_);
            }
        }

        tree_control_ptr_->Reflush();

        if (selected_area_code_ != 0)
        {
            AllClearACDialog(selected_area_code_);
        }
    }
}

void uiAreaConquestDialog::AllClearACDialog(int selected_item)
{
    uiAreaConquestMan* area_conquest_manager = GET_DIALOG_MANAGER(uiAreaConquestMan, UIMAN_AREACONQUEST);

    if(area_conquest_manager)
    {
        if(selected_item != -1)
        {
            HTREEITEM Selected_item = NULL;

            for(int i = 0 ; i < (int)tree_control_ptr_->GetTreeItem()->child_list_.size() ; ++i)
            {
                HTREEITEM item = tree_control_ptr_->GetTreeItem()->child_list_[i];

                bool check = CheckNationTree(item,&Selected_item,selected_item);
                if(Selected_item == NULL)
                {
                    Selected_item = item;
                }
            }

            tree_control_ptr_->SetSellItem(Selected_item);

            _stAcTreelist* user_ptr = 
                (_stAcTreelist*)tree_control_ptr_->GetSelItem()->tree_data.user_data_Ptr_;


            ShowControl(kControl_Button_Object_Detail, false);
            ShowControl(kControl_Button_Detail_Reward, false);

            InitControlReflush();

            ac_detail_display_index_ = INVALID_SLOT_POS;
            selected_achievement_info_ = NULL;
            expend_item_ = 0;
            check_expend_item_ = 0;

            OnSelectArea(user_ptr->nation_code_);
            OnUpdateACInfo();

            InitControlReflush();

            ac_detail_display_index_ = INVALID_SLOT_POS;
            selected_achievement_info_ = NULL;
            expend_item_ = 0;
            check_expend_item_ = 0;
        }
    }
}

bool uiAreaConquestDialog::CheckNationTree( const HTREEITEM item,HTREEITEM* Outdata,int selected_item)
{
    _stAcTreelist* user_ptr = 
        (_stAcTreelist*)item->tree_data.user_data_Ptr_;
    
    if(item->is_leaf() && user_ptr->nation_code_ == selected_item)
    {
        return true;
    }

    for(int i = 0 ; i < (int)item->child_list_.size() ; ++i)
    {
        HTREEITEM item_child = item->child_list_[i];
        if(CheckNationTree(item_child,Outdata,selected_item))
        {
            *Outdata = item_child;
            return true;
        }
    }

    return false;
}

#endif
#endif
