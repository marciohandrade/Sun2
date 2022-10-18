/*------------------------------------------------------------------------------
 * IniControl.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _INICONTROL_H_
#define _INICONTROL_H_

class IniControl
{
public:
	IniControl();
	IniControl(TCHAR * szFullFileName, TCHAR* szFilePath);
	~IniControl();
	void SetIniPath(TCHAR * szFullFileName, TCHAR* szFilePath);

	DWORD	   GetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, CONST TCHAR* lpDef, TCHAR* lpRet, DWORD nSize);
	ULONGLONG  GetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, INT nDef);
	BOOL	   SetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, CONST TCHAR* lpStr);
	BOOL	   SetValue(CONST TCHAR* lpApp, CONST TCHAR* lpKey, ULONGLONG  lpStr);


private:
	TCHAR                m_szIniPath[MAX_PATH];

};

#endif //_INICONTROL_H_