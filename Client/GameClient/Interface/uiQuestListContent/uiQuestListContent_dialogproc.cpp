//------------------------------------------------------------------------------
//  uiQuestListContent_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestListContent.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"

//------------------------------------------------------------------------------
WzID uiQuestListContent::m_wzId[DIALOG_MAX] = 
{ 
	StrToWzID("C001"),	//eDLGPOS_CHK_MISSION,
	StrToWzID("C002"),	//eDLGPOS_CHK_QUEST,		
	StrToWzID("C003"),	//eDLGPOS_CHK_QUEST_MISSION,	// 임무
	StrToWzID("C004"),	//eDLGPOS_CHK_QUEST_CONTENT,	// 내용
	StrToWzID("LS00"),	//eDLGPOS_LST_PROGRESS_QUEST,
	StrToWzID("B999"),	//eDLGPOS_BTN_EXIT,
	StrToWzID("B998"),	//eDLGPOS_BTN_CANCEL,
	StrToWzID("CT00"),	//eDLGPOS_CHK_QUEST_PREVIEW		
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestListContent::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        if (!this->GetManager())
			return;

		if (GetManager()->IsPlayerWindowPosActivated())
		{
			POINT pt = GetManager()->GetPlayerWindowPos();
			GetDialogWZ()->MoveWindowWZ((float)pt.x, (float)pt.y);
			GetManager()->DeactivatedPlayerWindowPos();
		}
		else
		{
			//MoveWindowWZ( -1.0f, 120.0f );
		}

        int iPos = GetManager()->GetCurBeginDrawProgressList();
        GetCtrlProgressQuestList()->SetBeginDrawIndex( iPos );

        iPos = GetManager()->GetCurPosProgressListVscr();
        GetCtrlProgressQuestVScroll()->SetScrollPosWZ(iPos);

		if ( -1 != GetManager()->GetCurSelectQuestIndex() )
		{
			GetCtrlProgressQuestList()->SetCurSel(GetManager()->GetCurSelectQuestIndex()) ;
		}

		initInfo();

		UpdateQuestInfo();
    }
    else
    {
		if (!this->GetManager())
			return;

		this->ClearInfo();

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
		GetManager()->SetPlayerWindowPos(pt);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListContent::MessageProc( SI_MESSAGE * pMessage )
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
			case eDLGPOS_CHK_MISSION:
			case eDLGPOS_CHK_QUEST:	
				{
					clickChkCategory(pos);
				}				
				break;

			case eDLGPOS_CHK_QUEST_MISSION:
			case eDLGPOS_CHK_QUEST_CONTENT:
				{
					clickChkQuestType(pos);
				}				
				break;

			case eDLGPOS_LST_PROGRESS_QUEST:
				{
					clickQuestList();
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
				}
				break;


			case eDLGPOS_CHK_QUEST_PREVIEW:
				{
					clickQuestPreviewCheck();
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
			case eDLGPOS_LST_PROGRESS_QUEST:
				{
					clickRbtnQuestList();		
				}
				break;

			case eDLGPOS_CHK_MISSION:
			case eDLGPOS_CHK_QUEST:		
				{
					clickChkCategory(pos);
				}
				break;

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
//uiQuestListContent::UpdatePosition()
//{
//    //SolarDialog::UpdatePosition();
//}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListContent::OnUpdateSolarDialog()
{
    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	updateInfo();

	m_FTextRender.SetRect(GetCtrlQuestContentList()->GetSizeRect());

}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestListContent::OnRenderSolarDialog()
{
	m_FTextRender.Render();
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
