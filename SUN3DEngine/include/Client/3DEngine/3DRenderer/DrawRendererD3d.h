// drawrendererd3d.h

#ifndef _3DRENDERER_DRAWRENDERERD3D_H_
#define _3DRENDERER_DRAWRENDERERD3D_H_

#include "drawsetupd3d.h"
#include <map>

class CDrawRendererD3d : public CDrawSetupD3d  
{
public:
	CDrawRendererD3d();
	virtual ~CDrawRendererD3d();

	virtual void Clear();

private:

    class EffectSpriteElementData
    {
    private:
        HANDLE m_hTexture;
        St_RenderState m_RenderState;
        CWzArray<SpriteVerts> m_SpriteVertexList;

    public:
        EffectSpriteElementData()
        {
            m_hTexture = NULL;
        }

        const St_RenderState& GetRenderState() const { return m_RenderState; }
        St_RenderState& GetRenderState() { return m_RenderState; }
        void SetRenderState( const St_RenderState& value) { m_RenderState = value; }

        HANDLE& GetTexture() { return m_hTexture; }
        const HANDLE& GetTexture() const { return m_hTexture; }
        void SetTexture(const HANDLE& value) { m_hTexture = value; }

        int GetVertextListCount() { return m_SpriteVertexList.GetCount(); }
        SpriteVerts* GetVertexList() { return m_SpriteVertexList.GetBufferPtr(); }

        void AddPrimitiveData( SpriteVerts* pSpriteVertex, int VertexCount )
        {
            m_SpriteVertexList.Adds( VertexCount, pSpriteVertex );
        }
    };

    class EffectSpriteElement
    {
    private:
        std::vector< EffectSpriteElementData* > m_SpritePrimitiveDataArray;

        EffectSpriteElementData* m_pCurData;

    public:
        EffectSpriteElement() 
        {
            m_pCurData = NULL;
        }
        ~EffectSpriteElement() 
        {
            Clear();
        }

        std::vector< EffectSpriteElementData* >& GetSpritePrimitiveDataArray() { return m_SpritePrimitiveDataArray; }
        const std::vector< EffectSpriteElementData* >& GetSpritePrimitiveDataArray() const { return m_SpritePrimitiveDataArray; }

        void AddPrimitiveData( HANDLE hTextureHandle, St_RenderState* pRenderState, SpriteVerts* pSpriteVertex, int VertexCount )
        {
            // 현재(이전) 위치의 데이터가 존재하고, 텍스쳐와 블랜딩 설정이 같으면 이어붙인다.
            if( m_pCurData != NULL )
            {
                if( m_pCurData->GetTexture() == hTextureHandle &&
                    m_pCurData->GetRenderState().m_abMode == pRenderState->m_abMode )
                {
                    m_pCurData->AddPrimitiveData( pSpriteVertex, VertexCount );
                    return;
                }
            }

            // 그렇지 않으면 그냥 새롭게 추가
            EffectSpriteElementData* pData = new EffectSpriteElementData();
            pData->SetTexture( hTextureHandle );
            pData->SetRenderState( *pRenderState );
            pData->AddPrimitiveData( pSpriteVertex, VertexCount );
            m_SpritePrimitiveDataArray.push_back( pData );

            m_pCurData = pData;
        }

        EffectSpriteElementData* FindPrimitiveData( HANDLE hTextureHandle, St_RenderState* pRenderState )
        {
            for( size_t i=0;i!=m_SpritePrimitiveDataArray.size();++i)
            {
                if( m_SpritePrimitiveDataArray[i]->GetTexture() != hTextureHandle )
                    continue;

                if( m_SpritePrimitiveDataArray[i]->GetRenderState().m_abMode != pRenderState->m_abMode )
                    continue;

                return m_SpritePrimitiveDataArray[i];
            }

            return NULL;
        }

        void Clear()
        {
            for( size_t i=0;i!=m_SpritePrimitiveDataArray.size();++i)
            {
                delete m_SpritePrimitiveDataArray[i];
            }
            m_SpritePrimitiveDataArray.clear();

            m_pCurData = NULL;
        }
    };


    struct SpriteElement
	{
		std::map< HANDLE, CWzArray<SpriteVerts> > mapSpriteList;
		std::map< HANDLE, St_RenderState > mapRenderState;

		
		SpriteElement() {}
		~SpriteElement() { clear(); }

