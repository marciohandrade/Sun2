/*------------------------------------------------------------------------------
 * IniControl.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _INICONTROL_H_
#define _INICONTROL_H_

class LogCriticalSection;

class IniControl
{
public:
	IniControl();
	IniControl(TCHAR * szFileName);
	~IniControl();
	void SetIniPath(TCHAR * szFileName);

	DWORD	   GetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, CONST TCHAR* lpDef, TCHAR* lpRet, DWORD nSize);
	ULONGLONG  GetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, INT nDef);
	BOOL	   SetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, CONST TCHAR* lpStr);
	BOOL	   SetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, ULONGLONG  lpStr);

	void  LockIni();
	void  UnLockIni();

private:
	TCHAR                m_szIniPath[MAX_PATH];
	LogCriticalSection * m_pIniCS;

};

#endif //_INICONTROL_H_