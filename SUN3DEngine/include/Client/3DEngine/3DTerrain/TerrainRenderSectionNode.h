// terrainrendersectionnode.h

#ifndef _3DTERRAIN_TERRAINRENDERSECTIONNODE_H_
#define _3DTERRAIN_TERRAINRENDERSECTIONNODE_H_

#include "octree.h"
#include <client/3dengine/common/boundingvolume.h>


class CWorldTerrain;
class CDrawBase;
class CTerrainRenderSection;

#ifdef CLIENT_ONLY


// ���� ������ ������ ������ ����ȭ�� ��Ʈ�� ���� ����
// �ڽĳ�带 �ϳ� �̻� �����ų� ���� �ּҰ��� ������.
class CTerrainRenderSectionNode
{
public:
	CTerrainRenderSectionNode( const BvAABB& bvBox);
	virtual ~CTerrainRenderSectionNode();

	BOOL Create( const WzVector& vFactor, CWzArray<CTerrainRenderSectionNode*> *parDeepestNode);

	BOOL Intersect( const BvAABB* pbvBox) const;
	virtual CTerrainRenderSectionNode* GetChildNode( int index) const;
	virtual BOOL IsChild() const { return m_bHasChild; }
	virtual const BvAABB& GetAABB() const { return m_bvBox; }
	
	virtual void SetSection( CTerrainRenderSection* pSection) { m_pSection = pSection; }
	virtual CTerrainRenderSection* GetSection() const { return m_pSection; }

	virtual BOOL Enumerate_IntersectFrustum( WzFrustum *pwf, CWzArray<CTerrainRenderSection*> *parResult, BOOL bContain = FALSE);	///< Frustum �� �����ϴ� �� ���

private:
	CTerrainRenderSection* m_pSection;
	BvAABB m_bvBox;
	BOOL m_bHasChild;
	CTerrainRenderSectionNode* m_apChild[8];	//
};


#endif // CLIENT_ONLY

#endif // _3DTERRAIN_TERRAINRENDERSECTIONNODE_H_