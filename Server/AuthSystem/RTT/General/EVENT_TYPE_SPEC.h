//--------------------------------------------------------------------
//	ARMORKIT 과 WOPS 연동을 위한 스펙 정의 
//--------------------------------------------------------------------
// history
//	2005.12.06 	노용환 	최초 작성 
//  2005.12.06	노용환	AKRUNTIME_ERROR 코드 추가 
//	2005.12.08	노용환	Hacktype 에 대응되는 구조체 정리
//						각종 상수값 정의 추가
//--------------------------------------------------------------------


//[[ 상수 정의 -- from AkCommon.h]]
#include <stdlib.h>

#define  _MAX_API_NAME		255			/* max length of api name */
#define  _OPCODE_DUMP_LENGTH	16			/* API 의 구현부 덤프 길이  */
#define  _OPCODE_DUMP_BUF_LENGTH		( _OPCODE_DUMP_LENGTH * 2 + sizeof(TCHAR) )	/* API 의 구현부 덤프를 보관할 버퍼의 길이  */
#define  MAX_SIG_NAME		255			/* signature 의 최대 길이 */






//[[ ArmorKit 에서 Client 콜백으로 넘겨주는 데이터 타입 - from ArmorKit.h]]

// ArmorKit 내부 에러 발생	--  에러에 대한 문자열 또는 정의되는 에러코드 사용예정(아직 미정..)
#define AKRUNTIME_ERROR					-1

// 스피드핵 탐지
#define AKDETECT_SPEEDHACK				1


// Malware 탐지
#define AKDETECT_MALWARE				2


// 게임바이너리변조
#define AKDETECT_PEINTEGRITY			3


// 게임함수변조
#define AKDETECT_APIHOOK				4


// 아머킷 자체 변조			-- 현재 사용하지 않음
#define AKDETECT_AKINTEGRITY			5


// 디버그 상태탐지
#define AKDETECT_DEBUG					6


// 메모리조작				-- 현재 사용하지 않음
#define AKDETECT_MEMEDIT				7

// 의심가는 S/W 탐지		-- 현재 사용하지 않음
#define AKDETECT_DOUBT					8

// 런타임 코드변조를 탐지  -- HackType 에 대응되는 부가적인 구조체 없음 (단순 해쉬 비교이므로..)
#define AKDETECT_RUNCODE				9

/*--- 2006. 7. 19 김종호 define 2종 추가 ---*/
											
// API Hook 탐지 by HOOKDefender			
#define AKDETECT_HOOK_DETECT			10	
											
// API Hook 복원 실패 by HOOKDefender		
#define AKDETECT_HOOK_RESTORE_FAIL		11	

/*----------------------------------- END --*/

//
// 아래 구조체는 HOOKED_API_INFO 구조체를 구성하는 구조체들에 대한 정의임
//

/*
 *	EXPORT TABLE HOOKING 탐지시 훅 함수에 대한 정보를 넘기기 위한 구조체
 *
 *		jmp 코드를 사용하는 경우 EXPORTED_API_INFO.ReadAddressOfApi 값을 통해 탐지하므로
 *		OriginalAddr, HookdAddr 은 EXPORTED_API_INFO.ReadAddressOfApi 값이다. 
 *
**/ 
typedef struct _HOOKED_EXPORT_API
{
	TCHAR		DLLName[_MAX_PATH + 1];
	TCHAR		ApiName[_MAX_API_NAME + 1];	
	DWORD_PTR	OriginalAddr;				// hook 되지 않은 원래의 api 의 주소
	DWORD_PTR	HookdAddr;					// hooking 된 현재의 api 주소	
	INT			HOOKED_CODE;				// hooking 타입(inline? address modify, ...)
} HOOKED_EXPORT_API, *PHOOKED_EXPORT_API;
typedef BOOL (*lpfnHookedApiCB) (PHOOKED_EXPORT_API pHooked_API, PVOID pTag);


/*
 *	IMPORT TABLE HOOKING 탐지시 훅 함수에 대한 정보를 넘기기 위한 구조체
 *
**/ 
typedef struct _HOOKED_IMPORT_API
{
	TCHAR		DLLName[_MAX_PATH + 1];
	TCHAR		ApiName[_MAX_API_NAME + 1];		
	DWORD_PTR	HookdAddr;					// hooking 된 현재의 api 주소	
	DWORD_PTR	DetourAddr;					// jmp 할 곳의 주소 (INLINE HOOKING 타입인 경우만 유효)
	DWORD_PTR	dwStart;
	DWORD_PTR	dwEnd;
	INT			HOOKED_CODE;				// hooking 타입(inline? address modify, ...)
} HOOKED_IMPORT_API, *PHOOKED_IMPORT_API;
typedef BOOL (*lpfnHookedImportApiCB) (PHOOKED_IMPORT_API pHookedImportApi, PVOID pTag);

