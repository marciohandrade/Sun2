#include "StdAfx.h"
#include ".\solarconsole.h"
#include "ISolarConsole_Guid.h"
#include "ListControl.h"
#include "resource.h"
#include <SolarLog.h>
#include <stdio.h>

/// m_fnOldCmdEditProc = (WNDPROC)SetWindowLong( m_hEdit, GWL_WNDPROC, (LONG)NewCmdEditProc );의 캐스트 워닝 무시!!
#pragma warning (disable:4311)
#pragma warning (disable:4312)

//===========================================================================================================
// Window Callback Procedure
//===========================================================================================================
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK	NewCmdEditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

extern LONG g_cComponents;
extern HINSTANCE g_hInstance;
SolarConsole * g_pConsole = NULL;

SolarConsole::SolarConsole()
	:	m_cRef ( 1 ),
		m_bInited ( FALSE ),
		m_wMenuNum ( 0 ),
		m_pMenuInfo ( NULL ),
		m_dwRefreshLastCheckTime ( 0 ),
		m_dwRefreshRateDelayTime ( 0 ),
		m_dwNoMoreRefreshDelayTime ( 0 ),
		m_hDrawingEvent ( NULL ),
		m_fnCommand ( NULL ),
		m_hWnd ( NULL ),
		m_hEdit ( NULL ),
		m_fnOldCmdEditProc ( 0 ),
		m_pListControl ( NULL ),
		m_pSolarLog ( NULL ),
		m_pUnkFTM ( NULL )
{
	HRESULT hr = CoCreateFreeThreadedMarshaler( this, &m_pUnkFTM );
	ASSERT( !FAILED( hr ) );
}

SolarConsole::~SolarConsole()
{
	m_pUnkFTM->Release();
}

//===========================================================================================================
// COM 객체를 사용하기 위해 IUnknown 기본적인 가상 함수 구현
//===========================================================================================================
HRESULT __stdcall SolarConsole::QueryInterface( const IID& iid, void ** ppv )
{
	if( iid == IID_IUnknown )				*ppv = static_cast<IUnknown *>(this);
	else if( iid == IID_SOLARConsole )		*ppv = static_cast<ISolarConsole *>(this);
	else if( iid == IID_IMarshal )
		return m_pUnkFTM->QueryInterface( iid, ppv );
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	static_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall SolarConsole::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall SolarConsole::Release()
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		--g_cComponents;
		return 0;
	}
	return m_cRef;
}
//===========================================================================================================


VOID SolarConsole::InitInstance( HINSTANCE hInstance, LPLOGFONT pLogFont, char * pszConsoleTitle, int width, int height )
{
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_TCONSOLEICON);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName	= (LPCSTR)IDR_CONSOLE_MENU;
	wcex.lpszClassName	= pszConsoleTitle;
	wcex.hIconSm		= NULL;
	RegisterClassEx( &wcex );

	int cx	= width + GetSystemMetrics(SM_CXSIZEFRAME) * 2;
	int cy	= height + GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYMENU);

	m_hWnd	= CreateWindow( pszConsoleTitle, pszConsoleTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, cx, cy, NULL, NULL, hInstance, NULL );
	
	
	CreateListControl( hInstance, pLogFont, 10, 10, (width - 20), (height - 70) );

	CreateEditControl( hInstance, 10, height - 55, width - 20, EDITBOX_HEIGHT );
	ShowWindow( m_hWnd, TRUE );
	UpdateWindow( m_hWnd );
}

VOID SolarConsole::CreateListControl( HINSTANCE hInstance, LPLOGFONT pLogFont, int x, int y, int width, int height )
{
	m_pListControl = new ListControl;
	m_pListControl->CreateList( hInstance, m_hWnd, pLogFont, x, y, width, height ) ;
}

VOID SolarConsole::CreateEditControl( HINSTANCE hInstance, int x, int y, int width, int height )
{
	m_hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, x, y, width, height, m_hWnd, 0, hInstance, NULL);
	//m_hEdit	= CreateWindow( "edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER , x, y, width, height, m_hWnd, 0, hInstance, NULL );
	m_fnOldCmdEditProc = (WNDPROC)SetWindowLong( m_hEdit, GWL_WNDPROC, (LONG)NewCmdEditProc );
	SetFocus( m_hEdit );
}

