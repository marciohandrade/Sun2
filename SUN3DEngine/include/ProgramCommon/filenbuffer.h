// filenbuffer.h

#ifndef _PROGRAMCOMMON_WZFILENBUFFER_H_
#define _PROGRAMCOMMON_WZFILENBUFFER_H_

#include "wztypedef.h"
#include <stdio.h>

//------------------------------------------------------------------------------
/**
    xxx : 이 파일명도 이상(오히려 util쪽에 가까움)하고,
          존재 자체가 불필요해 보이나, 기존 소스에서 참조하는 부분이
          많아 일단은 정리만 하고 그냥 놔 뒀다. 후에 namespace로 둘러싸든
          아니면 각 필요한 소스로 이동을 시키든 할 것이다.
*/

void Write( HANDLE hFile, const char* fmt, ... );
void HexWrite( HANDLE hFile, const void* buf, int len );

void GetFloatString( char* dst, float val, int lvl = 4 );
BOOL GetFromBuffer( void* dst, const BYTE* src, int size, int totSize, int* cur );

BOOL GetFolderOfFilePath( char* buf, const char* filePath );
BOOL GetFileNameOfFilePath( char* buf, const char* filePath );

//------------------------------------------------------------------------------
/**
*/
inline char CharToUpper( char c )
{
    return ( ( c >= 'a' && c <= 'z' ) ? ( c - 'a' + 'A' ) : c );
}

//------------------------------------------------------------------------------
/**
*/
inline char* ParamBool2Str( BOOL param )
{
    return ( param ? "1" : "0" );
}
//------------------------------------------------------------------------------ 
inline char* ParamBool2Str( bool param )
{
    return ( param ? "1" : "0" );
}
//------------------------------------------------------------------------------
/**
*/
inline char* ParamInt2Str( int param, char* buf )
{
    WzAssert( buf );

    sprintf( buf, "%d", param );

    return buf;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL ParamStr2Bool( char* param )
{
    WzAssert( param );

    return ( atoi( param ) != 0 );
}

//------------------------------------------------------------------------------
/**
*/
inline int ParamStr2Int( char* param )
{
    WzAssert( param );

    return ( atoi( param ) );
}

#endif // _PROGRAMCOMMON_WZFILENBUFFER_H_