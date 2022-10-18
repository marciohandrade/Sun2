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


	/// @name 2D 그리기 ( Dst 좌표들은 1024x768 해상도를 기준으로 넣으면 되고, Src 좌표들은 텍스쳐 이미지 상의 좌표를 기준으로 넣으면 된다.
public:
	virtual BOOL ColorFill( WzColor wcColor, float fxDst, float fyDst, float fDstWidth, float fDstHeight, WzColor *pwcColor = NULL, float fZ = 0.0f);
	virtual BOOL StartHyper2DMode();
	virtual void EndHyper2DMode();
protected:
	CWzArray<St_Hyper2DModeObject> m_arHyper2DModeObject;
	CWzArray<St_Hyper2DModeLine> m_arHyper2DModeLine;

	/// @name Silhouette 그리기
protected:
	CWzArray<CWzSilhouette*> m_arSilhouetteToRender;
	void AddSilhouetteToRender( CWzSilhouette *pws);
	void RenderSilhouettes();

	/// @name ShadowVolume 그리기
protected:
	WzColor m_wcShadow;	///< 그림자 색상
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


	/// @name 기타 그리기 - 느리므로 Release 버전에는 쓰지 말 것
protected:
	float m_fLineWidth;	///< 선 굵기
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