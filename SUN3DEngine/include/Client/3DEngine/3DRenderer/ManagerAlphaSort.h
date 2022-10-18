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

	// Alpha 按眉 府胶飘 贸府
protected:
	CWzArray<St_AlphaRender*> m_arAlphaObject;
public:
	BYTE* AddAlphaRenderObject( St_AlphaRender *par);
	void RemoveAllAlphaRenderObject( void);
	void RenderAlphaRenderList( void);

	// 钦捍 贸府
protected:
	int FindMergableObject( St_AlphaRender *par);
	BOOL IsMergableObject( St_AlphaRender *parDst, St_AlphaRender *par);
	BYTE* MergeObject( St_AlphaRender *parDst, St_AlphaRender *par);
};
