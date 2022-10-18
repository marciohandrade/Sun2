// managerunitlightmap.h

#ifndef _3DRENDERER_MANAGERUNITLIGHTMAP_H_
#define _3DRENDERER_MANAGERUNITLIGHTMAP_H_

#include "managerbase.h"

class CWzUnit;
class CWzUnitLightMap;

struct St_UnitLightMap
{
	HANDLE		m_hTexDiffuse;
	int			m_iNumVertices;		// Face 수는 m_iNumVertices / 3
	WzUVVert	*m_pUVVerts;

	int			m_iNumObj;			//오브젝트 번호
	int			m_iNumMeshGroup;	//메쉬그룹 번호
	int			*m_FaceTextureNum;	//해당 Face가 쓰는 라이트맵 텍스쳐 번호
};

struct St_TerrainLightMap
{
	int			m_iNumVertices; // Face 수는 m_iNumVertices / 3
	WzUVVert	*m_pUVVerts;
	int			*m_piLightmapIndex;
};


class CManagerUnitLightMap : 
	public CManagerBase, 
	public CWzHandleManager<CWzUnitLightMap*>
{
public:
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

	// WzUnitDraw에 라이트맵을 사용하고자 할때
	/*
		AttachLightMap( unitdraw, count, pUnitUVs[]); or
		AttachLightMap( unitdraw, count); for( i; count; ++i) SetLightMap( unitdraw, &pUnitUVs[i], i);
	*/
	virtual BOOL AttachLightMap( CWzUnitDraw* pUnitDraw, int iGroupNum, const St_UnitLightMap* pUnitUVs = NULL) = 0;
	virtual BOOL DetachLightMap( CWzUnitDraw* pUnitDraw) = 0;
	virtual BOOL SetLightMap( CWzUnitDraw* pUnitDraw, const St_UnitLightMap* pUnitUV, int iGroupIndex) = 0;

	virtual CWzUnitLightMap* GetLightMap( HANDLE hHandle);
	virtual HANDLE GetOwnUVBuffer( HANDLE hHandle, int iMeshGroup);
	virtual HANDLE GetTexture( HANDLE hHandle, int iMeshGroup);

};



#include "managerd3d.h"

class CManagerUnitLightMapD3D :
	public CManagerUnitLightMap, 
	public CManagerD3D
{
public:
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	virtual BOOL AttachLightMap( CWzUnitDraw* pUnitDraw, int iGroupNum, const St_UnitLightMap* pUnitUVs = NULL);
	virtual BOOL DetachLightMap( CWzUnitDraw* pUnitDraw);
	virtual BOOL SetLightMap( CWzUnitDraw* pUnitDraw, const St_UnitLightMap* pUnitUV, int iGroupIndex);

protected:
	virtual BOOL SetLightMap( CWzUnitLightMap* pUnitLightMap, CWzdBase* pWzd, const St_UnitLightMap* pUnitUV, int iGroupIndex);

	virtual HANDLE AddLightMap( CWzUnit* pWzUnit, const St_UnitLightMap* pUnitUVs, int iGroupNum);
	virtual BOOL DestroyLightMap( HANDLE hHandle);
	virtual BOOL DestroyLightMapPtr( CWzUnitLightMap *pulm);
	virtual BOOL UpdateLightMapBuffer( HANDLE hVB, const WzUVVert* pUVs, int iNumVertices);
};




class CWzUnitLightMap
{
public:
	CWzUnitLightMap();
	virtual ~CWzUnitLightMap();
	virtual void Clear();

	virtual void Destroy();
	virtual BOOL Create( int iNumMeshGroup, const St_UnitLightMap* pUnitLightMap);
	
	void SetTexture( int iMeshGroup, HANDLE hTexture);
	void SetOwnUVBuffer( int iMeshGroup, HANDLE hOwnUVBuffer);
	void SetLightMap( const St_UnitLightMap* pUnitLightMap, int iMeshGroupIndex);

	HANDLE GetOwnUVBuffer( int iMeshGroup) const { return m_ahOwnUVBuffer[iMeshGroup]; }
	HANDLE GetTexture( int iMeshGroup) const { return m_aUnitLightMap[iMeshGroup].m_hTexDiffuse; }
	int GetNumberOfGroups() const { return m_iNumGroup; }
	const WzUVVert* GetUVVert( int iMeshGroup) const { return m_aUnitLightMap[iMeshGroup].m_pUVVerts; }
	int GetNumberOfVertices( int iMeshGroup) const { return m_aUnitLightMap[iMeshGroup].m_iNumVertices; }

protected:
	HANDLE * m_ahOwnUVBuffer;
	int m_iNumGroup;
	St_UnitLightMap * m_aUnitLightMap;
};

#endif // _3DRENDERER_MANAGERUNITLIGHTMAP_H_