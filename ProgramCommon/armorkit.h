/*-----------------------------------------------------------------------------
 * ArmorKit.h 
 *-----------------------------------------------------------------------------
 * ArmorKit 라이브러러 헤더 파일
 *	최초 작성 : 서승현 (truefroggie@gmail.com , shseo@webzen.co.kr)
 *	2005.11.16	노용환 
 *				- 프로젝트 구성 바꾸면서 헤더파일도 함께 정리함 (양이 많아 어떤걸 정리한지 잘 모름 ^_^)
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2005, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2005, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#ifndef _ARMOR_KIT_H_
#define _ARMOR_KIT_H_

/*-----------------------------------------------------------------------------
 * ArmorKit 버전 정보
**---------------------------------------------------------------------------*/
#define	AKVERSION_MJ_HIGH		1
#define AKVERSION_MJ_LOW		3
#define AKVERSION_MN_HIGH		0
#define AKVERSION_MN_LOW		4

/*-----------------------------------------------------------------------------
 * ArmorKit 실행 모드
**---------------------------------------------------------------------------*/
#define AKMODE_ONCE		1
#define AKMODE_TIMER	2
#define AKMODE_DAEMON	3
#define AKMODE_LOOP		4

/*-----------------------------------------------------------------------------
 * SUN online 에서 사용가능한 전송 버퍼의 최대 크기 (fixed size)
**---------------------------------------------------------------------------*/
#define SUN_BUF_SIZE	1024

/*-----------------------------------------------------------------------------
 * 탐지시간 (DWORD), 밀리세컨         (모드가 AKMODE_TIMER 인 경우)
**---------------------------------------------------------------------------*/
#define AKTIME_DEFAULT	30000			// 30초		(DEFAULT)
#define AKTIME_NORMAL	60000			//  1분
#define AKTIME_LONG		300000			// 5분
#define AKTIME_LONGLONG	60000*10		// 10분
// ArmorKits 타이머 아이디
//
#define AKIDT_TIMER0	10001


/*-----------------------------------------------------------------------------
 * 탐지플래그 (DWORD)	- 플래그에 따라 해당 기능 수행
**---------------------------------------------------------------------------*/
// 스피드핵 탐지
#define AKCHECK_SPEEDHACK				0x00000001

// Malware 탐지
#define AKCHECK_MALWARE					0x00000002

// 게임바이너리변조
#define AKCHECK_PEINTEGRITY				0x00000004

// 아머킷 변조
#define AKCHECK_AKINTEGRITY				0x00000008

// 디버그 상태탐지
#define AKCHECK_DEBUG					0x00000010

// 메모리조작
#define	AKCHECK_MEMEDIT					0x00000020

// 의심가는 S/W 탐지
#define AKCHECK_DOUBT					0x00000040

// 런타임 코드 변조를 탐지
#define AKCHECK_RUNCODE					0x00000080

// 게임함수변조
#define AKCHECK_IAT_HOOK				0x00000100
#define AKCHECK_EAT_HOOK				0x00000200

// 모두체크
#define AKCHECK_ALL						AKCHECK_SPEEDHACK |			\
										AKCHECK_MALWARE |			\
										AKCHECK_PEINTEGRITY |		\
										AKCHECK_AKINTEGRITY |		\
										AKCHECK_DEBUG |				\
										AKCHECK_MEMEDIT |			\
										AKCHECK_DOUBT |				\
										AKCHECK_RUNCODE|			\
										AKCHECK_IAT_HOOK|			\
										AKCHECK_EAT_HOOK


/*-----------------------------------------------------------------------------
 * ArmorKit 사용자 정의 프로시져 결과값 정의 (HackType)
**---------------------------------------------------------------------------*/
// ArmorKit 내부 에러 발생
#define AKRUNTIME_ERROR					-1

// 스피드핵 탐지
#define AKDETECT_SPEEDHACK				1

// Malware 탐지
#define AKDETECT_MALWARE				2

// 게임바이너리변조
// 이 인증을 하려면 VC 컴파일러의 Linker 옵션에 /RELEASE 가 추가되어야 한다.
// 주어진 화일의 첵섬을 계산한다.
#define AKDETECT_PEINTEGRITY			3

