// astarnode.h

#ifndef _3DTERRAIN_ASTARNODE_H_
#define _3DTERRAIN_ASTARNODE_H_

#include <programcommon/wzarray.h>

class CWzArchive;


// �ִ� �˻� ���� ��� ��
#define MAX_NODE_FOR_PATHFIND		( 200)


// ��ã�� ���
enum ENUM_FINDPATH_RESULT
{
	EFR_FAILURE = 0,	// ����
    EFR_SAME_POSITION,  // ������ ��ġ
	EFR_GO_NEAR,		// ��ó�� ��
	EFR_SUCCESS,		// ���� (��������)
};


// �Ӽ�
#define PTA_SAFETY_ZONE				( 0x0001 )	// ��������
#define PTA_ONLY_JUMP				( 0x0002 )	// Jump �� �� �� �ִ� �Ӽ�
#define PTA_NO_WALK					( 0x0004 )	// �ɾ �� ���� �Ӽ�
#define PTA_EXTRA_TILE				( 0x0008 )   // ��Ȳ�� �°� ���Ǵ� �Ӽ�
#define PTA_CONFLICT_TILE			( 0x0010 )   // �������� �Ӽ�
#define PTA_FREE_CONFLICT_TILE		( 0x0020 )   // ������������ �Ӽ�
#define PTA_PK_TILE					( 0x0040 )	// PK ���� �Ӽ�
#define PTA_PLAYER_JUMP_TILE		( 0x0080 )	// Player Jump �Ӽ�
#define PTA_PICK					( 0x0100 )	// Picking ������ Ÿ���� (�ٸ��� ���̰ų� �ϴ� ��� ���)
#define PTA_PLAYER_FALLDOWN_TILE	( 0x0200 )	// �������� �Ӽ� (���� Ÿ���� ����� ���)
#define PTA_DOMINATION_WAR_TILE		( 0x0400 )	// ���������� 
#define PTA_JUMP_CONTROL_A          ( 0x0800 )	// �����Ұ� ����A
#define PTA_JUMP_CONTROL_B          ( 0x1000 )	// �����Ұ� ����B


// �ֺ� Node ����
typedef struct St_PathNearNodeInfo
{
	int m_iNearNode;
	float m_fCost;
} St_PathNearNodeInfo;


// ��ã��� ��� class - Tile, Convex ��� ���� ���� �� �ִ� ���� �̰��� ��ӹ޴´�.

class CAStarNode  
{
public:
	CAStarNode();
	virtual ~CAStarNode();
	virtual void Clear();

	virtual BOOL Serialize( CWzArchive *pArchive);

public:
	virtual void SetAttribute( WORD wAttribute)		{ WzAssert( !"x_pPathFinder ���� �Լ� �̿�"); }//m_wAttribute |= wAttribute; }
	virtual void UnsetAttribute( WORD wAttribute)	{ WzAssert( !"x_pPathFinder ���� �Լ� �̿�"); }//{ m_wAttribute &= ~wAttribute; }
	virtual BOOL CheckAttribute( WORD wAttribute)	{ WzAssert( !"x_pPathFinder ���� �Լ� �̿�"); return FALSE; }//{ return ( 0 != ( m_wAttribute & wAttribute)); }
	virtual WORD GetAttribute()	{ WzAssert( !"x_pPathFinder ���� �Լ� �̿�"); return ( 0); }//{ return ( m_wAttribute); }

	// �ֺ� ���
protected:
	CWzArray<St_PathNearNodeInfo> m_arNearNodes;
public:
	void AddNearNode( int iNearNode, float fCost, int iIndex = -1);

	int GetNumNearNodes()	{ return ( m_arNearNodes.GetCount()); }
	St_PathNearNodeInfo GetNearNodeInfo( int iIndex)	{ return ( m_arNearNodes.Get( iIndex)); }
	int FindNearNode( int iNearNode);	// �ش� ��尡 ���° �ε����� �ִ��� ��´�.

};

#endif // _3DTERRAIN_ASTARNODE_H_
