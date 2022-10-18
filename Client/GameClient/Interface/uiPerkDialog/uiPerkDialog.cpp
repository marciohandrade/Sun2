#include "SunClientPrecompiledHeader.h"

#include "uiPerkDialog.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "SCPerkSlot.h"
#include "ItemUnitRenderer.h"
#include "InterfaceManager.h"
#include "MouseHandler.h"
#include "GlobalFunc.h"
#include "Hero.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "GameFramework.h"
#include "uiInventoryMan/uiInventoryDef.h"
#include "InventoryDialog.h"
#include "SCItemSlot.h"
#include "OptionListParser.h"
#include "BattleScene.h"
#include "Map.h"
#include "IconMgr.h"
#include "Char_SkillDialog.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
#include "SkillSystem/PerkDataContainer.h"
#include "HeroGaugeDialog2.h"
#include "HeroActionInput.h"
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

const int kMaxOptionLine = 4;
const DWORD message_font_id = StrToWzID("mn12");
const DWORD option_font_id = StrToWzID("st10");

uiPerkDialog::uiPerkDialog(InterfaceManager* ui_manager):
    uiBase(ui_manager),
    ui_skill_manager_(NULL),
    unit_renderer_(NULL),
    is_mouse_over_in_perk_slot_(false),
    mouse_over_perk_slot_(INVALID_POSTYPE_VALUE),
    is_store_perk_data_(false)
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM    
    ,active_group_number_(0),
    active_group_number_prototype_(0),
    is_prototype_receive(FALSE)    
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM    
{
}

uiPerkDialog::~uiPerkDialog()
{
}

void uiPerkDialog::RegisterControl()
{
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    // 탭버튼
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C005);  
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C006);  
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C000);  
                                                                        
                                                                        
    RegisterControlPosition(StrToWzID("P317"), kControl_Button_B010);   
    RegisterControlPosition(StrToWzID("P318"), kControl_Button_B000);   
    RegisterControlPosition(StrToWzID("P319"), kControl_Button_B001);   
    RegisterControlPosition(StrToWzID("P320"), kControl_Button_B002);   
    RegisterControlPosition(StrToWzID("P321"), kControl_Button_B003);   
                                                                        
    RegisterControlPosition(StrToWzID("P322"), kControl_Button_B005);   
    RegisterControlPosition(StrToWzID("P323"), kControl_Button_B006);   
    RegisterControlPosition(StrToWzID("P324"), kControl_Button_B008);   
    RegisterControlPosition(StrToWzID("P325"), kControl_Button_B009);   
    RegisterControlPosition(StrToWzID("P326"), kControl_Button_B011);   
                                                                        
    RegisterControlPosition(StrToWzID("P327"), kControl_Button_B013);   
    RegisterControlPosition(StrToWzID("P328"), kControl_Button_B014);   
    RegisterControlPosition(StrToWzID("P329"), kControl_Button_B015);   
    RegisterControlPosition(StrToWzID("P330"), kControl_Button_B016);   
    RegisterControlPosition(StrToWzID("P331"), kControl_Button_B017);   
                                                                        
    RegisterControlPosition(StrToWzID("P337"), kControl_Button_B004);   
    RegisterControlPosition(StrToWzID("P338"), kControl_Button_B012);   
    RegisterControlPosition(StrToWzID("P339"), kControl_Button_B018);   
                                                                        
    RegisterControlPosition(StrToWzID("P313"), kControl_Button_B019);   
    RegisterControlPosition(StrToWzID("P314"), kControl_Button_B020);   
    RegisterControlPosition(StrToWzID("P315"), kControl_Button_B021);   
    RegisterControlPosition(StrToWzID("P316"), kControl_Button_B022);   
    RegisterControlPosition(StrToWzID("P271"), kControl_Button_B023);   
                                                                        
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B024);   
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B025);   
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B026);   
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B027);   
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B028);   
                                                                        
    RegisterControlPosition(StrToWzID("R000"), kControl_Rect_R000);     
    RegisterControlPosition(StrToWzID("R003"), kControl_Rect_R002);     
    RegisterControlPosition(StrToWzID("R001"), kControl_Rect_R004);
    RegisterControlPosition(StrToWzID("R002"), kControl_Rect_R005);     

    //RegisterControlPosition(StrToWzID("V000"), kControl_Scroll_V000); 
    //RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);

    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P001);

    RegisterControlPosition(StrToWzID("B086"), kControl_Button_B999);   
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM    
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P017);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005_New);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
#else
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
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
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("R000"), kControl_Rect_R000);
    RegisterControlPosition(StrToWzID("R002"), kControl_Rect_R002);
    RegisterControlPosition(StrToWzID("R004"), kControl_Rect_R004);
    RegisterControlPosition(StrToWzID("R005"), kControl_Rect_R005);
    RegisterControlPosition(StrToWzID("V000"), kControl_Scroll_V000);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);

#endif
}

void uiPerkDialog::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    RegisterPerkControlMessenger(this);
    SetSlotIdx(SI_PERK);
    RegisterControl();
    SetButtonControlBySlotPos();

    unit_renderer_ = new ItemUnitRenderer;
    assert(unit_renderer_);
    if (unit_renderer_)
    {
        unit_renderer_->Init(GetDrawBase(), kMaxNumberOfNodes);
        unit_renderer_->SetOverLap(FALSE);
    }

    ui_skill_manager_ = static_cast<uiSkillMan*>(GetMan());
    LoadPerkLevelImage();

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

    if( CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P001)) )
    {
        control->ShowWindowWZ(WZ_HIDE);
        control->SetDiscardMessage(true);
    }

    for( size_t i=kControl_Button_B010;i<=kControl_Button_B023;++i)
    {
        if( CCtrlPictureWZ* control = (CCtrlPictureWZ*)GetControlWZ(GetControlID_byPosition(i)) )
        {
            control->EnableLButtonEvent(true);
        }
    }
#endif

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    // Page 미리 셋팅.
    current_page_ = eStylePageA;
    active_group_number_ = eStylePageA;
    active_group_number_prototype_ = eStylePageA;
    is_wait_packet_ = false;

    // vector 미리 할당.
    my_perk_data_.reserve(kPerkGroupMax);
    my_prototype_perk_data_.reserve(kPerkGroupMax);
    for ( unsigned int i = 0; i < kPerkGroupMax; ++i)
    {
        PerkGroupStruct temp;
        my_perk_data_.push_back(temp);
        my_prototype_perk_data_.push_back(temp);
    }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
}

void uiPerkDialog::LoadPerkLevelImage()
{
    perk_level_image.clear();
    perk_level_image.reserve(KMaxPerkLevel);
    const DWORD perk_level_image_code[KMaxPerkLevel] = {36898, 36899, 36900, 36901, 36902};
    IconImage* icon_image = NULL;
    for (int index = 0; index < KMaxPerkLevel; ++index)
    {
        icon_image = ICONMGR()->Add(perk_level_image_code[index]);
        perk_level_image.push_back(icon_image);
    }
}

void uiPerkDialog::RenderPerkLevelImage(int perk_level, RECT* draw_rect)
{
    uiPerkDialog* perk_dialog = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);
    if (perk_dialog != NULL)
    {
        IconImage* perk_level_image = perk_dialog->GetPerkLevelImage(perk_level);
        if (perk_level_image != NULL)
        {
            perk_level_image->Render(draw_rect);
        }
    }
}

void uiPerkDialog::Release()
{
    if (unit_renderer_ != NULL)
    {
        unit_renderer_->FlushUnitDraw();
        unit_renderer_->Release();
        delete unit_renderer_;
        unit_renderer_ = NULL;
    }

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    my_prototype_perk_data_.clear();
    my_perk_data_.clear();
    is_wait_packet_ = false;
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
}

void uiPerkDialog::ClearAll()
{
    for (int slot_pos = 0; slot_pos < GetMaxSlotNum(); slot_pos++)
    {
        if(IsEmpty(slot_pos) == FALSE)
        {
            DeleteSlot(slot_pos, NULL);
        }
    }
    SCPerkSlotContainer::ClearAll();
    is_store_perk_data_ = false;

    benefit_option_map_.clear();
    penalty_option_map_.clear();

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    current_page_ = eStylePageA;
    active_group_number_ = eStylePageA;
    active_group_number_prototype_ = eStylePageA;

    // vector 미리 할당.
    my_perk_data_.reserve(kPerkGroupMax);
    my_prototype_perk_data_.reserve(kPerkGroupMax);
    for ( unsigned int i = 0; i < kPerkGroupMax; ++i)
    {        
        for ( unsigned j = 0; j < 4; ++j )
        {
            my_perk_data_[i].from_pos[j] = INVALID_POSTYPE_VALUE;
            my_prototype_perk_data_[i].from_pos[j] = INVALID_POSTYPE_VALUE;
        }
    }
    is_prototype_receive = FALSE;
    active_group_number_ = eStylePageA;
    active_group_number_prototype_ = eStylePageA;
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
}

