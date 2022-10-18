//=============================================================================================================================
/// UnitListOfGroup class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 31
	@note
		- NPC -> UnitListOfGroup
	@purpose
		- NPC그룹에 속한 다른 NPC에 AIMessage를 전달하기위한 grouping
	@history
		
*/
//=============================================================================================================================

#pragma once


#include <MemoryPoolFactory.h>

typedef STLX_HASH_MAP<REGENID, NPC *>	UNIT_MEMBER_HASH;

class UnitListOfGroup
{
	__DECL_CUSTOMPOOL_PTR( UnitListOfGroup )
public:
	UnitListOfGroup(void);
	~UnitListOfGroup(void);

	void Add( REGENID id, NPC * pNPC ) 
	{ 
		std::pair<UNIT_MEMBER_HASH::iterator,bool> ret = m_Hash.insert( UNIT_MEMBER_HASH::value_type(id, pNPC) ); 
		ASSERT(ret.second && "monster is already exist in UnitListOfGroup" );
	}
	void Remove( REGENID id )
	{ 
		UNIT_MEMBER_HASH::iterator it = m_Hash.find( id );
		if( it != m_Hash.end() )
			m_Hash.erase( it );
		else
		{
			ASSERT( "monster is exist in UnitListOfGroup" );
		}
	}

	VOID SendAIMessage( AI_MSG *pMsg, WORD wSize, DWORD dwDelay, DWORD dwExceptObjKey );

	bool IsAlive( REGENID id ) { return m_Hash.find(id) != m_Hash.end(); }

	UNIT_MEMBER_HASH & GetUnitMemberHash() { return m_Hash; }
private:
//	__PROPERTY( GROUPID, GroupID );
	UNIT_MEMBER_HASH m_Hash;
};
