// ProcessSnapShot.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <process.h>
#include "IniControl.h"

#include "../../ProgramCommon/Define.Build.h"



//using namespace std;

//static std::list<PROCESSINFO*> g_ProcessList;
static INT g_nWorldCount = 0;
static IniControl* piniFile;
static BOOL g_bZeroCount = 0;
//static CRITICAL_SECTION cs;
//static HANDLE* g_hHandleList;

static BOOL bQuitThread = FALSE;

void ConvertToTxt(CString strModulePath, char* const log_filename, char* const txt_filename)
{
    PROCESS_INFORMATION pProcInfo;
    TCHAR szParam[MAX_PATH * 2] = {0,};
    STARTUPINFO startUpInfo = { sizeof(STARTUPINFO),NULL,"",NULL,0,0,0,0,0,0,0,STARTF_USESHOWWINDOW,0,0,NULL,0,0,0};  
    startUpInfo.wShowWindow = SW_SHOW;
    startUpInfo.lpDesktop = NULL;//_T("WinSta0\\Default");

    StringCchPrintf(szParam, sizeof(szParam), _T("-logfile %s -txtfile %s"), log_filename, txt_filename);

    BOOL bResult = CreateProcess(strModulePath, szParam,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&startUpInfo,&pProcInfo);

    TRACE("Resut : %d", bResult);

    if(!bResult)
    {
        DWORD nErr = GetLastError();
        nErr = GetLastError();
        LOGF("Convert file fail : Error Code = %d", nErr);
    }
}

BOOL GetLogFileInfo()
{
	TCHAR * ptr = NULL;
	TCHAR szCurrentPath[MAX_PATH] = {0,};

	GetModuleFileName(NULL, szCurrentPath, MAX_PATH);
	ptr = _tcsrchr(szCurrentPath, _T('\\'));

	if ( NULL != ptr )
	{
		*ptr = 0;
	}
	else
	{
		if (FAILED(StringCchCopy(szCurrentPath, sizeof(szCurrentPath), _T("."))))
		{
			_ASSERTE(!"StringCchCopy fail");
		}
	}

	piniFile = new IniControl(SNAMPSHOT_CONFIG_FILE, szCurrentPath);

	g_nWorldCount = (INT)piniFile->GetValue(_T("LOG_COLLECTOR"), _T("WORLD_COUNT"), 0);

	if(g_nWorldCount == 0)
	{
		g_bZeroCount = TRUE;
		printf("\n=================================");
		printf("\nPlease Confirm \"Snapshot_Config.ini\" ");
		printf("\n=================================");
		return FALSE;
	}

	g_bZeroCount = FALSE;
	
	return TRUE;
}

/*
void ClearProcessList()
{
	::EnterCriticalSection(&cs);

	int cnt = (int)g_ProcessList.size();
	std::list<PROCESSINFO *>::iterator iter = g_ProcessList.begin();
	for (int i = 0; i < cnt; i++)
	{
		PROCESSINFO * pData = *iter;
		delete pData;
		pData = NULL;
		iter++;
	}
	g_ProcessList.empty();

	::LeaveCriticalSection(&cs);
}
*/

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch(fdwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
	//case CTRL_LOGOFF_EVENT:
	//case CTRL_SHUTDOWN_EVENT:
		printf("\n=================================");
		printf("\nPlease don't quit Program!!!!!");
		printf("\nThis SnapShot Program is keep watch LogManager Program!!");
		printf("\n=================================\n\n");
		return TRUE;
	default :
		return FALSE;
	}
}
/*
bool GetProcessModule(DWORD dwPID,CString sProcessName)
{ 
	HANDLE        hModuleSnap = NULL; 
	MODULEENTRY32 me32        = {0}; 

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 

	if (hModuleSnap == (HANDLE)-1) 
	{
		DWORD nErr = 0;
		nErr = GetLastError();
		LOGF("CreateToolhelp32Snapshot() FAIL : Error Code = %d", nErr);
		return (FALSE); 
	}
	me32.dwSize = sizeof(MODULEENTRY32); 

	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면 
	//true를 리턴한다.
	if(Module32First(hModuleSnap, &me32)) 
	{ 
		do 
		{ 
			if(me32.szModule == sProcessName)
			{ 
				CloseHandle (hModuleSnap); 
				return true;
			} 
		} 
		while(Module32Next(hModuleSnap, &me32)); 
	} 

	CloseHandle(hModuleSnap); 

	return false;
} 
*/

unsigned int __stdcall ReCreateProcessThread(LPVOID lParam)

