#include "stdafx.h"
//#include <3DEngine/3DTerrain/WorldBase.h>
#include "FieldInfo.h"
//#include "WorldServer/WorldServer.h"
//#include <ProgramCommon/WzArchive.h>
//#include <Macro.h>
//#include <3DEngine/IncludeAll_NewWorld.h>
//#include <ProgramCommon/DiscAccess.h>

//#include <ProgramCommon/WzArchive.h>
//#include "ViewPortManager.h"
#include "WorldServer.h"

////////////////////////////////////////////////////////////////////////////////////
// MapInfo class 
////////////////////////////////////////////////////////////////////////////////////
MapInfo::MapInfo():	
m_pMapInfo ( NULL )
{
	m_FieldInfoHashTable.Initialize( _MAX_FIELD_IN_MAP_NUM );
}

MapInfo::~MapInfo()
{
	m_FieldInfoHashTable.RemoveAll();
}

VOID MapInfo::AddFieldInfo( FieldInfo * pFieldInfo )
{
	m_FieldInfoHashTable.Add( pFieldInfo, pFieldInfo->GetFieldCode() );
}
////////////////////////////////////////////////////////////////////////////////////
// FieldInfo class 
////////////////////////////////////////////////////////////////////////////////////
FieldInfo::FieldInfo():
m_pWorldBase ( NULL ),
m_dwTotalSectorNum ( 0 ),
m_ppSectorInfo( NULL ),
m_pFieldInfo ( NULL )
//m_pViewPortManager(NULL)
{
	m_pMapObjectInfoHashTable = new util::SolarHashTable<MAPOBJECT_INFO *>;
	m_pMapObjectInfoHashTable->Initialize( _MAX_MAPOBJECT_INFO_NUM );
}

FieldInfo::~FieldInfo()
{
	for( DWORD i = 0 ; i < m_dwTotalSectorNum ; ++i )
	{
		SAFE_DELETE( m_ppSectorInfo[i] );		
	}
	SAFE_DELETE_ARRAY( m_ppSectorInfo );

	Destroy();

	SAFE_DELETE( m_pMapObjectInfoHashTable );
}

VOID FieldInfo::Create( const sFIELDINFO * pInfoIn )	//, ViewPortManager *pViewPortManager )
{
	SetFieldInfo( pInfoIn );
	
	ASSERT(!m_pWorldBase);

//	m_pViewPortManager = pViewPortManager;
//	m_pWorldBase = (CWorldBase *)ViewPortManager::Instance()->m_fnContactTerrain( CT_NEW_WORLD, 0 );
	m_pWorldBase = (CWorldBase *)WorldServer::Instance()->m_fnContactTerrain( CT_NEW_WORLD, 0 );

	m_pWorldBase->Create();
}

INT FieldInfo::Init()
{
	return m_pWorldBase->x_pRoomManager->DuplicateRoomInfo();
}

VOID FieldInfo::Release( INT Index )
{
	BOOL rt = m_pWorldBase->x_pRoomManager->DestroyRoomInfo(Index);
	ASSERT( rt );
}

