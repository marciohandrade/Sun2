// drawbase.h

#ifndef _3DRENDERER_DRAWBASE_H_
#define _3DRENDERER_DRAWBASE_H_

// xxx: ��ġ�°� ��ó�� ������ �ʴ�.
// ���� ���� ���� ����� �ƴ�����, ���� �� ���Ͽ� �־��� �����̾��� �ܺο���
// �� ����(drawbase.h)�� �ٷ� include�ϴ� ��찡 ���Ƽ� �ε��� �ϴ� �߰��Ѵ�.
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

#define RS_PROJECTION_PERSPECTIVE	( 0x00000001)	///< �۽���Ƽ�� ��������
#define RS_ZWRITE_ENABLED			( 0x00000010)	///< z ���� ����
#define RS_ZTEST_ENABLED			( 0x00000020)	///< z ���� �׽�Ʈ
#define RS_ALPHABLEND_ENABLED		( 0x00000040)	///< ���� ���� ����
#define RS_ALPHA_SORT_DISABLED		( 0x00000080)	///< ���� ���� �ڵ�ȭ
#define RS_FOG_ENABLED				( 0x00000100)	///< Fog ���
#define RS_FOG_TURNED_ON			( 0x00000200)	///< Fog ���
#define RS_TEXTURE_DISABLED			( 0x00000400)	///< Texture Off
#define RS_STENCIL_ENABLED			( 0x00000800)	///< Stencil ���
#define RS_SHADOW_DISABLED			( 0x00001000)	///< Shadow �ȱ׸���
#define RS_SHADOW_FOR_ALPHA_OBJECT	( 0x00002000)	///< Alpha Object �� Shadow �����ϱ�
#define RS_IN_TOGGLEFULLSCREEN		( 0x00100000)	///< Ǯ��ũ�� �����
#define RS_IN_HYPERRENDER			( 0x00200000)	///< ���� ������ �޽����� ���Ƽ� �Ѹ��� ��
#define RS_IN_2DHYPERRENDER			( 0x00400000)	///< 2D �׸��⿡�� ���Ƽ� �Ѹ��� ��
#define RS_IN_FLUSHINGDRAWCALL		( 0x00800000)	///< FlushReservedDrawCall ��
#define RS_EFFECT_RENDERED			( 0x01000000)	///< Effect �� render �Ǿ����� FlushReservedDrawCall ���� render �� �ʿ� ����
#define RS_IN_CHANGESCREENFORMAT	( 0x02000000)	///< ��ũ������ ��ȯ��
#define RS_ALPHALIST_RENDERED		( 0x04000000)	///< ���� alpha list ������ ����


/// @name Clear Render(er) Flags

#define CR_COLOR		( 0x00000001)	///< Į�� ���۸� �����
#define CR_DEPTH		( 0x00000002)	///< ����(Z) ���۸� �����
#define CR_STENCIL		( 0x00000004)	///< ���ٽ� ���۸� �����
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
	BOOL m_bPause;	///< ���� ���� - �׸��� �ʴ´�.
	RECT m_rtWindowClient;
	DWORD m_dwCreateOption;	///< ���� �ɼ�
public:
	CClock x_Clock;

	CEffectManager *x_pEffectManager;	/// Effect ���� ó��
	CWorldBase *x_pWorld;	///< World

	// ---------------------------------------------------------------------------------------------
	/// @name --- �⺻ ����/�ʱ�ȭ
public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();
	virtual void CreateManagers( char *lpszShaderPath = NULL);
	virtual void DestroyManagers();

	virtual BOOL Initialize3DEnvironment();
	virtual BOOL ToggleFullScreen( HWND hWndChange = 0) = 0;	// ToggleFullScreen �� �����츦 �ٲٰ��� �Ҷ� hWndChange �� ���� �ִ´�.
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
	/// @name --- Window ���� (API ����)