// 게임함수변조
#define AKDETECT_APIHOOK				4

// 아머킷 변조
#define AKDETECT_AKINTEGRITY			5

// 디버그 상태탐지
#define AKDETECT_DEBUG					6

// 메모리조작
#define AKDETECT_MEMEDIT				7

// 의심가는 S/W 탐지
#define AKDETECT_DOUBT					8

// 런타임 코드변조를 탐지
#define AKDETECT_RUNCODE				9

// API Hook 탐지 by HOOKDefender
// 
#define AKDETECT_HOOK_DETECT			10

// API Hook 복원 실패 by HOOKDefender
// 
#define AKDETECT_HOOK_RESTORE_FAIL		11


// touch service
#define AKDETECT_TOUCH					100

//
// 탐지한 해킹에 대한 정보를 넘겨주는 구조체 
//	-- wops 연동에 필요한 구조체임
//
typedef struct _HACK_DETECT_INFO
{
	INT		HackType;		// 탐지한 해킹 타입  (e.g. AKDETECT_SPEEDHACK)
	INT		Size;			// Buffer 의 길이
	LPSTR	Buf;			// 데이터 버퍼	
} HACK_DETECT_INFO, *PHACK_DETECT_INFO;


/*-----------------------------------------------------------------------------
 * Malicious S/W 의 타입 
**---------------------------------------------------------------------------*/
// 키로거류 
#define TYPE_KEYLOGER	1	

// 메모리에디터
#define	TYPE_MEMEDITOR	2	

// 디버거 
#define TYPE_DEBUGGER	3	

// 오토마우스 
#define TYPE_AUTOMOUSE	4	

// 스피드핵
#define TYPE_SPEEDHACK	5	

// 기타, 미분류 
#define TYPE_UNKNOWN	99


/*-----------------------------------------------------------------------------
 * 에러 코드 정의
**---------------------------------------------------------------------------*/
// 2006. 07. 03  삭제대상임 AKError.h로 옮겨주삼
#define AKERROR_INIT_FAIL					-1
#define AKERROR_GENERAL_APIFAIL				-2		// Windows API함수 호출이 실패했음
#define	AKERROR_SET_PRIVILEGE				-11		// 프로세스를 스캔하기 위해 권한 상승에 실패하였음, 
													// 관리자권한으로 프로그램을 실행시키는 것이 요구됨
#define AKERROR_INIT_INVALID_HANDLE			-13		// 초기화변수의 HWND 가 유효하지 않음
#define AKERROR_PECHKSUM_ZERO				-14		// PE헤더의 체크섬이 0입니다. 
#define AKERROR_FILEFROMID_GETFAIL			-16		// ID로 부터 화일을 가져오는데 실패했습니다.

#ifndef ret_true
#define ret_true	1		// 리턴코드 : 성공
#endif
#ifndef ret_false
#define ret_false	0		// 함수가 실패하거나 결과값이 없음
#endif


/*-----------------------------------------------------------------------------
 * 자료 구조 정의
**---------------------------------------------------------------------------*/

// 디버그체크를 위한 자료구조
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



// PE 체크섬을 위한 자료구조
typedef struct _PE_CHECKSUM_INFO
{
	DWORD HeaderSum;				// PE 헤더의 체크섬
	DWORD CalcSum ;					// 계산된 체크섬 (HeaderSum 과의 비교대상값)

} PE_CHECKSUM_INFO, *PPE_CHECKSUM_INFO;                 


// 스피드핵 체크를 위한 자료구조
typedef struct _SPEED_HACK_CHECK_INFO
{
	DWORD	dwTGT;			// TimeGetTime()
	DWORD	dwGTC;			// GetTickCount()
	DWORD	dwTSC;			// RDTSC
	DWORD	dwQPC;			// QueryPerformanceCounter()
} SPEED_HACK_CHECK_INFO, *PSPEED_HACK_CHECK_INFO;