typedef union _UAPI_INFO
{
	HOOKED_EXPORT_API	HEA;
	HOOKED_IMPORT_API	HIA;	
} UAPI_INFO;


/*-----------------------------------------------------------------------------
 * ArmorKit 이 game client 에게 CallBack 으로 넘겨주는 데이터 구조체 
 *		-- game client 와 game server 쪽에서는 이 구조체만 알면 됨
**---------------------------------------------------------------------------*/

typedef struct _HACK_DETECT_INFO
{
	INT		HackType;		// 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
	INT		Size;			// Buffer 의 길이
	LPSTR	Buf;			// 데이터 버퍼	
} HACK_DETECT_INFO, *PHACK_DETECT_INFO;





/*-----------------------------------------------------------------------------
 * 스피드핵 체크를 위한 자료구조	- AKDETECT_SPEEDHACK
**---------------------------------------------------------------------------*/
typedef struct _SPEED_HACK_CHECK_INFO
{
	DWORD	dwTGT;			// TimeGetTime()
	DWORD	dwGTC;			// GetTickCount()
	DWORD	dwTSC;			// RDTSC
	DWORD	dwQPC;			// QueryPerformanceCounter()
} SPEED_HACK_CHECK_INFO, *PSPEED_HACK_CHECK_INFO;


/*-----------------------------------------------------------------------------
 * 프로세스 스캔을 위한 자료구조	- AKDETECT_MALWARE
**---------------------------------------------------------------------------*/
typedef struct _PROCESS_SCAN_INFO
{
	/// 프로세스 정보
	TCHAR	ProcessName[MAX_PATH + 1];
	DWORD	ProcessId;

	/// signature 정보	
	DWORD	SignatureIndex;
	DWORD	SianatureType;
	TCHAR	SignatureName[255];

	// 스트링은 널 터미네이트를 보장해야함

} PROCESS_SCAN_INFO, *PPROCESS_SCAN_INFO;



/*-----------------------------------------------------------------------------
 * PE 체크섬을 위한 자료구조		- AKDETECT_PEINTEGRITY
**---------------------------------------------------------------------------*/
typedef struct _PE_CHECKSUM_INFO
{
	DWORD HeaderSum;				// PE 헤더의 체크섬
	DWORD CalcSum ;					// 계산된 체크섬 (HeaderSum 과의 비교대상값)

} PE_CHECKSUM_INFO, *PPE_CHECKSUM_INFO;                 



/*-----------------------------------------------------------------------------
 * 디버그체크를 위한 자료구조		- AKDETECT_DEBUG
**---------------------------------------------------------------------------*/
typedef struct _ANTI_DEBUG_CHECK_INFO
{
	// 검사 방법 지정
	BOOL bSpecificAPI;			// 특별한 API 들을 사용한 탐지
	BOOL bProcessInfo;			// 프로세스 정보에 의한 탐지
	BOOL bParentProcessInfo;	// 부모 프로세스 정보를 통한 탐지
	BOOL bActiveDevices;		// Soft-Ice 류의 특정 디바이스 활성화 여부에 의한 탐지
	BOOL bSEH;					// SEH 에 의한 탐지 

	// 검사 결과값
	INT bResSpecificAPI;		// 특별한 API 들을 사용한 탐지 결과
	INT bResProcessInfo;		// 프로세스 정보에 의한 탐지 결과
	INT bResParentProcessInfo;	// 부모 프로세스 정보를 통한 탐지 결과
	INT bResActiveDevices;		// Soft-Ice 류의 특정 디바이스 활성화 여부에 의한 탐지 결과
	INT bResSEH;				// SEH 에 의한 탐지 결과
} ANTI_DEBUG_CHECK_INFO, *PANTI_DEBUG_CHECK_INFO;




/*-----------------------------------------------------------------------------
 *  AntiHook 						- AKDETECT_APIHOOK
 *
 *	HOOKED_API_INFO.API_TYPE 이 0 인 경우 HEA 구조체 참조
 *	                            1 인 경우 HIA 구조체 참조
**---------------------------------------------------------------------------*/

// 콜백함수로 넘기는 최종 구조체
//
typedef struct _HOOKED_API_INFO
{
	INT				API_TYPE;		// 0: EXPORTED API, 1: IMPORTED API, (필요시 추가가능)
	UAPI_INFO		API_INFO;		
} HOOKED_API_INFO, *PHOOKED_API_INFO;




/**---------------------------------------------------------------------------
	2006. 7. 19 김종호 HookDefender 관련 자료 구조 정의
----------------------------------------------------------------------------*/

