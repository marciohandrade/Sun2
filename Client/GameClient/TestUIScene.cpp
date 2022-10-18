//------------------------------------------------------------------------------
//  TestUIScene.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "testuiscene.h"
#include "InterfaceManager.h"
#include "ChatDialog.h"
#include "IconMgr.h"
#include "BattleRoomEntry.h"
#include "Application.h"
#include "TestCase.h"
#include "TestCaseFactory.h"
#include "ObjectManager.h"
#include "Map.h"
#include "HeroActionInput.h"
#include "ProjectileManagerEx.h"
#include "Camera.h"
#include "BattleScene.h"

#include "uiChatMan/uiChatMan_def.h"

TestUIScene g_TestUIScene;

//------------------------------------------------------------------------------
/**
*/
TestUIScene::TestUIScene(void)
{
	this->m_SceneType = SCENE_TYPE_TESTUI;
}

//------------------------------------------------------------------------------
/**
*/
TestUIScene::~TestUIScene(void)
{
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene::Activate()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene::Init()
{
	if ( g_pMap )
		g_pMap->SetRenderFlag(FALSE);

#ifndef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.Create();
#endif

	g_InterfaceManager.SetRenderState(TRUE);

	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->Init();
		itr++;
	}

	//	인터페이스 랜더링 온
	g_InterfaceManager.SetRenderState( TRUE );
	m_bFristRender=TRUE;

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene::Process( DWORD dwTick )
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->Process(dwTick);
		itr++;
	}

	BattleScene::Process(dwTick);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
TestUIScene::Render( DWORD dwTick )
{
	BattleScene::Render(dwTick);

	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->Render(dwTick);
		itr++;
	}

	g_pSunRenderer->Set2DMode( TRUE, FALSE );
	
	POINT pt = g_pApplication->Get2DModeMapping();

	//g_pSunRenderer->RenderTexture( m_hBackGround[1], 0.0f, 0.0f, pt.x, pt.y, 0.0f, 0.0f );	
	g_pSunRenderer->Set2DMode( FALSE, FALSE );

	g_pSunRenderer->x_pManagerPostEffect->Apply();

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene::Release()
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->Release();
		delete itr->second;
		itr++;
	}

	this->m_TestCaseMap.clear();

	g_InterfaceManager.HideAllWindows();
    ICONMGR()->RemoveAll();

#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
	g_ObjectManager.Destroy();
#endif

	BattleScene::Release();
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene::ParsePacket( MSG_BASE * pMsg )
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->ParsePacket(pMsg);
		itr++;
	}

	BattleScene::ParsePacket(pMsg);
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene::RenderText( DWORD dwTick  )
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->RenderText();
		itr++;
	}

	// 준비 과정
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "DFLT" );

	g_pSunRenderer->x_pManagerTextOut->DrawText( _T("Test UI Scene"), 5, 5, WzColor_RGB( 255, 255, 255 ) );

	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void
