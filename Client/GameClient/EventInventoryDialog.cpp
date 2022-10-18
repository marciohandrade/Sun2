#include "SunClientPrecompiledHeader.h"
#include "eventinventorydialog.h"
#include "ItemUnitRenderer.h"
#include "SCItemSlot.h"
#include "slotkeygenerator.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "ItemManager.h"
#include "MouseCursor.h"
#include "CursorChangeTransact.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "InventoryDialog.h"

//----------------------------------------------------------------------------
/**
*/
WzID EventInventoryDialog::m_wzId[] = 
{
    StrToWzID("BT00"),//EVENT_BT_SLOT_01,
	StrToWzID("B001"),//EVENT_BT_SLOT_02, 
	StrToWzID("B002"),//EVENT_BT_SLOT_03, 
	StrToWzID("B003"),//EVENT_BT_SLOT_04, 
	StrToWzID("B004"),//EVENT_BT_SLOT_05, 
	StrToWzID("B005"),//EVENT_BT_SLOT_06, 
	StrToWzID("B006"),//EVENT_BT_SLOT_07, 
	StrToWzID("B007"),//EVENT_BT_SLOT_08, 
	StrToWzID("B008"),//EVENT_BT_SLOT_09, 
	StrToWzID("B009"),//EVENT_BT_SLOT_10, 
	StrToWzID("B010"),//EVENT_BT_SLOT_11, 
	StrToWzID("B011"),//EVENT_BT_SLOT_12, 
	StrToWzID("B012"),//EVENT_BT_SLOT_13, 
	StrToWzID("B013"),//EVENT_BT_SLOT_14, 
	StrToWzID("B034"),//EVENT_BT_SLOT_15, 
	StrToWzID("B015"),//EVENT_BT_SLOT_16, 
	StrToWzID("B016"),//EVENT_BT_SLOT_17, 
	StrToWzID("B017"),//EVENT_BT_SLOT_18, 
	StrToWzID("B018"),//EVENT_BT_SLOT_19, 
	StrToWzID("B019"),//EVENT_BT_SLOT_20, 
	StrToWzID("B020"),//EVENT_BT_SLOT_21, 
	StrToWzID("B021"),//EVENT_BT_SLOT_22, 
	StrToWzID("B022"),//EVENT_BT_SLOT_23, 
	StrToWzID("B023"),//EVENT_BT_SLOT_24, 
	StrToWzID("B024"),//EVENT_BT_SLOT_25, 
	StrToWzID("B000"),//EVENT_BT_REFRESH,
	StrToWzID("B999"),//EVENT_BT_EXIT,

};


//----------------------------------------------------------------------------
/**
*/
EventInventoryDialog::EventInventoryDialog(InterfaceManager* pUIMan) 
	: uiBase(pUIMan), m_pItemUnitRenderer(0)
{
	SCItemSlotContainer::Init(MAX_EVENT_INVENTORY_SLOT_NUM, SI_EVENT_INVENTORY);
}

//----------------------------------------------------------------------------
/**
*/
EventInventoryDialog::~EventInventoryDialog(void)
{
	SCItemSlotContainer::Release();
	SAFE_DELETE( m_pPressRefreshDelay );
	Release();
}

//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	
	m_TabNum = 0;
	m_pItemUnitRenderer = NULL;
	m_TabIndex = 0;

	m_pPressRefreshDelay = new util::Timer;
    
}


//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::Release()
{
	SAFE_RELEASENDELETE(this->m_pItemUnitRenderer);
}


