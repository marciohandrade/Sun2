
#ifndef _SUN_LOG_WRITER_FILE_H_
#define _SUN_LOG_WRITER_FILE_H_

class WriteFileLog
{
private:
    struct LOG_TYPE
    {
        DWORD log_count_;
        TCHAR log_decription[32];
    };
public:
	WriteFileLog();
	virtual ~WriteFileLog();	

public:
    bool WriteLogFile(TCHAR * const logfilepath, TCHAR * const txtfilepath);
};

#endif //_SUN_LOG_WRITER_FILE_H_
