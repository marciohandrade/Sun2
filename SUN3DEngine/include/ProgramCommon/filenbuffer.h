// filenbuffer.h

#ifndef _PROGRAMCOMMON_WZFILENBUFFER_H_
#define _PROGRAMCOMMON_WZFILENBUFFER_H_

#include "wztypedef.h"
#include <stdio.h>

//------------------------------------------------------------------------------
/**
    xxx : �� ���ϸ� �̻�(������ util�ʿ� �����)�ϰ�,
          ���� ��ü�� ���ʿ��� ���̳�, ���� �ҽ����� �����ϴ� �κ���
          ���� �ϴ��� ������ �ϰ� �׳� �� �״�. �Ŀ� namespace�� �ѷ��ε�
          �ƴϸ� �� �ʿ��� �ҽ��� �̵��� ��Ű�� �� ���̴�.
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