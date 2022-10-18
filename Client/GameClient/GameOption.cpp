//////////////////////////////////////////////////////////////////////////
//
//		게임 옵션
#include "SunClientPrecompiledHeader.h"
#include "gameoption.h"
#include "Sound.h"
#include "ChatMsg.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "GameOptionListParser.h"
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION


CGameOption::CGameOption()
: CGameParameter("GameData.dat"),
  m_bExceptionSize(FALSE)
{
	ZeroMemory( &m_OptionParam, sizeof(OptionParam) );

	m_ScreenSettingCount = 0;

	//	파일의 속성을 먼저 풀어준다
	SetFileAttributesA( m_szFullPath, FILE_ATTRIBUTE_NORMAL );
}

CGameOption::~CGameOption()
{
	Save();
	
	m_ScreenSettingArray.Clear();
}

void CGameOption::UpdateScreenSettingArray()
{
    const int c_iMaxCount = 256;
    St_ScreenSetting pGetSS[c_iMaxCount];
    int iMaxCount = g_pSunRenderer->GetAvailableScreenSettings( pGetSS, c_iMaxCount );
    if( iMaxCount >= c_iMaxCount )
    {
        assert( !"리스트 항목이 너무 많다! 256개 넘어갔다! 이런~" );
    }

    m_ScreenSettingCount = 0;
    m_ScreenSettingArray.Clear();

    int iCount = 0;

    char text[1024];

    //	지원하는 해상도가 있는 지 검사하여 필터링
    for( int j = 0 ; j < iMaxCount ; ++j )
    {
        if(pGetSS[j].m_iWidth>=640&&pGetSS[j].m_iHeight>=480)
        {
            SCREEN_SETTING temp;

            temp.ScreenSetting.m_iWidth = pGetSS[j].m_iWidth;
            temp.ScreenSetting.m_iHeight = pGetSS[j].m_iHeight;
            temp.ScreenSetting.m_iBpp = pGetSS[j].m_iBpp;
            temp.iOriginalScreenSetting = j;

            Sprintf(text, _T("%d x %d x %d"), pGetSS[j].m_iWidth, pGetSS[j].m_iHeight, pGetSS[j].m_iBpp );
            temp.SettingText = text;
            m_ScreenSettingArray.Append(temp);
            ++iCount;
        }
    }

    m_ScreenSettingCount = iCount;
}


BOOL CGameOption::Load()
{
    // 1. 엔진에서 로드된 해상도 정보를 가져온 후 세팅
	St_ScreenFormat SaveScreenFormat;
	g_pSunRenderer->GetScreenFormat( &SaveScreenFormat );

	St_RenderOption SaveOption;
	g_pSunRenderer->GetRenderOption(&SaveOption);

    int width = SaveScreenFormat.m_dwWidth;
    int height = SaveScreenFormat.m_dwHeight;
    int bpp = SaveScreenFormat.m_iRed == 8 ? 24 : 16;


#if (WZENGINEVER >= 360)
    if (SaveScreenFormat.m_bFullScreen)
    {
        m_OptionParam.SetScreenMode( OptionParam::kScreenMode_Full );
    }
    else if (SaveScreenFormat.m_bMaximizeScreen)
    {
        m_OptionParam.SetScreenMode( OptionParam::kScreenMode_MaxWindow );
    }
    else
    {
        m_OptionParam.SetScreenMode( OptionParam::kScreenMode_Window );
    }
#endif //(WZENGINEVER >= 360)

    m_OptionParam.SetScreenSize( width, height );
    m_OptionParam.SetScreenBitPerPixel( bpp );

    // 2. 사용가능한 해상도 목록을 구성
    UpdateScreenSettingArray();


	// 3. 엔진에서 로드된 해상도와 매칭되는 해상도로 선택
    // 기본 해상도인 1024 768로 맞춰놓고
	m_OptionParam.SetScreenIndex(0);

	// 적절한 해상도를 세팅
	for (int a = 0; a < m_ScreenSettingCount; a++)
	{
		if ((m_ScreenSettingArray[a].ScreenSetting.m_iBpp == bpp ) &&
			(m_ScreenSettingArray[a].ScreenSetting.m_iWidth == width) &&
			(m_ScreenSettingArray[a].ScreenSetting.m_iHeight == height))
		{
            m_OptionParam.SetScreenIndex(a);
			break;
		}
	}

    m_OptionParam.Load( this );

#ifdef _DH_OS_LANGUAGETYPE
    LanguageType DefaultLanguageType = GetOSUserDefaultLanguage();
    SetLanguageType( DefaultLanguageType );
#endif //_DH_OS_LANGUAGETYPE

	return TRUE;
}