// 해쉬값 체크를 위한 자료구조
typedef struct _PE_HASH_INFO
{
	DWORD	HeaderSum;				// PE 헤더의 체크섬
	BYTE	HashBytes[256];		// 해쉬 스트링
	DWORD	HashLen ;
} PE_HASH_INFO, *PPE_HASH_INFO;                 


// 프로세스 스캔을 위한 자료구조
typedef struct _PROCESS_SCAN_INFO
{
	/// 프로세스 정보
	TCHAR	ProcessName[MAX_PATH + 1];
	DWORD	ProcessId;

	/// signature 정보	
	DWORD	SignatureIndex;
	DWORD	SianatureType;
	TCHAR	SignatureName[MAX_PATH + 1];

	// 스트링은 널 터미네이트를 보장해야함

} PROCESS_SCAN_INFO, *PPROCESS_SCAN_INFO;

/*-----------------------------------------------------------------------------
 * checkspeedhack 헤더
**---------------------------------------------------------------------------*/
// tick 타이머가 10초이상 차이나면 이상으로 간주하고 보고 
#define MAX_TICK_ELAPSE		10

// SYSTEM_POWER_STATUS 의 AC파워관련 정의
#define AC_OFFLINE	0
#define AC_ONLINE	1

// 밀리세컨드 단위
#define	UNIT_MILISECOND	1000

// 밀리세컨드 클럭 측정단위
#define UNIT_CLOCKGAP	10

// 밀리 세컨드 한계차이
#define VAL_ELAPSE		50

// QueryPerformanceFreqeuncy 값들
#define FREQ_WIN98		1193000
#define FREQ_WINNT		3579545 
#define FREQ_WINXP		// 3579545  or CPU Clock
#define FREQ_WIN2000	3579545

INT CheckSpeedHack( IN PSPEED_HACK_CHECK_INFO, IN DWORD MaxTickElapse );


/*-----------------------------------------------------------------------------
 * scanproc 헤더
**---------------------------------------------------------------------------*/
#define MAX_SCANBUFF	256

// 디버깅을 위한 가비지 캐릭터
#define bGarbage	'\xCC'
#define MAX_SIGLEN		256

// Malicious S/W 를 탐지하기 위한 탐지시그니쳐 ( runtime process의 memory based )
//
typedef struct _PROC_SIGNATURE
{
	INT		dwIdx;					// signature’s unique number
	DWORD	ntype;					// signature 's type
	TCHAR	lptszSigName[MAX_SIGLEN];			// signature name
	DWORD 	dwOffset;				// 프로세스상에서의 signature 의 offset (from Image base address)
	DWORD 	dwSigLen;				// lpszSignature length
	TCHAR	lpszSignature[MAX_SIGLEN * 2];			// 실제 signature 	
} PROC_SIGNATURE , *PPROC_SIGNATURE;

/*
* example
*
*
* PROC_SIGNATURE procsigs[] = {
*	{ 1, TYPE_MEMEDIT, "tsearch.exe", "Tsearch 1.6", 0x4000ff32 , 21, "TSEARCH 1.6 ", NULL } , 
*  { 2, TYPE_MEMEDIT, "cheatengine.exe", "Cheat Engine 4.4", 0x3224241 , 22, "Cheat Engine 4.4", NULL } ,
*  ... , ... 
*  { -1, 0, NULL , 0, 0, NULL, NULL }
*  } ;

*/


// ScanDoubtProcessList() 에서 사용되는 의심되는 함수를 찾아내기 위한 자료구조 
//
typedef struct _PROC_DOUBTAPI {
	TCHAR	*lptszFuncName ;
	TCHAR	*lptszDllName ;
	DWORD	Reserved ;
} PROC_DOUBTAPI, *PPROC_DOUBTAPI ;

// 프로세스 스캔을 위한 콜백 함수 
//
//typedef BOOL (*lpfnProcessScanCB) (PPROCESS_SCAN_INFO pPSI, PVOID pTag);

//INT		ScanProcessList( lpfnProcessScanCB pfnPSCB, PVOID pTag ) ;
INT ScanProcessList( PPROCESS_SCAN_INFO pPSI );

// 게임 프로세스의 모듈을 체크
INT	CheckProxyDll(PBOOL pbResult, PPROCESS_SCAN_INFO pPSI);



