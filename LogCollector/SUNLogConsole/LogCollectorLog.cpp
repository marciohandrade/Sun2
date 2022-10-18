/*------------------------------------------------------------------------------
 * LogCollectorLog.cpp
 *------------------------------------------------------------------------------
 * 로그 콜랙터의 로그를 남기는 클래스
 * 생성시 파일이름과 디렉토리를 넣고
 * 상황에 맞게 쓰기 함수 호출.
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "LogCollectorLog.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogCollectorLog::LogCollectorLog(TCHAR * szFile, TCHAR * szDir)
{
	if (NULL == szFile)
	{
		_ASSERTE(!"LogCollectorLog");
	}

	if (NULL == szDir)
	{
		if (FAILED(StringCchCopy(m_szLogDir, sizeof(m_szLogDir), g_szCurrentPath)))
		{
			_ASSERTE(!"StringCchCopy");
		}
	}
	else
	{
		if (FAILED(StringCchPrintf(m_szLogDir, sizeof(m_szLogDir),
									_T("%s\\%s"),
									g_szCurrentPath,
									szDir)))
		{
			_ASSERTE(!"StringCchPrintf");
		}
	}

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	if (FAILED(StringCchPrintf(m_szLogFile, sizeof(m_szLogFile),
								_T("%s\\%s_%04d%02d%02d.txt"),
								m_szLogDir,
								szFile,
								stTime.wYear,
								stTime.wMonth,
								stTime.wDay)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

	if (FAILED(StringCchPrintf(m_szWzlFile, sizeof(m_szWzlFile),
								_T("%s\\%s_%04d%02d%02d.wzl"),
								m_szLogDir,
								szFile,
								stTime.wYear,
								stTime.wMonth,
								stTime.wDay)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogCollectorLog::~LogCollectorLog()
{
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogCollectorLog::AddLog(TCHAR* lpszFormat,...)
{
	TCHAR szTemp[1024*8] = {0};

    DWORD error = ::GetLastError();

	va_list list;
	va_start(list, lpszFormat);
	if (FAILED(StringCchVPrintf(szTemp, sizeof(szTemp), lpszFormat, list)))
	{
		_ASSERTE(!_T("StringCbVPrintf"));
		return;
	}
	va_end(list);

	FILE *pFile = _tfopen(m_szLogFile, _T("a+"));
	if (NULL == pFile)
	{
		CreateDirectory(m_szLogDir, NULL);

		pFile = _tfopen(m_szLogFile, _T("a+"));
		if (NULL == pFile)
		{
            ::SetLastError(error);
			return;
		}
	}

	fseek(pFile, 0L, SEEK_END);

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	TCHAR szReturn[10] = _T("\r\n");

	_ftprintf(pFile, _T("[%d-%02d-%02d %02d:%02d:%02d.%03d] %s%s"), 
			stTime.wYear, 
			stTime.wMonth, 
			stTime.wDay,
			stTime.wHour,
			stTime.wMinute,
			stTime.wSecond,
			stTime.wMilliseconds,
			szTemp,
			szReturn);

	fclose(pFile);

    ::SetLastError(error);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogCollectorLog::AddSQL(TCHAR* szSQL)
{
	if (NULL == szSQL)
	{
		_ASSERTE(!_T("AddSQL"));
	}

    DWORD error = ::GetLastError();

	FILE *pFile = _tfopen(m_szLogFile, _T("a+"));
	if (NULL == pFile)
	{
		CreateDirectory(m_szLogDir, NULL);

		pFile = _tfopen(m_szLogFile, _T("a+"));
		if (NULL == pFile)
		{
            ::SetLastError(error);
			return;
		}
	}

	fseek(pFile, 0L, SEEK_END);

	_ftprintf(pFile, _T("%s\r\n"), szSQL);

	fclose(pFile);

    ::SetLastError(error);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogCollectorLog::AddWzl(void * pLog, int nlen)
{
	if (NULL == pLog)
	{
		_ASSERTE(!_T("AddWzl"));
	}

    DWORD error = ::GetLastError();

	FILE *pFile = _tfopen(m_szWzlFile, _T("a+"));
	if (NULL == pFile)
	{
		CreateDirectory(m_szLogDir, NULL);

		pFile = _tfopen(m_szWzlFile, _T("a+"));
		if (NULL == pFile)
		{
            ::SetLastError(error);
			return;
		}
	}

    fseek(pFile, 0L, SEEK_END);

	TCHAR * pStr = (TCHAR *)pLog;
	fwrite(pLog, sizeof(char), nlen, pFile);
	_ftprintf(pFile, _T("\r\n"));
	fclose(pFile);

    ::SetLastError(error);
}
