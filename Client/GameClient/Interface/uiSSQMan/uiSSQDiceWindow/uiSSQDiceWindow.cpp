#include "SunClientPrecompiledHeader.h"
#include "uiSSQDiceWindow.h"
#include "Camera.h"
#include "InterfaceManager.h"
#include "SoundEffect.h"

const int DICE_PONTER_RESOURCE_ID	= 8000000 /*SCRIPT_RESOURCE_ID_OFFSET*/ + 120003;

uiSSQDiceWindow::uiSSQDiceWindow( InterfaceManager * pUIMan )
:uiBase( pUIMan )
,m_pDiceUnitDraw( NULL )
,m_pDiceUnit( NULL )
,m_dwShowTick( 0 )
{
}

uiSSQDiceWindow::~uiSSQDiceWindow( void )
{

}

void uiSSQDiceWindow::Init( CDrawBase * pDrawBase )
{
	g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( DICE_PONTER_RESOURCE_ID, "Data\\Interface\\dice\\dice.wzu", TRUE, ULS_NO_MULTITHREAD);

	m_pDiceUnit			= g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit( DICE_PONTER_RESOURCE_ID );
	m_pDiceUnitDraw		= (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 );

	m_pDiceUnitDraw->SetWzUnit( m_pDiceUnit,FALSE );
	WzVector pos;
	SetVector( &pos, 0.0f, 15.0f, 0.0f );
	m_pDiceUnitDraw->SetPosition( pos );
}

void uiSSQDiceWindow::Release()
{
	if ( m_pDiceUnitDraw )
    {
		Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_pDiceUnitDraw );
    }

	if ( m_pDiceUnit )
    {
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( DICE_PONTER_RESOURCE_ID );
    }
}


void uiSSQDiceWindow::OnRenderSolarDialog()
{
	RenderDice();
}

void uiSSQDiceWindow::Process( DWORD dwTick )
{
	if( IsVisible() == FALSE )
		return;

	if( m_pDiceUnitDraw == NULL )
		return;

	m_dwShowTick += dwTick;

	if( m_dwShowTick >= 5000 )
	{
		m_dwShowTick = 0;
		ShowWindow( FALSE );
	}

	WzEvent event;	
	g_pSunRenderer->ProgressUnitFrame( m_pDiceUnitDraw, (float)dwTick, &event );

	static DWORD Tick = 0;

	Tick += dwTick;

	if( Tick >= 4000 )
	{
		Tick = 0;
		g_SoundEffect.PlayUI( 567 );
	}
}

void uiSSQDiceWindow::OnShowWindow( BOOL val )
{
	if( val )
	{
		
	}
	else
	{

	}
}

void uiSSQDiceWindow::MessageProc( SI_MESSAGE * pMessage )
{

}

const RECT uiSSQDiceWindow::GetViewRect( void )
{
	RECT rc;

	CControlWZ * pConTrol = GetControlWZ( StrToWzID( "PI00" ) );

	rc = pConTrol->GetSizeRect();

	return rc;
}

void uiSSQDiceWindow::RenderDice( void )
{
	if( m_pDiceUnitDraw == NULL )
		return;
	
	WzVector m_CameraFrom, m_CameraTo, m_wvZ;
	SetVector(&m_CameraFrom, 0.0, 0.0, 0.0);
	SetVector(&m_CameraTo, 0.0, 100.0, 0.0);
	SetVector(&m_wvZ, 0.0, 0.0, 1.0);

	BOOL bOld2dMode,bOldRealSize;
	BOOL bZWrite,bZTest;
	DWORD alpharef;
	ENUM_CMP_FUNC alphafunc;
	ENUM_CMP_FUNC		eZFunc;
	ENUM_ALPHABLEND		eAlphablendMode;
	ENUM_CULL_FACE      eCullFaceMode;
	WzColor             oldcolor;
	ENUM_MESH_DRAW      eDrawMode;
	eDrawMode = g_pSunRenderer->GetMeshDrawMode();
	oldcolor = g_pSunRenderer->GetDiffuseColor();
	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	eZFunc = g_pSunRenderer->GetZFunc();
	eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	bZWrite = g_pSunRenderer->IsEnableZWrite();
	bZTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->GetAlphaTestMode(&alpharef,&alphafunc);
	g_pSunRenderer->Check2DMode(&bOld2dMode,&bOldRealSize);
	g_pSunRenderer->Set2DMode( FALSE );


	WzMatrix mat;
	MatrixIdentity(&mat);

	float fZ = 1.0f;
	RECT Viewport = g_InterfaceManager.CalcuateViewPortWithRect( GetViewRect() );

	g_pSunRenderer->SetWorldMatrix(&mat);
	g_pSunRenderer->SetCameraMatrix(&m_CameraFrom, &m_CameraTo, &m_wvZ);
	g_pSunRenderer->SetViewport( &Viewport );
	BOOL bFog = g_pSunRenderer->IsFogEnabled();
	g_pSunRenderer->EnableFog(FALSE);
	g_pSunRenderer->ClearRender( CR_DEPTH, NULL, &fZ,NULL, &Viewport );  

	int iWidth = Viewport.right - Viewport.left;
	int iHeight = Viewport.bottom - Viewport.top; 		

	g_pSunRenderer->SetOrthogonalMatrix( -0.6f, 0.6f, -0.5f, 0.5f, 1.0f, 20.0f );
	g_pSunRenderer->SetZBufferWrite(TRUE);
	g_pSunRenderer->SetZBufferTest(TRUE);
	
    DWORD  iTime  = timeGetTime() % 1000;
	float fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	m_pDiceUnitDraw->SetRotation( fAngle, 0.0f, fAngle );

	WzVector pos;
	SetVector( &pos, 0.0f, 15.0f, 0.0f );

	m_pDiceUnitDraw->SetPosition( pos );

	g_pSunRenderer->AnimateUnit( m_pDiceUnitDraw );
	g_pSunRenderer->RenderUnit( m_pDiceUnitDraw );

	g_pSunRenderer->FlushReservedDrawCall();
	g_pSunRenderer->SetViewport(NULL);
	g_Camera.RestoreProjection();
	g_pSunRenderer->Set2DMode( bOld2dMode ,bOldRealSize);			
	g_pSunRenderer->EnableFog(bFog);
	g_pSunRenderer->SetZBufferWrite(bZWrite);
	g_pSunRenderer->SetZBufferTest(bZTest);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetZFunc(eZFunc);
	g_pSunRenderer->SetAlphaBlendMode(eAlphablendMode);
	g_pSunRenderer->SetAlphaTestMode(alpharef,alphafunc);
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
	g_pSunRenderer->SetMeshDrawMode( eDrawMode );
	g_pSunRenderer->SetColor(oldcolor);
	g_pSunRenderer->EnableAlphaTest(FALSE);
}