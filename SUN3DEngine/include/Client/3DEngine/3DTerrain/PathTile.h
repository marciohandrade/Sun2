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
	WzOctIndex m_woi;	///< ��Ʈ�� ���� �ε���
	int m_iVertex[3];	///< �� ���� �ε���
	WzVector m_wvNormal;	///< �ﰢ���� Normal ��

	int m_iNearPathTile[3];		///< ���� Tile �ε���	0 : 0-1 ����, 1 : 1-2 ����, 2 : 2-0 ����
	int m_iNearPathEdge[3];		///< ���� Edge �ε���
	BYTE m_byIsConvex[3];		///< 1 : ������, 0 : �ƴ�

	/// @name ��ã�� ���� ���� ����
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

	/// @name ��ã�� ����
public:
	int GetPathEdgeToTile( int iTile);

	/// @name ���� ����
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
