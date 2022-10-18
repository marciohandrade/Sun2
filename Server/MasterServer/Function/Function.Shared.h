#pragma once

#include <Sessions/ServerSessionManager.h>

//// ========================================================================================
////   Master Server 의존적인 코드 수행
//class IProcess
//{
//public:
//	virtual VOID	Process( PVOID pArgument );
//};
////
//// ========================================================================================
//
//template<DWORD dwNodeSize, typename _INTERFACE_>
//class FuncRouter
//{
//public:
//	template<typename _TYPENAME_>
//	BOOL	Register( DWORD dwIndex,  )
//	{
//		if( dwIndex < dwNodeSize )
//		{
//			IVMemoryPool* pPool = new _TYPENAME_;
//			m_Interfaces[dwIndex] = pPool;
//			return TRUE;
//		}
//		return FALSE;
//	}
//	VOID	Process( DWORD dwIndex, PVOID pArgument );
//
//private:
//	_INTERFACE_*	m_Interfaces[dwNodeSize];
//}


