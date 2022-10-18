#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

#include <eXtreme/util/Console.h>

using namespace eXtreme;

enum COLOR_SET
{
	//Foreground color
	F_BLACK		= 0,
	F_RED		= FOREGROUND_RED,
	F_GREEN		= FOREGROUND_GREEN,
	F_YELLOW	= FOREGROUND_RED   | FOREGROUND_GREEN,
	F_BLUE	 	= FOREGROUND_BLUE,
	F_MAGENTA	= FOREGROUND_RED   | FOREGROUND_BLUE,
	F_CYAN		= FOREGROUND_GREEN | FOREGROUND_BLUE,
	F_WHITE		= FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,

	//Background color
	B_BLACK		= 0,
	B_RED		= BACKGROUND_RED,
	B_GREEN		= BACKGROUND_GREEN,
	B_YELLOW	= BACKGROUND_RED   | BACKGROUND_GREEN,
	B_BLUE		= BACKGROUND_BLUE,
	B_MAGENTA	= BACKGROUND_RED   | BACKGROUND_BLUE,
	B_CYAN		= BACKGROUND_GREEN | BACKGROUND_BLUE, 
	B_WHITE		= BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE,
};

Console::Console( LPCTSTR title )
{
	::AllocConsole();

	m_handleInput	= ::GetStdHandle( STD_INPUT_HANDLE );
	m_handleOutput	= ::GetStdHandle( STD_OUTPUT_HANDLE );

	NormalString[0] = 0;
	m_nsOffset		= 0;
	m_tabSize		= 4;

	if( title != 0 )
		SetTitle( title );
}

Console::~Console()
{
	if( m_handleOutput != INVALID_HANDLE_VALUE )
		::CloseHandle( m_handleOutput );
}

void Console::SetTitle( LPCTSTR title )
{
	::SetConsoleTitle( title );
}

void Console::EnableInput( bool flag )
{
	if( flag == true )
		::SetConsoleMode( m_handleInput, ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT );
	else
		::SetConsoleMode( m_handleInput, ENABLE_PROCESSED_OUTPUT );
}

void Console::WriteLine( LPTSTR format, ... )
{
	TCHAR	buffer[4092] = {0, };
	va_list	args;

	va_start( args, format );
	vsprintf( buffer, format, args );
	va_end( args );

	NormalString[0]	= 0;
	m_nsOffset		= 0;

	{
		WORD	attr = 0;

		SetForeAttr( &attr, 'w' );

		WORD len = ( WORD )strlen( buffer );
		for(WORD i = 0 ; i < len ; i++ )
		{
			if( buffer[i] == '\r' )
				continue;

			if( buffer[i] == '\n' )
			{
				NewLine();

				continue;
			}
			if( buffer[i] == '\t' )
			{
				Tab();
				i++;
				continue;
			}

			if( i + 1 < len && buffer[i] == '#' && buffer[i+1] =='#' )
			{
				i += 1;
			}
			else if( i + 2 < len && buffer[i] == '#' )
			{
				attr	= 0;
				SetForeAttr( &attr, buffer[i + 1] );
				SetBackAttr( &attr, buffer[i + 2] );
				i		+= 2;

				continue;
			}

			{
				char	tmp[3] = { 0, 0, 0 };

				tmp[0] = buffer[i];
				if( IsDBCSLeadByte( tmp[0] ) == TRUE )
				{
					tmp[1] = buffer[i + 1];
					++i;
				}

				OutputString( attr, tmp );
			}
		}
	}
}

void Console::WriteLineNormal( LPTSTR format, ... )
{
	TCHAR	buffer[4092] = {0, };
	va_list	args;
	
	va_start( args, format );
	vsprintf( buffer, format, args );
	va_end( args );

	strncpy( NormalString, buffer, CONSOLE_NORMALSTRING_LEN );
    m_nsOffset = ( int )strlen( NormalString );

	WORD	attr = 0;
	SetForeAttr( &attr, 'w' );
	SetBackAttr( &attr, 'b' );
	OutputString( attr, buffer );
}

void Console::ReadLine( LPTSTR buffer, DWORD bufferSize, DWORD* readLen )
{
	*readLen = 0;

	::ReadConsole( m_handleInput, buffer, bufferSize, readLen, 0 );
	buffer[*readLen] = 0;
}

void Console::SetForeAttr( WORD* attr, char c )
{
	switch( c )
	{
	case  'b':	*attr |= F_BLACK;							break;
	case  'r':	*attr |= F_RED;								break;
	case  'g':	*attr |= F_GREEN;							break;
	case  'y':	*attr |= F_YELLOW;							break;
	case  'l':	*attr |= F_BLUE;							break;
	case  'm':	*attr |= F_MAGENTA;							break;
	case  'c':	*attr |= F_CYAN;							break;
	case  'w':	*attr |= F_WHITE;							break;

	case  'B':	*attr |= FOREGROUND_INTENSITY | F_BLACK;	break;
	case  'R':	*attr |= FOREGROUND_INTENSITY | F_RED;		break;
	case  'G':	*attr |= FOREGROUND_INTENSITY | F_GREEN;	break;
	case  'Y':	*attr |= FOREGROUND_INTENSITY | F_YELLOW;	break;
	case  'L':	*attr |= FOREGROUND_INTENSITY | F_BLUE;		break;
	case  'M':	*attr |= FOREGROUND_INTENSITY | F_MAGENTA;	break;
	case  'C':	*attr |= FOREGROUND_INTENSITY | F_CYAN;		break;
	case  'W':	*attr |= FOREGROUND_INTENSITY | F_WHITE;	break;
	}
}

