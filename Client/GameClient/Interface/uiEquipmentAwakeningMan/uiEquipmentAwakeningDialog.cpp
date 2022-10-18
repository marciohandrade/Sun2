#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningDialog.h"
#include "uiEquipmentAwakeningMan.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "MouseHandler.h"
#include "ItemManager.h"
#include "SCItemSlot.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "CursorChangeTransact.h"
#include "SoundEffect.h"
#include "Mouse.h"
#include "AwakeningInfoParser.h"
#include "ItemInfoParser.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "InventoryDialog.h"

WzID uiEquipmentAwakeningDialog::wz_id_[kControl_Size] = 
{
	StrToWzID("B000"),
	StrToWzID("B002"),
	StrToWzID("B003"),

	StrToWzID("B999"),

	StrToWzID("T004"),
	StrToWzID("S001"),

	StrToWzID("S004"),
	StrToWzID("S007"),

	StrToWzID("S017"),
	StrToWzID("S016"),

	StrToWzID("P001"),
	StrToWzID("P008"),
	StrToWzID("P009"),
	StrToWzID("P010"),
	StrToWzID("P017"),
	StrToWzID("P020"),

	StrToWzID("B005"),

	StrToWzID("B006"),

	StrToWzID("B001"),
};

uiEquipmentAwakeningDialog::uiEquipmentAwakeningDialog(InterfaceManager* ui_manager) : 
uiBase( ui_manager ),
ui_equipment_awakening_manager_(NULL),
item_unit_renderer_(NULL),
equip_item_pos_(INVALID_SLOT_POS),
material_item_pos_(INVALID_SLOT_POS),
additive_item_pos_(INVALID_SLOT_POS)
{
}

uiEquipmentAwakeningDialog::~uiEquipmentAwakeningDialog(void)
{
}

void uiEquipmentAwakeningDialog::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
	RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
	RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
	RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
	RegisterControlPosition(StrToWzID("T004"), kControl_Text_T004);
	RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
	RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
	RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
	RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
	RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
	RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
	RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
	RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
	RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
	RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
	RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
	RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
	RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
	RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);

	ui_equipment_awakening_manager_ = 
		GET_DIALOG_MANAGER(uiEquipmentWakeupMan, UIMAN_EQUIPMENT_WAKEUP);

	slot_container_[EQUIPMENT_EQUIP_SLOT].Initialize( 1, SI_EQUIPMENT_AWAKENING );
	slot_container_[EQUIPMENT_MATERIAL_SLOT].Initialize( 1, SI_EQUIPMENT_AWAKENING );
	slot_container_[EQUIPMENT_ADDITIVE_SLOT].Initialize( 1, SI_EQUIPMENT_AWAKENING );

	RECT rc = {0, };
	if ( draw_base )
	{
		equipment_awakening_first_render_.Init(draw_base, rc);
		equipment_awakening_second_render_.Init(draw_base, rc);
		material_first_render_.Init(draw_base, rc);
		material_second_render_.Init(draw_base, rc);
		additive_first_render_.Init(draw_base, rc);
		additive_second_render_.Init(draw_base, rc);
	}

	m_packetStatus = PACKET_STATUS_NONE;
}

void uiEquipmentAwakeningDialog::Release()
{
	for ( int i = EQUIPMENT_EQUIP_SLOT; i < EQUIPMENT_AWAKENING_SLOT_MAX; ++i )
	{
		slot_container_[i].Release();
	}
}

void uiEquipmentAwakeningDialog::MessageProc( SI_MESSAGE* message )
{
	if ( IsLock() )
		return;

	if ( GetMan() == NULL )
		return;

	switch ( message->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			OnLButtonClick( message );
		}
		break;
	case RT_MSG_RBUTTONCLICK:
		{
			OnRButtonClick( message );
		}
		break;
	case RT_MSG_MOUSEOVER:
		{
			OnMouseOver( message );
		}
		break;

	default:
		SolarDialog::MessageProc( message );
		break;
	}
}

void uiEquipmentAwakeningDialog::NetworkProc( MSG_BASE* recv_packet )
{

}

void uiEquipmentAwakeningDialog::OnUpdateSolarDialog()
{
	for ( int i = EQUIPMENT_EQUIP_SLOT; i < EQUIPMENT_AWAKENING_SLOT_MAX; ++i )
	{
		slot_container_[i].Update();
	}

	UpdateTooltip();

	UpdateFtextRenderRectSetting();
}

void uiEquipmentAwakeningDialog::OnRenderSolarDialog()
{
	for ( int i = EQUIPMENT_EQUIP_SLOT; i < EQUIPMENT_AWAKENING_SLOT_MAX; ++i )
	{
		slot_container_[i].Render();
	}

	if ( equip_item_pos_ != INVALID_SLOT_POS )
	{
		equipment_awakening_first_render_.Render();
		equipment_awakening_second_render_.Render();
	}

	if ( material_item_pos_ != INVALID_SLOT_POS )
	{
		material_first_render_.Render();
		material_second_render_.Render();
	}
	
	additive_first_render_.Render();
	additive_second_render_.Render();	
}

