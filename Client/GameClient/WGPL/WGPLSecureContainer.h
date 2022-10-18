#pragma once

#include <windows.h>
#include "WGPLShuffle.h"
#include <vector>

namespace WGPL
{
#define CHECK_RETURN_CALLSTACK	DWORD ret_ptr_dlrjsTmwlakdy	= 0;													\
	__asm {pushad}																		\
	__asm {mov eax, ebp}																\
	__asm {add eax, 4}																	\
	__asm {mov eax, dword ptr ds:[eax]}													\
	__asm {mov ret_ptr_dlrjsTmwlakdy, eax}												\
	__asm {popad};																		\
	if( (ret_ptr_dlrjsTmwlakdy <= 0x00401000) || (ret_ptr_dlrjsTmwlakdy >= 0x00885000) ) \
	{																					\
		MessageBoxA(NULL, "CallStack Error!!!", 0, 0);									\
		exit(0);																		\
	}																					\

	class DUMMY
	{
	public:
		DUMMY(){}
	};


	template < class _Type >
	class SecureContainer
	{
	public:
		SecureContainer() { } 

		~SecureContainer()	{ Destroy(); }

		void	SetValue( _Type *pValue );
		_Type	*GetValue();
		void	Destroy();
	private :
		std::vector <_Type *>	m_ObjList;
		Shuffle<int, false, 1>	m_iIndex;		// 성능이슈로 Shuffle의 checksum 사용안함
	};

	template < class _Type >
	_Type *SecureContainer<_Type>::GetValue() 
	{
		_Type *pValue = m_ObjList[m_iIndex];
		CHECK_RETURN_CALLSTACK
			return pValue;
	}

	template < class _Type>
	void SecureContainer<_Type>::SetValue( _Type *pValue ) 
	{ 
		//CODE_GUARD

			m_iIndex = rand()%10;
		for (int i=0; i < 10; i++)
		{
			if (i != m_iIndex)
				m_ObjList.push_back((_Type *)new DUMMY);
			else
				m_ObjList.push_back(pValue);
		}
	}

	template < class _Type >
	void SecureContainer<_Type>::Destroy() 
	{
		for ( int i=0; i<(int)m_ObjList.size(); i++ )
		{
			if ( i == m_iIndex )
			{
				delete m_ObjList[i];
			}
			else
			{
				delete ((DUMMY *)m_ObjList[i]);
			}
		}
		m_ObjList.resize(0);
	}
}
