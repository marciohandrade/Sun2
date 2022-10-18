// managerrendercommand.h

#ifndef _3DRENDERER_MANAGERRENDERCOMMAND_H_
#define _3DRENDERER_MANAGERRENDERCOMMAND_H_

#include "managerbase.h"
#include "rendercommand.h"


/*-----------------------------------------------------------------------------
// RenderCommand 관련
//-----------------------------------------------------------------------------
사용 관련:

>우선순위 설정

예) RenderCommand::SetSortPriority(0, ESP_DEPTH);
	RenderCommand::SetSortPriority(1, ESP_BUFFER);


>추가하기

1) St_RenderCommand* pRC = AllocRender() 추가시켜서 해당 포인터를 얻는다.
2) pRC->... 값을 채워넣는다.
3) AddRender( pRC)를 호출하여 추가시킨다.
1) - 3) 반복


>그리기

1) SortRender() 호출해서 적절히 소트시킨다.
2) GetNumberOfRender()만큼의 인덱스를 가지고 GetRender(인덱스)를 통해 포인터를 
  얻어서 New_RealizeRenderCommmand의 호출한다.
3) GetNumberOfAlphaRender()만큼의 인덱스를 가지고 GetAlphaRender(인덱스)를
  통해 포인터를 얻어서 New_RealizeRenderCommmand의 호출한다.
4) ClearRender()를 호출해서 현재 정보를 모두 지운다.
--------------------------------------------------------------------------------*/
class CManagerRenderCommand : public CManagerBase
{
public:
	///< 생성자/소멸자
	CManagerRenderCommand();
	virtual ~CManagerRenderCommand();


	///< CManagerBase의 기본 함수
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

public:
	///< 깊이에 따른 알파 소트를 제외하고 소트를 사용할 지 설정한다, default: TRUE
	virtual void SetSort( BOOL bUse) { m_bUseSort = bUse; }
	virtual BOOL IsSort() const { return m_bUseSort; }

	///< 알파 오브젝트를 소트를 하지 않도록 설정, default : TRUE;
	virtual void SetAlphaSort( BOOL bUse) { m_bUseAlphaSort = bUse; }
	virtual BOOL IsAlphaSort() const { return m_bUseAlphaSort; }


	///< 소팅 우선 순위를 설정한다.

	enum { MAX_NUM_PRIORITY = NUM_ESP-1 };

	// RenderCommand 정보를 소트하는데 우선 순위를 결정한다.
	virtual void SetSortPriority( int iPriority, ENUM_SORT_PRIORITY eSp);
	virtual ENUM_SORT_PRIORITY GetSortPriority( int iPriority);


	
public:
	///< RenderCommand Manager 함수 -------------------------------------------------


	///< RenderCommand의 빈공간 할당 함수
	St_RenderCommand* AllocRender();
	St_TriRenderCommand* AllocTriRender( St_TriRenderCommand::ENUM_TRI_RENDERCOMMAND_TYPE eTriType);
	St_SpriteRenderCommand* AllocSpriteRender();
	St_NewSpriteRenderCommand* AllocNewSpriteRender();

	St_InstancingRenderCommand* AllocInstancingRender();
	St_InstancingInfo* AllocInstancingInfo( St_InstancingRenderCommand* pIRC);

public:
	///< 기본 렌더링 추가 함수
	virtual void AddRender( St_BaseRenderCommand* pBaseRC);

	///< 옵션에 따라 적절히 소트하는 함수
	virtual void SortRender();


	///< 추가된 렌더링 정보를 0 카운트 시킨다
	virtual void ClearRender();


	///< 추가된 렌더링 정보를 얻거나 그 갯수를 얻는 함수

	// 하나라도 렌더링 정보가 있는지 유무
	BOOL IsRenderCommand();

	int GetNumberOfRender() { return m_arRenderCmd.GetCount(); }
	int GetNumberOfAlphaRender() { return m_arAlphaRenderCmd.GetCount(); }
	int GetNumberOfLastAlphaRender() { return m_arLastAlphaRenderCmd.GetCount(); }
	int GetNumberOfZAlphaRender() { return m_arZAlphaRenderCmd.GetCount(); }
	St_BaseRenderCommand* GetRender( int iIndex) { return m_arRenderCmd.Get( iIndex); }
	St_BaseRenderCommand* GetAlphaRender( int iIndex) { return m_arAlphaRenderCmd.Get( iIndex); }
	St_BaseRenderCommand* GetLastAlphaRender( int iIndex) { return m_arLastAlphaRenderCmd.Get( iIndex); }
	St_BaseRenderCommand* GetZAlphaRender( int iIndex) { return m_arZAlphaRenderCmd.Get( iIndex); }

public:
	int GetNumberOfMotionBlur() { return m_arMotionBlur.GetCount(); }
	int GetNumberOfRefraction() { return m_arRefraction.GetCount(); }
	int GetNumberOfGlow() { return m_arGlow.GetCount(); }
	int GetNumberOfHaze() { return m_arHaze.GetCount(); }
	int GetNumberOfRefractionMask() { return m_arRefractionMask.GetCount(); }
	St_BaseRenderCommand* GetMotionBlur( int iIndex) { return m_arMotionBlur.Get( iIndex); }
	St_BaseRenderCommand* GetRefraction( int iIndex) { return m_arRefraction.Get( iIndex); }
	St_BaseRenderCommand* GetGlow( int iIndex) { return m_arGlow.Get( iIndex); }
	St_BaseRenderCommand* GetHaze( int iIndex) { return m_arHaze.Get( iIndex); }
	St_BaseRenderCommand* GetRefractionMask( int iIndex) { return m_arRefractionMask.Get( iIndex); }

