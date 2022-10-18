#include "StdAfx.h"
#include ".\UnitRegenManager.h"
#include ".\GameField.h"
#include <functional>
#include <Battle.h>

// 임시
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
	SetUnitRegenInfoManager( pInfoMgr );	// Lobby는 pInfoMgr이 NULL 이다.

	m_bPreRegen = bPreRegen;
	m_setRegenSector.clear();

	if( m_bPreRegen && pInfoMgr )
	{
		// 리젠정보가 있을경우만 MGR가 동작한다.
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
			// 해당 섹터 근처에 플레이어가 없으면 리젠을 하지 않는다.
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
                SUNLOG(eDEV_LOG, "[UnitRegenManager] triggerRegen은 UnitRegen이 무시 된다."
                                 "(AddOnWaiting 하지 않는다)"
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

//_NA_0_20100706_RANDOM_REGEN 삭제
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
    // 섹터 별로 리젠 되는 초기 리젠 불가
    // RegenTime 에 의해서 불가.
    // ACTION_SHOW_OR_HIDE_UNIT를 통해서 직접 SpawnMonster 

    const bool is_trigger_regen = pUnitRegenInfo->IsRegenTypeTrigger();
    if (is_trigger_regen == true)
        return;

    if (!m_pGameField->SpawnMonster(pUnitRegenInfo))
	{
		SUNLOG( eCRITICAL_LOG, "초기 리젠을 할 수 없습니다.  DubugName = %s, WutFileName = %s"
			, pUnitRegenInfo->GetDebugName(), pUnitRegenInfo->GetWutFileName() );
	}
}

void UnitRegenManager::SpawnSector( DWORD dwSectorIndex )
{
	if( m_bPreRegen || !GetUnitRegenInfoManager() )	return;

	// 이미 리젠된 섹터인지 체크
	SET_SECTOR_REGEN_ITR setItr = m_setRegenSector.find( dwSectorIndex );
	if( setItr != m_setRegenSector.end() )
		return;

	// 해당 섹터에 해당하는 리젠정보들을 리젠한다.
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

	// 이 섹터의 리젠을 완료했음을 저장한다.
	m_setRegenSector.insert( dwSectorIndex );
}

// 리젠 시작
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

	//ASSERT( NULL == Find(pUnitRegenInfo->GetID()) );  //중복검사
    if( NULL != Find(pUnitRegenInfo->GetID()) ) {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] Find pUnitRegenInfo->GetID() %d", id );
    }
	Insert( pUnitRegenInfo->GetID(), pUnit );
}

