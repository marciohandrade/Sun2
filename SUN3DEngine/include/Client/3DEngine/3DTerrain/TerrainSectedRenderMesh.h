// terrainsectedrendermesh.h

#ifndef _3DTERRAIN_TERRAINSECTEDRENDERMESH_H_
#define _3DTERRAIN_TERRAINSECTEDRENDERMESH_H_

#include "terrainrendersection.h"

class CWorldTerrain;
class CDrawBase;
class CTerrainRenderSectionNode;


#ifdef CLIENT_ONLY

/*
	Sectionalized Render Mesh
	
	* 렌더링할 지형을 블럭화 시키고, 관리한다.
		- 렌더섹션 단위로 렌더링
	* PS 2.0 이상에서만 지원가능함
*/
class CTerrainSectedRenderMesh
{
public:
	CTerrainSectedRenderMesh( CWorldTerrain* pWorld );
	virtual ~CTerrainSectedRenderMesh();	
	
	virtual BOOL Create( CDrawBase* pDraw );
	virtual VOID Destroy( CDrawBase* pDraw );
	virtual VOID Clear();


	virtual BOOL Enumerate_IntersectFrustum( WzFrustum *pwf, CWzArray<CTerrainRenderSection*> *parResult);


	struct St_TerrainMeshTextureInfo
	{
		BYTE m_bySortedTexture[CTerrainRenderSection::TRS_MAX_TEXTURE_STAGE];			///< 사용한 텍스쳐 인덱스
		BYTE m_bySortedBlendedAlpha[CTerrainRenderSection::TRS_MAX_TEXTURE_STAGE][3];	///< 블렌딩 된 정점의 Alpha 값(1.0f-B1, B1, B2, B3)
	};


	virtual St_TerrainMeshTextureInfo* GetTerrainMeshTextureInfo( int index) const { return &m_apTerrainMeshTextureInfo[index]; }
	virtual UINT GetSectionCount() const { return m_uiRenderSection; }
	virtual const CTerrainRenderSection* GetSection( UINT uiIndex ) const { return &m_apRenderSection[uiIndex]; }
	virtual BOOL RestoreAllRenderSection( CDrawBase* pDraw );

public:
	// Tool only
	virtual BOOL RenderAABBs( CDrawBase* pDraw );

protected:
	BOOL CreateSection( CDrawBase* pDraw );

	// 차후 Multi-Texture로 렌더링시 필요
	BOOL CreateTerrainMeshTextureInfo();

	// 섹션 단위의 렌더링 지형을 옥트리로 관리되도록 생성
	// bvTerrain	: 지형 크기
	// vFactor		: 블럭의 최소 단위(x,y,z)로 생성시 실제 블럭 크기는 이 Factor보다 같거나 크고, Factor의 2배보다는 작다.
	// parDeepestNode : 옥트리의 가장 하위 레벨의 노드 리스트로 섹션 정보를 가질수 있다.
	BOOL CreateSectionNode( const BvAABB& bvTerrain, const WzVector& vFactor, CWzArray<CTerrainRenderSectionNode*> *parDeepestNode);


private:
	CWorldTerrain* m_pWorld;

	CTerrainRenderSection * m_apRenderSection;
	UINT m_uiRenderSection;

	St_TerrainMeshTextureInfo *	m_apTerrainMeshTextureInfo;
	UINT m_uiTerrainMeshTextureInfo;

	CTerrainRenderSectionNode* m_pSectionRootNode;
};


#endif // CLIENT_ONLY

#endif // _3DTERRAIN_TERRAINSECTEDRENDERMESH_H_