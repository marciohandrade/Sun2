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

    // ���� : ��ġ ���� ���� ���Ϳ� ������ ����. 
	const SECTOR_IDX sector_index = rGridInfo.GetSectorIndex( &pUnitRegenInfo->GetStartPos(0) );
    pUnitRegenInfo->SetSectorIndex( sector_index );
	// TODO(WAVERIX) ���� ������ ���ϰ� �ִ�. Ȯ���� �䱸�ȴ�.
	m_UnitHash.insert( UNITREGENINFO_HASH::value_type(id, pUnitRegenInfo) );
}
