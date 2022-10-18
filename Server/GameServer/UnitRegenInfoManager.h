#pragma once
//=============================================================================================================================
/// UnitRegenInfoManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- 필드 마다 존재하는 유닛들의 정보
		- 필드에 종속적인 클래스
	@history
		
*/
//=============================================================================================================================

#include "UnitRegenInfo.h"

typedef STLX_HASH_MAP<REGENID, UnitRegenInfo *>		UNITREGENINFO_HASH;
typedef std::pair<REGENID, UnitRegenInfo *>			UNITREGENINFO_PAIR;
typedef UNITREGENINFO_HASH::iterator				UNITREGENINFO_HASH_ITR;

class UnitRegenInfoManager
{
public:
	UnitRegenInfoManager( FieldInfo *pFieldInfo );
	~UnitRegenInfoManager(void);

	__inline UnitRegenInfo * FindUnit( REGENID id )
	{
		UNITREGENINFO_HASH::iterator it = m_UnitHash.find(id);
		if( it != m_UnitHash.end() )
			return it->second;
		return NULL;
	}

	__inline int GetCount()									{ return (int)m_UnitHash.size(); }
	__inline UNITREGENINFO_HASH & GetUnitRegenInfoHash()	{ return m_UnitHash; }

	void Insert( REGENID id, UnitRegenInfo * pUnitRegenInfo );

private:
	UNITREGENINFO_HASH	m_UnitHash;
	FieldInfo *			m_pFieldInfo;
};















