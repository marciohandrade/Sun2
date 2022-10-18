//------------------------------------------------------------------------------
//  LoginScene.cc
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "GameFramework.h"
#include "LoginScene.h"
#include "Player.h"

#include "varpacket.h"
#include "Camera.h"
#include "ItemManager.h"
#include <SCSlotHeader.h>
#include "LoadScene.h"
#include "ResourceManager.h"
#include "LoadScene.h"
#include "InterfaceManager.h"
#include "ObjectManager.h"
#include "uiLoginMan/uiLoginMan.h"
#include "uiLoginBack/uiLoginBack.h"
#include "uiLoginNotice/uiLoginNotice.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "Version.h"
#include "BGMSoundInfoParser.h"
#include "SoundEffect.h"
#include "MouseCursor.h"
#include "Map.h"
#include "CursorChangeTransact.h"
#include "Application.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "SystemDialog.h"
#include "BattleScene.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "TextureAnimationManager.h"
#include "XignCode/XignCodeApi.h"

#include "LoginGameParam.h"
#include "Mouse.h"

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "uiLoginOneTimePassword/uiLoginOneTimePassword.h"

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#include "uiSUNRankingMan/uiSUNRankingDialog.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM

LoginScene      g_LoginScene;

const int FRAME_PER_LOGO = 34;




//------------------------------------------------------------------------------
/**
*/
LoginScene::LoginScene() :
    isConnected(false),
    isConfirmed(false),
    bReconnect(false),
    bReconnectTimer(false),
    dwReconnectTick(0),
    dwReconnectTickOver(0),
    accountTries(0),
    onceAutoConnectionTry(TRUE),
    bSceneInitialized(FALSE),
	save_mouse_pointer_x_(-1),
	save_mouse_pointer_y_(-1),
	delay_time_mouseMove_(10),
	off_ui_login_(false)
{
    m_SceneType = SCENE_TYPE_LOGIN;
}

//------------------------------------------------------------------------------
/**
*/
LoginScene::~LoginScene()
{

}

//------------------------------------------------------------------------------
/**
*/
void
_CallBack_Confirm(bool bYes)
{
    if (bYes)
    {
        SendMessage(g_hWndMain, WM_CLOSE, 0, 0);
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL
LoginScene::Init()
{
#ifdef _NHN_USA_CHANNELING
	//gs_ps:웹런처실행시 로그인창이 없기때문에 사실여기를 서버선택스테이지로봐도무방
	//		MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK은 수시로 호출되기때문에 이곳에 삽입했씀
	GlobalFunc::SendHanReportPacket(604);//서버선택스테이지로이동완료(604)
#endif//_NHN_USA_CHANNELING(북미 채널링:지표)

	g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_SYSTEM, FALSE );

	SystemDialog * pSystemDialog = (SystemDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SYSTEM );
	if ( pSystemDialog )
	{
		pSystemDialog->ResetPacketStatus();
	}

	BattleScene::m_bPendingBackToSomeWhere = false;

    isConnected = false;
    isConfirmed = false;
    bReconnect = false;
    bReconnectTimer = false;
    dwReconnectTick = 0;
    dwReconnectTickOver = 0;
    accountTries = 0;
    onceAutoConnectionTry = TRUE;
    bSceneInitialized = FALSE;
	
	delay_time_mouseMove_ = 10;

	CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Default );
	
    SceneBase::Init();

    // 인터페이스 렌더링 활성화
    g_InterfaceManager.SetRenderState( TRUE );

    BOOL	bRet = TRUE;

    this->Disconnect();
	bRet = this->Connect();

    // 오브젝트 메니저 생성
#ifndef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.Create();
#endif

    // 맵에 자동 카메라를 적용한다.
    g_pMap->SetUseAutoCamera(TRUE);

    // 관련된 유저 인터페이스를 띄어준다.
    GET_DIALOG( IM_LOGIN_MANAGER::LOGIN_BACK )->ShowInterface( TRUE );
    uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
    if (login_back_top != NULL)
    {
        login_back_top->ShowInterface(TRUE);
        login_back_top->SetSceneTitle(0);
    }

	if (g_pMap)
	{
		GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_LOBBY);
	}

	LoadSunAnimation();

    if (!bRet)
    {
        ReleaseSunAnimation();
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		//	인증 서버로부터 응답이 없습니다. 프로그램을 종료 하신 후 공지를 확인해주세요.
		g_InterfaceManager.GetInterfaceString( eST_CAN_NOT_CONNECT_AUTHSERVER, szMessage, INTERFACE_STRING_LENGTH );
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage, TRUE, 1, 30);

		return TRUE;
    }


	g_KeyQueueManager.Reset();
	g_KeyQueueManager.SetKey(DIK_ESCAPE);
	g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoLogin);

	// 프로토콜 암호화 정보 초기화
    if (g_pNetwork)
    {
	    g_pNetwork->InitCryptographyInfo();
    }

	g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_DEPTH_OF_FIELD);
	g_pMap->EnableBlur(GAMEOPTION->GetBlurState());

    g_pSunRenderer->EnableFog(GAMEOPTION->GetFogState());


    GAMEOPTION->ApplyOptionValue(nOptionType::TEXTURE_QUALITY, eOPTION_LEVEL_HIGH);


    g_Camera.SetProjection(g_pMap->GetMapNearClip(),g_pMap->GetMapFarClip(),CAMERA_DEFAULT_FOV);


    TextureAnimationManager::Instance()->Load(kTexAniGroup_Sword);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_SUN);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_EP2);
    TextureAnimationManager::Instance()->Load(kTexAniGroup_EP2_Effect);

    TextureAnimationManager::Instance()->Play(kTexAniGroup_Sword);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_SUN);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_EP2);
    TextureAnimationManager::Instance()->Play(kTexAniGroup_EP2_Effect);

