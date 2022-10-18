#pragma once

#include "BaseLogData.h"
#include "LogArchive.h"

class SolarLog;

//---------------------------------------------------------------------------------------------------------------------

#ifdef _NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------

// ������ ���� �����͸� �ڵ�� ��ü�Ѵ�.
// lseek�� ������� 2GByte�̻��� ������ �׼����� �����ϵ��� �����Ѵ�.
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
    BYTE*       m_pSpoolBuffer;                     // ��Ǯ ���� ������
    static const size_t SPOOL_BUFFER_SIZE = 102400; // ��Ǯ ���� ũ�� (100KB)
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
    // ���� ��Ʈ���� ���� sampleLength ���� ���� ���ڵ� ������ �ִ��� �˻��Ѵ�.
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
    BYTE*       m_pSpoolBuffer;                     // ��Ǯ ���� ������
    static const size_t SPOOL_BUFFER_SIZE = 102400; // ��Ǯ ���� ũ�� (100KB)
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
    // ���� ��Ʈ���� ���� sampleLength ���� ���� ���ڵ� ������ �ִ��� �˻��Ѵ�.
    bool SearchForNextRecord(size_t sampleLength, int* offset);
#endif // __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY
};

//---------------------------------------------------------------------------------------------------------------------

#endif //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------
