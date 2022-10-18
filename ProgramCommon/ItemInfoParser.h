#ifndef _ITEMINFOPARSER_H_
#define _ITEMINFOPARSER_H_
#pragma once
//==================================================================================================
/// ItemInfoParser class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >   
    @since
        2004. 11. 30
    @remarks
        - 
    @note
        - 
    @history
        - 2006.4.4 : m_wCoolTimeType, m_wSkillCode 필드가 추가 및 쿨타임 해쉬에 키를 m_byWasteType->m_wCoolTimeType변경
            m_wLimitSpr, m_wDivLimitSpr, m_wLimitVit, m_wDivLimitVit 추가
        - 2010.05.05|waverix|fit in a coding style and changes data structure 
            related to the BASE_ITEMINFO that the unidimentional array has 
            an O(1) time complexity about fixed size based key.
*/
//==================================================================================================

#include <ScriptCode.IParser.h>
#include <SCSlotStruct.h> // exported the BASE_ITEMINFO structure

#if defined(_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
//#include "Nstring.h" (CHANGES) remove unused header file
#include <string>
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

//==================================================================================================

struct ITEM_COOLTIME_INFO
{
    WORD  m_wCoolTimeType;
    DWORD m_dwCoolTime;
};

struct DUP_EQUIP_GROUP_INFO
{
    BYTE m_byDupEquipGroup;
    BYTE m_byDupEquipGroupNum;
};

#if defined(_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))  
struct _ITEM_NAME_INFO
{
    std::string m_stName;
    std::string description_string;
    DWORD m_dwItemCode;
};
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

typedef std::map<WORD, ITEM_COOLTIME_INFO>      ITEM_COOLTIME_MAP;
typedef ITEM_COOLTIME_MAP::iterator             ITEM_COOLTIME_ITER;

#if defined(_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
typedef std::list<_ITEM_NAME_INFO> ITEM_NAME_INFO;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

//==================================================================================================

class ItemInfoParser : public IParser
{
public:
    typedef STLX_VECTOR<SLOTCODE> RENDER_EFFECT_ITEM_LIST;
    typedef std::map<BYTE, DUP_EQUIP_GROUP_INFO*>   DUP_EQUIP_GROUP_INFO_MAP;
    // interfaces for util::Singleton<>
    static ItemInfoParser* Instance();
    static void DestroyInstance();

    bool IsLoaded() const;
    void Init(DWORD unused_arg, char* pack_file_name);
    // IParser interface implementation = { Init, Release, LoadScript }
    virtual void Init(DWORD unused_arg);
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    // (NOTE) the real type of an item_code is SLOTCODE(2Bytes),
    // the usage of the CODETYPE is needed checking boundaries
    BASE_ITEMINFO* GetItemInfo(CODETYPE item_code);
    const BASE_ITEMINFO* GetItemInfo(CODETYPE item_code) const;
    // CHANGES: f110408.3L, added interface to support external control like a event. _SERVER contents
    void ChangeToTimeExpirationItem(CODETYPE item_code);
    // CHANGES: f110802.3L, _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    bool CheckChecksum(CODETYPE item_code) const;
#if defined (_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
    void PushItemName(TCHAR* stName, DWORD ItemCode, TCHAR* description_string = NULL);
    DWORD GetItemNameforCode(char *stName);
    ITEM_NAME_INFO::iterator GetItemNamesBegin();
    ITEM_NAME_INFO::iterator GetItemNamesEnd();
    DWORD GetItemAllCount();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    ITEM_COOLTIME_MAP* GetItemCoolTimeMap();
    // compatible interfaces to support SolarHashtable interface
    void SetFirst() const ;
    BASE_ITEMINFO* GetNext() const;
private:
    // (CHANGES) (f100505.1L) add the item access facility which is supported an O(1) time complexity
    struct ItemTable {
        int number_of_total_items_;
        ulong* client_checksum_array_;
        // iterator
        mutable const uint16_t* seq_total_it_;
        const uint16_t* seq_total_indexes_;
        BASE_ITEMINFO* array_[USHRT_MAX];
    };
    //
    ItemInfoParser();
    ~ItemInfoParser();
    static ItemInfoParser* CreateInstance();

    BOOL _Load(BOOL reloaded);
    // (CHANGES) (f100505.1L)
    bool _AddItemInfo(CODETYPE item_code, BASE_ITEMINFO* item_info);
    // (CHANGES) (f100505.1L)
    bool _BuildReference(int added_number_of_items);
    //
    void CheckValidity(BASE_ITEMINFO* base_item_info);
    void Unload();
    //----------------------------------------------------------------------------------------------
    // data fields...
private:
    bool loaded_;
public:
    const RENDER_EFFECT_ITEM_LIST& RENDER_EFFECT_ITEMS;
private:
    //util::SolarHashTable<BASE_ITEMINFO *>* item_table_;
    ITEM_COOLTIME_MAP cool_time_table_;
#if defined (_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
    ITEM_NAME_INFO item_name_code_table_;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    // <Interface>
    RENDER_EFFECT_ITEM_LIST render_effect_item_list_;
    // (NOTE) it has not the usage case at this point in time (100505)
    DUP_EQUIP_GROUP_INFO_MAP dup_item_group_;
    // (CHANGES) (f100505.1L) 
    ItemTable item_table_;
    //
    char file_name_[MAX_PATH];
    static ItemInfoParser* item_info_parser_static_;
    __DISABLE_COPY(ItemInfoParser);
}; //end of class 'ItemInfoParser'

//==================================================================================================

// interfaces for util::Singleton<>
inline ItemInfoParser* ItemInfoParser::Instance()
{
    if (item_info_parser_static_) {
        return item_info_parser_static_;
    }
    return CreateInstance();
}

inline bool ItemInfoParser::IsLoaded() const
{
    return loaded_;
}

inline BASE_ITEMINFO* ItemInfoParser::GetItemInfo(CODETYPE item_code)
{
    // (CHANGES) (f100505.1L) changes a search algorithm
    if (item_code == 0 || item_code > USHRT_MAX) {
        return NULL;
    }
    --item_code;
    BASE_ITEMINFO* item_info = item_table_.array_[item_code];
    return item_info;
}

inline const BASE_ITEMINFO* ItemInfoParser::GetItemInfo(CODETYPE item_code) const
{
    // (CHANGES) (f100505.1L) changes a search algorithm + added const version
    if (item_code == 0 || item_code > USHRT_MAX) {
        return NULL;
    }
    --item_code;
    const BASE_ITEMINFO* item_info = item_table_.array_[item_code];
    return item_info;
}

inline void ItemInfoParser::SetFirst() const
{
    item_table_.seq_total_it_ = item_table_.seq_total_indexes_;
}

inline BASE_ITEMINFO* ItemInfoParser::GetNext() const
{
    const uint16_t* end = item_table_.seq_total_indexes_ + item_table_.number_of_total_items_;
    if (item_table_.seq_total_it_ >= end ||
        item_table_.seq_total_it_ < item_table_.seq_total_indexes_)
    {
        return NULL;
    }
    uint16_t index = *item_table_.seq_total_it_++; // change iterator
    assert(item_table_.array_[index]);
    return item_table_.array_[index];
}

inline ITEM_COOLTIME_MAP* ItemInfoParser::GetItemCoolTimeMap()
{
    return &cool_time_table_;
}

//==================================================================================================


//==================================================================================================
#endif // _ITEMINFOPARSER_H_