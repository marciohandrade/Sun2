/*++ HackShield SDK header file   

Copyright (C), AhnLab, Inc. 2002-2008, All rights reserved.

Module Name:

    HShield.h

Abstract:

    This header file defines the data types and constants, and exposes APIs 
	that are defined by HackShield Library.

Tag Information:
	
	This header file is auto-generated at $$HSHIELD_BUILD_DATETIME$$.

--*/
           
#ifndef _HSHIELD_H_INC
#define _HSHIELD_H_INC

// HShield Library Version Info
#define STATE_DEV		0x00
#define STATE_ALPHA		0x01
#define STATE_BETA		0x02
#define STATE_RC_RTM	0x03

// M : Major ����
// m : Minior ����
// p : ��ġ ����
// s1 : STATE_DEV, STATE_ALPHA, STATE_BETA, STATE_RC_RTM ����
// s2 : �� ���º� ���� ����. e.g.) Alpha1, Alpha2
// b : ���� ��ȣ
#define	HSVERSION(M,m,p,s1,s2,b) (M&0x0F)<<28|(m&0x0F)<<24|(p&0x1F)<<19|(s1&0x03)<<17|(s2&0x07)<<14|(b&0x3FFF)



// Initialize Option
#define AHNHS_CHKOPT_SPEEDHACK					0x2
#define AHNHS_CHKOPT_READWRITEPROCESSMEMORY 	0x4
#define AHNHS_CHKOPT_KDTRACER					0x8
#define AHNHS_CHKOPT_OPENPROCESS				0x10
#define AHNHS_CHKOPT_AUTOMOUSE					0x20
#define AHNHS_CHKOPT_MESSAGEHOOK                0x40
#define AHNHS_CHKOPT_PROCESSSCAN                0x80


#define AHNHS_CHKOPT_ALL (	AHNHS_CHKOPT_SPEEDHACK \
							| AHNHS_CHKOPT_READWRITEPROCESSMEMORY \
							| AHNHS_CHKOPT_KDTRACER \
							| AHNHS_CHKOPT_OPENPROCESS \
							| AHNHS_CHKOPT_AUTOMOUSE \
							| AHNHS_CHKOPT_PROCESSSCAN \
							| AHNHS_USE_LOG_FILE \
							| AHNHS_ALLOW_SVCHOST_OPENPROCESS \
							| AHNHS_ALLOW_LSASS_OPENPROCESS \
							| AHNHS_ALLOW_CSRSS_OPENPROCESS \
							| AHNHS_DONOT_TERMINATE_PROCESS )

#define AHNHS_USE_LOG_FILE                      0x100
#define AHNHS_ALLOW_SVCHOST_OPENPROCESS         0x400
#define AHNHS_ALLOW_LSASS_OPENPROCESS           0x800
#define AHNHS_ALLOW_CSRSS_OPENPROCESS           0x1000
#define AHNHS_DONOT_TERMINATE_PROCESS           0x2000
#define AHNHS_DISPLAY_HACKSHIELD_LOGO           0x4000
#define AHNHS_CHKOPT_LOADLIBRARY				0x10000

#define AHNHS_CHKOPT_PROTECTSCREEN				0x20000
#define AHNHS_CHKOPT_PROTECTSCREENEX			0x40000
#define AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION	0x80000
#define AHNHS_CHKOPT_ABNORMAL_FUNCTION_CALL		0x100000
#define AHNHS_CHKOPT_ANTIFREESERVER				0x200000
#define AHNHS_ALLOW_SWITCH_WINDOW				0x400000
#define AHNHS_CHKOPT_STANDALONE					0x800000
#define AHNHS_CHKOPT_PROTECT_D3DX				0x1000000
#define AHNHS_CHKOPT_SELF_DESTRUCTION			0x2000000
#define AHNHS_DISPLAY_HACKSHIELD_TRAYICON		0x4000000
#define AHNHS_CHKOPT_DETECT_VIRTUAL_MACHINE		0x8000000
#define	AHNHS_CHKOPT_UPDATED_FILE_CHECK			0x10000000

#define AHNHS_CHKOPT_SEND_MONITOR_DELAY			0x40000000

// SpeedHack Sensing Ratio
#define	AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST	0x1
#define	AHNHS_SPEEDHACK_SENSING_RATIO_HIGH		0x2
#define	AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL	0x4
#define	AHNHS_SPEEDHACK_SENSING_RATIO_LOW		0x8
#define	AHNHS_SPEEDHACK_SENSING_RATIO_LOWEST	0x10
#define	AHNHS_SPEEDHACK_SENSING_RATIO_GAME		0x20



