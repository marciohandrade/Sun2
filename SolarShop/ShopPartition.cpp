
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

#include "ShopPartition.h"
#include "ShopCategory.h"


//===============================================================================================
//
// class ShopPartition
//
//===============================================================================================
ShopPartition::ShopPartition()
: root_category_(NULL)
{
    category_memory_pool_ = new util::CMemoryPoolFactory<ShopCategory>();
}

ShopPartition::~ShopPartition()
{
    delete category_memory_pool_;
}

void ShopPartition::Initialize()
{
    category_memory_pool_->Initialize(20, 20);
    category_hashmap_.clear();
    category_stack_.clear();
    root_category_ = NULL;
}

void ShopPartition::Release()
{
    category_hashmap_.clear();
    category_stack_.clear();
    root_category_ = NULL;
    category_memory_pool_->Release();
}


bool ShopPartition::BuildCategory()
{
    category_build_success_ = true;
    on_build_category();
    return category_build_success_;
}


ShopCategory* ShopPartition::BeginCategory(eSHOP_CATEGORY category_index)
{
    ShopCategory* category = NULL;
    if (root_category_ == NULL)
    {
        category = CreateCategory(category_index);
        ASSERT(category);
        if (category)
            root_category_ = category;
        else
            category_build_success_ = false;
    }
    else
    {
        category = SuspendCategory(category_index);
    }

    if (category)
    {
        category_stack_.push_front(category);
    }

    return category;
}

void ShopPartition::EndCategory()
{
    if (category_stack_.empty())
        category_build_success_ = false;
    else
        category_stack_.pop_front();
}

ShopCategory* ShopPartition::SuspendCategory(eSHOP_CATEGORY category_index)
{
    if (category_stack_.empty())
    {
        category_build_success_ = false;
        return NULL;
    }

    ShopCategory* parent = category_stack_.front();
    ASSERT(parent);
    if (parent == NULL)
    {
        category_build_success_ = false;
        return NULL;
    }

    ShopCategory* category = CreateCategory(category_index);
    ASSERT(category);
    if (category == NULL)
    {
        category_build_success_ = false;
        return NULL;
    }

    parent->AttachCategory(category);

    return category;
}


ShopCategory* ShopPartition::CreateCategory(eSHOP_CATEGORY category_index)
{
    ShopCategory* category = FindCategory(category_index);
    if (category)
        return NULL;

    ASSERT(category_memory_pool_);
    if (category_memory_pool_ == NULL)
        return NULL;

    category = (ShopCategory*)category_memory_pool_->Alloc();
    ASSERT(category);
    if (category)
    {
        category->Initialize(category_index, shopitem_count_per_page());
        category_hashmap_[category_index] = category;
        return category;
    }
    return NULL;
}



//===============================================================================================
//
// class SingleItemPartition
//
//===============================================================================================
SingleItemPartition::SingleItemPartition()
{
}

SingleItemPartition::~SingleItemPartition()
{
}

void SingleItemPartition::on_build_category()
{
    BeginCategory(SHOP_CATEGORY_ROOT);
        BeginCategory(SHOP_CATEGORY_ALL);
            BeginCategory(SHOP_CATEGORY_EQUIP_ALL);
                SuspendCategory(SHOP_CATEGORY_EQUIP_HELMET);
                SuspendCategory(SHOP_CATEGORY_EQUIP_ARMOR);
                SuspendCategory(SHOP_CATEGORY_EQUIP_GLOVES);
                SuspendCategory(SHOP_CATEGORY_EQUIP_PANTS);
                SuspendCategory(SHOP_CATEGORY_EQUIP_BOOTS);
                SuspendCategory(SHOP_CATEGORY_EQUIP_PROTECTOR);
                SuspendCategory(SHOP_CATEGORY_EQUIP_BELT);
                SuspendCategory(SHOP_CATEGORY_EQUIP_SHIRTS);
                SuspendCategory(SHOP_CATEGORY_EQUIP_WEAPON);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_SKIN_ALL);
                SuspendCategory(SHOP_CATEGORY_SKIN_HELMET);
                SuspendCategory(SHOP_CATEGORY_SKIN_ARMOR);
                SuspendCategory(SHOP_CATEGORY_SKIN_GLOVES);
                SuspendCategory(SHOP_CATEGORY_SKIN_PENTS);
                SuspendCategory(SHOP_CATEGORY_SKIN_BOOTS);
                SuspendCategory(SHOP_CATEGORY_SKIN_PROTECTOR);
                SuspendCategory(SHOP_CATEGORY_SKIN_BELT);
                SuspendCategory(SHOP_CATEGORY_SKIN_SHIRTS);
                SuspendCategory(SHOP_CATEGORY_SKIN_WEAPON);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_CUSTOMIZING_ALL);
                SuspendCategory(SHOP_CATEGORY_CUSTOMIZING_HAIR);
                SuspendCategory(SHOP_CATEGORY_CUSTOMIZING_FACE);
                SuspendCategory(SHOP_CATEGORY_CUSTOMIZING_ETC);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_ITEM_ALL);
                SuspendCategory(SHOP_CATEGORY_ITEM_LUCKY);
                SuspendCategory(SHOP_CATEGORY_ITEM_MATERIAL);
                SuspendCategory(SHOP_CATEGORY_ITEM_GUILD);
                SuspendCategory(SHOP_CATEGORY_ITEM_TICKET);
                SuspendCategory(SHOP_CATEGORY_ITEM_STRENGTHEN);
                SuspendCategory(SHOP_CATEGORY_ITEM_FUNCTIONALITY);
                SuspendCategory(SHOP_CATEGORY_ITEM_CONVENIENCE);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_PET_ALL);
                SuspendCategory(SHOP_CATEGORY_PET);
                SuspendCategory(SHOP_CATEGORY_PET_FEED);
                SuspendCategory(SHOP_CATEGORY_PET_SKIN);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_VEHICLE_ALL);
                SuspendCategory(SHOP_CATEGORY_VEHICLE);
                SuspendCategory(SHOP_CATEGORY_VEHICLE_HEAD);
                SuspendCategory(SHOP_CATEGORY_VEHICLE_BODY);
                SuspendCategory(SHOP_CATEGORY_VEHICLE_LEGS);
            EndCategory();
        EndCategory();
        SuspendCategory(SHOP_CATEGORY_PCROOM);
        SuspendCategory(SHOP_CATEGORY_PPCASH);
        SuspendCategory(SHOP_CATEGORY_PERIOD_EXTEND);
    EndCategory();

    ShopCategory* category = NULL;
    category = FindCategory(SHOP_CATEGORY_PCROOM);
    ASSERT(category);
    category->allowed_visitors(false);

    category = FindCategory(SHOP_CATEGORY_PPCASH);
    ASSERT(category);
    category->allowed_visitors(false);

    category = FindCategory(SHOP_CATEGORY_PERIOD_EXTEND);
    ASSERT(category);
    category->allowed_visitors(false);
}



