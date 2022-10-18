
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#ifndef SHOP_MANAGER_SHOP_MANAGER_H
#define SHOP_MANAGER_SHOP_MANAGER_H
#pragma once

//==================================================================================================
/// ShopManager class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 12. 3(日)
    @remark
        - 
    @note
        - 
    @history 
        - 
*/
//==================================================================================================

#include "ShopDefine.h"
#include "Define.ShopServer.h"
#include "Protocol.ShopServer.h"


#include "SingleItem.h"
#include "PackageItem.h"
#include "ShopItemPrice.h"
#include "PresentRecvBox.h"
#include "ShopUser.h"

class ShopCategory;

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
class ShopPartition;
#else
#include "ItemCategory.h"
#include "PackageCategory.h"
#endif

//==================================================================================================

class ShopManager
{
public:
    //typedef STLX_HASH_MAP<BYTE/*eSHOP_CATEGORY*/,ShopCategory*>   CT_HASH;

#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    typedef STLX_HASH_MAP<eSHOP_CATEGORY,ShopCategory*> CT_HASH;
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

    typedef STLX_HASH_MAP<USERGUID,ShopUser*>      SHOPUSER_HASH;
    typedef STLX_HASH_MAP<ITEMGUID,ShopItem*>      SHOPITEM_HASH;
    typedef STLX_HASH_MAP<ITEMGUID,ShopItemPrice*> SHOPITEMPRICE_HASH;
    //
    ShopManager();
    virtual ~ShopManager();
    //
    virtual void Init();
    virtual void Release();
    virtual void Update();
    virtual void Clear();
    //

    // Shop서버로 부터 오는 CMD 패킷 처리

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    virtual RC::eSHOP_RESULT Serialize(eSHOP_PARTITION partition_index, eSHOP_CATEGORY category_index, BYTE* stream, uint size);
    void BuildShopItem(const tblSellItemContainer& container);
#else
    virtual RC::eSHOP_RESULT Serialize(BYTE Sep, BYTE Category, BYTE Page, 
        BYTE* pStream INOUT, WORD& Size INOUT,
        eSHOP_SERIALIZE se);
#endif
    virtual RC::eSHOP_RESULT ShopManager::SerializeItemDetail(ITEMGUID Guid,
        BYTE* pStream INOUT, WORD& Size INOUT,
        eSHOP_SERIALIZE se);

    virtual void BuildShopItem(DWORD Count, tblSellItem* pItem IN);
    virtual void BuildPackageItem(DWORD Count, tblSellPackage* pPackage IN);
    virtual void BuildTree(DWORD Count, tblItemBranch* pBranch IN);

    // 프로토콜 처리
    virtual RC::eSHOP_RESULT CanRequestBuy(USERGUID user_guid);
    virtual RC::eSHOP_RESULT DoRequestBuy(USERGUID user_guid);
    virtual RC::eSHOP_RESULT OnResultBuy(USERGUID user_guid, const CashData& cash_data);
    virtual RC::eSHOP_RESULT DoUserCash(USERGUID user_guid);
    virtual RC::eSHOP_RESULT OnResultUserCash(USERGUID user_guid, const CashData& cash_data);
    // manage object aloocators
    virtual ShopUser* CreateShopUser();
    virtual void DestroyShopUser(ShopUser* pUser);
    virtual void DestroyShopItem(ShopItem* p);

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    ShopCategory* FindCategory(eSHOP_PARTITION partition_index, eSHOP_CATEGORY category_index);
    virtual ShopItem* CreateShopItem(eSHOP_PARTITION partition_index);
#else
    virtual ShopItem* CreateShopItem(eSHOP_CATEGORY_SEPERATE sep);
    virtual ShopCategory* CreateShopCategory(eSHOP_CATEGORY_SEPERATE sep);
    virtual void DestroyShopCategory(ShopCategory* p);
    ShopCategory* FindShopCategory(const eSHOP_CATEGORY_SEPERATE ct_se, const eSHOP_CATEGORY ct);
    void AddShopCategory(const eSHOP_CATEGORY_SEPERATE ct_se, ShopCategory* p);
    int GetShopCategoryCount(const eSHOP_CATEGORY_SEPERATE ct_se) const;
    ShopManager::CT_HASH& GetShopCategoryHash(const eSHOP_CATEGORY_SEPERATE ct_se);
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

    virtual ShopItemPrice* CreateShopItemPrice();
    virtual void DestroyShopItemPrice(ShopItemPrice* p);
    //
    BOOLEAN HaveCash(USERGUID user_guid, CASH cash);
    //__NA_00387_20070605_SHOP_MILEAGE2
    BOOLEAN HaveMileage(USERGUID user_guid, DWORD mileage);
    //_NA00XXXX_090111_CASH_TYPE_SHARED_
    BOOLEAN HavePPCash(USERGUID user_guid, CASH pp_cash);
    //void OnChangedShopItem(BYTE cmd, tblSellItem& rItem);
    //
    void OnRemoveShopUser(USERGUID user_guid);
    // not implemented
    //virtual RC::eSHOP_RESULT CanBuyHistory();
    //virtual RC::eSHOP_RESULT DoReqBuyHistory();
    //virtual RC::eSHOP_RESULT ResultBuy();
    //
    //


