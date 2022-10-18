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
    // ���� ���̳ʸ� �α� �׸� ���� ���� ���ڼ� ������ ���� ���� ��Ǯ�� ���� �߰�
    // �ؽ�Ʈ �α״� ��Ǯ������ ����
    bool        m_bSpooling;        // ���� ��Ǯ�� ���ΰ��� ǥ���ϴ� Boolean �÷���
    BYTE*       m_pSpoolBuffer;     // ���̳ʸ� �α� ��Ǯ ���� ������. �Ҵ�/������ �ܺ��� å��
    size_t      m_uSpoolBufferSize; // ��Ǯ ���� ũ��
    size_t      m_uSpooledLength;   // ��Ǯ ���� ���� ����
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
    // ��Ǯ ���� �ʱ�ȭ
    void InitSpooling(void* spoolBuffer, size_t spoolBufferSize);
    // ��Ǯ���� �غ��Ѵ�. �̹� ��Ǯ�� ���̸� Flush() �� ���� �����Ѵ�.
    void BeginSpooling();
    // ��Ǯ���� ������ ������ ����
    bool Flush();
private:
    // ��Ǯ�� ������ �߰��Ѵ�
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
    // ���� ���̳ʸ� �α� �׸� ���� ���� ���ڼ� ������ ���� ���� ��Ǯ�� ���� �߰�
    // �ؽ�Ʈ �α״� ��Ǯ������ ����
    bool        m_bSpooling;        // ���� ��Ǯ�� ���ΰ��� ǥ���ϴ� Boolean �÷���
    BYTE*       m_pSpoolBuffer;     // ���̳ʸ� �α� ��Ǯ ���� ������. �Ҵ�/������ �ܺ��� å��
    size_t      m_uSpoolBufferSize; // ��Ǯ ���� ũ��
    size_t      m_uSpooledLength;   // ��Ǯ ���� ���� ����
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
    // ��Ǯ ���� �ʱ�ȭ
    void InitSpooling(void* spoolBuffer, size_t spoolBufferSize);
    // ��Ǯ���� �غ��Ѵ�. �̹� ��Ǯ�� ���̸� Flush() �� ���� �����Ѵ�.
    void BeginSpooling();
    // ��Ǯ���� ������ ������ ����
    bool Flush();
private:
    // ��Ǯ�� ������ �߰��Ѵ�
    bool Spool(const void* buffer, size_t length);
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
};

//---------------------------------------------------------------------------------------------------------------------

#endif //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------
