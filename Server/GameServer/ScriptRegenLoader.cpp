#include "StdAfx.h"
#include "ScriptRegenLoader.h"
#include "GroupParser.h"
#include "UnitGroupInfoManager.h"

ScriptRegenLoader g_ScriptRegenLoader;

ScriptRegenLoader::ScriptRegenLoader()
{
	m_RegenKeyGenerater.Create( 1, 50000 );
}

VOID ScriptRegenLoader::Load( FieldInfo *pFieldInfo, MAPCODE MapCode, FIELDCODE FieldCode, UnitRegenInfoManager * pUnitRegenInfoMGR, UnitGroupInfoManager * pGroupInfoMGR )
{
	CWorldBase *pWorldBase = pFieldInfo->GetWorldBase();

	RegenParser* pRegenMgr = RegenParser::Instance();
	REGENINFO_HASH_ITR itend = pRegenMgr->EndRegen();
	for( REGENINFO_HASH_ITR it=pRegenMgr->BeginRegen() ; it!=itend ; ++it )
	{
		REGEN_INFO* pRegenInfo = it->second;

		if( pRegenInfo->MapCode != MapCode )			continue;
		if( pRegenInfo->dwFieldIndex != FieldCode )		continue;

		if( pRegenInfo->bPosType == 0 )
		{
			WzSpecialArea *pArea = pWorldBase->GetSpecialArea( pRegenInfo->dwLocationID );
			if( !pArea )
			{
				TCHAR pszAreaID[128];	WzIDToString( pszAreaID, pRegenInfo->dwLocationID );pszAreaID[4] = '\0'; 
				SUNLOG( eCRITICAL_LOG, "[ScriptRegenLoader::Load] ScriptRegen[%d] Area[%s] doesn't exist!", pRegenInfo->dwNum, pszAreaID );
				continue;
			}
		}

		// 개별 NPC 셋팅
		for( int i = 0; i < MAX_REGEN_MONSTER_KIND; ++i )
		{
			if( pRegenInfo->dwMonCode[i] == 0 || pRegenInfo->dwMaxNum[i] == 0 ) continue;

			for( DWORD j = 0; j < pRegenInfo->dwMaxNum[i]; ++j )
			{
				UnitRegenInfo *pUnitRegenInfo = CreateSingleRegen( pFieldInfo, pRegenInfo, (WORD)pRegenInfo->dwMonCode[i] );

				if( pUnitRegenInfo )
					pUnitRegenInfoMGR->Insert( pUnitRegenInfo->GetID(), pUnitRegenInfo );
			}
		}

		// 그룹 NPC 셋팅
		for( int iGroup = 0; iGroup < MAX_GROUP_NUM; ++iGroup )
		{	
			BASE_GROUPINFO *pBaseGroupInfo = GroupParser::Instance()->GetGroupInfo( pRegenInfo->wGroupID[iGroup] );
			if( !pBaseGroupInfo ) continue;

			UnitGroupInfo * pUnitGroupInfo = new UnitGroupInfo;
			UNITGROUPID id = m_RegenKeyGenerater.GetKey();
			pUnitGroupInfo->SetID( id );
			pUnitGroupInfo->SetLeaderRegenID( 0 );
			pGroupInfoMGR->Insert( pUnitGroupInfo->GetID(), pUnitGroupInfo );

			// 리더 생성
			if( pBaseGroupInfo->dwLeaderCode != 0 )
			{
				UnitRegenInfo *pUnitRegenInfo = CreateSingleRegen( pFieldInfo, pRegenInfo, (WORD)pBaseGroupInfo->dwLeaderCode );

				if( pUnitRegenInfo )
				{
					pUnitRegenInfoMGR->Insert( pUnitRegenInfo->GetID(), pUnitRegenInfo );
					pUnitRegenInfo->SetGroupID( pUnitGroupInfo->GetID() );
					pUnitGroupInfo->InsertMember( pUnitRegenInfo->GetID(), pUnitRegenInfo );

					pUnitGroupInfo->SetLeaderRegenID( pUnitRegenInfo->GetID() );
				}
			}

			// 쫄따구들 생성
			for( int iFollower = 0; iFollower < MAX_FOLLOWER_NUM; ++iFollower )
			{
				FOLLOWERINFO &rFollowerInfo = pBaseGroupInfo->FollowerInfo[iFollower];

				if( rFollowerInfo.dwMonCode == 0 ) break;

				UnitRegenInfo *pUnitRegenInfo = CreateSingleRegen( pFieldInfo, pRegenInfo, (WORD)rFollowerInfo.dwMonCode );

				if( pUnitRegenInfo )
				{
					pUnitRegenInfoMGR->Insert( pUnitRegenInfo->GetID(), pUnitRegenInfo );
					pUnitRegenInfo->SetGroupID( pUnitGroupInfo->GetID() );
					pUnitGroupInfo->InsertMember( pUnitRegenInfo->GetID(), pUnitRegenInfo );
				}
			}
		}
	}
}

