//------------------------------------------------------------------------------
//  uiPartyLeavePopup_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyLeavePopup.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiPartyMan/uiPartyMan.h"
#include "GameFrameWork.h"
//------------------------------------------------------------------------------
WzID uiPartyLeavePopup::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("TI00"),  // ok
        StrToWzID("TI01"),  // cancel
        StrToWzID("L000"),  // msg
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiPartyLeavePopup::OnShowWindow( BOOL val )
{
    //this->m_bShow = val;    // for uiBase::IsVisable()

    if (val)
    {
        SolarDialog::MoveCenter();

        bDestroy = true;
        bDestroyTimer = false;
        dwDestroyTick = 0;
        dwDestroyTickOver = 0;

        this->OffLock();
        this->OffPending();

        this->m_status = ASK_STATUS_NORMAL;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_party_leave;
		msg.wParam=IM_PARTY_MANAGER::PARTY_LEAVE_POPUP;
		msg.DoSomething=GlobalFunc::DoPartyLeave;
		g_KeyQueueManager.PushBack(msg);

    }
    else
    {
        this->m_status = ASK_STATUS_CANCEL;
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_party_leave);

    }
}

//------------------------------------------------------------------------------
/**
*/
void 
uiPartyLeavePopup::Exit()
{
	this->m_status = ASK_STATUS_CANCEL;
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
                // 닫기
            case DLGPOS_CANCEL:
                {
                    this->m_status = ASK_STATUS_CANCEL;
                }
                break;

            case DLGPOS_OK:
                {
                    this->m_status = ASK_STATUS_OK;
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

//------------------------------------------------------------------------------
/**
*/
void uiPartyLeavePopup::Process(DWORD dwTick)
{
    if (this->m_status != ASK_STATUS_NORMAL)
        return;

    if (bDestroyTimer)
    {
        dwDestroyTick = clock_function::GetTickCount();

        if (dwDestroyTick > dwDestroyTickOver)
        {
            this->response_No();

            bDestroyTimer = false;
        }
    }

    if (bDestroy)
    {
        dwDestroyTick = clock_function::GetTickCount();
        dwDestroyTickOver = clock_function::GetTickCount() + (10*1000);
        bDestroy = false;
        bDestroyTimer = true;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::OnUpdateSolarDialog()
{
    
    if (this->GetStatus() == ASK_STATUS_OK)
    {
        this->response_Yes();
    }
    else if (this->GetStatus() == ASK_STATUS_CANCEL)
    {
        this->response_No();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeavePopup::OnRenderSolarDialog()
{
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));

    RECT rc, rc2;
    GetDialogWZ()->GetClientRect(&rc2);

	uiPartyMan* partyMan = 
		static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
	if (partyMan)
	{
		bool bCondition1 = partyMan->IsParty(); // 파티여부
		bool bCondition2 = (GameFramework::GetCurScene() != SCENE_TYPE_FIELD);		// 필드가 아닌경우 true
		bool bCondition3 = (GameFramework::GetCurScene() != SCENE_TYPE_VILLAGE);	// 마을이 아닌경우 true
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        bool bCondition4 = (GameFramework::GetCurScene() != SCENE_TYPE_DOMINATION   // 점령전이 아닌경우 true
#ifdef _NA_008334_20150608_SONNENSCHEIN
            && (GameFramework::GetCurScene() != SCENE_TYPE_SONNENSCHEIN)	        // 소넨샤인 아닌경우
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            && (GameFramework::GetCurScene() != SCENE_TYPE_GUILDMISSION)	        // 길드미션이 아닌경우
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            );
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

		if (bCondition1 && bCondition2 && bCondition3
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            && bCondition4
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
            )
		{
			//	파티를 떠나면 마을로
			const int	c_iMoveToVillage = 70415;
			g_InterfaceManager.GetInterfaceString( c_iMoveToVillage, szMessage, INTERFACE_STRING_LENGTH );
	
			SetRect(&rc,
				rc2.left + 25,
				rc2.top + 40 + (14*0),
				rc2.right - 10,
				rc2.top + 40 + (14*0) + 14);

			this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
				&rc,
				WzColor_RGBA(255, 133, 50, 255),
				WzColor_RGBA(0,0,0,0),
				TP_HLEFT);

			//	돌아가게 됩니다.
			const int	c_iMoveToVillageEnd = 70416;
			g_InterfaceManager.GetInterfaceString( c_iMoveToVillageEnd, szMessage, INTERFACE_STRING_LENGTH );
		

			SetRect(&rc,
				rc2.left + 25,
				rc2.top + 40 + (14*1),
				rc2.right - 10,
				rc2.top + 40 + (14*1) + 14);

			this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
				&rc,
				WzColor_RGBA(255, 133, 50, 255),
				WzColor_RGBA(0,0,0,0),
				TP_HLEFT);
		}
	}

	//	파티를 탈퇴하시겠습니까?
	const int	c_iAreYouSure = 5204;	// from StringTable.txt
	g_InterfaceManager.GetInterfaceString( c_iAreYouSure, szMessage, INTERFACE_STRING_LENGTH );

    SetRect(&rc,
        rc2.left + 25,
        rc2.top + 40 + (14*2),
        rc2.right - 10,
        rc2.top + 40 + (14*2) + 14);

	this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255, 133, 50, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT);

    //	남은 시간.... ( %d )
    g_InterfaceManager.GetInterfaceString( eST_REMAIN_TIME, szText, INTERFACE_STRING_LENGTH );
    Snprintf( szMessage, INTERFACE_STRING_LENGTH-1, szText, ((dwDestroyTickOver - dwDestroyTick)/1000)+1 );

    SetRect(&rc,
        rc2.left + 25,
        rc2.top + 40 + (14*4),
        rc2.right - 10,
        rc2.top + 40 + (14*4) + 14);

	this->GetDrawBase()->x_pManagerTextOut->DrawText(szMessage,
        &rc,
        WzColor_RGBA(255, 255, 255, 255),
        WzColor_RGBA(0,0,0,0),
        TP_HLEFT);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
