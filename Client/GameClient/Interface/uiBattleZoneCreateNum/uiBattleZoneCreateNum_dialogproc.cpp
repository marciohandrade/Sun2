//------------------------------------------------------------------------------
//  uiBattleZoneCreateNum_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneCreateNum.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "Mouse.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneCreateNum::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_LV1=0,
    StrToWzID("TI01"),  // DLGPOS_BTN_LV2,
    StrToWzID("TI02"),  // DLGPOS_BTN_LV3,
    StrToWzID("TI03"),  // DLGPOS_BTN_LV4,
    StrToWzID("TI04"),  // DLGPOS_BTN_LV5,
    StrToWzID("TI05"),  // DLGPOS_BTN_LV6,
    StrToWzID("TI06"),  // DLGPOS_BTN_LV7,
    StrToWzID("TI07"),  // DLGPOS_BTN_LV8,
    StrToWzID("TI08"),  // DLGPOS_BTN_LV9,
    StrToWzID("TI09"),  // DLGPOS_BTN_LV10,
};

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateNum::showWindow( BOOL val )
{
    if (val)
    {
        //POSTYPE pos;
        //CCtrlStaticWZ * pControl;
        //TCHAR szMessage[INTERFACE_STRING_LENGTH];

        //for (int i=DLGPOS_BTN_LV1; i<DLGPOS_BTN_LV9+1; ++i)
        //{
        //    pos = getCtrlIDToPos( m_wzId[DLGPOS_BTN_LV1+i] );
        //    pControl = (CCtrlStaticWZ*)getControl(pos);
        //    _stprintf(szMessage, "%d", i);
        //    pControl->SetTextColor(RGBA(255,255,255,255));
        //    pControl->SetCaption( szMessage ); 
        //}
   
        //keyMsg msg;
        //ZeroMemory(&msg,sizeof(msg));
        //msg.dwType=eDoType_dialog_battle_createnum;
        //msg.DoSomething=GlobalFunc::DoBattleCreateNum;
        //g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        //g_KeyQueueManager.DeleteMsg(eDoType_dialog_battle_createnum);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneCreateNum::MessageProc( SI_MESSAGE * pMessage )
{
    switch (pMessage->eResultMsg)
    {
    case RT_MSG_LBUTTONDOWN:
        {
            bool clickedInDlg = false;

            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if (pos >= DLGPOS_BTN_LV1 && pos <= DLGPOS_BTN_LV10)
            {
                //uiBattleZoneCreate* battleZoneCreate =
                //    static_cast<uiBattleZoneCreate *>(this->GetUIMan()->GetUserInterface(UI_BATTLEZONE_CREATE));

                //battleZoneCreate->setPeopleLimit(pos+1);
            }
            else
            {
                int iMouseX = Mouse::Instance()->GetMouseX();
                int iMouseY = Mouse::Instance()->GetMouseY();

                RECT rc;

                this->GetDialogWZ()->GetDlgSize(&rc);;

                if (rc.left  <= iMouseX
                    && rc.right >= iMouseX)
                {
                    if (rc.top    <= iMouseY
                        && rc.bottom >= iMouseY)
                    {
                        clickedInDlg = true;
                    }
                }
            }

            if (!clickedInDlg)
                this->ShowInterface(FALSE);
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
uiBattleZoneCreateNum::update()
{
    if (!this->GetUIMan()->GetUserInterface(UI_BATTLEZONE_CREATE)->IsVisiable())
        this->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
