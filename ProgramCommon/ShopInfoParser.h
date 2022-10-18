#ifndef __SHOP_INFO_PARSER_H__
#define __SHOP_INFO_PARSER_H__
#pragma once

//==================================================================================================
// @history
//  ~100726
//  100726|waverix|code-arrangement

#include <SolarHashTable.h>
#include <Singleton.h>
#include <ScriptCode.IParser.h>
//
#include <SCSlotHeader.h>

class ShopInfoParser;
class ItemType;
//==================================================================================================

class SHOPINFO
{
public:
    enum { _MAX_TAB_NUM = 5, _MAX_SELLITEM_NUM = 25, };
    //
    struct SHOP_ITEM
    {   
        BYTE            m_byTabIndex;
        BYTE            m_byItemIndex;
        DURATYPE        m_SellItemNum;
        SLOTCODE        m_SellItemCode;
        const ItemType* item_type_; //declared by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
        WORD           item_cost_; //���� �ܿ� �ٸ� ������ ��ȯ �� �� �ִ� ����
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        WORD           fame_area_; // �ʿ� �� ����
        WORD           repute_area_; // �ʿ� ���� ����
        BYTE           fame_rating_; // �ʿ� �� ���
        DWORD           repute_rating_; // �ʿ� ���� ���
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
        DWORD           timelimit_;
#endif //
    };

    SHOPINFO();
    ~SHOPINFO();

    void Init(DWORD shop_list_id, const char* shop_name);
    void Release();

    void DestroyItem(int tab_index);

    SHOP_ITEM* GetItemForIndex(BYTE tab_index, BYTE slot_pos);

    inline DWORD GetShopListID() { return shop_list_id_; }

#if !defined(_SERVER)
    SHOP_ITEM* GetItem(SLOTCODE code) { return shop_item_hash_->GetData(code);}// to support find an item group not a specific shop item
    inline void SetFirstItem(){ shop_item_hash_->SetFirst(); }
    inline SHOP_ITEM* GetNextItem(){ return shop_item_hash_->GetNext(); }

    bool HasSellItemByItemType( eITEMTYPE item_type );
    bool HasSellItemByItemCode( SLOTCODE item_code );

#endif
    //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    int GetLottoAccumRatio(BYTE tabIndex) { FASSERT(tabIndex < _MAX_TAB_NUM); return lottoAccumRatio[tabIndex]; }
#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    const NPC_SHOP_TYPE GetShopType() { return shop_type_; }
    const SLOTCODE GetCurrencyItemCode() { return currency_item_; }
#endif

private:
    typedef util::SolarHashTable<SHOP_ITEM*> SHOP_ITEM_HASH;
    void CreateItem(const SHOP_ITEM& shop_item_data);
    //void CreateItem(BYTE tab_index, BYTE slot_pos, SLOTCODE item_code, DURATYPE number_of_items);
    void DestroyItemAll();
    //
    DWORD shop_list_id_;
    // �ΰ��� ������� ������ �ִ´�. Ŭ���ڵ带 �״�� �α� ����
    SHOP_ITEM_HASH* shop_item_hash_;
    //
    SHOP_ITEM shop_item_table_[_MAX_TAB_NUM][_MAX_SELLITEM_NUM];
    int lottoAccumRatio[_MAX_TAB_NUM]; //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    NPC_SHOP_TYPE shop_type_;           //�߰� �Ǵ� �� ����(1:����)
    SLOTCODE currency_item_; //���� ������ ���Կ� ���̴� ȭ��������ڵ�(2:ȭ�������) �Ǵ� ȭ���ڵ�(4:ȭ������Ʈ)
#endif
    //
    char shop_name_debug_[MAX_SHOPNAME_LENGTH];
    static const SHOP_ITEM empty_shop_item_static_;
    //
    friend class ShopInfoParser;
    __DISABLE_COPY(SHOPINFO);
};

//
inline SHOPINFO::SHOP_ITEM* SHOPINFO::GetItemForIndex(BYTE tab_index, BYTE slot_pos)
{
    if (FlowControl::FCAssert(tab_index < _MAX_TAB_NUM && slot_pos < _MAX_SELLITEM_NUM)) {
        return &shop_item_table_[tab_index][slot_pos];
    }
    return const_cast<SHOP_ITEM*>(&empty_shop_item_static_);
}



//==================================================================================================

class ShopInfoParser : public util::Singleton<ShopInfoParser>, public IParser
{
public:
    ShopInfoParser();
    ~ShopInfoParser();

    void Init(DWORD unused);
    void Init(DWORD unused, char* pack_file_name);
    void Release();
    void Reload();
    // (NOTE) compatible interface
    SHOPINFO* GetShopList(DWORD shop_list_id);
    const SHOPINFO* GetShopList(DWORD shop_list_id) const;

    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);

#if !defined(_SERVER)
    bool HasSellItemByItemType( DWORD shop_list_id, eITEMTYPE item_type );
    bool HasSellItemByItemCode( DWORD shop_list_id, SLOTCODE item_code );
#endif

private:
    BOOL _Load(BOOL reloaded);

private:
    void Unload();
private:
    typedef STLX_HASH_MAP<DWORD, const SHOPINFO*> SHOP_INFO_TABLE;
    SHOP_INFO_TABLE shop_info_table_;
    char file_path_debug_[MAX_PATH];
    //
    __DISABLE_COPY(ShopInfoParser);
};

inline SHOPINFO* ShopInfoParser::GetShopList(DWORD shop_list_id)
{
    SHOP_INFO_TABLE::const_iterator it = shop_info_table_.find(shop_list_id);
    if (it == shop_info_table_.end()) {
        return NULL;
    }
    const SHOPINFO* shop_info = it->second;
    return const_cast<SHOPINFO*>(shop_info);
}

inline const SHOPINFO* ShopInfoParser::GetShopList(DWORD shop_list_id) const
{
    SHOP_INFO_TABLE::const_iterator it = shop_info_table_.find(shop_list_id);
    if (it == shop_info_table_.end()) {
        return NULL;
    }
    const SHOPINFO* shop_info = it->second;
    return shop_info;
}

#endif // __SHOP_INFO_PARSER_H__