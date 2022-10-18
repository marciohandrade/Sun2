#pragma once

#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501
#endif
#include <windows.h>

#include <string>
#include <vector>


#define PERF_CHECK_FRAME 100

extern int g_nCollConuter;
extern int g_nPerfCounter;;


#define PROFILE_TIME_TEST( func )	\
{								\
	static __int64 fIncreaseCount = 0;	\
	static bool bOutput = false;	\
	static int nCallCount = 0;		\
	LARGE_INTEGER liFrequency;		\
	LARGE_INTEGER liStartTime;	\
	LARGE_INTEGER liCurTime;	\
	QueryPerformanceFrequency(&liFrequency);	\
	QueryPerformanceCounter(&liStartTime);	\
	func;		\
	nCallCount++;	\
	QueryPerformanceCounter(&liCurTime);	\
	if( g_nPerfCounter % PERF_CHECK_FRAME == 0 ) {	\
	if( bOutput == false ) {	\
	char szStr[512];	\
	sprintf( szStr, "CallPerFrame : %d / %s TickPerFrame(%I64d) \n", nCallCount/PERF_CHECK_FRAME, #func, fIncreaseCount/PERF_CHECK_FRAME );	\
	OutputDebugString( szStr );	\
	fIncreaseCount = 0.f;	\
	nCallCount = 0;			\
	bOutput = true;		\
	}	\
	}	\
	else {	\
	__int64 elapsedclock = liCurTime.QuadPart - liStartTime.QuadPart;	\
	__int64 elapsedmillsecond = elapsedclock * 1000000 / liFrequency.QuadPart;	\
	fIncreaseCount += elapsedmillsecond;	\
	bOutput = false; \
	}	\
}
