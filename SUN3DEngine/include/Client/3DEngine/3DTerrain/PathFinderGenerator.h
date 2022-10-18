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
	/// @name 지형 생성
	virtual BOOL					GenerateFrom( CWzdBase *pWzd, int iMeshGroup);
	virtual BOOL					Refresh();	///< 길찾기 정보 다시 생성
	virtual BOOL					RenewalCurrentTileInfoToOcxtree(); //. 현재 있는 타일 정보를 옥트리에 다시 등록시킨다.
	
	/// @name --- 지형 구조

protected:
	/// @name 정점 리스트
	int								m_iMaxPathVertices;			///< 최대 정점 개수
	int								m_iNumPathVertices;			///< 정점 개수
	WzVector *						m_pwvPathVertices;			///< 정점을 담은 버퍼
	/// @name 타일들
	int								m_iNumPathTiles;
	CPathTile *						m_pPathTiles;
	/// @name 모서리들
	int								m_iMaxPathEdges;			///< 최대 모서리 개수
	int								m_iNumPathEdges;			///< 모서리 개수
	CPathEdge *						m_pPathEdges;				///< 모서리를 담은 버퍼

protected:
	void							ConstructPathTiles( CWzdBase *pWzd, int iMeshGroup);
	int								AddPathVertex( WzVector wvPos);
	void							ConstructPathEdges();						///< PathTile 경계 생성
	int								FindPathEdge( int iVertex1, int iVertex2);
	int								AddPathEdge( int iVertex1, int iVertex2, WzVector wvCenter);
	float							CalculateDistance( int iTile, int iTilePrev );

	/// @name --- 렌더링/Picking
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

	/// @name --- 타일 정보 세팅/얻기
public:
	virtual int						GetNumPathTile()	{ return ( m_iNumPathTiles); }
	/// @name 속성
	virtual void					SetAttribute( int iTile, WORD wAttribute );
	virtual void					UnsetAttribute( int iTile, WORD wAttribute );
	virtual BOOL					CheckAttribute( int iTile, WORD wAttribute );
	virtual WORD					GetAttribute( int iTile);

	/// @name Octree 관련
	virtual WzOctIndex				GetTileOctIndex( int iTile)	{ return ( m_pPathTiles[iTile].GetOctIndex()); }
	/// @name Convex
	virtual int						GetConvexOfTile( int iTile);
	virtual int						GetNearNodes( int iTile, int *piTile, int iMaxCount);	
	
    virtual void                    GetTileVertexInfo(WzVector* pvOut, const int iTile);

	/// @name --- 길찾기 결과를 이용해서 찾아갈 때
public:
	virtual BOOL					FindPath_Requst_NewAgent(CPathExplorer *ppe){return TRUE;}
	
	virtual float					GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

	virtual WzVector				GetNearestPointToTarget( int iTile, WzVector wvTarget);	///< iTile 내의 점들 중에 wvTarget 과 가장 가까운 점을 얻는다.
	WzVector						GetPerpendicular( WzVector wvPoint, WzVector wvVec1, WzVector wvVec2 );
	virtual ENUM_MD_RESULT			MoveByDirection( int iCurrentTile, WzVector wvCurrentPos, WzVector wvDistance, int *piResultTile, WzVector *pwvResultPos, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), WzVector *pwvSlide = NULL);

protected:
    /**
     *  주어진 두점(From과 To)을 잇는 직선과 목표 타일(iTime)과의 교차점을 검사하여 결과를 반환한다.
     *  @param piResult : int[3]형의 데이터로 교차시 1, 미교차시 0을 설정하여 반환한다.
     */
	void							Tile_HitTest( int iTile, WzVector wvFrom, WzVector wvTo, int *piResult, float *pfDist);
	BOOL							Tile_TestBlocked( int *piNearTiles, int *piCrossResult, WORD wAttribute, int iException, int *piResult, float *pfDist);
	BOOL							Tile_GetBlockedPos( WzVector *pwvResult, int iEdgeVert1, int iEdgeVert2, WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvSlide = NULL);
	BOOL							Tile_CrossCheck( WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvEdge1, WzVector *pwvEdge2, float *pfDist, WzVector* pwvCross = NULL);

	/// @name --- 길찾기 매쉬 저장/재생성 관련 함수.
public:
	virtual BOOL					SavePathTiles( CWzArchive *pArchive);
	
	virtual void					GetTilesInBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult);
	virtual	void					GetTilesInterSectBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult);

	// 검사
public:
	virtual BOOL					CheckTileConnection( WzVector *pwvError);
};


#endif // _3DTERRAIN_PATHFINDERGENERATOR_H_