//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::MessageProc( SI_MESSAGE * pMessage )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	switch (pMessage->eResultMsg)
	{
		case RT_MSG_LBUTTONCLICK:
			{
				switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
				{
				case EVENT_BT_SLOT_01: 
				case EVENT_BT_SLOT_02:
				case EVENT_BT_SLOT_03:
				case EVENT_BT_SLOT_04:
				case EVENT_BT_SLOT_05:
				case EVENT_BT_SLOT_06:
				case EVENT_BT_SLOT_07:
				case EVENT_BT_SLOT_08:
				case EVENT_BT_SLOT_09:
				case EVENT_BT_SLOT_10:
				case EVENT_BT_SLOT_11:
				case EVENT_BT_SLOT_12:
				case EVENT_BT_SLOT_13:
				case EVENT_BT_SLOT_14:
				case EVENT_BT_SLOT_15:
				case EVENT_BT_SLOT_16:
				case EVENT_BT_SLOT_17:
				case EVENT_BT_SLOT_18:
				case EVENT_BT_SLOT_19:
				case EVENT_BT_SLOT_20:
				case EVENT_BT_SLOT_21:
				case EVENT_BT_SLOT_22:
				case EVENT_BT_SLOT_23:
				case EVENT_BT_SLOT_24:
				case EVENT_BT_SLOT_25:
					{
						if ( this->m_packetStatus != NONE )
						{
							//	이벤트 정보를 전송중입니다.
							g_InterfaceManager.GetInterfaceString( eST_ALREADY_SEND_EVENT_INFO, szMessage, INTERFACE_STRING_LENGTH );
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
						}
						else
						{
							this->OnLbuttonClickSlot( pMessage );
						}
					}
					break;

				case EVENT_BT_REFRESH:
					{
						// TODO : 여기서 Refresh Message를 보낸다.
						if (this->m_packetStatus != 0)
						{
							return;
						}

						if ( m_pPressRefreshDelay->IsExpired() )
						{
							m_pPressRefreshDelay->Reset();

							SendRequestFetchEventItem();
						}
					}
					break;

				case EVENT_BT_EXIT:
					{
						if ( this->m_packetStatus != NONE )
						{
							//	이벤트 정보를 전송중입니다.
							g_InterfaceManager.GetInterfaceString( eST_ALREADY_SEND_EVENT_INFO, szMessage, INTERFACE_STRING_LENGTH );
							GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
						}
						else
						{
							ShowDialog(FALSE);
						}
					}
					break;
				}
			}
			break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::NetworkProc( MSG_BASE * pMsg )
{
	switch(pMsg->m_byCategory)
	{
		case CG_EVENT:
		{
			switch (pMsg->m_byProtocol)
			{
			case CG_EVENT_SELECT_ACK:
				{
					MSG_CG_EVENT_SELECT_ACK * pRecvPacket = (MSG_CG_EVENT_SELECT_ACK *)pMsg;

					BIT_REMOVE(this->m_packetStatus, EVENT_PACKET_STATUS_REFRESH);

					this->FlushUnitDraw();

					ItemManager::Instance()->InsertEventTotalInfo( pRecvPacket->m_TotalInfo );

					BOOL bShow = GET_DIALOG( IM_INVENTORY_MANAGER::EVENTINVEN_DLG )->IsVisible();
					if ( !bShow )
					{
						GET_DIALOG( IM_INVENTORY_MANAGER::EVENTINVEN_DLG )->ShowDialog(TRUE);
						InventoryDialog* pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
						pInvenDlg->SetbEventInventory(FALSE);
					}
				}
				break;

			case CG_EVENT_SELECT_NAK:
				{
					MSG_CG_EVENT_SELECT_NAK * pRecvPacket = (MSG_CG_EVENT_SELECT_NAK *)pMsg;

					BIT_REMOVE(this->m_packetStatus, EVENT_PACKET_STATUS_REFRESH);

					ItemManager::Instance()->NoticeReasonItemErrorCode( pRecvPacket->m_byErrorCode );
					InventoryDialog* pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );//static_cast<InventoryDialog *>(g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_INVENTORY ));
					pInvenDlg->SetbEventInventory(FALSE);
				}
				break;

			case CG_EVENT_MOVE_ITEM_TO_INVEN_ACK:
				{
					MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_ACK * pRecvPacket = (MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_ACK *)pMsg;
					
					assert( !IsEmpty( m_iPendingMovePos ) && m_iPendingMovePos < this->GetMaxSlotNum());
					this->DeleteSlot( m_iPendingMovePos, NULL );

					BIT_REMOVE(this->m_packetStatus, EVENT_PACKET_STATUS_MOVE_ITEM);

					ItemManager::Instance()->InsertJustInventory(EVENT, pRecvPacket->m_TotalInfo);

					MouseHandler::Instance()->RemoveItemAtHand();
				}
				break;

			case CG_EVENT_MOVE_ITEM_TO_INVEN_NAK:
				{
					MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_NAK * pRecvPacket = (MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_NAK *)pMsg;

					BIT_REMOVE(this->m_packetStatus, EVENT_PACKET_STATUS_MOVE_ITEM);

					ItemManager::Instance()->NoticeReasonItemErrorCode( pRecvPacket->m_byErrorCode );

					MouseHandler::Instance()->RollbackItemAtHand();
				}
				break;

			case CG_EVENT_QUERY_EXIST_ACK:
				{
					MSG_CG_EVENT_QUERY_EXIST_ACK* RecvPacket = (MSG_CG_EVENT_QUERY_EXIST_ACK*)pMsg;
					InventoryDialog* pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
					switch(RecvPacket->m_bExist)
					{
					case MSG_CG_EVENT_QUERY_EXIST_ACK::EXIST:
						{
							pInvenDlg->SetbEventInventory(TRUE);
						}
						break;

					case MSG_CG_EVENT_QUERY_EXIST_ACK::EMPTY:
					case MSG_CG_EVENT_QUERY_EXIST_ACK::DISABLED:
						{
							pInvenDlg->SetbEventInventory(FALSE);
						}
						break;
					}
				}
				break;
			}
		}
		break;
    }
		
}