void uiEquipmentAwakeningDialog::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_EQUIP, TRUE );

		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg));
		msg.dwType = eDoType_Equipment_Awakening_Dialog;
		msg.wParam = IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);

		// 결정화 상태일 때는 해제.
		if (MouseHandler::Instance()->GetMouseState() == MouseHandler::CRYSTALIZATION)
		{
			MouseHandler::Instance()->RemoveMouseState(MouseHandler::CRYSTALIZATION);
			g_KeyQueueManager.DeleteMsg(eDoType_Crystalization);
			MouseHandler::Instance()->RollbackItemAtHand();
		}

		InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));
		if (inventory_dialog_ptr)
		{
			inventory_dialog_ptr->HideWindowProcess();
			inventory_dialog_ptr->ImpossibleAwakeningItemIonDeActiveRender();
		}

		MouseHandler::Instance()->SetMouseState(MouseHandler::AWAKENING);
	}
	else
	{
		InitEquipmentAwakeningSlotContainer();
		InitFtextRender();
		EquipmentAwakeningSlotClearSetting();
		MaterialSlotClearSetting();
		AdditiveSlotClearSetting();

		g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_EQUIP, FALSE );

		g_KeyQueueManager.DeleteMsg(eDoType_Equipment_Awakening_Dialog);

		for ( int i = EQUIPMENT_EQUIP_SLOT; i < EQUIPMENT_AWAKENING_SLOT_MAX; ++i )
		{
			slot_container_[i].Clear();
		}

		m_packetStatus = PACKET_STATUS_NONE;

		InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));
		if (inventory_dialog_ptr)
		{
			inventory_dialog_ptr->RevertAwakeningItemIconDeActiveRender();
		}

		MouseHandler::Instance()->RemoveMouseState(MouseHandler::AWAKENING);
	}
}

bool uiEquipmentAwakeningDialog::OnLButtonClick( SI_MESSAGE* message )
{
	if ( BIT_CHECK(m_packetStatus, PACKET_STATUS_AWAKENING) )
	{
		return false;
	}

	DWORD control_position = GetControlPosition_byID( message->dwCtrlID );
	switch ( control_position )
	{
	case kControl_Button_B000:
	case kControl_Button_B002:
	case kControl_Button_B003:
		{
			if ( MouseHandler::Instance()->IsExistItemAtHand() )
			{
				MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();

				if ( mouse_item )
				{
					OnEquipmentAwakeningItem_Click( (POSTYPE)control_position, mouse_item->m_fromSlotIdx, mouse_item->m_fromPos );
				}

				MouseHandler::Instance()->RollbackItemAtHand();
			}

		}
		break;

	case kControl_Button_B999:
		{
			ShowInterface( FALSE );
		}
		break;

	case kControl_Button_B005:
		{
			Awakening();
		}
		break;

	case kControl_Button_B006:
		{
			ShowInterface( FALSE );
		}
		break;

	case kControl_Button_B001:
		{
			OpenTutorialGuid();
		}
		break;
	}

	return true;
}

bool uiEquipmentAwakeningDialog::OnRButtonClick( SI_MESSAGE* message )
{
	if ( BIT_CHECK(m_packetStatus, PACKET_STATUS_AWAKENING) )
	{
		return false;
	}

	bool is_change = false;
	DWORD control_position = GetControlPosition_byID( message->dwCtrlID );
	switch ( control_position )
	{
	case kControl_Button_B000:
		{
			EquipmentAwakeningSlotClearSetting();
			MaterialSlotClearSetting();
			AdditiveSlotClearSetting();
			is_change = true;
		}
		break;
	case kControl_Button_B002:
		{
			MaterialSlotClearSetting();
			AdditiveSlotClearSetting();
			is_change = true;
		}
		break;
	case kControl_Button_B003:
		{
			AdditiveSlotClearSetting();
			is_change = true;
		}
		break;
	}

	if ( is_change == true )
	{
		InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));
		if (inventory_dialog_ptr)
		{
			inventory_dialog_ptr->RevertAwakeningItemIconDeActiveRender();
			inventory_dialog_ptr->ImpossibleAwakeningItemIonDeActiveRender();
		}
	}

	return true;
}

bool uiEquipmentAwakeningDialog::OnMouseOver( SI_MESSAGE* message )
{
	m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

	int mx = Mouse::Instance()->GetMouseX();
	int my = Mouse::Instance()->GetMouseY();

	for ( POSTYPE pos = kControl_Button_B000; pos <= kControl_Button_B003; ++pos )
	{
		if ( CControlWZ* ctl = getControl( pos ) )
		{
			RECT rc = ctl->GetSizeRect();

			if( rc.left <= mx && rc.right >= mx && rc.top <= my && rc.bottom >= my)
			{
				int slotPos = ( pos - kControl_Button_B000 );

				if( SCItemSlot* item = slot_container_[pos].GetSlotItem(0) )
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = item;
				}				
			}
		}
	}

	return true;
}

