#ifndef _SOLAR_SLOT_SCQUICKSLOTCONTAINER_H
#define _SOLAR_SLOT_SCQUICKSLOTCONTAINER_H
#pragma once

//==================================================================================================
/// SCQuickSlotContainer
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - Äü½½·ÔÀ» Á¶ÀÛÇÏ´Â Äü ½½·Ô ÄÁÅ×ÀÌ³Ê Å¬·¡½º
    @note
    - 
    @history 
    - 09.09.06|waverix|code rearrangement
    - 10.03.10|waverix|refactoring for enhancement of the functionality of slot container
                      |enforce integrity check, to solve client crash fault problem
    - 11.05.28|waverix|change the quick link table using multi-map and does not control on server.
*/
//==================================================================================================

#include <SolarHashTable.h>
#include <ResultCode.h>

#include "./SCSlotStruct.h"
#include "./SCSlotContainer.h"

// (CHANGES) (100310) move to code file, internal macro utility
//// 1byte + 1byte + 2byte = 4byte
//#define QUICK_KEY(ORG_SIDX, ORG_SPOS, ORG_CODE) \
//    (DWORD)(((ORG_SIDX & 0xFF) << 24) | (( ORG_SPOS & 0xFF) << 16) | ( ORG_CODE & 0xFFFF))

class SCQuickSlot;
//==================================================================================================

class SCQuickSlotContainer : public SCSlotContainer
{
public:
    typedef ulong QuickKey_t;
    //
    SCQuickSlotContainer(void);
    virtual ~SCQuickSlotContainer(void);

    virtual eSlotType GetSlotType() const { return ST_QUICK; }
    virtual SCSlot* CreateSlot();

    void InsertSlot(POSTYPE at_pos, QUICKSTREAM& stream);

    virtual BOOL IsEmpty(POSTYPE pos);

    // virtual function overriding
    virtual void ClearAll();
    //< ½½·Ô¿¡ °´Ã¼ Ãß°¡
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);
    //< ½½·ÔÀÇ °´Ã¼ »èÁ¦
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);

    // f101112.2L, make a 'make quick slot key' macro to a function
    QuickKey_t MakeQuickKey(SLOTIDX original_slot_index, POSTYPE original_slot_pos,
                            SLOTCODE original_slot_code) const;
    void UpdateQuickSlot(SLOTIDX slot_index_prev, POSTYPE inven_pos_prev, SLOTCODE slot_code_prev,
                         SLOTIDX slot_index_next, POSTYPE inven_pos_next, SLOTCODE slot_code_next);
protected:
//#if !defined(_SERVER)
#if (SOLAR_HASH_TABLE_VERSION >= 0x0200)
    typedef STLX_MULTIMAP<ulong, SCQuickSlot*> QuickLinkTable;
    QuickLinkTable* const quick_link_table_;
#else //(SOLAR_HASH_TABLE_VERSION < 0x0200)
    util::SolarHashTable<SCQuickSlot*>* m_pItemHashTable; // key(Inventory  pos)
#endif
//#endif
};

// f101112.2L, make a 'make quick slot key' macro to a function
inline SCQuickSlotContainer::QuickKey_t
SCQuickSlotContainer::MakeQuickKey(SLOTIDX original_slot_index,
                                   POSTYPE original_slot_pos,
                                   SLOTCODE original_slot_code) const
{
    ulong quick_key = (ulong(original_slot_index) << 24) |
                      (ulong(original_slot_pos)   << 16) |
                      (ulong(original_slot_code));
    return quick_key;
}

#endif //_SOLAR_SLOT_SCQUICKSLOTCONTAINER_H
