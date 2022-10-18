// managershadowmap.h

#ifndef _3DRENDERER_MANAGERSHADOWMAP_H_
#define _3DRENDERER_MANAGERSHADOWMAP_H_

#include "managerbase.h"
#include "managerspeedtree.h"
#include <vector>

struct St_RenderCommand;

// from nvidia sdk 
#define FLT_AS_DW(F) (*(DWORD*)&(F))
#define ALMOST_ZERO(F) ((FLT_AS_DW(F) & 0x7f800000L)==0)
#define IS_SPECIAL(F)  ((FLT_AS_DW(F) & 0x7f800000L)==0x7f800000L)

#define UNIT_SUB_BB	1.0f
#define MAX_BV		500

enum ENUM_SHADOWMAP_MODE
{
	ESM_NONE = 0,			// ShadowMap 미지원
	ESM_DEPTHSTENCIL,		// NVidia 용 ( 하드웨어 지원 )
	ESM_R32F,				// ATI 용 ( 쉐이더를 쓰지만 소프트웨어 ShadowMap )
	ESM_PROTECTED_TEXTURE,	// 그림자 맵이 지원 안되는 그래픽카드용. 걍 지형에 맺히는 그림자.
	ESM_DEPTHSTENCIL_NO_SELFSHADOW,	// 자기그림자 없이 깊이버퍼 쓰는 그림자
	ESM_R32F_NO_SELFSHADOW,
	NUM_SHADOWTEXTURE,
};

// WzFrustum에 Frustum 의 각 꼭지점 8개 추가
struct WzFrustumShadow : public WzFrustum
{
	WzVector m_wvPoints[8];
	int		m_nVertexLUT[6];
};

#define NUM_SHADOW_BLUR	9

class CManagerShadowMap : public CManagerBase
{
public:
	CManagerShadowMap();
	virtual ~CManagerShadowMap();
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();
	virtual void Apply();

	virtual ENUM_SHADOWMAP_MODE ConfirmDevice() = 0;

    virtual void ResetShadowMode();

protected:
	virtual BOOL CreateDeviceObjects();
	virtual void DestroyDeviceObjects();
	
	// ManagerRenderCommand에 등록된 unit들 중에서 Caster와 Receiver를 추려냄
	virtual BOOL ChooseShadowCastersAndReceivers( St_RenderCommand** ppRenderCommand, int iNumRCShadow );
	
	virtual void SetTexBiasMatrix( DWORD dwTexWidth, DWORD dwTexHeight ) = 0;
	
protected:
	// 변환행렬 계산
	virtual void CalculateTransformMatrix() = 0;
	virtual void CalculateProjectionMatrix() = 0;
	// 투영행렬 또는 뷰*투영행렬로 부터 프러스텀 계산
	virtual void GenerateFrustumFromMatrix( WzFrustumShadow* pResult, WzMatrix* pwmInput ) = 0;
	

protected:
	ENUM_SHADOWMAP_MODE				m_eShadowMode;					// 하드웨어 지원 모드
	HANDLE							m_hShadowMap;					// 그림자 맵 텍스쳐
	HANDLE							m_hDepth;						// 그림자 맵 그릴때 깊이 버퍼
	DWORD							m_dwTexWidth, m_dwTexHeight;	// 그림자 맵 크기
	WzMatrix						m_wmShadowTransform;			// 변환 행렬( 그림자 맵에 그릴 때 )
	WzMatrix						m_wmProjForShadow;				// 그림자용 투영 행렬

	WzMatrix						m_wmShadowTransformBias;		// 변환 행렬( 그림자 맵을 Unit에 그릴 때 )
	WzMatrix						m_wmShadowTransformBiasBlur[4];	// 블러 변환 행렬( 그림자 맵을 Unit에 그릴 때 )
	WzMatrix						m_wmTexBias;					// 블러 안할 때( ATI )
	WzMatrix						m_wmTexBiasBlur[4];				// 그림자맵의 텍스쳐 좌표를 계산할 때 쓰는 행렬 사각형 필터

