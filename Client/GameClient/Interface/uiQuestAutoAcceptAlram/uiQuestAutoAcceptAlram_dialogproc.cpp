//------------------------------------------------------------------------------
//  uiQuestAutoAcceptAlram_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestAutoAcceptAlram.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"
#include "CursorChangeTransact.h"

//------------------------------------------------------------------------------
WzID uiQuestAutoAcceptAlram::m_wzId[DIALOG_MAX] = 
{ 
	StrToWzID("B001"), // eDLGPOS_ALRAM_BTN
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestAutoAcceptAlram::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        if (!this->GetManager())
			return;

		this->ClearInfo();

		initInfo();
    }
    else
    {
		this->ClearInfo();

		if (!this->GetManager())
			return;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestAutoAcceptAlram::MessageProc( SI_MESSAGE * pMessage )
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
			case eDLGPOS_ALRAM_BTN:
				{
					Quest * pQuest = GetManager()->GetAcceptAutoQuest();
					if (!pQuest)
					{
						return;							
					}

					GetManager()->OpenRewardWindowForAutoAlram( pQuest );					
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

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
uiQuestAutoAcceptAlram::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			if (eDLGPOS_ALRAM_BTN == pos) 
			{				
				CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Default_Highlight);
			}
		}
		break;

	default:
		break;
	}				
}

////------------------------------------------------------------------------------
///**
//*/
//void     
//uiQuestAutoAcceptAlram::UpdatePosition()
//{
//	//MoveWindowWZ( 900, 600 );
//}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestAutoAcceptAlram::OnUpdateSolarDialog()
{
    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	updateInfo();
		
	if ( !GetManager()->GetAcceptAutoQuestCount() )
	{
		ShowWindow(FALSE);
	}
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
