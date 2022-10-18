#include "stdafx.h"
#include ".\mapallocator.h"

#if(0)

#include "Map.h"
#include "MapInfoLoader.h"
#include "FieldInfo.h"
#include "GameField.h"
#include "Sector.h"
//#include "SectorGroup.h"

MapAllocator g_MapAllocator;

MapAllocator::MapAllocator()
{
	m_pMapPool			= new CMemoryPoolFactory<Map>;
	m_pSectorPool		= new CMemoryPoolFactory<Sector>;
	//m_pSectorGroupPool	= new CMemoryPoolFactory<SectorGroup>;
	m_pMapInfoLoader	= new MapInfoLoader;

	m_pFieldPool		= new CMemoryPoolFactory<GameField>;
}

MapAllocator::~MapAllocator()
{
	SAFE_DELETE( m_pMapInfoLoader );
	SAFE_DELETE( m_pSectorPool );
	SAFE_DELETE( m_pFieldPool );
	//SAFE_DELETE( m_pSectorGroupPool );
	SAFE_DELETE( m_pMapPool );
}
VOID MapAllocator::Init( DWORD MapPoolSize, DWORD SectorPoolSize, WORD wVillageSectorSize, WORD wRoomSectorSize )
{
	m_pMapPool->Initialize( MapPoolSize, MapPoolSize/2+1 );
	m_pFieldPool->Initialize( MapPoolSize, MapPoolSize/2+1 );
	m_pSectorPool->Initialize( SectorPoolSize, SectorPoolSize/2+1 );
	//m_pSectorGroupPool->Initialize( SectorPoolSize/25, SectorPoolSize/50+1 );

	m_pMapInfoLoader->EstablishInfo();
	Map::SetPool( m_pMapPool );
	GameField::SetPool( m_pFieldPool );
	Sector::SetPool( m_pSectorPool );
	//SectorGroup::SetPool( m_pSectorGroupPool );
}

VOID MapAllocator::Release()
{
	m_pMapInfoLoader->UnloadInfo();
	m_pSectorPool->Release();
//	m_pSectorGroupPool->Release();
	m_pFieldPool->Release();
	m_pMapPool->Release();

	Map::SetPool( NULL );
	GameField::SetPool( NULL );
	Sector::SetPool( NULL );
	//SectorGroup::SetPool( NULL );
}

Map * MapAllocator::AllocMap( MAPCODE MapCode, BOOL bUsingViewPort, BOOL bUsingPreLoading )
{
	MapInfo * pMapInfo = m_pMapInfoLoader->GetMapInfo( MapCode, bUsingViewPort );
	SASSERT( pMapInfo != NULL, VSTR("[%d]¹ø¸ÊÀÌ ¾ø´Ù.", MapCode) );
	if( !pMapInfo )		return NULL;

	Map * pNewMap = Map::ALLOC();
	pNewMap->Init( pMapInfo, bUsingPreLoading );

	return pNewMap;
}

VOID MapAllocator::FreeMap( Map * pMap )
{
	pMap->Release();
	Map::FREE( pMap );
}

#endif

