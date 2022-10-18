// worldpathfinder.h

#ifndef _3DTERRAIN_WORLDPATHFINDER_H_
#define _3DTERRAIN_WORLDPATHFINDER_H_

#include "worldterrain.h"

class CWorldPathFinder : public CWorldTerrain  
{
public:
	CWorldPathFinder();
	virtual ~CWorldPathFinder();
	virtual void					Clear();

	/// @name ����/����
	virtual BOOL					Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void					Destroy( CDrawBase *pDraw);
	virtual void					Release( CDrawBase *pDraw, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);

	virtual BOOL					Serialize( CWzArchive *pArchive);

	/// @name �ؽ��� �� �б�/����
protected:
	virtual BOOL					AnalyzeWzd( CWzdBase *pWzd, BvAABB* pBvSize );

#ifdef CLIENT_ONLY
	virtual BOOL					GenerateFrom( char *lpszFileName, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	virtual BOOL					GenerateFromWZM(CWzdBase *pWzd, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);			//. Wzm ����ü�� ���� Terrain Mesh�� ��ã�� Mesh�� �����Ѵ�.
#endif

	/// @name --- ��ã�� ����
public:	
	virtual	BvAABB					GetPathFinderSize() { return m_bvPathFinderMeshSize; }
	virtual WzOctIndex				GetTileOctIndex( int iTile)	{ return ( x_pPathFinder->GetTileOctIndex( iTile)); }	// Tile ���� ���
	virtual int						GetTileForTerrainMesh( int iTerrainMesh);	///< �޽��� ����� Ÿ�� ã��
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector wvPos, int iTile = -1);	///< 3D ��ǥ�� �༭ �ʱ�ȭ
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector *pwvPos, int iTile = -1);	///< 3D ��ǥ�� �༭ �ʱ�ȭ
	virtual ENUM_FINDPATH_RESULT	PathExp_FindPath( CPathExplorer *ppe, WzVector wvDest, int iDestTile, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);	///< �� ã��	
	virtual BOOL					PathExp_SetPath( CPathExplorer *ppe, WzVector *pwvDest, int *pTileRoute, int iNumTileRoute);	///< ��� ����
	virtual BOOL					PathExp_Progress( CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, ENUM_CHECK_COLLISION eCC = ECC_NO);	///< �� ã�� ����
	
protected:
	void							SetPathFinderSize( BvAABB bvSize ) { m_bvPathFinderMeshSize = bvSize; }		//. ��ã�� �޽��� ũ�� ������ �ִ´�.
	void							PathExp_HeadToNextTile( CPathExplorer *ppe);
	void							PathExp_AdjustVisibleZValue( CPathExplorer *ppe, float fAdjustLimit = -1.0f);	///< ������ �������� z �� ���
	BOOL							PathExp_GetJumpDestPos( CPathExplorer *ppe, WzVector *pwvDest);

public:
	virtual int						PathExp_GetMeshToStand( WzVector wvPos, float *pfT, float fMeshTileDist = -1.0f, DWORD dwTerrainGroupOptionCheck = 0,WzVector *pwvNormal = NULL, float wvHeight = 1.0f);	///< 3D ��ǥ �� �� ������ �� ���� �޽� ���
	virtual ENUM_FINDPATH_RESULT	PathExp_Thrust( CPathExplorer *ppe, WzVector *pwvMoveDistance, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK));	///< Ư�� �������� Ư�� �Ÿ���ŭ �б�

	/// @name --- Tile ������ ����/Picking
public:
	virtual void					RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0, BOOL bViewCollision = FALSE);	///< ��ã�� ���� ���
	virtual int						PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE)	{ return ( x_pPathFinder->PickPathTile( pwr, pfT)); }	///< ��ã�� ���� pick
    virtual int						PickPathTileForDebug( WzRay *pwr, WzVector* pResultVertexArray, int MaxVertexArrayCount, int* MeshCount, COctNode** pNode, float *pfT, BOOL bCulling)
    {
        return x_pPathFinder->PickPathTileForDebug( pwr, pResultVertexArray, MaxVertexArrayCount, MeshCount, pNode, pfT, bCulling );
    }

	virtual WzVector				GetRandomPosInTile( int iTile)	{ return ( x_pPathFinder->GetRandomPosInTile( iTile)); }	///< Tile �� random ��ġ ���
	virtual BOOL					CorrectHeightByTile( int iTile, WzVector *pwvPos)	{ return ( x_pPathFinder->CorrectHeightByTile( iTile, pwvPos)); }	// Tile, x, y -> z ���
	virtual void					RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor);	///< ������ ���

protected:
	BvAABB m_bvPathFinderMeshSize;
};

#endif // _3DTERRAIN_WORLDPATHFINDER_H_
