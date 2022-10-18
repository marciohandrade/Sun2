#pragma once


#include "IncludeAll.h"
#include "ManagerBase.h"
#include <BaseCls.h>
#include "DrawBase.h"



class CManagerAlphaSort :	public CManagerBase
{
public:
	CManagerAlphaSort(void);
	virtual ~CManagerAlphaSort(void);
	virtual void Clear( void);

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy( void);

	// Alpha ��ü ����Ʈ ó��
protected:
	CWzArray<St_AlphaRender*> m_arAlphaObject;
public:
	BYTE* AddAlphaRenderObject( St_AlphaRender *par);
	void RemoveAllAlphaRenderObject( void);
	void RenderAlphaRenderList( void);

	// �պ� ó��
protected:
	int FindMergableObject( St_AlphaRender *par);
	BOOL IsMergableObject( St_AlphaRender *parDst, St_AlphaRender *par);
	BYTE* MergeObject( St_AlphaRender *parDst, St_AlphaRender *par);
};
