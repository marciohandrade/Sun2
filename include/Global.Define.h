
#pragma once
#ifndef GLOBAL_DEFINE_H 
#define GLOBAL_DEFINE_H

//==================================================================================================  
// test tool �̹Ƿ� Ȱ��ȭ�� ���¿��� commit���� �� ��!!!!
//#define _DEBUGGING_TOOL_CLIENT_REDIRECT_NEW_  // new, delete ��ŷ�� ���� ���� (_NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_USE_SHARED_POOL_ �����ΰ� �Բ� ���)
#ifdef _DEBUGGING_TOOL_CLIENT_REDIRECT_NEW_  

#pragma warning(suppress: 4985)
void* /*__cdecl*/ operator new(size_t size);// _throw1(std::bad_alloc);
#pragma warning(suppress: 4985)
void  /*__cdecl*/ operator delete(void*);// _throw0();
#pragma warning(suppress: 4985)
void* /*__cdecl*/ operator new[](size_t size);// _throw1(std::bad_alloc);
#pragma warning(suppress: 4985)
void  /*__cdecl*/ operator delete[](void*);// _throw0();

#endif //_DEBUGGING_TOOL_CLIENT_REDIRECT_NEW_ 
//==================================================================================================  


#include "Deployment.h"

#define __INTELLISENSE__
#ifdef _SERVER
	#define __NA_0_20080829_NEW_PACKING_LIB
#endif

#endif //GLOBAL_DEFINE_H