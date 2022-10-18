#include "SunClientPrecompiledHeader.h"
#include "testuiscene_ext02.h"

#include "ChatDialog.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiChatMan/uiChatMan_def.h"

TestUIScene_Ext02 g_TestUIScene_Ext02;

TestUIScene_Ext02::TestUIScene_Ext02(void)
{
}

TestUIScene_Ext02::~TestUIScene_Ext02(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext02::Activate()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext02::Init()
{
	// 인터페이스 랜더링 온
	g_InterfaceManager.SetRenderState( TRUE );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext02::Process( DWORD dwTick )
{
	ProcessKeyInput(dwTick);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext02::Render( DWORD dwTick )
{
	RenderText();

	g_pSunRenderer->Set2DMode( TRUE, FALSE );

	if (!g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG))
    {
        ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

        if (pDlg)
            pDlg->RenderChatMsgWithoutDialog(dwTick);
    }
	
	g_pSunRenderer->x_pManagerTextOut->DrawText(
		m_szTest, 
		50, 
		600, 
		WzColor_RGBA(255,255,255,255));

    g_pSunRenderer->Set2DMode( FALSE, FALSE );

	g_pSunRenderer->x_pManagerPostEffect->Apply();

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext02::Release()
{
	GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG)->ShowDialog(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext02::ParsePacket( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext02::RenderText( void )
{
	// 준비 과정
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );

	TCHAR szBuf[1024] = {0,};
	Snprintf (szBuf, 1024-1, _T("TestUISceneExt02 : Let's Test!!"));
	g_pSunRenderer->x_pManagerTextOut->DrawText( szBuf, 5, 5, WzColor_RGBA(255, 162, 38, 255) );

	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext02::ProcessKeyInput(DWORD dwTick)
{
    if ( g_Input.GetState( DIK_RETURN, KS_UP ))
    {
		if (g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG) == FALSE)//if (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_CHAT) == FALSE)
        {
            ChatDialog *pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

            if(!pDlg->m_preShow)
            {
                pDlg->ShowDialog(TRUE);
            }
            else
            {
                pDlg->m_preShow = FALSE;	
            }
        }
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
