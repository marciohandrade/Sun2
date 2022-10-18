#include "StdAfx.h"
#include ".\wutloader.h"
#include ".\UnitRegenInfo.h"
#include ".\UnitGroupInfo.h"
#include ".\UnitRegenInfoManager.h"
#include ".\UnitGroupInfoManager.h"
#include ".\GameServerEx.h"
#include <io.h>
#include <3DEngine/3DTerrain/LoadHelper.h>
#include <ProgramCommon/WzArchive.h>
#include <MapInfoParser.h>
#include "SinTable.h"

WUTLoader::WUTLoader(void)
{
}

WUTLoader::~WUTLoader(void)
{
}
BOOL WUTLoader::Load( FIELDCODE wutcode, 
					 UnitRegenInfoManager * pUnitRegenInfoMGR, 
					 UnitGroupInfoManager * pGroupInfoMGR )
{

	const sWUTINFO * pInfo = MapInfoParser::Instance()->FindWUTInfo(wutcode);
	if( !pInfo ) return FALSE;
	if( _access(pInfo->pszPath,0) == -1 )
		return FALSE;

	CWzArchive l_archive;
	if(!l_archive.LoadFile( const_cast<char*>(pInfo->pszPath) ))
	{
		return FALSE;
	};
	
	CLoadHelper * pLoadHelper = (CLoadHelper *)g_pGameServer->GetLoadHelper();

	St_LHFileHeader fnInfo;
	pLoadHelper->LoadUnit(&l_archive, fnInfo);

	CWzArray<St_LHUnit*> * pUnitArray = pLoadHelper->GetUnits();
	CWzArray<St_LHUnitGroup*> * pUnitGroupArray = pLoadHelper->GetUnitGroups();
	for( int i = 0 ; i < pUnitArray->GetCount() ; ++i )
	{
		St_LHUnit * pUnit = pUnitArray->Get(i);
		ASSERT(pUnit);
		UnitRegenInfo * pUnitRegenInfo = new UnitRegenInfo;
		pUnitRegenInfo->CopyDebugName( pUnit->name );
		pUnitRegenInfo->CopyFileName( pUnit->wzufilename );
		pUnitRegenInfo->SetCategory( pUnit->category );
		pUnitRegenInfo->SetID( pUnit->wID );
		pUnitRegenInfo->SetDir( (float)(((int)(pUnit->radian*180/PAI))%360) );
		pUnitRegenInfo->SetUnitType( pUnit->unit_type );    // 이동 타입 (eNPC_MOVE_ATTITUDE)
		pUnitRegenInfo->SetAIType( pUnit->ai_type );        // 사용X, 스크립트 이용(NPCCode->npcInfo->AI_TYPE_INFO)
		pUnitRegenInfo->SetRegenRandDist( pUnit->userdata[0] );	//< 랜덤 거리
		pUnitRegenInfo->SetReservedData2( pUnit->userdata[1] );

		pUnitRegenInfo->Init( pUnit->ucNumKey, pUnit->PathList.GetCount() );
		for( int npc_idx = 0 ; npc_idx < pUnit->ucNumKey ; ++npc_idx )
		{
            // changed by _NA_0_20100706_RANDOM_REGEN
            const DWORD regen_rate = static_cast<DWORD>(pUnit->fRegenRate[npc_idx]);
            const MONSTERCODE regen_monster = static_cast<MONSTERCODE>(pUnit->iKeyList[npc_idx]);

            if (!pUnitRegenInfo->AddRandomRegenMonster(regen_monster, regen_rate))
            {
                SUNLOG(eCRITICAL_LOG, "NPC_CODE 의 중복 추가 시도 "
                      "DebugName[%s] file_name[%s] regen_monster[%d]",
                       pUnit->name, pUnit->wzufilename, regen_monster);
            }
		}
		for( int pos_idx = 0 ; pos_idx < pUnit->PathList.GetCount() ; ++pos_idx )
		{
			St_LHUnitPath path = pUnit->PathList.Get(pos_idx);
			pUnitRegenInfo->SetStartPos(pos_idx, &path.wvStartPos );
            pUnitRegenInfo->SetDestPos(pos_idx, &path.wvPath );
            //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
            pUnitRegenInfo->SetWalkState(pos_idx, static_cast<eCHAR_MOVE_STATE>(path.byWalkState));
		}

		pUnitRegenInfo->SetRegenTime( pUnit->regentime*1000 );	//< 분단위
        pUnitRegenInfo->SetRegenTypeTrigger( pUnit->ucRegenType );

		ASSERT( NULL == pUnitRegenInfoMGR->FindUnit( pUnitRegenInfo->GetID() ) );
		pUnitRegenInfoMGR->Insert( pUnitRegenInfo->GetID(), pUnitRegenInfo );
	}

	// 그룹 정보 읽기
	for( int i = 0 ; i < pUnitGroupArray->GetCount() ; ++i )
	{
		St_LHUnitGroup * pUnitGroup = pUnitGroupArray->Get(i);
		ASSERT(pUnitGroup);
		UnitGroupInfo * pGroupInfo = new UnitGroupInfo;
		pGroupInfo->SetID( pUnitGroup->wID );
		pGroupInfo->SetLeaderRegenID( pUnitGroup->leader );
		pGroupInfo->SetReservedData( 0, pUnitGroup->userdata[0] );
		pGroupInfo->SetReservedData( 1, pUnitGroup->userdata[1] );
		for( int j = 0 ; j < pUnitGroup->unitlist.GetCount() ; ++j )
		{
			UnitRegenInfo * pUnitRegenInfo = pUnitRegenInfoMGR->FindUnit( pUnitGroup->unitlist.Get(j) );
			ASSERT( pUnitRegenInfo );
			pUnitRegenInfo->SetGroupID(pGroupInfo->GetID());
			//ASSERT( 0 == pUnitRegenInfo->GetGroupID() );
			////if( pUnitRegenInfo->GetGroupID() ) 
			//	DISPMSG( "Unit[%d]이 이미 그룹[%d]에 속해있습니다.", pUnitRegenInfo->GetID(), pUnitGroup->wID );
			//pUnitRegenInfo->SetGroupID( pGroupInfo->GetID() );
			//pUnitRegenInfo->SetLeaderRegenID( pGroupInfo->GetLeaderRegenID() );
			pGroupInfo->InsertMember( pUnitRegenInfo->GetID(), pUnitRegenInfo );
		}
		ASSERT( NULL == pGroupInfoMGR->Find( pGroupInfo->GetID() ) );
		pGroupInfoMGR->Insert( pGroupInfo->GetID(), pGroupInfo );
	}

	pLoadHelper->DestroyUnit();
	
//	g_pGameServer->m_fnContactTerrain( CT_DELETE_MAPLOADER, pLoadHelper );
	
//	pWorldBase->Destroy( NULL );
//	g_pGameServer->m_fnContactTerrain( CT_DELETE_WORLD, pWorldBase );
//	pWorldBase = NULL;
	
	l_archive.Close();

	return TRUE;
}