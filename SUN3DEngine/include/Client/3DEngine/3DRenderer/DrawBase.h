// drawbase.h

#ifndef _3DRENDERER_DRAWBASE_H_
#define _3DRENDERER_DRAWBASE_H_

// xxx: 고치는게 맘처럼 쉽지가 않다.
// 여기 있을 만한 헤더는 아니지만, 원래 본 파일에 있었던 내용이었고 외부에서
// 본 파일(drawbase.h)을 바로 include하는 경우가 많아서 부득이 일단 추가한다.
#include "dllaccess.h"

#include <programcommon/clock.h>
#include <programcommon/filenbuffer.h>
#include <client/3dengine/common/wzsimplemesh.h>

#include "managertext.h"
#include "managervertexbuffer.h"
#include "managervertexshader.h"
#include "managerpixelshader.h"
#include "managervbfill.h"
#include "managertexture.h"
#include "managerwzdloader.h"
#include "managerwzunitloader.h"
#include "managertransformbuffer.h"
#include "managerlight.h"
#include "managerrendercommand.h"
#include "managermultiloader.h"
#include "managerspeedtree.h"
#include "managerposteffect.h"
#include "managershadowmap.h"
#include "managerocclusionculling.h"
#include "managerunitlod.h"

#include "managerunitlightmap.h"

class CEffectManager;
class CWorldBase;
class CElementRender;

/// @name Draw Create Option

#define DCO_USE_PHYSICS			( 0x00000001)
#define DCO_SET_IRRADIANCE_PATH	( 0x00000002)

/// @name Default Digit
#define MIN_SCREEN_WIDTH	(640)
#define MIN_SCREEN_HEIGHT	(480)

#define STD_2D_WIDTH	    (1024)
#define STD_2D_HEIGHT	    (768)
#define STD_2D_WIDTH_WIDE	(1280)
#define STD_2D_HEIGHT_WIDE	(720)
#define STD_2D_WIDTH_HALF_WIDE	(1280)
#define STD_2D_HEIGHT_HALF_WIDE	(800)

#define DEFAULT_ALPHATEST_REF		( 1)

/// @name Render state

#define RS_PROJECTION_PERSPECTIVE	( 0x00000001)	///< 퍼스펙티브 프로젝션
#define RS_ZWRITE_ENABLED			( 0x00000010)	///< z 버퍼 쓰기
#define RS_ZTEST_ENABLED			( 0x00000020)	///< z 버퍼 테스트
#define RS_ALPHABLEND_ENABLED		( 0x00000040)	///< 알파 블렌드 가능
#define RS_ALPHA_SORT_DISABLED		( 0x00000080)	///< 알파 소팅 자동화
#define RS_FOG_ENABLED				( 0x00000100)	///< Fog 사용
#define RS_FOG_TURNED_ON			( 0x00000200)	///< Fog 사용
#define RS_TEXTURE_DISABLED			( 0x00000400)	///< Texture Off
#define RS_STENCIL_ENABLED			( 0x00000800)	///< Stencil 사용
#define RS_SHADOW_DISABLED			( 0x00001000)	///< Shadow 안그리기
#define RS_SHADOW_FOR_ALPHA_OBJECT	( 0x00002000)	///< Alpha Object 에 Shadow 적용하기
#define RS_IN_TOGGLEFULLSCREEN		( 0x00100000)	///< 풀스크린 토글중
#define RS_IN_HYPERRENDER			( 0x00200000)	///< 같은 상태의 메쉬들을 몰아서 뿌리기 중
#define RS_IN_2DHYPERRENDER			( 0x00400000)	///< 2D 그리기에서 몰아서 뿌리기 중
#define RS_IN_FLUSHINGDRAWCALL		( 0x00800000)	///< FlushReservedDrawCall 중
#define RS_EFFECT_RENDERED			( 0x01000000)	///< Effect 가 render 되었으니 FlushReservedDrawCall 에서 render 할 필요 없음
#define RS_IN_CHANGESCREENFORMAT	( 0x02000000)	///< 스크린포맷 변환중
#define RS_ALPHALIST_RENDERED		( 0x04000000)	///< 최종 alpha list 렌더링 여부


/// @name Clear Render(er) Flags

#define CR_COLOR		( 0x00000001)	///< 칼라 버퍼를 지운다
#define CR_DEPTH		( 0x00000002)	///< 깊이(Z) 버퍼를 지운다
#define CR_STENCIL		( 0x00000004)	///< 스텐실 버퍼를 지운다
#define CR_DEFAULT		CR_COLOR | CR_DEPTH | CR_STENCIL

enum ENUM_WATER_TYPE
{
	EWT_NORMAL = 0,
	EWT_REFRACTION = 1,
	EWT_SPECULAR = 2,
};


// OPTIMIZELISTS

///////////////////////////////////////////////////////////////////////////////////////
/// @name --- CDrawBase class


class CDrawBase  
{
public:
	CDrawBase();
	virtual ~CDrawBase();
	virtual void Clear();

protected:
	HWND m_hWnd;
	BOOL m_bPause;	///< 멈춤 상태 - 그리지 않는다.
	RECT m_rtWindowClient;
	DWORD m_dwCreateOption;	///< 생성 옵션
public:
	CClock x_Clock;

	CEffectManager *x_pEffectManager;	/// Effect 관련 처리
	CWorldBase *x_pWorld;	///< World

	// ---------------------------------------------------------------------------------------------
	/// @name --- 기본 생성/초기화
public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();
	virtual void CreateManagers( char *lpszShaderPath = NULL);
	virtual void DestroyManagers();

	virtual BOOL Initialize3DEnvironment();
	virtual BOOL ToggleFullScreen( HWND hWndChange = 0) = 0;	// ToggleFullScreen 시 윈도우를 바꾸고자 할때 hWndChange 에 값을 넣는다.
	virtual BOOL ProcessWindowSizeChange();
    virtual BOOL ChangeScreenFormat(const St_ScreenFormat* pcScreenFormat, 
                                    HWND hWndChange = 0, 
                                    bool is_window_size_changed = false) = 0;	
	virtual BOOL SetEffectManager( CEffectManager *pEffectManager);
	virtual void SetWorld( CWorldBase *pWorld);	
	virtual BOOL GetWorld();


	// ---------------------------------------------------------------------------------------------
	/// @name --- CDiscAccess
protected:
	CDiscAccess *m_pDiscAccess;
	BOOL m_bDiscAccessAuto;
public:
	virtual void SetDiscAccess( CDiscAccess *pDiscAccess, BOOL bAuto = FALSE);
	virtual CDiscAccess* GetDiscAccess();

	CDiscAccess* CheckDiscAccessExistence();
	BOOL IsDiscAccessGeneratedAutomatically();


	// ---------------------------------------------------------------------------------------------
	/// @name --- Window 세팅 (API 관련)
protected:
	long m_lWindowStyle;
	//UINT m_uiSystemScreenWidth, m_uiSystemScreenHeight;
	virtual void PrepareWindowModeChange(bool full_mode, bool max_screen, bool use_init_style);
	virtual void FinishWindowModeChange(bool full_mode, bool max_screen);


	// ---------------------------------------------------------------------------------------------
	/// @name --- 매니저
public:
	int                     nType;
	BOOL                    bLog;
	CManagerText			*x_pManagerTextOut;
	CManagerVertexBuffer	*x_pManagerVB;
	CManagerVertexShader	*x_pManagerVS;
	CManagerPixelShader		*x_pManagerPS;
	CManagerVBFill			*x_pManagerVBFill;
	CManagerTexture			*x_pManagerTexture;
	CManagerWzdLoader		*x_pManagerWzdLoader;
	CManagerWzUnitLoader	*x_pManagerWzUnitLoader;
	CManagerTransformBuffer	*x_pManagerTransformBuffer;
	CManagerLight			*x_pManagerLight;
	CManagerSpeedTree		*x_pManagerSpeedTree;
	CManagerPostEffect		*x_pManagerPostEffect;
	CManagerRenderCommand	*x_pManagerRenderCommand;
	CManagerUnitLOD			*x_pManagerUnitLOD;

