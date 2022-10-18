
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL





#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

#include ".\packagecategory.h"
#include <ArchiveOfStream.h>
#include <algorithm>

__IMPL_CUSTOMPOOL_PTR(PackageCategory)
PackageCategory::PackageCategory()
{
}

PackageCategory::~PackageCategory()
{

}

void PackageCategory::Release()
{
	ShopCategory::Release();
}

void PackageCategory::Serialize(ShopManager& mgr IN, BYTE page, ArchiveOfStream& ar, eSHOP_SERIALIZE se)
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

void PackageCategory::Display()
{
	ShopCategory::Display();
	
}

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
