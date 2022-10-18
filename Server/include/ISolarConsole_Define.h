#ifndef __ISOLAR_CONSOLE_DEFINE_H__
#define __ISOLAR_CONSOLE_DEFINE_H__

#pragma once


enum eSOLAR_CONSOLE_STYLE
{
	SCS_ENABLE_WRITETOFILE				= 0x00000001,				/* 로그 파일 기록 */
};


#define LINE_COLOR_GREEN		RGB(0,255,0)
#define LINE_COLOR_ORANGE		RGB(212, 208, 200)
#define LINE_COLOR_PURPLE		RGB(128, 128, 200)
#define LINE_COLOR_WHITE		RGB(255, 255, 255)


typedef VOID (*fnCommandFunc)( char * pszCommand );

struct SOLAR_MENU_INFO
{
	char *				pszMenuName;
	VOID				(*cbMenuCommand)();
};

struct SOLAR_CONSOLE_DESC
{
	char *				pszConsoleTitle;
	char *				pszLogFileName;
	DWORD				dwConsoleStyle;
	DWORD				dwRefreshRateDelayTime;					///< 화면 리프레쉬 지연 시간 (ms)
	DWORD				dwNoMoreRefreshDelayTime;				///< 더이상 화면 리프레쉬를 하지 않아도 되는 시간 (ms) : 키보드, 마우스 이동 없을 때
	DWORD				dwBufferingLineNum;
	WORD				wConsoleWidthSize;
	WORD				wConsoleHeightSize;
	WORD				wMenuNum;
	SOLAR_MENU_INFO *	pMemnuInfo;	
	LOGFONT *			pLogFont;
	fnCommandFunc		fnCommand;
};

#endif // __ISOLAR_CONSOLE_DEFINE_H__