protected:
	long m_lWindowStyle;
	//UINT m_uiSystemScreenWidth, m_uiSystemScreenHeight;
	virtual void PrepareWindowModeChange(bool full_mode, bool max_screen, bool use_init_style);
	virtual void FinishWindowModeChange(bool full_mode, bool max_screen);


	// ---------------------------------------------------------------------------------------------
	/// @name --- �Ŵ���
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
	virtual BOOL IsReseted() const { return m_bReseted; } // BeginRender�Ŀ� üũ�Ѵ�.
	virtual BOOL IsRenderReady() const = 0;
	virtual void GetFpsValues( DWORD *pdwTicks, long *plCounts)	{ *pdwTicks = m_dwFpsLatestTicks; *plCounts = m_lFpsLatestCount; }
	virtual void GetPpsValues( DWORD *pdwTicks, long *plCounts)	{ *pdwTicks = m_dwFpsLatestTicks; *plCounts = m_lPpsLatest; }
	virtual void GetDcpsValues( DWORD *pdwTicks, long *plCounts)	{ *pdwTicks = m_dwFpsLatestTicks; *plCounts = m_DcpsLatest; }
	virtual void SetPaused( BOOL bPause)	{ m_bPause = bPause; }
	virtual void InitAllRenderStates() = 0;	// Render ���õ� ��� ������ �ʱ�ȭ �Ѵ�.
	
	// RenderUnit()�� �����ؼ� Render***()���� ȣ��� ������ ������ �Ѵ�.
	// - ������ FlushReservedDrawCall() ��� ����Ѵ�.
	// - bRecallEffect: TRUE�� ���, ���� Effect ������ �ѹ��̻� ȣ�Ⱑ���ϵ��� ����.
public:
	virtual void FlushDrawCall( BOOL bRecallEffect = FALSE);
	// ���� protected�� ���� ����.
    virtual void FlushReservedDrawCall( BOOL bLastFlush = FALSE);

protected:
    virtual void FlushReservedDrawCallEx( BOOL bLastFlush ) {}

public:

	// ���� ȭ��(buffer)�� �����.
	// CR_COLOR: Į�� ���۸� (*pClearColor)��[ARGB]���� �����
	// CR_DEPTH: ���� ���۸� (*pClearZ)��[0~1]���� �����.
	// CR_STENCIL: ���ٽ� ���۸� (*pClearStencil)��[2^n-1]���� �����
	// �� �������� ���ڰ����� NULL�� ���, Render ���ΰ��� ����Ѵ�.
	virtual BOOL ClearRender( DWORD dwFlags=CR_DEFAULT, WzColor* pClearColor=NULL, float* pClearZ=NULL, 
		DWORD* pClearStencil=NULL, const RECT* pRect=NULL) = 0;

	// ---------------------------------------------------------------------------------------------
	/// @name --- Camera, Projection �� Matrix ����
protected:
    St_FogParameter m_FogParameter;

	St_Camera m_Camera;
	BOOL m_bCustomViewport;	///< ����Ʈ �����ߴ°�
	RECT m_rtViewport;	///< ����Ʈ ������ ���õ�

	BOOL m_bIn2DMode;
	BOOL m_b2DModeInRealSize;	///< 2D ���� ��ǥ
	int m_fStd2DWidth;        ///< 2D ����� ũ��
    int m_fStd2DHeight;       ///< 2D ����� ũ��

    BOOL m_b2DModeAutoScale;    ///< 2D ��� �ڵ� �����ϸ� ��뿩��
    DWORD m_2DModeAutoScaleWidth;  ///< 2D ��� �ڵ� ������ ���� ���� ũ��


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

	virtual void SetViewport( RECT *prtViewport);	///< NULL �� ��� ������
	virtual BOOL PushViewport() = 0;
	virtual BOOL PopViewport() = 0;

	virtual WzMatrix* GetClipMatrixOfShadowMap( WzMatrix *pwmOutClip, WzMatrix* pwmLight) = 0;
	virtual void GetViewMatrix( WzMatrix* pwmOut) = 0;
	virtual void GetWorldMatrix( WzMatrix* pwmOut) = 0;
	virtual void GetProjectMatrix( WzMatrix* pwmOut) = 0;

    const RECT& GetViewport() const { return m_rtViewport; }