TestUIScene::ProcessKeyInput()
{
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		itr->second->ProcessKeyInput(0);
		itr++;
	}

	if ( g_Input.GetState( DIK_RETURN, KS_UP ))
	{
		if (g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG) == FALSE)//if (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_CHAT) == FALSE)
		{
			ChatDialog *pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
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
	else if ( g_Input.GetState( DIK_RETURN, IKS_UP ) )
	{
		if (g_InterfaceManager.IsShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG)==TRUE)
		{
			ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
TestUIScene::AttachTestCase(const SUN_STRING & strTestCaseName)
{
	BOOL bRet = FALSE;
	TEST_CASE_ITR itr = this->m_TestCaseMap.find(strTestCaseName);		
	if ( itr == this->m_TestCaseMap.end() )
	{
		TestCase * pTestCase = TestCaseFactory::MakeTestCase( strTestCaseName );	
		if ( pTestCase )
		{
			pTestCase->Init();
			m_TestCaseMap.insert( TEST_CASE_PAIR( strTestCaseName, pTestCase) );
		}
	}
	else
	{
		//
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
TestUIScene::DeAttachTestCase(const SUN_STRING & strTestCaseName)
{
	BOOL bRet = FALSE;
	TEST_CASE_ITR itr = this->m_TestCaseMap.find(strTestCaseName);		
	if ( itr != this->m_TestCaseMap.end() )
	{
		TestCase * pTestCase = itr->second;
		pTestCase->Release();
		m_TestCaseMap.erase(itr);

	}
	else
	{
		//
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
TestUIScene::EntMessage(WORD wMessage, int iParam1, int iParam2, int iParam3/*= 0*/, int iParam4/*= 0*/)
{
	BOOL bRet = FALSE;
	TEST_CASE_ITR itr = this->m_TestCaseMap.begin();
	while ( itr != this->m_TestCaseMap.end())
	{
		if ( itr->second->EntMessage(wMessage, iParam1, iParam2, iParam3, iParam4) )
		{
			bRet = TRUE;
			break;
		}

		itr++;
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL		 
TestUIScene::RenderMap( DWORD dwTick )
{
	//-----------------------------------------------------------------------------------
	//		배경 필드 랜더링
	//-----------------------------------------------------------------------------------
	//	배경
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_FRONT );
	if (!g_pMap)
	{
		return FALSE;
	}

	g_pMap->Render(dwTick);
	
	//	임시 : 잔상 효과

	g_ProjectileManagerEx.Render();

	g_InterfaceManager.RenderMovePointer(dwTick);  // 이동포인터 렌더링
	g_InterfaceManager.RenderAreaPointer(dwTick);  // 지역지정 포인터 렌더링

	return TRUE;	
}

//-------------------------------------------------------------------------------------------
/**
*/
//void 
//TestUIScene::ProcessCamera( LPDIMOUSESTATE pMouseState )
//{
//	return;
//	if (!g_pHero)
//	{
//		return;
//	}
//
//	//	동영상 촬영용 프리 카메라
//	if( GENERALPARAM->IsFreeCameraMode() )
//	{
//		//	프리 카메라
//		if( GENERALPARAM->GetFreeCameraState() )
//		{
//			if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) )
//			{
//				g_Camera.Rotate( pMouseState->lX, pMouseState->lY );
//			}
//
//			if( g_Input.GetState( DIK_RBRACKET, KS_DOWN ) )
//			{
//				fFreeCameraSpeed += FREE_CAMERA_SPEED_UP;
//				if( fFreeCameraSpeed >= FREE_CAMERA_MAX_SPEED_UP )
//				{
//					fFreeCameraSpeed = FREE_CAMERA_MAX_SPEED_UP;
//				}
//			}
//			else if( g_Input.GetState( DIK_LBRACKET, KS_DOWN ) )
//			{
//				fFreeCameraSpeed -= FREE_CAMERA_SPEED_UP;
//				if( fFreeCameraSpeed <= FREE_CAMERA_MIN_SPEED_UP )
//				{
//					fFreeCameraSpeed = FREE_CAMERA_MIN_SPEED_UP;
//				}
//			}
//
//			if( g_Input.GetState( DIK_W, KS_KEY ) )
//			{
//				m_FreeCameraVector -= (g_Camera.m_CameraDirection * (GENERALPARAM->GetFreeCameraSpeed() * fFreeCameraSpeed) );
//			}
//			else if( g_Input.GetState( DIK_S, KS_KEY ) )
//			{
//				m_FreeCameraVector += (g_Camera.m_CameraDirection * (GENERALPARAM->GetFreeCameraSpeed() * fFreeCameraSpeed) );
//			}
//			if(  g_Input.GetState( DIK_A, KS_KEY ) )
//			{
//				g_Camera.RotateYaw( GENERALPARAM->GetFreeCameraTurnSpeed() );
//			}
//			else if( g_Input.GetState( DIK_D, KS_KEY ) )
//			{
//				g_Camera.RotateYaw( -GENERALPARAM->GetFreeCameraTurnSpeed() );
//			}
//			if( g_Input.GetState( DIK_Q, KS_KEY ) )
//			{
//				WzVector RightVector;
//				WzVector UpVector;
//				SetVector( &UpVector, 0.0f, 0.0f, 1.0f );
//				VectorCrossProduct( &RightVector, &g_Camera.m_CameraDirection, &UpVector );
//				VectorNormalize( &RightVector, &RightVector );
//				m_FreeCameraVector += (RightVector * (GENERALPARAM->GetFreeCameraSpeed() * fFreeCameraSpeed) );
//			}
//			else if( g_Input.GetState( DIK_E, KS_KEY ) )
//			{
//				WzVector RightVector;
//				WzVector UpVector;
//				SetVector( &UpVector, 0.0f, 0.0f, 1.0f );
//				VectorCrossProduct( &RightVector, &g_Camera.m_CameraDirection, &UpVector );
//				VectorNormalize( &RightVector, &RightVector );
//				m_FreeCameraVector -= (RightVector * (GENERALPARAM->GetFreeCameraSpeed() * fFreeCameraSpeed) );
//			}
//
//			g_Camera.SetCamera( &(m_FreeCameraVector), FALSE );
//			g_Camera.UpdateCamera();
//
//			return;
//		}
//	}
//
//
//	WzVector vDirection = g_pHero->GetDirection();	
//	g_Camera.SetAngle( &vDirection );
//
//	//	마우스로 카메라 회전
//	if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) )
//	{
//		g_Camera.Rotate( pMouseState->lX, pMouseState->lY );
//	}
//	else if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY ) )
//	{
//		g_Camera.Rotate( pMouseState->lX, pMouseState->lY, Camera::CAMERA_ROTATE_RMOUSE );
//	}
//	else if (g_Input.GetState( MOUSE_MBUTTON, KS_MOUSE_DOWN ))
//	{
//		g_Camera.SetYaw( g_pHero->GetAngle() );
//		g_Camera.Rotate(0,0);	
//	}
//	else
//	{
//		//	카메라 키보드 회전
//		const int		iKeyRotateSpeed = 30;
//		if( g_Input.GetState( DIK_SEMICOLON, KS_KEY) )
//		{
//			g_Camera.Rotate( -iKeyRotateSpeed, 0, Camera::CAMERA_ROTATE_RMOUSE );
//		}
//		else if( g_Input.GetState( DIK_APOSTROPHE, KS_KEY ) )
//		{
//			g_Camera.Rotate( iKeyRotateSpeed, 0, Camera::CAMERA_ROTATE_RMOUSE );
//		}
//		else if( g_pHero->GetCurState() == STATE::KEYBOARDMOVE) 
//		{
//			g_Camera.KeyboardModeInterpolateAngle(g_pHero->GetAngle());
//		}
//		else
//		{
//			if( g_pHero->IsMoving() )
//			{
//				// 일반 이동시 카메라 자동회전 온/오프
//				if( GENERALPARAM->GetGetCameraRotaion() )
//				{
//					g_Camera.InterpolateAngle();
//				}
//			}
//		}
//
//
//		//	카메라 줌인/줌아웃
//		const int		iZoomSpeed = 120;
//		if(  g_Input.GetState( DIK_COMMA, KS_KEY ) )
//		{
//			g_Camera.MoveFrontBack( iZoomSpeed );
//		}
//		else if( g_Input.GetState( DIK_PERIOD, KS_KEY ) )
//		{
//			g_Camera.MoveFrontBack( -iZoomSpeed );
//		}
//
//
//
//		g_Camera.Rotate(0,0);
//		g_Camera.SetRotateState( FALSE );
//		g_Camera.RotateAngleCheck( 0 );
//		g_Camera.SetStartRotateCheck( FALSE );
//	}
//
//	g_Camera.MoveFrontBack( pMouseState->lZ );
//	g_Camera.SetCamera( &(g_pHero->GetVisiblePos()), TRUE );	
//	g_Camera.UpdateCamera();
//}


//------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------
