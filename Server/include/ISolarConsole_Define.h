#ifndef __ISOLAR_CONSOLE_DEFINE_H__
#define __ISOLAR_CONSOLE_DEFINE_H__

#pragma once


enum eSOLAR_CONSOLE_STYLE
{
	SCS_ENABLE_WRITETOFILE				= 0x00000001,				/* �α� ���� ��� */
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
	DWORD				dwRefreshRateDelayTime;					///< ȭ�� �������� ���� �ð� (ms)
	DWORD				dwNoMoreRefreshDelayTime;				///< ���̻� ȭ�� ���������� ���� �ʾƵ� �Ǵ� �ð� (ms) : Ű����, ���콺 �̵� ���� ��
	DWORD				dwBufferingLineNum;
	WORD				wConsoleWidthSize;
	WORD				wConsoleHeightSize;
	WORD				wMenuNum;
	SOLAR_MENU_INFO *	pMemnuInfo;	
	LOGFONT *			pLogFont;
	fnCommandFunc		fnCommand;
};

#endif // __ISOLAR_CONSOLE_DEFINE_H__