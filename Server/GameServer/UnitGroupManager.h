//=============================================================================================================================
/// UnitGroupManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 31
	@note
		-	<< diagram >>
			GameField -> UnitGroupManager <>- UnitListOfGroup
			NPC -> UnitListOfGroup
	@history
		
*/
//=============================================================================================================================
#pragma once

#include "UnitListOfGroup.h"

typedef STLX_HASH_MAP<UNITGROUPID, UnitListOfGroup *>	UNIT_GROUP_HASH;

class UnitGroupManager
{
	typedef util::CMemoryPoolFactory<UnitListOfGroup> _POOL;
	typedef std::pair<UNITGROUPID, UnitListOfGroup *>	_UNIT_GROUP_PAIR;
public:
	UnitGroupManager(void);
	~UnitGroupManager(void);
	
	void Clear();

	static void CreatePool( DWORD dwPoolSize );

	UnitListOfGroup * Join( UNITGROUPID id, REGENID regen_id, NPC * pNPC );
	BOOL Leave( UNITGROUPID id, REGENID regen_id );

	__inline UnitListOfGroup * Find( UNITGROUPID id )
	{
		UNIT_GROUP_HASH::iterator it = m_Hash.find( id );
		if( it != m_Hash.end() )
			return it->second;
		return NULL;
	}
	// 디버그용
	__inline int GetCount() { return (int)m_Hash.size(); }
private:
	__inline UnitListOfGroup * create( UNITGROUPID id ) 
	{ 
		UnitListOfGroup * pGroup = UnitListOfGroup::ALLOC(); 
		//pGroup->SetGroupID( id );
		m_Hash[id] = pGroup;
		return pGroup;
	}
	__inline void destroy( _UNIT_GROUP_PAIR value_type ) 
	{ 
		UnitListOfGroup::FREE(value_type.second);
	}

	UNIT_GROUP_HASH m_Hash;
	static _POOL m_Pool;
};
