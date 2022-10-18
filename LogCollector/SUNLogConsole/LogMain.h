/*------------------------------------------------------------------------------
 * LogMain.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _LOG_MAIN_H_
#define _LOG_MAIN_H_

class SUN_LogReader;
class SUN_LogWriter;
class SUN_DBHandler;
class LogCriticalSection;
class IniControl;

class LogMain
{
public:
	LogMain();
	~LogMain();

    int  Start(INT nWorld, TCHAR * szStartDate, TCHAR * szEndDate, TCHAR * szServerIP, TCHAR * szPrefix, TCHAR * logfilepath, TCHAR * txtfilepath, BOOL is_restore);
	void Stop();

	SUN_DBHandler * GetDBHandler(){ return m_pDBHandler; };
	SUN_LogReader * GetLogReader(){ return m_pLogReader; };
	SUN_LogWriter * GetLogWriter(){ return m_pLogWriter; };
	IniControl   * GetConfigIni(){ return m_pConfigIni; };

	BOOL	IsTimeSpanLog() { return m_bTimeSpan; };
	VOID	SetTimeSpanLog(BOOL bTimeSpan) { m_bTimeSpan = bTimeSpan; }
	VOID	LookoutProcess();   

	enum { FILE_START_BEGIN, FILE_START_LASTOFFSET };

private:
	static	void						ProcCrashHandling( LPCTSTR stackWalkLog, LPCTSTR dumpFileName );
	static	LPCTSTR						GenerateDmpName();

private:
	SUN_DBHandler		* m_pDBHandler;
	SUN_LogReader		* m_pLogReader;
	SUN_LogWriter		* m_pLogWriter;
	IniControl			* m_pConfigIni;

//	int					  m_iFileOrientation;
	int					  m_iMaxThreadNum;
	BOOL				  m_bTimeSpan;
    BOOL                    is_restore_;
	int		SetDBHandler(INT nWorldNo);
	void	GetCurrentPath();
	void    BackUpReadIni();

public:
	INT m_nWorldID;
	INT m_nChannelID;

    struct  
    {
        TCHAR szDSN    [MAX_DB_PW];
        TCHAR szDBName [MAX_DB_PW];
        TCHAR szID     [MAX_DB_PW];
        TCHAR szPwd    [MAX_DB_PW];
        TCHAR szIniFile[MAX_PATH];
        TCHAR szWorldDBName[MAX_DB_PW];
    };
};

#endif //_LOG_MAIN_H_