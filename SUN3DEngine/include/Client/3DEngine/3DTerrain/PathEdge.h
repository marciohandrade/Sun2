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
	int m_iVertex[2];		///< �� ��
	int m_iPathTile[2];		///< ���� Ÿ��
	BYTE m_byDir[2];		///< Ÿ�Ͽ� ���� ����

	WzVector m_wvCenter;	///< Edge �� ����

};

#endif // _3DTERRAIN_PATHEDGE_H_
