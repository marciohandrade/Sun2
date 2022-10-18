//------------------------------------------------------------------------------
//  uiQuestReward_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestReward.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"
#include "CursorChangeTransact.h"
#include "uiToolTipMan/uiToolTipMan.h"
//------------------------------------------------------------------------------
WzID uiQuestReward::m_wzId[DIALOG_MAX] = 
{ 
	StrToWzID("C101"),	//eDLGPOS_SLOT_SELECT_ITEM_1 = 0,
	StrToWzID("C102"),	//eDLGPOS_SLOT_SELECT_ITEM_2,
	StrToWzID("C103"),	//eDLGPOS_SLOT_SELECT_ITEM_3,
	StrToWzID("C104"),	//eDLGPOS_SLOT_SELECT_ITEM_4,
	StrToWzID("C000"),	//eDLGPOS_SLOT_SELECT_ITEM_5,
	StrToWzID("C105"),	//eDLGPOS_SLOT_FIX_ITEM,
	StrToWzID("V001"),	//eDLGPOS_VSCR_QUEST_INFO,
	StrToWzID("B999"),  //eDLGPOS_BTN_EXIT,
	StrToWzID("B001"),	//eDLGPOS_BTN_OK,
	StrToWzID("B002"),	//eDLGPOS_BTN_CANCEL,
    StrToWzID("B000"),	//eDLGPOS_BTN_REWARD_POINT,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestReward::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        if (!this->GetManager())
			return;

		ClearInfo();

		if (GetManager()->IsNPCWindowPosActivated())
		{
			POINT pt = GetManager()->GetNPCWindowPos();
			GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
			GetManager()->DeactivatedNPCWindowPos();
		}
		else
		{
			//MoveWindowWZ( -1.0f, 120.0f );
		}

		initInfo();
    }
    else
	{	
		if (!this->GetManager())
			return;

		this->ClearInfo();

		if (!this->CheckControls())
			return;

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
		GetManager()->SetNPCWindowPos(pt);

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Quest_NPC);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestReward::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    if (!this->GetManager())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch( pos )
            {
			case eDLGPOS_SLOT_SELECT_ITEM_1:
			case eDLGPOS_SLOT_SELECT_ITEM_2:
			case eDLGPOS_SLOT_SELECT_ITEM_3:
			case eDLGPOS_SLOT_SELECT_ITEM_4:
			case eDLGPOS_SLOT_SELECT_ITEM_5:
			//case eDLGPOS_SLOT_FIX_ITEM:
				{
					// 아이템 처리
					clickSelectItemSlot(pos);
				}
				break;

			case eDLGPOS_BTN_EXIT:
				{
					switch ( m_eQuestGainState )
					{
					case QUEST_AUTO_ACCEPT:
						{
							// 퀘스트 리스트 창을 열어준다.
							if ( !GetManager()->IsOpenQuestList() )
							{
								GetManager()->OpenForceQuestList();
							}						

							ShowInterface(FALSE);
						}
						break;  

					default:
						ShowInterfaceWithSound(FALSE);
						break;
					}
					
				}
				break;

			case eDLGPOS_BTN_OK:
			case eDLGPOS_BTN_CANCEL:
				{
					clickBtn(pos);
				}
				break;

			default:
				break;
            }
        }
        break;
	case RT_MSG_SB_LINETOP:
	case RT_MSG_SB_PAGETOP:
		{
			if ( GetWzIDQuestInfoVscr() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestInfoVscr()->GetScrollPosWZ();

				--index;
				index = max(0, index);
				m_FTextRender.SetBeginLineIndex( index );
				GetCtrlQuestInfoVscr()->SetScrollPosWZ( index );
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
	case RT_MSG_SB_PAGEBOTTOM:
		{
			if ( GetWzIDQuestInfoVscr() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestInfoVscr()->GetScrollPosWZ();

				++index;
				index = min((int)m_FTextRender.GetMaxLine(), index);
				m_FTextRender.SetBeginLineIndex( index );
				GetCtrlQuestInfoVscr()->SetScrollPosWZ( index );
			}

		}
		break;
	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
	case RT_MSG_SB_THUMBPOSITION:
		{
			//.썸을 드래그 하고 있다.

			if ( GetWzIDQuestInfoVscr() == pMessage->dwCtrlID )
			{
				int index = GetCtrlQuestInfoVscr()->GetScrollPosWZ();
				m_FTextRender.SetBeginLineIndex( index );
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

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiQuestReward::UpdatePosition()
//{
//    //SolarDialog::UpdatePosition();
//}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
uiQuestReward::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
	
			if (pos >= SLOT_BEGIN && pos < SLOT_SIZE)
			{
				if (!IsEmpty( pos ))
				{
					SCSlot & rSlot = GetSlot( pos );
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

//------------------------------------------------------------------------------
/**
*/
void
uiQuestReward::OnUpdateSolarDialog()
{
    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	updateInfo();

	if ( this->m_pItemUnitRenderer )
	{
		this->m_pItemUnitRenderer->Update();		
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestReward::OnRenderSolarDialog()
{
	if ( this->m_pItemUnitRenderer )
	{
		this->m_pItemUnitRenderer->Render();		
	}

	m_FTextRender.Render();

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
	for (POSTYPE a = SLOT_BEGIN; a < SLOT_SIZE; a++)
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

	std::list<sSelectRewardSlotIdx>::iterator itr =	m_SelectRewardItemList.begin();

	RECT rcControl;
	CControlWZ * pCurSelect = NULL;

	for ( itr; itr != m_SelectRewardItemList.end(); itr++)
	{
		sSelectRewardSlotIdx  SelectIndex = (*itr);
		assert(SelectIndex.pos >= SLOT_SELECT_BEGIN && 
			   SelectIndex.pos < SLOT_SELECT_BEGIN+SLOT_SELECT_SIZE);

		pCurSelect = getControl(SelectIndex.pos);
		rcControl = pCurSelect->GetSizeRect();

		g_InterfaceManager.RenderActivateMark( rcControl.left - 13, rcControl.top - 13);
	}
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
