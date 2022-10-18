// drawrendermeshd3d.h

#ifndef _3DRENDERER_DRAWRENDERMESHD3D_H_
#define _3DRENDERER_DRAWRENDERMESHD3D_H_

#include "drawrendererd3d.h"


class CDrawRenderMeshD3d : public CDrawRendererD3d
{
public:
	CDrawRenderMeshD3d(void);
	virtual ~CDrawRenderMeshD3d(void);

	virtual void Clear();

public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();


	/// @name Wzd 메쉬 그리기
public:	
	virtual BOOL RealizeRenderTrianglesIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzVector *pwvNormal, int *piNormalMap, WzUVVert *pwUVs, int *piUVMap, int iDataSize = sizeof ( WzVector));
	virtual BOOL RealizeRenderTrianglesColoredIndexed( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, int *piVertexMap, WzColor *pwcColor, int *piColorMap, WzUVVert *pwUVs, int *piUVMap, int iVertDataSize = sizeof ( WzVector), int iColorDataSize = sizeof ( WzColor));
	virtual BOOL RealizeRenderTrianglesPacked( HANDLE hTexture, int iNumFace, int *piVertexMap, WzVector *pwvPosVert, int *piPosVertIndex, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNormal, int iVertexDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNormalDataSize = sizeof ( WzVector));
	virtual BOOL RealizeRenderTrianglesPackedMultiTexture( HANDLE hTexture, int iNumFace, int *piVertexMap, WzVector *pwvPosVert, int *piPosVertIndex, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNormal, int *piSecondUVMap, WzUVVert *pwSecondUVs, int iVertexDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNormalDataSize = sizeof ( WzVector), int iSecondUVSize = sizeof (WzUVVert));
	virtual BOOL RealizeRenderVertexBuffer( HANDLE hTexture, HANDLE hVertexBuffer, int iNumFace, int *piIndex, WzUVVert *pwuvStream = NULL, int iVertexShader = 0, int iPixelShader = 0, BOOL bZWriteModeChange = TRUE, HANDLE hAdditionalVB = INVALID_HANDLE_VALUE);
	virtual BOOL RealizeRenderIndexBuffer( HANDLE hTexture, HANDLE hVertexBuffer, HANDLE hIndexBuffer, int iNumFace, int iStartIndex = 0, WzUVVert *pwuvStream = NULL, int iVertexShader = 0, int iPixelShader = 0, BOOL bZWriteModeChange = TRUE, HANDLE hAdditionalVB = INVALID_HANDLE_VALUE);
	virtual void SetUVStream( WzUVVert *pwuvStream = NULL);		
	virtual BOOL RealizeTriRenderCommmand( St_TriRenderCommand *prc);
	virtual BOOL RealizeSpriteRenderCommmand( St_SpriteRenderCommand *prc);
    virtual BOOL RealizeRenderTrianglesColored( int iNumFace, WzVector *pwvVertex );

	/// @name 새로운 Process
protected:
	virtual int New_CheckVertexShaderInRealizing( St_RenderCommand *prc);	// Vertex Shader 사용 여부 확인
	virtual int New_CheckPixelShaderInRealizing( St_RenderCommand *prc);	// Pixel Shader 사용 여부 확인
	virtual BOOL New_RealizeDrawingPrimitiveVS( St_RenderCommand *prc, int iVS, int iPS = 0);	// VS 를 가지고 메쉬 render

//-----------------------------------------------------------
//	라이팅 for Global Illumination 
//-----------------------------------------------------------
protected:
	// 전체 라이팅을 위한 Unit 색상 설정
	virtual void SetAmbientColorOfShader( WzColor wcColor);
	virtual void SetDiffuseColorOfShader( WzColor wcColor);
	virtual void SetSpecularColorOfShader( WzColor wcColor, float fIntensity = 1.0f);


//-----------------------------------------------------------
//	Constants for Alternative Render 
//-----------------------------------------------------------
protected:
	virtual void SetRefractionRateOfShader( float fRefractionRate);
	virtual void SetRefractionAlphaOfShader( float fRefractionAlpha);
	virtual void SetInflationFactorOfShader( float fInflationFactor );
};



// inline

inline void CDrawRenderMeshD3d::SetAmbientColorOfShader( WzColor wcColor)
{
	x_pManagerVS->SetAmbientColor( wcColor);
	x_pManagerPS->SetAmbientColor( wcColor);
}

inline void CDrawRenderMeshD3d::SetDiffuseColorOfShader( WzColor wcColor)
{
	x_pManagerVS->SetDiffuseColor( wcColor);
}

inline void CDrawRenderMeshD3d::SetSpecularColorOfShader( WzColor wcColor, float fIntensity)
{
	x_pManagerVS->SetSpecularColor( wcColor, fIntensity);
	x_pManagerPS->SetSpecularColor( wcColor, fIntensity);
}

inline void CDrawRenderMeshD3d::SetRefractionRateOfShader( float fRefractionRate)
{
	fRefractionRate *= 0.03f;
	x_pManagerVS->SetRefractionRate( fRefractionRate);
	x_pManagerPS->SetRefractionRate( fRefractionRate);
}

inline void CDrawRenderMeshD3d::SetRefractionAlphaOfShader( float fRefractionAlpha)
{
	x_pManagerPS->SetRefractionAlpha( fRefractionAlpha);
}

inline void CDrawRenderMeshD3d::SetInflationFactorOfShader( float fInflationFactor )
{
	x_pManagerVS->SetInflationFector( fInflationFactor);
}

#endif // _3DRENDERER_DRAWRENDERMESHD3D_H_