	CManagerShadowMap		*x_pManagerShadowMap;
	CManagerMultiLoader		*x_pManagerMultiLoader;
	CManagerOcclusionCulling* x_pManagerOccCulling;
	CManagerUnitLightMap	*x_pManagerUnitLightMap;

protected:
	enum ENUM_MANAGER_LEVEL
	{
		ML_LOW = 0,	ML_MEDIUM,	ML_HIGH
	};
	CWzArray<CManagerBase*> m_arManagerArray[3];
	void AddManager( CManagerBase *pManager, int iLevel = ML_MEDIUM);

	// ---------------------------------------------------------------------------------------------
	/// @name --- Render
protected:
	DWORD m_dwCurrentTick;
	float m_fCurrentTick1000;
	DWORD m_dwFpsInitiailTick;
	DWORD m_dwFpsLatestTicks;
	long m_lFpsRenderCount;	// Frame / second
	long m_lFpsLatestCount;
	long m_lPpsCount;	// Polygon / second
	long m_lPpsLatest;
	long m_lDcpsCount;	// Draw call / second
	long m_DcpsLatest;
	BOOL m_bReseted; 
public:
	virtual BOOL BeginRender( DWORD dwClearFlags=CR_DEFAULT) = 0;
	virtual void EndRender();
	virtual BOOL IsReseted() const { return m_bReseted; } // BeginRender후에 체크한다.
	virtual BOOL IsRenderReady() const = 0;
	virtual void GetFpsValues( DWORD *pdwTicks, long *plCounts)	{ *pdwTicks = m_dwFpsLatestTicks; *plCounts = m_lFpsLatestCount; }
	virtual void GetPpsValues( DWORD *pdwTicks, long *plCounts)	{ *pdwTicks = m_dwFpsLatestTicks; *plCounts = m_lPpsLatest; }
	virtual void GetDcpsValues( DWORD *pdwTicks, long *plCounts)	{ *pdwTicks = m_dwFpsLatestTicks; *plCounts = m_DcpsLatest; }
	virtual void SetPaused( BOOL bPause)	{ m_bPause = bPause; }
	virtual void InitAllRenderStates() = 0;	// Render 관련된 모든 정보를 초기화 한다.
	
	// RenderUnit()을 포함해서 Render***()으로 호출된 정보를 렌더링 한다.
	// - 기존의 FlushReservedDrawCall() 대신 사용한다.
	// - bRecallEffect: TRUE일 경우, 같은 Effect 정보를 한번이상 호출가능하도록 설정.
public:
	virtual void FlushDrawCall( BOOL bRecallEffect = FALSE);
	// 차후 protected로 변경 예정.
    virtual void FlushReservedDrawCall( BOOL bLastFlush = FALSE);

protected:
    virtual void FlushReservedDrawCallEx( BOOL bLastFlush ) {}

public:

	// 현재 화면(buffer)을 지운다.
	// CR_COLOR: 칼라 버퍼를 (*pClearColor)색[ARGB]으로 지운다
	// CR_DEPTH: 깊이 버퍼를 (*pClearZ)값[0~1]으로 지운다.
	// CR_STENCIL: 스텐실 버퍼를 (*pClearStencil)값[2^n-1]으로 지운다
	// 세 포인터형 인자값들은 NULL일 경우, Render 내부값을 사용한다.
	virtual BOOL ClearRender( DWORD dwFlags=CR_DEFAULT, WzColor* pClearColor=NULL, float* pClearZ=NULL, 
		DWORD* pClearStencil=NULL, const RECT* pRect=NULL) = 0;

	// ---------------------------------------------------------------------------------------------
	/// @name --- Camera, Projection 등 Matrix 관련
protected:
    St_FogParameter m_FogParameter;

	St_Camera m_Camera;
	BOOL m_bCustomViewport;	///< 뷰포트 지정했는가
	RECT m_rtViewport;	///< 뷰포트 지정시 세팅됨

	BOOL m_bIn2DMode;
	BOOL m_b2DModeInRealSize;	///< 2D 모드시 좌표
	int m_fStd2DWidth;        ///< 2D 모드의 크기
    int m_fStd2DHeight;       ///< 2D 모드의 크기

    BOOL m_b2DModeAutoScale;    ///< 2D 모드 자동 스케일링 사용여부
    DWORD m_2DModeAutoScaleWidth;  ///< 2D 모드 자동 스케일 사용시 기준 크기


protected:
	virtual void SetCameraMatrix( WzMatrix *pwmCamera) = 0;
	virtual void SetProjectionMatrix( WzMatrix *pwmProjection) = 0;

public:
	virtual void SetWorldMatrix( WzMatrix *pwmWorld) = 0;
	virtual void SetCameraMatrix( WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvUp) = 0;
	virtual void SetProjectionMatrix( float fFov, float fNear, float fFar, ENUM_SCREEN_RATIO eScreenRatio = SR_AUTO) = 0;
	virtual void SetOrthogonalMatrix( float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar) = 0;
    virtual void Set2DMode( BOOL b2D = TRUE, BOOL bInRealSize = FALSE) {}
	virtual void Check2DMode( BOOL *pb2D, BOOL *pbInRealSize)	{ *pb2D = m_bIn2DMode; *pbInRealSize = m_b2DModeInRealSize; }

    virtual void Set2DModeAutoScale( BOOL bUse ) { m_b2DModeAutoScale = bUse; }
    virtual BOOL Get2DModeAutoScale() { return m_b2DModeAutoScale; }
    virtual DWORD Get2DModeAutoScaleWidh() { return m_2DModeAutoScaleWidth; }
    virtual void Set2DModeAutoScaleWidth( DWORD width ) { m_2DModeAutoScaleWidth = width; }
    virtual int Get2DModeWidth() { return m_fStd2DWidth; }
    virtual int Get2DModeHeight() { return m_fStd2DHeight; }

	virtual BOOL SetLinearFog( WzColor wcFogColor, float fFogStart, float fFogEnd, float fMaxFogRate);
	virtual St_FogParameter GetFogParameter()	{ return ( m_FogParameter); }
	St_FogParameter* GetFogParameterPtr()	{ return ( &m_FogParameter); }

	virtual St_Camera* GetCameraInfo()	{ return ( &m_Camera); }
	virtual WzMatrix* GetClipMatrix( WzMatrix *pwmClip) = 0;
	virtual POINT* GetScreenPos( POINT *ptScreen, WzVector *pwvLocal, float *pfDepth = NULL) = 0;
	virtual WzRay* GetMouseCursorRay( WzRay *pwr, POINT *ptCursor = NULL);

	virtual void SetViewport( RECT *prtViewport);	///< NULL 인 경우 해제됨
	virtual BOOL PushViewport() = 0;
	virtual BOOL PopViewport() = 0;

	virtual WzMatrix* GetClipMatrixOfShadowMap( WzMatrix *pwmOutClip, WzMatrix* pwmLight) = 0;
	virtual void GetViewMatrix( WzMatrix* pwmOut) = 0;
	virtual void GetWorldMatrix( WzMatrix* pwmOut) = 0;
	virtual void GetProjectMatrix( WzMatrix* pwmOut) = 0;

    const RECT& GetViewport() const { return m_rtViewport; }


//-----------------------------------------------------------
// Light 관련(x_pManagerLight)
//-----------------------------------------------------------

	// 라이트 설정 및 정보 참조
	virtual BOOL SetDirectionalLight( int iIndex, WzColor wcColor, const WzVector& wvDirection, ENUM_LIGHT_IRRA_TYPE eliType =0, float fRange = DEFAULT_LIGHT_RANGE);
	virtual BOOL SetPointLight( int iIndex, WzColor wcColor, const WzVector& wvPosition, ENUM_LIGHT_IRRA_TYPE eliType =0, float fRange = DEFAULT_LIGHT_RANGE);
	virtual const St_LightInformation* GetLightInfo( int iIndex) const;

