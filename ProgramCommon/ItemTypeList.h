#pragma once
#ifndef PROGRAMCOMMON_ITEMTYPELIST_H
#define PROGRAMCOMMON_ITEMTYPELIST_H

//==================================================================================================
/// ItemTypeList class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >   
    @since
        2006. 4. 5 (�ĸ��� �� ������ �ƴϳ�$_$)
    @remarks
    - Ư���ε����� (��þƮ, �����, ��ũ)�� ���� ���� �� �ִ� Ŭ����
    - ���� : ���� ���������� DropCoreŬ�������� ��ũ��Ʈ�� �ε��Ѵ�.
    @purpose
    - ������ �����, ����Ʈ �������� Ư��(��þƮ, �����, ��ũ)�� �ٿ� �ٶ�, ���� ��ũ��Ʈ ����
    @history
    - 2006.4.7 : Ŭ���̾�Ʈ���� ����� �� �ְ� prgcommon�̵�
    - 2010.05.24|waverix|code arrangement
*/
//==================================================================================================

#include <SCSlotHeader.h>
#include <ScriptCode.IParser.h>

//==================================================================================================

class SCItemSlot;
class ItemType;

// _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
namespace nsSlot {

// (CHANGES) (f100523.3L) adapt the specification to support the item customizing content
// target : arg(4) = { 0=normal, 1=divine, 2=etheria, 3=etheria with divine, 4=shell }
struct ItemTypeChanger : KindOfItemType
{
    static KindOfItemType::value_type GetKindOfItemType(const SCItemSlot& item_slot);
    static bool IsEqualItemType(const SCItemSlot& item_slot, const ItemType* item_type);
    static bool ChangeItemByType(SCItemSlot* item_slot, KindOfItemType::value_type item_type);
    // (f100728.1L) add a making item interface related to the item type information
    static bool MakeItemSlotByType(SCItemSlot* item_slot,
                                   SLOTCODE item_code, DURATYPE duplicated_number,
                                   const ItemType* item_type);
};

}; //end of namespace
//
//
class ItemType
{
public:
    void  SetIndex(DWORD index) { index_ = index; }
    DWORD GetIndex() const { return index_; }
    void  SetEnchant(BYTE enchant) { enchant_ = enchant; }
    BYTE  GetEnchant() const { return enchant_; }
    void  SetItemType(KindOfItemType::value_type kind_of_type) {
        kind_of_type_ = kind_of_type;
    }
    KindOfItemType::value_type GetItemType() const { return kind_of_type_; }
    void  SetRank(BYTE rank) { rank_ = rank; }
    BYTE  GetRank() const { return rank_; }
private:
    DWORD index_;
    BYTE enchant_;
    // (NOTE) replace a value control by the content of a _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    KindOfItemType::value_type kind_of_type_;
    BYTE rank_;
};

//==================================================================================================

class ItemTypeList : public IParser
{
    typedef stdext::hash_map<DWORD, ItemType*> ITEM_TYPE_HASH;
public:
    // interfaces for util::Singleton<>
    static ItemTypeList* Instance();
    static void DestroyInstance();

    ItemTypeList(void);
    ~ItemTypeList(void);

    void Unload();
    void Reload();
    void Init(char* pack_file_name);
    ItemType* FindItemType(DWORD index);
    const ItemType* FindItemType(DWORD index) const;
public:
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
private:
    static ItemTypeList* CreateInstance();
    BOOL _Load(BOOL reloaded);

private:
    ItemType* item_type_chunk_block_;
    ITEM_TYPE_HASH  item_type_hash_;
    char m_pszFileName[MAX_PATH];
    static ItemTypeList* item_type_list_static_;
};

//==================================================================================================

// interfaces for util::Singleton<>
inline ItemTypeList* ItemTypeList::Instance()
{
    if (item_type_list_static_) {
        return item_type_list_static_;
    }
    return CreateInstance();
}

inline ItemType* ItemTypeList::FindItemType(DWORD index)
{
    ITEM_TYPE_HASH::const_iterator it = item_type_hash_.find(index);
    if (it != item_type_hash_.end()) {
        return it->second;
    }
    return NULL;
}

inline const ItemType* ItemTypeList::FindItemType(DWORD index) const
{
    ITEM_TYPE_HASH::const_iterator it = item_type_hash_.find(index);
    if (it != item_type_hash_.end()) {
        return it->second;
    }
    return NULL;
}

#endif //PROGRAMCOMMON_ITEMTYPELIST_H
