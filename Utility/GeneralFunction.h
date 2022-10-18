
#pragma once


BOOL EnterLockApplication( HANDLE hLock, TCHAR * pszLockName );
void LeaveLockApplication( HANDLE hLock );


BOOL				WriteParserErrLog(BYTE byLogLevel, TCHAR* pszMsg, ... );
void				SetSaveParserLog(BYTE byLogLevel);