#ifndef __HanRTA__
#define __HanRTA__

#if defined(_WIN32) || defined(_WIN64)

	#ifdef HANRTA_EXPORTS
		#define HANRTA_API __declspec(dllexport)
	#else
		#define HANRTA_API __declspec(dllimport)
		#ifdef _DEBUG
			#ifdef _WIN64
				#pragma comment(lib, "HanRTAD_x64.lib")
			#else				
				#pragma comment(lib, "HanRTAD.lib")
			#endif
		#else
			#ifdef _WIN64
				#pragma comment(lib, "HanRTA_x64.lib") 
			#else
				#pragma comment(lib, "HanRTA.lib") 
			#endif
		#endif
	#endif

#elif defined(__linux__)

	#define HANRTA_API
	#define __stdcall

#else

	#error OS not supported.

#endif

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif

#ifndef INOUT
	#define INOUT
#endif

#define SERVICE_NATION					0x000000FF
#define SERVICE_KOR						0x00000001
#define SERVICE_USA						0x00000002
#define SERVICE_JPN						0x00000003
#define SERVICE_CHN						0x00000004

#define SERVICE_TYPE					0x00000F00
#define SERVICE_ALPHA					0x00000100
#define SERVICE_REAL					0x00000200
#define SERVICE_BETA					0x00000300

//
// Initial values
//
#define INVALID_SERVICE_CODE			0xFFFFFFFF
#define INVALID_CONN_COUNT				0

#if defined(__linux) && defined(__cplusplus)
extern "C"
{
#endif // __linux__&&__cplusplus

/*
Return value Info:
0 : Ok
- : fault from  function / this system 
	-1 : general fault.
+ : fault from  auth server
	+1 : ???
*/
HANRTA_API int __stdcall HanRTAInit(IN char* szGameId,
									IN int nServiceCode = INVALID_SERVICE_CODE,
									IN unsigned int nConnCnt = INVALID_CONN_COUNT);

/*
Info String: like below..
"gameid=gameid&servername=1&conncount=2"
*/
HANRTA_API int __stdcall HanRTA(IN char* szInfoString);

#if defined(__linux) && defined(__cplusplus)
}
#endif // __linux__&&__cplusplus
									
#endif