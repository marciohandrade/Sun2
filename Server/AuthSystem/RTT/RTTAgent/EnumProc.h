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







// �������� ���μ��� ����Ʈ ���� ������ ���� ����ü
//
typedef struct _RUNNING_PROC_INFO
{
	DWORD		ProcessID;  
	TCHAR		FullExeFile[MAX_PATH];
	TCHAR		ExeFile[MAX_PATH];	
} RUNNING_PROC_INFO, *PRUNNING_PROC_INFO;











/**	-----------------------------------------------------------------------
	\brief	GetLastError() �� ��ȯ�ϴ� �����ڵ带 ���� �� �ִ� ���ڿ��� ������ �ִ� �Լ�
-------------------------------------------------------------------------*/
BOOL FormatErrorString(TCHAR *msg, DWORD msg_size, DWORD err);


/**	-----------------------------------------------------------------------
	\brief	gErrorMessage ���������� �����޼����� �����ϴ� �Լ� 
			(fmt ���� ���ڿ� : �ý��� �����޼��� ����)			
-------------------------------------------------------------------------*/
LPCTSTR SetErrorString(BOOL IncludeGetLastError, TCHAR *fmt,...);


/**	-----------------------------------------------------------------------
	\brief	�����޼��� ���ڿ��� ��� �ִ� ������ ��ȯ	
-------------------------------------------------------------------------*/
LPCTSTR GetErrorString(void);


/**	-----------------------------------------------------------------------
	\brief	OS �ý��� ������ �������� �Լ�
			% MSDN �� �������� ���� "Getting the System Version"
-------------------------------------------------------------------------*/
WORD	__getWinVersion( VOID );

/**	-----------------------------------------------------------------------
	\brief	__getWinVersion() �Լ��� ����Ʈ ����  
-------------------------------------------------------------------------*/
WORD	GetWinVersion( VOID );


/** -----------------------------------------------------------------------
	\brief	���ڿ� ���縦 ���� �Լ�
-------------------------------------------------------------------------*/
void MoveString(LPTSTR Dest, LPCTSTR Src, SIZE_T SizeOfDest);

/**	-----------------------------------------------------------------------
	\brief	���ѻ�� �Լ�
-------------------------------------------------------------------------*/
BOOL SetPrivilege(
				  HANDLE hToken,  // token handle
				  LPCTSTR Privilege,  // Privilege to enable/disable
				  BOOL bEnablePrivilege  // TRUE to enable. FALSE to disable
				  );




/**	-----------------------------------------------------------------------
	\brief	dwPID ���μ����� Ǯ �н��� ���ϴ� �Լ�
-------------------------------------------------------------------------*/
BOOL GetImageFullPath(DWORD dwPID, LPTSTR szFullPath);


/**	-----------------------------------------------------------------------
	\brief	
-------------------------------------------------------------------------*/
INT EnumProcs(std::vector<RUNNING_PROC_INFO> *vectProc);








#endif	// _ENUM_PROC_H_