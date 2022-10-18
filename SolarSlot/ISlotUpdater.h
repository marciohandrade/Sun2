#ifndef _SOLAR_SLOT_ISLOTUPDATER_H
#define _SOLAR_SLOT_ISLOTUPDATER_H
#pragma once

class SCItemSlot;

//==================================================================================================
//  <ISlotUpdater>
//      Code Hooking Handler
class ISlotUpdater
{
public:
    virtual BOOL SlotUpdate(SCItemSlot* IN pItemSlot, BOOL bAddinInfo) = 0;
};

//==================================================================================================
//
class ISlotUpdaterD
{
public:
    ISlotUpdaterD(ISlotUpdater* pISlotUpdater = NULL) : m_pISlotUpdater(pISlotUpdater) {}
    ISlotUpdaterD(const ISlotUpdaterD& rTHIS) : m_pISlotUpdater(rTHIS.m_pISlotUpdater) {}

    inline BOOL SlotUpdate(SCItemSlot* IN pItemSlot, BOOL bAddinInfo) {
        return m_pISlotUpdater
               ?    m_pISlotUpdater->SlotUpdate(pItemSlot, bAddinInfo)
               :    TRUE;
    }

private:
    ISlotUpdater* m_pISlotUpdater;
};

#endif //_SOLAR_SLOT_ISLOTUPDATER_H

