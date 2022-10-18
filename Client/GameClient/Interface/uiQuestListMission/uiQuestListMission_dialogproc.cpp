//------------------------------------------------------------------------------
//  uiQuestListMission_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestListMission.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"
#include "CursorChangeTransact.h"
#include "uiToolTipMan/uiToolTipMan.h"
//------------------------------------------------------------------------------
WzID uiQuestListMission::m_wzId[DIALOG_MAX] = 
{ 
	StrToWzID("C101"),	//eDLGPOS_CHK_SELECT_ITEM_1,
	StrToWzID("C102"),	//eDLGPOS_CHK_SELECT_ITEM_2,
	StrToWzID("C103"),	//eDLGPOS_CHK_SELECT_ITEM_3,
	StrToWzID("C104"),	//eDLGPOS_CHK_SELECT_ITEM_4,
	StrToWzID("C000"),	//eDLGPOS_CHK_SELECT_ITEM_5,
	StrToWzID("C105"),	//eDLGPOS_CHK_FIX_ITEM,
	StrToWzID("C003"),	//eDLGPOS_CHK_QUEST_MISSION,	// 임무
	StrToWzID("C004"),	//eDLGPOS_CHK_QUEST_CONTENT,	// 내용
	StrToWzID("LS01"),	//eDLGPOS_LST_MISSION,
	StrToWzID("B999"),	//eDLGPOS_BTN_EXIT,
	StrToWzID("B998"),	//eDLGPOS_BTN_CANCEL,
    StrToWzID("T999"),  //eDLGPOS_TITLE
    StrToWzID("B000"),	//eDLGPOS_BTN_REWARD_POINT,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestListMission::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        if (!this->GetManager())
			return;

		this->ClearInfo();

		if (GetManager()->IsPlayerWindowPosActivated())
		{
			POINT pt = GetManager()->GetPlayerWindowPos();
			GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
			GetManager()->DeactivatedPlayerWindowPos();
		}
		//else
		//{
		//	MoveWindowWZ( -1.0f, 120.0f );
		//}
        
		m_btDlgState = 0;

		initInfo();

		UpdateQuestInfo();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_Dialog_QuestInfo_Dlg;
		msg.DoSomething=GlobalFunc::DoQuestInfoDlg;
		g_KeyQueueManager.PushBack(msg);
    }
    else
    {
		this->ClearInfo();

		if (!this->GetManager())
			return;

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;

        if (pt.x != 0 || pt.y != 0)
        {
            GetManager()->SetPlayerWindowPos(pt);   
        }

		g_KeyQueueManager.DeleteMsg(eDoType_Dialog_QuestInfo_Dlg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListMission::MessageProc( SI_MESSAGE * pMessage )
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
			case eDLGPOS_CHK_QUEST_MISSION:
			case eDLGPOS_CHK_QUEST_CONTENT:
				{
					clickChkQuestType(pos);
				}
				break;

			case eDLGPOS_BTN_EXIT:
				{
					ShowInterfaceWithSound(FALSE);
				}				
				break;

			case eDLGPOS_BTN_CANCEL:
				{
					clickQuestAbandon();
					ShowInterfaceWithSound(FALSE);

					if(TRUE == GET_DIALOG(IM_QUEST_MANAGER::QUEST_COMMON_FORWARD)->IsVisible())
						GET_DIALOG(IM_QUEST_MANAGER::QUEST_COMMON_FORWARD)->ShowInterface(FALSE);

					if(TRUE == GET_DIALOG(IM_QUEST_MANAGER::QUEST_REWARD)->IsVisible())
						GET_DIALOG(IM_QUEST_MANAGER::QUEST_REWARD)->ShowInterface(FALSE);
				}
				break;

			default:
				break;
            }
        }
        break;

	case RT_MSG_RBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			switch( pos )
			{
			case eDLGPOS_CHK_QUEST_MISSION:
			case eDLGPOS_CHK_QUEST_CONTENT:
				{
					clickChkQuestType(pos);
				}
				break;
			default:
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

	ListMessageProc( pMessage );
}

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiQuestListMission::UpdatePosition()
//{
//    //SolarDialog::UpdatePosition();
//}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
uiQuestListMission::MouseOver_MsgProc( SI_MESSAGE * pMessage )
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

	SolarDialog::MouseOver_MsgProc(pMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListMission::OnUpdateSolarDialog()
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
#ifdef _INTERNATIONAL_UI
void uiQuestListMission::toolTipForTitle(int iMouseX,int iMouseY)
{
	CCtrlStaticWZ* pTitle =  (CCtrlStaticWZ*)getControl(eDLGPOS_TITLE);

	if(!pTitle) return;

	RECT rc = pTitle->GetSizeRect();

	if (rc.left <= iMouseX && rc.right >= iMouseX)
	{
		if (rc.top <= iMouseY && rc.bottom >= iMouseY)
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				TCHAR szTitle[INTERFACE_STRING_LENGTH + 1] = {0,};
				g_InterfaceManager.GetInterfaceString(m_pCurQuest->GetQuestInfo()->GetQNCode(), szTitle);
				pUIMan->RegisterTooltipSimpleText(szTitle);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
}
#endif//_INTERNATIONAL_UI
//------------------------------------------------------------------------------
/**
*/
void
uiQuestListMission::OnRenderSolarDialog()
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
#ifdef _INTERNATIONAL_UI
	toolTipForTitle(iMouseX,iMouseY);
#endif//_INTERNATIONAL_UI
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
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