	virtual	BOOL	SetLightIntensity( int iIndex, float fIntensity );
	virtual float	GetLightIntensity( int iIndex );

	// Active 라이트 설정 및 정보 참조
	virtual BOOL TurnOnLight( int iIndex, BOOL bOn);
	virtual int GetIndexOfActiveLight( int iActive) const;
	virtual const St_LightInformation* GetActiveLightInfo( int iActive) const;
	virtual int GetNumberOfActiveLight() const;

	// Irradiance 파일 경로 설정 - 임시
	virtual void SetIrradianceFilePath( LPSTR lpszPath );

	// 지역 라이트 설정 관련 : default FALSE
	virtual void EnableAreaLight( BOOL bEnable);
	virtual BOOL IsAreaLight() const;

	// ---------------------------------------------------------------------------------------------
	/// @name --- 그리기 모드
public:
	virtual void ApplyRenderState( St_RenderState *pRenderState, int iWithAlpha = 0);
	virtual ENUM_MESH_DRAW GetMeshDrawMode()	{ return ( m_RenderState.m_eMeshDrawMode); }
	virtual void SetMeshDrawMode( ENUM_MESH_DRAW eMeshDrawMode, BOOL bForce = FALSE);
	virtual ENUM_CULL_FACE GetCullFaceMode()	{ return ( m_RenderState.m_eCullFaceMode); }
	virtual void SetCullFaceMode( ENUM_CULL_FACE eCullFaceMode, BOOL bForce = FALSE);
	virtual ENUM_CMP_FUNC GetZFunc()	{ return ( m_RenderState.m_eZFunc); }
	virtual void SetZFunc( ENUM_CMP_FUNC eZFunc, BOOL bForce = FALSE);
	virtual DWORD GetColorMask()	{ return ( m_RenderState.m_dwColorMask); }
	virtual void SetColorMask( DWORD dwColorMask);
	virtual BOOL EnableAlphaSort( BOOL bEnable)	{ if ( !bEnable) { SetRenderState( RS_ALPHA_SORT_DISABLED); } else { UnsetRenderState( RS_ALPHA_SORT_DISABLED); } return TRUE; }
	virtual BOOL EnableShadow( BOOL bEnable)	{ if ( !bEnable) { SetRenderState( RS_SHADOW_DISABLED); } else { UnsetRenderState( RS_SHADOW_DISABLED); } return TRUE; }
	virtual BOOL EnableShadowForAlphaObject( BOOL bEnable)	{ if ( bEnable) { SetRenderState( RS_SHADOW_FOR_ALPHA_OBJECT); } else { UnsetRenderState( RS_SHADOW_FOR_ALPHA_OBJECT); } return TRUE; }
	virtual BOOL EnableFog( BOOL bEnable)	{ if ( bEnable)	{ SetRenderState( RS_FOG_ENABLED); } else { UnsetRenderState( RS_FOG_ENABLED); } return TRUE; }	// 객체에 따라 Fog 켜고 끄기
	virtual BOOL IsFogEnabled()	{ return ( CheckRenderState( RS_FOG_ENABLED)); }
	virtual BOOL TurnOnFog( BOOL bOn)	{ if ( bOn)	{ SetRenderState( RS_FOG_TURNED_ON); } else { UnsetRenderState( RS_FOG_TURNED_ON); } return TRUE; }	// 실제 render state 를 바꾸는 처리
	/// @name Texture 처리
	virtual BOOL EnableTexture( BOOL bEnable)	{ if ( !bEnable) { SetRenderState( RS_TEXTURE_DISABLED); } else { UnsetRenderState( RS_TEXTURE_DISABLED); } return TRUE; }
	virtual BOOL IsTextureEnabled()	{ return ( !CheckRenderState( RS_TEXTURE_DISABLED)); }
	/// @name Stencil 관련
	virtual BOOL EnableStencil( BOOL bEnable)	{ if ( bEnable) { SetRenderState( RS_STENCIL_ENABLED); } else { UnsetRenderState( RS_STENCIL_ENABLED); } return TRUE; }
	virtual BOOL SetStencilFunc( ENUM_CMP_FUNC eStencilFunc, DWORD dwRef, DWORD dwMask) = 0;
	virtual BOOL SetStencilOp( ENUM_STENCIL_OP eStencilOpFail, ENUM_STENCIL_OP eStencilOpZFail, ENUM_STENCIL_OP eStencilOpPass) = 0;
	/// @name Depth Bias 관련( Offset = m * SlopeScaleDepthBias + DepthBias)
	virtual void SetDepthBias( float* pfDepthBias, float* pfSlopeScaleDepthBias = NULL, BOOL bForce = FALSE);

	// ---------------------------------------------------------------------------------------------
	/// @name --- 설정 변경
protected:
	int m_iBackBufferCount;

	/// @name 설정 변경하기
protected:
	St_ScreenFormat m_CurrentScreenFormat;	///< 현재 화면 포맷
	St_RenderOption m_CurrentRenderOption;	///< 사용자가 선택한 옵션 - 이 값은 CWzdBase 의 static 멤버에 복사된다.
	St_RenderCaps m_RenderCaps;		///< 현재 그래픽카드의 처리 가능 범위
	St_RenderState m_RenderState;	///< Render State 저장
	St_StateParams m_StateParams;		///< 상태 파라미터들
protected:
	virtual void FillRenderCaps() = 0;
	void SetRenderState( DWORD dwFlag)		{ m_RenderState.m_dwRenderState |= dwFlag; }
	void UnsetRenderState( DWORD dwFlag)	{ m_RenderState.m_dwRenderState &= ~dwFlag; }
public:
	BOOL CheckRenderState( DWORD dwFlag)	{ return ( !!( m_RenderState.m_dwRenderState & dwFlag)); }
public:
	virtual BOOL SetRenderOption( const St_RenderOption& RenderOption);
	virtual void GetRenderOption( St_RenderOption *pRenderOption);
	virtual const St_RenderOption* GetRenderOption() const;
	virtual void GetScreenFormat( St_ScreenFormat *pScreenFormat);
	virtual BOOL SelectMode( St_ScreenFormat ScreenFormat);
	virtual int GetAvailableScreenSettings( St_ScreenSetting *pScreenSettings, int iMaxCount) = 0;
	virtual BOOL IsFullScreen()	{ return ( m_CurrentScreenFormat.m_bFullScreen); }
	virtual St_RenderCaps GetRenderCaps()	{ return ( m_RenderCaps); }
	virtual void SetClearColor( BYTE byRed, BYTE byGreen, BYTE byBlue, BYTE byAlpha = 0)	{ m_StateParams.m_wcClear = WzColor_RGBA( byRed, byGreen, byBlue, byAlpha); }
	virtual WzColor GetClearColor()	{ return m_StateParams.m_wcClear; }
	virtual void SetColor( WzColor wcDraw, ENUM_WRIC ewColorType = WRIC_DIFFUSE) = 0;
	virtual void SetColor( BYTE byRed, BYTE byGreen, BYTE byBlue, BYTE byAlpha = 255, ENUM_WRIC ewColorType = WRIC_DIFFUSE)	{ SetColor( WzColor_RGBA( byRed, byGreen, byBlue, byAlpha), ewColorType); }
	virtual WzColor GetDiffuseColor() { return m_RenderState.m_wcDraw[WRIC_DIFFUSE]; }
	virtual WzColor GetAmbientColor() { return m_RenderState.m_wcDraw[WRIC_AMBIENT]; }
	virtual WzColor GetSpecularColor() { return m_RenderState.m_wcDraw[WRIC_SPECULAR]; }
	virtual ENUM_ALPHABLEND GetAlphaBlendMode()	{ return ( m_RenderState.m_abMode); }
	virtual void SetAlphaBlendMode( ENUM_ALPHABLEND abMode, BOOL bZWriteModeChange = TRUE) = 0;
	virtual void GetAlphaTestMode( DWORD *pdwAlphaTestReference, ENUM_CMP_FUNC *peAlphaTestFunc)	{ *pdwAlphaTestReference = m_RenderState.m_dwAlphaTestReference; *peAlphaTestFunc = m_StateParams.m_eAlphaTestFunc; }
	virtual void SetAlphaTestMode( DWORD dwAlphaTestReference, ENUM_CMP_FUNC eAlphaTestFunc = CF_GREATEREQUAL, BOOL bForce = FALSE) = 0;
	virtual void EnableAlphaTest( BOOL bEnable, BOOL bForce=FALSE) = 0;
	virtual BOOL IsEnableAlphaTest() const { return m_StateParams.m_bEnableAlphaTest; }
	virtual void SetZBufferWrite( BOOL bZWriteEnable) = 0;
	virtual void SetZBufferTest( BOOL bZTestEnable) = 0;
	virtual void EnableSpecular( BOOL bEnable, BOOL bForce=FALSE) = 0;
	virtual BOOL IsSpecular() { return m_StateParams.m_bEnableSpecular; }
	virtual BOOL IsEnableZWrite() const;
	virtual BOOL IsEnableZTest() const;
	virtual void ShowSubTypeEffect(CWzUnitDraw *pUnitDraw ,BOOL bShow)=0;


