#include "StdAfx.h"
#include ".\unitgroupinfomanager.h"

UnitGroupInfoManager::UnitGroupInfoManager(void)
{
}

UnitGroupInfoManager::~UnitGroupInfoManager(void)
{
	for_each( m_UnitGroupHash.begin(), m_UnitGroupHash.end(), PairDeleter() );
	m_UnitGroupHash.clear();
}
