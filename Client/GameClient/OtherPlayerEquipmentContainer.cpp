#include "SunClientPrecompiledHeader.h"

#ifdef _NA_002935_20110704_ITEM_SCORE

#include "OtherPlayerEquipmentContainer.h"
#include "ItemUnitRenderer.h"
#include "ObjectManager.h"
#include "EnduranceDialog.h"
#include "InterfaceManager.h"
#include "Hero.h"
#include "InventoryDialog.h"
#include "GlobalFunc.h"
#include "AppearanceManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "CursorChangeTransact.h"
#include "ItemScoreParser.h"

OtherPlayerEquipmentContainer::OtherPlayerEquipmentContainer(InterfaceManager* ui_manager) :       
uiBase(ui_manager),
item_unit_renderer_(NULL)
{

}

OtherPlayerEquipmentContainer::~OtherPlayerEquipmentContainer()
{
    Release();
}

VOID OtherPlayerEquipmentContainer::Release()
{
    SAFE_RELEASENDELETE(item_unit_renderer_);
}

VOID OtherPlayerEquipmentContainer::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    is_pc_room_to_target_ = false;
    item_score_ = 0;

    InitItemRender();
}

VOID OtherPlayerEquipmentContainer::OnShowWindow( BOOL val )
{
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_OtherPlayerEquipment_Dialog;
        msg.wParam= IM_INVENTORY_MANAGER::OTHER_PLAYER_EQUIP_DLG;
        msg.DoSomething=OtherPlayerEquipmentContainer::ESCKeyProcess;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {

        //기존 슬롯 지움
        FlushUnitDraw();

        g_KeyQueueManager.DeleteMsg(eDoType_OtherPlayerEquipment_Dialog);
    }
}