///< FieldInfo를 읽은 다음,  SectorSize에 따른 SectorInfo를 셋팅
VOID FieldInfo::EstablishSectorInfo( WORD wSectorSize, BOOL bViewport )
{
	if(wSectorSize == 0)
		ASSERT( !"섹터 인덱스가 0이다" );

	m_dwSectorSize = wSectorSize;
	m_bViewport = bViewport;

	BvAABB bdv = m_pWorldBase->GetTerrainSize();
	// 최대값이 음수일수도 있으므로 절대값으로 계산
	DWORD XSize = 0, YSize = 0;
	XSize = (DWORD)abs(bdv.m_wvMax.x - bdv.m_wvMin.x);
	YSize = (DWORD)abs(bdv.m_wvMax.y - bdv.m_wvMin.y);

	// 섹터 인덱스를 계산하기 위한 변수
	m_iShiftMinX = (int)(bdv.m_wvMin.x);
	m_iShiftMinY = (int)(bdv.m_wvMin.y);
	m_iShiftMaxX = (int)(bdv.m_wvMax.x);
	m_iShiftMaxY = (int)(bdv.m_wvMax.y);

	BYTE byMarginX = 0, byMarginY = 0;
	XSize % wSectorSize == 0 ? byMarginX = 0 : byMarginX = 1;
	YSize % wSectorSize == 0 ? byMarginY = 0 : byMarginY = 1;

	m_dwSectorXNum				= XSize / wSectorSize + byMarginX;
	m_dwSectorYNum				= YSize / wSectorSize + byMarginY;
	m_dwTotalSectorNum			= m_dwSectorXNum * m_dwSectorYNum;
	m_ppSectorInfo				= new SectorInfo*[ m_dwSectorXNum * m_dwSectorYNum ];
	memset( m_ppSectorInfo, 0, sizeof(SectorInfo *) * m_dwSectorXNum * m_dwSectorYNum );

    /// 현재, 모든 Sector가 존재한다고 가정
	DWORD dwSectorIndex = 0;
	for( dwSectorIndex = 0 ; dwSectorIndex < m_dwTotalSectorNum ; ++dwSectorIndex )
	{
		m_ppSectorInfo[ dwSectorIndex ] = new SectorInfo( dwSectorIndex );
	}

	/// 이웃하는 sector를 연결
	for( dwSectorIndex = 0 ; dwSectorIndex < m_dwTotalSectorNum ; ++dwSectorIndex )
	{
		// 해당 섹터의 행과 열을 구한다.
		DWORD dwRow = dwSectorIndex / m_dwSectorXNum;		// Index_Y
		DWORD dwColumn = dwSectorIndex % m_dwSectorXNum;	// Index_X

		for( int i = -1; i <= 1; ++i )
		{
			int iNeighborColumn = dwColumn + i;
			if( iNeighborColumn < 0 || iNeighborColumn >= (int)m_dwSectorXNum )	continue;

			for( int j = -1; j <= 1; ++j )
			{
				int iNeighborRow = dwRow + j;
				if( iNeighborRow < 0 || iNeighborRow >= (int)m_dwSectorYNum )	continue;

				int index = iNeighborRow * m_dwSectorXNum + iNeighborColumn;

				if( dwSectorIndex == index )	continue;
				if( !m_ppSectorInfo[index] )	continue;

				m_ppSectorInfo[dwSectorIndex]->AddNeighbor( m_ppSectorInfo[index] );
			}
		}
	}
}

BOOL FieldInfo::IsValidCoordinates( WzVector* pwzVec )
{
	// X,Y좌표상에서 최소값보다 작은 값을 클라이언트에서 보낸 경우
	if( ( m_iShiftMinX > pwzVec->x ) || ( m_iShiftMinY > pwzVec->y ) )
	{
		return FALSE;
	}

	if( ( m_iShiftMaxX < pwzVec->x ) || ( m_iShiftMaxY < pwzVec->y ) )
	{
		return FALSE;
	}

	return TRUE;
}
// Client에서 넘어온 좌표를 가지고 해당하는 인덱스를 얻어옴
DWORD FieldInfo::GetSectorIndexFor( WzVector * pwzVec ) 
{
	DWORD dwXIndex = 0;
	DWORD dwYIndex = 0;

	if( IsValidCoordinates( pwzVec ) )
	{
		GetSectorIndexFor( pwzVec, dwXIndex, dwYIndex );
	}
	ASSERT( dwXIndex + m_dwSectorXNum * dwYIndex < m_dwTotalSectorNum );
	return ( dwXIndex + m_dwSectorXNum * dwYIndex );
}

VOID FieldInfo::GetSectorIndexFor( WzVector * pwzVec, DWORD & dwXIndexOut, DWORD & dwYIndexOut )
{
	dwXIndexOut = (DWORD)( pwzVec->x - m_iShiftMinX ) / GetSectorSize();
	dwYIndexOut = (DWORD)( pwzVec->y - m_iShiftMinY ) / GetSectorSize();
}


VOID FieldInfo::Destroy()
{
	_unloadMapObjectInfo();

	if( m_pWorldBase )
	{
		m_pWorldBase->Destroy( NULL );
//		ViewPortManager::Instance()->m_fnContactTerrain( CT_DELETE_WORLD, m_pWorldBase );
		WorldServer::Instance()->m_fnContactTerrain( CT_DELETE_WORLD, m_pWorldBase );
		m_pWorldBase = NULL;
	}
}
#include <io.h>

