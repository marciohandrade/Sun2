/*-----------------------------------------------------------------------------
 * ArmorKit.h 
 *-----------------------------------------------------------------------------
 * ArmorKit ���̺귯�� ��� ����
 *	���� �ۼ� : ������ (truefroggie@gmail.com , shseo@webzen.co.kr)
 *	2005.11.16	���ȯ 
 *				- ������Ʈ ���� �ٲٸ鼭 ������ϵ� �Բ� ������ (���� ���� ��� �������� �� �� ^_^)
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
 * ArmorKit ���� ����
**---------------------------------------------------------------------------*/
#define	AKVERSION_MJ_HIGH		1
#define AKVERSION_MJ_LOW		3
#define AKVERSION_MN_HIGH		0
#define AKVERSION_MN_LOW		4

/*-----------------------------------------------------------------------------
 * ArmorKit ���� ���
**---------------------------------------------------------------------------*/
#define AKMODE_ONCE		1
#define AKMODE_TIMER	2
#define AKMODE_DAEMON	3
#define AKMODE_LOOP		4

/*-----------------------------------------------------------------------------
 * SUN online ���� ��밡���� ���� ������ �ִ� ũ�� (fixed size)
**---------------------------------------------------------------------------*/
#define SUN_BUF_SIZE	1024

/*-----------------------------------------------------------------------------
 * Ž���ð� (DWORD), �и�����         (��尡 AKMODE_TIMER �� ���)
**---------------------------------------------------------------------------*/
#define AKTIME_DEFAULT	30000			// 30��		(DEFAULT)
#define AKTIME_NORMAL	60000			//  1��
#define AKTIME_LONG		300000			// 5��
#define AKTIME_LONGLONG	60000*10		// 10��
// ArmorKits Ÿ�̸� ���̵�
//
#define AKIDT_TIMER0	10001


/*-----------------------------------------------------------------------------
 * Ž���÷��� (DWORD)	- �÷��׿� ���� �ش� ��� ����
**---------------------------------------------------------------------------*/
// ���ǵ��� Ž��
#define AKCHECK_SPEEDHACK				0x00000001

// Malware Ž��
#define AKCHECK_MALWARE					0x00000002

// ���ӹ��̳ʸ�����
#define AKCHECK_PEINTEGRITY				0x00000004

// �Ƹ�Ŷ ����
#define AKCHECK_AKINTEGRITY				0x00000008

// ����� ����Ž��
#define AKCHECK_DEBUG					0x00000010

// �޸�����
#define	AKCHECK_MEMEDIT					0x00000020

// �ǽɰ��� S/W Ž��
#define AKCHECK_DOUBT					0x00000040

// ��Ÿ�� �ڵ� ������ Ž��
#define AKCHECK_RUNCODE					0x00000080

// �����Լ�����
#define AKCHECK_IAT_HOOK				0x00000100
#define AKCHECK_EAT_HOOK				0x00000200

// ���üũ
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
 * ArmorKit ����� ���� ���ν��� ����� ���� (HackType)
**---------------------------------------------------------------------------*/
// ArmorKit ���� ���� �߻�
#define AKRUNTIME_ERROR					-1

// ���ǵ��� Ž��
#define AKDETECT_SPEEDHACK				1

// Malware Ž��
#define AKDETECT_MALWARE				2

// ���ӹ��̳ʸ�����
// �� ������ �Ϸ��� VC �����Ϸ��� Linker �ɼǿ� /RELEASE �� �߰��Ǿ�� �Ѵ�.
// �־��� ȭ���� ý���� ����Ѵ�.
#define AKDETECT_PEINTEGRITY			3

// �����Լ�����
#define AKDETECT_APIHOOK				4

// �Ƹ�Ŷ ����
#define AKDETECT_AKINTEGRITY			5

// ����� ����Ž��
#define AKDETECT_DEBUG					6

// �޸�����
#define AKDETECT_MEMEDIT				7

// �ǽɰ��� S/W Ž��
#define AKDETECT_DOUBT					8

// ��Ÿ�� �ڵ庯���� Ž��
#define AKDETECT_RUNCODE				9

// API Hook Ž�� by HOOKDefender
// 
#define AKDETECT_HOOK_DETECT			10

// API Hook ���� ���� by HOOKDefender
// 
#define AKDETECT_HOOK_RESTORE_FAIL		11


// touch service
#define AKDETECT_TOUCH					100