VOID OtherPlayerEquipmentContainer::OnRenderSolarDialog()
{

    if (item_unit_renderer_ && 
        g_InterfaceManager.IsShowDialog(IM_INVENTORY_MANAGER::OTHER_PLAYER_EQUIP_DLG))
    {
        item_unit_renderer_->Render();
    }

    //캐릭터 출력
    Object* target_object = g_ObjectManager.GetObject(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY);
    Player* player = static_cast<Player*>(target_object);

    if (player != NULL)
    {			
        RECT rect_size;
        CControlWZ *control_pointer = getControl(EQUIP_CHARACTER_VIEW);
        if (control_pointer)
        {
            rect_size = control_pointer->GetSizeRect();
        }
        else
        {
            GetDialogWZ()->GetClientRect(&rect_size);
        }	       

        int rect_width = rect_size.right - rect_size.left;
        int rect_height = rect_size.bottom - rect_size.top; 

        float mesh_width = 1.0f;
        float mesh_height = 1.0f;

        // 캐릭터의 키를 구해보려 하나 무슨짓을해도 소용이없었다.
        if (player->GetBasePart())
        {
            WzBoundingVolume bounding_volume = player->GetBasePart()->GetBoundingVolume();

            switch(bounding_volume.m_eBvType)
            {
            case BVT_AABB:
                mesh_height = bounding_volume.m_aabb.m_wvMax.y - bounding_volume.m_aabb.m_wvMin.y;
                mesh_width = bounding_volume.m_aabb.m_wvMax.x - bounding_volume.m_aabb.m_wvMin.x;
                break;
            case BVT_SPHERE:
                mesh_height = (bounding_volume.m_BvSphere.m_wvCenter.z
                    + bounding_volume.m_BvSphere.m_fRadius) * 1.2f;
                mesh_width = bounding_volume.m_BvSphere.m_fRadius;
                break;
            }
        }

        // 현재 BoundingBox를 구할수없어 캐릭터의 키를 구할수없더.
        // 캐릭터의 키를 하드코딩한다.
        switch(player->GetClass())
        {
                case eCHAR_BERSERKER:
                    mesh_height = 2.0f;
                    mesh_width = 1.2f;
                    break;
                case eCHAR_DRAGON:
                    mesh_height = 1.8f;
                    mesh_width = 1.2f;
                    break;
                case eCHAR_VALKYRIE:
                    mesh_height = 1.7f;
                    mesh_width = 1.0f;
                    break;
                case eCHAR_SHADOW:
                    mesh_height = 1.7f;
                    mesh_width = 1.0f;
                    break;
                case eCHAR_MAGICIAN:
                    mesh_height = 1.7f;
                    mesh_width = 1.0f;
                    break;
                case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                    mesh_height = 1.7f;
                    mesh_width = 1.0f;
                    break;
                case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                    mesh_height = 1.7f;
                    mesh_width = 1.0f;
                    break;
                case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    mesh_height = 1.7f;
                    mesh_width = 1.0f;
                    break;
        }

        if(player->IsTransForm())//변신 중일때
        {
            mesh_height = 2.2f;
            mesh_width = 1.2f;
        }

        //변신이 풀렸지만 아직 캐릭터로 복원되지 않았을때
        if((player->IsTransForm() == FALSE) &&
            (player->GetTransformData().GetTransFormProcessType() != TRANSFORM_PROCESS_NONE) &&
            (player->GetTransformData().GetOldTransFormType()))
        {
            mesh_height = 2.2f;
            mesh_width = 1.2f;
        }

        // 뷰포트는 반드시 g_InterfaceManager.CalcuateViewPortWithRect로 생성해야함
        RECT rect_viewport;
        rect_viewport = g_InterfaceManager.CalcuateViewPortWithRect(rect_size);
        player->SetShow(TRUE);

        g_InterfaceManager.RenderObjectOnRect(player,rect_viewport,mesh_width,mesh_height);
        player->SetShow(FALSE);
    }

    //아이템 평점을 출력한다.
    DrawItemScore();

    if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
    {
        if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
        {
            uiToolTipMan* ui_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if(ui_manager != NULL)
            {
                if (is_pc_room_to_target_ == true)
                {
                    ui_manager->RegisterTooltipItem(m_MouseOverObject.pSlot,
                        FALSE,
                        kItemOtherEquipPcRoomEnable); //LimitStatCompositeType을 매개변수의 용도로 사용한다.
					ui_manager->SetTooltipDialogID(GetDialogKey());
                }
                else
                {
                    ui_manager->RegisterTooltipItem(m_MouseOverObject.pSlot,
                        FALSE,
                        kItemOtherEquipPcRoomDisable); //LimitStatCompositeType을 매개변수의 용도로 사용한다.
					ui_manager->SetTooltipDialogID(GetDialogKey());
                }
                if( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
                {
                    uiToolTipMan* ui_manager2 = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP2);
                    if(ui_manager2)
                    {
                        ui_manager2->RegisterHeroItemInfoTooltipFromSlot(m_MouseOverObject.pSlot);
                    }
                }
            }

            MouseHandler::MOUSE_STATE mouse_state = MouseHandler::Instance()->GetMouseState();

            if(MouseHandler::GUILD_WITHDRAW != mouse_state &&
                MouseHandler::GUILD_DEPOSIT != mouse_state &&
                MouseHandler::WITHDRAW != mouse_state &&
                MouseHandler::DEPOSIT != mouse_state)
            {
                CursorChangeTransact::Instance()->ChangeTranaction(
                    MouseCursor::eMouseCursor_Cursor_Cancle);
            }
        }
    }

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    BOOL mouse_over = FALSE;
    for (POSTYPE position = EQUIP_SLOT_START; position < EQUIP_SLOT_END; ++position)
    {
        CControlWZ * control = getControl(position);
        if (control == NULL)
        {
            continue;
        }

        RECT rect = control->GetSizeRect();

        if (rect.left <= mouse_x && rect.right >= mouse_x)
        {
            if (rect.top <= mouse_y && rect.bottom >= mouse_y)
            {
                mouse_over = TRUE;
            }
        }
    }
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	for (POSTYPE pos = EQUIP_SCORE_START; pos < EQUIP_SCORE_END; ++pos)
    {
        CControlWZ* control = getControl(pos);
        if (control == NULL)
        {
             continue;
		}
        RECT rect = control->GetSizeRect();
        if (rect.left <= mouse_x && rect.right >= mouse_x)
        {
            if (rect.top <= mouse_y && rect.bottom >= mouse_y)
            {
                mouse_over = TRUE;
            }
        }
    }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    if (mouse_over == NULL)
    {
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
    else
    {
        RenderTooltip(GetDialogKey());
    }

}

void OtherPlayerEquipmentContainer::DrawItemScore()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	CControlWZ* title_control = getControl(EQUIP_TXT_TITLE);

	if ( title_control== NULL )
	{
		return;
	}

	item_score_ = item_score_ > 99999 ? 99999 : item_score_;

	//현재 스트링ID를 받아온다.
	DWORD TextID= title_control->GetTextID();

	TCHAR notice_message[INTERFACE_STRING_LENGTH];
	memset(notice_message,0,sizeof(TCHAR)*INTERFACE_STRING_LENGTH);

	//장비평점은 최대 99999임.
	TCHAR notice_Score[6];
	sprintf(notice_Score,"%d",item_score_);

	g_InterfaceManager.GetInterfaceStringFormat(notice_message,INTERFACE_STRING_LENGTH,
		TextID,notice_Score);

	title_control->SetTextWZA(notice_message);
#else
	CControlWZ* number_control = getControl(EQUIP_SCORE_NUMBER1);
    if (number_control)
    {
        RECT first_position = number_control->GetSizeRect();
        g_InterfaceManager.RenderDrawItemScore(first_position, item_score_);
    }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
}

