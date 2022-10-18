// wztypedef.h

//------------------------------------------------------------------------------
/**
    * 내부 공유 (로그 제외) / 외부 노출

    - 이것 저것 고민한 결과 프로젝트내 파일들이 공유하는 파일 필요

    - 공유 디파인 및 타입 정의
    - 모든 파일은 로그를 쓸 수 있어야 함.
*/

#ifndef _PROGRAMCOMMON_WZTYPEDEF_H_
#define _PROGRAMCOMMON_WZTYPEDEF_H_

#define VC_EXTRALEAN

//#if (_MSC_VER >= 1500) // vs2008 이상
//    #ifndef _DEBUG //#if'N'def 
//        #define _SECURE_SCL 0
//    #endif
//#endif

#include <windows.h>
#include <assert.h>

#include "wzenginever.h"
#include "wzlog.h"
#include "wzvector.h"


#define WZ_PI               ( 3.14159265358979323846f )
#define WZ_EPSILON          ( 0.000001f )
#define WZ_INFINITE         ( 1000000000000.0f )

#define WZDEG2RAD( d )      ( ((d) * WZ_PI) / 180.0f )
#define WZRAD2DEG( r )      ( ((r) * 180.0f) / WZ_PI )

typedef unsigned long       WzID;

#define INVALID_WZID        ( 0 )

#define StrToWzID( s )      ( (WzID)( (*(s)) | ( *((s) + 1) << 8 ) | ( *((s) + 2) << 16 ) | ( *((s) + 3) << 24 ) ) )
#define CharToWzID( c1, c2, c3, c4 ) ( (WzID)( (c1) | ( (c2) << 8 ) | ( (c3) << 16 ) | ( (c4) << 24 ) ) )

inline char* WzIDToString( char* str, WzID wzID )
{
    *str = (char)( wzID & 0xFF );
    *(str + 1) = (char)( ( wzID >> 8  ) & 0xFF );
    *(str + 2) = (char)( ( wzID >> 16 ) & 0xFF );
    *(str + 3) = (char)( ( wzID >> 24 ) & 0xFF );

    return str;
}

static const char* WzIDToString( WzID wzID )
{
    static char str[8];
    *str = (char)( wzID & 0xFF );
    *(str + 1) = (char)( ( wzID >> 8  ) & 0xFF );
    *(str + 2) = (char)( ( wzID >> 16 ) & 0xFF );
    *(str + 3) = (char)( ( wzID >> 24 ) & 0xFF );
    str[4] = NULL;

    return str;
};

#endif // _PROGRAMCOMMON_WZTYPEDEF_H_