	virtual BOOL StartHyperRenderMode( int iNumVertices) = 0;
	virtual BOOL EndHyperRenderMode() = 0;

	// ---------------------------------------------------------------------------------------------
	/// @name Default Index Map
	static int s_iMaxDefaultIndexMap;
	static int *s_piDefaultIndexMap;
protected:
	void EnlargeDefaultIndexMap( int iSize);
	void DestroyDefaultIndexMap();


	// ---------------------------------------------------------------------------------------------
	/// @name --- Wzd 메쉬 그리기


protected:

	/// @name WzUnit 관련
public:
	virtual ENUM_ANI_FRAME_STATUS ProgressUnitFrame( CWzUnitDraw *pUnitDraw, float fFrameAdd, WzEvent *pEvent = NULL, float *pfProgressRate = NULL) = 0;
	virtual void AnimateUnit( CWzUnitDraw *pUnitDraw, int iLodLevel = 0, const WzMatrix *pwmParent = NULL, BOOL bUpdateBVExtra = FALSE) = 0;
	virtual BOOL RenderUnit( CWzUnitDraw *pUnitDraw, St_RenderCommand *prc = NULL, int iLodLevel = 0) = 0;
    virtual BOOL RenderUnitForOutline(CWzUnitDraw* unit_draw) = 0;
	virtual BOOL RenderUnitBone( CWzUnitDraw *pUnitDraw, int iStart = 0, int iEnd = -1) = 0;
    virtual BOOL RenderGrass(CWzUnitDraw *unitdraw_ptr) = 0;
	virtual BOOL CheckUnitLoaded( CWzUnitDraw *pUnitDraw); // MultiThread 등으로 로드가 되었는지 체크(add 2006/01/24 - koma0)

	virtual void CheckEffect( CWzUnitDraw *pUnitDraw, BOOL bCreateNow = FALSE ) = 0;// 이펙트생성이 안되었으면 생성
    virtual void MoveUnitEffect( CWzUnitDraw *pUnitDraw ) = 0;


	/// @name Wzd - HANDLE 관련
public:
	virtual WORD GetNumFrameOfAnimation( HANDLE hWzdAnimation) = 0;

	/// @name Wzd - 직접 접근
public:
	virtual void DestroyTextureForWzdPtr( CWzdBase *pWzd) = 0;

	/// @name 삼각형 그리기
public:
	/*
	추가된 파라미터:
		1. pwvCenterPos: NULL일 경우, 소트시 사용될 위치 정보를 내부에서 계산.
		2. pAlterRender: NULL일 경우, RenderType은 AL_NORMAL.
	*/

	virtual BOOL RenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzVector *pwvNormal, WzUVVert *pwUVs, int iDataSize = sizeof ( WzVector), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderTrianglesIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzVector *pwvNormal, int *piNormalMap, WzUVVert *pwUVs, int *piUVMap, int iDataSize = sizeof ( WzVector), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderTrianglesColoredIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzColor *pwcColor, int *piColorMap, WzUVVert *pwUVs, int *piUVMap, int iVertDataSize = sizeof ( WzVector), int iColorDataSize = sizeof ( WzColor), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderTrianglesPacked( HANDLE hTexture, int iNumFace, int *piVertexMap, WzVector *pwvPosVert, int *piPosVertIndex, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNormal, int iVertexDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNormalDataSize = sizeof ( WzVector), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderVertexBuffer( HANDLE hTexture, HANDLE hVertexBuffer, int iNumFace, int *piIndex, WzUVVert *pwuvStream = NULL, int iVertexShader = 0, int iPixelShader = 0, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual void SetUVStream( WzUVVert *pwuvStream = NULL) = 0;
	virtual BOOL RenderSimpleMesh( WzSimpleMesh *pwsmSimpleMesh, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);

	// pwvVertex와 pwUVs는 NULL일 수 없다.
	// Strip으로 렌더링시 NRTO_STRIP 플래그를 조합.
	// pwvCenterPos값은 NULL을 경우, 내부계산(최적화시 반드시 직접 넣는다) 
	// 인덱스값(pi__Index)은 경우에 따라 NULL 사용 가능.
	virtual BOOL New_RenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzUVVert *pwUVs, WzColor *pwcColor, int* piVertexIndex, int* piUVIndex, int* piColorIndex, int iVertexSize = sizeof( WzVector), int iUVSize = sizeof( WzUVVert), int iColorSize = sizeof( WzColor), DWORD dwFlags = NRTO_DEFAULT, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL New_RenderSprite( HANDLE hTexture, ENUM_ALPHABLEND eabMode, WzVector *pwvVertex, WzUVVert *pwUVs, WzColor *pwcColor, int* piVertexIndex, int* piUVIndex, int* piColorIndex, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL );


	///< 실제 삼각형 그리기
	virtual BOOL RealizeRenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzVector *pwvNormal, WzUVVert *pwUVs, int iDataSize = sizeof ( WzVector)) = 0;
	virtual BOOL RealizeRenderTrianglesIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzVector *pwvNormal, int *piNormalMap, WzUVVert *pwUVs, int *piUVMap, int iDataSize = sizeof ( WzVector)) = 0;
	virtual BOOL RealizeRenderTrianglesColoredIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzColor *pwcColor, int *piColorMap, WzUVVert *pwUVs, int *piUVMap, int iVertDataSize = sizeof ( WzVector), int iColorDataSize = sizeof ( WzColor)) = 0;
	virtual BOOL RealizeRenderTrianglesPacked( HANDLE hTexture, int iNumFace, int *piVertexMap, WzVector *pwvPosVert, int *piPosVertIndex, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNormal, int iVertexDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNormalDataSize = sizeof ( WzVector)) = 0;
	virtual BOOL RealizeRenderTrianglesPackedMultiTexture( HANDLE hTexture, int iNumFace, int *piVertexMap, WzVector *pwvPosVert, int *piPosVertIndex, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNormal, int *piSecondUVMap, WzUVVert *pwSecondUVs, int iVertexDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNormalDataSize = sizeof ( WzVector), int iSecondUVSize = sizeof (WzUVVert)) = 0;
    virtual BOOL RealizeRenderTrianglesColored( int iNumFace, WzVector* pwvVertex ) = 0;

	virtual BOOL RealizeRenderVertexBuffer( HANDLE hTexture, HANDLE hVertexBuffer, int iNumFace, int *piIndex, WzUVVert *pwuvStream = NULL, int iVertexShader = 0, int iPixelShader = 0, BOOL bZWriteModeChange = TRUE, HANDLE hAdditionalVB = INVALID_HANDLE_VALUE) = 0;
	virtual BOOL RealizeRenderIndexBuffer( HANDLE hTexture, HANDLE hVertexBuffer, HANDLE hIndexBuffer, int iNumFace, int iStartIndex = 0, WzUVVert *pwuvStream = NULL, int iVertexShader = 0, int iPixelShader = 0, BOOL bZWriteModeChange = TRUE, HANDLE hAdditionalVB = INVALID_HANDLE_VALUE) = 0;
	virtual BOOL RealizeRenderSimpleMesh( WzSimpleMesh *pwsmSimpleMesh) = 0;
	virtual BOOL RealizeTriRenderCommmand( St_TriRenderCommand *prc);
	virtual BOOL New_RealizeRenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzUVVert *pwUVs, 
		WzColor *pwcColor, int* piVertexIndex, int* piUVIndex, int* piColorIndex, int iVertexSize = sizeof( WzVector),
		int iUVSize = sizeof( WzUVVert), int iColorSize = sizeof( WzColor), DWORD dwFlags = NRTO_DEFAULT) = 0;




	/// @name 2D 그리기 ( Dst 좌표들은 1024x768 해상도를 기준으로 넣으면 되고, Src 좌표들은 텍스쳐 이미지 상의 좌표를 기준으로 넣으면 된다.
