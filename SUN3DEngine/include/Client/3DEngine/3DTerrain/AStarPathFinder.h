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
	CAStarNode *m_ppnNodes;	// ��� ���� (��� ���� �������� CPathNode �� ��ӹ��� class �� �� ���̴�. �̷� ������ m_ppnNodes ���� ������ �ȵǰ� GetNode �Լ��� ����.)
	int m_iNodeSize;	// ����� ũ��
	BOOL m_bNodeAllocatedOutside;

	CRoomManager *m_pRoomManagerPtr;
public:
	void SetRoomManager( CRoomManager* pRoomManager)	{ m_pRoomManagerPtr = pRoomManager; }

public:
	virtual float EstimateDistanceBetween( int iNode1, int iNode2);
	virtual float GetRealDistance( int iNode1, int iNode2, WzVector *pwvPos1, WzVector *pwvPos2, WzVector *pwvDir);

protected:
	BYTE *m_pbyIsNodeClosed;	// ��尡 ���ȴ��� �������� ����
	float *m_pfCost;	// ���
	int *m_piPrevNode;	// �ٷ� �� ���
	CWzBTree<int, float> m_btOpenNodes;	// ���� ���

	BOOL m_bUseStartPosition;
	WzVector m_wvStartPos;	// ������
	WzVector m_wvEndPos;	// ����
	WzVector m_wvTotalDirection;	// nor( m_wvEndPos - m_wvStartPos)

	// ��� �ּ�
	virtual CAStarNode* GetNode( int iNode)	{ return ( CAStarNode*)( ( ( BYTE*)m_ppnNodes) + iNode * m_iNodeSize); }

	// ��ã�� ����
	float CaluateRealDistance( int iNode1, int iNode2, float fCost );

	void Prepare( int iNodeStart, int iNodeEnd);
	BOOL Iterate( int *iNodeEnd, WORD wAttribute, int iSeekNode);
	int GetNextNodeToTest();
	ENUM_FINDPATH_RESULT GenerateFastestPath( int iNodeStart, int iNodeEnd, int *piResult, int iMaxResult, int *piNumResult);

};

#endif // _3DTERRAIN_ASTARPATHFINDER_H_
