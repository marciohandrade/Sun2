#ifndef __SOLAR_CONSOLE_H__
#define __SOLAR_CONSOLE_H__

#pragma once


#include "isolarconsole.h"
#include "ISolarConsole_Define.h"

class SolarLog;
class ListControl;
class SolarConsole;
struct SOLAR_CONSOLE_DESC;

class SolarConsole : public ISolarConsole
{
public:
	SolarConsole();
	virtual ~SolarConsole();

	/// Basic IUnknown Interface
	virtual HRESULT __stdcall			QueryInterface( const IID& iid, void ** ppv );
	virtual ULONG __stdcall				AddRef();
	virtual ULONG __stdcall				Release();

	/// external Interface
	virtual BOOL __stdcall				CreateConsole( const SOLAR_CONSOLE_DESC & desc, HWND * hWndOut );
	virtual VOID __stdcall				DestroyConsole();
	virtual VOID __stdcall				OutputDisplay( char * szString );
	virtual VOID __stdcall				OutputDisplayEx( char * szString, DWORD color );
	virtual VOID __stdcall				OutputDisplayArg( char * szString, ... );
	virtual VOID __stdcall				OutputFile( char * szString );
	virtual VOID __stdcall				MessageLoop();
	virtual BOOL __stdcall				MessageUpdate();

	/// internal public Interface

	VOID								InitInstance( HINSTANCE hInstance, LPLOGFONT pLogFont, char * pszConsoleTitle, int width, int height );
	LRESULT CALLBACK					WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	LRESULT CALLBACK					EditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	VOID								ExecuteMenu( WORD idx );
	VOID								CreateListControl( HINSTANCE hInstance, LPLOGFONT pLogFont, int x, int y, int width, int height );
	VOID								CreateEditControl( HINSTANCE hInstance, int x, int y, int width, int height );
	VOID								AttachMenu();
	BOOL								IsRefreshTimeOut();
	VOID								DrawTimeoutUpdate();
	VOID								CallBackCommand( char * pmsg );


	enum 
	{
		EDITBOX_HEIGHT					= 30,
		MAX_NAME_LENGTH					= 64,
		MAX_CUSTOM_ID_LIMIT				= 100,
		MAX_OUTPUTDISPLAYBUFFER_LENGTH	= 512,
	}; 
	struct MENU_INFO
	{
		char				pszMenuName[MAX_NAME_LENGTH];
		VOID				(*cbMenuCommand)();
	};

	/// internal private Interface
	MSG									m_msg;
	LONG								m_cRef;

	BOOL								m_bInited;

	WORD								m_wMenuNum;
	MENU_INFO *							m_pMenuInfo;
	DWORD								m_dwRefreshLastCheckTime;
	DWORD								m_dwRefreshRateDelayTime;
	DWORD								m_dwNoMoreRefreshDelayTime;
	DWORD								m_dwConsoleStyle;
	HANDLE								m_hDrawingEvent;
	fnCommandFunc						m_fnCommand;
	HWND								m_hWnd;
	HWND								m_hEdit;
	WNDPROC								m_fnOldCmdEditProc;
	ListControl *						m_pListControl;
	SolarLog *							m_pSolarLog;
	IUnknown *							m_pUnkFTM;
	
};

extern SolarConsole * g_pConsole;

#endif // __SOLAR_CONSOLE_H__