void uiEquipmentAwakeningDialog::OnEquipmentAwakeningItem_InventoryRightClick(const SLOTIDX from_container_index, 
																			   const POSTYPE from_container_position )
{
	 POSTYPE equipment_wakeup_slot_index = INVALID_SLOT_POS;

	 if ( equip_item_pos_ == INVALID_SLOT_POS )
	 {
		 equipment_wakeup_slot_index = EQUIPMENT_EQUIP_SLOT;
	 }
	 else if ( material_item_pos_ == INVALID_SLOT_POS )
	 {
		 equipment_wakeup_slot_index = EQUIPMENT_MATERIAL_SLOT;
	 }
	 else if ( additive_item_pos_ == INVALID_SLOT_POS )
	 {
		 equipment_wakeup_slot_index = EQUIPMENT_ADDITIVE_SLOT;
	 }

	 if ( equipment_wakeup_slot_index == INVALID_SLOT_POS )
	 {
		 return;
	 }


	 bool result = false;
	if ( MouseHandler::Instance()->ItemEquipmentAwakening_InventoryRightClick(
		from_container_index, SI_EQUIPMENT_AWAKENING, from_container_position, 0) )
	{
		if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( from_container_position ) )
		{
			switch ( equipment_wakeup_slot_index )
			{
			case EQUIPMENT_EQUIP_SLOT:
				{
					if ( PermitEquipmentCheck(item) == true )
					{
						equip_item_pos_ = from_container_position;
						EquipmentInfoSetting();
						result = true;
					}
				}
				break;
			case EQUIPMENT_MATERIAL_SLOT:
				{
					if ( PermitMaterialCheck(item) == true )
					{
						material_item_pos_ = from_container_position;
						MaterialInfoSetting();
						result = true;
					}
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
#else
					else
					{
						// 중앙 메시지 출력
						// 5958 : 재료로 사용 할 아이템을 먼저 등록해 주세요.
						GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(5958));
						return;
					}
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
				}
				break;
			case EQUIPMENT_ADDITIVE_SLOT:
				{
					if ( PermitAdditiveCheck(item) == true )
					{
						additive_item_pos_ = from_container_position;
						AdditiveInfoSetting();
						result = true;
					}
					else
					{
						// 중앙 메시지 출력
						// 3994 : 각성 첨가제 아이템을 등록하세요.						
						GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(3994));
						return;						
					}
				}
				break;
			}

			if ( result == true )
			{
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
				if ( equipment_wakeup_slot_index != EQUIPMENT_ADDITIVE_SLOT && equipment_wakeup_slot_index != EQUIPMENT_MATERIAL_SLOT )
#else
				if ( equipment_wakeup_slot_index != EQUIPMENT_ADDITIVE_SLOT )
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
				{
					slot_container_[equipment_wakeup_slot_index].SetSlotItem(0, *item, getControl(equipment_wakeup_slot_index));
					slot_container_[equipment_wakeup_slot_index].GetSlotItem(0)->SetLock(false);
				}
				InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));
				if (inventory_dialog_ptr)
				{
					inventory_dialog_ptr->RevertAwakeningItemIconDeActiveRender();
					inventory_dialog_ptr->ImpossibleAwakeningItemIonDeActiveRender();
				}
				item->SetLock(true);
			}
		}
	}
}

void uiEquipmentAwakeningDialog::OnEquipmentAwakeningItem_Click( const POSTYPE& equipment_wakeup_slot_index, const SLOTIDX from_container_index, const POSTYPE from_container_position )
{
	bool result = false;
	if ( MouseHandler::Instance()->ItemEquipmentAwakening_InventoryRightClick(
		from_container_index, SI_EQUIPMENT_AWAKENING, from_container_position, 0) )
	{
		if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( from_container_position ) )
		{
			switch ( equipment_wakeup_slot_index )
			{
			case EQUIPMENT_EQUIP_SLOT:
				{
					if ( PermitEquipmentCheck(item) == true )
					{
						if ( equip_item_pos_ != INVALID_SLOT_POS )
						{
							// 이미 아이템이 있을 경우 Lock을 해제
							SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);
							if ( equip_item )
							{
								equip_item->SetLock(false);
							}
							// 전체 클리어
							EquipmentAwakeningSlotClearSetting();
							MaterialSlotClearSetting();
							AdditiveSlotClearSetting();

							MaterialInfoSetting();
						}
						equip_item_pos_ = from_container_position;
						EquipmentInfoSetting();
						result = true;
					}
				}
				break;
			case EQUIPMENT_MATERIAL_SLOT:
				{
					if ( equip_item_pos_ != INVALID_SLOT_POS )
					{
						if ( PermitMaterialCheck(item) == true )
						{
							if ( material_item_pos_ != INVALID_SLOT_POS )
							{
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
								MaterialSlotClearSetting();
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
								// 이미 아이템이 있을 경우 Lock을 해제
								SCItemSlot* material_item = ItemManager::Instance()->GetInventoryItem(material_item_pos_);
								if ( material_item )
								{
									material_item->SetLock(false);
								}
								AdditiveInfoSetting();
							}
							material_item_pos_ = from_container_position;
							MaterialInfoSetting();
							result = true;
						}
					}
					else
					{
						// 중앙 메시지 출력
						// 5957 : 각성 할 아이템을 먼저 등록해 주세요
						GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(5957));
						return;
					}
				}
				break;
			case EQUIPMENT_ADDITIVE_SLOT:
				{
					if ( material_item_pos_ != INVALID_SLOT_POS )
					{
						if ( PermitAdditiveCheck(item) == true )
						{
							if ( additive_item_pos_ != INVALID_SLOT_POS )
							{
								// 이미 아이템이 있을 경우 Lock을 해제
								SCItemSlot* addictive_item = ItemManager::Instance()->GetInventoryItem(additive_item_pos_);
								if ( addictive_item )
								{
									addictive_item->SetLock(false);
								}
							}

							additive_item_pos_ = from_container_position;
							AdditiveInfoSetting();
							result = true;
						}
					}
					else
					{
						// 중앙 메시지 출력
						// 5958 : 재료로 사용 할 아이템을 먼저 등록해 주세요.
						GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(5958));
						return;
					}
				}
				break;
			}

			if ( result == true )
			{
				// 첨가제 아이템이 아닌 경우에만 슬롯 추가
				if ( equipment_wakeup_slot_index != EQUIPMENT_ADDITIVE_SLOT )
				{
					slot_container_[equipment_wakeup_slot_index].SetSlotItem(0, *item, getControl(equipment_wakeup_slot_index));
					slot_container_[equipment_wakeup_slot_index].GetSlotItem(0)->SetLock(false);
				}
				InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));
				if (inventory_dialog_ptr)
				{
					inventory_dialog_ptr->RevertAwakeningItemIconDeActiveRender();
					inventory_dialog_ptr->ImpossibleAwakeningItemIonDeActiveRender();
				}
				item->SetLock(true);
			}
		}
	}
}

