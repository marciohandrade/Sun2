/*------------------------------------------------------------------------------
 * SUNLogConsole.cpp
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "SUNLogConsole.h"
#include "LogMain.h"
#include "DBHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LogMain g_SUNLogMain;
DBHandler* g_pDBHandler;
//using namespace std;


int ArgumentParsing(TCHAR * szStart, TCHAR * szEnd, TCHAR * szServerIP, TCHAR * szPrefix);
int ArgumentParsing_NEW(TCHAR * szStart, TCHAR * szEnd, TCHAR * szServerIP, TCHAR * szPrefix, INT& nWorld, TCHAR * logfilepath, TCHAR * txtfilepath, BOOL& is_restore);
void SetArgValue(TCHAR * Argv, TCHAR * szIP, TCHAR * szPrefix);
int IsDate(TCHAR * szDate);
int IsInvalidDate(TCHAR * szDate);
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
static unsigned int __stdcall LookoutThread(LPVOID lParam);

/**	----------------------------------------------------------------------------
	\brief	main

	\param	
	\return	
------------------------------------------------------------------------------*/
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{	
	if ( !AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0) )
	{
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		return 1;
	}
	else
	{
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

		int nSize = sizeof(DWORD);
		TCHAR szStart   [MAX_PATH] = {0};
		TCHAR szEnd     [MAX_PATH] = {0};
		TCHAR szServerIP[MAX_PATH] = {0};
        TCHAR logfilepath[MAX_PATH] = {0};
        TCHAR txtfilepath[MAX_PATH] = {0};
		TCHAR szPrefix  [15]	   = {0};
        BOOL is_restore            = FALSE;
		INT   nWorld = 0;

		//if ( 0 != ArgumentParsing(szStart, szEnd, szServerIP, szPrefix) )
		if ( 0 != ArgumentParsing_NEW(szStart, szEnd, szServerIP, szPrefix, nWorld, logfilepath, txtfilepath, is_restore) )
			return FALSE;

		if ( 0 != g_SUNLogMain.Start(nWorld, szStart, szEnd, szServerIP, szPrefix, logfilepath, txtfilepath, is_restore) )
			return FALSE;

		char chQuitKey;

		_tprintf(_T("\n=============================================="));
		_tprintf(_T("\nRunning Real Time LogMain"));
		_tprintf(_T("\nQuit Key(q)"));

		while(TRUE)
		{
			if(	g_SUNLogMain.IsTimeSpanLog())
			{
				//정해준 모든 날짜가 끝났는지를 검사하는 스레드를 생성!!! 모든 작업이 끝났다면 
				// 메세지를 송출!!!!(사용자가 q 입력을 하도록 해야함!!!)
				DWORD dwThreadID = 0;
				HANDLE hLookoutThread = (HANDLE)_beginthreadex(NULL, 
															0, 
															LookoutThread, 
															NULL, 
															0, 
															(unsigned*)&dwThreadID);
			}

			_tprintf(_T("\nInput Key :"));

			chQuitKey = getchar();

			if ( chQuitKey == _T('q') || chQuitKey == _T('Q'))
			{
				break;
			}

			Sleep(100);
		}

		g_SUNLogMain.Stop();

	}

	return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	Console Control Handler

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch(fdwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	default :
		printf("\nreceive SHOTDOWN Event");
		printf("\nNow Exit Program~~!");
		g_SUNLogMain.Stop();
		return FALSE;
	}
}

/**	----------------------------------------------------------------------------
	\brief	프로그램 아규먼트로 날짜를 받을수 있다.
			IP도 지정할 수 있도록 수정해야함..
	\param	
	\return	
------------------------------------------------------------------------------*/
int ArgumentParsing(TCHAR * szStart, TCHAR * szEnd, TCHAR * szServerIP, TCHAR * szPrefix)
{
	int ret = 0;
	
	if ( __argc > 1 )
	{
		if ( FAILED(StringCchCopy(szStart, MAX_PATH, (const TCHAR*)__argv[1])) )
		{
			_ASSERTE(!"StringCchCopy fail");
			ret = -1;
		}
		else if ( 0 != IsDate(szStart) )
		{
			ret = -1;
		}
		else if ( __argc > 2)
		{
			if ( FAILED(StringCchCopy(szEnd, MAX_PATH, (const TCHAR*)__argv[2])) )
			{
				_ASSERTE(!"StringCchCopy fail");
				ret = -1;
			}
			else if ( 0 != IsDate(szEnd) )
			{
				
				SetArgValue(szEnd, szServerIP, szPrefix);

				ZeroMemory(szEnd, MAX_PATH);	
				

			}
			else if ( __argc > 3)
			{

				/*
				if (FAILED(StringCchCopy(szServerIP, MAX_PATH, (const TCHAR*)__argv[3])))
				{
					_ASSERTE(!"StringCchCopy fail");
					ret = -1;
				}
				*/
				SetArgValue((TCHAR*)__argv[3], szServerIP, szPrefix);
			}
		}

	}
	
	if( __argc > 4)
	{
		SetArgValue((TCHAR*)__argv[4], szServerIP, szPrefix);
		
	}

	if ( 0 != ret )
	{
		_tprintf(_T("Invalid Date!!!\n\n"));
		_tprintf(_T("Syntax: SUNLogConsole.exe [start date] [end date] [server IP]\n"));
		_tprintf(_T("(ex) SUNLogConsole.exe 20080101 20080101 10.1.1.1\n"));
		_tprintf(_T("(ex) SUNLogConsole.exe 20080101 20080102\n"));
		_tprintf(_T("(ex) SUNLogConsole.exe 20080101\n"));
	}

	return ret;
}

