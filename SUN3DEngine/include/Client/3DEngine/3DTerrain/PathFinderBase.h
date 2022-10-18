// pathfinderbase.h

#ifndef _3DTERRAIN_PATHFINDERBASE_H_
#define _3DTERRAIN_PATHFINDERBASE_H_

#include "octnode.h"
#include "pathtile.h"

class CDiscAccess;
class COctree;
class CWzdBase;
class CDrawBase;
class CPathEdge;
class CWzArchive;
class CPathExplorer;
class CRoomManager;


enum ENUM_MD_RESULT	// MoveByDirection 함수 결과
{
	MDR_UNKNOWN_ERROR = -2,	// 알 수 없는 에러
	MDR_INVALID_TILE = -1,		// 오류
	MDR_MOVED_WELL = 0,		// 목표점 도달
	MDR_BLOCKED,		// 벽에 부딪힘
	MDR_ON_EDGE,		// 모서리 위에 있음
};

enum ENUM_COLLISION_OBJECT
{
	ECO_OBJECT,
	ECO_SPEEDTREE,
};
enum ENUM_COLLISION_TYPE
{
	ECT_STATIC,
	ECT_DYNAMIC,
};
enum ENUM_CHECK_COLLISION
{
	ECC_YES,
	ECC_NO,
};
class CPathFinderBase  
{
public:
	CPathFinderBase();
	virtual ~CPathFinderBase();	

	virtual BOOL					Create( COctree *pOctree, CRoomManager *pRoomManager);

	virtual void					Clear();
	virtual void					Destroy();
	virtual BOOL					Serialize( CWzArchive *pArchive) = 0;

protected:
	COctree *						m_pOctree;
	CRoomManager *					m_pRoomManager;

public:
	/// @name 지형 생성
	virtual BOOL					GenerateFrom( CWzdBase *pWzd, int iMeshGroup) = 0;
	virtual BOOL					Refresh() = 0;					///< 길찾기 정보 다시 생성
	virtual BOOL					RenewalCurrentTileInfoToOcxtree() = 0; //. 현재 있는 타일 정보를 옥트리에 다시 등록시킨다.
	
	virtual BOOL					EntryCollisionTerritory(ENUM_COLLISION_OBJECT eCO, long lnID, WzVector wzPos, WzBoundingVolume* pwzBv) {return TRUE;} //. 바운딩 볼륨 aabb 등록할때
	virtual BOOL					EntryCollisionTerritoryUnderLine(ENUM_COLLISION_OBJECT eCO, long lnID, WzVector wzPos, int iVectexNum, WzVector* pwzVector) {return TRUE;}//. 그 외 충돌 영역을 언더라인으로 등록할때
	virtual BOOL					RemoveCollisionTerritory(ENUM_COLLISION_OBJECT eCO, long lnID){return TRUE;};
	
	virtual BOOL					RenewalDynamicCollisionAttribute(){ return TRUE; }	//. ActiveRoom 으로 속성 테이블이 바뀔경우 Attribute Collision도 갱신 해줘야 한다.
	
	virtual int						DuplicateDynamicCollision(int iSourceIndex = 0 ){return -1;}
	virtual BOOL					DestroyDynamicCollision( int iRoomIndex ){return FALSE;}

	virtual BOOL					LoadStaticCollision( CDiscAccess* pStream, const char* pszFileName) { return TRUE; }
	virtual BOOL					SaveStaticCollision( CDiscAccess* pStream, const char* pszFileName) { return TRUE; }

	/// @name 길 찾기
	virtual BOOL					FindPath_Requst_NewAgent(CPathExplorer *ppe) { return TRUE; }
	virtual BOOL					FindPath_Requst_DestroyAgent(CPathExplorer *ppe) { return TRUE; }
	virtual BOOL					FindPath_Agent_Reset(CPathExplorer *ppe, WzVector *pwvPos, int iTile) { return TRUE; };
	virtual BOOL					FindPath_Progress(CPathExplorer *ppe,float fMove, ENUM_CHECK_COLLISION eCC = ECC_NO ){ return TRUE; }
	virtual ENUM_FINDPATH_RESULT	FindPath( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND) = 0;	///< Convex 를 이용해서 필요한 거점 Tile 만 얻는다.
	virtual ENUM_FINDPATH_RESULT	FindPath_FullTile( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND) = 0;	///< Convex 를 안쓰면 모든 Tile 을 얻는다.
	virtual ENUM_FINDPATH_RESULT	FindPath_AsConvex( int iConvexStart, int iConvexEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK)) = 0;	///< Convex 단위의 길찾기만 해서 결과 얻기
	virtual BOOL					FindPath_CheckCollision(CPathExplorer *ppe) = 0;
	virtual BOOL					SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute) = 0;	///< 경로 지정

	/// @name --- 렌더링/Picking