	float							m_fShadowRange;					// 그림자가 적용 될 거리
	float							m_fRangeChangingColor;			// 그림자 색이 연해지는 거리
	float							m_fShadowFOV;					// 그림자 맵용 FOV
	float							m_fZNear;//, m_fZFar;				// m_wmProjForShadow용 ZMin, ZFar
	float							m_fTSMDelta;					// Delta 값
	float							m_fDepthBias;					// DepthBias 값
	CWzArray< St_RenderCommand* >	m_arShadowCasterUnits;			// Shadow Casters
	CWzArray< St_RenderCommand* >	m_arShadowReceiverUnits;		// Shadow Receivers
	int								m_iNumRCShadow;					// 
	St_RenderCommand**				m_ppRenderCommand;				//
	BOOL							m_bCastersAndReceivers;
	
	CWzArray< CSpeedTreeDrawInfo* > m_arShadowCasterUnitsST;		// Shadow Casters For SpeedTree
	CWzArray< CSpeedTreeDrawInfo* > m_arShadowReceiverUnitsST;		// Shadow Receivers For SpeedTree
	int								m_iNumSTShadow;					
	BOOL							m_bActiveSpeedTree;				// ShadowMap에 스피드 트리 적용여부
	std::vector< BvAABB >			m_vecShadowReceiversAABB;

	float							m_fPixelOffset[NUM_SHADOW_BLUR*4];
	
	// Get, Set
public:
	CWzArray< St_RenderCommand* >* GetShadowCasters(){ return &m_arShadowCasterUnits; }
	

	CWzArray< CSpeedTreeDrawInfo* >* GetShadowCastersForSpeedTree(){ return &m_arShadowCasterUnitsST; }
	BOOL GetActiveSpeedTree(){ return m_bActiveSpeedTree; }
	void SetActiveSpeedTree( BOOL bActive ){ m_bActiveSpeedTree = bActive; }
	WzMatrix* GetShadowTransform(){ return &m_wmShadowTransform; }
	WzMatrix* GetShadowTransformBias(){ return &m_wmShadowTransformBias; }
	WzMatrix* GetShadowTransformBiasBlur( int nIndex ){ return &m_wmShadowTransformBiasBlur[nIndex]; }
	
	virtual HANDLE GetRenderTarget(){ return m_hShadowMap; }
	virtual HANDLE GetDepthStencil(){ return m_hDepth; }
	virtual float GetDepthBias(){ return m_fDepthBias; }
	virtual void SetDepthBias( float fDepthBias ){ m_fDepthBias = fDepthBias; }
	virtual ENUM_SHADOWMAP_MODE GetShadowMode(){ return m_eShadowMode; }
	virtual HANDLE GetShadowTexture();
	virtual void ChangeTextureSize( DWORD dwWidth, DWORD dwHeight );
	virtual BOOL IsCastersAndReceivers() const { return m_bCastersAndReceivers; }
	virtual float GetShadowRange(){ return m_fShadowRange; }
	virtual void SetShadowRange( float fRange ){ m_fShadowRange = fRange; }
	virtual float GetRangeChangingColor(){ return m_fRangeChangingColor; }
	virtual void SetRangeChangingColor( float fRange ){ m_fRangeChangingColor = fRange; }
	virtual void SetTSMDelta( float fDelta ){ m_fTSMDelta = fDelta; }
	virtual float GetTSMDelta(){ return m_fTSMDelta; }
	virtual float* GetPixelOffset(){ return m_fPixelOffset; }
	virtual DWORD GetTexWidth(){ return m_dwTexWidth; }
	virtual DWORD GetTexHeight(){ return m_dwTexHeight; }

protected:
	virtual void SetShadowMode( ENUM_SHADOWMAP_MODE eMode );
};

#endif // _3DRENDERER_MANAGERSHADOWMAP_H_