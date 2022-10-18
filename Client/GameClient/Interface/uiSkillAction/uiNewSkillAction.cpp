
#include "SunClientPrecompiledHeader.h"
//------------------------------------------------------------------------------
//  (C) 2010 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "uiNewSkillAction.h"
#include "ItemUnitRenderer.h"
#include "SkillInventoryStoreParser.h"
#include "ItemManager.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "SlotKeyGenerator.h"
#include "SkillInfoParser.h"
#include "SkillSlot.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------
WzID uiSkillAction::wzId_[kDlgMax] = 
{ 
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    // 액션
    StrToWzID("P056"),StrToWzID("P242"),StrToWzID("P243"), StrToWzID("P077"),StrToWzID("P262"),StrToWzID("P263"),
    StrToWzID("P244"),StrToWzID("P245"),StrToWzID("P246"), StrToWzID("P264"),StrToWzID("P265"),StrToWzID("P266"),
    StrToWzID("P247"),StrToWzID("P248"),StrToWzID("P249"), StrToWzID("P267"),StrToWzID("P268"),StrToWzID("P269"),
    StrToWzID("P250"),StrToWzID("P251"),StrToWzID("P252"), StrToWzID("P270"),StrToWzID("P271"),StrToWzID("P272"),
    StrToWzID("P253"),StrToWzID("P254"),StrToWzID("P255"), StrToWzID("P273"),StrToWzID("P274"),StrToWzID("P275"),
    StrToWzID("P256"),StrToWzID("P257"),StrToWzID("P258"), StrToWzID("P276"),StrToWzID("P277"),StrToWzID("P278"),
    StrToWzID("P259"),StrToWzID("P260"),StrToWzID("P261"), StrToWzID("P279"),StrToWzID("P280"),StrToWzID("P281"),

    // 탭
    StrToWzID("C000"), StrToWzID("C001"), StrToWzID("C002"),

    // 텍스트
    StrToWzID("S001"), StrToWzID("S002"), 

    // ESC
    StrToWzID("B086")
#else
    // 액션
    StrToWzID("B010"), StrToWzID("B000"), StrToWzID("B001"),
    StrToWzID("B004"), StrToWzID("B003"), StrToWzID("B002"),
    StrToWzID("B005"), StrToWzID("B006"), StrToWzID("B007"),
    StrToWzID("B008"), StrToWzID("B009"), StrToWzID("B011"),
    StrToWzID("B018"), StrToWzID("B019"), StrToWzID("B020"),
    StrToWzID("B021"), StrToWzID("B022"), StrToWzID("B023"),
    StrToWzID("B012"), StrToWzID("B013"), StrToWzID("B014"),
    StrToWzID("B015"), StrToWzID("B016"), StrToWzID("B017"),
    StrToWzID("B031"), StrToWzID("B032"), StrToWzID("B033"),
    StrToWzID("B034"), StrToWzID("B035"), StrToWzID("B036"),
    StrToWzID("B024"), StrToWzID("B025"), StrToWzID("B026"),
    StrToWzID("B027"), StrToWzID("B028"), StrToWzID("B030"),
    // 탭
    StrToWzID("C005"), StrToWzID("C006"), StrToWzID("C000"),
    // ESC
    StrToWzID("B999")
#endif
};
//------------------------------------------------------------------------------
/**
*/
uiSkillAction::uiSkillAction(InterfaceManager* ui_man):
uiBase(ui_man),
invalid_control_(false),
item_renderer_(NULL)
{
    SkillSlotContainer::Init(kMaxSlot*2);
    this->InitItemRender();
}
//------------------------------------------------------------------------------
/**
*/
uiSkillAction::~uiSkillAction()
{
    this->Release();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    // 컨트롤러 유효성 체크
    for (int idx = 0; idx < kDlgMax; ++idx)
    {
        if (this->getControl(idx) == NULL)
        {
            invalid_control_ = true;
            break;
        }
    }

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    for (int idx = 0; idx < kMaxSlot; ++idx )
    {
        CCtrlPictureWZ* control = (CCtrlPictureWZ*)getControl(idx);
        control->EnableLButtonEvent(true);
    }

    // 좌우 타이틀
    TCHAR text[INTERFACE_STRING_LENGTH];
    if( CControlWZ* control = getControl(kTextLeft) )
    {
        g_InterfaceManager.GetInterfaceString(eST_BUTTON_TEXT_ACTION, text, INTERFACE_STRING_LENGTH);
        control->SetTextWZ(text);
    }

    if( CControlWZ* control = getControl(kTextRight) )
    {
        g_InterfaceManager.GetInterfaceString(eST_BUTTON_TEXT_EMOTICON, text, INTERFACE_STRING_LENGTH);
        control->SetTextWZ(text);
    }
#endif

}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::Release()
{
    FlushUnitDraw();
    SAFE_RELEASENDELETE(item_renderer_);
}
//------------------------------------------------------------------------------
/**
*/
SCSlotContainer* uiSkillAction::GetSlotContainer()
{
    return static_cast<SCSlotContainer *>(this);
}   
//------------------------------------------------------------------------------
/** 정보로딩
*/
void uiSkillAction::LoadSkillActionSlot(const SkillInven_s* data)
{
    SkillInfoParser* parser = SkillInfoParser::Instance();
   
    for (int idx = 0; idx < kMaxColumn; ++idx)
    {
        SLOTCODE skill_Code = SLOTCODE(data->iSkillID[idx]);

        if (skill_Code == 0)
        {
            continue;
        }
        
        SkillSlot skill_slot;
        skill_slot.SetCode(skill_Code);
        skill_slot.SetSerial(g_SlotKeyGenerator.GetKey());
        int real_pos = (data->bSkillTab2 - 1)*kMaxColumn + idx;
        InsertSlot(real_pos, skill_slot);
        uicSkillInfo info;
        info.m_pCurrentSkillInfo = parser->GetSkillInfo(skill_Code);

        // 스킬 정보 세팅
        info.m_SlotIndex = real_pos;
        info.m_State = SLOT_RENDER_STATE_FORCE_ACTIVATED;
        info.m_bEnableLevelup = false;
    
        // 삽입(순서대로 기입되어있다)
        skill_action_list().push_back(info);
    }
}
//------------------------------------------------------------------------------
/** 슬롯 삽입
*/
RC::eSLOT_INSERT_RESULT uiSkillAction::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    if (ValidPos(at_pos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    // 슬롯에 삽입
    return SCSkillSlotContainer::InsertSlot(at_pos, slot);
}
//------------------------------------------------------------------------------
/** 슬롯 삭제
*/
BOOL uiSkillAction::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    if (IsEmpty(at_pos))
    {
        return FALSE;
    }

    if (GetItemUnitRender())
    {
        GetItemUnitRender()->RemoveItemUnit(GetSlot(at_pos).GetSerial());
    }

    return SCSkillSlotContainer::DeleteSlot(at_pos, slot_clone_before_delete); 
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::OnShowWindow(BOOL val)
{
    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);

    if (invalid_control())
    {
        return;
    }

    ui_skill_man->OpenDialog(uiSkillMan::kSkillAction, val);

    if (val)
    {
        RefreshActionIcon();
        RefreshTab();
    }
}
//------------------------------------------------------------------------------
/** 
*/
void uiSkillAction::InitItemRender()
{
    if (GetItemUnitRender() == NULL)
    {
        item_renderer_ = new ItemUnitRenderer;
        item_renderer_->Init( uiBase::GetDrawBase(), GetMaxSlotNum() );
        item_renderer_->ShowCooldown(TRUE);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::OnUpdateSolarDialog()
{
    if (invalid_control())
    {
        return;
    }

    UpdateUnitRender();
    UpdateMouse();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::OnRenderSolarDialog()
{
    if (invalid_control() ||
        (GetItemUnitRender() == NULL))
    {
        return;
    }

    GetItemUnitRender()->Render();
    RenderToolTip();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::MessageProc(SI_MESSAGE* message)
{
    if (invalid_control())
    {
        return;
    }

    POSTYPE pos = GetControlPos(message->dwCtrlID);

    switch (message->eResultMsg) 
    {
    case RT_MSG_RBUTTONCLICK:
        {
            MouseClickSlot(pos, false);
        }
        break;
    case RT_MSG_LBUTTONCLICK:
        {
            MouseClickSlot(pos, true);
            MouseClickTab(pos);
            if (pos == kEscButton)
            {
                ShowInterface(FALSE);
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}
//------------------------------------------------------------------------------
/** 마우스클릭처리(우클릭:집기, 좌클릭:사용)
*/
void uiSkillAction::MouseClickSlot(POSTYPE pos, bool pickup)
{
    if ((pos >= kMaxSlot) ||
        IsEmpty(pos))
    {
        return;
    }

    if (pickup)
    {
        RECT rect = getControl(pos)->GetSizeRect();
        MouseHandler::Instance()->ItemTransaction(SI_SKILL, pos, &rect);
    }
    else
    {
        MouseHandler::Instance()->ItemUseTransaction(SI_SKILL, pos);
    }

}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::Clear()
{
    FlushUnitDraw();
    skill_action_list().clear();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::FlushUnitDraw()
{
    for (int idx = 0; idx < GetMaxSlotNum(); ++idx)
    {
        if (IsEmpty(idx) == FALSE)
        {
            g_SlotKeyGenerator.Restore(DWORD(GetSlot(idx).GetSerial()));
            this->DeleteSlot(idx, NULL);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::MouseClickTab(POSTYPE pos)
{
    if ((pos < kTabBein) ||
        (pos > kTabEnd))
    {
        return;
    }

    RefreshTab();

    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    if (ui_skill_man)
    {
        ui_skill_man->OpenTab((pos - kTabBein) + 1);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::RenderToolTip()
{
    if (m_MouseOverObject.eMouseOverType != eMOUSE_OVER_SKILL)
    {
        return;
    }

    uicSkillInfo& skill_info = skill_action_list(m_MouseOverObject.iSkillIdx);
    SCSkillSlot& skill_slot = static_cast<SCSkillSlot &>(GetSlot(skill_info.m_SlotIndex));

    uiToolTipMan* tooltip = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip)
    {
        tooltip->RegisterTooltipSkillEtc(skill_info.m_pCurrentSkillInfo);
		tooltip->SetTooltipDialogID(GetDialogKey());
    }
}
//------------------------------------------------------------------------------
/**  
*/
void uiSkillAction::RefreshActionIcon()
{
    int max_slot = GetMaxSlotNum();
    for( int idx = 0; idx < max_slot; ++idx)
    {
        if (IsEmpty(idx) == FALSE)
        {
            SCSlot& slot = SCSlotContainer::GetSlot(idx);
            if (GetItemUnitRender()->GetItemUnit(slot.GetSerial()) == false)
            {
                GetItemUnitRender()->AddItemUnit(slot.GetSerial(), slot.GetCode(),
                                                 &getControl(idx)->GetSizeRect(), &slot, 
                                                 getControl(idx)); 
            }
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::RefreshTab()
{
    // Tab
    CCtrlButtonCheckWZ* button = NULL;
    for (int idx = 0; idx < kMaxTab; ++idx)
    {
        button = static_cast<CCtrlButtonCheckWZ*>(getControl(idx + kTabBein));
        button->SetCheckState((idx == 2) ? (true) : (false));
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillAction::UpdateUnitRender()
{
    if (GetItemUnitRender() == NULL)
    {
        return;
    }

    GetItemUnitRender()->Update();
    int list_size = skill_action_size();
    ItemUnitRenderer::RenderUnit* render_unit = NULL;
    for (int idx = 0; idx < list_size; ++idx)
    {
        uicSkillInfo& info = skill_action_list(idx);
        render_unit = GetItemUnitRender()->GetItemUnit(GetSlot(info.m_SlotIndex).GetSerial());
        if (render_unit)
        {
            render_unit->SetShow(TRUE);
        }
    }
}
//------------------------------------------------------------------------------
/** 스킬정보를 통한 슬롯위치찾기
*/
SkillSlot* uiSkillAction::FindSlotByCode(CODETYPE skill_code)
{
    int max_slot = GetMaxSlotNum();
    for( int idx = 0; idx < max_slot; ++idx)
    {
        if (IsEmpty(idx) == FALSE)
        {
            SCSlot& slot = SCSlotContainer::GetSlot(idx);
            if (slot.GetCode() == skill_code)
            {
                return static_cast<SkillSlot*>(&GetSlot(idx));
            }
        }
    }

    return NULL;
}
//------------------------------------------------------------------------------
/** 슬롯돌며 마우스 오버 체크
*/
void uiSkillAction::UpdateMouse()
{
    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    int list_size = skill_action_size();
    for (int idx = 0; idx < list_size; ++idx)
    {
        POSTYPE slot_pos = skill_action_list(idx).m_SlotIndex;

        RECT rect = getControl(slot_pos)->GetSizeRect();
        if (rect.left <= mouse_x && rect.right >= mouse_x)
        {
            if (rect.top <= mouse_y && rect.bottom >= mouse_y)
            {
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SKILL;
                m_MouseOverObject.iSkillIdx = idx;//리스트 인덱스
                break;
            }
        }
    }
}