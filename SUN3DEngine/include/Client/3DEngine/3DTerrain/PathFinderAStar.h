// pathfinderastar.h

#ifndef _3DTERRAIN_PATHFINDERASTAR_H_
#define _3DTERRAIN_PATHFINDERASTAR_H_

#include "pathfindergenerator.h"
#include "astarpathfinder.h"


class CPathFinderAStar : public CPathFinderGenerator, public CAStarPathFinder  
{
public:
	CPathFinderAStar();
	virtual ~CPathFinderAStar();
	virtual void Clear();

	virtual BOOL Create( COctree *pOctree, CRoomManager *pRoomManager);
	virtual void Destroy();

	virtual BOOL Serialize( CWzArchive *pArchive);

	/// @name 지형 생성
	virtual BOOL GenerateFrom( CWzdBase *pWzd, int iMeshGroup);
	virtual BOOL Refresh();	///< 길찾기 정보 다시 생성
	/// @name 길 찾기
	virtual ENUM_FINDPATH_RESULT FindPath( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual ENUM_FINDPATH_RESULT FindPath_FullTile( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual BOOL SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute);	///< 경로 지정
		

public:
	virtual float EstimateDistanceBetween( int iNode1, int iNode2);
	virtual float GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

	/// @name --- 길찾기 결과를 이용해서 찾아갈 때
public:
	virtual int GetTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL);	///< 3D 좌표 상에 서 있을때 발 딛을 타일 얻기
	virtual int GetAllowTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL,WORD wNotAllowAttribute=PTA_NO_WALK);	///< 주어진 속성도 체크 한다
	
	virtual CPathEdge* GetNextPathEdge( int iTileFrom, int iTileTo);	///< iTileFrom 에서 iTileTo 로 갈때의 목표 CPathEdge
	virtual WzVector GetVisiblePointToNextEdge( WzVector wvCurrentPos, int iEdge, WzVector wvTargetNext);
	virtual WzVector GetEdgeVertex( int iEdge, int iIndex );

};

#endif // _3DTERRAIN_PATHFINDERASTAR_H_
