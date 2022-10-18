// octnode.h

#ifndef _3DTERRAIN_OCTNODE_H_
#define _3DTERRAIN_OCTNODE_H_

#include <programcommon/wzarray.h>
#include <programcommon/wzlist.h>
#include <programcommon/wzbintree.h>
#include <client/3dengine/common/wzcolor.h>

#include <map>
#include <vector>

class COctree;
class CDrawBase;


/// @name ��Ʈ�� ���� ��ġ ( ���� 3 ����Ʈ : ����, 29 ����Ʈ : �ε���)
typedef unsigned long WzOctIndex;

#define GetDepth_WzOctIndex( woi)	( ( woi) >> 29)
#define GetIndex_WzOctIndex( woi)	( ( woi) & 0x1FFFFFFF)
#define Make_WzOctIndex( iDepth, iIndex)	( (((iDepth) & 0x7) << 29) | ((iIndex) & 0x1FFFFFFF))
#define INVALID_WZOCTINDEX	( 0xFFFFFFFF)

#define Child_WzOctIndex( woiParent, iChildIndex)	Make_WzOctIndex( GetDepth_WzOctIndex( woiParent) + 1, ( GetIndex_WzOctIndex( woiParent) << 3) | ( iChildIndex))
#define Parent_WzOctIndex( woi)		Make_WzOctIndex( GetDepth_WzOctIndex( woi) - 1, GetIndex_WzOctIndex( woi) >> 3)
#define GetChild_ToReach( woiCurrent, woiDest)		( 0x7 & ( GetIndex_WzOctIndex( woiDest) >> ( 3 * ( GetDepth_WzOctIndex( woiDest) - GetDepth_WzOctIndex( woiCurrent) - 1))))

/// ���̴� 0 ���� �Ʒ� ������ - �� ������ 8^0 + 8^1 + ... + 8^d, �ִ� ���̰� 6 �� ��� 300000 �� ������ ��� ���� ���� �ʿ� = 1.2M
#define MAX_DEPTH		( 6)



// ��Ʈ������
typedef struct
{
	DWORD m_dwType;
	DWORD m_dwObjectInfo;
} St_OctObject;

#define ANODEOBJSLIST	CWzList<St_OctObject>

/// �� �������� Ÿ��
enum ENUM_DATATYPE_OCTNODE
{
	DTO_TO_GET_OCTNODES = -1,	///< Enumerate �Լ����� OctNode ����Ʈ�� ���� �� ����Ѵ�.
	DTO_TERRAIN_MESH = 0,		///< ���� �޽�
	DTO_PATH_MESH,				///< ��ã��� �޽�
	DTO_OBJECT,					///< ��Ĺ� ��ü (��� �������� �ʴ� ��� ��)
	DTO_UNIT,					///< ���� (��� �����̴� �͵鸸)	
	DTO_LIGHT,					///< ����Ʈ
	DTO_EFFECT,					///< ����Ʈ
	DTO_SOUND,					///< ���� ���� ���� ���� ��ü
	DTO_SIMPLEOCCLUSION,		///< Occlusion ��� ��ü
	DTO_ENVIRONMENTOBJECT,		///< Ư�� ������ ���� ȯ�� ���� ��ü
	DTO_CNT,

	DTO_NODE_HAS_TERRAIN_MESH = 100,
	DTO_USER = 1000,		///< �ش� ��ȣ �������ʹ� ���� �����ؼ� ����Ѵ�
};

/// Occlusion ó���� ���� �κ�
typedef struct
{
	float m_fLeft, m_fRight;				///< ����
	float m_fCenter;						///< ���� �߽�
	float m_fMin, m_fMax;					///< �ִ�/�ּ�
	int m_iEdge;							///< �ش� Edge
} St_HorizonEdge;

/// ����� �ؽ��ĺ� �޽� �ε��� ���� ����ü
typedef struct St_MeshChunk
{
	BYTE			m_iTexture;				///< �ؽ��� �ε���
	bool			m_bBlended;				///<
	CWzArray< int >	m_arVertexMap;

} St_MeshChunk;


/// ����

#define ONS_CULLED		( 0x0001)


class COctNode  
{
public:
	COctNode();
	virtual ~COctNode();
	virtual void Clear();

public:
	BOOL Create( COctree *pOctree, WzOctIndex woiIndex, BvAABB aabbRegion);
	void Destroy();

	/// @name --- ������ ���Կ� ���� �κ�
protected:
	int m_iTotalObjectCount;	///< �� ����Ʈ�� ���Ե� Object ����
	
	ANODEOBJSLIST			*m_arNodeObj;

	void		ConstructNodeTable();
	void		DestructNodeTable();
public:
	ANODEOBJSLIST	*GetObjectList(const ENUM_DATATYPE_OCTNODE dto);

public:
	int GetTotalObjectCount()	{ return ( m_iTotalObjectCount); }

