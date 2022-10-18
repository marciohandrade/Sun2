#ifndef __COMMON_DROP_RATIO_INFO_LIST_H
#define __COMMON_DROP_RATIO_INFO_LIST_H
#pragma once

//==================================================================================================
/// DropRatioInfoList class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >   
    @since
        2006. 4. 5 (식목일 왜 휴일이 아니냥$_$)
    @remarks
    -   서버 : 현재 서버에서는 DropCore클래스에서 스크립트를 로딩한다.
    @purpose
    -   아이템, 돈 드랍의 단순화
    -   밸런스를 컨트롤하기 쉬움
    @todo 
    -   DropObj를 상속받은 DropItem, DropMoney의 구현부를 로직에서 구현을 하고
    Drop될때 어떤 타입인지 모르게 캡슐화하라
    @history
    -   2006.4.7 : DropRatioInfo부분을 다른 클래스로 분리, 클라이언트에서 사용하기 위해
        DropObj를 전역 클래스로 분리, 외부에서 접근하기 위해
    -   091003|waverix|code arrangement
    -   100612|waverix|fit in a coding style and changes a data structure to more simple access
*/
//==================================================================================================

#include <ReliableRatioRandomizer.h>

//==================================================================================================

class DropRatioInfoList;

enum eDROP_OBJ_TYPES
{ 
    DROP_OBJ_NONE,
    DROP_OBJ_ITEM,
    DROP_OBJ_MONEY,
};

// _NA_0_20110318_ITEM_EXPIRATION_PERIOD
struct DROP_GROUP
{
    enum TYPE
    {
        NONE = 0,
        LINK_ITEM = (1 << 0),
        LINK_DROP = (1 << 1),
        LIMIT_TIME = (1 << 2),
        LIMIT_DATE = (1 << 3),
        LIMIT_DISTRIBUTE = (1 << 4), //_NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES
    };
};

//==================================================================================================
// (f100612.3L) a 'DropObj' change to a composed structure
class DropObj
{
public:
    struct DropItemInfo {
        BYTE drop_item_order;    // declared by _NA_1419_20100121_ACCUMULATE_POINT
        BYTE group_code;
        WORD limited_item_index; // (NOTE) reordering for pack
        DWORD item_code;
        DWORD probability_ratio;
        DWORD reserved; //< group_code == 0 이고 item_code가 스택아이템이면 개수
                        //< group_code == 0 이고 item_code가 장비아이템이면 ItemTypeIndex
    };
    struct DropMoneyInfo {
        DWORD min_money_seed;
        DWORD max_money_seed;
        DWORD min_multiple;
        DWORD max_multiple;
    };

    virtual ~DropObj() {}
    virtual eDROP_OBJ_TYPES GetType() const = 0;

    const eDROP_OBJ_TYPES kDropObjType; // for simple access
protected:
    explicit DropObj(eDROP_OBJ_TYPES drop_obj_type) : kDropObjType(drop_obj_type) {}
    DropObj() : kDropObjType(DROP_OBJ_NONE) {}
    union {
        DropItemInfo  drop_item_info_;
        DropMoneyInfo drop_money_info_;
    };
    __DISABLE_COPY(DropObj);
    friend class DropRatioInfoList;
};

//==================================================================================================

class DropItemObj : public DropObj
{
public:
    DropItemObj() : DropObj(DROP_OBJ_ITEM) {}
    virtual ~DropItemObj() {}
    // interface for DropObj
    virtual eDROP_OBJ_TYPES GetType() const { return DROP_OBJ_ITEM; }

    BYTE  GetGroupCode() const { return drop_item_info_.group_code; }
    DWORD GetItemCode() const { return drop_item_info_.item_code; }
    DWORD GetProbRatio() const { return drop_item_info_.probability_ratio; }
    // (DESC) GetReserved
    //  - Group == 0 이고 ItemCode가 스택아이템이면 개수
    //  - Group == 0 이고 ItemCode가 장비아이템이면 ItemTypeIndex
    DWORD GetReserved() const { return drop_item_info_.reserved; }
    WORD  GetLimitedItemIndex() const { return drop_item_info_.limited_item_index; }
    // declared by _NA_1419_20100121_ACCUMULATE_POINT
    BYTE  GetDropItemOrder() const { return drop_item_info_.drop_item_order; }

private:
    DropItemInfo* GetInfo() { return &drop_item_info_; } // for easy access
    friend class DropRatioInfoList;
};

