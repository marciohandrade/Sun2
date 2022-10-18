//=============================================================================================================================
/// UnitGroupInfoManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- 필드 마다 존재하는 그룹들의 정보
		- 필드에 종속적인 클래스
		- 현재까지 사용되지 않는 클래스, 추후 삭제 예정
	@purpose
		- GROUPID로 UnitGroupInfo를 찾을 때 
	@history
		
*/
//=============================================================================================================================

#pragma once

#include "UnitGroupInfo.h"

class UnitGroupInfoManager
{
	typedef STLX_HASH_MAP<UNITGROUPID, UnitGroupInfo *>		UNITGROUPINFO_HASH;
public:
	UnitGroupInfoManager(void);
	~UnitGroupInfoManager(void);

	__inline UnitGroupInfo * Find( UNITGROUPID id )
	{
		UNITGROUPINFO_HASH::iterator it = m_UnitGroupHash.find(id);
		if( it != m_UnitGroupHash.end() )
			return it->second;
		return NULL;
	}

	void Update();

	void Insert( UNITGROUPID id, UnitGroupInfo * p ) { m_UnitGroupHash.insert( UNITGROUPINFO_HASH::value_type(id, p) ); }

private:
	UNITGROUPINFO_HASH m_UnitGroupHash;
};
