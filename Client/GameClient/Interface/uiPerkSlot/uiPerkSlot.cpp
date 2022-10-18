#include "SunClientPrecompiledHeader.h"

#include "uiPerkSlot.h"
#include "uiPerkDialog/uiPerkDialog.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "SCPerkSlot.h"
#include "ItemUnitRenderer.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GameFramework.h"

uiPerkSlot::uiPerkSlot(InterfaceManager* ui_manager):
    uiBase(ui_manager),
    ui_perk_dialog_(NULL),
    is_mouse_over_in_perk_slot_(false),
    mouse_over_perk_slot_(INVALID_POSTYPE_VALUE)
{   
}

uiPerkSlot::~uiPerkSlot()
{
}

void uiPerkSlot::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
}

void uiPerkSlot::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
    RegisterControl();

    ui_perk_dialog_ = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);
}

void uiPerkSlot::Release()
{
}

void uiPerkSlot::OnShowWindow(BOOL val)
{
    if (val)
    {
    }
    else
    {
        is_mouse_over_in_perk_slot_ = false;
        mouse_over_perk_slot_ = INVALID_POSTYPE_VALUE;
    }
}

void uiPerkSlot::OnRenderSolarDialog()
{
    if (ui_perk_dialog_ == NULL)
    {
        return;
    }

    // PerkDialog 의 ActiveSlot 을 가져와서 렌더
    ItemUnitRenderer* item_unit_renderer = ui_perk_dialog_->GetItemUnitRender();
    if (item_unit_renderer != NULL)
    {
        ItemUnitRenderer::RenderUnit* render_unit = NULL;
        CControlWZ* slot_control = NULL;
        RECT slot_control_rect;
        for (int slot_pos = 0; slot_pos < PerkDataList::kNumberOfActiveSlots; ++slot_pos)
        {
            slot_control = GetControlWZ(GetControlID_byPosition(slot_pos));
            if (slot_control == NULL)
            {
                continue;
            }

            POSTYPE active_slot_pos = PerkDataList::kActiveSlotPositions[slot_pos];
            if (ui_perk_dialog_->ValidPos(active_slot_pos) == false)
            {
                continue;
            }
     
            SCPerkSlot& active_slot = 
                static_cast<SCPerkSlot&>(ui_perk_dialog_->GetSlot(active_slot_pos));
            if (active_slot.GetLinkSlot() == NULL)
            {
                continue;
            }

            render_unit = item_unit_renderer->GetItemUnit(active_slot.GetPerkID().group_);
            if (render_unit == NULL)
            {
                continue;
            }

            slot_control_rect = slot_control->GetSizeRect();
            item_unit_renderer->RenderPerkSlot(&slot_control_rect, render_unit);
        }
    }    

    // 툴팁 출력
    if (is_mouse_over_in_perk_slot_ && 
        (mouse_over_perk_slot_ != INVALID_POSTYPE_VALUE))
    {
        CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(mouse_over_perk_slot_));
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
                    POSTYPE active_slot_pos = 
                        PerkDataList::kActiveSlotPositions[mouse_over_perk_slot_];
                    SCPerkSlot& perk_slot = 
                        static_cast<SCPerkSlot&>(ui_perk_dialog_->GetSlot(active_slot_pos));
                    tooltip_manager->RegisterTooltipPerkInfo(&perk_slot, active_slot_pos);            
					tooltip_manager->SetTooltipDialogID(GetDialogKey());

                }
            }
        } 
    }
}

void uiPerkSlot::MessageProc(SI_MESSAGE * pMessage)
{
    if (IsLock())
        return;

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_MOUSEOVER:
        OnMouseOver(pMessage);
        break;
    default:
        SolarDialog::MessageProc(pMessage);
        break;
    }
}

bool uiPerkSlot::OnMouseOver(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_postion = GetControlPosition_byID(message->dwCtrlID);
    switch (control_postion)
    {
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B000:
        {
            mouse_over_perk_slot_ = static_cast<POSTYPE>(control_postion);
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

