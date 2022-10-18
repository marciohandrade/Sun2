#include "StdAfx.h"
#include ".\resrcmanager.h"
#include ".\UnitRegenInfoManager.h"
#include ".\UnitGroupInfoManager.h"
#include ".\FieldInfo.h"
#include ".\SharedFieldInfo.h"
#include ".\WUTLoader.h"
#include <Struct.h>
#include <MapInfoParser.h>
#include "Sector.h"
#include "GameField.h"
#include "GameServerEx.h"
#include "MapPartialLoadingParser.h"
#include "ScriptRegenLoader.h"

util::CMemoryPoolFactory<GameField> ResrcManager::m_GameFieldPool;

void ResrcManager::SetPoolSize( DWORD field_size, DWORD sector_size )
{
	m_GameFieldPool.Initialize(field_size, 0, "ResrcManager::GameField", AssertionLogger4Pool_Server);
	GameField::SetPool( &m_GameFieldPool );
}


ResrcManager g_ResrcMgr;
static SharedFieldInfo*		sp_LobbyTmpInfo = NULL;

ResrcManager::ResrcManager(void)
{
}

ResrcManager::~ResrcManager(void)
{
}

void ResrcManager::Establish()
{
	sMAPINFO_HASH & rHash = MapInfoParser::Instance()->GetMapInfoHash();
	for( sMAPINFO_HASH::const_iterator it = rHash.begin() ; it != rHash.end() ; ++it )
	{
		sMAPINFO * psMapinfo = it->second;

		// 빠른 로딩을 위해 MapPartialLoading.ini를 참조한다.
		MAP_PARTIAL_LOADING *pLoadingInfo = MapPartialLoadingParser::Instance()->FindMapInfo( psMapinfo->MapKind );
		if( pLoadingInfo && !pLoadingInfo->m_bLoading )
		{
			continue;
		}

		Establish( psMapinfo );
	}

	// 로비를 위한 로딩
	FIELDCODE LobbyTmpCode = 501, LobbyWMOCode=501;
	if( !FindFieldInfo( LobbyTmpCode, LobbyWMOCode ) ) 
	{
		const sFIELDINFO * pInfo = MapInfoParser::Instance()->GetFieldInfo(LobbyTmpCode);
		SharedFieldInfo*& pSharedFieldInfo = sp_LobbyTmpInfo;
		pSharedFieldInfo = new SharedFieldInfo;
		pSharedFieldInfo->Create(pInfo->FieldCode, pInfo->pszPath, LobbyWMOCode, pInfo->bNewPathEngine, pInfo->bWarCastle);
		FieldInfo * pFieldInfo = new FieldInfo(pSharedFieldInfo);
		pFieldInfo->EstablishSectorInfo( ROOM_SECTOR_SIZE );
		InsertFieldInfo( pInfo->FieldCode, LobbyWMOCode, pFieldInfo );
	}
}

void ResrcManager::Destroy()
{
	//SharedFieldInfo를 제거한다.
	SharedFieldInfo * pSharedFieldInfo = NULL;
	_SHAREDFIELD_HASH::const_iterator ShareIt = m_SharedFieldInfoHash.begin(), ShareEnd=m_SharedFieldInfoHash.end();
	while( ShareIt != ShareEnd )
	{
		pSharedFieldInfo = (ShareIt->second);
		SAFE_DELETE( pSharedFieldInfo );
		++ShareIt;
	}
	m_SharedFieldInfoHash.clear();

	FieldInfo* pInfo = NULL;
	_FIELD_HASH::const_iterator it = m_FieldInfoHash.begin(), end=m_FieldInfoHash.end();
	while( it != end )
	{
		pInfo = (it->second);
		SAFE_DELETE( pInfo );
		++it;
	}
	m_FieldInfoHash.clear();

	SAFE_DELETE( sp_LobbyTmpInfo );

	for_each( m_UnitRegenInfoMgrHash.begin(), m_UnitRegenInfoMgrHash.end(), PairDeleter() );
	m_UnitRegenInfoMgrHash.clear();

	for_each( m_UnitGroupInfoMgrHash.begin(), m_UnitGroupInfoMgrHash.end(), PairDeleter() );
	m_UnitGroupInfoMgrHash.clear();
}