public:
	virtual BOOL SaveScreen( char *lpszFilePath ) = 0;	///< 화면 저장 함수
    virtual void GetDisplayMode(D3DDISPLAYMODE* pDisplayMode) = 0; ///< 화면모드얻기

    //------------------------------------------------------------------------------
    // Remake : 외부에서 버프를 만들어 해당 버프 만큼 2d 텍스쳐 랜더링
    // Parameter : HANDLE texture_handle : 텍스쳐 핸들
    // Parameter : int vertices_count : 버텍스 갯수
    // Parameter : BYTE * buffer : (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) * vertices_count 만큼 필요
    //------------------------------------------------------------------------------
    virtual CElementRender* GetElementRender() = 0;
    virtual bool RenderTextureTriangle(HANDLE texture_handle, int vertices_count, BYTE* buffer) = 0;
	virtual BOOL RenderTexture( HANDLE hTexture, float fxDst, float fyDst, float fDstWidth, float fDstHeight, float fxSrc = 0.0f, float fySrc = 0.0f, float fSrcWidth = 0.0f, float fSrcHeight = 0.0f, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT, float fRot = 0.f) = 0;
	virtual BOOL RenderTexture( HANDLE hTexture, float *pfxDest, float *pfyDest, float *pfxSrc, float *pfySrc, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT) = 0;	// 4개의 정점을 시계방향으로 포인터에 넣어주면 됨 ( pfxSrc, pfySrc 배열의 내부 값에 -1.0f 를 넣으면 Texture 전체가 render 됨)
	virtual BOOL ColorFill( WzColor wcColor, float fxDst, float fyDst, float fDstWidth, float fDstHeight, WzColor *pwcColor = NULL, float fZ = 0.0f) = 0;
	virtual void Render2DLine( float fx1, float fy1, float fx2, float fy2, WzColor wcColor) = 0;
	virtual BOOL RenderTexture4SpinGage( HANDLE hTexture, float fxDst, float fyDst, float fDstWidth, float fDstHeight, float fxSrc = 0.0f, float fySrc = 0.0f, float fSrcWidth = 0.0f, float fSrcHeight = 0.0f, float fRate = 100.f, WzColor *pwcColor = NULL, float fZ = 0.0f) = 0;
	float StdXToRealX( float x)		{return ( m_b2DModeInRealSize ? x : ( x ) * m_rtWindowClient.right / (float)m_fStd2DWidth);}
	float StdYToRealY( float y)		{return ( m_b2DModeInRealSize ? y : ( y ) * m_rtWindowClient.bottom / (float)m_fStd2DHeight);}
	float RealXToStdX( float x)		{return ( m_b2DModeInRealSize ? x : ( ( x ) * (float)m_fStd2DWidth / m_rtWindowClient.right) ) + 0.5f;}
	float RealYToStdY( float y)		{return ( m_b2DModeInRealSize ? y : ( ( y ) * (float)m_fStd2DHeight / m_rtWindowClient.bottom) ) + 0.5f;}

	virtual BOOL StartHyper2DMode() = 0;
	virtual void EndHyper2DMode() = 0;
	virtual void Get3DPosOn2DMode( WzVector *pwvResult, float fxPos, float fyPos);

	virtual BOOL RenderTextureBlending( HANDLE hTexture1, HANDLE hTexture2, float fxDst, float fyDst, 
		float fDstWidth, float fDstHeight, float fxSrc1 = 0.0f, float fySrc1 = 0.0f, float fSrcWidth1 = 0.0f, 
		float fSrcHeight1 = 0.0f, float fxSrc2 = 0.0f, float fySrc2 = 0.0f, float fSrcWidth2 = 0.0f, 
		float fSrcHeight2 = 0.0f, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT) = 0;


public:
	//! 타켓 텍스처에 Text를 그린다
	// 하나의 Text만 그릴 경우, RenderText(...,타켓텍스처핸들)
	// 여러개의 Text를 영역별로 그릴 경우, BeginText(타켓텍스처핸들)-RenderText(...,INVALID_HANDLE_VALUE)-EndText()
	// - prcRegion값이 NULL일 경우, 텍스처 전체 크기가 대상이 된다.
	// - RECT 영역을 실제 텍스쳐 크기비율대로 사용하려면 이 함수를 사용하기전에 Set2DMode(0,TRUE)를 호출한다.
	// - RECT 영역에 대한 정보는 CManagerText::GetTextRect()를 통해 알수있다.
	// - lpszWzIDFont가 NULL일 경우, 현재 설정된(StartToUseFont) 폰트를 사용한다.
	virtual BOOL RenderText( char *lpszText, WzColor wcText, WzColor wcBack=WzColor_RGBA( 0, 0, 0, 0), 
		DWORD dwStyle=TP_HLEFT, RECT *prcRegion=NULL, char *lpszWzIDFont=NULL, HANDLE hTargetTexture=INVALID_HANDLE_VALUE) = 0;
	
	virtual BOOL BeginText( HANDLE hTargetTexture);
	virtual BOOL EndText();
protected:
	HANDLE m_hTextTexture;



public:
	RECT GetBackBufferSize();

	/// @name Silhouette/ShadowVolume 그리기
public:
	virtual BOOL RenderSilhouette( CWzSilhouette *pws, WzColor wcColor = WzColor_RGB( 0, 0, 0), float fLineWidth = 1.0f) = 0;
	virtual BOOL RenderShadowVolume( CWzShadowVolume *pws) = 0;
	virtual void SetShadowColor( WzColor wcShadow) = 0;
	virtual WzColor GetShadowColor() = 0;

	/// @name 기타 그리기 - 느리므로 Release 버전에는 쓰지 말 것
	//	해당 그리기는 모아서 그리지 않고, RealizeRender***()로 호출됨.
public:
	virtual BOOL RenderBone( HANDLE hWzd, int iStart = 0, int iEnd = -1) = 0;
	virtual BOOL RenderDot( WzVector wvPos, WzColor wcColor, float fDotSize = 1.0f) = 0;
	virtual BOOL RenderLine( WzVector wvPos1, WzVector wvPos2, WzColor wcColor, float fLineWidth = 1.0f) = 0;
	virtual BOOL RenderPyramid( WzVector wvPos1, WzVector wvPos2, WzColor wcColor) = 0;
	virtual BOOL RenderBoundingVolume( const WzBoundingVolume *pbv) = 0;
	virtual BOOL RenderBvAABB( const BvAABB *paabb) = 0;
	virtual BOOL RenderBvSphere( const BvSphere *pbvs) = 0;


	/// @name 알파 렌더 리스트/스프라이트/빌보드 그리기
