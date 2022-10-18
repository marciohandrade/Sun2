#include "StdAfx.h"
#include ".\unitgroupmanager.h"


UnitGroupManager::_POOL UnitGroupManager::m_Pool;
UnitGroupManager::UnitGroupManager(void)
{
}

UnitGroupManager::~UnitGroupManager(void)
{

}
void UnitGroupManager::Clear()
{
	std::for_each( m_Hash.begin(), m_Hash.end(), std::bind1st(std::mem_fun(&UnitGroupManager::destroy), this) );
	m_Hash.clear();
}

void UnitGroupManager::CreatePool( DWORD dwPoolSize )
{
	m_Pool.Initialize( dwPoolSize, 0, "UnitGroupManager", AssertionLogger4Pool_Server );
	UnitListOfGroup::SetPool( &m_Pool );
}
// NPC::OnEnterField();
UnitListOfGroup * UnitGroupManager::Join( UNITGROUPID id, REGENID regen_id, NPC * pNPC )
{
	UnitListOfGroup * pGroup = Find(id);
	if( !pGroup ) pGroup = create( id );

	pGroup->Add( regen_id, pNPC );

	return pGroup;
}
// NPC::OnLeaveField();
BOOL UnitGroupManager::Leave( UNITGROUPID id, REGENID regen_id )
{
	UnitListOfGroup * pGroup = Find(id);
	ASSERT( pGroup );
	if( !pGroup ) return FALSE;

	pGroup->Remove( regen_id );

	return TRUE;
}