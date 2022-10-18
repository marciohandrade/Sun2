/*-----------------------------------------------------------------------------
 * typedef_AKv1HackDetect.h 
 *-----------------------------------------------------------------------------
 * ArmorKit v1의 헤더 파일
 *	최초 작성 : 서승현 (truefroggie@gmail.com , shseo@webzen.co.kr)
 *	2005.11.16	노용환 
 *				- 프로젝트 구성 바꾸면서 헤더파일도 함께 정리함 (양이 많아 어떤걸 정리한지 잘 모름 ^_^)
 *	2007.03.21	박세율
 *				- ArmorKit.h파일이 변경되면 본 파일도 수정해줘야 함
 *	2007.07.13	박세율
 *				- WOPS에서 AKDETECT_PEINTEGRITY가 AKDETECT_ERROR로 변경됨에 따라 상수선언변경
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2005, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2005, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#ifndef _TYPEDEF_AKV1_HACK_DETECT_H_
#define _TYPEDEF_AKV1_HACK_DETECT_H_


/*-----------------------------------------------------------------------------
 * WOPS와 연동을 위한 RTT 상수 선언
**---------------------------------------------------------------------------*/
#define V1_SUN_BUF_SIZE					1024

#define V1_MAX_SIG_NAME					255

#define V1__MAX_PATH					260

#define V1__MAX_DRIVE					3

#define V1__MAX_DIR						256

#define V1__MAX_FNAME					256

#define V1__MAX_EXT						256

#define V1__MAX_API_NAME				256

#define V1__MAX_FORWARDER_NAME			V1_MAX_FNAME * 2

#define V1__OPCODE_DUMP_LENGTH			16

#define V1__OPCODE_DUMP_BUF_LENGTH		((V1__OPCODE_DUMP_LENGTH * 2) + sizeof(TCHAR))

#define V1_MAX_SCANBUFF					256

#define V1_bGarbage						'\xCC'

#define V1_MAX_SIGLEN					256

#define	AT_EXPORTED						0

#define V1_AT_IMPORTED					1

#define	V1_AT_INTEGRITY					2

#define	V1_RETCODE_LIB_SUCCESS			0

#define	V1_RETCODE_LIB_ERROR			-1

#define	V1_RETCODE_NOT_HOOKED			0

#define	V1_RETCODE_API_HOOKED			1

#define	V1_AH_EAT_MODIFIED				V1_RETCODE_API_HOOKED

#define	V1_AH_EAT_INLINE_HOOKED			-2

#define	V1_AH_EAT_COUNT_ERROR			-3

#define	V1_AH_EAT_COUNT_ZERO			-4

#define	V1_AH_IAT_MODIFIED				V1_RETCODE_API_HOOKED

#define	V1_AH_IAT_INLINE_HOOKED			-6

#define V1_ATR_STREAM_LEN				sizeof(int)


/*-----------------------------------------------------------------------------
 * 탐지플래그 (DWORD)	- 플래그에 따라 해당 기능 수행
**---------------------------------------------------------------------------*/
#define V1_AKCHECK_SPEEDHACK			0x00000001		// 스피드핵 탐지

#define V1_AKCHECK_MALWARE				0x00000002		// Malware 탐지

#define V1_AKCHECK_PEINTEGRITY			0x00000004		// 게임바이너리변조

#define V1_AKCHECK_AKINTEGRITY			0x00000008		// 아머킷 변조

#define V1_AKCHECK_DEBUG				0x00000010		// 디버그 상태탐지

#define	V1_AKCHECK_MEMEDIT				0x00000020		// 메모리조작

#define V1_AKCHECK_DOUBT				0x00000040		// 의심가는 S/W 탐지

#define V1_AKCHECK_RUNCODE				0x00000080		// 런타임 코드 변조를 탐지

#define V1_AKCHECK_IAT_HOOK				0x00000100		// 게임함수변조