void uiPerkDialog::OnShowWindow(BOOL val)
{
    ui_skill_manager_->OpenDialog(uiSkillMan::kSkillPerk, val);

    if (val)
    {
        RefreshOptionText();
        RefreshTab();
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        Refresh_PerkSlock_My_Data();
        
        current_page_ = active_group_number_;
        switch (active_group_number_)
        {
        case eStylePageA:
            OnClickAGroupButton(0 , 0);
            break;
        case eStylePageB:
            OnClickBGroupButton(0 , 0);
            break;
        }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }
    else
    {
        // PerkDialog 에서 집어올린 거라면 내려놓는다.
        if (MouseHandler::Instance()->IsExistItemAtHand() == TRUE)
        {
            RollbackPerkItemAtHand();
        }
        is_mouse_over_in_perk_slot_ = false;
        mouse_over_perk_slot_ = INVALID_POSTYPE_VALUE;

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        // 처음으로 서버에서 받은 정보와 현재 정보와 맞지 않을 경우 저장
        if ( is_prototype_receive )
        {
            SavePerkGroupListData();
        }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }
}

void uiPerkDialog::OnRenderSolarDialog()
{
    if (IsVisibleWindow())
    {
        for (int rect_index = kFTextRenderTop; rect_index < kFTextRenderMax; ++rect_index)
        {
            text_render_[rect_index].Render();
        }

        if (unit_renderer_ != NULL)
        {
            unit_renderer_->Render();
        }
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        // 현재 액티브 그룹을 보고 있다는 것을 표시해 주자.        
        if ( current_page_ == active_group_number_ )
        {
            for (int i = kControl_Button_B019; i <= kControl_Button_B022; ++i)
            {
                CCtrlButtonWZ* button_control_ptr = static_cast<CCtrlButtonWZ*>(GetControlWZ
                    (GetControlID_byPosition(i)));                
                if ( button_control_ptr )
                {                    
                    RECT button_control_rect;
                    button_control_rect = button_control_ptr->GetSizeRect();
                    button_control_rect.left -= 14;
                    button_control_rect.right += 14;
                    button_control_rect.top -= 14;
                    button_control_rect.bottom += 13;
                    g_InterfaceManager.RenderActivateMarkPerkGroup(button_control_rect.left, button_control_rect.top, 
                        button_control_rect.right - button_control_rect.left,
                        button_control_rect.bottom - button_control_rect.top);
                }
            }
        }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

        // 비활성 이미지 출력
        if( CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Picture_P001)) )
        {
            control_ptr->ShowWindowWZ( IsEnableSetting() == false ? WZ_SHOW : WZ_HIDE );    // 비활성이면 표시
        }
#else

        if (IsEnableSetting() == false)
        {
            // 비활성 이미지 출력
            CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(kControl_Picture_P001));
            if (control_ptr != NULL)
            {
                static int alpha_value = 125;
                RECT control_rect = control_ptr->GetSizeRect();
                g_InterfaceManager.RenderRect(control_rect, WzColor_RGBA(0, 0, 0, alpha_value));
            }
        }
#endif

        if (is_mouse_over_in_perk_slot_ && 
            (mouse_over_perk_slot_ != INVALID_POSTYPE_VALUE))
        {
            CControlWZ* control_ptr = GetButtonControlBySlotPos(mouse_over_perk_slot_);
            if (control_ptr != NULL)
            {
                ENUM_DRAWSTATE control_draw_state = control_ptr->GetDrawState();
                if (control_draw_state == E_BASE)
                {
                    is_mouse_over_in_perk_slot_ = false;
                    mouse_over_perk_slot_ = INVALID_POSTYPE_VALUE;
                }
                else
                {
                    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

                    if (tooltip_manager != NULL)
                    {
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
                        // 30 ~ 34-> pos가 active 슬롯
                        if ( mouse_over_perk_slot_ >= 30 && mouse_over_perk_slot_ < 34 )
                        {
                            if (my_perk_data_[current_page_].from_pos[mouse_over_perk_slot_ - 30] == INVALID_POSTYPE_VALUE)
                                return;

                            // 0 ~ 14가 default slot의 pos
                            if (!(my_perk_data_[current_page_].from_pos[mouse_over_perk_slot_ - 30] >= 0 &&
                                my_perk_data_[current_page_].from_pos[mouse_over_perk_slot_ - 30] <= 14))
                                return;

                            SCPerkSlot& perk_slot = 
                                static_cast<SCPerkSlot&>(GetSlot(my_perk_data_[current_page_].from_pos[mouse_over_perk_slot_ - 30]));
                            tooltip_manager->RegisterTooltipPerkInfo(&perk_slot, mouse_over_perk_slot_);
                            tooltip_manager->SetTooltipDialogID(GetDialogKey());
                        }
                        else
                        {
                            SCPerkSlot& perk_slot = 
                                static_cast<SCPerkSlot&>(GetSlot(mouse_over_perk_slot_));
                            tooltip_manager->RegisterTooltipPerkInfo(&perk_slot, mouse_over_perk_slot_);
                            tooltip_manager->SetTooltipDialogID(GetDialogKey());
                        }
#else
                        SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(mouse_over_perk_slot_));
                        tooltip_manager->RegisterTooltipPerkInfo(&perk_slot, mouse_over_perk_slot_);
						tooltip_manager->SetTooltipDialogID(GetDialogKey());
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
                    }
                }
            } 
        }
    }
}

void uiPerkDialog::OnUpdateSolarDialog()
{
    if (IsVisibleWindow())
    {
        RefreshTopMessage();

        WzID rect_control[kFTextRenderMax] = {GetControlID_byPosition(kControl_Rect_R000),
            GetControlID_byPosition(kControl_Rect_R002), 
            GetControlID_byPosition(kControl_Rect_R004),
            GetControlID_byPosition(kControl_Rect_R005)};

        CCtrlRectangleWZ* rect_control_ptr = NULL;
        RECT rect;
        for (int rect_index = kFTextRenderTop; rect_index < kFTextRenderMax; ++rect_index)
        {
            rect_control_ptr = static_cast<CCtrlRectangleWZ*>(GetControlWZ(rect_control[rect_index]));
            if (rect_control_ptr != NULL)
            {
                rect = rect_control_ptr->GetSizeRect();
                text_render_[rect_index].SetRect(rect);
            }
        }
    }    
}

void uiPerkDialog::SetButtonControlBySlotPos()
{
    button_control_map_.clear();

    const POSTYPE slot_num[3] = {kNumberOfDefaultSlots, kNumberOfCashSlots, kNumberOfActiveSlots};
    const POSTYPE start_slot_pos[3] = {kDefaultPerkButton_Start, 
                                       kCashPerkButton_Start, 
                                       kActivePerkButton_Start};
    const POSTYPE end_slot_pos[3] = {kDefaultPerkButton_End, 
                                     kCashPerkButton_End, 
                                     kActivePerkButton_End};
    const POSTYPE* use_slot_pos_by_type[3] = {PerkDataList::kDefaultSlotPositions, 
                                              PerkDataList::kCashSlotPositions, 
                                              PerkDataList::kActiveSlotPositions};
    pair<POSTYPE, CControlWZ*> temp_data_pair;
    CControlWZ* button_control = NULL;
    POSTYPE button_position;
    POSTYPE slot_pos;

    for (int slot_type = 0; slot_type < 3; ++slot_type)
    {
        for (int index = 0; index < slot_num[slot_type]; ++index)
        {
            button_position = start_slot_pos[slot_type] + index;
            if (button_position >= end_slot_pos[slot_type])
            {
                break;
            }
            const POSTYPE* use_slot_pos = use_slot_pos_by_type[slot_type];
            slot_pos = use_slot_pos[index];
            button_control = GetControlWZ(GetControlID_byPosition(button_position));
            temp_data_pair = pair<POSTYPE, CControlWZ*>(slot_pos, button_control);
            button_control_map_.insert(temp_data_pair);
        }
    }    


}

CControlWZ* uiPerkDialog::GetButtonControlBySlotPos(POSTYPE slot_pos)
{
    map<POSTYPE, CControlWZ*>::iterator map_itr_end = button_control_map_.end();
    map<POSTYPE, CControlWZ*>::iterator find_itr = button_control_map_.find(slot_pos);
    if (find_itr != map_itr_end)
    {
        return find_itr->second;
    }
    return NULL;
}

RC::eSLOT_INSERT_RESULT uiPerkDialog::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    RC::eSLOT_INSERT_RESULT result_code = SCPerkSlotContainer::InsertSlot(at_pos, slot);
    if (result_code != RC::RC_INSERT_SLOT_SUCCESS)
    {
        return result_code;
    }

    SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(at_pos));
    const SCPerkSlot* perk_slot_ptr = NULL;
    CControlWZ* button_control = GetButtonControlBySlotPos(at_pos);
    if (button_control != NULL)
    {
        // unit_render 추가
        
        bool is_active_slot = perk_slot.is_active_slot();
        if (is_active_slot == true)
        {
            // 액티브Perk 슬롯처리
            perk_slot_ptr = perk_slot.GetLinkSlot();
        }
        else
        {
            // 디폴트/캐시Perk 슬롯처리
            perk_slot_ptr = &perk_slot;
        }

        if ((perk_slot_ptr != NULL) && 
            (perk_slot_ptr->perk_info() != NULL))
        {
            RECT control_rect = button_control->GetSizeRect();
            DWORD perk_group = perk_slot.GetPerkID().group_;
            unit_renderer_->AddItemUnit(perk_group, 
                0, 
                &control_rect, 
                perk_slot_ptr, 
                button_control);
        }
    }   

    return result_code;
}