//ERROR CODE
#define HS_ERR_OK                               0x00000000		// �Լ� ȣ�� ����
#define HS_ERR_UNKNOWN                          0x00000001		// �� �� ���� ������ �߻��߽��ϴ�.
#define HS_ERR_INVALID_PARAM                    0x00000002		// �ùٸ��� ���� �����Դϴ�.
#define HS_ERR_NOT_INITIALIZED                  0x00000003		// �ٽ��� ����� �ʱ�ȭ���� ���� �����Դϴ�.
#define HS_ERR_COMPATIBILITY_MODE_RUNNING       0x00000004		// ���� ���μ����� ȣȯ�� ���� ����Ǿ����ϴ�.
#define HS_ERR_ALREADY_UNINITIALIZED			0x00000005		// Uninitialize�� ����� ���Ŀ��� ������ ��������� Initialize�ؾ� �մϴ�.
#define HS_ERR_STRING_CONVERSION_FAILED			0x00000006		// Unicode ���� ��ȯ�� �����Ͽ����ϴ�. 
#define HS_ERR_EXCEPTION_RAISED					0x00000007		// ����(Exception)�� �߻��Ͽ����ϴ�.
	
#define HS_ERR_INVALID_LICENSE                  0x00000100		// �ùٸ��� ���� ���̼��� Ű�Դϴ�.
#define HS_ERR_INVALID_FILES                    0x00000101		// �߸��� ���� ��ġ�Ǿ����ϴ�. ���α׷��� �缳ġ�Ͻñ� �ٶ��ϴ�.
#define HS_ERR_INIT_DRV_FAILED                  0x00000102
#define HS_ERR_ANOTHER_SERVICE_RUNNING          0x00000103		// �ٸ� �����̳� ���μ������� �ٽ��带 �̹� ����ϰ� �ֽ��ϴ�.
#define HS_ERR_ALREADY_INITIALIZED              0x00000104		// �̹� �ٽ��� ����� �ʱ�ȭ�Ǿ� �ֽ��ϴ�.
#define HS_ERR_DEBUGGER_DETECT                  0x00000105		// ��ǻ�Ϳ��� ����� ������ �����Ǿ����ϴ�. ������� ������ ������Ų �ڿ� �ٽ� ��������ֽñ�ٶ��ϴ�.
#define HS_ERR_EXECUTABLE_FILE_CRACKED			0x00000106		// ���� ������ �ڵ尡 ũ�� �Ǿ����ϴ�.
#define HS_ERR_NEED_ADMIN_RIGHTS				0x00000107		// ADMIN ������ �ʿ��մϴ�.
#define HS_ERR_MODULE_INIT_FAILED				0x00000108	    // �ٽ��� ��� �ʱ�ȭ�� �����Ͽ����ϴ�.


#define HS_ERR_START_ENGINE_FAILED              0x00000200		// ��ŷ ���α׷� ���� ������ ������ �� �����ϴ�.
#define HS_ERR_ALREADY_SERVICE_RUNNING          0x00000201		// �̹� �ٽ��� ���񽺰� ���� ���Դϴ�.
#define HS_ERR_DRV_FILE_CREATE_FAILED           0x00000202		// �ٽ��� ����̹� ������ ������ �� �����ϴ�.
#define HS_ERR_REG_DRV_FILE_FAILED              0x00000203		// �ٽ��� ����̹��� ����� �� �����ϴ�.
#define HS_ERR_START_DRV_FAILED                 0x00000204		// �ٽ��� ����̹��� ������ �� �����ϴ�.
#define HS_ERR_START_THREAD_FAILED			    0x00000205      // �ٽ��� �˻� �����带 ������ �� �����ϴ�.
#define HS_ERR_ALREADY_GAME_STARTED	            0x00000206		// ������ �̹� ���� ���Դϴ�. ������ �ߺ��ؼ� ������ �� �����ϴ�. 
#define HS_ERR_VIRTUAL_MACHINE_DETECT			0x00000207		// ���� OS �Ǵ� ���ķ����Ϳ����� ������ ������ �� �����ϴ�.  

#define HS_ERR_SERVICE_NOT_RUNNING              0x00000301		// �ٽ��� ���񽺰� ����ǰ� ���� ���� �����Դϴ�.
#define HS_ERR_SERVICE_STILL_RUNNING            0x00000302		// �ٽ��� ���񽺰� ���� �������� �����Դϴ�.

#define HS_ERR_NEED_UPDATE                      0x00000401		// �ٽ��� ����� ������Ʈ�� �ʿ��մϴ�.

#define HS_ERR_API_IS_HOOKED					0x00000501		// �ش� API�� ��ŷ�Ǿ� �ִ� �����Դϴ�.

#define HS_ERR_GET_SDKVERSION_FAILED			0x00000601		// �������� �������µ� ����

#define HS_ERR_LMP_START						0x00000701		// LMP ����� ������ �� �����ϴ�.



