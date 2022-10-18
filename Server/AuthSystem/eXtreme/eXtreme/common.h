#ifndef  __eXtreme_Common_H
#define  __eXtreme_Common_H

#pragma warning(disable:4127)

#include <winsock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
#include <mswsock.h>

#include <process.h>

#include <assert.h>
#include <stdarg.h>
#include <conio.h>

#pragma warning(push)
#pragma warning(disable:4702)

#include <iostream>

#pragma warning(pop)

namespace eXtreme
{
	extern	void	CallbackErrorHandler(DWORD lastError, char* desc);
}


#endif