//
// Ž���� ��ŷ�� ���� ������ �Ѱ��ִ� ����ü 
//	-- wops ������ �ʿ��� ����ü��
//
typedef struct _HACK_DETECT_INFO
{
	INT		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
	INT		Size;			// Buffer �� ����
	LPSTR	Buf;			// ������ ����	
} HACK_DETECT_INFO, *PHACK_DETECT_INFO;


/*-----------------------------------------------------------------------------
 * Malicious S/W �� Ÿ�� 
**---------------------------------------------------------------------------*/
// Ű�ΰŷ� 
#define TYPE_KEYLOGER	1	

// �޸𸮿�����
#define	TYPE_MEMEDITOR	2	

// ����� 
#define TYPE_DEBUGGER	3	

// ���丶�콺 
#define TYPE_AUTOMOUSE	4	

// ���ǵ���
#define TYPE_SPEEDHACK	5	

// ��Ÿ, �̺з� 
#define TYPE_UNKNOWN	99


/*-----------------------------------------------------------------------------
 * ���� �ڵ� ����
**---------------------------------------------------------------------------*/
// 2006. 07. 03  ��������� AKError.h�� �Ű��ֻ�
#define AKERROR_INIT_FAIL					-1
#define AKERROR_GENERAL_APIFAIL				-2		// Windows API�Լ� ȣ���� ��������
#define	AKERROR_SET_PRIVILEGE				-11		// ���μ����� ��ĵ�ϱ� ���� ���� ��¿� �����Ͽ���, 
													// �����ڱ������� ���α׷��� �����Ű�� ���� �䱸��
#define AKERROR_INIT_INVALID_HANDLE			-13		// �ʱ�ȭ������ HWND �� ��ȿ���� ����
#define AKERROR_PECHKSUM_ZERO				-14		// PE����� üũ���� 0�Դϴ�. 
#define AKERROR_FILEFROMID_GETFAIL			-16		// ID�� ���� ȭ���� �������µ� �����߽��ϴ�.

#ifndef ret_true
#define ret_true	1		// �����ڵ� : ����
#endif
#ifndef ret_false
#define ret_false	0		// �Լ��� �����ϰų� ������� ����
#endif


/*-----------------------------------------------------------------------------
 * �ڷ� ���� ����
**---------------------------------------------------------------------------*/

// �����üũ�� ���� �ڷᱸ��
typedef struct _ANTI_DEBUG_CHECK_INFO
{
	// �˻� ��� ����
	BOOL bSpecificAPI;			// Ư���� API ���� ����� Ž��
	BOOL bProcessInfo;			// ���μ��� ������ ���� Ž��
	BOOL bParentProcessInfo;	// �θ� ���μ��� ������ ���� Ž��
	BOOL bActiveDevices;		// Soft-Ice ���� Ư�� ����̽� Ȱ��ȭ ���ο� ���� Ž��
	BOOL bSEH;					// SEH �� ���� Ž�� 

	// �˻� �����
	INT bResSpecificAPI;		// Ư���� API ���� ����� Ž�� ���
	INT bResProcessInfo;		// ���μ��� ������ ���� Ž�� ���
	INT bResParentProcessInfo;	// �θ� ���μ��� ������ ���� Ž�� ���
	INT bResActiveDevices;		// Soft-Ice ���� Ư�� ����̽� Ȱ��ȭ ���ο� ���� Ž�� ���
	INT bResSEH;				// SEH �� ���� Ž�� ���
} ANTI_DEBUG_CHECK_INFO, *PANTI_DEBUG_CHECK_INFO;



// PE üũ���� ���� �ڷᱸ��
typedef struct _PE_CHECKSUM_INFO
{
	DWORD HeaderSum;				// PE ����� üũ��
	DWORD CalcSum ;					// ���� üũ�� (HeaderSum ���� �񱳴��)

} PE_CHECKSUM_INFO, *PPE_CHECKSUM_INFO;                 


// ���ǵ��� üũ�� ���� �ڷᱸ��
typedef struct _SPEED_HACK_CHECK_INFO
{
	DWORD	dwTGT;			// TimeGetTime()
	DWORD	dwGTC;			// GetTickCount()
	DWORD	dwTSC;			// RDTSC
	DWORD	dwQPC;			// QueryPerformanceCounter()
} SPEED_HACK_CHECK_INFO, *PSPEED_HACK_CHECK_INFO;