//-----------------------------------------------------------
// Light ����(x_pManagerLight)
//-----------------------------------------------------------

	// ����Ʈ ���� �� ���� ����
	virtual BOOL SetDirectionalLight( int iIndex, WzColor wcColor, const WzVector& wvDirection, ENUM_LIGHT_IRRA_TYPE eliType =0, float fRange = DEFAULT_LIGHT_RANGE);
	virtual BOOL SetPointLight( int iIndex, WzColor wcColor, const WzVector& wvPosition, ENUM_LIGHT_IRRA_TYPE eliType =0, float fRange = DEFAULT_LIGHT_RANGE);
	virtual const St_LightInformation* GetLightInfo( int iIndex) const;

	virtual	BOOL	SetLightIntensity( int iIndex, float fIntensity );
	virtual float	GetLightIntensity( int iIndex );

	// Active ����Ʈ ���� �� ���� ����
	virtual BOOL TurnOnLight( int iIndex, BOOL bOn);
	virtual int GetIndexOfActiveLight( int iActive) const;
	virtual const St_LightInformation* GetActiveLightInfo( int iActive) const;
	virtual int GetNumberOfActiveLight() const;

	// Irradiance ���� ��� ���� - �ӽ�
	virtual void SetIrradianceFilePath( LPSTR lpszPath );

	// ���� ����Ʈ ���� ���� : default FALSE
	virtual void EnableAreaLight( BOOL bEnable);
	virtual BOOL IsAreaLight() const;

	// ---------------------------------------------------------------------------------------------
	/// @name --- �׸��� ���
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
	virtual BOOL EnableFog( BOOL bEnable)	{ if ( bEnable)	{ SetRenderState( RS_FOG_ENABLED); } else { UnsetRenderState( RS_FOG_ENABLED); } return TRUE; }	// ��ü�� ���� Fog �Ѱ� ����
	virtual BOOL IsFogEnabled()	{ return ( CheckRenderState( RS_FOG_ENABLED)); }
	virtual BOOL TurnOnFog( BOOL bOn)	{ if ( bOn)	{ SetRenderState( RS_FOG_TURNED_ON); } else { UnsetRenderState( RS_FOG_TURNED_ON); } return TRUE; }	// ���� render state �� �ٲٴ� ó��
	/// @name Texture ó��
	virtual BOOL EnableTexture( BOOL bEnable)	{ if ( !bEnable) { SetRenderState( RS_TEXTURE_DISABLED); } else { UnsetRenderState( RS_TEXTURE_DISABLED); } return TRUE; }
	virtual BOOL IsTextureEnabled()	{ return ( !CheckRenderState( RS_TEXTURE_DISABLED)); }
	/// @name Stencil ����
	virtual BOOL EnableStencil( BOOL bEnable)	{ if ( bEnable) { SetRenderState( RS_STENCIL_ENABLED); } else { UnsetRenderState( RS_STENCIL_ENABLED); } return TRUE; }
	virtual BOOL SetStencilFunc( ENUM_CMP_FUNC eStencilFunc, DWORD dwRef, DWORD dwMask) = 0;
	virtual BOOL SetStencilOp( ENUM_STENCIL_OP eStencilOpFail, ENUM_STENCIL_OP eStencilOpZFail, ENUM_STENCIL_OP eStencilOpPass) = 0;
	/// @name Depth Bias ����( Offset = m * SlopeScaleDepthBias + DepthBias)
	virtual void SetDepthBias( float* pfDepthBias, float* pfSlopeScaleDepthBias = NULL, BOOL bForce = FALSE);

	// ---------------------------------------------------------------------------------------------
	/// @name --- ���� ����
protected:
	int m_iBackBufferCount;

	/// @name ���� �����ϱ�
protected:
	St_ScreenFormat m_CurrentScreenFormat;	///< ���� ȭ�� ����
	St_RenderOption m_CurrentRenderOption;	///< ����ڰ� ������ �ɼ� - �� ���� CWzdBase �� static ����� ����ȴ�.
	St_RenderCaps m_RenderCaps;		///< ���� �׷���ī���� ó�� ���� ����
	St_RenderState m_RenderState;	///< Render State ����
	St_StateParams m_StateParams;		///< ���� �Ķ���͵�
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
	/// @name --- Wzd �޽� �׸���


protected:

	/// @name WzUnit ����
public:
	virtual ENUM_ANI_FRAME_STATUS ProgressUnitFrame( CWzUnitDraw *pUnitDraw, float fFrameAdd, WzEvent *pEvent = NULL, float *pfProgressRate = NULL) = 0;
	virtual void AnimateUnit( CWzUnitDraw *pUnitDraw, int iLodLevel = 0, const WzMatrix *pwmParent = NULL, BOOL bUpdateBVExtra = FALSE) = 0;
	virtual BOOL RenderUnit( CWzUnitDraw *pUnitDraw, St_RenderCommand *prc = NULL, int iLodLevel = 0) = 0;
    virtual BOOL RenderUnitForOutline(CWzUnitDraw* unit_draw) = 0;
	virtual BOOL RenderUnitBone( CWzUnitDraw *pUnitDraw, int iStart = 0, int iEnd = -1) = 0;
    virtual BOOL RenderGrass(CWzUnitDraw *unitdraw_ptr) = 0;
	virtual BOOL CheckUnitLoaded( CWzUnitDraw *pUnitDraw); // MultiThread ������ �ε尡 �Ǿ����� üũ(add 2006/01/24 - koma0)

	virtual void CheckEffect( CWzUnitDraw *pUnitDraw, BOOL bCreateNow = FALSE ) = 0;// ����Ʈ������ �ȵǾ����� ����
    virtual void MoveUnitEffect( CWzUnitDraw *pUnitDraw ) = 0;


	/// @name Wzd - HANDLE ����
public:
	virtual WORD GetNumFrameOfAnimation( HANDLE hWzdAnimation) = 0;

	/// @name Wzd - ���� ����
public:
	virtual void DestroyTextureForWzdPtr( CWzdBase *pWzd) = 0;

	/// @name �ﰢ�� �׸���
