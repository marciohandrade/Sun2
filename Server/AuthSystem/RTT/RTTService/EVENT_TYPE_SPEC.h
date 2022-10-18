//--------------------------------------------------------------------
//	ARMORKIT �� WOPS ������ ���� ���� ���� 
//--------------------------------------------------------------------
// history
//	2005.12.06 	���ȯ 	���� �ۼ� 
//  2005.12.06	���ȯ	AKRUNTIME_ERROR �ڵ� �߰� 
//	2005.12.08	���ȯ	Hacktype �� �����Ǵ� ����ü ����
//						���� ����� ���� �߰�
//--------------------------------------------------------------------


//[[ ��� ���� -- from AkCommon.h]]
#include <stdlib.h>

#define  _MAX_API_NAME		255			/* max length of api name */
#define  _OPCODE_DUMP_LENGTH	16			/* API �� ������ ���� ����  */
#define  _OPCODE_DUMP_BUF_LENGTH		( _OPCODE_DUMP_LENGTH * 2 + sizeof(TCHAR) )	/* API �� ������ ������ ������ ������ ����  */
#define  MAX_SIG_NAME		255			/* signature �� �ִ� ���� */






//[[ ArmorKit ���� Client �ݹ����� �Ѱ��ִ� ������ Ÿ�� - from ArmorKit.h]]

// ArmorKit ���� ���� �߻�	--  ������ ���� ���ڿ� �Ǵ� ���ǵǴ� �����ڵ� ��뿹��(���� ����..)
#define AKRUNTIME_ERROR					-1

// ���ǵ��� Ž��
#define AKDETECT_SPEEDHACK				1


// Malware Ž��
#define AKDETECT_MALWARE				2


// ���ӹ��̳ʸ�����
#define AKDETECT_PEINTEGRITY			3


// �����Լ�����
#define AKDETECT_APIHOOK				4


// �Ƹ�Ŷ ��ü ����			-- ���� ������� ����
#define AKDETECT_AKINTEGRITY			5


// ����� ����Ž��
#define AKDETECT_DEBUG					6


// �޸�����				-- ���� ������� ����
#define AKDETECT_MEMEDIT				7

// �ǽɰ��� S/W Ž��		-- ���� ������� ����
#define AKDETECT_DOUBT					8

// ��Ÿ�� �ڵ庯���� Ž��  -- HackType �� �����Ǵ� �ΰ����� ����ü ���� (�ܼ� �ؽ� ���̹Ƿ�..)
#define AKDETECT_RUNCODE				9




//
// �Ʒ� ����ü�� HOOKED_API_INFO ����ü�� �����ϴ� ����ü�鿡 ���� ������
//

/*
 *	EXPORT TABLE HOOKING Ž���� �� �Լ��� ���� ������ �ѱ�� ���� ����ü
 *
 *		jmp �ڵ带 ����ϴ� ��� EXPORTED_API_INFO.ReadAddressOfApi ���� ���� Ž���ϹǷ�
 *		OriginalAddr, HookdAddr �� EXPORTED_API_INFO.ReadAddressOfApi ���̴�. 
 *
**/ 
typedef struct _HOOKED_EXPORT_API
{
	TCHAR		DLLName[_MAX_PATH + 1];
	TCHAR		ApiName[_MAX_API_NAME + 1];	
	DWORD_PTR	OriginalAddr;				// hook ���� ���� ������ api �� �ּ�
	DWORD_PTR	HookdAddr;					// hooking �� ������ api �ּ�	
	INT			HOOKED_CODE;				// hooking Ÿ��(inline? address modify, ...)
} HOOKED_EXPORT_API, *PHOOKED_EXPORT_API;
typedef BOOL (*lpfnHookedApiCB) (PHOOKED_EXPORT_API pHooked_API, PVOID pTag);


