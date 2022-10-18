/*-----------------------------------------------------------------------------
 * typedef_AKv1HackDetect.h 
 *-----------------------------------------------------------------------------
 * ArmorKit v1�� ��� ����
 *	���� �ۼ� : ������ (truefroggie@gmail.com , shseo@webzen.co.kr)
 *	2005.11.16	���ȯ 
 *				- ������Ʈ ���� �ٲٸ鼭 ������ϵ� �Բ� ������ (���� ���� ��� �������� �� �� ^_^)
 *	2007.03.21	�ڼ���
 *				- ArmorKit.h������ ����Ǹ� �� ���ϵ� ��������� ��
 *	2007.07.13	�ڼ���
 *				- WOPS���� AKDETECT_PEINTEGRITY�� AKDETECT_ERROR�� ����ʿ� ���� ������𺯰�
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
 * WOPS�� ������ ���� RTT ��� ����
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
 * Ž���÷��� (DWORD)	- �÷��׿� ���� �ش� ��� ����
**---------------------------------------------------------------------------*/
#define V1_AKCHECK_SPEEDHACK			0x00000001		// ���ǵ��� Ž��

#define V1_AKCHECK_MALWARE				0x00000002		// Malware Ž��

#define V1_AKCHECK_PEINTEGRITY			0x00000004		// ���ӹ��̳ʸ�����

#define V1_AKCHECK_AKINTEGRITY			0x00000008		// �Ƹ�Ŷ ����

#define V1_AKCHECK_DEBUG				0x00000010		// ����� ����Ž��

#define	V1_AKCHECK_MEMEDIT				0x00000020		// �޸�����

#define V1_AKCHECK_DOUBT				0x00000040		// �ǽɰ��� S/W Ž��

#define V1_AKCHECK_RUNCODE				0x00000080		// ��Ÿ�� �ڵ� ������ Ž��

#define V1_AKCHECK_IAT_HOOK				0x00000100		// �����Լ�����

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
 * ArmorKit ����� ���� ���ν��� ����� ���� (HackType)
**---------------------------------------------------------------------------*/
#define V1_AKRUNTIME_ERROR				-1				// ArmorKit ���� ���� �߻�

#define V1_AKDETECT_SPEEDHACK			1				// ���ǵ��� Ž��

#define V1_AKDETECT_MALWARE				2				// Malware Ž��

#define V1_AKDETECT_ERROR				3				// �Ƹ�Ŷ ����

#define V1_AKDETECT_APIHOOK				4				// �����Լ�����

#define V1_AKDETECT_AKINTEGRITY			5				// �Ƹ�Ŷ ����

#define V1_AKDETECT_DEBUG				6				// ����� ����Ž��

#define V1_AKDETECT_MEMEDIT				7				// �޸�����

#define V1_AKDETECT_DOUBT				8				// �ǽɰ��� S/W Ž��

#define V1_AKDETECT_RUNCODE				9				// ��Ÿ�� �ڵ庯���� Ž��

#define V1_AKDETECT_HOOK_DETECT			10				// API Hook Ž�� by HOOKDefender

#define V1_AKDETECT_HOOK_RESTORE_FAIL	11				// API Hook ���� ���� by HOOKDefender

#define V1_AKDETECT_KABUSE				12				// by somma. kernel mode abuse detected

#define V1_AKDETECT_TOUCH				100				// touch service


/*-----------------------------------------------------------------------------
 * Malicious S/W �� Ÿ�� 
**---------------------------------------------------------------------------*/
#define V1_TYPE_KEYLOGER				1				// Ű�ΰŷ� 

#define	V1_TYPE_MEMEDITOR				2				// �޸𸮿�����

#define V1_TYPE_DEBUGGER				3				// ����� 

#define V1_TYPE_AUTOMOUSE				4				// ���丶�콺 

#define V1_TYPE_SPEEDHACK				5				// ���ǵ���

#define V1_TYPE_UNKNOWN					99				// ��Ÿ, �̺з� 


/*-----------------------------------------------------------------------------
 * HookDefender�� ���� Ÿ��
**---------------------------------------------------------------------------*/
#define	V1_HDERR_NONE					0				// ���� ����

#define V1_HDERR_GENERIC				-1				// �Ϲ� ���� ���� (SetErrorString() ���� ���� ���� ����)
	
#define V1_HDERR_IAT_RESTORE			-2				// IMPORT TABLE ���� ����

#define V1_HDERR_EAT_RESTORE			-3				// EXPORT TABLE ���� ���� 

#define V1_HDERR_INVALID_PE				-4				// �������� PE ������ �ƴ�

#define V1_HDERR_NO_API_FOUNDED			-5				// API �� PE �κ��� ã�� �� ����

#define V1_HDERR_INVALID_ADDRESS		-6				// VirtualProtect() �Ǵ� �ش� �޸� �������� ������ ���


/*-----------------------------------------------------------------------------
 * ���� �ڵ� ����
**---------------------------------------------------------------------------*/
#define V1_AKERROR_INIT_FAIL			-1

#define V1_AKERROR_GENERAL_APIFAIL		-2			// Windows API�Լ� ȣ���� ��������

#define	V1_AKERROR_SET_PRIVILEGE			-11			// ���μ����� ��ĵ�ϱ� ���� ���� ��¿� �����Ͽ���, 