#ifdef _DH_OS_LANGUAGETYPE
LanguageType CGameOption::GetOSUserDefaultLanguage()
{
	LanguageType language_type = kLanguageGlobal;
	LANGID language_code = 0;
	switch(language_code = GetUserDefaultLangID())
	{
#ifdef _DEV_VER
	case 0x0412://Korean 
		language_type = kLanguageKorea;
		break;
	case 0x411://Japanese
		language_type = kLanguageJapan;
		break;
	case 0x0404://Chinese (Taiwan)
		language_type = kLanguageTaiwan;
		break;
	case 0x0804://Chinese (PRC) 
	case 0x0c04://Chinese (Hong Kong SAR, PRC)
	case 0x1004://Chinese (Singapore) 
	case 0x1404://Windows 98/Me, Windows 2000/XP: Chinese (Macao SAR) 
		language_type = kLanguageChina;
		break;
		/*case 0x0419://Russian 
		language_type = kLanguageRussian;
		break;*/
#endif//_DEV_VER
	case 0x1407://German (Liechtenstein) 
	case 0x1007://German (Luxembourg) 
	case 0x0c07://German (Austria) 
	case 0x0407://German (Standard) 
	case 0x0807://German (Switzerland) 
		language_type = kLanguageGermany;
		break;
	default:    //English
		language_type = kLanguageGlobal;
		break;
	}
	return language_type;
}
#endif//_DH_OS_LANGUAGETYPE

BOOL CGameOption::Save()
{
    m_OptionParam.Save( this );


	St_ScreenFormat SaveScreenFormat;
	g_pSunRenderer->GetScreenFormat( &SaveScreenFormat );

	St_RenderOption SaveOption;
	g_pSunRenderer->GetRenderOption(&SaveOption);


	char szCurDirectory[512];
	char szFileName[512];
	ZeroMemory(szCurDirectory,sizeof(szCurDirectory));
	ZeroMemory(szCurDirectory,sizeof(szFileName));
	GetCurrentDirectoryA(512, szCurDirectory); 
	sprintf( szFileName, "%s\\%s", szCurDirectory, "3dsetup.ini");

	//GS_BUG_FIX:최대해상도시 정상적으로 윈도우사이즈를 변경하지못한다.
	if( GAMEOPTION->IsExceptionSize())
	{
		GAMEOPTION->GetExceptionSize(SaveScreenFormat);
	}
	SaveScreenFormatAndRenderOption( szFileName,&SaveScreenFormat,&SaveOption);

	return TRUE;
}

#include "Application.h"
#include "InterfaceManager.h"
#include "Camera.h"
#include "uiQuestMan/uiQuestMan.h"
#include "GameFunc.h"
#include "Map.h"
#include "HeroData.h"
#include "Mouse.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "uiHelpMan/uiHelpMan.h"
#include "GlobalFunc.h"
#include "SoundBGM.h"