		void clear()
		{
			mapSpriteList.clear();
			mapRenderState.clear();
		}
	};

    EffectSpriteElement m_EMSortedSpriteElement;
    EffectSpriteElement m_EMUnSortedSpriteElement;

	SpriteElement					m_RSSortedSpriteElement;		// Sort된 RenderSprite Container
	SpriteElement					m_RSUnSortedSpriteElement;		// Sort되지 않는 RenderSprite Container
public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();

	virtual BOOL Initialize3DEnvironment();
	virtual BOOL ToggleFullScreen( HWND hWndChange = 0);
	virtual BOOL ProcessWindowSizeChange();

	// ---------------------------------------------------------------------------------------------
	/// @name --- Window 세팅 (API 관련)
protected:
	virtual void FinishWindowModeChange(bool full_mode, bool max_screen);

protected:
	virtual void PrepareDeviceSetting( BOOL bFullScreen);
	virtual void FinishDeviceSetting( BOOL bFullScreen);

public:
	virtual BOOL BeginRender( DWORD dwClearFlags=CR_DEFAULT);
	virtual void EndRender();
	virtual BOOL ClearRender( DWORD dwFlags=CR_DEFAULT, WzColor* pClearColor=NULL, float* pClearZ=NULL, 
		DWORD* pClearStencil=NULL, const RECT* pRect=NULL);

	virtual BOOL RenderSprite( HANDLE hTexture, const WzVector& wvPos, ENUM_ALPHABLEND eabMode, float fXSize, float fYSize, float fRotate = 0.0f, float fDepthOffset = 0.0f, WzUVVert *pwuvRegion = NULL, ENUM_BILLBOARD_TYPE eType = EBBT_SPRITE, WzVector* pResultPos = NULL, St_AlterRender* pAlterRender=NULL);
	virtual BOOL New_RenderSprite(HANDLE hTexture, ENUM_ALPHABLEND eabMode, WzVector *pwvVertex, WzUVVert *pwUVs, WzColor *pwcColor, 
								 int* piVertexIndex, int* piUVIndex, int* piColorIndex, WzVector* pwvCenterPos, 
								 St_AlterRender* pAlterRender);

	/// @name Camera, Projection 등 Matrix 관련
protected:
	D3DXMATRIXA16 m_d3dxmWorld, m_d3dxmCamera, m_d3dxmProjection;
	D3DXMATRIXA16 m_d3dxmOrtho;	///< 2D 모드용
	D3DXMATRIXA16 m_d3dxmDeviceClip;
	D3DXMATRIXA16* m_pDeviceProjection;

	BOOL m_bNeedUpdateClipMatrix;

    CElementRender* element_render_;

protected:
	virtual void SetCameraMatrix( WzMatrix *pwmCamera);
	virtual void SetProjectionMatrix( WzMatrix *pwmProjection);
public:
	virtual void SetWorldMatrix( WzMatrix *pwmWorld);
	virtual void SetCameraMatrix( WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvUp);
	virtual void SetProjectionMatrix( float fFov, float fNear, float fFar, ENUM_SCREEN_RATIO eScreenRatio = SR_AUTO);
	virtual void SetOrthogonalMatrix( float fLeft, float fRight, float fBottom, float fTop, float fNear, float fFar);
	virtual void Set2DMode( BOOL b2D = TRUE, BOOL bInRealSize = FALSE);
	virtual void FlushDrawCall(BOOL bRecallEffect = FALSE );
	virtual BOOL SetLinearFog( WzColor wcFogColor, float fFogStart, float fFogEnd, float fMaxFogRate);

	virtual WzMatrix* GetClipMatrix( WzMatrix *pwmClip);
	virtual POINT* GetScreenPos( POINT *ptScreen, WzVector *pwvLocal, float *pfDepth = NULL);

	virtual void SetViewport( RECT *prtViewport);	///< NULL 인 경우 해제됨

	virtual WzMatrix* GetClipMatrixOfShadowMap( WzMatrix *pwmOutClip, WzMatrix* pwmLight);
	virtual void GetViewMatrix( WzMatrix* pwmOut);
	virtual void GetWorldMatrix( WzMatrix* pwmOut);
	virtual void GetProjectMatrix( WzMatrix* pwmOut);

