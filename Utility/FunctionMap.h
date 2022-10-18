/**
	함수 포인터를 관리하는 기능을 제공하는 클래스를 정의함
*/

#ifndef __FUNCTION_MAP_H__
#define __FUNCTION_MAP_H__

#pragma once

#include <windows.h>
#include <hash_map>


namespace util {

	class BASE_FUNC
	{
	public:
		BASE_FUNC(){}
		virtual ~BASE_FUNC(){}

		DWORD m_dwFunctionKey;
	};

	class FunctionMap
	{
		typedef stdext::hash_map< DWORD, BASE_FUNC * >	MAP_FUNCTION;
		typedef MAP_FUNCTION::iterator					MAP_FUNCTION_IT;
		typedef MAP_FUNCTION::value_type				MAP_FUNCTION_VT;

	protected:
		MAP_FUNCTION							m_mFunction; // 함수 포인터를 유지하는 맵 객체
		MAP_FUNCTION_IT							m_itr;
	public:
		
		FunctionMap(){}
		~FunctionMap()
		{
			BASE_FUNC * pFunc;
			MAP_FUNCTION_IT itr;
			for(itr = m_mFunction.begin(); itr != m_mFunction.end();)
			{
				pFunc = itr->second;
				if(pFunc)
				{
					delete pFunc;
					pFunc = NULL;
				}
				itr = m_mFunction.erase(itr);
			}
		}

		VOID SetFirst()
		{
			m_itr = m_mFunction.begin();
		}
		BOOL GetNext( BASE_FUNC *& pFuncOut )
		{
			++m_itr;
			pFuncOut = m_itr->second;
			if( m_itr != m_mFunction.end() )
				return TRUE;
			return FALSE;
		}

		BOOL  Add( BASE_FUNC * pFunction )
		{
			MAP_FUNCTION_IT			it;
			BASE_FUNC *				pTemp = NULL;

			if( !m_mFunction.empty() )
			{
				it = m_mFunction.find( pFunction->m_dwFunctionKey );
				if(it != m_mFunction.end())	pTemp = it->second;
			}

			if( pTemp != NULL ) 
			{
				return FALSE;
			}

			m_mFunction.insert(MAP_FUNCTION_VT( pFunction->m_dwFunctionKey, pFunction ));
			return TRUE;
		}

		BASE_FUNC * Find(const DWORD dwFunctionKey)
		{
			MAP_FUNCTION_IT			it;
			BASE_FUNC *				pTemp = NULL;


			if( !m_mFunction.empty() )
			{
				it = m_mFunction.find( dwFunctionKey );
				if(it != m_mFunction.end())	pTemp = it->second;
			}

			return pTemp;
		}
	};

}//End of namespace util

#endif // __FUNCTION_MAP_H__


