/*--
	Copyright (C) 2007-2009 Wellbia.com Co., Ltd.

	Module Name:
		xtypes.h

	Abstract:
		this file contains types for XIGNCODE system.

	Author:
		2009-06-22 YoungJin Shin <codewiz@wellbia.com>

	Environment:
		any

	Library:
		none

--*/

#ifndef XIGNCODE_TYPE_SYSTEM
#define XIGNCODE_TYPE_SYSTEM

#if defined(__GNUC__) || defined(__UNIX__)
#define UNIX
#endif

typedef char xch;
typedef char *xpch;
typedef const char *xpcch;

typedef unsigned char xuch;
typedef unsigned char *xpuch;
typedef const unsigned char *xpcuch;

typedef short xshort;
typedef short *xpshort;
typedef const short *xpcshort;

typedef unsigned short xushort;
typedef unsigned short *xpushort;
typedef const unsigned short *xpcushort;

typedef long xlong;
typedef long *xplong;
typedef const long *xpclong;

typedef unsigned long xulong;
typedef unsigned long *xpulong;
typedef const unsigned long *xpculong;

typedef int xint;
typedef int *xpint;
typedef const int *xpcint;

typedef unsigned int xuint;
typedef unsigned int *xpuint;
typedef const unsigned int *xpcuint;

typedef __int64 xint64;
typedef __int64 *xpint64;
typedef const __int64 *xpcint64;

typedef unsigned __int64 xuint64;
typedef unsigned __int64 *xpuint64;
typedef unsigned const __int64 *xpcuint64;

#ifdef UNIX

#define XCALL __attribute__((stdcall))
#define __w64
typedef wchar_t xwch;

#else

#define XCALL __stdcall

#ifdef _NATIVE_WCHAR_T_DEFINED
typedef wchar_t xwch;
#else
typedef unsigned short xwch;
#endif

#if _MSC_VER < 1300
#define __w64
#endif

#endif

typedef char *xstr;
typedef const char *xcstr;
typedef xwch *xwstr;
typedef const xwch *xcwstr;

typedef void xvoid;
typedef void *xpvoid;
typedef const void *xpcvoid;

typedef unsigned long xstatus;
typedef xpvoid xhandle;
typedef xhandle *xphandle;

typedef int xbool;

#ifdef _M_X64
typedef __w64 unsigned __int64 xsize;
typedef __w64 unsigned __int64 *xpsize;
typedef __w64 __int64 xlptr;
typedef __w64 unsigned __int64 xulptr;
#else
typedef __w64 unsigned int xsize;
typedef __w64 unsigned int *xpsize;
typedef __w64 long xlptr;
typedef __w64 unsigned long xulptr;

#endif

/* Define NULL pointer value */
#ifndef X_NULL
#ifdef __cplusplus
#define X_NULL    0
#else
#define X_NULL    ((void *)0)
#endif
#endif

#define X_FALSE 0
#define X_TRUE 1

#define X_EOSA '\0'
#define X_EOSW L'\0'

#ifdef UNIX
#define X_DIR_MARKA '/'
#define X_DIR_MARKW L'/'

#define X_DIR_MARKSA '/'
#define X_DIR_MARKDA "/"
#define X_DIR_MARKSW L'/'
#define X_DIR_MARKDW L"/"

#else
#define X_DIR_MARKA '\\'
#define X_DIR_MARKW L'\\'

#define X_DIR_MARKSA '\\'
#define X_DIR_MARKDA "\\"
#define X_DIR_MARKSW L'\\'
#define X_DIR_MARKDW L"\\"

#endif

#ifdef _UNICODE
#define X_EOS X_EOSW
#define X_DIR_MARK X_DIR_MARKW
#define xtstr xwstr
#define xctstr xcwstr
#define xtch xwch
#else
#define X_EOS X_EOSA
#define X_DIR_MARK X_DIR_MARKA
#define xtstr xstr
#define xctstr xcstr
#define xtch xch
#endif

#ifdef UNIX
#define X_MAX_PATH 260
#else
#define X_MAX_PATH MAX_PATH
#endif

typedef int (XCALL *xproc)();



#define XGetPtr32(base, offset)		((PVOID)((ULONG) (base) + (ULONG) (offset)))
#define XGetPtr64(base, offset)		((PVOID)((ULONGLONG) (base) + (ULONGLONG) (offset)))
#define XGetPtr(base, offset)		((PVOID)((ULONG_PTR) (base) + (ULONG_PTR) (offset)))

#define XGetOffset32(dst, src)		((ULONG)((ULONG) (dst) - (ULONG) (src)))
#define XGetOffset64(dst, src)		((ULONGLONG)((ULONGLONG) (dst) - (ULONGLONG) (src)))
#define XGetOffset(dst, src)		((ULONG_PTR)((ULONG_PTR) (dst) - (ULONG_PTR) (src)))

#define XArraySize(arr)				(sizeof((arr))/sizeof(*(arr)))
#define XFieldSize(strt, field)		(sizeof(((strt *) X_NULL)->field))
#define XALIGN(v, b) (((v) + (b) - 1) & ~((b) - 1))
#define XROUNGUP(a, b) ((((a) + (b) - 1) / (b)) * (b))

//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//

#define XSTATUS_SEV_ERROR		3
#define XSTATUS_SEV_WARN		2
#define XSTATUS_SEV_INFO		1
#define XSTATUS_SEV_SUCCESS		0

#define MAKE_XSTATUS(sev, fac, code) \
	((xstatus) (((unsigned long)(sev)<<31) \
				| 0x20000000 \
				| ((unsigned long)(fac)<<16) \
				| ((unsigned long)(code))) )

#define MAKE_E_XSTATUS(fac, code) \
	((xstatus) ((unsigned long) 0xE0000000 \
				| ((unsigned long)(fac)<<16) \
				| ((unsigned long)(code))) )

#define X_OK 0
#define X_SUCCESS(s) (((s) & 0x80000000) == 0)

#define XM_BASE 1
#define X_E_UNSUCCESSFUL			MAKE_E_XSTATUS(XM_BASE, 1)
#define X_E_INVALID_PARAMETER		MAKE_E_XSTATUS(XM_BASE, 2)
#define X_E_INSUFFICIENT_RESOURCES	MAKE_E_XSTATUS(XM_BASE, 3)
#define X_E_CANT_OPEN_FILE			MAKE_E_XSTATUS(XM_BASE, 4)
#define X_E_CANT_READ_FILE			MAKE_E_XSTATUS(XM_BASE, 5)
#define X_E_CANT_CREATE_FILE		MAKE_E_XSTATUS(XM_BASE, 6)
#define X_E_LOCK_FAIL				MAKE_E_XSTATUS(XM_BASE, 7)
#define X_E_BUFFER_OVERFLOW			MAKE_E_XSTATUS(XM_BASE, 8)
#define X_E_INVALID_OPERATION		MAKE_E_XSTATUS(XM_BASE, 9)
#define X_E_UNICODE_ERROR			MAKE_E_XSTATUS(XM_BASE, 10)
#endif
