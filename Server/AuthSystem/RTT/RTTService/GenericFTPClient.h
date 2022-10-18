#pragma once

#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#define __GENERIC_FTP_AGENT		"__GENERIC_FTP_AGENT"	// default internet agent name
#define __DEFAULT_BUFFER_SIZE	1024

class CGenericFTPClient
{
public:
	CGenericFTPClient(void);
	virtual ~CGenericFTPClient(void);

protected:
	void Initialize(void);
	void UnInitialize(void);

	HINTERNET m_hInetOpen;			// internet open handle
	HINTERNET m_hInetConnection;	// internet connection handle

public:		
	BOOL Open(LPCSTR lpFTPAddress, LPCSTR lpUserName, LPCSTR lpPassword);
	BOOL Open(LPCSTR lpFTPAddress, DWORD dwPort, LPCSTR lpUserName, LPCSTR lpPassword);
	
	void GetInternetFile(LPCSTR lpRemoteFileName, LPCSTR lpLocalFileName);
	void Close(void);
};
