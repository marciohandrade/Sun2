#include "stdafx.h"
#include "Map.h"

#if (0)

#include <macro.h>
#include "FieldInfo.h"
#include "GameField.h"
#include "GameZone.h"
#include "MapInfo.h"


__IMPL_CUSTOMPOOL_PTR(Map)
Map::Map()
{
	m_FieldHashTable.Initialize( _MAX_FIELD_IN_MAP_NUM );
}

Map::~Map()
{

}

VOID Map::Init( MapInfo * pMapInfo, BOOL bUsingPreLoading )
{
	m_pGameZone = NULL;
	m_pMapInfo = pMapInfo;
	
	FIELDINFO_HASH * pFieldInfoHash = m_pMapInfo->GetFieldInfoHash();
	BYTE idx = 0;
	FIELDINFO_HASH::iterator it, end(pFieldInfoHash->end());
	for( it = pFieldInfoHash->begin() ; it != end ; ++it, ++idx )
	{
		FieldInfo * pFieldInfo = it->second;
		GameField * pNewField = GameField::ALLOC();
		pNewField->Init( this, pFieldInfo, pMapInfo->GetMapInfo()->FGCode[idx], bUsingPreLoading );
		m_FieldHashTable.Add( pNewField, pFieldInfo->GetFieldCode() );
	}
}

VOID Map::Release()
{
	m_pGameZone = NULL;
	GameField * pField = NULL;
	m_FieldHashTable.SetFirst();
	while( (pField = m_FieldHashTable.GetNext()) != NULL )
	{
		pField->Release();
		GameField::FREE(pField); 
	}

	m_FieldHashTable.RemoveAll();
}

BOOL Map::CheckMapVersion(CODETYPE dwCodeType, DWORD dwCheckSum, DWORD * OUT pCurCheckSum )
{
	GameField* pField = GetFieldx(dwCodeType);
	if( NULL == pField ) return FALSE;

	return pField->CheckMapVersion( dwCheckSum, pCurCheckSum );
}

VOID Map::Update( DWORD dwDeltaTick )
{
	FIELD_HASH::iterator it, end(m_FieldHashTable.end());
	GameField* pField = NULL;
	for( it = m_FieldHashTable.begin() ; it != end ; ++it )
	{
		pField = *it;
		pField->Update( dwDeltaTick );
	}
}

DWORD Map::GetTotalMonsterNum( DWORD & OUT dwDeadNPCNum )
{
	DWORD dwNPCNum = 0;
	dwDeadNPCNum = 0;

	GameField* pField = NULL;
	FIELD_HASH::iterator itend = m_FieldHashTable.end();
	for( FIELD_HASH::iterator it = m_FieldHashTable.begin() ; it != itend ; ++it )
	{
		pField = *it;
		dwNPCNum += pField->GetNPCNum();
#ifdef _DEV_TAIYO_ADD_REGEN_TIME
		dwDeadNPCNum += pField->GetDeadNPCNum();
#endif
	}

	return dwNPCNum;
}



#endif


