CControlWZ* uiEquipmentAwakeningDialog::getControl( POSTYPE pos )
{
	assert( pos >= 0 && pos < kControl_Size );

	return GetControlWZ( wz_id_[pos] );
}

void uiEquipmentAwakeningDialog::UpdateTooltip()
{
	uiToolTipMan* ui_tooltip_manager = static_cast<uiToolTipMan*>(g_InterfaceManager.GetUserInterfaceManager( UIMAN_TOOLTIP ) );
	if ( m_MouseOverObject.eMouseOverType == eMOUSE_OVER_SLOT )
	{
		if ( ui_tooltip_manager )
		{
			assert( m_MouseOverObject.pSlot );
			ui_tooltip_manager->RegisterTooltipItem( m_MouseOverObject.pSlot, FALSE, kLimitStatCompositeNormal );
			ui_tooltip_manager->SetTooltipDialogID(GetDialogKey());
		}

		CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
	}
}

void uiEquipmentAwakeningDialog::InitEquipmentAwakeningSlotContainer()
{
	SCItemSlot* item_slot = NULL;
	if ( equip_item_pos_ != INVALID_SLOT_POS )
	{
		item_slot = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);
		if ( item_slot )
		{
			item_slot->SetLock(false);
		}
		equip_item_pos_ = INVALID_SLOT_POS;
	}

	if ( material_item_pos_ != INVALID_SLOT_POS )
	{
		item_slot = ItemManager::Instance()->GetInventoryItem(material_item_pos_);
		if ( item_slot )
		{
			item_slot->SetLock(false);
		}
		material_item_pos_ = INVALID_SLOT_POS;
	}

	if ( additive_item_pos_ != INVALID_SLOT_POS )
	{
		item_slot = ItemManager::Instance()->GetInventoryItem(additive_item_pos_);
		if ( item_slot )
		{
			item_slot->SetLock(false);
		}
		additive_item_pos_ = INVALID_SLOT_POS;
	}

	for ( int i = EQUIPMENT_EQUIP_SLOT; i < EQUIPMENT_AWAKENING_SLOT_MAX; ++i )
	{
		slot_container_[i].RemoveSlot(0);
	}
}

void uiEquipmentAwakeningDialog::InitFtextRender()
{
	equipment_awakening_first_render_.ClearSentence();
	equipment_awakening_second_render_.ClearSentence();
	material_first_render_.ClearSentence();
	material_second_render_.ClearSentence();
	additive_first_render_.ClearSentence();
	additive_second_render_.ClearSentence();
}

void uiEquipmentAwakeningDialog::UpdateFtextRenderRectSetting()
{
	RECT rc = {0, };
	CControlWZ* ctl = getControl(kControl_Picture_P001);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		equipment_awakening_first_render_.SetRect(rc);
	}
	ctl = getControl(kControl_Picture_P008);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		equipment_awakening_second_render_.SetRect(rc);
	}
	ctl = getControl(kControl_Picture_P009);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		material_first_render_.SetRect(rc);
	}
	ctl = getControl(kControl_Picture_P010);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		material_second_render_.SetRect(rc);
	}
	ctl = getControl(kControl_Picture_P017);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		additive_first_render_.SetRect(rc);
	}
	ctl = getControl(kControl_Picture_P020);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		additive_second_render_.SetRect(rc);
	}
}

void uiEquipmentAwakeningDialog::EquipmentInfoSetting()
{
	if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( equip_item_pos_ ) )
	{
		TCHAR buf[256] = {0, };

		// 아이템 이름
		g_InterfaceManager.GetItemName( item->GetCode(), buf, sizeof(buf) );

		TCHAR awakening_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR max_awakening_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR awakening_format_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR awakening_result_string[INTERFACE_STRING_LENGTH] = {0, };

		// 3988 : 각성레벨 (%s/%s)
		g_InterfaceManager.GetInterfaceString(3988, awakening_format_string);
		_itoa(item->GetAwakening(), awakening_string, 10);
		_itoa(MAX_AWAKENING_GRADE, max_awakening_string, 10);
		Snprintf(awakening_result_string, INTERFACE_STRING_LENGTH-1, awakening_format_string, awakening_string, max_awakening_string);

		TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
		Snprintf(result_string,
			INTERFACE_STRING_LENGTH-1,
			_T("<FONT=st10><COLOR=FFFFFFFF>%s </COLOR><COLOR=FFFF0000>%s</COLOR></FONT>"),
			buf, awakening_result_string);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			equipment_awakening_first_render_.ClearSentence();
			equipment_awakening_first_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}
		CControlWZ* ctl = getControl( kControl_Picture_P001 );
		CControlWZ* text_ctl = getControl( kControl_Text_T004 );
		if ( ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			equipment_awakening_first_render_.SetRect(rc);
		}
		ZeroMemory(result_string, INTERFACE_STRING_LENGTH-1);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			equipment_awakening_second_render_.ClearSentence();
			equipment_awakening_second_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}
		ctl = getControl( kControl_Picture_P008 );
		text_ctl = getControl( kControl_Text_S001 );
		if ( ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			equipment_awakening_second_render_.SetRect(rc);
		}

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
		AllRoundMaterialInfo temp_allround_material_info;

		if (AwakeningInfoParser::Instance()->GetAllRoundMaterialInfo_First(item->GetCode(), temp_allround_material_info) == false)
		{
			return;
		}

		SCItemSlot tempSlot;
		tempSlot.SetCode( (SLOTCODE)temp_allround_material_info.allround_material_code );
		tempSlot.SetNum( temp_allround_material_info.allround_material_count );
		slot_container_[EQUIPMENT_MATERIAL_SLOT].SetSlotItem(0, tempSlot, getControl(kControl_Button_B002) );
		slot_container_[EQUIPMENT_MATERIAL_SLOT].SetSlotRenderState_DeActive(0);
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	}
}