#ifndef _MAX_FORWARDER_NAME
	#define _MAX_FORWARDER_NAME _MAX_FNAME * 2		// 그냥 넉넉하게 잡았다. -_-
#endif

#ifndef _OPCODE_DUMP_LENGTH
	#ifdef _DEBUG
		#define _OPCODE_DUMP_LENGTH	32	/* API 의 구현부 덤프 길이  */
	#else
		#define _OPCODE_DUMP_LENGTH	16	/* API 의 구현부 덤프 길이  */	
	#endif
#endif

#ifndef _OPCODE_DUMP_BUF_LENGTH
	#define _OPCODE_DUMP_BUF_LENGTH	( (_OPCODE_DUMP_LENGTH * 2 ) + sizeof(TCHAR) )	/* API 의 구현부 덤프를 보관할 버퍼의 길이  */
#endif

//
//	API Hook 타입
//
typedef enum _ENUM_HOOK_TYPE 
{
	HT_NONE, 
	HT_ENTRY_HOOK_INVALID_RANGE,	// Entry point 를 바꿔치기 한 경우(가장 잘 알려진 방식)
	HT_ENTRY_HOOK_USE_CAVE,			// cave (e.g. 패딩 영역)에 점프 코드 삽입후 entry 를 해당 cave 로 바꾸는 기법
	HT_INLINE_HOOK					// inline hook (a.k.a detours)		
} ENUM_HOOK_TYPE;


#define		HDERR_NONE				0		// 오류 없음
#define		HDERR_GENERIC			-1		// 일반 적인 오류 (SetErrorString() 으로 오류 정보 설정)
#define		HDERR_IAT_RESTORE		-2		// IMPORT TABLE 복원 실패
#define		HDERR_EAT_RESTORE		-3		// EXPORT TABLE 복원 실패 

#define		HDERR_INVALID_PE		-4		// 정상적인 PE 파일이 아님
#define		HDERR_NO_API_FOUNDED	-5		// API 를 PE 로부터 찾을 수 없음
#define		HDERR_INVALID_ADDRESS	-6		// VirtualProtect() 또는 해당 메모리 엑세스가 실패한 경우

// 
// API 복원을 위한 정보를 담고 있는 구조체
//
//	armorkit 에서 callback 을 통해서 넘겨준 HACK_DETECT_INFO.HackType 이 
//	AKDETECT_HOOK_DETECT 인 경우 사용된느 구조체
//	(binary stream 형태의 버퍼로 전달됨)
//
typedef struct _API_TO_RESTORE
{
	ENUM_HOOK_TYPE		HookType;
	
	TCHAR				DLLName[_MAX_PATH];
	INT					Ordinal;				// IAT 훅탐지시 API 가 서수로 import 된경우 사용됨 
	TCHAR				ApiName[_MAX_API_NAME];	// IAT 훅탐지시 API 가 이름으로 import 된경우 사용됨	
	DWORD_PTR			OriginalAddr;				// hook 되지 않은 원래의 api 의 주소
	DWORD_PTR			CurrentAddr;				// hooking 된 현재의 api 주소		
	DWORD_PTR			DetourAddr;					// 계산된 detour 의 주소
	TCHAR				OriginalOpcodes[_OPCODE_DUMP_BUF_LENGTH];
	TCHAR				CurrentOpcodes[_OPCODE_DUMP_BUF_LENGTH];
	TCHAR				DetourAddrOpcodes[_OPCODE_DUMP_BUF_LENGTH];	// IAT 에서는 사용안함. EAT 에서만 사용
	DWORD_PTR			ValidStartAddr;				// dll 의 유효 시작 주소
	DWORD_PTR			ValidEndAddr;				// dll 의 유효 끝 주소
} API_TO_RESTORE, *PAPI_TO_RESTORE;

//
// API 복원에 실패한 API 와 이에 대한 에러 정보를 담기 위한 구조체
//
//	armorkit 에서 callback 을 통해서 넘겨준 HACK_DETECT_INFO.HackType 이 
//	AKDETECT_HOOK_RESTORE_FAIL 인 경우 사용되는 구조체
//	(binary stream 형태의 버퍼로 전달됨)
//
typedef struct _RESTORE_FAULT_API
{
	INT				ErrorCode;
	API_TO_RESTORE	apiInfo;
} RESTORE_FAULT_API, *PRESTORE_FAULT_API;

#include <vector>
typedef std::vector<API_TO_RESTORE>		HOOKED_API_LIST;
typedef std::vector<RESTORE_FAULT_API>	HDFAULT_API_LIST;

/*--------------------------------------------------------------------------------------------------- END --*/
