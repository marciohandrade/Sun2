// pathfindergenerator.h

#ifndef _3DTERRAIN_PATHFINDERGENERATOR_H_
#define _3DTERRAIN_PATHFINDERGENERATOR_H_

#include "pathfinderbase.h"
#include "pathtile.h"
#include "pathedge.h"
#include <programcommon/wzarray.h>
#include <client/3dengine/common/wzuvvert.h>

class CPathFinderGenerator : public CPathFinderBase  
{
public:
	CPathFinderGenerator();
	virtual ~CPathFinderGenerator();
	virtual void					Clear();

	virtual void					Destroy();

	virtual BOOL					Serialize( CWzArchive *pArchive);

public:
	/// @name ���� ����
	virtual BOOL					GenerateFrom( CWzdBase *pWzd, int iMeshGroup);
	virtual BOOL					Refresh();	///< ��ã�� ���� �ٽ� ����
	virtual BOOL					RenewalCurrentTileInfoToOcxtree(); //. ���� �ִ� Ÿ�� ������ ��Ʈ���� �ٽ� ��Ͻ�Ų��.
	
	/// @name --- ���� ����

protected:
	/// @name ���� ����Ʈ
	int								m_iMaxPathVertices;			///< �ִ� ���� ����
	int								m_iNumPathVertices;			///< ���� ����
	WzVector *						m_pwvPathVertices;			///< ������ ���� ����
	/// @name Ÿ�ϵ�
	int								m_iNumPathTiles;
	CPathTile *						m_pPathTiles;
	/// @name �𼭸���
	int								m_iMaxPathEdges;			///< �ִ� �𼭸� ����
	int								m_iNumPathEdges;			///< �𼭸� ����
	CPathEdge *						m_pPathEdges;				///< �𼭸��� ���� ����

protected:
	void							ConstructPathTiles( CWzdBase *pWzd, int iMeshGroup);
	int								AddPathVertex( WzVector wvPos);
	void							ConstructPathEdges();						///< PathTile ��� ����
	int								FindPathEdge( int iVertex1, int iVertex2);
	int								AddPathEdge( int iVertex1, int iVertex2, WzVector wvCenter);
	float							CalculateDistance( int iTile, int iTilePrev );

	/// @name --- ������/Picking
public:
	virtual void					RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0, BOOL bViewCollision = FALSE);
	virtual int						PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE);
    virtual int                     PickPathTileForDebug( WzRay *pwr, WzVector* pResultVertexArray, int MaxVertexArrayCount, int* MeshCount, COctNode** pNode, float *pfT, BOOL bCulling);
	virtual BOOL					CheckPickablePathTile(WzVector* pwvCollision);
	virtual BOOL					IsOnTile( int iTile, WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE);
	virtual WzVector				GetRandomPosInTile( int iTile);
	virtual BOOL					CorrectHeightByTile( int iTile, WzVector *pwvPos);

protected:
	CWzArray<DWORD>					m_arTileMap;
	CWzArray<int>					m_arVertexMap;
	CWzArray<WzUVVert >				m_arUV;
	void							InitInstantBufferToRender();

	/// @name --- Ÿ�� ���� ����/���
public:
	virtual int						GetNumPathTile()	{ return ( m_iNumPathTiles); }
	/// @name �Ӽ�
	virtual void					SetAttribute( int iTile, WORD wAttribute );
	virtual void					UnsetAttribute( int iTile, WORD wAttribute );
	virtual BOOL					CheckAttribute( int iTile, WORD wAttribute );
	virtual WORD					GetAttribute( int iTile);

	/// @name Octree ����
	virtual WzOctIndex				GetTileOctIndex( int iTile)	{ return ( m_pPathTiles[iTile].GetOctIndex()); }
	/// @name Convex
	virtual int						GetConvexOfTile( int iTile);
	virtual int						GetNearNodes( int iTile, int *piTile, int iMaxCount);	
	
    virtual void                    GetTileVertexInfo(WzVector* pvOut, const int iTile);

	/// @name --- ��ã�� ����� �̿��ؼ� ã�ư� ��
public:
	virtual BOOL					FindPath_Requst_NewAgent(CPathExplorer *ppe){return TRUE;}
	
	virtual float					GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

	virtual WzVector				GetNearestPointToTarget( int iTile, WzVector wvTarget);	///< iTile ���� ���� �߿� wvTarget �� ���� ����� ���� ��´�.
	WzVector						GetPerpendicular( WzVector wvPoint, WzVector wvVec1, WzVector wvVec2 );
	virtual ENUM_MD_RESULT			MoveByDirection( int iCurrentTile, WzVector wvCurrentPos, WzVector wvDistance, int *piResultTile, WzVector *pwvResultPos, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), WzVector *pwvSlide = NULL);

protected:
    /**
     *  �־��� ����(From�� To)�� �մ� ������ ��ǥ Ÿ��(iTime)���� �������� �˻��Ͽ� ����� ��ȯ�Ѵ�.
     *  @param piResult : int[3]���� �����ͷ� ������ 1, �̱����� 0�� �����Ͽ� ��ȯ�Ѵ�.
     */
	void							Tile_HitTest( int iTile, WzVector wvFrom, WzVector wvTo, int *piResult, float *pfDist);
	BOOL							Tile_TestBlocked( int *piNearTiles, int *piCrossResult, WORD wAttribute, int iException, int *piResult, float *pfDist);
	BOOL							Tile_GetBlockedPos( WzVector *pwvResult, int iEdgeVert1, int iEdgeVert2, WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvSlide = NULL);
	BOOL							Tile_CrossCheck( WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvEdge1, WzVector *pwvEdge2, float *pfDist, WzVector* pwvCross = NULL);

	/// @name --- ��ã�� �Ž� ����/����� ���� �Լ�.
public:
	virtual BOOL					SavePathTiles( CWzArchive *pArchive);
	
	virtual void					GetTilesInBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult);
	virtual	void					GetTilesInterSectBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult);

	// �˻�
public:
	virtual BOOL					CheckTileConnection( WzVector *pwvError);
};


#endif // _3DTERRAIN_PATHFINDERGENERATOR_H_