BOOL FieldInfo::Load()
{
	CWzArchive archive;

	CDiscAccess cda;
	cda.Create();
	//m_pWorldBase->SetDiscAccess(&cda);

	ASSERT(_access(m_pFieldInfo->pszPath,0)!=-1);

	if( archive.LoadFile( const_cast<char *>(m_pFieldInfo->pszPath) ) == FALSE )
	{
		DISPMSG( "Failed LoadFile() : From Fieldxx.map" );
		archive.Close();
		return FALSE;
	}
	
	if( m_pWorldBase->Serialize( &archive ) == FALSE )
	{
		archive.Close();
		return FALSE;
	}

	//_loadMapObjectInfo( archive );

	//_loadAreaInfo( archive );

	archive.Close();

	SetStartingTileIndex();

	return TRUE;
}
VOID FieldInfo::_loadMapObjectInfo( CWzArchive & archive )
{
	BOOL bAreaChunk = FALSE;
	while( TRUE )
	{
		St_ChunkInfo ci = archive.ReadChunk();
		if( ci.m_nID == 0x1781 )
		{
			bAreaChunk = TRUE;
			break;
		}
		archive.SkipCurrentChunk(ci);
	}

	ASSERT( TRUE == bAreaChunk );

	DWORD dwNumbers = 0;
	DWORD dwDummy = 0;

	// WZU건너띰
	archive >> dwNumbers;
	char pszUnitFileFullPath[0xff];
	for( DWORD j = 0 ; j < dwNumbers ; ++j )
	{
		archive.ReadName( pszUnitFileFullPath );
		archive >> dwDummy;
	}
	// 텍스쳐 건너띰
	archive >> dwNumbers;
	for (DWORD j = 0; j < dwNumbers; ++j )
	{
		archive >> dwDummy;
		archive.ReadName( pszUnitFileFullPath );
	}


	MAPOBJECT_INFO * pObject = NULL;
	// 실제 맵오브젝트
	archive >> dwNumbers;
	for( DWORD i = 0 ; i < dwNumbers ; ++i )
	{
		pObject = new MAPOBJECT_INFO;
		//pObject->dwMapObjectID = MAP_OBJECT_KEY+i; //< 클라이언트와 대역을 맞춰줘야 한다.
		pObject->dwMapObjectID = i;

		archive	>> pObject->wzId
				>> pObject->dwAttribute
				>> pObject->wvPos
				>> pObject->wzRot
				>> pObject->wzScale
				>> pObject->wzBV;

		if(archive.GetVersion() >= 138)
		{
			archive >> dwDummy;
		}

		ASSERT( NULL == m_pMapObjectInfoHashTable->GetData( pObject->dwMapObjectID ) );
		m_pMapObjectInfoHashTable->Add( pObject, pObject->dwMapObjectID );
	}
}
/*
VOID FieldInfo::_loadAreaInfo( CWzArchive & archive )
{
	//------------------------------------------------------------------------------------
	// 
	if( archive.GetVersion() < 140 ) return;
	if( archive.GetVersion() > 143 ) return;

	BOOL bAreaChunk = FALSE;
	while( TRUE )
	{
		St_ChunkInfo ci = archive.ReadChunk();
		if( ci.m_nID == 0x1785 )
		{
			bAreaChunk = TRUE;
			break;
		}
		archive.SkipCurrentChunk(ci);
	}

	if( TRUE != bAreaChunk ) return;

	// 영역 개수
	WORD wNumberOfAreas		= 0;
	BYTE byAreaType			= 0;
	WzID wzAreaID			= 0;
	archive >> wNumberOfAreas;
	ASSERT( wNumberOfAreas < 1000 );
	DEBUG_CODE( if(wNumberOfAreas >= 1000) return; );

	for( DWORD idx = 0 ; idx < wNumberOfAreas ; ++idx )
	{
		archive >> byAreaType;
		archive >> wzAreaID;
		if( byAreaType == AREA_INFO::AREA_AABB )
		{
			AABB_AREA_INFO * pAreaInfo = new AABB_AREA_INFO;
			pAreaInfo->dwKey		= idx;
			pAreaInfo->byAreaType	= byAreaType;
			pAreaInfo->wzAreaID		= wzAreaID;
			archive >> pAreaInfo->bvAABB;

			//m_pAreaInfoHashTable->Add( pAreaInfo, pAreaInfo->wzAreaID );
			
			m_pAreaInfoHashTable->Add( pAreaInfo, pAreaInfo->dwKey );
		}
		else
		{
			TILE_AREA_INFO * pAreaInfo = new TILE_AREA_INFO;
			pAreaInfo->dwKey		= idx;
			pAreaInfo->byAreaType	= byAreaType;
			pAreaInfo->wzAreaID		= wzAreaID;
			archive >> pAreaInfo->wNumberOfTiles;
			pAreaInfo->piTileIndex = new int[pAreaInfo->wNumberOfTiles];
			memset( pAreaInfo->piTileIndex, 0, sizeof(int) * pAreaInfo->wNumberOfTiles );

			WORD wTileAttr;		int iTileIndex = 0;		WORD wValidateTiles = 0;
			for( WORD i = 0 ; i < pAreaInfo->wNumberOfTiles ; ++i )
			{
				if( !m_pWorldBase )		continue;

				archive >> iTileIndex;

				wTileAttr = m_pWorldBase->x_pPathFinder->GetAttribute( iTileIndex );

				if( wTileAttr & PTA_NO_WALK )
				{
					continue;
				}

				pAreaInfo->piTileIndex[wValidateTiles] = iTileIndex;

				++wValidateTiles;
			}
			pAreaInfo->wNumberOfTiles = wValidateTiles;

			m_pAreaInfoHashTable->Add( pAreaInfo, pAreaInfo->dwKey );
		}
	}	

}
*/
VOID FieldInfo::_unloadMapObjectInfo()
{
	MAPOBJECT_INFO * pDelNode = NULL;
	m_pMapObjectInfoHashTable->SetFirst();
	while( (pDelNode = m_pMapObjectInfoHashTable->GetNext()) != NULL )
	{
		SAFE_DELETE( pDelNode );
	}
	m_pMapObjectInfoHashTable->RemoveAll();
}
/*
VOID FieldInfo::_unloadAreaInfo()
{
	AREA_INFO * pDelNode = NULL;
	m_pAreaInfoHashTable->SetFirst();
	while( (pDelNode = m_pAreaInfoHashTable->GetNext()) != NULL )
	{
		SAFE_DELETE( pDelNode );
	}
	m_pAreaInfoHashTable->RemoveAll();
}
*/
VOID FieldInfo::SetStartingTileIndex()
{
	m_StartTiles.clear();
	for( int i = '0' ; i <= '9' ; ++i )
	{
		WzSpecialArea * pArea = m_pWorldBase->GetSpecialArea( CharToWzID('S','T','R',i) );
		if( !pArea ) continue;
		ASSERT( pArea->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX );
		if( pArea->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX )
			continue;
		ASSERT( pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount != 0 );
		for( DWORD j = 0 ; j < pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount ; ++j )
			m_StartTiles.push_back( pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[j] );
	}
	for( int i = 'A' ; i <= 'Z' ; ++i )
	{
		WzSpecialArea * pArea = m_pWorldBase->GetSpecialArea( CharToWzID('S','T','R', i) );
		if( !pArea ) continue;
		ASSERT( pArea->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX );
		if( pArea->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX )
			continue;
		ASSERT( pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount != 0 );
		for( DWORD j = 0 ; j < pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount ; ++j )
			m_StartTiles.push_back( pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[j] );
	}
}

//DWORD FieldInfo::GetRandomStartingTileIndex()
//{
//	// 랜덤 셔플
//	ASSERT( m_StartTiles.size() != 0 );
//	random_shuffle( m_StartTiles.begin(), m_StartTiles.end() );
//	return *(m_StartTiles.begin());
//}

BOOL FieldInfo::CheckMapVersion( DWORD dwCheckSum )
{
	if( m_pWorldBase->GetCheckSum() != dwCheckSum )
	{
		return FALSE;
	}
	else
	{
		if( 0 == dwCheckSum)
		{
			// 0인 경우는 구버전 맵!!
			return FALSE;
		}
		return TRUE;
	}
}

////////////////////////////////////////////////////////////////////////////////////
// SectorInfo class 
////////////////////////////////////////////////////////////////////////////////////
SectorInfo::SectorInfo( DWORD index )
	:	m_dwSectorIndex ( index ),
		m_NeighborSectorInfo ( MAX_NEIGHBOR_SECTOR_NUM )
{

}

SectorInfo::~SectorInfo()
{

}

VOID SectorInfo::AddNeighbor( SectorInfo * pSectorInfo )
{
	m_NeighborSectorInfo.AddTail( pSectorInfo );
}
