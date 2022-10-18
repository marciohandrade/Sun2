#include "SunClientPrecompiledHeader.h"
#include "testuiscene_ext03.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattleZoneList/uiBattleZoneList.h"

#include "uiBattleZoneReadyCHuntingConfig/uiBattleZoneReadyCHuntingConfig.h"


#include "uiBattleZoneCHuntingScoreBoard/uiBattleZoneCHuntingScoreBoard.h"

#include "uiBattleZoneWinDialog/uiBattleZoneWinDialog.h"

#include "interfacemanager.h"
#include "globalfunc.h"
#include "chatdialog.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiChatMan/uiChatMan_def.h"
TestUIScene_Ext03 g_TestUIScene_Ext03;

TestUIScene_Ext03::TestUIScene_Ext03(void)
{
}

TestUIScene_Ext03::~TestUIScene_Ext03(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext03::Activate()
{
}

CTimerSimple timerTest;

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext03::Init()
{
	g_InterfaceManager.SetRenderState( TRUE );

    timerTest.On(3);

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext03::Process( DWORD dwTick )
{
	ProcessKeyInput(dwTick);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext03::Render( DWORD dwTick )
{
	RenderText();

	g_pSunRenderer->Set2DMode( TRUE, FALSE );

    // OnRender2D

    g_pSunRenderer->Set2DMode( FALSE, FALSE );

	g_pSunRenderer->x_pManagerPostEffect->Apply();

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext03::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext03::ParsePacket( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext03::RenderText( void )
{
	// 준비 과정
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );

    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    Sprintf (szMessage, _T("TestUISceneExt02"));
    g_pSunRenderer->x_pManagerTextOut->DrawText( szMessage, 5, 5, WzColor_RGBA(255, 162, 38, 255) );
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	uiBase * zonecfg_CHunting_IFC = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

	if (zonecfg_CHunting_IFC && zonecfg_CHunting_IFC->IsVisible())
    {
        RECT rc;
        zonecfg_CHunting_IFC->GetDialogWZ()->GetClientRect(&rc);
        Sprintf (szMessage, _T("경쟁 헌팅 존 설정창 (LEFT_SHIFT + N)"));
        g_pSunRenderer->x_pManagerTextOut->DrawText( szMessage,
            rc.left,
            rc.top - 30,
            WzColor_RGBA(255, 162, 38, 255) );
    }
#endif//#ifndef ifndef _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext03::ProcessKeyInput(DWORD dwTick)
{
    static int openCount = 0;

    if (timerTest.Check() == TIMER_STAT_FINISH)
    {
        TCHAR   szMessage[INTERFACE_STRING_LENGTH];

        int seed = rand() % 2;

        switch (seed)
        {
        default:
        case 0:
            {
                Sprintf (szMessage, _T("테스트 하시겠습니까?\n\nSys_Confirm\nopenCount = %d"), openCount);

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Confirm(szMessage, NULL);

            }
            break;

        case 1:
            {
                Sprintf (szMessage, _T("테스트 하시겠습니까?\n\nSys_ConfirmLock\nopenCount = %d"), openCount);

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Confirm(szMessage, NULL);

            }
            break;
        }

        openCount++;

        timerTest.Off();
        timerTest.On(3);
    }

    if ( g_Input.GetState (DIK_LCONTROL, KS_KEY) && g_Input.GetState(DIK_Z, KS_DOWN ))
	{
		uiBase * zoneCreate = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE );

		if (zoneCreate)
		{
			zoneCreate->ShowInterface( !zoneCreate->IsVisible() );
		}
	}

    if ( g_Input.GetState (DIK_LCONTROL, KS_KEY) && g_Input.GetState(DIK_SPACE, KS_DOWN ))
    {
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		uiBattleZoneWinDialog * zoneWinDlg = GET_CAST_DIALOG( uiBattleZoneWinDialog, IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_WIN );

        if (zoneWinDlg)
        {

            zoneWinDlg->ShowResultWin(rand() % 10, 10, 100, 0, 0);
            zoneWinDlg->ShowInterface(TRUE);
        }
#endif //#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    }

    if ( g_Input.GetState (DIK_LSHIFT, KS_KEY) && g_Input.GetState(DIK_SPACE, KS_DOWN ))
    {
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		uiBase * zoneCHSCBoard = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_SCORE_BOARD );


        if (zoneCHSCBoard)
        {
            zoneCHSCBoard->ShowInterface(!zoneCHSCBoard->IsVisible());
        }
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    }

    if ( g_Input.GetState (DIK_LSHIFT, KS_KEY) && g_Input.GetState(DIK_N, KS_DOWN ))
    {
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		uiBase * zonecfg_CHunting_IFC = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

        if (zonecfg_CHunting_IFC)
        {
            zonecfg_CHunting_IFC->ShowInterface(!zonecfg_CHunting_IFC->IsVisible());
        }
#endif //#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    }

    if ( g_Input.GetState (DIK_LSHIFT, KS_KEY) && g_Input.GetState(DIK_V, KS_DOWN ))
    {
		uiBase * zoneListIFC = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST );

        if (zoneListIFC)
        {
            zoneListIFC->ShowInterface(!zoneListIFC->IsVisible());
        }
    }

    if ( g_Input.GetState(DIK_LSHIFT, KS_KEY) && g_Input.GetState(DIK_C, KS_DOWN) )
    {
		uiBase * zoneCreateIFC = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE );

        if (zoneCreateIFC)
        {
            zoneCreateIFC->ShowInterface(!zoneCreateIFC->IsVisible());
        }
    }

    if (g_Input.GetState( DIK_RETURN, KS_DOWN ) )
    {
        uiBase * pDlg = GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG);
        
        if (pDlg)
        {
            pDlg->ShowWindow(TRUE);
        }
        else
        {
            pDlg->ShowWindow(FALSE);
        }
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