	virtual D3DXMATRIX* GetDeviceClipMatrix();

protected:
	void ApplyViewport( BOOL bProjection = TRUE);

	/// @name Wzd 메쉬 그리기
public:
	virtual void SetMeshDrawMode( ENUM_MESH_DRAW eMeshDrawMode, BOOL bForce = FALSE);
	virtual void SetCullFaceMode( ENUM_CULL_FACE eCullFaceMode, BOOL bForce = FALSE);
	virtual void SetZFunc( ENUM_CMP_FUNC eZFunc, BOOL bForce = FALSE);
	virtual void SetColorMask( DWORD dwColorMask);
	virtual BOOL TurnOnFog( BOOL bOn);
	virtual BOOL EnableTexture( BOOL bEnable);
	virtual BOOL EnableStencil( BOOL bEnable);
	virtual BOOL SetStencilFunc( ENUM_CMP_FUNC eStencilFunc, DWORD dwRef, DWORD dwMask);
	virtual BOOL SetStencilOp( ENUM_STENCIL_OP eStencilOpFail, ENUM_STENCIL_OP eStencilOpZFail, ENUM_STENCIL_OP eStencilOpPass);

	/// @name 2D 그리기
public:
	virtual BOOL SaveScreen( char *lpszFilePath );	///< 화면 저장 함수
    virtual void GetDisplayMode(D3DDISPLAYMODE* pDisplayMode); ///< 화면모드얻기

    virtual CElementRender* GetElementRender()  {   return element_render_; }
    virtual bool RenderTextureTriangle(HANDLE texture_handle, int vertices_count, BYTE* buffer);
	virtual BOOL RenderTexture( HANDLE hTexture, float fxDst, float fyDst, float fDstWidth, float fDstHeight, float fxSrc = 0.0f, float fySrc = 0.0f, float fSrcWidth = 0.0f, float fSrcHeight = 0.0f, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT, float fRot = 0.f);
	virtual BOOL RenderTexture( HANDLE hTexture, float *pfxDest, float *pfyDest, float *pfxSrc, float *pfySrc, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT);
	virtual void Render2DLine( float fx1, float fy1, float fx2, float fy2, WzColor wcColor);
	virtual BOOL RenderTexture4SpinGage(  HANDLE hTexture, float fxDst, float fyDst, float fDstWidth, float fDstHeight, float fxSrc = 0.0f, float fySrc = 0.0f, float fSrcWidth = 0.0f, float fSrcHeight = 0.0f, float fRate = 100.f, WzColor *pwcColor = NULL, float fZ = 0.0f);

	virtual BOOL RenderTextureBlending( HANDLE hTexture1, HANDLE hTexture2, float fxDst, float fyDst, 
		float fDstWidth, float fDstHeight, float fxSrc1 = 0.0f, float fySrc1 = 0.0f, float fSrcWidth1 = 0.0f, 
		float fSrcHeight1 = 0.0f, float fxSrc2 = 0.0f, float fySrc2 = 0.0f, float fSrcWidth2 = 0.0f, 
		float fSrcHeight2 = 0.0f, WzColor *pwcColor = NULL, float fZ = 0.0f, DWORD dwOption = RTO_DEFAULT);

	virtual void EndHyper2DMode();

	// DrawBase::RenderText() 참조
	virtual BOOL RenderText( char *lpszText, WzColor wcText, WzColor wcBack=WzColor_RGBA( 0, 0, 0, 0),
		DWORD dwStyle=TP_HLEFT, RECT *prcRegion=NULL, char *lpszWzIDFont=NULL, HANDLE hTargetTexture=INVALID_HANDLE_VALUE);
	virtual BOOL BeginText( HANDLE hTargetTexture);
	virtual BOOL EndText();

	/// @name 그리기 처리
protected:
	void* StartToRender( int iNumVertex, WzColor wcColor, DWORD dwFVF, LPDIRECT3DVERTEXBUFFER9 *ppVB);
	BOOL EndToRender( int iNumPolygon, DWORD dwFVF, LPDIRECT3DVERTEXBUFFER9 pVB);
	BOOL EndToRender_Alternative( BOOL bLine, int iNumPrimitive, DWORD dwFVF, LPDIRECT3DVERTEXBUFFER9 pVB);

