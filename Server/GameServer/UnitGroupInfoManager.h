//=============================================================================================================================
/// UnitGroupInfoManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- �ʵ� ���� �����ϴ� �׷���� ����
		- �ʵ忡 �������� Ŭ����
		- ������� ������ �ʴ� Ŭ����, ���� ���� ����
	@purpose
		- GROUPID�� UnitGroupInfo�� ã�� �� 
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