BOOL uiPerkDialog::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    SCPerkSlot temp_slot;
    if (slot_clone_before_delete == NULL)
    {
        slot_clone_before_delete = &temp_slot;
    }

    BOOL is_delete = SCPerkSlotContainer::DeleteSlot(at_pos, slot_clone_before_delete);
    if ((is_delete == TRUE) &&
        (slot_clone_before_delete != NULL) &&
        (unit_renderer_ != NULL))
    {
        // unit_render 제거 - 슬롯타입상관 없이 unit_render 제거
        SCPerkSlot* perk_slot = static_cast<SCPerkSlot*>(slot_clone_before_delete);
        DWORD perk_group = perk_slot->GetPerkID().group_;
        unit_renderer_->RemoveItemUnit(perk_group);
    }

    return is_delete;
}

void uiPerkDialog::UpdateSlot(POSTYPE at_pos, int changed_num)
{
    // UpdateSlot 슬롯함수는 Active슬롯에 대해서만 호출된다.
    // changed_num  -   0   :   상태업데이트(레벨업등), 이 경우 처리할 일은 없다.
    //                  1   :   다른Perk이 등록됨.
    //                 -1   :   Cash Active 슬롯의 해제(Cash Perk의 만료)
    SCPerkSlotContainer::UpdateSlot(at_pos, changed_num);

    if (unit_renderer_ == NULL)
    {
        return;
    }
    SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(at_pos));
    if (perk_slot.is_active_slot() == false)
    {
        return;
    }

    const SCPerkSlot* link_perk_slot = perk_slot.GetLinkSlot();

    DWORD perk_group = perk_slot.GetPerkID().group_;
    if (changed_num == 1)   // 액티브Perk슬롯에 Perk link
    {
        if (link_perk_slot == NULL)
        {
            return;
        }

        const PerkInfo* link_perk_info = link_perk_slot->perk_info();
        if (link_perk_info != NULL)
        {
            // 컨트롤을 얻어와야 한다.
            CControlWZ* button_control = GetButtonControlBySlotPos(at_pos);
            if (button_control != NULL)
            {
                
                RECT control_rect = button_control->GetSizeRect();
                unit_renderer_->RemoveItemUnit(perk_group);
                unit_renderer_->AddItemUnit(perk_group, 
                                           0, 
                                           &control_rect, 
                                           link_perk_slot, 
                                           button_control);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
                // 0 ~ 3까지 active 슬롯을 의미                
                if (at_pos - 30 >= 0 && at_pos - 30 <= 3)
                {
                    my_perk_data_[active_group_number_].from_pos[at_pos-30] = 
                        link_perk_slot->GetPos();
                }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
            }
        }
    }
    else if (changed_num == -1) // 액티브Perk슬롯에 Perk unlink
    {
        unit_renderer_->RemoveItemUnit(perk_group);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        if (at_pos - 30 >= 0 && at_pos - 30 <= 3)
        {
            my_perk_data_[active_group_number_].from_pos[at_pos-30] = 
                INVALID_POSTYPE_VALUE;
        }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }
    else if (changed_num == 0)  // 상태업데이트(레벨업등)
    {
        ItemUnitRenderer::RenderUnit* render_unit = unit_renderer_->GetItemUnit(perk_group);
        if (render_unit != NULL)
        {
            render_unit->m_pRefSlot = link_perk_slot;
        }        
    }
}

void uiPerkDialog::MessageProc(SI_MESSAGE * pMessage)
{
    if (IsLock())
        return;

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(pMessage);
        break;
    case RT_MSG_MOUSEOVER:
        OnMouseOver(pMessage);
        break;
#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    case RT_MSG_SB_PAGETOP://. 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP://.윗쪽 화살표를 눌렀다.
        OnUpScroll(pMessage);
        break;
    case RT_MSG_SB_PAGEBOTTOM://. 아래쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM://.아랫쪽 화살표를 눌렀다
        OnDownScroll(pMessage);
        break;
    case RT_MSG_SB_THUMBTRACK://. 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION://. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL://. 스크롤이 종료 되었다.
        OnThumbScroll(pMessage);
        break;
#endif
    default:
        SolarDialog::MessageProc(pMessage);
        break;
    }
}

POSTYPE uiPerkDialog::GetSlotPositionByControlPosition(DWORD control_postion)
{
    POSTYPE slot_postion = INVALID_POSTYPE_VALUE;
    int index_offset = 0;
    if (control_postion < kDefaultPerkButton_End)
    {
        index_offset = control_postion - kDefaultPerkButton_Start;
        if (index_offset < kNumberOfDefaultSlots)
        {
            slot_postion = static_cast<POSTYPE>(PerkDataList::kDefaultSlotPositions[index_offset]);
        }        
    }
    else if (control_postion < kCashPerkButton_End)
    {
        index_offset = control_postion - kCashPerkButton_Start;
        if (index_offset < kNumberOfCashSlots)
        {
            slot_postion = static_cast<POSTYPE>(PerkDataList::kCashSlotPositions[index_offset]);
        }        
    }
    else if (control_postion < kActivePerkButton_End)
    {
        index_offset = control_postion - kActivePerkButton_Start;
        if (index_offset < kNumberOfActiveSlots)
        {
            slot_postion = static_cast<POSTYPE>(PerkDataList::kActiveSlotPositions[index_offset]);
        }        
    }
    return slot_postion;
}

bool uiPerkDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_postion = GetControlPosition_byID(message->dwCtrlID);
    switch (control_postion)
    {
        // 탭버튼
    case kControl_CheckBT_C005:
    case kControl_CheckBT_C006:
    case kControl_CheckBT_C000:
        {
            RefreshTab();
            int tab_index = (control_postion - kControl_CheckBT_C005) + 1;
            ui_skill_manager_->OpenTab(tab_index);
        }
        break;
        // 일반Perk 슬롯
    case kControl_Button_B010:
    case kControl_Button_B000:
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B005:
    case kControl_Button_B006:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B011:
    case kControl_Button_B013:
    case kControl_Button_B014:
    case kControl_Button_B015:
    case kControl_Button_B016:
    case kControl_Button_B017:
        // 캐시 액티브Perk 슬롯
    case kControl_Button_B004:
    case kControl_Button_B012:
    case kControl_Button_B018:
        {
            OnClickPerkPalette(control_postion, message->dwCtrlID);
        }
        break;
        // Perk 액티브Perk 슬롯
    case kControl_Button_B019:
    case kControl_Button_B020:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
        {
            OnClickActivePerk(control_postion, message->dwCtrlID);
        }
        break;
        // 닫기버튼
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
        // 액티브Perk 등록해제 버튼
    case kControl_Button_B024:
    case kControl_Button_B025:
    case kControl_Button_B026:
    case kControl_Button_B027:
    case kControl_Button_B028:
        {
            OnClickUnregisterButton(control_postion, message->dwCtrlID);
        }
        break;
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    case kControl_CheckBT_C003:    
        {
            OnClickAGroupButton(control_postion, message->dwCtrlID);            
            if (eStylePageA == current_page_)
            {
                return false;
            }
            current_page_ = eStylePageA;            
            ChangeGroupPageLogic();
        }
        break;
    case kControl_CheckBT_C004:    
        {
            OnClickBGroupButton(control_postion, message->dwCtrlID);
            if (eStylePageB == current_page_)
            {
                return false;
            }
            current_page_ = eStylePageB;            
            ChangeGroupPageLogic();
        }
        break;
    case kControl_Button_B005_New:
        {
            // 액션 번호
            g_HeroInput.OnSkill(35023);
        }
        break;
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    default:
        {
        }
        break;
    }
    return result;
}

//------------------------------------------------------------------------------
bool uiPerkDialog::OnMouseOver(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_postion = GetControlPosition_byID(message->dwCtrlID);
    switch (control_postion)
    {
        // 일반Perk 슬롯
    case kControl_Button_B010:
    case kControl_Button_B000:
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B005:
    case kControl_Button_B006:
    case kControl_Button_B008:
    case kControl_Button_B009:
    case kControl_Button_B011:
    case kControl_Button_B013:
    case kControl_Button_B014:
    case kControl_Button_B015:
    case kControl_Button_B016:
    case kControl_Button_B017:
        // 캐시 액티브Perk 슬롯
    case kControl_Button_B004:
    case kControl_Button_B012:
    case kControl_Button_B018:
        // Perk 액티브Perk 슬롯
    case kControl_Button_B019:
    case kControl_Button_B020:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
        {
            mouse_over_perk_slot_ = GetSlotPositionByControlPosition(control_postion);
            if (mouse_over_perk_slot_ != INVALID_POSTYPE_VALUE)
            {
                is_mouse_over_in_perk_slot_ = true;
            }
        }
        break;
    default:
        {
            is_mouse_over_in_perk_slot_ = false;
            mouse_over_perk_slot_ = INVALID_POSTYPE_VALUE;
        }
    }
    return result;
}