int ArgumentParsing_NEW(TCHAR * szStart, TCHAR * szEnd, TCHAR * szServerIP, TCHAR * szPrefix, INT& nWorld, TCHAR * logfilepath, TCHAR * txtfilepath, BOOL& is_restore)
{
	int ret = 0;
	TCHAR *token;
	TCHAR seps[4];
	TCHAR szCommand[MAX_PATH] = {0,};
	TCHAR szModuleName[MAX_PATH] = {0,};

	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	StringCchCopy(seps, sizeof(seps), _T(" "));

	StringCchCopy(szCommand, sizeof(szCommand), GetCommandLine());

	//////////////////////////////////////////////////////////////////////////
	// 테스트를 위해.
	//StringCchCat(szCommand, sizeof(szCommand), _T("-world 1 -start 20090318 -end 20090320"));
	//////////////////////////////////////////////////////////////////////////
	

	TCHAR *szCommandLine = szCommand;

	GetModuleFileName( GetModuleHandle( NULL ), szModuleName, MAX_PATH );

	//szCommandLine += _tcslen(szModuleName) + 2;

	//////////////////////////////////////////////////////////////////////
	// 연구가 필요!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
/*
	TCHAR* pSepChar = NULL;

	if(NULL != (pSepChar = _tcschr(szCommandLine, ' ')) && NULL != _tcsstr(szCommandLine, TIMESPAN_PROC_NAME))
	{
		PrintUserColorString(hStdOut, USER_RED, _T("TimeSpan LogProcess"));
		szCommandLine = ++pSepChar;
	}
	else
	{
		PrintUserColorString(hStdOut, USER_RED, _T("REALTIME LogProcess"));
		// 리얼타임 로그콜렉터임!
		szCommandLine += _tcslen(szModuleName) + 2;
	}
*/
	token = _tcstok( szCommandLine, seps );

    while( token != NULL )
	{
		if(!_tcsicmp(token, _T("-world")))
		{
			token = _tcstok( NULL, seps );
			if(token == NULL)
			{
				PrintUserColorString(hStdOut, USER_WHITE, _T("-world Token NULL\n"));
				break;
			}

			nWorld = atoi(token);

			TCHAR szPrint[MAX_PATH];

			if( nWorld <= 0 )
			{
				StringCchPrintf(szPrint, sizeof(szPrint), _T("-world Token NOT Find : %d"), nWorld);
				PrintUserColorString(hStdOut, USER_RED, szPrint);
				nWorld = 0;
			}

			StringCchPrintf(szPrint, sizeof(szPrint), _T("-world Token Find : %d"), nWorld);
			PrintUserColorString(hStdOut, USER_WHITE, szPrint);

		}
		else if(!_tcsicmp(token, _T("-start")))
		{
			token = _tcstok( NULL, seps );
			if(token == NULL)
				break;

			if ( 0 != IsDate(token) )
			{
				ret = -1;
				break;
			}

			if(FAILED(StringCchCopy(szStart, MAX_PATH, token)))
			{
				_ASSERTE(!"StringCchCopy fail");
				ret = -1;
				break;
			}

		}
		else if(!_tcsicmp(token, _T("-end")))
		{
			token = _tcstok( NULL, seps );
			if(token == NULL)
				break;

			if ( 0 != IsDate(token) )
			{
				ret = -1;
				break;
			}

			if(FAILED(StringCchCopy(szEnd, MAX_PATH, token)))
			{
				_ASSERTE(!"StringCchCopy fail");
				ret = -1;
				break;
			}
		}
		else if(!_tcsicmp(token, _T("-ip")))
		{
			token = _tcstok( NULL, seps );
			if(token == NULL)
				break;

			if(FAILED(StringCchCopy(szServerIP, MAX_PATH, token)))
			{
				_ASSERTE(!"StringCchCopy fail");
				ret = -1;
				break;
			}

		}
		else if(!_tcsicmp(token, _T("-prefix")))
		{
			token = _tcstok( NULL, seps );
			if(token == NULL)
				break;

			size_t len = 0;
			StringCchLength(token, MAX_PATH, &len);

			if(FAILED(StringCchCopy(szPrefix, MAX_PATH, token)))
			{
				_ASSERTE(!"StringCchCopy fail");
				ret = -1;
				break;
			}

			for(int i = 0; i < (int)len; i++)
			{
				TCHAR cTmp = szPrefix[i];

				if(0 == _istupper(cTmp))
				{
					szPrefix[i] = _totupper(cTmp);
				}

			}
		}
        else if(!_tcsicmp(token, _T("-logfile")))
        {
            token = _tcstok( NULL, seps );
            if(token == NULL)
                break;

            if(FAILED(StringCchCopy(logfilepath, MAX_PATH, token)))
            {
                _ASSERTE(!"StringCchCopy fail");
                ret = -1;
                break;
            }
        }
        else if(!_tcsicmp(token, _T("-txtfile")))
        {
            token = _tcstok( NULL, seps );
            if(token == NULL)
                break;

            if(FAILED(StringCchCopy(txtfilepath, MAX_PATH, token)))
            {
                _ASSERTE(!"StringCchCopy fail");
                ret = -1;
                break;
            }
        }
        else if(!_tcsicmp(token, _T("-restore")))
        {
            is_restore = TRUE;
        }

		token = _tcstok( NULL, seps );
	}

	return ret;
}