void uiEquipmentAwakeningDialog::MaterialInfoSetting()
{
	if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( material_item_pos_ ) )
	{
		TCHAR buf[256] = {0, };

		// 아이템 이름
		g_InterfaceManager.GetItemName( item->GetCode(), buf, sizeof(buf) );

		TCHAR awakening_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR max_awakening_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR awakening_format_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR awakening_result_string[INTERFACE_STRING_LENGTH] = {0, };

		const BASE_ITEMINFO* material_info = ItemInfoParser::Instance()->GetItemInfo(item->GetCode());

		// 만능 재료가 아니라는 것.
		if ( material_info && material_info->m_wType != eITEMTYPE_AWAKENING_ALLROUND_MATERIAL )
		{
			// 3988 : 각성레벨 (%s/%s)
			g_InterfaceManager.GetInterfaceString(3988, awakening_format_string);
			_itoa(item->GetAwakening(), awakening_string, 10);
			_itoa(MAX_AWAKENING_GRADE, max_awakening_string, 10);
			Snprintf(awakening_result_string, INTERFACE_STRING_LENGTH-1, awakening_format_string, awakening_string, max_awakening_string);
		}

		TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
		Snprintf(result_string,
			INTERFACE_STRING_LENGTH-1,
			_T("<FONT=st10><COLOR=FFFFFFFF>%s </COLOR><COLOR=FFFF0000>%s</COLOR></FONT>"),
			buf, awakening_result_string);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			material_first_render_.ClearSentence();
			material_first_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}

		SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem( equip_item_pos_ );
		const AwakeningInfo* awakening_info = AwakeningInfoParser::Instance()->FindData(equip_item->GetCode());
		if ( awakening_info == NULL )
		{
			return;
		}
		BYTE material_awakening = 0;
		if ( material_info && material_info->m_wType == eITEMTYPE_AWAKENING_ALLROUND_MATERIAL )
		{
			material_awakening = material_info->m_iOptionValue[0];
		}
		else
		{
			material_awakening = item->GetAwakening() + 1;
		}
		BYTE result_awakening_level = equip_item->GetAwakening() + material_awakening;
		BYTE additive_number = awakening_info->additive_count; // 첨가제 수량
		if ( result_awakening_level > MAX_AWAKENING_GRADE )
		{
			result_awakening_level = MAX_AWAKENING_GRADE;
		}
		if ( result_awakening_level > awakening_info->awake_permit_level )
		{
			result_awakening_level = awakening_info->awake_permit_level;
		}
		
		std::map<BYTE, DWORD>::const_iterator iter;
		iter = awakening_info->awakening_level_info_map.find(result_awakening_level);
		DWORD level_info = iter->second;
		DWORD option_param = level_info;

		_itoa(result_awakening_level, awakening_string, 10);
		TCHAR option_param_string[INTERFACE_STRING_LENGTH] = {0, };
		_itoa(option_param, option_param_string, 10);
		// 3985 : %s레벨 각성효과 : %s
		g_InterfaceManager.GetInterfaceString(3985, awakening_format_string);
		Snprintf(awakening_result_string, 
			     INTERFACE_STRING_LENGTH-1, 
				 awakening_format_string, 
				 awakening_string, 
				 option_param_string);

		Snprintf(result_string,
			INTERFACE_STRING_LENGTH-1,
			_T("<FONT=st10><COLOR=FFFF0000>%s</COLOR></FONT>"),
			awakening_result_string);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			equipment_awakening_second_render_.ClearSentence();
			equipment_awakening_second_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}
		CControlWZ* ctl = getControl( kControl_Picture_P009 );
		CControlWZ* text_ctl = getControl( kControl_Text_S004 );
		if ( ctl && text_ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			equipment_awakening_first_render_.SetRect(rc);
		}

		// 3984 : 재료로 사용 시 각성레벨 %s를 상승시켜 줍니다.
		g_InterfaceManager.GetInterfaceString(3984, awakening_format_string);
		_itoa(result_awakening_level - equip_item->GetAwakening(), awakening_string, 10);
		Snprintf(result_string, INTERFACE_STRING_LENGTH-1, awakening_format_string, awakening_string);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			material_second_render_.ClearSentence();
			material_second_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}

		// 3983 : 각성 첨가제는 각성레벨이 상승되는 수치만큼 필요합니다.
		g_InterfaceManager.GetInterfaceString(3983, result_string);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			additive_first_render_.ClearSentence();
			additive_first_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}

		ctl = getControl( kControl_Picture_P017 );
		text_ctl = getControl( kControl_Text_S017 );
		if ( ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			equipment_awakening_first_render_.SetRect(rc);
		}

		additive_second_render_.ClearSentence();		

		ctl = getControl( kControl_Picture_P020 );
		text_ctl = getControl( kControl_Text_S016 );
		if ( ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			equipment_awakening_second_render_.SetRect(rc);
		}

		SCItemSlot tempSlot;
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
		int material_num = AwakeningInfoParser::Instance()->GetAllRoundMaterialNum(awakening_info->item_code, item->GetCode());
		tempSlot.SetCode( (SLOTCODE)item->GetCode() );
		if (material_num != 0)
		{
			tempSlot.SetNum(material_num);
		}
		slot_container_[EQUIPMENT_MATERIAL_SLOT].SetSlotItem(0, tempSlot, getControl(kControl_Button_B002) );
		slot_container_[EQUIPMENT_MATERIAL_SLOT].SetSlotRenderState_Active(0);
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
		tempSlot.SetCode( (SLOTCODE)awakening_info->additive_item_code );
		tempSlot.SetNum( additive_number );
		slot_container_[EQUIPMENT_ADDITIVE_SLOT].SetSlotItem(0, tempSlot, getControl(kControl_Button_B003) );
		slot_container_[EQUIPMENT_ADDITIVE_SLOT].SetSlotRenderState_DeActive(0);

		ctl = getControl( kControl_Picture_P010 );
		text_ctl = getControl( kControl_Text_S007 );
		if ( ctl && text_ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			equipment_awakening_second_render_.SetRect(rc);
		}
	}
}

