#ifndef	__MANAGER_MORPH_MESH__H
#define __MANAGER_MORPH_MESH__H
#pragma once
#include "ManagerBase.h"
class CManagerMorphMesh;
class CWzMorphMesh;
#include "DrawBase.h"
#ifdef ENABLE_PRESERVED_MORPH


// WzUnitDraw당 할당되는 모핑용 컨테이너 
struct St_MorphContainer
{
	CWzMorphMesh* pMorph;
	CWzdBase* pWzdTarget;
};


class CManagerMorphMesh : 
	public CManagerBase, 
	public CHandleManager<St_MorphContainer*>
{
public:
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

	// WzUnitDraw에 모핑을 사용하고자 할때
	virtual BOOL AttachMorph( CWzUnitDraw* pUnitDraw) = 0;
	virtual BOOL DetachMorph( CWzUnitDraw* pUnitDraw) = 0;
	virtual BOOL PreserveMorph( CWzUnitDraw* pUnitDraw) = 0;

	virtual HANDLE GetMorphVertexBuffer( HANDLE hHandle, int iMeshGroup);
	virtual CWzMorphMesh* GetMorph( HANDLE hHandle);
	virtual BOOL DestroyAllMorph();

protected:
	virtual BOOL DestroyMorphPtr( St_MorphContainer *pmc) = 0;
};


class CManagerMorphMeshD3D :
	public CManagerMorphMesh, 
	public CManagerD3D
{
public:
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice( void);
	virtual void OnResetDevice( void);

	virtual BOOL AttachMorph( CWzUnitDraw* pUnitDraw);
	virtual BOOL DetachMorph( CWzUnitDraw* pUnitDraw);
	virtual BOOL PreserveMorph( CWzUnitDraw* pUnitDraw);

protected:
	virtual HANDLE AddMorph( CWzUnit* pWzUnit);
	virtual BOOL DestroyMorph( HANDLE hHandle);
	virtual BOOL DestroyMorphPtr( St_MorphContainer *pmc);
};






// 
class CWzMorphMesh
{	
public:
	CWzMorphMesh();
	virtual ~CWzMorphMesh();
	virtual void Clear();

	virtual void Destroy();
	virtual BOOL Create( int iMeshGroup);
	
	void SetOwnVertexBuffer( int iMeshGroup, HANDLE hOwnVertexBuffer);
	HANDLE GetOwnVertexBuffer( int iMeshGroup) const { return m_ahOwnVertexBuffer[iMeshGroup]; }
	int GetNumberOfGroups() const { return m_iNumGroup; }

protected:
	HANDLE * m_ahOwnVertexBuffer;
	int m_iNumGroup;
};


#endif // ENABLE_PRESERVED_MORPH

#endif // __MANAGER_MORPH_MESH__H