// ConsoleTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ConsoleTester.h"

#include "DBTest.h"
#include "FileTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define BUILD_KOREA_061211 
//#define BUILD_TAIWAN


#define LOGBUFFER_SIZE	2048

void _LOG(const char* szFormat, ...)
{
	char		szBuf[LOGBUFFER_SIZE] = {0};
	va_list list;
	va_start (list, szFormat);
	vsprintf (szBuf, szFormat, list);
	va_end (list);

	OutputDebugString(szBuf);
}

void WriteToFile(char* filepath, BYTE* lpData, int nSize)
{
	HANDLE hFile; 


	hFile = CreateFile(filepath,           // create MYFILE.TXT 
		GENERIC_WRITE,                // open for writing 
		FILE_SHARE_READ,                            // do not share 
		NULL,                         // no security 
		CREATE_ALWAYS,                // overwrite existing 
		FILE_ATTRIBUTE_NORMAL,      // normal file 
		NULL);                        // no attr. template 

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		//		_LOG("Could not open file. %s", filepath);  // process error 
		return;
	} 

	DWORD dwWritten;
	WriteFile(hFile, (BYTE*)lpData, nSize, &dwWritten, NULL);

	CloseHandle(hFile);

}


void ReadFromFile(char* filepath, char* lpData, int& nSize)
{
	HANDLE hFile; 

	hFile = CreateFile(filepath,           // create MYFILE.TXT 
		GENERIC_READ,                // open for writing 
		FILE_SHARE_READ,                            // do not share 
		NULL,                         // no security 
		OPEN_EXISTING,                // overwrite existing 
		FILE_ATTRIBUTE_NORMAL,      // normal file 
		NULL);                        // no attr. template 

	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		_LOG("Could not open file. %s", filepath);  // process error 
		return;
	} 

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwRead;
	ReadFile(hFile, (BYTE*)lpData, dwFileSize, &dwRead, NULL);

	nSize = dwRead;
	CloseHandle(hFile);
}


// The one and only application object

CWinApp theApp;

using namespace std;

typedef std::map<std::string, int> KEYMAP;
KEYMAP keyMap;
#define MAX_SKILL_SIZE	200
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{    
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	//_com_ptr_t aaa;
	ICSunOnlinePtr ptr;

	OutputDebugString("CreateInstance Start");

	ptr.CreateInstance(__uuidof(CCSunOnline));

	OutputDebugString("CreateInstance End");

	OutputDebugString("init Start");

#ifdef LANG_KO
	ptr->init(_bstr_t("ko"), 0);
#elif LANG_JP
	ptr->init(_bstr_t("jp"), 0);
#elif LANG_TW
	ptr->init(_bstr_t("tw"), 0);	
#elif LANG_CN
	ptr->init(_bstr_t("cn"), 0);	
#elif LANG_GLOBAL
	ptr->init(_bstr_t("en"), 0);	
#elif LANG_EN
    ptr->init(_bstr_t("en"), 0);	
#elif LANG_RU
    ptr->init(_bstr_t("ru"), 0);	
#else
	ptr->init(_bstr_t("ko"), 0);
#endif
	
	OutputDebugString("init End");

	ptr->createItem();


    return 0;

    
    BOOL skill_ret;

    
    _bstr_t mission_name2;
    _bstr_t area_name2;
    skill_ret = ptr->GetMissionName(10, &mission_name2.GetBSTR(), &area_name2.GetBSTR());
    printf("%s, %s, %s\n",skill_ret ? "TRUE" : "FALSE", mission_name2.operator char*(), area_name2.operator const char*());

    _bstr_t mission_name3;
    _bstr_t area_name3;
    skill_ret = ptr->GetMissionName(5, &mission_name3.GetBSTR(), &area_name3.GetBSTR());
    printf("%s, %s, %s\n",skill_ret ? "TRUE" : "FALSE", mission_name3.operator const char*(), area_name3.operator const char*());

        

    CDBTest dbtest;
    if(dbtest.DBConnect("10.101.152.16",6279,"SUNOnline_TW_0904","SunServiceID","qksWkr#17"))
    {
        dbtest.Test_Inventory(ptr);
    }

	return nRetCode;
}

