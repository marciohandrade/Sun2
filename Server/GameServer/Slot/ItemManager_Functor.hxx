#pragma once
#ifndef GAMESERVER_ITEM_MANAGER_INTER_DECLARATION
    #error "can't direct include this file"
#endif

//==================================================================================================
// the specifications of functor interface for SlotManager
namespace ns_functor {
;

struct ISlotForeachFunctor
{
    virtual bool operator() (SCSlotContainer* slot_container, const SCSlot& slot) const = 0;
};

struct IItemSlotForeachFunctor
{
    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& slot) const = 0;
};

}; //end of namespace


//==================================================================================================
//==================================================================================================
// CHANGES: f100524.3L, waverix, move to a code file related to large size code blocks
// CHANGES: f110406.6L, waverix, change functors to interface implementation objects
//  known functors currently (f110406) = {
//      DeleteItem, DeleteComposeMaterialItem, EmptyItemCheck,
//      ComposeMaterialItemCheck, OverlappedItemCheck
//  };

namespace ns_functor {
;

class DeleteItem : public IItemSlotForeachFunctor
{
    CODETYPE    item_code_;
    mutable int max_number_;
    mutable DBWLDSERIAL item_world_serial_;

public:
    DeleteItem(CODETYPE code, int max_num);
    ~DeleteItem(){}
    //
    DBWLDSERIAL GetItemSerial() const { return item_world_serial_; }

    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const;
};

class DeleteComposeMaterialItem : public IItemSlotForeachFunctor
{
    SLOTCODE item_code_;
    DWORD item_type_index_;
    mutable int max_number_;
    mutable DBWLDSERIAL item_world_serial_;

public:
    DeleteComposeMaterialItem(SLOTCODE code, DWORD item_type_index, int max_num);
    ~DeleteComposeMaterialItem() {}
    DBWLDSERIAL GetItemSerial() const { return item_world_serial_; }
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    SLOTCODE GetItemCode() { return item_code_; }
    DWORD GetItemTypeIndex() {return item_type_index_; }

    // 기존 오퍼레이터를 상속받지 않고, 새 오퍼레이터를 쓴다
    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const {return false;}
    bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot, 
                             INVEN_DELETE_TOTAL_INFO* OUT delete_item_info) const;
#else
    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const;
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
};

class EmptyItemCheck : public IItemSlotForeachFunctor
{
    CODETYPE item_code_;
    mutable int max_number_;
public:
    EmptyItemCheck(CODETYPE code, int max_num);
    ~EmptyItemCheck(){}

    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const;
};

class ComposeMaterialItemCheck : public IItemSlotForeachFunctor
{
    SLOTCODE item_code_;
    mutable int max_number_;
    DWORD item_type_index_;
public:
    ComposeMaterialItemCheck(SLOTCODE code, DWORD item_type_index, int max_num);
    ~ComposeMaterialItemCheck(){}

    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const;
};

class OverlappedItemCheck : public IItemSlotForeachFunctor
{
    CODETYPE item_code_;
    mutable int max_number_;
public:
    OverlappedItemCheck(CODETYPE code, int num);
    ~OverlappedItemCheck(){}

    void SetNum(int num) { max_number_ = num; }
    virtual bool operator() (SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const;
};

//==================================================================================================
}; //end of namespace
//==================================================================================================

//==================================================================================================
