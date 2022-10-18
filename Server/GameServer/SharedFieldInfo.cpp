#include "StdAfx.h"
#include ".\sharedfieldinfo.h"
#include <io.h>
#include <fstream>
#include <ProgramCommon/WzArchive.h>
#include <3DEngine/Common/IncludeAll_NewWorld.h>
#include <3DEngine/3DTerrain/WorldBase.h>
#include <ProgramCommon/DiscAccess.h>
#include "GameServerEx.h"
#include <MapInfoParser.h>
#include "AppliedNpcDependencyChain.h"


SharedFieldInfo::SharedFieldInfo(void)
	: m_pWorldBase(NULL)
	, m_LastUsedRoomIndex(0)
{
    // (f100623.1L) cache for checksums
    ZeroMemory(&checksums_, sizeof(checksums_));
}

SharedFieldInfo::~SharedFieldInfo(void)
{
	Destroy();
}

BOOL SharedFieldInfo::Create( FIELDCODE fieldcode, const char * field_path, FIELDCODE wmocode, BOOL bNewPathEngine, BOOL bWarCastle )
{
	m_Code = fieldcode;
	m_WMOCode = wmocode;
	m_bNewPathEngine = bNewPathEngine;
	m_bWarCastle = bWarCastle;

	_tcsncpy( m_tszMapPath, field_path, MAX_PATH );
	m_tszMapPath[MAX_PATH] = '\0';

	m_MapObjectInfoHash.clear();

	ASSERT(!m_pWorldBase);
	m_pWorldBase = (CWorldBase *)g_pGameServer->m_fnContactTerrain( CT_NEW_WORLD, 0 );

	if( m_bNewPathEngine )
		m_pWorldBase->Create();
	else
		m_pWorldBase->Create( FALSE );

    // (CHANGES) (f100501.2L) (WAVERIX) add the debugging facility
    SUNLOG(eDEV_LOG, _T("|Start Read|FieldCode=%d|WMOCode=%d|Path=%s|"),
           fieldcode, wmocode, field_path);
    if (_Load(field_path) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|Reading failed!|Path=%s|"), field_path);
        Destroy();
        return false;
    }
    SUNLOG(eDEV_LOG, _T("|End Read|Path=%s|"), field_path);

	_SetStartingTileIndex();
	_SetCRStartingTileIndex();

	if( m_StartTiles.size() == 0 && m_CR1StartTiles.size() == 0 && m_CR2StartTiles.size() == 0 )
	{
		SUNLOG( eCRITICAL_LOG, "[SharedFieldInfo::Create] Error, StartTiles Not Found, FieldCode = %u, FidldPath = %s", fieldcode, field_path );
	}

#ifdef _DEV_TAIYO_PATHENGINE_CODE
	//LARGE_INTEGER firstcounter,nextcounter;
	//QueryPerformanceCounter(&firstcounter);

	//if( m_bNewPathEngine )
	//{
	//	char pszFilePath[_MAX_PATH]={0,};
	//	char pszFileName[_MAX_FNAME]={0,};
	//	char pszDir[_MAX_PATH]={0,};
	//	_splitpath( field_path, NULL, pszDir, pszFileName, NULL );
	//	_snprintf( pszFilePath, _MAX_PATH, "%s\\%s.pfc", pszDir, pszFileName );
	//	m_pWorldBase->x_pPathFinder->GenerateCollisionPreprocess( m_pWorldBase->GetDiscAccess(), pszFilePath );
	//}
	//else
	//	m_pWorldBase->x_pPathFinder->GenerateCollisionPreprocess( NULL, NULL );

	//QueryPerformanceCounter(&nextcounter);
	//__int64 elapsedclock = nextcounter.QuadPart - firstcounter.QuadPart;
	//__int64 elapsedmillsecond = elapsedclock / g_pGameServer->m_freq.QuadPart;
	//printf( "[GenerateCollisionPreprocess] FieldCode[%d] Tick[%I64d]\n", m_Code, elapsedmillsecond );
#endif

	return TRUE;
}

