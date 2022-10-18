/*-----------------------------------------------------------------------------
 * SALib_dll.h
 *-----------------------------------------------------------------------------
 * 외부 배포용 인터페이스 파일
 *-----------------------------------------------------------------------------
 * All rights reserved by Ham Ji Man (faethm@gmail.com, jmham@webzen.com)
 *-----------------------------------------------------------------------------
 * Revision History:
 * Date					Who					What
 * ----------------		----------------	----------------
 * 10.02.2007			Ham Ji Man			birth
**---------------------------------------------------------------------------*/
#ifndef _SA_LIB_DLL_H_
#define _SA_LIB_DLL_H_

#ifdef SHAPEAUTH_DLL_EXPORTS
	#ifdef __cplusplus
		#define SA_DLL_API		extern "C" __declspec(dllexport)
		#define SA_DLL_CLASS	__declspec(dllexport)
	#else
		#define SA_DLL_API		__declspec(dllexport) 
	#endif//__cplusplus
#else
	#ifdef __cplusplus
		#define SA_DLL_API		extern "C" __declspec(dllimport) 
		#define SA_DLL_CLASS	__declspec(dllimport) 
	#else
		#define SA_DLL_API		__declspec(dllimport) 
	#endif//__cplusplus		
#endif//SA_DLL_EXPORTS

#include "IShapeAuth.h"

//-----------------------------------------------------------------------------
// Shape Auth 라이브러리 wrapper dll
//-----------------------------------------------------------------------------
class SA_DLL_CLASS RMSA_dll
{
public:
	RMSA_dll()
	{
		m_instance = new RMSA();
	};
	~RMSA_dll()
	{
		delete m_instance;
		m_instance = NULL;
	};
	
	IShapeAuth* get()
	{
		return m_instance->get();
	}
protected:
private:
	RMSA*	m_instance;
};

#endif //_SA_LIB_DLL_H_