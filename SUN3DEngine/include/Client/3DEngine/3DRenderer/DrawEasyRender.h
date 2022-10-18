// draweasyrender.h

#ifndef _3DRENDERER_DRAWEASYRENDER_H_
#define _3DRENDERER_DRAWEASYRENDER_H_

#include "drawbase.h"


typedef struct
{
	float m_fPos[4];
	WzColor m_wcVertexColor[4];
} St_Hyper2DModeObject;

typedef struct
{
	WzVector m_wvPos[2];
	WzColor m_wcVertexColor;
	float m_fLineWidth;
} St_Hyper2DModeLine;

#define MAX_VIEWPORT_STACK		( 16)

typedef struct
{
	BOOL m_bFullView;
	RECT m_rtView;
} St_ViewportObject;


///////////////////////////////////////////////////////////////////////////////////////
// --- CDrawEasyRender class


class CDrawEasyRender : public CDrawBase  
{
public:
	CDrawEasyRender();
	virtual ~CDrawEasyRender();
	virtual void Clear();

	// ---------------------------------------------------------------------------------------------
	/// @name --- 기본 생성/초기화
public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();
	virtual void CreateManagers( char *lpszShaderPath = NULL);
	virtual void DestroyManagers();

	virtual void EndRender();
	virtual void InitAllRenderStates();	// Render 관련된 모든 정보를 초기화 한다.

	// ---------------------------------------------------------------------------------------------
	/// @name --- Wzd 메쉬 그리기

	/// @name Wzd - HANDLE 관련
public:
	virtual WORD GetNumFrameOfAnimation( HANDLE hWzdAnimation);

	/// @name Wzd - 직접 접근
public:
	virtual void DestroyTextureForWzdPtr( CWzdBase *pWzd);

	/// @name 삼각형 그리기
public:
	virtual BOOL RealizeRenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzVector *pwvNormal, WzUVVert *pwUVs, int iDataSize = sizeof ( WzVector));
	virtual BOOL RealizeRenderSimpleMesh( WzSimpleMesh *pwsmSimpleMesh);

protected:
	BOOL AnalyzeTriangles( St_RendermeshGroupAnalyzer *prgaResult, HANDLE hTexture);

	// Viewport
public:
	virtual BOOL PushViewport();
	virtual BOOL PopViewport();
protected:
	int m_iNumViewportObject;
	St_ViewportObject m_ViewportObject[MAX_VIEWPORT_STACK];
};

#endif // _3DRENDERER_DRAWEASYRENDER_H_
