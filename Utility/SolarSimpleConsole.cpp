// SolarSimpleConsole.cpp: implementation of the SolarSimpleConsole class.
//
//////////////////////////////////////////////////////////////////////

#include "UtilityCommon.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <windows.h>
#include "SolarSimpleConsole.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



SolarSimpleConsole::SolarSimpleConsole()
	:	m_bUsingConsoleInput( FALSE ),
		m_hStdOut( NULL ),
		m_hStdIn( NULL ),
		m_OldSaveMode( 0 ),
		m_fn_KeyEventProc( NULL )
{

}

SolarSimpleConsole::~SolarSimpleConsole()
{

}
BOOL SolarSimpleConsole::Release()
{
	if( 0 != m_OldSaveMode )
	{
		if( !SetConsoleMode( m_hStdIn, m_OldSaveMode ) )
			Print("SolarSimpleConsole : SetConsoleMode operation is FALSE!"); 
	}
	return FreeConsole();
}

BOOL SolarSimpleConsole::Init( CONSOLE_DESC * pDesc )
{
	if( AllocConsole() )
	{
		m_hStdIn						= GetStdHandle( STD_INPUT_HANDLE );
		m_hStdOut						= GetStdHandle( STD_OUTPUT_HANDLE );
		//m_hStdErr						= GetStdHandle( STD_ERROR_HANDLE );

		m_fn_KeyEventProc				= pDesc->fn_KeyProc;
		m_bUsingConsoleInput			= pDesc->bUsingConsoleInput;

		if( m_bUsingConsoleInput && NULL != m_fn_KeyEventProc )
		{
			if( !GetConsoleMode( m_hStdIn, &m_OldSaveMode ) )
				Print("SolarSimpleConsole : GetConsoleMode operation is FALSE!"); 
			
			DWORD mode = ENABLE_WINDOW_INPUT;// | ENABLE_MOUSE_INPUT; 
			if( !SetConsoleMode( m_hStdIn, mode ) )
				Print("SolarSimpleConsole : SetConsoleMode operation is FALSE!"); 
		}
		
		SetConsoleParam( CONSOLE_LINE_NUM, pDesc->nLine );
		SetConsoleParam( CONSOLE_COLUMN_NUM, pDesc->nColumn );
		SetConsoleParam( CONSOLE_VISIBLE_LINE_NUM, pDesc->nVisibleLine );

		if( pDesc->bUsingRedirect )		_redirectToConsole();

		return TRUE;
	}
	
	return FALSE;
}

VOID SolarSimpleConsole::ProcessConsole()
{
	/*
	if( kbhit() )
		{
			char ch = getch();
			m_fn_KeyEventProc( ch );
	}*/
	
}
VOID SolarSimpleConsole::MessageLoop()
{
	if( !m_bUsingConsoleInput ) return ;

	DWORD nNumRead, i;
    INPUT_RECORD irInBuf[128];

	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
    { 
        if ( !ReadConsoleInput( m_hStdIn, irInBuf, 128, &nNumRead ) )
		{
			Print("SolarSimpleConsole : ReadConsoleInput operation is FALSE!");
			break;
		}
		
        for ( i = 0 ; i < nNumRead ; ++i ) 
        {
            switch( irInBuf[i].EventType )   
            { 
			case KEY_EVENT:
				m_fn_KeyEventProc( &irInBuf[i].Event.KeyEvent );
				break;	
			case MOUSE_EVENT:
			case WINDOW_BUFFER_SIZE_EVENT:
			case FOCUS_EVENT:
			case MENU_EVENT:
				break;
			default:
				Print("SolarSimpleConsole : Unknown Event is Occoured!");
				break;
            }
        }
    } 
}

VOID SolarSimpleConsole::_redirectToConsole()
{
	int                        consoleHandle;
	HANDLE                     stdHandle;
	FILE                       *fp;
    
	// redirect unbuffered STDOUT to the console
	consoleHandle = _open_osfhandle(reinterpret_cast<long>(m_hStdOut), _O_TEXT);
	fp = _fdopen(consoleHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, 0, _IONBF, 0);
    
	// redirect unbuffered STDIN to the console
	consoleHandle = _open_osfhandle(reinterpret_cast<long>(m_hStdIn), _O_TEXT);
	fp = _fdopen(consoleHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, 0, _IONBF, 0);
    
	// redirect unbuffered STDERR to the console
	stdHandle = GetStdHandle(STD_ERROR_HANDLE);
	consoleHandle = _open_osfhandle(reinterpret_cast<long>(stdHandle), _O_TEXT);
	fp = _fdopen(consoleHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, 0, _IONBF, 0);
    
	std::ios::sync_with_stdio();
}

VOID SolarSimpleConsole::SetConsoleParam( eConsoleParam param, int value )
{
	if( value <= 0 )	return;

	CONSOLE_SCREEN_BUFFER_INFO consoleScreenInfo;
	GetConsoleScreenBufferInfo( m_hStdOut, &consoleScreenInfo );

	switch( param )
	{
	case CONSOLE_LINE_NUM:
		{
			consoleScreenInfo.dwSize.Y = (SHORT)value;
			SetConsoleScreenBufferSize( m_hStdOut, consoleScreenInfo.dwSize);
		}
		break;
	case CONSOLE_COLUMN_NUM:
		{
			consoleScreenInfo.dwSize.X = (SHORT)value;
			SetConsoleScreenBufferSize( m_hStdOut, consoleScreenInfo.dwSize);
		}
		break;
	case CONSOLE_VISIBLE_LINE_NUM:
		{
			SMALL_RECT rect;
			rect.Bottom		= (SHORT)((value > 0) ? value : consoleScreenInfo.dwMaximumWindowSize.Y);
			rect.Right		= consoleScreenInfo.dwMaximumWindowSize.X;
			rect.Top		= rect.Left = 0;
			
			SetConsoleWindowInfo( m_hStdOut, TRUE, &rect );
		}
		break;
	default:
		Print("SolarSimpleConsole : Unknown eConsoleParam!");
		break;
	}
}

int SolarSimpleConsole::GetConsoleParam( eConsoleParam param )
{
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenInfo;
    GetConsoleScreenBufferInfo( m_hStdOut, &consoleScreenInfo );
	
    switch (param)
    {
	case CONSOLE_LINE_NUM:
		return consoleScreenInfo.dwSize.Y;
		break;
	case CONSOLE_COLUMN_NUM:
		return consoleScreenInfo.dwSize.X;
		break;
	case CONSOLE_VISIBLE_LINE_NUM:
		return consoleScreenInfo.dwMaximumWindowSize.Y;
		break;
	default:
		Print("SolarSimpleConsole : Unknown eConsoleParam!");
		break;
    }
	
    return 0;
}

VOID SolarSimpleConsole::Print( char * szMsg, ... )
{
	char strBuffer[256]; 

    va_list args; 
    va_start(args, szMsg); 
    _vsnprintf( strBuffer, 256, szMsg, args ); 
    va_end(args); 

	DWORD dwWrite; 
	WriteConsole( m_hStdOut, strBuffer, DWORD(strlen(strBuffer)), &dwWrite, NULL ); 
}