/*-----------------------------------------------------------------------------
 *  AntiHook 관련 
 *
 *	HOOKED_API_INFO.API_TYPE 이 0 인 경우 HEA 구조체 참조
 *	                            1 인 경우 HIA 구조체 참조
**---------------------------------------------------------------------------*/


/**	-----------------------------------------------------------------------
	HOOKED_API_INFO.API_TYPE == AT_EXPORTED 인 경우 사용되는 구조체
	EXPORT API 후킹이 탐지 경우
	
	jmp 코드를 사용하는 경우 EXPORTED_API_INFO.ReadAddressOfApi 값을 통해 탐지하므로
	OriginalAddr, HookdAddr 은 EXPORTED_API_INFO.ReadAddressOfApi 값이다. 
-------------------------------------------------------------------------*/

typedef struct _HOOKED_EXPORT_API
{
	INT			HOOKED_CODE;				// hooking 타입(inline? address modify, ...)
	
	TCHAR		DLLName[MAX_PATH];
	TCHAR		ApiName[MAX_PATH];	
	DWORD_PTR	OriginalAddr;				// hook 되지 않은 원래의 api 의 주소
	DWORD_PTR	HookdAddr;					// hooking 된 현재의 api 주소		
} HOOKED_EXPORT_API, *PHOOKED_EXPORT_API;
typedef BOOL (*lpfnHookedApiCB) (PHOOKED_EXPORT_API pHooked_API, PVOID pTag);

/**	-----------------------------------------------------------------------
	HOOKED_API_INFO.API_TYPE == AT_IMPORTED 인 경우 사용되는 구조체
	IMPORT API 후킹이 탐지 경우
-------------------------------------------------------------------------*/
typedef struct _HOOKED_IMPORT_API
{
	INT			HOOKED_CODE;				// hooking 타입(inline? address modify, ...)
	
	TCHAR		DLLName[_MAX_PATH];
	TCHAR		ApiName[MAX_PATH];		
	DWORD_PTR	HookdAddr;					// hooking 된 현재의 api 주소	
	DWORD_PTR	DetourAddr;					// jmp 할 곳의 주소 (INLINE HOOKING 타입인 경우만 유효)
	DWORD_PTR	dwStart;
	DWORD_PTR	dwEnd;	
} HOOKED_IMPORT_API, *PHOOKED_IMPORT_API;
typedef BOOL (*lpfnHookedImportApiCB) (PHOOKED_IMPORT_API pHookedImportApi, PVOID pTag);

/**	-----------------------------------------------------------------------
	HOOKED_API_INFO.API_TYPE == AT_INTEGRITY 인 경우 사용되는 구조체
	원래의 임포트 모듈갯수와 현재의 임포트 모듈 갯수가 다른 경우를 탐지
-------------------------------------------------------------------------*/
typedef struct _IMP_MOD_INTEGRITY
{
	INT OrgImpModCount;
	INT CurImpModCount;
} IMP_MOD_INTEGRITY, *PIMP_MOD_INTEGRITY;


typedef union _UAPI_INFO
{
	HOOKED_EXPORT_API	HEA;			// if (API_TYPE == AT_EXPORTED)
	HOOKED_IMPORT_API	HIA;			// if (API_TYPE == AT_IMPORTED)
	IMP_MOD_INTEGRITY	IMI;			// if (API_TYPE == AT_INTEGRITY)
} UAPI_INFO;

// API_TYPE 정의
//
#define		AT_EXPORTED			0	// exported api 
#define		AT_IMPORTED			1	// imported api
#define		AT_INTEGRITY		2	// IAT 의 변경 유무 검사

// 콜백함수로 넘기는 최종 구조체
//
typedef struct _HOOKED_API_INFO
{
	INT				API_TYPE;		// 0: EXPORTED API, 1: IMPORTED API, 2: AT_INTEGRITY
	UAPI_INFO		API_INFO;		
} HOOKED_API_INFO, *PHOOKED_API_INFO;


// API 리턴값
//
#define		RETCODE_LIB_SUCCESS				0				// 성공
#define		RETCODE_LIB_ERROR				-1				// 실패

