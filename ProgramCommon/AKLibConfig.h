/*-----------------------------------------------------------------------------
 * AKLibConfig.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _AK_LIB_CONFIG_H_
#define _AK_LIB_CONFIG_H_


// �� hack ���� ��⿡�� �� ���� �� ���� Ƚ���� MAX_HACK_NOTI �� �ѱ��
// ���μ����� ������ �����Ѵ�. 
// 
#define MAX_HACK_NOTI		3

// AKSvc Ÿ�̸� �ֱ� 
//
#ifdef _DEBUG
#define AKSVC_PERIOD		10 * 1000		// aksvc Ÿ�̸�
#else
#define AKSVC_PERIOD		40 * 1000		// aksvc Ÿ�̸�
#endif

#define AKHD_PERIOD			50 * 1000		// hook defender Ÿ�̸�

// ���� ����
#define AKCONFIG_PATH		"AKCfg.ini"

//----------- AKSvc ���� ���� �÷��� ----------------------------------------
// ���ǵ��� Ž��
#define AKCHECK_SPEEDHACK				0x00000001		// 1

// Malware Ž��
#define AKCHECK_MALWARE					0x00000002		// 2

// ���ӹ��̳ʸ�����
#define AKCHECK_PEINTEGRITY				0x00000004		// 3

// �Ƹ�Ŷ ����
#define AKCHECK_AKINTEGRITY				0x00000008		// 4

// ����� ����Ž��
#define AKCHECK_DEBUG					0x00000010		// 5

// ����� ����Ž��(svc)
#define AKCHECK_DEBUGSVC				0x00000020		// 6

// �޸�����
#define AKCHECK_MEMEDIT					0x00000040		// 7

// �ǽɰ��� DLL Ž��
#define AKCHECK_DOUBTDLL				0x00000080		// 8

// ��Ÿ�� �ڵ� ������ Ž��
#define AKCHECK_RUNCODE					0x00000100		// 9

// ���� �ؽ� Ž��
#define AKCHECK_FILEHASH				0x00000200		// 10

// hookdefender - IAT ��ȣ
#define AKCHECK_IAT_HOOK				0x00000400		// 11

// hookdefender - EAT ��ȣ
#define AKCHECK_EAT_HOOK				0x00000800		// 12

//----------- ArmorKit v2 ���� ���� �÷��� ----------------------------------
// AGENT ���� ���� 
#define AKUSE_AGENT						0x00001000		// 13

// update server  ��� ����
#define AKUSE_UPDATE_SVR				0x00002000		// 14

// kernel driver ��� ���� 
#define AKUSE_VIRGINE_SVC				0x00004000		// 15

// api hooker ��� ����
#define AKUSE_API_HOOKER				0x00008000		// 16

// predefined combination flag
// 
#define AKFLAG_NONE						0x00000000
#define AKFLAG_ALL						(AKCHECK_MALWARE |		\
										AKCHECK_DEBUG |			\
										AKCHECK_DEBUGSVC |		\
										AKCHECK_FILEHASH |		\
										AKCHECK_IAT_HOOK |		\
										AKCHECK_EAT_HOOK |		\
										AKUSE_UPDATE_SVR |		\
										AKUSE_VIRGINE_SVC)

#define AKFLAG_ALL_DBG					(AKFLAG_ALL &			\
										~AKCHECK_DEBUG &		\
										~AKCHECK_DEBUGSVC)



// huxley �������� ���� ���� ���� 
//	- AGENT  ��� ����
//	- release mode ������ iat ��ȣ ��� ��� ���� (��Ŀ�� �浹)
#define AKFLAG_FOR_HUXLEY				(AKFLAG_ALL &			\
										~AKCHECK_IAT_HOOK &		\
										~AKCHECK_FILEHASH)
										

										
										
// ����� ���������� ����̹� ������
//
#define AKFLAG_FOR_HUXLEY_DBG			(AKFLAG_ALL_DBG &		\
										~AKCHECK_IAT_HOOK &		\
										~AKCHECK_FILEHASH &		\
										~AKUSE_VIRGINE_SVC)
										
// sun �������� ���� ���� ���� 
//	- AGENT  ��� ����
//	- release mode ������ iat ��ȣ ��� ��� ���� (��Ŀ�� �浹)
//
#define AKFLAG_FOR_SUN					(AKFLAG_ALL &			\
										~AKCHECK_IAT_HOOK &		\
										~AKUSE_AGENT)

										
										
#define AKFLAG_FOR_SUN_DBG				(AKFLAG_ALL_DBG &		\
										~AKCHECK_IAT_HOOK &		\
										~AKUSE_AGENT)


// parfait station �������� ���� ���� ����
//	
#define AKFLAG_FOR_PARFAIT				(AKFLAG_ALL &			\
										~AKCHECK_FILEHASH &		\
										~AKUSE_VIRGINE_SVC)


// ����� ���������� ����̹� ������
//
#define AKFLAG_FOR_PARFAIT_DBG			(AKFLAG_ALL_DBG &		\
										~AKCHECK_FILEHASH &		\
										~AKUSE_VIRGINE_SVC)
										
										



#endif//_AK_LIB_CONFIG_H_