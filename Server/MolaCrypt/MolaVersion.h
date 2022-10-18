#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<BUILD MODE CONTROL>
#include "MolaVersion.RCG.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<VERSION CONTROL & MILESTONE UPDATE>
#define __MOLA_HSEL_REFACTORING__

#define MOLA_CURRENT_VERSION				0x5000

#define MOLA_DEVELOP_VERSION				0x5001
#define MOLA_DEVELOP_VERSION_EQ(ver)		((ver) == MOLA_CURRENT_VERSION)
#define MOLA_DEVELOP_VERSION_GE(ver)		((ver) >= MOLA_CURRENT_VERSION)
#define MOLA_DEVELOP_VERSION_LE(ver)		((ver) <= MOLA_CURRENT_VERSION)
#define MOLA_DEVELOP_VERSION_NE(ver)		((ver) != MOLA_CURRENT_VERSION)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<REFACTORING&TEST>
#define CODE_BACKUP							(0)				// 기존 작업 백업시
#define CODE_RENEWAL						(1)				// 기존 작업 대체, 새롭게 적용하는 경우
#define CODE_SAMPLE							(0)				// 샘플 코드
#define CODE_TEST(b)						(!!(b))			// 코드 테스트시
#define CHANGEUP_CODE(b)					(!!(b))			// 코드 설정 모드
#define LOG_DEBUG_WRITER(macro)				(!!(macro))

#if MOLA_DEVELOP_VERSION_GE(0x5001)
#	define WzASM_UPDATE_0x5001				(1)
#	define WzASM_UPDATE_0x5001_TEST			(1)
//#	define MOLA_UPDATE_0x5001	(1)	<-- SAMPLE
#else
#	define WzASM_UPDATE_0x5001				(0)
#	define WzASM_UPDATE_0x5001_TEST			(0)
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#ifdef _DEBUG
//#	define __MOLA_CLIENT_DEBUGGING__
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<TEST&LOG>
#	define __MOLA_RESOURCE_TEST__						0
#	define __MOLA_IwASM_CMD_TEST__						0
#	define __MOLA_IwASM_CMD_SERVER_CODE_TEST__			0
#	define __MOLA_CLIENT_DEBUGGING_WRITELOG__			0
#	define __MOLA_CLIENT_DEBUGGING_TESTLOG__			0
#	define __MOLA_CLIENT_DEBUGGING_FLUSHLOG__			0
#	define __MOLA_CLIENT_DEBUGGING_VMLOG__				0
#	define __MOLA_CLIENT_DEBUGGING_VMLOG_T__			0
#	define __MOLA_CLIENT_DEBUGGING_VMLOG_TRACE__		0
#	define __MOLA_CLIENT_DEBUGGING_BITTEST__			0	// BitOperation
#	define __MOLA_CLIENT_DEBUGGING_HSEL__				0	// xxxHSELxxx
#	define __MOLA_CLIENT_DEBUGGING_RNDSNK__				0	// RandomSNKey
#	define __MOLA_CLIENT_DEBUGGING_BBTAN__				0	// BBTan~~~
#	define __MOLA_CLIENT_DEBUGGING_BBTAN_CALL__			0	// BBTan~~~
#	define __MOLA_CLIENT_DEBUGGING_SHMEM__				0	// SharedMemoryPool


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#if CHANGEUP_CODE(__MOLA_RESOURCE_TEST__)
#	define DEBUGGING_INTERRUPT_INMOLA()	__asm { int 3 }
#else
#	define DEBUGGING_INTERRUPT_INMOLA()	
#endif

#define FORCED_INTERRUPT_INMOLA()		__asm { int 3 }

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_VMLOG_TRACE__)
#	define	DEBUGGING_OUTPUT()			printf( "%s\n", __FUNCTION__ );
#	define	NOT_IMPLEMENTATION()		printf( "\tNOT IMPLEMENTATION\n" );
#else
#	define	DEBUGGING_OUTPUT()
#	define	NOT_IMPLEMENTATION()
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_BBTAN_CALL__)
#	define	CALLSTACK_TRACING()			printf( "%s\n", __FUNCTION__ );
#else
#	define	CALLSTACK_TRACING()
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#ifdef __MOLA_CLIENT_DEBUGGING__

extern HANDLE	shFILE;
extern DWORD	sdwLOGINC;

#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_FLUSHLOG__)
#	define	FLUSHFILEBUFFER(handle)		FlushFileBuffers( handle )
#else
#	define	FLUSHFILEBUFFER(handle)
#endif


#define __DEBUGGING_OUTPUT0(msg)		\
	do {								\
	CHAR	buffer[256];			\
	DWORD dwWritten1 = (DWORD)_snprintf( buffer, sizeof(buffer), "[%04u] %s\r\n", sdwLOGINC++, msg );	\
	WriteFile( shFILE, buffer, dwWritten1, &dwWritten1, NULL );						\
	FLUSHFILEBUFFER( shFILE );		\
	} while(0);

#define __DEBUGGING_OUTPUT1(msg, arg0)	\
	do {								\
	CHAR	buffer[256];			\
	DWORD dwWritten1 = (DWORD)_snprintf( buffer, sizeof(buffer), "[%04u] %s\t%08X\r\n", sdwLOGINC++, msg, arg0 );	\
	WriteFile( shFILE, buffer, dwWritten1, &dwWritten1, NULL );						\
	FLUSHFILEBUFFER( shFILE );		\
	} while(0);

#define __DEBUGGING_OUTPUT2(msg, arg0, arg1)		\
	do {											\
	CHAR	buffer[256];						\
	DWORD dwWritten1 = (DWORD)_snprintf( buffer, sizeof(buffer), "[%04u] %s\t%08X\t%08X\r\n", sdwLOGINC++, msg, arg0, arg1 );	\
	WriteFile( shFILE, buffer, dwWritten1, &dwWritten1, NULL );						\
	FLUSHFILEBUFFER( shFILE );					\
	} while(0);

#	if (__MOLA_CLIENT_DEBUGGING_VMLOG__ == 1)
#		define __DEBUGGING_VM_OUTPUT0(msg)					printf( "%s\n", msg )
#		define __DEBUGGING_VM_OUTPUT1(msg, arg0)			printf( "%s\t%08X\n", msg, arg0 );
#		define __DEBUGGING_VM_OUTPUT2(msg, arg0, arg1)		printf( "%s\t%08X\t%08X\n", msg, arg0, arg1 );
#	else
#		define __DEBUGGING_VM_OUTPUT0(msg)					
#		define __DEBUGGING_VM_OUTPUT1(msg, arg0)			
#		define __DEBUGGING_VM_OUTPUT2(msg, arg0, arg1)		
#	endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#else //__MOLA_CLIENT_DEBUGGING__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define __DEBUGGING_OUTPUT0(msg)
#define __DEBUGGING_OUTPUT1(msg, arg0)
#define __DEBUGGING_OUTPUT2(msg, arg0, arg1)

#define __DEBUGGING_VM_OUTPUT0(msg)
#define __DEBUGGING_VM_OUTPUT1(msg, arg0)
#define __DEBUGGING_VM_OUTPUT2(msg, arg0, arg1)

#endif //__MOLA_CLIENT_DEBUGGING__

