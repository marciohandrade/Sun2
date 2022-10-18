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
//	// 128�϶� ��ϸ���
//	// 128�� '\0' ���� ����
//	if( MAX_ADDINFO_SIZE-1 < m_nAddInfoCount ) return;
//
//	_sntprintf( &m_szAddInfo[m_nAddInfoCount], (MAX_ADDINFO_SIZE-m_nAddInfoCount), tszAddInfo );
//	m_szAddInfo[MAX_ADDINFO_SIZE]='\0';
//	m_nAddInfoCount += lstrlen(&m_szAddInfo[m_nAddInfoCount]);
//}

// pszLogFilePrefix : ���̳ʸ� ������ �α׸� ������ �����̸�(Ȯ���� ����)
// pszBinDirName : ���̳ʸ� ������ �α׸� ������ ���丮 ���
// pszTextDirName : �ؽ�Ʈ ������ �α׸� ������ ���丮 ���
BOOL BaseLog::Init( BYTE byLogOption, TCHAR* pszLogFilePrefix, TCHAR* pszBinDirName, TCHAR* pszTextDirName )
{
	if( byLogOption == 0 )	//�α� ������ ����.
	{
		m_bRun = FALSE;
		return TRUE;
	}

	if( byLogOption == 1 )		// ���̳ʸ� ���·θ� ���
		m_bWriteText = FALSE;
	else if( byLogOption == 2 )  // ���̳ʸ��� �ؽ�Ʈ ���� �Ѵ� ���
		m_bWriteText = TRUE;

	//Init���� �����ϸ� �αװ� ����� �۵����� �����Ƿ�, m_bRun�� FALSE�� �ϱ�� �Ѵ�.
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
