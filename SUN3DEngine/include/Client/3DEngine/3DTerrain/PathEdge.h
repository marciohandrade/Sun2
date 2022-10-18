// pathedge.h

#ifndef _3DTERRAIN_PATHEDGE_H_
#define _3DTERRAIN_PATHEDGE_H_

class CWzArchive;


class CPathEdge  
{
public:
	CPathEdge();
	virtual ~CPathEdge();
	void Clear();

	BOOL Create( int iVertex1, int iVertex2, WzVector wvCenter);
	void Destroy();

	BOOL Serialize( CWzArchive *pArchive);

	int GetVertex( int iIndex)	{ return ( m_iVertex[iIndex]); }
	BOOL GetVerticesForTile( int iTile, int *piVertices);
	WzVector GetCenter()	{ return ( m_wvCenter); }
	BOOL IsSameEdge( int iVertex1, int iVertex2)	{ return ( ( m_iVertex[0] == iVertex1 && m_iVertex[1] == iVertex2) || ( m_iVertex[0] == iVertex2 && m_iVertex[1] == iVertex1)); }
	void SetPathTile( int iPathTile, BYTE byDir, int iIndex = 0);
	int GetPathTile( int iIndex)	{ return ( m_iPathTile[iIndex]); }
	BYTE GetPathTileDir( int iIndex)	{ return ( m_byDir[iIndex]); }

protected:
	int m_iVertex[2];		///< 두 점
	int m_iPathTile[2];		///< 붙은 타일
	BYTE m_byDir[2];		///< 타일에 붙은 방향

	WzVector m_wvCenter;	///< Edge 의 중점

};

#endif // _3DTERRAIN_PATHEDGE_H_