public:
	virtual BOOL RenderSprite( HANDLE hTexture, const WzVector& wvPos, ENUM_ALPHABLEND eabMode = AB_ALPHA, float fSize = 1.0f, float fRotate = 0.0f, float fDepthOffset = 0.0f, WzUVVert *pwuvRegion = NULL, ENUM_BILLBOARD_TYPE eType = EBBT_SPRITE, WzVector* pResultPos = NULL, St_AlterRender* pAlterRender = NULL);
	virtual BOOL RenderSprite( HANDLE hTexture, const WzVector& wvPos, ENUM_ALPHABLEND eabMode, float fXSize, float fYSize, float fRotate = 0.0f, float fDepthOffset = 0.0f, WzUVVert *pwuvRegion = NULL, ENUM_BILLBOARD_TYPE eType = EBBT_SPRITE, WzVector* pResultPos = NULL, St_AlterRender* pAlterRender = NULL) = 0;

	virtual BOOL RealizeRenderSprite( HANDLE hTexture, const WzVector& wvPos, float fSize = 1.0f, float fRotate = 0.0f, float fDepthOffset = 0.0f, WzUVVert *pwuvRegion = NULL, ENUM_BILLBOARD_TYPE eType = EBBT_SPRITE, WzVector* pResultPos = NULL, St_RenderState* pRS = NULL);
	virtual BOOL RealizeRenderSprite( HANDLE hTexture, const WzVector& wvPos, float fXSize, float fYSize, float fRotate = 0.0f, float fDepthOffset = 0.0f, WzUVVert *pwuvRegion = NULL, ENUM_BILLBOARD_TYPE eType = EBBT_SPRITE, WzVector* pResultPos = NULL, St_RenderState* pRS = NULL) = 0;
	virtual BOOL RealizeNewRenderSprite(HANDLE hTexture, const SpriteVerts*  wvVerts, St_RenderState* pRS = NULL) = 0;
	virtual BOOL RealizeSpriteRenderCommmand( St_SpriteRenderCommand *prc);
	virtual BOOL RealizeNewSpriteRenderCommmand( St_NewSpriteRenderCommand *prc );
    virtual void FlushRenderSprite(bool bAllRenderSprite) {}
    virtual void NewFlushRenderSprite(bool bSortedRenderSprite, bool bUnSortedRenderSprite) {}

protected:
    // 메시 렌더링
    virtual BOOL New_RealizeDrawingPrimitiveVS( St_RenderCommand* prc, int iVS, int iPS = 0 ) { return TRUE; }

public:
	virtual void DestroyResourceForWzUnitDraw( CWzUnitDraw *pUnitDraw );


	// @name 감마 정보 
public:
	// 풀스크린 모드에서 적용됨
	// 색상 당 256개의 인덱스를 가짐 red[256], green[256], blue[256]
	// 그 값은 0~65535값을 가진다.
	virtual BOOL GetGammaRamp( WORD* pwOutRed, WORD* pwOutGreen, WORD* pwOutBlue ) const = 0;
	virtual BOOL SetGammaRamp( const WORD* pwColor) = 0;
	virtual BOOL SetGammaRamp( const WORD* pwRed, const WORD* pwGreen, const WORD* pwBlue) = 0;
	virtual BOOL SetGammaRamp( int iMinIndex, WORD wMinValue, int iMaxIndex, WORD wMaxValue) = 0;

public:
	/// @name 사용가능한/사용된 텍스처 메모리
	virtual UINT GetAvailableTextureMem() const;
	virtual UINT GetUsedTextureMem() const;
	virtual UINT GetAvailableVidMem() const;
	virtual UINT GetUsedVidMem() const;
	virtual UINT GetLocalVidMem() const;

	/// @name water type 관련
protected:
	ENUM_WATER_TYPE m_eWaterType;

public:
	virtual void SetWaterType( ENUM_WATER_TYPE eType){ m_eWaterType = eType; }
	virtual ENUM_WATER_TYPE GetWaterType(){ return m_eWaterType; }


	/// @name DetailTexture VS/PS constant 관련
public:
	virtual void SetDetailTextureScale( float fScale );
	virtual void SetDetailFunc( int nIndex, float fFunc);

	/// @name TerrainGlowFactor PS constant 관련
public:
	virtual void SetTerrainGlowFactor( float fFactor);
	virtual float CalculateGlowFactor( float fMin, float fMax, float fTime);

	virtual void SetTerrainHeightFactor( float fHeightFactor );

	/// @name IndexBuffer 32 지원 관련
public:
	virtual BOOL IsSupportedIndex32() = 0;


	/// @name for Occlusion Culling
public:
	virtual void EnableOcclusionCulling( BOOL bEnable, BOOL bShouldBlockOnQuery = FALSE) { m_bUseOcclusionCulling = bEnable; m_bShouldBlockOnQuery = bShouldBlockOnQuery; }
	virtual BOOL IsOcclusionCulling() const { return m_bUseOcclusionCulling; }
	virtual BOOL IsShouldBlockOnQuery() const { return m_bShouldBlockOnQuery; }
protected:
	BOOL m_bUseOcclusionCulling;
	BOOL m_bShouldBlockOnQuery;


public:
	virtual void OutputAllMemoryInfo();
};



// inline
inline BOOL CDrawBase::CheckUnitLoaded( CWzUnitDraw *pUnitDraw)
{
    CWzUnit* pUnit = pUnitDraw->GetWzUnit();
    if( pUnit)
    {
        return ( pUnit->GetLoadState() != eLOAD_NONE );
    }

    return FALSE;
}

inline BOOL CDrawBase::SetDirectionalLight( int iIndex, WzColor wcColor, const WzVector& wvDirection, ENUM_LIGHT_IRRA_TYPE eliType, float fRange)
{
	return x_pManagerLight->SetDirectionalLight( iIndex, wcColor, wvDirection, eliType, fRange );
}

inline BOOL CDrawBase::SetPointLight( int iIndex, WzColor wcColor, const WzVector& wvPosition, ENUM_LIGHT_IRRA_TYPE eliType, float fRange)
{
	return x_pManagerLight->SetPointLight( iIndex, wcColor, wvPosition, eliType, fRange );
}

inline BOOL CDrawBase::SetLightIntensity( int iIndex, float fIntensity )
{
	return x_pManagerLight->SetLightIntensity( iIndex, fIntensity );
}

inline float CDrawBase::GetLightIntensity( int iIndex )
{
	return x_pManagerLight->GetLightIntensity( iIndex );
}

inline const St_LightInformation* CDrawBase::GetLightInfo( int iIndex) const
{
	return x_pManagerLight->GetLightInfo( iIndex );
}

inline BOOL CDrawBase::TurnOnLight( int iIndex, BOOL bOn)
{
	return x_pManagerLight->TurnOnLight( iIndex, bOn );
}

inline int CDrawBase::GetIndexOfActiveLight( int iActive) const
{
	return x_pManagerLight->GetIndexOfActiveLight( iActive );
}

inline const St_LightInformation* CDrawBase::GetActiveLightInfo( int iActive) const
{
	return x_pManagerLight->GetActiveLightInfo( iActive );
}

inline int CDrawBase::GetNumberOfActiveLight() const
{
	return x_pManagerLight->GetNumberOfActiveLight();
}


inline void CDrawBase::SetIrradianceFilePath( LPSTR lpszPath )
{
	CManagerLight::SetFilePath( lpszPath );
}

inline void CDrawBase::EnableAreaLight( BOOL bEnable)
{
	x_pManagerLight->EnableAreaLight( bEnable);
}

inline BOOL CDrawBase::IsAreaLight() const
{
	return x_pManagerLight->IsAreaLight();
}

inline UINT CDrawBase::GetUsedTextureMem() const
{
	return x_pManagerTexture->GetUsedTextureMem();
}

