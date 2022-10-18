#include "StdAfx.h"
#include ".\UnitRegenManager.h"
#include ".\GameField.h"
#include <functional>
#include <Battle.h>

// �ӽ�
#include "Monster.h"


UnitRegenManager::_POOL UnitRegenManager::m_Pool;
UnitRegenManager::UnitRegenManager(void)
:	m_pUnitRegenInfoManager(NULL)
,	m_pGameField(NULL)
{
	
}
void UnitRegenManager::CreatePool( DWORD dwPoolSize )
{
	m_Pool.Initialize( dwPoolSize, 0, "UnitRegenManager", AssertionLogger4Pool_Server );
	UnitRegen::SetPool( &m_Pool );
}

void UnitRegenManager::Release()
{
	SetUnitRegenInfoManager( NULL );
	SetGameField( NULL );

	std::for_each( m_WaitingUnitRegenHash.begin(), m_WaitingUnitRegenHash.end(), std::bind1st(std::mem_fun(&UnitRegenManager::free), this) );
	m_WaitingUnitRegenHash.clear();

	m_bPreRegen = true;
	m_setRegenSector.clear();
}

void UnitRegenManager::free( UNITREGEN_PAIR value_type )
{
	UnitRegen::FREE(value_type.second);
}

void UnitRegenManager::Init( GameField * pField, UnitRegenInfoManager * pInfoMgr, bool bPreRegen )
{
	SetGameField( pField );
	SetUnitRegenInfoManager( pInfoMgr );	// Lobby�� pInfoMgr�� NULL �̴�.

	m_bPreRegen = bPreRegen;
	m_setRegenSector.clear();

	if( m_bPreRegen && pInfoMgr )
	{
		// ���������� ������츸 MGR�� �����Ѵ�.
		UNITREGENINFO_HASH & rUnitInfoHash = pInfoMgr->GetUnitRegenInfoHash();

		std::for_each( rUnitInfoHash.begin(), rUnitInfoHash.end(), 
			std::bind1st(std::mem_fun(&UnitRegenManager::SpawnAll), this) );
	}
}

void UnitRegenManager::Update()
{
	UNITREGEN_HASH::iterator it = m_WaitingUnitRegenHash.begin(), end = m_WaitingUnitRegenHash.end();

	while( it != end )
	{
		UnitRegen * pUnit = (it->second); 

		if( pUnit->IsRegenTime() )
		{
			// �ش� ���� ��ó�� �÷��̾ ������ ������ ���� �ʴ´�.
			Sector *pSector = m_pGameField->GetSector( pUnit->GetUnitRegenInfo()->GetSectorIndex() );
			if( !pSector || !pSector->GetReferenceCount() )
			{
				++it;
				continue;
			}

            //_NA_0_20100222_UNIT_TRIGGERS_REGEN = {
            UnitRegenInfo* const unit_regen_info = pUnit->GetUnitRegenInfo();
            ASSERT(unit_regen_info);

            const bool is_trigger_regen = unit_regen_info->IsRegenTypeTrigger();
            if (is_trigger_regen == true)
            {
                SUNLOG(eDEV_LOG, "[UnitRegenManager] triggerRegen�� UnitRegen�� ���� �ȴ�."
                                 "(AddOnWaiting ���� �ʴ´�)"
                                 "DubugName = %s, WutFileName = %s",
                                 unit_regen_info->GetDebugName(),
                                 unit_regen_info->GetWutFileName());

                m_WaitingUnitRegenHash.erase(it++);
                UnitRegen::FREE(pUnit);

				continue;
            }
            //}
            if (m_pGameField->SpawnMonster(pUnit->GetUnitRegenInfo()))
			{
				m_WaitingUnitRegenHash.erase(it++);
				UnitRegen::FREE( pUnit );
			}
			else 
			{
				pUnit->Restart();
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}

//_NA_0_20100706_RANDOM_REGEN ����
bool UnitRegenManager::Spawn( UnitRegenInfo * pUnitInfo )
{
	if( !pUnitInfo->IsRandomRegen() )
	{
		return false;
	}

	if( !m_pGameField->SpawnMonster( pUnitInfo ) )
	{
		return false;
	}

	return true;
}

void UnitRegenManager::SpawnAll( UNITREGENINFO_PAIR value_type )
{
	UnitRegenInfo * pUnitRegenInfo =  value_type.second;

    // _NA_0_20100222_UNIT_TRIGGERS_REGEN
    // ���� ���� ���� �Ǵ� �ʱ� ���� �Ұ�
    // RegenTime �� ���ؼ� �Ұ�.
    // ACTION_SHOW_OR_HIDE_UNIT�� ���ؼ� ���� SpawnMonster 

    const bool is_trigger_regen = pUnitRegenInfo->IsRegenTypeTrigger();
    if (is_trigger_regen == true)
        return;

    if (!m_pGameField->SpawnMonster(pUnitRegenInfo))
	{
		SUNLOG( eCRITICAL_LOG, "�ʱ� ������ �� �� �����ϴ�.  DubugName = %s, WutFileName = %s"
			, pUnitRegenInfo->GetDebugName(), pUnitRegenInfo->GetWutFileName() );
	}
}

void UnitRegenManager::SpawnSector( DWORD dwSectorIndex )
{
	if( m_bPreRegen || !GetUnitRegenInfoManager() )	return;

	// �̹� ������ �������� üũ
	SET_SECTOR_REGEN_ITR setItr = m_setRegenSector.find( dwSectorIndex );
	if( setItr != m_setRegenSector.end() )
		return;

	// �ش� ���Ϳ� �ش��ϴ� ������������ �����Ѵ�.
	UNITREGENINFO_HASH & rUnitInfoHash = GetUnitRegenInfoManager()->GetUnitRegenInfoHash();
	UNITREGENINFO_HASH_ITR itend = rUnitInfoHash.end();
	for( UNITREGENINFO_HASH_ITR it = rUnitInfoHash.begin(); it != itend ; ++it )
	{
		UnitRegenInfo *pUnitRegenInfo = it->second;
		if( pUnitRegenInfo->GetSectorIndex() == dwSectorIndex )
		{
			SpawnAll( *it );
		}
	}

	// �� ������ ������ �Ϸ������� �����Ѵ�.
	m_setRegenSector.insert( dwSectorIndex );
}

// ���� ����
void UnitRegenManager::AddOnWaiting( REGENID id, DWORD SomeRegenTime )
{
	UnitRegenInfo * pUnitRegenInfo = GetUnitRegenInfoManager()->FindUnit(id);
    if( !pUnitRegenInfo ) {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] Can't Find RegenID %d", id );
        return;
    }

    if (pUnitRegenInfo->IsRegenTypeTrigger() == true)
        return;

	UnitRegen * pUnit = UnitRegen::ALLOC();
	pUnit->Init( pUnitRegenInfo );

	if( 0 != SomeRegenTime )
		pUnit->SetRegenTime( SomeRegenTime );

	//ASSERT( NULL == Find(pUnitRegenInfo->GetID()) );  //�ߺ��˻�
    if( NULL != Find(pUnitRegenInfo->GetID()) ) {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] Find pUnitRegenInfo->GetID() %d", id );
    }
	Insert( pUnitRegenInfo->GetID(), pUnit );
}

