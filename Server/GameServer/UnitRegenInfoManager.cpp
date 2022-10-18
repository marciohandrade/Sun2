#include "StdAfx.h"
#include ".\UnitRegenInfoManager.h"

UnitRegenInfoManager::UnitRegenInfoManager( FieldInfo *pFieldInfo )
{
	m_pFieldInfo = pFieldInfo;
}

UnitRegenInfoManager::~UnitRegenInfoManager(void)
{
	for_each( m_UnitHash.begin(), m_UnitHash.end(), PairDeleter() );
	m_UnitHash.clear();
}

void UnitRegenInfoManager::Insert( REGENID id, UnitRegenInfo * pUnitRegenInfo )
{
	const SectorGridInfo& rGridInfo = m_pFieldInfo->SectorGridInfoRef;

    // 주의 : 위치 값이 여러 섹터에 찍힐수 있음. 
	const SECTOR_IDX sector_index = rGridInfo.GetSectorIndex( &pUnitRegenInfo->GetStartPos(0) );
    pUnitRegenInfo->SetSectorIndex( sector_index );
	// TODO(WAVERIX) 에러 검증은 안하고 있다. 확인이 요구된다.
	m_UnitHash.insert( UNITREGENINFO_HASH::value_type(id, pUnitRegenInfo) );
}