// Server-Client ���� ����  ERROR CODE
#define HS_ERR_ANTICPCNT_MAKEACKMSG_INVALIDPARAM				0x00010000
#define HS_ERR_ANTICPCNT_MAKEACKMSG_MAKESESSIONKEY_FAIL			0x00010001
#define HS_ERR_ANTICPCNT_MAKEACKMSG_INITCRYPT_FAIL				0x00010002
#define HS_ERR_ANTICPCNT_MAKEACKMSG_DECRYPTMESSAGE_FAIL			0x00010003
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETMEMHASH_FAIL				0x00010004
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETMODNAME_FAIL				0x00010005
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETFILEHASH_FAIL			0x00010006
#define HS_ERR_ANTICPCNT_MAKEACKMSG_GETHSHIELDHASH_FAIL			0x00010007
#define HS_ERR_ANTICPCNT_MAKEACKMSG_MAKEHSHIELDFLAG_FAIL		0x00010008
#define HS_ERR_ANTICPCNT_MAKEACKMSG_MAKEACKSEQUENCENUM_FAIL		0x00010009
#define HS_ERR_ANTICPCNT_MAKEACKMSG_ENCRYPTMESSAGE_FAIL			0x0001000A

#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_INVALIDPARAM			0x00010010
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_MAKESESSIONKEY_FAIL		0x00010011
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_INITCRYPT_FAIL			0x00010012
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_DECRYPTMESSAGE_FAIL		0x00010013
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_GETGUIDFROMFILE_FAIL	0x00010014
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_MAKEACKSEQUENCENUM_FAIL	0x00010015
#define HS_ERR_ANTICPCNT_MAKEGUIDACKMSG_ENCRYPTMESSAGE_FAIL		0x00010016	

#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_INVALIDPARAM			0x00010020
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_OPENFILEMAPPING_FAIL	0x00010021
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_MAPVIEWOFFILE_FAIL		0x00010022
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_INITCRYPT_FAIL			0x00010023
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_DECRYPTMESSAGE_FAIL	0x00010024
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_INVALIDMMF				0x00010025
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_OPENEVENT_FAIL			0x00010026
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_GETMODFILENAME_FAIL	0x00010027
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_GETINSTRUCTION_FAIL	0x00010028
#define HS_ERR_ANTICPCNT_SAVEFUNCADDRESS_ENCRYPTMESSAGE_FAIL	0x00010029

#define HS_ERR_ANTICPCNT_DEBUGGER_DETECTED						0x00010030


// Server-Client Ex ���� ���� ���� �ڵ� ����
#define HS_ERR_ANTICPXCNT_BASECODE								0xE4010000

// �Ķ���� ���� �ùٸ��� �ʽ��ϴ�
#define HS_ERR_ANTICPXCNT_INVALID_PARAMETER						(HS_ERR_ANTICPXCNT_BASECODE + 0x1)
// �߸��� �޸� �ּҸ� �����Ͽ����ϴ�
#define HS_ERR_ANTICPXCNT_INVALID_ADDRESS						(HS_ERR_ANTICPXCNT_BASECODE + 0x2)
// �޸𸮰� �����մϴ�
#define HS_ERR_ANTICPXCNT_NOT_ENOUGH_MEMORY						(HS_ERR_ANTICPXCNT_BASECODE + 0x3)
// CRC Table�� �ʱ�ȭ �ϴµ� �����Ͽ����ϴ�
#define HS_ERR_ANTICPXCNT_CRC_TABLE_INIT_FAILED					(HS_ERR_ANTICPXCNT_BASECODE + 0x4)
// �޽��� ���̰� �ùٸ��� �ʽ��ϴ�
#define HS_ERR_ANTICPXCNT_BAD_LENGTH							(HS_ERR_ANTICPXCNT_BASECODE + 0x5)
// ������ ũ�Ⱑ �۽��ϴ�
#define HS_ERR_ANTICPXCNT_INSUFFICIENT_BUFFER					(HS_ERR_ANTICPXCNT_BASECODE + 0x6)
// ���� �������� �������� �ʴ� ����̴�
#define HS_ERR_ANTICPXCNT_NOT_SUPPORTED							(HS_ERR_ANTICPXCNT_BASECODE + 0x7)
// ������ ã�� �� �����ϴ�
#define HS_ERR_ANTICPXCNT_FILE_NOT_FOUND						(HS_ERR_ANTICPXCNT_BASECODE + 0x8)
// �Է¹��� �޽����� ũ�Ⱑ �ùٸ��� �ʽ��ϴ�
#define HS_ERR_ANTICPXCNT_INVALID_MESSAGE_SIZE					(HS_ERR_ANTICPXCNT_BASECODE + 0x9)
// �ùٸ� ������ �ƴմϴ�
#define HS_ERR_ANTICPXCNT_BAD_FORMAT							(HS_ERR_ANTICPXCNT_BASECODE + 0xA)
// ����� ��Ȳ�� �����Ͽ����ϴ�
#define HS_ERR_ANTICPXCNT_DEBUGGER_DETECTED						(HS_ERR_ANTICPXCNT_BASECODE + 0xB)
// �ٽ��� ��� ��ΰ� �� ���Ǿ��ų� �ٽ��� ����� �ùٸ��� �ʽ��ϴ�
#define HS_ERR_ANTICPXCNT_BAD_HSHIELD_MODULE					(HS_ERR_ANTICPXCNT_BASECODE + 0xC)
// Ŭ���̾�Ʈ ����� �ùٸ��� �ʽ��ϴ�
#define HS_ERR_ANTICPXCNT_BAD_CLIENT_FILE						(HS_ERR_ANTICPXCNT_BASECODE + 0xD)
// ������ ���� ���� ���� ��û �޽����� �ùٸ��� �ʽ��ϴ�
#define HS_ERR_ANTICPXCNT_BAD_REQUEST							(HS_ERR_ANTICPXCNT_BASECODE + 0xE)
// �ٽ��� �ھ� ������ ���������� �������� �ʽ��ϴ� (����̽� ����̹�)
#define HS_ERR_ANTICPXCNT_HSHIELD_CORE_ENGINE_NOT_WORKING		(HS_ERR_ANTICPXCNT_BASECODE + 0xF)
// ��ŷ �õ��� ���� �ý����� ������ �� ��� �߻�
#define HS_ERR_ANTICPXCNT_UNKNOWN								(HS_ERR_ANTICPXCNT_BASECODE + 0xFF)


