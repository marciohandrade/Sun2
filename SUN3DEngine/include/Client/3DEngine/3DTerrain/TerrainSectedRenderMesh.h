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
	
	* �������� ������ ��ȭ ��Ű��, �����Ѵ�.
		- �������� ������ ������
	* PS 2.0 �̻󿡼��� ����������
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
		BYTE m_bySortedTexture[CTerrainRenderSection::TRS_MAX_TEXTURE_STAGE];			///< ����� �ؽ��� �ε���
		BYTE m_bySortedBlendedAlpha[CTerrainRenderSection::TRS_MAX_TEXTURE_STAGE][3];	///< ���� �� ������ Alpha ��(1.0f-B1, B1, B2, B3)
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

	// ���� Multi-Texture�� �������� �ʿ�
	BOOL CreateTerrainMeshTextureInfo();

	// ���� ������ ������ ������ ��Ʈ���� �����ǵ��� ����
	// bvTerrain	: ���� ũ��
	// vFactor		: ���� �ּ� ����(x,y,z)�� ������ ���� �� ũ��� �� Factor���� ���ų� ũ��, Factor�� 2�躸�ٴ� �۴�.
	// parDeepestNode : ��Ʈ���� ���� ���� ������ ��� ����Ʈ�� ���� ������ ������ �ִ�.
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