VOID SharedFieldInfo::Destroy()
{
	_UnloadMapObjectInfo();

	m_TriggerInfoManager.Release();

	if( m_pWorldBase )
	{
		m_pWorldBase->Destroy( NULL );
		g_pGameServer->m_fnContactTerrain( CT_DELETE_WORLD, m_pWorldBase );
		m_pWorldBase = NULL;
	}

	m_MapObjectInfoHash.clear();
}

VOID SharedFieldInfo::_UnloadMapObjectInfo()
{
	MAPOBJECT_INFO * pDelNode = NULL;
	MAPOBJECT_INFO_HASH_IT itend = m_MapObjectInfoHash.end();
	for( MAPOBJECT_INFO_HASH_IT it = m_MapObjectInfoHash.begin(); it != itend ; ++it )
	{
		pDelNode = it->second;
		SAFE_DELETE( pDelNode );
	}
	m_MapObjectInfoHash.clear();
}

VOID SharedFieldInfo::_SetStartingTileIndex()
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

VOID SharedFieldInfo::_SetCRStartingTileIndex()
{
	m_CR1StartTiles.clear();
	m_CR2StartTiles.clear();
	for( int i = '0' ; i <= '9' ; ++i )
	{
		WzSpecialArea * pArea = m_pWorldBase->GetSpecialArea( CharToWzID('C','R','1',i) );
		if( !pArea ) continue;
		ASSERT( pArea->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX );
		if( pArea->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX )
			continue;
		ASSERT( pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount != 0 );
		for( DWORD j = 0 ; j < pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount ; ++j )
			m_CR1StartTiles.push_back( pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[j] );

		pArea = m_pWorldBase->GetSpecialArea( CharToWzID('C','R','2',i) );
		if( !pArea ) continue;
		ASSERT( pArea->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX );
		if( pArea->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX )
			continue;
		ASSERT( pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount != 0 );
		for( DWORD j = 0 ; j < pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount ; ++j )
			m_CR2StartTiles.push_back( pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData[j] );
	}
}

BOOL SharedFieldInfo::_Load( const char * field_path )
{
	CWzArchive archive;

	CDiscAccess cda;
	cda.Create();
    // (WAVERIX) (090718) (CHANGES) Sun3DEngine_090717 version으로 로드시 DiscAccess 문제 발생 부분 수정.
	m_pWorldBase->SetDiscAccess(&cda);

	if( _access(field_path,0) == -1 )
	{
		SUNLOG( eCRITICAL_LOG,  "[FieldInfo::Load] Failed : %s file is not Exist", field_path );
		return FALSE;
	}

	if( archive.LoadFile( const_cast<char *>(field_path) ) == FALSE )
	{
		SUNLOG( eCRITICAL_LOG,  "[FieldInfo::Load] Failed LoadFile() : From Fieldxx.map[%s]", field_path );
		archive.Close();
		return FALSE;
	}

    // (f100623.1L)
    ZeroMemory(&checksums_, sizeof(checksums_));

	archive.Seek( 4, SO_CURRENT );
	archive >> m_Version;
	archive.Seek( -6, SO_CURRENT );

	if( m_pWorldBase->Serialize( &archive ) == FALSE )
	{
		SUNLOG( eCRITICAL_LOG,  "[FieldInfo::Load] Failed WorldBase_Serialize() : From Fieldxx.map[%s]", field_path );
		archive.Close();
		return FALSE;
	}

	// 맵오브젝트 + 트리거 정보
	if( !_Serialize_WMO( archive ) )
	{
		SUNLOG( eCRITICAL_LOG,  "[FieldInfo::Load] Failed _Serialize_WMO() : From Fieldxx.map[%s]", field_path );
		archive.Close();
		return FALSE;
	}
    // release resources
    archive.Close();
    return true;
}