void CGameOption::ApplyScreenOption()
{

    // 현재의 설정
    St_ScreenFormat ScreenFormat; 
    g_pSunRenderer->GetScreenFormat( &ScreenFormat );

    St_ScreenFormat backup_screen_format = ScreenFormat;

    const SCREEN_SETTING& ScreenSetting = GAMEOPTION->GetAvailableScreenSetting(m_OptionParam.GetScreenIndex());

    // 변경사항을 재확인
    BOOL IsChanged = FALSE;
    if( ScreenFormat.m_bFullScreen == TRUE )
    {
        if ( m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_Full)
        {
            IsChanged = TRUE;
        }
    }
    else if( ScreenFormat.m_bMaximizeScreen == true)
    {
        if (m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_MaxWindow)
        {
            IsChanged = TRUE;
        }
    }
    else
    {
        if (m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_Window)
        {
            IsChanged = TRUE;
        }
    }

    if( (ScreenSetting.ScreenSetting.m_iWidth != ScreenFormat.m_dwWidth) ||
        (ScreenSetting.ScreenSetting.m_iHeight != ScreenFormat.m_dwHeight))
    {
        IsChanged = TRUE;
    }

    // 변경 사항이 없음
    if( IsChanged == FALSE )
        return;

    BOOL backup_is_wide = g_pApplication->IsWideModeFlag();
    ENUM_SCREEN_RATIO backup_screen_ratio = g_pApplication->screen_ratio();

#ifdef _YMS_WINDOW_MAXSCREEN
    if ((m_OptionParam.GetScreenMode() != OptionParam::kScreenMode_Full) && 
        (ScreenSetting.ScreenSetting.m_iWidth == g_pApplication->desktop_screen_size().x) && 
        (ScreenSetting.ScreenSetting.m_iHeight == g_pApplication->desktop_screen_size().y))
    {
        m_OptionParam.SetScreenMode(OptionParam::kScreenMode_MaxWindow);
    }

    if (m_OptionParam.GetScreenMode() == OptionParam::kScreenMode_Full)
    {
        ScreenFormat.m_bFullScreen = TRUE;
        ScreenFormat.m_bMaximizeScreen = false;
        ScreenFormat.m_dwWidth = ScreenSetting.ScreenSetting.m_iWidth;
        ScreenFormat.m_dwHeight = ScreenSetting.ScreenSetting.m_iHeight;
    }
    else if (m_OptionParam.GetScreenMode() == OptionParam::kScreenMode_MaxWindow)
    {
        ScreenFormat.m_bFullScreen = FALSE;
        ScreenFormat.m_bMaximizeScreen = true;
        ScreenFormat.m_dwWidth = g_pApplication->desktop_screen_size().x;
        ScreenFormat.m_dwHeight = g_pApplication->desktop_screen_size().y;
    }
    else
    {
        ScreenFormat.m_bFullScreen = FALSE;
        ScreenFormat.m_bMaximizeScreen = false;
        ScreenFormat.m_dwWidth = ScreenSetting.ScreenSetting.m_iWidth;
        ScreenFormat.m_dwHeight = ScreenSetting.ScreenSetting.m_iHeight;
    }
#else
    ScreenFormat.m_dwWidth = ScreenSetting.ScreenSetting.m_iWidth;
    ScreenFormat.m_dwHeight = ScreenSetting.ScreenSetting.m_iHeight;
#endif //_YMS_WINDOW_MAXSCREEN

    //	와이드 모드 플래그 설정
    float fRate = (float)ScreenFormat.m_dwWidth / (float)ScreenFormat.m_dwHeight;

    if( ScreenSetting.ScreenSetting.m_iBpp == 24 )
    {
        ScreenFormat.m_iRed = 8;
        ScreenFormat.m_iGreen = 8;
        ScreenFormat.m_iBlue = 8;
    }
    else if( ScreenSetting.ScreenSetting.m_iBpp == 16 )
    {
        ScreenFormat.m_iRed = 5;
        ScreenFormat.m_iGreen = 5;
        ScreenFormat.m_iBlue = 5;
    }
    else
    {
        //	뭐지?
    }
#ifdef _YMS_WINDOW_MAXSCREEN
    bool is_window_size_changed = false;
    if ((backup_screen_format.m_dwWidth != ScreenFormat.m_dwWidth) ||
        (backup_screen_format.m_dwHeight != ScreenFormat.m_dwHeight) || 
        (backup_screen_format.m_bFullScreen != ScreenFormat.m_bFullScreen) || 
        (backup_screen_format.m_bMaximizeScreen != ScreenFormat.m_bMaximizeScreen))
    {
        is_window_size_changed = TRUE;
    }

    g_pSunRenderer->SelectMode(ScreenFormat);
    g_pSunRenderer->ChangeScreenFormat(&ScreenFormat, g_pApplication->GetHWnd(), is_window_size_changed);
#else
    g_pSunRenderer->SelectMode(ScreenFormat);
    g_pSunRenderer->ChangeScreenFormat(&ScreenFormat, g_pApplication->GetHWnd());
#endif //_YMS_WINDOW_MAXSCREEN

    RECT rcWindow;
    rcWindow.left = 0;
    rcWindow.top = 0;
    rcWindow.right = ScreenFormat.m_dwWidth;
    rcWindow.bottom = ScreenFormat.m_dwHeight;
    g_InterfaceManager.SetWindowRC(&rcWindow);

    g_Camera.SetAspectRatio(SR_AUTO);

    POINT	Point;
    Point.x = ScreenFormat.m_dwWidth;
    Point.y = ScreenFormat.m_dwHeight;
    g_pApplication->OnChangeRes(&Point );

    g_InterfaceManager.MoveDefaultWindowPos();

    //if (backup_screen_ratio != g_pApplication->screen_ratio())
    //{
    //    g_InterfaceManager.MoveDefaultWindowPos();
    //}

    if( uiQuestMan* pMan = GET_DIALOG_MANAGER( uiQuestMan, UIMAN_QUEST ) )
    {
        pMan->SetQuestViewProgressRect();
    }
}