void ResrcManager::Establish( sMAPINFO * mapinfo )
{
	IEventRegisterForNpcKillByTrigger* const pEventRegister =
		AppliedNpcDependencyChain::Instance()->GetIEventRegisterForNpcKillByTrigger();

	for(int i=0;i<sMAPINFO::MAX_FIELD_NUM;++i)
	{
		if( 0 == mapinfo->FCode[i] ) continue;

		// WMO, SharedFieldInfo
		FieldInfo * pFieldInfo = FindFieldInfo( mapinfo->FCode[i], mapinfo->FWMOCode[i] );
		if( !pFieldInfo ) 
		{
//			SUNLOG( eDEV_LOG, "Map Establish, [Find FieldInfo], MKind = %d, FCode = %d, FWMOCode = %d",
//				mapinfo->byMKind, mapinfo->FCode[i], mapinfo->FWMOCode[i] );

            // (WAVERIX) (090507) (LOGIC-CHANGE) 섹터 사이즈 (40->30)으로 변경
            DWORD SectorSize = ROOM_SECTOR_SIZE;
			//DWORD SectorSize = 0;
			switch(mapinfo->GetZoneType4SharedCode())
			{
			case eZONETYPE_VILLAGE: //< 마을
				if( g_pGameServer->GetServerType() == BATTLE_SERVER )
					continue;
				break;
			case eZONETYPE_FIELD:	//< 뷰포트 적용
				//SectorSize = FIELD_SECTOR_SIZE; 
				if( g_pGameServer->GetServerType() == BATTLE_SERVER )
					continue;
				break;
            case eZONETYPE_DOMINATION_FIELD:
                SectorSize = DOMINATION_SECTOR_SIZE; 
                if( g_pGameServer->GetServerType() == BATTLE_SERVER )
                    continue;
                break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            case eZONETYPE_SPA_FIELD:
                if( g_pGameServer->GetServerType() == BATTLE_SERVER ) {
                    continue;
                }
                break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
			case eZONETYPE_CHARSELECT:
				continue;
			case eZONETYPE_LOBBY:
			case eZONETYPE_MISSION:
			case eZONETYPE_HUNTING:
			case eZONETYPE_QUEST:
			case eZONETYPE_PVP:
			case eZONETYPE_EVENT:
			case eZONETYPE_GUILD:
			case eZONETYPE_SIEGE:
			case eZONETYPE_CHUNTING:
                if( g_pGameServer->GetServerType() == FIELD_SERVER )
                    continue;
                break;
			case eZONETYPE_INSTANCE:
                if( g_pGameServer->GetServerType() == FIELD_SERVER )
                    continue;
				break;
			default:
				{
					SUNLOG( eCRITICAL_LOG, "Not Defined byMKind = %u", mapinfo->GetZoneType4SharedCode() );
					continue;
				}break;
			}
			const sFIELDINFO * pInfo = MapInfoParser::Instance()->GetFieldInfo(mapinfo->FCode[i]);
			ASSERT(pInfo);
			if(!pInfo)
			{
				SUNLOG( eCRITICAL_LOG, "Field for FieldCode[%u] is not exist!", mapinfo->FCode[i] );
				continue;
			}

			SharedFieldInfo* pSharedFieldInfo = NULL;
			pSharedFieldInfo = FindSharedFieldInfo( mapinfo->FCode[i], mapinfo->FWMOCode[i] );
			if( pSharedFieldInfo == NULL )
			{
				pSharedFieldInfo = new SharedFieldInfo;
				pEventRegister->PrepareRegistering();
				if( !pSharedFieldInfo->Create(mapinfo->FCode[i], pInfo->pszPath, mapinfo->FWMOCode[i], pInfo->bNewPathEngine, pInfo->bWarCastle) )
				{
					SUNLOG( eCRITICAL_LOG, "[ResrcManager::Establish] SharedFieldInfo Create Fail, FCode[%u], WMOCode[%u]", mapinfo->FCode[i], mapinfo->FWMOCode[i] );
					delete pSharedFieldInfo;
					continue;
				}

				InsertSharedFieldInfo( mapinfo->FCode[i], mapinfo->FWMOCode[i], pSharedFieldInfo );
			}

			pFieldInfo = new FieldInfo(pSharedFieldInfo);
			pFieldInfo->EstablishSectorInfo( SectorSize );
			InsertFieldInfo( mapinfo->FCode[i], mapinfo->FWMOCode[i], pFieldInfo );
		}
		
		// WUT info
		if( !FindRegenInfoMgr( mapinfo->FCode[i], mapinfo->FWUTCode[i] ) )
		{
			UnitRegenInfoManager * pUnitRegenInfoManager = new UnitRegenInfoManager(pFieldInfo);
			UnitGroupInfoManager * pUnitGroupInfoManager = new UnitGroupInfoManager;
			InsertRegenInfoMgr( mapinfo->FCode[i], mapinfo->FWUTCode[i], pUnitRegenInfoManager );
			InsertGroupInfoMgr( mapinfo->FCode[i], mapinfo->FWUTCode[i], pUnitGroupInfoManager );
			WUTLoader::Load( mapinfo->FWUTCode[i], pUnitRegenInfoManager, pUnitGroupInfoManager );

			// 스크립트에서 제어하는 해당맵의 리젠정보를 로드한다.
			g_ScriptRegenLoader.Load( pFieldInfo, mapinfo->MapCode, mapinfo->FCode[i], pUnitRegenInfoManager, pUnitGroupInfoManager );

            pEventRegister->BuildRegisterInfo(mapinfo->MapCode, mapinfo->FCode[i],
                                              pUnitRegenInfoManager);
		}
	}
}






















