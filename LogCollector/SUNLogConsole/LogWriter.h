/*------------------------------------------------------------------------------
 * LogWriter.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _LOG_WRITER_H_
#define _LOG_WRITER_H_

class DBHandler;

class LogWriter
{
public:
	LogWriter();
	~LogWriter();

    void StartWriteLog(int iMaxThreadNum);
	void StartWriteFailLog();
	void Stop();
	virtual void DoWriteLogDB(LPVOID pLogData) = 0;

	int					  m_iRunningThread;

protected:
	DBHandler			* m_pDBHandler;
	BOOL                  m_bWriteThreadExitFlag;	// 스레드 종료 플래그

	void AddMark(TCHAR * pString);

private:
	void DoWriteFailLogDB();

	static unsigned int __stdcall WriteLogThread(LPVOID lParam);
	static unsigned int __stdcall WriteFailLogThread(LPVOID lParam);

public:
	INT m_nWorldID;
	INT m_nChannelID;
};

#endif //_LOG_WRITER_H_