public:
	virtual void					RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0, BOOL bViewCollision = FALSE) = 0;
	virtual int						PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE) = 0;
    virtual int						PickPathTileForDebug( WzRay *pwr, WzVector* pResultVertexArray, int MaxVertexArrayCount, int* MeshCount, COctNode** pNode, float *pfT, BOOL bCulling) = 0;

	virtual BOOL					CheckPickablePathTile(WzVector* pwvCollision)=0;
	virtual BOOL					IsOnTile( int iTile, WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE) = 0;	// 특정 타일에 대해서만 Picking 하기
	virtual WzVector				GetRandomPosInTile( int iTile) = 0;
	virtual BOOL					CorrectHeightByTile( int iTile, WzVector *pwvPos) = 0;
	virtual void					RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor) = 0;

	
	/// @name --- 타일 정보 세팅/얻기
public:
	virtual int						GetNumPathTile() = 0;
	/// @name 속성
	virtual void					SetAttribute( int iTile, WORD wAttribute ) = 0;
	virtual void					UnsetAttribute( int iTile, WORD wAttribute ) = 0;
	virtual BOOL					CheckAttribute( int iTile, WORD wAttribute) = 0;
	virtual WORD					GetAttribute( int iTile) = 0;
	/// Octree 관련
	virtual WzOctIndex				GetTileOctIndex( int iTile) = 0;
	/// Convex
	virtual int						GetConvexOfTile( int iTile) = 0;
	/// 주변 타일 얻기
	virtual int						GetNearNodes( int iTile, int *piTile, int iMaxCount) = 0;
    /// 타일에 대한 3정점을 얻는다.
    /// @param : pvOut : 타일에 대한 정점 배열을 얻기 위한 버퍼. WzVector[3]
    /// @param : iTile : 원하는 길메쉬 타일의 인덱스
    virtual void                    GetTileVertexInfo( WzVector* pvOut, const int iTile) = 0;

	/// @name --- 길찾기 결과를 이용해서 찾아갈 때
public:
	virtual int						GetTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL) = 0;	///< 3D 좌표 상에 서 있을때 발 딛을 타일 얻기 (리턴값이 -1인 경우 갈 수 없는 곳)

	virtual int						GetAllowTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL,WORD wNotAllowAttribute=PTA_NO_WALK)=0;
	virtual CPathEdge*				GetNextPathEdge( int iTileFrom, int iTileTo) = 0;	///< iTileFrom 에서 iTileTo 로 갈때의 목표 CPathEdge
	virtual int						GetGatewayIndexToConvex( int iConvexFrom, int iConvexTo) = 0;
	virtual CPathEdge*				GetGatewayToConvex( int iConvexFrom, int iConvexTo) = 0;	///< Convex 에서 Convex 로 넘어갈때 경유점
	virtual WzVector				GetGatewayInfoBetweenConvex( int iConvexFrom, int iConvexTo, WzVector *pwvEdge = NULL, Wz2DLine *pw2lLine = NULL) = 0;
	virtual WzVector				GetVisiblePointToNextEdge( WzVector wvCurrentPos, int iEdge, WzVector wvTargetNext) = 0;
	virtual WzVector				GetNearestPointToTarget( int iTile, WzVector wvTarget) = 0;	///< iTile 내의 점들 중에 wvTarget 과 가장 가까운 점을 얻는다.
	virtual WzVector				GetEdgeVertex( int iEdge, int iIndex ) = 0;
	virtual ENUM_MD_RESULT			MoveByDirection( int iCurrentTile, WzVector wvCurrentPos, WzVector wvDistance, int *piResultTile, WzVector *pwvResultPos, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), WzVector *pwvSlide = NULL) = 0;
	virtual void					HeadToNextTile( CPathExplorer *ppe) = 0;	// 다음 타일로 향하도록 하기
	virtual float					GetPathTotalLength(CPathExplorer *ppe) = 0;	
	/// @name --- 결과 얻기
public:
	virtual int						GetNumConvex() = 0;

	/// @name --- 길찾기 매쉬 저장/재생성 관련 함수.
public:
	virtual BOOL					SavePathTiles( CWzArchive *pArchive)=0;
	
	/// 주어진 BoundingVolume안에 위치한 Tile의 목록을 반환한다.
	virtual void					GetTilesInBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult) = 0;
	virtual	void					GetTilesInterSectBoundingVolume( CDrawBase *pDraw, WzBoundingVolume *pBV, CWzArray<DWORD> *arResult) = 0;

	// 검사
public:
	virtual BOOL					CheckTileConnection( WzVector *pwvError) = 0;	// 잘못된 것이 있으면 FALSE
};


#endif // !defined(AFX_PATHFINDERBASE_H__62B6EF9C_2EF9_49EF_9679_FBA93BD8FFA1__INCLUDED_)
