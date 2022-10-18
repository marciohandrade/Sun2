#include "StdAfx.h"
#include "ItemOwnFlag.h"
#include "ServerStructInPacket.h"

VOID ItemOwnFlag::Init()
{
	m_mapSpecialItemInfo.clear();
}

BOOL ItemOwnFlag::IsSendToWorldServer( eITEMTYPE eItemType )
{
	switch( eItemType )
	{
	case eITEMTYPE_SUPERMASTER:
	case eITEMTYPE_CHAT_SHOUT:
	case eITEMTYPE_CHAT_SHOUT_CONSUME:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL ItemOwnFlag::IsSendToAgentServer( eITEMTYPE eItemType )
{
	switch( eItemType )
	{
	case eITEMTYPE_SUPERMASTER:
	case eITEMTYPE_CHAT_SHOUT:
	case eITEMTYPE_CHAT_SHOUT_CONSUME:
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	case eITEMTYPE_GUILD_PVP_ENTRANCE:
#endif
		return TRUE;
	default:
		return FALSE;
	}
}

int ItemOwnFlag::GetItemNum( eITEMTYPE eItemType )
{
	SPECIAL_ITEM_INFO *pSItemInfo = FindItemInfo( eItemType );

	if( pSItemInfo )
	{
		return pSItemInfo->m_Count;
	}
	else
	{
		return 0;
	}
}

SPECIAL_ITEM_INFO * ItemOwnFlag::FindItemInfo( eITEMTYPE eItemType )
{
	HASHMAP_SPECIALITEMINFO_ITR it = m_mapSpecialItemInfo.find( eItemType );
	if( it != m_mapSpecialItemInfo.end() )
		return &it->second;

	return NULL;
}

VOID ItemOwnFlag::IncItemInfo( eITEMTYPE eItemType, int Count )
{
	SPECIAL_ITEM_INFO *pSItemInfo = FindItemInfo( eItemType );

	if( pSItemInfo )
	{
		pSItemInfo->m_Count += Count;
	}
	else
	{
		SPECIAL_ITEM_INFO NewSItemInfo;
		NewSItemInfo.m_eItemType = eItemType;
		NewSItemInfo.m_Count = Count;
		m_mapSpecialItemInfo.insert( HASHMAP_SPECIALITEMINFO_PAIR(eItemType, NewSItemInfo) );
	}
}

BOOL ItemOwnFlag::DecItemInfo( eITEMTYPE eItemType, int Count )
{
	SPECIAL_ITEM_INFO *pSItemInfo = FindItemInfo( eItemType );

	if( pSItemInfo )
	{
		if( pSItemInfo->m_Count <= Count )
			m_mapSpecialItemInfo.erase( eItemType );
		else
			pSItemInfo->m_Count -= Count;

		return TRUE;
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[ItemOwnFlag::RemoveItemInfo] Can't Remove ItemInfo[%d]", eItemType );
		return FALSE;
	}
}

int ItemOwnFlag::GetAllSpecialItemInfoToWorld( SPECIAL_ITEM_OWN_INFO *pItemOwnInfo, int iMaxCount )
{
	int iCount = 0;
	HASHMAP_SPECIALITEMINFO_ITR itend = m_mapSpecialItemInfo.end();
	for( HASHMAP_SPECIALITEMINFO_ITR it = m_mapSpecialItemInfo.begin(); it != itend ; it++ )
	{
		if( iCount >= iMaxCount )	break;

		SPECIAL_ITEM_INFO & SItemInfo = it->second;

		if( IsSendToWorldServer( SItemInfo.m_eItemType ) )
		{
			pItemOwnInfo[iCount].m_Apply = true;
			pItemOwnInfo[iCount].m_ItemType = SItemInfo.m_eItemType;
			pItemOwnInfo[iCount].m_Count = SItemInfo.m_Count;
			iCount++;
		}
	}

	return iCount;
}


void ItemOwnFlag::QueryShoutItemsExist(BOOL& bShoutItem, BOOL& bSuperMasterItem, BOOL& bShoutConsumeItem)
{
	if(FindItemInfo(eITEMTYPE_CHAT_SHOUT) != NULL)	bShoutItem = TRUE;
	if(FindItemInfo(eITEMTYPE_SUPERMASTER) != NULL) bSuperMasterItem = TRUE;
	if(FindItemInfo(eITEMTYPE_CHAT_SHOUT_CONSUME) != NULL) bShoutConsumeItem = TRUE;
}















