#include "SunClientPrecompiledHeader.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
#include "uiSmartNpcDialogMan/uiSmartNpcDialogReply.h"
#include "InterfaceManager.h"
#include "ObjectManager.h"
#include "PacketStruct_CG_SmartNPC.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "ItemUnitRenderer.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"
#include "RewardInfoList.h"
#include "ItemManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "Application.h"
#include "json/json.h"

uiSmartNpcDialogReply::uiSmartNpcDialogReply(InterfaceManager* ui_manager) : uiSmartNpcDialog(ui_manager),
list_control_(NULL),
item_unit_renderer_(NULL)
{
    dialog_type_ = SmartDialogType_Reply;

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);

    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);

    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);

    RegisterControlPosition(StrToWzID("SLT0"), kControl_Button_SLT0);
    RegisterControlPosition(StrToWzID("SLT1"), kControl_Button_SLT1);
    RegisterControlPosition(StrToWzID("SLT2"), kControl_Button_SLT2);
    RegisterControlPosition(StrToWzID("SLT3"), kControl_Button_SLT3);
    RegisterControlPosition(StrToWzID("SLT4"), kControl_Button_SLT4);

    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);

    for (int i = 0; i < uiSmartNpcDialogMan::MaxReplyButton; ++i)
    {
        button_event_id[i].event_id = 0;
        button_event_id[i].step = 0;
    }
}

uiSmartNpcDialogReply::~uiSmartNpcDialogReply()
{
}

