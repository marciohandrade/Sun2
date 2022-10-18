#ifndef __CRYPTFUNC_H__
#define __CRYPTFUNC_H__

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include <tchar.h>
#define STRSAFE_LIB
#include <strsafe.h>

#define CONFIG_ENCKEY "ZwQuerySystemInformation"

int 
__stdcall 
SimpleXorCryption(
	PBYTE in_data, 
	DWORD in_size, 
	PBYTE *out_data,	
	DWORD *out_size
	);


#endif	//	 __CRYPTFUNC_H__ 