//===============================================================================================
//
// class PackageItemPartition
//
//===============================================================================================
PackageItemPartition::PackageItemPartition()
{
}

PackageItemPartition::~PackageItemPartition()
{
}

void PackageItemPartition::on_build_category()
{
    BeginCategory(SHOP_CATEGORY_ROOT);
        SuspendCategory(SHOP_CATEGORY_ALL);
    EndCategory();
}




//===============================================================================================
//
// class MileageItemPartition
//
//===============================================================================================
MileageItemPartition::MileageItemPartition()
{
}

MileageItemPartition::~MileageItemPartition()
{
}

void MileageItemPartition::on_build_category()
{
    BeginCategory(SHOP_CATEGORY_ROOT);
        BeginCategory(SHOP_CATEGORY_ALL);
            BeginCategory(SHOP_CATEGORY_EQUIP_ALL);
                SuspendCategory(SHOP_CATEGORY_EQUIP_HELMET);
                SuspendCategory(SHOP_CATEGORY_EQUIP_ARMOR);
                SuspendCategory(SHOP_CATEGORY_EQUIP_GLOVES);
                SuspendCategory(SHOP_CATEGORY_EQUIP_PANTS);
                SuspendCategory(SHOP_CATEGORY_EQUIP_BOOTS);
                SuspendCategory(SHOP_CATEGORY_EQUIP_PROTECTOR);
                SuspendCategory(SHOP_CATEGORY_EQUIP_BELT);
                SuspendCategory(SHOP_CATEGORY_EQUIP_SHIRTS);
                SuspendCategory(SHOP_CATEGORY_EQUIP_WEAPON);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_SKIN_ALL);
                SuspendCategory(SHOP_CATEGORY_SKIN_HELMET);
                SuspendCategory(SHOP_CATEGORY_SKIN_ARMOR);
                SuspendCategory(SHOP_CATEGORY_SKIN_GLOVES);
                SuspendCategory(SHOP_CATEGORY_SKIN_PENTS);
                SuspendCategory(SHOP_CATEGORY_SKIN_BOOTS);
                SuspendCategory(SHOP_CATEGORY_SKIN_PROTECTOR);
                SuspendCategory(SHOP_CATEGORY_SKIN_BELT);
                SuspendCategory(SHOP_CATEGORY_SKIN_SHIRTS);
                SuspendCategory(SHOP_CATEGORY_SKIN_WEAPON);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_CUSTOMIZING_ALL);
                SuspendCategory(SHOP_CATEGORY_CUSTOMIZING_HAIR);
                SuspendCategory(SHOP_CATEGORY_CUSTOMIZING_FACE);
                SuspendCategory(SHOP_CATEGORY_CUSTOMIZING_ETC);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_ITEM_ALL);
                SuspendCategory(SHOP_CATEGORY_ITEM_LUCKY);
                SuspendCategory(SHOP_CATEGORY_ITEM_MATERIAL);
                SuspendCategory(SHOP_CATEGORY_ITEM_GUILD);
                SuspendCategory(SHOP_CATEGORY_ITEM_TICKET);
                SuspendCategory(SHOP_CATEGORY_ITEM_STRENGTHEN);
                SuspendCategory(SHOP_CATEGORY_ITEM_FUNCTIONALITY);
                SuspendCategory(SHOP_CATEGORY_ITEM_CONVENIENCE);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_PET_ALL);
                SuspendCategory(SHOP_CATEGORY_PET);
                SuspendCategory(SHOP_CATEGORY_PET_FEED);
                SuspendCategory(SHOP_CATEGORY_PET_SKIN);
            EndCategory();
            BeginCategory(SHOP_CATEGORY_VEHICLE_ALL);
                SuspendCategory(SHOP_CATEGORY_VEHICLE);
                SuspendCategory(SHOP_CATEGORY_VEHICLE_HEAD);
                SuspendCategory(SHOP_CATEGORY_VEHICLE_BODY);
                SuspendCategory(SHOP_CATEGORY_VEHICLE_LEGS);
            EndCategory();
        EndCategory();
        SuspendCategory(SHOP_CATEGORY_PCROOM);
        SuspendCategory(SHOP_CATEGORY_PPCASH);
        SuspendCategory(SHOP_CATEGORY_PERIOD_EXTEND);
    EndCategory();
}





#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
