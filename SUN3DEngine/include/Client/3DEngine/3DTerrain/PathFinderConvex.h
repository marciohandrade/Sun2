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

	/// @name ���� ����

	virtual BOOL					GenerateFrom( CWzdBase *pWzd, int iMeshGroup);
	virtual BOOL					Refresh();	///< ��ã�� ���� �ٽ� ����
	/// @name �� ã��
	virtual ENUM_FINDPATH_RESULT	FindPath( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual ENUM_FINDPATH_RESULT	FindPath_AsConvex( int iConvexStart, int iConvexEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK));	///< Convex ������ ��ã�⸸ �ؼ� ��� ���
	virtual BOOL					FindPath_CheckCollision(CPathExplorer *ppe);//. Find Path �ؼ� ���� ��ο� Collision�� �ٽ� üũ �Ͽ� �浹�� ���� TRUE ���� FALSE ����.
	virtual BOOL					SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute);


public:
	virtual float					EstimateDistanceBetween( int iNode1, int iNode2);
	//virtual float GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

	/// @name --- Convex ����
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
	void							ConnectConvexes();	///< Convex �鳢�� ������� üũ
	WzVector						CalculateConvexCenter( int iConvex );
	float							GetDistance( int iConvex, int iNearConvex, int *iEdgeVertex );

	/// @name --- ��ã�� ����� �̿��ؼ� ã�ư� ��
public:
	virtual int						GetGatewayIndexToConvex( int iConvexFrom, int iConvexTo);	// �� ������ ������ Edge ��ȣ ���
	virtual CPathEdge*				GetGatewayToConvex( int iConvexFrom, int iConvexTo);	// �� ������ ������ Edge ���
	virtual WzVector				GetGatewayInfoBetweenConvex( int iConvexFrom, int iConvexTo, WzVector *pwvEdge = NULL, Wz2DLine *pw2lLine = NULL);	// �� ������ ������ Edge�� �������� ���� ���
	virtual void					HeadToNextTile( CPathExplorer *ppe);	// ���� Ÿ�Ϸ� ���ϵ��� �ϱ�
	virtual float					GetPathTotalLength(CPathExplorer *ppe);

protected:
	WzVector						GetTargetPosToReachNextTile( CPathExplorer *ppe);	// ���� Ÿ�Ϸ� ���� ��� ���� ���� �������� ���
	WzVector						GetTargetPosToReachNextTile( int iTile, int iTileNext, WzVector wvPos, WzVector wvTargetPos, int iTileNum, int * piTileIndex);	// ���� Ÿ�Ϸ� ���� ��� ���� ���� �������� ���
	BOOL							GetEdgeVertexToReachTile( int iTileFrom, int iTileTo, int *piEdgeVertex);	// �ٸ� Ÿ�Ϸ� �̵��� ��� ������ Edge �� �糡��
	/// @name --- ��� ���

public:
	virtual int						GetNumConvex()	{ return ( m_iNumPathConvex); }

	/// @name --- ������/Picking
public:
	virtual void					RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor);
};

#endif // _3DTERRAIN_PATHFINDERCONVEX_H_