	int GetNumberOfInstancing() { return m_arInstancingRenderCmd.GetCount(); }
	int GetNumberOfInstancingAlpha() { return m_arInstancingAlphaRenderCmd.GetCount(); }
	St_BaseRenderCommand* GetInstancing( int iIndex) { return m_arInstancingRenderCmd.Get( iIndex); }
	St_BaseRenderCommand* GetInstancingAlpha( int iIndex) { return m_arInstancingAlphaRenderCmd.Get( iIndex); }

public:
	int GetNumberOfShadow() { return m_arShadowRenderCmd.GetCount(); }
	St_RenderCommand* GetShadow( int iIndex) { return m_arShadowRenderCmd.Get( iIndex); }
	St_RenderCommand** GetShadowArray() { return m_arShadowRenderCmd.GetBufferPtr(); }

protected:
	void SortByDepth( St_BaseRenderCommand** pArray, int iCount, BOOL bFrontToBack=TRUE) const;
	void SortByBuffer( St_BaseRenderCommand** pArray, int iCount) const;
	void SortByTexture( St_BaseRenderCommand** pArray, int iCount) const;
	void SortByRenderState( St_BaseRenderCommand** pArray, int iCount) const;
	
	static int CompareF( LPCVOID l, LPCVOID r);
	static int CompareDW( LPCVOID l, LPCVOID r);

protected:
	//< 맴버 변수들 ----------------------------------------------------------

	BOOL m_bUseSort;
	BOOL m_bUseAlphaSort;
	
	///< 메모리 할당 관련 변수들
	int m_iAllocRenderCmd;
	int m_iAllocTriRenderCmd;
	int m_iAllocSpriteRenderCmd;
	int m_iAllocNewSpriteRenderCmd;
	int m_iAllocInstancingRenderCmd;

	CWzArray<St_RenderCommand*>	m_arAllocRenderCmd;
	CWzArray<St_TriRenderCommand*> m_arAllocTriRenderCmd;
	CWzArray<St_SpriteRenderCommand*> m_arAllocSpriteRenderCmd;
	CWzArray<St_NewSpriteRenderCommand*> m_arAllocNewSpriteRenderCmd;
	CWzArray<St_InstancingRenderCommand*> m_arAllocInstancingRenderCmd;

	///< 분류된 렌더링 변수들
	CWzArray<St_BaseRenderCommand*> m_arRenderCmd;
	CWzArray<St_BaseRenderCommand*> m_arZAlphaRenderCmd;// Z-Write Disabled
	CWzArray<St_BaseRenderCommand*> m_arAlphaRenderCmd;
	CWzArray<St_BaseRenderCommand*> m_arLastAlphaRenderCmd;
	CWzArray<St_BaseRenderCommand*>	m_arMotionBlur;
	CWzArray<St_BaseRenderCommand*> m_arRefraction;
	CWzArray<St_BaseRenderCommand*> m_arGlow;
	CWzArray<St_BaseRenderCommand*> m_arHaze;
	CWzArray<St_BaseRenderCommand*> m_arRefractionMask;

	CWzArray<St_InstancingRenderCommand*> m_arInstancingRenderCmd;
	CWzArray<St_InstancingRenderCommand*> m_arInstancingAlphaRenderCmd;

	CWzArray<St_RenderCommand*>		m_arShadowRenderCmd;
	ENUM_SORT_PRIORITY m_aiSortPriority[MAX_NUM_PRIORITY];

	///< Instancing 용 RenderCommand 추가
protected:
	void AddRCForInstancing( St_RenderCommand* pRC, BOOL bAlphaRender );

	///< 특수한 지형 렌더링  
public:
	enum ENUM_RENDER_TERRAIN_ETCS
	{
		ERTE_SHADOWMAP	= 0,
		ERTE_WATER,
		ERTE_GLOW,
		ERTE_NUM
	};

	virtual void AddRenderTerrainEtcs( ENUM_RENDER_TERRAIN_ETCS eRender) { m_abRenderTerrainEtcs[eRender] = TRUE; }
	virtual BOOL IsRenderTerrainEtcs( ENUM_RENDER_TERRAIN_ETCS eRender) const { return m_abRenderTerrainEtcs[eRender]; }
protected:
	BOOL IsRenderTerrainEtcs() const;

	BOOL m_abRenderTerrainEtcs[ERTE_NUM];
};



// inline

inline void CManagerRenderCommand::SetSortPriority( int iPriority, ENUM_SORT_PRIORITY eSp)
{
	WzAssert( iPriority < MAX_NUM_PRIORITY);
	m_aiSortPriority[iPriority] = eSp;
}

inline ENUM_SORT_PRIORITY CManagerRenderCommand::GetSortPriority( int iPriority)
{
	WzAssert( iPriority < MAX_NUM_PRIORITY);
	return m_aiSortPriority[iPriority];
}


#endif // _3DRENDERER_MANAGERRENDERCOMMAND_H_