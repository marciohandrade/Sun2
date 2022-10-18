
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



//=============================================================================================================================
/// ShopCategory class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 12. 3(日)
	@remark
		- 
	@note
		- 
	@history 
		- 2007/1/12 : 인터페이스 클래스로 변경
*/
//=============================================================================================================================
#pragma once


#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

//===============================================================================================
//
// class Category
//
//===============================================================================================

class ShopItem;

class ShopCategory
{
public:
    ShopCategory();
    ~ShopCategory();

    void Initialize(eSHOP_CATEGORY category_index, uint shopitem_count_per_page);
    void AttachCategory(ShopCategory* child_category);

    void InsertShopItem(ShopItem* shopitem);
    void RemoveShopItem(ShopItem* shopitem);
    void RemoveShopItemAll();

    // 자신과 하위 계층의 아이템을 가져온다.
    void Search(STLX_DEQUE<ShopItem*>& shopitem_array, uint shopitem_filter);

    eSHOP_CATEGORY get_category_index() const;

    void allowed_visitors(bool allow);
    uint get_shopitem_count_per_page() const;
    uint get_shopitem_changed_count() const;

private:
    void child_shopitem_change();
    // fields...
    typedef STLX_VECTOR<ShopCategory*> ShopCategoryArray;
    typedef STLX_VECTOR<ShopItem*> ShopItemArray;
    //
    eSHOP_CATEGORY          category_index_;
    bool                    allowed_visitors_;
    uint                    child_shopitem_changed_count_;
    ShopCategory*           parent_;
    ShopCategoryArray   child_category_array_;
    ShopItemArray       child_shopitem_array_;
    uint                    shopitem_count_per_page_;
};


//---------------------------------------------------------------------------
inline eSHOP_CATEGORY ShopCategory::get_category_index() const
{
    return category_index_;
}

//---------------------------------------------------------------------------
inline void ShopCategory::allowed_visitors(bool allow)
{
    allowed_visitors_ = allow;
}

//---------------------------------------------------------------------------
inline uint ShopCategory::get_shopitem_count_per_page() const
{
    return shopitem_count_per_page_;
}

//---------------------------------------------------------------------------
inline uint ShopCategory::get_shopitem_changed_count() const
{
    return child_shopitem_changed_count_;
}


//===============================================================================================


#else // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


//===============================================================================================


#define ITEM_NUM_PER_PAGE		(10)

#include "ShopDefine.h"
#include <list>
#include <functional_custom.h>

class ShopManager;
class ArchiveOfStream;

class ShopCategory
{
public:
	ShopCategory();
	virtual ~ShopCategory();

	virtual eSHOP_CATEGORY_SEPERATE GetSeperateType() = 0;
	virtual void Release();
	virtual void Serialize(ShopManager& mgr IN, BYTE page, ArchiveOfStream& ar, eSHOP_SERIALIZE se);
	virtual void Display();

	int	GetElementCount() const { return (int)m_ShopItemList.size(); }
	int	GetTotalPageNum() { return (int)ceil((double)m_ShopItemList.size()/(double)GetItemNumPerPage()); }
	virtual int GetItemNumPerPage() { return 10; }

	SHOPITEM_LIST &		GetShopItemList() { return m_ShopItemList; }
	ShopItem *			FindShopItem(const ITEMGUID guid);	//< loop searching
	void				RemoveShopItem(ShopItem* pShopItem);
	void				AddShopItem(ShopItem* pShopItem);

	template< class Operator >
		void for_each(Operator& op);

private:
	__PROPERTY(eSHOP_CATEGORY, Category);
	SHOPITEM_LIST		m_ShopItemList;
};


template< class Operator >
void ShopCategory::for_each(Operator& op)
{
	std::for_each(m_ShopItemList.begin(), m_ShopItemList.end(), op);
	//		compose1(op, select2nd<SHOPITEM_HASH::value_type>()));
}

//===============================================================================================

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
