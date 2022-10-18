#include "stdafx.h"
#include "MapPartialLoadingParser.h"


#include <SolarFileReader.h>
#include <iostream>


MapPartialLoadingParser::MapPartialLoadingParser()
{
}

MapPartialLoadingParser::~MapPartialLoadingParser()
{
}

MAP_PARTIAL_LOADING	* MapPartialLoadingParser::FindMapInfo( MAPCODE MapKind )
{
	MAP_PARTIAL_LOADING_MAP_ITER it = m_mapPartialLoadingInfo.find( MapKind );
	if( it != m_mapPartialLoadingInfo.end() )
		return &(it->second);

	return NULL;
}

VOID MapPartialLoadingParser::Reload()
{
	m_mapPartialLoadingInfo.clear();
//	Load( m_pszFileName, TRUE );
}

BOOL	MapPartialLoadingParser::_Load( BOOL bReload )
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	const BYTE full_loading		= GetINIDataBYTE( "OPTION", "LOADING_TYPE" );
	const DWORD loading_count	= GetINIDataDWORD( "OPTION", "PARTIAL_LOADING_COUNT" );

	TCHAR mapKindString[32] = { 0, };
	TCHAR mapLoadString[32] = { 0, };
	const INT mapKind_tail_idx = _sntprintf( mapKindString, _countof(mapKindString), "MAPKIND_" );
	const INT mapLoad_tail_idx = _sntprintf( mapLoadString, _countof(mapLoadString), "MAPLOAD_" );
	for( DWORD count=0 ; count<loading_count ; ++count )
	{
		MAP_PARTIAL_LOADING loadingInfo;
		ZeroMemory( &loadingInfo, sizeof(loadingInfo) );

		const BYTE index = (BYTE)count;
		{	// make format
			const BYTE carry	= index/10;
			const BYTE remain	= index%10;
			mapKindString[mapKind_tail_idx]		= '0' + carry;
			mapLoadString[mapLoad_tail_idx]		= '0' + carry;

			mapKindString[mapKind_tail_idx+1]	= '0' + remain;
			mapLoadString[mapLoad_tail_idx+1]	= '0' + remain;
		};

		loadingInfo.m_MapKind = GetINIDataWORD( "PARTIAL_LOADING_INFO", mapKindString );
		if( FindMapInfo( loadingInfo.m_MapKind ) )
		{
			if( FALSE == bReload )
			{
                SUNLOG(eCRITICAL_LOG, "Script File Data Error, File = %s, ID = %d",
                       mapKindString, loadingInfo.m_MapKind );
				FASSERT( !"데이터에 오류가 있습니다." );
			}
		}

		loadingInfo.m_bLoading = full_loading
			?	TRUE
			:	GetINIDataBYTE( "PARTIAL_LOADING_INFO", mapLoadString )
			;

		m_mapPartialLoadingInfo.insert( MAP_PARTIAL_LOADING_PAIR(loadingInfo.m_MapKind, loadingInfo) );
	}

	return TRUE;
}


BOOL	MapPartialLoadingParser::LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload )
{
	switch( scriptCode )
	{
	case SCRIPT_MAP_PARTIAL_LOADING:
		return _Load( bReload );
	}

	return FALSE; 
}

