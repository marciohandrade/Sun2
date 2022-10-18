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


/// @name 옥트리 상의 위치 ( 앞의 3 바이트 : 깊이, 29 바이트 : 인덱스)
typedef unsigned long WzOctIndex;

#define GetDepth_WzOctIndex( woi)	( ( woi) >> 29)
#define GetIndex_WzOctIndex( woi)	( ( woi) & 0x1FFFFFFF)
#define Make_WzOctIndex( iDepth, iIndex)	( (((iDepth) & 0x7) << 29) | ((iIndex) & 0x1FFFFFFF))
#define INVALID_WZOCTINDEX	( 0xFFFFFFFF)

#define Child_WzOctIndex( woiParent, iChildIndex)	Make_WzOctIndex( GetDepth_WzOctIndex( woiParent) + 1, ( GetIndex_WzOctIndex( woiParent) << 3) | ( iChildIndex))
#define Parent_WzOctIndex( woi)		Make_WzOctIndex( GetDepth_WzOctIndex( woi) - 1, GetIndex_WzOctIndex( woi) >> 3)
#define GetChild_ToReach( woiCurrent, woiDest)		( 0x7 & ( GetIndex_WzOctIndex( woiDest) >> ( 3 * ( GetDepth_WzOctIndex( woiDest) - GetDepth_WzOctIndex( woiCurrent) - 1))))

/// 깊이는 0 부터 아래 값까지 - 총 개수는 8^0 + 8^1 + ... + 8^d, 최대 깊이가 6 일 경우 300000 개 가량의 노드 저장 공간 필요 = 1.2M
#define MAX_DEPTH		( 6)



// 옥트리수정
typedef struct
{
	DWORD m_dwType;
	DWORD m_dwObjectInfo;
} St_OctObject;

#define ANODEOBJSLIST	CWzList<St_OctObject>

/// 들어갈 데이터의 타입
enum ENUM_DATATYPE_OCTNODE
{
	DTO_TO_GET_OCTNODES = -1,	///< Enumerate 함수에서 OctNode 리스트를 얻을 때 사용한다.
	DTO_TERRAIN_MESH = 0,		///< 지형 메쉬
	DTO_PATH_MESH,				///< 길찾기용 메쉬
	DTO_OBJECT,					///< 장식물 객체 (살아 움직이지 않는 모든 것)
	DTO_UNIT,					///< 유닛 (살아 움직이는 것들만)	
	DTO_LIGHT,					///< 라이트
	DTO_EFFECT,					///< 이펙트
	DTO_SOUND,					///< 사운드 영역 지정 볼륨 객체
	DTO_SIMPLEOCCLUSION,		///< Occlusion 평면 객체
	DTO_ENVIRONMENTOBJECT,		///< 특정 지역에 대한 환경 정보 객체
	DTO_CNT,

	DTO_NODE_HAS_TERRAIN_MESH = 100,
	DTO_USER = 1000,		///< 해당 번호 다음부터는 직접 선언해서 사용한다
};

/// Occlusion 처리를 위한 부분
typedef struct
{
	float m_fLeft, m_fRight;				///< 영역
	float m_fCenter;						///< 수평 중심
	float m_fMin, m_fMax;					///< 최대/최소
	int m_iEdge;							///< 해당 Edge
} St_HorizonEdge;

/// 노드의 텍스쳐별 메쉬 인덱스 정보 구조체
typedef struct St_MeshChunk
{
	BYTE			m_iTexture;				///< 텍스쳐 인덱스
	bool			m_bBlended;				///<
	CWzArray< int >	m_arVertexMap;

} St_MeshChunk;


/// 상태

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

	/// @name --- 데이터 포함에 대한 부분
protected:
	int m_iTotalObjectCount;	///< 총 리스트에 포함된 Object 개수
	
	ANODEOBJSLIST			*m_arNodeObj;

	void		ConstructNodeTable();
	void		DestructNodeTable();
public:
	ANODEOBJSLIST	*GetObjectList(const ENUM_DATATYPE_OCTNODE dto);

public:
	int GetTotalObjectCount()	{ return ( m_iTotalObjectCount); }

	/// @name --- 옥트리 구조에 대한 부분
protected:
	COctree				*m_pOctree;	///< 포함되어 있는 옥트리
	WzBoundingVolume	m_BoundingVolume; ///< 영역 : AABB 형태 값.
	WzOctIndex			m_woiIndex; ///< 옥트리에서의 인덱스
	int					m_iNumChild; ///< 하위 노드 개수
	WzOctIndex			m_woiChild[8]; ///< 하위 노드 8 개 [z*4 + y*2 + x]

