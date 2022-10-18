#pragma once


//--------------------------------------------------------------------------------
//s ServerSession의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------
#include <MemoryPoolFactory.h>

template<class _Ct>
class SESSIONPOOL
{
public:
	SESSIONPOOL()			{ m_Pool.Initialize(20, 20, "SESSIONPOOL"); }
	~SESSIONPOOL()			{ m_Pool.Release();	}
	_Ct * Alloc()			{ return m_Pool.Alloc(); }
	BOOL Free( _Ct * p )	{ return m_Pool.Free(p); }
private:
	util::CMemoryPoolFactory<_Ct> m_Pool;
};


#define __DECL_SESSIONPOOL(_ClassType)										\
	private:															\
	static SESSIONPOOL<_ClassType> s_Pool;								\
	public:																\
	static _ClassType *	ALLOC()				{ return s_Pool.Alloc(); }	\
	static VOID FREE( _ClassType * pClass ) { s_Pool.Free( pClass ); }

#define __IMPL_SESSIONPOOL(_ClassType)										\
	SESSIONPOOL<_ClassType> _ClassType::s_Pool;
//--------------------------------------------------------------------------------
//e ServerSession의 메모리 풀 관련 코드
//--------------------------------------------------------------------------------

