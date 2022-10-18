/*-----------------------------------------------------------------------------
 * IGhost.h
 *-----------------------------------------------------------------------------
 * GhostAPI 에 대한 인터페이스 정의 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/

#ifndef _I_GHOST_H_
#define _I_GHOST_H_

#include <windows.h>

//-----------------------------------------------------------------------------
// Class 전방 선언
//
class IGhost;


//-----------------------------------------------------------------------------
// IGhost 객체 관리용 전역 함수
//
IGhost*	CreateGhostInstance(void);
void DestroyGhostInstance(IGhost* p);



//-----------------------------------------------------------------------------
// Ghost api 를 외부에서 사용하기 위한 인터페이스
//
class IGhost
{
public:
	// custom GetProcAddress 구현
	//
	FARPROC	MyGetProcAddress(HINSTANCE hMod,LPCTSTR pszSymbol);

	//-------------------------------------------------------------------------
	// kernel32.dll
	//-------------------------------------------------------------------------
	
	// GetModuleHandle()
	//	
	virtual HMODULE __stdcall GH_GetModuleHandle(IN LPCTSTR lpModuleName) = 0;
	
	// LoadLibrary()
	// 	
	virtual HMODULE __stdcall GH_LoadLibrary(IN LPCTSTR lpLibFileName) = 0;
	virtual BOOL __stdcall GH_FreeLibrary(IN OUT HMODULE hLibModule) = 0;
	
	// WaitForSingleObject
	//
	virtual DWORD __stdcall GH_WaitForSingleObject(IN HANDLE hHandle, IN DWORD dwMilliseconds) = 0;
	
	// SetEvent
	virtual BOOL __stdcall GH_SetEvent(IN HANDLE hEvent) = 0;

	// ResetEvent
	virtual BOOL __stdcall GH_ResetEvent(IN HANDLE hEvent) = 0;

	// CreateEvent
	virtual HANDLE __stdcall GH_CreateEvent(
						IN LPSECURITY_ATTRIBUTES lpEventAttributes,
						IN BOOL bManualReset,
						IN BOOL bInitialState,
						IN LPCTSTR lpName
						) = 0;

	// CloseHandle
	//
	virtual BOOL __stdcall GH_CloseHandle(IN OUT HANDLE hObject) = 0;

	// CreateThread
	virtual HANDLE __stdcall GH_CreateThread (
						IN LPSECURITY_ATTRIBUTES lpThreadAttributes,
						IN SIZE_T dwStackSize,
						IN LPTHREAD_START_ROUTINE lpStartAddress,
						IN LPVOID lpParameter,
						IN DWORD dwCreationFlags,
						OUT LPDWORD lpThreadId
						) = 0;

	// CreateRemoteThrread
	virtual HANDLE __stdcall GH_CreateRemoteThread (
						IN HANDLE hProcess,
						IN LPSECURITY_ATTRIBUTES lpThreadAttributes,
						IN SIZE_T dwStackSize,
						IN LPTHREAD_START_ROUTINE lpStartAddress,
						IN LPVOID lpParameter,
						IN DWORD dwCreationFlags,
						OUT LPDWORD lpThreadId
						) = 0;

	// SuspendThread
	virtual DWORD __stdcall GH_SuspendThread(IN HANDLE hThread) = 0;

	// ResumeThread
	virtual DWORD __stdcall GH_ResumeThread(IN HANDLE hThread) = 0;	    
	
	// GetCurrentProcessId	
	virtual DWORD __stdcall GH_GetCurrentProcessId(void) = 0;


	// OpenProcess
	virtual HANDLE __stdcall GH_OpenProcess(
								IN DWORD dwDesiredAccess,
								IN BOOL bInheritHandle,
								IN DWORD dwProcessId
								) = 0;

	// VirtualAlloc
	virtual LPVOID __stdcall GH_VirtualAlloc(
								IN LPVOID lpAddress,
								IN SIZE_T dwSize,
								IN DWORD flAllocationType,
								IN DWORD flProtect
								) = 0;

	// VirtualFree
	virtual BOOL __stdcall GH_VirtualFree(
								IN LPVOID lpAddress,
								IN SIZE_T dwSize,
								IN DWORD dwFreeType
								) = 0;

	// VirtualAllocEx
	virtual LPVOID __stdcall GH_VirtualAllocEx(
								IN HANDLE hProcess,
								IN LPVOID lpAddress,
								IN SIZE_T dwSize,
								IN DWORD flAllocationType,
								IN DWORD flProtect
								) = 0;

	// VirtualFreeEx
	virtual BOOL __stdcall GH_VirtualFreeEx(
								IN HANDLE hProcess,
								IN LPVOID lpAddress,
								IN SIZE_T dwSize,
								IN DWORD dwFreeType
								) = 0;

	// VirtualProtect
	virtual BOOL __stdcall GH_VirtualProtect(
								IN  LPVOID lpAddress,
								IN  SIZE_T dwSize,
								IN  DWORD flNewProtect,
								OUT PDWORD lpflOldProtect
								) = 0;

	// VirtualProtectEx
	virtual BOOL __stdcall GH_VirtualProtectEx(
								IN  HANDLE hProcess,
								IN  LPVOID lpAddress,
								IN  SIZE_T dwSize,
								IN  DWORD flNewProtect,
								OUT PDWORD lpflOldProtect
								) = 0;

	// ReadProcessMemory
	virtual BOOL __stdcall GH_ReadProcessMemory(
								IN HANDLE hProcess,
								IN LPCVOID lpBaseAddress,
								OUT LPVOID lpBuffer,
								IN SIZE_T nSize,
								OUT SIZE_T * lpNumberOfBytesRead
								) = 0;

	// WriteProcessMemory
	virtual BOOL __stdcall GH_WriteProcessMemory(
								IN HANDLE hProcess,
								IN LPVOID lpBaseAddress,
								IN LPCVOID lpBuffer,
								IN SIZE_T nSize,
								OUT SIZE_T * lpNumberOfBytesWritten
								) = 0;
	// TerminateProcess
	virtual BOOL __stdcall GH_TerminateProcess(IN HANDLE hProcess,
								IN UINT uExitCode) = 0;

	// OutputDebugString
	virtual VOID __stdcall GH_OutputDebugString(IN LPCSTR lpOutputString) = 0;

	// CreateFile
	virtual  HANDLE __stdcall GH_CreateFile(
								IN LPCSTR lpFileName,
								IN DWORD dwDesiredAccess,
								IN DWORD dwShareMode,
								IN LPSECURITY_ATTRIBUTES lpSecurityAttributes,
								IN DWORD dwCreationDisposition,
								IN DWORD dwFlagsAndAttributes,
								IN HANDLE hTemplateFile
								) = 0;

	// CreateFileMapping
	virtual  HANDLE __stdcall GH_CreateFileMapping(
								IN HANDLE hFile,
								IN LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
								IN DWORD flProtect,
								IN DWORD dwMaximumSizeHigh,
								IN DWORD dwMaximumSizeLow,
								IN LPCSTR lpName
								) = 0;

	// MapViewOfFile
	virtual  LPVOID __stdcall GH_MapViewOfFile(
								IN HANDLE hFileMappingObject,
								IN DWORD dwDesiredAccess,
								IN DWORD dwFileOffsetHigh,
								IN DWORD dwFileOffsetLow,
								IN SIZE_T dwNumberOfBytesToMap
								) = 0;
	// UnmapViewOfFile
	virtual BOOL __stdcall GH_UnmapViewOfFile(IN LPCVOID lpBaseAddress) = 0;

	//-------------------------------------------------------------------------
	// msvcr71.dll
	//-------------------------------------------------------------------------
	
	// thread 관련 api 
	// 
	virtual uintptr_t __cdecl GH__beginthreadex(
									void *security, 
									unsigned stacksize, 
									unsigned (__stdcall *thread_proc)(void *), 
									void * argument, 
									unsigned createflag, 
									unsigned *thrdaddr) = 0;

};





#endif
