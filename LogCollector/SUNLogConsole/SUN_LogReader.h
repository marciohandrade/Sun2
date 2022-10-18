/*------------------------------------------------------------------------------
 * SUN_LogReader.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _SUN_LOG_READER_H_
#define _SUN_LOG_READER_H_

#include "LogReader.h"

class LogWriter;
class BaseLogData;

class SUN_LogReader : public LogReader
{
public:
    SUN_LogReader(INT nWorld, TCHAR * szStartDate, TCHAR * szEndDate, TCHAR * szServerIP, TCHAR * szPrefix, BOOL is_restore);
    virtual ~SUN_LogReader();

private:
    virtual int    DoReadLogFile(SERVERINFO *pstInfo);    
    int            ReadLog(CWriteLogMng& LogMgr, SERVERINFO *pServerInfo);

private:
    void WriteLogToDataBase(SERVERINFO* server_info, LogWriter* log_writer, fpos_t offset, BaseLogData* log_data); 
};

#endif //_SUN_LOG_READER_H_
