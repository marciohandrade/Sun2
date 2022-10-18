/*-----------------------------------------------------------------------------
 * EnumProc.h
 *-----------------------------------------------------------------------------
 * process enumerating module 
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2006, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2006, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#ifndef _ENUM_PROC_H_
#define _ENUM_PROC_H_

#include <windows.h>
#include <vector>

/// Window 플랫폼코드

#define OSTYPE_WIN_95		0x0400
#define OSTYPE_WIN_98		0x0410
#define OSTYPE_WIN_SE		0x0410
#define OSTYPE_WIN_ME		0x0490

#define	OSTYPE_WIN_NT		0x0400

#define OSTYPE_WIN_2000		0x0500
#define OSTYPE_WIN_XP		0x0501
#define OSTYPE_WIN_2003		0x0502
#define OSTYPE_WIN_LONGHORN	0x0600


/// 편의상 재정의
// windows 95 family 계열은 0x000F 마스크적용

#define		_WIN95			0x0001
#define		_WIN98			0x0002
#define		_WINSE			0x0004
#define		_WINME			0x0008

// windows nt family 계열은 0x00F0 마스크적용

#define		_WINNT			0x0010
#define		_WIN2000		0x0020
#define		_WINXP			0x0040
#define		_WIN2003		0x0080
#define		_WINLONGHORN	0x00c0

#define		MASK_WIN95		0x000F
#define		MASK_WINNT		0x00F0

/// Windows NT/2000/XP/2003/LONGHORN 패밀리인지를 확인하는 매크로
//
//  < 2006.03.31 노용환 >
//  ISNTFAMILY(GetWinVersion()) 형태로 사용하면 되는데 
//	절대로 
//		if ( ISNTFAMILY(GetWinVersion()) == TRUE ) 
//		{
//			// for ntfamily code 
//			//
//		}
//	형태로 사용하면 안된다. 
//	TRUE 는 windef.h (또는 다른 데서..) 단순히 1 로 정의되어있을 뿐이므로
//	nt family 인 경우라 하더라도 TRUE 를 리턴할 수 없으므로 버그를 만들수 있다.
//	
//		if ( ISNTFAMILY(GetWinVersion()  ) 
//		{
//			// for ntfamily code 
//			//
//		}
//
//	또는 
// 
//		if ( ISNTFAMILY(GetWinVersion() != 0 ) 
//		{
//			// for ntfamily code 
//			//
//		}
// 형태로 사용해야 한다. 
//
#define	ISNTFAMILY(x)	(x & MASK_WINNT)

/// Windows 95/98/SE/ME 패밀리인지를 확인하는 매크로
#define IS95FAMILY(x)	(x & MASK_WIN95)







// 실행중인 프로세스 리스트 정보 관리를 위한 구조체
//
typedef struct _RUNNING_PROC_INFO
{
	DWORD		ProcessID;  
	TCHAR		FullExeFile[MAX_PATH];
	TCHAR		ExeFile[MAX_PATH];	
} RUNNING_PROC_INFO, *PRUNNING_PROC_INFO;











/**	-----------------------------------------------------------------------
	\brief	GetLastError() 가 반환하는 에러코드를 읽을 수 있는 문자열로 리턴해 주는 함수
-------------------------------------------------------------------------*/
BOOL FormatErrorString(TCHAR *msg, DWORD msg_size, DWORD err);


/**	-----------------------------------------------------------------------
	\brief	gErrorMessage 전역변수에 에러메세지를 세팅하는 함수 
			(fmt 포맷 문자열 : 시스템 에러메세지 형식)			
-------------------------------------------------------------------------*/
LPCTSTR SetErrorString(BOOL IncludeGetLastError, TCHAR *fmt,...);


/**	-----------------------------------------------------------------------
	\brief	에러메세지 문자열을 담고 있는 포인터 반환	
-------------------------------------------------------------------------*/
LPCTSTR GetErrorString(void);


/**	-----------------------------------------------------------------------
	\brief	OS 시스템 정보를 가져오는 함수
			% MSDN 을 참고했음 예제 "Getting the System Version"
-------------------------------------------------------------------------*/
WORD	__getWinVersion( VOID );

/**	-----------------------------------------------------------------------
	\brief	__getWinVersion() 함수의 프론트 엔드  
-------------------------------------------------------------------------*/
WORD	GetWinVersion( VOID );


/** -----------------------------------------------------------------------
	\brief	문자열 복사를 위한 함수
-------------------------------------------------------------------------*/
void MoveString(LPTSTR Dest, LPCTSTR Src, SIZE_T SizeOfDest);

/**	-----------------------------------------------------------------------
	\brief	권한상승 함수
-------------------------------------------------------------------------*/
BOOL SetPrivilege(
				  HANDLE hToken,  // token handle
				  LPCTSTR Privilege,  // Privilege to enable/disable
				  BOOL bEnablePrivilege  // TRUE to enable. FALSE to disable
				  );




/**	-----------------------------------------------------------------------
	\brief	dwPID 프로세스의 풀 패스를 구하는 함수
-------------------------------------------------------------------------*/
BOOL GetImageFullPath(DWORD dwPID, LPTSTR szFullPath);


/**	-----------------------------------------------------------------------
	\brief	
-------------------------------------------------------------------------*/
INT EnumProcs(std::vector<RUNNING_PROC_INFO> *vectProc);








#endif	// _ENUM_PROC_H_