VOID OtherPlayerEquipmentContainer::MessageProc(SI_MESSAGE* message_pointer)
{
    switch(message_pointer->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE  position = getCtrlIDToPos(message_pointer->dwCtrlID);
            if (position == EQUIP_EXIT)
            {
                ShowInterface(FALSE);
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message_pointer);
        }
        break;
    }
}

VOID OtherPlayerEquipmentContainer::MouseOver_MsgProc(SI_MESSAGE* message_pointer)
{
    switch(message_pointer->eResultMsg)
    {
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE position = getCtrlIDToPos(message_pointer->dwCtrlID);

            if (EQUIP_SLOT_START <= position && EQUIP_SLOT_END > position)
            {
                POSTYPE relative_pos = GetRealSlotPos(position);

                if (relative_pos >= GetMaxSlotNum())
                {
                    // 잘못된 슬롯 위치를 참조할려고한다 리턴.
                    return;
                }

                if (IsEmpty(relative_pos) == FALSE)
                {
                    m_iTooltipStringCode = 0;

                    SCSlot & slot = GetSlot(relative_pos);
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
                    m_MouseOverObject.pSlot = &slot;
                }
                else
                {
                    m_iTooltipStringCode = getTooltipString(position);
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
                }
            }
#ifdef _NA_002935_20110704_ITEM_SCORE
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			else if ((EQUIP_SCORE_START <= position) && (EQUIP_SCORE_END > position))
            {
                // 11478 : 장비 평점
                m_iTooltipStringCode = 11478;
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
            }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
#endif //_NA_002935_20110704_ITEM_SCORE
            else
            {
                m_iTooltipStringCode = 0;
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
            }
        }
        break;
    }
}

