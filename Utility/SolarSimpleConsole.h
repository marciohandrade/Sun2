// Console.h: interface for the Console class.
// taiyo
//////////////////////////////////////////////////////////////////////

#ifndef __SOLAR_SIMPLE_CONSOLE_H__
#define __SOLAR_SIMPLE_CONSOLE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////////////////////////////////
// Singleton으로 사용하던지 전역 변수를 사용하던지 사용하는 측에서 결정하라.!
////////////////////////////////////////////////////////////////////////////////////////


typedef VOID (*ConsoleKeyEventProc)( KEY_EVENT_RECORD * pEv );

typedef struct tagCONSOLE_DESC
{
	BOOL					bUsingConsoleInput;
	BOOL					bUsingRedirect;
	int						nLine;
	int						nColumn;
	int						nVisibleLine;
	ConsoleKeyEventProc		fn_KeyProc;

}CONSOLE_DESC;

class SolarSimpleConsole
{
	enum eConsoleParam { CONSOLE_LINE_NUM, CONSOLE_COLUMN_NUM, CONSOLE_VISIBLE_LINE_NUM };
public:
	SolarSimpleConsole();
	~SolarSimpleConsole();
	
	BOOL					Init( CONSOLE_DESC * pDesc );
	BOOL					Release();

	VOID					MessageLoop();
	VOID					ProcessConsole();

	VOID					SetConsoleParam( eConsoleParam param, int value );
	int						GetConsoleParam( eConsoleParam param );
	VOID					Print( char * szMsg, ... );

private:
	
	VOID					_redirectToConsole();
	BOOL					m_bUsingConsoleInput;
	HANDLE					m_hStdOut;
	HANDLE					m_hStdIn;
	DWORD					m_OldSaveMode;
	ConsoleKeyEventProc		m_fn_KeyEventProc;
};

#endif // __SOLAR_SIMPLE_CONSOLE_H__
