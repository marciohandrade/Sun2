#include "stdafx.h"
#include <ProgramCommon/WzDataType.h>
#include <SolarFileReader.h>

#include <NPCInfoParser.h>
#include <GroupParser.h>
#include "RegenParser.h"


RegenParser::RegenParser()
{
}

RegenParser::~RegenParser()
{
}

VOID RegenParser::Release()
{
	Unload();
}

VOID RegenParser::Init( DWORD dwPoolSize )
{
}

VOID RegenParser::Unload()
{
	do {
		REGENINFO_HASH_ITR itend = m_RegenHashTable.end();
		for( REGENINFO_HASH_ITR it=m_RegenHashTable.begin() ; it != itend ; ++it )
		{
			REGEN_INFO* pINFO = it->second;
			delete pINFO;
		}
		m_RegenHashTable.clear();
	} while(0);
}


VOID RegenParser::Reload()
{
	//	Load( m_pszFileName, TRUE );
}

//----------------------------------------------------------------------------
// 몬스터를 추가한다.
BOOL RegenParser::AddMonster( REGEN_INFO* pInfo, DWORD dwID )
{
	m_RegenHashTable.insert( std::make_pair( dwID, pInfo ) );
	return TRUE;
}

BOOL RegenParser::DelMonster( REGEN_INFO* pInfo, DWORD dwID )
{
	m_RegenHashTable.erase( dwID );
	return TRUE;
}