// �ؽ��� üũ�� ���� �ڷᱸ��
typedef struct _PE_HASH_INFO
{
	DWORD	HeaderSum;				// PE ����� üũ��
	BYTE	HashBytes[256];		// �ؽ� ��Ʈ��
	DWORD	HashLen ;
} PE_HASH_INFO, *PPE_HASH_INFO;                 


// ���μ��� ��ĵ�� ���� �ڷᱸ��
typedef struct _PROCESS_SCAN_INFO
{
	/// ���μ��� ����
	TCHAR	ProcessName[MAX_PATH + 1];
	DWORD	ProcessId;

	/// signature ����	
	DWORD	SignatureIndex;
	DWORD	SianatureType;
	TCHAR	SignatureName[MAX_PATH + 1];

	// ��Ʈ���� �� �͹̳���Ʈ�� �����ؾ���

} PROCESS_SCAN_INFO, *PPROCESS_SCAN_INFO;

/*-----------------------------------------------------------------------------
 * checkspeedhack ���
**---------------------------------------------------------------------------*/
// tick Ÿ�̸Ӱ� 10���̻� ���̳��� �̻����� �����ϰ� ���� 
#define MAX_TICK_ELAPSE		10

// SYSTEM_POWER_STATUS �� AC�Ŀ����� ����
#define AC_OFFLINE	0
#define AC_ONLINE	1

// �и������� ����
#define	UNIT_MILISECOND	1000

// �и������� Ŭ�� ��������
#define UNIT_CLOCKGAP	10

// �и� ������ �Ѱ�����
#define VAL_ELAPSE		50

// QueryPerformanceFreqeuncy ����
#define FREQ_WIN98		1193000
#define FREQ_WINNT		3579545 
#define FREQ_WINXP		// 3579545  or CPU Clock
#define FREQ_WIN2000	3579545

INT CheckSpeedHack( IN PSPEED_HACK_CHECK_INFO, IN DWORD MaxTickElapse );


/*-----------------------------------------------------------------------------
 * scanproc ���
**---------------------------------------------------------------------------*/
#define MAX_SCANBUFF	256

// ������� ���� ������ ĳ����
#define bGarbage	'\xCC'
#define MAX_SIGLEN		256

