// astarpathfinder.h

#ifndef _3DTERRAIN_ASTARPATHFINDER_H_
#define _3DTERRAIN_ASTARPATHFINDER_H_

#include <programcommon/wzbtree.h>
#include "astarnode.h"


class CRoomManager;


class CAStarPathFinder  
{
public:
	CAStarPathFinder();
	virtual ~CAStarPathFinder();
	virtual void Clear();

	virtual void Create( int iNumNode, CAStarNode *pNodeOutside = NULL, int iNodeSize = sizeof ( CAStarNode));
	virtual void Destroy();

	virtual ENUM_FINDPATH_RESULT FindPath( int iNodeStart, int iNodeEnd, WzVector *wvStartPos, WzVector *wvEndPos, int *piResult, int iMaxResult, int *piNumResult, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND);
	virtual BOOL SetPath( int *pTileRouteDst, int *piNumTileRouteDst, int *pTileRoute, int iNumTileRoute);
	void SetParent( CAStarPathFinder *pParent)	{ m_pParent = pParent; }

protected:
	CAStarPathFinder *m_pParent;
	int m_iNumNode;
private:
	CAStarNode *m_ppnNodes;	// 노드 정보 (상속 받은 곳에서는 CPathNode 를 상속받은 class 를 쓸 것이다. 이런 이유로 m_ppnNodes 직접 접근은 안되고 GetNode 함수를 쓴다.)
	int m_iNodeSize;	// 노드의 크기
	BOOL m_bNodeAllocatedOutside;

	CRoomManager *m_pRoomManagerPtr;
public:
	void SetRoomManager( CRoomManager* pRoomManager)	{ m_pRoomManagerPtr = pRoomManager; }

public:
	virtual float EstimateDistanceBetween( int iNode1, int iNode2);
	virtual float GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

protected:
	BYTE *m_pbyIsNodeClosed;	// 노드가 열렸는지 닫혔는지 상태
	float *m_pfCost;	// 비용
	int *m_piPrevNode;	// 바로 전 노드
	CWzBTree<int, float> m_btOpenNodes;	// 열린 노드

	BOOL m_bUseStartPosition;
	WzVector m_wvStartPos;	// 시작점
	WzVector m_wvEndPos;	// 끝점
	WzVector m_wvTotalDirection;	// nor( m_wvEndPos - m_wvStartPos)

	// 노드 주소
	virtual CAStarNode* GetNode( int iNode)	{ return ( CAStarNode*)( ( ( BYTE*)m_ppnNodes) + iNode * m_iNodeSize); }

	// 길찾기 세부
	float CaluateRealDistance( int iNode1, int iNode2, float fCost );

	void Prepare( int iNodeStart, int iNodeEnd);
	BOOL Iterate( int *iNodeEnd, WORD wAttribute, int iSeekNode);
	int GetNextNodeToTest();
	ENUM_FINDPATH_RESULT GenerateFastestPath( int iNodeStart, int iNodeEnd, int *piResult, int iMaxResult, int *piNumResult);

};

#endif // _3DTERRAIN_ASTARPATHFINDER_H_