VOID SolarConsole::AttachMenu()
{
	int StartMenuID = CUSTOM_MENU_ID;
	HMENU MenuBar = GetSubMenu( GetMenu( m_hWnd ), 1 );
	for( int i = 0 ; i < m_wMenuNum ; ++i )
	{
		InsertMenu( MenuBar, i, MF_BYPOSITION, StartMenuID, m_pMenuInfo[i].pszMenuName );
		++StartMenuID;
	}
}

VOID __stdcall SolarConsole::DestroyConsole()
{
	DestroyWindow( m_hWnd );

	if( m_hDrawingEvent ) 
	{
		KillTimer( m_hWnd, 1 );
		m_hDrawingEvent = NULL;
	}

	if( m_pListControl )
	{
		m_pListControl->DestroyList();
		delete m_pListControl;
		m_pListControl = NULL;
	}

	if( m_pMenuInfo )
	{
		delete [] m_pMenuInfo;
		m_pMenuInfo = NULL;
	}

	if( m_pSolarLog )
	{
		delete m_pSolarLog;
		m_pSolarLog = NULL;
	}
}
BOOL __stdcall SolarConsole::CreateConsole( const SOLAR_CONSOLE_DESC & desc, HWND * hWndOut )
{
	m_dwConsoleStyle			= desc.dwConsoleStyle;
	m_dwRefreshRateDelayTime	= desc.dwRefreshRateDelayTime;
	m_dwNoMoreRefreshDelayTime	= desc.dwNoMoreRefreshDelayTime;
	m_wMenuNum					= desc.wMenuNum;
	m_fnCommand					= desc.fnCommand;
	m_pMenuInfo = new MENU_INFO[ m_wMenuNum ];
	for( int i = 0 ; i < m_wMenuNum ; ++i )
	{
		strncpy(m_pMenuInfo[i].pszMenuName, desc.pMemnuInfo[i].pszMenuName, MAX_NAME_LENGTH );
		m_pMenuInfo[i].cbMenuCommand = desc.pMemnuInfo[i].cbMenuCommand;
	}

	m_pSolarLog					= new SolarLog;
	if( !m_pSolarLog->Create( "log_file", desc.pszLogFileName ) )
		return FALSE;

	InitInstance( g_hInstance, desc.pLogFont, desc.pszConsoleTitle, desc.wConsoleWidthSize, desc.wConsoleHeightSize );

	m_hDrawingEvent = (HANDLE)SetTimer( m_hWnd, 1, m_dwRefreshRateDelayTime, NULL );

	AttachMenu();

	m_bInited = TRUE;

	// out?
	hWndOut = NULL;

	return TRUE;
}

VOID __stdcall SolarConsole::OutputDisplay( char * szString )
{
	m_pListControl->AddString( szString );
	if( m_dwConsoleStyle & SCS_ENABLE_WRITETOFILE )
		m_pSolarLog->LOG( szString );
}

VOID __stdcall SolarConsole::OutputDisplayEx( char * szString, DWORD color )
{
	m_pListControl->AddString( szString, color );
	if( m_dwConsoleStyle & SCS_ENABLE_WRITETOFILE )
		m_pSolarLog->LOG( szString );
}

VOID __stdcall SolarConsole::OutputDisplayArg( char * szString, ... )
{
	char		szBuffer[MAX_OUTPUTDISPLAYBUFFER_LENGTH] = "";
	va_list		pArguments;

	va_start( pArguments, szString );
#ifdef	_DEBUG
	int rt = vsprintf( szBuffer, szString, pArguments );
	ASSERT( rt >= 0 );
#else
	vsprintf( szBuffer, szString, pArguments );
#endif 
	va_end( pArguments );

	OutputDisplay( szBuffer );
}

VOID __stdcall SolarConsole::OutputFile( char * szString )
{
	m_pSolarLog->LOG( szString );
}


