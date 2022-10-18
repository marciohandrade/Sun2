/*------------------------------------------------------------------------------
 * IniControl.cpp
 *------------------------------------------------------------------------------
 * ini파일의 값을 읽고 쓰고, lock
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "IniControl.h"
#include "LogCriticalSection.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
IniControl::IniControl()
{
	m_pIniCS = new LogCriticalSection;
	memset(m_szIniPath, 0 , sizeof(m_szIniPath));
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
IniControl::IniControl(TCHAR * szFileName)
{
	m_pIniCS = new LogCriticalSection;

	if (FAILED(StringCchPrintf(m_szIniPath, sizeof(m_szIniPath),
								_T("%s\\%s"),
								g_szCurrentPath,
								szFileName)))
	{
		_ASSERTE(!"StringCchPrintf");
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
IniControl::~IniControl()
{
	if (m_pIniCS)
	{
		delete m_pIniCS, m_pIniCS = NULL;
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void IniControl::SetIniPath(TCHAR * szFileName)
{
	if (FAILED(StringCchPrintf(m_szIniPath, sizeof(m_szIniPath),
								_T("%s\\%s"),
								g_szCurrentPath,
								szFileName)))
	{
		_ASSERTE(!"StringCchPrintf");
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
DWORD IniControl::GetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, CONST TCHAR* lpDef, TCHAR* lpRet, DWORD nSize)
{
	return GetPrivateProfileString(lpApp, lpKey, lpDef, lpRet, nSize, m_szIniPath);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
ULONGLONG IniControl::GetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, INT nDef)
{
	TCHAR szRet[MAX_PATH] = {0,};
	ULONGLONG ulObj = 0;

	if(0 < GetPrivateProfileString(lpApp, lpKey, "0", szRet, MAX_PATH, m_szIniPath))
	{
		ulObj = _atoi64(szRet);
	}
	else
	{
		ulObj = 0;
	}
	//return GetPrivateProfileInt(lpApp, lpKey, nDef, m_szIniPath);
	return ulObj;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL IniControl::SetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, CONST TCHAR* lpStr)
{
	return WritePrivateProfileString(lpApp, lpKey, lpStr, m_szIniPath);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL IniControl::SetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, ULONGLONG lpStr)
{
	TCHAR szTmp[128] = {0};

    if (FAILED(StringCchPrintf(szTmp, sizeof(szTmp), _T("%I64d"), lpStr)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

	return WritePrivateProfileString(lpApp, lpKey, szTmp, m_szIniPath);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void IniControl::LockIni()
{
	m_pIniCS->Enter();
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void IniControl::UnLockIni()
{
	m_pIniCS->Leave();
}