void uiPerkDialog::OnClickPerkPalette(DWORD control_position, DWORD control_id)
{
    // 필드/미션이면 동작 안함
    if (IsEnableSetting() == false)
    {
        return;
    }

    // 마우스핸들러에 무엇인가 들려있을 때
    if (MouseHandler::Instance()->IsExistItemAtHand() == TRUE)
    {
        if ((control_position >= kCashPerkButton_Start) && 
            (control_position < kCashPerkButton_End))
        {
            POSTYPE slot_pos = GetSlotPositionByControlPosition(control_position);
            MouseHandler::Instance()->ItemTransaction(SI_PERK, slot_pos, NULL);
        }
        RollbackPerkItemAtHand();
    }
    else
    {
        // Perk을 집어 올릴 때
        // 해당 슬롯에 정보가 있다면 처리 / 없으면 처리하지 않음
        POSTYPE slot_pos = GetSlotPositionByControlPosition(control_position);
        BOOL is_empty_slot = IsEmpty(slot_pos);
        if (is_empty_slot == TRUE)
        {
            return;
        }

        CControlWZ* button_control = GetControlWZ(control_id);
        if (button_control == NULL)
        {
            return;
        }

        RECT control_rect = button_control->GetSizeRect();
        MouseHandler::Instance()->ItemTransaction(SI_PERK, slot_pos, &control_rect);
    }
}

void uiPerkDialog::OnClickActivePerk(DWORD control_position, DWORD control_id)
{
    // 필드/미션이면 동작 안함
    if (IsEnableSetting() == false)
    {
        return;
    }

    // 마우스핸들러에 무엇인가 들려있을 때
    MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();
    if (mouse_item->m_fromSlotIdx == SI_PERK)
    {
        // 들고있던 Perk을 해당 슬롯에 등록
        POSTYPE slot_pos = GetSlotPositionByControlPosition(control_position);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        // 현재 page가 active page가 아닐 때
        if ( current_page_ != active_group_number_ && 
            slot_pos - 30 < 4)
        {
            my_perk_data_[current_page_].from_pos[slot_pos - 30] = mouse_item->m_fromPos;
        }
        
        // slot_pos->39 Cash 액티브 슬롯
        if ( current_page_ == active_group_number_ || 
            slot_pos == 39)
        {
            RequestPerkOperaion(PerkControl::eReq_Register, mouse_item->m_fromPos, slot_pos);            
        }
        else
        {
            ChangeGroupPageLogic();
        }
        MouseHandler::Instance()->RollbackItemAtHand();
#else
        RequestPerkOperaion(PerkControl::eReq_Register, mouse_item->m_fromPos, slot_pos);
        MouseHandler::Instance()->RollbackItemAtHand();
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }
    // 액티브Perk에서 들어올리는 것은 없음.
}

void uiPerkDialog::OnClickUnregisterButton(DWORD control_position, DWORD control_id)
{
    // 필드/미션이면 동작 안함
    if (IsEnableSetting() == false)
    {
        return;
    }

    control_position -= kUnregisterButtonOffest;
    POSTYPE slot_pos = GetSlotPositionByControlPosition(control_position);
    BOOL is_empty_slot = IsEmpty(slot_pos);
    if (is_empty_slot == TRUE)
    {
        return;
    }

    SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(slot_pos));
    bool is_active_slot = perk_slot.is_active_slot();
    bool is_link_slot = (perk_slot.GetLinkSlot() != NULL);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    if (is_active_slot == true)
#else
    if ((is_active_slot == true) &&
        (is_link_slot == true))
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    {
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        if (slot_pos - 30 >= 0 && slot_pos - 30 <= 3)
        {            
            if ( current_page_ != active_group_number_ )
            {
                my_perk_data_[current_page_].from_pos[slot_pos - 30] = INVALID_POSTYPE_VALUE;
                ChangeGroupPageLogic();
            }
            else
            {
                // Active 슬롯이다.
                RequestPerkOperaion(PerkControl::eReq_Unregister, slot_pos, 0);
            }
        }
        else
        {
            if (is_link_slot == true)
            {
                RequestPerkOperaion(PerkControl::eReq_Unregister, slot_pos, 0);
            }
        }
#else
        RequestPerkOperaion(PerkControl::eReq_Unregister, slot_pos, 0);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }
}

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
void uiPerkDialog::OnClickAGroupButton( DWORD control_position, DWORD control_id )
{
    // Tab
    CCtrlButtonCheckWZ* button = NULL;
    WzID control_id_temp = GetControlID_byPosition(kControl_CheckBT_C003);
    button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id_temp));
    if (button)
    {
        button->SetCheckState(true);
    }
    control_id_temp = GetControlID_byPosition(kControl_CheckBT_C004);
    button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id_temp));
    if (button)
    {
        button->SetCheckState(false);
    }    
    ChangeGroupPageLogic();
}

void uiPerkDialog::OnClickBGroupButton( DWORD control_position, DWORD control_id )
{
    // Tab    
    CCtrlButtonCheckWZ* button = NULL;
    WzID control_id_temp = GetControlID_byPosition(kControl_CheckBT_C004);
    button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id_temp));
    if (button)
    {
        button->SetCheckState(true);
    }
    control_id_temp = GetControlID_byPosition(kControl_CheckBT_C003);
    button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id_temp));
    if (button)
    {
        button->SetCheckState(false);
    }    
    ChangeGroupPageLogic();
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL


bool uiPerkDialog::OnUpScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V000);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        --current_scroll_pos;
        current_scroll_pos = max(0, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        text_render_[kFTextRenderOption1].SetBeginLineIndex(current_scroll_pos);
        text_render_[kFTextRenderOption2].SetBeginLineIndex(current_scroll_pos);
    }

    return result;
}

bool uiPerkDialog::OnDownScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V000);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        ++current_scroll_pos;
        current_scroll_pos = min(kMaxOptionLine, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        text_render_[kFTextRenderOption1].SetBeginLineIndex(current_scroll_pos);
        text_render_[kFTextRenderOption2].SetBeginLineIndex(current_scroll_pos);
    }

    return result;
}

bool uiPerkDialog::OnThumbScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V000);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        current_scroll_pos = max(0, current_scroll_pos);
        current_scroll_pos = min(kMaxOptionLine, current_scroll_pos);
        text_render_[kFTextRenderOption1].SetBeginLineIndex(current_scroll_pos);
        text_render_[kFTextRenderOption2].SetBeginLineIndex(current_scroll_pos);
    }

    return result;
}
#endif
void uiPerkDialog::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C005; position < (kControl_CheckBT_C000 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        check_button->SetCheckState((position == kControl_CheckBT_C006) ? (true) : (false));
    }
}

void uiPerkDialog::NetworkProc(MSG_BASE* recv_msg)
{
    switch (recv_msg->m_byCategory)
    {
    case CG_CHARINFO:
        switch (recv_msg->m_byProtocol)
        {
        case CG_CHARINFO_SKILL_PERK_CMD:
            {
                MSG_CG_CHARINFO_SKILL_PERK_CMD* recv_packet 
                    = static_cast<MSG_CG_CHARINFO_SKILL_PERK_CMD*>(recv_msg);
                RECV_CG_CHARINFO_SKILL_PERK_CMD(recv_packet);
            }
            break;
        }
        break;
    
    case CG_SKILL:
        switch (recv_msg->m_byProtocol)
        {
        case CG_SKILL_PERK_CONTROL_CMD:
            {
                MSG_CG_SKILL_PERK_CONTROL_CMD* recv_packet 
                    = static_cast<MSG_CG_SKILL_PERK_CONTROL_CMD*>(recv_msg);
                RECV_CG_SKILL_PERK_CONTROL_CMD(recv_packet);
            }
            break;
        case CG_SKILL_PERK_STATUS_CMD:
            {
                MSG_CG_SKILL_PERK_STATUS_CMD* recv_packet 
                    = static_cast<MSG_CG_SKILL_PERK_STATUS_CMD*>(recv_msg);
                RECV_CG_SKILL_PERK_STATUS_CMD(recv_packet);
            }
            break;
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        case CG_SKILL_PERK_GROUP_CONTROL_CMD:
            {
                MSG_CG_SKILL_PERK_GROUP_CONTROL_CMD* recv_packet
                    = static_cast<MSG_CG_SKILL_PERK_GROUP_CONTROL_CMD*>(recv_msg);
                RECV_CG_SKILL_PERK_GROUP_CONTROL_CMD(recv_packet);
                is_wait_packet_ = false;
            }
            break;
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
        }
        break;
    }
    
}

void uiPerkDialog::RECV_CG_CHARINFO_SKILL_PERK_CMD(MSG_CG_CHARINFO_SKILL_PERK_CMD* recv_packet)
{
    // 컨테이너에 세팅
    ClearAll();
    StorePerk(recv_packet->container_, 0);
    is_store_perk_data_ = true;
}

