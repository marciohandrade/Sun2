//------------------------------------------------------------------------------
//  uiQuestTimeInfo_dialogproc.cpp
//  (C) 2005 SUN ONLINE 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiQuestTimeInfo.h"
#include "uiQuestMan/uiQuestMan.h"
#include "ItemUnitRenderer.h"
#include "mouse.h"
#include "Application.h"
#include "uiRadarMan/uiRadarMan.h"

//------------------------------------------------------------------------------
WzID uiQuestTimeInfo::m_wzId[DIALOG_MAX] = 
{ 
	0,

};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiQuestTimeInfo::OnShowWindow( BOOL val )
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

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		POINT pt;
		pt.x = rc.left;
		pt.y = rc.top;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestTimeInfo::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    if (!this->GetManager())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void     
uiQuestTimeInfo::UpdatePosition()
{
	RECT rc;

	uiBase * pRadarDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
	if ( pRadarDlg )
	{
		pRadarDlg->GetDialogWZ()->GetClientRect(&rc);
	}	

	POINT pt = g_pApplication->Get2DModeMapping();
	
	int y = rc.bottom += 5;
	GetDialogWZ()->GetClientRect(&rc);
    int x = pt.x - (rc.right-rc.left);	

	MoveWindowWZ( x, y );
}

//------------------------------------------------------------------------------
/**
*/
void
uiQuestTimeInfo::OnUpdateSolarDialog()
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