void uiEquipmentAwakeningDialog::AdditiveInfoSetting()
{
	if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( additive_item_pos_ ) )
	{
		CControlWZ* ctl = getControl( kControl_Picture_P017 );
		CControlWZ* text_ctl = getControl( kControl_Text_S017 );
		if ( ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			additive_first_render_.SetRect(rc);
		}

		additive_second_render_.ClearSentence();		
		
		ctl = getControl( kControl_Picture_P020 );
		text_ctl = getControl( kControl_Text_S016 );
		if ( ctl )
		{
			text_ctl->ShowWindowWZ(WZ_HIDE);
			RECT rc = ctl->GetSizeRect();
			rc.right = rc.left + rc.right;
			rc.bottom = rc.top + rc.bottom;
			additive_second_render_.SetRect(rc);
		}

		slot_container_[EQUIPMENT_ADDITIVE_SLOT].SetSlotRenderState_Active(0);
	}
}

void uiEquipmentAwakeningDialog::EquipmentAwakeningSlotClearSetting()
{
	SCItemSlot* item = NULL;
	
	item = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);
	

	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };				
	equipment_awakening_first_render_.ClearSentence();
	equipment_awakening_second_render_.ClearSentence();

	CControlWZ* text_ctl = getControl( kControl_Text_T004 );
	if ( text_ctl )
	{
		text_ctl->ShowWindowWZ(WZ_SHOW);
	}
	text_ctl = getControl( kControl_Text_S001 );
	if ( text_ctl )
	{
		text_ctl->ShowWindowWZ(WZ_SHOW);
	}
	equip_item_pos_ = INVALID_SLOT_POS;
	if ( item )
	{
		item->SetLock(false);
	}
	slot_container_[EQUIPMENT_EQUIP_SLOT].RemoveSlot(0);
}

void uiEquipmentAwakeningDialog::MaterialSlotClearSetting()
{	
	SCItemSlot* item = NULL;

	item = ItemManager::Instance()->GetInventoryItem(material_item_pos_);

	material_first_render_.ClearSentence();
	material_second_render_.ClearSentence();
	equipment_awakening_second_render_.ClearSentence();
	
	CControlWZ* text_ctl = getControl( kControl_Text_S004 );
	if ( text_ctl )
	{
		text_ctl->ShowWindowWZ(WZ_SHOW);
	}
	text_ctl = getControl( kControl_Text_S007 );
	if ( text_ctl )
	{
		text_ctl->ShowWindowWZ(WZ_SHOW);
	}
	material_item_pos_ = INVALID_SLOT_POS;
	if ( item )
	{
		item->SetLock(false);
	}
	slot_container_[EQUIPMENT_MATERIAL_SLOT].RemoveSlot(0);

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	if (equip_item_pos_ != INVALID_SLOT_POS)
	{
		SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( equip_item_pos_ );
		if( item )
		{
			AllRoundMaterialInfo material_info;
			if (AwakeningInfoParser::Instance()->GetAllRoundMaterialInfo_First(item->GetCode(), material_info) == true)
			{
				SCItemSlot tempSlot;
				tempSlot.SetCode((SLOTCODE)material_info.allround_material_code);
				tempSlot.SetNum(material_info.allround_material_count);
				
				slot_container_[EQUIPMENT_MATERIAL_SLOT].SetSlotItem(0, tempSlot, getControl(kControl_Button_B002) );
				slot_container_[EQUIPMENT_MATERIAL_SLOT].SetSlotRenderState_DeActive(0);
			}
		}
	}
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
}