VOID uiSmartNpcDialogReply::Init(CDrawBase* pDrawBase)
{
    draw_ = pDrawBase;

    SCItemSlotContainer::Init(kSlotTotalCount, SI_SMARTNPC);

    // 랜더러 초기화
    item_unit_renderer_ = new ItemUnitRenderer;
    item_unit_renderer_->Init(uiBase::GetDrawBase(), kSlotTotalCount);

    uiBase::Init(pDrawBase);
}
void uiSmartNpcDialogReply::Release()
{
    if(item_unit_renderer_)
    {
        item_unit_renderer_->Release();
        delete item_unit_renderer_;
        item_unit_renderer_ = NULL;
    }
}
BOOL uiSmartNpcDialogReply::CreateControls(BYTE sub_type)
{
    CDialogWZ* dialog_ = GetDialogWZ();

    // ui 무늬 - 중앙상단
    CCtrlPictureWZ* picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("P000"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 100, dialog_->GetDlgY(), 100, 20);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 0;
        image_sub_info.m_fTileCornerY = 0;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
        image_sub_info.m_sFRect.fTopX = 430;
        image_sub_info.m_sFRect.fTopY = 940;
        image_sub_info.m_sFRect.fWidth = 100;
        image_sub_info.m_sFRect.fHeight = 20;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

        picture_control->SetDiscardMessage(true);
    }
    // ui 무늬 - 좌측상단
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("P001"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 12, dialog_->GetDlgY() + 11, 100, 20);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 0;
        image_sub_info.m_fTileCornerY = 0;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
        image_sub_info.m_sFRect.fTopX = 430;
        image_sub_info.m_sFRect.fTopY = 960;
        image_sub_info.m_sFRect.fWidth = 100;
        image_sub_info.m_sFRect.fHeight = 20;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

        picture_control->SetDiscardMessage(true);
    }
    // ui 무늬 - 우측상단
    picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("P002"));
    if (picture_control == NULL)
    {
        return FALSE;
    }
    else
    {
        picture_control->MoveWindowWZ(dialog_->GetDlgX() + 168, dialog_->GetDlgY() + 11, 100, 20);
        SI_TSUBINFO image_sub_info;
        image_sub_info.Initialize();
        image_sub_info.m_blIs = true;

        image_sub_info.m_sImageName = "Common_bg01.tga";
        image_sub_info.m_blUseGradation = false;
        image_sub_info.m_fTileCornerX = 0;
        image_sub_info.m_fTileCornerY = 0;

        image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
        image_sub_info.m_sFRect.fTopX = 430;
        image_sub_info.m_sFRect.fTopY = 980;
        image_sub_info.m_sFRect.fWidth = 100;
        image_sub_info.m_sFRect.fHeight = 20;
        image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
        picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

        picture_control->SetDiscardMessage(true);
    }

    // 창 이름
    CCtrlStaticWZ* static_control = CreateContorlStatic(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("T999"));
    if (static_control == NULL)
    {
        return FALSE;
    }
    else
    {
        static_control->MoveWindowWZ(dialog_->GetDlgX() + 24, dialog_->GetDlgY() + 12, 233, 17);
    }

    CCtrlButtonWZ* button_control;
    // x 버튼
    if (sub_type != SmartDialogSubType_ForceOk)
    {
        button_control = CreateContorlCloseButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("B999"));
        if (button_control == NULL)
        {
            return FALSE;
        }
        else
        {
            button_control->MoveWindowWZ(dialog_->GetDlgX() + 248, dialog_->GetDlgY() + 13, 17, 17);
        }
    }

    // 리스트
    if ((sub_type == SmartDialogSubType_Ok) ||
        (sub_type == SmartDialogSubType_OkNo) ||
        (sub_type == SmartDialogSubType_OkNoCancle) ||
        (sub_type == SmartDialogSubType_ForceOk))
    {
        CCtrlListWZ* list_control = CreateContorlList(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("L000"));
        if (list_control == NULL)
        {
            return FALSE;
        }
        else
        {
            list_control->MoveWindowWZ(dialog_->GetDlgX() + 27, dialog_->GetDlgY() + 49, 211, 62);
            list_control_ = list_control;
            RECT list_rect = list_control_->GetSizeRect();
            ftext_render_.Init(draw_, list_rect);

            ftext_render_.SetMargin(0,0,0,-5);
            ftext_render_.SetGap(0,0);

            ftext_render_.convertLineInfo();
        }

        // 리스트 뒤에 이미지1
        picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("P003"));
        if (picture_control == NULL)
        {
            return FALSE;
        }
        else
        {
            picture_control->MoveWindowWZ(dialog_->GetDlgX() + 21, dialog_->GetDlgY() + 41, 238, 77);
            SI_TSUBINFO image_sub_info;
            image_sub_info.Initialize();
            image_sub_info.m_blIs = true;

            image_sub_info.m_sImageName = "Common_bg01.tga";
            image_sub_info.m_blUseGradation = false;
            image_sub_info.m_fTileCornerX = 5;
            image_sub_info.m_fTileCornerY = 5;

            image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
            image_sub_info.m_sFRect.fTopX = 398;
            image_sub_info.m_sFRect.fTopY = 147;
            image_sub_info.m_sFRect.fWidth = 17;
            image_sub_info.m_sFRect.fHeight = 17;
            image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
            picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
        }

        // 리스트 뒤에 이미지2
        picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("P004"));
        if (picture_control == NULL)
        {
            return FALSE;
        }
        else
        {
            picture_control->MoveWindowWZ(dialog_->GetDlgX() + 13, dialog_->GetDlgY() + 33, 254, 93);
            SI_TSUBINFO image_sub_info;
            image_sub_info.Initialize();
            image_sub_info.m_blIs = true;

            image_sub_info.m_sImageName = "Common_bg01.tga";
            image_sub_info.m_blUseGradation = false;
            image_sub_info.m_fTileCornerX = 12;
            image_sub_info.m_fTileCornerY = 12;

            image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
            image_sub_info.m_sFRect.fTopX = 390;
            image_sub_info.m_sFRect.fTopY = 0;
            image_sub_info.m_sFRect.fWidth = 26;
            image_sub_info.m_sFRect.fHeight = 26;
            image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
            picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
        }
    }
    
    // 3종 버튼
    if ((sub_type == SmartDialogSubType_Ok) ||
        (sub_type == SmartDialogSubType_ForceOk))
    {
        button_control = CreateContorlButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Button_B001));
        if (button_control == NULL)
        {
            return FALSE;
        }
        else
        {
            button_control->MoveWindowWZ(dialog_->GetDlgX() + 88, dialog_->GetDlgY() + 125, 104, 29);
        }
    }
    else if ((sub_type == SmartDialogSubType_OkNo) ||
        (sub_type == SmartDialogSubType_Edit) || 
        (sub_type == SmartDialogSubType_Slot) ||
        (sub_type == SmartDialogSubType_SlotInput))
    {
        for (int i = 0; i < 2; ++i)
        {
            button_control = CreateContorlButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Button_B001+i));
            if (button_control == NULL)
            {
                return FALSE;
            }
            else
            {
                button_control->MoveWindowWZ(dialog_->GetDlgX() + 33 + (i*110), dialog_->GetDlgY() + 125, 104, 29);
            }
        }

    }
    else if (sub_type == SmartDialogSubType_OkNoCancle)
    {
        for (int i = 0; i < 3; ++i)
        {
            button_control = CreateContorlButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Button_B001+i));
            if (button_control == NULL)
            {
                return FALSE;
            }
            else
            {
                button_control->MoveWindowWZ(dialog_->GetDlgX() + 19 + (83*i), dialog_->GetDlgY() + 125, 79, 29);
            }
        }
    }
    
    // 에디트
    if (sub_type == SmartDialogSubType_Edit)
    {
        // 에디트 뒤에 이미지
        picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, StrToWzID("P005"));
        if (picture_control == NULL)
        {
            return FALSE;
        }
        else
        {
            picture_control->MoveWindowWZ(dialog_->GetDlgX() + 27, dialog_->GetDlgY() + 49, 221, 69);
            SI_TSUBINFO image_sub_info;
            image_sub_info.Initialize();
            image_sub_info.m_blIs = true;

            image_sub_info.m_sImageName = "";
            image_sub_info.m_blUseGradation = false;
            image_sub_info.m_fTileCornerX = 0;
            image_sub_info.m_fTileCornerY = 0;

            image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
            image_sub_info.m_sFRect.fTopX = 0;
            image_sub_info.m_sFRect.fTopY = 0;
            image_sub_info.m_sFRect.fWidth = 0;
            image_sub_info.m_sFRect.fHeight = 0;
            image_sub_info.m_color = 0xA0000000; //RGBA(255,255,255,255);
            picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
        }

        // 에디트컨트롤
        CCtrlMultilineEditWZ* edit_control = CreateContorlEdit(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Edit_E001));
        if (edit_control == NULL)
        {
            return FALSE;
        }
        else
        {
            edit_control->MoveWindowWZ(dialog_->GetDlgX() + 27, dialog_->GetDlgY() + 49, 231, 69);
            GlobalFunc::SetFocus((SolarDialog*)this, edit_control);
        }
    }

    // 슬롯 이미지
    if ((sub_type == SmartDialogSubType_Slot) ||
        (sub_type == SmartDialogSubType_SlotInput))
    {
        // 슬롯 배경 이미지
        picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Picture_P011));
        if (picture_control == NULL)
        {
            return FALSE;
        }
        else
        {
            picture_control->MoveWindowWZ(dialog_->GetDlgX() + 17, dialog_->GetDlgY() + 38, 246, 87);
            SI_TSUBINFO image_sub_info;
            image_sub_info.Initialize();
            image_sub_info.m_blIs = true;

            image_sub_info.m_sImageName = "Common_bg01.tga";
            image_sub_info.m_blUseGradation = false;
            image_sub_info.m_fTileCornerX = 5;
            image_sub_info.m_fTileCornerY = 5;

            image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
            image_sub_info.m_sFRect.fTopX = 398;
            image_sub_info.m_sFRect.fTopY = 147;
            image_sub_info.m_sFRect.fWidth = 17;
            image_sub_info.m_sFRect.fHeight = 17;
            image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
            picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
        }

        for (int i = 0; i < 5; ++i)
        {
            // 슬롯 뒤에 이미지
            picture_control = CreateContorlPicture(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Picture_P006+i));
            if (picture_control == NULL)
            {
                return FALSE;
            }
            else
            {
                picture_control->MoveWindowWZ(dialog_->GetDlgX() + 32 + (i*(44)), dialog_->GetDlgY() + 65, 36, 36);
                SI_TSUBINFO image_sub_info;
                image_sub_info.Initialize();
                image_sub_info.m_blIs = true;

                image_sub_info.m_sImageName = "Common_bg01.tga";
                image_sub_info.m_blUseGradation = false;
                image_sub_info.m_fTileCornerX = 0;
                image_sub_info.m_fTileCornerY = 0;

                image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
                image_sub_info.m_sFRect.fTopX = 531;
                image_sub_info.m_sFRect.fTopY = 245;
                image_sub_info.m_sFRect.fWidth = 36;
                image_sub_info.m_sFRect.fHeight = 36;
                image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);
                picture_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
            }

            button_control = CreateContorlButton(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY, GetControlID_byPosition(kControl_Button_SLT0+i));
            if (button_control == NULL)
            {
                return FALSE;
            }
            else
            {
                button_control->MoveWindowWZ(dialog_->GetDlgX() + 34 + (i*(44)), dialog_->GetDlgY() +67, 32, 32);
                SI_TSUBINFO image_sub_info;
                image_sub_info.Initialize();
                image_sub_info.m_blIs = true;

                image_sub_info.m_sImageName = "";
                image_sub_info.m_blUseGradation = false;
                image_sub_info.m_fTileCornerX = 0;
                image_sub_info.m_fTileCornerY = 0;

                image_sub_info.m_nType = SI_TSUBINFO::DrawType_Whole;
                image_sub_info.m_sFRect.fTopX = 0;
                image_sub_info.m_sFRect.fTopY = 0;
                image_sub_info.m_sFRect.fWidth = 0;
                image_sub_info.m_sFRect.fHeight = 0;
                image_sub_info.m_color = 0;
                button_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);
                image_sub_info.Initialize();
                button_control->SetCtrlDetailImageFormat(E_ON, &image_sub_info);
                button_control->SetCtrlDetailImageFormat(E_CLICK, &image_sub_info);
                button_control->SetCtrlDetailImageFormat(E_DISABLE, &image_sub_info);
                button_control->SetCtrlDetailImageFormat(E_CHECK, &image_sub_info);
            }
        }
        
    }
    
    return TRUE;
}