void CGameOption::ApplyOptionValue( nOptionType::eOptionTypeIndex TypeIndex, int Value )
{
    switch( TypeIndex )
    {
        // DisplayOption

    case nOptionType::TEXTURE_QUALITY: 
        {
            float NewTextureRate = 0.25f;
            if( Value == eOPTION_LEVEL_HIGH )
                NewTextureRate = 1.0f;
            else if( Value == eOPTION_LEVEL_MIDDLE )
                NewTextureRate = 0.5f;
            else if ( Value == eOPTION_LEVEL_LOW )
                NewTextureRate = 0.25f;

            St_RenderOption Option;
            g_pSunRenderer->GetRenderOption(&Option);
            if( Option.m_fTextureRate == NewTextureRate )
                break;

            Option.m_fTextureRate = NewTextureRate;
            g_pSunRenderer->SetRenderOption(Option);

            g_pSunRenderer->x_pManagerTexture->ResizeAllTexture();
            break;
        }

    case nOptionType::SHOW_SHADOW:
        {
            ENUM_SHADOWMAP_MODE eType = g_pSunRenderer->x_pManagerShadowMap->ConfirmDevice();
            ENUM_SHADOW_TYPE eTypeToSet;

            // 그림자 퀄리티
            if( Value == TRUE ) 
                eTypeToSet = SWT_SHADOWMAP_NO_SELFSHADOW;
            else 
                eTypeToSet = SWT_DISABLED;

            switch (eType)
            {
                // 최고사양임 어떠한것도 필요없음
            case ESM_DEPTHSTENCIL:
            case ESM_R32F:
                break;

                // 프로젝션 쉐도우밖에 지원안함
            case ESM_PROTECTED_TEXTURE:
                {
                    // 쉐도우맵을 버퍼로 변환
                    if (eTypeToSet == SWT_SHADOWMAP_NO_SELFSHADOW || eTypeToSet == SWT_SHADOWMAP)
                    {
                        eTypeToSet = SWT_CIRCLESHADOW;
                    }
                }
                break;

                // 다 지원안함.
            case ESM_NONE:
                {
                    switch (m_OptionParam.GetShadowQuality())
                    {
                    case eOPTION_LEVEL_HIGH:			
                    case eOPTION_LEVEL_MIDDLE:				
                        eTypeToSet = SWT_CIRCLESHADOW;
                        break;

                    case eOPTION_LEVEL_LOW:
                    default:
                        eTypeToSet = SWT_DISABLED;
                        break;

                    }
                }
                break;
            }

            St_RenderOption Option;
            g_pSunRenderer->GetRenderOption(&Option);
            if( Option.m_eShadowType == eTypeToSet &&
                Option.m_bRenderShadow == Value )
                break;

            Option.m_eShadowType = eTypeToSet;
            Option.m_bRenderShadow = Value;

            g_pSunRenderer->SetRenderOption(Option);
            g_pSunRenderer->x_pManagerShadowMap->ResetShadowMode();
        }
        break;

    case nOptionType::LOD_LEVEL:
        {
            // LOD
            GameFunc::ApplyLODDistance( Value );
            GameFunc::ApplyEffectLODDistance( Value );
        }
        break;

    case nOptionType::USE_BLUR:
        {
            g_pMap->EnableBlur( Value );
        }
        break;
    case nOptionType::USE_FOG:
        {
            g_pSunRenderer->EnableFog( Value );
        }
        break;
    case nOptionType::USE_MAP_DISTANCE_VIEW:
        {
            if( Value )
                g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_DEPTH_OF_FIELD);
            else
                g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_DEPTH_OF_FIELD);
        }
        break;

    case nOptionType::EFFECT_QUALITY:
        {
            if( g_pEffectManager != NULL )
            {
#if WZENGINEVER >= 610
                int CurrentEffectLevel = g_pEffectManager->GetBaseEffectLevel();
#else
                int CurrentEffectLevel = -1;    // 항상 변경된다
#endif 

                // 현재 설정과 차이가 있는경우
                if( 3 - Value != CurrentEffectLevel )
                {
                    if( Value == 3 )
                    {
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);

                        g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource1.txt");
                        g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource2.txt");
                        g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");
                    }
                    else if( Value == 2 )
                    {
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1);

                        g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource2.txt");
                        g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");
                    }
                    else if( Value == 1 )
                    {
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1);
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource2.txt","Data\\Effect\\", EELT_ADD, 2);

                        g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");
                    }
                    else
                    {
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1);
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource2.txt","Data\\Effect\\", EELT_ADD, 2);
                        g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource3.txt","Data\\Effect\\", EELT_ADD, 3);
                    }


                    // 이펙트 기본 레벨 설정
                    g_pEffectManager->SetBaseEffectLevel( 3 - Value );
                }
            }
        }
        break;

    case nOptionType::GAMMA_VALUE:
        {
            DWORD Gamma = Value / 100;

            Gamma=min(255,Gamma);
            Gamma=max(0,Gamma);

            float fGammaRate = (float)Gamma / 255;

            WORD wSred[256];
            WORD wSgreen[256];
            WORD wSblue[256];


            for(int i=0;i<256;i++)
            {
                wSred[i] =WORD(g_D3dGammaRamp.red[i] * fGammaRate);
                wSred[i]=min(65535,wSred[i]);

                wSgreen[i] = WORD(g_D3dGammaRamp.green[i] * fGammaRate);
                wSgreen[i]=min(65535,wSgreen[i]);

                wSblue[i] = WORD(g_D3dGammaRamp.blue[i] * fGammaRate);
                wSblue[i]=min(65535,wSblue[i]);

            }

            g_pSunRenderer->SetGammaRamp(wSred,wSgreen,wSblue);
        }
        break;
    case nOptionType::USE_VOLUME_LIGHT:
        {
#ifdef _NA_006998_20131217_DISABLE_VOLUMLIGHT_OPTION
            g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_VOLUME_LIGHT);
