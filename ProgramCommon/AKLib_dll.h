/*-----------------------------------------------------------------------------
 * AKLib_dll.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _AKLIB_DLL_HEADER_
#define _AKLIB_DLL_HEADER_



#ifdef AKV2_DLL_EXPORTS
	#ifdef __cplusplus
		#define AKV2_DLL_API		extern "C" __declspec(dllexport)
		#define AKV2_DLL_CLASS		__declspec(dllexport)
	#else
		#define AKV2_DLL_API		__declspec(dllexport) 
	#endif//__cplusplus
#else
	#ifdef __cplusplus
		#define AKV2_DLL_API		extern "C" __declspec(dllimport) 
		#define AKV2_DLL_CLASS		__declspec(dllimport) 
	#else
		#define AKV2_DLL_API		__declspec(dllimport) 
	#endif//__cplusplus		
#endif//AKV2_DLL_EXPORTS


#include "AKLib.h"

//-----------------------------------------------------------------------------
// ArmorKit version 2 라이브러리 wrapper dll
//-----------------------------------------------------------------------------
class AKV2_DLL_CLASS RSCM_IAKLib_dll
{
public:
	RSCM_IAKLib_dll()
	{
		m_instance = new RSCM_IAKLib();
	};
    ~RSCM_IAKLib_dll()
	{
		delete m_instance; 
		m_instance = NULL;
	};


	IAKLib* get() 
	{
		return m_instance->get();
	}
protected:
private:
	RSCM_IAKLib*	m_instance;
};




#endif//_AKLIB_DLL_HEADER_