#define V1_AKERROR_INIT_INVALID_HANDLE	-13			// �ʱ�ȭ������ HWND �� ��ȿ���� ����

#define V1_AKERROR_PECHKSUM_ZERO		-14			// PE����� üũ���� 0�Դϴ�. 

#define V1_AKERROR_FILEFROMID_GETFAIL	-16			// ID�� ���� ȭ���� �������µ� �����߽��ϴ�.



/*-----------------------------------------------------------------------------
 * �ڷ� ���� ����
**---------------------------------------------------------------------------*/

//
// Ž���� ��ŷ�� ���� ������ �Ѱ��ִ� ����ü 
//
typedef struct _HACK_DETECT_INFO_V1
{
	INT		HackType;
	INT		Size;
	LPSTR	Buf;

} HACK_DETECT_INFO_V1, *PHACK_DETECT_INFO_V1;

//
// �����üũ�� ���� �ڷᱸ��
//
typedef struct _ANTI_DEBUG_CHECK_INFO_V1
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

} ANTI_DEBUG_CHECK_INFO_V1, *PANTI_DEBUG_CHECK_INFO_V1;


//
// PE üũ���� ���� �ڷᱸ��
//
typedef struct _PE_CHECKSUM_INFO_V1
{
	DWORD HeaderSum;				// PE ����� üũ��
	DWORD CalcSum ;					// ���� üũ�� (HeaderSum ���� �񱳴��)

} PE_CHECKSUM_INFO_V1, *PPE_CHECKSUM_INFO_V1;                 


//
// ���ǵ��� üũ�� ���� �ڷᱸ��
//
typedef struct _SPEED_HACK_CHECK_INFO_V1
{
	DWORD	dwTGT;			// TimeGetTime()
	DWORD	dwGTC;			// GetTickCount()
	DWORD	dwTSC;			// RDTSC
	DWORD	dwQPC;			// QueryPerformanceCounter()

} SPEED_HACK_CHECK_INFO_V1, *PSPEED_HACK_CHECK_INFO_V1;


//
// �ؽ��� üũ�� ���� �ڷᱸ��
//
typedef struct _PE_HASH_INFO_V1
{
	DWORD	HeaderSum;
	BYTE	HashBytes[256];
	DWORD	HashLen ;

} PE_HASH_INFO_V1, *PPE_HASH_INFO_V1;                 


//
// ���μ��� ��ĵ�� ���� �ڷᱸ��
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
// ScanDoubtProcessList() ���� ���Ǵ� �ǽɵǴ� �Լ��� ã�Ƴ��� ���� �ڷᱸ�� 
//
typedef struct _PROC_DOUBTAPI_V1 {
	TCHAR	*lptszFuncName ;
	TCHAR	*lptszDllName ;
	DWORD	Reserved ;

} PROC_DOUBTAPI_V1, *PPROC_DOUBTAPI_V1;


//
// HOOKED_API_INFO.API_TYPE == AT_EXPORTED �� ��� ���Ǵ� ����ü
//
typedef struct _HOOKED_EXPORT_API_V1
{
	INT			HOOKED_CODE;				// hooking Ÿ��(inline? address modify, ...)	
	TCHAR		DLLName[MAX_PATH];
	TCHAR		ApiName[MAX_PATH];	
	DWORD_PTR	OriginalAddr;				// hook ���� ���� ������ api �� �ּ�
	DWORD_PTR	HookdAddr;					// hooking �� ������ api �ּ�	

} HOOKED_EXPORT_API_V1, *PHOOKED_EXPORT_API_V1;


//
// HOOKED_API_INFO.API_TYPE == AT_IMPORTED �� ��� ���Ǵ� ����ü
//
typedef struct _HOOKED_IMPORT_API_V1
{
	INT			HOOKED_CODE;				// hooking Ÿ��(inline? address modify, ...)	
	TCHAR		DLLName[V1__MAX_PATH];
	TCHAR		ApiName[MAX_PATH];		
	DWORD_PTR	HookdAddr;					// hooking �� ������ api �ּ�	
	DWORD_PTR	DetourAddr;					// jmp �� ���� �ּ� (INLINE HOOKING Ÿ���� ��츸 ��ȿ)
	DWORD_PTR	dwStart;
	DWORD_PTR	dwEnd;	

} HOOKED_IMPORT_API_V1, *PHOOKED_IMPORT_API_V1;


//
// HOOKED_API_INFO.API_TYPE == AT_INTEGRITY �� ��� ���Ǵ� ����ü
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
// �ݹ��Լ��� �ѱ�� ���� ����ü
//
typedef struct _HOOKED_API_INFO_V1
{
	INT				API_TYPE;
	UAPI_INFO_V1	API_INFO;		

} HOOKED_API_INFO_V1, *PHOOKED_API_INFO_V1;


//
// API Hook Ÿ�� 
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
// API ������ ���� ������ ��� �ִ� ����ü
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
// API ������ ������ API �� �̿� ���� ���� ������ ��� ���� ����ü
//
typedef struct _RESTORE_FAULT_API_V1
{
	INT					ErrorCode;
	API_TO_RESTORE_V1	apiInfo;

} RESTORE_FAULT_API_V1, *PRESTORE_FAULT_API_V1;

#include <vector>
typedef std::vector<RESTORE_FAULT_API_V1>	HDFAULT_API_LIST;

#endif	//	_TYPEDEF_AKV1_HACK_DETECT_H_