#define V1_AKCHECK_EAT_HOOK				0x00000200

#define V1_AKCHECK_ALL					V1_AKCHECK_SPEEDHACK   |	\
										V1_AKCHECK_MALWARE     |	\
										V1_AKCHECK_PEINTEGRITY |	\
										V1_AKCHECK_AKINTEGRITY |	\
										V1_AKCHECK_DEBUG       |	\
										V1_AKCHECK_MEMEDIT     |	\
										V1_AKCHECK_DOUBT       |	\
										V1_AKCHECK_RUNCODE     |	\
										V1_AKCHECK_IAT_HOOK    |	\
										V1_AKCHECK_EAT_HOOK


/*-----------------------------------------------------------------------------
 * ArmorKit 사용자 정의 프로시져 결과값 정의 (HackType)
**---------------------------------------------------------------------------*/
#define V1_AKRUNTIME_ERROR				-1				// ArmorKit 내부 에러 발생

#define V1_AKDETECT_SPEEDHACK			1				// 스피드핵 탐지

#define V1_AKDETECT_MALWARE				2				// Malware 탐지

#define V1_AKDETECT_ERROR				3				// 아머킷 에러

#define V1_AKDETECT_APIHOOK				4				// 게임함수변조

#define V1_AKDETECT_AKINTEGRITY			5				// 아머킷 변조

#define V1_AKDETECT_DEBUG				6				// 디버그 상태탐지

#define V1_AKDETECT_MEMEDIT				7				// 메모리조작

#define V1_AKDETECT_DOUBT				8				// 의심가는 S/W 탐지

#define V1_AKDETECT_RUNCODE				9				// 런타임 코드변조를 탐지

#define V1_AKDETECT_HOOK_DETECT			10				// API Hook 탐지 by HOOKDefender

#define V1_AKDETECT_HOOK_RESTORE_FAIL	11				// API Hook 복원 실패 by HOOKDefender

#define V1_AKDETECT_KABUSE				12				// by somma. kernel mode abuse detected

#define V1_AKDETECT_TOUCH				100				// touch service


/*-----------------------------------------------------------------------------
 * Malicious S/W 의 타입 
**---------------------------------------------------------------------------*/
#define V1_TYPE_KEYLOGER				1				// 키로거류 

#define	V1_TYPE_MEMEDITOR				2				// 메모리에디터

#define V1_TYPE_DEBUGGER				3				// 디버거 

#define V1_TYPE_AUTOMOUSE				4				// 오토마우스 

#define V1_TYPE_SPEEDHACK				5				// 스피드핵

#define V1_TYPE_UNKNOWN					99				// 기타, 미분류 


/*-----------------------------------------------------------------------------
 * HookDefender의 에러 타입
**---------------------------------------------------------------------------*/
#define	V1_HDERR_NONE					0				// 오류 없음

#define V1_HDERR_GENERIC				-1				// 일반 적인 오류 (SetErrorString() 으로 오류 정보 설정)
	
#define V1_HDERR_IAT_RESTORE			-2				// IMPORT TABLE 복원 실패

#define V1_HDERR_EAT_RESTORE			-3				// EXPORT TABLE 복원 실패 

#define V1_HDERR_INVALID_PE				-4				// 정상적인 PE 파일이 아님

#define V1_HDERR_NO_API_FOUNDED			-5				// API 를 PE 로부터 찾을 수 없음

#define V1_HDERR_INVALID_ADDRESS		-6				// VirtualProtect() 또는 해당 메모리 엑세스가 실패한 경우


/*-----------------------------------------------------------------------------
 * 에러 코드 정의
**---------------------------------------------------------------------------*/
#define V1_AKERROR_INIT_FAIL			-1

#define V1_AKERROR_GENERAL_APIFAIL		-2			// Windows API함수 호출이 실패했음

#define	V1_AKERROR_SET_PRIVILEGE			-11			// 프로세스를 스캔하기 위해 권한 상승에 실패하였음, 

