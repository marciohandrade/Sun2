/*-----------------------------------------------------------------------------
 * AkCommon.h
 *-----------------------------------------------------------------------------
 * COMMON ������ �ҽ��� �̿��ϱ� ���� ���� ��Ŭ��� ���� ���� 
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
	#pragma warning(disable : 4995)		// deprecated ���� ����	 _stprintf() ��� ���÷�... strsafe.h ���� �߻���Ŵ.

#endif

#include <stdlib.h>
#include <assert.h>
#include <time.h>			// xasctime() �Լ������� �ʿ�
#include <crtdbg.h>


// c:\program files\microsoft visual studio .net 2003\vc7\include\list(907): warning C4702: ������ �� ���� �ڵ��Դϴ�.
// ���� ���� (������ ��� /W4 ������ �߻�)
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
//	�Լ��� ���ϰ��� 
//		INT �� �����ϴ� �Լ�	- ������ 0, ���н� -1 �Ǵ� ������ �����ڵ� �����ϵ��� �����
//		SIZE_T, DWORD �����Լ�	- ������ > 0, ���н� 0 ����, ���� �޼��� �����Ұ� (SetErrorString() ���)
//
//	ret_true ---> 0 ����, ret_false ----> -1 �� �ٲٰ� �ڵ� �����Ұ�...
//	ret_notdefined �� ���� �� ��
//
#ifndef ret_true
	#define ret_true	1		// �����ڵ� : ����
#endif
#ifndef ret_false
	#define ret_false	0		// �Լ��� �����ϰų� ������� ����
#endif
#ifndef ret_notdefined
	#define ret_notdefined	0
#endif
#ifndef ret_error
	#define ret_error	-1		// �Լ� �����߻�
#endif


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
	#define _OPCODE_DUMP_LENGTH	32	/* API �� ������ ���� ����  */
#endif

#ifndef _OPCODE_DUMP_BUF_LENGTH
	// �Լ��� ������ 16 ����Ʈ ����
	// 			
	#define _OPCODE_DUMP_BUF_LENGTH	( (_OPCODE_DUMP_LENGTH * 2 ) + sizeof(TCHAR) )	/* API �� ������ ������ ������ ������ ����  */
#endif

#ifndef MAX_SIG_NAME
	#define MAX_SIG_NAME	255
#endif

/// ����Ʈ �α� ���� �̸�
///
#define LOG_FILE_NAME	_TEXT("c:\\aklog.txt")			

/// Window �÷����ڵ�

#define OSTYPE_WIN_95		0x0400
#define OSTYPE_WIN_98		0x0410
#define OSTYPE_WIN_SE		0x0410
#define OSTYPE_WIN_ME		0x0490

#define	OSTYPE_WIN_NT		0x0400

#define OSTYPE_WIN_2000		0x0500
#define OSTYPE_WIN_XP		0x0501
#define OSTYPE_WIN_2003		0x0502
#define OSTYPE_WIN_LONGHORN	0x0600


/// ���ǻ� ������
// windows 95 family �迭�� 0x000F ����ũ����

#define		_WIN95			0x0001
#define		_WIN98			0x0002
#define		_WINSE			0x0004
#define		_WINME			0x0008

// windows nt family �迭�� 0x00F0 ����ũ����

#define		_WINNT			0x0010
#define		_WIN2000		0x0020
#define		_WINXP			0x0040
#define		_WIN2003		0x0080
#define		_WINLONGHORN	0x00c0

#define		MASK_WIN95		0x000F
#define		MASK_WINNT		0x00F0

/// Windows NT/2000/XP/2003/LONGHORN �йи������� Ȯ���ϴ� ��ũ��
//
//  < 2006.03.31 ���ȯ >
//  ISNTFAMILY(GetWinVersion()) ���·� ����ϸ� �Ǵµ� 
//	����� 
//		if ( ISNTFAMILY(GetWinVersion()) == TRUE ) 
//		{
//			// for ntfamily code 
//			//
//		}
//	���·� ����ϸ� �ȵȴ�. 
//	TRUE �� windef.h (�Ǵ� �ٸ� ����..) �ܼ��� 1 �� ���ǵǾ����� ���̹Ƿ�
//	nt family �� ���� �ϴ��� TRUE �� ������ �� �����Ƿ� ���׸� ����� �ִ�.
//	
//		if ( ISNTFAMILY(GetWinVersion()  ) 
//		{
//			// for ntfamily code 
//			//
//		}
//
//	�Ǵ� 
// 
//		if ( ISNTFAMILY(GetWinVersion() != 0 ) 
//		{
//			// for ntfamily code 
//			//
//		}
// ���·� ����ؾ� �Ѵ�. 
//
#define	ISNTFAMILY(x)	(x & MASK_WINNT)

/// Windows 95/98/SE/ME �йи������� Ȯ���ϴ� ��ũ��
#define IS95FAMILY(x)	(x & MASK_WIN95)

#endif //0 -- unused
#endif	//_AK_COMMON_H
