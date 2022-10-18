// stdafx.cpp : source file that includes just the standard includes
// gameLogic_Sun.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void ConvertBSTRToString(BSTR pSrc, std::string& strSrc)
{
	if(!pSrc) return ;

	DWORD cb,cwch = ::SysStringLen(pSrc);//convert even embeded NULL

	char *szOut = NULL;

	if(cb = ::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch, NULL, 0, NULL, NULL))
	{
		szOut = new char[cb + 1];
		memset(szOut,0,cb + 1);
		if(szOut)
		{
			if(!::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch, szOut, cb + 1, NULL, NULL))
			{
				delete []szOut;//clean up if failed;
				szOut = NULL;
			}
		}
	}

	if(szOut)
	{
		strSrc = szOut;
		delete[] szOut;	
	}	
};

BOOL MessageOut(BYTE byLogLevel, TCHAR* pszMsg, ...)
{
    return true;
}
