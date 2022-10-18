// octree.h

#ifndef _3DTERRAIN_OCTTREE_H_
#define _3DTERRAIN_OCTTREE_H_

#include "octnode.h"
#include <programcommon/wzhashtable.h>

// 코딩의 편의를 위해 정의한 데이터 타입
typedef CWzHashTable< COctNode*, WzOctIndex > WzHashQctNode;
typedef CWzArray< COctNode* >				  WzAryQctNode;
typedef CWzArray< DWORD >					  WzAryDWORD;

class COctree  
{
public:
	COctree();
	virtual ~COctree();
	virtual void Clear();

public:
	BOOL Create( BvAABB aabbRegion);
	void Destroy();

	/// @name --- 옥트리 구조
protected:
	CWzHashTable<COctNode*, WzOctIndex> m_htOctNodes;	///< 옥트리 노드들을 저장

	int m_iCountNode;	///< 전체 노드 개수
public:
	int	GetNodeCount(){return m_iCountNode;}
	WzOctIndex CreateOctNode( WzOctIndex woiParent, int iChildIndex, BvAABB aabbNew);
	void DestroyOctNode( WzOctIndex woiIndex);

	COctNode* GetOctNode( WzOctIndex woiIndex)	{ return ( m_htOctNodes.Get( woiIndex + 1)); }	// +1 하는 이유는 Root node 의 0 을 잘못된 값으로 인식하기 때문
	COctNode* GetRootNode()	{ return ( GetOctNode( 0)); }

	/// @name --- Object 추가/삭제/포함여부 관련
public:
	virtual WzOctIndex GetOctIndexToAddObject( const WzBoundingVolume *pwbv, WzOctIndex woiFrom = 0);
	virtual WzOctIndex GetOctIndexToSearch( WzOctIndex woi);
	virtual WzOctIndex AddObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, WzOctIndex woiIndex);
	virtual WzOctIndex AddObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, const WzBoundingVolume *pwbv);
	virtual BOOL RemoveObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, WzOctIndex woiIndex);
	virtual BOOL RemoveObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, const WzBoundingVolume *pwbv);
	virtual BOOL IsContainable( WzOctIndex woiIndex, const WzBoundingVolume *pwbv);

	/// @name --- 비어 있는 Node 삭제하기
protected:
	CWzList<WzOctIndex> m_lstNodeDelete;
public:
	virtual void InsertNodeDeleteList( WzOctIndex woi);	///< 제거될 노드에 추가
	virtual void DeleteNodeInDeleteList();	///< 제거될 노드들 제거 Enumerate_IntersectFrustum 에서 자동호출됨

	/// @name --- 검색하기
public:
	virtual CWzArray<DWORD>* New_ArrayToEnumerate();	///< 외부에서 만들어진 CWzArray 는 아래의 Enumerate_ 시리즈에서 쓸 수 없고 이 함수로 만들어야 한다.
	virtual void Delete_ArrayToEnumerate( CWzArray<DWORD> *parDelete);

    virtual int Enumerate_IntersectRay_MultiType( std::vector<ENUM_DATATYPE_OCTNODE>& type_array, WzRay* ray_ptr, CWzArray<DWORD>* result_ptr );
	virtual int Enumerate_IntersectRay( ENUM_DATATYPE_OCTNODE dto, WzRay *pwr, CWzArray<DWORD> *parResult);	///< 광선과 교차하는 것 얻기
	virtual int Enumerate_IntersectBoundingVolume( ENUM_DATATYPE_OCTNODE dto, WzBoundingVolume *pwbv, CWzArray<DWORD> *parResult);	///< BV와 교차하는 것 얻기
	virtual int Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, WzFrustum *pwf, CWzArray<DWORD> *parResult);	///< Frustum 과 교차하는 것 얻기
	virtual int Enumerate_All( ENUM_DATATYPE_OCTNODE dto, CWzArray<WzOctIndex> *parResult);	// 모든 노드 얻기

	virtual void RemoveAllEnumeratedList( CWzArray<DWORD> *parResult);	///< 외부에서 Enumerate 한 List 의 경우 RemoveAll 도 DLL 에서 해야 한다.


	/** @addtogroup the occlusion culling
	 *
	 *	Occlusion culling 기능을 이용하기 위한 코드.
	 *	@{
	 */	
