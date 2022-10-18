// managerrendercommand.h

#ifndef _3DRENDERER_MANAGERRENDERCOMMAND_H_
#define _3DRENDERER_MANAGERRENDERCOMMAND_H_

#include "managerbase.h"
#include "rendercommand.h"


/*-----------------------------------------------------------------------------
// RenderCommand ����
//-----------------------------------------------------------------------------
��� ����:

>�켱���� ����

��) RenderCommand::SetSortPriority(0, ESP_DEPTH);
	RenderCommand::SetSortPriority(1, ESP_BUFFER);


>�߰��ϱ�

1) St_RenderCommand* pRC = AllocRender() �߰����Ѽ� �ش� �����͸� ��´�.
2) pRC->... ���� ä���ִ´�.
3) AddRender( pRC)�� ȣ���Ͽ� �߰���Ų��.
1) - 3) �ݺ�


>�׸���

1) SortRender() ȣ���ؼ� ������ ��Ʈ��Ų��.
2) GetNumberOfRender()��ŭ�� �ε����� ������ GetRender(�ε���)�� ���� �����͸� 
  �� New_RealizeRenderCommmand�� ȣ���Ѵ�.
3) GetNumberOfAlphaRender()��ŭ�� �ε����� ������ GetAlphaRender(�ε���)��
  ���� �����͸� �� New_RealizeRenderCommmand�� ȣ���Ѵ�.
4) ClearRender()�� ȣ���ؼ� ���� ������ ��� �����.
--------------------------------------------------------------------------------*/
class CManagerRenderCommand : public CManagerBase
{
public:
	///< ������/�Ҹ���
	CManagerRenderCommand();
	virtual ~CManagerRenderCommand();


	///< CManagerBase�� �⺻ �Լ�
	virtual void Clear();
	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

public:
	///< ���̿� ���� ���� ��Ʈ�� �����ϰ� ��Ʈ�� ����� �� �����Ѵ�, default: TRUE
	virtual void SetSort( BOOL bUse) { m_bUseSort = bUse; }
	virtual BOOL IsSort() const { return m_bUseSort; }

	///< ���� ������Ʈ�� ��Ʈ�� ���� �ʵ��� ����, default : TRUE;
	virtual void SetAlphaSort( BOOL bUse) { m_bUseAlphaSort = bUse; }
	virtual BOOL IsAlphaSort() const { return m_bUseAlphaSort; }


	///< ���� �켱 ������ �����Ѵ�.

	enum { MAX_NUM_PRIORITY = NUM_ESP-1 };

	// RenderCommand ������ ��Ʈ�ϴµ� �켱 ������ �����Ѵ�.
	virtual void SetSortPriority( int iPriority, ENUM_SORT_PRIORITY eSp);
	virtual ENUM_SORT_PRIORITY GetSortPriority( int iPriority);


	
public:
	///< RenderCommand Manager �Լ� -------------------------------------------------


	///< RenderCommand�� ����� �Ҵ� �Լ�
	St_RenderCommand* AllocRender();
	St_TriRenderCommand* AllocTriRender( St_TriRenderCommand::ENUM_TRI_RENDERCOMMAND_TYPE eTriType);
	St_SpriteRenderCommand* AllocSpriteRender();
	St_NewSpriteRenderCommand* AllocNewSpriteRender();

	St_InstancingRenderCommand* AllocInstancingRender();
	St_InstancingInfo* AllocInstancingInfo( St_InstancingRenderCommand* pIRC);

public:
	///< �⺻ ������ �߰� �Լ�
	virtual void AddRender( St_BaseRenderCommand* pBaseRC);

	///< �ɼǿ� ���� ������ ��Ʈ�ϴ� �Լ�
	virtual void SortRender();


	///< �߰��� ������ ������ 0 ī��Ʈ ��Ų��
	virtual void ClearRender();


	///< �߰��� ������ ������ ��ų� �� ������ ��� �Լ�

	// �ϳ��� ������ ������ �ִ��� ����
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
	//< �ɹ� ������ ----------------------------------------------------------

	BOOL m_bUseSort;
	BOOL m_bUseAlphaSort;
	
	///< �޸� �Ҵ� ���� ������
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

	///< �з��� ������ ������
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

	///< Instancing �� RenderCommand �߰�
protected:
	void AddRCForInstancing( St_RenderCommand* pRC, BOOL bAlphaRender );

	///< Ư���� ���� ������  
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