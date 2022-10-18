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

	// ���� �Ҵ�/���/����
public:
	HANDLE Alloc( int iNumVertex);	// �Ҵ�
	BOOL AddRef( HANDLE hBuffer);	// ���� ����
	BOOL Release( HANDLE hBuffer);	// ���� (���� ����)
	WzVector* GetBuffer( HANDLE hBuffer);	// �ش� ���� ����ϱ�
	BOOL TestBuffer( HANDLE hBuffer);	// �ش� ���۰� ���������� ���ǰ� �ִ°� ( ������ �Ҵ��� �Ǿ��°�? )

	// ���� ����
protected:
	CWzHandleManager<St_TransformBuffer*> m_hmBufferInUse;	// ���� ���ǰ� �ִ� ����
	CWzList<St_TransformBuffer*> m_lstFreeBuffer;	// ���� ����� �ȵǰ� �ִ� ����
	int m_iCountTotalBuffer;	// �� ���� ũ��
	int m_iSizeTotalBuffer;		// ��� ���۵��� ũ�� ����

	void FreeBuffer( HANDLE hBuffer);	// ���� - Free Buffer list �� ����
	void DeleteAllBuffers();	// ��� ���۵��� ����
	void DeleteBuffer( St_TransformBuffer *ptb);	// ���� ����
};

#endif // _3DRENDERER_MANAGERTRANSFORMBUFFER_H_