void uiEquipmentAwakeningDialog::AdditiveSlotClearSetting()
{	
	SCItemSlot* item = NULL;

	item = ItemManager::Instance()->GetInventoryItem(additive_item_pos_);

	if ( material_item_pos_ == INVALID_SLOT_POS )
	{
		additive_first_render_.ClearSentence();
		additive_second_render_.ClearSentence();
		CControlWZ* text_ctl = getControl( kControl_Text_S017 );
		if ( text_ctl )
		{
			text_ctl->ShowWindowWZ(WZ_SHOW);
		}
		text_ctl = getControl( kControl_Text_S016 );
		if ( text_ctl )
		{
			text_ctl->ShowWindowWZ(WZ_SHOW);
		}
	}
	else
	{
		TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
		// 3983 : 각성 첨가제는 각성레벨이 상승되는 수치만큼 필요합니다.
		g_InterfaceManager.GetInterfaceString(3983, result_string);
		if ( equipment_awakening_ftext_parser_.Parser(result_string, equipment_awakening_ftext_sentence_))
		{
			additive_first_render_.ClearSentence();
			additive_first_render_.Append(equipment_awakening_ftext_sentence_);
			equipment_awakening_ftext_sentence_.Release();
		}
	}
	additive_item_pos_ = INVALID_SLOT_POS;
	if ( material_item_pos_ == INVALID_SLOT_POS )
	{
		slot_container_[EQUIPMENT_ADDITIVE_SLOT].RemoveSlot(0);
	}
	else
	{
		slot_container_[EQUIPMENT_ADDITIVE_SLOT].SetSlotRenderState_DeActive(0);
	}
	if ( item )
	{
		item->SetLock(false);
	}
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	slot_container_[EQUIPMENT_ADDITIVE_SLOT].RemoveSlot(0);
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
}

bool uiEquipmentAwakeningDialog::PermitEquipmentCheck( const SCItemSlot* item )
{
	if ( item == NULL )
	{
		return false;
	}

	RC::eITEM_RESULT result_code = AwakeningInfoParser::Instance()->PermitEquipmentCheck(*item);

	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		ResultCodeProc( result_code );
		return false;
	}

	return true;
}

bool uiEquipmentAwakeningDialog::PermitMaterialCheck( const SCItemSlot* item )
{
	if ( item == NULL )
	{
		return false;
	}

	SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem( equip_item_pos_ );
	if ( equip_item == NULL )
	{
		return false;
	}

#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	RC::eITEM_RESULT result_code = AwakeningInfoParser::Instance()->PermitMaterialCheck(equip_item->GetCode(), item->GetCode(), item->GetNum());
#else
	RC::eITEM_RESULT result_code = AwakeningInfoParser::Instance()->PermitMaterialCheck(equip_item->GetCode(), item->GetCode());
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		ResultCodeProc( result_code );
		return false;
	}

	return true;
}

bool uiEquipmentAwakeningDialog::PermitAdditiveCheck( const SCItemSlot* item )
{
	if ( item == NULL )
	{
		return false;
	}

	SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem( equip_item_pos_ );
	if ( equip_item == NULL )
	{
		return false;
	}

	SCItemSlot* material_item = ItemManager::Instance()->GetInventoryItem( material_item_pos_ );
	if ( material_item == NULL )
	{
		return false;
	}

	RC::eITEM_RESULT result_code = AwakeningInfoParser::Instance()->PermitAdditiveCheck(*equip_item, *material_item, *item);
	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		ResultCodeProc( result_code );
		return false;
	}

	return true;
}

