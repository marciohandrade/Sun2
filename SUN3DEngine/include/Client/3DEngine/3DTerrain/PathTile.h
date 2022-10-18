// pathtile.h

#ifndef _3DTERRAIN_PATHTILE_H_
#define _3DTERRAIN_PATHTILE_H_

#include "astarnode.h"
#include "octnode.h"

class CWzArchive;


class CPathTile : public CAStarNode  
{
public:
	CPathTile();
	virtual ~CPathTile();
	virtual void Clear();

public:
	virtual void Init( int *piVertexIndex, WzOctIndex woi, WzVector wvNormal);
	virtual BOOL Serialize( CWzArchive *pArchive);

protected:
	WzOctIndex m_woi;	///< 옥트리 상의 인덱스
	int m_iVertex[3];	///< 세 점의 인덱스
	WzVector m_wvNormal;	///< 삼각형의 Normal 값

	int m_iNearPathTile[3];		///< 인접 Tile 인덱스	0 : 0-1 사이, 1 : 1-2 사이, 2 : 2-0 사이
	int m_iNearPathEdge[3];		///< 인접 Edge 인덱스
	BYTE m_byIsConvex[3];		///< 1 : 컨벡스, 0 : 아님

	/// @name 길찾기 정보 구성 관련
public:
	WzOctIndex GetOctIndex()	{ return ( m_woi); }
	void SetOctIndex(WzOctIndex woi)	{m_woi = woi;}

	int GetVertex( int iDir)	{ return ( m_iVertex[iDir]); }
	void GetVertices( int *piVertex)
	{
		memcpy( piVertex, &m_iVertex, 3 * sizeof ( int));
	}
	void SetNearPathTile( int iNearPathTile, int iNearPathEdge, int iDir, float fCost )
	{
		m_iNearPathTile[iDir] = iNearPathTile;
		m_iNearPathEdge[iDir] = iNearPathEdge;

		St_PathNearNodeInfo pnni;
		pnni.m_iNearNode = iNearPathTile;
		pnni.m_fCost = fCost;
		m_arNearNodes.Set( iDir, pnni);
	}
	void GetNearPathTile( int *piNearPathTile)
	{
		memcpy( piNearPathTile, m_iNearPathTile, 3 * sizeof ( int));
	}
	void GetNearPathEdge( int *piNearPathEdge)
	{
		memcpy( piNearPathEdge, m_iNearPathEdge, 3 * sizeof ( int));
	}
	void SetNearPathTileConvex( int iDir, BOOL bConvex = TRUE)
	{
		m_byIsConvex[iDir] = bConvex ? 1 : 0;
	}
	BOOL IsNearPathTileConvex( int iDir)
	{
		return ( ( BOOL)m_byIsConvex[iDir]);
	}

	/// @name 길찾기 관련
public:
	int GetPathEdgeToTile( int iTile);

	/// @name 정보 참고
public:
	int GetNextVertexOf( int iVertex);

	/// @name Convex
protected:
	int m_iConvex;
public:
	void SetConvex( int iConvex)	{ m_iConvex = iConvex; }
	int GetConvex()	{ return ( m_iConvex); }
};

#endif // _3DTERRAIN_PATHTILE_H_