void uiPerkDialog::RECV_CG_SKILL_PERK_CONTROL_CMD(MSG_CG_SKILL_PERK_CONTROL_CMD* recv_packet)
{
    bool result = ProcessReplyMessage(recv_packet->header, recv_packet->data);
    
    if (result == true)
    {
        PerkControl* control_header = &(recv_packet->header);
        POSTYPE item_pos = INVALID_POSTYPE_VALUE;
        bool is_play_sound = false;
        switch (recv_packet->header.request)
        {
        case PerkControl::eReq_Unregister:
            {
                is_play_sound = true;
            }
            break;
        case PerkControl::eReq_Plants:
            {
                item_pos = control_header->args.plants.from_item_pos;
                ItemManager::Instance()->UseInventoryItem(SI_INVENTORY, item_pos);
                is_play_sound = true;
                
            }
            break;
        case PerkControl::eReq_Uproot:
            {                
                InventoryDialog* inventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
                if (inventory)
                {
                    item_pos = control_header->args.uproot.to_item_pos;
                    recv_packet->data.uproot.maked_item;
                    SCItemSlot item_slot(recv_packet->data.uproot.maked_item);
                    inventory->InsertSlot(item_pos, item_slot);
                }
            }
            break;
        }

        if (is_play_sound == true)
        {
            SCPerkSlot temp_perk_slot;
            ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_PUT, 
                                                   &temp_perk_slot);
        }

        RefreshBottomMessage();
        RefreshOptionList();
    }
    else
    {
        ProcessResultCode(recv_packet->header.result, recv_packet->header.result_code);
    }    

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

}

void uiPerkDialog::SEND_CG_SKILL_PERK_CONTROL_SYN(MSG_CG_SKILL_PERK_CONTROL_SYN* send_packet)
{
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, send_packet, sizeof(*send_packet));
}

void uiPerkDialog::RECV_CG_SKILL_PERK_STATUS_CMD(MSG_CG_SKILL_PERK_STATUS_CMD* recv_packet)
{
    bool result = ProcessEventMessage(recv_packet->header, recv_packet->data);

    if (result == true)
    {
        RefreshOptionList();
    }
    else
    {
        ProcessResultCode(recv_packet->header.result, recv_packet->header.result_code);
    } 
}

void uiPerkDialog::RequestPerkOperaion(PerkControl::eRequest request_operation, 
                                       POSTYPE from_pos, 
                                       POSTYPE to_pos)
{
    if (IsRequestTransaction() == true)
    {
        return;
    }

    if (IsEnableSetting() == false)
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {0, };
        // 5536 퍽은 안전지대와 마을에서만 변경 가능합니다.
        g_InterfaceManager.GetInterfaceString(5536, message);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);

        return;
    }

    MSG_CG_SKILL_PERK_CONTROL_SYN request_packet;
    bool result_value = false;

    switch (request_operation)
    {
    case PerkControl::eReq_Register:
        result_value = RequestRegister(&request_packet.header, from_pos, to_pos);
        break;
    case PerkControl::eReq_Unregister:
        result_value = RequestUnregister(&request_packet.header, from_pos);
        break;
    case PerkControl::eReq_Plants:
        result_value = RequestPlants(&request_packet.header, from_pos);
        break;
    case PerkControl::eReq_Uproot:
        result_value = RequestUproot(&request_packet.header, from_pos, to_pos);
        break;
    }

    if (result_value == true)
    {
        SEND_CG_SKILL_PERK_CONTROL_SYN(&request_packet);
    }
    else
    {
        ProcessResultCode(request_packet.header.result, request_packet.header.result_code);
    }
}

void uiPerkDialog::ProcessResultCode(DWORD result, DWORD result_code)
{
    switch (result)
    {
    case PerkControl::eRet_SkillFailed:
        {
            BattleScene::ShowErrorCodeSkill(result_code);
        }
        break;
    case PerkControl::eRet_ItemFailed:
        {
            ItemManager::Instance()->NoticeReasonItemErrorCode(result_code);
        }
        break;
    default:
        {
#ifdef _DEV_VER
            TCHAR debug_message[INTERFACE_STRING_LENGTH] = {0, };
            Snprintf(debug_message, INTERFACE_STRING_LENGTH - 1, "PerkControl Result[%d] ResultCode[%d]"
                , result, result_code);
#endif //_DEV_VER
        }
    }    
}

void uiPerkDialog::RollbackPerkItemAtHand()
{
    MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();
    if (mouse_item->m_fromSlotIdx == SI_PERK)
    {
        MouseHandler::Instance()->RollbackItemAtHand();
    }
}

bool uiPerkDialog::CheckRegisterActivePerk()
{
    for (int index = 0; index < kNumberOfActiveSlots; ++index)
    {
        SCPerkSlot& active_slot = 
            static_cast<SCPerkSlot&>(GetSlot(PerkDataList::kActiveSlotPositions[index]));
        if (active_slot.GetLinkSlot() != NULL)
        {
            return true;
        }
    }
    return false;
}

void uiPerkDialog::RefreshOptionText()
{
    RefreshTopMessage();
    RefreshBottomMessage();
    RefreshOptionList();
}

void uiPerkDialog::RefreshTopMessage()
{
    WzID rect_control_id = GetControlID_byPosition(kControl_Rect_R000);
    CCtrlRectangleWZ* rect_control = static_cast<CCtrlRectangleWZ*>(GetControlWZ(rect_control_id));
    if (rect_control == NULL)
    {
        return;
    }

    TCHAR message[INTERFACE_STRING_LENGTH] = {0, };
    FTextRender& text_render = text_render_[kFTextRenderTop];
    FTextParser parser;
    FTextSentence sentence;
    
    RECT rect = rect_control->GetSizeRect();
    text_render.Init(GetDrawBase(), rect);
    text_render.SetVCenterAlign(true);
    text_render.SetHCenterAlign(true);
    text_render.ClearSentence();

    if (IsEnableSetting() == true)
    {
        // 5535 자신의 성향과 상황에 맞는 버프를 직접 조합하여 사용합니다.
        g_InterfaceManager.GetInterfaceString(5535, message);
    }
    else
    {
        // 5536 퍽은 안전지대와 마을에서만 변경 가능합니다.
        g_InterfaceManager.GetInterfaceString(5536, message);
    }

    parser.SetDefaultFont(message_font_id, WzColor_RGBA(247, 251, 190, 255));
    BOOL result = parser.Parser(message, sentence);
    if (result)
    {
        text_render.Append(sentence);
        text_render.convertLineInfo();
    }
}

void uiPerkDialog::RefreshBottomMessage()
{
    WzID rect_control_id = GetControlID_byPosition(kControl_Rect_R002);
    CCtrlRectangleWZ* rect_control = static_cast<CCtrlRectangleWZ*>(GetControlWZ(rect_control_id));
    if (rect_control == NULL)
    {
        return;
    }

    TCHAR message[INTERFACE_STRING_LENGTH] = {0, };
    FTextRender& text_render = text_render_[kFTextRenderBottom];
    FTextParser parser;
    FTextSentence sentence;

    RECT rect = rect_control->GetSizeRect();
    text_render.Init(GetDrawBase(), rect);
    text_render.SetVCenterAlign(true);
    text_render.SetHCenterAlign(true);
    text_render.ClearSentence();

    // 액티브등록했는지 체크
    bool is_register_active_perk = CheckRegisterActivePerk();

    // 액티브슬롯에 하나도 등록되어 있지 않다면 안내메시지 출력
    if (is_register_active_perk == false)
    {
        // 5537 출력
        g_InterfaceManager.GetInterfaceString(5537, message);
        parser.SetDefaultFont(message_font_id, WzColor_RGBA(188, 240, 120, 255));
        BOOL result = parser.Parser(message, sentence);
        if (result)
        {
            text_render.Append(sentence);
            text_render.convertLineInfo();
        }
    }
}