	/// @name --- ��Ʈ�� ������ ���� �κ�
protected:
	COctree				*m_pOctree;	///< ���ԵǾ� �ִ� ��Ʈ��
	WzBoundingVolume	m_BoundingVolume; ///< ���� : AABB ���� ��.
	WzOctIndex			m_woiIndex; ///< ��Ʈ�������� �ε���
	int					m_iNumChild; ///< ���� ��� ����
	WzOctIndex			m_woiChild[8]; ///< ���� ��� 8 �� [z*4 + y*2 + x]

protected:
	void SetChild( int x, int y, int z, WzOctIndex woiChild)	{ m_woiChild[(z << 2) | (y << 1) | x] = woiChild; }
	WzOctIndex GetChild( int x, int y, int z)	{ return ( m_woiChild[(z << 2) | (y << 1) | x]); }
public:
	int GetNumChild()	{ return ( m_iNumChild); }
	COctNode* GetChildNode( int i);
	BOOL CreateChild( int iChild);
	void DestroyChild( int iChild);
	void DestroyAllChild();
	/// @name ��ü �߰�/���� - Add �� ��� Child �� ������ ����� �ΰ������� �ִ�.
public:
	WzBoundingVolume GetBoundingVolume()	{ return ( m_BoundingVolume); }
	WzOctIndex GetOctIndex()	{ return ( m_woiIndex); }
	void AddObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, WzOctIndex woiDest = INVALID_WZOCTINDEX);
	BOOL RemoveObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject);
	BOOL IsContainable( const WzBoundingVolume *pwbv);

	int GetData(ENUM_DATATYPE_OCTNODE dto, CWzArray<DWORD> *pArray);

	/// @name �˻��ϱ�
public:
    virtual int Enumerate_IntersectRay_MultiType( std::vector<ENUM_DATATYPE_OCTNODE>& type_array, WzRay* ray_ptr, CWzArray<DWORD>* result_ptr );
	virtual int Enumerate_IntersectRay( ENUM_DATATYPE_OCTNODE dto, WzRay *pwr, CWzArray<DWORD> *parResult);
	virtual int Enumerate_IntersectBoundingVolume( ENUM_DATATYPE_OCTNODE dto, WzBoundingVolume *pwbv, CWzArray<DWORD> *parResult, BOOL bContain = FALSE);	///< BV�� �����ϴ� �� ���
	virtual int Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, WzFrustum *pwf, CWzArray<DWORD> *parResult, BOOL bContain = FALSE);	///< Frustum �� �����ϴ� �� ���
	virtual int Enumerate_All( ENUM_DATATYPE_OCTNODE dto, CWzArray<WzOctIndex> *parResult);

	virtual int GetObjectList( ENUM_DATATYPE_OCTNODE dto, CWzArray<DWORD> *parResult);	///< ���� ��忡 �ִ� �ش� Ÿ���� Object ��� ���

protected:
	int TestHorizonEdge( CWzBinTree<St_HorizonEdge> *pbitHorizonEdge, WzVector *pwvRight, WzVector *pwvUp);	///< 0 - ������, 1 - �ణ ������, 2 - ������ ����
	int TestHorizonEdgeInside( CWzBinTree<St_HorizonEdge> *pbitHorizonEdge, float fLeft, float fRight, float fVerMin, float fVerMax);


	/// @name �׸���
public:
	virtual void RenderRange( CDrawBase *pDraw, WzColor wcRender = WzColor_RGB( 255, 0, 0));


	/** @addtogroup the occlusion culling
	 *
	 *	Occlusion �ø��� �����ϱ� ���� �߰� ���� �κ�
	 *	@{
	 */
protected:
	BOOL							m_bTerrainNode; ///< ���� ��尡 ���� �޽��� �����ϴ� ����ΰ�?

public:
	/** ���� �޽��� ������ ����ΰ�? */
	BOOL	IsTMNode() { return m_bTerrainNode; }
	/** ��Ʈ�� ����� ������ ���� ���� ������ �����Ѵ�. */
	void	SetOcclusionBound(WzVector &wvMin, WzVector &wvMax);
	void	SetOcclusionBound(WzBoundingVolume &aabb, WzBoundingVolume&sphere, BOOL bTerrain);
	/** Frustum�ȿ� ������ ��Ʈ�� ��带 �����Ѵ�. */	
	int		CullNode(CDrawBase *pDraw, WzFrustum *pwf, WzVector *pwvViewer, BOOL bContain);

public:
	CWzArray< St_MeshChunk* >		m_arMesh;	///< ����� ���� ������ �ϳ��� �������� ������ �ְ��Ѵ�.

	WzBoundingVolume				m_BSphere;	///< ��� ��
	WzBoundingVolume				m_OBSphere; ///< ���� ��
	WzBoundingVolume				m_OBAABB;	///< ���� AABB

	/** Node�� ���� ���� �޽��� ������ ��´�. */
	void	AddMesh(BYTE iTexture, int *vertices, int count, const bool blended=false);
	void	ClearMesh();

	/*	@}	*/ // end of the occlusion culling
};

// inline

inline ANODEOBJSLIST *COctNode::GetObjectList( const ENUM_DATATYPE_OCTNODE dto)
{
	ENUM_DATATYPE_OCTNODE	eDto;

	if( dto == DTO_USER ) eDto = DTO_CNT;
	else eDto = dto;

	return &m_arNodeObj[static_cast<int>(eDto)];
};

#endif // _3DTERRAIN_OCTNODE_H_