// CallBack Code
#define AHNHS_ACTAPC_STATUS_HACKSHIELD_RUNNING		0x010001		// �ٽ��� ���� Ȯ�� ����� ����ϴ� ��쿡 ȣ�� �Ǵ� �ݹ��Դϴ�.  
																	// �ٽ��尡 ���������� ���� �ϰ� �ִ� ���� �� �ݹ��� ȣ�� �˴ϴ�. 
																	// ���� �ڵ� �� �ƴϹǷ� ���ӿ��� ���������� Ȯ�� �մϴ�. 
#define AHNHS_ACTAPC_DETECT_ALREADYHOOKED			0x010101		// �Ϻ� API�� �̹� ��ŷ�Ǿ� �ִ� �����Դϴ�. (�׷��� �����δ� �̸� �����ϰ� �ֱ� ������ ��ŷ���α׷��� �������� �ʽ��ϴ�.)
#define AHNHS_ACTAPC_DETECT_AUTOMOUSE				0x010102		// ���丶�콺 ������ �����Ǿ����ϴ�.	
#define AHNHS_ACTAPC_DETECT_AUTOMACRO				0x010104		// �����ũ�� ������ �����Ǿ����ϴ�.	
#define AHNHS_ACTAPC_DETECT_HOOKFUNCTION			0x010301		// ��ȣ API�� ���� ��ŷ ������ �����Ǿ����ϴ�.
#define AHNHS_ACTAPC_DETECT_DRIVERFAILED			0x010302		// ��ŷ ���� ����̹��� �ε���� �ʾҽ��ϴ�.
#define AHNHS_ACTAPC_DETECT_SPEEDHACK				0x010303		// ���ǵ��ٷ��� ���α׷��� ���� �ý��� �ð��� ����Ǿ����ϴ�.
#define AHNHS_ACTAPC_DETECT_MESSAGEHOOK				0x010306		// �޽��� ��ŷ�� �õ��Ǿ����� �̸� �������� ���߽��ϴ�.
#define AHNHS_ACTAPC_DETECT_KDTRACE					0x010307		// ����� Ʈ���̽��� �߻��ߴ�.(Ŀ�� ����� Ȱ��ȭ, ���� �극��ũ ������ ó��)
#define AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED			0x010308		// ��ġ�� ����� Ʈ���̽��� ����Ǿ���.
#define AHNHS_ACTAPC_DETECT_SPEEDHACK_RATIO			0x01030B		// ���ǵ��� ���� �ɼ��� 'GAME'�� ��� �� �ݹ����� �ֱ� 5�ʵ����� �ð������� ���޵˴ϴ�.
#define AHNHS_ENGINE_DETECT_GAME_HACK				0x010501		// ���� ��ŷ���� ������ �߰ߵǾ����ϴ�.
#define AHNHS_ENGINE_DETECT_WINDOWED_HACK			0x010503		// â��� ��ŷ��
#define AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS	0x010601		// �ڵ� ��ġ �õ��� �����Ǿ����ϴ�.
#define AHNHS_ACTAPC_DETECT_MODULE_CHANGE			0x010701		// �ٽ��� ���ø���� ����Ǿ����ϴ�.
#define AHNHS_ACTAPC_DETECT_ENGINEFAILED			0x010702		// ��������� ���������� �ʽ��ϴ�.
#define AHNHS_ACTAPC_DETECT_CODEMISMATCH			0x010703		// �ڵ尡 ��ġ���� �ʽ��ϴ�. 
#define AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED		0x010704		// ȭ�� ��ȣ����� ���������� �ʽ��ϴ�.
#define AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP		0x010705		// ��ȣ ��⿡ ���ؼ� �޸� ������ �����Ǿ����ϴ�.
#define AHNHS_ACTAPC_DETECT_LMP_FAILED				0x010706		// �ٽ��� ���� �޸� ��ȣ ����� ���������� �ʽ��ϴ�.
#define AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL	0x010801		// Game EXE���� �ڵ带 �ܺ� ��⿡�� ���Ƿ� ȣ���Ͽ����ϴ�.
#define AHNHS_ACTAPC_DETECT_ANTIFREESERVER			0x010901		// Game�� �ùٸ� ������ ������ ���� �ʾҽ��ϴ�.
#define AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS	0x010A01	// HackShield ���� ����� ���������� �������� �ʽ��ϴ�.


