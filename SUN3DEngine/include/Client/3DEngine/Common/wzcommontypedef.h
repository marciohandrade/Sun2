// wzcommontypedef.h

// programcommon을 제외한 모든 프로젝트는 본 파일을 포함(include)할 것

#ifndef _COMMON_TYPEDEF_H_
#define _COMMON_TYPEDEF_H_

#include <programcommon/wztypedef.h>
#include "wzsharedef.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE( p )        if( p ) { delete (p); (p) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY( p )  if( p ) { delete [] (p); (p) = NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE( p )       if( p ) { (p)->Release(); (p) = NULL; }
#endif

#ifndef RETURN_TRUE_IF
#define RETURN_TRUE_IF( x )     if( x ) { return 1; }
#endif

#ifndef RETURN_FALSE_IF
#define RETURN_FALSE_IF( x )    if( x ) { return 0; }
#endif

#ifndef RETURN_IF
#define RETURN_IF( x )          if( x ) { return; }
#endif

//#if (_MSC_VER >= 1500) // vs2008 이상
//    #ifndef _DEBUG //#if'N'def 
//        #define _SECURE_SCL 0
//    #endif
//#endif

#endif // _COMMON_TYPEDEF_H_