#pragma once

#include "BaseLogData.h"
#include "LogArchive.h"

class SolarLog;

//---------------------------------------------------------------------------------------------------------------------

#ifdef _NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------

// 기존의 파일 포인터를 핸들로 교체한다.
// lseek의 사용으로 2GByte이상의 데이터 액세스를 가능하도록 수정한다.
class DLL_DECLARE CWriteLogMng
{
public:
    CWriteLogMng(VOID);
    ~CWriteLogMng(VOID);

private:
    struct _MEMBER;
    _MEMBER * m_pMEMBER;

protected:
    SolarLog *  m_pSolarLog;
    SolarLog *	m_pSolarLogText;
    BOOL		m_bTextOut;	

    enum 
    { 
        Null_File = -1, 
    };
    int         file_handle_;
    fpos_t      end_offset_;

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    BYTE*       m_pSpoolBuffer;                     // 스풀 버퍼 포인터
    static const size_t SPOOL_BUFFER_SIZE = 102400; // 스풀 버퍼 크기 (100KB)
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

public:
    BOOL	Init( TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName = NULL, BOOL bTextOut = FALSE );
    VOID	Serialize( BaseLogData& IN rData );
    BOOL	Open( TCHAR* pszFileName);
    BOOL	Open( TCHAR* pszFileName, fpos_t& end_offset);
    BOOL	SetLogFileOffset(fpos_t Pos);
    fpos_t	GetLogFileOffset();
    BOOL	Close();

    enum 
    {
        error_unknown = -1,
        error_success = 0,
        error_eof = 1,
        error_category = 2,
    };

    BaseLogData* Read(int& error_code);
#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
    BaseLogData* Read(bool autoSkip = false);
#else
    BaseLogData* Read();
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
    SolarLog* GetSolarLog() { return m_pSolarLog; }
    SolarLog* GetSolarText() { return m_pSolarLogText; }
    BOOL GetTextOut() { return m_bTextOut; }
#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
private:
    // 파일 스트림의 다음 sampleLength 길이 내에 레코드 시작이 있는지 검색한다.
    bool SearchForNextRecord(size_t sampleLength, __int64& offset_next);
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
};

//---------------------------------------------------------------------------------------------------------------------

#else //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------

class DLL_DECLARE CWriteLogMng
{
public:
	CWriteLogMng(VOID);
	~CWriteLogMng(VOID);

private:

	struct _MEMBER;
	_MEMBER * m_pMEMBER;
protected:
	SolarLog *  m_pSolarLog;
	SolarLog *	m_pSolarLogText;
	BOOL		m_bTextOut;	
	FILE*		m_pFileStream;	
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    BYTE*       m_pSpoolBuffer;                     // 스풀 버퍼 포인터
    static const size_t SPOOL_BUFFER_SIZE = 102400; // 스풀 버퍼 크기 (100KB)
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
public:
	BOOL	Init( TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName = NULL, BOOL bTextOut = FALSE );
	VOID	Serialize( BaseLogData& IN rData );
	BOOL	Open( TCHAR* pszFileName);
    BOOL	Open( TCHAR* pszFileName, fpos_t& end_offset);
	BOOL	SetLogFileOffset(fpos_t Pos);
	fpos_t	GetLogFileOffset();
	BOOL	Close();
#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
    BaseLogData* Read(bool autoSkip = false);
#else
	BaseLogData* Read();
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
	SolarLog* GetSolarLog() { return m_pSolarLog; }
	SolarLog* GetSolarText() { return m_pSolarLogText; }
	BOOL GetTextOut() { return m_bTextOut; }
#ifdef __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
private:
    // 파일 스트림의 다음 sampleLength 길이 내에 레코드 시작이 있는지 검색한다.
    bool SearchForNextRecord(size_t sampleLength, int* offset);
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
};

//---------------------------------------------------------------------------------------------------------------------

#endif //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------
