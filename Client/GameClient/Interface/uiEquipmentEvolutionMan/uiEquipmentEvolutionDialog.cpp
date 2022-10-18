#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionDialog.h"
#include "uiEquipmentEvolutionMan.h"
#include "GlobalFunc.h"
#include "SCItemSlot.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "CursorChangeTransact.h"
#include "Mouse.h"
#include "MouseHandler.h"
#include "SoundEffect.h"
#include "ItemManager.h"
#include "AwakeningInfoParser.h"
#include "GlobalFunc.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"

WzID uiEquipmentEvolutionDialog::wz_id_[kControl_Size] = 
{
	StrToWzID("B002"), 
	StrToWzID("B000"), 
	StrToWzID("B003"), 

	StrToWzID("B999"), 

	StrToWzID("B005"),
	StrToWzID("B006"),

	StrToWzID("S008"),
	StrToWzID("S002"),
	StrToWzID("S003"),

	StrToWzID("B001"),
};

uiEquipmentEvolutionDialog::uiEquipmentEvolutionDialog(InterfaceManager* ui_manager) : 
uiBase( ui_manager ),
equip_item_pos_(INVALID_SLOT_POS),
additive_item_pos_(INVALID_SLOT_POS),
prediction_item_pos_(INVALID_SLOT_POS)
{
}

uiEquipmentEvolutionDialog::~uiEquipmentEvolutionDialog(void)
{
}

void uiEquipmentEvolutionDialog::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
	RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
	RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
	RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
	RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
	RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
	RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
	RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
	RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
	RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);

	slot_container_[EQUIPMENT_EVOLUTION_SLOT].Initialize(1, SI_EQUIPMENT_EVOLUTION);
	slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].Initialize(1, SI_EQUIPMENT_EVOLUTION);
	slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].Initialize(1, SI_EQUIPMENT_EVOLUTION);

	m_packetStatus = PACKET_STATUS_NONE;

	CCtrlStaticWZ* static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S008)));
	if ( static_control_ptr )
	{
		static_control_ptr->SetTextWZ(_T(""));
	}
	static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S002)));
	if ( static_control_ptr )
	{
		static_control_ptr->SetTextWZ(_T(""));
	}
	static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S003)));
	if ( static_control_ptr )
	{
		static_control_ptr->SetTextWZ(_T(""));
	}

	RECT rc = {0, };
	if ( draw_base )
	{
		for ( int i = 0; i < 3; ++i )
		{
			description_ftext_render_[i].Init(draw_base, rc);
		}
	}

	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	g_InterfaceManager.GetInterfaceString(3972, format_string);
	if ( description_ftext_parser_.Parser(format_string, description_ftext_sentence_) )
	{
		description_ftext_render_[0].ClearSentence();
		description_ftext_render_[0].Append(description_ftext_sentence_);
		description_ftext_sentence_.Release();
	}

	g_InterfaceManager.GetInterfaceString(3974, format_string);
	if ( description_ftext_parser_.Parser(format_string, description_ftext_sentence_) )
	{
		description_ftext_render_[1].ClearSentence();
		description_ftext_render_[1].Append(description_ftext_sentence_);
		description_ftext_sentence_.Release();
	}

	g_InterfaceManager.GetInterfaceString(3975, format_string);
	if ( description_ftext_parser_.Parser(format_string, description_ftext_sentence_) )
	{
		description_ftext_render_[2].ClearSentence();
		description_ftext_render_[2].Append(description_ftext_sentence_);
		description_ftext_sentence_.Release();
	}
}

void uiEquipmentEvolutionDialog::Release()
{
	for ( int i = EQUIPMENT_EVOLUTION_SLOT; i < EQUIPMENT_EVOLUTION_SLOT_MAX; ++i )
	{
		slot_container_[i].Release();
	}
}

void uiEquipmentEvolutionDialog::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_EQUIP, TRUE );

		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg));
		msg.dwType = eDoType_Equipment_Evolution_Dialog;
		msg.wParam = IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		InitEquipmentEvolutionSlotContainer();

		EvolutionResultClear();

		g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_EQUIP, FALSE );

		g_KeyQueueManager.DeleteMsg(eDoType_Equipment_Evolution_Dialog);

		for ( int i = EQUIPMENT_EVOLUTION_SLOT; i < EQUIPMENT_EVOLUTION_SLOT_MAX; ++i )
		{
			slot_container_[i].Clear();
		}

		m_packetStatus = PACKET_STATUS_NONE;
	}
}

void uiEquipmentEvolutionDialog::MessageProc( SI_MESSAGE* message )
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

