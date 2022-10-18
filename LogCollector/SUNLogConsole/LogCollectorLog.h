/*------------------------------------------------------------------------------
 * LogCollectorLog.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _LOGCOLLECTOR_H_
#define _LOGCOLLECTOR_H_

class LogCollectorLog
{
public:
	LogCollectorLog(TCHAR * szFile, TCHAR * szDir = NULL);
	~LogCollectorLog();
	void AddLog(TCHAR* lpszFormat,...);
	void AddSQL(TCHAR* szSQL);
	void AddWzl(void * pLog, int nlen);

private:
	TCHAR  m_szLogDir[MAX_PATH];
	TCHAR  m_szLogFile[MAX_PATH];
	TCHAR  m_szWzlFile[MAX_PATH];
};

#endif //_LOGCOLLECTOR_H_