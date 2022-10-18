#include "UtilityCommon.h"
#include "SolarAssert.h"
#include <string>
#pragma warning( disable:4786 )


const char *	_pszAssertCaption = "Solar Assert";
const char *	_pszIgnoreCaption = "Ask to Ignore Message";
const char *	_pszCoypyRight = "Copyright (c) SUN Studio & Webzen.Ltd..";
const char *	_pszIgnoreMessage = "이 지점의 AssertMessageBox를 다음에도 실행하시겠습니까?";
UINT			_uiAssertMessageBoxType = MB_SETFOREGROUND | MB_APPLMODAL | MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION;
UINT			_uiIgnoreMessageBoxType = MB_YESNO | MB_DEFBUTTON2;
char 			_pszVarDescription[512];
static int (APIENTRY * _pfnMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = NULL;
static HWND (APIENTRY * _pfnGetActiveWindow)(void) = NULL;
static HWND (APIENTRY * _pfnGetLastActivePopup)(HWND) = NULL;


void _copy2Clipboard( const char * msg )
{
	if( OpenClipboard( NULL ) )
	{
		HGLOBAL hMem = NULL;
		char * pMem = 0;
		hMem = GlobalAlloc( GHND | GMEM_DDESHARE, strlen( msg ) + 1 );
		if( pMem )
		{
			pMem = (char *)GlobalLock( hMem );
			strcpy( pMem, msg );
			GlobalUnlock( hMem );
			EmptyClipboard();
			SetClipboardData( CF_TEXT, hMem );
		}
		CloseClipboard();
	}
}

int _crtMessageBox( LPCSTR lpMsg, LPCSTR lpCaption, UINT uiType )
{

	HWND hwnd = NULL;
	if( NULL == _pfnMessageBoxA )
	{
		HANDLE hLib = LoadLibrary( "user32.dll" );
		if( NULL == hLib || NULL == ( _pfnMessageBoxA = (int (APIENTRY *)(HWND, LPCSTR, LPCSTR, UINT))GetProcAddress( (HMODULE)hLib, "MessageBoxA" ) ) )
			return 0;
		_pfnGetActiveWindow = (HWND (APIENTRY *)(void))GetProcAddress( (HMODULE)hLib, "GetActiveWindow" ) ;
		_pfnGetLastActivePopup = (HWND (APIENTRY *)(HWND))GetProcAddress( (HMODULE)hLib, "GetLastActivePopup" ) ;
	}
	if( _pfnGetActiveWindow )
		hwnd = (*_pfnGetActiveWindow)();
	if( hwnd != NULL && _pfnGetLastActivePopup )
		hwnd = (*_pfnGetLastActivePopup)(hwnd);

	return (*_pfnMessageBoxA)( hwnd, lpMsg, lpCaption, uiType);
}
int _ignoreMessageBox()
{
	return _crtMessageBox( _pszIgnoreMessage, _pszIgnoreMessage, _uiIgnoreMessageBoxType );
}
int _solarAssertBox( const char * pszDesc, int line, char * file, char * date, char * time )
{

	char tmpLineNum[16];
	std::string strMsgBuf;
	strMsgBuf.append( "File : " );
	strMsgBuf.append( file );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( "Line # : " );
	strMsgBuf.append( _itoa(line, tmpLineNum, 10) );
	strMsgBuf.append( " line\r\n" );
	strMsgBuf.append( "Description : " );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( "DateTime : " );
	strMsgBuf.append( date );
	strMsgBuf.append( " " );
	strMsgBuf.append( time );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( "-----------------------------------------------------------------------" );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( pszDesc );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( "\r\n" );

	_copy2Clipboard( strMsgBuf.c_str() );

	strMsgBuf.append( "-----------------------------------------------------------------------" );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( "[중단(A)]	를 누르면 프로그램을 중단합니다.\r\n" );
	strMsgBuf.append( "[다시시도(R)]를 누르면 Assert 지점으로 이동합니다.\r\n" );
	strMsgBuf.append( "[무시(I)]	를 누르면 프로그램을 계속 진행합니다.\r\n" );
	strMsgBuf.append( "-----------------------------------------------------------------------" );
	strMsgBuf.append( "\r\n" );
	strMsgBuf.append( _pszCoypyRight );
	strMsgBuf.append( "\r\n" );

	int rt = _crtMessageBox( strMsgBuf.c_str(), _pszAssertCaption, _uiAssertMessageBoxType );
	if( rt == IDABORT )
	{
		exit(9);// return 1;
	}
	if( rt == IDRETRY )
	{
		return 2;
	}
	if( rt == IDIGNORE )
	{
		return 3;
	}
	return 0;
}


char * VSTR( char * s, ...)
{
	va_list arg;
	va_start( arg, s );
	vsprintf( _pszVarDescription, s, arg );
	va_end( arg );
	return _pszVarDescription;
}