// terrainrendersection.h

#ifndef _3DTERRAIN_TERRAINRENDERSECTION_H_
#define _3DTERRAIN_TERRAINRENDERSECTION_H_

#include "octree.h"
#include <client/3dengine/common/boundingvolume.h>

class CWorldTerrain;
class CDrawBase;
class CTerrainSectedRenderMesh;


#ifdef CLIENT_ONLY


class CTerrainRenderSection
{	
public:
	CTerrainRenderSection();
	virtual ~CTerrainRenderSection();

	virtual BOOL CreateSection( CDrawBase* pDraw, CWorldTerrain* pWorld, CTerrainSectedRenderMesh* pRenderMesh, CWzArray<DWORD>* pMeshIndices );
	virtual VOID Clear();
	virtual VOID Destroy( CDrawBase* pDraw );


	enum { TRS_MAX_TEXTURE_BLENDED = 3 }; // MAX_BLENDED_LAYER
	enum { TRS_MAX_TEXTURE_STAGE = 1 + TRS_MAX_TEXTURE_BLENDED };
	enum { TRS_MAX_NUM_TEXTURE = 255 }; // MAX_NUM_TEXTURE_FOR_TERRAIN is 1024?!

	struct St_RenderSegmentInfo
	{
		INT iTextureIndex;
		UINT uiStartIndex;
		UINT uiPrimitiveCount;
	};

	struct St_TextureLayer
	{
		BOOL bEnable;
		UINT uiStartSegment;
		UINT uiEndSegment;
	};
	
	virtual const BvAABB& GetAABB() const { return m_bvSize; }
	virtual void SetAABB( const BvAABB& aabb) { m_bvSize = aabb; }
	virtual UINT GetSegmentCount() const { return m_uiRenderSegmentCount; }
	virtual const St_RenderSegmentInfo* GetSegment( UINT uiIndex) const { return &m_apRenderSegment[uiIndex]; }
	virtual HANDLE GetIndexBuffer() const { return m_hOwnIndexBuffer; }
	virtual BOOL IsRenderable() const { return (m_uiRenderSegmentCount) ? TRUE : FALSE; }
	virtual BOOL RestoreSection( CDrawBase* pDraw);
	virtual DWORD GetNumberOfIndex() const { return m_dwNumIndices; }
	virtual const St_TextureLayer* GetTextureLayer( UINT uiLayer) const { return &m_aLayer[uiLayer]; }

protected:
	virtual BOOL FillIndexBuffer( CDrawBase* pDraw);

private:
	BvAABB m_bvSize;
	HANDLE	m_hOwnIndexBuffer;
	St_RenderSegmentInfo * m_apRenderSegment;
	UINT m_uiRenderSegmentCount;	
	DWORD *	m_pdwIndices;
	DWORD m_dwNumIndices;
	St_TextureLayer m_aLayer[TRS_MAX_TEXTURE_STAGE];
};


#endif // CLIENT_ONLY

#endif // _3DTERRAIN_TERRAINRENDERSECTION_H_