//----------------------------------------------------------------------------
/**
*/
VOID			
EventInventoryDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			POSTYPE RelativePos = GetRealSlotPos(pos);

			// 0~ INVEN_MAX_SLOT 까지 아이템 정보를 보여준다.
			if (RelativePos < this->GetMaxSlotNum() && 
				pos >= 0 && pos < eEVENT_SLOT_MAX)
			{
				if (!IsEmpty( RelativePos ))
				{
					SCSlot & rSlot = GetSlot( RelativePos );
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
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::OnRenderSolarDialog()
{
	if ( this->m_pItemUnitRenderer )
	{
		this->m_pItemUnitRenderer->Render();
	}

	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterSlotInfoTooltip(m_MouseOverObject.pSlot);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}

			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < eEVENT_SLOT_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
EventInventoryDialog::OnShowWindow(BOOL val)
{
	if (val)
	{
		this->UpdatePosition();
		m_pPressRefreshDelay->SetTimer(EVENT_INVENTORY_REFRESH_DELAY);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_event_inventory;
		msg.wParam=InterfaceManager::DIALOG_EVENT_INVENTORY;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_event_inventory);		
		this->m_packetStatus = 0;
	}
}


//----------------------------------------------------------------------------
/**
*/
void            
EventInventoryDialog::UpdatePosition()
{
	RECT rcDlg;
	RECT rcEventDlg;

	uiBase * pInventoryDlg = GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG );

	pInventoryDlg->GetDialogWZ()->GetClientRect(&rcDlg);
	GetDialogWZ()->GetClientRect(&rcEventDlg);
	
	int x, y;
	x = rcDlg.left - ( rcEventDlg.right - rcEventDlg.left );
	y = rcDlg.top - 60;
	
	this->MoveWindowWZ( x, y );
}

//----------------------------------------------------------------------------
/**
*/
RC::eSLOT_INSERT_RESULT		EventInventoryDialog::InsertSlot( POSTYPE AtPos, SCSlot & rSlot )
{
	// 클라에서 따로 시리얼을 발급한다.
	/// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rRealSlot = GetSlot(AtPos);
	rRealSlot.SetSerial(dwSerial);

	CControlWZ * pCtrl = getControl( GetRelativeSlotPos(AtPos) );
	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		ItemUnitRenderer::RenderUnit * pRenderUnit = 
			m_pItemUnitRenderer->AddItemUnit( rRealSlot.GetSerial(), rRealSlot.GetCode(), &rect, &rRealSlot,pCtrl);
	}
	else
	{
		assert(m_pItemUnitRenderer);
	}

	ReFreshRenderSlot();

	return eInsertSlotResult;
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
EventInventoryDialog::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	assert( FALSE == IsEmpty(AtPos) );

	// render unit 를 먼저 지운다.
	SCSlot & rSlot = GetSlot(AtPos);
	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rSlot.GetSerial() );
	}
	else
	{	
		assert(m_pItemUnitRenderer);
	}

	g_SlotKeyGenerator.Restore((DWORD)rSlot.GetSerial());

	SCSlotContainer::DeleteSlot(AtPos, pSlotOut );

	ReFreshRenderSlot();

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
VOID			
EventInventoryDialog::UpdateSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	
}

//----------------------------------------------------------------------------
/**
*/
VOID 
EventInventoryDialog::OnInsert( SCSlot & IN rSlotIn )
{
	ItemManager::Instance()->AddUpdateItemSlot(&rSlotIn);
}