BOOL SharedFieldInfo::_Serialize_WMO( CWzArchive & archive )
{
	BOOL bAreaChunk = FALSE;
	while( TRUE )
	{
		St_ChunkInfo ci = archive.ReadChunk();
		if( ci.m_nID == 0x1792 )
		{
			bAreaChunk = TRUE;
			break;
		}
		archive.SkipCurrentChunk(ci);
	}
	ASSERT( TRUE == bAreaChunk );

	char pszFilePath[_MAX_PATH]={0,};
	char pszFileName[_MAX_FNAME]={0,};
	char pszDir[_MAX_PATH]={0,};

	const sWMOINFO * pInfo = MapInfoParser::Instance()->FindWMOInfo(GetWMOCode());
	if( !pInfo ) return FALSE;

#ifdef _HARDMODE_TRIGGER_PRINT_
    ;{
        std::ofstream out_trigger("TRIGGER.txt", std::ios::app);
        CHAR buffer[512] = { 0, };
        sprintf(buffer, "mapcode : [%d] filename : [%s]\n", pInfo->Code, pInfo->pszPath);
        out_trigger << buffer;
    };
#endif //_HARDMODE_TRIGGER_PRINT_

	return _LoadMapObjectInfo(pInfo->pszPath);
}

BOOL SharedFieldInfo::_LoadMapObjectInfo( const char * pszFilePath )
{
	_tcsncpy( m_tszWmoPath, pszFilePath, MAX_PATH );
	m_tszWmoPath[MAX_PATH-1] = '\0';

	CWzArchive l_archive;
	l_archive.SetDiscAccess(GetWorldBase()->GetDiscAccess());

	if( _access(pszFilePath,0) == -1 )
	{
		SUNLOG( eCRITICAL_LOG,  "[_LoadMapObjectInfo] Failed : %s file is not Exist", pszFilePath );
		return FALSE;
	}

	if(!l_archive.LoadFile( const_cast<char*>(pszFilePath) ))
	{
		return FALSE;
	}

	WORD wVersion = 0;
	l_archive.Seek( 4, SO_CURRENT );
	l_archive >> wVersion;
	l_archive.SetVersion( wVersion );

    St_LHFileHeader fhInfo = { 0, };
	if( 170 < l_archive.GetVersion() )
	{
		l_archive.Read(&fhInfo.wSecondVersion, sizeof(WORD));
		l_archive.Read(&fhInfo.dwCheckSum,     sizeof(DWORD));
		SUNLOG( eDEV_LOG, "Second Ver[%u] and CheckSum[%u]", fhInfo.wSecondVersion, fhInfo.dwCheckSum );
	}
    // (f100623.1L)
    checksums_.checksum_map = m_pWorldBase->GetCheckSum();
    checksums_.checksum_wmo = fhInfo.dwCheckSum;

	GetWorldBase()->SerializeExtraAreaInfo(&l_archive, TRUE);

	_SerializeMapObjectAll(l_archive);

	// Map과 Wmo 파일의 Version 무결성 체크!
	_CheckVersionBetweenMapWmo( GetVersion(), l_archive.GetVersion() );

	l_archive.Close();

	return TRUE;
}

VOID SharedFieldInfo::_SerializeMapObjectAll( CWzArchive & archive )
{
	CLoadHelper * pLoadHelper = GetWorldBase()->x_pLoadHelper;
	pLoadHelper->LoadObj(&archive);

	//오브 젝트 리스트 로딩
	int count = pLoadHelper->GetObjectCount();
	St_LHObject * pLHObject = pLoadHelper->GetObject();
	//ASSERT( count < (ITEM_OBJECT_KEY - MAP_OBJECT_KEY) ); //< 키가 5만개 이하여야 한다.
	for( int i = 0 ; i < count ; ++i )
	{
		MAPOBJECT_INFO * pObject = new MAPOBJECT_INFO;
        //__NA001283_20090303_COLLECTION_MAPOBJECTID
		pObject->dwMapObjectID	= i + MAP_OBJECT_KEY;
		pObject->wzId			= pLHObject[i].m_WzID;
		pObject->wvPos			= pLHObject[i].m_wvPos;
		pObject->wzRot			= pLHObject[i].m_wvRot;
		pObject->wzScale		= pLHObject[i].m_wvScale;
		pObject->wzBV			= pLHObject[i].m_wzBV;
		pObject->dwAttribute	= pLHObject[i].m_dwAttribute;
		pObject->CollectionID		= pLHObject[i].m_wiUser;  // 어디다 쓸까? -> user == 기획자 (채집물 인덱스)
		// 추가 데이터 설정
		pObject->dwExtraInfos[eMAPOBJECT_TYPE] = pLHObject[i].m_dwParam[eMAPOBJECT_TYPE];
		pObject->dwExtraInfos[eMAPOBJECT_MONSTER] = pLHObject[i].m_dwParam[eMAPOBJECT_MONSTER];
		pObject->dwExtraInfos[eMAPOBJECT_USE_ALPHA] = pLHObject[i].m_dwParam[eMAPOBJECT_USE_ALPHA];
		pObject->dwExtraInfos[eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE] = 
            pLHObject[i].m_dwParam[eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE];

		ASSERT( NULL == FindMapObjectInfo( pObject->dwMapObjectID ) );
		m_MapObjectInfoHash.insert( MAPOBJECT_INFO_PAIR(pObject->dwMapObjectID, pObject) );
	}

	pLoadHelper->DeleteObject();

	// 트리거 정보 로딩
	m_TriggerInfoManager.LoadGroupInfos( archive, AppliedNpcDependencyChain::Instance()->GetIEventRegisterForNpcKillByTrigger() );
}