void uiEquipmentAwakeningDialog::ResultCodeProc( const RC::eITEM_RESULT& result_code )
{
	TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
	int string_index = 0;
	switch ( result_code )
	{
	case RC::RC_EQUIPMENT_AWAKENING_SUCCESS:
		{
			// 5961 : 각성에 성공하였습니다.
			string_index = 5961;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_FAIL:
		{
			// 5962 : 각성에 실패하였습니다.
			string_index = 5962;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_EQUIP_EXCEEDED_PERMIT_LEVEL:
		{
			// 5967 : 제한 각성 레벨에 도달한 아이템 입니다.
			string_index = 5967;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_EQUIP_MAX_LEVEL:
		{
			// 5966 : 해당 아이템은 이미 각성 최고레벨입니다.
			string_index = 5966;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE:
		{
			// 3959 : 첨가제 아이템이 맞지 않습니다.
			string_index = 3959;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_ADDITIVE_AMOUNT:
		{
			// 3960 : 수량이 맞지 않습니다.
			string_index = 3960;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL:
		{
			// 5985 : 재료로 사용 할 수 없는 아이템입니다.
			string_index = 5985;
		}
		break;
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	case RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL_AMOUNT:
		{
			// 3958 : 재료의 수량을 확인해 주세요.
			string_index = 3958;
		}
		break;
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	case RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO:
		{
			// 3961 : 장비 정보를 알 수 없습니다.
			string_index = 3961;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_INVALID_MATERIAL_INFO:
		{
			// 3962 : 재료 정보를 알 수 없습니다.
			string_index = 3962;
		}
		break;
	case RC::RC_EQUIPMENT_AWAKENING_INVALID_AWAKENING_INFO:
		{
			// 3966 : 각성 정보를 알 수 없습니다.
			string_index = 3966;
		}
		break;
	}

	if ( string_index > 0 )
	{
		g_InterfaceManager.GetInterfaceString(string_index, temp_string);
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, temp_string);
	}
}

void uiEquipmentAwakeningDialog::Callback_SendPacketItemAwakeningSyn( uiSystemMessage::ButtonIndex button_index )
{
	uiEquipmentAwakeningDialog* dialog = GET_CAST_DIALOG(uiEquipmentAwakeningDialog, IM_EQUIPMENT_AWAKENING_MANAGER::EQUIPMENT_AWAKENING_DIALOG);
	if ( dialog )
	{
		dialog->SendPacketItemAwakeningSyn(button_index == uiSystemMessage::kConfirm_Button);
	}
}

void uiEquipmentAwakeningDialog::SendPacketItemAwakeningSyn( bool is_send_packet )
{
	if ( is_send_packet == false )
	{
		return;
	}

	if (!BIT_CHECK(m_packetStatus, PACKET_STATUS_AWAKENING))
	{
		MSG_CG_ITEM_AWAKENING_SYN packet;
		packet.equip_item_pos = equip_item_pos_;
		packet.material_item_pos = material_item_pos_;
		packet.additive_item_pos = additive_item_pos_;

		BOOL ret = GlobalFunc::SendPacket( CI_GAMESERVERIDX, &packet, sizeof ( packet ) );

		if ( ret )
		{
			BIT_ADD( m_packetStatus, PACKET_STATUS_AWAKENING );
		}
	}
}

void uiEquipmentAwakeningDialog::RemoveBitCheck()
{
	if ( BIT_CHECK(m_packetStatus, PACKET_STATUS_AWAKENING) )
	{
		BIT_REMOVE( m_packetStatus, PACKET_STATUS_AWAKENING );
	}
}

void uiEquipmentAwakeningDialog::Awakening()
{
	if ( equip_item_pos_ == INVALID_SLOT_POS || 
		 material_item_pos_ == INVALID_SLOT_POS || 
		 additive_item_pos_ == INVALID_SLOT_POS )
	{
		// 중앙 메시지 출력
		// 5987 : 각성 조건에 만족하지 못하여 각성을 진행 할 수 없습니다.
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(5987));
		return;
	}

	SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);
	SCItemSlot* material_item = ItemManager::Instance()->GetInventoryItem(material_item_pos_);
	const AwakeningInfo* equip_info = AwakeningInfoParser::Instance()->FindData(equip_item->GetCode());
	const BASE_ITEMINFO* material_info = ItemInfoParser::Instance()->GetItemInfo(material_item->GetCode());

	if ( equip_item == NULL || 
		 material_item == NULL || 
		 equip_info == NULL || 
		 material_info == NULL )
	{
		// 중앙 메시지 출력
		// 5987 : 각성 조건에 만족하지 못하여 각성을 진행 할 수 없습니다.
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(5987));
		return;
	}

	BYTE material_awakening = 0;
	if ( material_info && material_info->m_wType == eITEMTYPE_AWAKENING_ALLROUND_MATERIAL )
	{
		material_awakening = material_info->m_iOptionValue[0];
	}
	else
	{
		material_awakening = material_item->GetAwakening() + 1;
	}
	BYTE result_awakening_level = equip_item->GetAwakening() + material_awakening;
	if ( result_awakening_level > MAX_AWAKENING_GRADE )
	{
		result_awakening_level = MAX_AWAKENING_GRADE;
	}
	if ( result_awakening_level > equip_info->awake_permit_level )
	{
		result_awakening_level = equip_info->awake_permit_level;
	}

	TCHAR level_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	_itoa(result_awakening_level, level_string, 10);
	TCHAR item_name[64] = {0, };
	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };

	// 3963 : %s를 %s레벨로 각성하시겠습니까?
	g_InterfaceManager.GetInterfaceString(3963, format_string);

	// 아이템 이름
	g_InterfaceManager.GetItemName( equip_item->GetCode(), item_name );
	Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, item_name, level_string);
	GlobalFunc::ConfirmMessage(result_string, 0, Callback_SendPacketItemAwakeningSyn);
}

void uiEquipmentAwakeningDialog::ResetAwakeningDialog()
{
	RemoveBitCheck();
	InitEquipmentAwakeningSlotContainer();
	InitFtextRender();
	EquipmentAwakeningSlotClearSetting();
	MaterialSlotClearSetting();
	AdditiveSlotClearSetting();

	InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));
	if (inventory_dialog_ptr)
	{
		inventory_dialog_ptr->RevertAwakeningItemIconDeActiveRender();
		inventory_dialog_ptr->ImpossibleAwakeningItemIonDeActiveRender();
	}
}

void uiEquipmentAwakeningDialog::ResultCodeSoundProc( const RC::eITEM_RESULT& result_code )
{
	eUI_SOUND_INDEX index = UI_SOUND_DEFAULT_CLICK;

	if ( result_code == RC::RC_EQUIPMENT_AWAKENING_SUCCESS )
	{
		index = UI_SOUND_COMPOSE_ENCHANT_SUCCESS;
	}
	else if ( result_code != 0 )
	{
		index = UI_SOUND_COMPOSE_ENCHANT_FAIL;
	}

	if ( index != UI_SOUND_DEFAULT_CLICK )
	{
		// 사운드 출력
		g_SoundEffect.PlayUI( index );
	}
}

void uiEquipmentAwakeningDialog::OpenTutorialGuid()
{
	uiGuideSystemMan* ui_guide_system_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
	if ( ui_guide_system_manager )
	{
		ui_guide_system_manager->OpenGuideInfoDialog();
		ui_guide_system_manager->OpenGuideDialog(uiGuideSystemMan::kDialogType_Information, 6008);
	}
}

uiEquipmentAwakeningDialog::AWAKENING_MODE uiEquipmentAwakeningDialog::GetAwakeningUIMode()
{
	if ( equip_item_pos_ == INVALID_SLOT_POS )
	{
		return AWAKENING_MODE_EQUIPMENT;
	}

	if ( material_item_pos_ == INVALID_SLOT_POS )
	{
		return AWAKENING_MODE_MATERIAL;
	}

	if ( additive_item_pos_ == INVALID_SLOT_POS )
	{
		return AWAKENING_MODE_ADDITIVE;
	}

	return AWAKENING_MODE_COMPLETE;
}

#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
