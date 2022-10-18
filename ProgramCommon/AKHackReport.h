/*-----------------------------------------------------------------------------
 * AKHackReport.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _AK_HACK_REPORT_H_
#define _AK_HACK_REPORT_H_

#include <windows.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
// hack type ����
//-----------------------------------------------------------------------------
const DWORD		AK_ERROR		= 0x00000001;		// ArmorKit ���� ����
const DWORD		AK_HOOKDEFENDER	= 0x00000002;		// hook restore fail
const DWORD		AK_DEBUGGED		= 0x00000003;		// debugging ���� Ž��
const DWORD		AK_PROCESS		= 0x00000004;		// hack process Ž��
const DWORD		AK_FILEHASH		= 0x00000005;		// file hash �� Ž��
const DWORD		AK_KABUSE		= 0x00000006;		// kernel mode component �ջ�
const DWORD		AK_APIHOOK		= 0x00000007;		// hooked api ���� Ž��

//-----------------------------------------------------------------------------
// Armorkit �� hack Ž�� ������ �Ѱ��ֱ� ���� ����ü
//-----------------------------------------------------------------------------
typedef struct _HACK_DETECT_INFO
{
	DWORD			HackType;
	DWORD			Size;
	BYTE*			TypedBuf;
} HACK_DETECT_INFO, *PHACK_DETECT_INFO;


//-----------------------------------------------------------------------------
// �� hack type �� TypedBuf 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// structure for HackType AK_ERROR 
//		-- ArmorKit Error Code 
//-----------------------------------------------------------------------------
const int AKEC_GENERIC_ERROR	= 0x00000001;	// unknown error, api fail, system err..

const int AKEC_SVC_STARTFAIL	= 0x00000002;	// AKSvc - Start fail
const int AKEC_INVALID_SVCTMR	= 0x00000003;	// invalid service timer id

const int AKEC_INVALID_HDTMR	= 0x00000004;	// invalid hookdefender timer id
const int AKEC_HD_CHECK_FAIL	= 0x00000005;	// hookdefender check api hook fail
const int AKEC_HD_RSTR_FAIL		= 0x00000006;	// hookdefender restore api hook fail
const int AKEC_AKSVC_FILEHASH	= 0x00000007;	// AKSvc - ScanProc fail
const int AKEC_HOOKDLL_FAIL		= 0x00000008;	// api hook module init fail
const int AKEC_AH_CHECK_FAIL	= 0x00000009;	// api hook module check fail
const int AKEC_AH_SETCMD_FAIL	= 0x0000000a;	// api hook set cmd fail
const int AKEC_AH_CMD_TIMEOUT	= 0x0000000b;	// api cmd timeout

typedef struct _AK_ERROR_REC
{
	int		code;			
} AK_ERROR_REC, *PAK_ERROR_REC;

// helper function
// 
PHACK_DETECT_INFO build_akerror_hdi(int errorcode);
void free_akerror_hdi(PHACK_DETECT_INFO pHDI);


//-----------------------------------------------------------------------------
// structure for HackType AK_HOOKDEFENDER
//-----------------------------------------------------------------------------
#define MAX_HOOKED_API_INFO			5
#define MAX_APINAME_LEN_REPORT		64

typedef struct _HD_HAI	// HookDefender Hooked Api Info (HAI ����ü ����)
{
	DWORD			HD_ErrorCode;
	DWORD			HookType;
	WORD			Ordinal_or_Hint;	// import by name �� ��� HINT ��
										// import by ordinal �� ���
										// ApiName �ʵ�� 0 ���� �ʱ�ȭ

	DWORD			ApiNameLen;			// api name length
	TCHAR			ApiName[MAX_APINAME_LEN_REPORT];	// api name ó�� 64 ����Ʈ
} HD_HAI, *PHD_HAI;

typedef struct _AK_HD_REC
{
	DWORD	total_api_count;			// api ������ ������ ��� api ����
	HD_HAI	apis[MAX_HOOKED_API_INFO];	// 0-4 ��° ������ api ����
}AK_HD_REC, *PAK_HD_REC;

//-----------------------------------------------------------------------------
// structure for HackType AK_DEBUGGED 
//
// AKHackReport.h, antidebug.h  �� ��� ���Ͽ� �ߺ� ���ǵ�
// ������ �ݵ�� üũ�� ��
//-----------------------------------------------------------------------------
#ifndef _HACK_REPORT_AKSVC_ANTI_DEBUG_DEFINDED_
#define _HACK_REPORT_AKSVC_ANTI_DEBUG_DEFINDED_
const unsigned int ADF_IS_DEBUGGER_PRESENT	= 0x00000001; // IsDebuggerPresent
const unsigned int ADF_NTGLOBALFLAG			= 0x00000002; // Peb NtGlobalFlag
const unsigned int ADF_FORCEFLAGS			= 0x00000004; // Peb ForceFlags
const unsigned int ADF_SOFTICE				= 0x00000008; // Soft-Ice Detected
const unsigned int ADF_DEBUGPORT			= 0x00000010; // Check Opened Debug Port
typedef struct _AK_DEBUGGED_REC
{
	unsigned int debugging_flag;		
}AK_DEBUGGED_REC, *PAK_DEBUGGED_REC;

#endif//_HACK_REPORT_AKSVC_ANTI_DEBUG_DEFINDED_

//-----------------------------------------------------------------------------
// structure for HackType AK_PROCESS
//
// AKHackReport.h, scanproc.h  �� ��� ���Ͽ� �ߺ� ���ǵ�
// ������ �ݵ�� üũ�� ��
//-----------------------------------------------------------------------------
#ifndef _HACK_REPORT_AKSVC_SCAN_PROC_DEFINDED_
#define _HACK_REPORT_AKSVC_SCAN_PROC_DEFINDED_

	#define MAX_SIGLEN		256	// �ñ״�ó �׸��� �ִ� ����
	#define MAX_SIG_NAME	255
	
	#define MAX_SCANBUFF	256	// �޸𸮿��� READ�ϴ� �ִ�ũ��

	// Ž���� ���μ����� ���� 
	//
	typedef struct _AK_PROCESS_INFO
	{
		TCHAR	ProcessName[_MAX_PATH];
		DWORD	ProcessId;
		
		DWORD	SignatureIndex;
		DWORD	SignatureType;
		TCHAR	SignatureName[MAX_PATH];	// must be null terminated!!		
	} AK_PROCESS_INFO, *PAK_PROCESS_INFO;

#endif//_HACK_REPORT_AKSVC_SCAN_PROC_DEFINDED_

//-----------------------------------------------------------------------------
// structure for HackType AK_FILEHASH
// AKHackReport.h, scanfilehash.h  �� ��� ���Ͽ� �ߺ� ���ǵ�
// ������ �ݵ�� üũ�� ��
//-----------------------------------------------------------------------------
#ifndef _HACK_REPORT_AKSVC_FILE_HASH_DEFINDED_
#define _HACK_REPORT_AKSVC_FILE_HASH_DEFINDED_

	// ���� �ؽ� �� ���
	// Ž���� ���μ����� ���� 

	typedef struct _AK_FILEHASH_INFO
	{
		TCHAR	ProcessName[_MAX_PATH];
		DWORD	HashListIdx;
	} AK_FILEHASH_INFO, *PAK_FILEHASH_INFO;

#endif//_HACK_REPORT_AKSVC_FILE_HASH_DEFINDED_

//-----------------------------------------------------------------------------
// structure for HackType AK_KABUSE
// vg_common.h , AKHackReport.h �� �ߺ� ���ǵ� 
// ���� �� �ݵ�� üũ�� ��
//-----------------------------------------------------------------------------
#ifndef _K_ABUSE_TYPE_DEFINED_
	#define ABUSE_UNKNOWN				0x00000000
	#define ABUSE_INT01					0x00000001	// int 0x01 �߻� (debugger)
	#define ABUSE_INT03					0x00000002	// int 0x03 �߻� (debugger) 
	#define ABUSE_BROKEN_ISR			0x00000003	// interrupt handler modified
	#define ABUSE_BROKEN_DETOUR			0x00000004	// detour modified
#endif//_K_ABUSE_TYPE_DEFINED_

typedef struct _AK_KABUSE_INFO
{
	DWORD AbuseType;
} AK_KABUSE_INFO, *PAK_KABUSE_INFO;

//-----------------------------------------------------------------------------
// AKHackReport.h, AKApiHooker.h  �� ��� ���Ͽ� �ߺ� ���ǵ�
// ������ �ݵ�� üũ�� ��
//-----------------------------------------------------------------------------
#ifndef _HACK_REPORT_AK_APIHOOK_DEFINDED_
#define _HACK_REPORT_AK_APIHOOK_DEFINDED_

	// Hook API�� ������ ���μ����� ���� 
	//
	typedef struct _AK_APIHOOK_INFO
	{
		TCHAR	ProcessName[MAX_PATH];
		DWORD	ProcessId;
	} AK_APIHOOK_INFO, *PAK_APIHOOK_INFO;

#endif//_HACK_REPORT_AK_APIHOOK_DEFINDED_



#endif//_AK_HACK_REPORT_H_