#pragma pack(push)
#pragma pack(8)

#ifndef __ANTICPX_TRANS_BUFFER__
#define __ANTICPX_TRANS_BUFFER__

#define ANTICPX_TRANS_BUFFER_MAX	400

typedef struct _AHNHS_TRANS_BUFFER
{
	unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX/* �ۼ��� ��Ŷ�� �ִ� ũ�� */];
	unsigned short nLength;
} AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;

#endif // __ANTICPX_TRANS_BUFFER__

#ifndef __AHNHS_EXT_ERRORINFO__
#define __AHNHS_EXT_ERRORINFO__

typedef struct AHNHS_EXT_ERRORINFOA
{

	CHAR szServer[MAX_PATH];
	CHAR szUserId[MAX_PATH];
	CHAR szGameVersion[MAX_PATH];

} AHNHS_EXT_ERRORINFOA, *PAHNHS_EXT_ERRORINFOA;

typedef struct AHNHS_EXT_ERRORINFOW
{

	WCHAR szServer[MAX_PATH];
	WCHAR szUserId[MAX_PATH];
	WCHAR szGameVersion[MAX_PATH];

} AHNHS_EXT_ERRORINFOW, *PAHNHS_EXT_ERRORINFOW;
#if defined(UNICODE) | defined(_UNICODE)
	#define AHNHS_EXT_ERRORINFO	AHNHS_EXT_ERRORINFOW
#else
	#define	AHNHS_EXT_ERRORINFO	AHNHS_EXT_ERRORINFOA
#endif // !UNICODE

#endif __AHNHS_EXT_ERRORINFO__

#pragma pack(pop) // ���� alignment ����.






// CallBack�� ���޵Ǵ� ������
#pragma pack(push)
#pragma pack(1)

typedef struct tagACTAPCPARAM_DETECT_HOOKFUNCTION
{	
	char szFunctionName[128];		// file path
	char szModuleName[128];
	
} ACTAPCPARAM_DETECT_HOOKFUNCTION, *PACTAPCPARAM_DETECT_HOOKFUNCTION;


// ���丶�콺 ���� APC Structure
typedef struct tagACTAPCPARAM_DETECT_AUTOMOUSE
{
	BYTE	byDetectType;			// AutoMouse ���� API ȣ�� 1, AutoMouse ���� API ���� 2
	DWORD	dwPID;					// AutoMouse ���μ��� �Ǵ� API �� ������ ���μ���
	CHAR	szProcessName[16+1];	// ���μ�����
	CHAR	szAPIName[128];			// ȣ��� API �� �Ǵ� ������ API ��

} ACTAPCPARAM_DETECT_AUTOMOUSE, *PACTAPCPARAM_DETECT_AUTOMOUSE;

#define	EAGLE_AUTOMOUSE_APCTYPE_API_CALLED					1
#define	EAGLE_AUTOMOUSE_APCTYPE_API_ALTERATION				2
#define	EAGLE_AUTOMOUSE_APCTYPE_SHAREDMEMORY_ALTERATION		3

// �����ũ�� ���� APC Structure
typedef struct
{	
	BYTE byDetectType;		// AutoKeyboard 1, AutoMouse ���� API ���� 2
	CHAR szModuleName[128];
	
} ACTAPCPARAM_DETECT_AUTOMACRO, *PACTAPCPARAM_DETECT_AUTOMACRO;

#define	EAGLE_AUTOMACRO_APCTYPE_KEYBOARD	1
#define	EAGLE_AUTOMACRO_APCTYPE_MOUSE		2

// AHNHS_ENGINE_DETECT_GAME_HACK	Callback Structure
typedef struct tagAHNHS_ENGINE_DETECT_GAME_HACK_CALLBACK
{
	CHAR	szMessageA[128+1];		// Detect game hack name
	DWORD	cbMessageLen;			// name length
	DWORD	dwHackNo;

} AHNHS_ENGINE_DETECT_GAME_HACK_CALLBACK, *PAHNHS_ENGINE_DETECT_GAME_HACK_CALLBACK;

#pragma pack(pop)


