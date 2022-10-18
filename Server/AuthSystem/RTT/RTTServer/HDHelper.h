/*-----------------------------------------------------------------------------
 * HDHelper.h
 *-----------------------------------------------------------------------------
 * HookDefender 관련 상수 / 코드 --> 문자열 변환 모듈
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2006, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2006, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#ifndef _HOOK_DEFENDER_HELPER_H_
#define _HOOK_DEFENDER_HELPER_H_

//
// 에러 코드 문자열 정의 (use S_ prefix)
//
#define		S_HDERR_NONE			"No err"
#define		S_HDERR_GENERIC			"Generic error"
#define		S_HDERR_EAT_RESTORE		"An error occurred when defence export table"
#define		S_HDERR_IAT_RESTORE		"An error occurred when defence import table"


#define		S_HDERR_INVALID_PE		"Invalid PE format"
#define		S_HDERR_NO_API_FOUNDED	"No API founded in module"
#define		S_HDERR_INVALID_ADDRESS	"Address is invalid or protected"



//
// hook type 
// 
#define		S_HT_NONE							"No hook"
#define		S_HT_ENTRY_HOOK_INVALID_ADDR_IAT	"IAT hook "
#define		S_HT_ENTRY_HOOK_INVALID_ADDR_EAT	"EAT hook "
#define		S_HT_ENTRY_HOOK_USE_CAVE_EAT		"EAT hook by CAVE"
#define		S_HT_INLINE_HOOK_EAT				"EAT hook by Inline"


class CHDHelper
{
private:	
protected:
public:
	CHDHelper(){};
	~CHDHelper(){};

	static LPCTSTR	HDErrorCodeToString(INT iErrorCode);
	static LPCTSTR	HDHookTypeToString(ENUM_HOOK_TYPE HookType);
	static DWORD_PTR computeDetour(DWORD_PTR dwFuncEntryPoint);
	static int		convertHexBufToBinbuf(LPCTSTR hexBuf, PBYTE binBuf);

	// HOOKDED_API_LIST 
	//
	static int	hal_to_stream(HOOKED_API_LIST &hal, int buf_limit, int *pbuf_size, char *&pbuf);
	static int  stream_to_hal(int buf_size, char *&pbuf, HOOKED_API_LIST& cleared_hal);

	
	// HDFAULT_API_LIST 
	//
	static int  hdfal_to_stream(HDFAULT_API_LIST &hdfal, int buf_limit, int *pbuf_size, char*&pbuf);
	static int  stream_to_hdfal(int buf_size, char *&pbuf, HDFAULT_API_LIST &cleared_hdfal);
	

	static int  free_converted_stream(char *buf_to_free);
};






#endif