protected:
	void SetChild( int x, int y, int z, WzOctIndex woiChild)	{ m_woiChild[(z << 2) | (y << 1) | x] = woiChild; }
	WzOctIndex GetChild( int x, int y, int z)	{ return ( m_woiChild[(z << 2) | (y << 1) | x]); }
public:
	int GetNumChild()	{ return ( m_iNumChild); }
	COctNode* GetChildNode( int i);
	BOOL CreateChild( int iChild);
	void DestroyChild( int iChild);
	void DestroyAllChild();
	/// @name 객체 추가/삭제 - Add 의 경우 Child 를 만들어가는 기능이 부가적으로 있다.
public:
	WzBoundingVolume GetBoundingVolume()	{ return ( m_BoundingVolume); }
	WzOctIndex GetOctIndex()	{ return ( m_woiIndex); }
	void AddObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, WzOctIndex woiDest = INVALID_WZOCTINDEX);
	BOOL RemoveObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject);
	BOOL IsContainable( const WzBoundingVolume *pwbv);

	int GetData(ENUM_DATATYPE_OCTNODE dto, CWzArray<DWORD> *pArray);

	/// @name 검색하기
public:
    virtual int Enumerate_IntersectRay_MultiType( std::vector<ENUM_DATATYPE_OCTNODE>& type_array, WzRay* ray_ptr, CWzArray<DWORD>* result_ptr );
	virtual int Enumerate_IntersectRay( ENUM_DATATYPE_OCTNODE dto, WzRay *pwr, CWzArray<DWORD> *parResult);
	virtual int Enumerate_IntersectBoundingVolume( ENUM_DATATYPE_OCTNODE dto, WzBoundingVolume *pwbv, CWzArray<DWORD> *parResult, BOOL bContain = FALSE);	///< BV와 교차하는 것 얻기
	virtual int Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, WzFrustum *pwf, CWzArray<DWORD> *parResult, BOOL bContain = FALSE);	///< Frustum 과 교차하는 것 얻기
	virtual int Enumerate_All( ENUM_DATATYPE_OCTNODE dto, CWzArray<WzOctIndex> *parResult);

	virtual int GetObjectList( ENUM_DATATYPE_OCTNODE dto, CWzArray<DWORD> *parResult);	///< 현재 노드에 있는 해당 타입의 Object 모두 얻기

protected:
	int TestHorizonEdge( CWzBinTree<St_HorizonEdge> *pbitHorizonEdge, WzVector *pwvRight, WzVector *pwvUp);	///< 0 - 가려짐, 1 - 약간 가려짐, 2 - 완전히 보임
	int TestHorizonEdgeInside( CWzBinTree<St_HorizonEdge> *pbitHorizonEdge, float fLeft, float fRight, float fVerMin, float fVerMax);


	/// @name 그리기
public:
	virtual void RenderRange( CDrawBase *pDraw, WzColor wcRender = WzColor_RGB( 255, 0, 0));


	/** @addtogroup the occlusion culling
	 *
	 *	Occlusion 컬링을 지원하기 위한 추가 정보 부분
	 *	@{
	 */
protected:
	BOOL							m_bTerrainNode; ///< 현재 노드가 지형 메쉬를 포함하는 노드인가?

public:
	/** 지형 메쉬를 포함한 노드인가? */
	BOOL	IsTMNode() { return m_bTerrainNode; }
	/** 옥트리 노드의 지형에 의한 부피 면적을 설정한다. */
	void	SetOcclusionBound(WzVector &wvMin, WzVector &wvMax);
	void	SetOcclusionBound(WzBoundingVolume &aabb, WzBoundingVolume&sphere, BOOL bTerrain);
	/** Frustum안에 들어오는 옥트리 노드를 선별한다. */	
	int		CullNode(CDrawBase *pDraw, WzFrustum *pwf, WzVector *pwvViewer, BOOL bContain);

public:
	CWzArray< St_MeshChunk* >		m_arMesh;	///< 노드의 지형 정보를 하나의 묶음으로 가지고 있게한다.

	WzBoundingVolume				m_BSphere;	///< 경계 구
	WzBoundingVolume				m_OBSphere; ///< 차페 구
	WzBoundingVolume				m_OBAABB;	///< 차페 AABB

	/** Node에 대한 지형 메쉬의 정보를 담는다. */
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
