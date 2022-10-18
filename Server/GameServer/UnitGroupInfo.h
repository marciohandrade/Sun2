
//=============================================================================================================================
/// UnitGroupInfo class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- WUT에 읽은 UnitGroup정보, 공유 정보
		- 현재까지 사용되지 않는 클래스, 추후 삭제 예정
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
