/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.06.10
*	��    �� : Include Header
*******************************************************************************/

#pragma once

#include <Windows.h>
#include <Wininet.h>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <crtdbg.h>
#include <SLM/FileDownloader/WZResult.h>
#include <SLM/FileDownloader/DownloadInfo.h>
#include <SLM/FileDownloader/IDownloaderStateEvent.h>

#pragma comment(lib, "Wininet.lib")

// VS 6.0 ���带 ���� �ڵ�

#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES ((DWORD)-1) 
#endif

#ifndef _WIN64
	#ifndef DWORD_PTR
		typedef unsigned long DWORD_PTR, *PDWORD_PTR;
	#endif
#endif