inline UINT CDrawBase::GetAvailableTextureMem() const
{
	return x_pManagerTexture->GetAvailableTextureMem();
}
inline UINT CDrawBase::GetAvailableVidMem() const
{
	return x_pManagerTexture->GetAvailableVidMem();

}

inline UINT CDrawBase::GetUsedVidMem() const
{
	return x_pManagerTexture->GetUsedVidMem();
}


inline UINT CDrawBase::GetLocalVidMem() const
{
	return x_pManagerTexture->GetLocalVidMem();
}


/// @name --- 그리기 모드 inline

inline void CDrawBase::SetMeshDrawMode( ENUM_MESH_DRAW eMeshDrawMode, BOOL bForce)
{
	m_RenderState.m_eMeshDrawMode = eMeshDrawMode;
}

inline void CDrawBase::SetCullFaceMode( ENUM_CULL_FACE eCullFaceMode, BOOL bForce)
{
	m_RenderState.m_eCullFaceMode = eCullFaceMode;
}

inline void CDrawBase::SetZFunc( ENUM_CMP_FUNC eZFunc, BOOL bForce)
{
	m_RenderState.m_eZFunc = eZFunc;
}

inline void CDrawBase::SetColorMask( DWORD dwColorMask)
{
	m_RenderState.m_dwColorMask = dwColorMask;
}

inline BOOL CDrawBase::IsEnableZWrite() const
{
	return (m_RenderState.m_dwRenderState & RS_ZWRITE_ENABLED);
}

inline BOOL CDrawBase::IsEnableZTest() const
{
	return (m_RenderState.m_dwRenderState & RS_ZTEST_ENABLED);
}

inline void CDrawBase::SetDepthBias( float* pfDepthBias, float* pfSlopeScaleDepthBias, BOOL bForce)
{
}


/// @name --- 설정 변경 inline

inline void CDrawBase::GetRenderOption( St_RenderOption *pRenderOption)
{
	*pRenderOption = m_CurrentRenderOption;
}

inline const St_RenderOption* CDrawBase::GetRenderOption() const
{
	return &m_CurrentRenderOption;
}

inline void CDrawBase::GetScreenFormat( St_ScreenFormat *pScreenFormat)
{
	*pScreenFormat = m_CurrentScreenFormat;
}

//------------------------------------------------------------------------------ 
/// @name 삼각형 그리기 inline
inline BOOL CDrawBase::RealizeRenderSprite( HANDLE hTexture, const WzVector& wvPos, 
                                           float fSize, float fRotate, float fDepthOffset, 
                                           WzUVVert *pwuvRegion, 
                                           ENUM_BILLBOARD_TYPE eType, WzVector* pResultPos, 
                                           St_RenderState* pRS )
{
	return RealizeRenderSprite( hTexture, wvPos, fSize, fSize, fRotate, fDepthOffset, pwuvRegion, eType, pResultPos, pRS);
}

//------------------------------------------------------------------------------ 
inline BOOL CDrawBase::RenderSprite( HANDLE hTexture, const WzVector& wvPos, 
                                    ENUM_ALPHABLEND eabMode, float fSize, float fRotate, 
                                    float fDepthOffset, WzUVVert *pwuvRegion, 
                                    ENUM_BILLBOARD_TYPE eType, WzVector* pResultPos, 
                                    St_AlterRender* pAlterRender)
{
	return RenderSprite( hTexture, wvPos, eabMode, fSize, fSize, fRotate, fDepthOffset, pwuvRegion, eType, pResultPos, pAlterRender);
}
//------------------------------------------------------------------------------ 
inline BOOL CDrawBase::New_RenderSprite( HANDLE hTexture, ENUM_ALPHABLEND eabMode, 
                                        WzVector *pwvVertex, WzUVVert *pwUVs, WzColor *pwcColor, 
                                        int* piVertexIndex, int* piUVIndex, int* piColorIndex, 
                                        WzVector* pwvCenterPos, St_AlterRender* pAlterRender )
{
	return New_RenderSprite(hTexture, eabMode, pwvVertex, pwUVs, pwcColor, piVertexIndex, piUVIndex, piColorIndex, pwvCenterPos, pAlterRender);
}