#else // _NA_006998_20131217_DISABLE_VOLUMLIGHT_OPTION
            if( Value == TRUE )
                g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_VOLUME_LIGHT);
            else
                g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_VOLUME_LIGHT);
#endif // _NA_006998_20131217_DISABLE_VOLUMLIGHT_OPTION

        }
        break;


        // GameOption
    case nOptionType::MOUSE_SENS_VALUE:
        {
            Mouse::Instance()->SetMoveLevel(Value);
        }
        break;
    case nOptionType::MOUSE_VIEW_SENS_VALUE:
        {
            g_Camera.SetRotationLevel(Value);
        }
        break;
    case nOptionType::CAMERA_VIEW_HEIGHT_OFFSET:
        if( Value < 3 )
        {
            static float data[3] = {0.f, 0.35f, 0.45f};
            g_Camera.MoveUpDown( data[Value] );
        }
        break;

    case nOptionType::SHOW_HELMET:
        {
            
            BOOL bHide = !Value;

            if( g_pHero )
            {
                if( g_pHero->IsHideHelmet() != bHide )
                {
                    g_pHero->HideHelmet( bHide );

                    MSG_CG_STATUS_RENDER_OPTION_SYN msg;
                    msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION;
                    msg.m_byOptionLevel = bHide ? 1 : 0;

                    g_pHero->SendPacket(&msg,sizeof(msg));
                }
            }
            else
            {
                xGetHeroData()->SetHideHelmet( bHide );
            }
            
        }
        break;
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    case nOptionType::HIDE_COSTUME:
        {
            
            BOOL bHide = Value;
            if( g_pHero )
            {
                if( g_pHero->isHideCostume() != bHide )
                {
                    g_pHero->HideCostume( bHide );

                    MSG_CG_STATUS_RENDER_OPTION_SYN msg;
                    msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_COSTUME_OPTION;
                    msg.m_byOptionLevel = bHide ? 1 : 0;

                    g_pHero->SendPacket(&msg,sizeof(msg));
                }
            }
            else
            {
                xGetHeroData()->SetHideCostume( bHide );
            }
        }
        break;
