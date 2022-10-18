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
	int m_iIndex;	// 위치
	int m_iSize;	// 크기
} St_InstantMemoryRecycle;


class CManagerInstantMemory : public CManagerBase  
{
public:
	CManagerInstantMemory();
	virtual ~CManagerInstantMemory();
	virtual void Clear( void);

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy( void);

	/// @name 메모리 할당
public:
	virtual void* Alloc( int iSize);	///< 할당
	virtual void Free( void *pPtr);		///< 제거 (실제적으로 하는 일 없음)
	void Flush( void);	///< 모두 비움 (한 프레임을 모두 그린 후에 비운다)

	virtual int GetRollBackPoint( void);	///< 되돌릴 위치 얻기
	virtual BOOL RollBack( int iRollBackPoint);	///< 되돌리기

	/// @name 메모리 관리
protected:
	BYTE *m_pMemoryPool;	///< 메모리 풀
	int m_iMemoryPoolSize;	///< 전체 용량
	int m_iUsedMemoryPool;	///< 사용된 용량
	int m_iFreedMemoryPool;	///< 할당 해제된 용량
	int m_iMaxUsedMemoryPool;	///< 현재까지 m_iUsedMemoryPool 의 최대값 (통계자료)
	void AllocateMemoryPool( int iSize);

/*	/// @name 해제된 메모리 재사용
protected:
	CWzArray<St_InstantMemoryRecycle> m_arRecycle;
	void AddRecycleMemory( int iStartIndex, int iSize);
	void RemoveRecycleMemory( int iIndex);
	BYTE* AllocAsRecycledMemory( int iSize);*/
};

#endif // !defined(AFX_MANAGERINSTANTMEMORY_H__62EF5B74_2C69_4D54_AAC6_0597CE40BA6A__INCLUDED_)