// 후킹 체크함수 리턴값
//
#define		RETCODE_NOT_HOOKED				0				// 후킹된 API 가 없음
#define		RETCODE_API_HOOKED				1				// API 후킹이 탐지되었음

// HOOKED_CODE 정의
//
//	AT_EXPORTED 인 경우
//
#define		AH_EAT_MODIFIED					RETCODE_API_HOOKED				// EAT 주소값 변조 탐지
#define		AH_EAT_INLINE_HOOKED			-2				// EAT 인라인 후킹 탐지
#define		AH_EAT_COUNT_ERROR				-3				// Disk 와 VAS 상의 Export 된 api 카운트가 다름		
#define		AH_EAT_COUNT_ZERO				-4				// Export 된 api 가 없다 ! (이런 경우가 생길순 없다. )

//	AT_IMPORTED 인 경우
//
#define		AH_IAT_MODIFIED					RETCODE_API_HOOKED				// IAT 주소값 변조 탐지됨
#define		AH_IAT_INLINE_HOOKED			-6				// IAT 인라인 후킹 탐지


/**	-----------------------------------------------------------------------
	\brief	touch service 일때 touch 결과값을 리턴		
-------------------------------------------------------------------------*/
int GetTouchValue(int seed);

/**	-----------------------------------------------------------------------
	\brief	에러메세지 문자열 리턴
-------------------------------------------------------------------------*/
LPCTSTR GetErrorString(void);






///////////////////////////////////////////////////////////////////////////////
// 공통 상수 정의 
//		AkCommon.h 와 중복됨
///////////////////////////////////////////////////////////////////////////////

// 파일이름, 경로에 관한 상수 정의 
// from stdlib.h
//
#ifndef _INC_STDLIB		// stdlib.h
	#define _MAX_PATH   260 /* max. length of full pathname */
	#define _MAX_DRIVE  3   /* max. length of drive component */
	#define _MAX_DIR    256 /* max. length of path component */
	#define _MAX_FNAME  256 /* max. length of file name component */
	#define _MAX_EXT    256 /* max. length of extension component */	
#endif

#ifndef _MAX_API_NAME
	#define _MAX_API_NAME	256	/* max length of api name */
#endif

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

#ifndef MAX_SIG_NAME
	#define MAX_SIG_NAME	255
#endif



///////////////////////////////////////////////////////////////////////////////
// HDCommon.h																 
///////////////////////////////////////////////////////////////////////////////
#ifndef _HOOK_DEFENDER_COMMON_H_
#define _HOOK_DEFENDER_COMMON_H_


//
// 에러 코드 정의
//
#define		HDERR_NONE				0		// 오류 없음
#define		HDERR_GENERIC			-1		// 일반 적인 오류 (SetErrorString() 으로 오류 정보 설정)
#define		HDERR_IAT_RESTORE		-2		// IMPORT TABLE 복원 실패
#define		HDERR_EAT_RESTORE		-3		// EXPORT TABLE 복원 실패 

#define		HDERR_INVALID_PE		-4		// 정상적인 PE 파일이 아님
#define		HDERR_NO_API_FOUNDED	-5		// API 를 PE 로부터 찾을 수 없음
#define		HDERR_INVALID_ADDRESS	-6		// VirtualProtect() 또는 해당 메모리 엑세스가 실패한 경우



//
// API Hook 타입 
// 
typedef enum _ENUM_HOOK_TYPE 
{
	HT_NONE, 
	HT_ENTRY_HOOK_INVALID_ADDR_IAT,	// Entry point 를 바꿔치기 한 경우(가장 잘 알려진 방식)
	HT_ENTRY_HOOK_INVALID_ADDR_EAT,
	HT_ENTRY_HOOK_USE_CAVE_EAT,		// cave (e.g. 패딩 영역)에 점프 코드 삽입후 entry 를 해당 cave 로 바꾸는 기법
	HT_INLINE_HOOK_EAT				// inline hook (a.k.a detours)		
} ENUM_HOOK_TYPE;


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

#endif	// _HOOK_DEFENDER_COMMON_H_




#endif