RC::eSLOT_INSERT_RESULT OtherPlayerEquipmentContainer::InsertSlot(POSTYPE position, SCSlot& slot)
{
    RC::eSLOT_INSERT_RESULT insert_slot_result = EquipmentContainer::InsertSlot(position, slot);

    if (insert_slot_result != RC::RC_INSERT_SLOT_SUCCESS)
    {
        return insert_slot_result;
    }


    // 3D 모델링 Render
    CControlWZ* ctrl_pointer = getControl(position);
    RECT rect = ctrl_pointer->GetSizeRect();

    SCSlot& real_slot = GetSlot(position);


    if(item_unit_renderer_)
    {
        item_unit_renderer_->AddItemUnit(
            real_slot.GetSerial(),
            real_slot.GetCode(),
            &rect,
            &real_slot,
            ctrl_pointer);
    }
    else
    {
        assert(item_unit_renderer_);
    }

    Player* player =
        static_cast<Player*>(g_ObjectManager.GetObject(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY));
    if (player) 
    {
        SCSlotContainer* slot_container = player->GetSlotContainer(SI_EQUIPMENT);
        if (slot_container) 
        {
            slot_container->InsertSlot(position,slot);
        }
    }

    return insert_slot_result;
}

BOOL OtherPlayerEquipmentContainer::DeleteSlot(POSTYPE position, SCSlot* slot_out)
{
    // render unit 를 먼저 지운다.
    SCSlot& slot = GetSlot(position);

    if(item_unit_renderer_)
    {
        item_unit_renderer_->RemoveItemUnit(slot.GetSerial());
    }
    else
    {
        assert(item_unit_renderer_);
    }

    // 시리얼 Restore는 EquipmentContainer 에서 한다. 
    EquipmentContainer::DeleteSlot(position, slot_out);

    Player* player =
        static_cast<Player*>(g_ObjectManager.GetObject(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY));
    if (player) 
    {
        SCSlotContainer *slot_container = player->GetSlotContainer(SI_EQUIPMENT);
        if (slot_container) 
        {
            slot_container->DeleteSlot(position, NULL);
        }
    }

    return TRUE;
}

VOID OtherPlayerEquipmentContainer::ReFreshRenderSlot()
{
    ItemUnitRenderer::RenderUnit* render_unit = NULL;
    RECT slot_rect;
    CControlWZ* ctrl_pointer = NULL;

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        SCSlot& slot = SCSlotContainer::GetSlot(i);

        if (slot.GetSerial())
        {
            if ((i >= m_TabNum) && (i < m_TabNum + EQUIP_SLOT_END))
            {
                // 보여줘야할 목록
                if(item_unit_renderer_ == NULL)
                {
                    continue;
                }

                render_unit = item_unit_renderer_->GetItemUnit(slot.GetSerial());

                if (render_unit == NULL)
                {
                    ctrl_pointer = getControl(GetRelativeSlotPos(i));
                    slot_rect = ctrl_pointer->GetSizeRect();

                    item_unit_renderer_->AddItemUnit(
                        slot.GetSerial(),
                        slot.GetCode(),
                        &slot_rect,
                        &slot,
                        ctrl_pointer);
                }

                if (render_unit)
                {
                    render_unit->SetShow(TRUE);
                }
            }
        }
    }
}

VOID OtherPlayerEquipmentContainer::FlushUnitDraw()
{
    if (item_unit_renderer_ == NULL)
    {
        return;
    }

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        if(IsEmpty(i) == FALSE)
        {
            DeleteSlot(i, NULL);
        }
    }
}

VOID OtherPlayerEquipmentContainer::InitItemRender()
{
    if (item_unit_renderer_ == NULL)
    {
        item_unit_renderer_ = new ItemUnitRenderer;
        item_unit_renderer_->Init(uiBase::GetDrawBase(), GetMaxSlotNum());
    }
}

