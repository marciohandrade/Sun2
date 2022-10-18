#include "stdafx.h"
#include "./SCQuickSlotContainer.h"
#include "./SCQuickSlot.h"

//==================================================================================================
// 1byte + 1byte + 2byte = 4byte
#define QUICK_KEY(ORG_SIDX, ORG_SPOS, ORG_CODE) \
    (DWORD)(((ORG_SIDX & 0xFF) << 24) | (( ORG_SPOS & 0xFF) << 16) | ( ORG_CODE & 0xFFFF))
//==================================================================================================

SCQuickSlotContainer::SCQuickSlotContainer(void)
#if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
    : quick_link_table_(TAllocNew(QuickLinkTable))
#else
    : m_pItemHashTable(new util::SolarHashTable<SCQuickSlot*>)
#endif
{
#if (SOLAR_HASH_TABLE_VERSION < 0x0200)
    m_pItemHashTable->Initialize(MAX_QUICK_SLOT_NUM * 2);
#endif
}

SCQuickSlotContainer::~SCQuickSlotContainer(void)
{
#if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
    TAllocDelete(QuickLinkTable, quick_link_table_);
#else
    delete m_pItemHashTable;
#endif
}

SCSlot*
SCQuickSlotContainer::CreateSlot()
{
    return new SCQuickSlot;
}

void
SCQuickSlotContainer::InsertSlot(POSTYPE at_pos, QUICKSTREAM& stream)
{
    // slot_in_container
    SCSlot* base_slot = GetValidSlotPtr(NULL, at_pos);
    if (base_slot == NULL) {
        return;
    }
    if (GetEmptySlotNum() == 0) {
        return;
    }
    // desired empty slot
    if (IsEmpty(at_pos) == false) {
        return;
    }
    //
    ++m_nSlotNum;
    SCQuickSlot* quick_slot = static_cast<SCQuickSlot*>(base_slot);
    quick_slot->Copy(stream);
    quick_slot->SetPos(at_pos);
    quick_slot->SetSlotIdx(GetSlotIdx());
    quick_slot->OnInsert();
    //
    this->OnInsert(*quick_slot);
#if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
    ulong quick_key = QUICK_KEY(quick_slot->GetOrgSlotIndex(),
                                quick_slot->GetOrgPos(),
                                quick_slot->GetOrgCode());
    quick_link_table_->insert(QuickLinkTable::value_type(quick_key, quick_slot));
#else
    m_pItemHashTable->Add(quick_slot, QUICK_KEY(quick_slot->GetOrgSlotIndex(),
                                                quick_slot->GetOrgPos(),
                                                quick_slot->GetOrgCode()));
#endif
}

RC::eSLOT_INSERT_RESULT
SCQuickSlotContainer::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    RC::eSLOT_INSERT_RESULT result = SCSlotContainer::InsertSlot(at_pos, slot);
    if (result == RC::RC_INSERT_SLOT_SUCCESS)
    {   //                                              current is safe state
        SCQuickSlot& quick_slot = static_cast<SCQuickSlot&>(GetSlot(at_pos));
    #if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
        ulong quick_key = QUICK_KEY(quick_slot.GetOrgSlotIndex(),
                                    quick_slot.GetOrgPos(),
                                    quick_slot.GetOrgCode());
        quick_link_table_->insert(QuickLinkTable::value_type(quick_key, &quick_slot));
    #else
        m_pItemHashTable->Add(&quick_slot, QUICK_KEY(quick_slot.GetOrgSlotIndex(),
                                                     quick_slot.GetOrgPos(),
                                                     quick_slot.GetOrgCode()));
    #endif
    }

    return result;
}