//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::OnDelete( SCSlot & IN rSlotIn )
{

	ItemManager::Instance()->RemoveUpdateItemSlot(&rSlotIn);

}
//----------------------------------------------------------------------------
/**
*/
VOID			
EventInventoryDialog::OnLbuttonClickSlot(SI_MESSAGE * pMessage)
{
	POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
	
	CControlWZ * pControl = getControl(pos);
	if (!pControl)
	{
		return;
	}

	switch (pos)
	{
	case EVENT_BT_SLOT_01: 
	case EVENT_BT_SLOT_02:
	case EVENT_BT_SLOT_03:
	case EVENT_BT_SLOT_04:
	case EVENT_BT_SLOT_05:
	case EVENT_BT_SLOT_06:
	case EVENT_BT_SLOT_07:
	case EVENT_BT_SLOT_08:
	case EVENT_BT_SLOT_09:
	case EVENT_BT_SLOT_10:
	case EVENT_BT_SLOT_11:
	case EVENT_BT_SLOT_12:
	case EVENT_BT_SLOT_13:
	case EVENT_BT_SLOT_14:
	case EVENT_BT_SLOT_15:
	case EVENT_BT_SLOT_16:
	case EVENT_BT_SLOT_17:
	case EVENT_BT_SLOT_18:
	case EVENT_BT_SLOT_19:
	case EVENT_BT_SLOT_20:
	case EVENT_BT_SLOT_21:
	case EVENT_BT_SLOT_22:
	case EVENT_BT_SLOT_23:
	case EVENT_BT_SLOT_24:
	case EVENT_BT_SLOT_25:
		{
			RECT rcRect = pControl->GetSizeRect();
			MouseHandler::Instance()->ItemTransaction( SI_EVENT_INVENTORY, GetRealSlotPos(pos), &rcRect);
		}
		break;

	default:
		{
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID	
EventInventoryDialog::ReFreshRenderSlot()
{
	ItemUnitRenderer::RenderUnit * pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		SCSlot & rSlot = GetSlot(i);

		if (this->IsEmpty(i))
		{
			continue;
		}

		if ( i >= m_TabNum && i < m_TabNum + eEVENT_SLOT_MAX )
		{ 
			// 보여줘야할 목록
			if(!m_pItemUnitRenderer)
				continue;

			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

			if (!pRender )
			{
				pCtrl = getControl( GetRelativeSlotPos(i) );
				rcSlot = pCtrl->GetSizeRect();

				pRender = m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}

			if (pRender && pRender != MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit)
				pRender->SetShow(TRUE);
		}
		else
		{
			// 보여지지 말아야 할 목록
			if(!m_pItemUnitRenderer)
				continue;

			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );

			if (!pRender )
			{
				pCtrl = getControl( GetRelativeSlotPos(i) );
				rcSlot = pCtrl->GetSizeRect();

				pRender = m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}

			if (pRender)
				pRender->SetShow(FALSE);				
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID					
EventInventoryDialog::InitRenderer()
{
	if (m_pItemUnitRenderer)
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, GetMaxSlotNum() );
}

//----------------------------------------------------------------------------
/**
*/
VOID					
EventInventoryDialog::FlushUnitDraw()
{
	SCItemSlot FromSlot;

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

}


//----------------------------------------------------------------------------
/**
*/
ItemUnitRenderer *
EventInventoryDialog::GetItemUnitRender() const
{
	assert(this->m_pItemUnitRenderer);
	return this->m_pItemUnitRenderer;
}

//----------------------------------------------------------------------------
/** 실제 슬롯정보관련한 위치
*/
POSTYPE					
EventInventoryDialog::GetRealSlotPos( POSTYPE pos )
{
	return pos + m_TabNum;
}

//----------------------------------------------------------------------------
/** 인터페이스 컨트롤 위치.
*/
POSTYPE					
EventInventoryDialog::GetRelativeSlotPos( POSTYPE pos )
{
	return pos % eEVENT_SLOT_MAX;
}

//----------------------------------------------------------------------------
/**
*/
VOID					
EventInventoryDialog::SendItemMoveMsg( POSTYPE FromPos )
{
	if ( !BIT_CHECK( this->m_packetStatus, EVENT_PACKET_STATUS_MOVE_ITEM ) )
	{
		BIT_ADD( this->m_packetStatus, EVENT_PACKET_STATUS_MOVE_ITEM);
		
		MSG_CG_EVENT_MOVE_ITEM_TO_INVEN_SYN	SendPacket;
		
		assert( FromPos <= this->GetMaxSlotNum() );

		m_iPendingMovePos = SendPacket.m_ItemPos = FromPos;

		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
	}
}

//----------------------------------------------------------------------------
/**
*/
CControlWZ *	
EventInventoryDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE			
EventInventoryDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

//----------------------------------------------------------------------------
/**
*/
VOID					
EventInventoryDialog::SendRequestFetchEventItem()
{
	if ( !BIT_CHECK(this->m_packetStatus, EVENT_PACKET_STATUS_REFRESH) )
	{

		BIT_ADD(this->m_packetStatus, EVENT_PACKET_STATUS_REFRESH);
		this->m_pPressRefreshDelay->Reset();

		MSG_CG_EVENT_SELECT_SYN		SendPacket;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	}
}