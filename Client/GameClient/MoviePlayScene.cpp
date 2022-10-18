
#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "MoviePlayScene.h"
#include "objectmanager.h"
#include "ItemManager.h"
#include "InterfaceManager.h"
#include "SoundBGM.h"
#include "LoadScene.h"

MoviePlayScene g_MoviePlayScene;

MoviePlayScene::MoviePlayScene()
{
	InitData();
    m_SceneType = SCENE_TYPE_MOVIEPLAY;
}

MoviePlayScene::~MoviePlayScene()
{

}

void MoviePlayScene::InitData( void )
{
	m_hBink			= 0;
	m_hTexture		= INVALID_HANDLE_VALUE;
	m_bEnableRender	= TRUE;

	m_hTexGameRatingText = INVALID_HANDLE_VALUE;
}

BOOL MoviePlayScene::Init()
{
	g_InterfaceManager.SetRenderState( TRUE );
	BinkSoundUseDirectSound( 0 );
	m_dwCurFadeTime=0;
	m_eFadeState=eFADE_STATE_NONE;
	m_bFristRender=TRUE;

	if( OpenMovieFile( GENERALPARAM->GetCinematicFileName() ) )
	{
		GenerateEmptyTexture();

		m_wzPrevClearColor = g_pSunRenderer->GetClearColor();
		g_pSunRenderer->SetClearColor( 0, 0, 0 );

		ShowCursor( FALSE );
	}

#ifdef _JBH_ADD_AGE_LIMIT_MARK

	LoadingGameRatingMarkTexture();

#endif
	return TRUE;
}

BOOL MoviePlayScene::OpenMovieFile( char *szFileName )
{
	if( !szFileName )
		return FALSE;

	m_hBink = BinkOpen( szFileName, 0 );
	if ( !m_hBink )
		return FALSE;

	return TRUE;
}

BOOL MoviePlayScene::GenerateEmptyTexture( void )
{
	if( !m_hBink )
		return FALSE;

	POINT	pt = g_pApplication->Get2DModeMapping();
	int		iPosX = 0, iPosY = 0;

	if( g_pApplication->IsWideModeFlag() == FALSE )
	{
		LONG	SizeY	= (LONG)( (float)pt.x / 1.77777778f );
		iPosX			= 0;
		iPosY			= (pt.y - SizeY) / 2;
		pt.y			= SizeY;
	}

	m_rcScreenRect.left		= iPosX;
	m_rcScreenRect.top		= iPosY;
	m_rcScreenRect.right	= pt.x;
	m_rcScreenRect.bottom	= pt.y;

	//	3 - RGB     4 - RGBA
	m_hTexture = g_pSunRenderer->x_pManagerTexture->GenerateEmptyTexture(
										m_hBink->Width, m_hBink->Height, 4,
										TLS_NO_MIPMAP | TLS_EMPTY_TEXTURE );
	return (BOOL)m_hTexture;
}

BOOL MoviePlayScene::IsStopable( void )
{
	// 스페이스 키를 누르면 정지
	return ( GENERALPARAM->GetSpecialMode() && g_Input.GetState( DIK_SPACE, KS_KEY ) );
}

BOOL MoviePlayScene::IsQuitable( void )
{
	return	( !m_hBink ||										// 동영상 파일이 없거나
			  g_Input.IsPressAnyKey() ||						// 유저 인풋이 들어오거나
			( !BinkWait( m_hBink ) && !GetNextFrame() ) );		// 다음 프레임이 없다면...
}

BOOL MoviePlayScene::Process( DWORD dwTick )
{
	// 스페이스 키를 누르면 정지
	if( IsStopable() )
	{
		return TRUE;
	}
	else if( IsQuitable() )
	{
#if defined( _JSH_ADD__PLAY_MOVIE_BEFORE_MISSION_ )
		static BOOL bFirst = TRUE;
		if( bFirst )
		{
			bFirst = FALSE;
			GameFramework::ChangeScene( SCENE_TYPE_START );
		}
		else
		{
			GameFramework::ChangeScene( SCENE_TYPE_MISSION );
		}
#else
		GameFramework::ChangeScene( SCENE_TYPE_START );
#endif
		return TRUE;
	}

	return TRUE;
}

// GetNextFrame - advances to the next Bink frame.
BOOL MoviePlayScene::GetNextFrame( void )
{
	if( !m_hBink )
		return FALSE;

	// Decompress the Bink frame.
	BinkDoFrame( m_hBink );

	// Lock the BinkBuffer so that we can copy the decompressed frame into it.
	long lPitch;
	int iComponent;
	VOID* pByte = g_pSunRenderer->x_pManagerTexture->LockTexture( m_hTexture, &lPitch, &iComponent );

	if( pByte )
	{
		BinkCopyToBuffer( m_hBink, pByte, lPitch, m_hBink->Height, 0,0, BINKSURFACE32 );
		g_pSunRenderer->x_pManagerTexture->UnlockTexture( m_hTexture );
	}
	else
	{
		return FALSE;
	}

	// Are we at the end of the movie?
	if ( m_hBink->FrameNum == m_hBink->Frames )
	{
		return FALSE;
	}
	else
	{
		BinkNextFrame( m_hBink );
	}

	return TRUE;
}