// Server-Client ���� ������ ���Ǵ� ������ ũ��
#define SIZEOF_REQMSG		160
#define SIZEOF_ACKMSG		72
#define SIZEOF_GUIDREQMSG	20
#define SIZEOF_GUIDACKMSG	340


// HackShield ���� ���� ��  
enum HS_RUNNING_STATUS {
	HS_RUNNING_STATUS_CHECK_MONITORING_THREAD = 1,
};


// Callback Function Prototype
typedef int (__stdcall* PFN_AhnHS_Callback)(
	long lCode,
	long lParamSize,
	void* pParam 
);



// Export API
#if defined(__cplusplus)
extern "C"
{
#endif

/*!
 * 
 * @remarks	�ٽ��� �ʱ�ȭ �Լ�
 *
 * @param	szFileName : [IN] Ehsvc.dll�� Full Path
 * @param	pfn_Callback : [OUT] Callback Function Pointer
 * @param	nGameCode : [IN] Game Code
 * @param	szLicenseKey : [IN] License Code
 * @param	unOption : [IN] Initialize Options
 * @param	unSHackSensingRatio : [IN] SpeedHack Sensing Ratio
 *
 */
int
__stdcall
_AhnHS_InitializeA (
	const char*			szFileNameA,
	PFN_AhnHS_Callback	pfn_Callback,
	int					nGameCode,
	const char*			szLicenseKeyA,
	unsigned int		unOption,
	unsigned int		unSHackSensingRatio
	);
int
__stdcall
_AhnHS_InitializeW (
	const wchar_t*		szFileNameW,
	PFN_AhnHS_Callback	pfn_Callback,
	int					nGameCode,
	const wchar_t*			szLicenseKeyW,
	unsigned int		unOption,
	unsigned int		unSHackSensingRatio
	);
#if defined(UNICODE) | defined(_UNICODE) 
	#define _AhnHS_Initialize	_AhnHS_InitializeW
#else
	#define _AhnHS_Initialize	_AhnHS_InitializeA
#endif // !UNICODE

/*!
 * 
 * @remarks	�ٽ��� ���� �Լ�
 *
 */
int
__stdcall
_AhnHS_StartService ();


/*!
 * 
 * @remarks	�ٽ��� ���� �Լ�
 *
 */
int
__stdcall
_AhnHS_StopService ();


/*!
 * 
 * @remarks	�ٽ��� ��� ���� �Լ��� ����� Ű���� ���� �޽��� �Ű��� ��ɸ� ���� ��Ų��.
 *
 * @param	unPauseOption : [IN] ���� �Ϸ��� ����� �ɼ� ( ����� AHNHS_CHKOPT_MESSAGEHOOK�� ����)
 *
 */
int
__stdcall
_AhnHS_PauseService (
	unsigned int unPauseOption
	);


/*!
 * 
 * @remarks	�ٽ��� ��� �簳 �Լ��� ����� Ű���� ���� �޽��� �Ű��� ��ɸ� �簳 ��Ų��.
 *
 * @param	unResumeOption : [IN] �簳 �Ϸ��� ����� �ɼ� ( ����� AHNHS_CHKOPT_MESSAGEHOOK�� ����)
 *
 */
int
__stdcall
_AhnHS_ResumeService (
	unsigned int unResumeOption
	);


/*!
 * 
 * @remarks	�ٽ��� ���� ���� �Լ�
 *
 */
int
__stdcall
_AhnHS_Uninitialize ();

/*!
 * 
 * @remarks	�����κ��� CRC�� ��û���� ��� �ش� �޽����� ���� ����޽����� �����.
 *
 * @param	pbyReqMsg : [IN] ��ȣȭ�� Request Message, �� ������ ũ��� ���� ���ǵ� SIZEOF_REQMSG�̴�.
 * @param	pbyAckMsg : [OUT] ��ȣȭ�� Ack Message, �� ������ ũ��� ���� ���ǵ� SIZEOF_ACKMSG�̴�.
 *
 */
int
__stdcall
_AhnHS_MakeAckMsg (
	unsigned char *pbyReqMsg,
	unsigned char *pbyAckMsg
	);


/*!
 * 
 * @remarks	�����κ��� GUID�� ��û���� ��� �ش� �޽����� ���� ����޽����� �����.
 *
 * @param	pbyGuidReqMsg : [IN] ��ȣȭ�� GUID Request Message, �� ������ ũ��� ���� ���ǵ� SIZEOF_GUIDREQMSG�̴�.
 * @param	pbyGuidAckMsg : [OUT] ��ȣȭ�� GUID Ack Message, �� ������ ũ��� ���� ���ǵ� SIZEOF_GUIDACKMSG�̴�.
 *
 */
int
__stdcall
_AhnHS_MakeGuidAckMsg (
	unsigned char *pbyGuidReqMsg,
	unsigned char *pbyGuidAckMsg
	);
							

/*!
 * 
 * @remarks	�Լ� ������ ����Ʈ�� �޾Ƶ鿩 ���� ���ӻ翡 ������ ���� �������̶��
 *			�޸𸮿� ���� CRC ������ ����� ����.
 *
 * @param	unNumberOfFunc : [IN] �Լ� ������ ����
 * @param	... : [IN] �Լ� ������ ����Ʈ
 *
 */
int
__stdcall
_AhnHS_SaveFuncAddress (
	unsigned int unNumberOfFunc,
	...
	);


int 
__stdcall
_AhnHS_CheckAPIHookedA (
	const char*	szModuleNameA,
	const char*	szFunctionNameA,
	const char*	szSpecificPathA
	);
int 
__stdcall
_AhnHS_CheckAPIHookedW (
	const wchar_t*	szModuleNameW,
	const wchar_t*	szFunctionNameW,
	const wchar_t*	szSpecificPathW
	);
#if defined(UNICODE) | defined(_UNICODE)
	#define _AhnHS_CheckAPIHooked	_AhnHS_CheckAPIHookedW
#else
	#define _AhnHS_CheckAPIHooked	_AhnHS_CheckAPIHookedA
#endif // !UNICODE


/*!
 * 
 * @remarks	�����κ��� ��û���� ��� �ش� �޽����� ���� ����޽����� �����.
 *
 * @param	pbyRequest : [IN] ��û �޽��� ����
 * @param	ulRequestLength : [IN] ��û �޽��� ����
 * @param	pResponseBuffer : [OUT] ���� �޽��� ����
 *
 */
int
__stdcall
_AhnHS_MakeResponse (
	unsigned char *pbyRequest,
	unsigned long ulRequestLength, 
	PAHNHS_TRANS_BUFFER pResponseBuffer
	);

/*!
 * 
 * @remarks	DirectInput�� Buffered ��Ŀ����� Scan �ڵ� ó��.
 *
 * @param	pbyRequest : [IN] ��û �޽��� ����
 *
 */
int
__stdcall
_AhnHS_PreTranslateBufferedDIMessage ( 
	IN USHORT usMakeCode 
	);

/*!
 * 
 * @remarks	DirectInput�� Buffered ��Ŀ����� Scan �ڵ� ó��.
 *
 * @param	dwCnt : [IN] �Է����� ó���ϴ� ���� ( key down, up�� ���� ó���ϴ� ���� 1 ���)
 *
 */
int
__stdcall
_AhnHS_PreTranslateBufferedDICounter(DWORD dwCnt = 2);

/*!
 * 
 * @remarks	DirectInput�� Immediate ��Ŀ����� Scan �ڵ� ó��.
 *
 * @param	pbyRequest : [IN] ��û �޽��� ����
 * @param	ulRequestLength : [IN] ��û �޽��� ����
 * @param	pResponseBuffer : [OUT] ���� �޽��� ����
 *
 */
int
__stdcall
_AhnHS_PreTranslateImmediateDIMessage ( 
   IN PBYTE pBitmap, 
   IN UINT unBitmapSize 
   );

/*!
 * 
 * @remarks	������ Direct3DCreate9 API�� ���� Wrapper Function
 *
 * @param	SDKVersion : [IN] Direct3DCreate9 API�� ���ڷ� ���� SDK Version
 *
 */
PVOID
__stdcall
_AhnHS_Direct3DCreate9 (
	IN UINT SDKVersion
	);


/*!
 * 
 * @remarks	������ Direct3DCreate8 API�� ���� Wrapper Function
 *
 * @param	SDKVersion : [IN] Direct3DCreate8 API�� ���ڷ� ���� SDK Version
 *
 */
PVOID
__stdcall
_AhnHS_Direct3DCreate8 (
	IN UINT SDKVersion
	);


/*!
 * 
 * @remarks	������ sendto API�� ���� Wrapper Function (sendto API�� �����ϰ� ���)
 *
 * @param	s : [IN] ����
 * @param	buf : [IN] ���۵� ����
 * @param	len : [IN] ������ ����
 * @param	flags : [IN] �Լ� ��Ȱ�� ��Ÿ���� �÷���
 * @param	to : [IN] ����(������)�� ���۵� ���� ȣ��Ʈ�� ��巹��
 * @param	tolen : to ������ ũ��
 *
 */
int
__stdcall
_AhnHS_sendto (			
	IN  int s,
	IN  char* buf,
	IN  int len,
	IN  int flags,
	IN  const struct sockaddr* to,
	IN  int tolen
	);

int
__stdcall
_AhnHS_GetSDKVersion ( 
	OUT DWORD *dwVersion 
	);

/*!
 * 
 * @remarks	�ٽ��� ����͸� ���񽺸� �����մϴ�.
 *
 * @param	IN AHNHS_EXT_ERRORINFO HsExtErrorInfo :  ����͸� ���� �Է� ������
                                  - ���� URL�ּ�. ( "123.456.789.123" or "www.monitor.com" )
								  - User ID		  ( "User1" )
								  - Game Version  ( "5.0.2.1" )
			IN LPCSTR szFileName : ehsvc.dll ��ü ���.
 */
int 
__stdcall 
_AhnHS_StartMonitorA ( 
		IN AHNHS_EXT_ERRORINFOA HsExtErrorInfoA,
		IN const char* szFileNameA 
		);		
int
__stdcall
_AhnHS_StartMonitorW ( 
		IN AHNHS_EXT_ERRORINFOW HsExtErrorInfoW,
		IN const wchar_t* szFileNameW 
		);
#if defined(UNICODE) | defined(_UNICODE)
	#define _AhnHS_StartMonitor	_AhnHS_StartMonitorW
#else
	#define _AhnHS_StartMonitor _AhnHS_StartMonitorA
#endif // !UNOCODE	


/*!
 * 
 * @remarks	�ٽ��� ����͸� ���񽺸� ���� ���̵� ���� �Է�
 *
 * @param	IN LPCSTR szUserID : ���̵� 
 *
 * @remarks
		_AhnHS_StartMonitor������ ���̵� ������ ������ �ٽ��� �ʱ�ȭ ������ ���� ������ ���� ���ϴ� ��찡 
		�ִ�. ���� ���̵� �ƴ� ������ �� �Լ��� ȣ���Ͽ� ���������� ��´�.
		���̵� ��� �������� ���̵� ������ ���� �������� ���۵ȴ�.
 */
void
__stdcall 
_AhnHS_SetUserIdA ( 
		IN const char* szUserIDA
		);
void
__stdcall
_AhnHS_SetUserIdW ( 
		IN const wchar_t* szUserIDW
		);
#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_SetUserId	_AhnHS_SetUserIdW
#else
	#define _AhnHS_SetUserId	_AhnHS_SetUserIdA
#endif // !UNICODE




/*!
 * 
 * @remarks	�Էµ� ��ο� �ش��ϴ� ��⳻�� �ٽ��� ������ ���������� ���ԵǾ� �ִ��� Ȯ���Ѵ�.
 *
 * @param	const char* szModulePath : ��� ���(��ü ���)
 *
 * @remarks
		�ش� ����� ���ȿ� CSInspector���� ��ŷ ������ ������ ������  �����ϴ��� Ȯ����
 * @return
        TRUE : ���� ���� 
		FALSE : �������� ����
 */

BOOL
__stdcall 
_AhnHS_IsModuleSecureA ( 
		IN const char* szModulePathA
		);
BOOL
__stdcall
_AhnHS_IsModuleSecureW ( 
		IN const wchar_t* szModulePathW
		);
#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_IsModuleSecure	_AhnHS_IsModuleSecureW
#else
	#define _AhnHS_IsModuleSecure	_AhnHS_IsModuleSecureA
#endif // !UNICODE



/*!
 * 
 * @remarks	
 *
 *
 */
int
__stdcall
_AhnHS_CheckFileCRCA ();

int
__stdcall
_AhnHS_CheckFileCRCW ();
#if defined(UNICODE) | defined(_UNICODE) 
	#define _AhnHS_CheckFileCRC	_AhnHS_CheckFileCRCW
#else
	#define _AhnHS_CheckFileCRC	_AhnHS_CheckFileCRCA
#endif // !UNICODE


/*!
 * 
 * �ٽ���� ���õ� �������� ������ ���ӻ縦 ������ �ʰ� ������ ó���ϱ� ���� ���ߵ� ������μ� 
 * ��ŷ�� �߻��� �� �ٽ����� �α׸� ���� ������ �����Ͽ� ��ö�������ҿ� ���� �� �ִ±���̴�.
 *
 * @param	dwError : �ٽ��� �����ڵ�.   ex) AHNHS_ENGINE_DETECT_GAME_HACK(0x10501)
 * @param	szUserID : ���� ���� ���̵�. ex) "User1"
 *
 * @remarks
 *		�ٽ��� �����ڵ�� _AhnHS_Initialize�Լ��� 2��° ���ڰ��� �ԷµǴ� �ٽ��� �ݹ��Լ��� 
 *		ù��° ������ lCode ���̴�.
 */

void
__stdcall 
_AhnHS_SendHsLogA ( IN DWORD dwError,
					IN const char* szUserID,
					IN const char* szHShieldPath
				  );
void
__stdcall
_AhnHS_SendHsLogW ( IN DWORD dwError,
					IN const wchar_t* szUserIDW,
					IN const wchar_t* szHShieldPathW
				  );
#if defined(UNICODE) | defined(_UNICODE)
	#define	_AhnHS_SendHsLog	_AhnHS_SendHsLogW
#else
	#define _AhnHS_SendHsLog	_AhnHS_SendHsLogA
#endif // !UNICODE

int 
__stdcall 
_AhnHS_CheckHackShieldRunningStatus();



#if defined(__cplusplus)
}
#endif

#endif _HSHIELD_H_INC
