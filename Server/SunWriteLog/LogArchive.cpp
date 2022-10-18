#include "stdafx.h"
#include ".\logarchive.h"
//#include "afxcom_.h"
#include "SolarLog.h"



//---------------------------------------------------------------------------------------------------------------------

#ifdef _NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------


CLogArchive::~CLogArchive(VOID)
{
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        if (m_bSpooling) {
            Flush();
        }
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
}

void CLogArchive::Write(const void * pBuffer, int size)
{
    if( !pBuffer || size < 0 )
        return;

    if(m_pSolarLog == NULL)
    {
        ASSERT( FALSE );
        return;
    }

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if ( m_bSpooling && m_pSpoolBuffer != NULL ) {
        Spool(pBuffer, size);
        return;
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

    m_pSolarLog->LOG( (BYTE*)pBuffer, size );
}

void CLogArchive::WriteText( TCHAR* pBuffer, DWORD dwSize )
{
    if( !pBuffer || dwSize <= 0 )
        return;

    if( m_pSolarLogText ==  NULL )
    {
        ASSERT(FALSE);
        return;
    }

    DWORD dwLength = (DWORD)_tcslen( pBuffer );
    if( dwLength >= dwSize )
    {
        pBuffer[dwSize-1] = '\0';
        dwLength = dwSize;
    }

    m_pSolarLogText->LOG( (BYTE*)pBuffer, dwLength );
}

void CLogArchive::Read(void* pBuffer, DWORD dwSize)
{
    if (file_handle_ == Null_File)
    {
        ASSERT(FALSE);
        return;
    }
    _read(file_handle_, pBuffer, dwSize);
}

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
void CLogArchive::InitSpooling(void* spoolBuffer, size_t spoolBufferSize)
{
    if ( NULL == spoolBuffer || 0 == spoolBufferSize ) {
        return;
    }
    m_pSpoolBuffer = (BYTE*)spoolBuffer;
    m_uSpoolBufferSize = spoolBufferSize;
}

void CLogArchive::BeginSpooling()
{
    if (NULL == m_pSpoolBuffer) {
        return;
    }
    if (m_bSpooling) {
        Flush();
    }
    m_uSpooledLength = 0;
    m_bSpooling = true;
}

bool CLogArchive::Flush()
{
    if (NULL == m_pSpoolBuffer) {
        return false;
    }
    if (!m_bSpooling) {
        return true;
    }

    BOOL result = m_pSolarLog->LOG(m_pSpoolBuffer, (DWORD)m_uSpooledLength);

    if (FALSE == result) {
        // 거의 SolarLog 내부 버퍼 오버플로우의 경우일 것이다
        return false;
    }

    m_bSpooling = false;

    return true;
}

bool CLogArchive::Spool(const void* buffer, size_t length)
{
    if ( !m_bSpooling || NULL == m_pSpoolBuffer ) {
        return false; // Not supported
    }
    if ( length > (m_uSpoolBufferSize - m_uSpooledLength) ) {
        return false; // Spooling buffer overflow
    }
    ::memcpy(m_pSpoolBuffer + m_uSpooledLength, buffer, length);
    m_uSpooledLength += length;
    return true;
}
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT


//---------------------------------------------------------------------------------------------------------------------

#else //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------


CLogArchive::~CLogArchive(VOID)
{
#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if (m_pSpoolBuffer != NULL) {
        if (m_bSpooling) {
            Flush();
        }
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
}

void CLogArchive::Write(const void * pBuffer, int size)
{
	if( !pBuffer || size < 0 )
		return;

	if(m_pSolarLog == NULL)
	{
		ASSERT( FALSE );
		return;
	}

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
    if ( m_bSpooling && m_pSpoolBuffer != NULL ) {
        Spool(pBuffer, size);
        return;
    }
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

    m_pSolarLog->LOG( (BYTE*)pBuffer, size );
}

void CLogArchive::WriteText( TCHAR* pBuffer, DWORD dwSize )
{
	if( !pBuffer || dwSize <= 0 )
		return;

	if( m_pSolarLogText ==  NULL )
	{
		ASSERT(FALSE);
		return;
	}

	DWORD dwLength = (DWORD)_tcslen( pBuffer );
	if( dwLength >= dwSize )
	{
		pBuffer[dwSize-1] = '\0';
		dwLength = dwSize;
	}

	m_pSolarLogText->LOG( (BYTE*)pBuffer, dwLength );
}

void CLogArchive::Read(void* pBuffer, DWORD dwSize)
{
	if( m_pReadStream == NULL )
	{
		ASSERT( FALSE );
		return;
	}

	fread( pBuffer, dwSize, 1, m_pReadStream );
}

#ifdef __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT
void CLogArchive::InitSpooling(void* spoolBuffer, size_t spoolBufferSize)
{
    if ( NULL == spoolBuffer || 0 == spoolBufferSize ) {
        return;
    }
    m_pSpoolBuffer = (BYTE*)spoolBuffer;
    m_uSpoolBufferSize = spoolBufferSize;
}

void CLogArchive::BeginSpooling()
{
    if (NULL == m_pSpoolBuffer) {
        return;
    }
    if (m_bSpooling) {
        Flush();
    }
    m_uSpooledLength = 0;
    m_bSpooling = true;
}

bool CLogArchive::Flush()
{
    if (NULL == m_pSpoolBuffer) {
        return false;
    }
    if (!m_bSpooling) {
        return true;
    }

    BOOL result = m_pSolarLog->LOG(m_pSpoolBuffer, (DWORD)m_uSpooledLength);

    if (FALSE == result) {
        // 거의 SolarLog 내부 버퍼 오버플로우의 경우일 것이다
        return false;
    }

    m_bSpooling = false;

    return true;
}

bool CLogArchive::Spool(const void* buffer, size_t length)
{
    if ( !m_bSpooling || NULL == m_pSpoolBuffer ) {
        return false; // Not supported
    }
    if ( length > (m_uSpoolBufferSize - m_uSpooledLength) ) {
        return false; // Spooling buffer overflow
    }
    ::memcpy(m_pSpoolBuffer + m_uSpooledLength, buffer, length);
    m_uSpooledLength += length;
    return true;
}
#endif // __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT

//---------------------------------------------------------------------------------------------------------------------

#endif //_NA_0_20110305_BUG_READ_BIG_LOGFILE

//---------------------------------------------------------------------------------------------------------------------