#if USING_XIGNCODE
    ZeroMemory(XignCodeApi::xigncode_user_info(), sizeof(XignCodeApi::XignCode_UserInfo));
#endif //USING_XIGNCODE

	g_pEffectManager->UnsetOption(EOPT_AUTOEFFLVLCTL | EOPT_APPLYEFFLOD | EOPT_APPLYLOD_LOOPONLY);
	g_pEffectManager->UnsetOption(EOPT_AUTOEFFRATECTL);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    ChaosSystem::ChaosZoneData::ReleaseMutexHandle();
#else
    ChaosSystem::GetBattleGroundData().ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
	uiSUNRankingDialog* sun_ranking_dialog = GET_CAST_DIALOG(uiSUNRankingDialog, IM_SUNRANKING_MANAGER::SUNRANKING_DIALOG);
	if ( sun_ranking_dialog )
	{
		sun_ranking_dialog->ResetSettleDate();
		sun_ranking_dialog->ResetSUNRankingDialog();
	}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM

    SetLoginProgressState(0);

	return bRet;
}

//------------------------------------------------------------------------------
/**
*/
void
LoginScene::Release()
{
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_Sword);
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_SUN);
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_EP2);
    TextureAnimationManager::Instance()->Clear(kTexAniGroup_EP2_Effect);

	ReleaseSunAnimation();

	this->Disconnect();

    // 오브젝트 메니저 해제
#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
    g_ObjectManager.Destroy();
#endif

    // 멥 Blur 효과를 해제한다.
    g_pMap->EnableBlur(FALSE);
    
    //! 포그를 옵션에 있는 값으로 셋팅
    g_pSunRenderer->EnableFog(GAMEOPTION->GetFogState());

    
    GAMEOPTION->ApplyOptionValue(nOptionType::TEXTURE_QUALITY, GAMEOPTION->GetGameOption().GetTextureQuality());


    // 맵 해제
    g_pMap->Destroy();

    // 관련된 유저 인터페이스를 닫는다.
	GET_DIALOG( IM_LOGIN_MANAGER::LOGIN_BACK )->ShowInterface( FALSE );

    uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
    if (login_back_top != NULL)
    {
        login_back_top->ShowInterface(FALSE);
    }

	uiLoginMan* pLoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);

	if(pLoginMan)
	{
		pLoginMan->HideAllDialogsRelease();
	}

#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
    // no operation
#else //_COMMON_CHANNELING_LOGIC_DEFINE
	GET_DIALOG( IM_LOGIN_MANAGER::LOGIN_ACCOUNT )->ShowInterface(FALSE);
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
	
	g_pEffectManager->SetOption(EOPT_AUTOEFFLVLCTL | EOPT_APPLYEFFLOD | EOPT_APPLYLOD_LOOPONLY);


	// 자동 이펙트 생성 비율 조절
	if (PROGRAMPARAM->GetAutoEffectRate() >= 1)
	{
		g_pEffectManager->SetOption( EOPT_AUTOEFFRATECTL );
	}
	else
	{
		g_pEffectManager->UnsetOption( EOPT_AUTOEFFRATECTL );
	}
}

