//=============================================================================================================================
/// 스크립트를 파싱하여 데이터를 읽는 MapInfoLoader class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 11. 10
	@remarks
		- World.txt를 파싱한다. FieldInfo, MapInfo를 구성하여 hash에 넣는다.
	@history
		- 2006.2.20 ~ 2006.2.25 : 리펙토링, NoneViewport/SupportedViewPort를 가진 MapInfo를 가진다.
*/
//=============================================================================================================================


#pragma once

#if(0)

#include <SolarHashTable.h>
#include "MapDefine.h"

#define MAPINFO_KEY( c, b )		(DWORD)(((c&0xFFFF)<<16)|(WORD)(b&0xFFFF))

class MapInfoLoader
{
	enum 
	{ 
		MAX_MAPINFO_NUM		= 50,
		MAX_SHAREDFIELDINFO_NUM	= 100,
	};

public:
	MapInfoLoader();
	~MapInfoLoader();

	VOID				UnloadInfo();

	inline MapInfo *	GetMapInfo( MAPCODE MapCode, BOOL bUsingViewPort );
	VOID				EstablishInfo();

private:
	MapInfo *			_CreateMapInfo( sMAPINFO * _pMAPINFO_, BOOL bUsingViewPort );
	inline VOID			_AddMapInfo( MAPCODE MapCode, BOOL bUsingViewPort, MapInfo * pInfo );
	inline VOID			_AddSharedFieldInfo( MAPCODE FieldCode, SharedFieldInfo * pInfo );
	inline SharedFieldInfo *_GetSharedFieldInfo( MAPCODE FieldCode );

	MAPINFO_HASH *			m_pMapInfoHash;
	SHAREDFIELDINFO_HASH *	m_pFieldInfoHash;
	char					m_pszRootDir[MAX_PATH];
};

inline MapInfo * MapInfoLoader::GetMapInfo( MAPCODE MapCode, BOOL bUsingViewPort )
{
	return m_pMapInfoHash->GetData(MAPINFO_KEY(MapCode,(WORD)bUsingViewPort));
}
inline VOID	MapInfoLoader::_AddMapInfo( MAPCODE MapCode, BOOL bUsingViewPort, MapInfo * pInfo )
{
	DWORD key = MAPINFO_KEY(MapCode,(WORD)bUsingViewPort);
	ASSERT( m_pMapInfoHash->GetData( key ) == NULL );
	m_pMapInfoHash->Add( pInfo, key );	
}
inline SharedFieldInfo * MapInfoLoader::_GetSharedFieldInfo( MAPCODE FieldCode )
{
	return m_pFieldInfoHash->GetData( FieldCode );
}
inline VOID MapInfoLoader::_AddSharedFieldInfo( MAPCODE FieldCode, SharedFieldInfo * pInfo )
{
	ASSERT( m_pFieldInfoHash->GetData( FieldCode ) == NULL );
	m_pFieldInfoHash->Add( pInfo, FieldCode );
}

#endif