#define V1_AKERROR_INIT_INVALID_HANDLE	-13			// 초기화변수의 HWND 가 유효하지 않음

#define V1_AKERROR_PECHKSUM_ZERO		-14			// PE헤더의 체크섬이 0입니다. 

#define V1_AKERROR_FILEFROMID_GETFAIL	-16			// ID로 부터 화일을 가져오는데 실패했습니다.



/*-----------------------------------------------------------------------------
 * 자료 구조 정의
**---------------------------------------------------------------------------*/

//
// 탐지한 해킹에 대한 정보를 넘겨주는 구조체 
//
typedef struct _HACK_DETECT_INFO_V1
{
	INT		HackType;
	INT		Size;
	LPSTR	Buf;

} HACK_DETECT_INFO_V1, *PHACK_DETECT_INFO_V1;

//
// 디버그체크를 위한 자료구조
//
typedef struct _ANTI_DEBUG_CHECK_INFO_V1
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

} ANTI_DEBUG_CHECK_INFO_V1, *PANTI_DEBUG_CHECK_INFO_V1;


//
// PE 체크섬을 위한 자료구조
//
typedef struct _PE_CHECKSUM_INFO_V1
{
	DWORD HeaderSum;				// PE 헤더의 체크섬
	DWORD CalcSum ;					// 계산된 체크섬 (HeaderSum 과의 비교대상값)

} PE_CHECKSUM_INFO_V1, *PPE_CHECKSUM_INFO_V1;                 


//
// 스피드핵 체크를 위한 자료구조
//
typedef struct _SPEED_HACK_CHECK_INFO_V1
{
	DWORD	dwTGT;			// TimeGetTime()
	DWORD	dwGTC;			// GetTickCount()
	DWORD	dwTSC;			// RDTSC
	DWORD	dwQPC;			// QueryPerformanceCounter()

} SPEED_HACK_CHECK_INFO_V1, *PSPEED_HACK_CHECK_INFO_V1;


//
// 해쉬값 체크를 위한 자료구조
//
typedef struct _PE_HASH_INFO_V1
{
	DWORD	HeaderSum;
	BYTE	HashBytes[256];
	DWORD	HashLen ;

} PE_HASH_INFO_V1, *PPE_HASH_INFO_V1;                 


//
// 프로세스 스캔을 위한 자료구조
//
typedef struct _PROCESS_SCAN_INFO_V1
{
	TCHAR	ProcessName[MAX_PATH + 1];
	DWORD	ProcessId;
	DWORD	SignatureIndex;
	DWORD	SianatureType;
	TCHAR	SignatureName[MAX_PATH + 1];

} PROCESS_SCAN_INFO_V1, *PPROCESS_SCAN_INFO_V1;


//
// ScanDoubtProcessList() 에서 사용되는 의심되는 함수를 찾아내기 위한 자료구조 
//
typedef struct _PROC_DOUBTAPI_V1 {
	TCHAR	*lptszFuncName ;
	TCHAR	*lptszDllName ;
	DWORD	Reserved ;

} PROC_DOUBTAPI_V1, *PPROC_DOUBTAPI_V1;


//
// HOOKED_API_INFO.API_TYPE == AT_EXPORTED 인 경우 사용되는 구조체
//
typedef struct _HOOKED_EXPORT_API_V1
{
	INT			HOOKED_CODE;				// hooking 타입(inline? address modify, ...)	
	TCHAR		DLLName[MAX_PATH];
	TCHAR		ApiName[MAX_PATH];	
	DWORD_PTR	OriginalAddr;				// hook 되지 않은 원래의 api 의 주소
	DWORD_PTR	HookdAddr;					// hooking 된 현재의 api 주소	

} HOOKED_EXPORT_API_V1, *PHOOKED_EXPORT_API_V1;