void uiPerkDialog::RefreshOptionList()
{
    // 정규화 텍스트 세팅
    const int kOptionTabCount = kFTextRenderMax - kFTextRenderOption1;
    const int kOptionTab1 = 0;
    const int kOptionTab2 = 1;

    WzID rect_control_id[kOptionTabCount] = {GetControlID_byPosition(kControl_Rect_R004),
                                             GetControlID_byPosition(kControl_Rect_R005)};
    CCtrlRectangleWZ* rect_control[kOptionTabCount];

    for (int index = 0; index < kOptionTabCount; ++index)
    {
        rect_control[index] = 
            static_cast<CCtrlRectangleWZ*>(GetControlWZ(rect_control_id[index]));
        
        if (rect_control[index] == NULL)
        {
            return;
        }
    }

    TCHAR message[INTERFACE_STRING_LENGTH] = {0, };
    FTextRender* text_render[kOptionTabCount] = {&text_render_[kFTextRenderOption1],
                                                 &text_render_[kFTextRenderOption2]};
    TCHAR* empty_token = " ";
    FTextParser parser;
    FTextSentence sentence;
    FTextSentence empty_sentence;
    parser.Parser(empty_token, empty_sentence);

    RECT rect;
    for (int index = 0; index < kOptionTabCount; ++index)
    {
        rect = rect_control[index]->GetSizeRect();
        text_render[index]->Init(GetDrawBase(), rect);
        text_render[index]->SetVCenterAlign(false);
        text_render[index]->SetHCenterAlign(false);
        text_render[index]->ClearSentence();
    }
    
#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Scroll_V000)));
    CControlWZ* scroll_picture_control = 
        GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    if (scroll_control != NULL)
    {
        scroll_control->ShowWindowWZ(WZ_HIDE);
    }
    if (scroll_picture_control != NULL)
    {
        scroll_picture_control->ShowWindowWZ(WZ_HIDE);
    }
#endif

    // 액티브등록했는지 체크
    bool is_register_active_perk = CheckRegisterActivePerk();
    // 액티브슬롯이 하나라도 등록되어 있으면 옵션 출력
    if (is_register_active_perk == false)
    {
        return;
    }

    // 퍽 세팅에 대한 옵션 계산
    CalculateOptionList();

    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR* option_value_string = {"%s : %+d"};
    TCHAR* option_ratio_string = {"%s : %+d%%"};
    BASE_OptionList* option_list = NULL;
    DWORD option_type = eATTR_TYPE_INVALID;
    DWORD option_value_type = 0;
    int option_value = 0;
    int option_count = 0;

    const int kMaxOptionMap = 2;
    CALCULATED_OPTION_VALUE_MAP* option_map_ptr[kMaxOptionMap] = {
        &benefit_option_map_, 
            &penalty_option_map_};
        CALCULATED_OPTION_VALUE_MAP::iterator option_list_itr;
        CALCULATED_OPTION_VALUE_MAP::iterator end_option_list_itr;

        for (int map_index = 0; map_index < kMaxOptionMap; ++map_index)
        {
            DWORD text_color;
            if (map_index < 1)
            {
                text_color = WzColor_RGBA(188, 240, 120, 255);
            }
            else
            {
                text_color = WzColor_RGBA(244, 120, 82, 255);
            }



			option_list_itr = option_map_ptr[map_index]->begin();
			end_option_list_itr = option_map_ptr[map_index]->end();
			for ( ; option_list_itr != end_option_list_itr; ++option_list_itr)
			{
				option_type = option_list_itr->first;
				option_list = OptionListParser::Instance()->GetOptionList(option_type);
				if (option_list == NULL)
				{
					continue;
				}
	            
				option_value = option_list_itr->second.option_value;

				g_InterfaceManager.GetInterfaceString(option_list->mdwNameCode, 
					temp_message, 
					INTERFACE_STRING_LENGTH);
				Snprintf(message, INTERFACE_STRING_LENGTH, _T("%s : %+d"), 
					temp_message, option_value);
				GlobalFunc::AddOptionSuffix( option_list_itr->second.option_type, option_list, message);

				parser.SetDefaultFont(option_font_id, text_color);
				BOOL result = parser.Parser(message, sentence);
				if (result)
				{
					text_render[(option_count % kOptionTabCount)]->Append(sentence);
					++option_count;
				}
			}
		}

		for (int rect_index = 0; rect_index < kOptionTabCount; ++rect_index)
		{
			text_render[rect_index]->convertLineInfo();
			text_render[rect_index]->SetBeginLineIndex(0);
		}

#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    if ((scroll_control != NULL) &&
        (scroll_picture_control != NULL))
    {
        int max_scroll = max(text_render_[kFTextRenderOption1].GetMaxLine(),
                             text_render_[kFTextRenderOption2].GetMaxLine());
        if (max_scroll > kMaxOptionLine)
        {
            scroll_control->ShowWindowWZ(WZ_SHOW);
            scroll_picture_control->ShowWindowWZ(WZ_SHOW);

            max_scroll -= kMaxOptionLine;
            scroll_control->SetScrollRangeWZ(0, max_scroll);
            scroll_control->SetScrollPosWZ(0);
            scroll_control->SetChangeSizeThumb(30);
        }            
        else
        {
            max_scroll = 0;
        }            
    }
#endif
}

void uiPerkDialog::CalculateOptionList()
{
    // 컨테이너 생성
    // 필요컨테이너 :PerkInfo 및 중복개수 :Option+Type 별 Value(Type별로 2개필요)

    // 2011. 05. 30 기획팀 김준호씨와 협의한 내용으로 증가치 먼저 보여주고 감소치를 보여주되, 
    //증가치 끼리의 순서는 중요치 않아 value, ratio t구분은 하지 않음
    CALCULATED_OPTION_VALUE_MAP option_map;
    benefit_option_map_.clear();
    penalty_option_map_.clear();

    PERK_INFO_DUPICATE_MAP register_active_perk_info;
    const PerkInfo* perk_info = NULL;
    int duplicate_value = 0;
    PERK_INFO_DUPICATE_MAP::iterator found_perk_info_itr;
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    for (int index = 0; index < kNumberOfActiveSlots; ++index)
    {
        bool is_default_slot = false;
        if ( index < kNumberOfActiveSlots - 1 && 
            my_perk_data_[current_page_].from_pos[index] == INVALID_POSTYPE_VALUE )
        {
            continue;
        }
        
        SCPerkSlot& active_slot = index < kNumberOfActiveSlots - 1 ? 
            static_cast<SCPerkSlot&>(GetSlot(my_perk_data_[current_page_].from_pos[index])) : 
            static_cast<SCPerkSlot&>(GetSlot(PerkDataList::kActiveSlotPositions[index]));

        if (index >= kNumberOfActiveSlots - 1 && active_slot.GetLinkSlot() != NULL)
        {
            perk_info = active_slot.GetLinkSlot()->perk_info();
            if (perk_info == NULL)
            {
                continue;
            }
            found_perk_info_itr = register_active_perk_info.find(perk_info);
            if (found_perk_info_itr != register_active_perk_info.end())
            {
                duplicate_value = found_perk_info_itr->second;
                ++duplicate_value;
                found_perk_info_itr->second = duplicate_value;
            }
            else
            {
                register_active_perk_info.insert(std::make_pair<const PerkInfo*, int>(perk_info, 1));
            }
        }
        else
        {
            perk_info = active_slot.perk_info();
            if (perk_info == NULL)
            {
                continue;
            }
            found_perk_info_itr = register_active_perk_info.find(perk_info);
            if (found_perk_info_itr != register_active_perk_info.end())
            {
                duplicate_value = found_perk_info_itr->second;
                ++duplicate_value;
                found_perk_info_itr->second = duplicate_value;
            }
            else
            {
                register_active_perk_info.insert(std::make_pair<const PerkInfo*, int>(perk_info, 1));
            }
        }
    }
#else
    for (int index = 0; index < kNumberOfActiveSlots; ++index)
    {
        SCPerkSlot& active_slot = 
            static_cast<SCPerkSlot&>(GetSlot(PerkDataList::kActiveSlotPositions[index]));
        if (active_slot.GetLinkSlot() != NULL)
        {
            perk_info = active_slot.GetLinkSlot()->perk_info();
            if (perk_info == NULL)
            {
                continue;
            }
            found_perk_info_itr = register_active_perk_info.find(perk_info);
            if (found_perk_info_itr != register_active_perk_info.end())
            {
                duplicate_value = found_perk_info_itr->second;
                ++duplicate_value;
                found_perk_info_itr->second = duplicate_value;
            }
            else
            {
                register_active_perk_info.insert(std::make_pair<const PerkInfo*, int>(perk_info, 1));
            }
        }
    }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

    BASE_OptionList* option_list = NULL;
    BYTE ontion_value_type = 0;
    DWORD ontion_value = 0;
    DWORD ontion_type = eATTR_TYPE_INVALID;
    PERK_INFO_DUPICATE_MAP::iterator end_perk_info_itr = register_active_perk_info.end();
    found_perk_info_itr = register_active_perk_info.begin();
    for ( ; found_perk_info_itr != end_perk_info_itr; ++found_perk_info_itr)
    {
        // 옵션 정보 취합
        perk_info = found_perk_info_itr->first;
        if (perk_info == NULL)
        {
            continue;
        }
        
        const PerkInfo::OptionNode* perk_optin_node = &(perk_info->benefit_option_[0]);
        for (int index = 0; index < PerkInfo::kNumberOfBenefitOptions; ++index)
        {
            ontion_type = perk_optin_node[index].item_option_;
            option_list = OptionListParser::Instance()->GetOptionList(ontion_type);
            if (option_list == NULL)
            {
                continue;
            }

            ontion_value_type = perk_optin_node[index].value_type_;//수치타입
            ontion_value = perk_optin_node[index].value_;//값

            ontion_value *= found_perk_info_itr->second;

            CALCULATED_OPTION_VALUE_MAP* temp_option_container = NULL;
            temp_option_container = &option_map;
            CALCULATED_OPTION_VALUE_MAP::iterator found_option = temp_option_container->find(ontion_type);
            if (found_option != temp_option_container->end())
            {
                ontion_value += found_option->second.option_value;
                found_option->second.option_value = ontion_value;
            }
            else
            {
                OPTION_VALUE_AND_TYPE value_and_option;
                value_and_option.option_value = ontion_value;
                value_and_option.option_type = ontion_value_type;
                temp_option_container->insert(std::make_pair<DWORD, OPTION_VALUE_AND_TYPE>(ontion_type, value_and_option));
            }

        }

        switch (found_perk_info_itr->second)
        {
        case 2:
            perk_optin_node = &(perk_info->penalty_2x_option_[0]);
            break;
        case 3:
            perk_optin_node = &(perk_info->penalty_3x_option_[0]);
            break;
        case 4:
            perk_optin_node = &(perk_info->penalty_4x_option_[0]);
            break;
        default:
            continue;
        }
		int index;
        for (index = 0; index < PerkInfo::kNumberOfPenaltyOptions; ++index)
        {
            ontion_type = perk_optin_node[index].item_option_;
            option_list = OptionListParser::Instance()->GetOptionList(ontion_type);
            if (option_list == NULL)
            {
                continue;
            }

            ontion_value_type = perk_optin_node[index].value_type_;//수치타입
            ontion_value = perk_optin_node[index].value_;//값

            CALCULATED_OPTION_VALUE_MAP* temp_option_container = NULL;
            temp_option_container = &option_map;

            CALCULATED_OPTION_VALUE_MAP::iterator found_option = temp_option_container->find(ontion_type);
            if (found_option != temp_option_container->end())
            {
                ontion_value += found_option->second.option_value;
                found_option->second.option_value = ontion_value;
            }
            else
            {
                OPTION_VALUE_AND_TYPE value_and_option;
                value_and_option.option_value = ontion_value;
                value_and_option.option_type = ontion_value_type;
                temp_option_container->insert(std::make_pair<DWORD, OPTION_VALUE_AND_TYPE>(ontion_type, value_and_option));
            }
        }
    }

    // ( + / - ) 옵션 분리
    BASE_OptionList* optin_list_info;
    std::pair<DWORD, OPTION_VALUE_AND_TYPE> option_pair;
    CALCULATED_OPTION_VALUE_MAP::iterator end_option_map_itr = option_map.end();
    CALCULATED_OPTION_VALUE_MAP::iterator option_map_itr = option_map.begin();
    for ( ; option_map_itr != end_option_map_itr; ++option_map_itr)
    {
        if (option_map_itr->second.option_value == 0)
        {
            continue;
        }

        option_pair = std::make_pair<DWORD, OPTION_VALUE_AND_TYPE>(option_map_itr->first, option_map_itr->second);
        optin_list_info = OptionListParser::Instance()->GetOptionList(option_map_itr->first);
        if ((optin_list_info != NULL) &&
            (optin_list_info->valuetype2 == OptionListParser::eOPTION_VALUE_TYPE_TWO_REVERSE_BENEFIT)) //음수가 이득인 옵션
        {
            if (option_map_itr->second.option_value < 0)
            {
                benefit_option_map_.insert(option_pair);
            }
            else
            {
                penalty_option_map_.insert(option_pair);
            }
        }
        else
        {
            if (option_map_itr->second.option_value > 0)
            {
                benefit_option_map_.insert(option_pair);
            }
            else
            {
                penalty_option_map_.insert(option_pair);
            }
        }
    }
}