BOOL
SCQuickSlotContainer::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    if (ValidPos(at_pos) == false) {
        return false;
    }
    // empty or not empty ... valid slot
    // (CHANGES) (f100329.1L) (WAVERIX) fix up mismatched object handling problem that \
    // slot_clone_before_delete of the client is derived from the SCQuickSlot
    DWORD quick_slot_key = 0;
    { // to set quick_slot_key operation, it has short life-time with scope
        // the key of the quick_slot have the zero have no problem to do delete key
        const SCQuickSlot& quick_slot = static_cast<SCQuickSlot&>(GetSlot(at_pos));
        quick_slot_key = QUICK_KEY(quick_slot.GetOrgSlotIndex(),
                                   quick_slot.GetOrgPos(),
                                   quick_slot.GetOrgCode());
    };

    if (SCSlotContainer::DeleteSlot(at_pos, slot_clone_before_delete))
    {
    #if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
        quick_link_table_->erase(quick_slot_key);
    #else
        m_pItemHashTable->Remove(quick_slot_key);
    #endif
        return true;
    }
    return false;
}   

void
SCQuickSlotContainer::ClearAll()
{
    SCSlotContainer::ClearAll();
#if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
    quick_link_table_->clear();
#else
    m_pItemHashTable->RemoveAll();
#endif
}

BOOL
SCQuickSlotContainer::IsEmpty(POSTYPE at_pos)
{
    SCSlot* base_slot = GetValidSlotPtr(NULL, at_pos);
    if (base_slot == NULL) {
        // problem point... traditional custom is required the result is true
        return true;
    }
    if (static_cast<SCQuickSlot*>(base_slot)->GetOrgSlotIndex() == 0) { //...correct?
        return true;
    }
    return false;
}

void
SCQuickSlotContainer::UpdateQuickSlot(\
    SLOTIDX slot_index_prev, POSTYPE inven_pos_prev, SLOTCODE slot_code_prev,
    SLOTIDX slot_index_next, POSTYPE inven_pos_next, SLOTCODE slot_code_next)
{
    if (slot_index_prev == slot_index_next &&
        inven_pos_prev == inven_pos_next &&
        slot_code_prev == slot_code_next)
    {
        return;
    }
    //
#if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
    ulong quick_slot_key = QUICK_KEY(slot_index_prev, inven_pos_prev, slot_code_prev);
    ulong new_quick_slot_key = QUICK_KEY(slot_index_next, inven_pos_next, slot_code_next);
    QuickLinkTable::_Paircc ranges = quick_link_table_->equal_range(quick_slot_key);
    if (ranges.first == ranges.second) {
        return;
    };
    // NOTE: f110529.1L, changes to avoid 'end' position changed
    typedef STLX_VECTOR<SCQuickSlot*> TempIterateList;
    static TempIterateList temp_iterate_list;
    temp_iterate_list.resize(0);
    for (; ranges.first != ranges.second; ++ranges.first) {
        temp_iterate_list.push_back(ranges.first->second);
    };
    TempIterateList::const_iterator it = temp_iterate_list.begin(),
                                    end = temp_iterate_list.end();
    for (; it != end; ++it)
    {
        SCQuickSlot* quick_slot = (*it);
        quick_slot->SetOrgSlotIndex(slot_index_next);
        quick_slot->SetOrgPos(inven_pos_next);
        quick_slot->SetOrgCode(slot_code_next);
        // NOTE: f110528.2L, ranges is stable because of shadow list iterating
        quick_link_table_->insert(QuickLinkTable::value_type(new_quick_slot_key, quick_slot));
    };
    quick_link_table_->erase(quick_slot_key); //multi-delete
    //    
#else //(SOLAR_HASH_TABLE_VERSION < 0x0200)
    m_pItemHashTable->SetBucketFirst(QUICK_KEY(slot_index_prev, inven_pos_prev, slot_code_prev));
    while (SCQuickSlot* quick_slot = m_pItemHashTable->GetBucketNext())
    {
        m_pItemHashTable->RemoveCurBucketData();
        quick_slot->SetOrgSlotIndex(slot_index_next);
        quick_slot->SetOrgPos(inven_pos_next);
        quick_slot->SetOrgCode(slot_code_next);
        m_pItemHashTable->Add(quick_slot, QUICK_KEY(slot_index_next,
                                                    inven_pos_next,
                                                    slot_code_next));
    }
#endif
}










