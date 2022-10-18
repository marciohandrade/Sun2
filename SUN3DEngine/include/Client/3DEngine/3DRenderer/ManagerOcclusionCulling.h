// managerocclusionculling.h

#ifndef _3DRENDERER_MANAGEROCCLUSIONCULLING_H_
#define _3DRENDERER_MANAGEROCCLUSIONCULLING_H_

#include "managerbase.h"

class CManagerOcclusionCulling : public CManagerBase
{
public:
	CManagerOcclusionCulling();

	virtual void ClearOcclusion() {}
	virtual BOOL CheckOccluded( St_OcclusionInfo* pInfo, BOOL bWait=FALSE) { return FALSE; }
	virtual St_OcclusionInfo* AddOcclusion( CWzUnitDraw* pUnitDraw) { return NULL; }
	virtual UINT GetOccludedCount( BOOL bOccluded) const { return bOccluded ? m_uiOccluded : m_uiNonOccluded; }

	virtual VOID UpdateFrame( DWORD dwTick);

	virtual BOOL IsOccludedByBV( WzBoundingVolume* pBv) { return FALSE; }
	virtual St_OcclusionInfo* BeginOcclusion() { return NULL; }
	virtual BOOL EndOcclusion( St_OcclusionInfo* pOcclusion) { return FALSE; }


protected:
	UINT m_uiOccluded;
	UINT m_uiNonOccluded;
	BOOL m_bSupportHardware;
};


#include "managerd3d.h"

class CManagerOcclusionCullingD3D : public CManagerOcclusionCulling, CManagerD3D
{
public:
	///< 생성자/소멸자
	CManagerOcclusionCullingD3D();
	virtual ~CManagerOcclusionCullingD3D();

	///< CManagerBase의 기본 함수
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	///< 
	virtual void ClearOcclusion();
	virtual BOOL CheckOccluded( St_OcclusionInfo* pInfo, BOOL bWait=FALSE);
	virtual St_OcclusionInfo* AddOcclusion( CWzUnitDraw* pUnitDraw);

	///< 
	virtual BOOL IsOccludedByBV( WzBoundingVolume* pBv);
	virtual St_OcclusionInfo* BeginOcclusion();
	virtual BOOL EndOcclusion( St_OcclusionInfo* pOcclusion);

protected:
	virtual St_OcclusionInfo* AllocOcclusion();
	virtual BOOL OcclusionByAABB( void* pQuery, const BvAABB* paabb);

protected:
	virtual BOOL SetupOcclusion();
	virtual BOOL DestroyOcclusion();

protected:
	CWzArray<St_OcclusionInfo*> m_arOcclusion;
	int m_iAllocOcclusion;
};

#endif // _3DRENDERER_MANAGEROCCLUSIONCULLING_H_