//
// HOOKED_API_INFO.API_TYPE == AT_IMPORTED 인 경우 사용되는 구조체
//
typedef struct _HOOKED_IMPORT_API_V1
{
	INT			HOOKED_CODE;				// hooking 타입(inline? address modify, ...)	
	TCHAR		DLLName[V1__MAX_PATH];
	TCHAR		ApiName[MAX_PATH];		
	DWORD_PTR	HookdAddr;					// hooking 된 현재의 api 주소	
	DWORD_PTR	DetourAddr;					// jmp 할 곳의 주소 (INLINE HOOKING 타입인 경우만 유효)
	DWORD_PTR	dwStart;
	DWORD_PTR	dwEnd;	

} HOOKED_IMPORT_API_V1, *PHOOKED_IMPORT_API_V1;


//
// HOOKED_API_INFO.API_TYPE == AT_INTEGRITY 인 경우 사용되는 구조체
//
typedef struct _IMP_MOD_INTEGRITY_V1
{
	INT OrgImpModCount;
	INT CurImpModCount;

} IMP_MOD_INTEGRITY_V1, *PIMP_MOD_INTEGRITY_V1;


//
//
//
typedef union _UAPI_INFO_V1
{
	HOOKED_EXPORT_API_V1	HEA;		// if (API_TYPE == AT_EXPORTED)
	HOOKED_IMPORT_API_V1	HIA;		// if (API_TYPE == AT_IMPORTED)
	IMP_MOD_INTEGRITY_V1	IMI;		// if (API_TYPE == AT_INTEGRITY)

} UAPI_INFO_V1;


//
// 콜백함수로 넘기는 최종 구조체
//
typedef struct _HOOKED_API_INFO_V1
{
	INT				API_TYPE;
	UAPI_INFO_V1	API_INFO;		

} HOOKED_API_INFO_V1, *PHOOKED_API_INFO_V1;


//
// API Hook 타입 
// 
typedef enum _ENUM_HOOK_TYPE_V1
{
	V1_HT_NONE, 
	V1_HT_ENTRY_HOOK_INVALID_ADDR_IAT,
	V1_HT_ENTRY_HOOK_INVALID_ADDR_EAT,
	V1_HT_ENTRY_HOOK_USE_CAVE_EAT,
	V1_HT_INLINE_HOOK_EAT
}ENUM_HOOK_TYPE_V1;


// 
// API 복원을 위한 정보를 담고 있는 구조체
//
typedef struct _API_TO_RESTORE_V1
{
	ENUM_HOOK_TYPE_V1	HookType;	
	TCHAR				DLLName[V1__MAX_PATH];
	INT					Ordinal;
	TCHAR				ApiName[V1__MAX_API_NAME];
	DWORD_PTR			OriginalAddr;
	DWORD_PTR			CurrentAddr;
	DWORD_PTR			DetourAddr;
	TCHAR				OriginalOpcodes[V1__OPCODE_DUMP_BUF_LENGTH];
	TCHAR				CurrentOpcodes[V1__OPCODE_DUMP_BUF_LENGTH];
	TCHAR				DetourAddrOpcodes[V1__OPCODE_DUMP_BUF_LENGTH];
	DWORD_PTR			ValidStartAddr;
	DWORD_PTR			ValidEndAddr;

} API_TO_RESTORE_V1, *PAPI_TO_RESTORE_V1;


//
// API 복원에 실패한 API 와 이에 대한 에러 정보를 담기 위한 구조체
//
typedef struct _RESTORE_FAULT_API_V1
{
	INT					ErrorCode;
	API_TO_RESTORE_V1	apiInfo;

} RESTORE_FAULT_API_V1, *PRESTORE_FAULT_API_V1;

#include <vector>
typedef std::vector<RESTORE_FAULT_API_V1>	HDFAULT_API_LIST;

#endif	//	_TYPEDEF_AKV1_HACK_DETECT_H_
