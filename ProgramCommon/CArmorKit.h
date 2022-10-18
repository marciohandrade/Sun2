/*-----------------------------------------------------------------------------
 * CArmorKit.h 
 *-----------------------------------------------------------------------------
 * ArmorKit 클래스 
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2005, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2005, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#ifndef _C_ARMOR_KIT_H_
#define _C_ARMOR_KIT_H_


#include "ArmorKit.h"
#include <MMSystem.h>
#include <vector>

#include "IGhostUtil.h"			// GhostApi 를 위한 인터페이스

// 전체 탐지 및 에러처리를 위한 유저 콜백함수 타입
//
typedef INT (*LPFN_USERCALLBACK )(PHACK_DETECT_INFO pHDI);


/*-----------------------------------------------------------------------------
 * CArmorKit 클래스 정의
**---------------------------------------------------------------------------*/
class CArmorKit 
{
private:
	unsigned int	dwActionFlags;		// 액션 플래그
	DWORD	dwPEAuthType;				// PE인증 방법
	DWORD	dwActionMode;				// 액션 모드
	DWORD	dwFrequency;				// 액션 주기	
	MMRESULT	uTimerId;				// 타이머 아이디
	DWORD	dwDebugLevel;				// 디버그 레벨 		
	TCHAR	szLauncherName[MAX_PATH+1];	// 디버그 체크를 위한 부모프로세스 이름

	CRITICAL_SECTION m_Critical;

	/// CArmorKit 이 동작중인지.. -> CArmorKit.Start() 가 호출되었다면 True..
	//	WARNING !!!
	//		이 멤버는 thread-safe 하지 않음.
	//
	BOOL m_bStarted;
	BOOL m_useFilelog;

	// 2006.09.20 노용환 추가 
	//	GhostApi 사용
	// 
	RSCM_IGhost m_rscm_ghost;
	IGhost* m_ghost;

	// 2007.01.22 함지만 추가
	// AKSupport적용을 위한 멤버변수 추가
	// m_Timer와 m_DetectThread의 경우 헤더파일로 인해
	// LPVOID로 지정 (실제 사용시 캐스팅 주의)
	HANDLE	m_hEvent;
	LPVOID	m_Timer;
	LPVOID	m_DetectThread;
	int		ResumeDetectThread();
	IGhost* GetGhostPtr() { return m_ghost; }

public:
	DWORD	m_os_version ;							// 클라이언트의 윈도우즈 버젼 

	LPFN_USERCALLBACK	lpfnUserCallBack;		// 유저 콜백 함수 포인터	
	BOOL IsThreadRunning(void);		

	////
	// 환경설정 및 인터페이스 함수
	////
	VOID	SetActionFlags( unsigned int dwSetActionFlags );	// 액션 플래그를 설정하는 함수
	DWORD	GetActionFlags( VOID );						// 액션 플래그를 가져오는 함수
    VOID	SetLauncherName( TCHAR * lptszLauncherName);// 디버깅탐지를 위해 게임의 런처문자열을 설정하는 함수
	TCHAR*	GetLauncherName(VOID);						// 런처문자열을 가져오는 함수 
	
	VOID	SetActionMode ( DWORD dwSetActionMode, DWORD dwFrequency );	// 액션모드를 설정하는 함수
	VOID	SetDebugLevel ( DWORD dwSetDebugLevel ) ;					// 디버그 레벨을 설정하는 함수
	VOID	SetWindowVersion() ;										// 윈도우 버젼을 설정하는 함수	

	BOOL GetUseFileLog(void);							// 파일로그 관련 메소드 
	void SetUseFileLog(const BOOL Value);

	////
	// 구동에 필요한 함수들
	////
	INT		Init(LPFN_USERCALLBACK lpfnUserCallBack, TCHAR	*lpVersion);	// Armorkit을 초기화하는 함수, 버전 지정 기능 추가
	BOOL	Start() ;									// Armorkit을 구동시키는 함수
	VOID	Release() ;									// Armorkit의 자원을 해제시키는 함수
	
	INT		SetPatternList(TCHAR *list, int length);
	INT		GenerateSid( int seed, char *sid );         // Server와의 헬쓰체크 위한 함수
	
	// 각 모드에 따른 구동함수들
	BOOL	DoByTimer() ;
	
	// AKTimer callback
	static void __stdcall OnAKTimer(unsigned int timer_id, DWORD_PTR user_tag);

	CArmorKit();
	~CArmorKit() ;

};




#endif