void Console::SetBackAttr( WORD* attr, char c )
{
	switch( c )
	{
	case  'b':	*attr |= B_BLACK;							break;
	case  'r':	*attr |= B_RED;								break;
	case  'g':	*attr |= B_GREEN;							break;
	case  'y':	*attr |= B_YELLOW;							break;
	case  'l':	*attr |= B_BLUE;							break;
	case  'm':	*attr |= B_MAGENTA;							break;
	case  'c':	*attr |= B_CYAN;							break;
	case  'w':	*attr |= B_WHITE;							break;

	case  'B':	*attr |= BACKGROUND_INTENSITY | B_BLACK;	break;
	case  'R':	*attr |= BACKGROUND_INTENSITY | B_RED;		break;
	case  'G':	*attr |= BACKGROUND_INTENSITY | B_GREEN;	break;
	case  'Y':	*attr |= BACKGROUND_INTENSITY | B_YELLOW;	break;
	case  'L':	*attr |= BACKGROUND_INTENSITY | B_BLUE;		break;
	case  'M':	*attr |= BACKGROUND_INTENSITY | B_MAGENTA;	break;
	case  'C':	*attr |= BACKGROUND_INTENSITY | B_CYAN;		break;
	case  'W':	*attr |= BACKGROUND_INTENSITY | B_WHITE;	break;
	}
}

void Console::OutputString( WORD attr, char* buffer )
{
	CONSOLE_SCREEN_BUFFER_INFO	consoleScrInfo;
	DWORD						dwsize = 0;
	COORD						curPos = {0, 0};

	int len = ( int )strlen( buffer );

	::GetConsoleScreenBufferInfo( m_handleOutput, &consoleScrInfo );
	curPos.X = consoleScrInfo.dwCursorPosition.X;
	curPos.Y = consoleScrInfo.dwCursorPosition.Y;

	::WriteConsoleOutputCharacter( m_handleOutput, buffer, len, curPos, &dwsize );
	::FillConsoleOutputAttribute( m_handleOutput, attr, len, curPos, &dwsize );

	curPos.X = curPos.X + (SHORT)len;
	if( curPos.X > 80 )
	{
		curPos.X = 0;
		curPos.Y++;
	}

	::SetConsoleCursorPosition( m_handleOutput, curPos );

	if( m_nsOffset + len < CONSOLE_NORMALSTRING_LEN )
	{
		strcat( &NormalString[m_nsOffset], buffer );
		m_nsOffset += len;

		NormalString[m_nsOffset] = 0;
	}
}

void Console::Tab()
{
	CONSOLE_SCREEN_BUFFER_INFO	consoleScrInfo;
	COORD						curPos = {0, 0};

	::GetConsoleScreenBufferInfo( m_handleOutput, &consoleScrInfo );
	curPos.X	= consoleScrInfo.dwCursorPosition.X + m_tabSize;
	curPos.Y	= consoleScrInfo.dwCursorPosition.Y;

	::SetConsoleCursorPosition( m_handleOutput, curPos );

	if( m_nsOffset + 1 < CONSOLE_NORMALSTRING_LEN )
	{
		NormalString[m_nsOffset++]	= '\t';
		NormalString[m_nsOffset]	= 0;
	}
}

void Console::NewLine()
{
	CONSOLE_SCREEN_BUFFER_INFO	consoleScrInfo;
	COORD						curPos = {0, 0};

	::GetConsoleScreenBufferInfo( m_handleOutput, &consoleScrInfo );
	if( consoleScrInfo.dwCursorPosition.Y == consoleScrInfo.dwSize.Y - 1 )
	{
		ScrollScreenBuffer( 0 );
		::GetConsoleScreenBufferInfo( m_handleOutput, &consoleScrInfo );

		curPos.X = 0;
		curPos.Y = consoleScrInfo.dwCursorPosition.Y;
	}
	else
	{
		curPos.X = 0;
		curPos.Y = consoleScrInfo.dwCursorPosition.Y + 1;
	}

	::SetConsoleCursorPosition( m_handleOutput, curPos );

	if( m_nsOffset + 1 < CONSOLE_NORMALSTRING_LEN )
	{
		NormalString[m_nsOffset++]	= '\n';
		NormalString[m_nsOffset]	= 0;
	}
}

void Console::ScrollScreenBuffer( int width )
{
	CONSOLE_SCREEN_BUFFER_INFO	consoleScrInfo;
    SMALL_RECT					srctScrollRect, srctClipRect;
    CHAR_INFO					chiFill;
    COORD						coordDest = {0, 0};

	::GetConsoleScreenBufferInfo( m_handleOutput, &consoleScrInfo );

    srctScrollRect.Left		= 0;
    srctScrollRect.Top		= 1;
    srctScrollRect.Right	= consoleScrInfo.dwSize.X - (SHORT)width;
    srctScrollRect.Bottom	= consoleScrInfo.dwSize.Y - (SHORT)width;

    coordDest.X		= 0;
    coordDest.Y		= 0;
    srctClipRect	= srctScrollRect;
 
    chiFill.Attributes		= FOREGROUND_RED|FOREGROUND_INTENSITY;
    chiFill.Char.AsciiChar	= ( char )' ';

    ::ScrollConsoleScreenBuffer( m_handleOutput, &srctScrollRect, &srctClipRect, coordDest, &chiFill );
}

bool Console::IsKeyboardHit()
{
	return ( kbhit() != 0 );
}

int Console::GetHitKey()
{
	if( kbhit() == 0 )
		return -1;

	return getch();
}