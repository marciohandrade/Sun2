// managershadowmapd3d.h

#ifndef _3DRENDERER_MANAGERSHADOWMAPD3D_H_
#define _3DRENDERER_MANAGERSHADOWMAPD3D_H_

#include "managershadowmap.h"
#include "managerd3d.h"


class CManagerShadowMapD3D : public CManagerShadowMap, CManagerD3D
{
public:
	CManagerShadowMapD3D(void);
	virtual ~CManagerShadowMapD3D(void);
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void OnResetDevice();
	virtual ENUM_SHADOWMAP_MODE ConfirmDevice();

protected:
	virtual void Clear();
	
	virtual void CalculateTransformMatrix();
	virtual void CalculateProjectionMatrix();
	virtual void GenerateFrustumFromMatrix( WzFrustumShadow* pResult, WzMatrix* pwmInput );

	virtual void SetTexBiasMatrix( DWORD dwTexWidth, DWORD dwTexHeight );

	WORD TestAABB( WzFrustumShadow* pwfs, BvAABB* pAABB );

protected:
	D3DCAPS9	*m_pD3DCaps;

};

#endif // _3DRENDERER_MANAGERSHADOWMAPD3D_H_