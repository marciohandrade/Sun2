
//=============================================================================================================================
/// UnitGroupInfo class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- WUT�� ���� UnitGroup����, ���� ����
		- ������� ������ �ʴ� Ŭ����, ���� ���� ����
	@history
		
*/
//=============================================================================================================================

#pragma once

#include <hash_map>
#include "UnitRegenInfo.h"

class UnitGroupInfo
{
	typedef STLX_HASH_MAP<REGENID, UnitRegenInfo*>	GROUP_MEMBERID_HASH;
public:
	UnitGroupInfo(void);
	~UnitGroupInfo(void);

	BOOL FindMember( REGENID id ) { return (m_MemberIDHash.find(id) != m_MemberIDHash.end()); }
	void InsertMember( REGENID id, UnitRegenInfo * p ) { m_MemberIDHash.insert( GROUP_MEMBERID_HASH::value_type(id,p) ); }

	bool IsLeader( REGENID myid ) { return myid == GetLeaderRegenID(); }
private:
	__PROPERTY( UNITGROUPID, ID );
	__PROPERTY( REGENID, LeaderRegenID );
	__ARRAY_PROPERTY( DWORD, 2, ReservedData );

private:
	GROUP_MEMBERID_HASH m_MemberIDHash;
};