VOID OtherPlayerEquipmentContainer::SetEquipmentContainer(Player* player)
{
    if (player == NULL)
    {
        return;
    }

    //대상이 pc방인지를 저장해둠
    is_pc_room_to_target_ = !!(player->GetPCBangMode());
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	//창 제목에 아이디 표시
    CControlWZ* title_control = getControl(EQUIP_TXT_TITLE);
    if (title_control)
    {
        title_control->SetTextWZ(player->GetName());
    }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

    Object* object_pointer =
        g_ObjectManager.GetObject(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY);
    Player* other_player = static_cast<Player*>(object_pointer);
    if (other_player == NULL)
    {
        //창 내의 캐릭터 생성
        DWORD class_code = player->GetClass();
        ClassCodeEx class_code_ex;
        class_code_ex.class_type = player->GetClass();
        class_code_ex.gender = player->GetGender();
        class_code = class_code_ex.class_code;

        other_player = static_cast<Player*>(g_ObjectManager.Add(
            OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY, 
            PLAYER_OBJECT,
            class_code,
            FALSE));
    }
    else
    {
        if(other_player->IsTransForm())//변신 중일때
        {
            UnTransForm(player);
        }

        APPEARANCEINFO parts_info;
        ZeroMemory(&parts_info, sizeof(parts_info));
        other_player->SetClass(player->GetClass());
        other_player->SetGender(player->GetGender());

        //파츠 갱신 (뼈대까지)
        other_player->RefreshAppearRender(TRUE);
    }


    //캐쉬 슬롯 설정
    ChargeItemExtendSlot(player->GetCharInfo()->m_ExtraEquipCashSlotCount);

    //캐릭터 모습 설정
    if (other_player != NULL)
    {
        WzVector vec_camera_from, vec_camera_to, m_wvZ;
        SetVector(&vec_camera_from, 0.0, 0.0, 0.0);
        SetVector(&vec_camera_to, 0.0, 100.0, 0.0);
        SetVector(&m_wvZ, 0.0, 0.0, 1.0);
        other_player->SetDirection(vec_camera_from, vec_camera_to,TRUE);
        WzVector pos;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = -100.0f;
        other_player->SetPosition(pos);

        CLIENT_CHARACTER_PART part;
        ZeroMemory(&part,sizeof(part));
        part.m_byClass = player->GetClass();
        part.m_bySlot =0;

        other_player->SetClientChar(part);
        other_player->SetAnimation("N002", TRUE);
        other_player->SetColor(WzColor_RGBA(255,255,255,255));
        other_player->SetShow(TRUE); //직접 그릴것임 
        other_player->SetFreeMove(TRUE);
        other_player->ProcessAnimation(0);

        other_player->SetPartVariation( PLAYER_VARIATION_FACE, player->GetPartVariation(PLAYER_VARIATION_FACE) );
        other_player->SetPartVariation( PLAYER_VARIATION_HAIR, player->GetPartVariation(PLAYER_VARIATION_HAIR) );

        other_player->HideHelmet(player->IsHideHelmet(),FALSE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        other_player->HideWing(player->IsHideWing(),FALSE);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
        other_player->SetCostumeItemCode( player->GetCostumeItemCode() );
#endif//_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		other_player->SetDecorateCostumeItemCode( player->GetDecorateCostumeItemCode() );
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

        EQUIP_ITEM_INFO out;
        player->GetEquipItemTotalInfo(out);
        other_player->SetEquipItemTotalInfo(out);

        if(player->IsTransForm())//변신 중일때
        {
            TransForm(player);
        }

        other_player->RefreshAppearRender();

        other_player->SetShow(FALSE);
        other_player->SetShadowDraw(FALSE);
    }

    if (IsVisibleWindow() == TRUE)
    {
        //기존 슬롯 지움
        FlushUnitDraw();
    }

    
    EquipmentContainer* equipment_container = player->GetEquipmentContainer();
    if (equipment_container != NULL)
    {
        //점수 넣기
        item_score_ = player->GetItemScore();

        //슬롯 넣기
        for (POSTYPE index = 0; index < equipment_container->GetMaxSlotNum(); ++index)
        {
            SCSlot& slot = equipment_container->GetSlot(index);
            if (slot.GetCode() > 0)
            {
                InsertSlot(index, slot);
            }
        }
        ReFreshRenderSlot();
    }
}

void OtherPlayerEquipmentContainer::ESCKeyProcess(DWORD wparam,DWORD lparam)
{
    OtherPlayerEquipmentContainer* dialog =
        GET_CAST_DIALOG(OtherPlayerEquipmentContainer,
        IM_INVENTORY_MANAGER::OTHER_PLAYER_EQUIP_DLG);
    if (dialog)
    {
        dialog->ShowInterface(FALSE);
    }
}

void OtherPlayerEquipmentContainer::TransForm(Player* player)
{
    Object* target_object = g_ObjectManager.GetObject(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY);
    Player* dialog_player = static_cast<Player *>(target_object);
    if(dialog_player && player)
    {
        dialog_player->GetTransformData().SetTransFormResourceCode(player->GetTransformData().GetTransFormResourceCode());
        dialog_player->GetTransformData().SetTransFormType(player->GetTransformData().GetTransFormType());
        dialog_player->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_TRANSFORM);
        dialog_player->GetTransformData().SetAppearanceKind(player->GetTransformData().GetAppearanceKind());
        dialog_player->TransForm();
        dialog_player->SetAnimation("N002", TRUE);        
    }
}

