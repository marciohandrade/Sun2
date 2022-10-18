//------------------------------------------------------------------------------
//  uiQuestMissionList_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestMissionList.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"

//------------------------------------------------------------------------------
WzID uiQuestMissionList::m_wzId[DIALOG_MAX] = 
{ 
	StrToWzID("C001"),	//eDLGPOS_CHK_MISSION,
	StrToWzID("C002"),	//eDLGPOS_CHK_QUEST,	
	StrToWzID("LS00"),	//eDLGPOS_LST_MISSION_LIST,
	StrToWzID("BT00"),	//eDLGPOS_BTN_SELECT_MISSION,
	StrToWzID("B999"),	//eDLGPOS_BTN_EXIT,
	StrToWzID("CT00"),	//eDLGPOS_BTN_MAIN,
	StrToWzID("CT01"),	//eDLGPOS_BTN_SUB1,
	StrToWzID("CT02"),	//eDLGPOS_BTN_SUB2,
	StrToWzID("CT03"),	//eDLGPOS_BTN_SUB3,
	StrToWzID("CT04"),	//eDLGPOS_BTN_SUB4,
	StrToWzID("T400"),	//eDLGPOS_TXT_REWARD_1,
	StrToWzID("T401"),	//eDLGPOS_TXT_REWARD_2,
	StrToWzID("T402"),  //eDLGPOS_TXT_REWARD_3,

};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestMissionList::OnShowWindow( BOOL val )
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

		initInfo();

		UpdateMissionInfo();
		
    }
    else
    {
		if (!this->GetManager())
			return;

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
		GetManager()->SetPlayerWindowPos(pt);

		this->ClearInfo();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestMissionList::MessageProc( SI_MESSAGE * pMessage )
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
			case eDLGPOS_LST_MISSION_LIST:
				{
					clickMissionList();
				}
				break;
			case eDLGPOS_BTN_SELECT_MISSION:
				{
					clickMissionBtn();
				}
				break;

			case eDLGPOS_BTN_MAIN:
			case eDLGPOS_BTN_SUB1:
			case eDLGPOS_BTN_SUB2:
			case eDLGPOS_BTN_SUB3:
			case eDLGPOS_BTN_SUB4:
				{
					pos -= eDLGPOS_BTN_MAIN;
					clickMissionSubBtn(pos);
				}
				break;

			case eDLGPOS_BTN_EXIT:
				{
					ShowInterfaceWithSound(FALSE);
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
}
//
////------------------------------------------------------------------------------
///**
//*/
//void     
//uiQuestMissionList::UpdatePosition()
//{
//    //SolarDialog::UpdatePosition();
//}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestMissionList::OnUpdateSolarDialog()
{
    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	updateInfo();

}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