VOID __stdcall SolarConsole::MessageLoop()
{
	while( GetMessage(&m_msg, NULL, 0, 0) )
	{
		if( m_msg.message == WM_QUIT )
			return;
		TranslateMessage( &m_msg );
		DispatchMessage( &m_msg );
	}
}
BOOL __stdcall SolarConsole::MessageUpdate()
{
	while( PeekMessage( &m_msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if( m_msg.message == WM_QUIT )	
			return FALSE;
		TranslateMessage( &m_msg );
		DispatchMessage( &m_msg );
	}

	return TRUE;
}
VOID SolarConsole::CallBackCommand( char * pmsg )
{
	if( m_fnCommand )	m_fnCommand( pmsg );
}
VOID SolarConsole::ExecuteMenu( WORD idx )
{
	if( idx < m_wMenuNum )
		m_pMenuInfo[idx].cbMenuCommand();
}


BOOL SolarConsole::IsRefreshTimeOut()
{
	if( m_dwRefreshLastCheckTime != 0 && 
		( GetTickCount() - m_dwRefreshLastCheckTime > m_dwNoMoreRefreshDelayTime ) )
		return FALSE;
	return TRUE;
}


VOID SolarConsole::DrawTimeoutUpdate()
{
	m_dwRefreshLastCheckTime = GetTickCount();
}

LRESULT CALLBACK SolarConsole::WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	m_pListControl->MessageProc( message, wParam, lParam );

	switch ( message ) 
	{
	case WM_COMMAND:
		{
			WORD wmId    = LOWORD(wParam); 
			WORD wmEvent = HIWORD(wParam); 

			__UNUSED(wmEvent);

			if( CUSTOM_MENU_ID <= wmId && wmId <= CUSTOM_MENU_ID + MAX_CUSTOM_ID_LIMIT )
			{
				ExecuteMenu( wmId - CUSTOM_MENU_ID );
				return 0;
			}

			switch (wmId)
			{
			case ID_FILE_FLUSHTOFILE:
					//ForceFlushFile();
				break;

			case ID_CONSOLE_ABOUTSOLARCONSOLE:
				DialogBox( g_hInstance, (LPCTSTR)IDD_ABOUT_DLG, hWnd, (DLGPROC)About );
				break;
			case ID_FILE_EXIT:
				SendMessage( hWnd, WM_CLOSE, 0, 0 );
				break;
			default:
				return DefWindowProc( hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_SIZE:
		{
			if( m_bInited )
			{
				RECT crt;
				GetClientRect( hWnd, &crt);
				int Width = crt.right - crt.left;
				int Height = crt.bottom - crt.top;

				MoveWindow( m_hEdit, 10, (Height - 55), (Width - 20), EDITBOX_HEIGHT, TRUE );	
				m_pListControl->OnSize( (Width - 20), (Height - 70) );
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint( hWnd, &ps);
			m_pListControl->InvalidateUpdate();
			EndPaint( hWnd, &ps);
		}
		break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_KEYDOWN:
	case WM_MOUSEMOVE:
		DrawTimeoutUpdate();
		return DefWindowProc( hWnd, message, wParam, lParam );
	case WM_TIMER:
		if( IsRefreshTimeOut() )	m_pListControl->InvalidateUpdate();
		break;
	case WM_CLOSE:
		{
			if(IDYES == MessageBox( hWnd, "Do you want to close this console?", "Warning!", MB_YESNO))
				return DefWindowProc( hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:	
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK SolarConsole::EditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message ) 
	{
	case WM_CHAR: 
		switch( wParam ) 
		{ 
		case 0x0D:
			{
				DrawTimeoutUpdate();
				static char buff[256];
				SendMessage( hWnd, WM_GETTEXT, (WPARAM)128, (LPARAM)buff );
				if( strcmp(buff, "") != 0 ) 
				{
					CallBackCommand( buff );
					OutputDisplay( buff );	
					SendMessage( hWnd, WM_SETTEXT, 0,0 );
				}
			}
			break;
		}
		break;
	}
	return CallWindowProc( (WNDPROC)m_fnOldCmdEditProc, hWnd, message, wParam, lParam );
}

	


LRESULT CALLBACK NewCmdEditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return g_pConsole->EditProc( hWnd, message, wParam, lParam );
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return g_pConsole->WindowProc( hWnd, message, wParam, lParam );
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	__UNUSED(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog( hDlg, LOWORD(wParam) );
			return TRUE;
		}
		break;
	}
	return FALSE;
}