void OtherPlayerEquipmentContainer::UnTransForm(Player* player)
{
    Object* target_object = g_ObjectManager.GetObject(OTHER_PLAYER_EQUIPMENT_CHARACTER_KEY);
    Player* dialog_player = static_cast<Player *>(target_object);

    if(dialog_player && player)
    {
        //강제로 변신 해제
        dialog_player->GetTransformData().SetTransFormType(eTRANSFORM_TYPE_NONE);
        dialog_player->GetTransformData().SetAppearanceKind(APPEARANCEINFO::PLAYER);
        dialog_player->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_UNTRANSFORM);
        dialog_player->UnTransForm();
        dialog_player->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_NONE);
        dialog_player->SetAnimation("N002", TRUE);
    }
}

CControlWZ* OtherPlayerEquipmentContainer::getControl(POSTYPE at_pos)
{
    assert(at_pos < EQUIP_MAX);

    return GetControlWZ(m_wzId[at_pos]);
}

VOID OtherPlayerEquipmentContainer::getControlPos(POSTYPE at_pos, RECT& rect_out)
{
    CControlWZ* control = getControl(at_pos);
    rect_out = control->GetSizeRect();
}

VOID OtherPlayerEquipmentContainer::ChargeItemExtendSlot(int extend_count)
{
    // 상대방의 캐쉬 확장 슬롯 정보를 보이도록 설정한다. (컨텐츠에 영향이 없고 조금이라도 서버의 부하를 줄이기 위함)
    extend_count = 2;

    CCtrlButtonWZ* charge_slot4 =	static_cast<CCtrlButtonWZ*>(getControl(EQUIP_CHARGE4));
    CCtrlButtonWZ* charge_slot5 =  static_cast<CCtrlButtonWZ*>(getControl(EQUIP_CHARGE5));
    if( !charge_slot4 || !charge_slot5 )
        return;

    charge_slot4->EnableWindowWZ(FALSE);
    charge_slot5->EnableWindowWZ(FALSE);

    charge_slot4->ShowWindowWZ(WZ_HIDE);
    charge_slot5->ShowWindowWZ(WZ_HIDE);

    CControlWZ* charge_image4 = getControl(EQUIP_PIC_CHARGE4);
    assert(charge_image4);
    CControlWZ* charge_image5 = getControl(EQUIP_PIC_CHARGE5);
    assert(charge_image5);

    if( !charge_image4 || !charge_image5)
        return;

    charge_image4->ShowWindowWZ(WZ_HIDE);
    charge_image5->ShowWindowWZ(WZ_HIDE);

    if( extend_count >= 1)
    {
        charge_slot4->EnableWindowWZ(TRUE);
        charge_slot4->ShowWindowWZ(WZ_SHOW);
        charge_image4->ShowWindowWZ(WZ_SHOW);
    }

    if( extend_count >= 2)
    {
        charge_slot5->EnableWindowWZ(TRUE);
        charge_slot5->ShowWindowWZ(WZ_SHOW);
        charge_image5->ShowWindowWZ(WZ_SHOW);
    }
}

#endif //_NA_002935_20110704_ITEM_SCORE