void uiPerkDialog::OnChangeAttributes()
{
    if (g_pHero != NULL)
    {
        PlayerAttributes* attributes = g_pHero->GetPlayerAttribute();
        if (attributes != NULL)
        {
            attributes->Update();
        }
    }
}
void uiPerkDialog::OnChangeSlotContainer()
{

}


void uiPerkDialog::OnLevelup()
{
    if (g_pHero == NULL)
    {
        return;
    }

    WzVector effect_pos = g_pHero->GetSkillEffectPos(eSKILLEFFECT_POS_FOOT);
    GlobalFunc::CreateEffectBeUpToOption(StrToWzID("I535"), effect_pos, g_pHero->GetObjectKey());
}

Attributes* uiPerkDialog::GetOwnerAttributes()
{
    if (g_pHero != NULL)
    {
        return (Attributes*)(g_pHero->GetPlayerAttribute());
    }
    return NULL;
}

bool uiPerkDialog::IsEnableSetting()
{
    // program.ini - ENABLE_PERK_SETTING_FREE 을 통하여 마을외에서도 퍽교체가 가능하도록 세팅
    if (PROGRAMPARAM->IsEnablePerkSetting() == true)
    {
        return true;
    }
    
    if (g_pMap == NULL)
    {
        return false;
    }

    if ((GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE) || 
        (g_pMap->IsTutorialField() == TRUE) 
        || (IsSwitchablePerkState() == true)
        )
    {
        return true;
    }

    if (g_pHero != NULL)
    {
        DWORD tile_attribute = g_pHero->GetCurrentTileAtt(FALSE);
        if (BIT_CHECK(tile_attribute, PTA_SAFETY_ZONE) == true)
        {
            return true;
        }
    }
    return false;
}
bool uiPerkDialog::IsSwitchablePerkState()
{
    return g_pHero->FindSkillEffect( eCHAR_STATE_PERK_SWITCHABLE_STATE ) != NULL;
}
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
void uiPerkDialog::Refresh_PerkSlock_My_Data()
{
    current_active_perk_my_data_.Clear();

    bool is_differ = false;

    // Active Slot을 찾아 Link되어 있는 Perk슬롯을 my_perk_data에 저장 한다.
    const bool kFindOption = true; // active slots
    for (int i = kActiveSlotPos0; i < kActiveSlotPosMax; ++i)
    {
        POSTYPE slot_pos = i;
        RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;
        const SCPerkSlot* active_perk_slot = \
            this->GetActivePartSlot(&result, slot_pos, kFindOption);

        // 현재 액티브 슬롯에 존재하지 않는다.
        if (!active_perk_slot)
        {
            my_perk_data_[active_group_number_].from_pos[i - kActiveSlotPos0] = INVALID_POSTYPE_VALUE;
            continue;
        }

        const SCPerkSlot* link_slot = active_perk_slot->GetLinkSlot();
        if ( link_slot )
        {
            if (my_perk_data_[active_group_number_].from_pos[i - kActiveSlotPos0] != link_slot->GetPos())
            {
                is_differ = true;
                my_perk_data_[active_group_number_].from_pos[i - kActiveSlotPos0] = link_slot->GetPos();
            }
        }
    }

    if ( is_differ )
    {
        ChangeGroupPerkLogic();
    }
}

void uiPerkDialog::SEND_CG_SKILL_PERK_GROUP_CONTROL_SYN( MSG_CG_SKILL_PERK_GROUP_CONTROL_SYN* send_packet )
{    
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, send_packet, sizeof(*send_packet));

    is_wait_packet_ = true;
}

void uiPerkDialog::RECV_CG_SKILL_PERK_GROUP_CONTROL_CMD( MSG_CG_SKILL_PERK_GROUP_CONTROL_CMD* recv_packet )
{
	if (recv_packet->isCanProcessState == false)
	{
		ProcessResultCode(PerkControl::eRet_ItemFailed, RC_ITEM_INVALIDSTATE);
		return;
	}

    switch (active_group_number_)
    {
    case eStylePageA:
        {
            active_group_number_ = eStylePageB;
        }
        break;
    case eStylePageB:
        {
            active_group_number_ = eStylePageA;
        }
        break;
    default:
        {

        }
    }

    for (int i = 0; i < 4; ++i)
    {
        bool result = ProcessReplyMessage(recv_packet->header[i], recv_packet->data[i]);

        if (result == true)
        {
            PerkControl* control_header = &(recv_packet->header[i]);
            POSTYPE item_pos = INVALID_POSTYPE_VALUE;
            bool is_play_sound = false;
            switch (recv_packet->header[i].request)
            {
            case PerkControl::eReq_Unregister:
                {
                    is_play_sound = true;
                }
                break;
            case PerkControl::eReq_Plants:
                {
                    item_pos = control_header->args.plants.from_item_pos;
                    ItemManager::Instance()->UseInventoryItem(SI_INVENTORY, item_pos);
                    is_play_sound = true;
                }
                break;
            case PerkControl::eReq_Uproot:
                {                
                    InventoryDialog* inventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
                    if (inventory)
                    {
                        item_pos = control_header->args.uproot.to_item_pos;
                        recv_packet->data[i].uproot.maked_item;
                        SCItemSlot item_slot(recv_packet->data[i].uproot.maked_item);
                        inventory->InsertSlot(item_pos, item_slot);
                    }
                }
                break;
            }

            RefreshBottomMessage();
            RefreshOptionList();
        }

        uiCharacterStatusMan::RefreshSimulationPlayerAttributes();
    }

    SCPerkSlot temp_perk_slot;
    ItemManager::Instance()->PlaySlotSound(eSLOT_SOUND_TYPE_UNEQUIP, 
        &temp_perk_slot);

    int current_active_group_number = get_active_group_number_();
    hero_gauge_style_setting(current_active_group_number);
    ChangeGroupPageLogic();
}

void uiPerkDialog::SEND_CG_ETC_GAMEOPTION_LOAD_SYN_PERK_LIST( MSG_CG_ETC_GAMEOPTION_LOAD_SYN* send_packet )
{
    send_packet->option_type_ = GameOption::kPerkListChar;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, send_packet, sizeof(*send_packet));
    is_wait_packet_ = false;
}

