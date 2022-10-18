#pragma once
#ifndef DATABASEPROXY_NEW_CHARACTER_ITEM_PROVIDER_H
#define DATABASEPROXY_NEW_CHARACTER_ITEM_PROVIDER_H

//==================================================================================================
// @history
//  - 2010.10.06, waverix, created
//                         CHANGES: f101006.4L, renewals.
//                         this provide default items in the inventory for a creating character.
//
namespace nsSlot {
;

class NewCharacterItemProvider
{
public:
    static const NewCharacterItemProvider* Instance();
    bool IsValidState(eCHAR_TYPE char_type) const;
    // NOTE: returns (int) - number of aligned items
    int MakeStream(eCHAR_TYPE char_type,
                   ITEMSLOTEX_INSERVER* const slot_array) const; // must be number of inventory slots
private:
    NewCharacterItemProvider();
    ~NewCharacterItemProvider();
    //
    bool BuildTable();
    //
    static const NewCharacterItemProvider* CreateInstance();
    //
    size_t number_of_total_items_;
    ITEMSTREAMEX* total_items_;
    //
    struct Node {
        int number_of_items_;
        ITEMSTREAMEX* items_;
    } character_items_info_[eCHAR_TYPE_MAX];
    //
    static NewCharacterItemProvider* instance_static_;
};

//--------------------------------------------------------------------------------------------------

inline const NewCharacterItemProvider* NewCharacterItemProvider::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

inline bool NewCharacterItemProvider::IsValidState(eCHAR_TYPE char_type) const
{
    int index = static_cast<int>(char_type);
    if (index <= 0 || index >= eCHAR_TYPE_MAX) {
        return false;
    };
    return character_items_info_[index].number_of_items_ != 0;
}

};

#endif //DATABASEPROXY_NEW_CHARACTER_ITEM_PROVIDER_H