{
	CString strProcessFullName;
	PROCESSINFO* pInfo = (PROCESSINFO*)lParam;

	strProcessFullName.Format("%s", (LPCTSTR)pInfo->strProcessFullName);

	CString strProcessName = strProcessFullName.Right(strProcessFullName.GetLength() - strProcessFullName.ReverseFind('\\') - 1);
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 

	while(TRUE)
	{
		BOOL bFindProcess = FALSE;
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

		if (hProcessSnap == (HANDLE)-1) 
		{
			DWORD nErr = 0;
			nErr = GetLastError();
			LOGF("ReCreateProcessThread CreateToolhelp32Snapshot() FAIL : Error Code = %d", nErr);

			return -1;
		}

		pe32.dwSize = sizeof(PROCESSENTRY32); 

		//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다

		if (Process32First(hProcessSnap, &pe32)) 
		{ 
			BOOL          bCurrent = FALSE; 
			BOOL		bExistList = FALSE;

			do 
			{ 
				if(pe32.szExeFile == strProcessName)
				{ 
					bFindProcess = TRUE;

					if(pe32.th32ProcessID == pInfo->dwProcessID)
					{
						bExistList = TRUE;
						break;
					}
					else
					{
						bExistList = FALSE;
					}

					if(!bExistList)
					{
						bFindProcess = FALSE;
					}
					else
					{
						bFindProcess = TRUE;
						break;
					}
				} 
				
			}while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.

			if(!bFindProcess || !bExistList) // 못 찾았다.
			{
				LOGF("Find SunLogManager Program Terminate!~!");
				//int cnt = (int)g_ProcessList.size();
				//cnt++;
				PROCESS_INFORMATION pProcInfo;
				TCHAR szParam[100] = {0,};
				STARTUPINFO startUpInfo = { sizeof(STARTUPINFO),NULL,"",NULL,0,0,0,0,0,0,0,STARTF_USESHOWWINDOW,0,0,NULL,0,0,0};  
				startUpInfo.wShowWindow = SW_SHOW;
				startUpInfo.lpDesktop = NULL;//_T("WinSta0\\Default");

#ifdef _NA_0_20110810_LOG_COLLECTOR_RESOTRE
                if(pInfo->is_restore_)
                {
                    StringCchPrintf(szParam, sizeof(szParam), _T("-world %d -restore"),9);// pInfo->dwWorldNo);
                }
                else
                {
#endif
                    StringCchPrintf(szParam, sizeof(szParam), _T("-world %d"), pInfo->dwWorldNo);
#ifdef _NA_0_20110810_LOG_COLLECTOR_RESOTRE
                }				
#endif
 
				BOOL bResult = CreateProcess(strProcessFullName, szParam,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&startUpInfo,&pProcInfo);

				TRACE("Resut : %d", bResult);

				if(!bResult)
				{
					DWORD nErr = GetLastError();
					nErr = GetLastError();
					LOGF("ReCreateProcessThread CreateProcess FAIL : Error Code = %d", nErr);
				}
				else
				{
					pInfo->dwProcessID = pProcInfo.dwProcessId;

					WaitForSingleObject(pProcInfo.hProcess, INFINITE);
			
					LOGF("ReExecute SunLogManager Program SUCCESS");
				}
			}

		} 
        
		CloseHandle (hProcessSnap); 

		if(bQuitThread)
		{
			break;
		}

		Sleep(3000);
	}
	
	delete pInfo;
	pInfo = NULL;

	return -1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char szModuleFullName[256] = {0};
	CString strModuleFullName;
	CString strModulePath;
	std::list<PROCESSINFO*> g_ProcessList;
	

	if(!GetLogFileInfo())
	{
		g_nWorldCount = 1;
	}

	//g_hHandleList = new HANDLE[g_nWorldCount];
	
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	GetModuleFileName(NULL, szModuleFullName, 256);

	strModuleFullName.Format("%s", szModuleFullName);

	int nIdx = strModuleFullName.ReverseFind('\\');
	strModulePath = strModuleFullName.Left(nIdx + 1);

	strModulePath += SNAPSHOT_PROC_NAME;

	DWORD dwPerformanceThreadID = 0;

	for(int i = 1; i <= g_nWorldCount; i++)
	{
		PROCESSINFO* pInfo = new PROCESSINFO;

		TCHAR szField[100] = {0,};

		INT nWorld = 0;
		StringCchPrintf(szField, sizeof(szField), _T("COUNT_%d"), i);
		nWorld = (INT)piniFile->GetValue(szField, _T("WORLDNO"), 0);

		pInfo->dwWorldNo = nWorld;

		StringCchCopy(pInfo->strProcessFullName, sizeof(pInfo->strProcessFullName), strModulePath);
		
        pInfo->is_restore_ = FALSE;
		_beginthreadex(NULL, 0, ReCreateProcessThread, (void*)pInfo, FALSE, (unsigned int*)&dwPerformanceThreadID );

#ifdef _NA_0_20110810_LOG_COLLECTOR_RESOTRE
        PROCESSINFO* restore_info = new PROCESSINFO;
        memcpy(restore_info, pInfo, sizeof(PROCESSINFO));
        restore_info->is_restore_ = TRUE;
        _beginthreadex(NULL, 0, ReCreateProcessThread, (void*)restore_info, FALSE, (unsigned int*)&dwPerformanceThreadID );		
#endif
	}

	

	printf( "SUN_LogManager Snapshot Processing.." );

    char input_strings[MAX_PATH];
    char* token = NULL;

    while(TRUE)
    {    
        gets(input_strings);

        token = strtok(input_strings ," ");

        if(token == NULL)
            continue;

        if(stricmp("q", token) == 0)
        {
            bQuitThread = TRUE;
            break;
        }
        else if(stricmp("c", token) == 0)
        {
            char* src_filename = strtok(NULL ," ");
            if(src_filename == NULL)
            {
                printf("Not Find Source fileName.\n");
                continue;
            }

            char* dest_filename = strtok(NULL ," ");
            if(dest_filename == NULL)
            {
                printf("Not Find Convert fileName.\n");
                continue;
            }

            ConvertToTxt(strModulePath, src_filename, dest_filename);
        }        
    }

    /*
	char chQuitKey;

	while(TRUE)
	{
		chQuitKey = getchar();

		if(chQuitKey == 'q' || chQuitKey == 'Q')
		{
			bQuitThread = TRUE;
			break;
		}

	}*/

	Sleep(1000);
/*
	DWORD dwStatus = 0;

	while(1)
	{
		if( g_nWorldCount == 0 )
			break;

		dwStatus = WaitForMultipleObjects(g_nWorldCount, g_hHandleList, TRUE, INFINITE);
		if( dwStatus == WAIT_FAILED )
		{
			printf("waitformultipleobject error");
			break;
		}


	}
*/

	//::WaitForMultipleObjects()

	delete piniFile;
	piniFile = NULL;

//	delete g_hHandleList;
//	g_hHandleList = NULL;

	return 0;
}

