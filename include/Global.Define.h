
#pragma once
#ifndef GLOBAL_DEFINE_H 
#define GLOBAL_DEFINE_H

//==================================================================================================  
// test tool 이므로 활성화된 상태에서 commit하지 말 것!!!!
//#define _DEBUGGING_TOOL_CLIENT_REDIRECT_NEW_  // new, delete 후킹을 위한 도구 (_NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_USE_SHARED_POOL_ 디파인과 함께 사용)
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