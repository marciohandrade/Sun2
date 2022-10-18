
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#ifndef SOLAR_SHOP_SHOPPARTITION_H_
#define SOLAR_SHOP_SHOPPARTITION_H_


#if _MSC_VER > 1000
#pragma once
#endif


//===============================================================================================
//
// class ShopPartition
//
//===============================================================================================

class ShopCategory;
class ShopItem;

class ShopPartition
{
public:
    ShopPartition();
    virtual ~ShopPartition();

    void Initialize();
    void Release();

    bool BuildCategory();

    ShopCategory* FindCategory(eSHOP_CATEGORY category_index);
    
    virtual uint shopitem_count_per_page() const = 0;

protected:
    ShopCategory* BeginCategory(eSHOP_CATEGORY category_index);
    ShopCategory* SuspendCategory(eSHOP_CATEGORY category_index);
    void EndCategory();

protected:
    virtual void on_build_category() = 0;

private:
    ShopCategory* CreateCategory(eSHOP_CATEGORY category_index);

private:
    ShopCategory*   root_category_;
    bool            category_build_success_;
    STLX_DEQUE<ShopCategory*> category_stack_;

    typedef STLX_HASH_MAP<eSHOP_CATEGORY, ShopCategory*> CategoryHashMap;
    CategoryHashMap category_hashmap_;
    util::IVMemoryPool* category_memory_pool_;
};


//-----------------------------------------------------------------------------------
inline ShopCategory* ShopPartition::FindCategory(eSHOP_CATEGORY category_index)
{
    CategoryHashMap::iterator iter = category_hashmap_.find(category_index);
    if (iter != category_hashmap_.end())
        return iter->second;
    return NULL;
}


//===============================================================================================
//
// class SingleItemPartition
//
//===============================================================================================
class SingleItemPartition : public ShopPartition
{
public:
    SingleItemPartition();
    virtual ~SingleItemPartition();

    virtual uint shopitem_count_per_page() const { return 10; }

protected:
    virtual void on_build_category();
};



//===============================================================================================
//
// class PackageItemPartition
//
//===============================================================================================
class PackageItemPartition : public ShopPartition
{
public:
    PackageItemPartition();
    virtual ~PackageItemPartition();

    virtual uint shopitem_count_per_page() const { return 3; }

protected:
    virtual void on_build_category();
};



//===============================================================================================
//
// class MileageItemPartition
//
//===============================================================================================
class MileageItemPartition : public ShopPartition
{
public:
    MileageItemPartition();
    virtual ~MileageItemPartition();

    virtual uint shopitem_count_per_page() const { return 10; }

protected:
    virtual void on_build_category();
};



#endif // SOLAR_SHOP_SHOPPARTITION_H_

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