void uiEquipmentEvolutionDialog::NetworkProc( MSG_BASE* recv_packet )
{

}

void uiEquipmentEvolutionDialog::OnUpdateSolarDialog()
{
	UpdateFtextRenderRectSetting();

	for ( int i = EQUIPMENT_EVOLUTION_SLOT; i < EQUIPMENT_EVOLUTION_SLOT_MAX; ++i )
	{
		slot_container_[i].Update();
	}

	UpdateTooltip();
}

void uiEquipmentEvolutionDialog::OnRenderSolarDialog()
{
	for ( int i = 0; i < 3; ++i )
	{
		description_ftext_render_[i].Render();
	}

	for ( int i = EQUIPMENT_EVOLUTION_SLOT; i < EQUIPMENT_EVOLUTION_SLOT_MAX; ++i )
	{
		slot_container_[i].Render();
	}
}

bool uiEquipmentEvolutionDialog::OnLButtonClick( SI_MESSAGE* message )
{
	if ( BIT_CHECK(m_packetStatus, PACKET_STATUS_EVOLUTION) )
	{
		return false;
	}

	DWORD control_position = GetControlPosition_byID( message->dwCtrlID );
	switch ( control_position )
	{
	case kControl_Button_B002:
	case kControl_Button_B000:
		{
			if ( MouseHandler::Instance()->IsExistItemAtHand() )
			{
				MouseHandler::MouseItem* mouse_item = MouseHandler::Instance()->GetMouseItem();

				if ( mouse_item )
				{
					OnEquipmentEvolutionItem_Click( (POSTYPE)control_position, mouse_item->m_fromSlotIdx, mouse_item->m_fromPos );
				}

				MouseHandler::Instance()->RollbackItemAtHand();
			}
		}
		break;

	case kControl_Button_B003:
		{
			if ( MouseHandler::Instance()->IsExistItemAtHand() )
			{
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
			Evolution();
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

bool uiEquipmentEvolutionDialog::OnRButtonClick( SI_MESSAGE* message )
{
	if ( BIT_CHECK(m_packetStatus, PACKET_STATUS_EVOLUTION) )
	{
		return false;
	}

	DWORD control_position = GetControlPosition_byID( message->dwCtrlID );
	switch ( control_position )
	{
	case kControl_Button_B002:
		{
			EvolutionResultClear();
			InitEquipmentEvolutionSlotContainer();
		}
		break;
	case kControl_Button_B000:
		{
			DeActiveAdditiveSlot();
			DeActivePredictionSlot();
			if (SCItemSlot* item = ItemManager::Instance()->GetInventoryItem(additive_item_pos_))
			{
				item->SetLock(false);
			}
			additive_item_pos_ = INVALID_SLOT_POS;
		}
		break;
	case kControl_Button_B003:
		{
			// no operator
		}
		break;
	}

	return true;
}

bool uiEquipmentEvolutionDialog::OnMouseOver( SI_MESSAGE* message )
{
	m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

	int mx = Mouse::Instance()->GetMouseX();
	int my = Mouse::Instance()->GetMouseY();

	for ( POSTYPE pos = kControl_Button_B002; pos <= kControl_Button_B003; ++pos )
	{
		if ( CControlWZ* ctl = getControl( pos ) )
		{
			RECT rc = ctl->GetSizeRect();

			if ( rc.left <= mx && rc.right >= mx && rc.top <= my && rc.bottom >= my )
			{
				int slotPos = ( pos - kControl_Button_B002 );

				if ( SCItemSlot* item = slot_container_[pos].GetSlotItem(0) )
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = item;
				}
			}
		}
	}

	return true;
}

CControlWZ* uiEquipmentEvolutionDialog::getControl( POSTYPE pos )
{
	assert( pos >= 0 && pos < kControl_Size );

	return GetControlWZ( wz_id_[pos] );
}

void uiEquipmentEvolutionDialog::UpdateTooltip()
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

void uiEquipmentEvolutionDialog::OnEquipmentEvolutionItem_Click( const POSTYPE& equipment_evolution_slot_index, const SLOTIDX from_container_index, const POSTYPE from_container_position )
{
	bool result = false;
	if ( MouseHandler::Instance()->ItemEquipmentAwakening_InventoryRightClick(
		from_container_index, SI_EQUIPMENT_AWAKENING, from_container_position, 0) )
	{
		if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( from_container_position ) )
		{
			switch ( equipment_evolution_slot_index )
			{
			case EQUIPMENT_EVOLUTION_SLOT:
				{
					if ( PermitEquipmentCheck(item) == true )
					{
						if ( equip_item_pos_ != INVALID_SLOT_POS )
						{
							SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem( equip_item_pos_ );
							if ( equip_item )
							{
								equip_item->SetLock(false);
							}
							EvolutionResultClear();
							InitEquipmentEvolutionSlotContainer();
						}

						equip_item_pos_ = from_container_position;
						EvolutionResultSetting();
						result = true;
					}
				}
				break;
			case EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT:
				{
					if ( equip_item_pos_ != INVALID_SLOT_POS )
					{
						if ( PermitAdditiveCheck(item) == true )
						{
							if ( additive_item_pos_ != INVALID_SLOT_POS )
							{
								SCItemSlot* additive_item = ItemManager::Instance()->GetInventoryItem( additive_item_pos_ );
								if ( additive_item )
								{
									additive_item->SetLock(false);
								}
							}

							additive_item_pos_ = from_container_position;
							ActiveAdditiveSlot();
							ActivePredictionSlot();
							result = true;
						}
					}
					else
					{
						// 중앙 메시지 출력
						// 3070 : 진화 할 아이템을 먼저 등록해 주세요
						GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(3070));
						return;
					}
				}
				break;
			}

			if ( result == true )
			{
				item->SetLock(true);
			}			
		}
	}
}

void uiEquipmentEvolutionDialog::InitEquipmentEvolutionSlotContainer()
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

	if ( additive_item_pos_ != INVALID_SLOT_POS )
	{
		item_slot = ItemManager::Instance()->GetInventoryItem(additive_item_pos_);
		if ( item_slot )
		{
			item_slot->SetLock(false);
		}
		additive_item_pos_ = INVALID_SLOT_POS;
	}

	if ( prediction_item_pos_ != INVALID_SLOT_POS )
	{
		item_slot = ItemManager::Instance()->GetInventoryItem(prediction_item_pos_);
		if ( item_slot )
		{
			item_slot->SetLock(false);
		}
		prediction_item_pos_ = INVALID_SLOT_POS;
	}

	for ( int i = EQUIPMENT_EVOLUTION_SLOT; i < EQUIPMENT_EVOLUTION_SLOT_MAX; ++i )
	{
		slot_container_[i].RemoveSlot(0);
	}
}

