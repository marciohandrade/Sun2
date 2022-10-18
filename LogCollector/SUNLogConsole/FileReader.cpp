/*------------------------------------------------------------------------------
 * FileReader.cpp
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by 
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "FileReader.h"
#include "LogCollectorLog.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
CFileReader::CFileReader(void)
:	m_hFileStream(NULL),
	m_dwErrCode(0),
	m_dwSystemErrCode(0)
{
	ZeroMemory(m_szFileName, sizeof(m_szFileName));
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
CFileReader::~CFileReader(void)
{
	Close();
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL CFileReader::Open(TCHAR* pszFileName)
{
	memcpy(m_szFileName, pszFileName, MAX_PATH);

	m_hFileStream = ::CreateFile(pszFileName, 
								GENERIC_READ, 
								FILE_SHARE_WRITE,
								NULL,
								OPEN_EXISTING, 
								FILE_ATTRIBUTE_NORMAL,
								0);
	
	if (INVALID_HANDLE_VALUE == m_hFileStream)
	{
		m_hFileStream = NULL;
		m_dwSystemErrCode = ::GetLastError();
		SetConvertSystemErr();
		//m_dwErrCode = LOGSTATE_OPEN_FAIL;
/*
		LogCollectorLog cLog("FAIL_FILE_READ_LOG", "FAIL_FILE_READ_LOG");
	
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);

		cLog.AddLog("CreateFile MSG : %s", lpMsgBuf);

		LocalFree( lpMsgBuf );
*/
		return FALSE;
	}

	return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL CFileReader::SetLogFileOffset(fpos_t ulOffset)
{
	if (ulOffset < 0)
	{
		ulOffset = 0;
	}

	LARGE_INTEGER li;
	
	li.QuadPart = ulOffset;

	BOOL bSuccess = ::SetFilePointerEx(m_hFileStream, li, NULL, FILE_CURRENT);

	if (bSuccess == INVALID_SET_FILE_POINTER
		&& GetLastError() != NO_ERROR)
	{
		/*
		LogCollectorLog cLog("FAIL_FILE_READ_LOG", "FAIL_FILE_READ_LOG");
		
				LPVOID lpMsgBuf;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					::GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		
				cLog.AddLog("SetFilePointerEx MSG : %s", lpMsgBuf);
		
				LocalFree( lpMsgBuf );
		*/
		

		return FALSE;
	}

	return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
fpos_t CFileReader::GetLogFileOffset()
{
	LARGE_INTEGER liInput;
	LARGE_INTEGER liOutput;

	liInput.QuadPart = 0;
	liOutput.QuadPart = 0;

	BOOL bSuccess = ::SetFilePointerEx(m_hFileStream, liInput, &liOutput, FILE_CURRENT);

	if (bSuccess == INVALID_SET_FILE_POINTER
		&& GetLastError() != NO_ERROR)
	{
		liOutput.QuadPart = 0;
	}

	return liOutput.QuadPart;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LP_BINLOG_BASE_INFO CFileReader::Read()
{
	DWORD dwReadLen = 0;
	BINLOG_BASE_INFO BaseInfo;
	LP_BINLOG_BASE_INFO pReadLog = NULL;

	memset(&BaseInfo, 0, sizeof(BINLOG_BASE_INFO));

	BOOL bSuccess = ::ReadFile(m_hFileStream, 
								(LPVOID)&BaseInfo,
								sizeof(BINLOG_BASE_INFO),
								&dwReadLen,
								NULL);

	if (bSuccess)
	{
		if (dwReadLen < sizeof(BINLOG_BASE_INFO) && dwReadLen != 0)
		{
			m_dwErrCode = LOGSTATE_INVALIED_OFFSET;	
		}
		else if(dwReadLen == 0)
		{
			m_dwErrCode = LOGSTATE_END;
		}
		else
		{
			pReadLog = SelectRealObject(&BaseInfo, dwReadLen);
			SetConvertSystemErr();
		}
	}
	else
	{
		/*
		LogCollectorLog cLog("FAIL_FILE_READ_LOG", "FAIL_FILE_READ_LOG");

		LPVOID lpMsgBuf;
		FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					::GetLastError(),
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
		
		cLog.AddLog("ReadFile MSG : %s", lpMsgBuf);
		
		LocalFree( lpMsgBuf );
		*/

		SetConvertSystemErr();
	}
	
	
	
	return pReadLog;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LP_BINLOG_BASE_INFO CFileReader::SelectRealObject(LP_BINLOG_BASE_INFO pBaseInfo, DWORD dwReadLen)
{
	LP_BINLOG_BASE_INFO pReadLog = NULL;
	DWORD dwRealReadLen = 0;
	BOOL bSuccess = FALSE;

	switch(pBaseInfo->category.category)
	{
/*
	case CATEGORY_LOGIN :
		{
			int nSize = sizeof(BINLOG__LOGIN) - dwReadLen;

			pReadLog = new BINLOG__LOGIN;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
*/
	case CATEGORY_CHARACTER_LOGIN :
		{
			int nSize = sizeof(BINLOG__CHARACTER_LOGIN) - dwReadLen;
			//LP_BINLOG__CHARACTER_LOGIN pReadLog = NULL;
			pReadLog = new BINLOG__CHARACTER_LOGIN;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_CHARACTER_CREATE :
		{
			int nSize = sizeof(BINLOG__CHARACTER_CREATE) - dwReadLen;
			//LP_BINLOG__CHARACTER_CREATE pReadLog = NULL;
			pReadLog = new BINLOG__CHARACTER_CREATE;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_CHAT :
		{
			break;
		}
	case CATEGORY_SKILL :
		{
			break;
		}
	case CATEGORY_QUEST :
		{
			int nSize = sizeof(BINLOG__QUEST) - dwReadLen;
			//LP_BINLOG__QUEST pReadLog = NULL;
			pReadLog = new BINLOG__QUEST;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_COMBAT :
		{
			int nSize = sizeof(BINLOG__COMBAT) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__COMBAT;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_ITEM :
		{
			int nSize = sizeof(BINLOG__ITEM) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__ITEM;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_PARTY :
		{
			//pReadLog = new ;
			break;
		}
	case CATEGORY_SQUAD :
		{
			//pReadLog = new ;
			break;
		}
	case CATEGORY_CHARACTER_STATE :
		{
			int nSize = sizeof(BINLOG__CHARACTER_STATE) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__CHARACTER_STATE;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_CHARACTER_SNAPSHOT :
		{
			int nSize = sizeof(BINLOG__CHARACTER_SNAPSHOT) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__CHARACTER_SNAPSHOT;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_WAREHOUSE_SNAPSHOT :
		{
			int nSize = sizeof(BINLOG__WAREHOUSE_SNAPSHOT) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__WAREHOUSE_SNAPSHOT;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_SKILL_SNAPSHOT :
		{
			int nSize = sizeof(BINLOG__SKILL_SNAPSHOT) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__SKILL_SNAPSHOT;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
		/*
	case CATEGORY_LADDER_SNAPSHOT:
		{
			int nSize = sizeof(BINLOG__LADDER_SNAPSHOT) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__LADDER_SNAPSHOT;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
		*/
	case CATEGORY_MISSION_SNAPSHOT:
		{
			int nSize = sizeof(BINLOG__MISSION_SNAPSHOT) - dwReadLen;
			//LP_BINLOG__COMBAT pReadLog = NULL;
			pReadLog = new BINLOG__MISSION_SNAPSHOT;
			memcpy(pReadLog, pBaseInfo, sizeof(BINLOG_BASE_INFO));
			BYTE* pTmpReadLog = (BYTE*)pReadLog + sizeof(BINLOG_BASE_INFO);
			bSuccess = ::ReadFile(m_hFileStream, (LPVOID)pTmpReadLog, nSize, &dwRealReadLen, NULL);
			break;
		}
	case CATEGORY_GM_LOG :
		{
			//pReadLog = new ;
			break;
		}
	case CATEGORY_MAIL :
		{
			//pReadLog = new ;
			break;
		}
	case CATEGORY_GUILD :
		{
			//pReadLog = new ;
			break;
		}
	default :
		LogCollectorLog cLog(_T("UNKNOWN_OBJ"), _T("UNKNOWN_OBJ"));
		cLog.AddLog(_T("==================================================\n"));
		cLog.AddLog(_T("Not Define Log\n"));
		cLog.AddLog(_T("Name : %s, OFFSET : %I64d\n"), m_szFileName, GetLogFileOffset());
		cLog.AddLog(_T("==================================================\n"));
		m_dwErrCode = LOGSTATE_INVALIED_OFFSET;
		break;
	}

	return pReadLog;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void CFileReader::Close()
{
	::CloseHandle(m_hFileStream);
	m_hFileStream = NULL;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
VOID CFileReader::SetConvertSystemErr()
{
	switch(m_dwSystemErrCode)
	{
	case 0:
		m_dwErrCode = LOGSTATE_LOADING;
		break;
	case ERROR_FILE_NOT_FOUND :
		m_dwErrCode = LOGSTSTE_NOT_FOUND_FILE;
		break;
	case ERROR_NETWORK_BUSY :
		// 네트워크 상태가 좋지 않음.
		m_dwErrCode = LOGSTATE_NETWORK_BUSY;
		break;
	case ERROR_HANDLE_EOF :
		// 파일의 끝에 도달.
		m_dwErrCode = LOGSTATE_REALTIME_SAME_OFFSET;
		break;
	case ERROR_BAD_NETPATH :
	case ERROR_NETWORK_ACCESS_DENIED :
		// 네트워크 경로가 잘못되어 있음.
		m_dwErrCode = LOGSTATE_ACCESS_DENY;
		break;
	case ERROR_PATH_NOT_FOUND :
		m_dwErrCode = LOGSTATE_FOLDER_NOT_FOUND;
		break;
	default :
		m_dwErrCode = LOGSTATE_UNKNOWN_FAIL;
		break;
	}
}