BOOL MoviePlayScene::Render( DWORD dwTick )
{
	m_eFadeState=eFADE_STATE_NONE;

	if( m_hTexture == INVALID_HANDLE_VALUE )
		return FALSE;

	g_pSunRenderer->ClearRender();
	g_pSunRenderer->Set2DMode( TRUE, FALSE );

	g_pSunRenderer->RenderTexture(	m_hTexture,
									(float)m_rcScreenRect.left, (float)m_rcScreenRect.top,
									(float)m_rcScreenRect.right, (float)m_rcScreenRect.bottom - 1,
									0.0f, 0.0f );
	RenderFrameNo();
#ifdef _JBH_ADD_AGE_LIMIT_MARK
	RenderGameRatingMark();
#endif

	g_pSunRenderer->Set2DMode( FALSE );

	if(g_pEffectManager)
		g_pEffectManager->Move( dwTick );


	g_pSunRenderer->x_pManagerPostEffect->Apply();

	return TRUE;
}

void MoviePlayScene::RenderFrameNo( void )
{
	if( !m_hBink )
		return;

	if(GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE)
		return;

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "DFLT" );

	//	프레임 출력
	static TCHAR	pText[256] = {0,};
	Snprintf( pText, 256-1, _T("Current Frame : %d"), m_hBink->FrameNum );
	POINT	pt = g_pApplication->Get2DModeMapping();
	g_pSunRenderer->x_pManagerTextOut->DrawText( pText, 30, (pt.y - 30), WzColor_RGB( 0x0F, 255, 0x0f ) );

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}


void MoviePlayScene::Release()
{
	if( m_hBink )
    {
        BinkClose( m_hBink );
    }

	if( m_hTexture != INVALID_HANDLE_VALUE )
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTexture );
        m_hTexture = INVALID_HANDLE_VALUE;
    }

	InitData();

	g_pSunRenderer->SetClearColor( Red_WzColor( m_wzPrevClearColor ), Blue_WzColor( m_wzPrevClearColor ),
								   Green_WzColor( m_wzPrevClearColor ), Alpha_WzColor( m_wzPrevClearColor ) );
	ShowCursor( TRUE );

#ifdef _JBH_ADD_AGE_LIMIT_MARK

	ReleaseGameRatingMarkTexture();

#endif
}

void MoviePlayScene::ParsePacket( MSG_BASE * pMsg )
{
}

void MoviePlayScene::RenderText( void )
{
}

#ifdef _JBH_ADD_AGE_LIMIT_MARK
void MoviePlayScene::RenderGameRatingMark()
{
	if( !m_hBink )
		return;

	if(m_hBink->FrameNum > 0 )
	{
		static DWORD dwAccumulateTick = clock_function::GetTickCount();

		if( dwAccumulateTick + 3000 > clock_function::GetTickCount() )
		{
			RenderGameRatingMarkAndText();
		}
	}
}


void MoviePlayScene::LoadingGameRatingMarkTexture()
{	
	SceneBase::LoadingGameRatingMarkTexture();

	m_hTexGameRatingText = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Loading_WebzenCI_Text.tga",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD );
}

void MoviePlayScene::ReleaseGameRatingMarkTexture()
{
	SceneBase::ReleaseGameRatingMarkTexture();

	if( m_hTexGameRatingText != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTexGameRatingText);
		m_hTexGameRatingText = INVALID_HANDLE_VALUE;
	}

}

void MoviePlayScene::RenderGameRatingMarkAndText()
{
	ENUM_ALPHABLEND oldMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->Set2DMode( TRUE);
	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
	g_pSunRenderer->SetColor(255,255,255,255);	

	POINT ScreenSize = g_pApplication->Get2DModeMapping();
	float fDestX = max( 0.0f , ((float ) ScreenSize.x  - 780.0f) / 2.0f);
	float fDestXSize = min( ScreenSize.x , 780.0f);
	//텍스트 출력
#ifdef _KOREA
	if( m_hTexGameRatingText != INVALID_HANDLE_VALUE )
		g_pSunRenderer->RenderTexture( m_hTexGameRatingText ,fDestX , ScreenSize.y - 100.0f , fDestXSize , 68.0f);
	// 마크 출력
	if ( m_hGameRatingMarkTexture != INVALID_HANDLE_VALUE )
		g_pSunRenderer->RenderTexture( m_hGameRatingMarkTexture ,((float) ScreenSize.x  - 252.0f), 30.0f, 232.0f , 89.0f);
#endif
	g_pSunRenderer->Set2DMode( FALSE );
	g_pSunRenderer->SetAlphaBlendMode(oldMode);
}


#endif