#endif
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    case nOptionType::SHOW_WING:
        {
            BOOL bHide = Value;

            if( g_pHero )
            {
                if( g_pHero->IsHideWing() != bHide )
                {
                    g_pHero->HideWing( bHide );

                    MSG_CG_STATUS_RENDER_OPTION_SYN msg;
                    msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_WING_OPTION;
                    msg.m_byOptionLevel = bHide ? 1 : 0;

                    g_pHero->SendPacket(&msg,sizeof(msg));
                }
            }
            else
            {
                xGetHeroData()->SetHideWing( bHide );
            }
        }
        break;
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    case nOptionType::USE_MOVE_TO_CAMERA:
        {
            g_HeroInput.SetCameraBaseMove( Value );
        }
        break;
    case nOptionType::USE_BLOCK_WHISPER:
        {
            MSG_CG_STATUS_INTERCEPTION_SYN sendPacket;
            sendPacket.m_byInterceptionCode = eINTERCEPTION_WHISPER;
            sendPacket.m_bRequestFlag = Value;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sendPacket, sizeof (sendPacket));
        }
        break;
    case nOptionType::USE_BLOCK_TRADE:
        {
            MSG_CG_STATUS_INTERCEPTION_SYN sendPacket;
            sendPacket.m_byInterceptionCode = eINTERCEPTION_TRADE;
            sendPacket.m_bRequestFlag = Value;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sendPacket, sizeof (sendPacket));
        }
        break;
    case nOptionType::USE_CHANGE_EYE_MOUSE:
        {
            g_Input.SetMouseChange( Value );
        }
        break;
    case nOptionType::USE_FIXED_TARGET:
        {
            g_HeroInput.SetFixTarget( Value );
        }
        break;
    case nOptionType::USE_FORWARD_TARGET:
        {
            g_HeroInput.SetForwardTargeting( Value );
        }
        break;
    case nOptionType::USE_AUTO_TARGET:
        {
            g_HeroInput.SetAutoTarget( Value );
        }
        break;
    case nOptionType::USE_HELP:
        if( uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP)) )
        {
            helpMan->ShowHelpMenu( Value, TRUE );
        }
        break;

        // Sound Option
    case nOptionType::BGM_VOLUME:
        {
            float fVolume = (float)Value * 0.01f;
            Sound::Instance()->SetBGMVolume( fVolume );
            g_SoundBGM.SetVolume( fVolume );
        }
        break;
    case nOptionType::AMBIENT_VOLUME:
        {
            float fVolume = (float)Value * 0.01f;
            Sound::Instance()->SetAmbientEffectVolume( fVolume );
            if( g_pMap )
                g_pMap->SetAmbientVolume( fVolume );
        }
        break;
    case nOptionType::EFFECT_VOLUME:
        {
            float fVolume = (float)Value * 0.01f;
            Sound::Instance()->SetEffectVolume( fVolume );
        }
        break;
    case nOptionType::SYSTEM_VOLUME:
        {
            float fVolume = (float)Value * 0.01f;
            Sound::Instance()->SetMasterVolume( fVolume, TRUE );
        }
        break;
    case nOptionType::SPEAKER_VOLUME:
        {
            float fVolume = (float)Value * 0.01f;
            Sound::Instance()->SetWaveVolume( fVolume, TRUE );
        }
        break;

