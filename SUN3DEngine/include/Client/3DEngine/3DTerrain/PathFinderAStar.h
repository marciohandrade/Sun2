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

	/// @name ���� ����
	virtual BOOL GenerateFrom( CWzdBase *pWzd, int iMeshGroup);
	virtual BOOL Refresh();	///< ��ã�� ���� �ٽ� ����
	/// @name �� ã��
	virtual ENUM_FINDPATH_RESULT FindPath( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual ENUM_FINDPATH_RESULT FindPath_FullTile( int iTileStart, int iTileEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual BOOL SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute);	///< ��� ����
		

public:
	virtual float EstimateDistanceBetween( int iNode1, int iNode2);
	virtual float GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

	/// @name --- ��ã�� ����� �̿��ؼ� ã�ư� ��
public:
	virtual int GetTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL);	///< 3D ��ǥ �� �� ������ �� ���� Ÿ�� ���
	virtual int GetAllowTileToStand( WzVector wvPos, float *pfT = NULL, float fJumpLimit = -1.0f, float fAdjustLimit = -1.0f, WORD *pwAttribute = NULL,WORD wNotAllowAttribute=PTA_NO_WALK);	///< �־��� �Ӽ��� üũ �Ѵ�
	
	virtual CPathEdge* GetNextPathEdge( int iTileFrom, int iTileTo);	///< iTileFrom ���� iTileTo �� ������ ��ǥ CPathEdge
	virtual WzVector GetVisiblePointToNextEdge( WzVector wvCurrentPos, int iEdge, WzVector wvTargetNext);
	virtual WzVector GetEdgeVertex( int iEdge, int iIndex );

};

#endif // _3DTERRAIN_PATHFINDERASTAR_H_
