#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL





#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



#include ".\itemcategory.h"
#include <ArchiveOfStream.h>
#include <algorithm>
#include "ShopManager.h"

__IMPL_CUSTOMPOOL_PTR(ItemCategory)
ItemCategory::ItemCategory()
{
}

ItemCategory::~ItemCategory()
{

}

void ItemCategory::Release()
{
	ShopCategory::Release();

}

void ItemCategory::Serialize(ShopManager& mgr IN, BYTE page, ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
	ShopCategory::Serialize(mgr, page, ar, se);

	switch(se)
	{
	case eSHOP_SERIALIZE_ITEM_LIST_SAVE:
		{

		}break;
	case eSHOP_SERIALIZE_ITEM_LIST_LOAD:
		{

		}break;
	}
}

void ItemCategory::Display()
{
	ShopCategory::Display();
	
}

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