void uiSmartNpcDialogReply::SetControlText(BYTE contorl_index, const TCHAR *text_massage)
{
    if (contorl_index >= kControl_Size)
    {
        return;
    }
    switch (contorl_index)
    {
    case kControl_Text_T999:
        {
            CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(contorl_index));
            if (control)
            {
                control->SetTextWZ(text_massage);
            }
        }
        break;
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
        {
            CCtrlButtonWZ* control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(contorl_index));
            if (control)
            {
                control->SetTextWZ(text_massage);
            }
        }
        break;
    case kControl_List_L000:
        {
            SetTextMessage(text_massage);
        }
        break;
    }
}
void uiSmartNpcDialogReply::SetControlEventID(BYTE contorl_index, DWORD event_id, DWORD step)
{
    if (contorl_index >= kControl_Size)
    {
        return;
    }
    switch (contorl_index)
    {
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
        {
            CCtrlButtonWZ* control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(contorl_index));
            if (control)
            {
                int index = contorl_index - kControl_Button_B001;
                button_event_id[index].event_id = event_id;
                button_event_id[index].step = step;
            }
        }
        break;
    }
}
void uiSmartNpcDialogReply::SetSlotItem(BYTE slot_index, DWORD item_code, DWORD item_number)
{
    // 보상 슬롯 갱신
    if (kSlotTotalCount <= slot_index)
    {
        return;
    }

    InsertSlot(kControl_Button_SLT0 + slot_index, item_code, item_number);
}
void uiSmartNpcDialogReply::OnShowWindow(BOOL val)
{
    if (val)
    {

        if (GetSubType() == SmartDialogSubType_ForceOk)
        {
            g_InterfaceManager.LockDlgs(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
        }
        else
        {
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType=eDoType_SmartReply_Dialog;
            msg.wParam=IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY;
            msg.DoSomething=GlobalFunc::DoSmartNpcReplyDialog;
            g_KeyQueueManager.PushBack(msg);
        }
    }
    else
    {
        if (GetSubType() == SmartDialogSubType_ForceOk)
        {
            g_InterfaceManager.UnLockDlgs();
        }
        else
        {
            g_KeyQueueManager.DeleteMsg(eDoType_SmartReply_Dialog);
        }
    }
}
void uiSmartNpcDialogReply::OnRenderSolarDialog()
{
    if (GetSubType() == SmartDialogSubType_ForceOk)
    {
        RECT rc;
        POINT pt = g_pApplication->Get2DModeMapping();
        SetRect(&rc, 0, 0, pt.x, pt.y);
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0, 0, static_cast<float>(rc.right), static_cast<float>(rc.bottom));
        GetDialogWZ()->OnDraw(true);
    }


    ftext_render_.Render();

    if (item_unit_renderer_)
    {
        item_unit_renderer_->Render();
    }

    RenderTooltip();
}
void uiSmartNpcDialogReply::OnUpdateSolarDialog()
{
    if (GetOwnerKey() != 0)
    {
        Object* object = (g_ObjectManager.GetObject(GetOwnerKey()));
        if (object && g_pHero)
        {
            WzVector vDiff = object->GetPosition() - g_pHero->GetVisiblePos();
            float distance = VectorLength2(&vDiff);
            if (distance > (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET))
            {
                //g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU);
                SetDestroyWindow(TRUE);
                return;
            }
        }
    }

    if (list_control_ == NULL)
    {
        return;
    }

    RECT list_rect = list_control_->GetSizeRect();
    ftext_render_.SetRect(list_rect);
}
void uiSmartNpcDialogReply::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_RBUTTONCLICK:
        OnRButtonClick(message);
        break;
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
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
bool uiSmartNpcDialogReply::OnLButtonClick(SI_MESSAGE* message)
{
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B999:
        {
            g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
        }
        break;
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
        {
            DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
            int index = control_position - kControl_Button_B001;
            EVENT_IDSTEP& event_id = button_event_id[index];

            MSG_CG_NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD send_packet(
                g_pHero->GetObjectKey(),
                GetOwnerKey(),
                event_id.event_id,
                event_id.step);

            // 파싱
            Json::Value root;
            root["BUTTON"] = index+1; // 예 : 1, 아니오 : 2, 취소 : 3, x버튼은 그냥 창 닫음
        
            // 에디트
            CCtrlMultilineEditWZ* edit_control = static_cast<CCtrlMultilineEditWZ*>(GetControlWZ_byPosition(kControl_Edit_E001));
            if (edit_control)
            {
                TCHAR edit_message[256] = {0,};
                edit_control->GetTextWZ(edit_message, 256);
                root["EDIT"] = edit_message;
            }
            CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_SLT0));
            if (button_control)
            {
                Json::Value slot_list;
                for (POSTYPE pos = kControl_Button_SLT0; pos <= kControl_Button_SLT4; ++pos)
                {
                    Json::Value slot;    
                    if (IsEmpty(pos) == FALSE)
                    {
                        SCSlot& sc_slot = GetSlot(pos);
                        slot["ITEMCODE"] = sc_slot.GetCode();
                        slot["ITEMNUMBER"] = sc_slot.GetNum();

                        slot_list.append(slot);
                    }
                }
                root["SLOTS"] = slot_list;
            }
            

            Json::FastWriter writer;
            std::string string = writer.write(root);

            strncpy(send_packet.data_, string.c_str(), 1024);
            send_packet.data_length_ = string.length();


            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
            g_InterfaceManager.DeleteInterface(IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY);
        }
        break;
    case kControl_Button_SLT0:
    case kControl_Button_SLT1:
    case kControl_Button_SLT2:
    case kControl_Button_SLT3:
    case kControl_Button_SLT4:
        {
            DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
            CControlWZ* control = GetControlWZ_byPosition(control_position);
            if (control)
            {
                RECT rcSlot = control->GetSizeRect();
                MouseHandler::Instance()->ItemTransaction(SI_SMARTNPC, (POSTYPE)control_position, &rcSlot);
            }
        }
        break;
    default:
        {
        }
        break;
    }

    return true;
}
bool uiSmartNpcDialogReply::OnRButtonClick(SI_MESSAGE* message)
{
    DWORD control_pos = GetControlPosition_byID(message->dwCtrlID);
    switch (control_pos)
    {
    case kControl_Button_SLT0:
    case kControl_Button_SLT1:
    case kControl_Button_SLT2:
    case kControl_Button_SLT3:
    case kControl_Button_SLT4:
        {
            if (MouseHandler::Instance()->IsExistItemAtHand() != TRUE)
            {
                this->DeleteSlot(static_cast<POSTYPE>(control_pos), NULL);
            }
        }
        break;
    default:
        {
        }
        break;
    }

    return true;
}