	/// @name Silhouette/ShadowVolume 그리기
public:
	virtual BOOL RenderSilhouette( CWzSilhouette *pws, WzColor wcColor = WzColor_RGB( 0, 0, 0), float fLineWidth = 1.0f);
	virtual BOOL RenderShadowVolumeMesh( CWzShadowVolume *pws);

	/// @name 기타 그리기 - 느리므로 Release 버전에는 쓰지 말 것
public:
	virtual BOOL RenderDot( WzVector wvPos, WzColor wcColor, float fDotSize = 1.0f);
	virtual BOOL RenderLine( WzVector wvPos1, WzVector wvPos2, WzColor wcColor, float fLineWidth = 1.0f);
	virtual BOOL RenderPyramid( WzVector wvPos1, WzVector wvPos2, WzColor wcColor);
	virtual BOOL RealizeRenderSprite( HANDLE hTexture, const WzVector& wvPos, float fXSize, float fYSize, 
		float fRotate = 0.0f, float fDepthOffset = 0.0f, WzUVVert *pwuvRegion = NULL, 
		ENUM_BILLBOARD_TYPE eType = EBBT_SPRITE, WzVector* pResultPos = NULL, St_RenderState* pRS = NULL);
	virtual BOOL RealizeNewRenderSprite( HANDLE hTexture, const SpriteVerts* wvVerts, St_RenderState* pRS=NULL);
	virtual void FlushRenderSprite(bool bAllRenderSprite);
	virtual void NewFlushRenderSprite(bool bSortedRenderSprite, bool bUnSortedRenderSprite);

protected:
	void* FillRectangleVertices( D3DVECTOR *pd3vTarget, WzVector wvVertices[4], WzColor wcColor);

public:
	virtual BOOL New_RealizeRenderTriangles( HANDLE hTexture, int iNumFace, WzVector *pwvVertex, WzUVVert *pwUVs, 
		WzColor *pwcColor,  int* piVertexIndex, int* piUVIndex,	int* piColorIndex, int iVertexSize = sizeof( WzVector), 
		int iUVSize = sizeof( WzUVVert), int iColorSize = sizeof( WzColor), DWORD dwFlags = NRTO_DEFAULT);


	/// @name 설정 변경
public:
	virtual void SetColor( WzColor wcDraw, ENUM_WRIC ewColorType = WRIC_DIFFUSE);
	virtual void SetAlphaBlendMode( ENUM_ALPHABLEND abMode, BOOL bZWriteModeChange = TRUE);
	virtual void SetAlphaTestMode( DWORD dwAlphaTestReference, ENUM_CMP_FUNC eAlphaTestFunc = CF_GREATEREQUAL, BOOL bForce = FALSE);
	virtual void EnableAlphaTest( BOOL bEnable, BOOL bForce=FALSE);
	virtual void SetZBufferWrite( BOOL bZWriteEnable);
	virtual void SetZBufferTest( BOOL bZTestEnable);
	virtual BOOL StartHyperRenderMode( int iNumVertices);
	virtual BOOL EndHyperRenderMode();
	virtual void EnableSpecular( BOOL bEnable, BOOL bForce=FALSE);
	virtual void SetDepthBias( float* pfDepthBias, float* pfSlopeScaleDepthBias = NULL, BOOL bForce = FALSE);


private:
    void _AddSpriteElement( EffectSpriteElement&, HANDLE, St_RenderState*, SpriteVerts*, int );
    void _RenderSpriteElement( EffectSpriteElement& );
    void _RenderSpriteElement2( EffectSpriteElement& );
};



/// @name 설정 변경 inline

inline void CDrawRendererD3d::SetColor( WzColor wcDraw, ENUM_WRIC ewColorType)
{
	m_RenderState.m_wcDraw[ewColorType] = wcDraw;
//	m_lpD3dDevice->SetRenderState( D3DRS_AMBIENT, WzColorToD3DCOLOR( m_RenderState.m_wcDraw));
}

inline void CDrawRendererD3d::GetDisplayMode(D3DDISPLAYMODE* pDisplayMode)
{
    if (m_lpD3dDevice)
    {
        m_lpD3dDevice->GetDisplayMode(0, pDisplayMode);
    }
}

#endif // _3DRENDERER_DRAWRENDERERD3D_H_
