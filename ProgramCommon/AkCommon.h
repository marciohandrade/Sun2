/*-----------------------------------------------------------------------------
 * AkCommon.h
 *-----------------------------------------------------------------------------
 * COMMON 폴더의 소스를 이용하기 위한 공통 인클루드 파일 정의 
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2005, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2005, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/		

#ifndef _AK_COMMON_H
#define	_AK_COMMON_H
#if 0 // <- unused section, this file support armorkit module that don't use currently.
#include <windows.h>

#ifdef _VC60
	#include <stdio.h>
	#include "libConverter.h"
#else
	#include <tchar.h>
	#include <strsafe.h>
	#pragma warning(disable : 4995)		// deprecated 워닝 무시	 _stprintf() 등등 땜시롱... strsafe.h 에서 발생시킴.

#endif

#include <stdlib.h>
#include <assert.h>
#include <time.h>			// xasctime() 함수쓰려면 필요
#include <crtdbg.h>


// c:\program files\microsoft visual studio .net 2003\vc7\include\list(907): warning C4702: 접근할 수 없는 코드입니다.
// 워닝 제거 (릴리즈 모드 /W4 에서만 발생)
// 
#pragma warning(push)
#pragma warning(disable: 4702)
#include <list>
#include <vector>
#pragma	warning(pop)

using namespace std;

#ifndef IN
	#define IN
#endif
#ifndef OUT
	#define OUT
#endif
#ifndef IN_OUT
	#define IN_OUT
#endif

/* Define NULL pointer value */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

//
//	TODO
//	함수의 리턴값은 
//		INT 를 리턴하는 함수	- 성공시 0, 실패시 -1 또는 음수형 에러코드 리턴하도록 만들것
//		SIZE_T, DWORD 리턴함수	- 성공시 > 0, 실패시 0 리턴, 에러 메세지 세팅할것 (SetErrorString() 사용)
//
//	ret_true ---> 0 으로, ret_false ----> -1 로 바꾸고 코드 점검할것...
//	ret_notdefined 는 삭제 할 것
//
#ifndef ret_true
	#define ret_true	1		// 리턴코드 : 성공
#endif
#ifndef ret_false
	#define ret_false	0		// 함수가 실패하거나 결과값이 없음
#endif
#ifndef ret_notdefined
	#define ret_notdefined	0
#endif
#ifndef ret_error
	#define ret_error	-1		// 함수 에러발생
#endif


// 파일이름, 경로에 관한 상수 정의 
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
	#define _MAX_FORWARDER_NAME _MAX_FNAME * 2		// 그냥 넉넉하게 잡았다. -_-
#endif

#ifndef _OPCODE_DUMP_LENGTH
	#define _OPCODE_DUMP_LENGTH	32	/* API 의 구현부 덤프 길이  */
#endif

#ifndef _OPCODE_DUMP_BUF_LENGTH
	// 함수의 구현부 16 바이트 덤프
	// 			
	#define _OPCODE_DUMP_BUF_LENGTH	( (_OPCODE_DUMP_LENGTH * 2 ) + sizeof(TCHAR) )	/* API 의 구현부 덤프를 보관할 버퍼의 길이  */
#endif

#ifndef MAX_SIG_NAME
	#define MAX_SIG_NAME	255
#endif

/// 디폴트 로그 파일 이름
///
#define LOG_FILE_NAME	_TEXT("c:\\aklog.txt")			

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

#endif //0 -- unused
#endif	//_AK_COMMON_H
