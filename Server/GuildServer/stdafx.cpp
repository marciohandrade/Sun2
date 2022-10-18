#include "stdafx.h"

GuildLog* g_pLOG = NULL;

VOID DebugOut( TCHAR * tszMsg, ... )
{
	static TCHAR strBuffer[1024]; 

	va_list args; 
	va_start(args, tszMsg); 
	_vsnprintf( strBuffer, 1024, tszMsg, args ); 
	va_end(args); 

	_tprintf("%s\n", strBuffer);
}
