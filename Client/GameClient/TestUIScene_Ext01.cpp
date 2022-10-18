#include "SunClientPrecompiledHeader.h"
#include "testuiscene_ext01.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "MissionScene.h"
#include "ChatDialog.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"

#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "SCItemSlot.h"
#include "TestCase.h"
#include "CompetitionScene.h"
#include "FieldScene.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiChatMan/uiChatMan_def.h"

TestUIScene_Ext01 g_TestUIScene_Ext01;

TestUIScene_Ext01::TestUIScene_Ext01(void)
{
	m_SceneType = SCENE_TYPE_TESTUI;
}

TestUIScene_Ext01::~TestUIScene_Ext01(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext01::Activate()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext01::Init()
{
	// 인터페이스 랜더링 온
	g_InterfaceManager.SetRenderState( TRUE );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext01::Process( DWORD dwTick )
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->Process(dwTick);
		itr++;
	}

	ProcessKeyInput(dwTick);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene_Ext01::Render( DWORD dwTick )
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->Render(dwTick);
		itr++;
	}

	

	g_pSunRenderer->Set2DMode( TRUE, FALSE );
	RenderText();
	if (g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG))
    {
        ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

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
TestUIScene_Ext01::Release()
{
	TestUIScene::Release();

	GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG)->ShowDialog(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext01::ParsePacket( MSG_BASE * pMsg )
{
	TestUIScene::ParsePacket(pMsg );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext01::RenderText( void )
{
	TestUIScene::RenderText(0);

	// 준비 과정
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );

	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene_Ext01::ProcessKeyInput(DWORD dwTick)
{
	TestUIScene::ProcessKeyInput();

	if ( g_Input.GetState(DIK_I, KS_DOWN ) || g_Input.GetState(DIK_I, IKS_DOWN ))
	{
		this->m_bScanMode ^= TRUE;
	}

	if ( m_bScanMode && g_Input.IsPressAnyKeyExceptExt() )
	{	
		util::_tstring strKeyInfo;
		KeyCodeVec vecKeyCode;
		if ( g_Input.ScanCurKeyInfo( vecKeyCode ) )
		{
			g_Input.ConvertCodeToStr(vecKeyCode, strKeyInfo);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, (TCHAR *)(const TCHAR *)strKeyInfo.c_str());
		}
	}

	if ( g_Input.GetState(DIK_C, KS_DOWN ) )
	{
		g_MissionScene.verifyDeadMessageBox();
	}
	else if ( g_Input.GetState(DIK_F, KS_DOWN)) 
	{
		g_FieldScene.verifyDeadMessageBox();
	}
	else if ( g_Input.GetState(DIK_H, KS_DOWN)) 
	{
		g_CompetitionScene.ConfirmDeadMessageBox();
	}
}



//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
