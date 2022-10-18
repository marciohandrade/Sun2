// pathconvex.h

#ifndef _3DTERRAIN_PATHCONVEX_H_
#define _3DTERRAIN_PATHCONVEX_H_

#include "AStarNode.h"

typedef struct St_NearConvexInfo
{
	int m_iTileOfNearConvex;	///< ���� Convex �� Tile
	Wz2DLine m_w2lLine;
} St_NearConvexInfo;


class CPathConvex : public CAStarNode  
{
public:
	CPathConvex();
	virtual ~CPathConvex();
	virtual void Clear();

	virtual BOOL Serialize( CWzArchive *pArchive);

protected:
	int m_iNumEdge;	///< Convex �������� Edge ����
	int *m_piEdges;	///< Convex �������� Edge (�ε���)
	int *m_piCorners;	///< Convex ���� (�ε���)
	WzVector *m_pwvCorners;	///< Convex ���� (����)
	CWzArray<int> m_arPathTilesInConvex;	///< Convex �� ���Ե� Ÿ�ϵ�

	WORD m_wConvexAttribute;
public:
	virtual void SetAttribute( WORD wAttribute)		{ m_wConvexAttribute |= wAttribute; }
	virtual void UnsetAttribute( WORD wAttribute)	{ m_wConvexAttribute &= ~wAttribute; }
	virtual BOOL CheckAttribute( WORD wAttribute)	{ return ( 0 != ( m_wConvexAttribute & wAttribute)); }
	virtual WORD GetAttribute()	{ return ( m_wConvexAttribute); }

	/// @name --- �����ϱ�
public:
	CPathConvex& operator = ( CPathConvex &pcFrom);
	void Init( int iPathTile, int *piEdges);
	BOOL ReplaceEdge( int iEdge1, int iEdge2, int iEdgeToRemove);
	void AddPathTile( int iPathTile)	{ m_arPathTilesInConvex.Add( iPathTile); }
	void SetCorners( int *piCorners, WzVector *pwvCorners);

	/// @name --- ���� ���
	int GetNumEdge()	{ return ( m_iNumEdge); }
	int GetEdgeIndex( int iIndex)	{ return ( m_piEdges[iIndex]); }
	void GetEdgeIndexes( int *piEdges)	{ memcpy( piEdges, m_piEdges, m_iNumEdge * sizeof ( int)); }
	int GetCornerIndex( int iCornerIndex)	{ return ( m_piCorners[iCornerIndex]); }
	void GetCornerIndexes( int *piCornerEdges)	{ memcpy( piCornerEdges, m_piCorners, m_iNumEdge * sizeof ( int)); }
	WzVector GetCorner( int iIndex)	{ return ( m_pwvCorners[iIndex]); }
	void GetCorners( WzVector *pwvCorners)	{ memcpy( pwvCorners, m_pwvCorners, m_iNumEdge * sizeof ( WzVector)); }

	/// @name ������ ������� ���
	int GetNextEdgeOf( int iEdge);
	int GetPrevEdgeOf( int iEdge);

	/// @name --- �ֺ� ����
protected:
	CWzArray<St_NearConvexInfo> m_arNearConvex;	///< ������ Convex ����
public:
	void AddNearConvex( int iOnThisEdge, int iNearConvex, int iTileOfNearConvex, Wz2DLine w2lLine, float fCost );
	void SetDistance( int iConvex, float fCost );
	int GetNearConvexIndex( int iNearConvex)	{ return ( FindNearNode( iNearConvex)); }
	St_NearConvexInfo GetNearConvexInfo( int iNearConvex)	{ return ( m_arNearConvex.Get( GetNearConvexIndex( iNearConvex))); }
};

#endif // _3DTERRAIN_PATHCONVEX_H_
