#pragma once

#include "LogCommonDefine.h"

class SolarLog;


//---------------------------------------------------------------------------------------------------------------------

#ifdef _NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------

class DLL_DECLARE CLogArchive
{
    enum
    {
        Null_File = -1,
    };

	SolarLog*	m_pSolarLog;	
	SolarLog*	m_pSolarLogText;
	int		    file_handle_;
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    // 단일 바이너리 로그 항목에 대한 쓰기 원자성 보장을 위해 쓰기 스풀링 지원 추가
    // 텍스트 로그는 스풀링하지 않음
    bool        m_bSpooling;        // 현재 스풀링 중인가를 표현하는 Boolean 플래그
    BYTE*       m_pSpoolBuffer;     // 바이너리 로그 스풀 버퍼 포인터. 할당/해제는 외부의 책임
    size_t      m_uSpoolBufferSize; // 스풀 버퍼 크기
    size_t      m_uSpooledLength;   // 스풀 버퍼 내용 길이
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

public:
	CLogArchive(SolarLog* pSolarLog, SolarLog* pSolarLogText)
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
      : m_bSpooling(false)
      , m_pSpoolBuffer(NULL)
      , m_uSpoolBufferSize(0)
      , m_uSpooledLength(0)
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
	{
		m_pSolarLog		= pSolarLog;
		m_pSolarLogText = pSolarLogText;
	}
	CLogArchive(int file_handle)
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
      : m_bSpooling(false)
      , m_pSpoolBuffer(NULL)
      , m_uSpoolBufferSize(0)
      , m_uSpooledLength(0)
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
	{
		file_handle_ = file_handle;
	}
	~CLogArchive(VOID);
	
	template <class T>
		CLogArchive& operator << ( const T & type)
	{
		Write( &type, sizeof(T) );
		return (*this);
	}

	template <class T>
		CLogArchive& operator >> ( T & type)
	{
		Read( &type, sizeof(T));
		return (*this);
	}

	__inline void Write(const void * pBuffer, int size);
	__inline void WriteText(TCHAR* pBuffer, DWORD dwSize);
	__inline void Read(void* pBuffer, DWORD dwSize);

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
public:
    // 스풀 버퍼 초기화
    void InitSpooling(void* spoolBuffer, size_t spoolBufferSize);
    // 스풀링을 준비한다. 이미 스풀링 중이면 Flush() 후 새로 시작한다.
    void BeginSpooling();
    // 스풀링된 내용을 실제로 쓴다
    bool Flush();
private:
    // 스풀링 내용을 추가한다
    bool Spool(const void* buffer, size_t length);
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
};


//---------------------------------------------------------------------------------------------------------------------

#else //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------

class DLL_DECLARE CLogArchive
{
    SolarLog*	m_pSolarLog;	
    SolarLog*	m_pSolarLogText;
    FILE*		m_pReadStream;
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    // 단일 바이너리 로그 항목에 대한 쓰기 원자성 보장을 위해 쓰기 스풀링 지원 추가
    // 텍스트 로그는 스풀링하지 않음
    bool        m_bSpooling;        // 현재 스풀링 중인가를 표현하는 Boolean 플래그
    BYTE*       m_pSpoolBuffer;     // 바이너리 로그 스풀 버퍼 포인터. 할당/해제는 외부의 책임
    size_t      m_uSpoolBufferSize; // 스풀 버퍼 크기
    size_t      m_uSpooledLength;   // 스풀 버퍼 내용 길이
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

public:
    CLogArchive(SolarLog* pSolarLog, SolarLog* pSolarLogText)
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
        : m_bSpooling(false)
        , m_pSpoolBuffer(NULL)
        , m_uSpoolBufferSize(0)
        , m_uSpooledLength(0)
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    {
        m_pSolarLog		= pSolarLog;
        m_pSolarLogText = pSolarLogText;
    }
    CLogArchive(FILE* pStream)
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
        : m_bSpooling(false)
        , m_pSpoolBuffer(NULL)
        , m_uSpoolBufferSize(0)
        , m_uSpooledLength(0)
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    {
        m_pReadStream = pStream;
    }
    ~CLogArchive(VOID);

    template <class T>
        CLogArchive& operator << ( const T & type)
    {
        Write( &type, sizeof(T) );
        return (*this);
    }

    template <class T>
        CLogArchive& operator >> ( T & type)
    {
        Read( &type, sizeof(T));
        return (*this);
    }

    __inline void Write(const void * pBuffer, int size);
    __inline void WriteText(TCHAR* pBuffer, DWORD dwSize);
    __inline void Read(void* pBuffer, DWORD dwSize);

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
public:
    // 스풀 버퍼 초기화
    void InitSpooling(void* spoolBuffer, size_t spoolBufferSize);
    // 스풀링을 준비한다. 이미 스풀링 중이면 Flush() 후 새로 시작한다.
    void BeginSpooling();
    // 스풀링된 내용을 실제로 쓴다
    bool Flush();
private:
    // 스풀링 내용을 추가한다
    bool Spool(const void* buffer, size_t length);
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
};

//---------------------------------------------------------------------------------------------------------------------

#endif //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------