void uiSmartNpcDialogReply::RenderTooltip()
{
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    BOOL bMouseOver = FALSE;
    for (POSTYPE pos = kControl_Button_SLT0; pos <= kControl_Button_SLT4; ++pos)
    {
        CControlWZ* control = GetControlWZ_byPosition(pos);
        if (control == NULL)
        {
            continue;
        }

        RECT rc = control->GetSizeRect();

        if (rc.left <= iMouseX && rc.right >= iMouseX)
        {
            if (rc.top <= iMouseY && rc.bottom >= iMouseY)
            {
                bMouseOver = TRUE;
            }
        }
    }

    if (bMouseOver == FALSE)
    {
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
    else
    {
        if (m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT)
        {
            uiToolTipMan* tooltip_manager = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if (tooltip_manager)
            {
                tooltip_manager->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
                tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}
void uiSmartNpcDialogReply::SetTextMessage(const TCHAR *text_massage)
{
    if (list_control_ == NULL)
    {
        return;
    }

    ftext_render_.ClearSentence();
    ftext_render_.SetBeginLineIndex(0);

    FTextParser parser;
    FTextSentence sentence;

    TCHAR str_format[INTERFACE_STRING_LENGTH]={0,};

    BOOL result = parser.Parser(text_massage, sentence);
    if (result == TRUE)
    {
        ftext_render_.Append(sentence);
        ftext_render_.convertLineInfo();
    }
}

RC::eSLOT_INSERT_RESULT uiSmartNpcDialogReply::InsertSlot(POSTYPE at_pos, DWORD type_code, DWORD type_index)
{
    DeleteSlot(at_pos,NULL);

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(type_code);
    if (item_info == NULL)
    {
        return RC_INSERT_SLOT_FAILED;
    }

    static DWORD SERIAL = 10000000;
    SCItemSlot item_slot;
    item_slot.SetSlotType(ST_ITEM);
    item_slot.SetCode(static_cast<SLOTCODE>(type_code));
    item_slot.SetSerial(++SERIAL);

    if (type_index >= RewardInfo::MIN_ITEM_TYPECODE)
    {
        // 장비/펫의 아이템타입/커스터마이징 아이템 처리
        const ItemType* item_type_info = ItemTypeList::Instance()->FindItemType(type_index);
        {
            KindOfItemType::value_type kind_of_item = item_type_info->GetItemType();

            nsSlot::ItemTypeChanger::ChangeItemByType(&item_slot, kind_of_item);
        }
        item_slot.SetEnchant( item_type_info->GetEnchant() );
        item_slot.SetNum(1);
    }
    else
    {
        item_slot.SetNum(static_cast<DURATYPE>(type_index));
    }

    // 슬롯에 넣음
    eSLOT_INSERT_RESULT slot_insert_result = SCSlotContainer::InsertSlot(at_pos, item_slot);

    CControlWZ* control_ptr = GetControlWZ(GetControlID_byPosition(at_pos));

    if(control_ptr)
    {
        RECT rect = control_ptr->GetSizeRect();

        SCSlot& real_slot = GetSlot(at_pos);

        if (item_unit_renderer_)
        {
            item_unit_renderer_->AddItemUnit(item_slot.GetSerial(), item_slot.GetCode(), &rect, &real_slot, control_ptr);
        }
        else
        {
            assert(item_unit_renderer_);
        }
        ReFreshRenderSlot();
    }

    return slot_insert_result;
}
//------------------------------------------------------------------------------ 
BOOL uiSmartNpcDialogReply::DeleteSlot(POSTYPE at_pos, SCSlot* slot_out)
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
void uiSmartNpcDialogReply::DeleteAllSlot()
{
    if (item_unit_renderer_ == NULL)
    {
        assert(item_unit_renderer_);
        return;
    }
    for (int slot_pos = kControl_Button_SLT0; slot_pos <= kControl_Button_SLT4; ++slot_pos)
    {
        if (IsEmpty(slot_pos))
        {
            continue;
        }
        this->DeleteSlot(slot_pos, NULL);
    }
}
SCSlotContainer* uiSmartNpcDialogReply::GetSlotContainer()
{
    return static_cast<SCSlotContainer*>(this);
}

ItemUnitRenderer* uiSmartNpcDialogReply::GetItemUnitRender() const
{
    return item_unit_renderer_;
}

RC::eSLOT_INSERT_RESULT uiSmartNpcDialogReply::InsertSlot(POSTYPE at_pos, SCSlot& IN inputed_slot)
{
    DeleteSlot(at_pos,NULL);

    // 같은 아이템을 또 올릴 경우 기존꺼 제거
    for (int slot_pos = kControl_Button_SLT0; slot_pos <= kControl_Button_SLT4; ++slot_pos)
    {
        if (IsEmpty(slot_pos))
        {
            continue;
        }

        SCSlot& slot = GetSlot(slot_pos);
        if (slot.GetSerial() == inputed_slot.GetSerial())
        {
            DeleteSlot(slot_pos,NULL);
        }
    }

    RC::eSLOT_INSERT_RESULT result = SCSlotContainer::InsertSlot(at_pos, inputed_slot);

    ReFreshRenderSlot();

    return result;
}

void uiSmartNpcDialogReply::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit* item_unit_render = NULL;
    RECT slot_rect;
    CControlWZ* control_ptr = NULL;

    for (int i = kControl_Button_SLT0; i <= kControl_Button_SLT4; ++i)
    {
        SCSlot& rSlot = GetSlot(i);
        if (IsEmpty(i))
        {
            continue;
        }

        // 보여줘야할 목록
        if(item_unit_renderer_ == NULL)
        {
            continue;
        }
        item_unit_render = item_unit_renderer_->GetItemUnit(rSlot.GetSerial());

        control_ptr = GetControlWZ(GetControlID_byPosition(i));
        if (control_ptr == NULL)
        {
            continue;
        }

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

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