void uiEquipmentEvolutionDialog::OnEquipmentEvolutionItem_InventoryRightClick( const SLOTIDX from_container_index, const POSTYPE from_container_position )
{
	POSTYPE equipment_evolution_slot_index = INVALID_SLOT_POS;

	if ( equip_item_pos_ == INVALID_SLOT_POS )
	{
		equipment_evolution_slot_index = EQUIPMENT_EVOLUTION_SLOT;
	}
	else if ( additive_item_pos_ == INVALID_SLOT_POS )
	{
		equipment_evolution_slot_index = EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT;
	}

	if ( equipment_evolution_slot_index == INVALID_SLOT_POS )
	{
		return;
	}

	bool result = false;
	if ( MouseHandler::Instance()->ItemEquipmentEvolution_InventoryRightClick(
		from_container_index, SI_EQUIPMENT_EVOLUTION, from_container_position, 0) )
	{
		if ( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( from_container_position ) )
		{
			switch ( equipment_evolution_slot_index )
			{
			case EQUIPMENT_EVOLUTION_SLOT:
				{
					if ( PermitEquipmentCheck(item) == true )
					{
						equip_item_pos_ = from_container_position;
						EvolutionResultSetting();
						result = true;
					}
				}
				break;
			case EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT:
				{
					if ( PermitAdditiveCheck(item) == true )
					{
						additive_item_pos_ = from_container_position;
						ActiveAdditiveSlot();
						ActivePredictionSlot();
						result = true;
					}
				}
				break;
			}

			if ( result == true )
			{
				item->SetLock(true);
			}
		}
	}
}

