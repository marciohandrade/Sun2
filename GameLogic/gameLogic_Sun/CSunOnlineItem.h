#ifndef __GAMELOGIC_SUNONLINE_ITEM_H
#define __GAMELOGIC_SUNONLINE_ITEM_H

#include "./CSunOnline.h"

#ifdef __NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

class CSunOnlineItem
{
public:
    CSunOnlineItem(CCSunOnline* root);

    BOOLEAN DecodeItem(CMarkupArchive& xml, const SCItemSlot& item_slot, POSTYPE item_pos);
    BOOLEAN EncodeItem(CMarkupArchive& xml, SCItemSlot& item_slot, POSTYPE* item_pos);
private:
    static void _Serialize_EtherWeapon(BOOLEAN is_decode, CMarkupArchive& xml,
                                       const nsSlot::ItemRuleInfo& item_rule_info,
                                       SCItemSlot& item_slot);
    static void _Serialize_PetItem(BOOLEAN is_decode, CMarkupArchive& xml,
                                   const nsSlot::ItemRuleInfo& item_rule_info,
                                   SCItemSlot& item_slot);
    static void _Serialize_FateItem(BOOLEAN is_decode, CMarkupArchive& xml,
                                    const nsSlot::ItemRuleInfo& item_rule_info,
                                    SCItemSlot& item_slot,
                                    char time_string_inout[32]);
    static void _Serialize_LimitedItem(BOOLEAN is_decode, CMarkupArchive& xml,
                                       const nsSlot::ItemRuleInfo& item_rule_info,
                                       SCItemSlot& item_slot);


    CCSunOnline* root_;
};

#endif //__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

#endif //__GAMELOGIC_SUNONLINE_ITEM_H