BOOL SharedFieldInfo::_LoadPathEngineCollision( char * pszFileName )
{
	CWzArchive l_archive;

	l_archive.SetDiscAccess(GetWorldBase()->GetDiscAccess());

	if( _access(pszFileName,0) == -1 )
	{
		SUNLOG( eCRITICAL_LOG,  "[_LoadPathEngineCollision] Failed : %s file is not Exist", pszFileName );
		return FALSE;
	}

	if( !l_archive.LoadFile( pszFileName ) )
		return FALSE;

	l_archive.Seek( 4, SO_CURRENT );
	WORD wVersion = 0;
	l_archive >> wVersion;
	wVersion = VERSION_WEBZEN_MAP;
	l_archive.SetVersion(wVersion);


	CWzArray<WzVector> wzArrayVect;
	int iObjCnt = 0;
	int iBvCnt = 0;
	int iVCnt = 0;
	int iID = 0;	
	WzVector wvPos;
	//. Object 개수를 얻어와서 등록
	l_archive >> iObjCnt;
	for(int iCnt=0;iCnt<iObjCnt;++iCnt)
	{
		l_archive >> iBvCnt;
		if( iBvCnt )
		{
			l_archive >> iVCnt;
			l_archive >> iID;
			l_archive >> wvPos;
			l_archive.Read(wzArrayVect.GetBufferPtr(),iVCnt*sizeof(WzVector));
			GetWorldBase()->x_pPathFinder->EntryCollisionTerritoryUnderLine(ECO_OBJECT,iID,wvPos,iVCnt,wzArrayVect.GetBufferPtr());
		}
	}
	//. SpeedTree 개수를 얻어와서 등록
	l_archive >> iObjCnt;
	for(int iCnt=0;iCnt<iObjCnt;++iCnt)
	{
		l_archive >> iVCnt;
		if( iVCnt )
		{	
			l_archive >> iID;
			l_archive >> wvPos;
			l_archive.Read(wzArrayVect.GetBufferPtr(),iVCnt*sizeof(WzVector));
			GetWorldBase()->x_pPathFinder->EntryCollisionTerritoryUnderLine(ECO_SPEEDTREE,iID,wvPos,iVCnt,wzArrayVect.GetBufferPtr());
		}
	}
	l_archive.Close();	

	return TRUE;
}

VOID SharedFieldInfo::_CheckVersionBetweenMapWmo( DWORD MapVersion, DWORD WmoVersion )
{
	// 167 이하 버전의 Map은 신규로 교체하는 것이 로딩에 부하가 적다.
	if( MapVersion <= 167 )
	{
		SUNLOG( eCRITICAL_LOG, "[_CheckVersionBetweenMapWmo] Warning! %s Version[%u] is Low!!", m_tszMapPath, MapVersion );
	}

	// 162, 166, 167을 경계로 Wmo 보다 Map의 버전이 더 높으면 안된다.
	if( MapVersion > 162 && WmoVersion <= 162 ||
		MapVersion > 166 && WmoVersion <= 166 ||
		MapVersion > 167 && WmoVersion <= 167 ||
		MapVersion > 170 && WmoVersion <= 170 )
	{
		SUNLOG( eCRITICAL_LOG, "[_CheckVersionBetweenMapWmo] Critical! %s Version[%u] is Low!!", m_tszMapPath, WmoVersion );
	}
}




















