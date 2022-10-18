#pragma once
#ifndef GAMESERVER_ITEM_MANAGER_INTER_DECLARATION
    #error "can't direct include this file"
#endif

// (f100527.4L)
template<class KINDS_OF_INVENTORY_TOTAL_INFO>
void ItemManager::SerializeInventoryTotalInfo(KINDS_OF_INVENTORY_TOTAL_INFO& total_info,
                                              eSERIALIZE serialize_type)
{
    typedef SCItemSlotContainer::SlotNode SlotNode;
    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
    static const SLOTIDX seq_slots[1] = { SI_INVENTORY };
    POSTYPE* seq_pos_selector[1] = { &total_info.m_InvenCount };

    if (serialize_type == SERIALIZE_LOAD)
    {
        ;{
            total_info.m_InvenCount = 0;
            total_info.m_TmpInvenCount = 0;
        };
        //
        KINDS_OF_INVENTORY_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot;
        //
        POSTYPE** seq_pos_it = seq_pos_selector;
        for (const SLOTIDX* seq_it = seq_slots;
             seq_it != &seq_slots[_countof(seq_slots)];
             ++seq_it, ++seq_pos_it)
        {
            SCItemSlotContainer* item_container = GetItemSlotContainer(*seq_it);
            const SLOT_LINEAR_LIST& item_list = item_container->GetSlotList();
            SLOT_LINEAR_LIST::const_iterator it = item_list.begin(),
                                             end = item_list.end();
            for (POSTYPE pos_index = 0; it != end; ++it, ++pos_index)
            {
                const SlotNode& item_node = *it;
                SCItemSlot* item_slot = static_cast<SCItemSlot*>(item_node.slot);
                if (item_slot->GetItemCode() == 0) { // a replacement of an empty check
                    continue;
                }
                slot_it->m_Pos = pos_index;
                item_slot->CopyOut(slot_it->m_Stream);
                //
                ++(**seq_pos_it);
                ++slot_it;
            }
            // next
        };
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        //
        KINDS_OF_INVENTORY_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot; // sequential stream
        //
        POSTYPE** seq_pos_it = seq_pos_selector;
        for (const SLOTIDX* seq_it = seq_slots;
             seq_it != &seq_slots[_countof(seq_slots)];
             ++seq_it, ++seq_pos_it)
        {
            const SLOTIDX slot_index = *seq_it;
            SCItemSlotContainer* item_container = GetItemSlotContainer(slot_index);
            ASSERT(item_container->GetSlotNum() == 0);
            for (int loop = **seq_pos_it; loop; --loop, ++slot_it)
            {
                item_container->SerializeItemStream(slot_it->m_Pos, &slot_it->m_Stream,
                                                    serialize_type);
                // store event handling
                SCItemSlot* item_slot = \
                    static_cast<SCItemSlot*>(&item_container->GetSlot(slot_it->m_Pos));
                // bottleneck point
                updater_impl_.AddSerialCode(eSERIAL_LOAD, slot_index, slot_it->m_Pos, item_slot->GetWorldSerial());
                const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
                if (item_info->m_Code == 0) {
                    SUNLOG(eCRITICAL_LOG,
                           _T("[%s]|ItemCode=%d|SlotIndex=%d|Pos=%d|Msg=can't find item|"),
                           __FUNCTION__, item_info->m_Code, slot_index, slot_it->m_Pos);
                }
            }
        }
    }
}