public:
	void			AddVisibleNode(COctNode* data)		{ m_aryVisibleNode.Add(data); }
	void			AddVisibleTMNode(COctNode* data)	{ m_aryVisibleTMNode.Add(data); }
	void			AddVisibleObject(DWORD data)		{ m_aryVisibleObj.Add(data); }
	void			AddVisibleOcclusion(DWORD data)		{ m_aryVisibleOcc.Add(data); }
	void			AddVisibleTerrain(DWORD data)		{ m_aryVisibleTMesh.Add(data); }
	void			AddVisibleSound(DWORD data)			{ m_aryVisibleSnd.Add(data); }
	void			AddVisibleUnit(DWORD data)			{ m_aryVisibleUnit.Add(data); }

	virtual int		GetNumVisibleNode()					{ return m_aryVisibleNode.GetCount(); }
	virtual int		GetNumVisibleTMNode()				{ return m_aryVisibleTMNode.GetCount(); }
	virtual int		GetNumVisibleObject()				{ return m_aryVisibleObj.GetCount(); }
	virtual int		GetNumVisibleOcclusion()			{ return m_aryVisibleOcc.GetCount(); }
	virtual int		GetNumVisibleTerrain()				{ return m_aryVisibleTMesh.GetCount(); }
	virtual int		GetNumVisibleSound()				{ return m_aryVisibleSnd.GetCount(); }
	virtual int		GetNumVisibleUnit()					{ return m_aryVisibleUnit.GetCount(); }
	virtual float	GetRangeForOcclusion()				{ return m_fRangeForOcclusion; }

	virtual COctNode *GetVisibleNode(int index)			{ return m_aryVisibleNode.Get(index); }
	virtual COctNode *GetVisibleTMNode(int index)		{ return m_aryVisibleTMNode.Get(index); }
	virtual DWORD	GetVisibleObject(int index)			{ return m_aryVisibleObj.Get(index); }
	virtual DWORD	GetVisibleOcclusion(int index)		{ return m_aryVisibleOcc.Get(index); }
	virtual DWORD	GetVisibleTerrain(int index)		{ return m_aryVisibleTMesh.Get(index); }
	virtual DWORD	GetVisibleSound(int index)			{ return m_aryVisibleSnd.Get(index); }
	virtual DWORD	GetVisibleUnit(int index)			{ return m_aryVisibleUnit.Get(index); }
	
	/** Occlusion의 유효 거리값을 정한다. */
	virtual void	SetRangeForOcclusion(float range)	{ m_fRangeForOcclusion = range; }
	/** Frustum과 교차하는 것 얻기. */
	virtual int		CullNode( CDrawBase *pDraw, WzFrustum *pwf, WzVector *pwvViewer);


protected:
	int				m_nNumVisibleNode;	///< 가시적인 노드의 갯수
	float			m_fRangeForOcclusion; ///< Occlusion시 유효한 occluder의 범위값

public:
	WzAryQctNode	m_aryVisibleNode;	///< Frustum안에 있는 노드 목록
	WzAryQctNode	m_aryVisibleTMNode;	///< Frustum안에 있는 지형을 포함한 노드 목록
	WzAryDWORD		m_aryVisibleOcc;	///< Frustum안에 있는 Occlusion 평면 목록
	WzAryDWORD		m_aryVisibleTMesh;	///< Frustum안에 있는 지형 목록
	WzAryDWORD		m_aryVisibleObj;	///< 보Frustum안에 있는 오브젝트 목록	
	WzAryDWORD		m_aryVisibleSnd;	///< Frustum안에 있는 사운드 영역 목록
	WzAryDWORD		m_aryVisibleUnit;	///< Frustum안에 있는 유닛 목록

	/**	@}	*/ // end of the occlusion culling
};

#endif // _3DTERRAIN_OCTTREE_H_