//------------------------------------------------------------------------------
/**
*/
void
LoginScene::Activate()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL
LoginScene::Process( DWORD dwTick )
{
    SceneBase::Process(dwTick);

    // 맵 업데이트
    g_pMap->Update(dwTick);

    // 오브젝트의 프로세스를 진행한다.
    Object_Map_Itr	itr = g_ObjectManager.GetBegin();

    while( g_ObjectManager.GetEnd() != itr )
    {
        Object *pObj = (Object *)itr->second;
		if(pObj)
		{
			pObj->Process(dwTick);
		}

        ++itr;
    }

    // 맵에서 얻어온 (맵툴에서 정한) 카메라 위치와 바라보는 방향과 목표를 설정한다.
    WzVector wzCamera, wzTarget;
    WzVector wzDir,wzBase;
#ifdef _NA_000000_20130114_RENEWER_UI
	if(LOGINPARAM->GetUserCamera())
	{
		SetVector(&wzTarget, LOGINPARAM->GetLogineCameraLookX() , 
								LOGINPARAM->GetLogineCameraLookY() , 
								LOGINPARAM->GetLogineCameraLookZ() );

		SetVector(&wzCamera, LOGINPARAM->GetLogineCameraEyeX(), 
								LOGINPARAM->GetLogineCameraEyeY(), 
								LOGINPARAM->GetLogineCameraEyeZ());

		wzDir = wzTarget - wzCamera;
		wzBase = wzDir;
		wzBase.z = 0.0f;
		float fBase = VectorLength(&wzBase);
		g_Camera.SetCameraDistance(fBase);
		float fYaw = Math_GetAngleFromVector(&wzDir);
		g_Camera.SetYaw( fYaw );
		g_Camera.SetPitch(atan( wzDir.z / fBase));
		g_Camera.SetCameraFromTo(wzCamera,wzTarget);
		g_Camera.SetCameraPosition(&wzCamera);
	}
    else if( CAMERA_EXTRA_INFO * camExtra = g_pMap->GetExtraCameraInfo() )
    {
        SetVector(&wzTarget, camExtra->WzTargetPos.x, camExtra->WzTargetPos.y, camExtra->WzTargetPos.z);
        SetVector(&wzCamera, camExtra->WzPos.x, camExtra->WzPos.y, camExtra->WzPos.z);
        wzDir = wzTarget - wzCamera;
        wzBase = wzDir;
        wzBase.z = 0.0f;
        float fBase = VectorLength(&wzBase);
        g_Camera.SetCameraDistance(fBase);
        float fYaw = Math_GetAngleFromVector(&wzDir);
        g_Camera.SetYaw( fYaw );
        g_Camera.SetPitch(atan( wzDir.z / fBase));
        g_Camera.SetCameraFromTo(wzCamera,wzTarget);
        g_Camera.SetCameraPosition(&wzCamera);
    }
#else
 	if( CAMERA_EXTRA_INFO * camExtra = g_pMap->GetExtraCameraInfo() )
    {
        SetVector(&wzTarget, camExtra->WzTargetPos.x, camExtra->WzTargetPos.y, camExtra->WzTargetPos.z);
        SetVector(&wzCamera, camExtra->WzPos.x, camExtra->WzPos.y, camExtra->WzPos.z);

        wzDir = wzTarget - wzCamera;
        wzBase = wzDir;
        wzBase.z = 0.0f;
        float fBase = VectorLength(&wzBase);
        g_Camera.SetCameraDistance(fBase);
        float fYaw = Math_GetAngleFromVector(&wzDir);
        g_Camera.SetYaw( fYaw );
        g_Camera.SetPitch(atan( wzDir.z / fBase));
        g_Camera.SetCameraFromTo(wzCamera,wzTarget);
        g_Camera.SetCameraPosition(&wzCamera);
    }
#endif

    g_Camera.UpdateCamera();

    // 재접속 타이머를 필요에 따라 dwTick단위로 계산한다.
    if (bReconnectTimer)
    {
        dwReconnectTick += dwTick;

		if (dwReconnectTick > dwReconnectTickOver)
        {
            if (!this->Connect())
                bReconnect = true;
            else
                bReconnectTimer = false;
        }
    }

    if (bReconnect)
    {
        dwReconnectTick = dwTick;
        dwReconnectTickOver = dwTick + (1*1000);
        bReconnect = false;
        bReconnectTimer = true;
    }
 
    if( GetLoginProgressState() == 0 )
    {
        ///////////////////////////////////////////////////////////////////////////////////////v
        // Process LoginAccount Visible
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
        uiLoginOneTimePassword* pLoginOneTimePassword = GET_CAST_DIALOG(uiLoginOneTimePassword, IM_LOGIN_MANAGER::LOGIN_ONETIMEPASSWORD);
        if( pLoginOneTimePassword->IsVisible() == TRUE )
        {
            return TRUE;
        }
#endif //
     
	    int posx = Mouse::Instance()->GetMouseX();
	    int posy = Mouse::Instance()->GetMouseY();

 	    if(save_mouse_pointer_x_ == posx && save_mouse_pointer_y_ == posy)
	    {
		    same_position_to_mouse_ = true;
	    }
	    else
	    {
		    uiLoginMan* pLoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);

		    if(pLoginMan)
		    {
			    pLoginMan->ShowOnOffUI(true);
		    }

		    same_position_to_mouse_ = false;
		    off_ui_login_ = false;
		    delay_time_mouseMove_ = 30.0f;
	    }

	    uiLoginMan* pLoginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);

	    if(same_position_to_mouse_ && pLoginMan)
	    {
		    if( pLoginMan->GetTransTime())
		    {
			    if(off_ui_login_ == false)
			    {
				    delay_time_mouseMove_ -= dwTick/1000.0f;
			    }

			    if(delay_time_mouseMove_ < 0.0f)
			    {
				    off_ui_login_ = true;
				    delay_time_mouseMove_ = 30.0f;

				    pLoginMan->ShowOnOffUI(false);
			    }
		    }
		    else
		    {
			    off_ui_login_ = false;
			    delay_time_mouseMove_ = 30.0f;
		    }
    		
	    }

	    save_mouse_pointer_x_ = posx;
	    save_mouse_pointer_y_ = posy;
        ///////////////////////////////////////////////////////////////////////////////////////v
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
LoginScene::Render( DWORD dwTick )
{

    // 맵을 그린다.
    g_pMap->Render(dwTick);


	if(g_pEffectManager)
		g_pEffectManager->Move( dwTick );


	g_pSunRenderer->x_pManagerPostEffect->Apply();
	
	SceneBase::Render(dwTick);

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
LoginScene::Connect()
{
    return g_pApplication->ConnectToAuthServer();
}

//------------------------------------------------------------------------------
/**
*/
void
LoginScene::Disconnect()
{
	if (g_pNetwork && (g_pNetwork->IsConnected(CI_AUTHSERVERIDX) == TRUE))
    {
		g_pNetwork->Disconnect(CI_AUTHSERVERIDX);
    }

    this->isConnected = false;
    this->isConfirmed = false;
    this->accountTries = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
LoginScene::Reconnect()
{
    this->Disconnect();
    this->bReconnect = true;

    // 관련된 유저 인터페이스를 재접속에 의해 새로 띄어준다.
}

//------------------------------------------------------------------------------
/**
*/
void
LoginScene::ParsePacket( MSG_BASE * pMsg )
{
    SceneBase::ParsePacket(pMsg);

    switch( pMsg->m_byCategory )
    {
		case CATEGORY_AUTH:
        {
			GET_DIALOG( IM_LOGIN_MANAGER::LOGIN_SERVERLIST )->NetworkProc(pMsg);
        }
        break;
    }
}

void LoginScene::SetViewport()
{
}

void LoginScene::LoadSunAnimation()
{
}

void LoginScene::ReleaseSunAnimation()
{
}

BOOL LoginScene::PreInterfaceRender(DWORD dwTick)
{
	return TRUE;
}

BOOL LoginScene::PostInterfaceRender(DWORD dwTick)
{
    RenderSunAnimation(dwTick);

	return TRUE;
}


void LoginScene::RenderSunAnimation(DWORD dwTick)
{
    g_pSunRenderer->Set2DMode(TRUE);
    BOOL bZTest = g_pSunRenderer->IsEnableZTest();
    DWORD dwRef;
    ENUM_CMP_FUNC eMode;
    g_pSunRenderer->GetAlphaTestMode(&dwRef, &eMode);
    g_pSunRenderer->SetAlphaTestMode(0);

    TextureAnimationManager::Instance()->Render();

    g_pSunRenderer->SetAlphaTestMode(dwRef,eMode);
    g_pSunRenderer->SetZBufferTest(bZTest);
    g_pSunRenderer->Set2DMode(FALSE);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