void uiEquipmentEvolutionDialog::EvolutionResultSetting()
{
	if ( equip_item_pos_ == INVALID_SLOT_POS )
	{
		return;
	}

	SCItemSlot* item = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);
	if ( item == NULL )
	{
		return;
	}

	// 장비 슬롯 셋팅
	slot_container_[EQUIPMENT_EVOLUTION_SLOT].SetSlotItem(0, *item, getControl(kControl_Button_B002));

	// 첨가제 슬롯 셋팅
	CODETYPE additive_item_code = AwakeningInfoParser::Instance()->GetEvolutionAdditiveItemCode(item->GetCode());
	SCItemSlot tempSlot;
	tempSlot.SetCode((SLOTCODE)additive_item_code);
	BYTE additive_item_count = AwakeningInfoParser::Instance()->GetEvolutionAdditiveCount(item->GetCode());
	tempSlot.SetNum(additive_item_count);
	slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].SetSlotItem(0, tempSlot, getControl(kControl_Button_B000));
	slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].SetSlotRenderState_DeActive(0);

	// 결과 장비 슬롯 셋팅
	tempSlot.SetNum(1);
	CODETYPE prediction_item_code = AwakeningInfoParser::Instance()->GetResultEvolutionItemCode(item->GetCode());
	tempSlot.SetCode((SLOTCODE)prediction_item_code);
	slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].SetSlotItem(0, tempSlot, getControl(kControl_Button_B003));
	slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].SetSlotRenderState_DeActive(0);
}

void uiEquipmentEvolutionDialog::EvolutionResultClear()
{
	for ( int i = EQUIPMENT_EVOLUTION_SLOT; i < EQUIPMENT_EVOLUTION_SLOT_MAX; ++i )
	{
		slot_container_[i].RemoveSlot(0);
	}
}

void uiEquipmentEvolutionDialog::ActiveAdditiveSlot()
{
	if ( slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].GetSlotItem(0) != NULL )
	{
		slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].SetSlotRenderState_Active(0);
	}
}

void uiEquipmentEvolutionDialog::DeActiveAdditiveSlot()
{
	if ( slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].GetSlotItem(0) != NULL )
	{
		slot_container_[EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT].SetSlotRenderState_DeActive(0);
	}
}

void uiEquipmentEvolutionDialog::ActivePredictionSlot()
{
	if ( slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].GetSlotItem(0) != NULL )
	{
		slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].SetSlotRenderState_Active(0);
	}
}

void uiEquipmentEvolutionDialog::DeActivePredictionSlot()
{
	if ( slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].GetSlotItem(0) != NULL )
	{
		slot_container_[EQUIPMENT_EVOLUTION_PREDICTION_SLOT].SetSlotRenderState_DeActive(0);
	}
}

bool uiEquipmentEvolutionDialog::PermitEquipmentCheck( const SCItemSlot* item )
{
	if ( item == NULL )
	{
		return false;
	}

	RC::eITEM_RESULT result_code = AwakeningInfoParser::Instance()->PermitEvolutionEquipmentCheck(*item);

	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		ResultCodeProc( result_code );
		return false;
	}

	return true;
}

bool uiEquipmentEvolutionDialog::PermitAdditiveCheck( const SCItemSlot* item )
{
	if ( item == NULL )
	{
		return false;
	}

	SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);

	if ( equip_item == NULL )
	{
		return false;
	}

	RC::eITEM_RESULT result_code = AwakeningInfoParser::Instance()->PermitEvolutionAdditiveCheck(*equip_item, *item);

	if ( result_code != RC::RC_ITEM_SUCCESS )
	{
		ResultCodeProc( result_code );
		return false;
	}

	return true;
}

void uiEquipmentEvolutionDialog::ResultCodeProc( const RC::eITEM_RESULT& result_code )
{
	TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
	int string_index = 0;
	switch ( result_code )
	{
	case RC::RC_EQUIPMENT_EVOLUTION_SUCCESS:
		{
			// 5963 : 진화에 성공하였습니다.
			string_index = 5963;
		}
		break;
	case RC::RC_EQUIPMENT_EVOLUTION_FAIL:
		{
			// 5964 : 진화에 실패하였습니다.
			string_index = 5964;
		}
		break;
	case RC::RC_EQUIPMENT_EVOLUTION_INVALID_EQUIP_INFO:
		{
			// 3961 : 장비 정보를 알 수 없습니다.
			string_index = 3961;
		}
		break;
	case RC::RC_EQUIPMENT_EVOLUTION_ITEM_CAN_NOT_EVOLUTION:
		{
			// 5959 : 진화 할 수 없는 장비입니다.
			string_index = 5959;
		}
		break;
	case RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE:
		{
			// 5960 : 첨가제 아이템을 확인해 주세요
			string_index = 5960;
		}
		break;
	case RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_ADDITIVE_AMOUT:
		{
			// 3960 : 수량이 맞지 않습니다.
			string_index = 3960;
		}
		break;
	case RC::RC_EQUIPMENT_EVOLUTION_DOES_NOT_MATCH_AWAKENING_GRADE:
		{
			// 5968 : 각성 최고레벨에 도달하지 않아 진화 할 수 없습니다.
			string_index = 5968;
		}
		break;
	}

	if ( string_index > 0 )
	{
		g_InterfaceManager.GetInterfaceString(string_index, temp_string);
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, temp_string);
	}
}

