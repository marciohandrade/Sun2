#ifndef __SOLAR_DUMP_H__
#define __SOLAR_DUMP_H__

//=======================================================================================================================
/// Solar MiniDump
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2005. 4. 21
	@remarks
			- MinDump파일을 남김
	@todo
			- dump파일 전달 자동화
	@example
			- SolarDump dp( ".", MiniDumpNormal )
			AppName날짜=시간.dmp파일을 현재 디렉토리에 남김
*/
//=======================================================================================================================

#include <dbghelp.h>

#pragma once
/*
typedef enum _MINIDUMP_TYPE
{
MiniDumpNormal = 0x0000, 
MiniDumpWithDataSegs = 0x0001, 
MiniDumpWithFullMemory = 0x0002, 
MiniDumpWithHandleData = 0x0004, 
MiniDumpFilterMemory = 0x0008, 
MiniDumpScanMemory = 0x0010, 
MiniDumpWithUnloadedModules = 0x0020, 
MiniDumpWithIndirectlyReferencedMemory = 0x0040, 
MiniDumpFilterModulePaths = 0x0080, 
MiniDumpWithProcessThreadData = 0x0100, 
MiniDumpWithPrivateReadWriteMemory = 0x0200
} MINIDUMP_TYPE;
*/

class SolarDump
{
public:
	SolarDump( char * pszDir = ".", MINIDUMP_TYPE eDumpType = MiniDumpWithDataSegs );
	~SolarDump();

	void SetBuildVersion(const char* pszBuildVersion);

private:
	static LONG WINAPI		ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo );
    static int NewHandler(size_t);
    static void PureCallHandler();
    static void BufferOverflowHandler(int code, void* unused);
	static MINIDUMP_TYPE	s_eDumpType;
	static TCHAR			s_pszDumpFileDir[MAX_PATH];
	static TCHAR			s_pszFilePrefix[MAX_PATH];
	static TCHAR			s_pszBuildVersion[MAX_PATH];
    static int old_new_mode_;
};


#endif // __SOLAR_DUMP_H__