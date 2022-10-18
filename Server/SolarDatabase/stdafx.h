#pragma once
#ifndef __STDAFX_H__
#define __STDAFX_H__

#define WIN32_LEAN_AND_MEAN
//#define SOLARDATABASE_API  //< 프로젝트 셋팅에 포함
#define _LIB_IGNORE
#pragma warning (disable:4251)

#include <CommonDef.h>

#include <windows.h>
#include <oledb.h>
#include <stdio.h>

#include <Linkedlist.h>
#include <CriticalSection.h>
#include <MemoryPoolFactory.h>

#include "SolarDatabaseDefine.h"

#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")

#define  NOTE(s) { (DWORD) s, #s }

#ifndef MAX
#  define MIN(a,b)  ( (a) < (b) ? (a) : (b) )
#  define MAX(a,b)  ( (a) > (b) ? (a) : (b) )
#endif
#define NUMELEM(x)    (sizeof(x)/sizeof(*x))
#define MAX_QUERY_LEN 4096
#define EOL           '\0'
#define wEOL          L'\0'

//++
// Useful rounding macros.
// Rounding amount is always a power of two.
//--

// moved to ProgramCommon( misc.h )
//#define ROUND_DOWN( Size, Amount )  ((DBLENGTH)(Size) & ~((Amount) - 1))
//#define ROUND_UP(   Size, Amount ) (((DBLENGTH)(Size) +  ((Amount) - 1)) & ~((Amount) - 1))
//#define random(min, max) ((rand()%(int)(((max) + 1)-(min)))+ (min)) 

#endif // __STDAFX_H__