void uiPerkDialog::SavePerkGroupListData()
{
    bool is_differ = false;
    // 처음으로 서버에서 받은 정보와 지금 정보와 다른지 비교
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (my_prototype_perk_data_[i].from_pos[j] != my_perk_data_[i].from_pos[j])
            {
                is_differ = true;
                my_prototype_perk_data_[i].from_pos[j] = my_perk_data_[i].from_pos[j];
            }
        }
    }

    if ( active_group_number_prototype_ != active_group_number_ )
    {
        is_differ = true;
    }

    if ( false == is_differ )
    {
        return;
    }

    // 스트림으로 변경
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN send_packet;
    send_packet.game_option_.option_type_ = GameOption::kPerkListChar;
    if (MakeToStream(send_packet.game_option_) == true)
    {
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
}

bool uiPerkDialog::MakeToStream( GameOption& game_option )
{
    WORD* stream = reinterpret_cast<WORD*>(&game_option.binary_data_[0]);
    uint offset = 0;
    bool is_success = false;

    // 현재 active_group_number 저장
    memcpy(&game_option.binary_data_[offset], &active_group_number_, sizeof(active_group_number_));
    offset += sizeof(active_group_number_);
    active_group_number_prototype_ = active_group_number_;

    vector<PerkGroupStruct>::iterator found_itr = my_perk_data_.begin();
    vector<PerkGroupStruct>::iterator end_itr = my_perk_data_.end();
    
    for (int i = 0; i < kPerkGroupMax; ++i)
    {
        memcpy(&game_option.binary_data_[offset], &(my_perk_data_[i]), sizeof(PerkGroupStruct));
        offset += sizeof(PerkGroupStruct);
        my_prototype_perk_data_[i] = my_perk_data_[i];        
    }
    is_success = true;

    game_option.binary_data_size_ = offset;
    return is_success;
}

void uiPerkDialog::RecvPerkGroupList( GameOption& game_option )
{
    if (game_option.option_type_ == GameOption::kPerkListChar)
    {
        if (MakeToPerkGroupList(game_option))
        {
            // 성공
            is_prototype_receive = TRUE;
            ChangeGroupPageLogic();
        }
    }
}

bool uiPerkDialog::MakeToPerkGroupList( GameOption& game_option )
{
    bool is_success = false;
    if (game_option.binary_data_size_ < sizeof(WORD))
    {
        // 업데이트 이후 처음으로 받았다.
        // 모두 INVALID_POTYPE_VALUE 넣어 준다.
        PerkGroupStruct temp;
        current_page_ = active_group_number_prototype_ = active_group_number_ = eStylePageA;
        for ( int i = 0; i < 4; ++i)
        {
            if (my_perk_data_[active_group_number_].from_pos[i] >= 0 && my_perk_data_[active_group_number_].from_pos[i] <= 14)
            {
                my_prototype_perk_data_[eStylePageA].from_pos[i] = 
                          my_perk_data_[eStylePageA].from_pos[i];
            }
            else
            {
                my_prototype_perk_data_[eStylePageA].from_pos[i] = 
                          my_perk_data_[eStylePageA].from_pos[i] = INVALID_POSTYPE_VALUE;                    
            }
        }        
        my_prototype_perk_data_[eStylePageB] = my_perk_data_[eStylePageB] = temp;

        // 처음이기 때문에 여기서 AStyle로 일단 해준다.
        hero_gauge_style_setting(active_group_number_);

        is_success = true;
        
        return is_success;
    }

    WORD* stream = reinterpret_cast<WORD*>(&game_option.binary_data_[0]);
    uint offset = 0;

    memcpy(&active_group_number_prototype_, (game_option.binary_data_ + offset), sizeof(active_group_number_prototype_));
    offset += sizeof(active_group_number_prototype_);

    if (active_group_number_prototype_ > eStylePageB)
        active_group_number_prototype_ = eStylePageA;    

    current_page_ = active_group_number_;

    PerkGroupStruct temp;
    for (int i = 0; i < kPerkGroupMax; ++i)
    {
        memcpy(&temp, (game_option.binary_data_ + offset), sizeof(PerkGroupStruct));
        offset += sizeof(PerkGroupStruct);

        for (int j = 0; j < 4; ++j)
        {
            if (!(temp.from_pos[j] >= 0 && temp.from_pos[j] <= 14))
            {
                // 데이터가 잘 못 되었을 때
                temp.from_pos[j] = INVALID_POSTYPE_VALUE;
            }
        }

        // 서버에서 처음받은 데이터 셋팅
        my_prototype_perk_data_[i] = temp;
    }

    // A스타일인지 B스타일인지 체크
    // MakeToPerkGroupList함수 이전에 UpdateSlot에서 my_perk_data_가 먼저 받아져 있다.
    if (my_prototype_perk_data_[eStylePageA] == my_perk_data_[active_group_number_])
    {
        active_group_number_ = active_group_number_prototype_ = eStylePageA;        
    }
    else if (my_prototype_perk_data_[eStylePageB] == my_perk_data_[active_group_number_])
    {
        active_group_number_ = active_group_number_prototype_ = eStylePageB;
    }
    active_group_number_prototype_ = active_group_number_;
    my_perk_data_[eStylePageA] = my_prototype_perk_data_[eStylePageA];
    my_perk_data_[eStylePageB] = my_prototype_perk_data_[eStylePageB];
    hero_gauge_style_setting(active_group_number_);
    is_success = true;

    return is_success;
}

void uiPerkDialog::ChangeGroupPerkLogic()
{
    // 미리 저장해 두었다가 자리를 바꿔준다.
    current_active_perk_my_data_.Clear();
    int reserve_active_group_number = 0;
    switch (active_group_number_)
    {
    case eStylePageA:
        {
            reserve_active_group_number = eStylePageB;
        }
        break;
    case eStylePageB:
        {
            reserve_active_group_number = eStylePageA;
        }
        break;
    default:
        {
            // 여기에 들어오면 안된다.
        }
    }
    
    current_active_perk_my_data_ = my_perk_data_[reserve_active_group_number];
    // 나중에 OnClickLeftButton()과 OnClickRightButton()함수 안에 있는 로직은 하나로 묶어서 함수를 만들 것.
    int current_pos = 0;
    MSG_CG_SKILL_PERK_GROUP_CONTROL_SYN request_packet;
    bool result_value = false;

    for (int i = kActiveSlotPos0; i < kActiveSlotPosMax; ++i)
    {
        POSTYPE slot_pos = i;

        if ( current_active_perk_my_data_.from_pos[current_pos] != INVALID_POSTYPE_VALUE )
        {
            result_value = RequestRegister(&request_packet.header[current_pos], 
                current_active_perk_my_data_.from_pos[current_pos], slot_pos);
        }
        else
        {
            result_value = RequestUnregister(&request_packet.header[current_pos], slot_pos);
        }
        ++current_pos;
    }
    SEND_CG_SKILL_PERK_GROUP_CONTROL_SYN(&request_packet);
}

void uiPerkDialog::set_active_group_number( const int& active_group_number )
{
    active_group_number_ = active_group_number;
}

int uiPerkDialog::get_active_group_number_()
{
    return active_group_number_;
}

void uiPerkDialog::ChangeGroupPageLogic()
{
    // 여기에 들어왔다는 얘기는 무조건 페이지가 바뀌었다는 얘기다.
    if (unit_renderer_ == NULL)
    {
        return;
    }
    
    // active_slot은 65001 ~ 65004까지의 perk_group를 가진다.
    for (int i = 0; i < 4; ++i)
    {
        if (my_perk_data_[current_page_].from_pos[i] != INVALID_POSTYPE_VALUE
            && (my_perk_data_[current_page_].from_pos[i] >= 0 && my_perk_data_[current_page_].from_pos[i] <= 14))
        {
             SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(my_perk_data_[current_page_].from_pos[i]));
             DWORD perk_group = perk_slot.GetPerkID().group_;             
             // 컨트롤을 얻어와야 한다.
             CControlWZ* button_control = GetButtonControlBySlotPos(i + 30);             
             if (button_control != NULL)
             {
                 RECT control_rect = button_control->GetSizeRect();
                 unit_renderer_->RemoveItemUnit(PerkDataList::eActiveGroup01 + i); 

                 if (perk_slot.GetPos() == INVALID_POSTYPE_VALUE)
                     continue;
                     
                 unit_renderer_->AddItemUnit(PerkDataList::eActiveGroup01 + i,
                     0,
                     &control_rect,
                     static_cast<const SCSlot*>(&perk_slot),
                     button_control);
             }
        }
        else
        {
            unit_renderer_->RemoveItemUnit(PerkDataList::eActiveGroup01 + i);
        }
    }
    RefreshOptionText();
}

void uiPerkDialog::hero_gauge_style_setting( const int& style )
{
    // 이건 바뀌는 로직
    HeroGaugeDialog2* hero_gauge = static_cast<HeroGaugeDialog2*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE));
    if (hero_gauge != NULL)
    {
        hero_gauge->set_perk_style_change(style);
    }
}

const PerkGroupStruct& uiPerkDialog::get_active_group_struct()
{
    return my_perk_data_[active_group_number_];
}

void uiPerkDialog::PerkGroupChange_On()
{
    if (is_wait_packet_ == true)
    {
        return;
    }
    ChangeGroupPerkLogic();
}

bool uiPerkDialog::get_is_wait_packet_()
{
    return is_wait_packet_;
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM