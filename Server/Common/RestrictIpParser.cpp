#include "StdAfx.h"
#include ".\restrictipparser.h"
#include <string>
#include <io.h>


RestrictIpParser::RestrictIpParser(void)
{
	m_dwStartAllowIP = 0;
	m_dwEndAllowIP = 0;
	m_byUseRestrictIP = 0;
	m_byBasicSts = 0;
}

RestrictIpParser::~RestrictIpParser(void)
{
}

VOID		RestrictIpParser::Release()
{

}

BOOL		RestrictIpParser::Load( TCHAR* pszFileName, BOOL bReload )
{
	if( !bReload ) strncpy( m_pszFileName, pszFileName, MAX_PATH );

	if( _access( pszFileName, 0 ) != -1 )	//파일존재
		return _Load( pszFileName );	

	return TRUE; 
}



BOOL		RestrictIpParser::_Load( const TCHAR* pszFileName )
{
	m_byUseRestrictIP = GetPrivateProfileInt( "COMMON", "USE_RESTRICT_IP", 0, pszFileName );
	m_byBasicSts = GetPrivateProfileInt( "COMMON", "BASIC_STS", 0, pszFileName );

	TCHAR szStartIP[MAX_PATH];
	TCHAR szEndIP[MAX_PATH];
	GetPrivateProfileString( "ALLOW_IP", "START_IP", "", szStartIP, MAX_PATH, pszFileName );
	GetPrivateProfileString( "ALLOW_IP", "END_IP", "", szEndIP, MAX_PATH, pszFileName );

	m_dwStartAllowIP = ConvertIP( szStartIP );
	m_dwEndAllowIP = ConvertIP( szEndIP );

	return TRUE;
}


BOOL		RestrictIpParser::CheckRestrictIP( const TCHAR* pszIP )
{
	DWORD dwCurIP = ConvertIP( pszIP );
	if( dwCurIP == 0 )	return FALSE;

	if( dwCurIP >= m_dwStartAllowIP && dwCurIP <= m_dwEndAllowIP )
		return TRUE;

	return FALSE;
}


DWORD		RestrictIpParser::ConvertIP( const TCHAR* pszIP )
{
	if( !pszIP || (strcmp( pszIP, "") == 0) )		return 0;

	int nCnt = 0;
	BYTE byIP[4];
	std::string strIP = pszIP;
	while( nCnt < 4 )
	{
		int nIdx = (int)strIP.find( '.' );
		if( nIdx == std::string::npos )
		{
			if( strIP.size() > 0 )
				byIP[nCnt] = atoi( strIP.c_str() );					

			break;
		}

		std::string strTemp = strIP.substr( 0, nIdx );
		byIP[nCnt] = atoi( strTemp.c_str() );
		strIP.erase( 0, strTemp.size() + 1 );
		nCnt++;
	}	

	DWORD dwKey = ( (DWORD)byIP[0] << 24 | (DWORD)byIP[1] << 16 | (DWORD)byIP[2] << 8 | (DWORD)byIP[3] );

	return dwKey;
}

#ifdef __NA_000252_20070208_VERSION_GMCMD
BOOL		RestrictIpParser::ILoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize )
{
	return FALSE;
}

BOOL		RestrictIpParser::_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload )
{
	return FALSE;
}

#else

BOOL		RestrictIpParser::ILoad( SCRIPT_CODE_T scriptCode, BYTE* pSTREAM, DWORD dwSize )
{
	return FALSE;
}

#endif