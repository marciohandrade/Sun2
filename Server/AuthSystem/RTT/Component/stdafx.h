#pragma once

#pragma  warning(disable:4996)
#ifdef _DEBUG
	#pragma  comment(lib, "eXtremed.lib")
#else
	#pragma  comment(lib, "eXtreme.lib")
#endif

#include <Winsock2.h>
#include <Windows.h>
#include <Assert.h>

#include <stdio.h>
#include <conio.h>

#include <iostream>

#include <wininet.h>

#include <eXtreme/eXtreme.h>
#include <eXtreme/util/Stream.h>
#include <eXtreme/util/Console.h>

void	OutputDebug( char* format, ... );