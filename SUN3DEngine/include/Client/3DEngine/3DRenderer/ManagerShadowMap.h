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
	ESM_NONE = 0,			// ShadowMap ������
	ESM_DEPTHSTENCIL,		// NVidia �� ( �ϵ���� ���� )
	ESM_R32F,				// ATI �� ( ���̴��� ������ ����Ʈ���� ShadowMap )
	ESM_PROTECTED_TEXTURE,	// �׸��� ���� ���� �ȵǴ� �׷���ī���. �� ������ ������ �׸���.
	ESM_DEPTHSTENCIL_NO_SELFSHADOW,	// �ڱ�׸��� ���� ���̹��� ���� �׸���
	ESM_R32F_NO_SELFSHADOW,
	NUM_SHADOWTEXTURE,
};

// WzFrustum�� Frustum �� �� ������ 8�� �߰�
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
	
	// ManagerRenderCommand�� ��ϵ� unit�� �߿��� Caster�� Receiver�� �߷���
	virtual BOOL ChooseShadowCastersAndReceivers( St_RenderCommand** ppRenderCommand, int iNumRCShadow );
	
	virtual void SetTexBiasMatrix( DWORD dwTexWidth, DWORD dwTexHeight ) = 0;
	
protected:
	// ��ȯ��� ���
	virtual void CalculateTransformMatrix() = 0;
	virtual void CalculateProjectionMatrix() = 0;
	// ������� �Ǵ� ��*������ķ� ���� �������� ���
	virtual void GenerateFrustumFromMatrix( WzFrustumShadow* pResult, WzMatrix* pwmInput ) = 0;
	

protected:
	ENUM_SHADOWMAP_MODE				m_eShadowMode;					// �ϵ���� ���� ���
	HANDLE							m_hShadowMap;					// �׸��� �� �ؽ���
	HANDLE							m_hDepth;						// �׸��� �� �׸��� ���� ����
	DWORD							m_dwTexWidth, m_dwTexHeight;	// �׸��� �� ũ��
	WzMatrix						m_wmShadowTransform;			// ��ȯ ���( �׸��� �ʿ� �׸� �� )
	WzMatrix						m_wmProjForShadow;				// �׸��ڿ� ���� ���

	WzMatrix						m_wmShadowTransformBias;		// ��ȯ ���( �׸��� ���� Unit�� �׸� �� )
	WzMatrix						m_wmShadowTransformBiasBlur[4];	// �� ��ȯ ���( �׸��� ���� Unit�� �׸� �� )
	WzMatrix						m_wmTexBias;					// �� ���� ��( ATI )
	WzMatrix						m_wmTexBiasBlur[4];				// �׸��ڸ��� �ؽ��� ��ǥ�� ����� �� ���� ��� �簢�� ����

	float							m_fShadowRange;					// �׸��ڰ� ���� �� �Ÿ�
	float							m_fRangeChangingColor;			// �׸��� ���� �������� �Ÿ�
	float							m_fShadowFOV;					// �׸��� �ʿ� FOV
	float							m_fZNear;//, m_fZFar;				// m_wmProjForShadow�� ZMin, ZFar
	float							m_fTSMDelta;					// Delta ��
	float							m_fDepthBias;					// DepthBias ��
	CWzArray< St_RenderCommand* >	m_arShadowCasterUnits;			// Shadow Casters
	CWzArray< St_RenderCommand* >	m_arShadowReceiverUnits;		// Shadow Receivers
	int								m_iNumRCShadow;					// 
	St_RenderCommand**				m_ppRenderCommand;				//
	BOOL							m_bCastersAndReceivers;
	
	CWzArray< CSpeedTreeDrawInfo* > m_arShadowCasterUnitsST;		// Shadow Casters For SpeedTree
	CWzArray< CSpeedTreeDrawInfo* > m_arShadowReceiverUnitsST;		// Shadow Receivers For SpeedTree
	int								m_iNumSTShadow;					
	BOOL							m_bActiveSpeedTree;				// ShadowMap�� ���ǵ� Ʈ�� ���뿩��
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