UnitRegenInfo * ScriptRegenLoader::CreateSingleRegen( FieldInfo *pFieldInfo, REGEN_INFO *pRegenInfo, WORD NPCCode )
{
	if( pRegenInfo->bPosType != 0 && pRegenInfo->bPosType != 1 )
		return NULL;

	// ID 발급
	REGENID id = m_RegenKeyGenerater.GetKey();

	UnitRegenInfo *pUnitRegenInfo = new UnitRegenInfo;
	pUnitRegenInfo->CopyDebugName( "temp" );
	pUnitRegenInfo->CopyFileName( "N/A" );
	pUnitRegenInfo->SetCategory( 99999 );
	pUnitRegenInfo->SetID( id );
	pUnitRegenInfo->SetDir( 0 );
	pUnitRegenInfo->SetRegenRandDist( pRegenInfo->bDistance );
	pUnitRegenInfo->SetAIType( 0 );
	pUnitRegenInfo->SetUnitType( pRegenInfo->byMoveAttitude );
	pUnitRegenInfo->SetReservedData2( 0 );

	WzVector vDestPos;	SetVector( &vDestPos, 0, 0, 0 );

	// Regen.txt
	if( pRegenInfo->bPosType == 0 )
	{
		WzSpecialArea *pArea = pFieldInfo->GetWorldBase()->GetSpecialArea( pRegenInfo->dwLocationID );
		if( !SetStartPos( pFieldInfo, pArea, pUnitRegenInfo ) )
		{
			// 좌표 셋팅에 실패할 경우 리젠을 하지 않는다.
			SUNLOG( eFULL_LOG, "[CreateSingleRegen] ScriptRegen[%d] MonCode[%d] Setting Fail!", pRegenInfo->dwNum, NPCCode );
			pUnitRegenInfo->Release();
			delete pUnitRegenInfo;
			return NULL;
		}
	}
	// Regen_pos.txt
	else if( pRegenInfo->bPosType == 1 )
	{
		pUnitRegenInfo->Init( 1, 1 );
		pUnitRegenInfo->SetStartPos( 0, &pRegenInfo->pvPos );
		pUnitRegenInfo->SetDestPos( 0, &vDestPos );
	}

    // changed by _NA_0_20100706_RANDOM_REGEN
    const DWORD regen_rate = static_cast<DWORD>(pRegenInfo->fRegenRate*100);
    const MONSTERCODE regen_monster = static_cast<MONSTERCODE>(NPCCode);

    pUnitRegenInfo->AddRandomRegenMonster(regen_monster, regen_rate);
    pUnitRegenInfo->SetRegenTime( pRegenInfo->dwRegenTime );

	return pUnitRegenInfo;
}

BOOL ScriptRegenLoader::SetStartPos( FieldInfo *pFieldInfo, WzSpecialArea *pArea, UnitRegenInfo *pUnitRegenInfo )
{
	CWorldBase *pWorldBase = pFieldInfo->GetWorldBase();

	WzVector vRegenPos;	WzVector vDestPos;	SetVector( &vDestPos, 0, 0, 0 );
	if( pArea ->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
	{
		int iAreaTileCount = max( 1, pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount );
		pUnitRegenInfo->Init( 1, iAreaTileCount );

		for( int pos_idx = 0 ; pos_idx < iAreaTileCount ; ++pos_idx )
		{
			DWORD *pdwTileIndex = pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData;
			vRegenPos = pWorldBase->GetRandomPosInTile( pdwTileIndex[pos_idx] );
			pUnitRegenInfo->SetStartPos( pos_idx, &vRegenPos );
			pUnitRegenInfo->SetDestPos( pos_idx, &vDestPos );
		}
		return TRUE;
	}
	else if( pArea ->m_wbvRegion.m_eBvType == BVT_AABB )
	{
		pUnitRegenInfo->Init( 1, 1 );

		WzVector vMinPos = pArea->m_wbvRegion.m_aabb.m_wvMin;
		WzVector vMaxPos = pArea->m_wbvRegion.m_aabb.m_wvMax;
		vRegenPos.z = vMaxPos.z;
		// 10번까지만 시도한다.
		for( int i = 0; i < 10; ++i )
		{
			vRegenPos.x = (float)dRandom( vMinPos.x, vMaxPos.x );
			vRegenPos.y = (float)dRandom( vMinPos.y, vMaxPos.y );

			if( pFieldInfo->IsMovableCoordinatesRaw( &vRegenPos ) )
			{
				pUnitRegenInfo->SetStartPos( 0, &vRegenPos );
				pUnitRegenInfo->SetDestPos( 0, &vDestPos );
				return TRUE;
			}
		}
	}
	return FALSE;
}



























































