#ifndef  __WBANetwork_Common_H
#define  __WBANetwork_Common_H

#include <winsock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#include <process.h> 
#include <assert.h>
#include <stdarg.h>
#include <conio.h>
#include <atlconv.h>

#include <iostream>
#include <tchar.h>
#include <strsafe.h>


namespace WBANetwork
{
	extern void CallbackErrorHandler(DWORD lastError, char* desc);
}


#endif