void uiEquipmentEvolutionDialog::Evolution()
{
	if ( equip_item_pos_ == INVALID_SLOT_POS || 
		 additive_item_pos_ == INVALID_SLOT_POS )
	{
		// 중앙 메시지 출력
		// 5988 : 진화 조건에 만족하지 못하여 진화를 진행 할 수 없습니다.
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, g_InterfaceManager.GetInterfaceString(5988));
		return;
	}

	SCItemSlot* equip_item = ItemManager::Instance()->GetInventoryItem(equip_item_pos_);
	if ( equip_item == NULL )
	{
		return;
	}
	CODETYPE prediction_item_code = AwakeningInfoParser::Instance()->GetResultEvolutionItemCode(equip_item->GetCode());

	TCHAR equip_name_string[64] = {0, };
	TCHAR prediction_name_string[64] = {0, };
	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };

	g_InterfaceManager.GetItemName( equip_item->GetCode(), equip_name_string );
	g_InterfaceManager.GetItemName( prediction_item_code, prediction_name_string );

	// 3969 : %s를 %s로 진화하시겠습니까?
	g_InterfaceManager.GetInterfaceString(3969, format_string);
	Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, equip_name_string, prediction_name_string);
	GlobalFunc::ConfirmMessage(result_string, 0, Callback_SendPacketItemEvolutionSyn);
}

void uiEquipmentEvolutionDialog::Callback_SendPacketItemEvolutionSyn( uiSystemMessage::ButtonIndex button_index )
{
	uiEquipmentEvolutionDialog* dialog = GET_CAST_DIALOG(uiEquipmentEvolutionDialog, IM_EQUIPMENT_EVOLUTION_MANAGER::EQUIPMENT_EVOLUTION_DIALOG);
	if ( dialog )
	{
		dialog->SendPacketItemEvolutionSyn(button_index == uiSystemMessage::kConfirm_Button);
	}
}

void uiEquipmentEvolutionDialog::SendPacketItemEvolutionSyn( const bool& is_send_packet )
{
	if ( is_send_packet == false )
	{
		return;
	}

	if (!BIT_CHECK(m_packetStatus, PACKET_STATUS_EVOLUTION))
	{
		MSG_CG_ITEM_EVOLUTION_SYN packet;
		packet.equip_item_pos = equip_item_pos_;
		packet.additive_item_pos = additive_item_pos_;

		BOOL ret = GlobalFunc::SendPacket( CI_GAMESERVERIDX, &packet, sizeof(packet) );

		if ( ret )
		{
			BIT_ADD( m_packetStatus, PACKET_STATUS_EVOLUTION );
		}
	}
}

void uiEquipmentEvolutionDialog::RemoveBitCheck()
{
	if ( BIT_CHECK(m_packetStatus, PACKET_STATUS_EVOLUTION) )
	{
		BIT_REMOVE( m_packetStatus, PACKET_STATUS_EVOLUTION );
	}
}

void uiEquipmentEvolutionDialog::ResetEvolutionDialog()
{
	InitEquipmentEvolutionSlotContainer();

	EvolutionResultClear();

	RemoveBitCheck();
}

void uiEquipmentEvolutionDialog::UpdateFtextRenderRectSetting()
{
	RECT rc = {0, };
	CControlWZ* ctl = getControl(kControl_Text_S008);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		description_ftext_render_[0].SetRect(rc);
	}
	ctl = getControl(kControl_Text_S002);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		description_ftext_render_[1].SetRect(rc);
	}
	ctl = getControl(kControl_Text_S003);
	if ( ctl )
	{
		rc = ctl->GetSizeRect();
		description_ftext_render_[2].SetRect(rc);
	}
}

void uiEquipmentEvolutionDialog::ResultCodeSoundProc( const RC::eITEM_RESULT& result_code )
{
	eUI_SOUND_INDEX index = UI_SOUND_DEFAULT_CLICK;

	if ( result_code == RC::RC_EQUIPMENT_EVOLUTION_SUCCESS )
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

void uiEquipmentEvolutionDialog::OpenTutorialGuid()
{
	uiGuideSystemMan* ui_guide_system_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
	if ( ui_guide_system_manager )
	{
		ui_guide_system_manager->OpenGuideInfoDialog();
		ui_guide_system_manager->OpenGuideDialog(uiGuideSystemMan::kDialogType_Information, 6009);
	}
}

#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
