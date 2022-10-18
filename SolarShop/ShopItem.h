
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#pragma once
#ifndef SOLAR_SHOP_ITEM_H
#define SOLAR_SHOP_ITEM_H
//==================================================================================================
/// ShopItem class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 12. 3(��)
    @remark
        - ������ �Ǹ��ϴ� ������ Ŭ����
    @note
        - ShopItem�� �����Ǵ� Ŭ�����̴�.
    @history 
        - 2007/1/12 : ��Ű�� �������� �߰��ϸ鼭 �������̽� Ŭ������ ����
*/
//==================================================================================================

#include "ShopDefine.h"
#include "ShopItemPrice.h"
#include <functional_custom.h>


class ArchiveOfStream;
class ShopManager;
class ShopCategory;
typedef struct sellItem tblSellItem;

//==================================================================================================

class ShopItem
{
public:
    static const int MAX_PRICE_INFO_NUM = 4; // client UI specification
    enum eTYPE 
    { 
        SINGLE, 
        PACKAGE, 
    };
    struct ShopItemData {
        ITEMGUID item_guid_;
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
        uint8_t partition_index_;
        uint16_t cache_index_;
        DWORD   item_filter_;
#endif

        uint8_t category_;  //eSHOP_CATEGORY
        uint8_t money_type_;//eSHOP_MONEY_TYPE
        DWORD cool_time_;   //< ���� ��Ÿ�� //< �� ����?
        DWORD limit_sell_count_;
        DWORD64 sell_expiration_date_; //< 20061209243500
        DWORD state_;     //< ������ ����(1:�Ǹ��� 2:�ǸŴ����)
    };
    //
    ShopItem();
    virtual ~ShopItem();

    virtual eTYPE GetType() const = 0;
    virtual void Release();
    virtual void Serialize(ShopManager& mgr, ArchiveOfStream& ar, eSHOP_SERIALIZE se);
    virtual BOOL IsSellEndDate();
    virtual void Display();
    virtual void DisplayElement();
    //virtual void Set(BYTE* pInfo IN, ShopManager& mgr IN)=0;
    // f100905.3L
    void AddPriceInfo(PRICEGUID price_guid, ShopItemPrice* shop_item_price);
    ShopItemPrice* FindShopItemPrice(const PRICEGUID guid);
    __inline SHOPITEMPRICE_LIST& GetShopItemPriceList() { return shop_item_price_list_; }
    __inline int GetShopItemPriceCount() const { return (int)shop_item_price_list_.size(); }
    __inline const char* GetName() const { return package_name_; }
    __inline const char* GetDesc() const { return package_desc_; }
    template<class Operator>
    void for_each(Operator& op);

    // Properties, these are shared data for derived class like a SingleItem, PackageItem
    ITEMGUID GetGuid() const { return shop_item_data_.item_guid_; }
    uint8_t GetCategory() const { return shop_item_data_.category_; }
    uint8_t GetMoneyType() const { return shop_item_data_.money_type_; }
    DWORD GetCoolTime() const { return shop_item_data_.cool_time_; }
    DWORD GetLimitSellCount() const { return shop_item_data_.limit_sell_count_; }
    const DWORD64& GetSellEndDate() const { return shop_item_data_.sell_expiration_date_; }
    DWORD GetState() const { return shop_item_data_.state_; }

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    uint8_t GetPartition_Index() const { return shop_item_data_.partition_index_; }
    uint16_t GetCache_Index() const { return shop_item_data_.cache_index_; }
    DWORD GetItemFilter() const { return shop_item_data_.item_filter_; }
    void SetItemFilter(DWORD filter) { shop_item_data_.item_filter_ = filter; }
    virtual void Set(const tblSellItemContainer& container, ShopManager& mgr) = 0;
#endif

protected:
    virtual void vSerialize(ShopManager& IN, ArchiveOfStream&, eSHOP_SERIALIZE) {}

    // �̱۾������̳� ��Ű�� ������ �������� ����
    //__REF_PROPERTY(ITEMGUID, Guid);
    //__REF_PROPERTY(BYTE, Category);       //< eSHOP_CATEGORY
    //__REF_PROPERTY(BYTE, MoneyType);      //< 0:ĳ��,1:���� //< eSHOP_MONEY_TYPE
    //__REF_PROPERTY(DWORD, CoolTime);      //< ���� ��Ÿ�� //< �� ����?
    //__REF_PROPERTY(DWORD, LimitSellCount);
    //__REF_PROPERTY(DWORD64, SellEndDate); //< 20061209243500
    //__REF_PROPERTY(DWORD, State);         //< ������ ����(1:�Ǹ��� 2:�ǸŴ����)
    ShopItemData* GetEditableShopItemData() { return &shop_item_data_; };
private:
    ShopItemData shop_item_data_;
    SHOPITEMPRICE_LIST shop_item_price_list_;
protected:
    char package_name_[MAX_PACKAGE_NAME_LENGTH+1];
    char package_desc_[MAX_PACKAGE_DESC_LENGTH+1];
    friend class ShopCategory;
    friend class ShopManager;
};

//==================================================================================================

template< class Operator >
void ShopItem::for_each(Operator& op)
{
    std::for_each(shop_item_price_list_.begin(), shop_item_price_list_.end(), op);
    //compose1(op, select2nd<SHOPITEMPRICE_LIST::value_type>()));
}

//==================================================================================================

#endif //SOLAR_SHOP_ITEM_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