public:
	/*
	�߰��� �Ķ����:
		1. pwvCenterPos: NULL�� ���, ��Ʈ�� ���� ��ġ ������ ���ο��� ���.
		2. pAlterRender: NULL�� ���, RenderType�� AL_NORMAL.
	*/

	virtual BOOL RenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzVector *pwvNormal, WzUVVert *pwUVs, int iDataSize = sizeof ( WzVector), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderTrianglesIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzVector *pwvNormal, int *piNormalMap, WzUVVert *pwUVs, int *piUVMap, int iDataSize = sizeof ( WzVector), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderTrianglesColoredIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzColor *pwcColor, int *piColorMap, WzUVVert *pwUVs, int *piUVMap, int iVertDataSize = sizeof ( WzVector), int iColorDataSize = sizeof ( WzColor), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderTrianglesPacked( HANDLE hTexture, int iNumFace, int *piVertexMap, WzVector *pwvPosVert, int *piPosVertIndex, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNormal, int iVertexDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNormalDataSize = sizeof ( WzVector), WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL RenderVertexBuffer( HANDLE hTexture, HANDLE hVertexBuffer, int iNumFace, int *piIndex, WzUVVert *pwuvStream = NULL, int iVertexShader = 0, int iPixelShader = 0, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual void SetUVStream( WzUVVert *pwuvStream = NULL) = 0;
	virtual BOOL RenderSimpleMesh( WzSimpleMesh *pwsmSimpleMesh, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);

	// pwvVertex�� pwUVs�� NULL�� �� ����.
	// Strip���� �������� NRTO_STRIP �÷��׸� ����.
	// pwvCenterPos���� NULL�� ���, ���ΰ��(����ȭ�� �ݵ�� ���� �ִ´�) 
	// �ε�����(pi__Index)�� ��쿡 ���� NULL ��� ����.
	virtual BOOL New_RenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzUVVert *pwUVs, WzColor *pwcColor, int* piVertexIndex, int* piUVIndex, int* piColorIndex, int iVertexSize = sizeof( WzVector), int iUVSize = sizeof( WzUVVert), int iColorSize = sizeof( WzColor), DWORD dwFlags = NRTO_DEFAULT, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL New_RenderSprite( HANDLE hTexture, ENUM_ALPHABLEND eabMode, WzVector *pwvVertex, WzUVVert *pwUVs, WzColor *pwcColor, int* piVertexIndex, int* piUVIndex, int* piColorIndex, WzVector* pwvCenterPos=NULL, St_AlterRender* pAlterRender=NULL );


	///< ���� �ﰢ�� �׸���
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




	/// @name 2D �׸��� ( Dst ��ǥ���� 1024x768 �ػ󵵸� �������� ������ �ǰ�, Src ��ǥ���� �ؽ��� �̹��� ���� ��ǥ�� �������� ������ �ȴ�.
public:
	virtual BOOL SaveScreen( char *lpszFilePath ) = 0;	///< ȭ�� ���� �Լ�
    virtual void GetDisplayMode(D3DDISPLAYMODE* pDisplayMode) = 0; ///< ȭ������

    //------------------------------------------------------------------------------
    // Remake : �ܺο��� ������ ����� �ش� ���� ��ŭ 2d �ؽ��� ������
    // Parameter : HANDLE texture_handle : �ؽ��� �ڵ�
    // Parameter : int vertices_count : ���ؽ� ����
    // Parameter : BYTE * buffer : (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) * vertices_count ��ŭ �ʿ�
    //------------------------------------------------------------------------------
    virtual CElementRender* GetElementRender() = 0;
    virtual bool RenderTextureTriangle(HANDLE texture_handle, int vertices_count, BYTE* buffer) = 0;
	virtual BOOL RenderTexture( HANDLE hTexture, float fxDst, float fyDst, float fDstWidth, float fDstHeight, float fxSrc = 0.0f, float fySrc = 0.0f, float fSrcWidth = 0.0f, float fSrcHeight = 0.0f, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT, float fRot = 0.f) = 0;
	virtual BOOL RenderTexture( HANDLE hTexture, float *pfxDest, float *pfyDest, float *pfxSrc, float *pfySrc, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT) = 0;	// 4���� ������ �ð�������� �����Ϳ� �־��ָ� �� ( pfxSrc, pfySrc �迭�� ���� ���� -1.0f �� ������ Texture ��ü�� render ��)
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
	//! Ÿ�� �ؽ�ó�� Text�� �׸���
	// �ϳ��� Text�� �׸� ���, RenderText(...,Ÿ���ؽ�ó�ڵ�)
	// �������� Text�� �������� �׸� ���, BeginText(Ÿ���ؽ�ó�ڵ�)-RenderText(...,INVALID_HANDLE_VALUE)-EndText()
	// - prcRegion���� NULL�� ���, �ؽ�ó ��ü ũ�Ⱑ ����� �ȴ�.
	// - RECT ������ ���� �ؽ��� ũ�������� ����Ϸ��� �� �Լ��� ����ϱ����� Set2DMode(0,TRUE)�� ȣ���Ѵ�.
	// - RECT ������ ���� ������ CManagerText::GetTextRect()�� ���� �˼��ִ�.
	// - lpszWzIDFont�� NULL�� ���, ���� ������(StartToUseFont) ��Ʈ�� ����Ѵ�.
	virtual BOOL RenderText( char *lpszText, WzColor wcText, WzColor wcBack=WzColor_RGBA( 0, 0, 0, 0), 
		DWORD dwStyle=TP_HLEFT, RECT *prcRegion=NULL, char *lpszWzIDFont=NULL, HANDLE hTargetTexture=INVALID_HANDLE_VALUE) = 0;
	
	virtual BOOL BeginText( HANDLE hTargetTexture);
	virtual BOOL EndText();
protected:
	HANDLE m_hTextTexture;



public:
	RECT GetBackBufferSize();

	/// @name Silhouette/ShadowVolume �׸���
public:
	virtual BOOL RenderSilhouette( CWzSilhouette *pws, WzColor wcColor = WzColor_RGB( 0, 0, 0), float fLineWidth = 1.0f) = 0;
	virtual BOOL RenderShadowVolume( CWzShadowVolume *pws) = 0;
	virtual void SetShadowColor( WzColor wcShadow) = 0;
	virtual WzColor GetShadowColor() = 0;

	/// @name ��Ÿ �׸��� - �����Ƿ� Release �������� ���� �� ��
	//	�ش� �׸���� ��Ƽ� �׸��� �ʰ�, RealizeRender***()�� ȣ���.
public:
	virtual BOOL RenderBone( HANDLE hWzd, int iStart = 0, int iEnd = -1) = 0;
	virtual BOOL RenderDot( WzVector wvPos, WzColor wcColor, float fDotSize = 1.0f) = 0;
	virtual BOOL RenderLine( WzVector wvPos1, WzVector wvPos2, WzColor wcColor, float fLineWidth = 1.0f) = 0;
	virtual BOOL RenderPyramid( WzVector wvPos1, WzVector wvPos2, WzColor wcColor) = 0;
	virtual BOOL RenderBoundingVolume( const WzBoundingVolume *pbv) = 0;
	virtual BOOL RenderBvAABB( const BvAABB *paabb) = 0;
	virtual BOOL RenderBvSphere( const BvSphere *pbvs) = 0;


	/// @name ���� ���� ����Ʈ/��������Ʈ/������ �׸���
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
    // �޽� ������
    virtual BOOL New_RealizeDrawingPrimitiveVS( St_RenderCommand* prc, int iVS, int iPS = 0 ) { return TRUE; }

public:
	virtual void DestroyResourceForWzUnitDraw( CWzUnitDraw *pUnitDraw );


	// @name ���� ���� 
public:
	// Ǯ��ũ�� ��忡�� �����
	// ���� �� 256���� �ε����� ���� red[256], green[256], blue[256]
	// �� ���� 0~65535���� ������.
	virtual BOOL GetGammaRamp( WORD* pwOutRed, WORD* pwOutGreen, WORD* pwOutBlue ) const = 0;
	virtual BOOL SetGammaRamp( const WORD* pwColor) = 0;
	virtual BOOL SetGammaRamp( const WORD* pwRed, const WORD* pwGreen, const WORD* pwBlue) = 0;
	virtual BOOL SetGammaRamp( int iMinIndex, WORD wMinValue, int iMaxIndex, WORD wMaxValue) = 0;

public:
	/// @name ��밡����/���� �ؽ�ó �޸�
	virtual UINT GetAvailableTextureMem() const;
	virtual UINT GetUsedTextureMem() const;
	virtual UINT GetAvailableVidMem() const;
	virtual UINT GetUsedVidMem() const;
	virtual UINT GetLocalVidMem() const;

	/// @name water type ����
protected:
	ENUM_WATER_TYPE m_eWaterType;

public:
	virtual void SetWaterType( ENUM_WATER_TYPE eType){ m_eWaterType = eType; }
	virtual ENUM_WATER_TYPE GetWaterType(){ return m_eWaterType; }


	/// @name DetailTexture VS/PS constant ����
public:
	virtual void SetDetailTextureScale( float fScale );
	virtual void SetDetailFunc( int nIndex, float fFunc);

	/// @name TerrainGlowFactor PS constant ����
public:
	virtual void SetTerrainGlowFactor( float fFactor);
	virtual float CalculateGlowFactor( float fMin, float fMax, float fTime);

	virtual void SetTerrainHeightFactor( float fHeightFactor );

	/// @name IndexBuffer 32 ���� ����
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


/// @name --- �׸��� ��� inline

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


/// @name --- ���� ���� inline

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
/// @name �ﰢ�� �׸��� inline
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
    // ȭ�� ����
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

    // ȭ�� �ɼ�
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

    // ȭ�� ����
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


    // ȭ�� �ɼ�
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
    char lpszParamBuffer[1024];	// �Ķ���� Save/Load �� ���� ����

    // ȭ�� ����
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

    // ȭ�� �ɼ�
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
