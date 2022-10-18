#pragma once

#include <hash_map>

struct SPECIAL_ITEM_OWN_INFO;

struct SPECIAL_ITEM_INFO
{
	eITEMTYPE	m_eItemType;
	int			m_Count;
};

typedef STLX_HASH_MAP<eITEMTYPE, SPECIAL_ITEM_INFO>		HASHMAP_SPECIALITEMINFO;
typedef	HASHMAP_SPECIALITEMINFO::iterator				HASHMAP_SPECIALITEMINFO_ITR;
typedef std::pair<eITEMTYPE, SPECIAL_ITEM_INFO>			HASHMAP_SPECIALITEMINFO_PAIR;

class ItemOwnFlag
{
public:
	ItemOwnFlag(void) {}
	~ItemOwnFlag(void) {}

	VOID				Init();

	BOOL				IsSendToWorldServer( eITEMTYPE eItemType );
	BOOL				IsSendToAgentServer( eITEMTYPE eItemType );

	int					GetItemNum()	{ return (int)m_mapSpecialItemInfo.size(); }
	int					GetItemNum( eITEMTYPE eItemType );

	SPECIAL_ITEM_INFO *	FindItemInfo( eITEMTYPE eItemType );
	VOID				IncItemInfo( eITEMTYPE eItemType, int Count );
	BOOL				DecItemInfo( eITEMTYPE eItemType, int Count );

	int					GetAllSpecialItemInfoToWorld( SPECIAL_ITEM_OWN_INFO *pItemOwnInfo, int iMaxCount );
	void					QueryShoutItemsExist(BOOL& bShoutItem, BOOL& bSuperMasterItem, BOOL& bShoutConsumeItem);

private:
	HASHMAP_SPECIALITEMINFO	m_mapSpecialItemInfo;
};