#if WZENGINEVER >= 620
    case nOptionType::USE_FIXED_UI_POSITION:
        {
            BOOL UseAutoScale = Value == 0;

            if( g_pSunRenderer->Get2DModeAutoScale() != UseAutoScale )
            {
                g_pSunRenderer->Set2DModeAutoScale( UseAutoScale );

                St_ScreenFormat ScreenFormat; 
                g_pSunRenderer->GetScreenFormat( &ScreenFormat );


                RECT rcWindow;
                rcWindow.left = 0;
                rcWindow.top = 0;
                rcWindow.right = ScreenFormat.m_dwWidth;
                rcWindow.bottom = ScreenFormat.m_dwHeight;
                g_InterfaceManager.SetWindowRC(&rcWindow);

                g_Camera.SetAspectRatio(SR_AUTO);

                g_pSunRenderer->ChangeScreenFormat(&ScreenFormat, g_pApplication->GetHWnd(), false );

                POINT	Point;
                Point.x = ScreenFormat.m_dwWidth;
                Point.y = ScreenFormat.m_dwHeight;
                g_pApplication->OnChangeRes(&Point );

                g_InterfaceManager.MoveDefaultWindowPos();
            }
        }
        break;
#endif // WZENGINEVER >= 620

#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
    case nOptionType::SHADOW_DISTANCE:
    {
        if (Value >= 0)
        {
            g_pSunRenderer->x_pManagerShadowMap->SetShadowRange(static_cast<float>(Value));
        }
    }
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS

    default:
        break;

    }
}

void CGameOption::ApplyOption()
{
    ApplyScreenOption();

    for( size_t i=0;i!=nOptionType::MAX;++i)
    {
        nOptionType::eOptionTypeIndex TypeIndex = (nOptionType::eOptionTypeIndex)i;
        ApplyOptionValue( TypeIndex, m_OptionParam.GetOptionValue(TypeIndex) );
    }

    Save();
}


const SCREEN_SETTING& CGameOption::GetAvailableScreenSetting(int index)
{
	assert(index < m_ScreenSettingCount);
	assert(index >= 0);
	
	return m_ScreenSettingArray[index];
		
}
//------------------------------------------------------------------------------ 
int CGameOption::GetAvailableScreenSettingIndex(int width, int height, int bpp)
{
    for (int i = 0; i < m_ScreenSettingCount; ++i)
    {
        SCREEN_SETTING& screen_setting = m_ScreenSettingArray[i];
        if ((screen_setting.ScreenSetting.m_iWidth == width) && 
            (screen_setting.ScreenSetting.m_iHeight == height) && 
            (screen_setting.ScreenSetting.m_iBpp == bpp))
        {
            return screen_setting.iOriginalScreenSetting;
        }
    }
    return -1;
}

int CGameOption::GetAvailableScreenSettingCount()
{
	return m_ScreenSettingCount;
}

void CGameOption::ToggleFullScreenMode()
{
    if( g_pSunRenderer->ToggleFullScreen() == TRUE )
    {
        St_ScreenFormat ScreenFormat;
        g_pSunRenderer->GetScreenFormat( &ScreenFormat );

        if (ScreenFormat.m_bFullScreen)
        {
            m_OptionParam.SetScreenMode( OptionParam::kScreenMode_Full );
        }
        else if (ScreenFormat.m_bMaximizeScreen)
        {
            m_OptionParam.SetScreenMode( OptionParam::kScreenMode_MaxWindow );
        }
        else
        {
            m_OptionParam.SetScreenMode( OptionParam::kScreenMode_Window );
        }
    }
}

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
MONEY CGameOption::GetHeimTradingvolumeNotification()
{
	MONEY money = 0;

	int option_value = m_OptionParam.GetOptionValue( nOptionType::HEIM_TRADINGVOLUME);
	sOptionItemInfo* option_item_info = OptionItemInfoParser::Instance()->GetOptionItemInfo(236);
	if (option_item_info == NULL)
	{
		return money;
	}

	if (option_item_info->m_ItemDataArray.empty())
	{
		return money;
	}

	if (option_item_info->m_ItemDataArray.size()-1 < (unsigned int)option_value || 
		option_value < 0)
	{
		return money;
	}

	if (option_value == 0)
	{
		money = 0;
	}
	else
	{
		std::string option_string = option_item_info->m_ItemDataArray[option_value];
		money = _atoi64(const_cast<char*>(option_string.c_str()));
	}

	return money;
}
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

