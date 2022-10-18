// pathfinderconvex.h

#ifndef _3DTERRAIN_PATHFINDERCONVEX_H_
#define _3DTERRAIN_PATHFINDERCONVEX_H_

#include "pathfinderastar.h"


class CPathConvex;
class CDrawBase;
class CPathExplorer;


class CPathFinderConvex : public CPathFinderAStar  
{
public:
	CPathFinderConvex();
	virtual ~CPathFinderConvex();
	virtual void					Clear();

	virtual void					Destroy();

	virtual BOOL					Serialize( CWzArchive *pArchive);

	/// @name 지형 생성

	virtual BOOL					GenerateFrom( CWzdBase *pWzd, int iMeshGroup);
	virtual BOOL					Refresh();	///< 길찾기 정보 다시 생성
	/// @name 길 찾기
	virtual ENUM_FINDPATH_RESULT	FindPath( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual ENUM_FINDPATH_RESULT	FindPath_AsConvex( int iConvexStart, int iConvexEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK));	///< Convex 단위의 길찾기만 해서 결과 얻기
	virtual BOOL					FindPath_CheckCollision(CPathExplorer *ppe);//. Find Path 해서 얻어온 경로에 Collision를 다시 체크 하여 충돌이 있음 TRUE 없음 FALSE 리턴.
	virtual BOOL					SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute);


public:
	virtual float					EstimateDistanceBetween( int iNode1, int iNode2);
	//virtual float GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

	/// @name --- Convex 관련
protected:
	int								m_iMaxPathConvex;
	int								m_iNumPathConvex;
	CPathConvex*					m_ppcPathConvex;
	CAStarPathFinder				m_aspfPathFinder;

	int								m_BufferSize;
	int								m_iNumEdge;
	WzVector*						m_pwvBuffer;

	void							MakeSurePathConvexSize( int iSize);
	void							ConstructConvexes();
	int								GetTileListByJam( int *piTiles);
	void							ExpandConvex( CPathConvex *ppcConvex, int iIndexConvex);
	void							ConnectConvexes();	///< Convex 들끼리 연결관계 체크
	WzVector						CalculateConvexCenter( int iConvex );
	float							GetDistance( int iConvex, int iNearConvex, int *iEdgeVertex );

	/// @name --- 길찾기 결과를 이용해서 찾아갈 때
public:
	virtual int						GetGatewayIndexToConvex( int iConvexFrom, int iConvexTo);	// 두 컨벡스 사이의 Edge 번호 얻기
	virtual CPathEdge*				GetGatewayToConvex( int iConvexFrom, int iConvexTo);	// 두 컨벡스 사이의 Edge 얻기
	virtual WzVector				GetGatewayInfoBetweenConvex( int iConvexFrom, int iConvexTo, WzVector *pwvEdge = NULL, Wz2DLine *pw2lLine = NULL);	// 두 컨벡스 사이의 Edge의 여러가지 정보 얻기
	virtual void					HeadToNextTile( CPathExplorer *ppe);	// 다음 타일로 향하도록 하기
	virtual float					GetPathTotalLength(CPathExplorer *ppe);

protected:
	WzVector						GetTargetPosToReachNextTile( CPathExplorer *ppe);	// 다음 타일로 갈때 어느 곳을 향해 가야할지 얻기
	WzVector						GetTargetPosToReachNextTile( int iTile, int iTileNext, WzVector wvPos, WzVector wvTargetPos, int iTileNum, int * piTileIndex);	// 다음 타일로 갈때 어느 곳을 향해 가야할지 얻기
	BOOL							GetEdgeVertexToReachTile( int iTileFrom, int iTileTo, int *piEdgeVertex);	// 다른 타일로 이동할 경우 지나갈 Edge 의 양끝점
	/// @name --- 결과 얻기

public:
	virtual int						GetNumConvex()	{ return ( m_iNumPathConvex); }

	/// @name --- 렌더링/Picking
public:
	virtual void					RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor);
};

#endif // _3DTERRAIN_PATHFINDERCONVEX_H_
