#include "stdafx.h"
#include ".\baselog.h"
#include <LogTimeFormat.h>

#define __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__

	#include <new.h>
	static VOID		ErrorHandler_new()
	{
		printf( "Can't Alloc Memory\n" );
		__debugbreak();
	}

	#include <signal.h>
	VOID		signal_redirect( INT signo )
	{
        __UNUSED(signo);
		__debugbreak();
	}

#endif

BaseLog::BaseLog()
{
	m_bRun = FALSE;
	m_bWriteText = FALSE;

#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
	set_new_handler( ErrorHandler_new );
	signal( SIGABRT, signal_redirect );
	_set_error_mode( _OUT_TO_STDERR );
#endif
}

//
//VOID BaseLog::AddAddInfo( TCHAR * tszAddInfo )
//{
//	// 128일때 기록못함
//	// 128에 '\0' 들어가기 때문
//	if( MAX_ADDINFO_SIZE-1 < m_nAddInfoCount ) return;
//
//	_sntprintf( &m_szAddInfo[m_nAddInfoCount], (MAX_ADDINFO_SIZE-m_nAddInfoCount), tszAddInfo );
//	m_szAddInfo[MAX_ADDINFO_SIZE]='\0';
//	m_nAddInfoCount += lstrlen(&m_szAddInfo[m_nAddInfoCount]);
//}

// pszLogFilePrefix : 바이너리 형태의 로그를 저장할 파일이름(확장자 제외)
// pszBinDirName : 바이너리 형태의 로그를 저장할 디렉토리 경로
// pszTextDirName : 텍스트 형태의 로그를 저장할 디렉토리 경로
BOOL BaseLog::Init( BYTE byLogOption, TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName )
{
	if( byLogOption == 0 )	//로그 남기지 않음.
	{
		m_bRun = FALSE;
		return TRUE;
	}

	if( byLogOption == 1 )		// 바이너리 형태로만 출력
		m_bWriteText = FALSE;
	else if( byLogOption == 2 )  // 바이너리와 텍스트 형태 둘다 출력
		m_bWriteText = TRUE;

	//Init에서 실패하면 로그가 제대로 작동하지 않으므로, m_bRun를 FALSE로 하기로 한다.
	m_bRun = m_LogMng.Init( pszLogFilePrefix, pszBinDirName, pszTextDirName, m_bWriteText );

	return m_bRun;
}

//
//VOID BaseLog::WriteBaseData( BaseLogData* pData, const int nLogType, const TCHAR* szAccountID, const TCHAR* szCharName, const TCHAR* szUserIP )
//{
//	pData->SetLogType( nLogType );
//
//	if( szAccountID )
//		pData->SetAccountID( szAccountID );
//
//	if( szCharName )
//		pData->SetCharName( szCharName );
//
//	if( szUserIP )
//		pData->SetUserIP( szUserIP );
//}
//
