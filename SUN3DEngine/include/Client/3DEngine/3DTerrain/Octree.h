// octree.h

#ifndef _3DTERRAIN_OCTTREE_H_
#define _3DTERRAIN_OCTTREE_H_

#include "octnode.h"
#include <programcommon/wzhashtable.h>

// �ڵ��� ���Ǹ� ���� ������ ������ Ÿ��
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

	/// @name --- ��Ʈ�� ����
protected:
	CWzHashTable<COctNode*, WzOctIndex> m_htOctNodes;	///< ��Ʈ�� ������ ����

	int m_iCountNode;	///< ��ü ��� ����
public:
	int	GetNodeCount(){return m_iCountNode;}
	WzOctIndex CreateOctNode( WzOctIndex woiParent, int iChildIndex, BvAABB aabbNew);
	void DestroyOctNode( WzOctIndex woiIndex);

	COctNode* GetOctNode( WzOctIndex woiIndex)	{ return ( m_htOctNodes.Get( woiIndex + 1)); }	// +1 �ϴ� ������ Root node �� 0 �� �߸��� ������ �ν��ϱ� ����
	COctNode* GetRootNode()	{ return ( GetOctNode( 0)); }

	/// @name --- Object �߰�/����/���Կ��� ����
public:
	virtual WzOctIndex GetOctIndexToAddObject( const WzBoundingVolume *pwbv, WzOctIndex woiFrom = 0);
	virtual WzOctIndex GetOctIndexToSearch( WzOctIndex woi);
	virtual WzOctIndex AddObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, WzOctIndex woiIndex);
	virtual WzOctIndex AddObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, const WzBoundingVolume *pwbv);
	virtual BOOL RemoveObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, WzOctIndex woiIndex);
	virtual BOOL RemoveObject( ENUM_DATATYPE_OCTNODE dto, DWORD dwObject, const WzBoundingVolume *pwbv);
	virtual BOOL IsContainable( WzOctIndex woiIndex, const WzBoundingVolume *pwbv);

	/// @name --- ��� �ִ� Node �����ϱ�
protected:
	CWzList<WzOctIndex> m_lstNodeDelete;
public:
	virtual void InsertNodeDeleteList( WzOctIndex woi);	///< ���ŵ� ��忡 �߰�
	virtual void DeleteNodeInDeleteList();	///< ���ŵ� ���� ���� Enumerate_IntersectFrustum ���� �ڵ�ȣ���

	/// @name --- �˻��ϱ�
public:
	virtual CWzArray<DWORD>* New_ArrayToEnumerate();	///< �ܺο��� ������� CWzArray �� �Ʒ��� Enumerate_ �ø���� �� �� ���� �� �Լ��� ������ �Ѵ�.
	virtual void Delete_ArrayToEnumerate( CWzArray<DWORD> *parDelete);

    virtual int Enumerate_IntersectRay_MultiType( std::vector<ENUM_DATATYPE_OCTNODE>& type_array, WzRay* ray_ptr, CWzArray<DWORD>* result_ptr );
	virtual int Enumerate_IntersectRay( ENUM_DATATYPE_OCTNODE dto, WzRay *pwr, CWzArray<DWORD> *parResult);	///< ������ �����ϴ� �� ���
	virtual int Enumerate_IntersectBoundingVolume( ENUM_DATATYPE_OCTNODE dto, WzBoundingVolume *pwbv, CWzArray<DWORD> *parResult);	///< BV�� �����ϴ� �� ���
	virtual int Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, WzFrustum *pwf, CWzArray<DWORD> *parResult);	///< Frustum �� �����ϴ� �� ���
	virtual int Enumerate_All( ENUM_DATATYPE_OCTNODE dto, CWzArray<WzOctIndex> *parResult);	// ��� ��� ���

	virtual void RemoveAllEnumeratedList( CWzArray<DWORD> *parResult);	///< �ܺο��� Enumerate �� List �� ��� RemoveAll �� DLL ���� �ؾ� �Ѵ�.


	/** @addtogroup the occlusion culling
	 *
	 *	Occlusion culling ����� �̿��ϱ� ���� �ڵ�.
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
	
	/** Occlusion�� ��ȿ �Ÿ����� ���Ѵ�. */
	virtual void	SetRangeForOcclusion(float range)	{ m_fRangeForOcclusion = range; }
	/** Frustum�� �����ϴ� �� ���. */
	virtual int		CullNode( CDrawBase *pDraw, WzFrustum *pwf, WzVector *pwvViewer);


protected:
	int				m_nNumVisibleNode;	///< �������� ����� ����
	float			m_fRangeForOcclusion; ///< Occlusion�� ��ȿ�� occluder�� ������

public:
	WzAryQctNode	m_aryVisibleNode;	///< Frustum�ȿ� �ִ� ��� ���
	WzAryQctNode	m_aryVisibleTMNode;	///< Frustum�ȿ� �ִ� ������ ������ ��� ���
	WzAryDWORD		m_aryVisibleOcc;	///< Frustum�ȿ� �ִ� Occlusion ��� ���
	WzAryDWORD		m_aryVisibleTMesh;	///< Frustum�ȿ� �ִ� ���� ���
	WzAryDWORD		m_aryVisibleObj;	///< ��Frustum�ȿ� �ִ� ������Ʈ ���	
	WzAryDWORD		m_aryVisibleSnd;	///< Frustum�ȿ� �ִ� ���� ���� ���
	WzAryDWORD		m_aryVisibleUnit;	///< Frustum�ȿ� �ִ� ���� ���

	/**	@}	*/ // end of the occlusion culling
};

#endif // _3DTERRAIN_OCTTREE_H_