    bool AddShopItem(ShopItem* p);
    bool AddShopItemPrice(ShopItemPrice* p);
    //----------------------------------------------------------------------------------------------
    // ShopItemPrice
    ShopItemPrice* FindShopItemPrice(const PRICEGUID guid);
    //----------------------------------------------------------------------------------------------
    // ShopItem
    ShopItem* FindShopItem(const ITEMGUID guid);
    //----------------------------------------------------------------------------------------------
    // ShopUser
    ShopUser* FindShopUser(const USERGUID guid);
    void AddShopUser(ShopUser* p);
    //----------------------------------------------------------------------------------------------

public:
    void Initialize_DefaultUserPool(DWORD size);
    template<class Operator>
    void for_each(Operator& op);
    void Display();

private:
    __BOOL_PROPERTY(Transaction);

    SHOPUSER_HASH       m_ShopUserHash;
    SHOPITEM_HASH       m_ShopItemHash;
    SHOPITEMPRICE_HASH  m_ShopItemPriceHash;
    // pool...
    util::CMemoryPoolFactory<SingleItem>      m_SingleItemPool;
    util::CMemoryPoolFactory<PackageItem>     m_PackageItemPool;

    util::CMemoryPoolFactory<ShopItemPrice>   m_ShopItemPricePool;
    util::CMemoryPoolFactory<ShopUser>* m_pDefaultShopUserPool;
    util::CMemoryPoolFactory<PresentRecvBox::PresentInfo> m_PresentInfoPool;


#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    ShopPartition*  shop_partition_[SHOP_PARTITION_MAXCOUNT];
#else
    util::CMemoryPoolFactory<PackageCategory> m_PackageCategoryPool;
    util::CMemoryPoolFactory<ItemCategory>    m_ItemCategoryPool;
    CT_HASH             m_CategoryHash[SHOP_SEPERATE_MAX];
#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

};

//==================================================================================================
#ifndef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
inline ShopCategory* ShopManager::FindShopCategory(const eSHOP_CATEGORY_SEPERATE ct_se,
                                                   const eSHOP_CATEGORY ct)
{
    if (ct_se >= SHOP_SEPERATE_MAX) {
        return NULL;
    }
    CT_HASH::iterator it = m_CategoryHash[ct_se].find(ct);
    if (it == m_CategoryHash[ct_se].end()) {
        return NULL;
    }
    return it->second;
}

inline void ShopManager::AddShopCategory(const eSHOP_CATEGORY_SEPERATE ct_se, ShopCategory* p)
{
    if (ct_se >= SHOP_SEPERATE_MAX) {
        return;
    }
    std::pair<CT_HASH::iterator, bool> ret = \
        m_CategoryHash[ct_se].insert(CT_HASH::value_type(p->GetCategory(),p));
    ASSERT(ret.second && "ShopCategory Add Failed");
}

inline int ShopManager::GetShopCategoryCount(const eSHOP_CATEGORY_SEPERATE ct_se) const
{
    if (ct_se >= SHOP_SEPERATE_MAX) {
        return 0;
    }
    return (int)m_CategoryHash[ct_se].size();
}

inline ShopManager::CT_HASH& ShopManager::GetShopCategoryHash(const eSHOP_CATEGORY_SEPERATE ct_se)
{
    return m_CategoryHash[ct_se];
}

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

inline bool ShopManager::AddShopItem(ShopItem* p)
{
    std::pair<SHOPITEM_HASH::iterator, bool> ret = \
        m_ShopItemHash.insert(SHOPITEM_HASH::value_type(p->GetGuid(),p));
    ASSERT(ret.second && "Whole ShopItem Add Failed");
    return ret.second;
}

inline bool ShopManager::AddShopItemPrice(ShopItemPrice* p)
{
    std::pair<SHOPITEMPRICE_HASH::iterator, bool> ret = \
        m_ShopItemPriceHash.insert(SHOPITEMPRICE_HASH::value_type(p->GetPriceGuid(),p));
    ASSERT(ret.second && "Whole ShopItemPrice Add Failed");
    return ret.second;
}

//--------------------------------------------------------------------------------------------------
// ShopItemPrice
inline ShopItemPrice* ShopManager::FindShopItemPrice(const PRICEGUID guid)
{
    SHOPITEMPRICE_HASH::iterator it = m_ShopItemPriceHash.find(guid);
    if (it == m_ShopItemPriceHash.end()) {
        return NULL;
    }
    return it->second;
}

//--------------------------------------------------------------------------------------------------
// ShopItem
inline ShopItem* ShopManager::FindShopItem(const ITEMGUID guid)
{
    SHOPITEM_HASH::iterator it = m_ShopItemHash.find(guid);
    if (it == m_ShopItemHash.end()) {
        return NULL;
    }
    return it->second;
}

//--------------------------------------------------------------------------------------------------
// ShopUser
inline ShopUser* ShopManager::FindShopUser(const USERGUID guid)
{
    SHOPUSER_HASH::iterator it = m_ShopUserHash.find(guid);
    if (it == m_ShopUserHash.end()) {
        return NULL;
    }
    return it->second;
}

inline void ShopManager::AddShopUser(ShopUser* p)
{
    std::pair<SHOPUSER_HASH::iterator, bool> ret = \
        m_ShopUserHash.insert(SHOPUSER_HASH::value_type(p->GetUserGuid(),p));
    ASSERT(ret.second && "ShopUser Add Failed");
}

#endif //SHOP_MANAGER_SHOP_MANAGER_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
