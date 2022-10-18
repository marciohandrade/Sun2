
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



//=============================================================================================================================
/// PackageCategory class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2007. 1. 11
	@remark
		- 
	@note
		- 
	@history 
		- 2007/1/11: Package 처리를 하면서 같은 범주에 묶기 위해 ItemCategory와 PackageCategory를 나눔.
*/
//=============================================================================================================================
#pragma once

#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#include "ShopCategory.h"
#include "PackageItem.h"

#define PACK_NUM_PER_PAGE		(3)

class ShopManager;

class PackageCategory : public ShopCategory
{__DECL_CUSTOMPOOL_PTR(PackageCategory)
public:
	PackageCategory();
	virtual ~PackageCategory();

	virtual eSHOP_CATEGORY_SEPERATE GetSeperateType() { return SHOP_PACKAGE; }
	virtual void		Release();
	virtual void		Serialize(ShopManager& mgr IN, BYTE page, ArchiveOfStream& ar, eSHOP_SERIALIZE se);
	virtual void		Display();
	virtual int			GetItemNumPerPage() { return 3; }
};

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
