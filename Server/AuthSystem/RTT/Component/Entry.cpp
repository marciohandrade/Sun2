#include "stdafx.h"
#include "Entry.h"

#include <eXtreme/util/WinService.h>
#include <eXtreme/util/Registry.h>

eXtreme::Console	g_console;

void ErrorHandler( DWORD lastError, char* desc )
{
	OutputDebug( "Error = %d\n  %s\n", lastError, desc );
}

void OutputDebug( char* format, ... )
{
	SYSTEMTIME	systemTime;
	char		buffer[4092];
	va_list		args;

	va_start( args, format );
	vsprintf( buffer, format, args );
	va_end( args );


	GetLocalTime( &systemTime );

//	fprintf( g_fpLog, "[%2d/%2d %2d:%2d:%d] %s",
//		systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
//	fflush( g_fpLog );

	g_console.WriteLine( "[%2d/%2d %2d:%2d:%d] %s",
		systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
}

void main()
{
	eXtreme::Initialize( 0, ErrorHandler );

	if( Entry::OnStartService() == true )
		Entry::Instance()->Join();

	Entry::OnStopService();

	OutputDebug( "Press any key to abort program..." );
	getch();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
Entry::Entry()
{
}

Entry::~Entry()
{
}

bool Entry::OnStartService()
{
	if( Instance()->m_networkRTTS.Create( 1, &Instance()->m_managerRTTS ) == false )
		return false;


	OutputDebug( "#Wb!-*-*-* Service Started *-*-*-!#wb\n\n" );

	return true;
}

void Entry::OnStopService()
{
	Instance()->m_networkRTTS.Destroy();

	OutputDebug( "#Wb!-*-*-* Service Closed *-*-*-!#wb\n\n" );
}

void Entry::Join()
{
	while( true )
	{
		m_networkRTTS.Update();

		::Sleep( 1 );
	}
}