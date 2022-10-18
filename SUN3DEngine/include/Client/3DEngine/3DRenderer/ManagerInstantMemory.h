// ManagerInstantMemory.h: interface for the CManagerInstantMemory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANAGERINSTANTMEMORY_H__62EF5B74_2C69_4D54_AAC6_0597CE40BA6A__INCLUDED_)
#define AFX_MANAGERINSTANTMEMORY_H__62EF5B74_2C69_4D54_AAC6_0597CE40BA6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IncludeAll.h"
#include "ManagerBase.h"



typedef struct
{
	int m_iIndex;	// ��ġ
	int m_iSize;	// ũ��
} St_InstantMemoryRecycle;


class CManagerInstantMemory : public CManagerBase  
{
public:
	CManagerInstantMemory();
	virtual ~CManagerInstantMemory();
	virtual void Clear( void);

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy( void);

	/// @name �޸� �Ҵ�
public:
	virtual void* Alloc( int iSize);	///< �Ҵ�
	virtual void Free( void *pPtr);		///< ���� (���������� �ϴ� �� ����)
	void Flush( void);	///< ��� ��� (�� �������� ��� �׸� �Ŀ� ����)

	virtual int GetRollBackPoint( void);	///< �ǵ��� ��ġ ���
	virtual BOOL RollBack( int iRollBackPoint);	///< �ǵ�����

	/// @name �޸� ����
protected:
	BYTE *m_pMemoryPool;	///< �޸� Ǯ
	int m_iMemoryPoolSize;	///< ��ü �뷮
	int m_iUsedMemoryPool;	///< ���� �뷮
	int m_iFreedMemoryPool;	///< �Ҵ� ������ �뷮
	int m_iMaxUsedMemoryPool;	///< ������� m_iUsedMemoryPool �� �ִ밪 (����ڷ�)
	void AllocateMemoryPool( int iSize);

/*	/// @name ������ �޸� ����
protected:
	CWzArray<St_InstantMemoryRecycle> m_arRecycle;
	void AddRecycleMemory( int iStartIndex, int iSize);
	void RemoveRecycleMemory( int iIndex);
	BYTE* AllocAsRecycledMemory( int iSize);*/
};

#endif // !defined(AFX_MANAGERINSTANTMEMORY_H__62EF5B74_2C69_4D54_AAC6_0597CE40BA6A__INCLUDED_)