void SetArgValue(TCHAR * Argv, TCHAR * szIP, TCHAR * szPrefix)
{
	size_t len = 0;
	int nCount = 0;
	StringCchLength(Argv, MAX_PATH, &len);

	for(int i = 0; i< (int)len; i++)
	{
		TCHAR cTmp = Argv[i];
		if(cTmp == _T('.'))
		{
			//szContext[i] = '\"';
			nCount++;
		}
	}

	if(nCount == 3) // IP일 경우
	{
		if ( FAILED(StringCchCopy(szIP, MAX_PATH, Argv)) )
		{
			_ASSERTE(!"StringCchCopy fail");
		}
	}
	else  //Prefix(서버군)일 경우
	{
		if ( FAILED(StringCchCopy(szPrefix, MAX_PATH, Argv)) )
		{
			_ASSERTE(!"StringCchCopy fail");
		}

		for(int i = 0; i < (int)len; i++)
		{
			TCHAR cTmp = szPrefix[i];
            
			if(0 == _istupper(cTmp))
			{
				szPrefix[i] = _totupper(cTmp);
			}

		}
	}

}

/**	----------------------------------------------------------------------------
	\brief	szDate가 8자리 숫자로 된 날짜인지 체크한다.

	\param	szDate = "20080101" 형식의 날짜
	\return	
------------------------------------------------------------------------------*/
int IsDate(TCHAR * szDate)
{
	if (NULL == szDate)
	{
		_ASSERTE(!"IsDate");
		return -1;
	}

	if ( 8 == _tcslen(szDate) )
	{
		for (int i = 0; i < 8; i++)
		{
			if ( szDate[i] < 48 ||  szDate[i] > 57 )
			{
				return -1;
			}
		}
		return IsInvalidDate(szDate);
	}
	return -1;
}

int IsInvalidDate(TCHAR * szDate)
{
	if (NULL == szDate)
	{
		_ASSERTE(!"IsInvalidDate");
		return -1;
	}

	// 0 1 2 3 4 5 6 7 - index
	// 2 0 0 7 1 2 3 1 - date

	TCHAR szY[16] = {0};
	TCHAR szM[16] = {0};
	TCHAR szD[16] = {0};

	int nY = 0, nM = 0, nD = 0;
	BOOL bNextM = FALSE;
	BOOL bYY    = FALSE;

	szY[0] = szDate[0];
	szY[1] = szDate[1];
	szY[2] = szDate[2];
	szY[3] = szDate[3];
	szM[0] = szDate[4];
	szM[1] = szDate[5];
	szD[0] = szDate[6];
	szD[1] = szDate[7];

	nY = _ttoi(szY);
	nM = _ttoi(szM);
	nD = _ttoi(szD);

	if ( /*2008 > nY ||*/ 1 > nM || 12 < nM || 1 > nD )
	{
		return -1;
	}

	if ( 2 == nM )
	{
		if ( 0 == nY % 400 )		// 윤년
		{
			if ( 29 < nD )		return -1;
		}
		else if ( 0 == nY % 100 )	// 평년
		{
			if ( 28 < nD )		return -1;
		}
		else if ( 0 == nY % 4 )		// 윤년
		{
			if ( 29 < nD )		return -1;
		}
		else
		{
			if ( nD > 28 )		return -1;
		}
	}
	else if ( 4 == nM ||  6 == nM ||  9 == nM ||  11 == nM )	// 30일인 달
	{
		if ( 30 < nD )			return -1;
	}
	else
	{
		if ( 31 < nD )			return -1;
	}

	return 0;
}

static unsigned int __stdcall LookoutThread(LPVOID lParam)
{
	g_SUNLogMain.LookoutProcess();

	//_endthreadex(1);

	return 1;
}