BOOL RegenParser::CheckValidity( REGEN_INFO* pRegenInfo )
{
	BOOL bSuccess = TRUE;

	// 이동 성향 체크
	if( pRegenInfo->byMoveAttitude >= eMOVE_ATTITUDE_MAX )
	{
		SUNLOG( eCRITICAL_LOG, "[RegenParser] RegenIndex[%d] Invalid byMoveAttitude[%d]! \n", pRegenInfo->dwNum, pRegenInfo->byMoveAttitude );
		pRegenInfo->byMoveAttitude = eMOVE_ATTITUDE_WANDER;
		bSuccess = FALSE;
	}

	// 리젠 타임

	if( pRegenInfo->dwRegenTime == 0 )
	{
		SUNLOG( eCRITICAL_LOG, "[RegenParser] RegenIndex[%d] dwRegenTime is 0! \n", pRegenInfo->dwNum );
		pRegenInfo->dwRegenTime = 1000;
		bSuccess = FALSE;
	}

	// 리젠 확률
	if( pRegenInfo->fRegenRate == 0 )
	{
		SUNLOG( eCRITICAL_LOG, "[RegenParser] RegenIndex[%d] fRegenRate is 0! \n", pRegenInfo->dwNum );
		pRegenInfo->fRegenRate = 1.f;
		bSuccess = FALSE;
	}

	// 그룹
	for( int i = 0; i < MAX_GROUP_NUM; ++i )
	{
		if( !pRegenInfo->wGroupID[i] )		continue;

		BASE_GROUPINFO *pBaseGroupInfo = GroupParser::Instance()->GetGroupInfo( pRegenInfo->wGroupID[i] );
		if( !pBaseGroupInfo )
		{
			SUNLOG( eCRITICAL_LOG, "[RegenParser] RegenIndex[%d] GroupInfo[%d] doesn't exist! \n", pRegenInfo->dwNum, pRegenInfo->wGroupID[i] );
			pRegenInfo->wGroupID[i] = 0;
			bSuccess = FALSE;
		}
	}

	// 리젠 NPC
	for( int i = 0; i < MAX_REGEN_MONSTER_KIND; ++i )
	{
		if( !pRegenInfo->dwMonCode[i] )		continue;

		BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( pRegenInfo->dwMonCode[i] );
		if( !pBaseNPCInfo )
		{
			SUNLOG( eCRITICAL_LOG, "[RegenParser] RegenIndex[%d] NPCInfo[%d] doesn't exist! \n", pRegenInfo->dwNum, pRegenInfo->dwMonCode[i] );
			pRegenInfo->dwMonCode[i] = 0;
			pRegenInfo->dwMaxNum[i] = 0;
			bSuccess = FALSE;
		}
	}

	return bSuccess;
}

	BOOL RegenParser::_LoadRegen( BOOL bReload )
	{
		char szLocationID[MAX_AREA_ID_LENGTH+1];

		int nRowSize = GetRowSize();

		for( int nRow = 0; nRow < nRowSize; ++nRow )
		{
			BOOL bAdd = FALSE;
			DWORD dwID = GetDataInt( "Num", nRow );

			REGENINFO_HASH_ITR itFind = m_RegenHashTable.find( dwID );
			REGEN_INFO* pINFO = NULL;
			if( itFind == m_RegenHashTable.end() )
			{
				pINFO = new REGEN_INFO;
				ZeroMemory( pINFO, sizeof(REGEN_INFO) );
				bAdd = TRUE;
			}
			else
			{
				pINFO = itFind->second;
				if( FALSE == bReload )
				{
					ASSERT( !"데이터에 오류가 있습니다." );
				}
			}

			pINFO->bPosType	= 0;
			pINFO->dwNum = dwID;

			strncpy( pINFO->szDesc, GetDataString( "Desc", nRow ), MAX_REGEN_DESC_LEN );

			pINFO->MapCode				= GetDataInt( "Mapcode", nRow );
			pINFO->dwFieldIndex			= GetDataInt( "FieldIndex", nRow );

			ZeroMemory( szLocationID, sizeof(szLocationID) );
			strncpy( szLocationID, GetDataString( "LocationID", nRow ), MAX_AREA_ID_LENGTH );
			pINFO->dwLocationID = (DWORD)StrToWzID( szLocationID );

			pINFO->wMonsterType			= GetDataInt( "MonType", nRow );
			pINFO->bAllRegen			= GetDataInt( "bAllRegen", nRow );
			pINFO->byMoveAttitude		= GetDataInt( "byMoveAttitude", nRow );
			pINFO->dwRegenTime			= GetDataInt( "dRegenTime", nRow );
			pINFO->fRegenRate			= GetDataFloat( "fRegenRate", nRow );

			TCHAR szTemp[MAX_FIELD_NAME_SIZE];
			for( INT i = 0 ; i < MAX_GROUP_NUM ; ++i )
			{
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "GroupID%d\0", i+1 );
				pINFO->wGroupID[i]		= GetDataInt( szTemp, nRow );
			}

			for( INT i = 0 ; i < MAX_REGEN_MONSTER_KIND ; ++i )
			{
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MonCode%d\0", i+1 );
				pINFO->dwMonCode[i]		= GetDataInt( szTemp, nRow );
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MaxRegenLimit%d\0", i+1 );
				pINFO->dwMaxNum[i]		= GetDataInt( szTemp, nRow );
			}

			if( bAdd )
			{
				m_RegenHashTable.insert( std::make_pair(dwID, pINFO) );
			}

			// 유효성을 검증한다.
			CheckValidity( pINFO );
		}

		return TRUE;
	}

	BOOL RegenParser::_LoadPos( BOOL bReload )
	{
		int nRowSize = GetRowSize();

		for( int nRow = 0; nRow < nRowSize; ++nRow )
		{
			BOOL bAdd = FALSE;
			DWORD dwID = GetDataInt( "Index", nRow );
			dwID += 1000000;	// 주의!!!! 위치세팅 몬스터는 강제로 dwID를 1000000 번 다음부터

			REGENINFO_HASH_ITR itFind = m_RegenHashTable.find( dwID );
			REGEN_INFO* pINFO = NULL;
			if( itFind == m_RegenHashTable.end() )
			{
				pINFO = new REGEN_INFO;
				ZeroMemory( pINFO, sizeof(REGEN_INFO) );
				bAdd = TRUE;
			}
			else
			{
				pINFO = itFind->second;
				if( FALSE == bReload )
				{
					ASSERT( !"데이터에 오류가 있습니다." );
				}
			}

			pINFO->bPosType = 1;
			pINFO->dwNum = dwID;

			pINFO->bDistance			= GetDataInt( "Distance", nRow );
			pINFO->bAllRegen			= GetDataInt( "bAllRegen", nRow );
			pINFO->fRegenRate			= GetDataFloat( "fRegenRate", nRow );

			TCHAR szTemp[MAX_FIELD_NAME_SIZE];
			for( int i = 0; i < MAX_REGEN_MONSTER_KIND; i++ )
			{
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MCode%d\0", i+1 );
				pINFO->dwMonCode[i]		= GetDataInt( szTemp, nRow );
				_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "MnUm%d\0", i+1 );
				pINFO->dwMaxNum[i]		= GetDataInt( szTemp, nRow );
			}

			strncpy( pINFO->szDesc, GetDataString( "Desc", nRow ), MAX_REGEN_DESC_LEN );

			pINFO->MapCode				= GetDataInt( "Mapcode", nRow );
			pINFO->dwFieldIndex			= GetDataInt( "FieldIndex", nRow );
			pINFO->dwRegenTime			= GetDataInt( "dRegenTime", nRow );

			//	script date type : int
			pINFO->pvPos.x				= (float)GetDataInt( "X", nRow );
			pINFO->pvPos.y				= (float)GetDataInt( "Y", nRow );
			pINFO->pvPos.z				= (float)GetDataInt( "Z", nRow );

			pINFO->dwLocationID		= dwID;
			pINFO->wMonsterType		= 1;
			pINFO->byMoveAttitude	= 0;
			for( int i = 0; i < MAX_GROUP_NUM; ++i )
			{
				pINFO->wGroupID[i]	= 0;
			}

			if( bAdd )
			{
				m_RegenHashTable.insert( std::make_pair( dwID, pINFO ) );
			}
		}

		return TRUE;
	}

	BOOL RegenParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
	{
		switch( ScriptCode )
		{
		case SCRIPT_REGEN:
			return _LoadRegen( bReload );
		case SCRIPT_REGEN_POS:
			return _LoadPos( bReload );
		};

		return FALSE;
	}

