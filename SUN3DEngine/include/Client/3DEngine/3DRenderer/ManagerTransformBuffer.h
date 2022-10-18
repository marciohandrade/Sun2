// managertransformbuffer.h

#ifndef _3DRENDERER_MANAGERTRANSFORMBUFFER_H_
#define _3DRENDERER_MANAGERTRANSFORMBUFFER_H_

#include "managerbase.h"
#include <programcommon/wzlist.h>


typedef struct
{
	int m_iMaxVertex;
	int m_iReference;
	WzVector *m_pwvBuffer;
} St_TransformBuffer;


class CManagerTransformBuffer : public CManagerBase
{
public:
	CManagerTransformBuffer(void);
	virtual ~CManagerTransformBuffer(void);
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

	// 버퍼 할당/사용/해제
public:
	HANDLE Alloc( int iNumVertex);	// 할당
	BOOL AddRef( HANDLE hBuffer);	// 참조 증가
	BOOL Release( HANDLE hBuffer);	// 해제 (참조 감소)
	WzVector* GetBuffer( HANDLE hBuffer);	// 해당 버퍼 사용하기
	BOOL TestBuffer( HANDLE hBuffer);	// 해당 버퍼가 실질적으로 사용되고 있는가 ( 실제로 할당이 되었는가? )

	// 버퍼 관리
protected:
	CWzHandleManager<St_TransformBuffer*> m_hmBufferInUse;	// 현재 사용되고 있는 버퍼
	CWzList<St_TransformBuffer*> m_lstFreeBuffer;	// 현재 사용이 안되고 있는 버퍼
	int m_iCountTotalBuffer;	// 총 버퍼 크기
	int m_iSizeTotalBuffer;		// 모든 버퍼들의 크기 총합

	void FreeBuffer( HANDLE hBuffer);	// 해제 - Free Buffer list 로 보냄
	void DeleteAllBuffers();	// 모든 버퍼들을 삭제
	void DeleteBuffer( St_TransformBuffer *ptb);	// 버퍼 삭제
};

#endif // _3DRENDERER_MANAGERTRANSFORMBUFFER_H_