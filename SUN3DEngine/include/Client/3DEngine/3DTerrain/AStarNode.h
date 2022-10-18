// astarnode.h

#ifndef _3DTERRAIN_ASTARNODE_H_
#define _3DTERRAIN_ASTARNODE_H_

#include <programcommon/wzarray.h>

class CWzArchive;


// 최대 검색 가능 노드 수
#define MAX_NODE_FOR_PATHFIND		( 200)


// 길찾기 결과
enum ENUM_FINDPATH_RESULT
{
	EFR_FAILURE = 0,	// 실패
    EFR_SAME_POSITION,  // 동일한 위치
	EFR_GO_NEAR,		// 근처로 감
	EFR_SUCCESS,		// 성공 (목적지로)
};


// 속성
#define PTA_SAFETY_ZONE				( 0x0001 )	// 안전지대
#define PTA_ONLY_JUMP				( 0x0002 )	// Jump 로 갈 수 있는 속성
#define PTA_NO_WALK					( 0x0004 )	// 걸어갈 수 없는 속성
#define PTA_EXTRA_TILE				( 0x0008 )   // 상황에 맞게 사용되는 속성
#define PTA_CONFLICT_TILE			( 0x0010 )   // 분쟁지역 속성
#define PTA_FREE_CONFLICT_TILE		( 0x0020 )   // 자유분쟁지역 속성
#define PTA_PK_TILE					( 0x0040 )	// PK 지역 속성
#define PTA_PLAYER_JUMP_TILE		( 0x0080 )	// Player Jump 속성
#define PTA_PICK					( 0x0100 )	// Picking 가능한 타일임 (다리가 놓이거나 하는 경우 사용)
#define PTA_PLAYER_FALLDOWN_TILE	( 0x0200 )	// 떨어지는 속성 (현재 타락한 사원만 사용)
#define PTA_DOMINATION_WAR_TILE		( 0x0400 )	// 지역점령전 
#define PTA_JUMP_CONTROL_A          ( 0x0800 )	// 점프불가 제어A
#define PTA_JUMP_CONTROL_B          ( 0x1000 )	// 점프불가 제어B


// 주변 Node 정보
typedef struct St_PathNearNodeInfo
{
	int m_iNearNode;
	float m_fCost;
} St_PathNearNodeInfo;


// 길찾기용 노드 class - Tile, Convex 등등 노드로 쓰일 수 있는 것은 이것을 상속받는다.

class CAStarNode  
{
public:
	CAStarNode();
	virtual ~CAStarNode();
	virtual void Clear();

	virtual BOOL Serialize( CWzArchive *pArchive);

public:
	virtual void SetAttribute( WORD wAttribute)		{ WzAssert( !"x_pPathFinder 상의 함수 이용"); }//m_wAttribute |= wAttribute; }
	virtual void UnsetAttribute( WORD wAttribute)	{ WzAssert( !"x_pPathFinder 상의 함수 이용"); }//{ m_wAttribute &= ~wAttribute; }
	virtual BOOL CheckAttribute( WORD wAttribute)	{ WzAssert( !"x_pPathFinder 상의 함수 이용"); return FALSE; }//{ return ( 0 != ( m_wAttribute & wAttribute)); }
	virtual WORD GetAttribute()	{ WzAssert( !"x_pPathFinder 상의 함수 이용"); return ( 0); }//{ return ( m_wAttribute); }

	// 주변 노드
protected:
	CWzArray<St_PathNearNodeInfo> m_arNearNodes;
public:
	void AddNearNode( int iNearNode, float fCost, int iIndex = -1);

	int GetNumNearNodes()	{ return ( m_arNearNodes.GetCount()); }
	St_PathNearNodeInfo GetNearNodeInfo( int iIndex)	{ return ( m_arNearNodes.Get( iIndex)); }
	int FindNearNode( int iNearNode);	// 해당 노드가 몇번째 인덱스에 있는지 얻는다.

};

#endif // _3DTERRAIN_ASTARNODE_H_