// Malicious S/W �� Ž���ϱ� ���� Ž���ñ״��� ( runtime process�� memory based )
//
typedef struct _PROC_SIGNATURE
{
	INT		dwIdx;					// signature��s unique number
	DWORD	ntype;					// signature 's type
	TCHAR	lptszSigName[MAX_SIGLEN];			// signature name
	DWORD 	dwOffset;				// ���μ����󿡼��� signature �� offset (from Image base address)
	DWORD 	dwSigLen;				// lpszSignature length
	TCHAR	lpszSignature[MAX_SIGLEN * 2];			// ���� signature 	
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


// ScanDoubtProcessList() ���� ���Ǵ� �ǽɵǴ� �Լ��� ã�Ƴ��� ���� �ڷᱸ�� 
//
typedef struct _PROC_DOUBTAPI {
	TCHAR	*lptszFuncName ;
	TCHAR	*lptszDllName ;
	DWORD	Reserved ;
} PROC_DOUBTAPI, *PPROC_DOUBTAPI ;

// ���μ��� ��ĵ�� ���� �ݹ� �Լ� 
//
//typedef BOOL (*lpfnProcessScanCB) (PPROCESS_SCAN_INFO pPSI, PVOID pTag);

//INT		ScanProcessList( lpfnProcessScanCB pfnPSCB, PVOID pTag ) ;
INT ScanProcessList( PPROCESS_SCAN_INFO pPSI );

// ���� ���μ����� ����� üũ
INT	CheckProxyDll(PBOOL pbResult, PPROCESS_SCAN_INFO pPSI);



/*-----------------------------------------------------------------------------
 *  AntiHook ���� 
 *
 *	HOOKED_API_INFO.API_TYPE �� 0 �� ��� HEA ����ü ����
 *	                            1 �� ��� HIA ����ü ����
**---------------------------------------------------------------------------*/


/**	-----------------------------------------------------------------------
	HOOKED_API_INFO.API_TYPE == AT_EXPORTED �� ��� ���Ǵ� ����ü
	EXPORT API ��ŷ�� Ž�� ���
	
	jmp �ڵ带 ����ϴ� ��� EXPORTED_API_INFO.ReadAddressOfApi ���� ���� Ž���ϹǷ�
	OriginalAddr, HookdAddr �� EXPORTED_API_INFO.ReadAddressOfApi ���̴�. 
-------------------------------------------------------------------------*/

typedef struct _HOOKED_EXPORT_API
{
	INT			HOOKED_CODE;				// hooking Ÿ��(inline? address modify, ...)
	
	TCHAR		DLLName[MAX_PATH];
	TCHAR		ApiName[MAX_PATH];	
	DWORD_PTR	OriginalAddr;				// hook ���� ���� ������ api �� �ּ�
	DWORD_PTR	HookdAddr;					// hooking �� ������ api �ּ�		
} HOOKED_EXPORT_API, *PHOOKED_EXPORT_API;
typedef BOOL (*lpfnHookedApiCB) (PHOOKED_EXPORT_API pHooked_API, PVOID pTag);

/**	-----------------------------------------------------------------------
	HOOKED_API_INFO.API_TYPE == AT_IMPORTED �� ��� ���Ǵ� ����ü
	IMPORT API ��ŷ�� Ž�� ���
-------------------------------------------------------------------------*/
typedef struct _HOOKED_IMPORT_API
{
	INT			HOOKED_CODE;				// hooking Ÿ��(inline? address modify, ...)
	
	TCHAR		DLLName[_MAX_PATH];
	TCHAR		ApiName[MAX_PATH];		
	DWORD_PTR	HookdAddr;					// hooking �� ������ api �ּ�	
	DWORD_PTR	DetourAddr;					// jmp �� ���� �ּ� (INLINE HOOKING Ÿ���� ��츸 ��ȿ)
	DWORD_PTR	dwStart;
	DWORD_PTR	dwEnd;	
} HOOKED_IMPORT_API, *PHOOKED_IMPORT_API;
typedef BOOL (*lpfnHookedImportApiCB) (PHOOKED_IMPORT_API pHookedImportApi, PVOID pTag);

/**	-----------------------------------------------------------------------
	HOOKED_API_INFO.API_TYPE == AT_INTEGRITY �� ��� ���Ǵ� ����ü
	������ ����Ʈ ��ⰹ���� ������ ����Ʈ ��� ������ �ٸ� ��츦 Ž��
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

// API_TYPE ����
//
#define		AT_EXPORTED			0	// exported api 
#define		AT_IMPORTED			1	// imported api
#define		AT_INTEGRITY		2	// IAT �� ���� ���� �˻�

// �ݹ��Լ��� �ѱ�� ���� ����ü
//
typedef struct _HOOKED_API_INFO
{
	INT				API_TYPE;		// 0: EXPORTED API, 1: IMPORTED API, 2: AT_INTEGRITY
	UAPI_INFO		API_INFO;		
} HOOKED_API_INFO, *PHOOKED_API_INFO;


// API ���ϰ�
//
#define		RETCODE_LIB_SUCCESS				0				// ����
#define		RETCODE_LIB_ERROR				-1				// ����

// ��ŷ üũ�Լ� ���ϰ�
//
#define		RETCODE_NOT_HOOKED				0				// ��ŷ�� API �� ����
#define		RETCODE_API_HOOKED				1				// API ��ŷ�� Ž���Ǿ���

// HOOKED_CODE ����
//
//	AT_EXPORTED �� ���
//
#define		AH_EAT_MODIFIED					RETCODE_API_HOOKED				// EAT �ּҰ� ���� Ž��
#define		AH_EAT_INLINE_HOOKED			-2				// EAT �ζ��� ��ŷ Ž��
#define		AH_EAT_COUNT_ERROR				-3				// Disk �� VAS ���� Export �� api ī��Ʈ�� �ٸ�		
#define		AH_EAT_COUNT_ZERO				-4				// Export �� api �� ���� ! (�̷� ��찡 ����� ����. )

//	AT_IMPORTED �� ���
//
#define		AH_IAT_MODIFIED					RETCODE_API_HOOKED				// IAT �ּҰ� ���� Ž����
#define		AH_IAT_INLINE_HOOKED			-6				// IAT �ζ��� ��ŷ Ž��


/**	-----------------------------------------------------------------------
	\brief	touch service �϶� touch ������� ����		
-------------------------------------------------------------------------*/
int GetTouchValue(int seed);

/**	-----------------------------------------------------------------------
	\brief	�����޼��� ���ڿ� ����
-------------------------------------------------------------------------*/
LPCTSTR GetErrorString(void);






///////////////////////////////////////////////////////////////////////////////
// ���� ��� ���� 
//		AkCommon.h �� �ߺ���
///////////////////////////////////////////////////////////////////////////////

// �����̸�, ��ο� ���� ��� ���� 
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
	#define _MAX_FORWARDER_NAME _MAX_FNAME * 2		// �׳� �˳��ϰ� ��Ҵ�. -_-
#endif

#ifndef _OPCODE_DUMP_LENGTH
	#ifdef _DEBUG
		#define _OPCODE_DUMP_LENGTH	32	/* API �� ������ ���� ����  */
	#else
		#define _OPCODE_DUMP_LENGTH	16	/* API �� ������ ���� ����  */	
	#endif
#endif

#ifndef _OPCODE_DUMP_BUF_LENGTH
	#define _OPCODE_DUMP_BUF_LENGTH	( (_OPCODE_DUMP_LENGTH * 2 ) + sizeof(TCHAR) )	/* API �� ������ ������ ������ ������ ����  */
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
// ���� �ڵ� ����
//
#define		HDERR_NONE				0		// ���� ����
#define		HDERR_GENERIC			-1		// �Ϲ� ���� ���� (SetErrorString() ���� ���� ���� ����)
#define		HDERR_IAT_RESTORE		-2		// IMPORT TABLE ���� ����
#define		HDERR_EAT_RESTORE		-3		// EXPORT TABLE ���� ���� 

#define		HDERR_INVALID_PE		-4		// �������� PE ������ �ƴ�
#define		HDERR_NO_API_FOUNDED	-5		// API �� PE �κ��� ã�� �� ����
#define		HDERR_INVALID_ADDRESS	-6		// VirtualProtect() �Ǵ� �ش� �޸� �������� ������ ���



//
// API Hook Ÿ�� 
// 
typedef enum _ENUM_HOOK_TYPE 
{
	HT_NONE, 
	HT_ENTRY_HOOK_INVALID_ADDR_IAT,	// Entry point �� �ٲ�ġ�� �� ���(���� �� �˷��� ���)
	HT_ENTRY_HOOK_INVALID_ADDR_EAT,
	HT_ENTRY_HOOK_USE_CAVE_EAT,		// cave (e.g. �е� ����)�� ���� �ڵ� ������ entry �� �ش� cave �� �ٲٴ� ���
	HT_INLINE_HOOK_EAT				// inline hook (a.k.a detours)		
} ENUM_HOOK_TYPE;


// 
// API ������ ���� ������ ��� �ִ� ����ü
//
//	armorkit ���� callback �� ���ؼ� �Ѱ��� HACK_DETECT_INFO.HackType �� 
//	AKDETECT_HOOK_DETECT �� ��� ���ȴ� ����ü
//	(binary stream ������ ���۷� ���޵�)
//
typedef struct _API_TO_RESTORE
{
	ENUM_HOOK_TYPE		HookType;
	
	TCHAR				DLLName[_MAX_PATH];
	INT					Ordinal;				// IAT ��Ž���� API �� ������ import �Ȱ�� ���� 
	TCHAR				ApiName[_MAX_API_NAME];	// IAT ��Ž���� API �� �̸����� import �Ȱ�� ����	
	DWORD_PTR			OriginalAddr;				// hook ���� ���� ������ api �� �ּ�
	DWORD_PTR			CurrentAddr;				// hooking �� ������ api �ּ�		
	DWORD_PTR			DetourAddr;					// ���� detour �� �ּ�
	TCHAR				OriginalOpcodes[_OPCODE_DUMP_BUF_LENGTH];
	TCHAR				CurrentOpcodes[_OPCODE_DUMP_BUF_LENGTH];
	TCHAR				DetourAddrOpcodes[_OPCODE_DUMP_BUF_LENGTH];	// IAT ������ ������. EAT ������ ���
	DWORD_PTR			ValidStartAddr;				// dll �� ��ȿ ���� �ּ�
	DWORD_PTR			ValidEndAddr;				// dll �� ��ȿ �� �ּ�
} API_TO_RESTORE, *PAPI_TO_RESTORE;

//
// API ������ ������ API �� �̿� ���� ���� ������ ��� ���� ����ü
//
//	armorkit ���� callback �� ���ؼ� �Ѱ��� HACK_DETECT_INFO.HackType �� 
//	AKDETECT_HOOK_RESTORE_FAIL �� ��� ���Ǵ� ����ü
//	(binary stream ������ ���۷� ���޵�)
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