//==================================================================================================

class DropMoneyObj : public DropObj
{
public:
    DropMoneyObj() : DropObj(DROP_OBJ_MONEY) {}
    virtual ~DropMoneyObj() {}
    // interface for DropObj
    virtual eDROP_OBJ_TYPES GetType() const { return DROP_OBJ_MONEY; }

    DWORD GetMinMoneySeed() const { return drop_money_info_.min_money_seed; }
    DWORD GetMaxMoneySeed() const { return drop_money_info_.max_money_seed; }
    DWORD GetMinMultiple() const { return drop_money_info_.min_multiple; }
    DWORD GetMaxMultiple() const { return drop_money_info_.max_multiple; }

private:
    DropMoneyInfo* GetInfo() { return &drop_money_info_; } // for easy access
    friend class DropRatioInfoList;
};

//==================================================================================================

class DropRatioInfoList : public IParser
{
public:
    static const size_t kMaxSharedDropObjectSize = max(sizeof(DropItemObj), sizeof(DropMoneyObj));
    static const int kNumberOfMoneyColumns = 5;
    static const int kNumberOfItemProbabilityColumns = 30;
    enum { MAX_RANDOM_STEP_NUM = 50 + 3 };
    typedef ReliableRatioRandomizer<MAX_RANDOM_STEP_NUM, DWORD, DropObj*> RANDOMIZER;
    typedef STLX_HASH_MAP<DWORD, RANDOMIZER*> RANDOMIZER_HASH;

    typedef STLX_LIST<DropObj*>               DELETE_DROP_LIST;
    typedef DELETE_DROP_LIST::iterator        DELETE_DROP_LIST_IT;
    typedef STLX_HASH_MAP<DWORD, DELETE_DROP_LIST> DROP_LIST_HASH;
    //
    // interfaces for util::Singleton<>
    static DropRatioInfoList* Instance();
    static void DestroyInstance();

    // (CHANGES) (WAVERIX) (091127) move from header to code file,
    // changes codes for debugging factility
    DropObj* PopRandomItemTypeIndex(DWORD drop_index);
    // (CHANGES) (f100613.1L) change an export interface from GetDropListHash to GetRandomizerTable.
    const RANDOMIZER_HASH& GetRandomizerTable() const { return random_drop_type_selector_; }
    // (CHANGES) make to const interface
    //__inline const DROP_LIST_HASH& GetDropListHash() const { return drop_list_hash_; }
    UINT PopDropItemCodeArray(DWORD drop_index,
                              SLOTCODE* OUT item_code_array, UINT item_code_array_count);

    // IParser interface implementation = { Init, Release, LoadScript }
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);

    void Reload();
    void Unload();

private:
    struct DropObjPoolChunk : public util::__LList<DropObjPoolChunk> {
        typedef int8_t node_type[kMaxSharedDropObjectSize];
        int number_of_allocations_;
        node_type array_[100];
        DropObjPoolChunk() {
            ZeroMemory(this, sizeof(*this));
            util::LList::Init(this);
        }
    };
    DropRatioInfoList(void);
    ~DropRatioInfoList(void);
    static DropRatioInfoList* CreateInstance();

    BOOL _Load(BOOL reloaded);
    // use only in loading
    DropObj* AllocDropObject(eDROP_OBJ_TYPES drop_obj_type);
    bool _BuildReference(int added_number_of_nodes);
    //
    // 아이템, 돈 결정
    RANDOMIZER_HASH  random_drop_type_selector_;
    //DROP_LIST_HASH   drop_list_hash_;
    RANDOMIZER* randomizer_array_;
    DropObjPoolChunk* drop_object_free_chunk_;
    util::__LList<DropObjPoolChunk> drop_object_alloc_root_;
    static DropRatioInfoList* drop_ratio_info_list_static_;
    char file_path_[MAX_PATH]; // for debug
};

//==================================================================================================

// interfaces for util::Singleton<>
inline DropRatioInfoList* DropRatioInfoList::Instance()
{
    if (drop_ratio_info_list_static_) {
        return drop_ratio_info_list_static_;
    }
    return CreateInstance();
}

#endif //__COMMON_DROP_RATIO_INFO_LIST_H
