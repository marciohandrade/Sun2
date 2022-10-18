/*-----------------------------------------------------------------------------
 * AKLibConfig.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
**---------------------------------------------------------------------------*/
#ifndef _AK_LIB_CONFIG_H_
#define _AK_LIB_CONFIG_H_


// 각 hack 감지 모듈에서 핵 감지 및 통지 횟수가 MAX_HACK_NOTI 을 넘기면
// 프로세스를 강제로 종료한다. 
// 
#define MAX_HACK_NOTI		3

// AKSvc 타이머 주기 
//
#ifdef _DEBUG
#define AKSVC_PERIOD		10 * 1000		// aksvc 타이머
#else
#define AKSVC_PERIOD		40 * 1000		// aksvc 타이머
#endif

#define AKHD_PERIOD			50 * 1000		// hook defender 타이머

// 설정 파일
#define AKCONFIG_PATH		"AKCfg.ini"

//----------- AKSvc 동작 관련 플래그 ----------------------------------------
// 스피드핵 탐지
#define AKCHECK_SPEEDHACK				0x00000001		// 1

// Malware 탐지
#define AKCHECK_MALWARE					0x00000002		// 2

// 게임바이너리변조
#define AKCHECK_PEINTEGRITY				0x00000004		// 3

// 아머킷 변조
#define AKCHECK_AKINTEGRITY				0x00000008		// 4

// 디버그 상태탐지
#define AKCHECK_DEBUG					0x00000010		// 5

// 디버그 상태탐지(svc)
#define AKCHECK_DEBUGSVC				0x00000020		// 6

// 메모리조작
#define AKCHECK_MEMEDIT					0x00000040		// 7

// 의심가는 DLL 탐지
#define AKCHECK_DOUBTDLL				0x00000080		// 8

// 런타임 코드 변조를 탐지
#define AKCHECK_RUNCODE					0x00000100		// 9

// 파일 해쉬 탐지
#define AKCHECK_FILEHASH				0x00000200		// 10

// hookdefender - IAT 보호
#define AKCHECK_IAT_HOOK				0x00000400		// 11

// hookdefender - EAT 보호
#define AKCHECK_EAT_HOOK				0x00000800		// 12

//----------- ArmorKit v2 동작 관련 플래그 ----------------------------------
// AGENT 실행 여부 
#define AKUSE_AGENT						0x00001000		// 13

// update server  사용 여부
#define AKUSE_UPDATE_SVR				0x00002000		// 14

// kernel driver 사용 여부 
#define AKUSE_VIRGINE_SVC				0x00004000		// 15

// api hooker 사용 여부
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



// huxley 배포본을 위한 실행 설정 
//	- AGENT  사용 안함
//	- release mode 에서는 iat 보호 기능 사용 안함 (패커와 충돌)
#define AKFLAG_FOR_HUXLEY				(AKFLAG_ALL &			\
										~AKCHECK_IAT_HOOK &		\
										~AKCHECK_FILEHASH)
										

										
										
// 디버그 버전에서는 드라이버 사용안함
//
#define AKFLAG_FOR_HUXLEY_DBG			(AKFLAG_ALL_DBG &		\
										~AKCHECK_IAT_HOOK &		\
										~AKCHECK_FILEHASH &		\
										~AKUSE_VIRGINE_SVC)
										
// sun 배포본을 위한 실행 설정 
//	- AGENT  사용 안함
//	- release mode 에서는 iat 보호 기능 사용 안함 (패커와 충돌)
//
#define AKFLAG_FOR_SUN					(AKFLAG_ALL &			\
										~AKCHECK_IAT_HOOK &		\
										~AKUSE_AGENT)

										
										
#define AKFLAG_FOR_SUN_DBG				(AKFLAG_ALL_DBG &		\
										~AKCHECK_IAT_HOOK &		\
										~AKUSE_AGENT)


// parfait station 배포본을 위한 실행 설정
//	
#define AKFLAG_FOR_PARFAIT				(AKFLAG_ALL &			\
										~AKCHECK_FILEHASH &		\
										~AKUSE_VIRGINE_SVC)


// 디버그 버전에서는 드라이버 사용안함
//
#define AKFLAG_FOR_PARFAIT_DBG			(AKFLAG_ALL_DBG &		\
										~AKCHECK_FILEHASH &		\
										~AKUSE_VIRGINE_SVC)
										
										



#endif//_AK_LIB_CONFIG_H_