//------------------------------------------------------------------------------ 
inline void InitScreenFormatAndRenderOption( St_ScreenFormat *pScreenFormat, St_RenderOption *pRenderOption)
{
    // 화면 포맷
    pScreenFormat->m_bUseDialogMode = FALSE;
    pScreenFormat->m_bFullScreen = FALSE;
    pScreenFormat->m_bMaximizeScreen = false;
    pScreenFormat->m_dwWidth = 640;
    pScreenFormat->m_dwHeight = 480;
    pScreenFormat->m_iRed = 8;
    pScreenFormat->m_iGreen = 8;
    pScreenFormat->m_iBlue = 8;
    pScreenFormat->m_iAlpha = 0;
    pScreenFormat->m_iDepthBits = 16;
    pScreenFormat->m_iStencilBits = 0;
    pScreenFormat->m_iMultiSampling = 0;
    pScreenFormat->m_bUseInitWindowStyle = false;

    // 화면 옵션
    pRenderOption->m_eShadingType = ST_DISABLED;
    pRenderOption->m_eShadowType = SWT_DISABLED;
    pRenderOption->m_eTextureFormat = TF_BEST;
    pRenderOption->m_bShaderUse = TRUE;
    pRenderOption->m_bUseSpecularMap = TRUE;
    pRenderOption->m_bFogOn = TRUE;
    pRenderOption->m_fMipmapBias = -2.0f;
    //pRenderOption->m_iTextureBit = 32;
    pRenderOption->m_iTextureLarge = 0;
    pRenderOption->m_iTextureSmall = 0;
    pRenderOption->m_fTextureRate = 1.0f;
    pRenderOption->m_iDDS = 0; 
    pRenderOption->m_iCompressedNormalMap = 0;
    pRenderOption->m_iTerrainWater = 2;
    pRenderOption->m_iTerrainSpecular = 2;
    pRenderOption->m_iTerrainDetail = 1;
    pRenderOption->m_iSkinningBone = MIN_BONE_FOR_SKINNING;
    pRenderOption->m_iLightmap = 1;

    pRenderOption->m_bRenderShadow = TRUE;
}
//------------------------------------------------------------------------------ 
inline BOOL LoadScreenFormatAndRenderOption( const char *lpszFilePath, St_ScreenFormat *pScreenFormat, St_RenderOption *pRenderOption)
{
    char lpszTemp[1024];
    char *lpszSectionScreen = "Screen";
    char *lpszSectionPerformance = "Performance";

    // 화면 포맷
    GetPrivateProfileStringA( lpszSectionScreen, "FullScreen", "0", lpszTemp, 1023, lpszFilePath );
    pScreenFormat->m_bFullScreen = ParamStr2Bool( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "MaximizeScreen", "1", lpszTemp, 1023, lpszFilePath );
    pScreenFormat->m_bMaximizeScreen = (ParamStr2Bool( lpszTemp) == TRUE);

    //GetPrivateProfileStringA( lpszSectionScreen, "Width", "1024", lpszTemp, 1023, lpszFilePath);
    //pScreenFormat->m_dwWidth = ParamStr2Int( lpszTemp);
    //GetPrivateProfileStringA( lpszSectionScreen, "Height", "768", lpszTemp, 1023, lpszFilePath);
    //pScreenFormat->m_dwHeight = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "Width", "0", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_dwWidth = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "Height", "0", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_dwHeight = ParamStr2Int( lpszTemp);


    GetPrivateProfileStringA( lpszSectionScreen, "Red", "8", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iRed = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "Green", "8", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iGreen = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "Blue", "8", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iBlue = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "Alpha", "0", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iAlpha = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "DepthBits", "24", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iDepthBits = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "StencilBits", "0", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iStencilBits = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "UseDialogMode", "0", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_bUseDialogMode = ParamStr2Bool( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "MultiSampling", "0", lpszTemp, 1023, lpszFilePath);
    pScreenFormat->m_iMultiSampling = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionScreen, "UseInitWindowStyle", "0", lpszTemp, 1023, lpszFilePath );
    pScreenFormat->m_bUseInitWindowStyle = (ParamStr2Bool( lpszTemp) == TRUE);


    // 화면 옵션
    GetPrivateProfileStringA( lpszSectionPerformance, "Shading", "3", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_eShadingType = ( ENUM_SHADING_TYPE)ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "Shadow", "3", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_eShadowType = ( ENUM_SHADOW_TYPE)ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TextureFormat", "0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_eTextureFormat = ( ENUM_TEXTURE_FORMAT)ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "Shader", "1", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_bShaderUse = ParamStr2Bool( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "Specular", "1", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_bUseSpecularMap = ParamStr2Bool( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "Fog", "1", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_bFogOn = ParamStr2Bool( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "MipmapBias", "-1.0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_fMipmapBias = (float)atof( lpszTemp);
    //GetPrivateProfileStringA( lpszSectionPerformance, "TextureBit", "32", lpszTemp, 1023, lpszFilePath);
    //pRenderOption->m_iTextureBit = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TextureLarge", "0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iTextureLarge = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TextureSmall", "0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iTextureSmall = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TextureRate", "1.0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_fTextureRate = (float)atof( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "DDS", "1", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iDDS = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "CompressedNormalMap", "0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iCompressedNormalMap = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TerrainWater", "2", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iTerrainWater = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TerrainSpecular", "1", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iTerrainSpecular = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "TerrainDetail", "0", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iTerrainDetail = ParamStr2Int( lpszTemp);
    GetPrivateProfileStringA( lpszSectionPerformance, "SkinningBone", "2", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iSkinningBone = __max( MIN_BONE_FOR_SKINNING, 	__min( SUPPORT_BONE_FOR_SKINNING, ParamStr2Int( lpszTemp)));
    GetPrivateProfileStringA( lpszSectionPerformance, "Lightmap", "1", lpszTemp, 1023, lpszFilePath);
    pRenderOption->m_iLightmap = ParamStr2Int( lpszTemp);

    //------------------------------------------------------------------------------ 
    if ((pScreenFormat->m_dwWidth == 0) || (pScreenFormat->m_dwHeight == 0))
    {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        float aspect = (float)width / (float)height;

        if (aspect <= 1.4f)  
        {
            pScreenFormat->m_dwWidth = static_cast<DWORD>(STD_2D_WIDTH);
            pScreenFormat->m_dwHeight = static_cast<DWORD>(STD_2D_HEIGHT);
        }
        else if (aspect <= 1.65f) 
        {
            pScreenFormat->m_dwWidth = static_cast<DWORD>(STD_2D_WIDTH_HALF_WIDE);
            pScreenFormat->m_dwHeight = static_cast<DWORD>(STD_2D_HEIGHT_HALF_WIDE);
        }
        else
        {
            pScreenFormat->m_dwWidth = static_cast<DWORD>(STD_2D_WIDTH_WIDE);
            pScreenFormat->m_dwHeight = static_cast<DWORD>(STD_2D_HEIGHT_WIDE);
        }
    }
    if (pScreenFormat->m_bFullScreen == FALSE)
    {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        if (pScreenFormat->m_bMaximizeScreen == true)
        {
            pScreenFormat->m_dwWidth = width;
            pScreenFormat->m_dwHeight = height;
        }
        else if ((pScreenFormat->m_dwWidth == width) && (pScreenFormat->m_dwHeight == height))
        {
            pScreenFormat->m_bMaximizeScreen  = true;
        }
    }
    //------------------------------------------------------------------------------ 
    return TRUE;
}
//------------------------------------------------------------------------------ 
inline BOOL SaveScreenFormatAndRenderOption( const char *lpszFilePath, St_ScreenFormat *pScreenFormat, St_RenderOption *pRenderOption)
{
    char *lpszSectionScreen = "Screen";
    char *lpszSectionPerformance = "Performance";
    char lpszParamBuffer[1024];	// 파라미터 Save/Load 를 위한 버퍼

    // 화면 포맷
    WritePrivateProfileStringA( lpszSectionScreen, "FullScreen", ParamBool2Str( pScreenFormat->m_bFullScreen ), lpszFilePath );
    WritePrivateProfileStringA( lpszSectionScreen, "MaximizeScreen", ParamBool2Str(pScreenFormat->m_bMaximizeScreen), lpszFilePath );
    WritePrivateProfileStringA( lpszSectionScreen, "Width", ParamInt2Str( pScreenFormat->m_dwWidth, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "Height", ParamInt2Str( pScreenFormat->m_dwHeight, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "Red", ParamInt2Str( pScreenFormat->m_iRed, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "Green", ParamInt2Str( pScreenFormat->m_iGreen, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "Blue", ParamInt2Str( pScreenFormat->m_iBlue, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "Alpha", ParamInt2Str( pScreenFormat->m_iAlpha, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "DepthBits", ParamInt2Str( pScreenFormat->m_iDepthBits, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "StencilBits", ParamInt2Str( pScreenFormat->m_iStencilBits, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "UseDialogMode", ParamInt2Str( pScreenFormat->m_bUseDialogMode, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionScreen, "MultiSampling", ParamInt2Str( pScreenFormat->m_iMultiSampling, lpszParamBuffer), lpszFilePath);

    // 화면 옵션
    WritePrivateProfileStringA( lpszSectionPerformance, "Shading", ParamInt2Str( pRenderOption->m_eShadingType, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "Shadow", ParamInt2Str( pRenderOption->m_eShadowType, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "TextureFormat", ParamInt2Str( pRenderOption->m_eTextureFormat, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "Shader", ParamInt2Str( pRenderOption->m_bShaderUse, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "Specular", ParamInt2Str( pRenderOption->m_bUseSpecularMap, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "Fog", ParamInt2Str( pRenderOption->m_bFogOn, lpszParamBuffer), lpszFilePath);
    GetFloatString( lpszParamBuffer, pRenderOption->m_fMipmapBias, 2);
    WritePrivateProfileStringA( lpszSectionPerformance, "MipmapBias", lpszParamBuffer, lpszFilePath);
    //WritePrivateProfileStringA( lpszSectionPerformance, "TextureBit", ParamInt2Str(pRenderOption->m_iTextureBit, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "TextureLarge", ParamInt2Str(pRenderOption->m_iTextureLarge, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "TextureSmall", ParamInt2Str(pRenderOption->m_iTextureSmall, lpszParamBuffer), lpszFilePath);
    GetFloatString( lpszParamBuffer, pRenderOption->m_fTextureRate, 3);
    WritePrivateProfileStringA( lpszSectionPerformance, "TextureRate", lpszParamBuffer, lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "DDS", ParamInt2Str( pRenderOption->m_iDDS, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "CompressedNormalMap", ParamInt2Str( pRenderOption->m_iCompressedNormalMap, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "TerrainWater", ParamInt2Str( pRenderOption->m_iTerrainWater, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "TerrainSpecular", ParamInt2Str( pRenderOption->m_iTerrainSpecular, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "TerrainDetail", ParamInt2Str( pRenderOption->m_iTerrainDetail, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "SkinningBone", ParamInt2Str( pRenderOption->m_iSkinningBone, lpszParamBuffer), lpszFilePath);
    WritePrivateProfileStringA( lpszSectionPerformance, "Lightmap", ParamInt2Str( pRenderOption->m_iLightmap, lpszParamBuffer), lpszFilePath);
    return TRUE;
}
//------------------------------------------------------------------------------ 
inline DWORD GetAppWindowStyle(bool full_mode, bool max_screen)
{
    if (full_mode || max_screen)
    {
        return WS_POPUP;
    }
    else
    {
         return (WS_VISIBLE | WS_CLIPSIBLINGS | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
    }
}

#endif // _3DRENDERER_DRAWBASE_H_
