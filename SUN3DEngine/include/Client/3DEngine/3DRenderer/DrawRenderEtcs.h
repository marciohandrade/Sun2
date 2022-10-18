// drawrenderetcs.h

#ifndef _3DRENDERER_DRAWRENDERETCS_H_
#define _3DRENDERER_DRAWRENDERETCS_H_

#include "draweasyrender.h"

class CDrawRenderEtcs : public CDrawEasyRender
{
public:
	CDrawRenderEtcs(void);
	virtual ~CDrawRenderEtcs(void);
	virtual void Clear();

public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();


	/// @name 2D �׸��� ( Dst ��ǥ���� 1024x768 �ػ󵵸� �������� ������ �ǰ�, Src ��ǥ���� �ؽ��� �̹��� ���� ��ǥ�� �������� ������ �ȴ�.
public:
	virtual BOOL ColorFill( WzColor wcColor, float fxDst, float fyDst, float fDstWidth, float fDstHeight, WzColor *pwcColor = NULL, float fZ = 0.0f);
	virtual BOOL StartHyper2DMode();
	virtual void EndHyper2DMode();
protected:
	CWzArray<St_Hyper2DModeObject> m_arHyper2DModeObject;
	CWzArray<St_Hyper2DModeLine> m_arHyper2DModeLine;

	/// @name Silhouette �׸���
protected:
	CWzArray<CWzSilhouette*> m_arSilhouetteToRender;
	void AddSilhouetteToRender( CWzSilhouette *pws);
	void RenderSilhouettes();

	/// @name ShadowVolume �׸���
protected:
	WzColor m_wcShadow;	///< �׸��� ����
	CWzArray<CWzShadowVolume*> m_arShadowVolumeToRender;
	void AddShadowVolumeToRender( CWzShadowVolume *pwsv);
	void RenderShadowVolumes( WzColor wcColor);
	void ShadeWithShadowVolume();
	void RenderShadowToScreen( WzColor wcColor);
	virtual BOOL RenderShadowVolumeMesh( CWzShadowVolume *pws) = 0;
public:
	virtual BOOL RenderShadowVolume( CWzShadowVolume *pws);
	virtual void SetShadowColor( WzColor wcShadow)	{ m_wcShadow = wcShadow; }
	virtual WzColor GetShadowColor()	{ return ( m_wcShadow); }


	/// @name ��Ÿ �׸��� - �����Ƿ� Release �������� ���� �� ��
protected:
	float m_fLineWidth;	///< �� ����
public:
	virtual BOOL RenderBone( HANDLE hWzd, int iStart = 0, int iEnd = -1);
protected:
	void RenderBone( int iNumNode, WzNode *pNodes, WzMatrix *pwmTransform, int iStart = 0, int iEnd = -1);
public:
	virtual BOOL RenderBoundingVolume( const WzBoundingVolume *pbv);
	virtual BOOL RenderBvAABB( const BvAABB *paabb);
	virtual BOOL RenderBvSphere( const BvSphere *pbvs);
};

#endif // _3DRENDERER_DRAWRENDERETCS_H_