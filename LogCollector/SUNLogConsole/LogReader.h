/*------------------------------------------------------------------------------
 * LogReader.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _LOG_READER_H_
#define _LOG_READER_H_

#include <stdio.h>
#include <list>

class IniControl;
class CFileReader;

class LogReader
{
public:
	LogReader();
	virtual ~LogReader();

    int            StartReadLogFile();
	void           Stop();
	IniControl   * GetReadIni(){ return m_pReadIni; };

	std::list <LPVOID>        m_LogDataList;			// 읽어온 로그 데이터
	int					 m_iRunningThread;

protected:
	IniControl         * m_pConfigIni;
	IniControl         * m_pReadIni;
	BOOL                 m_bReadThreadExitFlag;	// 스레드 종료 플래그
	TCHAR                m_szLogFileName[MAX_PATH];
	TCHAR                m_szStartDate[16];
	TCHAR                m_szEndDate[16];
	TCHAR                m_szServerIP[32];
	TCHAR				 m_szPrefix[15];
	INT					 m_nWorld;
	std::list <SERVERINFO *>  m_ServerList;			// 읽을 서버의 정보리스트
	ULONGLONG			 m_FindOffsetSize;
	ULONGLONG			 m_FirstFindOffset;
	LONG					 m_nNextDayCount;
	INT					 m_nTotalReadCount;
	std::list<TCHAR*>    m_ReadList;
	BOOL				 m_bFirstPrint;
	TCHAR				 m_szLocalLogPath[MAX_PATH];
	BOOL				 m_bLocalLog;
    BOOL                 is_restore_;
	
	void           SaveLastReadInfo(SERVERINFO * pstInfo);
	void		   GetLastReadInfo(SERVERINFO * pstInfo);
	BOOL		   IsLogComplete(SERVERINFO* sInfo, BOOL bFindOffset = FALSE);
	VOID		   RefreshDayDateQueue();
	VOID		   InsertDayDateQueue(LPCTSTR pstrDay);
	VOID		   PrintNowReadDay();

	VOID           PrintfFirstReadFileName(SERVERINFO* pInfo = NULL);

#ifdef _NA_0_20110803_DIV_GAMELOG_FILE
    struct CHANGE_FILE
    {
        bool change_;
        bool change_day_;
    };
    BOOL InCreaseFileNumber(const TCHAR* src_file, TCHAR* des_file, size_t des_size);
    void ChangeNextFilePath(SERVERINFO* sInfo, CHANGE_FILE& OUT result);
    int FindNextFilePath(SERVERINFO* sInfo, SERVERINFO* OUT out_put);
    BOOL GetNextFilePath(SERVERINFO* sInfo);
    BOOL IsReadComplete(SERVERINFO* sInfo);
    BOOL IsExistFilePath(const TCHAR* file_path);
#endif

private:
	HANDLE				m_hThreadSemaphore;
	CRITICAL_SECTION	m_ReadListCS;

private:
	virtual int    DoReadLogFile(SERVERINFO *pstInfo) = 0;

    void            StartRestore();
	void           StartRealTimeRead();
	void           StartTimeSpanRead();
	int            ConnectSession(SERVERINFO * pServerInfo);
	int            GetServerInfo();
	void           ClearServerList();
	void           ClearLogDataList();
	void           GetReadFilePath(SERVERINFO * pServerInfo, TCHAR * szDate = NULL);
	void		   GetReadLocalFilePath(SERVERINFO * pInfo, TCHAR * szDate, TCHAR* szLocalPrefixList = NULL);
	void           GetServerTypeString(DWORD nType, TCHAR * szType);
	BOOL           IsTodayFile(SERVERINFO* sInfo);
    BOOL            GetTodayDate(TCHAR * szDate);
	BOOL           IsNextDay(TCHAR * szCurrentDate);
    BOOL            IsPrevDay(TCHAR * szCurrentDate);
    


    static unsigned int __stdcall RestoreReadLogThread(LPVOID lParam);
    static unsigned int __stdcall RealTimeReadLogThread(LPVOID lParam);
	static unsigned int __stdcall TimeSpanReadLogThread(LPVOID lParam);

public:
	void		   GetSemaphore();
	void		   ReturnSemaphore();
	void		   MakeSemaphore(int iMax = 12);
	void		   DeleteSemaphore(void);
};

#endif //_LOG_READER_H_