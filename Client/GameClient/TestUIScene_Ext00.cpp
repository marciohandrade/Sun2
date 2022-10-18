//------------------------------------------------------------------------------
//  TestUIScene_Ext00.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Version.h"
#include "TestUIScene_Ext00.h"
#include "InterfaceManager.h"

#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildMan/uiGuildMan.h"

#include "uiPartyMember/uiPartyMember.h"
#include "uiBattleZoneReadyMissionConfig/uiBattleZoneReadyMissionConfig.h"

#include "ChatDialog.h"

#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"

#include "uiChatMan/uiChatMan_def.h"

TestUIScene_Ext00 g_TestUIScene_Ext00;

//------------------------------------------------------------------------------
/**
*/
TestUIScene_Ext00::TestUIScene_Ext00(void)
{
}

//------------------------------------------------------------------------------
/**
*/
TestUIScene_Ext00::~TestUIScene_Ext00(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext00::Activate()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext00::Init()
{
	// 인터페이스 랜더링 온
	g_InterfaceManager.SetRenderState( TRUE );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext00::Process( DWORD dwTick )
{
	ProcessKeyInput(dwTick);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext00::Render( DWORD dwTick )
{
	RenderText();

	g_pSunRenderer->Set2DMode( TRUE, FALSE );

    if (!g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG))
    {
        ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
            //static_cast<ChatDialog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT));
        if (pDlg)
            pDlg->RenderChatMsgWithoutDialog(dwTick);
    }

    g_pSunRenderer->Set2DMode( FALSE, FALSE );

	g_pSunRenderer->x_pManagerPostEffect->Apply();

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext00::Release()
{
	GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG)->ShowDialog(FALSE);//g_InterfaceManager.ShowDialog( InterfaceManager::DIALOG_CHAT, FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext00::ParsePacket( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext00::RenderText( void )
{
	// 준비 과정
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );

	TCHAR szBuf[1024] = {0,};
    Snprintf (szBuf, 1024-1, _T("SUN TestUIScene - %d.%d.%d (Ext%02d)"),
                                                    C2S_HIGH_VERSION_NO,
                                                    C2S_MIDDLE_VERSION_NO,
                                                    C2S_LOW_VERSION_NO,
                                                    (GENERALPARAM->GetTestUISceneID() - 1));

	g_pSunRenderer->x_pManagerTextOut->DrawText( szBuf, 5, 5, WzColor_RGBA(255, 162, 38, 255) );

	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext00::ProcessKeyInput(DWORD dwTick)
{
    if ( g_Input.GetState(DIK_B, KS_DOWN ) )
    {
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		uiBase * missionCfg = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG );
        //uiBattleZoneReadyMissionConfig* missionCfg =
        //    static_cast<uiBattleZoneReadyMissionConfig *>(g_InterfaceManager.GetUserInterface(UI_BATTLEZONE_READY_MISSION_CFG));
        if (missionCfg)
        {
            missionCfg->ShowInterface(TRUE);
        }
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES))
    }



    if ( g_Input.GetState( DIK_RETURN, KS_UP ))
    {
        //if (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_CHAT) == FALSE)
		if (g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG) == FALSE)
        {
            ChatDialog *pDlg = GET_CAST_DIALOG( ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                //(ChatDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);

            if(!pDlg->m_preShow)
            {
                pDlg->ShowDialog(TRUE);//g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_CHAT, TRUE);
            }
            else
            {
                pDlg->m_preShow = FALSE;	
            }
        }
    }
    else if (g_Input.GetState( DIK_RETURN, IKS_UP ) )
    {
        if (g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG) == TRUE)
        {
            //ChatDialog * pDlg =
            //    (ChatDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
        }
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