/*
 *	IMPORT TABLE HOOKING Ž���� �� �Լ��� ���� ������ �ѱ�� ���� ����ü
 *
**/ 
typedef struct _HOOKED_IMPORT_API
{
	TCHAR		DLLName[_MAX_PATH + 1];
	TCHAR		ApiName[_MAX_API_NAME + 1];		
	DWORD_PTR	HookdAddr;					// hooking �� ������ api �ּ�	
	DWORD_PTR	DetourAddr;					// jmp �� ���� �ּ� (INLINE HOOKING Ÿ���� ��츸 ��ȿ)
	DWORD_PTR	dwStart;
	DWORD_PTR	dwEnd;
	INT			HOOKED_CODE;				// hooking Ÿ��(inline? address modify, ...)
} HOOKED_IMPORT_API, *PHOOKED_IMPORT_API;
typedef BOOL (*lpfnHookedImportApiCB) (PHOOKED_IMPORT_API pHookedImportApi, PVOID pTag);

typedef union _UAPI_INFO
{
	HOOKED_EXPORT_API	HEA;
	HOOKED_IMPORT_API	HIA;	
} UAPI_INFO;


/*-----------------------------------------------------------------------------
 * ArmorKit �� game client ���� CallBack ���� �Ѱ��ִ� ������ ����ü 
 *		-- game client �� game server �ʿ����� �� ����ü�� �˸� ��
**---------------------------------------------------------------------------*/

typedef struct _HACK_DETECT_INFO
{
	INT		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
	INT		Size;			// Buffer �� ����
	LPSTR	Buf;			// ������ ����	
} HACK_DETECT_INFO, *PHACK_DETECT_INFO;





/*-----------------------------------------------------------------------------
 * ���ǵ��� üũ�� ���� �ڷᱸ��	- AKDETECT_SPEEDHACK
**---------------------------------------------------------------------------*/
typedef struct _SPEED_HACK_CHECK_INFO
{
	DWORD	dwTGT;			// TimeGetTime()
	DWORD	dwGTC;			// GetTickCount()
	DWORD	dwTSC;			// RDTSC
	DWORD	dwQPC;			// QueryPerformanceCounter()
} SPEED_HACK_CHECK_INFO, *PSPEED_HACK_CHECK_INFO;


/*-----------------------------------------------------------------------------
 * ���μ��� ��ĵ�� ���� �ڷᱸ��	- AKDETECT_MALWARE
**---------------------------------------------------------------------------*/
typedef struct _PROCESS_SCAN_INFO
{
	/// ���μ��� ����
	TCHAR	ProcessName[MAX_PATH + 1];
	DWORD	ProcessId;

	/// signature ����	
	DWORD	SignatureIndex;
	DWORD	SianatureType;
	TCHAR	SignatureName[255];

	// ��Ʈ���� �� �͹̳���Ʈ�� �����ؾ���

} PROCESS_SCAN_INFO, *PPROCESS_SCAN_INFO;



/*-----------------------------------------------------------------------------
 * PE üũ���� ���� �ڷᱸ��		- AKDETECT_PEINTEGRITY
**---------------------------------------------------------------------------*/
typedef struct _PE_CHECKSUM_INFO
{
	DWORD HeaderSum;				// PE ����� üũ��
	DWORD CalcSum ;					// ���� üũ�� (HeaderSum ���� �񱳴��)

} PE_CHECKSUM_INFO, *PPE_CHECKSUM_INFO;                 



/*-----------------------------------------------------------------------------
 * �����üũ�� ���� �ڷᱸ��		- AKDETECT_DEBUG
**---------------------------------------------------------------------------*/
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




/*-----------------------------------------------------------------------------
 *  AntiHook 						- AKDETECT_APIHOOK
 *
 *	HOOKED_API_INFO.API_TYPE �� 0 �� ��� HEA ����ü ����
 *	                            1 �� ��� HIA ����ü ����
**---------------------------------------------------------------------------*/

// �ݹ��Լ��� �ѱ�� ���� ����ü
//
typedef struct _HOOKED_API_INFO
{
	INT				API_TYPE;		// 0: EXPORTED API, 1: IMPORTED API, (�ʿ�� �߰�����)
	UAPI_INFO		API_INFO;		
} HOOKED_API_INFO, *PHOOKED_API_INFO;