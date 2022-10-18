#include "StdAfx.h"
#include "GMList.h"
#include <SolarFileReader.h>


GMList::GMList()
{
}

GMList::~GMList()
{
}

VOID GMList::Release()
{
	GM_INFO *pInfo = NULL ;

	GM_LIST_MAP_ITER iter;
	for( iter = m_mapGMList.begin(); iter != m_mapGMList.end(); ++iter )
	{
		pInfo = iter->second;
		SAFE_DELETE( pInfo );
	}
}

BOOL GMList::AddGM( GM_INFO* pInfo )
{
	GM_LIST_MAP_ITER iter;
	iter = m_mapGMList.find( pInfo->m_szID );
	if( iter != m_mapGMList.end() )	//존재하면..
	{
		return FALSE;
	}
	else
	{
		GM_INFO* pGMInfo = new GM_INFO;
		memcpy( pGMInfo, pInfo, sizeof(GM_INFO) );
		m_mapGMList.insert( std::make_pair( pInfo->m_szID, pGMInfo ) );
	}

	return TRUE;
}


#pragma warning ( push )
#pragma warning ( disable : 4244)
VOID GMList::Reload()
{
	Load( m_pszGMListName, TRUE );
}


BOOL GMList::Load( char * pszGMListName, BOOL bReload )
{
	if( !bReload ) 
		strncpy( m_pszGMListName, pszGMListName, MAX_PATH );

	BOOL bRet = FALSE;

	// 파일열기
	util::SolarFileReader sr;
	if ( !sr.OpenFile( pszGMListName, SEPERATOR_WHITESPACE ) )
	{
		SUNLOG( eCRITICAL_LOG, "Script File Load Fail, File = %s",  pszGMListName );		
		ASSERT( !"File Open Error : GMList.txt" );
		return (FALSE);
	}
	else
		SUNLOG( eDEV_LOG, "Script File Load Start, File = %s",  pszGMListName );		

	bRet = _LoadGMList( sr, bReload );
	if( bRet )
		SUNLOG( eDEV_LOG, "Script File Load Complete, File = %s",  pszGMListName );		

	sr.CloseFile();

	return bRet;
}


BOOL	GMList::_LoadGMList( util::SolarFileReader& sr, BOOL bReload )
{
	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType(); 
		if ( TOKEN_END == ettType )
		{
			break;
		}
		else if ( TOKEN_NUMBER != ettType )
		{
			ASSERT(!"잘못된 스크립트");
			return FALSE;
		}

		GM_INFO* pInfo = new GM_INFO;
		pInfo->m_wIndex = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_STRING);	strncpy( pInfo->m_szID, _strupr(sr.GetTokenString()), ID_MAX_LEN );
		sr.GetNextTokenType(TOKEN_STRING);	strncpy( pInfo->m_szPW, sr.GetTokenString(), PASSWD_MAX_LEN );
		sr.GetNextTokenType(TOKEN_ANYTYPE);	strncpy( pInfo->m_szIP, sr.GetTokenString(), IP_MAX_LEN );
		sr.GetNextTokenType(TOKEN_NUMBER);	pInfo->m_byGrade = sr.GetTokenNumber();

#ifndef __NA_20070102_GMIP_CHECK 
		sr.GetNextTokenType(TOKEN_STRING);	strncpy( pInfo->m_szDepartment, sr.GetTokenString(), MAX_TITLEID_LENGTH );
		sr.GetNextTokenType(TOKEN_STRING);	strncpy( pInfo->m_szName, sr.GetTokenString(), MAX_CHARNAME_LENGTH );
		sr.GetNextTokenType(TOKEN_STRING);	strncpy( pInfo->m_szRole, sr.GetTokenString(), MAX_TITLEID_LENGTH );

		// 날짜 변환
		/*		TCHAR szDate[MAX_DATE_LENGTH];
		sr.GetNextTokenType(TOKEN_STRING); strncpy( szDate, sr.GetTokenString(), MAX_DATE_LENGTH );
		pInfo->m_eExpireDate.m_wYear;
		pInfo->m_eExpireDate.m_wMonth;
		pInfo->m_eExpireDate.m_wDay;*/
#endif
		m_mapGMList.insert( std::make_pair( pInfo->m_szID, pInfo ) );

	}

	sr.CloseFile();

	return TRUE;
}


GM_INFO* GMList::FindGM( const TCHAR* pszUserID )
{
	GM_LIST_MAP_ITER iter;

	iter = m_mapGMList.find( pszUserID );
	if( iter != m_mapGMList.end() )
		return iter->second;

	return NULL;
}


#ifdef __NA_20070102_GMIP_CHECK

VOID	GMList::SerializeGMListStream( GM_TOTAL_INFO& TotalInfo, BOOL bSave )
{
	GM_LIST_MAP_ITER iter;

	if( bSave )
	{
		for( BYTE byCount = 0; byCount < TotalInfo.m_Count; ++byCount )
		{
			GM_INFO* pGMInfo = &TotalInfo.m_Slot[byCount];
			if( !pGMInfo )	continue;

			m_mapGMList[pGMInfo->m_szID] = pGMInfo;
		}		
	}
	else
	{
		for( iter = m_mapGMList.begin(); iter != m_mapGMList.end(); iter++ )
		{
			GM_INFO* pInfo = NULL;
			pInfo = iter->second;
			if( !pInfo )		continue;

			memcpy( &TotalInfo.m_Slot[TotalInfo.m_Count], pInfo, sizeof(GM_INFO) );
			TotalInfo.m_Count++;
		}
	}
}

#endif


#pragma warning ( pop )

#ifdef __NA_000252_20070208_VERSION_GMCMD
BOOL	GMList::ILoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize )
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 파일 열기
	util::SolarFileReader sr;
	if ( !sr.OpenBuffer( pSTREAM, dwSize, SEPERATOR_WHITESPACE ) )
	{
		ASSERT( !"OpenBuffer" );
		return (FALSE);
	}

	switch( ScriptCode )
	{
	case SCRIPT_GMLIST:
		return _LoadGMList( sr, TRUE );
		case SCRIPT_CODE_KEY(SUPERUSERLIST):
			return _LoadSuperList( sr, TRUE );
	}

	ASSERT( !"Invalid Type :Script Code" );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	return FALSE;
}

BOOL	GMList::_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_GMLIST:
		return _LoadGMList( sr, bReload );
			case SCRIPT_CODE_KEY(SUPERUSERLIST):
				return _LoadSuperList( sr, TRUE );
	}

	return FALSE;
}

#else

BOOL	GMList::ILoad( SCRIPT_CODE_T scScriptCode, BYTE* pSTREAM, DWORD dwSize )
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 파일 열기
	util::SolarFileReader sr;
	if ( !sr.OpenBuffer( pSTREAM, dwSize, SEPERATOR_WHITESPACE ) )
	{
		ASSERT( !"OpenBuffer" );
		return (FALSE);
	}

	switch( scScriptCode )
	{
	case SCRIPT_CODE_KEY(GMLIST):
		return _LoadGMList( sr, TRUE );
	case SCRIPT_CODE_KEY(SUPERUSERLIST):
		return _LoadSuperList( sr, TRUE );
	}

	ASSERT( !"Invalid Type :Script Code" );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	return FALSE;
}

#endif


