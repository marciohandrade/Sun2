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

	/// @name 생성/제거
	virtual BOOL					Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void					Destroy( CDrawBase *pDraw);
	virtual void					Release( CDrawBase *pDraw, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);

	virtual BOOL					Serialize( CWzArchive *pArchive);

	/// @name 텍스쳐 등 읽기/제거
protected:
	virtual BOOL					AnalyzeWzd( CWzdBase *pWzd, BvAABB* pBvSize );

#ifdef CLIENT_ONLY
	virtual BOOL					GenerateFrom( char *lpszFileName, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	virtual BOOL					GenerateFromWZM(CWzdBase *pWzd, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);			//. Wzm 구조체로 부터 Terrain Mesh나 길찾기 Mesh를 생성한다.
#endif

	/// @name --- 길찾기 관련
public:	
	virtual	BvAABB					GetPathFinderSize() { return m_bvPathFinderMeshSize; }
	virtual WzOctIndex				GetTileOctIndex( int iTile)	{ return ( x_pPathFinder->GetTileOctIndex( iTile)); }	// Tile 정보 얻기
	virtual int						GetTileForTerrainMesh( int iTerrainMesh);	///< 메쉬와 연결된 타일 찾기
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector wvPos, int iTile = -1);	///< 3D 좌표를 줘서 초기화
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector *pwvPos, int iTile = -1);	///< 3D 좌표를 줘서 초기화
	virtual ENUM_FINDPATH_RESULT	PathExp_FindPath( CPathExplorer *ppe, WzVector wvDest, int iDestTile, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);	///< 길 찾기	
	virtual BOOL					PathExp_SetPath( CPathExplorer *ppe, WzVector *pwvDest, int *pTileRoute, int iNumTileRoute);	///< 경로 지정
	virtual BOOL					PathExp_Progress( CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, ENUM_CHECK_COLLISION eCC = ECC_NO);	///< 길 찾기 진행
	
protected:
	void							SetPathFinderSize( BvAABB bvSize ) { m_bvPathFinderMeshSize = bvSize; }		//. 길찾기 메쉬의 크기 가지고 있는다.
	void							PathExp_HeadToNextTile( CPathExplorer *ppe);
	void							PathExp_AdjustVisibleZValue( CPathExplorer *ppe, float fAdjustLimit = -1.0f);	///< 겉으로 보여지는 z 값 얻기
	BOOL							PathExp_GetJumpDestPos( CPathExplorer *ppe, WzVector *pwvDest);

public:
	virtual int						PathExp_GetMeshToStand( WzVector wvPos, float *pfT, float fMeshTileDist = -1.0f, DWORD dwTerrainGroupOptionCheck = 0,WzVector *pwvNormal = NULL, float wvHeight = 1.0f);	///< 3D 좌표 상에 서 있을때 발 딛을 메쉬 얻기
	virtual ENUM_FINDPATH_RESULT	PathExp_Thrust( CPathExplorer *ppe, WzVector *pwvMoveDistance, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK));	///< 특정 방향으로 특정 거리만큼 밀기

	/// @name --- Tile 렌더링 관련/Picking
public:
	virtual void					RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0, BOOL bViewCollision = FALSE);	///< 길찾기 지형 출력
	virtual int						PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE)	{ return ( x_pPathFinder->PickPathTile( pwr, pfT)); }	///< 길찾기 지형 pick
    virtual int						PickPathTileForDebug( WzRay *pwr, WzVector* pResultVertexArray, int MaxVertexArrayCount, int* MeshCount, COctNode** pNode, float *pfT, BOOL bCulling)
    {
        return x_pPathFinder->PickPathTileForDebug( pwr, pResultVertexArray, MaxVertexArrayCount, MeshCount, pNode, pfT, bCulling );
    }

	virtual WzVector				GetRandomPosInTile( int iTile)	{ return ( x_pPathFinder->GetRandomPosInTile( iTile)); }	///< Tile 내 random 위치 얻기
	virtual BOOL					CorrectHeightByTile( int iTile, WzVector *pwvPos)	{ return ( x_pPathFinder->CorrectHeightByTile( iTile, pwvPos)); }	// Tile, x, y -> z 얻기
	virtual void					RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor);	///< 컨벡스 출력

protected:
	BvAABB m_bvPathFinderMeshSize;
};

#endif // _3DTERRAIN_WORLDPATHFINDER_H_
